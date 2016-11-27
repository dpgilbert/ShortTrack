// C++
#include <iostream>
#include <vector>
#include <set>
#include <cmath>

// ROOT
#include "TDirectory.h"
#include "TTreeCache.h"
#include "Math/VectorUtil.h"
#include "TVector2.h"
#include "TBenchmark.h"

// Tools
#include "../CORE/Tools/utils.h"
#include "../CORE/Tools/goodrun.h"
#include "../CORE/Tools/dorky/dorky.h"
#include "../CORE/Tools/badEventFilter.h"

// header
#include "ZllMTLooper.h"

//MT2
#include "../MT2CORE/Plotting/PlotUtilities.h"


using namespace std;
using namespace mt2;
using namespace duplicate_removal;

class mt2tree;
class SR;

// turn on to apply Nvtx reweighting to MC
bool doNvtxReweight = false;
// turn on to apply json file to data
bool applyJSON = true;
// turn on to apply btag sf to central value
bool applyBtagSF = true;
// turn on to apply lepton sf to central value - take from babies
bool applyLeptonSFfromBabies = true;

//_______________________________________
ZllMTLooper::ZllMTLooper(){
}
//_______________________________________
ZllMTLooper::~ZllMTLooper(){

};

//_______________________________________
void ZllMTLooper::SetSignalRegions(){

  // CRMT base: just 2 leptons, Z
  CRMTBase.SetName("crmtbase");

  // CRMT with 2 jets, bveto
  CRMTNj2Nb0.SetName("crmtnj2nb0");
  CRMTNj2Nb0.SetVar("nJet30", 2, -1);
  CRMTNj2Nb0.SetVar("nBJet20", 0, 1);

  // CRMT with 2 jets, bveto, ht > 250
  CRMTHT250.SetName("crmtht250");
  CRMTHT250.SetVar("nJet30", 2, -1);
  CRMTHT250.SetVar("nBJet20", 0, 1);
  CRMTHT250.SetVar("ht", 250, -1);

  // CRMT with 2 jets, bveto, ht > 450
  CRMTHT450.SetName("crmtht450");
  CRMTHT450.SetVar("nJet30", 2, -1);
  CRMTHT450.SetVar("nBJet20", 0, 1);
  CRMTHT450.SetVar("ht", 450, -1);

}


//_______________________________________
void ZllMTLooper::loop(TChain* chain, std::string sample, std::string output_dir){

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  gROOT->cd();
  TString output_name = Form("%s/%s.root",output_dir.c_str(),sample.c_str());
  cout << "[ZllMTLooper::loop] creating output file: " << output_name << endl;

  outfile_ = new TFile(output_name.Data(),"RECREATE") ; 

  // full 2016 dataset json, 36.26/fb:
  const char* json_file = "../babymaker/jsons/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON_snt.txt";
  if (applyJSON) {
    cout << "Loading json file: " << json_file << endl;
    set_goodrun_file(json_file);
  }

  h_nvtx_weights_ = 0;
  if (doNvtxReweight) {
    //    TFile* f_weights = new TFile("../babymaker/data/hists_reweight_zjets_Run2015B.root");
    //    TH1D* h_nvtx_weights_temp = (TH1D*) f_weights->Get("h_nVert_ratio");
    TFile* f_weights = new TFile("hists_reweight_zjets_2016B.root");
    TH1D* h_nvtx_weights_temp = (TH1D*) f_weights->Get("h_nVert_ratio");
    outfile_->cd();
    h_nvtx_weights_ = (TH1D*) h_nvtx_weights_temp->Clone("h_nvtx_weights");
    f_weights->Close();
  }
  
  eventFilter metFilterTxt;
  TString stringsample = sample;
  if (stringsample.Contains("data")) {
    cout<<"Loading bad event files ..."<<endl;
    metFilterTxt.loadBadEventList("/nfs-6/userdata/mt2utils/eventlist_DoubleEG_csc2015.txt");
    metFilterTxt.loadBadEventList("/nfs-6/userdata/mt2utils/eventlist_DoubleMuon_csc2015.txt");
    metFilterTxt.loadBadEventList("/nfs-6/userdata/mt2utils/eventlist_HTMHT_csc2015.txt");
    metFilterTxt.loadBadEventList("/nfs-6/userdata/mt2utils/eventlist_JetHT_csc2015.txt");
    metFilterTxt.loadBadEventList("/nfs-6/userdata/mt2utils/eventlist_MET_csc2015.txt");
    metFilterTxt.loadBadEventList("/nfs-6/userdata/mt2utils/eventlist_SingleElectron_csc2015.txt");
    metFilterTxt.loadBadEventList("/nfs-6/userdata/mt2utils/eventlist_SingleMuon_csc2015.txt");
    metFilterTxt.loadBadEventList("/nfs-6/userdata/mt2utils/eventlist_SinglePhoton_csc2015.txt");
    cout<<" ... finished!"<<endl;
  }

  cout << "[ZllMTLooper::loop] setting up histos" << endl;

  SetSignalRegions();

  // File Loop
  int nDuplicates = 0;
  int nEvents = chain->GetEntries();
  unsigned int nEventsChain = nEvents;
  cout << "[ZllMTLooper::loop] running on " << nEventsChain << " events" << endl;
  unsigned int nEventsTotal = 0;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;
  while ( (currentFile = (TFile*)fileIter.Next()) ) {
    cout << "[ZllMTLooper::loop] running on file: " << currentFile->GetTitle() << endl;

    // Get File Content
    TFile f( currentFile->GetTitle() );
    TTree *tree = (TTree*)f.Get("mt2");
    TTreeCache::SetLearnEntries(10);
    tree->SetCacheSize(128*1024*1024);
    //mt2tree t(tree);
    
    // Use this to speed things up when not looking at genParticles
    //tree->SetBranchStatus("genPart_*", 0); 

    t.Init(tree);

    // Event Loop
    unsigned int nEventsTree = tree->GetEntriesFast();
    for( unsigned int event = 0; event < nEventsTree; ++event) {
      
      t.GetEntry(event);

      //---------------------
      // bookkeeping and progress report
      //---------------------
      ++nEventsTotal;
      if (nEventsTotal%10000==0) {
	ULong64_t i_permille = (int)floor(1000 * nEventsTotal / float(nEventsChain));
	if (isatty(1)) {
	  printf("\015\033[32m ---> \033[1m\033[31m%4.1f%%"
		 "\033[0m\033[32m <---\033[0m\015", i_permille/10.);
	  fflush(stdout);
	}
      }

      //---------------------
      // skip duplicates -- needed when running on mutiple streams in data
      //---------------------
      if( t.isData ) {
	DorkyEventIdentifier id(t.run, t.evt, t.lumi);
	if (is_duplicate(id) ){
	  ++nDuplicates;
	  continue;
	}
      }

      //---------------------
      // basic event selection and cleaning
      //---------------------

      if( applyJSON && t.isData && !goodrun(t.run, t.lumi) ) continue;

      // MET filters - only apply to data for now, buggy in miniaodV1 MC
      if (t.isData) {
	// MET filters (data and MC)
	if (!t.Flag_goodVertices) continue;
	if (!t.Flag_globalTightHalo2016Filter) continue; 
	if (!t.Flag_eeBadScFilter) continue; 
	if (!t.Flag_HBHENoiseFilter) continue;
	if (!t.Flag_HBHENoiseIsoFilter) continue;
	if (!t.Flag_EcalDeadCellTriggerPrimitiveFilter) continue;
	if (!t.Flag_badMuonFilter) continue;
	if (!t.Flag_badChargedHadronFilter) continue;
      }
      
      // // txt MET filters (data only)
      // if (t.isData && metFilterTxt.eventFails(t.run, t.lumi, t.evt)) {
      // 	//cout<<"Found bad event in data: "<<t.run<<", "<<t.lumi<<", "<<t.evt<<endl;
      // 	continue;
      // }

      //bool passJetID = true;
      if (t.nJet30FailId > 0) continue;

      // basic dilepton selection: vertex, 2 leptons, dilepton trigger
      if (t.nVert == 0) continue;
      if (t.nlep < 2) continue;
      if (t.isData && (!t.HLT_DoubleEl && !t.HLT_DoubleMu && !t.HLT_DoubleMu_NonIso && !t.HLT_DoubleEl33)) continue;
      //if (!t.HLT_DoubleEl && !t.HLT_DoubleMu) continue;
      // also allow single muon triggers
      //if (!t.HLT_DoubleEl && !t.HLT_DoubleMu && !t.HLT_SingleMu) continue;

      // remove low pt QCD samples 
      if (t.evt_id >= 100 && t.evt_id < 108) continue;

      //---------------------
      // set weights and start making plots
      //---------------------
      outfile_->cd();
      //      const float lumi = 12.9;
      const float lumi = 36.26;
      evtweight_ = 1.;

      // apply relevant weights to MC
      if (!t.isData) {
	evtweight_ = t.evt_scale1fb * lumi;
	// get pu weight from hist, restrict range to nvtx 4-31
	if (doNvtxReweight) {
	  int nvtx_input = t.nVert;
	  if (t.nVert > 34) nvtx_input = 34;
	  if (t.nVert < 1) nvtx_input = 1;
	  float puWeight = h_nvtx_weights_->GetBinContent(h_nvtx_weights_->FindBin(nvtx_input));
	  evtweight_ *= puWeight;
	}
	if (applyBtagSF) {
	  // remove events with 0 btag weight for now..
	  if (fabs(t.weight_btagsf) < 0.001) continue;
	  evtweight_ *= t.weight_btagsf;
	}
	if (applyLeptonSFfromBabies) {
	  evtweight_ *= t.weight_lepsf;
	}
      } // !isData

      // --- additional selection for dilep events
      if (t.nlep != 2) continue; // require exactly 2 loose leptons
      if (t.lep_pt[0] < 25. || t.lep_pt[1] < 20.) continue; // pt 25,20
      if (t.lep_charge[0] * t.lep_charge[1] != -1) continue; // OS
      if (fabs(t.zll_mass - 90.) > 20.) continue; // z mass

      // require 2 SF leptons, ID+iso
      // bool pass_electrons = bool(t.HLT_DoubleEl && abs(t.lep_pdgId[0]) == 11 && abs(t.lep_pdgId[1]) == 11 && t.lep_tightId[0] > 1 && t.lep_tightId[1] > 1 && t.lep_miniRelIso[0] < 0.1 && t.lep_miniRelIso[1] < 0.1);
    
      // bool pass_muons = bool((t.HLT_DoubleMu || t.HLT_SingleMu) && abs(t.lep_pdgId[0]) == 13 && abs(t.lep_pdgId[1]) == 13 && t.lep_tightId[0] > 0 && t.lep_tightId[1] > 0 && t.lep_miniRelIso[0] < 0.2 && t.lep_miniRelIso[1] < 0.2);

      bool pass_electrons = bool(abs(t.lep_pdgId[0]) == 11 && abs(t.lep_pdgId[1]) == 11 && t.lep_tightId[0] > 1 && t.lep_tightId[1] > 1 && t.lep_miniRelIso[0] < 0.1 && t.lep_miniRelIso[1] < 0.1);
    
      bool pass_muons = bool(abs(t.lep_pdgId[0]) == 13 && abs(t.lep_pdgId[1]) == 13 && t.lep_tightId[0] > 0 && t.lep_tightId[1] > 0 && t.lep_miniRelIso[0] < 0.2 && t.lep_miniRelIso[1] < 0.2);

      if (!pass_electrons && !pass_muons) continue;

      fillHistosMT(CRMTBase.crslHistMap,"crmtbase");
      if (pass_electrons) fillHistosMT(CRMTBase.crslelHistMap,"crmtelbase");
      else if (pass_muons) fillHistosMT(CRMTBase.crslmuHistMap,"crmtmubase");

      std::map<std::string, float> valuesNj2Nb0;
      valuesNj2Nb0["nJet30"] = t.nJet30;
      valuesNj2Nb0["nBJet20"] = t.nBJet20;
      if (CRMTNj2Nb0.PassesSelection(valuesNj2Nb0)) {
	fillHistosMT(CRMTNj2Nb0.crslHistMap,"crmtnj2nb0");
	if (pass_electrons) fillHistosMT(CRMTNj2Nb0.crslelHistMap,"crmtelnj2nb0");
	else if (pass_muons) fillHistosMT(CRMTNj2Nb0.crslmuHistMap,"crmtmunj2nb0");
      }

      std::map<std::string, float> valuesHT250;
      valuesHT250["ht"] = t.ht;
      valuesHT250["nJet30"] = t.nJet30;
      valuesHT250["nBJet20"] = t.nBJet20;
      if (CRMTHT250.PassesSelection(valuesHT250)) {
	fillHistosMT(CRMTHT250.crslHistMap,"crmtht250");
	if (pass_electrons) fillHistosMT(CRMTHT250.crslelHistMap,"crmtelht250");
	else if (pass_muons) fillHistosMT(CRMTHT250.crslmuHistMap,"crmtmuht250");
      }

      std::map<std::string, float> valuesHT450;
      valuesHT450["ht"] = t.ht;
      valuesHT450["nJet30"] = t.nJet30;
      valuesHT450["nBJet20"] = t.nBJet20;
      if (CRMTHT450.PassesSelection(valuesHT450)) {
	fillHistosMT(CRMTHT450.crslHistMap,"crmtht450");
	if (pass_electrons) fillHistosMT(CRMTHT450.crslelHistMap,"crmtelht450");
	else if (pass_muons) fillHistosMT(CRMTHT450.crslmuHistMap,"crmtmuht450");
      }

    }//end loop on events in a file
  
    delete tree;
    f.Close();
  }//end loop on files
  
  cout << "[ZllMTLooper::loop] processed " << nEventsTotal << " events" << endl;
  if ( nEventsChain != nEventsTotal ) {
    std::cout << "ERROR: number of events from files is not equal to total number of events" << std::endl;
  }

  cout << nDuplicates << " duplicate events were skipped." << endl;

  //---------------------
  // Save Plots
  //---------------------

  outfile_->cd();
  savePlotsDir(h_1d_global,outfile_,"");
  savePlotsDir(CRMTBase.crslHistMap,outfile_,"crmtbase");
  savePlotsDir(CRMTBase.crslelHistMap,outfile_,"crmtelbase");
  savePlotsDir(CRMTBase.crslmuHistMap,outfile_,"crmtmubase");
  savePlotsDir(CRMTNj2Nb0.crslHistMap,outfile_,"crmtnj2nb0");
  savePlotsDir(CRMTNj2Nb0.crslelHistMap,outfile_,"crmtelnj2nb0");
  savePlotsDir(CRMTNj2Nb0.crslmuHistMap,outfile_,"crmtmunj2nb0");
  savePlotsDir(CRMTHT250.crslHistMap,outfile_,"crmtht250");
  savePlotsDir(CRMTHT250.crslelHistMap,outfile_,"crmtelht250");
  savePlotsDir(CRMTHT250.crslmuHistMap,outfile_,"crmtmuht250");
  savePlotsDir(CRMTHT450.crslHistMap,outfile_,"crmtht450");
  savePlotsDir(CRMTHT450.crslelHistMap,outfile_,"crmtelht450");
  savePlotsDir(CRMTHT450.crslmuHistMap,outfile_,"crmtmuht450");

  //---------------------
  // Write and Close file
  //---------------------
  outfile_->Write();
  outfile_->Close();
  delete outfile_;

  bmark->Stop("benchmark");
  cout << endl;
  cout << nEventsTotal << " Events Processed" << endl;
  cout << "------------------------------" << endl;
  cout << "CPU  Time:	" << Form( "%.01f s", bmark->GetCpuTime("benchmark")  ) << endl;
  cout << "Real Time:	" << Form( "%.01f s", bmark->GetRealTime("benchmark") ) << endl;
  cout << endl;
  delete bmark;

  return;
}

//_______________________________________
void ZllMTLooper::fillHistosMT(std::map<std::string, TH1*>& h_1d, const std::string& dirname, const std::string& s) {

  if (dirname.size()) {
    TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
    if (dir == 0) {
      dir = outfile_->mkdir(dirname.c_str());
    } 
    dir->cd();
  } else {
    outfile_->cd();
  }

  // kill_lep = lepton that was turned into MET
  int kill_lep = t.evt%2;
  int keep_lep = (kill_lep+1)%2;
	
  plot1D("h_Events"+s,  1, 1, h_1d, ";Events, Unweighted", 1, 0, 2);
  plot1D("h_Events_w"+s,  1,   evtweight_, h_1d, ";Events, Weighted", 1, 0, 2);
  plot1D("h_nVert"+s,          t.nVert,   evtweight_, h_1d, ";N(vertices)",50,0,50);
  plot1D("h_nJet30"+s,     t.nJet30,   evtweight_, h_1d, ";N(jets)", 15, 0, 15);
  plot1D("h_nBJet20"+s,    t.nBJet20,   evtweight_, h_1d, ";N(bjets)", 6, 0, 6);
  plot1D("h_kill_lep_pt"+s,    t.lep_pt[kill_lep],   evtweight_, h_1d, ";p_{T}(removed lepton) [GeV]", 1000, 0., 1000.);
  plot1D("h_keep_lep_pt"+s,    t.lep_pt[keep_lep],   evtweight_, h_1d, ";p_{T}(kept lepton) [GeV]", 1000, 0., 1000.);
  plot1D("h_zll_mass"+s,       t.zll_mass,   evtweight_, h_1d, ";dilepton mass [GeV]",100,0,200);
  plot1D("h_zll_pt"+s,         t.zll_pt,   evtweight_, h_1d, ";dilepton p_{T} [GeV]",500,0,1000);
  plot1D("h_zllmt_met_pt"+s,   t.zllmt_met_pt,   evtweight_, h_1d, ";Transformed E_{T}^{miss} [GeV]",250,0,500);
  plot1D("h_zllmt_mt2"+s,      t.zllmt_mt2,   evtweight_, h_1d, ";Transformed M_{T2} [GeV]",250,0,500);
  plot1D("h_zllmt_mt"+s,       t.zllmt_mt,   evtweight_, h_1d, ";Transformed M_{T} [GeV]",250,0,500);
  if (t.zllmt_mt2 > 50.) plot1D("h_zllmt_mt_mt2gt50"+s,          t.zllmt_mt,   evtweight_, h_1d, ";Transformed M_{T} [GeV]",250,0,500);
  if (t.zllmt_mt2 > 100.) plot1D("h_zllmt_mt_mt2gt100"+s,        t.zllmt_mt,   evtweight_, h_1d, ";Transformed M_{T} [GeV]",250,0,500);
  if (t.zllmt_mt2 > 150.) plot1D("h_zllmt_mt_mt2gt150"+s,        t.zllmt_mt,   evtweight_, h_1d, ";Transformed M_{T} [GeV]",250,0,500);
  if (t.zllmt_mt2 > 200.) plot1D("h_zllmt_mt_mt2gt200"+s,        t.zllmt_mt,   evtweight_, h_1d, ";Transformed M_{T} [GeV]",250,0,500);

  outfile_->cd();
  return;
}

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
#include "EmuLooper.h"

//MT2
#include "../MT2CORE/Plotting/PlotUtilities.h"


using namespace std;
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
// turn on to enable plots with systematic variations applied. will only do variations for applied weights
bool doSystVariationPlots = true;

//_______________________________________
EmuLooper::EmuLooper(){
}
//_______________________________________
EmuLooper::~EmuLooper(){

};

//_______________________________________
void EmuLooper::SetSignalRegions(){

  CREmuBase.SetName("cremubase");
  CREmuNj4.SetName("cremunj4");
  CREmuNj5.SetName("cremunj5");

}


//_______________________________________
void EmuLooper::loop(TChain* chain, std::string sample, std::string output_dir){

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  gROOT->cd();
  TString output_name = Form("%s/%s.root",output_dir.c_str(),sample.c_str());
  cout << "[EmuLooper::loop] creating output file: " << output_name << endl;

  outfile_ = new TFile(output_name.Data(),"RECREATE") ; 

  const char* json_file = "../babymaker/jsons/Cert_271036-276811_13TeV_PromptReco_Collisions16_JSON_snt.txt";
  if (applyJSON) {
    cout << "Loading json file: " << json_file << endl;
    set_goodrun_file(json_file);
  }

  h_nvtx_weights_ = 0;
  if (doNvtxReweight) {
    //    TFile* f_weights = new TFile("../babymaker/data/hists_reweight_zjets_Run2015B.root");
    //    TH1D* h_nvtx_weights_temp = (TH1D*) f_weights->Get("h_nVert_ratio");
    TFile* f_weights = new TFile("nvtx_ratio.root");
    TH1D* h_nvtx_weights_temp = (TH1D*) f_weights->Get("h_vtx_ratio");
    outfile_->cd();
    h_nvtx_weights_ = (TH1D*) h_nvtx_weights_temp->Clone("h_nvtx_weights");
    f_weights->Close();
  }
  
  // eventFilter metFilterTxt;
  // TString stringsample = sample;
  // if (stringsample.Contains("data")) {
  //   cout<<"Loading bad event files ..."<<endl;
  //   metFilterTxt.loadBadEventList("/nfs-6/userdata/mt2utils/eventlist_DoubleEG_csc2015.txt");
  //   metFilterTxt.loadBadEventList("/nfs-6/userdata/mt2utils/eventlist_DoubleMuon_csc2015.txt");
  //   metFilterTxt.loadBadEventList("/nfs-6/userdata/mt2utils/eventlist_HTMHT_csc2015.txt");
  //   metFilterTxt.loadBadEventList("/nfs-6/userdata/mt2utils/eventlist_JetHT_csc2015.txt");
  //   metFilterTxt.loadBadEventList("/nfs-6/userdata/mt2utils/eventlist_MET_csc2015.txt");
  //   metFilterTxt.loadBadEventList("/nfs-6/userdata/mt2utils/eventlist_SingleElectron_csc2015.txt");
  //   metFilterTxt.loadBadEventList("/nfs-6/userdata/mt2utils/eventlist_SingleMuon_csc2015.txt");
  //   metFilterTxt.loadBadEventList("/nfs-6/userdata/mt2utils/eventlist_SinglePhoton_csc2015.txt");
  //   cout<<" ... finished!"<<endl;
  // }

  cout << "[EmuLooper::loop] setting up histos" << endl;

  SetSignalRegions();

  // File Loop
  int nDuplicates = 0;
  int nEvents = chain->GetEntries();
  unsigned int nEventsChain = nEvents;
  cout << "[EmuLooper::loop] running on " << nEventsChain << " events" << endl;
  unsigned int nEventsTotal = 0;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;
  while ( (currentFile = (TFile*)fileIter.Next()) ) {
    cout << "[EmuLooper::loop] running on file: " << currentFile->GetTitle() << endl;

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

      // Jet ID Veto
      if (t.nJet30FailId > 0) continue;

      // remove low pt QCD samples 
      if (t.evt_id >= 100 && t.evt_id < 108) continue;

      //---------------------
      // set weights and start making plots
      //---------------------
      outfile_->cd();
      const float lumi = 12.9;
      evtweight_ = 1.;

      // apply relevant weights to MC
      if (!t.isData) {
	evtweight_ = t.evt_scale1fb * lumi;
	// get pu weight from hist, restrict range to nvtx 4-31
	if (doNvtxReweight) {
	  int nvtx_input = t.nVert;
	  if (t.nVert > 31) nvtx_input = 31;
	  if (t.nVert < 4) nvtx_input = 4;
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

      // basic dilepton emu selection: vertex, 2 leptons, dilepton trigger, MET 50, at least one btag
      if (t.nVert == 0) continue;
      if (t.nlep != 2) continue; // require exactly 2 loose leptons
      if (t.isData && !(t.HLT_MuX_Ele12 || t.HLT_Mu8_EleX) ) continue;
      if (t.met_pt < 50.) continue;
      if (t.nJet30 < 3) continue;
      if (t.nBJet20csv < 1) continue;
      if (t.lep_pt[0] < 25. || t.lep_pt[1] < 20.) continue; // pt 25,20
      if (t.lep_charge[0] * t.lep_charge[1] != -1) continue; // OS
      if (abs(t.lep_pdgId[0]) == abs(t.lep_pdgId[1])) continue; // OF
      if (abs(t.lep_pdgId[0]) == 11 && t.lep_tightId[0] < 1) continue; // electron Loose ID
      if (abs(t.lep_pdgId[1]) == 11 && t.lep_tightId[1] < 1) continue; // electron Loose ID

      fillHistosEMU(CREmuBase.crdyHistMap,"cremubase");
      if (t.nJet30 >= 4) fillHistosEMU(CREmuNj4.crdyHistMap,"cremunj4");
      if (t.nJet30 >= 5) fillHistosEMU(CREmuNj5.crdyHistMap,"cremunj5");

    }//end loop on events in a file
  
    delete tree;
    f.Close();
  }//end loop on files
  
  cout << "[EmuLooper::loop] processed " << nEventsTotal << " events" << endl;
  if ( nEventsChain != nEventsTotal ) {
    std::cout << "ERROR: number of events from files is not equal to total number of events" << std::endl;
  }

  cout << nDuplicates << " duplicate events were skipped." << endl;

  //---------------------
  // Save Plots
  //---------------------

  outfile_->cd();
  savePlotsDir(h_1d_global,outfile_,"");
  savePlotsDir(CREmuBase.crdyHistMap,outfile_,"cremubase");
  savePlotsDir(CREmuNj4.crdyHistMap,outfile_,"cremunj4");
  savePlotsDir(CREmuNj5.crdyHistMap,outfile_,"cremunj5");

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
void EmuLooper::fillHistosEMU(std::map<std::string, TH1*>& h_1d, const std::string& dirname, const std::string& s) {

  TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
  if (dir == 0) {
    dir = outfile_->mkdir(dirname.c_str());
  } 
  dir->cd();

  plot1D("h_Events"+s,  1, 1, h_1d, ";Events, Unweighted", 1, 0, 2);
  plot1D("h_Events_w"+s,  1,   evtweight_, h_1d, ";Events, Weighted", 1, 0, 2);
  plot1D("h_mt2"+s,       t.mt2,   evtweight_, h_1d, "; M_{T2} [GeV]", 150, 0, 1500);
  plot1D("h_met"+s,       t.met_pt,   evtweight_, h_1d, ";E_{T}^{miss} [GeV]", 150, 0, 1500);
  plot1D("h_ht"+s,       t.ht,   evtweight_, h_1d, ";H_{T} [GeV]", 120, 0, 3000);
  plot1D("h_nJet30"+s,       t.nJet30,   evtweight_, h_1d, ";N(jets)", 15, 0, 15);
  plot1D("h_nBJet20"+s,      t.nBJet20,   evtweight_, h_1d, ";N(bjets)", 6, 0, 6);
  plot1D("h_nlep"+s,     t.nlep,   evtweight_, h_1d, ";N(leps)", 10, 0, 10);
  plot1D("h_leppt1"+s,      t.lep_pt[0],   evtweight_, h_1d, ";p_{T}(lep1) [GeV]", 50, 0, 200);
  plot1D("h_leppt2"+s,      t.lep_pt[1],   evtweight_, h_1d, ";p_{T}(lep2) [GeV]", 50, 0, 200);
  plot1D("h_zllmass"+s,      t.zll_mass,   evtweight_, h_1d, ";m_{ll} [GeV]", 60, 0, 120);

  if (!t.isData && doSystVariationPlots && applyBtagSF) {
    // assume weights are already applied to central value
    plot1D("h_nBJet20_btagsf_heavy_UP"+s,      t.nBJet20,  evtweight_ / t.weight_btagsf * t.weight_btagsf_heavy_UP, h_1d, ";N(bjets)", 6, 0, 6);
    plot1D("h_nBJet20_btagsf_heavy_DN"+s,      t.nBJet20,  evtweight_ / t.weight_btagsf * t.weight_btagsf_heavy_DN, h_1d, ";N(bjets)", 6, 0, 6);
    plot1D("h_nBJet20_btagsf_light_UP"+s,      t.nBJet20,  evtweight_ / t.weight_btagsf * t.weight_btagsf_light_UP, h_1d, ";N(bjets)", 6, 0, 6);
    plot1D("h_nBJet20_btagsf_light_DN"+s,      t.nBJet20,  evtweight_ / t.weight_btagsf * t.weight_btagsf_light_DN, h_1d, ";N(bjets)", 6, 0, 6);
  }

  outfile_->cd();
  return;
}

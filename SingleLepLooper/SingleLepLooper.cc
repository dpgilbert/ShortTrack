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

// header
#include "SingleLepLooper.h"

//MT2
#include "../MT2CORE/Plotting/PlotUtilities.h"
#include "../MT2CORE/applyWeights.h"

using namespace std;
using namespace mt2;
using namespace duplicate_removal;

class mt2tree;
class SR;

// turn on to apply Nvtx reweighting to MC
bool doNvtxReweight = false;
// turn on to apply json file to data
bool applyJSON = true;
// veto on jets with pt > 30, |eta| > 3.0
bool doHFJetVeto = false;
// turn on to apply btag sf to central value
bool applyBtagSF = false;
// turn on to apply lepton sf to central value - take from babies
bool applyLeptonSFfromBabies = true;
// turn on to apply lepton sf to central value - reread from files
bool applyLeptonSFfromFiles = false;
// use 2016 ICHEP ISR weights based on nisrMatch, signal and ttbar only
bool applyISRWeights = true;
// turn on to enable plots with systematic variations applied. will only do variations for applied weights
bool doSystVariationPlots = true;

//_______________________________________
SingleLepLooper::SingleLepLooper(){
}
//_______________________________________
SingleLepLooper::~SingleLepLooper(){

};

//_______________________________________
void SingleLepLooper::SetSignalRegions(){

  // CRSLNoCut: no cuts beyond lepton
  CRSLNoCut.SetName("crslnocut");

  // CRSLMET50: met > 50
  CRSLMET50.SetName("crslmet50");
  CRSLMET50.SetVar("met_pt", 50, -1);

  // CRSLMET50Nj2: 2 jets, met > 50
  CRSLMET50Nj2.SetName("crslmet50nj2");
  CRSLMET50Nj2.SetVar("met_pt", 50, -1);
  CRSLMET50Nj2.SetVar("nJet30", 2, -1);

  // CRSLMET50Nj2Nb0: 2 jets, 0b, met > 50
  CRSLMET50Nj2Nb0.SetName("crslmet50nj2nb0");
  CRSLMET50Nj2Nb0.SetVar("met_pt", 50, -1);
  CRSLMET50Nj2Nb0.SetVar("nJet30", 2, -1);
  CRSLMET50Nj2Nb0.SetVar("nBJet20", 0, 1);

  // CRSLMET50Nj2Nb2: 2 jets, geq 2b, met > 50
  CRSLMET50Nj2Nb2.SetName("crslmet50nj2nb2");
  CRSLMET50Nj2Nb2.SetVar("met_pt", 50, -1);
  CRSLMET50Nj2Nb2.SetVar("nJet30", 2, -1);
  CRSLMET50Nj2Nb2.SetVar("nBJet20", 2, -1);

  // CRSLMET80MT30Nj2: 2 jets, met > 80, mt > 30
  CRSLMET80MT30Nj2.SetName("crslmet80mt30nj2");
  CRSLMET80MT30Nj2.SetVar("met_pt", 80, -1);
  CRSLMET80MT30Nj2.SetVar("nJet30", 2, -1);
  CRSLMET80MT30Nj2.SetVar("mt", 30, -1);

  // CRSLMET80MT30Nj2Nb0: 2 jets, 0b, met > 80, mt > 30
  CRSLMET80MT30Nj2Nb0.SetName("crslmet80mt30nj2nb0");
  CRSLMET80MT30Nj2Nb0.SetVar("met_pt", 80, -1);
  CRSLMET80MT30Nj2Nb0.SetVar("nJet30", 2, -1);
  CRSLMET80MT30Nj2Nb0.SetVar("nBJet20", 0, 1);
  CRSLMET80MT30Nj2Nb0.SetVar("mt", 30, -1);

  // CRSLMET80MT30Nj2Nb2: 2 jets, geq 2b, met > 80, mt > 30
  CRSLMET80MT30Nj2Nb2.SetName("crslmet80mt30nj2nb2");
  CRSLMET80MT30Nj2Nb2.SetVar("met_pt", 80, -1);
  CRSLMET80MT30Nj2Nb2.SetVar("nJet30", 2, -1);
  CRSLMET80MT30Nj2Nb2.SetVar("nBJet20", 2, -1);
  CRSLMET80MT30Nj2Nb2.SetVar("mt", 30, -1);

  // CRSLMET50MT30to100Nj6Nb1: 6 jets, geq 1b, met > 50, mt 30-100
  CRSLMET50MT30to100Nj6Nb1.SetName("crslmet50mt30to100nj6nb1");
  CRSLMET50MT30to100Nj6Nb1.SetVar("met_pt", 50, -1);
  CRSLMET50MT30to100Nj6Nb1.SetVar("nJet30", 6, -1);
  CRSLMET50MT30to100Nj6Nb1.SetVar("nBJet20", 1, -1);
  CRSLMET50MT30to100Nj6Nb1.SetVar("mt", 30, 100);

  // CRSLMET50MT30to100Nj7Nb1: 7 jets, geq 1b, met > 50, mt 30-100
  CRSLMET50MT30to100Nj7Nb1.SetName("crslmet50mt30to100nj7nb1");
  CRSLMET50MT30to100Nj7Nb1.SetVar("met_pt", 50, -1);
  CRSLMET50MT30to100Nj7Nb1.SetVar("nJet30", 7, -1);
  CRSLMET50MT30to100Nj7Nb1.SetVar("nBJet20", 1, -1);
  CRSLMET50MT30to100Nj7Nb1.SetVar("mt", 30, 100);

  // CRSLSUSWJETS: 2 jets, 0b, met > 250, ht > 250
  CRSLSUSWJETS.SetName("crslsuswjets");
  CRSLSUSWJETS.SetVar("met_pt", 250, -1);
  CRSLSUSWJETS.SetVar("nJet30", 2, -1);
  CRSLSUSWJETS.SetVar("nBJet20", 0, 1);
  CRSLSUSWJETS.SetVar("ht", 250, -1);

  // CRSLSUSTTBAR: 2 jets, >= 1b, met > 250, ht > 250
  CRSLSUSTTBAR.SetName("crslsusttbar");
  CRSLSUSTTBAR.SetVar("met_pt", 250, -1);
  CRSLSUSTTBAR.SetVar("nJet30", 2, -1);
  CRSLSUSTTBAR.SetVar("nBJet20", 1, -1);
  CRSLSUSTTBAR.SetVar("ht", 250, -1);
  
  // CRSLSUSTTBAR2: 2 jets, >= 2b, met > 250, ht > 250
  CRSLSUSTTBAR2.SetName("crslsusttbar2");
  CRSLSUSTTBAR2.SetVar("met_pt", 250, -1);
  CRSLSUSTTBAR2.SetVar("nJet30", 2, -1);
  CRSLSUSTTBAR2.SetVar("nBJet20", 2, -1);
  CRSLSUSTTBAR2.SetVar("ht", 250, -1);
}


//_______________________________________
void SingleLepLooper::loop(TChain* chain, std::string output_name){

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  gROOT->cd();
  cout << "[SingleLepLooper::loop] creating output file: " << output_name << endl;

  outfile_ = new TFile(output_name.c_str(),"RECREATE") ; 

  // full 2016 dataset json, 36.26/fb:
  const char* json_file = "../babymaker/jsons/Cert_271036-284044_13TeV_23Sep2016ReReco_Collisions16_JSON_snt.txt";
  if (applyJSON) {
    cout << "Loading json file: " << json_file << endl;
    set_goodrun_file(json_file);
  }

  h_nvtx_weights_ = 0;
  if (doNvtxReweight) {
    TFile* f_weights = new TFile("../babymaker/data/hists_reweight_zjets_Run2015B.root");
    TH1D* h_nvtx_weights_temp = (TH1D*) f_weights->Get("h_nVert_ratio");
    outfile_->cd();
    h_nvtx_weights_ = (TH1D*) h_nvtx_weights_temp->Clone("h_nvtx_weights");
    f_weights->Close();
  }
  
  if (applyLeptonSFfromFiles) {
    setElSFfile("../babymaker/lepsf/moriond17/scaleFactors_el_moriond_2017.root", "../babymaker/lepsf/moriond17/egammaEffi.txt_EGM2D.root" );
    setMuSFfileNoTrk("../babymaker/lepsf/moriond17/TnP_NUM_LooseID_DENOM_generalTracks_VAR_map_pt_eta.root",
		     "../babymaker/lepsf/moriond17/TnP_NUM_MiniIsoTight_DENOM_LooseID_VAR_map_pt_eta.root",
		     "../babymaker/lepsf/moriond17/TnP_NUM_MediumIP2D_DENOM_LooseID_VAR_map_pt_eta.root");
  }
  
  cout << "[SingleLepLooper::loop] setting up histos" << endl;

  SetSignalRegions();

  // File Loop
  int nDuplicates = 0;
  int nEvents = chain->GetEntries();
  unsigned int nEventsChain = nEvents;
  cout << "[SingleLepLooper::loop] running on " << nEventsChain << " events" << endl;
  unsigned int nEventsTotal = 0;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;
  while ( (currentFile = (TFile*)fileIter.Next()) ) {
    cout << "[SingleLepLooper::loop] running on file: " << currentFile->GetTitle() << endl;

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

      // MET filters 
      if (t.isData) {
	if (!t.Flag_goodVertices) continue;
	if (!t.Flag_globalTightHalo2016Filter) continue; 
	if (!t.Flag_eeBadScFilter) continue; 
	if (!t.Flag_HBHENoiseFilter) continue;
	if (!t.Flag_HBHENoiseIsoFilter) continue;
	if (!t.Flag_EcalDeadCellTriggerPrimitiveFilter) continue;
	if (!t.Flag_badMuonFilter) continue;
	if (!t.Flag_badChargedHadronFilter) continue;
      }

      // basic single lepton selection: vertex, 1 lepton, single lep trigger
      if (t.nVert == 0) continue;
      if (t.nlep < 1) continue;
      //      if (t.isData && (!t.HLT_PFMETNoMu100_PFMHTNoMu100 && !t.HLT_PFMET100_PFMHT100)) continue;
      if (t.isData && (!t.HLT_SingleEl && !t.HLT_SingleEl_NonIso && !t.HLT_SingleMu && !t.HLT_SingleMu_NonIso)) continue;
      //if (!t.HLT_SingleEl && !t.HLT_SingleMu) continue;

      // remove low pt QCD samples 
      if (t.evt_id >= 100 && t.evt_id < 108) continue;

      //---------------------
      // set weights and start making plots
      //---------------------
      outfile_->cd();
      const float lumi = 36.81431;
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
	// prioritize files for lepton SF, if we accidentally gave both options
	if (applyLeptonSFfromFiles) {
	  fillLepSFWeightsFromFile();
	  evtweight_ *= weight_lepsf_;
	}
	// taking lepton SF from babies
	else if (applyLeptonSFfromBabies) {
	  evtweight_ *= t.weight_lepsf;
	}
	if (applyISRWeights && t.evt_id >= 301 && t.evt_id <= 303) {
	  float avgweight_isr = getAverageISRWeight(t.evt_id, 0);
	  evtweight_ *= t.weight_isr / avgweight_isr;
	}
      } // !isData

      // count number of forward jets
      nJet30Eta3_ = 0;
      for (int ijet = 0; ijet < t.njet; ++ijet) {
	if (t.jet_pt[ijet] > 30. && fabs(t.jet_eta[ijet]) > 3.0) ++nJet30Eta3_;
      }

      // veto on forward jets
      if (doHFJetVeto && nJet30Eta3_ > 0) continue;

      // --- additional selection for single lepton events
      if (t.nlep != 1) continue; // require exactly 1 loose lepton
      //if (t.nlep < 1) continue; // require at least 1 loose lepton
      //      if (t.lep_pt[0] < 30.) continue; // pt 30
      if (t.lep_pt[0] < 25.) continue; // pt 25

      // flavor specific cuts
      bool pass_electrons = bool( abs(t.lep_pdgId[0]) == 11 && t.lep_tightId[0] > 2 && t.lep_relIso03[0] < 0.1 && t.lep_relIso03[0] * t.lep_pt[0] < 5. && (fabs(t.lep_eta[0]) < 1.4 || fabs(t.lep_eta[0]) > 1.6) && fabs(t.lep_eta[0]) < 2.1 );
      //bool pass_electrons = bool( abs(t.lep_pdgId[0]) == 11 && t.lep_tightId[0] > 2 && t.lep_miniRelIso[0] < 0.1 );
      bool is_eb = bool(fabs(t.lep_eta[0]) < 1.479);
    
      //bool pass_muons = bool(abs(t.lep_pdgId[0]) == 13 && t.lep_tightId[0] > 0 && t.lep_miniRelIso[0] < 0.2);
      bool pass_muons = bool(abs(t.lep_pdgId[0]) == 13 && t.lep_tightId[0] > 0 && t.lep_miniRelIso[0] < 0.2 && t.lep_relIso03[0] * t.lep_pt[0] < 4.);

      if (!pass_electrons && !pass_muons) continue;
      float mt = MT(t.lep_pt[0],t.lep_phi[0],t.met_pt,t.met_phi);
      if (mt > 100.) continue;

      fillHistos(CRSLNoCut.crslHistMap,"crslnocut");
      if (pass_electrons) {
	fillHistos(CRSLNoCut.crslelHistMap,"crslelnocut");
	if (is_eb) fillHistos(CRSLNoCut.crslelHistMap,"crslelnocut","_eb");
	else fillHistos(CRSLNoCut.crslelHistMap,"crslelnocut","_ee");
      }
      else if (pass_muons) fillHistos(CRSLNoCut.crslmuHistMap,"crslmunocut");

      std::map<std::string, float> valuesMET50;
      valuesMET50["met_pt"] = t.met_pt;
      if (CRSLMET50.PassesSelection(valuesMET50)) {
	fillHistos(CRSLMET50.crslHistMap,"crslmet50");
	if (pass_electrons) {
	  fillHistos(CRSLMET50.crslelHistMap,"crslelmet50");
	  if (is_eb) fillHistos(CRSLMET50.crslelHistMap,"crslelmet50","_eb");
	  else fillHistos(CRSLMET50.crslelHistMap,"crslelmet50","_ee");
	}
	else if (pass_muons) fillHistos(CRSLMET50.crslmuHistMap,"crslmumet50");
      }

      std::map<std::string, float> valuesMET50Nj2;
      valuesMET50Nj2["met_pt"] = t.met_pt;
      valuesMET50Nj2["nJet30"] = t.nJet30;
      if (CRSLMET50Nj2.PassesSelection(valuesMET50Nj2)) {
	fillHistos(CRSLMET50Nj2.crslHistMap,"crslmet50nj2");
	if (pass_electrons) {
	  fillHistos(CRSLMET50Nj2.crslelHistMap,"crslelmet50nj2");
	  if (is_eb) fillHistos(CRSLMET50Nj2.crslelHistMap,"crslelmet50nj2","_eb");
	  else fillHistos(CRSLMET50Nj2.crslelHistMap,"crslelmet50nj2","_ee");
	}
	else if (pass_muons) fillHistos(CRSLMET50Nj2.crslmuHistMap,"crslmumet50nj2");
      }

      std::map<std::string, float> valuesMET50Nj2Nb0;
      valuesMET50Nj2Nb0["met_pt"] = t.met_pt;
      valuesMET50Nj2Nb0["nJet30"] = t.nJet30;
      valuesMET50Nj2Nb0["nBJet20"] = t.nBJet20;
      if (CRSLMET50Nj2Nb0.PassesSelection(valuesMET50Nj2Nb0)) {
	fillHistos(CRSLMET50Nj2Nb0.crslHistMap,"crslmet50nj2nb0");
	if (pass_electrons) {
	  fillHistos(CRSLMET50Nj2Nb0.crslelHistMap,"crslelmet50nj2nb0");
	  if (is_eb) fillHistos(CRSLMET50Nj2Nb0.crslelHistMap,"crslelmet50nj2nb0","_eb");
	  else fillHistos(CRSLMET50Nj2Nb0.crslelHistMap,"crslelmet50nj2nb0","_ee");
	}
	else if (pass_muons) fillHistos(CRSLMET50Nj2Nb0.crslmuHistMap,"crslmumet50nj2nb0");
      }

      std::map<std::string, float> valuesMET50Nj2Nb2;
      valuesMET50Nj2Nb2["met_pt"] = t.met_pt;
      valuesMET50Nj2Nb2["nJet30"] = t.nJet30;
      valuesMET50Nj2Nb2["nBJet20"] = t.nBJet20;
      if (CRSLMET50Nj2Nb2.PassesSelection(valuesMET50Nj2Nb2)) {
	fillHistos(CRSLMET50Nj2Nb2.crslHistMap,"crslmet50nj2nb2");
	if (pass_electrons) {
	  fillHistos(CRSLMET50Nj2Nb2.crslelHistMap,"crslelmet50nj2nb2");
	  if (is_eb) fillHistos(CRSLMET50Nj2Nb2.crslelHistMap,"crslelmet50nj2nb2","_eb");
	  else fillHistos(CRSLMET50Nj2Nb2.crslelHistMap,"crslelmet50nj2nb2","_ee");
	}
	else if (pass_muons) fillHistos(CRSLMET50Nj2Nb2.crslmuHistMap,"crslmumet50nj2nb2");
      }

      std::map<std::string, float> valuesMET80MT30Nj2;
      valuesMET80MT30Nj2["met_pt"] = t.met_pt;
      valuesMET80MT30Nj2["nJet30"] = t.nJet30;
      valuesMET80MT30Nj2["mt"] = mt;
      if (CRSLMET80MT30Nj2.PassesSelection(valuesMET80MT30Nj2)) {
	fillHistos(CRSLMET80MT30Nj2.crslHistMap,"crslmet80mt30nj2");
	if (pass_electrons) {
	  fillHistos(CRSLMET80MT30Nj2.crslelHistMap,"crslelmet80mt30nj2");
	  if (is_eb) fillHistos(CRSLMET80MT30Nj2.crslelHistMap,"crslelmet80mt30nj2","_eb");
	  else fillHistos(CRSLMET80MT30Nj2.crslelHistMap,"crslelmet80mt30nj2","_ee");
	}
	else if (pass_muons) fillHistos(CRSLMET80MT30Nj2.crslmuHistMap,"crslmumet80mt30nj2");
      }

      std::map<std::string, float> valuesMET80MT30Nj2Nb0;
      valuesMET80MT30Nj2Nb0["met_pt"] = t.met_pt;
      valuesMET80MT30Nj2Nb0["nJet30"] = t.nJet30;
      valuesMET80MT30Nj2Nb0["nBJet20"] = t.nBJet20;
      valuesMET80MT30Nj2Nb0["mt"] = mt;
      if (CRSLMET80MT30Nj2Nb0.PassesSelection(valuesMET80MT30Nj2Nb0)) {
	fillHistos(CRSLMET80MT30Nj2Nb0.crslHistMap,"crslmet80mt30nj2nb0");
	if (pass_electrons) {
	  fillHistos(CRSLMET80MT30Nj2Nb0.crslelHistMap,"crslelmet80mt30nj2nb0");
	  if (is_eb) fillHistos(CRSLMET80MT30Nj2Nb0.crslelHistMap,"crslelmet80mt30nj2nb0","_eb");
	  else fillHistos(CRSLMET80MT30Nj2Nb0.crslelHistMap,"crslelmet80mt30nj2nb0","_ee");
	}
	else if (pass_muons) fillHistos(CRSLMET80MT30Nj2Nb0.crslmuHistMap,"crslmumet80mt30nj2nb0");
      }

      std::map<std::string, float> valuesMET80MT30Nj2Nb2;
      valuesMET80MT30Nj2Nb2["met_pt"] = t.met_pt;
      valuesMET80MT30Nj2Nb2["nJet30"] = t.nJet30;
      valuesMET80MT30Nj2Nb2["nBJet20"] = t.nBJet20;
      valuesMET80MT30Nj2Nb2["mt"] = mt;
      if (CRSLMET80MT30Nj2Nb2.PassesSelection(valuesMET80MT30Nj2Nb2)) {
	fillHistos(CRSLMET80MT30Nj2Nb2.crslHistMap,"crslmet80mt30nj2nb2");
	if (pass_electrons) {
	  fillHistos(CRSLMET80MT30Nj2Nb2.crslelHistMap,"crslelmet80mt30nj2nb2");
	  if (is_eb) fillHistos(CRSLMET80MT30Nj2Nb2.crslelHistMap,"crslelmet80mt30nj2nb2","_eb");
	  else fillHistos(CRSLMET80MT30Nj2Nb2.crslelHistMap,"crslelmet80mt30nj2nb2","_ee");
	}
	else if (pass_muons) fillHistos(CRSLMET80MT30Nj2Nb2.crslmuHistMap,"crslmumet80mt30nj2nb2");
      }

      std::map<std::string, float> valuesMET50MT30to100Nj6Nb1;
      valuesMET50MT30to100Nj6Nb1["met_pt"] = t.met_pt;
      valuesMET50MT30to100Nj6Nb1["nJet30"] = t.nJet30;
      valuesMET50MT30to100Nj6Nb1["nBJet20"] = t.nBJet20;
      valuesMET50MT30to100Nj6Nb1["mt"] = mt;
      if (CRSLMET50MT30to100Nj6Nb1.PassesSelection(valuesMET50MT30to100Nj6Nb1)) {
	fillHistos(CRSLMET50MT30to100Nj6Nb1.crslHistMap,"crslmet50mt30to100nj6nb1");
	if (pass_electrons) {
	  fillHistos(CRSLMET50MT30to100Nj6Nb1.crslelHistMap,"crslelmet50mt30to100nj6nb1");
	  if (is_eb) fillHistos(CRSLMET50MT30to100Nj6Nb1.crslelHistMap,"crslelmet50mt30to100nj6nb1","_eb");
	  else fillHistos(CRSLMET50MT30to100Nj6Nb1.crslelHistMap,"crslelmet50mt30to100nj6nb1","_ee");
	}
	else if (pass_muons) fillHistos(CRSLMET50MT30to100Nj6Nb1.crslmuHistMap,"crslmumet50mt30to100nj6nb1");
      }

      std::map<std::string, float> valuesMET50MT30to100Nj7Nb1;
      valuesMET50MT30to100Nj7Nb1["met_pt"] = t.met_pt;
      valuesMET50MT30to100Nj7Nb1["nJet30"] = t.nJet30;
      valuesMET50MT30to100Nj7Nb1["nBJet20"] = t.nBJet20;
      valuesMET50MT30to100Nj7Nb1["mt"] = mt;
      if (CRSLMET50MT30to100Nj7Nb1.PassesSelection(valuesMET50MT30to100Nj7Nb1)) {
	fillHistos(CRSLMET50MT30to100Nj7Nb1.crslHistMap,"crslmet50mt30to100nj7nb1");
	if (pass_electrons) {
	  fillHistos(CRSLMET50MT30to100Nj7Nb1.crslelHistMap,"crslelmet50mt30to100nj7nb1");
	  if (is_eb) fillHistos(CRSLMET50MT30to100Nj7Nb1.crslelHistMap,"crslelmet50mt30to100nj7nb1","_eb");
	  else fillHistos(CRSLMET50MT30to100Nj7Nb1.crslelHistMap,"crslelmet50mt30to100nj7nb1","_ee");
	}
	else if (pass_muons) fillHistos(CRSLMET50MT30to100Nj7Nb1.crslmuHistMap,"crslmumet50mt30to100nj7nb1");
      }

      std::map<std::string, float> valuesSUSWJETS;
      valuesSUSWJETS["met_pt"] = t.met_pt;
      valuesSUSWJETS["nJet30"] = t.nJet30;
      valuesSUSWJETS["nBJet20"] = t.nBJet20;
      valuesSUSWJETS["ht"] = t.ht;
      if (CRSLSUSWJETS.PassesSelection(valuesSUSWJETS)) {
	fillHistos(CRSLSUSWJETS.crslHistMap,"crslsuswjets");
	if (pass_electrons) {
	  fillHistos(CRSLSUSWJETS.crslelHistMap,"crslelsuswjets");
	  if (is_eb) fillHistos(CRSLSUSWJETS.crslelHistMap,"crslelsuswjets","_eb");
	  else fillHistos(CRSLSUSWJETS.crslelHistMap,"crslelsuswjets","_ee");
	}
	else if (pass_muons) fillHistos(CRSLSUSWJETS.crslmuHistMap,"crslmususwjets");
      }

      std::map<std::string, float> valuesSUSTTBAR;
      valuesSUSTTBAR["met_pt"] = t.met_pt;
      valuesSUSTTBAR["nJet30"] = t.nJet30;
      valuesSUSTTBAR["nBJet20"] = t.nBJet20;
      valuesSUSTTBAR["ht"] = t.ht;
      if (CRSLSUSTTBAR.PassesSelection(valuesSUSTTBAR)) {
	fillHistos(CRSLSUSTTBAR.crslHistMap,"crslsusttbar");
	if (pass_electrons) {
	  fillHistos(CRSLSUSTTBAR.crslelHistMap,"crslelsusttbar");
	  if (is_eb) fillHistos(CRSLSUSTTBAR.crslelHistMap,"crslelsusttbar","_eb");
	  else fillHistos(CRSLSUSTTBAR.crslelHistMap,"crslelsusttbar","_ee");
	}
	else if (pass_muons) fillHistos(CRSLSUSTTBAR.crslmuHistMap,"crslmususttbar");
      }

      std::map<std::string, float> valuesSUSTTBAR2;
      valuesSUSTTBAR2["met_pt"] = t.met_pt;
      valuesSUSTTBAR2["nJet30"] = t.nJet30;
      valuesSUSTTBAR2["nBJet20"] = t.nBJet20;
      valuesSUSTTBAR2["ht"] = t.ht;
      if (CRSLSUSTTBAR2.PassesSelection(valuesSUSTTBAR2)) {
	fillHistos(CRSLSUSTTBAR2.crslHistMap,"crslsusttbar2");
	if (pass_electrons) {
	  fillHistos(CRSLSUSTTBAR2.crslelHistMap,"crslelsusttbar2");
	  if (is_eb) fillHistos(CRSLSUSTTBAR2.crslelHistMap,"crslelsusttbar2","_eb");
	  else fillHistos(CRSLSUSTTBAR2.crslelHistMap,"crslelsusttbar2","_ee");
	}
	else if (pass_muons) fillHistos(CRSLSUSTTBAR2.crslmuHistMap,"crslmususttbar2");
      }

    }//end loop on events in a file
  
    delete tree;
    f.Close();
  }//end loop on files
  
  cout << "[SingleLepLooper::loop] processed " << nEventsTotal << " events" << endl;
  if ( nEventsChain != nEventsTotal ) {
    std::cout << "ERROR: number of events from files is not equal to total number of events" << std::endl;
  }

  cout << nDuplicates << " duplicate events were skipped." << endl;

  //---------------------
  // Save Plots
  //---------------------

  outfile_->cd();
  savePlotsDir(h_1d_global,outfile_,"");
  savePlotsDir(CRSLNoCut.crslHistMap,outfile_,"crslnocut");
  savePlotsDir(CRSLNoCut.crslelHistMap,outfile_,"crslelnocut");
  savePlotsDir(CRSLNoCut.crslmuHistMap,outfile_,"crslmunocut");

  savePlotsDir(CRSLMET50.crslHistMap,outfile_,"crslmet50");
  savePlotsDir(CRSLMET50.crslelHistMap,outfile_,"crslelmet50");
  savePlotsDir(CRSLMET50.crslmuHistMap,outfile_,"crslmumet50");

  savePlotsDir(CRSLMET50Nj2.crslHistMap,outfile_,"crslmet50nj2");
  savePlotsDir(CRSLMET50Nj2.crslelHistMap,outfile_,"crslelmet50nj2");
  savePlotsDir(CRSLMET50Nj2.crslmuHistMap,outfile_,"crslmumet50nj2");

  savePlotsDir(CRSLMET50Nj2Nb0.crslHistMap,outfile_,"crslmet50nj2nb0");
  savePlotsDir(CRSLMET50Nj2Nb0.crslelHistMap,outfile_,"crslelmet50nj2nb0");
  savePlotsDir(CRSLMET50Nj2Nb0.crslmuHistMap,outfile_,"crslmumet50nj2nb0");

  savePlotsDir(CRSLMET50Nj2Nb2.crslHistMap,outfile_,"crslmet50nj2nb2");
  savePlotsDir(CRSLMET50Nj2Nb2.crslelHistMap,outfile_,"crslelmet50nj2nb2");
  savePlotsDir(CRSLMET50Nj2Nb2.crslmuHistMap,outfile_,"crslmumet50nj2nb2");

  savePlotsDir(CRSLMET80MT30Nj2.crslHistMap,outfile_,"crslmet80mt30nj2");
  savePlotsDir(CRSLMET80MT30Nj2.crslelHistMap,outfile_,"crslelmet80mt30nj2");
  savePlotsDir(CRSLMET80MT30Nj2.crslmuHistMap,outfile_,"crslmumet80mt30nj2");

  savePlotsDir(CRSLMET80MT30Nj2Nb0.crslHistMap,outfile_,"crslmet80mt30nj2nb0");
  savePlotsDir(CRSLMET80MT30Nj2Nb0.crslelHistMap,outfile_,"crslelmet80mt30nj2nb0");
  savePlotsDir(CRSLMET80MT30Nj2Nb0.crslmuHistMap,outfile_,"crslmumet80mt30nj2nb0");

  savePlotsDir(CRSLMET80MT30Nj2Nb2.crslHistMap,outfile_,"crslmet80mt30nj2nb2");
  savePlotsDir(CRSLMET80MT30Nj2Nb2.crslelHistMap,outfile_,"crslelmet80mt30nj2nb2");
  savePlotsDir(CRSLMET80MT30Nj2Nb2.crslmuHistMap,outfile_,"crslmumet80mt30nj2nb2");

  savePlotsDir(CRSLMET50MT30to100Nj6Nb1.crslHistMap,outfile_,"crslmet50mt30to100nj6nb1");
  savePlotsDir(CRSLMET50MT30to100Nj6Nb1.crslelHistMap,outfile_,"crslelmet50mt30to100nj6nb1");
  savePlotsDir(CRSLMET50MT30to100Nj6Nb1.crslmuHistMap,outfile_,"crslmumet50mt30to100nj6nb1");

  savePlotsDir(CRSLMET50MT30to100Nj7Nb1.crslHistMap,outfile_,"crslmet50mt30to100nj7nb1");
  savePlotsDir(CRSLMET50MT30to100Nj7Nb1.crslelHistMap,outfile_,"crslelmet50mt30to100nj7nb1");
  savePlotsDir(CRSLMET50MT30to100Nj7Nb1.crslmuHistMap,outfile_,"crslmumet50mt30to100nj7nb1");

  savePlotsDir(CRSLSUSWJETS.crslHistMap,outfile_,"crslsuswjets");
  savePlotsDir(CRSLSUSWJETS.crslelHistMap,outfile_,"crslelsuswjets");
  savePlotsDir(CRSLSUSWJETS.crslmuHistMap,outfile_,"crslmususwjets");

  savePlotsDir(CRSLSUSTTBAR.crslHistMap,outfile_,"crslsusttbar");
  savePlotsDir(CRSLSUSTTBAR.crslelHistMap,outfile_,"crslelsusttbar");
  savePlotsDir(CRSLSUSTTBAR.crslmuHistMap,outfile_,"crslmususttbar");

  savePlotsDir(CRSLSUSTTBAR2.crslHistMap,outfile_,"crslsusttbar2");
  savePlotsDir(CRSLSUSTTBAR2.crslelHistMap,outfile_,"crslelsusttbar2");
  savePlotsDir(CRSLSUSTTBAR2.crslmuHistMap,outfile_,"crslmususttbar2");

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
void SingleLepLooper::fillHistos(std::map<std::string, TH1*>& h_1d, const std::string& dirname, const std::string& s) {

  if (dirname.size()) {
    TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
    if (dir == 0) {
      dir = outfile_->mkdir(dirname.c_str());
    } 
    dir->cd();
  } else {
    outfile_->cd();
  }

  float mt = MT(t.lep_pt[0],t.lep_phi[0],t.met_pt,t.met_phi);

  plot1D("h_Events"+s,  1, 1, h_1d, ";Events, Unweighted", 1, 0, 2);
  plot1D("h_Events_w"+s,  1,   evtweight_, h_1d, ";Events, Weighted", 1, 0, 2);
  plot1D("h_nVert"+s,      t.nVert,   evtweight_, h_1d, ";N(vertices)",50,0,50);
  plot1D("h_lep_pt"+s,     t.lep_pt[0],   evtweight_, h_1d, ";p_{T} [GeV]", 1000, 0., 1000.);
  plot1D("h_ht"+s,         t.ht,   evtweight_, h_1d, ";H_{T} [GeV]", 120, 0, 3000);
  plot1D("h_nJet30"+s,     t.nJet30,   evtweight_, h_1d, ";N(jets)", 15, 0, 15);
  plot1D("h_nJet30Eta3"+s, nJet30Eta3_,   evtweight_, h_1d, ";N(jets, |#eta| > 3.0)", 10, 0, 10);
  plot1D("h_nBJet20"+s,    t.nBJet20,   evtweight_, h_1d, ";N(bjets)", 6, 0, 6);
  plot1D("h_nBJet30"+s,    t.nBJet30,   evtweight_, h_1d, ";N(bjets, p_{T} > 30 GeV)", 6, 0, 6);
  plot1D("h_met"+s,        t.met_pt,   evtweight_, h_1d, "; E_{T}^{miss} [GeV]",500,0,1000);
  plot1D("h_mt2"+s,        t.mt2,   evtweight_, h_1d, ";M_{T2} [GeV]",500,0,1000);
  plot1D("h_mt"+s,         mt,   evtweight_, h_1d, ";M_{T} [GeV]",250,0,500);

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

//_______________________________________
void SingleLepLooper::fillLepSFWeightsFromFile() {

  weight_lepsf_ = 1.;
  weight_lepsf_UP_ = 1.;
  weight_lepsf_DN_ = 1.;

  for (int ilep = 0; ilep < t.nlep; ++ilep) {
    weightStruct weights = getLepSFFromFile(t.lep_pt[ilep], t.lep_eta[ilep], t.lep_pdgId[ilep]);
    weight_lepsf_ *= weights.cent;
    weight_lepsf_UP_ *= weights.up;
    weight_lepsf_DN_ *= weights.dn;
  } // loop over leps
  
  return;
}

//_______________________________________
float SingleLepLooper::getAverageISRWeight(const int evt_id, const int var) {

  // madgraph ttsl, from RunIISummer16MiniAODv2
  if (evt_id == 301 || evt_id == 302) {
    if (var == 0) return 0.909; // nominal
    else if (var == 1) return 0.954; // UP
    else if (var == -1) return 0.863; // DN
  }
  // madgraph ttdl, from RunIISummer16MiniAODv2
  else if (evt_id == 303) {
    if (var == 0) return 0.895; // nominal
    else if (var == 1) return 0.948; // UP
    else if (var == -1) return 0.843; // DN
  }

  std::cout << "WARNING: MT2Looper::getAverageISRWeight: didn't recognize either evt_id: " << evt_id
	    << " or variation: " << var << std::endl;
  return 1.;
}

// C++
#include <iostream>
#include <vector>
#include <set>
#include <cmath>
#include <sstream>
#include <stdexcept>

// ROOT
#include "TDirectory.h"
#include "TTreeCache.h"
#include "Math/VectorUtil.h"
#include "Math/Vector4D.h"
#include "TVector2.h"
#include "TBenchmark.h"
#include "TMath.h"
#include "TLorentzVector.h"

#include "RooRealVar.h"
#include "RooDataSet.h"

// Tools
#include "../CORE/Tools/utils.h"
#include "../CORE/Tools/goodrun.h"
#include "../CORE/Tools/dorky/dorky.h"
#include "../CORE/Tools/badEventFilter.h"
#include "../CORE/Tools/hemJet.h" 
#include "../CORE/Tools/MT2/MT2.h"

// header
#include "MT2Looper.h"

//MT2
#include "../MT2CORE/Plotting/PlotUtilities.h"
#include "../MT2CORE/applyWeights.h"

using namespace std;
using namespace mt2;
using namespace duplicate_removal;
using namespace RooFit;

class mt2tree;
class SR;

std::string toString(float in){
  std::stringstream ss;
  ss << in;
  return ss.str();
}

// generic binning for signal scans - need arrays since mt2 dimension will be variable
//   assuming here: 25 GeV binning, m1 from 0-2000, m2 from 0-2000
const int n_m1bins = 81;
float m1bins[n_m1bins+1];
const int n_m2bins = 81;
float m2bins[n_m2bins+1];

const int n_htbins = 5;
const float htbins[n_htbins+1] = {200, 450., 575., 1000., 1500., 3000.};
const int n_htbins2 = 7;
const float htbins2[n_htbins2+1] = {200, 250., 350., 450., 575., 700., 1000., 3000.};
const int n_njbins = 4;
const float njbins[n_njbins+1] = {1, 2, 4, 7, 12};
const int n_nbjbins = 4;
const float nbjbins[n_nbjbins+1] = {0, 1, 2, 3, 6};
const int n_ptVbins = 19;
float logstep(int i) {
  return TMath::Power(10, 2+4.5e-02*i);
}
const float ptVbins[n_ptVbins+1] = {100, logstep(1), logstep(2), logstep(3), logstep(4), logstep(5), logstep(6), logstep(7), logstep(8), logstep(9), 
				    logstep(10), logstep(11), logstep(12), logstep(13), logstep(14), logstep(15), logstep(16), logstep(17), 800, 1200};

//RooRealVars for unbinned data hist
RooRealVar* x_ = new RooRealVar( "x", "", 0., 50.);
RooRealVar* w_ = new RooRealVar( "w", "", 0., 1000.);

bool useDRforGammaQCDMixing = true; // requires GenParticles
// turn on to apply weights to central value
bool applyWeights = false;
// turn on to apply btag sf to central value
bool applyBtagSF = true;
// turn on to apply lepton sf to central value
bool applyLeptonSF = true;
// turn on to apply reweighting to ttbar based on top pt
bool applyTopPtReweight = true;
// turn on to apply lepton sf to central value for 0L sample in fastsim
bool applyLeptonSFfastsim = false;
// turn on to enable plots of MT2 with systematic variations applied. will only do variations for applied weights
bool doSystVariationPlots = false;
// turn on to apply Nvtx reweighting to MC
bool doNvtxReweight = false;
// turn on to apply json file to data
bool applyJSON = false;
// veto on jets with pt > 30, |eta| > 3.0
bool doHFJetVeto = false;
// get signal scan nevents from file
bool doScanWeights = true;
// doesn't plot data for MT2 > 200 in signal regions
bool doBlindData = false;
// make variation histograms for tau efficiency
bool doGenTauVars = false;
// make variation histograms for e+mu efficiency
bool doLepEffVars = false;
// make only minimal hists needed for results
bool doMinimalPlots = false;

// This is meant to be passed as the third argument, the predicate, of the standard library sort algorithm
inline bool sortByPt(const LorentzVector &vec1, const LorentzVector &vec2 ) {
  return vec1.pt() > vec2.pt();
}

MT2Looper::MT2Looper(){

  // set up signal binning
  for (int i = 0; i <= n_m1bins; ++i) {
    m1bins[i] = i*25.;
  }
  for (int i = 0; i <= n_m2bins; ++i) {
    m2bins[i] = i*25.;
  }

}
MT2Looper::~MT2Looper(){

};

void MT2Looper::SetSignalRegions(){

  //SRVec =  getSignalRegionsZurich_jetpt30(); //same as getSignalRegionsZurich(), but with j1pt and j2pt cuts changed to 30 GeV
  SRVec =  getSignalRegionsJamboree(); //adds HT 200-450 regions
  SRVecLep =  getSignalRegionsLep(); 
  SRVecMonojet = getSignalRegionsMonojet(); // first pass of monojet regions

  //store histograms with cut values for all variables
  storeHistWithCutValues(SRVec, "sr");
  storeHistWithCutValues(SRVec, "crsl");
  storeHistWithCutValues(SRVec, "crgj");
  storeHistWithCutValues(SRVec, "crrl");
  storeHistWithCutValues(SRVecMonojet, "sr");
  storeHistWithCutValues(SRVecMonojet, "crsl");
  storeHistWithCutValues(SRVecMonojet, "crgj");
  storeHistWithCutValues(SRVecMonojet, "crrl");

  storeHistWithCutValues(SRVecLep, "srLep");

  SRBase.SetName("srbase");
  SRBase.SetVar("mt2", 200, -1);
  SRBase.SetVar("j1pt", 30, -1);
  SRBase.SetVar("j2pt", 30, -1);
  SRBase.SetVar("deltaPhiMin", 0.3, -1);
  SRBase.SetVar("diffMetMhtOverMet", 0, 0.5);
  SRBase.SetVar("nlep", 0, 1);
  SRBase.SetVar("passesHtMet", 1, 2);
  SRBase.SetVarCRSL("mt2", 200, -1);
  SRBase.SetVarCRSL("j1pt", 30, -1);
  SRBase.SetVarCRSL("j2pt", 30, -1);
  SRBase.SetVarCRSL("deltaPhiMin", 0.3, -1);
  SRBase.SetVarCRSL("diffMetMhtOverMet", 0, 0.5);
  SRBase.SetVarCRSL("nlep", 1, 2);
  SRBase.SetVarCRSL("passesHtMet", 1, 2);
  float SRBase_mt2bins[8] = {200, 300, 400, 500, 600, 800, 1000, 1500}; 
  SRBase.SetMT2Bins(7, SRBase_mt2bins);


  //setup inclusive regions
  SR InclusiveHT200to450 = SRBase;
  InclusiveHT200to450.SetName("srbaseVL");
  InclusiveHT200to450.SetVar("ht", 200, 450);
  InclusiveHT200to450.SetVarCRSL("ht", 200, 450);
  InclusiveRegions.push_back(InclusiveHT200to450);

  SR InclusiveHT450to575 = SRBase;
  InclusiveHT450to575.SetName("srbaseL");
  InclusiveHT450to575.SetVar("ht", 450, 575);
  InclusiveHT450to575.SetVarCRSL("ht", 450, 575);
  InclusiveRegions.push_back(InclusiveHT450to575);

  SR InclusiveHT575to1000 = SRBase;
  InclusiveHT575to1000.SetName("srbaseM");
  InclusiveHT575to1000.SetVar("ht", 575, 1000);
  InclusiveHT575to1000.SetVarCRSL("ht", 575, 1000);
  InclusiveRegions.push_back(InclusiveHT575to1000);

  SR InclusiveHT1000to1500 = SRBase;
  InclusiveHT1000to1500.SetName("srbaseH");
  InclusiveHT1000to1500.SetVar("ht", 1000, 1500);
  InclusiveHT1000to1500.SetVarCRSL("ht", 1000, 1500);
  InclusiveRegions.push_back(InclusiveHT1000to1500);

  SR InclusiveHT1500toInf = SRBase;
  InclusiveHT1500toInf.SetName("srbaseUH");
  InclusiveHT1500toInf.SetVar("ht", 1500, -1);
  InclusiveHT1500toInf.SetVarCRSL("ht", 1500, -1);
  InclusiveRegions.push_back(InclusiveHT1500toInf);

  SR InclusiveNJets2to3 = SRBase;
  InclusiveNJets2to3.SetName("InclusiveNJets2to3");
  InclusiveNJets2to3.SetVar("njets", 2, 4);
  InclusiveRegions.push_back(InclusiveNJets2to3);

  SR InclusiveNJets4to6 = SRBase;
  InclusiveNJets4to6.SetName("InclusiveNJets4to6");
  InclusiveNJets4to6.SetVar("njets", 4, 7);
  InclusiveRegions.push_back(InclusiveNJets4to6);

  SR InclusiveNJets7toInf = SRBase;
  InclusiveNJets7toInf.SetName("InclusiveNJets7toInf");
  InclusiveNJets7toInf.SetVar("njets", 7, -1);
  InclusiveRegions.push_back(InclusiveNJets7toInf);

  SR InclusiveNBJets0 = SRBase;
  InclusiveNBJets0.SetName("InclusiveNBJets0");
  InclusiveNBJets0.SetVar("nbjets", 0, 1);
  InclusiveRegions.push_back(InclusiveNBJets0);

  SR InclusiveNBJets1 = SRBase;
  InclusiveNBJets1.SetName("InclusiveNBJets1");
  InclusiveNBJets1.SetVar("nbjets", 1, 2);
  InclusiveRegions.push_back(InclusiveNBJets1);

  SR InclusiveNBJets2 = SRBase;
  InclusiveNBJets2.SetName("InclusiveNBJets2");
  InclusiveNBJets2.SetVar("nbjets", 2, 3);
  InclusiveRegions.push_back(InclusiveNBJets2);

  SR InclusiveNBJets3toInf = SRBase;
  InclusiveNBJets3toInf.SetName("InclusiveNBJets3toInf");
  InclusiveNBJets3toInf.SetVar("nbjets", 3, -1);
  InclusiveRegions.push_back(InclusiveNBJets3toInf);

  for(unsigned int i=0; i<InclusiveRegions.size(); i++){
    TDirectory * dir = (TDirectory*)outfile_->Get((InclusiveRegions.at(i).GetName()).c_str());
    if (dir == 0) {
      dir = outfile_->mkdir((InclusiveRegions.at(i).GetName()).c_str());
    } 
  }

  // CRSL inclusive regions to isolate W+jets and ttbar
  CRSL_WJets = SRBase;
  CRSL_WJets.SetName("crslwjets");
  CRSL_WJets.SetVarCRSL("nbjets", 0, 1);
  CRSL_WJets.crslHistMap.clear();

  CRSL_TTbar = SRBase;
  CRSL_TTbar.SetName("crslttbar");
  CRSL_TTbar.SetVarCRSL("nbjets", 2, -1);
  CRSL_TTbar.crslHistMap.clear();

  // ----- monojet base regions

  SRBaseMonojet.SetName("srbaseJ");
  SRBaseMonojet.SetVar("j1pt", 200, -1);
  SRBaseMonojet.SetVar("njets", 1, 2);
  SRBaseMonojet.SetVar("nlep", 0, 1);
  SRBaseMonojet.SetVar("met", 200, -1);
  SRBaseMonojet.SetVar("deltaPhiMin", 0.3, -1);
  SRBaseMonojet.SetVar("diffMetMhtOverMet", 0, 0.5);
  SRBaseMonojet.SetVarCRSL("j1pt", 200, -1);
  SRBaseMonojet.SetVarCRSL("njets", 1, 2);
  SRBaseMonojet.SetVarCRSL("nlep", 1, 2);
  SRBaseMonojet.SetVarCRSL("met", 200, -1);
  SRBaseMonojet.SetVarCRSL("deltaPhiMin", 0.3, -1);
  SRBaseMonojet.SetVarCRSL("diffMetMhtOverMet", 0, 0.5);
  float SRBaseMonojet_mt2bins[8] = {200, 300, 400, 500, 600, 800, 1000, 1500};
  SRBaseMonojet.SetMT2Bins(7, SRBaseMonojet_mt2bins);

  SRBaseIncl.SetName("srbaseIncl");
  float SRBaseIncl_mt2bins[8] = {200, 300, 400, 500, 600, 800, 1000, 1500};
  SRBaseIncl.SetMT2Bins(7, SRBaseIncl_mt2bins);
}


void MT2Looper::storeHistWithCutValues(std::vector<SR> & srvector, TString SR) {
  
  for(unsigned int i = 0; i < srvector.size(); i++){
    std::vector<std::string> vars = srvector.at(i).GetListOfVariables();
    if (SR=="crsl") vars = srvector.at(i).GetListOfVariablesCRSL();
    TString dirname = SR+srvector.at(i).GetName();
    TDirectory * dir = (TDirectory*)outfile_->Get(dirname);
    if (dir == 0) {
      dir = outfile_->mkdir(dirname);
    } 
    dir->cd();
    for(unsigned int j = 0; j < vars.size(); j++){
      if (SR=="crsl") {
	plot1D("h_"+vars.at(j)+"_"+"LOW",  1, srvector.at(i).GetLowerBoundCRSL(vars.at(j)), srvector.at(i).crslHistMap, "", 1, 0, 2);
	plot1D("h_"+vars.at(j)+"_"+"HI",   1, srvector.at(i).GetUpperBoundCRSL(vars.at(j)), srvector.at(i).crslHistMap, "", 1, 0, 2);
      }
      else if (SR=="crgj") {
	plot1D("h_"+vars.at(j)+"_"+"LOW",  1, srvector.at(i).GetLowerBound(vars.at(j)), srvector.at(i).crgjHistMap, "", 1, 0, 2);
	plot1D("h_"+vars.at(j)+"_"+"HI",   1, srvector.at(i).GetLowerBound(vars.at(j)), srvector.at(i).crgjHistMap, "", 1, 0, 2);
      }
      else if (SR=="crrl") {
	plot1D("h_"+vars.at(j)+"_"+"LOW",  1, srvector.at(i).GetLowerBound(vars.at(j)), srvector.at(i).crrlHistMap, "", 1, 0, 2);
	plot1D("h_"+vars.at(j)+"_"+"HI",   1, srvector.at(i).GetLowerBound(vars.at(j)), srvector.at(i).crrlHistMap, "", 1, 0, 2);
      }
      else {
	plot1D("h_"+vars.at(j)+"_"+"LOW",  1, srvector.at(i).GetLowerBound(vars.at(j)), srvector.at(i).srHistMap, "", 1, 0, 2);
	plot1D("h_"+vars.at(j)+"_"+"HI",   1, srvector.at(i).GetUpperBound(vars.at(j)), srvector.at(i).srHistMap, "", 1, 0, 2);
      }
    }
    if (SR=="crsl")       plot1D("h_n_mt2bins",  1, srvector.at(i).GetNumberOfMT2Bins(), srvector.at(i).crslHistMap, "", 1, 0, 2);
    else if (SR=="crgj")  plot1D("h_n_mt2bins",  1, srvector.at(i).GetNumberOfMT2Bins(), srvector.at(i).crgjHistMap, "", 1, 0, 2);
    else if (SR=="crrl")  plot1D("h_n_mt2bins",  1, srvector.at(i).GetNumberOfMT2Bins(), srvector.at(i).crrlHistMap, "", 1, 0, 2);
    else                  plot1D("h_n_mt2bins",  1, srvector.at(i).GetNumberOfMT2Bins(), srvector.at(i).srHistMap, "", 1, 0, 2);
    
  }
  outfile_->cd();
  
}


void MT2Looper::loop(TChain* chain, std::string sample, std::string output_dir){

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  gROOT->cd();
  TString output_name = Form("%s/%s.root",output_dir.c_str(),sample.c_str());
  cout << "[MT2Looper::loop] creating output file: " << output_name << endl;

  outfile_ = new TFile(output_name.Data(),"RECREATE") ; 

  const char* json_file = "../babymaker/jsons/Cert_246908-251883_13TeV_PromptReco_Collisions15_JSON_v2_snt.txt";
  if (applyJSON) {
    cout << "Loading json file: " << json_file << endl;
    set_goodrun_file(json_file);
  }

  eventFilter metFilterTxt;
  TString stringsample = sample;
  if (stringsample.Contains("data")) {
    cout<<"Loading bad event files ..."<<endl;
    // updated lists for full dataset
    metFilterTxt.loadBadEventList("/nfs-6/userdata/mt2utils/csc2015_Dec01.txt");
    metFilterTxt.loadBadEventList("/nfs-6/userdata/mt2utils/ecalscn1043093_Dec01.txt");
    cout<<" ... finished!"<<endl;
  }

  h_nvtx_weights_ = 0;
  if (doNvtxReweight) {
    TFile* f_weights = new TFile("../babymaker/data/hists_reweight_zjets_Run2015B.root");
    TH1D* h_nvtx_weights_temp = (TH1D*) f_weights->Get("h_nVert_ratio");
    outfile_->cd();
    h_nvtx_weights_ = (TH1D*) h_nvtx_weights_temp->Clone("h_nvtx_weights");
    h_nvtx_weights_->SetDirectory(0);
    f_weights->Close();
    delete f_weights;
  }

  h_sig_nevents_ = 0;
  h_sig_avgweight_btagsf_ = 0;
  h_sig_avgweight_btagsf_heavy_UP_ = 0;
  h_sig_avgweight_btagsf_light_UP_ = 0;
  h_sig_avgweight_btagsf_heavy_DN_ = 0;
  h_sig_avgweight_btagsf_light_DN_ = 0;
  h_sig_avgweight_isr_ = 0;
  if ((doScanWeights || applyBtagSF) && ((sample.find("T1") != std::string::npos) || (sample.find("T2") != std::string::npos))) {
    std::string scan_name = sample;
    if (sample.find("T1") != std::string::npos) scan_name = sample.substr(0,6);
    else if (sample.find("T2") != std::string::npos) scan_name = sample.substr(0,4);
    TFile* f_nsig_weights = new TFile(Form("../babymaker/data/nsig_weights_%s.root",scan_name.c_str()));
    TH2D* h_sig_nevents_temp = (TH2D*) f_nsig_weights->Get("h_nsig");
    TH2D* h_sig_avgweight_btagsf_temp = (TH2D*) f_nsig_weights->Get("h_avg_weight_btagsf");
    TH2D* h_sig_avgweight_btagsf_heavy_UP_temp = (TH2D*) f_nsig_weights->Get("h_avg_weight_btagsf_heavy_UP");
    TH2D* h_sig_avgweight_btagsf_light_UP_temp = (TH2D*) f_nsig_weights->Get("h_avg_weight_btagsf_light_UP");
    TH2D* h_sig_avgweight_btagsf_heavy_DN_temp = (TH2D*) f_nsig_weights->Get("h_avg_weight_btagsf_heavy_DN");
    TH2D* h_sig_avgweight_btagsf_light_DN_temp = (TH2D*) f_nsig_weights->Get("h_avg_weight_btagsf_light_DN");
    TH2D* h_sig_avgweight_isr_temp = (TH2D*) f_nsig_weights->Get("h_avg_weight_isr");
    h_sig_nevents_ = (TH2D*) h_sig_nevents_temp->Clone("h_sig_nevents");
    h_sig_avgweight_btagsf_ = (TH2D*) h_sig_avgweight_btagsf_temp->Clone("h_sig_avgweight_btagsf");
    h_sig_avgweight_btagsf_heavy_UP_ = (TH2D*) h_sig_avgweight_btagsf_heavy_UP_temp->Clone("h_sig_avgweight_btagsf_heavy_UP");
    h_sig_avgweight_btagsf_light_UP_ = (TH2D*) h_sig_avgweight_btagsf_light_UP_temp->Clone("h_sig_avgweight_btagsf_light_UP");
    h_sig_avgweight_btagsf_heavy_DN_ = (TH2D*) h_sig_avgweight_btagsf_heavy_DN_temp->Clone("h_sig_avgweight_btagsf_heavy_DN");
    h_sig_avgweight_btagsf_light_DN_ = (TH2D*) h_sig_avgweight_btagsf_light_DN_temp->Clone("h_sig_avgweight_btagsf_light_DN");
    h_sig_avgweight_isr_ = (TH2D*) h_sig_avgweight_isr_temp->Clone("h_sig_avgweight_isr");
    h_sig_nevents_->SetDirectory(0);
    h_sig_avgweight_btagsf_->SetDirectory(0);
    h_sig_avgweight_btagsf_heavy_UP_->SetDirectory(0);
    h_sig_avgweight_btagsf_light_UP_->SetDirectory(0);
    h_sig_avgweight_btagsf_heavy_DN_->SetDirectory(0);
    h_sig_avgweight_btagsf_light_DN_->SetDirectory(0);
    h_sig_avgweight_isr_->SetDirectory(0);
    f_nsig_weights->Close();
    delete f_nsig_weights;
  }

  if (doLepEffVars) {
    setElSFfile("../babymaker/lepsf/kinematicBinSFele.root");
    setMuSFfile("../babymaker/lepsf/TnP_MuonID_NUM_LooseID_DENOM_generalTracks_VAR_map_pt_eta.root","../babymaker/lepsf/TnP_MuonID_NUM_MiniIsoTight_DENOM_LooseID_VAR_map_pt_eta.root");
    setVetoEffFile_fullsim("../babymaker/lepsf/vetoeff_emu_etapt_lostlep.root");  
  }
  
  if (applyLeptonSFfastsim && ((sample.find("T1") != std::string::npos) || (sample.find("T2") != std::string::npos))) {
    setElSFfile_fastsim("../babymaker/lepsf/sf_el_vetoCB_mini01.root");  
    setMuSFfile_fastsim("../babymaker/lepsf/sf_mu_looseID_mini02.root");  
    setVetoEffFile_fastsim("../babymaker/lepsf/vetoeff_emu_etapt_T1tttt_mGluino-1500to1525.root");  
  }
  
  cout << "[MT2Looper::loop] setting up histos" << endl;

  SetSignalRegions();

  SRNoCut.SetName("nocut");
  float SRNoCut_mt2bins[8] = {200, 300, 400, 500, 600, 800, 1000, 1500}; 
  SRNoCut.SetMT2Bins(7, SRNoCut_mt2bins);

  // These will be set to true if any SL GJ or DY control region plots are produced
  bool saveGJplots = false;
  bool saveDYplots = false;
  bool saveRLplots = false;
  bool saveRLELplots = false;
  bool saveRLMUplots = false;
  bool saveSLplots = false;
  bool saveSLMUplots = false;
  bool saveSLELplots = false;
  bool saveQCDplots = false;
  bool saveSoftLplots = false;

  // File Loop
  int nDuplicates = 0;
  int nEvents = chain->GetEntries();
  unsigned int nEventsChain = nEvents;
  cout << "[MT2Looper::loop] running on " << nEventsChain << " events" << endl;
  unsigned int nEventsTotal = 0;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;
  while ( (currentFile = (TFile*)fileIter.Next()) ) {
    cout << "[MT2Looper::loop] running on file: " << currentFile->GetTitle() << endl;

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
      //for( unsigned int event = 0; event < 10000; ++event) {
      
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
	else {
	  cout<<i_permille/10.<<" ";
	  if (nEventsTotal%100000==0) cout<<endl;
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
      
      if (t.nVert == 0) continue;

      // MET filters (data and MC)
      if (!t.Flag_goodVertices) continue;
      //if (!t.Flag_CSCTightHaloFilter) continue; // use txt files instead
      if (!t.Flag_eeBadScFilter) continue; // txt files are in addition to this flag
      if (!t.Flag_HBHENoiseFilter) continue;
      if (!t.Flag_HBHEIsoNoiseFilter) continue;
      // txt MET filters (data only)
      if (t.isData && metFilterTxt.eventFails(t.run, t.lumi, t.evt)) {
	//cout<<"Found bad event in data: "<<t.run<<", "<<t.lumi<<", "<<t.evt<<endl;
	continue;
      }

      // Jet ID Veto
      bool passJetID = true;
      //if (t.nJet30FailId > 0) continue;
      if (t.nJet30FailId > 0) passJetID = false;

      // remove low pt QCD samples 
      if (t.evt_id >= 100 && t.evt_id < 109) continue;
      // remove low HT QCD samples 
      if (t.evt_id >= 120 && t.evt_id < 123) continue;
      if (t.evt_id >= 151 && t.evt_id < 154) continue;

      // flag signal samples
      if (t.evt_id >= 1000) isSignal_ = true;
      else isSignal_ = false;

      //if (isSignal_ && (t.GenSusyMScan1 != 1600 || t.GenSusyMScan2 != 0)) continue;
      
      // note: this will double count some leptons, since reco leptons can appear as PFcands
      nlepveto_ = t.nMuons10 + t.nElectrons10 + t.nPFLep5LowMT + t.nPFHad10LowMT;

      //---------------------
      // set weights and start making plots
      //---------------------
      outfile_->cd();
      //      const float lumi = 3;
      //      const float lumi = 0.042;
      //      const float lumi = 0.209;
      //      const float lumi = 0.579;
      //      const float lumi = 1.264;
      const float lumi = 2.115;
      //const float lumi = 4;

      if (isSignal_ 
          && !(t.GenSusyMScan1 == 400 && t.GenSusyMScan2 == 325)
          && !(t.GenSusyMScan1 == 275 && t.GenSusyMScan2 == 200)
	  //&& !(t.GenSusyMScan1 == 900 && t.GenSusyMScan2 == 875 && sample == "T1bbbb_mGluino-875-900-925")
          ) continue;


      evtweight_ = 1.;

      // apply relevant weights to MC
      if (!t.isData) {
	if (isSignal_ && doScanWeights) {
	  int binx = h_sig_nevents_->GetXaxis()->FindBin(t.GenSusyMScan1);
	  int biny = h_sig_nevents_->GetYaxis()->FindBin(t.GenSusyMScan2);
	  double nevents = h_sig_nevents_->GetBinContent(binx,biny);
	  evtweight_ = lumi * t.evt_xsec*1000./nevents; // assumes xsec is already filled correctly
	} else {
	  evtweight_ = t.evt_scale1fb * lumi;
	}
	if (applyBtagSF) {
	  // remove events with 0 btag weight for now..
	  if (fabs(t.weight_btagsf) < 0.001) continue;
	  evtweight_ *= t.weight_btagsf;
	  if (isSignal_) {
	    int binx = h_sig_avgweight_btagsf_->GetXaxis()->FindBin(t.GenSusyMScan1);
	    int biny = h_sig_avgweight_btagsf_->GetYaxis()->FindBin(t.GenSusyMScan2);
	    float avgweight_btagsf = h_sig_avgweight_btagsf_->GetBinContent(binx,biny);
	    evtweight_ /= avgweight_btagsf;
	  }
	}
	// get pu weight from hist, restrict range to nvtx 4-31
	if (doNvtxReweight) {
	  int nvtx_input = t.nVert;
	  if (t.nVert > 31) nvtx_input = 31;
	  if (t.nVert < 4) nvtx_input = 4;
	  float puWeight = h_nvtx_weights_->GetBinContent(h_nvtx_weights_->FindBin(nvtx_input));
	  evtweight_ *= puWeight;
	}
	if (isSignal_ && applyLeptonSFfastsim && nlepveto_ == 0) {
	  fillLepCorSRfastsim();
	  evtweight_ *= (1. + cor_lepeff_sr_);
	}
	else if (doLepEffVars && nlepveto_ == 0) fillLepUncSR();
	if (applyLeptonSF) evtweight_ *= t.weight_lepsf;
	if (applyTopPtReweight && t.evt_id >= 300 && t.evt_id < 400) {
	  evtweight_ *= t.weight_toppt;
	}
      } // !isData

      plot1D("h_nvtx",       t.nVert,       evtweight_, h_1d_global, ";N(vtx)", 80, 0, 80);
      plot1D("h_mt2",       t.mt2,       evtweight_, h_1d_global, ";M_{T2} [GeV]", 80, 0, 800);

      // variables for single lep control region
      bool doSLplots = false;
      bool doSLMUplots = false;
      bool doSLELplots = false;
      leppt_ = -1.;
      mt_ = -1.;

      // count number of forward jets
      nJet30Eta3_ = 0;
      for (int ijet = 0; ijet < t.njet; ++ijet) {
	if (t.jet_pt[ijet] > 30. && fabs(t.jet_eta[ijet]) > 3.0) ++nJet30Eta3_;
      }

      // veto on forward jets
      if (doHFJetVeto && nJet30Eta3_ > 0) continue;

      // check jet id for monojet
      passMonojetId_ = false;
      if (t.nJet30 >= 1) {
	// loop to find central (highest pt) monojet
	for (int ijet = 0; ijet < t.njet; ++ijet) {
	  if (fabs(t.jet_eta[ijet]) > 2.5) continue;
	  if (t.jet_pt[ijet] < 200.) continue;
	  if (t.jet_id[ijet] >= 4) passMonojetId_ = true;
	  break;
	}
      }

      //-------------------------//
      //-------Soft Lepton-------//
      //-------------------------//

      //initialize
      bool doSoftLepSRplots = false;
      bool doSoftLepMuSRplots = false;
      bool doSoftLepElSRplots = false;
      bool doSoftLepCRplots = false;
      bool doSoftLepMuCRplots = false;
      bool doSoftLepElCRplots = false;
      softleppt_ = -1;
      softlepeta_ = -1;
      softlepphi_ = -1;
      softlepM_ = -1;
      softlepId_ = -1;
      softlepElId_ = false;
      softlepmt_ = -1;
      softlepmt2_ = -1;
      softlepDPhiMin_ = 999;

      //loop over isotracks to count PFleptons between 5-10GeV
      int nPFlepLowPt = 0;
      for (int itrk = 0; itrk < t.nisoTrack; ++itrk) {
	float pt = t.isoTrack_pt[itrk];
	if (pt < 5. || pt > 10.) continue;
	int pdgId = t.isoTrack_pdgId[itrk];
	if ((abs(pdgId) != 11) && (abs(pdgId) != 13)) continue;
	if (t.isoTrack_absIso[itrk]/pt > 0.2) continue;


	//overlap removal with reco leps
	bool overlap = false;
	for(int ilep = 0; ilep < t.nlep; ilep++){
	  float thisDR = DeltaR(t.isoTrack_eta[itrk], t.lep_eta[ilep], t.isoTrack_phi[itrk], t.lep_phi[ilep]);
	  if (thisDR < 0.01) {
	    overlap = true;
	    break;
	  }
	} // loop over reco leps
	if (overlap) continue;

	nPFlepLowPt++;
      }
      nUniqueLep_ = t.nMuons10 + t.nElectrons10 + nPFlepLowPt;

      //if nleps==1, find soft lepton
      if (nUniqueLep_ == 1) {
	// find unique lepton to plot pt,MT and get flavor
	bool foundlep = false;
	int cand_pdgId = 0;

	// if reco leps, check those
	if (t.nlep > 0) {
      	  for (int ilep = 0; ilep < t.nlep; ++ilep) {
      	    float mt = sqrt( 2 * t.met_pt * t.lep_pt[ilep] * ( 1 - cos( t.met_phi - t.lep_phi[ilep]) ) );

	    // good candidate: save
	    softleppt_ = t.lep_pt[ilep];
	    softlepeta_ = t.lep_eta[ilep];
	    softlepphi_ = t.lep_phi[ilep];
	    softlepM_ = t.lep_mass[ilep];
	    softlepId_ = t.lep_pdgId[ilep];
	    if ( t.lep_relIso03[ilep]<0.1 // tighter selection for electrons
		 && t.lep_relIso03[ilep]*t.lep_pt[ilep]<5 // tighter selection for electrons
		 && t.lep_tightId[ilep]>2
		 ) 
	      softlepElId_ = true;
	    else softlepElId_ = false;
	    softlepmt_ = mt;
	    cand_pdgId = t.lep_pdgId[ilep];
	    foundlep = true;
	    break;
	  }
	} // t.nlep > 0

	// otherwise check PF leps that don't overlap with a reco lepton
	if (!foundlep /*&& t.nPFLep5LowMT > 0*/) {
      	  for (int itrk = 0; itrk < t.nisoTrack; ++itrk) {
	    float pt = t.isoTrack_pt[itrk];
	    if (pt < 5. || pt > 10.) continue;
      	    int pdgId = t.isoTrack_pdgId[itrk];
	    if ((abs(pdgId) != 11) && (abs(pdgId) != 13)) continue;
      	    if (t.isoTrack_absIso[itrk]/pt > 0.2) continue;
	    float eta = t.isoTrack_eta[itrk];
	    float phi = t.isoTrack_phi[itrk];
	    float mass = t.isoTrack_mass[itrk];
      	    float mt = sqrt( 2 * t.met_pt * pt * ( 1 - cos( t.met_phi - t.isoTrack_phi[itrk]) ) );
	    //if (mt > 100.) continue;

	    // check overlap with reco leptons
	    bool overlap = false;
	    for(int ilep = 0; ilep < t.nlep; ilep++){
	      float thisDR = DeltaR(t.isoTrack_eta[itrk], t.lep_eta[ilep], t.isoTrack_phi[itrk], t.lep_phi[ilep]);
	      if (thisDR < 0.01) {
		overlap = true;
		break;
	      }
	    } // loop over reco leps
	    if (overlap) continue;

	    
	    // good candidate: save
	    softleppt_ = pt;
	    softlepeta_ = eta;
	    softlepphi_ = phi;
	    softlepM_ = mass;
	    softlepId_ = pdgId;
	    softlepElId_ = false;
	    softlepmt_ = mt;
	    cand_pdgId = pdgId;
	    foundlep = true;
	    break;
	  } // loop on isotracks
	} //!foundlep
	    
	if (softleppt_ < 20 && softleppt_ > 5) doSoftLepSRplots = true;
	if (softleppt_ > 30) doSoftLepCRplots = true;
	if (abs(softlepId_) == 13) {
	  if (softleppt_ < 20 && softleppt_ > 5) doSoftLepMuSRplots = true;
	  if (softleppt_ > 30) doSoftLepMuCRplots = true;
	}
	if (abs(softlepId_) == 11) {
	  if (softleppt_ < 20 && softleppt_ > 5) doSoftLepElSRplots = true;
	  if (softleppt_ > 30) doSoftLepElCRplots = true;
	}
	
	if (!foundlep) {
	  std::cout << "MT2Looper::Loop: WARNING! didn't find a lowMT candidate when expected: evt: " << t.evt
		    << ", nMuons10: " << t.nMuons10 << ", nElectrons10: " << t.nElectrons10 
		    << ", nPFLep5LowMT: " << t.nPFLep5LowMT << ", nLepLowMT: " << t.nLepLowMT << std::endl;
	}

      } //nUniqueLep_==1

      
      //recompute dPhiMin for SR/CR
      for(int ijet = 0; ijet < t.njet; ijet++){
	//deltaPhiMin of 4 leading jet objects
	if (ijet < 4) softlepDPhiMin_ = min(softlepDPhiMin_, DeltaPhi( softlepphi_ , t.jet_phi[ijet] ));
      }

      
      //recompute mt2 for softlep CR
      if (doSoftLepCRplots == true && softleppt_ > 200){

	//first make list of jets for hemispheres
	vector<LorentzVector> p4sForHems;
	for(int ijet = 0; ijet < t.njet; ijet++){

	  if (t.jet_pt[ijet] < 30 || fabs(t.jet_eta[ijet]) > 2.5) continue;

	  TLorentzVector jetp4(0,0,0,0);
	  jetp4.SetPtEtaPhiM(t.jet_pt[ijet],t.jet_eta[ijet],t.jet_phi[ijet],t.jet_mass[ijet]);
	  // jetp4.SetPt(t.jet_pt[ijet]);
	  // jetp4.SetEta(t.jet_eta[ijet]);
	  // jetp4.SetPhi(t.jet_phi[ijet]);
	  // jetp4.SetM(t.jet_mass[ijet]);

	  LorentzVector jetp4LV(jetp4.Px(),jetp4.Py(),jetp4.Pz(),jetp4.E());
	  
	  p4sForHems.push_back(jetp4LV);
	}

	//sort list
	std::sort(p4sForHems.begin(), p4sForHems.end(), sortByPt);

	//recalculate mt2 using lep pt as met
	vector<LorentzVector> hemJets;
	if (p4sForHems.size() > 1) {
	  hemJets = getHemJets(p4sForHems);
	  softlepmt2_ = HemMT2(softleppt_, softlepphi_, hemJets.at(0), hemJets.at(1));
	}
      }//softlep recompute CR

      //dR matching for soft-lepton
      bool softlepMatched = false;
      if (doSoftLepSRplots || doSoftLepCRplots){
	double minDR = 999;
	for(int ilep = 0; ilep < t.ngenLep; ilep++){
	  if (t.genLep_pdgId[ilep] != softlepId_) continue;
	  if (t.genLep_pt[ilep] / softleppt_ < 0.5 || t.genLep_pt[ilep] / softleppt_ > 2) continue; 
	  float thisDR = DeltaR(t.genLep_eta[ilep], softlepeta_, t.genLep_phi[ilep], softlepphi_);
	  if (thisDR < minDR) minDR = thisDR;
	}
	if (minDR < 0.1) softlepMatched = true;

	//if still not matched, check genLepFromTau
	if (!softlepMatched){
	  minDR = 999;
	  for(int ilep = 0; ilep < t.ngenLepFromTau; ilep++){
	    if (t.genLepFromTau_pdgId[ilep] != softlepId_) continue;
	    if (t.genLepFromTau_pt[ilep] / softleppt_ < 0.5 || t.genLepFromTau_pt[ilep] / softleppt_ > 2) continue; 
	    float thisDR = DeltaR(t.genLepFromTau_eta[ilep], softlepeta_, t.genLepFromTau_phi[ilep], softlepphi_);
	    if (thisDR < minDR) minDR = thisDR;
	  }
	  if (minDR < 0.1) softlepMatched = true;
	}
      }//if doSoftLep plots

      //-------------------------------------//
      //----------2 lep control region-------//
      //-------------------------------------//
      
      bool doDoubleLepCRplots = false;
      lep1pt_ = -1;
      lep1eta_ = -1;
      lep1phi_ = -1;
      lep1M_ = -1;
      lep2pt_ = -1;
      lep2eta_ = -1;
      lep2phi_ = -1;
      lep2M_ = -1;
      dilepmll_ = -1;
      if (nUniqueLep_ == 2) {
	// find unique lepton to plot pt,MT and get flavor
	int nfoundlep = 0;
	int cand1_pdgId = 0;
	int cand2_pdgId = 0;

	// if reco leps, check those
	if (t.nlep > 0) {
      	  for (int ilep = 0; ilep < t.nlep; ++ilep) {

	    // good candidate: save
	    if (nfoundlep == 0){
	      lep1pt_ = t.lep_pt[ilep];
	      lep1eta_ = t.lep_eta[ilep];
	      lep1phi_ = t.lep_phi[ilep];
	      lep1M_ = t.lep_mass[ilep];
	      cand1_pdgId = t.lep_pdgId[ilep];
	      nfoundlep++;
	      continue;
	    }
	    if (nfoundlep == 1){
	      lep2pt_ = t.lep_pt[ilep];
	      lep2eta_ = t.lep_eta[ilep];
	      lep2phi_ = t.lep_phi[ilep];
	      lep2M_ = t.lep_mass[ilep];
	      cand2_pdgId = t.lep_pdgId[ilep];
	      nfoundlep++;
	      break;
	    }
	  }
	} // t.nlep > 0

	// otherwise check PF leps that don't overlap with a reco lepton
	if (nfoundlep < 2) {
      	  for (int itrk = 0; itrk < t.nisoTrack; ++itrk) {
	    float pt = t.isoTrack_pt[itrk];
	    if (pt < 5. || pt > 10.) continue;
      	    int pdgId = t.isoTrack_pdgId[itrk];
	    if ((abs(pdgId) != 11) && (abs(pdgId) != 13)) continue;
      	    if (t.isoTrack_absIso[itrk]/pt > 0.2) continue;
	    float eta = t.isoTrack_eta[itrk];
	    float phi = t.isoTrack_phi[itrk];
	    float mass = t.isoTrack_mass[itrk];

	    // check overlap with reco leptons
	    bool overlap = false;
	    for(int ilep = 0; ilep < t.nlep; ilep++){
	      float thisDR = DeltaR(t.isoTrack_eta[itrk], t.lep_eta[ilep], t.isoTrack_phi[itrk], t.lep_phi[ilep]);
	      if (thisDR < 0.01) {
		overlap = true;
		break;
	      }
	    } // loop over reco leps
	    if (overlap) continue;

	    
	    // good candidate: save
	    if (nfoundlep == 0){
	      lep1pt_ = pt;
	      lep1eta_ = eta;
	      lep1phi_ = phi;
	      lep1M_ = mass;
	      cand1_pdgId = pdgId;
	      nfoundlep++;
	      continue;
	    }
	    if (nfoundlep == 1){
	      lep2pt_ = pt;
	      lep2eta_ = eta;
	      lep2phi_ = phi;
	      lep2M_ = mass;
	      cand2_pdgId = pdgId;
	      nfoundlep++;
	      break;
	    }
	  } // loop on isotracks	  
	} //nfoundlep < 2

	//at least one soft lepton
	if (lep1pt_ > 20 && lep2pt_ > 20) { continue; }
	
	//opposite sign
	if (((cand1_pdgId < 0) && (cand2_pdgId < 0)) || ((cand1_pdgId > 0) && (cand2_pdgId > 0)) ){ continue; }
	
	
	//construct dilep mass
	TLorentzVector lep1_p4(0,0,0,0);
	TLorentzVector lep2_p4(0,0,0,0);
	lep1_p4.SetPtEtaPhiM(lep1pt_,lep1eta_,lep1phi_,lep1M_);
	lep2_p4.SetPtEtaPhiM(lep2pt_,lep2eta_,lep2phi_,lep2M_);
	TLorentzVector dilep_p4 = lep1_p4 + lep2_p4;
	dilepmll_ = dilep_p4.M();

	//z-window veto
	if (dilepmll_ > 76 && dilepmll_ < 106) { continue; }
	
	//if you get to here, fill in CR
	doDoubleLepCRplots = true;
	
	if (nfoundlep != 2) {
	  std::cout << "MT2Looper::Loop: WARNING! didn't find 2 leptons when expected: evt: " << t.evt
		    << ", nMuons10: " << t.nMuons10 << ", nElectrons10: " << t.nElectrons10 
		    << ", nPFLep5LowMT: " << t.nPFLep5LowMT << ", nLepLowMT: " << t.nLepLowMT << std::endl;
	}
	
      }//nUniqueLep_==2
      
      // simple counter to check for 1L CR
      if (t.nLepLowMT == 1) {
	doSLplots = true;

	// find unique lepton to plot pt,MT and get flavor
	bool foundlep = false;
	int cand_pdgId = 0;

	// if reco leps, check those
	if (t.nlep > 0) {
      	  for (int ilep = 0; ilep < t.nlep; ++ilep) {
      	    float mt = sqrt( 2 * t.met_pt * t.lep_pt[ilep] * ( 1 - cos( t.met_phi - t.lep_phi[ilep]) ) );
	    if (mt > 100.) continue;

	    // good candidate: save
	    leppt_ = t.lep_pt[ilep];
	    mt_ = mt;
	    cand_pdgId = t.lep_pdgId[ilep];
	    foundlep = true;
	    break;
	  }
	} // t.nlep > 0

	// otherwise check PF leps that don't overlap with a reco lepton
	if (!foundlep && t.nPFLep5LowMT > 0) {
      	  for (int itrk = 0; itrk < t.nisoTrack; ++itrk) {
	    float pt = t.isoTrack_pt[itrk];
	    if (pt < 5.) continue;
      	    int pdgId = t.isoTrack_pdgId[itrk];
	    if ((abs(pdgId) != 11) && (abs(pdgId) != 13)) continue;
      	    if (t.isoTrack_absIso[itrk]/pt > 0.2) continue;
      	    float mt = sqrt( 2 * t.met_pt * pt * ( 1 - cos( t.met_phi - t.isoTrack_phi[itrk]) ) );
	    if (mt > 100.) continue;

	    // check overlap with reco leptons
	    bool overlap = false;
	    for(int ilep = 0; ilep < t.nlep; ilep++){
	      float thisDR = DeltaR(t.isoTrack_eta[itrk], t.lep_eta[ilep], t.isoTrack_phi[itrk], t.lep_phi[ilep]);
	      if (thisDR < 0.01) {
		overlap = true;
		break;
	      }
	    } // loop over reco leps
	    if (overlap) continue;

	    // good candidate: save
	    leppt_ = pt;
	    mt_ = mt;
	    cand_pdgId = pdgId;
	    foundlep = true;
	    break;
	  } // loop on isotracks
	}

	if (!foundlep) {
	  std::cout << "MT2Looper::Loop: WARNING! didn't find a lowMT candidate when expected: evt: " << t.evt
		    << ", nMuons10: " << t.nMuons10 << ", nElectrons10: " << t.nElectrons10 
		    << ", nPFLep5LowMT: " << t.nPFLep5LowMT << ", nLepLowMT: " << t.nLepLowMT << std::endl;
	}

	if (abs(cand_pdgId) == 11) doSLELplots = true;
	else if (abs(cand_pdgId) == 13) doSLMUplots = true;

      } // for 1L control region

      // Variables for gamma+jets control region
      bool doGJplots = false;
      if (t.ngamma > 0) {
	if (t.isData) {
	  doGJplots = true;
	}
	else if (!useDRforGammaQCDMixing) {
	  if ( (t.evt_id < 200 && t.gamma_mcMatchId[0]>0  && t.gamma_genIso04[0]<5)    // Reject true photons from QCD (iso is always 0 for now)
	       || (t.evt_id >=200 && t.gamma_mcMatchId[0]==0 )                           // Reject unmatched photons from Gamma+Jets
	       || (t.evt_id >=200 && t.gamma_mcMatchId[0] >0 && t.gamma_genIso04[0]>5)    // Reject non-iso photons from Gamma+Jets
	       )
	    { doGJplots = false; }
	  else {
	    doGJplots = true;
	  }	    
	}
	else {
	  if ( (t.evt_id < 200 && t.gamma_mcMatchId[0]>0 && t.gamma_drMinParton[0] > 0.4)   // reject DR>0.4 photons from QCD
	       || (t.evt_id >=200 && t.gamma_mcMatchId[0]==0 )   // Reject unmatched photons from Gamma+Jets (should be small)    
	       || (t.evt_id >=200 && t.gamma_drMinParton[0]<0.4 )   // Reject fragmentation photons from Gamma+Jets (only there in samples with DR>0.05)
	       || (t.evt_id >=300) || (t.evt_id < 0) || (isSignal_) // Don't make plots for other samples
	       )
	    { doGJplots = false; }
	  else {
	    doGJplots = true;
	  }
	} //useDRforGammaQCDMixing
      } // ngamma > 0

      // Variables for Zll (DY) control region
      bool doDYplots = false;
      if (t.nlep == 2 && !isSignal_) {
	if ( (t.lep_charge[0] * t.lep_charge[1] == -1)
	     && (abs(t.lep_pdgId[0]) == abs(t.lep_pdgId[1]) )
             && (abs(t.lep_pdgId[0]) == 13 ||  t.lep_tightId[0] > 0 )
             && (abs(t.lep_pdgId[1]) == 13 ||  t.lep_tightId[1] > 0 )
	     && (fabs(t.zll_mass - 90) < 10 ) 
	     && t.lep_pt[0] > 25 && t.lep_pt[1] > 20
	     && (t.HLT_DoubleEl || t.HLT_DoubleMu || t.HLT_Photon165_HE10)
	     ) {
	  // no additional explicit lepton veto
	  // i.e. implicitly allow 3rd PF lepton or hadron
	  //nlepveto_ = 0; 
	  doDYplots = true;
	}
      } // nlep == 2
      
      // Variables for Removed single lepton (RL) region
      //muon only
      bool doRLplots = false;
      bool doRLMUplots = false;
      bool doRLELplots = false;
      if ( t.nlep == 1 && !isSignal_) {
	if ( t.lep_pt[0] > 30 && fabs(t.lep_eta[0])<2.5 && t.nBJet20 == 0) { // raise threshold to avoid Ele23 in MC
	  if (abs(t.lep_pdgId[0])==13) { // muons
	    if (t.HLT_SingleMu)  doRLMUplots = true;
	  }
	  if (abs(t.lep_pdgId[0])==11) { // electrons
	    if ( (t.HLT_SingleEl )   // Ele23 trigger not present in MC. Need to keep lepton threshold high
		 && t.lep_relIso03[0]<0.1 // tighter selection for electrons
		 && t.lep_relIso03[0]*t.lep_pt[0]<5 // tighter selection for electrons
		 && t.lep_tightId[0]>2
		 ) 
	      doRLELplots = true;
	  }
	  // no additional explicit lepton veto
	  // i.e. implicitly allow 2nd PF lepton or hadron
	  //nlepveto_ = 0; 
	  if (doRLMUplots || doRLELplots )  doRLplots = true;
	}
      } // nlep == 1

      // check for failing minDphi, for QCD CR
      bool doQCDplots = false;
      if (t.nJet30 >= 2 && t.deltaPhiMin < 0.3) doQCDplots = true;

      ////////////////////////////////////
      /// done with overall selection  /// 
      ////////////////////////////////////
      ///   time to fill histograms    /// 
      ////////////////////////////////////

      if (doGJplots) {
        saveGJplots = true;
	if (t.gamma_nJet30FailId == 0) {

	  //// To test Madgraph fragmentation (need to remove drMinParton requirement from above) //
	  //if ( t.gamma_mcMatchId[0] > 0 ) {
	  //  if (t.evt_id < 200 || t.gamma_drMinParton[0]>0.4) fillHistosCRGJ("crgj"); // Prompt photon
	  //  if (t.evt_id >=200 && t.gamma_drMinParton[0]<0.4)  fillHistosCRGJ("crgj", "FragGJ");
	  //  if (t.evt_id < 200 && t.gamma_drMinParton[0]<0.05) fillHistosCRGJ("crgj", "FragGJ");
	  //}
	  //// End of Madgraph fragmentation tests //
	  
	  if ( t.gamma_mcMatchId[0] > 0 || t.isData) fillHistosCRGJ("crgj"); // Prompt photon 
	  else fillHistosCRGJ("crgj", "Fake");
	}
      }
      
      if (!passJetID) continue;
      
      if ( !(t.isData && doBlindData && t.mt2 > 200) ) {
	fillHistos(SRNoCut.srHistMap, SRNoCut.GetNumberOfMT2Bins(), SRNoCut.GetMT2Bins(), SRNoCut.GetName(), "");

	fillHistosSignalRegion("sr");

	fillHistosSRBase();
	fillHistosInclusive();
      }

      // if (doDYplots) {
      //   saveDYplots = true;
      //   fillHistosCRDY("crdy");
      // }
      // if (doRLplots) {
      //   saveRLplots = true;
      //   fillHistosCRRL("crrl");
      // }
      // if (doRLELplots && !doMinimalPlots) {
      //   saveRLELplots = true;
      //   fillHistosCRRL("crrlel");
      // }
      // if (doRLMUplots && !doMinimalPlots) {
      //   saveRLMUplots = true;
      //   fillHistosCRRL("crrlmu");
      // }
      // if (doSLplots) {
      //   saveSLplots = true;
      //   fillHistosCRSL("crsl");
      // }
      // if (doSLMUplots && !doMinimalPlots) {
      //   saveSLMUplots = true;
      //   fillHistosCRSL("crslmu");
      // }
      // if (doSLELplots && !doMinimalPlots) {
      //   saveSLELplots = true;
      //   fillHistosCRSL("crslel");
      // }
      // if (doQCDplots) {
      //   saveQCDplots = true;
      //   fillHistosCRQCD("crqcd");
      // }
      
      if (doSoftLepSRplots) {
        //saveSoftLplots = true;
        if (softlepMatched) fillHistosSoftL("srsoftl");
	else fillHistosSoftL("srsoftl", "Fake");
        fillHistosLepSignalRegions("srLep");
      }
      if (doSoftLepMuSRplots && !doMinimalPlots) {
        //saveSoftLplots = true;
        if (softlepMatched) fillHistosSoftL("srsoftlmu");
	else  fillHistosSoftL("srsoftlmu", "Fake");
      }
      if (doSoftLepElSRplots && !doMinimalPlots) {
        //saveSoftLplots = true;
        if (softlepMatched) fillHistosSoftL("srsoftlel");
	else fillHistosSoftL("srsoftlel", "Fake");
      }
      if (doSoftLepCRplots) {
        //saveSoftLplots = true;
        if (softlepMatched) fillHistosSoftL("crsoftl");
	else fillHistosSoftL("crsoftl","Fake");
      }
      if (doSoftLepMuCRplots && !doMinimalPlots) {
        //saveSoftLplots = true;
         if (softlepMatched) fillHistosSoftL("crsoftlmu");
	 else fillHistosSoftL("crsoftlmu","Fake");
      }
      if (doSoftLepElCRplots && !doMinimalPlots) {
        //saveSoftLplots = true;
         if (softlepMatched) fillHistosSoftL("crsoftlel");
	 else fillHistosSoftL("crsoftlel","Fake");
      }
      if (doDoubleLepCRplots) {
        //saveSoftLplots = true;
         fillHistosDoubleL("crdoublel");
      }


   }//end loop on events in a file
  
    delete tree;
    f.Close();
  }//end loop on files
  
  cout << "[MT2Looper::loop] processed " << nEventsTotal << " events" << endl;
  if ( nEventsChain != nEventsTotal ) {
    std::cout << "ERROR: number of events from files is not equal to total number of events" << std::endl;
  }

  cout << nDuplicates << " duplicate events were skipped." << endl;

  //---------------------
  // Save Plots
  //---------------------

  outfile_->cd();
  savePlotsDir(h_1d_global,outfile_,"");
  savePlotsDir(SRNoCut.srHistMap,outfile_,SRNoCut.GetName().c_str());
  savePlotsDir(SRBase.srHistMap,outfile_,SRBase.GetName().c_str());
  savePlotsDir(SRBase.crslHistMap,outfile_,"crslbase");
  savePlotsDir(SRBase.crrlHistMap,outfile_,"crrlbase");
  if (!doMinimalPlots) {
    savePlotsDir(CRSL_WJets.crslHistMap,outfile_,CRSL_WJets.GetName().c_str());
    savePlotsDir(CRSL_TTbar.crslHistMap,outfile_,CRSL_TTbar.GetName().c_str());
  }
  savePlotsDir(SRNoCut.crgjHistMap,outfile_,"crgjnocut");
  savePlotsDir(SRBase.crgjHistMap,outfile_,"crgjbase");

  //softlepton
  savePlotsDir(SRBase.crloosesoftlHistMap,outfile_,"crloosesoftl");
  savePlotsDir(SRBase.crloosesoftlmuHistMap,outfile_,"crloosesoftlmu");
  savePlotsDir(SRBase.crloosesoftlelHistMap,outfile_,"crloosesoftlel");
  savePlotsDir(SRBase.crsoftlHistMap,outfile_,"crsoftlbase");
  savePlotsDir(SRBase.crsoftlmuHistMap,outfile_,"crsoftlmubase");
  savePlotsDir(SRBase.crsoftlelHistMap,outfile_,"crsoftlelbase");
  savePlotsDir(SRBase.crsoftlLowMtHistMap,outfile_,"crsoftlLowMt");
  savePlotsDir(SRBase.crsoftlmuLowMtHistMap,outfile_,"crsoftlmuLowMt");
  savePlotsDir(SRBase.crsoftlelLowMtHistMap,outfile_,"crsoftlelLowMt");
  savePlotsDir(SRBase.crsoftlHighMtHistMap,outfile_,"crsoftlHighMt");
  savePlotsDir(SRBase.crsoftlmuHighMtHistMap,outfile_,"crsoftlmuHighMt");
  savePlotsDir(SRBase.crsoftlelHighMtHistMap,outfile_,"crsoftlelHighMt");
  savePlotsDir(SRBase.srsoftlHistMap,outfile_,"srsoftlbase");
  savePlotsDir(SRBase.srsoftlmuHistMap,outfile_,"srsoftlmubase");
  savePlotsDir(SRBase.srsoftlelHistMap,outfile_,"srsoftlelbase");
  savePlotsDir(SRBase.srsoftlLowMtHistMap,outfile_,"srsoftlLowMt");
  savePlotsDir(SRBase.srsoftlmuLowMtHistMap,outfile_,"srsoftlmuLowMt");
  savePlotsDir(SRBase.srsoftlelLowMtHistMap,outfile_,"srsoftlelLowMt");
  savePlotsDir(SRBase.srsoftlHighMtHistMap,outfile_,"srsoftlHighMt");
  savePlotsDir(SRBase.srsoftlmuHighMtHistMap,outfile_,"srsoftlmuHighMt");
  savePlotsDir(SRBase.srsoftlelHighMtHistMap,outfile_,"srsoftlelHighMt");
  savePlotsDir(SRBase.crdoublelHistMap,outfile_,"crdoublelbase");
  
  for(unsigned int srN = 0; srN < SRVec.size(); srN++){
    if(!SRVec.at(srN).srHistMap.empty()){
      savePlotsDir(SRVec.at(srN).srHistMap, outfile_, ("sr"+SRVec.at(srN).GetName()).c_str());
    }
  }
  if (saveGJplots) {
    for(unsigned int srN = 0; srN < SRVec.size(); srN++){
      if(!SRVec.at(srN).crgjHistMap.empty()){
        savePlotsDir(SRVec.at(srN).crgjHistMap, outfile_, ("crgj"+SRVec.at(srN).GetName()).c_str());
	saveRooDataSetsDir(SRVec.at(srN).crgjRooDataSetMap, outfile_,  ("crgj"+SRVec.at(srN).GetName()).c_str());
      }
    }
  }
  if (saveDYplots) {
    for(unsigned int srN = 0; srN < SRVec.size(); srN++){
      if(!SRVec.at(srN).crdyHistMap.empty()){
        savePlotsDir(SRVec.at(srN).crdyHistMap, outfile_, ("crdy"+SRVec.at(srN).GetName()).c_str());
      }
    }
  }
  if (saveRLplots) {
    for(unsigned int srN = 0; srN < SRVec.size(); srN++){
      if(!SRVec.at(srN).crrlHistMap.empty()){
        savePlotsDir(SRVec.at(srN).crrlHistMap, outfile_, ("crrl"+SRVec.at(srN).GetName()).c_str());
      }
    }
  }
  if (saveRLELplots) {
    for(unsigned int srN = 0; srN < SRVec.size(); srN++){
      if(!SRVec.at(srN).crrlelHistMap.empty()){
        savePlotsDir(SRVec.at(srN).crrlelHistMap, outfile_, ("crrlel"+SRVec.at(srN).GetName()).c_str());
      }
    }
  }
  if (saveRLMUplots) {
    for(unsigned int srN = 0; srN < SRVec.size(); srN++){
      if(!SRVec.at(srN).crrlmuHistMap.empty()){
        savePlotsDir(SRVec.at(srN).crrlmuHistMap, outfile_, ("crrlmu"+SRVec.at(srN).GetName()).c_str());
      }
    }
  }
  if (saveSLplots) {
    for(unsigned int srN = 0; srN < SRVec.size(); srN++){
      if(!SRVec.at(srN).crslHistMap.empty()){
        savePlotsDir(SRVec.at(srN).crslHistMap, outfile_, ("crsl"+SRVec.at(srN).GetName()).c_str());
      }
    }
  }
  if (saveSLMUplots) {
    for(unsigned int srN = 0; srN < SRVec.size(); srN++){
      if(!SRVec.at(srN).crslmuHistMap.empty()){
        savePlotsDir(SRVec.at(srN).crslmuHistMap, outfile_, ("crslmu"+SRVec.at(srN).GetName()).c_str());
      }
    }
  }
  if (saveSLELplots) {
    for(unsigned int srN = 0; srN < SRVec.size(); srN++){
      if(!SRVec.at(srN).crslelHistMap.empty()){
        savePlotsDir(SRVec.at(srN).crslelHistMap, outfile_, ("crslel"+SRVec.at(srN).GetName()).c_str());
      }
    }
  }
  if (saveQCDplots) {
    for(unsigned int srN = 0; srN < SRVec.size(); srN++){
      if(!SRVec.at(srN).crqcdHistMap.empty()){
        savePlotsDir(SRVec.at(srN).crqcdHistMap, outfile_, ("crqcd"+SRVec.at(srN).GetName()).c_str());
      }
    }
  }
  if (saveSoftLplots) {
    for(unsigned int srN = 0; srN < SRVecLep.size(); srN++){
      if(!SRVecLep.at(srN).srHistMap.empty()){
	cout<<"Saving srLep"<< SRVecLep.at(srN).GetName() <<endl;
        savePlotsDir(SRVecLep.at(srN).srHistMap, outfile_, ("srLep"+SRVecLep.at(srN).GetName()).c_str());
      }
    }
  }

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

void MT2Looper::fillHistosSRBase() {

  // trigger requirement on data
  if (t.isData && !(t.HLT_PFHT800 || t.HLT_PFHT350_PFMET100 || t.HLT_PFMETNoMu90_PFMHTNoMu90)) return;

  std::map<std::string, float> values;
  values["deltaPhiMin"] = t.deltaPhiMin;
  values["diffMetMhtOverMet"]  = t.diffMetMht/t.met_pt;
  values["nlep"]        = nlepveto_;
  values["j1pt"]        = t.jet1_pt;
  values["j2pt"]        = t.jet2_pt;
  values["mt2"]         = t.mt2;
  values["passesHtMet"] = ( (t.ht > 200. && t.met_pt > 200.) || (t.ht > 1000. && t.met_pt > 30.) );

  if(SRBase.PassesSelection(values)) fillHistos(SRBase.srHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), SRBase.GetName(), "");

  // do monojet SRs
  bool passMonojet = false;
  if (passMonojetId_ && (!t.isData || t.HLT_PFMETNoMu90_PFMHTNoMu90 || t.HLT_PFHT350_PFMET100)) {
    std::map<std::string, float> values_monojet;
    values_monojet["deltaPhiMin"] = t.deltaPhiMin;
    values_monojet["diffMetMhtOverMet"]  = t.diffMetMht/t.met_pt;
    values_monojet["nlep"]        = nlepveto_;
    values_monojet["j1pt"]        = t.jet1_pt;
    values_monojet["njets"]       = t.nJet30;
    values_monojet["met"]         = t.met_pt;
    
    if(SRBaseMonojet.PassesSelection(values_monojet)) passMonojet = true;
    if (passMonojet) fillHistos(SRBaseMonojet.srHistMap, SRBaseMonojet.GetNumberOfMT2Bins(), SRBaseMonojet.GetMT2Bins(), SRBaseMonojet.GetName(), "");
  }
  if ((SRBase.PassesSelection(values)) || (passMonojet)) {
    fillHistos(SRBaseIncl.srHistMap, SRBaseIncl.GetNumberOfMT2Bins(), SRBaseIncl.GetMT2Bins(), SRBaseIncl.GetName(), "");
  }


  return;
}

void MT2Looper::fillHistosInclusive() {

  // trigger requirement on data
  if (t.isData && !(t.HLT_PFHT800 || t.HLT_PFHT350_PFMET100)) return;
  
  std::map<std::string, float> values;
  values["deltaPhiMin"] = t.deltaPhiMin;
  values["diffMetMhtOverMet"]  = t.diffMetMht/t.met_pt;
  values["nlep"]        = nlepveto_;
  values["j1pt"]        = t.jet1_pt;
  values["j2pt"]        = t.jet2_pt;
  values["mt2"]         = t.mt2;
  values["passesHtMet"] = ( (t.ht > 200. && t.met_pt > 200.) || (t.ht > 1000. && t.met_pt > 30.) );

  for(unsigned int srN = 0; srN < InclusiveRegions.size(); srN++){
    std::map<std::string, float> values_temp = values;
    std::vector<std::string> vars = InclusiveRegions.at(srN).GetListOfVariables();
    for(unsigned int iVar=0; iVar<vars.size(); iVar++){
      if(vars.at(iVar) == "ht") values_temp["ht"] = t.ht;
      else if(vars.at(iVar) == "njets") values_temp["njets"] = t.nJet30;
      else if(vars.at(iVar) == "nbjets") values_temp["nbjets"] = t.nBJet20;
    }
    if(InclusiveRegions.at(srN).PassesSelection(values_temp)){
      fillHistos(InclusiveRegions.at(srN).srHistMap, InclusiveRegions.at(srN).GetNumberOfMT2Bins(), InclusiveRegions.at(srN).GetMT2Bins(), InclusiveRegions.at(srN).GetName(), "");
    }
  }

  return;
}

void MT2Looper::fillHistosSignalRegion(const std::string& prefix, const std::string& suffix) {

  // trigger requirement on data
  if (t.isData && !(t.HLT_PFHT800 || t.HLT_PFHT350_PFMET100 || t.HLT_PFMETNoMu90_PFMHTNoMu90)) return;
  
  std::map<std::string, float> values;
  values["deltaPhiMin"] = t.deltaPhiMin;
  values["diffMetMhtOverMet"]  = t.diffMetMht/t.met_pt;
  values["nlep"]        = nlepveto_;
  values["j1pt"]        = t.jet1_pt;
  values["j2pt"]        = t.jet2_pt;
  values["njets"]       = t.nJet30;
  values["nbjets"]      = t.nBJet20;
  values["mt2"]         = t.mt2;
  values["ht"]          = t.ht;
  values["met"]         = t.met_pt;
  //values["passesHtMet"] = ( (t.ht > 200. && t.met_pt > 200.) || (t.ht > 1000. && t.met_pt > 30.) );


  for(unsigned int srN = 0; srN < SRVec.size(); srN++){
    if(SRVec.at(srN).PassesSelection(values)){
      fillHistos(SRVec.at(srN).srHistMap, SRVec.at(srN).GetNumberOfMT2Bins(), SRVec.at(srN).GetMT2Bins(), prefix+SRVec.at(srN).GetName(), suffix);
      break;//signal regions are orthogonal, event cannot be in more than one
    }
  }
  
  // do monojet SRs
  if (passMonojetId_ && (!t.isData || t.HLT_PFMETNoMu90_PFMHTNoMu90 || t.HLT_PFHT350_PFMET100)) {
    std::map<std::string, float> values_monojet;
    values_monojet["deltaPhiMin"] = t.deltaPhiMin;
    values_monojet["diffMetMhtOverMet"]  = t.diffMetMht/t.met_pt;
    values_monojet["nlep"]        = nlepveto_;
    values_monojet["j1pt"]        = t.jet1_pt;
    values_monojet["njets"]       = t.nJet30;
    values_monojet["nbjets"]      = t.nBJet20;
    values_monojet["ht"]          = t.ht;
    values_monojet["met"]         = t.met_pt;

    for(unsigned int srN = 0; srN < SRVecMonojet.size(); srN++){
      if(SRVecMonojet.at(srN).PassesSelection(values_monojet)){
	fillHistos(SRVecMonojet.at(srN).srHistMap, SRVecMonojet.at(srN).GetNumberOfMT2Bins(), SRVecMonojet.at(srN).GetMT2Bins(), prefix+SRVecMonojet.at(srN).GetName(), suffix);
	//break;//signal regions are orthogonal, event cannot be in more than one --> not true for Monojet, since we have inclusive regions
      }
    }
  } // monojet regions

  return;
}

//hists for soft lepton regions
void MT2Looper::fillHistosLepSignalRegions(const std::string& prefix, const std::string& suffix) {

  // trigger requirement on data
  if (t.isData && !(t.HLT_PFHT800 || t.HLT_PFHT350_PFMET100 || t.HLT_PFMETNoMu90_PFMHTNoMu90)) return;

  std::map<std::string, float> values;
  values["deltaPhiMin"] = softlepDPhiMin_;
  values["diffMetMhtOverMet"]  = t.diffMetMht/t.met_pt;
  values["nlep"]        = nUniqueLep_;
  values["njets"]       = t.nJet30;
  values["nbjets"]      = t.nBJet20;
  values["mt2"]         = t.nJet30 > 1 ? t.mt2 : t.met_pt; // require large MT2 for multijet events
  values["ht"]          = t.ht;

  for(unsigned int srN = 0; srN < SRVecLep.size(); srN++){
    if(SRVecLep.at(srN).PassesSelection(values)){
      fillHistosSingleSoftLepton(SRVecLep.at(srN).srHistMap, SRVecLep.at(srN).GetNumberOfMT2Bins(), SRVecLep.at(srN).GetMT2Bins(), prefix+SRVecLep.at(srN).GetName(), suffix);
      break;//signal regions are orthogonal, event cannot be in more than one
    }
  }

}

//hists for soft lepton regions
void MT2Looper::fillHistosSoftL(const std::string& prefix, const std::string& suffix) {

  //cuts for wjets loose CR
  bool passSoftLepLoose = true;
  //trigger
  if (!t.HLT_SingleMu && !t.HLT_SingleEl) passSoftLepLoose = false;
  //tighter electron id
  if (abs(softlepId_) == 11 && !softlepElId_) passSoftLepLoose = false;
  //min ht cut
  if (t.ht < 200) passSoftLepLoose = false;
  //inverted lepton pT
  if (softleppt_ < 30) passSoftLepLoose = false;
  //inverted MT cut
  if (softlepmt_ > 100) passSoftLepLoose = false;
  //met cut
  if (t.met_pt < 50) passSoftLepLoose = false;
  //b veto
  if (t.nBJet20 != 0) passSoftLepLoose = false;

  //fill for soft lep loose CR cuts
  if (passSoftLepLoose) {
    if(prefix=="crsoftl") fillHistosSingleSoftLepton(SRBase.crloosesoftlHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crloosesoftl", suffix);
    else if(prefix=="crsoftlmu") fillHistosSingleSoftLepton(SRBase.crloosesoftlmuHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crloosesoftlmu", suffix);
    else if(prefix=="crsoftlel") fillHistosSingleSoftLepton(SRBase.crloosesoftlelHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crloosesoftlel", suffix);
  }
  
  // trigger requirement on data
  //if (t.isData && !(t.HLT_PFHT800 || t.HLT_PFHT350_PFMET100)) return;

  bool passBaseline = false;
  if (t.ht > 200 &&
      t.mt2 > 200 &&
      //t.nBJet20 < 2 &&
      t.nJet30 >= 2 &&
      t.met_pt > 200 &&
      softlepDPhiMin_ > 0.3 && //deltaPhiMin compute only with jet objects
      //t.deltaPhiMin > 0.3 &&
      //t.rl_deltaPhiMin > 0.3 &&
      t.diffMetMht/t.met_pt < 0.5 
      ) passBaseline = true;

  if (passBaseline) {
    if(prefix=="srsoftl") fillHistosSingleSoftLepton(SRBase.srsoftlHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "srsoftlbase", suffix);
    else if(prefix=="srsoftlmu") fillHistosSingleSoftLepton(SRBase.srsoftlmuHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "srsoftlmubase", suffix);
    else if(prefix=="srsoftlel") fillHistosSingleSoftLepton(SRBase.srsoftlelHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "srsoftlelbase", suffix);
  }

  bool passSRLowMt = false;
  bool passSRHighMt = false;
  if (passBaseline && softlepmt_ < 120 && softlepmt_ > 90 && t.nJet30 > 5 && t.nBJet20 == 0 && t.mt2 > 200) passSRLowMt = true;
  if (passBaseline && softlepmt_ > 120 && t.nJet30 > 5 && t.nBJet20 == 0 && t.mt2 > 200) passSRHighMt = true;

  if (passSRLowMt) { 
    //cout << "test sr low mt" << endl;
    if(prefix=="srsoftl") fillHistosSingleSoftLepton(SRBase.srsoftlLowMtHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "srsoftlLowMt", suffix);
    else if(prefix=="srsoftlmu") fillHistosSingleSoftLepton(SRBase.srsoftlmuLowMtHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "srsoftlmuLowMt", suffix);
    else if(prefix=="srsoftlel") fillHistosSingleSoftLepton(SRBase.srsoftlelLowMtHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "srsoftlelLowMt", suffix);
  }
  if (passSRHighMt) {
    //cout << "test sr high mt" << endl;
    if(prefix=="srsoftl") fillHistosSingleSoftLepton(SRBase.srsoftlHighMtHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "srsoftlHighMt", suffix);
    else if(prefix=="srsoftlmu") fillHistosSingleSoftLepton(SRBase.srsoftlmuHighMtHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "srsoftlmuHighMt", suffix);
    else if(prefix=="srsoftlel") fillHistosSingleSoftLepton(SRBase.srsoftlelHighMtHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "srsoftlelHighMt", suffix);
  }

  bool passCRBaseline = false;
  if (t.ht > 200 &&
      t.met_pt < 100 && //upper bound on met to constrain W pt
      softlepmt2_ > 200 &&
      //t.nBJet20 < 2 &&
      t.nJet30 >= 2 &&
      softleppt_ > 200 && //replace MET with lepton pt
      softlepDPhiMin_ > 0.3 //deltaPhiMin compute only with jet objects
      //t.deltaPhiMin > 0.3
      //t.rl_deltaPhiMin > 0.3 &&
      //t.diffMetMht/t.met_pt < 0.5
      ) passCRBaseline = true;

  if (passCRBaseline) {
    if(prefix=="crsoftl") fillHistosSingleSoftLepton(SRBase.crsoftlHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crsoftlbase", suffix);
    else if(prefix=="crsoftlmu") fillHistosSingleSoftLepton(SRBase.crsoftlmuHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crsoftlmubase", suffix);
    else if(prefix=="crsoftlel") fillHistosSingleSoftLepton(SRBase.crsoftlelHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crsoftlelbase", suffix);
  }
  
  bool passCRLowMt = false;
  bool passCRHighMt = false;
  if (passCRBaseline && softlepmt_ < 120 && softlepmt_ > 90 && t.nJet30 > 5 && t.nBJet20 == 0 /*&& t.mt2 > 200*/) passCRLowMt = true;
  if (passCRBaseline && softlepmt_ > 120 && t.nJet30 > 5 && t.nBJet20 == 0 /*&& t.mt2 > 200*/) passCRHighMt = true;

  if (passCRLowMt) {
    //cout << "test cr low mt" << endl;
    if(prefix=="crsoftl") fillHistosSingleSoftLepton(SRBase.crsoftlLowMtHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crsoftlLowMt", suffix);
    else if(prefix=="crsoftlmu") fillHistosSingleSoftLepton(SRBase.crsoftlmuLowMtHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crsoftlmuLowMt", suffix);
    else if(prefix=="crsoftlel") fillHistosSingleSoftLepton(SRBase.crsoftlelLowMtHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crsoftlelLowMt", suffix);
  }
  if (passCRHighMt) {
    //cout << "test cr low mt" << endl;
    if(prefix=="crsoftl") fillHistosSingleSoftLepton(SRBase.crsoftlHighMtHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crsoftlHighMt", suffix);
    else if(prefix=="crsoftlmu") fillHistosSingleSoftLepton(SRBase.crsoftlmuHighMtHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crsoftlmuHighMt", suffix);
    else if(prefix=="crsoftlel") fillHistosSingleSoftLepton(SRBase.crsoftlelHighMtHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crsoftlelHighMt", suffix);
  }
 
  return;
} //soft lepton


void MT2Looper::fillHistosDoubleL(const std::string& prefix, const std::string& suffix) {

  bool passBaseline = false;
  if (t.ht > 200 &&
      t.mt2 > 200 &&
      //t.nBJet20 < 2 &&
      t.nJet30 >= 2 &&
      t.met_pt > 200 &&
      softlepDPhiMin_ > 0.3 && //deltaPhiMin compute only with jet objects
      //t.deltaPhiMin > 0.3 &&
      //t.rl_deltaPhiMin > 0.3 &&
      t.diffMetMht/t.met_pt < 0.5 
      ) passBaseline = true;

  
  if (passBaseline) fillHistosDoubleLepton(SRBase.crdoublelHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crdoublelbase", suffix);
 
  return;
} //double lep

// hists for single lepton control region
void MT2Looper::fillHistosCRSL(const std::string& prefix, const std::string& suffix) {

  // trigger requirement on data
  if (t.isData && !(t.HLT_PFHT800 || t.HLT_PFHT350_PFMET100 || t.HLT_PFMETNoMu90_PFMHTNoMu90)) return;
  
  // first fill base region
  std::map<std::string, float> valuesBase;
  valuesBase["deltaPhiMin"] = t.deltaPhiMin;
  valuesBase["diffMetMhtOverMet"]  = t.diffMetMht/t.met_pt;
  valuesBase["nlep"]        = t.nLepLowMT;
  valuesBase["j1pt"]        = t.jet1_pt;
  valuesBase["j2pt"]        = t.jet2_pt;
  valuesBase["mt2"]         = t.mt2;
  valuesBase["passesHtMet"] = ( (t.ht > 200. && t.met_pt > 200.) || (t.ht > 1000. && t.met_pt > 30.) );

  if (SRBase.PassesSelectionCRSL(valuesBase)) {
    if(prefix=="crsl") fillHistosSingleLepton(SRBase.crslHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crslbase", suffix);
    else if(prefix=="crslmu") fillHistosSingleLepton(SRBase.crslmuHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crslmubase", suffix);
    else if(prefix=="crslel") fillHistosSingleLepton(SRBase.crslelHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crslelbase", suffix);

    // only fill inclusive HT regions for inclusive lepton case
    if (prefix=="crsl") {
      if(t.ht > 200.  && t.ht < 450.)  fillHistosSingleLepton(InclusiveRegions.at(0).crslHistMap,   SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crslbaseVL", suffix);
      if(t.ht > 450.  && t.ht < 575.)  fillHistosSingleLepton(InclusiveRegions.at(1).crslHistMap,   SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crslbaseL", suffix);
      if(t.ht > 575.  && t.ht < 1000.) fillHistosSingleLepton(InclusiveRegions.at(2).crslHistMap,  SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crslbaseM", suffix);
      if(t.ht > 1000. && t.ht < 1500.) fillHistosSingleLepton(InclusiveRegions.at(3).crslHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crslbaseH", suffix);
      if(t.ht > 1500.                 ) fillHistosSingleLepton(InclusiveRegions.at(4).crslHistMap,  SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crslbaseUH", suffix);
    }
  }

  // only fill wjets/ttbar histograms for inclusive lepton case
  if(!doMinimalPlots && prefix=="crsl") {

    // inclusive regions with btag cuts for wjets/ttbar
    std::map<std::string, float> valuesInc;
    valuesInc["deltaPhiMin"] = t.deltaPhiMin;
    valuesInc["diffMetMhtOverMet"]  = t.diffMetMht/t.met_pt;
    valuesInc["nlep"]        = t.nLepLowMT;
    valuesInc["j1pt"]        = t.jet1_pt;
    valuesInc["j2pt"]        = t.jet2_pt;
    valuesInc["mt2"]         = t.mt2;
    valuesInc["passesHtMet"] = ( (t.ht > 200. && t.met_pt > 200.) || (t.ht > 1000. && t.met_pt > 30.) );
    valuesInc["nbjets"]         = t.nBJet20;
    if (CRSL_WJets.PassesSelectionCRSL(valuesInc)) {
      fillHistosSingleLepton(CRSL_WJets.crslHistMap, CRSL_WJets.GetNumberOfMT2Bins(), CRSL_WJets.GetMT2Bins(), CRSL_WJets.GetName().c_str(), suffix);
    }
    if (CRSL_TTbar.PassesSelectionCRSL(valuesInc)) {
      fillHistosSingleLepton(CRSL_TTbar.crslHistMap, CRSL_TTbar.GetNumberOfMT2Bins(), CRSL_TTbar.GetMT2Bins(), CRSL_TTbar.GetName().c_str(), suffix);
    }
  }

  // topological regions
  std::map<std::string, float> values;
  values["deltaPhiMin"] = t.deltaPhiMin;
  values["diffMetMhtOverMet"]  = t.diffMetMht/t.met_pt;
  values["nlep"]        = t.nLepLowMT;
  values["j1pt"]        = t.jet1_pt;
  values["j2pt"]        = t.jet2_pt;
  values["njets"]       = t.nJet30;
  values["nbjets"]      = t.nBJet20;
  values["mt2"]         = t.mt2;
  values["ht"]          = t.ht;
  values["met"]         = t.met_pt;

  for(unsigned int srN = 0; srN < SRVec.size(); srN++){
    if(SRVec.at(srN).PassesSelectionCRSL(values)){
      if(prefix=="crsl")    fillHistosSingleLepton(SRVec.at(srN).crslHistMap,    SRVec.at(srN).GetNumberOfMT2Bins(), SRVec.at(srN).GetMT2Bins(), prefix+SRVec.at(srN).GetName(), suffix);
      else if(prefix=="crslmu")  fillHistosSingleLepton(SRVec.at(srN).crslmuHistMap,  SRVec.at(srN).GetNumberOfMT2Bins(), SRVec.at(srN).GetMT2Bins(), prefix+SRVec.at(srN).GetName(), suffix);
      else if(prefix=="crslel")  fillHistosSingleLepton(SRVec.at(srN).crslelHistMap,  SRVec.at(srN).GetNumberOfMT2Bins(), SRVec.at(srN).GetMT2Bins(), prefix+SRVec.at(srN).GetName(), suffix);
      //      break;//control regions are not necessarily orthogonal
    }
  }

  // do monojet SRs
  if (passMonojetId_ && (!t.isData || t.HLT_PFMETNoMu90_PFMHTNoMu90 || t.HLT_PFHT350_PFMET100)) {

    // first fill base region
    std::map<std::string, float> valuesBase_monojet;
    valuesBase_monojet["deltaPhiMin"] = t.deltaPhiMin;
    valuesBase_monojet["diffMetMhtOverMet"]  = t.diffMetMht/t.met_pt;
    valuesBase_monojet["nlep"]        = t.nLepLowMT;
    valuesBase_monojet["j1pt"]        = t.jet1_pt;
    valuesBase_monojet["njets"]       = t.nJet30;
    valuesBase_monojet["met"]         = t.met_pt;

    if (SRBaseMonojet.PassesSelectionCRSL(valuesBase_monojet)) {
      if(prefix=="crsl") fillHistosSingleLepton(SRBaseMonojet.crslHistMap, SRBaseMonojet.GetNumberOfMT2Bins(), SRBaseMonojet.GetMT2Bins(), "crslbaseJ", suffix);
      else if(prefix=="crslmu") fillHistosSingleLepton(SRBaseMonojet.crslmuHistMap, SRBaseMonojet.GetNumberOfMT2Bins(), SRBaseMonojet.GetMT2Bins(), "crslmubaseJ", suffix);
      else if(prefix=="crslel") fillHistosSingleLepton(SRBaseMonojet.crslelHistMap, SRBaseMonojet.GetNumberOfMT2Bins(), SRBaseMonojet.GetMT2Bins(), "crslelbaseJ", suffix);
    }

    
    std::map<std::string, float> values_monojet;
    values_monojet["deltaPhiMin"] = t.deltaPhiMin;
    values_monojet["diffMetMhtOverMet"]  = t.diffMetMht/t.met_pt;
    values_monojet["nlep"]        = t.nLepLowMT;
    values_monojet["j1pt"]        = t.jet1_pt;
    values_monojet["njets"]       = t.nJet30;
    values_monojet["nbjets"]      = t.nBJet20;
    values_monojet["ht"]          = t.ht;
    values_monojet["met"]         = t.met_pt;

    for(unsigned int srN = 0; srN < SRVecMonojet.size(); srN++){
      if(SRVecMonojet.at(srN).PassesSelectionCRSL(values_monojet)){
	if(prefix=="crsl")    fillHistosSingleLepton(SRVecMonojet.at(srN).crslHistMap,    SRVecMonojet.at(srN).GetNumberOfMT2Bins(), SRVecMonojet.at(srN).GetMT2Bins(), prefix+SRVecMonojet.at(srN).GetName(), suffix);
	else if(prefix=="crslmu")  fillHistosSingleLepton(SRVecMonojet.at(srN).crslmuHistMap,  SRVecMonojet.at(srN).GetNumberOfMT2Bins(), SRVecMonojet.at(srN).GetMT2Bins(), prefix+SRVecMonojet.at(srN).GetName(), suffix);
	else if(prefix=="crslel")  fillHistosSingleLepton(SRVecMonojet.at(srN).crslelHistMap,  SRVecMonojet.at(srN).GetNumberOfMT2Bins(), SRVecMonojet.at(srN).GetMT2Bins(), prefix+SRVecMonojet.at(srN).GetName(), suffix);
	//      break;//control regions are not necessarily orthogonal
      }
    }
  } // monojet regions
  
  return;
}

// hists for Gamma+Jets control region
void MT2Looper::fillHistosCRGJ(const std::string& prefix, const std::string& suffix) {

  if (t.ngamma==0) return;

  // trigger requirement
  if (!t.HLT_Photon165_HE10) return;
  
  bool passSieie = t.gamma_idCutBased[0] ? true : false; // just deal with the standard case now. Worry later about sideband in sieie

  // fill hists
  std::string add="";

  float passPtMT2 = false;
  if (t.mt2 < 200 && t.gamma_pt[0]>180.) passPtMT2 = true;

  std::map<std::string, float> values;
  values["deltaPhiMin"] = t.gamma_deltaPhiMin;
  values["diffMetMhtOverMet"]  = t.gamma_diffMetMht/t.gamma_met_pt;
  values["nlep"]        = nlepveto_;
  values["j1pt"]        = t.gamma_jet1_pt;
  values["j2pt"]        = t.gamma_jet2_pt;
  values["njets"]       = t.gamma_nJet30;
  values["nbjets"]      = t.gamma_nBJet20;
  values["mt2"]         = t.gamma_mt2;
  values["ht"]          = t.gamma_ht;
  values["met"]         = t.gamma_met_pt;
  
  // Separate list for SRBASE
  std::map<std::string, float> valuesBase;
  valuesBase["deltaPhiMin"] = t.gamma_deltaPhiMin;
  valuesBase["diffMetMhtOverMet"]  = t.gamma_diffMetMht/t.gamma_met_pt;
  valuesBase["nlep"]        = nlepveto_;
  valuesBase["j1pt"]        = t.gamma_jet1_pt;
  valuesBase["j2pt"]        = t.gamma_jet2_pt;
  valuesBase["mt2"]         = t.gamma_mt2;
  valuesBase["passesHtMet"] = ( (t.gamma_ht > 200. && t.gamma_met_pt > 200.) || (t.gamma_ht > 1000. && t.gamma_met_pt > 30.) );
  bool passBase = SRBase.PassesSelection(valuesBase);

  std::map<std::string, float> valuesBase_monojet;
  valuesBase_monojet["deltaPhiMin"] = t.gamma_deltaPhiMin;
  valuesBase_monojet["diffMetMhtOverMet"]  = t.gamma_diffMetMht/t.gamma_met_pt;
  valuesBase_monojet["nlep"]        = nlepveto_;
  valuesBase_monojet["j1pt"]        = t.gamma_jet1_pt;
  valuesBase_monojet["njets"]       = t.gamma_nJet30;
  valuesBase_monojet["met"]         = t.gamma_met_pt;

  bool passBaseJ = SRBaseMonojet.PassesSelection(valuesBase_monojet);

  std::map<std::string, float> values_monojet;
  values_monojet["deltaPhiMin"] = t.gamma_deltaPhiMin;
  values_monojet["diffMetMhtOverMet"]  = t.gamma_diffMetMht/t.gamma_met_pt;
  values_monojet["nlep"]        = nlepveto_;
  values_monojet["j1pt"]        = t.gamma_jet1_pt;
  values_monojet["njets"]       = t.gamma_nJet30;
  values_monojet["nbjets"]      = t.gamma_nBJet20;
  values_monojet["ht"]          = t.gamma_ht;
  values_monojet["met"]         = t.gamma_met_pt;
  

  //float iso = t.gamma_chHadIso[0] + t.gamma_phIso[0];
  float iso = t.gamma_chHadIso[0];
  float isoCutTight = 2.5;
  float isoCutLoose = 20.;
  if (t.gamma_ht > 200) fillHistosGammaJets(SRNoCut.crgjHistMap, SRNoCut.crgjRooDataSetMap, SRNoCut.GetNumberOfMT2Bins(), SRNoCut.GetMT2Bins(), prefix+SRNoCut.GetName(), suffix+add+"All");

  if (iso>isoCutTight && iso < isoCutLoose) add += "LooseNotTight";
  if (iso>isoCutLoose) add += "NotLoose";
  if (!passSieie) add += "SieieSB"; // Keep Sigma IEta IEta sideband
  if (t.gamma_ht > 200) fillHistosGammaJets(SRNoCut.crgjHistMap, SRNoCut.crgjRooDataSetMap, SRNoCut.GetNumberOfMT2Bins(), SRNoCut.GetMT2Bins(), prefix+SRNoCut.GetName(), suffix+add);
  
  // Monojet Regions
  if ( passBaseJ && t.gamma_pt[0] > 180. ) {
    fillHistosGammaJets(SRBaseMonojet.crgjHistMap, SRBaseMonojet.crgjRooDataSetMap, SRBaseMonojet.GetNumberOfMT2Bins(), SRBaseMonojet.GetMT2Bins(), "crgjbaseJ", suffix+add);
    for(unsigned int srN = 0; srN < SRVecMonojet.size(); srN++){
      if(SRVecMonojet.at(srN).PassesSelection(values_monojet)){
	fillHistosGammaJets(SRVecMonojet.at(srN).crgjHistMap, SRVecMonojet.at(srN).crgjRooDataSetMap,   SRVecMonojet.at(srN).GetNumberOfMT2Bins(), SRVecMonojet.at(srN).GetMT2Bins(), prefix+SRVecMonojet.at(srN).GetName(), suffix+add);
      }
    }
  }

  // Multijet Regions
  if(passBase && passPtMT2) {
    //if (t.nJet30FailId > 0) cout<<"Event "<<t.evt<<" in run "<<t.run<<" fails jet ID"<<endl;
    //if (iso<isoCutTight && passSieie) {
    //  cout<<" "<<endl;
    //  cout<<"Event "<<t.evt<<" in run "<<t.run<<" passes baseline selection"<<endl;
    //}
    fillHistosGammaJets(SRBase.crgjHistMap, SRBase.crgjRooDataSetMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crgjbase", suffix+add);
    if(passBase && t.gamma_ht > 200.  && t.gamma_ht < 450.)  fillHistosGammaJets(InclusiveRegions.at(0).crgjHistMap,   InclusiveRegions.at(0).crgjRooDataSetMap,   SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crgjbaseVL", suffix+add);
    if(passBase && t.gamma_ht > 450.  && t.gamma_ht < 575.)  fillHistosGammaJets(InclusiveRegions.at(1).crgjHistMap,   InclusiveRegions.at(1).crgjRooDataSetMap,   SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crgjbaseL", suffix+add);
    if(passBase && t.gamma_ht > 575.  && t.gamma_ht < 1000.) fillHistosGammaJets(InclusiveRegions.at(2).crgjHistMap,  InclusiveRegions.at(2).crgjRooDataSetMap,  SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crgjbaseM", suffix+add);
    if(passBase && t.gamma_ht > 1000. && t.gamma_ht < 1500.) fillHistosGammaJets(InclusiveRegions.at(3).crgjHistMap, InclusiveRegions.at(3).crgjRooDataSetMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crgjbaseH", suffix+add);
    if(passBase && t.gamma_ht > 1500.                  ) fillHistosGammaJets(InclusiveRegions.at(4).crgjHistMap,  InclusiveRegions.at(4).crgjRooDataSetMap,  SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crgjbaseUH", suffix+add);
    for(unsigned int srN = 0; srN < SRVec.size(); srN++){
      if(SRVec.at(srN).PassesSelection(values)){
	fillHistosGammaJets(SRVec.at(srN).crgjHistMap, SRVec.at(srN).crgjRooDataSetMap, SRVec.at(srN).GetNumberOfMT2Bins(), SRVec.at(srN).GetMT2Bins(), prefix+SRVec.at(srN).GetName(), suffix+add);
	break;//control regions are orthogonal, event cannot be in more than one
      }
    }//SRloop
  }

  // BaseInclusive
  if ((passBaseJ && t.gamma_pt[0] > 180.) || (passBase && passPtMT2)) {
    fillHistosGammaJets(SRBaseIncl.crgjHistMap, SRBaseIncl.crgjRooDataSetMap, SRBaseIncl.GetNumberOfMT2Bins(), SRBaseIncl.GetMT2Bins(), "crgjbaseIncl", suffix+add);
  }

  // Remake everything again to get "Loose"
  if (iso<isoCutLoose) {
    add = "Loose";
    if (!passSieie) add += "SieieSB"; // Keep Sigma IEta IEta sideband
    fillHistosGammaJets(SRNoCut.crgjHistMap, SRNoCut.crgjRooDataSetMap, SRNoCut.GetNumberOfMT2Bins(), SRNoCut.GetMT2Bins(), prefix+SRNoCut.GetName(), suffix+add);
    if ( passBaseJ && t.gamma_pt[0] > 180.) {
      fillHistosGammaJets(SRBaseMonojet.crgjHistMap, SRBaseMonojet.crgjRooDataSetMap, SRBaseMonojet.GetNumberOfMT2Bins(), SRBaseMonojet.GetMT2Bins(), "crgjbaseJ", suffix+add);
      for(unsigned int srN = 0; srN < SRVecMonojet.size(); srN++){
	if(SRVecMonojet.at(srN).PassesSelection(values_monojet)){
	  fillHistosGammaJets(SRVecMonojet.at(srN).crgjHistMap, SRVecMonojet.at(srN).crgjRooDataSetMap,   SRVecMonojet.at(srN).GetNumberOfMT2Bins(), SRVecMonojet.at(srN).GetMT2Bins(), prefix+SRVecMonojet.at(srN).GetName(), suffix+add);
	}
      }
    }
    if(passBase && passPtMT2) {
      fillHistosGammaJets(SRBase.crgjHistMap, SRBase.crgjRooDataSetMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crgjbase", suffix+add);
      for(unsigned int srN = 0; srN < SRVec.size(); srN++){
	if(SRVec.at(srN).PassesSelection(values)){
	  fillHistosGammaJets(SRVec.at(srN).crgjHistMap, SRVec.at(srN).crgjRooDataSetMap, SRVec.at(srN).GetNumberOfMT2Bins(), SRVec.at(srN).GetMT2Bins(), prefix+SRVec.at(srN).GetName(), suffix+add);
	  break;//control regions are orthogonal, event cannot be in more than one
	}
      }//SRloop
    }
    if ((passBaseJ && t.gamma_pt[0] > 180.) || (passBase && passPtMT2)) {
      fillHistosGammaJets(SRBaseIncl.crgjHistMap, SRBaseIncl.crgjRooDataSetMap, SRBaseIncl.GetNumberOfMT2Bins(), SRBaseIncl.GetMT2Bins(), "crgjbaseIncl", suffix+add);
    }
  }
  if (!doMinimalPlots) {
    // Remake everything for inclusive isolation
    if (passSieie) {
      add = "AllIso";
      fillHistosGammaJets(SRNoCut.crgjHistMap, SRNoCut.crgjRooDataSetMap, SRNoCut.GetNumberOfMT2Bins(), SRNoCut.GetMT2Bins(), prefix+SRNoCut.GetName(), suffix+add);
      if ( passBaseJ && t.gamma_pt[0] > 180.) {
	fillHistosGammaJets(SRBaseMonojet.crgjHistMap, SRBaseMonojet.crgjRooDataSetMap, SRBaseMonojet.GetNumberOfMT2Bins(), SRBaseMonojet.GetMT2Bins(), "crgjbaseJ", suffix+add);
      }
      if(passBase && passPtMT2) {
	fillHistosGammaJets(SRBase.crgjHistMap, SRBase.crgjRooDataSetMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crgjbase", suffix+add);
      }
    }
  }
      

  return;
}

// hists for Zll control region
void MT2Looper::fillHistosCRDY(const std::string& prefix, const std::string& suffix) {

  if (t.nlep!=2) return;

  // trigger requirement on data and MC already implemented when defining doDYplots
  
  std::map<std::string, float> values;
  values["deltaPhiMin"] = t.zll_deltaPhiMin;
  values["diffMetMhtOverMet"]  = t.zll_diffMetMht/t.zll_met_pt;
  values["nlep"]        = 0; // dummy value
  values["j1pt"]        = t.jet1_pt;
  values["j2pt"]        = t.jet2_pt;
  values["njets"]       = t.nJet30;
  values["nbjets"]      = t.nBJet20;
  values["mt2"]         = t.zll_mt2;
  values["ht"]          = t.zll_ht;
  values["met"]         = t.zll_met_pt;

 // Separate list for SRBASE
  std::map<std::string, float> valuesBase;
  valuesBase["deltaPhiMin"] = t.zll_deltaPhiMin;
  valuesBase["diffMetMhtOverMet"]  = t.zll_diffMetMht/t.zll_met_pt;
  valuesBase["nlep"]        = 0; // dummy value
  valuesBase["j1pt"]        = t.jet1_pt;
  valuesBase["j2pt"]        = t.jet2_pt;
  valuesBase["mt2"]         = t.zll_mt2;
  valuesBase["passesHtMet"] = ( (t.zll_ht > 200. && t.zll_met_pt > 200.) || (t.zll_ht > 1000. && t.zll_met_pt > 30.) );
  bool passBase = SRBase.PassesSelection(valuesBase);

  std::map<std::string, float> valuesBase_monojet;
  valuesBase_monojet["deltaPhiMin"] = t.zll_deltaPhiMin;
  valuesBase_monojet["diffMetMhtOverMet"]  = t.zll_diffMetMht/t.zll_met_pt;
  valuesBase_monojet["nlep"]        = 0;
  valuesBase_monojet["j1pt"]        = t.jet1_pt;
  valuesBase_monojet["njets"]       = t.nJet30;
  valuesBase_monojet["met"]         = t.zll_met_pt;

  bool passBaseJ = SRBaseMonojet.PassesSelection(valuesBase_monojet) && passMonojetId_;

  std::map<std::string, float> values_monojet;
  values_monojet["deltaPhiMin"] = t.zll_deltaPhiMin;
  values_monojet["diffMetMhtOverMet"]  = t.zll_diffMetMht/t.zll_met_pt;
  values_monojet["nlep"]        = 0;
  values_monojet["j1pt"]        = t.jet1_pt;
  values_monojet["njets"]       = t.nJet30;
  values_monojet["nbjets"]      = t.nBJet20;
  values_monojet["ht"]          = t.zll_ht;
  values_monojet["met"]         = t.zll_met_pt;
  
  if (t.zll_ht > 200) fillHistosDY(SRNoCut.crdyHistMap, SRNoCut.GetNumberOfMT2Bins(), SRNoCut.GetMT2Bins(), prefix+SRNoCut.GetName(), suffix);
  if(passBase) fillHistosDY(SRBase.crdyHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crdybase", suffix);
  if(passBaseJ) {
    fillHistosDY(SRBaseMonojet.crdyHistMap, SRBaseMonojet.GetNumberOfMT2Bins(), SRBaseMonojet.GetMT2Bins(), "crdybaseJ", suffix);
    for(unsigned int srN = 0; srN < SRVecMonojet.size(); srN++){
      if(SRVecMonojet.at(srN).PassesSelection(values_monojet)){
	fillHistosDY(SRVecMonojet.at(srN).crdyHistMap, SRVecMonojet.at(srN).GetNumberOfMT2Bins(), SRVecMonojet.at(srN).GetMT2Bins(), prefix+SRVecMonojet.at(srN).GetName(), suffix); 
      }
    }
  }
  if(passBase || passBaseJ) fillHistosDY(SRBaseIncl.crdyHistMap, SRBaseIncl.GetNumberOfMT2Bins(), SRBaseIncl.GetMT2Bins(), "crdybaseIncl", suffix);



  if(passBase && t.zll_ht > 200.  && t.zll_ht < 450.)  fillHistosDY(InclusiveRegions.at(0).crdyHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crdybaseVL", suffix);
  if(passBase && t.zll_ht > 450.  && t.zll_ht < 575.)  fillHistosDY(InclusiveRegions.at(1).crdyHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crdybaseL", suffix);
  if(passBase && t.zll_ht > 575.  && t.zll_ht < 1000.) fillHistosDY(InclusiveRegions.at(2).crdyHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crdybaseM", suffix);
  if(passBase && t.zll_ht > 1000. && t.zll_ht < 1500.) fillHistosDY(InclusiveRegions.at(3).crdyHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crdybaseH", suffix);
  if(passBase && t.zll_ht > 1500.                  ) fillHistosDY(InclusiveRegions.at(4).crdyHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crdybaseUH", suffix);

  for(unsigned int srN = 0; srN < SRVec.size(); srN++){
    if(SRVec.at(srN).PassesSelection(values)){
      fillHistosDY(SRVec.at(srN).crdyHistMap, SRVec.at(srN).GetNumberOfMT2Bins(), SRVec.at(srN).GetMT2Bins(), prefix+SRVec.at(srN).GetName(), suffix);
      break;//control regions are orthogonal, event cannot be in more than one
    }
  }

  return;
}

// hists for removed single lepton control region
void MT2Looper::fillHistosCRRL(const std::string& prefix, const std::string& suffix) {

  if (t.nlep!=1) return;

  // trigger requirement on data and MC already included in RL selection
  
  std::map<std::string, float> values;
  values["deltaPhiMin"] = t.rl_deltaPhiMin;
  values["diffMetMhtOverMet"]  = t.rl_diffMetMht/t.rl_met_pt;
  values["nlep"]        = 0; // dummy value
  values["j1pt"]        = t.jet1_pt;
  values["j2pt"]        = t.jet2_pt;
  values["njets"]       = t.nJet30;
  values["nbjets"]      = t.nBJet20;
  values["mt2"]         = t.rl_mt2;
  values["ht"]          = t.rl_ht;
  values["met"]         = t.rl_met_pt;

 // Separate list for SRBASE
  std::map<std::string, float> valuesBase;
  valuesBase["deltaPhiMin"] = t.rl_deltaPhiMin;
  valuesBase["diffMetMhtOverMet"]  = t.rl_diffMetMht/t.rl_met_pt;
  valuesBase["nlep"]        = 0; // dummy value
  valuesBase["j1pt"]        = t.jet1_pt;
  valuesBase["j2pt"]        = t.jet2_pt;
  valuesBase["mt2"]         = t.rl_mt2;
  valuesBase["passesHtMet"] = ( (t.rl_ht > 200. && t.rl_met_pt > 200.) || (t.rl_ht > 1000. && t.rl_met_pt > 30.) );
  bool passBase = SRBase.PassesSelection(valuesBase);

  std::map<std::string, float> valuesBase_monojet;
  valuesBase_monojet["deltaPhiMin"] = t.rl_deltaPhiMin;
  valuesBase_monojet["diffMetMhtOverMet"]  = t.rl_diffMetMht/t.rl_met_pt;
  valuesBase_monojet["nlep"]        = 0;
  valuesBase_monojet["j1pt"]        = t.jet1_pt;
  valuesBase_monojet["njets"]       = t.nJet30;
  valuesBase_monojet["met"]         = t.rl_met_pt;

  bool passBaseJ = SRBaseMonojet.PassesSelection(valuesBase_monojet);
  
  std::map<std::string, float> values_monojet;
  values_monojet["deltaPhiMin"] = t.rl_deltaPhiMin;
  values_monojet["diffMetMhtOverMet"]  = t.rl_diffMetMht/t.rl_met_pt;
  values_monojet["nlep"]        = 0;
  values_monojet["j1pt"]        = t.jet1_pt;
  values_monojet["njets"]       = t.nJet30;
  values_monojet["nbjets"]      = t.nBJet20;
  values_monojet["ht"]          = t.rl_ht;
  values_monojet["met"]         = t.rl_met_pt;

  if (t.rl_ht > 200) {
    if(prefix=="crrl")        fillHistosRemovedLepton(SRNoCut.crrlHistMap,   SRNoCut.GetNumberOfMT2Bins(), SRNoCut.GetMT2Bins(), prefix+SRNoCut.GetName(), suffix);
    else if(prefix=="crrlmu") fillHistosRemovedLepton(SRNoCut.crrlmuHistMap, SRNoCut.GetNumberOfMT2Bins(), SRNoCut.GetMT2Bins(), prefix+SRNoCut.GetName(), suffix);
    else if(prefix=="crrlel") fillHistosRemovedLepton(SRNoCut.crrlelHistMap, SRNoCut.GetNumberOfMT2Bins(), SRNoCut.GetMT2Bins(), prefix+SRNoCut.GetName(), suffix);
  }

  if(passBase) {
    if(prefix=="crrl")        fillHistosRemovedLepton(SRBase.crrlHistMap,   SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crrlbase",   suffix);
    else if(prefix=="crrlmu") fillHistosRemovedLepton(SRBase.crrlmuHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crrlmubase", suffix);
    else if(prefix=="crrlel") fillHistosRemovedLepton(SRBase.crrlelHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crrlelbase", suffix);
  }
  if(passBaseJ) {
    if(prefix=="crrl")        fillHistosRemovedLepton(SRBaseMonojet.crrlHistMap,   SRBaseMonojet.GetNumberOfMT2Bins(), SRBaseMonojet.GetMT2Bins(), "crrlbaseJ",   suffix);
    else if(prefix=="crrlmu") fillHistosRemovedLepton(SRBaseMonojet.crrlmuHistMap, SRBaseMonojet.GetNumberOfMT2Bins(), SRBaseMonojet.GetMT2Bins(), "crrlmubaseJ", suffix);
    else if(prefix=="crrlel") fillHistosRemovedLepton(SRBaseMonojet.crrlelHistMap, SRBaseMonojet.GetNumberOfMT2Bins(), SRBaseMonojet.GetMT2Bins(), "crrlelbaseJ", suffix);
    for(unsigned int srN = 0; srN < SRVecMonojet.size(); srN++){
      if(SRVecMonojet.at(srN).PassesSelection(values_monojet)){
	if(prefix=="crrl") fillHistosRemovedLepton(SRVecMonojet.at(srN).crrlHistMap, SRVecMonojet.at(srN).GetNumberOfMT2Bins(), SRVecMonojet.at(srN).GetMT2Bins(), prefix+SRVecMonojet.at(srN).GetName(), suffix);
      }
    }
  }
  if(passBase || passBaseJ) {
    if(prefix=="crrl")        fillHistosRemovedLepton(SRBaseIncl.crrlHistMap,   SRBaseIncl.GetNumberOfMT2Bins(), SRBaseIncl.GetMT2Bins(), "crrlbaseIncl",   suffix);
  }

  if(passBase && t.rl_ht > 200.  && t.rl_ht < 450.)  fillHistosRemovedLepton(InclusiveRegions.at(0).crrlHistMap,   SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crrlbaseVL", suffix);
  if(passBase && t.rl_ht > 450.  && t.rl_ht < 575.)  fillHistosRemovedLepton(InclusiveRegions.at(1).crrlHistMap,   SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crrlbaseL", suffix);
  if(passBase && t.rl_ht > 575.  && t.rl_ht < 1000.) fillHistosRemovedLepton(InclusiveRegions.at(2).crrlHistMap,  SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crrlbaseM", suffix);
  if(passBase && t.rl_ht > 1000. && t.rl_ht < 1500.) fillHistosRemovedLepton(InclusiveRegions.at(3).crrlHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crrlbaseH", suffix);
  if(passBase && t.rl_ht > 1500.                 ) fillHistosRemovedLepton(InclusiveRegions.at(4).crrlHistMap,  SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crrlbaseUH", suffix);

  // if(passBase && t.evt_id == 400 ) fillHistosRemovedLepton(SRBase.crrlHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crrlbase", "TsChan");
  // if(passBase && t.evt_id == 401 ) fillHistosRemovedLepton(SRBase.crrlHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crrlbase", "TtChan");
  // if(passBase && t.evt_id == 402 ) fillHistosRemovedLepton(SRBase.crrlHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crrlbase", "TtWChan");
  // if(passBase && t.evt_id == 403 ) fillHistosRemovedLepton(SRBase.crrlHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crrlbase", "TbarsChan");
  // if(passBase && t.evt_id == 404 ) fillHistosRemovedLepton(SRBase.crrlHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crrlbase", "TbartChan");
  // if(passBase && t.evt_id == 405 ) fillHistosRemovedLepton(SRBase.crrlHistMap, SRBase.GetNumberOfMT2Bins(), SRBase.GetMT2Bins(), "crrlbase", "TbartwChan");

  for(unsigned int srN = 0; srN < SRVec.size(); srN++){
    if(SRVec.at(srN).PassesSelection(values)){
      if(prefix=="crrl")        fillHistosRemovedLepton(SRVec.at(srN).crrlHistMap,   SRVec.at(srN).GetNumberOfMT2Bins(), SRVec.at(srN).GetMT2Bins(), prefix+SRVec.at(srN).GetName(), suffix);
      else if(prefix=="crrlmu") fillHistosRemovedLepton(SRVec.at(srN).crrlmuHistMap, SRVec.at(srN).GetNumberOfMT2Bins(), SRVec.at(srN).GetMT2Bins(), prefix+SRVec.at(srN).GetName(), suffix);
      else if(prefix=="crrlel") fillHistosRemovedLepton(SRVec.at(srN).crrlelHistMap, SRVec.at(srN).GetNumberOfMT2Bins(), SRVec.at(srN).GetMT2Bins(), prefix+SRVec.at(srN).GetName(), suffix);
      break;//control regions are orthogonal, event cannot be in more than one
    }
  }

  return;
}

// hists for single lepton control region
void MT2Looper::fillHistosCRQCD(const std::string& prefix, const std::string& suffix) {

  // trigger requirement on data
  if (t.isData && !(t.HLT_PFHT800 || t.HLT_PFHT350_PFMET100 || t.HLT_PFMETNoMu90_PFMHTNoMu90)) return;
  
  // topological regions
  std::map<std::string, float> values;
  values["deltaPhiMin"] = t.deltaPhiMin;
  values["diffMetMhtOverMet"]  = t.diffMetMht/t.met_pt;
  values["nlep"]        = nlepveto_;
  values["j1pt"]        = t.jet1_pt;
  values["j2pt"]        = t.jet2_pt;
  values["mt2"]         = t.mt2;
  values["ht"]          = t.ht;
  values["met"]         = t.met_pt;

  for(unsigned int srN = 0; srN < SRVec.size(); srN++){
    if(SRVec.at(srN).PassesSelectionCRQCD(values)){
      fillHistosQCD(SRVec.at(srN).crqcdHistMap,    SRVec.at(srN).GetNumberOfMT2Bins(), SRVec.at(srN).GetMT2Bins(), prefix+SRVec.at(srN).GetName(), suffix);
      //      break;//control regions are not necessarily orthogonal
    }
  }

  // do monojet SRs
  if (passMonojetId_ && (!t.isData || t.HLT_PFMETNoMu90_PFMHTNoMu90 || t.HLT_PFHT350_PFMET100)) {

    std::map<std::string, float> values_monojet;
    values_monojet["deltaPhiMin"] = t.deltaPhiMin;
    values_monojet["diffMetMhtOverMet"]  = t.diffMetMht/t.met_pt;
    values_monojet["nlep"]        = nlepveto_;
    values_monojet["j1pt"]        = t.jet1_pt;
    values_monojet["j2pt"]        = t.jet2_pt;
    values_monojet["njets"]       = t.nJet30;
    values_monojet["nbjets"]      = t.nBJet20;
    values_monojet["ht"]          = t.ht;
    values_monojet["met"]         = t.met_pt;

    for(unsigned int srN = 0; srN < SRVecMonojet.size(); srN++){
      if(SRVecMonojet.at(srN).PassesSelectionCRQCD(values_monojet)){
	fillHistosQCD(SRVecMonojet.at(srN).crqcdHistMap,    SRVecMonojet.at(srN).GetNumberOfMT2Bins(), SRVecMonojet.at(srN).GetMT2Bins(), prefix+SRVecMonojet.at(srN).GetName(), suffix);
	//      break;//control regions are not necessarily orthogonal
      }
    }
  } // monojet regions
  
  return;
}

void MT2Looper::fillHistos(std::map<std::string, TH1*>& h_1d, int n_mt2bins, float* mt2bins, const std::string& dirname, const std::string& s) {
  TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
  if (dir == 0) {
    dir = outfile_->mkdir(dirname.c_str());
  } 
  dir->cd();

  // workaround for monojet bins
  float mt2_temp = t.mt2;
  if (t.nJet30 == 1) mt2_temp = t.jet1_pt;

  plot1D("h_Events"+s,  1, 1, h_1d, ";Events, Unweighted", 1, 0, 2);
  plot1D("h_Events_w"+s,  1,   evtweight_, h_1d, ";Events, Weighted", 1, 0, 2);
  plot1D("h_mt2bins"+s,       mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
  plot1D("h_htbins"+s,       t.ht,   evtweight_, h_1d, ";H_{T} [GeV]", n_htbins, htbins);
  plot1D("h_htbins2"+s,       t.ht,   evtweight_, h_1d, ";H_{T} [GeV]", n_htbins2, htbins2);
  plot1D("h_njbins"+s,       t.nJet30,   evtweight_, h_1d, ";N(jets)", n_njbins, njbins);
  plot1D("h_nbjbins"+s,       t.nBJet20,   evtweight_, h_1d, ";N(bjets)", n_nbjbins, nbjbins);
  if (!doMinimalPlots) {
    plot1D("h_mt2"+s,       mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", 150, 0, 1500);
    plot1D("h_met"+s,       t.met_pt,   evtweight_, h_1d, ";E_{T}^{miss} [GeV]", 150, 0, 1500);
    plot1D("h_ht"+s,       t.ht,   evtweight_, h_1d, ";H_{T} [GeV]", 120, 0, 3000);
    plot1D("h_nJet30"+s,       t.nJet30,   evtweight_, h_1d, ";N(jets)", 15, 0, 15);
    plot1D("h_nJet30Eta3"+s,       nJet30Eta3_,   evtweight_, h_1d, ";N(jets, |#eta| > 3.0)", 10, 0, 10);
    plot1D("h_nBJet20"+s,      t.nBJet20,   evtweight_, h_1d, ";N(bjets)", 6, 0, 6);
    plot1D("h_deltaPhiMin"+s,  t.deltaPhiMin,   evtweight_, h_1d, ";#Delta#phi_{min}", 32, 0, 3.2);
    plot1D("h_diffMetMht"+s,   t.diffMetMht,   evtweight_, h_1d, ";|E_{T}^{miss} - MHT| [GeV]", 120, 0, 300);
    plot1D("h_diffMetMhtOverMet"+s,   t.diffMetMht/t.met_pt,   evtweight_, h_1d, ";|E_{T}^{miss} - MHT| / E_{T}^{miss}", 100, 0, 2.);
    plot1D("h_minMTBMet"+s,   t.minMTBMet,   evtweight_, h_1d, ";min M_{T}(b, E_{T}^{miss}) [GeV]", 150, 0, 1500);
    plot1D("h_nlepveto"+s,     nlepveto_,   evtweight_, h_1d, ";N(leps)", 10, 0, 10);
    plot1D("h_J0pt"+s,       t.jet1_pt,   evtweight_, h_1d, ";p_{T}(jet1) [GeV]", 150, 0, 1500);
    plot1D("h_J1pt"+s,       t.jet2_pt,   evtweight_, h_1d, ";p_{T}(jet2) [GeV]", 150, 0, 1500);
  }
  TString directoryname(dirname);
  if (isSignal_) {
    plot3D("h_mt2bins_sigscan"+s, mt2_temp, t.GenSusyMScan1, t.GenSusyMScan2, evtweight_, h_1d, ";M_{T2} [GeV];mass1 [GeV];mass2 [GeV]", n_mt2bins, mt2bins, n_m1bins, m1bins, n_m2bins, m2bins);
  }

  if (!t.isData && isSignal_ && doSystVariationPlots) {
    int binx = h_sig_avgweight_isr_->GetXaxis()->FindBin(t.GenSusyMScan1);
    int biny = h_sig_avgweight_isr_->GetYaxis()->FindBin(t.GenSusyMScan2);
    // stored ISR weight is for DN variation
    float weight_isr_DN = t.weight_isr;
    float avgweight_isr_DN = h_sig_avgweight_isr_->GetBinContent(binx,biny);
    float weight_isr_UP = 2. - weight_isr_DN;
    float avgweight_isr_UP = 2. - avgweight_isr_DN;
    plot1D("h_mt2bins_isr_UP"+s,       mt2_temp,   evtweight_ * weight_isr_UP / avgweight_isr_UP, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    plot1D("h_mt2bins_isr_DN"+s,       mt2_temp,   evtweight_ * weight_isr_DN / avgweight_isr_DN, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    plot3D("h_mt2bins_sigscan_isr_UP"+s, mt2_temp, t.GenSusyMScan1, t.GenSusyMScan2, evtweight_ * weight_isr_UP / avgweight_isr_UP, h_1d, ";M_{T2} [GeV];mass1 [GeV];mass2 [GeV]", n_mt2bins, mt2bins, n_m1bins, m1bins, n_m2bins, m2bins);
    plot3D("h_mt2bins_sigscan_isr_DN"+s, mt2_temp, t.GenSusyMScan1, t.GenSusyMScan2, evtweight_ * weight_isr_DN / avgweight_isr_DN, h_1d, ";M_{T2} [GeV];mass1 [GeV];mass2 [GeV]", n_mt2bins, mt2bins, n_m1bins, m1bins, n_m2bins, m2bins);
  }

  if (!t.isData && applyBtagSF && doSystVariationPlots) {

    int binx,biny;
    float avgweight_btagsf = 1.;
    float avgweight_heavy_UP = 1.;
    float avgweight_heavy_DN = 1.;
    float avgweight_light_UP = 1.;
    float avgweight_light_DN = 1.;

    if (isSignal_) {
      binx = h_sig_avgweight_btagsf_heavy_UP_->GetXaxis()->FindBin(t.GenSusyMScan1);
      biny = h_sig_avgweight_btagsf_heavy_UP_->GetYaxis()->FindBin(t.GenSusyMScan2);
      avgweight_btagsf = h_sig_avgweight_btagsf_->GetBinContent(binx,biny);
      avgweight_heavy_UP = h_sig_avgweight_btagsf_heavy_UP_->GetBinContent(binx,biny);
      avgweight_heavy_DN = h_sig_avgweight_btagsf_heavy_DN_->GetBinContent(binx,biny);
      avgweight_light_UP = h_sig_avgweight_btagsf_light_UP_->GetBinContent(binx,biny);
      avgweight_light_DN = h_sig_avgweight_btagsf_light_DN_->GetBinContent(binx,biny);
    }

    // assume weights are already applied to central value
    plot1D("h_mt2bins_btagsf_heavy_UP"+s,       mt2_temp,   evtweight_ / t.weight_btagsf * avgweight_btagsf * t.weight_btagsf_heavy_UP / avgweight_heavy_UP, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    plot1D("h_mt2bins_btagsf_light_UP"+s,       mt2_temp,   evtweight_ / t.weight_btagsf * avgweight_btagsf * t.weight_btagsf_light_UP / avgweight_light_UP, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    plot1D("h_mt2bins_btagsf_heavy_DN"+s,       mt2_temp,   evtweight_ / t.weight_btagsf * avgweight_btagsf * t.weight_btagsf_heavy_DN / avgweight_heavy_DN, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    plot1D("h_mt2bins_btagsf_light_DN"+s,       mt2_temp,   evtweight_ / t.weight_btagsf * avgweight_btagsf * t.weight_btagsf_light_DN / avgweight_light_DN, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    if (isSignal_) {
      plot3D("h_mt2bins_sigscan_btagsf_heavy_UP"+s, mt2_temp, t.GenSusyMScan1, t.GenSusyMScan2, evtweight_ / t.weight_btagsf * avgweight_btagsf * t.weight_btagsf_heavy_UP / avgweight_heavy_UP, h_1d, ";M_{T2} [GeV];mass1 [GeV];mass2 [GeV]", n_mt2bins, mt2bins, n_m1bins, m1bins, n_m2bins, m2bins);
      plot3D("h_mt2bins_sigscan_btagsf_light_UP"+s, mt2_temp, t.GenSusyMScan1, t.GenSusyMScan2, evtweight_ / t.weight_btagsf * avgweight_btagsf * t.weight_btagsf_light_UP / avgweight_light_UP, h_1d, ";M_{T2} [GeV];mass1 [GeV];mass2 [GeV]", n_mt2bins, mt2bins, n_m1bins, m1bins, n_m2bins, m2bins);
      plot3D("h_mt2bins_sigscan_btagsf_heavy_DN"+s, mt2_temp, t.GenSusyMScan1, t.GenSusyMScan2, evtweight_ / t.weight_btagsf * avgweight_btagsf * t.weight_btagsf_heavy_DN / avgweight_heavy_DN, h_1d, ";M_{T2} [GeV];mass1 [GeV];mass2 [GeV]", n_mt2bins, mt2bins, n_m1bins, m1bins, n_m2bins, m2bins);
      plot3D("h_mt2bins_sigscan_btagsf_light_DN"+s, mt2_temp, t.GenSusyMScan1, t.GenSusyMScan2, evtweight_ / t.weight_btagsf * avgweight_btagsf * t.weight_btagsf_light_DN / avgweight_light_DN, h_1d, ";M_{T2} [GeV];mass1 [GeV];mass2 [GeV]", n_mt2bins, mt2bins, n_m1bins, m1bins, n_m2bins, m2bins);
    }
  }

  if (!t.isData && doGenTauVars) {
    float unc_tau1p = 0.;
    float unc_tau3p = 0.;
    if (t.ngenTau1Prong > 0 || t.ngenTau3Prong > 0) {
      // loop on gen taus
      for (int itau = 0; itau < t.ngenTau; ++itau) {
	// check acceptance for veto: pt > 10
       if (t.genTau_leadTrackPt[itau] < 10.) continue;
       if (t.genTau_decayMode[itau] == 1) unc_tau1p += 0.14; // 14% relative uncertainty for missing a 1-prong tau
       else if (t.genTau_decayMode[itau] == 3) unc_tau3p += 0.06; // 6% relative uncertainty for missing a 3-prong tau
      }
    }
    
    plot1D("h_mt2bins_tau1p_UP"+s,       mt2_temp,   evtweight_ * (1. + unc_tau1p), h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    plot1D("h_mt2bins_tau1p_DN"+s,       mt2_temp,   evtweight_ * (1. - unc_tau1p), h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    plot1D("h_mt2bins_tau3p_UP"+s,       mt2_temp,   evtweight_ * (1. + unc_tau3p), h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    plot1D("h_mt2bins_tau3p_DN"+s,       mt2_temp,   evtweight_ * (1. - unc_tau3p), h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
  }

  // --------------------------------------------------------
  // -------------- old dummy uncertainty code --------------
  // --------------------------------------------------------
  
  // // lepton efficiency variation in signal region: large uncertainty on leptons NOT vetoed
  // if (!t.isData && doLepEffVars && directoryname.Contains("sr")) {
  //   float unc_lepeff = 0.;
  //   if (t.ngenLep > 0 || t.ngenLepFromTau > 0) {
  //     // loop on gen e/mu
  //     for (int ilep = 0; ilep < t.ngenLep; ++ilep) {
  // 	// check acceptance for veto: pt > 5, |eta| < 2.4
  //      if (t.genLep_pt[ilep] < 5.) continue;
  //      if (fabs(t.genLep_eta[ilep]) > 2.4) continue;
  //      unc_lepeff += 0.20; // 12% relative uncertainty for missing lepton
  //     }
  //     for (int ilep = 0; ilep < t.ngenLepFromTau; ++ilep) {
  // 	// check acceptance for veto: pt > 5
  //      if (t.genLepFromTau_pt[ilep] < 5.) continue;
  //      if (fabs(t.genLepFromTau_eta[ilep]) > 2.4) continue;
  //      unc_lepeff += 0.20; // 12% relative uncertainty for missing lepton
  //     }
  //   }

  //   // if lepeff goes up, number of events in SR should go down.
  //   plot1D("h_mt2bins_lepeff_UP"+s,       mt2_temp,   evtweight_ * (1. - unc_lepeff), h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
  //   plot1D("h_mt2bins_lepeff_DN"+s,       mt2_temp,   evtweight_ * (1. + unc_lepeff), h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
  // }
  // --------------------------------------------------------

  // lepton efficiency variation in signal region for fastsim: large uncertainty on leptons NOT vetoed
  if (!t.isData && isSignal_ && doLepEffVars && applyLeptonSFfastsim && directoryname.Contains("sr")) {

    // if lepeff goes up, number of events in SR should go down. Already taken into account in unc_lepeff_sr_
    //  need to first remove lepeff SF
    plot1D("h_mt2bins_lepeff_UP"+s,       mt2_temp,   evtweight_ / (1. + cor_lepeff_sr_) * (1. + cor_lepeff_sr_ + unc_lepeff_sr_), h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    plot1D("h_mt2bins_lepeff_DN"+s,       mt2_temp,   evtweight_ / (1. + cor_lepeff_sr_) * (1. + cor_lepeff_sr_ - unc_lepeff_sr_), h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    plot3D("h_mt2bins_sigscan_lepeff_UP"+s, mt2_temp, t.GenSusyMScan1, t.GenSusyMScan2, evtweight_ / (1. + cor_lepeff_sr_) * (1. + cor_lepeff_sr_ + unc_lepeff_sr_), h_1d, ";M_{T2} [GeV];mass1 [GeV];mass2 [GeV]", n_mt2bins, mt2bins, n_m1bins, m1bins, n_m2bins, m2bins);
    plot3D("h_mt2bins_sigscan_lepeff_DN"+s, mt2_temp, t.GenSusyMScan1, t.GenSusyMScan2, evtweight_ / (1. + cor_lepeff_sr_) * (1. + cor_lepeff_sr_ - unc_lepeff_sr_), h_1d, ";M_{T2} [GeV];mass1 [GeV];mass2 [GeV]", n_mt2bins, mt2bins, n_m1bins, m1bins, n_m2bins, m2bins);
    
  }

  else if (!t.isData && doLepEffVars && directoryname.Contains("sr")) {
    // if lepeff goes up, number of events in SR should go down. Already taken into account in unc_lepeff_sr_
    plot1D("h_mt2bins_lepeff_UP"+s,       mt2_temp,   evtweight_ * (1. + unc_lepeff_sr_), h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    plot1D("h_mt2bins_lepeff_DN"+s,       mt2_temp,   evtweight_ * (1. - unc_lepeff_sr_), h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
  }
  
  // lepton efficiency variation in control region: smallish uncertainty on leptons which ARE vetoed
  else if (!t.isData && doLepEffVars && directoryname.Contains("crsl")) {
    // lepsf was already applied as a central value, take it back out
    plot1D("h_mt2bins_lepeff_UP"+s,       mt2_temp,   evtweight_ / t.weight_lepsf * t.weight_lepsf_UP, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    plot1D("h_mt2bins_lepeff_DN"+s,       mt2_temp,   evtweight_ / t.weight_lepsf * t.weight_lepsf_DN, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    
    // --------------------------------------------------------
    // -------------- old dummy uncertainty code --------------
    // --------------------------------------------------------
    // float unc_lepeff = 0.;
    // if (t.ngenLep > 0 || t.ngenLepFromTau > 0) {
    //   // loop on gen e/mu
    //   for (int ilep = 0; ilep < t.ngenLep; ++ilep) {
    // 	// check acceptance for veto: pt > 5
    //    if (t.genLep_pt[ilep] < 5.) continue;
    //    if (fabs(t.genLep_eta[ilep]) > 2.4) continue;
    //    unc_lepeff += 0.03; // 3% relative uncertainty for finding lepton
    //   }
    //   for (int ilep = 0; ilep < t.ngenLepFromTau; ++ilep) {
    // 	// check acceptance for veto: pt > 5
    //    if (t.genLepFromTau_pt[ilep] < 5.) continue;
    //    if (fabs(t.genLepFromTau_eta[ilep]) > 2.4) continue;
    //    unc_lepeff += 0.03; // 3% relative uncertainty for finding lepton
    //   }
    // }

    // // if lepeff goes up, number of events in CR should go up
    // plot1D("h_mt2bins_lepeff_UP"+s,       mt2_temp,   evtweight_ * (1. + unc_lepeff), h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    // plot1D("h_mt2bins_lepeff_DN"+s,       mt2_temp,   evtweight_ * (1. - unc_lepeff), h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
    // --------------------------------------------------------
  }
  
  outfile_->cd();
  return;
}

void MT2Looper::fillHistosSingleSoftLepton(std::map<std::string, TH1*>& h_1d, int n_mt2bins, float* mt2bins, const std::string& dirname, const std::string& s) {
  TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
  if (dir == 0) {
    dir = outfile_->mkdir(dirname.c_str());
  } 
  dir->cd();

  plot1D("h_leppt"+s,      softleppt_,   evtweight_, h_1d, ";p_{T}(lep) [GeV]", 200, 0, 1000);
  plot1D("h_lepptshort"+s,      softleppt_,   evtweight_, h_1d, ";p_{T}(lep) [GeV]", 30, 0, 30);
  plot1D("h_lepphi"+s,      softlepphi_,   evtweight_, h_1d, "phi",  64, -3.2, 3.2);
  plot1D("h_lepeta"+s,      softlepeta_,   evtweight_, h_1d, "eta",  60, -3, 3);
  plot1D("h_mt"+s,            softlepmt_,   evtweight_, h_1d, ";M_{T} [GeV]", 500, 0, 500);
  plot1D("h_softlepmt2"+s,    softlepmt2_,   evtweight_, h_1d, "; M_{T2} [GeV]", 150, 0, 1500);
  plot1D("h_rlmt2"+s,    t.rl_mt2,   evtweight_, h_1d, "; M_{T2} [GeV]", 150, 0, 1500);
  plot1D("h_softlepmet"+s,       softleppt_,   evtweight_, h_1d, ";E_{T}^{miss} [GeV]", 150, 0, 1500);
  plot1D("h_softlepht"+s,       t.ht-softleppt_,   evtweight_, h_1d, ";H_{T} [GeV]", 120, 0, 3000);
  plot1D("h_rlht"+s,       t.rl_ht,   evtweight_, h_1d, ";H_{T} [GeV]", 120, 0, 3000);

  //compute stuff for soft lep
  float metX = t.met_pt * cos(t.met_phi);
  float metY = t.met_pt * sin(t.met_phi);
  float lepX = softleppt_ * cos(softlepphi_);
  float lepY = softleppt_ * sin(softlepphi_);
  float wX   = metX + lepX;
  float wY   = metY + lepY;
  float wPt  = sqrt(wX*wX + wY*wY);
  //float wPhi = (wX > 0) ? TMath::ATan(wY/wX) : (TMath::ATan(wY/wX) + TMath::Pi()/2);

  //polarization stuff
  {
    //3vectors in transverse plane
    //set eta to zero for both lepton and MET 3-vectors
    //sum gives "transverse W" vector
    TVector3 Lp(0,0,0);
    TVector3 METp(0,0,0);
    TVector3 Wp(0,0,0);
    Lp.SetPtEtaPhi(softleppt_,0,softlepphi_);
    METp.SetPtEtaPhi(t.met_pt,0,t.met_phi);
    Wp = Lp+METp;

    //4vectors in transverse plane
    //set eta to zero for both lepton and MET 4-vectors
    //sum gives "transverse W" vector
    TLorentzVector Lvec(0,0,0,0);
    TLorentzVector METvec(0,0,0,0);
    TLorentzVector Wvec(0,0,0,0); 
    TVector3 Wrf(0,0,0); //3vector to boost to W rest frame
    TLorentzVector Lrf(0,0,0,0); //4vector of lepton boosted to W rest frame
    Lvec.SetPtEtaPhiM(softleppt_,0,softlepphi_,softlepM_);
    METvec.SetPtEtaPhiM(t.met_pt,0,t.met_phi,0); // m=0 for met vector
    Wvec = Lvec+METvec;
    Wrf = Wvec.BoostVector(); //boost vector to w rest frame
    Lrf = ROOT::Math::VectorUtil::boost(Lvec, Wrf);
    TVector3 Lrf3 = Lrf.Vect(); //3vector piece of boosted lepton 4vector
    
    //polarization variables
    float cmsPol = Lp.Dot(Wp) / (wPt*wPt);
    float atlasPol = Wp.Dot(Lrf3)/(wPt*Lrf3.Mag());

    //some test plots to debug difference between cmsPol,cmsPol2
    plot1D("h_wMagDiff"+s,      wPt/Wp.Mag(),   evtweight_, h_1d, "wPt / Wp.Mag", 20, 0, 2);
    plot1D("h_wPhiDiff"+s,      TMath::ATan(wY/wX) - Wp.Phi(),   evtweight_, h_1d, "arctan(wx/wy) - Wp.Phi",  64, -3.2, 3.2);
    plot1D("h_phi1"+s,      TMath::ATan(wY/wX),   evtweight_, h_1d, "arctan(wx/wy)",  64, -3.2, 3.2);
    plot1D("h_phi2"+s,       Wp.Phi(),   evtweight_, h_1d, "Wp.Phi",  64, -3.2, 3.2);
    plot1D("h_eta"+s,       Wp.Eta(),   evtweight_, h_1d, "Wp.Phi",  64, -3.2, 3.2);
    plot1D("h_lepPhi"+s,       softlepphi_,   evtweight_, h_1d, " lep Phi",  64, -3.2, 3.2);
    plot1D("h_metPhi"+s,       t.met_phi,   evtweight_, h_1d, "MET Phi",  64, -3.2, 3.2);
    
    plot1D("h_cmsPol"+s,      cmsPol,   evtweight_, h_1d, "L_{P}", 26, -1.3, 1.3);
    plot1D("h_cmsPol2"+s,      TMath::Cos(TMath::ATan(wY/wX) - softlepphi_) * softleppt_ / wPt,   evtweight_, h_1d, "L_{P}", 26, -1.3, 1.3);
    plot1D("h_atlasPol"+s,      atlasPol,   evtweight_, h_1d, "L_{P}", 20, -1, 1);
    if (softlepId_ > 0) {
    plot1D("h_cmsPolMinus"+s,      cmsPol,   evtweight_, h_1d, "L_{P}", 13, 0, 1.3);
    plot1D("h_cmsPol2Minus"+s,      TMath::Cos(TMath::ATan(wY/wX) - softlepphi_) * softleppt_ / wPt,   evtweight_, h_1d, "L_{P}", 13, 0, 1.3);
    plot1D("h_atlasPolMinus"+s,      atlasPol,   evtweight_, h_1d, "L_{P}", 20, -1, 1);
    }
    if (softlepId_ < 0) {
    plot1D("h_cmsPolPlus"+s,      cmsPol,   evtweight_, h_1d, "L_{P}", 13, 0, 1.3);
    plot1D("h_cmsPol2Plus"+s,      TMath::Cos(TMath::ATan(wY/wX) - softlepphi_) * softleppt_ / wPt,   evtweight_, h_1d, "L_{P}", 13, 0, 1.3);
    plot1D("h_atlasPolPlus"+s,      atlasPol,   evtweight_, h_1d, "L_{P}", 20, -1, 1);
    }
  }

  plot1D("h_Wpt"+s,      wPt,   evtweight_, h_1d, ";p_{T}(l,MET) [GeV]", 200, 0, 1000);
  plot1D("h_deltaPhiMETminusLep"+s, t.met_phi - softlepphi_ ,   evtweight_, h_1d, ";#Delta#phi_{MET-Lep}", 64, -3.2, 3.2);
  plot1D("h_deltaPhiWminusLep"+s, TMath::ATan(wY/wX) - softlepphi_ ,   evtweight_, h_1d, ";#Delta#phi_{W-Lep}", 64, -3.2, 3.2);

  //dPhi for diff Wpt cuts
  if (wPt < 100 && wPt > 50) plot1D("h_deltaPhiWminusLep_W50t100"+s, TMath::ATan(wY/wX) - softlepphi_ ,   evtweight_, h_1d, ";#Delta#phi_{W-Lep}", 64, -3.2, 3.2);
  if (wPt < 200 && wPt > 100) plot1D("h_deltaPhiWminusLep_W100t200"+s, TMath::ATan(wY/wX) - softlepphi_ ,   evtweight_, h_1d, ";#Delta#phi_{W-Lep}", 64, -3.2, 3.2);
  if (wPt < 300 && wPt > 200) plot1D("h_deltaPhiWminusLep_W200t300"+s, TMath::ATan(wY/wX) - softlepphi_ ,   evtweight_, h_1d, ";#Delta#phi_{W-Lep}", 64, -3.2, 3.2);
  if (wPt < 500 && wPt > 300) plot1D("h_deltaPhiWminusLep_W300t500"+s, TMath::ATan(wY/wX) - softlepphi_ ,   evtweight_, h_1d, ";#Delta#phi_{W-Lep}", 64, -3.2, 3.2);

  if (softlepId_ > 0) {
    plot1D("h_deltaPhiWminusLepMinus"+s, TMath::ATan(wY/wX) - softlepphi_ ,   evtweight_, h_1d, ";#Delta#phi_{W-Lep}", 64, -3.2, 3.2);
    if (wPt < 100 && wPt > 50) plot1D("h_deltaPhiWminusLepMinus_W50t100"+s, TMath::ATan(wY/wX) - softlepphi_ ,   evtweight_, h_1d, ";#Delta#phi_{W-Lep}", 64, -3.2, 3.2);
    if (wPt < 200 && wPt > 100) plot1D("h_deltaPhiWminusLepMinus_W100t200"+s, TMath::ATan(wY/wX) - softlepphi_ ,   evtweight_, h_1d, ";#Delta#phi_{W-Lep}", 64, -3.2, 3.2);
    if (wPt < 300 && wPt > 200) plot1D("h_deltaPhiWminusLepMinus_W200t300"+s, TMath::ATan(wY/wX) - softlepphi_ ,   evtweight_, h_1d, ";#Delta#phi_{W-Lep}", 64, -3.2, 3.2);
    if (wPt < 500 && wPt > 300) plot1D("h_deltaPhiWminusLepMinus_W300t500"+s, TMath::ATan(wY/wX) - softlepphi_ ,   evtweight_, h_1d, ";#Delta#phi_{W-Lep}", 64, -3.2, 3.2);
  }
  if (softlepId_ < 0) {
    plot1D("h_deltaPhiWminusLepPlus"+s, TMath::ATan(wY/wX) - softlepphi_ ,   evtweight_, h_1d, ";#Delta#phi_{W-Lep}", 64, -3.2, 3.2);
    if (wPt < 100 && wPt > 50) plot1D("h_deltaPhiWminusLepPlus_W50t100"+s, TMath::ATan(wY/wX) - softlepphi_ ,   evtweight_, h_1d, ";#Delta#phi_{W-Lep}", 64, -3.2, 3.2);
    if (wPt < 200 && wPt > 100) plot1D("h_deltaPhiWminusLepPlus_W100t200"+s, TMath::ATan(wY/wX) - softlepphi_ ,   evtweight_, h_1d, ";#Delta#phi_{W-Lep}", 64, -3.2, 3.2);
    if (wPt < 300 && wPt > 200) plot1D("h_deltaPhiWminusLepPlus_W200t300"+s, TMath::ATan(wY/wX) - softlepphi_ ,   evtweight_, h_1d, ";#Delta#phi_{W-Lep}", 64, -3.2, 3.2);
    if (wPt < 500 && wPt > 300) plot1D("h_deltaPhiWminusLepPlus_W300t500"+s, TMath::ATan(wY/wX) - softlepphi_ ,   evtweight_, h_1d, ";#Delta#phi_{W-Lep}", 64, -3.2, 3.2);
  }

  
  outfile_->cd();

  fillHistos(h_1d, n_mt2bins, mt2bins, dirname, s);
  return;
}

void MT2Looper::fillHistosDoubleLepton(std::map<std::string, TH1*>& h_1d, int n_mt2bins, float* mt2bins, const std::string& dirname, const std::string& s) {
  TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
  if (dir == 0) {
    dir = outfile_->mkdir(dirname.c_str());
  } 
  dir->cd();

  plot1D("h_lep1pt"+s,      lep1pt_,   evtweight_, h_1d, ";p_{T}(lep) [GeV]", 200, 0, 1000);
  plot1D("h_lep1ptshort"+s,      lep1pt_,   evtweight_, h_1d, ";p_{T}(lep) [GeV]", 30, 0, 30);
  plot1D("h_lep1phi"+s,      lep1phi_,   evtweight_, h_1d, "phi",  64, -3.2, 3.2);
  plot1D("h_lep1eta"+s,      lep1eta_,   evtweight_, h_1d, "eta",  60, -3, 3);
  plot1D("h_lep2pt"+s,      lep2pt_,   evtweight_, h_1d, ";p_{T}(lep) [GeV]", 200, 0, 1000);
  plot1D("h_lep2ptshort"+s,      lep2pt_,   evtweight_, h_1d, ";p_{T}(lep) [GeV]", 30, 0, 30);
  plot1D("h_lep2phi"+s,      lep2phi_,   evtweight_, h_1d, "phi",  64, -3.2, 3.2);
  plot1D("h_lep2eta"+s,      lep2eta_,   evtweight_, h_1d, "eta",  60, -3, 3);
  
  //compute mt with softest lepton for soft lep
  float lowpt = -1;
  float lowphi = -1;
  float highpt = -1;
  float highphi = -1;
  if (lep1pt_ < lep2pt_) {
    lowpt = lep1pt_;
    lowphi = lep1phi_;
    highpt = lep2pt_;
    highphi = lep2phi_;
  }
  else {
    lowpt = lep2pt_;
    lowphi = lep2phi_; 
    highpt = lep1pt_;
    highphi = lep1phi_; 
  }
  float mt = sqrt( 2 * t.met_pt * lowpt * ( 1 - cos( t.met_phi - lowphi) ) );

  plot1D("h_mt"+s,            mt,   evtweight_, h_1d, ";M_{T} [GeV]", 500, 0, 500);
  plot1D("h_lowleppt"+s,      lowpt,   evtweight_, h_1d, ";p_{T}(lep) [GeV]", 30, 0, 30);
  plot1D("h_highleppt"+s,      highpt,   evtweight_, h_1d, ";p_{T}(lep) [GeV]", 200, 0, 1000);

  //save type
  int type = -1;
  if (lep1pt_ < 20 && lep2pt_ < 20) type = 0;
  else if (lep1pt_ > 20 && lep2pt_ < 20) type = 1;
  else if (lep1pt_ < 20 && lep2pt_ > 20) type = 1;
  //else if (lep1pt_ > 20 && lep2pt_ > 20) type = 2;
  plot1D("h_type"+s,      type,   evtweight_, h_1d, ";type", 2, 0, 2);

  
  outfile_->cd();

  fillHistos(h_1d, n_mt2bins, mt2bins, dirname, s);
  return;
}

void MT2Looper::fillHistosSingleLepton(std::map<std::string, TH1*>& h_1d, int n_mt2bins, float* mt2bins, const std::string& dirname, const std::string& s) {
  TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
  if (dir == 0) {
    dir = outfile_->mkdir(dirname.c_str());
  } 
  dir->cd();

  if (!doMinimalPlots) {
    plot1D("h_leppt"+s,      leppt_,   evtweight_, h_1d, ";p_{T}(lep) [GeV]", 200, 0, 1000);
    plot1D("h_mt"+s,            mt_,   evtweight_, h_1d, ";M_{T} [GeV]", 200, 0, 1000);
  }
  
  outfile_->cd();

  fillHistos(h_1d, n_mt2bins, mt2bins, dirname, s);
  return;
}

void MT2Looper::fillHistosGammaJets(std::map<std::string, TH1*>& h_1d, std::map<std::string, RooDataSet*>& datasets, int n_mt2bins, float* mt2bins, const std::string& dirname, const std::string& s) {
  TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
  if (dir == 0) {
    dir = outfile_->mkdir(dirname.c_str());
  } 
  dir->cd();

  //bins for FR
  const int n_FRhtbins = 6;
  const int n_FRptbins = 5;
  const float FRhtbins[n_FRhtbins+1] = {0,200,450,1000,1500,2000,3000};
  const float FRptbins[n_FRptbins+1] = {0,150,300,450,600,1500};

  float iso = t.gamma_chHadIso[0] + t.gamma_phIso[0];
  float chiso = t.gamma_chHadIso[0];

  //RooRealVars for unbinned data hist
  x_->setVal(chiso);
  w_->setVal(evtweight_);
 
  plot1D("h_iso"+s,      iso,   evtweight_, h_1d, ";iso [GeV]", 100, 0, 50);
  plot1D("h_chiso"+s,      chiso,   evtweight_, h_1d, ";iso [GeV]", 100, 0, 50);
  if (fabs(t.gamma_eta[0]) < 1.479) plot1D("h_chisoEB"+s,      chiso,   evtweight_, h_1d, ";iso [GeV]", 100, 0, 50);
  else plot1D("h_chisoEE"+s,      chiso,   evtweight_, h_1d, ";iso [GeV]", 100, 0, 50);
  plot1D("h_isoW1"+s,      iso,   1, h_1d, ";iso [GeV]", 100, 0, 50);
  plot1D("h_chisoW1"+s,      chiso,   1, h_1d, ";ch iso [GeV]", 100, 0, 50);

  plotRooDataSet("rds_chIso_"+s, x_, w_, evtweight_, datasets, "");

  //for FR calculation
  //if( (t.evt_id>110 && t.evt_id<120) || t.isData){ //only use qcd samples with pt>=470 to compute FR
  if(t.gamma_idCutBased[0]){ //passSieie
    plot2D("h2d_gammaht_gammapt"+s, t.gamma_ht, t.gamma_pt[0], evtweight_, h_1d, ";H_{T} [GeV];gamma p_{T} [GeV]",n_FRhtbins,FRhtbins,n_FRptbins,FRptbins);
    plot2D("h2d_gammaht_gammaptW1"+s, t.gamma_ht, t.gamma_pt[0], 1, h_1d, ";H_{T} [GeV];gamma p_{T} [GeV]",n_FRhtbins,FRhtbins,n_FRptbins,FRptbins);
  }
  if(!t.gamma_idCutBased[0]){ //!passSieie
    plot2D("h2d_gammaht_gammaptSingleBin"+s, t.gamma_ht, t.gamma_pt[0], evtweight_, h_1d, ";H_{T} [GeV];gamma p_{T} [GeV]",1,0,3000,1,0,1500);
    plot2D("h2d_gammaht_gammaptSingleBinW1"+s, t.gamma_ht, t.gamma_pt[0], 1, h_1d, ";H_{T} [GeV];gamma p_{T} [GeV]",1,0,3000,1,0,1500);
  }
  //  }
    float gamma_mt2_temp = t.gamma_mt2;
  // workaround for monojet bins
  if (t.gamma_nJet30 == 1) gamma_mt2_temp = t.gamma_ht;

//  for (int i = 0; i < SRBase.GetNumberOfMT2Bins(); i++) {
//    if ( gamma_mt2_temp > SRBase.GetMT2Bins()[i] &&  gamma_mt2_temp < SRBase.GetMT2Bins()[i+1]) {
//      plotRooDataSet("rds_chIso_"+toString(SRBase.GetMT2Bins()[i])+s, x_, w_, evtweight_, datasets, "");
//      plot1D("h_chiso_mt2bin"+toString(SRBase.GetMT2Bins()[i])+s,  iso,  evtweight_, h_1d, "; iso", 100, 0, 50);
//      plot2D("h2d_gammaht_gammapt"+toString(SRBase.GetMT2Bins()[i])+s, t.gamma_ht, t.gamma_pt[0], evtweight_, h_1d, ";H_{T} [GeV];gamma p_{T} [GeV]",n_FRhtbins,FRhtbins,n_FRptbins,FRptbins);
//      //plot2D("h2d_gammaht_gammaptW1"+toString(SRBase.GetMT2Bins()[i])+s, t.gamma_ht, t.gamma_pt[0], evtweight_, h_1d, ";H_{T} [GeV];gamma p_{T} [GeV]",n_FRhtbins,FRhtbins,n_FRptbins,FRptbins);	
//      plot2D("h2d_gammaht_gammaptSingleBin"+toString(SRBase.GetMT2Bins()[i])+s, t.gamma_ht, t.gamma_pt[0], evtweight_, h_1d, ";H_{T} [GeV];gamma p_{T} [GeV]",1,0,3000,1,0,1500);
//      //plot2D("h2d_gammaht_gammaptSingleBinW1"+toString(SRBase.GetMT2Bins()[i])+s, t.gamma_ht, t.gamma_pt[0], evtweight_, h_1d, ";H_{T} [GeV];gamma p_{T} [GeV]",1,0,3000,1,0,1500);
//    }
//  }

  for (int i = 0; i < n_mt2bins; i++) {
    if ( gamma_mt2_temp > mt2bins[i] &&  gamma_mt2_temp < mt2bins[i+1]) {
      plotRooDataSet("rds_chIso_"+toString(mt2bins[i])+s, x_, w_, evtweight_, datasets, "");
      plot1D("h_chiso_mt2bin"+toString(mt2bins[i])+s,  iso,  evtweight_, h_1d, "; iso", 100, 0, 50);
      plot2D("h2d_gammaht_gammapt"+toString(mt2bins[i])+s, t.gamma_ht, t.gamma_pt[0], evtweight_, h_1d, ";H_{T} [GeV];gamma p_{T} [GeV]",n_FRhtbins,FRhtbins,n_FRptbins,FRptbins);
      //plot2D("h2d_gammaht_gammaptW1"+toString(mt2bins[i])+s, t.gamma_ht, t.gamma_pt[0], evtweight_, h_1d, ";H_{T} [GeV];gamma p_{T} [GeV]",n_FRhtbins,FRhtbins,n_FRptbins,FRptbins);	
      plot2D("h2d_gammaht_gammaptSingleBin"+toString(mt2bins[i])+s, t.gamma_ht, t.gamma_pt[0], evtweight_, h_1d, ";H_{T} [GeV];gamma p_{T} [GeV]",1,0,3000,1,0,1500);
      //plot2D("h2d_gammaht_gammaptSingleBinW1"+toString(mt2bins[i])+s, t.gamma_ht, t.gamma_pt[0], evtweight_, h_1d, ";H_{T} [GeV];gamma p_{T} [GeV]",1,0,3000,1,0,1500);
    }
  }

  //cout<<"Event "<<t.evt<<" with weight "<< evtweight_ <<" is in sr "<<dirname<<endl;
  plot1D("h_mt2bins"+s,       gamma_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
  plot1D("h_htbins"+s,       t.gamma_ht,   evtweight_, h_1d, ";H_{T} [GeV]", n_htbins, htbins);
  plot1D("h_htbins2"+s,       t.gamma_ht,   evtweight_, h_1d, ";H_{T} [GeV]", n_htbins2, htbins2);
  plot1D("h_njbins"+s,       t.gamma_nJet30,   evtweight_, h_1d, ";N(jets)", n_njbins, njbins);
  plot1D("h_nbjbins"+s,       t.gamma_nBJet20,   evtweight_, h_1d, ";N(bjets)", n_nbjbins, nbjbins);

  if ( (dirname=="crgjnocut" || TString(dirname).Contains("crgjbase") || dirname=="crgjL" || dirname=="crgjM" || dirname=="crgjH") 
       && (s=="" || s=="Fake" || s=="FragGJ" || s=="AllIso" || s=="LooseNotTight") )// Don't make these for Loose, NotLoose. SieieSB
    {
    plot1D("h_Events"+s,  1, 1, h_1d, ";Events, Unweighted", 1, 0, 2);
    plot1D("h_Events_w"+s,  1,   evtweight_, h_1d, ";Events, Weighted", 1, 0, 2);
    plot1D("h_mt2"+s,       gamma_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", 150, 0, 1500);
    plot1D("h_met"+s,       t.gamma_met_pt,   evtweight_, h_1d, ";E_{T}^{miss} [GeV]", 150, 0, 1500);
    plot1D("h_simplemet"+s,       t.met_pt,   evtweight_, h_1d, ";E_{T}^{miss} [GeV]", 150, 0, 1500);
    plot1D("h_gammaPt"+s,       t.gamma_pt[0],   evtweight_, h_1d, ";gamma p_{T} [GeV]", 264, 180, 1500);
    if (fabs(t.gamma_eta[0]) < 1.479) plot1D("h_gammaPtEB"+s,       t.gamma_pt[0],   evtweight_, h_1d, ";gamma p_{T} [GeV]", 264, 180, 1500);
    else plot1D("h_gammaPtEE"+s,       t.gamma_pt[0],   evtweight_, h_1d, ";gamma p_{T} [GeV]", 264, 180, 1500);
    plot1D("h_gammaEta"+s,       t.gamma_eta[0],   evtweight_, h_1d, ";gamma #eta [GeV]", 50, -2.5, 2.5);
    if (t.HLT_Photons) plot1D("h_gammaPt_HLT"+s,       t.gamma_pt[0],   evtweight_, h_1d, ";gamma p_{T} [GeV]", 300, 0, 1500);
    plot1D("h_ht"+s,       t.gamma_ht,   evtweight_, h_1d, ";H_{T} [GeV]", 120, 0, 3000);
    plot1D("h_nJet30"+s,       t.gamma_nJet30,   evtweight_, h_1d, ";N(jets)", 15, 0, 15);
    plot1D("h_nBJet20"+s,      t.gamma_nBJet20,   evtweight_, h_1d, ";N(bjets)", 6, 0, 6);
    plot1D("h_deltaPhiMin"+s,  t.gamma_deltaPhiMin,   evtweight_, h_1d, ";#Delta#phi_{min}", 32, 0, 3.2);
    plot1D("h_diffMetMht"+s,   t.gamma_diffMetMht,   evtweight_, h_1d, ";|E_{T}^{miss} - MHT| [GeV]", 120, 0, 300);
    plot1D("h_diffMetMhtOverMet"+s,   t.gamma_diffMetMht/t.gamma_met_pt,   evtweight_, h_1d, ";|E_{T}^{miss} - MHT| / E_{T}^{miss}", 100, 0, 2.);
    plot1D("h_minMTBMet"+s,   t.gamma_minMTBMet,   evtweight_, h_1d, ";min M_{T}(b, E_{T}^{miss}) [GeV]", 150, 0, 1500);
    plot1D("h_nlepveto"+s,     nlepveto_,   evtweight_, h_1d, ";N(leps)", 10, 0, 10);

    plot1D("h_drMinParton"+s,   t.gamma_drMinParton[0],   evtweight_, h_1d, ";DRmin(photon, parton)", 100, 0, 5);
    TString drName = "h_drMinPartonSample";
    drName += TString::Itoa(t.evt_id, 10);
    drName += s;
    plot1D(drName.Data(),   t.gamma_drMinParton[0],   evtweight_, h_1d, ";DRmin(photon, parton)", 100, 0, 5);
    if (t.gamma_ht > 200) 
      {
	plot1D("h_bosonptbins"+s,      t.gamma_pt[0],   evtweight_, h_1d, ";p_{T}^{V} [GeV]", n_ptVbins, ptVbins);
	plot1D("h_bosonpt"+s,      t.gamma_pt[0],   evtweight_, h_1d, ";p_{T}^{V} [GeV]",300, 0, 1500);
	if ( fabs(t.gamma_eta[0])<1.4 ) plot1D("h_bosonptbinsCentral"+s,      t.gamma_pt[0],   evtweight_, h_1d, ";p_{T}^{V} [GeV]", n_ptVbins, ptVbins);
      }
    }

  outfile_->cd();
  return;
}

void MT2Looper::fillHistosDY(std::map<std::string, TH1*>& h_1d, int n_mt2bins, float* mt2bins, const std::string& dirname, const std::string& s) {
  TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
  if (dir == 0) {
    dir = outfile_->mkdir(dirname.c_str());
  } 
  dir->cd();

  // workaround for monojet bins
  float zll_mt2_temp = t.zll_mt2;
  if (t.nJet30 == 1) zll_mt2_temp = t.zll_ht;

  plot1D("h_mt2bins"+s,       zll_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
  
  if (dirname=="crdynocut" || TString(dirname).Contains("crdybase") || dirname=="crdyL" || dirname=="crdyM" || dirname=="crdyH") {
    plot1D("h_Events"+s,  1, 1, h_1d, ";Events, Unweighted", 1, 0, 2);
    plot1D("h_Events_w"+s,  1,   evtweight_, h_1d, ";Events, Weighted", 1, 0, 2);
    plot1D("h_mt2"+s,       zll_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", 150, 0, 1500);
    plot1D("h_met"+s,       t.zll_met_pt,   evtweight_, h_1d, ";E_{T}^{miss} [GeV]", 150, 0, 1500);
    plot1D("h_ht"+s,       t.zll_ht,   evtweight_, h_1d, ";H_{T} [GeV]", 120, 0, 3000);
    plot1D("h_nJet30"+s,       t.nJet30,   evtweight_, h_1d, ";N(jets)", 15, 0, 15);
    plot1D("h_nBJet20"+s,      t.nBJet20,   evtweight_, h_1d, ";N(bjets)", 6, 0, 6);
    plot1D("h_deltaPhiMin"+s,  t.zll_deltaPhiMin,   evtweight_, h_1d, ";#Delta#phi_{min}", 32, 0, 3.2);
    plot1D("h_diffMetMht"+s,   t.zll_diffMetMht,   evtweight_, h_1d, ";|E_{T}^{miss} - MHT| [GeV]", 120, 0, 300);
    plot1D("h_diffMetMhtOverMet"+s,   t.zll_diffMetMht/t.zll_met_pt,   evtweight_, h_1d, ";|E_{T}^{miss} - MHT| / E_{T}^{miss}", 100, 0, 2.);
    plot1D("h_minMTBMet"+s,   t.zll_minMTBMet,   evtweight_, h_1d, ";min M_{T}(b, E_{T}^{miss}) [GeV]", 150, 0, 1500);
    plot1D("h_nlepveto"+s,     t.nLepLowMT,   evtweight_, h_1d, ";N(leps)", 10, 0, 10);
    plot1D("h_htbins"+s,       t.zll_ht,   evtweight_, h_1d, ";H_{T} [GeV]", n_htbins, htbins);
    plot1D("h_htbins2"+s,       t.zll_ht,   evtweight_, h_1d, ";H_{T} [GeV]", n_htbins2, htbins2);
    if (t.nJet30>7) cout<<"event "<<t.evt<<" in run "<<t.run<<" has njets "<<t.nJet30<<" and ht "<<t.zll_ht<<endl;
    plot1D("h_njbins"+s,       t.nJet30,   evtweight_, h_1d, ";N(jets)", n_njbins, njbins);
    plot1D("h_nbjbins"+s,       t.nBJet20,   evtweight_, h_1d, ";N(bjets)", n_nbjbins, nbjbins);
    plot1D("h_leppt1"+s,      t.lep_pt[0],   evtweight_, h_1d, ";p_{T}(lep1) [GeV]", 50, 0, 200);
    plot1D("h_leppt2"+s,      t.lep_pt[1],   evtweight_, h_1d, ";p_{T}(lep2) [GeV]", 50, 0, 200);
    plot1D("h_zllmass"+s,      t.zll_mass,   evtweight_, h_1d, ";m_{ll} [GeV]", 60, 0, 120);
    if (abs(t.lep_pdgId[0])==11) plot1D("h_zllmassEle"+s,      t.zll_mass,   evtweight_, h_1d, ";m_{ll} [GeV]", 60, 0, 120);
    if (abs(t.lep_pdgId[0])==13) plot1D("h_zllmassMu"+s,      t.zll_mass,   evtweight_, h_1d, ";m_{ll} [GeV]", 60, 0, 120);
    if (t.zll_ht > 200) 
      {
	plot1D("h_bosonptbins"+s,      t.zll_pt,   evtweight_, h_1d, ";p_{T}^{V} [GeV]", n_ptVbins, ptVbins);
	TLorentzVector Zll(0,0,0,0);
	Zll.SetPtEtaPhiM(t.zll_pt, t.zll_eta, t.zll_phi, t.zll_mass);
	float Zrapidity = Zll.Rapidity();
	if ( fabs(Zrapidity)<1.4 ) plot1D("h_bosonptbinsCentral"+s,      t.zll_pt,   evtweight_, h_1d, ";p_{T}^{V} [GeV]", n_ptVbins, ptVbins);
      }
  }

  outfile_->cd();
  return;
}

void MT2Looper::fillHistosRemovedLepton(std::map<std::string, TH1*>& h_1d, int n_mt2bins, float* mt2bins, const std::string& dirname, const std::string& s) {
  TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
  if (dir == 0) {
    dir = outfile_->mkdir(dirname.c_str());
  } 
  dir->cd();

  // workaround for monojet bins
  float rl_mt2_temp = t.rl_mt2;
  if (t.nJet30 == 1) rl_mt2_temp = t.rl_ht;

  plot1D("h_mt2bins"+s,       rl_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);

  if (dirname=="crrlnocut" || TString(dirname).Contains("crrlbase") || dirname=="crrlL" || dirname=="crrlM" || dirname=="crrlH" ||
      dirname=="crrlelnocut" || dirname=="crrlelbase" || dirname=="crrlelL" || dirname=="crrlelM" || dirname=="crrlelH" ||
      dirname=="crrlmunocut" || dirname=="crrlmubase" || dirname=="crrlmuL" || dirname=="crrlmuM" || dirname=="crrlmuH" ) { 

    float mt = MT(t.lep_pt[0],t.lep_phi[0],t.met_pt,t.met_phi);
    plot1D("h_Events"+s,  1, 1, h_1d, ";Events, Unweighted", 1, 0, 2);
    plot1D("h_Events_w"+s,  1,   evtweight_, h_1d, ";Events, Weighted", 1, 0, 2);
    plot1D("h_mt2"+s,       rl_mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", 150, 0, 1500);
    plot1D("h_met"+s,       t.rl_met_pt,   evtweight_, h_1d, ";E_{T}^{miss} [GeV]", 150, 0, 1500);
    plot1D("h_ht"+s,       t.rl_ht,   evtweight_, h_1d, ";H_{T} [GeV]", 120, 0, 3000);
    plot1D("h_nJet30"+s,       t.nJet30,   evtweight_, h_1d, ";N(jets)", 15, 0, 15);
    plot1D("h_nBJet20"+s,      t.nBJet20,   evtweight_, h_1d, ";N(bjets)", 6, 0, 6);
    plot1D("h_deltaPhiMin"+s,  t.rl_deltaPhiMin,   evtweight_, h_1d, ";#Delta#phi_{min}", 32, 0, 3.2);
    plot1D("h_diffMetMht"+s,   t.rl_diffMetMht,   evtweight_, h_1d, ";|E_{T}^{miss} - MHT| [GeV]", 120, 0, 300);
    plot1D("h_diffMetMhtOverMet"+s,   t.rl_diffMetMht/t.rl_met_pt,   evtweight_, h_1d, ";|E_{T}^{miss} - MHT| / E_{T}^{miss}", 100, 0, 2.);
    plot1D("h_minMTBMet"+s,   t.rl_minMTBMet,   evtweight_, h_1d, ";min M_{T}(b, E_{T}^{miss}) [GeV]", 150, 0, 1500);
    plot1D("h_nlepveto"+s,     t.nLepLowMT,   evtweight_, h_1d, ";N(leps)", 10, 0, 10);
    plot1D("h_htbins"+s,       t.rl_ht,   evtweight_, h_1d, ";H_{T} [GeV]", n_htbins, htbins);
    plot1D("h_htbins2"+s,       t.rl_ht,   evtweight_, h_1d, ";H_{T} [GeV]", n_htbins2, htbins2);
    plot1D("h_njbins"+s,       t.nJet30,   evtweight_, h_1d, ";N(jets)", n_njbins, njbins);
    plot1D("h_nbjbins"+s,       t.nBJet20,   evtweight_, h_1d, ";N(bjets)", n_nbjbins, nbjbins);
    plot1D("h_leppt"+s,      t.lep_pt[0],   evtweight_, h_1d, ";p_{T}(lep) [GeV]", 200, 0, 1000);
    plot1D("h_lepeta"+s,      t.lep_eta[0],   evtweight_, h_1d, "#eta(lep) [GeV]", 50, -2.5, 2.5);
    plot1D("h_mt"+s,      mt,   evtweight_, h_1d, ";m_{T}(lep, MET) [GeV]", 200, 0, 400);
    plot1D("h_lepreliso"+s,      t.lep_relIso03[0],   evtweight_, h_1d, ";RelIso03", 50, 0, 1);
    plot1D("h_lepabsiso"+s,      t.lep_relIso03[0]*t.lep_pt[0],   evtweight_, h_1d, ";AbsIso03 [GeV]", 50, 0, 50);
    plot1D("h_simplemet"+s,      t.met_pt,   evtweight_, h_1d, ";E_{T}^{miss} [GeV]", 200, 0, 1000);
  }//dirname

  outfile_->cd();
  return;
}

void MT2Looper::fillHistosQCD(std::map<std::string, TH1*>& h_1d, int n_mt2bins, float* mt2bins, const std::string& dirname, const std::string& s) {
  TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
  if (dir == 0) {
    dir = outfile_->mkdir(dirname.c_str());
  } 
  dir->cd();

  // workaround for monojet bins
  float mt2_temp = t.mt2;
  TString directoryname(dirname);
  // to include QCD estimate for monojet region
  if (t.nJet30 == 1 || directoryname.Contains("J")) mt2_temp = t.jet1_pt;

  plot1D("h_Events"+s,  1, 1, h_1d, ";Events, Unweighted", 1, 0, 2);
  plot1D("h_Events_w"+s,  1,   evtweight_, h_1d, ";Events, Weighted", 1, 0, 2);
  plot1D("h_mt2bins"+s,       mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
  plot1D("h_htbins"+s,       t.ht,   evtweight_, h_1d, ";H_{T} [GeV]", n_htbins, htbins);
  plot1D("h_htbins2"+s,       t.ht,   evtweight_, h_1d, ";H_{T} [GeV]", n_htbins2, htbins2);
  plot1D("h_njbins"+s,       t.nJet30,   evtweight_, h_1d, ";N(jets)", n_njbins, njbins);
  plot1D("h_nbjbins"+s,       t.nBJet20,   evtweight_, h_1d, ";N(bjets)", n_nbjbins, nbjbins);
  if (!doMinimalPlots) {
    plot1D("h_mt2"+s,       mt2_temp,   evtweight_, h_1d, "; M_{T2} [GeV]", 150, 0, 1500);
    plot1D("h_met"+s,       t.met_pt,   evtweight_, h_1d, ";E_{T}^{miss} [GeV]", 150, 0, 1500);
    plot1D("h_ht"+s,       t.ht,   evtweight_, h_1d, ";H_{T} [GeV]", 120, 0, 3000);
    plot1D("h_nJet30"+s,       t.nJet30,   evtweight_, h_1d, ";N(jets)", 15, 0, 15);
    plot1D("h_nJet30Eta3"+s,       nJet30Eta3_,   evtweight_, h_1d, ";N(jets, |#eta| > 3.0)", 10, 0, 10);
    plot1D("h_nBJet20"+s,      t.nBJet20,   evtweight_, h_1d, ";N(bjets)", 6, 0, 6);
    plot1D("h_deltaPhiMin"+s,  t.deltaPhiMin,   evtweight_, h_1d, ";#Delta#phi_{min}", 32, 0, 3.2);
    plot1D("h_diffMetMht"+s,   t.diffMetMht,   evtweight_, h_1d, ";|E_{T}^{miss} - MHT| [GeV]", 120, 0, 300);
    plot1D("h_diffMetMhtOverMet"+s,   t.diffMetMht/t.met_pt,   evtweight_, h_1d, ";|E_{T}^{miss} - MHT| / E_{T}^{miss}", 100, 0, 2.);
    plot1D("h_minMTBMet"+s,   t.minMTBMet,   evtweight_, h_1d, ";min M_{T}(b, E_{T}^{miss}) [GeV]", 150, 0, 1500);
    plot1D("h_nlepveto"+s,     nlepveto_,   evtweight_, h_1d, ";N(leps)", 10, 0, 10);
    plot1D("h_J0pt"+s,       t.jet1_pt,   evtweight_, h_1d, ";p_{T}(jet1) [GeV]", 150, 0, 1500);
    plot1D("h_J1pt"+s,       t.jet2_pt,   evtweight_, h_1d, ";p_{T}(jet2) [GeV]", 150, 0, 1500);
  }
  
  outfile_->cd();
  return;
}

void MT2Looper::fillLepUncSR() {

  // lepton efficiency variation in signal region for fastsim: large uncertainty on leptons NOT vetoed
  cor_lepeff_sr_ = 0.;
  unc_lepeff_sr_ = 0.;
  if (t.ngenLep > 0 || t.ngenLepFromTau > 0) {
    // loop on gen e/mu
    for (int ilep = 0; ilep < t.ngenLep; ++ilep) {
      // check acceptance for veto: pt > 5, |eta| < 2.4
      if (t.genLep_pt[ilep] < 5.) continue;
      if (fabs(t.genLep_eta[ilep]) > 2.4) continue;
      // look up SF and vetoeff, by flavor
      weightStruct sf_struct = getLepSFFromFile(t.genLep_pt[ilep], t.genLep_eta[ilep], t.genLep_pdgId[ilep]);
      float sf = sf_struct.cent;
      float vetoeff = getLepVetoEffFromFile_fullsim(t.genLep_pt[ilep], t.genLep_eta[ilep], t.genLep_pdgId[ilep]);
      float unc = sf_struct.up - sf;
      float vetoeff_unc_UP = vetoeff * (1. + unc);
      float unc_UP_0l = ( (1. - vetoeff_unc_UP) / (1. - vetoeff) ) - 1.;
      unc_lepeff_sr_ += unc_UP_0l;
    }
    for (int ilep = 0; ilep < t.ngenLepFromTau; ++ilep) {
      // check acceptance for veto: pt > 5
      if (t.genLepFromTau_pt[ilep] < 5.) continue;
      if (fabs(t.genLepFromTau_eta[ilep]) > 2.4) continue;
      weightStruct sf_struct = getLepSFFromFile(t.genLepFromTau_pt[ilep], t.genLepFromTau_eta[ilep], t.genLepFromTau_pdgId[ilep]);
      float sf = sf_struct.cent;
      float vetoeff = getLepVetoEffFromFile_fullsim(t.genLepFromTau_pt[ilep], t.genLepFromTau_eta[ilep], t.genLepFromTau_pdgId[ilep]);
      float unc = sf_struct.up - sf;
      float vetoeff_unc_UP = vetoeff * (1. + unc);
      float unc_UP_0l = ( (1. - vetoeff_unc_UP) / (1. - vetoeff) ) - 1.;
      unc_lepeff_sr_ += unc_UP_0l;
    }
  }

  return;
}

void MT2Looper::fillLepCorSRfastsim() {

  // lepton efficiency variation in signal region for fastsim: large uncertainty on leptons NOT vetoed
  cor_lepeff_sr_ = 0.;
  unc_lepeff_sr_ = 0.;
  if (t.ngenLep > 0 || t.ngenLepFromTau > 0) {
    // loop on gen e/mu
    for (int ilep = 0; ilep < t.ngenLep; ++ilep) {
      // check acceptance for veto: pt > 5, |eta| < 2.4
      if (t.genLep_pt[ilep] < 5.) continue;
      if (fabs(t.genLep_eta[ilep]) > 2.4) continue;
      // look up SF and vetoeff, by flavor
      weightStruct sf_struct = getLepSFFromFile_fastsim(t.genLep_pt[ilep], t.genLep_eta[ilep], t.genLep_pdgId[ilep]);
      float sf = sf_struct.cent;
      float vetoeff = getLepVetoEffFromFile_fastsim(t.genLep_pt[ilep], t.genLep_eta[ilep], t.genLep_pdgId[ilep]);
      // apply SF to vetoeff, then correction for 0L will be (1 - vetoeff_cor) / (1 - vetoeff) - 1.
      float vetoeff_cor = vetoeff * sf;
      float cor_0l = ( (1. - vetoeff_cor) / (1. - vetoeff) ) - 1.;
      cor_lepeff_sr_ += cor_0l;
      float unc = sf_struct.up - sf;
      float vetoeff_cor_unc_UP = vetoeff_cor * (1. + unc);
      float unc_UP_0l = ( (1. - vetoeff_cor_unc_UP) / (1. - vetoeff_cor) ) - 1.;
      unc_lepeff_sr_ += unc_UP_0l;
    }
    for (int ilep = 0; ilep < t.ngenLepFromTau; ++ilep) {
      // check acceptance for veto: pt > 5
      if (t.genLepFromTau_pt[ilep] < 5.) continue;
      if (fabs(t.genLepFromTau_eta[ilep]) > 2.4) continue;
      weightStruct sf_struct = getLepSFFromFile_fastsim(t.genLepFromTau_pt[ilep], t.genLepFromTau_eta[ilep], t.genLepFromTau_pdgId[ilep]);
      float sf = sf_struct.cent;
      float vetoeff = getLepVetoEffFromFile_fastsim(t.genLepFromTau_pt[ilep], t.genLepFromTau_eta[ilep], t.genLepFromTau_pdgId[ilep]);
      // apply SF to vetoeff, then correction for 0L will be 1. - (1 - vetoeff_cor) / (1 - vetoeff)
      float vetoeff_cor = vetoeff * sf;
      float cor_0l = ( (1. - vetoeff_cor) / (1. - vetoeff) ) - 1.;
      cor_lepeff_sr_ += cor_0l;
      float unc = sf_struct.up - sf;
      float vetoeff_cor_unc_UP = vetoeff_cor * (1. + unc);
      float unc_UP_0l = ( (1. - vetoeff_cor_unc_UP) / (1. - vetoeff_cor) ) - 1.;
      unc_lepeff_sr_ += unc_UP_0l;
    }
  }

  return;
}

// C++
#include <iostream>
#include <vector>
#include <set>

// ROOT
#include "TDirectory.h"
#include "TTreeCache.h"
#include "Math/VectorUtil.h"
#include "TVector2.h"
#include "TBenchmark.h"
#include "TLorentzVector.h"
#include "TH2.h"

// CORE
#include "../CORE/CMS3.h"
#include "../CORE/Base.h"
#include "../CORE/JetSelections.h"
#include "../CORE/MuonSelections.h"
#include "../CORE/ElectronSelections.h"
#include "../CORE/PhotonSelections.h"
#include "../CORE/TriggerSelections.h"
#include "../CORE/MCSelections.h"
#include "../CORE/IsoTrackVeto.h"
#include "../CORE/IsolationTools.h"
#include "../CORE/MetSelections.h"

// Tools
#include "../CORE/Tools/utils.h"
#include "../CORE/Tools/hemJet.h" 
#include "../CORE/Tools/MT2/MT2.h"
#include "../CORE/Tools/JetCorrector.h"
#include "../CORE/Tools/jetcorr/FactorizedJetCorrector.h"
#include "../CORE/Tools/jetcorr/Utilities.icc"
#include "../CORE/Tools/jetcorr/JetCorrectionUncertainty.icc"
#include "../CORE/Tools/jetcorr/SimpleJetCorrectionUncertainty.icc"
#include "../CORE/Tools/goodrun.h"
#include "../CORE/Tools/btagsf/BTagCalibrationStandalone.h"

// MT2CORE
#include "../MT2CORE/sampleID.h"
#include "../MT2CORE/applyWeights.h"

// header
#include "ScanChain.h"

using namespace std;
using namespace tas;

// turn on to add debugging statements (default false)
const bool verbose = false;
// turn on to apply JEC from text files (default true)
const bool applyJECfromFile = true;
// change to do JEC uncertainty variations. 0 = DEFAULT, 1 = UP, -1 = DN
const int applyJECunc = 0;
// change to do unclustered energy uncertainty MET variations. 0 = DEFAULT, 1 = UP, -1 = DN
const int applyUnclusteredUnc = 0;
// turn on to apply btag SFs (default true)
const bool applyBtagSFs = true;
// turn on to recompute type1 MET using JECs from file (default true)
const bool recomputeT1MET = true;
// turn on to save prunedGenParticle collection (default false)
const bool saveGenParticles = false;
// turn on to apply trigger cuts to ntuples -> OR of all triggers used (default false)
const bool applyTriggerCuts = false;
// turn on to apply dummy weights for lepton SFs, btag SFs, etc (default false)
const bool applyDummyWeights = false;
// turn on to apply lepton SF
const bool applyLeptonSFs = true;
// turn on to apply json file to data (default true)
const bool applyJSON = true;
// for testing purposes, running on unmerged files (default false)
const bool removePostProcVars = false;
// for merging prompt reco with reMINIAOD (default true)
const bool removeEarlyPromptReco = true;
// turn on to remove jets overlapping with leptons (default true)
const bool doJetLepOverlapRemoval = true;
// turn on to save only isolated leptons (default true)
const bool applyLeptonIso = true;
// turn on to save MC scale and PDF weights (default false, makes babies ~4x as large)
const bool saveLHEweights = false;
// turn on to save MC scale weights (default false, small size impact)
const bool saveLHEweightsScaleOnly = false;

//--------------------------------------------------------------------

// This is meant to be passed as the third argument, the predicate, of the standard library sort algorithm
inline bool sortByPt(const LorentzVector &vec1, const LorentzVector &vec2 ) {
  return vec1.pt() > vec2.pt();
}

// This is meant to be passed as the third argument, the predicate, of the standard library sort algorithm
inline bool sortByValue(const std::pair<int,float>& pair1, const std::pair<int,float>& pair2 ) {
  return pair1.second > pair2.second;
}

//--------------------------------------------------------------------

void babyMaker::ScanChain(TChain* chain, std::string baby_name, int bx, bool isFastsim){

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  if (bx != 25 && bx != 50) {
    cout << "ERROR: invalid value for bx: " << bx << ".  Exiting" << endl;
    return;
  }

  isPromptReco = false;
  if (baby_name.find("data_Run2015") != std::string::npos) {
    isDataFromFileName = true;
    cout << "running on DATA, based on file name" << endl;
    if (baby_name.find("PromptReco") != std::string::npos) {
      isPromptReco = true;
      cout << "  DATA is PromptReco" << endl;
      if (removeEarlyPromptReco) cout << "  Removing runs <= 251562" << endl;
    }
  } else {
    isDataFromFileName = false;
    cout << "running on MC, based on file name" << endl;
  }

  MakeBabyNtuple( Form("%s.root", baby_name.c_str()) );

  // 25ns is hardcoded here, would need an option for 50ns
  const char* json_file = "jsons/Cert_246908-260627_13TeV_PromptReco_Collisions15_25ns_JSON_snt.txt";
  if (applyJSON) {
    cout << "Loading json file: " << json_file << endl;
    set_goodrun_file(json_file);
  }

  if (applyBtagSFs) {
    // setup btag calibration readers
    calib = new BTagCalibration("csvv2", "btagsf/CSVv2.csv"); // 25s version of SFs
    reader_heavy = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "mujets", "central"); // central
    reader_heavy_UP = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "mujets", "up");  // sys up
    reader_heavy_DN = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "mujets", "down");  // sys down
    reader_light = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "comb", "central");  // central
    reader_light_UP = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "comb", "up");  // sys up
    reader_light_DN = new BTagCalibrationReader(calib, BTagEntry::OP_MEDIUM, "comb", "down");  // sys down

    // get btag efficiencies
    TFile* f_btag_eff = new TFile("btagsf/btageff__ttbar_powheg_pythia8_25ns.root");
    TH2D* h_btag_eff_b_temp = (TH2D*) f_btag_eff->Get("h2_BTaggingEff_csv_med_Eff_b");
    TH2D* h_btag_eff_c_temp = (TH2D*) f_btag_eff->Get("h2_BTaggingEff_csv_med_Eff_c");
    TH2D* h_btag_eff_udsg_temp = (TH2D*) f_btag_eff->Get("h2_BTaggingEff_csv_med_Eff_udsg");
    BabyFile_->cd();
    h_btag_eff_b = (TH2D*) h_btag_eff_b_temp->Clone("h_btag_eff_b");
    h_btag_eff_c = (TH2D*) h_btag_eff_c_temp->Clone("h_btag_eff_c");
    h_btag_eff_udsg = (TH2D*) h_btag_eff_udsg_temp->Clone("h_btag_eff_udsg");
    f_btag_eff->Close();

    std::cout << "loaded fullsim btag SFs" << std::endl;
    
    // extra copy for fastsim -> fullsim SFs
    if (isFastsim) {
      // setup btag calibration readers
      calib_fastsim = new BTagCalibration("CSV", "btagsf/CSV_13TEV_Combined_20_11_2015.csv"); // 25ns fastsim version of SFs
      reader_fastsim = new BTagCalibrationReader(calib_fastsim, BTagEntry::OP_MEDIUM, "fastsim", "central"); // central
      reader_fastsim_UP = new BTagCalibrationReader(calib_fastsim, BTagEntry::OP_MEDIUM, "fastsim", "up");  // sys up
      reader_fastsim_DN = new BTagCalibrationReader(calib_fastsim, BTagEntry::OP_MEDIUM, "fastsim", "down");  // sys down

      // get btag efficiencies
      TFile* f_btag_eff_fastsim = new TFile("btagsf/btageff__SMS-T1bbbb-T1qqqq_fastsim.root");
      TH2D* h_btag_eff_b_fastsim_temp = (TH2D*) f_btag_eff_fastsim->Get("h2_BTaggingEff_csv_med_Eff_b");
      TH2D* h_btag_eff_c_fastsim_temp = (TH2D*) f_btag_eff_fastsim->Get("h2_BTaggingEff_csv_med_Eff_c");
      TH2D* h_btag_eff_udsg_fastsim_temp = (TH2D*) f_btag_eff_fastsim->Get("h2_BTaggingEff_csv_med_Eff_udsg");
      BabyFile_->cd();
      h_btag_eff_b_fastsim = (TH2D*) h_btag_eff_b_fastsim_temp->Clone("h_btag_eff_b_fastsim");
      h_btag_eff_c_fastsim = (TH2D*) h_btag_eff_c_fastsim_temp->Clone("h_btag_eff_c_fastsim");
      h_btag_eff_udsg_fastsim = (TH2D*) h_btag_eff_udsg_fastsim_temp->Clone("h_btag_eff_udsg_fastsim");
      f_btag_eff_fastsim->Close();
      
      std::cout << "loaded fastsim btag SFs" << std::endl;
    } // if (isFastsim)
  }

  // Lepton Scale Factors
  if (applyLeptonSFs) {
    setElSFfile("lepsf/kinematicBinSFele.root");
    setMuSFfile("lepsf/TnP_MuonID_NUM_LooseID_DENOM_generalTracks_VAR_map_pt_eta.root","lepsf/TnP_MuonID_NUM_MiniIsoTight_DENOM_LooseID_VAR_map_pt_eta.root");
  }

  // ----------------------------------
  // retrieve JEC from files, if using
  // ----------------------------------

  std::vector<std::string> jetcorr_filenames_pfL1FastJetL2L3;
  FactorizedJetCorrector *jet_corrector_pfL1FastJetL2L3(0);
  JetCorrectionUncertainty* jetcorr_uncertainty(0);

  if (applyJECfromFile) {
    jetcorr_filenames_pfL1FastJetL2L3.clear();
    std::string jetcorr_uncertainty_filename;

    // files for RunIISpring15 MC
    if (bx == 50) {
      if (isDataFromFileName) {
	jetcorr_filenames_pfL1FastJetL2L3.push_back  ("jetCorrections/Summer15_50nsV4_DATA_L1FastJet_AK4PFchs.txt");
	jetcorr_filenames_pfL1FastJetL2L3.push_back  ("jetCorrections/Summer15_50nsV4_DATA_L2Relative_AK4PFchs.txt");
	jetcorr_filenames_pfL1FastJetL2L3.push_back  ("jetCorrections/Summer15_50nsV4_DATA_L3Absolute_AK4PFchs.txt");
	jetcorr_filenames_pfL1FastJetL2L3.push_back  ("jetCorrections/Summer15_50nsV4_DATA_L2L3Residual_AK4PFchs.txt");
      } else {
	jetcorr_filenames_pfL1FastJetL2L3.push_back  ("jetCorrections/Summer15_50nsV4_MC_L1FastJet_AK4PFchs.txt");
	jetcorr_filenames_pfL1FastJetL2L3.push_back  ("jetCorrections/Summer15_50nsV4_MC_L2Relative_AK4PFchs.txt");
	jetcorr_filenames_pfL1FastJetL2L3.push_back  ("jetCorrections/Summer15_50nsV4_MC_L3Absolute_AK4PFchs.txt");
	jetcorr_uncertainty_filename = "jetCorrections/Summer15_50nsV4_DATA_Uncertainty_AK4PFchs.txt";
      }
    }
    else if (bx == 25) {
      if (isDataFromFileName) {
	jetcorr_filenames_pfL1FastJetL2L3.push_back  ("jetCorrections/Summer15_25nsV6_DATA_L1FastJet_AK4PFchs.txt");
	jetcorr_filenames_pfL1FastJetL2L3.push_back  ("jetCorrections/Summer15_25nsV6_DATA_L2Relative_AK4PFchs.txt");
	jetcorr_filenames_pfL1FastJetL2L3.push_back  ("jetCorrections/Summer15_25nsV6_DATA_L3Absolute_AK4PFchs.txt");
	jetcorr_filenames_pfL1FastJetL2L3.push_back  ("jetCorrections/Summer15_25nsV6_DATA_L2L3Residual_AK4PFchs.txt");
      } else if (isFastsim) {
	jetcorr_filenames_pfL1FastJetL2L3.push_back  ("jetCorrections/MCRUN2_74_V9_L1FastJet_AK4PFchs.txt");
	jetcorr_filenames_pfL1FastJetL2L3.push_back  ("jetCorrections/MCRUN2_74_V9_L2Relative_AK4PFchs.txt");
	jetcorr_filenames_pfL1FastJetL2L3.push_back  ("jetCorrections/MCRUN2_74_V9_L3Absolute_AK4PFchs.txt");
	jetcorr_uncertainty_filename = "jetCorrections/MCRUN2_74_V9_Uncertainty_AK4PFchs.txt"; // not sure if these are correct..
      } else {
	jetcorr_filenames_pfL1FastJetL2L3.push_back  ("jetCorrections/Summer15_25nsV2_MC_L1FastJet_AK4PFchs.txt");
	jetcorr_filenames_pfL1FastJetL2L3.push_back  ("jetCorrections/Summer15_25nsV2_MC_L2Relative_AK4PFchs.txt");
	jetcorr_filenames_pfL1FastJetL2L3.push_back  ("jetCorrections/Summer15_25nsV2_MC_L3Absolute_AK4PFchs.txt");
	jetcorr_uncertainty_filename = "jetCorrections/Summer15_25nsV6_DATA_Uncertainty_AK4PFchs.txt";
      }
    }

    cout << "applying JEC from the following files:" << endl;
    for (unsigned int ifile = 0; ifile < jetcorr_filenames_pfL1FastJetL2L3.size(); ++ifile) {
      cout << "   " << jetcorr_filenames_pfL1FastJetL2L3.at(ifile) << endl;
    }

    jet_corrector_pfL1FastJetL2L3  = makeJetCorrector(jetcorr_filenames_pfL1FastJetL2L3);

    if (!isDataFromFileName && applyJECunc != 0) {
      cout << "applying JEC uncertainties with weight " << applyJECunc << " from file: " << endl
	   << "   " << jetcorr_uncertainty_filename << endl;
      jetcorr_uncertainty = new JetCorrectionUncertainty(jetcorr_uncertainty_filename);
    }
      
  } // if applyJECfromFile

  // get susy xsec file
  TH1F* h_sig_xsec(0);
  if ((baby_name.find("T1") != std::string::npos) || (baby_name.find("T2") != std::string::npos)) {
    // determine which susy particle is being produced
    TString sparticle = "";
    if (baby_name.find("T1") != std::string::npos) sparticle = "gluino";
    else if ((baby_name.find("T2tt") != std::string::npos) || (baby_name.find("T2bb") != std::string::npos)) sparticle = "stop";
    else if (baby_name.find("T2qq") != std::string::npos) sparticle = "squark";
    if (sparticle == "") std::cout << "WARNING: didn't recognize signal sample from name: " << baby_name << std::endl;
    
    TFile* f_xsec = new TFile("data/xsec_susy_13tev.root");
    TH1F* h_sig_xsec_temp = (TH1F*) f_xsec->Get(Form("h_xsec_%s",sparticle.Data()));
    BabyFile_->cd();
    h_sig_xsec = (TH1F*) h_sig_xsec_temp->Clone("h_sig_xsec");
    f_xsec->Close();
  }
  
  // File Loop
  int nDuplicates = 0;
  int nFailJSON = 0;
  int nFailRunNumber = 0;
  int nEvents = chain->GetEntries();
  unsigned int nEventsChain = nEvents;
  cout << "Running on " << nEventsChain << " events" << endl;
  unsigned int nEventsTotal = 0;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;
  while ( (currentFile = (TFile*)fileIter.Next()) ) {
    cout << "running on file: " << currentFile->GetTitle() << endl;

    evt_id = sampleID(currentFile->GetTitle());

    // Get File Content
    TFile f( currentFile->GetTitle() );
    TTree *tree = (TTree*)f.Get("Events");
    TTreeCache::SetLearnEntries(10);
    tree->SetCacheSize(128*1024*1024);
    cms3.Init(tree);

    // Event Loop
    unsigned int nEventsTree = tree->GetEntriesFast();
    for( unsigned int event = 0; event < nEventsTree; ++event) {
      //for( unsigned int event = 0; event < 1000; ++event) {

      // Get Event Content
      tree->LoadTree(event);
      cms3.GetEntry(event);
      ++nEventsTotal;
      count_hist_->Fill(1);

      // Progress
      CMS3::progress( nEventsTotal, nEventsChain );

      InitBabyNtuple();

      isData = cms3.evt_isRealData();
      // sanity check on whether this is data
      if (bool(isData) != isDataFromFileName) {
        cout << "ERROR: file name and content disagree on whether this is real data!! Exiting" << endl;
        return;
      }

      if (verbose) cout << "before trigger" << endl;

      //TRIGGER - check first to enable cuts
      HLT_PFHT800        = passHLTTriggerPattern("HLT_PFHT800_v");
      HLT_PFHT900        = passHLTTriggerPattern("HLT_PFHT900_v");
      HLT_PFMET170       = passHLTTriggerPattern("HLT_PFMET170_NoiseCleaned_v") || passHLTTriggerPattern("HLT_PFMET170_JetIdCleaned_v") || passHLTTriggerPattern("HLT_PFMET170_v"); 
      HLT_PFHT350_PFMET100  = passHLTTriggerPattern("HLT_PFHT350_PFMET100_NoiseCleaned_v") || passHLTTriggerPattern("HLT_PFHT350_PFMET100_JetIdCleaned_v") || passHLTTriggerPattern("HLT_PFHT350_PFMET100_v"); 
      HLT_PFHT350_PFMET120  = passHLTTriggerPattern("HLT_PFHT350_PFMET120_NoiseCleaned_v") || passHLTTriggerPattern("HLT_PFHT350_PFMET120_JetIdCleaned_v"); 
      HLT_PFMETNoMu90_PFMHTNoMu90   = passHLTTriggerPattern("HLT_PFMETNoMu90_JetIdCleaned_PFMHTNoMu90_IDTight_v") || passHLTTriggerPattern("HLT_PFMETNoMu90_NoiseCleaned_PFMHTNoMu90_IDTight_v") || passHLTTriggerPattern("HLT_PFMETNoMu90_PFMHTNoMu90_IDTight_v");
      HLT_MonoCentralPFJet80_PFMETNoMu90_PFMHTNoMu90   = passHLTTriggerPattern("HLT_MonoCentralPFJet80_PFMETNoMu90_NoiseCleaned_PFMHTNoMu90_IDTight_v") || passHLTTriggerPattern("HLT_MonoCentralPFJet80_PFMETNoMu90_JetIdCleaned_PFMHTNoMu90_IDTight_v") || passHLTTriggerPattern("HLT_MonoCentralPFJet80_PFMETNoMu90_PFMHTNoMu90_IDTight_v");
      HLT_PFMETNoMu120_PFMHTNoMu120 = passHLTTriggerPattern("HLT_PFMETNoMu120_JetIdCleaned_PFMHTNoMu120_IDTight_v") || passHLTTriggerPattern("HLT_PFMETNoMu120_NoiseCleaned_PFMHTNoMu120_IDTight_v") || passHLTTriggerPattern("HLT_PFMETNoMu120_PFMHTNoMu120_IDTight_v");
      HLT_PFMET90_PFMHT90           = passHLTTriggerPattern("HLT_PFMET90_PFMHT90_IDTight_v") || passHLTTriggerPattern("HLT_PFMET90_PFMHT90_IDLoose_v");

      HLT_SingleMu     = passHLTTriggerPattern("HLT_IsoMu17_eta2p1_v") ||
        passHLTTriggerPattern("HLT_IsoMu20_v") || passHLTTriggerPattern("HLT_IsoMu20_eta2p1_v") ||
        passHLTTriggerPattern("HLT_IsoTkMu20_v"); 
      HLT_SingleEl     = passHLTTriggerPattern("HLT_Ele23_WPLoose_Gsf_v") ||
        passHLTTriggerPattern("HLT_Ele22_eta2p1_WPLoose_Gsf_v") ||
        passHLTTriggerPattern("HLT_Ele23_WP75_Gsf_v") ||
        passHLTTriggerPattern("HLT_Ele22_eta2p1_WP75_Gsf_v");
      HLT_DoubleEl     = passHLTTriggerPattern("HLT_Ele23_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v") ||
        passHLTTriggerPattern("HLT_Ele17_Ele12_CaloIdL_TrackIdL_IsoVL_DZ_v");
      HLT_DoubleEl33   = passHLTTriggerPattern("HLT_DoubleEle33_CaloIdL_GsfTrkIdVL_v");
      HLT_MuX_Ele12 = passHLTTriggerPattern("HLT_Mu23_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v") ||
        passHLTTriggerPattern("HLT_Mu17_TrkIsoVVL_Ele12_CaloIdL_TrackIdL_IsoVL_v");
      HLT_Mu8_EleX = passHLTTriggerPattern("HLT_Mu8_TrkIsoVVL_Ele23_CaloIdL_TrackIdL_IsoVL_v") ||
        passHLTTriggerPattern("HLT_Mu8_TrkIsoVVL_Ele17_CaloIdL_TrackIdL_IsoVL_v");
      HLT_DoubleMu     = passHLTTriggerPattern("HLT_Mu17_TrkIsoVVL_Mu8_TrkIsoVVL_DZ_v") ||
        passHLTTriggerPattern("HLT_Mu17_TrkIsoVVL_TkMu8_TrkIsoVVL_DZ_v");
      HLT_Photon120 = passHLTTriggerPattern("HLT_Photon120_v"); 
      HLT_Photon165_HE10 = passHLTTriggerPattern("HLT_Photon165_HE10_v"); 
      HLT_PFHT200_Prescale  = passHLTTriggerPattern("HLT_PFHT200_v") ? HLT_prescale(triggerName("HLT_PFHT200_v")) : 0; 
      HLT_PFHT300_Prescale  = passHLTTriggerPattern("HLT_PFHT300_v") ? HLT_prescale(triggerName("HLT_PFHT300_v")) : 0; 
      HLT_PFHT350_Prescale  = passHLTTriggerPattern("HLT_PFHT350_v") ? HLT_prescale(triggerName("HLT_PFHT350_v")) : 0; 
      HLT_PFHT475_Prescale  = passHLTTriggerPattern("HLT_PFHT475_v") ? HLT_prescale(triggerName("HLT_PFHT475_v")) : 0; 
      HLT_PFHT600_Prescale  = passHLTTriggerPattern("HLT_PFHT600_v") ? HLT_prescale(triggerName("HLT_PFHT600_v")) : 0; 
      HLT_DiCentralPFJet70_PFMET120  = passHLTTriggerPattern("HLT_DiCentralPFJet70_PFMET120_NoiseCleaned_v") || passHLTTriggerPattern("HLT_DiCentralPFJet70_PFMET120_JetIdCleaned_v"); 
      HLT_DiCentralPFJet55_PFMET110  = passHLTTriggerPattern("HLT_DiCentralPFJet55_PFMET110_NoiseCleaned_v") || passHLTTriggerPattern("HLT_DiCentralPFJet55_PFMET110_JetIdCleaned_v"); 

      if (!isData && applyTriggerCuts && !(HLT_PFHT900 || HLT_PFHT350_PFMET120 || HLT_Photon165_HE10 || HLT_SingleMu 
            || HLT_DoubleMu || HLT_DoubleEl || HLT_MuX_Ele12 || HLT_Mu8_EleX)) continue;

      run  = cms3.evt_run();
      lumi = cms3.evt_lumiBlock();
      evt  = cms3.evt_event();

      if ( applyJSON && isData && !goodrun(run, lumi) ) {
        ++nFailJSON;
        continue;
      }

      if ( isData && isPromptReco && removeEarlyPromptReco && (run <= 251562) ) {
        ++nFailRunNumber;
        continue;
      }

      if (!removePostProcVars) {
        evt_nEvts = cms3.evt_nEvts();
        evt_scale1fb = cms3.evt_scale1fb();
        evt_xsec = cms3.evt_xsec_incl();
        evt_kfactor = cms3.evt_kfactor();
        evt_filter = cms3.evt_filt_eff();
      }
      if (!isData) {
        genWeight = cms3.genps_weight();
        puWeight = 1.;
        nTrueInt = -1;
	if (cms3.puInfo_trueNumInteractions().size() > 0) nTrueInt = cms3.puInfo_trueNumInteractions().at(0);
      }

      rho = cms3.evt_fixgridfastjet_all_rho(); //this one is used in JECs

      if (verbose) cout << "before vertices" << endl;

      //VERTICES
      nVert = 0;
      for(unsigned int ivtx=0; ivtx < cms3.evt_nvtxs(); ivtx++){

        if(cms3.vtxs_isFake().at(ivtx)) continue;
        if(cms3.vtxs_ndof().at(ivtx) <= 4) continue;
        if(fabs(cms3.vtxs_position().at(ivtx).z()) > 24) continue;
        if(cms3.vtxs_position().at(ivtx).Rho() > 2) continue;

        nVert++;

      }

      if (applyJECfromFile && recomputeT1MET) {
	std::pair <float, float> t1met;
	if (!isData) t1met = getT1CHSMET_fromMINIAOD(jet_corrector_pfL1FastJetL2L3, jetcorr_uncertainty, bool(applyJECunc == 1));
	else t1met = getT1CHSMET_fromMINIAOD(jet_corrector_pfL1FastJetL2L3); // never apply variations to data
	met_pt  = t1met.first;
	met_phi = t1met.second;
	met_rawPt  = cms3.evt_METToolbox_pfmet_raw();
	met_rawPhi = cms3.evt_METToolbox_pfmetPhi_raw();
      } else {
	met_pt  = cms3.evt_pfmet();
	met_phi = cms3.evt_pfmetPhi();
	met_rawPt  = cms3.evt_pfmet_raw();
	met_rawPhi = cms3.evt_pfmetPhi_raw();
      }
      if (!isData) {
        met_genPt  = cms3.gen_met();
        met_genPhi = cms3.gen_metPhi();
      }
      met_caloPt  = cms3.evt_calomet();
      met_caloPhi = cms3.evt_calometPhi();
      metStruct trkmet = trackerMET(0.1);
      met_trkPt = trkmet.met;
      met_trkPhi = trkmet.metphi;

      // MET FILTERS -- not present in fastsim
      if (!isFastsim) {
	Flag_EcalDeadCellTriggerPrimitiveFilter       = cms3.filt_ecalTP();
	Flag_trkPOG_manystripclus53X                  = cms3.filt_trkPOG_manystripclus53X();
	Flag_ecalLaserCorrFilter                      = cms3.filt_ecalLaser();
	Flag_trkPOG_toomanystripclus53X               = cms3.filt_trkPOG_toomanystripclus53X();
	Flag_hcalLaserEventFilter                     = cms3.filt_hcalLaser();
	Flag_trkPOG_logErrorTooManyClusters           = cms3.filt_trkPOG_logErrorTooManyClusters();
	Flag_trkPOGFilters                            = cms3.filt_trkPOGFilters();
	Flag_trackingFailureFilter                    = cms3.filt_trackingFailure();
	Flag_goodVertices                             = cms3.filt_goodVertices();
	Flag_eeBadScFilter                            = cms3.filt_eeBadSc();
	// note: in CMS3, filt_cscBeamHalo and evt_cscTightHaloId are the same
	Flag_CSCTightHaloFilter                       = cms3.filt_cscBeamHalo();
	// note: in CMS3, filt_hbheNoise and evt_hbheFilter are the same
	//      Flag_HBHENoiseFilter                          = cms3.filt_hbheNoise();
	// recompute HBHE noise filter decision using CORE to avoid maxZeros issue
	if (bx == 25) Flag_HBHENoiseFilter            = hbheNoiseFilter_25ns();
	else Flag_HBHENoiseFilter                     = hbheNoiseFilter();
	Flag_HBHEIsoNoiseFilter                       = hbheIsoNoiseFilter();
	// necessary?
	Flag_METFilters                               = cms3.filt_metfilter();
      }
      
      // gen block -- for MC only
      ngenPart = 0;
      ngenLep = 0;
      ngenStat23 = 0;
      ngenGamma = 0;
      ngenTau = 0;
      ngenTau1Prong = 0;
      ngenTau3Prong = 0;
      ngenLepFromTau = 0;
      if (!isData) {

	// !!!!!!!!!!! HACK !!!!!!!!!!!!!
	// fix xsec/kfactor for HT binned Wjets, Zinv, DYjets
	if ((evt_id >= 502 && evt_id <= 505) || (evt_id >= 602 && evt_id <= 605) || (evt_id >= 702 && evt_id <= 705)) {
	  if (evt_id >= 502 && evt_id <= 505) evt_kfactor = 1.21;
	  else evt_kfactor = 1.23;
	  
	  if (evt_id == 502) evt_xsec = 1347.;
	  else if (evt_id == 503) evt_xsec = 360.;
	  else if (evt_id == 504) evt_xsec = 48.9;
	  else if (evt_id == 505) evt_xsec = 18.77;
	  evt_scale1fb = evt_xsec*evt_kfactor*1000.*evt_filter/(Double_t)evt_nEvts;
	}
	
        if (verbose) cout << "before sparm values" << endl;

	// T1 models
        if (evt_id >= 1000 && evt_id < 1100) {
          for (unsigned int i=0; i<sparm_values().size(); ++i) {
            if (sparm_names().at(i).Contains("mGluino")) GenSusyMScan1 = sparm_values().at(i);
            if (sparm_names().at(i).Contains("mLSP")) GenSusyMScan2 = sparm_values().at(i);
          }
        }
	// T2 models
        else if (evt_id >= 1100 && evt_id < 1200) {
          for (unsigned int i=0; i<sparm_values().size(); ++i) {
            if (sparm_names().at(i).Contains("mStop")) GenSusyMScan1 = sparm_values().at(i);
            if (sparm_names().at(i).Contains("mLSP")) GenSusyMScan2 = sparm_values().at(i);
          }
        }

	// use sparm values to look up xsec
	if (evt_id >= 1000 && evt_id < 1200) evt_xsec = h_sig_xsec->GetBinContent(h_sig_xsec->FindBin(GenSusyMScan1));

        if (verbose) cout << "before gen particles" << endl;

        //GEN PARTICLES
        LorentzVector recoil(0,0,0,0);
        int nHardScatter = 0;
	genTop_pt = -1.;
	genTbar_pt = -1.;
        for(unsigned int iGen = 0; iGen < cms3.genps_p4().size(); iGen++){
          if (ngenPart >= max_ngenPart) {
            std::cout << "WARNING: attempted to fill more than " << max_ngenPart << " gen particles" << std::endl;
            break;
          }
          genPart_pt[ngenPart] = cms3.genps_p4().at(iGen).pt();
          genPart_eta[ngenPart] = cms3.genps_p4().at(iGen).eta();
          genPart_phi[ngenPart] = cms3.genps_p4().at(iGen).phi();
          genPart_mass[ngenPart] = cms3.genps_mass().at(iGen);
          genPart_pdgId[ngenPart] = cms3.genps_id().at(iGen);
          genPart_status[ngenPart] = cms3.genps_status().at(iGen);
          genPart_charge[ngenPart] = cms3.genps_charge().at(iGen);
          genPart_motherId[ngenPart] = cms3.genps_id_simplemother().at(iGen);
          genPart_grandmotherId[ngenPart] = cms3.genps_id_simplegrandma().at(iGen);
          ngenPart++;

          int pdgId = abs(cms3.genps_id().at(iGen));
          int status = cms3.genps_status().at(iGen);
          int isLastCopy = cms3.genps_isLastCopy().at(iGen);

          // find hard scatter products to get recoil pt
          if (evt_id >= 300 && evt_id < 400) {
            // ttbar
            if (isLastCopy == 1 && pdgId == 6) {
              recoil += cms3.genps_p4().at(iGen);
	      if (cms3.genps_id().at(iGen) == 6) genTop_pt = cms3.genps_p4().at(iGen).pt();
	      else if (cms3.genps_id().at(iGen) == -6) genTbar_pt = cms3.genps_p4().at(iGen).pt();
              ++nHardScatter;
            }
            if (nHardScatter > 2) std::cout << "WARNING: found too many tops in ttbar MC!" << std::endl;
          }
          else if (evt_id >= 500 && evt_id < 600) {
            // W+jets
            if (isLastCopy == 1 && pdgId == 24) {
              recoil += cms3.genps_p4().at(iGen);
              ++nHardScatter;
            }
            if (nHardScatter > 1) std::cout << "WARNING: found too many Ws in Wjets MC!" << std::endl;
          }
          else if (evt_id >= 1000 && evt_id < 1100) {
            // SMS T1 models - gluinos
            if (isLastCopy == 1 && pdgId == 1000021) {
              recoil += cms3.genps_p4().at(iGen);
              ++nHardScatter;
            }
            if (nHardScatter > 2) std::cout << "WARNING: found too many gluinos in T1 MC!" << std::endl;
          }
          else if (evt_id >= 1100 && evt_id < 1110) {
            // SMS T2tt - stops
            if (isLastCopy == 1 && pdgId == 1000006) {
              recoil += cms3.genps_p4().at(iGen);
              ++nHardScatter;
            }
            if (nHardScatter > 2) std::cout << "WARNING: found too many stops in T2tt MC!" << std::endl;
          }
          else if (evt_id >= 1110 && evt_id < 1120) {
            // SMS T2qq - squarks
            if (isLastCopy == 1 && ( (pdgId >= 1000001 && pdgId <= 1000004) || (pdgId >= 2000001 && pdgId <= 2000004) ) ) {
              recoil += cms3.genps_p4().at(iGen);
              ++nHardScatter;
            }
            if (nHardScatter > 2) std::cout << "WARNING: found too many squarks in T2qq MC!" << std::endl;
          }
          else if (evt_id >= 1120 && evt_id < 1130) {
            // SMS T2bb - sbottoms
            if (isLastCopy == 1 && pdgId == 1000005) {
              recoil += cms3.genps_p4().at(iGen);
              ++nHardScatter;
            }
            if (nHardScatter > 2) std::cout << "WARNING: found too many sbottoms in T2bb MC!" << std::endl;
          }

          int motherId = abs(cms3.genps_id_simplemother().at(iGen));
          int grandmotherId = abs(cms3.genps_id_simplegrandma().at(iGen));

          // save all status 22 or 23 gen particles
          if (status == 23 || status == 22) {
            genStat23_pt[ngenStat23] = cms3.genps_p4().at(iGen).pt();
            genStat23_eta[ngenStat23] = cms3.genps_p4().at(iGen).eta();
            genStat23_phi[ngenStat23] = cms3.genps_p4().at(iGen).phi();
            genStat23_mass[ngenStat23] = cms3.genps_mass().at(iGen);
            genStat23_pdgId[ngenStat23] = cms3.genps_id().at(iGen);
            genStat23_status[ngenStat23] = cms3.genps_status().at(iGen);
            genStat23_charge[ngenStat23] = cms3.genps_charge().at(iGen);
            genStat23_sourceId[ngenStat23] = getSourceId(iGen);
            ++ngenStat23;
          }

          // save gen gammas good for matching
          bool goodGamma = false;
          //gamma: status 1 matched to mother photon or proton
          if (pdgId == 22 && status == 1) { 
            if (motherId <= 22  || motherId == 2212) {
              goodGamma = true;
            } 
          }

          if (goodGamma) {
            genGamma_pt[ngenGamma] = cms3.genps_p4().at(iGen).pt();
            genGamma_eta[ngenGamma] = cms3.genps_p4().at(iGen).eta();
            genGamma_phi[ngenGamma] = cms3.genps_p4().at(iGen).phi();
            genGamma_motherId[ngenGamma] = cms3.genps_id_simplemother().at(iGen);
            ++ngenGamma;
          }

          // save lepton info
          if ((pdgId != 11) && (pdgId != 13) && (pdgId != 15)) continue;

          // reject leptons with direct parents of quarks or hadrons. 
          //  Allow SUSY parents - not explicitly checking for now though
          if (motherId <= 5 || (motherId > 100 && motherId < 1000000)) continue;

          bool goodLep = false;
          bool goodTau = false;
	  int decayMode = 0;
	  int neutralDaughters = 0;
	  float leadTrackPt = 0.;
          bool goodLepFromTau = false;
          int sourceId = 0;

          // electrons, muons: status 1 or 23 and mother W/Z/H or tau from W/Z/H
          if ( (pdgId == 11 || pdgId == 13) && (status == 1 || status == 23) ) {
            // save leptons pre-FSR: prefer status 23 over status 1
            if (status == 1 && motherId == pdgId && (cms3.genps_status().at(cms3.genps_idx_simplemother().at(iGen)) == 23)) {
              // don't save
            }
            // leptons from taus
            else if (motherId == 15 && (grandmotherId == 25 || grandmotherId == 24 || grandmotherId == 23 || grandmotherId == 15)) {
              goodLepFromTau = true;
            } 
            // leptons from W/Z/H
            else if (motherId == 25 || motherId == 24 || motherId == 23) {
              goodLep = true;
            } 
          } // status 1 e or mu

          // taus: status 2 or 23, from W/Z/H
          else if (pdgId == 15 && (status == 2 || status == 23)) {
            // save leptons pre-FSR: prefer status 23 over status 2
            if (status == 2 && motherId == pdgId && (cms3.genps_status().at(cms3.genps_idx_simplemother().at(iGen)) == 23)) {
              // don't save
            }
            // leptons from W/Z/H
            else if (motherId == 25 || motherId == 24 || motherId == 23) {
              goodTau = true;
            }
	    
	    // find decay mode - only count charged daughters
	    if (goodTau) {
	      int tauIdx = iGen;
	      // for status 23 taus: must find the status 2 taus for decays..
	      if (status == 23) {
		for(unsigned int jGen = 0; jGen < cms3.genps_p4().size(); jGen++) {
		  if (abs(cms3.genps_id().at(jGen)) != 15) continue;
		  if (abs(cms3.genps_id_simplemother().at(jGen)) != 15) continue;
		  if ( (cms3.genps_idx_mother().at(jGen) != int(iGen)) && (cms3.genps_idx_mother().at(jGen) != cms3.genps_idx_mother().at(iGen)) ) continue;
		  if (cms3.genps_status().at(jGen) != 2) continue;
		  tauIdx = jGen;
		  break;
		}
	      }
	      for(unsigned int jGen = 0; jGen < cms3.genps_p4().size(); jGen++) {
		int id2 = abs(cms3.genps_id().at(jGen));
		if (cms3.genps_idx_mother().at(jGen) != int(tauIdx)) continue;
		//if (cms3.genps_charge().at(jGen) == 0) continue;
		if ((cms3.genps_status().at(jGen) != 1) && (cms3.genps_status().at(jGen) != 2)) continue;
		// don't count duplicated taus as daughters
		if (id2 == 15) continue;
		if ( (id2 == 11) || (id2 == 13) ) {
		  decayMode = cms3.genps_id().at(jGen);
		  leadTrackPt = cms3.genps_p4().at(jGen).pt();
		  break;
		}
		// not a lepton: count number of prongs
		else if (abs(cms3.genps_charge().at(jGen)) == 1) {
		  ++decayMode;
		  if (cms3.genps_p4().at(jGen).pt() > leadTrackPt) leadTrackPt = cms3.genps_p4().at(jGen).pt();
		}
		// not charged: count neutral (non-neutrino) daughters
		else if ((cms3.genps_charge().at(jGen) == 0) && (id2 != 12 && id2 != 14 && id2 != 16)) {
		  ++neutralDaughters;
		}
	      }
	    } // if goodTau
	    
          } // status 2 tau

          if (goodLep || goodTau || goodLepFromTau) {
            sourceId = getSourceId(iGen);
          }

          // save gen leptons (e/mu) directly from W/Z/H
          if (goodLep) {
            genLep_pt[ngenLep] = cms3.genps_p4().at(iGen).pt();
            genLep_eta[ngenLep] = cms3.genps_p4().at(iGen).eta();
            genLep_phi[ngenLep] = cms3.genps_p4().at(iGen).phi();
            genLep_mass[ngenLep] = cms3.genps_mass().at(iGen);
            genLep_pdgId[ngenLep] = cms3.genps_id().at(iGen);
            genLep_status[ngenLep] = cms3.genps_status().at(iGen);
            genLep_charge[ngenLep] = cms3.genps_charge().at(iGen);
            genLep_sourceId[ngenLep] = sourceId;
            ++ngenLep;
          }

          // save gen taus from W/Z/H
          if (goodTau) {
            genTau_pt[ngenTau] = cms3.genps_p4().at(iGen).pt();
            genTau_eta[ngenTau] = cms3.genps_p4().at(iGen).eta();
            genTau_phi[ngenTau] = cms3.genps_p4().at(iGen).phi();
            genTau_mass[ngenTau] = cms3.genps_mass().at(iGen);
            genTau_pdgId[ngenTau] = cms3.genps_id().at(iGen);
            genTau_status[ngenTau] = cms3.genps_status().at(iGen);
            genTau_charge[ngenTau] = cms3.genps_charge().at(iGen);
            genTau_sourceId[ngenTau] = sourceId;
            genTau_decayMode[ngenTau] = decayMode;
            genTau_leadTrackPt[ngenTau] = leadTrackPt;
            genTau_neutralDaughters[ngenTau] = neutralDaughters;
            ++ngenTau;
	    if (decayMode == 1) ++ngenTau1Prong;
	    else if (decayMode == 3) ++ngenTau3Prong;
          }

          // save gen e/mu from taus (which are from W/Z/H)
          if (goodLepFromTau) {
            genLepFromTau_pt[ngenLepFromTau] = cms3.genps_p4().at(iGen).pt();
            genLepFromTau_eta[ngenLepFromTau] = cms3.genps_p4().at(iGen).eta();
            genLepFromTau_phi[ngenLepFromTau] = cms3.genps_p4().at(iGen).phi();
            genLepFromTau_mass[ngenLepFromTau] = cms3.genps_mass().at(iGen);
            genLepFromTau_pdgId[ngenLepFromTau] = cms3.genps_id().at(iGen);
            genLepFromTau_status[ngenLepFromTau] = cms3.genps_status().at(iGen);
            genLepFromTau_charge[ngenLepFromTau] = cms3.genps_charge().at(iGen);
            genLepFromTau_sourceId[ngenLepFromTau] = sourceId;
            ++ngenLepFromTau;
          }

        } // loop over genPart

	// top pt weight
	if (evt_id >= 300 && evt_id < 400) {
	  weight_toppt = topPtWeight(genTop_pt,genTbar_pt);
	}

        // recoil "ISR" weight
	genRecoil_pt = recoil.pt();
	weight_isr = 1.;
	if (genRecoil_pt > 400. && genRecoil_pt < 600.)  weight_isr = 0.85;
	else if (genRecoil_pt > 600.)                    weight_isr = 0.70;

	// store LHE weight variations
	if (saveLHEweights || saveLHEweightsScaleOnly) {
	  nLHEweight = 0;
	  for (unsigned int iwgt = 0; iwgt < cms3.genweights().size(); ++iwgt) {
	    if (saveLHEweightsScaleOnly && iwgt > 8) break;
	    if (iwgt >= max_nLHEweight) {
	      std::cout << "WARNING: attempted to fill more than " << max_nLHEweight << " LHEweights" << std::endl;
	      break;
	    }
	    LHEweight_wgt[nLHEweight] = cms3.genweights().at(iwgt);
	    ++nLHEweight;
	  }
	}
      } // !isData

      //LEPTONS
      std::vector<std::pair<int, float> > lep_pt_ordering;
      vector<float>vec_lep_pt;
      vector<float>vec_lep_eta;
      vector<float>vec_lep_phi;
      vector<float>vec_lep_mass;
      vector<float>vec_lep_charge;
      vector<int>  vec_lep_pdgId;
      vector<float>vec_lep_dxy;
      vector<float>vec_lep_dz;
      vector<int>  vec_lep_tightId;
      vector<int>  vec_lep_heepId;
      vector<float>vec_lep_relIso03;
      vector<float>vec_lep_relIso04;
      vector<float>vec_lep_miniRelIso;
      vector<float>vec_lep_relIsoAn04;
      vector<int>  vec_lep_mcMatchId;
      vector<int>  vec_lep_lostHits;
      vector<int>  vec_lep_convVeto;
      vector<int>  vec_lep_tightCharge;

      vector<LorentzVector> p4sUniqueLeptons;

      vector<LorentzVector> p4sForHems;
      vector<LorentzVector> p4sForHemsGamma;
      vector<LorentzVector> p4sForHemsZll;
      vector<LorentzVector> p4sForHemsZllMT;
      vector<LorentzVector> p4sForHemsRl;

      vector<LorentzVector> p4sForDphi;
      vector<LorentzVector> p4sForDphiGamma;
      vector<LorentzVector> p4sForDphiZll;
      vector<LorentzVector> p4sForDphiZllMT;
      vector<LorentzVector> p4sForDphiRl;

      if (verbose) cout << "before electrons" << endl;

      //ELECTRONS
      nlep = 0;
      nElectrons10 = 0;
      for(unsigned int iEl = 0; iEl < cms3.els_p4().size(); iEl++){
        if(cms3.els_p4().at(iEl).pt() < 10.0) continue;
        if(fabs(cms3.els_p4().at(iEl).eta()) > 2.4) continue;
        // first check ID then iso
        if(!electronID(iEl,id_level_t::HAD_veto_noiso_v4)) continue;
        bool pass_iso = electronID(iEl,id_level_t::HAD_veto_v4);
        if(applyLeptonIso && !pass_iso) continue;
        lep_pt_ordering.push_back( std::pair<int,float>(nlep,cms3.els_p4().at(iEl).pt()) );
        vec_lep_pt.push_back ( cms3.els_p4().at(iEl).pt());
        vec_lep_eta.push_back ( cms3.els_p4().at(iEl).eta()); //save eta, even though we use SCeta for ID
        vec_lep_phi.push_back ( cms3.els_p4().at(iEl).phi());
        vec_lep_mass.push_back ( cms3.els_mass().at(iEl));
        vec_lep_charge.push_back ( cms3.els_charge().at(iEl));
        vec_lep_pdgId.push_back ( (-11)*cms3.els_charge().at(iEl));
        vec_lep_dxy.push_back ( cms3.els_dxyPV().at(iEl));
        vec_lep_dz.push_back ( cms3.els_dzPV().at(iEl));
        vec_lep_tightId.push_back ( eleTightID(iEl,analysis_t::HAD,4) );
        vec_lep_heepId.push_back ( isHEEPV60(iEl) );
        vec_lep_relIso03.push_back (  eleRelIso03(iEl,analysis_t::HAD));
        vec_lep_relIso04.push_back ( 0);
        vec_lep_miniRelIso.push_back ( elMiniRelIsoCMS3_EA(iEl,1) );
        vec_lep_relIsoAn04.push_back ( elRelIsoAn04(iEl) );
        if (!isData && cms3.els_mc3dr().size() > 0 && cms3.els_mc3dr().at(iEl) < 0.2 && cms3.els_mc3idx().at(iEl) != -9999 && abs(cms3.els_mc3_id().at(iEl)) == 11) { // matched to a prunedGenParticle electron?
          int momid =  abs(genPart_motherId[cms3.els_mc3idx().at(iEl)]);
          vec_lep_mcMatchId.push_back ( momid != 11 ? momid : genPart_grandmotherId[cms3.els_mc3idx().at(iEl)]); // if mother is different store mother, otherwise store grandmother
        }
        else vec_lep_mcMatchId.push_back (0);

        vec_lep_lostHits.push_back ( cms3.els_exp_innerlayers().at(iEl)); //cms3.els_lost_pixelhits().at(iEl);
        vec_lep_convVeto.push_back ( !cms3.els_conv_vtx_flag().at(iEl));
        vec_lep_tightCharge.push_back ( tightChargeEle(iEl));

        nlep++;

        // only use isolated leptons for counters, overlaps, etc
        if (pass_iso) {
          nElectrons10++;
          p4sUniqueLeptons.push_back(cms3.els_p4().at(iEl));

          // for mt2 and mht in lepton control region
          if (doJetLepOverlapRemoval) {
            p4sForHems.push_back(cms3.els_p4().at(iEl));
            p4sForDphi.push_back(cms3.els_p4().at(iEl));
          }
        }

        if (!isData && applyDummyWeights) {
	  weightStruct weights = getLepSF(cms3.els_p4().at(iEl).pt(), cms3.els_p4().at(iEl).eta(), 11);
          weight_lepsf *= weights.cent;
          weight_lepsf_UP *= weights.up;
          weight_lepsf_DN *= weights.dn;
        }
        if (!isData && applyLeptonSFs) {
          weightStruct weights = getLepSFFromFile(cms3.els_p4().at(iEl).pt(), cms3.els_p4().at(iEl).eta(), 11);
          weight_lepsf *= weights.cent;
          weight_lepsf_UP *= weights.up;
          weight_lepsf_DN *= weights.dn;
        }
      }

      if (verbose) cout << "before muons" << endl;

      //MUONS
      nMuons10 = 0;
      for(unsigned int iMu = 0; iMu < cms3.mus_p4().size(); iMu++){
        if(cms3.mus_p4().at(iMu).pt() < 10.0) continue;
        if(fabs(cms3.mus_p4().at(iMu).eta()) > 2.4) continue;
        // first check ID then iso
        if(!muonID(iMu,id_level_t::HAD_loose_noiso_v4)) continue;
        bool pass_iso = muonID(iMu,id_level_t::HAD_loose_v4);
        if (applyLeptonIso && !pass_iso) continue;
        lep_pt_ordering.push_back( std::pair<int,float>(nlep,cms3.mus_p4().at(iMu).pt()) );
        vec_lep_pt.push_back ( cms3.mus_p4().at(iMu).pt());
        vec_lep_eta.push_back ( cms3.mus_p4().at(iMu).eta());
        vec_lep_phi.push_back ( cms3.mus_p4().at(iMu).phi());
        vec_lep_mass.push_back ( cms3.mus_mass().at(iMu));
        vec_lep_charge.push_back ( cms3.mus_charge().at(iMu));
        vec_lep_pdgId.push_back ( (-13)*cms3.mus_charge().at(iMu));
        vec_lep_dxy.push_back ( cms3.mus_dxyPV().at(iMu)); // this uses the silicon track. should we use best track instead?
        vec_lep_dz.push_back ( cms3.mus_dzPV().at(iMu)); // this uses the silicon track. should we use best track instead?
        vec_lep_tightId.push_back ( muTightID(iMu,analysis_t::HAD,4) );
        vec_lep_heepId.push_back ( 0 );
        vec_lep_relIso03.push_back ( muRelIso03(iMu,analysis_t::HAD) );
        vec_lep_relIso04.push_back ( muRelIso04(iMu,analysis_t::HAD) );
        vec_lep_miniRelIso.push_back ( muMiniRelIsoCMS3_EA(iMu,1) );
        vec_lep_relIsoAn04.push_back ( muRelIsoAn04(iMu) );
        if (!isData && cms3.mus_mc3dr().size() > 0 && cms3.mus_mc3dr().at(iMu) < 0.2 && cms3.mus_mc3idx().at(iMu) != -9999 && abs(cms3.mus_mc3_id().at(iMu)) == 13) { // matched to a prunedGenParticle electron?
          int momid =  abs(genPart_motherId[cms3.mus_mc3idx().at(iMu)]);
          vec_lep_mcMatchId.push_back ( momid != 13 ? momid : genPart_grandmotherId[cms3.mus_mc3idx().at(iMu)]); // if mother is different store mother, otherwise store grandmother
        }
        else vec_lep_mcMatchId.push_back (0);
        vec_lep_lostHits.push_back ( cms3.mus_exp_innerlayers().at(iMu) ); // use defaults as if "good electron"
        vec_lep_convVeto.push_back ( 1);// use defaults as if "good electron"
        vec_lep_tightCharge.push_back ( tightChargeMuon(iMu) );

        nlep++;

        // only use isolated leptons for counters, overlaps, etc
        if (pass_iso) {
          nMuons10++;
          p4sUniqueLeptons.push_back(cms3.mus_p4().at(iMu));

          // for mt2 and mht in lepton control region
          if (doJetLepOverlapRemoval) {
            p4sForHems.push_back(cms3.mus_p4().at(iMu));
            p4sForDphi.push_back(cms3.mus_p4().at(iMu));
          }
        }

        if (!isData && applyDummyWeights) {
	  weightStruct weights = getLepSF(cms3.mus_p4().at(iMu).pt(), cms3.mus_p4().at(iMu).eta(), 13);
          weight_lepsf *= weights.cent;
          weight_lepsf_UP *= weights.up;
          weight_lepsf_DN *= weights.dn;
        }
        if (!isData && applyLeptonSFs) {
          weightStruct weights = getLepSFFromFile(cms3.mus_p4().at(iMu).pt(), cms3.mus_p4().at(iMu).eta(), 13);
          weight_lepsf *= weights.cent;
          weight_lepsf_UP *= weights.up;
          weight_lepsf_DN *= weights.dn;
        }

      }

      // Implement pT ordering for leptons (it's irrelevant but easier for us to add than for ETH to remove)
      //now fill arrays from vectors, isotracks with largest pt first
      int i = 0;
      std::sort(lep_pt_ordering.begin(), lep_pt_ordering.end(), sortByValue);
      for(std::vector<std::pair<int, float> >::iterator it = lep_pt_ordering.begin(); it!= lep_pt_ordering.end(); ++it){

        if (i >= max_nlep) {
          std::cout << "WARNING: attempted to fill more than " << max_nlep << " leptons" << std::endl;
          break;
        }

        lep_pt[i]     = vec_lep_pt.at(it->first);
        lep_eta[i]    = vec_lep_eta.at(it->first);
        lep_phi[i]    = vec_lep_phi.at(it->first);
        lep_mass[i]   = vec_lep_mass.at(it->first);
        lep_charge[i] = vec_lep_charge.at(it->first);
        lep_pdgId[i]  = vec_lep_pdgId.at(it->first);
        lep_dz[i]     = vec_lep_dz.at(it->first);
        lep_dxy[i]    = vec_lep_dxy.at(it->first);
        lep_tightId[i]     = vec_lep_tightId.at(it->first);
        lep_heepId[i]      = vec_lep_heepId.at(it->first);
        lep_relIso03[i]    = vec_lep_relIso03.at(it->first);
        lep_relIso04[i]    = vec_lep_relIso04.at(it->first);
        lep_miniRelIso[i]  = vec_lep_miniRelIso.at(it->first);
        lep_relIsoAn04[i]  = vec_lep_relIsoAn04.at(it->first);
        lep_mcMatchId[i]   = vec_lep_mcMatchId.at(it->first);
        lep_lostHits[i]    = vec_lep_lostHits.at(it->first);
        lep_convVeto[i]    = vec_lep_convVeto.at(it->first);
        lep_tightCharge[i] = vec_lep_tightCharge.at(it->first);
        i++;
      }

      // for Z-->ll control regions 
      // (use two leading leptons regardless of flavor, charge, ID)
      if (nlep == 2) {
        float zll_met_px  = met_pt * cos(met_phi);
        float zll_met_py  = met_pt * sin(met_phi);	
        zll_met_px += lep_pt[0] * cos(lep_phi[0]);
        zll_met_px += lep_pt[1] * cos(lep_phi[1]);
        zll_met_py += lep_pt[0] * sin(lep_phi[0]);
        zll_met_py += lep_pt[1] * sin(lep_phi[1]);
        // recalculated MET with photons added
        TVector2 zll_met_vec(zll_met_px, zll_met_py);
        zll_met_pt = zll_met_vec.Mod();
        zll_met_phi = TVector2::Phi_mpi_pi(zll_met_vec.Phi());      
        TLorentzVector l0(0,0,0,0);
        TLorentzVector l1(0,0,0,0);
        l0.SetPtEtaPhiM(lep_pt[0], lep_eta[0], lep_phi[0], lep_mass[0]);
        l1.SetPtEtaPhiM(lep_pt[1], lep_eta[1], lep_phi[1], lep_mass[1]);
        TLorentzVector ll = l0+l1;
        zll_mass = ll.M();
        zll_pt = ll.Pt();
        zll_eta = ll.Eta();
        zll_phi = ll.Phi();

        // alternate set of vars transforming Zll to look like Wlnu. 
        //  "Randomly" turn one lepton into MET and recalc vars
        int kill_lep = evt%2;
        int keep_lep = (kill_lep+1)%2;

        float zllmt_met_px  = met_pt * cos(met_phi);
        float zllmt_met_py  = met_pt * sin(met_phi);	
        zllmt_met_px += lep_pt[kill_lep] * cos(lep_phi[kill_lep]);
        zllmt_met_py += lep_pt[kill_lep] * sin(lep_phi[kill_lep]);
        // recalculated MET with photons added
        TVector2 zllmt_met_vec(zllmt_met_px, zllmt_met_py);
        zllmt_met_pt = zllmt_met_vec.Mod();
        zllmt_met_phi = TVector2::Phi_mpi_pi(zllmt_met_vec.Phi());      
        zllmt_mt = MT(lep_pt[keep_lep],lep_phi[keep_lep],zllmt_met_pt,zllmt_met_phi);

        LorentzVector keep_lep_vec;
        if (keep_lep == 0) {
          keep_lep_vec.SetPxPyPzE(l0.Px(),l0.Py(),l0.Pz(),l0.E());
        } else {
          keep_lep_vec.SetPxPyPzE(l1.Px(),l1.Py(),l1.Pz(),l1.E());
        }
        p4sForHemsZllMT.push_back(keep_lep_vec);
        p4sForDphiZllMT.push_back(keep_lep_vec);

      }
      //--for removed lepton control regions (w->lnu)
      if (nlep == 1) {
        float rl_met_px  = met_pt * cos(met_phi);
        float rl_met_py  = met_pt * sin(met_phi);	
        rl_met_px += lep_pt[0] * cos(lep_phi[0]);
        rl_met_py += lep_pt[0] * sin(lep_phi[0]);
        // recalculated MET with photons added
        TVector2 rl_met_vec(rl_met_px, rl_met_py);
        rl_met_pt = rl_met_vec.Mod();
        rl_met_phi = TVector2::Phi_mpi_pi(rl_met_vec.Phi());      
        // TLorentzVector l0(0,0,0,0);
        // TLorentzVector l1(0,0,0,0);
        // l0.SetPtEtaPhiM(lep_pt[0], lep_eta[0], lep_phi[0], lep_mass[0]);
        // TLorentzVector ll = l0;
        rl_mass = lep_mass[0];
        rl_pt  = lep_pt[0];
        rl_eta = lep_eta[0];
        rl_phi = lep_phi[0];
      }

      if (verbose) cout << "before isotracks" << endl;

      //ISOTRACK
      std::vector<std::pair<int, float> > pt_ordering;
      vector<float>vec_isoTrack_pt;
      vector<float>vec_isoTrack_eta;
      vector<float>vec_isoTrack_phi;
      vector<float>vec_isoTrack_mass;
      vector<float>vec_isoTrack_absIso;
      vector<float>vec_isoTrack_relIsoAn04;
      vector<float>vec_isoTrack_dz;
      vector<int>  vec_isoTrack_pdgId;
      vector<int>  vec_isoTrack_mcMatchId;

      nisoTrack = 0;
      nPFLep5LowMT = 0;
      nPFHad10LowMT = 0;
      nPFCHCand3 = 0;
      for (unsigned int ipf = 0; ipf < pfcands_p4().size(); ipf++) {

        if(cms3.pfcands_charge().at(ipf) == 0) continue;
        if(fabs(cms3.pfcands_dz().at(ipf)) > 0.1) continue;
        if(cms3.pfcands_fromPV().at(ipf) <= 1) continue;

        float cand_pt = cms3.pfcands_p4().at(ipf).pt();
        if(cand_pt > 3) ++nPFCHCand3;
        if(cand_pt < 5) continue;

        float absiso  = TrackIso(ipf, 0.3, 0.0, true, false);
        if(applyLeptonIso && absiso >= min(0.2*cand_pt, 8.0)) continue;

        float mt = MT(cand_pt,cms3.pfcands_p4().at(ipf).phi(),met_pt,met_phi);
        int pdgId = abs(cms3.pfcands_particleId().at(ipf));
        float an04 = PFCandRelIsoAn04(ipf);

        if ((cand_pt > 5.) && (pdgId == 11 || pdgId == 13) && (absiso/cand_pt < 0.2) && (mt < 100.)) {
          ++nPFLep5LowMT;

          // use PF leptons for hemispheres etc same as reco leptons
          //  BUT first do overlap removal with reco leptons to avoid double counting
          bool overlap = false;
          for(unsigned int iLep = 0; iLep < p4sUniqueLeptons.size(); iLep++){
            float thisDR = DeltaR(pfcands_p4().at(ipf).eta(), p4sUniqueLeptons.at(iLep).eta(), pfcands_p4().at(ipf).phi(), p4sUniqueLeptons.at(iLep).phi());
            // use small DR threshold to ONLY remove objects that are exactly the same (reco/pf leptons)
            if (thisDR < 0.01) {
              overlap = true;
              break;
            }
          } // loop over reco leps
          if (!overlap) {
            p4sUniqueLeptons.push_back(cms3.pfcands_p4().at(ipf));
            if (doJetLepOverlapRemoval) {
              p4sForHems.push_back(cms3.pfcands_p4().at(ipf));
              p4sForDphi.push_back(cms3.pfcands_p4().at(ipf));
            }
          }
        } // passing pflepton 

        if ((cand_pt > 10.) && (pdgId == 211) && (absiso/cand_pt < 0.1) && (mt < 100.)) ++nPFHad10LowMT;

        pt_ordering.push_back(std::pair<int,float>(nisoTrack,cand_pt));

        vec_isoTrack_pt.push_back    ( cand_pt                          );
        vec_isoTrack_eta.push_back   ( cms3.pfcands_p4().at(ipf).eta()  );
        vec_isoTrack_phi.push_back   ( cms3.pfcands_p4().at(ipf).phi()  );
        vec_isoTrack_mass.push_back  ( cms3.pfcands_mass().at(ipf)      );
        vec_isoTrack_absIso.push_back( absiso                           );
        vec_isoTrack_relIsoAn04.push_back( an04                         );
        vec_isoTrack_dz.push_back    ( cms3.pfcands_dz().at(ipf)        );
        vec_isoTrack_pdgId.push_back ( cms3.pfcands_particleId().at(ipf));
        vec_isoTrack_mcMatchId.push_back ( 0 );

        nisoTrack++;
      }  

      //now fill arrays from vectors, isotracks with largest pt first
      i = 0;
      std::sort(pt_ordering.begin(), pt_ordering.end(), sortByValue);
      for(std::vector<std::pair<int, float> >::iterator it = pt_ordering.begin(); it!= pt_ordering.end(); ++it){

        if (i >= max_nisoTrack) {
          std::cout << "WARNING: attempted to fill more than " << max_nisoTrack << " iso tracks" << std::endl;
          break;
        }

        isoTrack_pt[i]     = vec_isoTrack_pt.at(it->first);
        isoTrack_eta[i]    = vec_isoTrack_eta.at(it->first);
        isoTrack_phi[i]    = vec_isoTrack_phi.at(it->first);
        isoTrack_mass[i]   = vec_isoTrack_mass.at(it->first);
        isoTrack_absIso[i] = vec_isoTrack_absIso.at(it->first);
        isoTrack_relIsoAn04[i] = vec_isoTrack_relIsoAn04.at(it->first);
        isoTrack_dz[i]     = vec_isoTrack_dz.at(it->first);
        isoTrack_pdgId[i]  = vec_isoTrack_pdgId.at(it->first);
        isoTrack_mcMatchId[i]  = vec_isoTrack_mcMatchId.at(it->first);
        i++;
      }

      // count number of unique lowMT leptons (e/mu)
      //  same collection as those used for jet/lepton overlap, but require MT < 100 explicitly
      nLepLowMT = 0;
      for (unsigned int ilep = 0; ilep < p4sUniqueLeptons.size(); ++ilep) {
        float mt = MT(p4sUniqueLeptons.at(ilep).pt(),p4sUniqueLeptons.at(ilep).phi(),met_pt,met_phi);
        if (mt < 100.) ++nLepLowMT;
      }

      if (verbose) cout << "before photons" << endl;

      //PHOTONS
      std::vector<std::pair<int, float> > gamma_pt_ordering;
      vector<float> vec_gamma_pt;
      vector<float> vec_gamma_eta;
      vector<float> vec_gamma_phi;
      vector<float> vec_gamma_mass;
      vector<int>   vec_gamma_mcMatchId;
      vector<float> vec_gamma_genIso04;
      vector<float> vec_gamma_drMinParton;
      vector<float> vec_gamma_chHadIso;
      vector<float> vec_gamma_neuHadIso;
      vector<float> vec_gamma_phIso;
      vector<float> vec_gamma_sigmaIetaIeta;
      vector<float> vec_gamma_r9;
      vector<float> vec_gamma_hOverE;
      vector<int>   vec_gamma_idCutBased;

      ngamma = 0;
      nGammas20 = 0;
      // to recalculate MET adding photons
      float gamma_met_px  = met_pt * cos(met_phi);
      float gamma_met_py  = met_pt * sin(met_phi);
      for(unsigned int iGamma = 0; iGamma < cms3.photons_p4().size(); iGamma++){
        if(cms3.photons_p4().at(iGamma).pt() < 20.0) continue;
        if(fabs(cms3.photons_p4().at(iGamma).eta()) > 2.5) continue;
        if ( !isLoosePhoton(iGamma,analysis_t::HAD,3) ) continue;

        if (ngamma >= max_ngamma) {
          std::cout << "WARNING: attempted to fill more than " << max_ngamma << " photons" << std::endl;
          break;
        }
	gamma_pt_ordering.push_back( std::pair<int,float>(ngamma,cms3.photons_p4().at(iGamma).pt()) );
        float pt = cms3.photons_p4().at(iGamma).pt();
        float eta = cms3.photons_p4().at(iGamma).eta();
        float phi = cms3.photons_p4().at(iGamma).phi();
        vec_gamma_pt.push_back ( pt );
        vec_gamma_eta.push_back ( eta );
        vec_gamma_phi.push_back ( phi );
        vec_gamma_mass.push_back ( cms3.photons_mass().at(iGamma) );
        vec_gamma_sigmaIetaIeta.push_back ( cms3.photons_full5x5_sigmaIEtaIEta().at(iGamma) );
        vec_gamma_chHadIso.push_back ( photons_recoChargedHadronIso().at(iGamma) );
        vec_gamma_neuHadIso.push_back ( photons_recoNeutralHadronIso().at(iGamma) );
        vec_gamma_phIso.push_back ( photons_recoPhotonIso().at(iGamma) );
        vec_gamma_r9.push_back (  photons_full5x5_r9().at(iGamma) );
        vec_gamma_hOverE.push_back (  photons_full5x5_hOverEtowBC().at(iGamma) );
        vec_gamma_idCutBased.push_back (  isTightPhoton(iGamma,analysis_t::HAD,3) ? 1 : 0 ); 
        if(pt > 20) nGammas20++;

        // Some work for truth-matching (should be integrated in CMS3 as for the leptons)
        int bestMatch = -1;
        float bestDr = 0.1;
        float bestMatchEta = 999;
        float bestMatchPhi = 999;
        if (!isData) {
          for(unsigned int iGen = 0; iGen < cms3.genps_p4().size(); iGen++){
            if (cms3.genps_id().at(iGen) != 22) continue; 
            if (cms3.genps_status().at(iGen) != 1) continue; 
            if (fabs(cms3.genps_id_simplemother().at(iGen)) > 22  && cms3.genps_id_simplemother().at(iGen)!=2212) continue; // pions etc // but keep photons from the leading proton
            if ( fabs(eta - cms3.genps_p4().at(iGen).eta()) > 0.1) continue;
            if ( pt > 2*cms3.genps_p4().at(iGen).pt() ) continue;
            if ( pt < 0.5*cms3.genps_p4().at(iGen).pt() ) continue;
            float thisDR = DeltaR( cms3.genps_p4().at(iGen).eta(), eta, cms3.genps_p4().at(iGen).phi(), phi);
            if (thisDR < bestDr) {
              bestDr = thisDR;
              bestMatch = iGen;
              bestMatchEta = cms3.genps_p4().at(iGen).eta();
              bestMatchPhi = cms3.genps_p4().at(iGen).phi();
            }
          }
        } // !isData
        if (bestMatch != -1) {
          // 7 is a special code for photons without a mother. this seems to be due to a miniAOD bug where links are broken.
          vec_gamma_mcMatchId.push_back ( cms3.genps_id_simplemother().at(bestMatch) == 0 ? 7 : 22 ); 
          vec_gamma_genIso04.push_back ( cms3.genps_iso().at(bestMatch) );
          // Now want to look at DR between photon and parton
          float minDR = 999.; 
          for(unsigned int iGen = 0; iGen < cms3.genps_p4().size(); iGen++){
            if (cms3.genps_status().at(iGen) != 22 && cms3.genps_status().at(iGen) != 23) continue;
            if (fabs(cms3.genps_id().at(iGen)) > 21) continue;
            float dr = DeltaR( cms3.genps_p4().at(iGen).eta(), bestMatchEta, cms3.genps_p4().at(iGen).phi(), bestMatchPhi);
            if (dr < minDR) minDR = dr;
          }
          vec_gamma_drMinParton.push_back ( minDR );
        }
        else {
          vec_gamma_mcMatchId.push_back ( 0 );
          vec_gamma_genIso04.push_back ( -1 );
          vec_gamma_drMinParton.push_back ( -1 );
        }

        // do not use photon in MT2 or MHT calculations!!
        //p4sForHemsGamma.push_back(cms3.photons_p4().at(iGamma));
        //p4sForDphiGamma.push_back(cms3.photons_p4().at(iGamma));
        ngamma++;
      }

      // Implement pT ordering for photons (to match ETH)
      i = 0;
      std::sort(gamma_pt_ordering.begin(), gamma_pt_ordering.end(), sortByValue);
      for(std::vector<std::pair<int, float> >::iterator it = gamma_pt_ordering.begin(); it!= gamma_pt_ordering.end(); ++it){

        if (i >= max_ngamma) {
          std::cout << "WARNING: attempted to fill more than " << max_ngamma << " gammas" << std::endl;
          break;
        }

        gamma_pt[i]     = vec_gamma_pt.at(it->first);
        gamma_eta[i]    = vec_gamma_eta.at(it->first);
        gamma_phi[i]    = vec_gamma_phi.at(it->first);
        gamma_mass[i]   = vec_gamma_mass.at(it->first);
        gamma_mcMatchId[i]    = vec_gamma_mcMatchId.at(it->first);
        gamma_genIso04[i]     = vec_gamma_genIso04.at(it->first);
        gamma_drMinParton[i]  = vec_gamma_drMinParton.at(it->first);
        gamma_chHadIso[i]     = vec_gamma_chHadIso.at(it->first);
        gamma_neuHadIso[i]    = vec_gamma_neuHadIso.at(it->first);
        gamma_phIso[i]        = vec_gamma_phIso.at(it->first);
        gamma_sigmaIetaIeta[i]  = vec_gamma_sigmaIetaIeta.at(it->first);
        gamma_r9[i]           = vec_gamma_r9.at(it->first);
        gamma_hOverE[i]       = vec_gamma_hOverE.at(it->first);
        gamma_idCutBased[i]   = vec_gamma_idCutBased.at(it->first);
        i++;
      }

      // for photon+jets control regions
      if (nGammas20>0) { // Only use the leading Loose photon. Otherwise mt2 will be affected by a bunch of tiny photons
        gamma_met_px += gamma_pt[0] * cos(gamma_phi[0]);
        gamma_met_py += gamma_pt[0] * sin(gamma_phi[0]);
      }

      // recalculated MET with photons added
      TVector2 gamma_met_vec(gamma_met_px, gamma_met_py);
      gamma_met_pt = gamma_met_vec.Mod();
      gamma_met_phi = TVector2::Phi_mpi_pi(gamma_met_vec.Phi());

      if (verbose) cout << "before jets" << endl;

      //JETS
      //before we start, check that no genGet is matched to multiple recoJets
      //vector<float> pTofMatchedGenJets;
      //for(unsigned int iJet = 0; iJet < cms3.pfjets_p4().size(); iJet++){
      //  if(cms3.pfjets_p4().at(iJet).pt() < 10.0) continue;
      //  if(fabs(cms3.pfjets_p4().at(iJet).eta()) > 4.7) continue;
      //	float matchedPt = cms3.pfjets_mc_p4().at(iJet).pt();
      //	if (matchedPt!=0) {
      //	  if ( find( pTofMatchedGenJets.begin(), pTofMatchedGenJets.end(), matchedPt  ) != pTofMatchedGenJets.end() ) {
      //	    cout<<" This genJetPt is identical to one that was already matched to another reco jet of higher pT. Need to find another match"<<endl;
      //	    cout<<"event"<<evt_event()<<" jet pT: "<<cms3.pfjets_p4().at(iJet).pt()<<" genJetPt: "<<matchedPt<<endl;
      //	  }
      //	  else {
      //	    pTofMatchedGenJets.push_back( matchedPt );
      //	  }
      //	}
      //}

      //correct jets and check baseline selections
      vector<LorentzVector> p4sCorrJets; // store corrected p4 for ALL jets, so indices match CMS3 ntuple
      vector<std::pair<int,float> > passJets; //index of jets that pass baseline selections with their corrected pt
      for(unsigned int iJet = 0; iJet < cms3.pfjets_p4().size(); iJet++){

        LorentzVector pfjet_p4_cor = cms3.pfjets_p4().at(iJet);

        if (applyJECfromFile) {

          // get uncorrected jet p4 to use as input for corrections
          LorentzVector pfjet_p4_uncor = cms3.pfjets_p4().at(iJet) * cms3.pfjets_undoJEC().at(iJet);

          // get L1FastL2L3Residual total correction
          jet_corrector_pfL1FastJetL2L3->setRho   ( cms3.evt_fixgridfastjet_all_rho() );
          jet_corrector_pfL1FastJetL2L3->setJetA  ( cms3.pfjets_area().at(iJet)       );
          jet_corrector_pfL1FastJetL2L3->setJetPt ( pfjet_p4_uncor.pt()               );
          jet_corrector_pfL1FastJetL2L3->setJetEta( pfjet_p4_uncor.eta()              );
          double corr = jet_corrector_pfL1FastJetL2L3->getCorrection();

          // check for negative correction
          if (corr < 0. && fabs(pfjet_p4_uncor.eta()) < 4.7) {
            std::cout << "ScanChain::Looper: WARNING: negative jet correction: " << corr
              << ", raw jet pt: " << pfjet_p4_uncor.pt() << ", eta: " << pfjet_p4_uncor.eta() << std::endl;
          }

	  // include protections here on jet kinematics to prevent rare warnings/crashes
	  double var = 1.;
	  if (!isData && applyJECunc != 0 && pfjet_p4_uncor.pt()*corr > 0. && fabs(pfjet_p4_uncor.eta()) < 5.4) {
	    jetcorr_uncertainty->setJetEta(pfjet_p4_uncor.eta());
	    jetcorr_uncertainty->setJetPt(pfjet_p4_uncor.pt() * corr); // must use CORRECTED pt
	    double unc = jetcorr_uncertainty->getUncertainty(true);
	    var = (1. + applyJECunc * unc);
	  }

          // apply new JEC to p4
          pfjet_p4_cor = pfjet_p4_uncor * corr * var;
        }

        p4sCorrJets.push_back(pfjet_p4_cor);

        if(p4sCorrJets.at(iJet).pt() < 10.0) continue;
        if(fabs(p4sCorrJets.at(iJet).eta()) > 4.7) continue;

        passJets.push_back( std::pair<int,float>(iJet, pfjet_p4_cor.pt()) );

      }

      // sort passing jets by corrected pt
      std::sort(passJets.begin(), passJets.end(), sortByValue);

      if (verbose) cout << "before jet/lepton overlap" << endl;

      //check overlapping with leptons
      //only want to remove the closest jet to a lepton, threshold deltaR < 0.4
      // if a jet is the closest one to 2 leptons, only remove that 1 jet. This loop will find it twice
      vector<int> removedJets; //index of jets to be removed because they overlap with a lepton
      for(unsigned int iLep = 0; iLep < p4sUniqueLeptons.size(); iLep++){

        float minDR = 0.4;
        int minIndex = -1;
        for(unsigned int passIdx = 0; passIdx < passJets.size(); passIdx++){ //loop through jets that passed baseline selections

          int iJet = passJets.at(passIdx).first;

          if(p4sCorrJets.at(iJet).pt() < 10.0) continue;
          if(fabs(p4sCorrJets.at(iJet).eta()) > 4.7) continue;

          float thisDR = DeltaR(p4sCorrJets.at(iJet).eta(), p4sUniqueLeptons.at(iLep).eta(), p4sCorrJets.at(iJet).phi(), p4sUniqueLeptons.at(iLep).phi());
          if(thisDR < minDR){
            minDR = thisDR; 
            minIndex = iJet;
          }
        }
        removedJets.push_back(minIndex);
      }

      if (verbose) cout << "before jet/photon overlap" << endl;

      //check overlapping with photons
      //only want to remove the closest jet to a photon, threshold deltaR < 0.4
      vector<int> removedJetsGamma; //index of jets to be removed because they overlap with a photon
      for(int iGamma = 0; iGamma < ngamma; iGamma++){
        if (iGamma>0) continue; // Only check leading photon. Let the others be

        float minDR = 0.4;
        int minIndex = -1;
        for(unsigned int passIdx = 0; passIdx < passJets.size(); passIdx++){ //loop through jets that passed baseline selections

          int iJet = passJets.at(passIdx).first;

          if(p4sCorrJets.at(iJet).pt() < 10.0) continue;
          if(fabs(p4sCorrJets.at(iJet).eta()) > 4.7) continue;

          bool alreadyRemoved = false;
          for(unsigned int j=0; j<removedJetsGamma.size(); j++){
            if(iJet == removedJetsGamma.at(j)){
              alreadyRemoved = true;
              break;
            }
          }
          if(alreadyRemoved) continue;

          float thisDR = DeltaR(p4sCorrJets.at(iJet).eta(), gamma_eta[iGamma], p4sCorrJets.at(iJet).phi(), gamma_phi[iGamma]);
          if(thisDR < minDR){
            minDR = thisDR; 
            minIndex = iJet;
          }
        }
        removedJetsGamma.push_back(minIndex);
      }

      njet = 0;
      nJet30 = 0;
      nJet40 = 0;
      nBJet20 = 0;
      nBJet25 = 0;
      nBJet30 = 0;
      nBJet40 = 0;
      nJet30FailId = 0;
      nJet100FailId = 0;
      minMTBMet = 999999.;
      jet1_pt = 0.;
      jet2_pt = 0.;

      gamma_nJet30 = 0;
      gamma_nJet40 = 0;
      gamma_nJet30FailId = 0;
      gamma_nJet100FailId = 0;
      gamma_nBJet20 = 0;
      gamma_nBJet25 = 0;
      gamma_nBJet30 = 0;
      gamma_nBJet40 = 0;
      gamma_minMTBMet = 999999.;
      gamma_jet1_pt = 0.;
      gamma_jet2_pt = 0.;
      zll_minMTBMet = 999999.;
      rl_minMTBMet = 999999.;

      // for applying btagging SFs, using Method 1a from the twiki below:
      //   https://twiki.cern.ch/twiki/bin/viewauth/CMS/BTagSFMethods#1a_Event_reweighting_using_scale
      //   https://twiki.cern.ch/twiki/pub/CMS/BTagSFMethods/Method1aExampleCode_CSVM.cc.txt
      float btagprob_data = 1.;
      float btagprob_err_heavy_UP = 0.;
      float btagprob_err_heavy_DN = 0.;
      float btagprob_err_light_UP = 0.;
      float btagprob_err_light_DN = 0.;
      float btagprob_mc = 1.;

      if (verbose) cout << "before main jet loop" << endl;

      //now fill variables for jets that pass baseline selections and don't overlap with a lepton
      for(unsigned int passIdx = 0; passIdx < passJets.size(); passIdx++){

        int iJet = passJets.at(passIdx).first;

        //check against list of jets that overlap with a lepton
        bool isOverlapJet = false;
        for(unsigned int j=0; j<removedJets.size(); j++){
          if(iJet == removedJets.at(j)){
            isOverlapJet = true;
            break;
          }
        }
        if(doJetLepOverlapRemoval && isOverlapJet) continue;

        bool isOverlapJetGamma = false;
        if( ( p4sCorrJets.at(iJet).pt() > 20.0) && (fabs(p4sCorrJets.at(iJet).eta()) < 2.5) ) { 
          //check against list of jets that overlap with a photon
          for(unsigned int j=0; j<removedJetsGamma.size(); j++){
            if(iJet == removedJetsGamma.at(j)){
              isOverlapJetGamma = true;
              break;
            }
          }
        } // pt 20 eta 2.5

        if (njet >= max_njet) {
          std::cout << "WARNING: attempted to fill more than " << max_njet << " jets" << std::endl;
          break;
        }

        // only save jets with pt 20 eta 4.7
        if( (p4sCorrJets.at(iJet).pt() > 20.0) && (fabs(p4sCorrJets.at(iJet).eta()) < 4.7) ) {
        //if( (p4sCorrJets.at(iJet).pt() > 10.0) && (fabs(p4sCorrJets.at(iJet).eta()) < 4.7) ) {//for RS

          // first check jet ID - count the number of jets that fail
          if(!isLoosePFJet_50nsV1(iJet)) {
            if (p4sCorrJets.at(iJet).pt() > 30.0) ++nJet30FailId;
            if (p4sCorrJets.at(iJet).pt() > 100.0) ++nJet100FailId;
            if (!isOverlapJetGamma) {
              if (p4sCorrJets.at(iJet).pt() > 30.0) ++gamma_nJet30FailId;
              if (p4sCorrJets.at(iJet).pt() > 100.0) ++gamma_nJet100FailId;
            }
            continue;
          }

          jet_pt[njet]   = p4sCorrJets.at(iJet).pt();
          jet_eta[njet]  = p4sCorrJets.at(iJet).eta();
          jet_phi[njet]  = p4sCorrJets.at(iJet).phi();
          jet_mass[njet] = cms3.pfjets_mass().at(iJet);
          jet_btagCSV[njet] = cms3.pfjets_pfCombinedInclusiveSecondaryVertexV2BJetTag().at(iJet); 
          if (!isData) {
	    jet_mcPt[njet] = -1;
            if (cms3.pfjets_mc_p4().size() > 0) jet_mcPt[njet] = cms3.pfjets_mc_p4().at(iJet).pt();
	    jet_mcFlavour[njet] = 0;
            if (cms3.pfjets_partonFlavour().size() > 0) jet_mcFlavour[njet] = cms3.pfjets_partonFlavour().at(iJet);
          } else {
            jet_mcPt[njet] = -999.;
            jet_mcFlavour[njet] = -999;
          }
          //jet_qgl
          jet_area[njet] = cms3.pfjets_area().at(iJet);
          jet_rawPt[njet] = cms3.pfjets_p4().at(iJet).pt() * cms3.pfjets_undoJEC().at(iJet);

          if (isMonoPFJet_Monojet(iJet)) jet_id[njet] = 5;
          else if (isMonoPFJet_MT2(iJet)) jet_id[njet] = 4;
          else if (isTightPFJet_50nsV1(iJet)) jet_id[njet] = 3;
          else if (isLoosePFJet_50nsV1(iJet)) jet_id[njet] = 1;
          else jet_id[njet] = 0;

          jet_puId[njet] = loosePileupJetId(iJet) ? 1 : 0;

          // use pt20 for bjet counting, pt30 for everything else
          if( (jet_pt[njet] > 20.0) && (fabs(jet_eta[njet]) < 2.5) ){ 
            if (jet_pt[njet] > 30.0) {
              // store leading/subleading central jet pt.
              //  jets should be pt-ordered before entering this loop
              if (jet1_pt < 0.1) jet1_pt = p4sCorrJets.at(iJet).pt();
              else if (jet2_pt < 0.1) jet2_pt = p4sCorrJets.at(iJet).pt();
              p4sForHems.push_back(p4sCorrJets.at(iJet));
              p4sForDphi.push_back(p4sCorrJets.at(iJet));
              p4sForHemsZll.push_back(p4sCorrJets.at(iJet));
              p4sForDphiZll.push_back(p4sCorrJets.at(iJet));
              p4sForHemsZllMT.push_back(p4sCorrJets.at(iJet));
              p4sForDphiZllMT.push_back(p4sCorrJets.at(iJet));
              p4sForHemsRl.push_back(p4sCorrJets.at(iJet));
              p4sForDphiRl.push_back(p4sCorrJets.at(iJet));
              nJet30++;
              if (jet_pt[njet] > 40.) nJet40++;
            } // pt40
            //CSVv2IVFM
            if(jet_btagCSV[njet] >= 0.890) {
              nBJet20++; 
              // btag SF - not final yet
              if (!isData && applyBtagSFs) {
                float eff = getBtagEffFromFile(jet_pt[njet], jet_eta[njet], jet_mcFlavour[njet], isFastsim);
		BTagEntry::JetFlavor flavor = BTagEntry::FLAV_UDSG;
		if (abs(jet_mcFlavour[njet]) == 5) flavor = BTagEntry::FLAV_B;
		else if (abs(jet_mcFlavour[njet]) == 4) flavor = BTagEntry::FLAV_C;
		float pt_cutoff = std::max(30.,std::min(669.,double(jet_pt[njet])));
		float eta_cutoff = std::min(2.39,fabs(double(jet_eta[njet])));
		float weight_cent(1.), weight_UP(1.), weight_DN(1.);
		if (flavor == BTagEntry::FLAV_UDSG) {
		  weight_cent = reader_light->eval(flavor, eta_cutoff, pt_cutoff);
		  weight_UP = reader_light_UP->eval(flavor, eta_cutoff, pt_cutoff);
		  weight_DN = reader_light_DN->eval(flavor, eta_cutoff, pt_cutoff);
		} else {
		  weight_cent = reader_heavy->eval(flavor, eta_cutoff, pt_cutoff);
		  weight_UP = reader_heavy_UP->eval(flavor, eta_cutoff, pt_cutoff);
		  weight_DN = reader_heavy_DN->eval(flavor, eta_cutoff, pt_cutoff);
		}
		// extra SF for fastsim
		if (isFastsim) {
		  weight_cent *= reader_fastsim->eval(flavor, eta_cutoff, pt_cutoff);
		  weight_UP *= reader_fastsim_UP->eval(flavor, eta_cutoff, pt_cutoff);
		  weight_DN *= reader_fastsim_DN->eval(flavor, eta_cutoff, pt_cutoff);
		}
                btagprob_data *= weight_cent * eff;
                btagprob_mc *= eff;
                float abserr_UP = weight_UP - weight_cent;
                float abserr_DN = weight_cent - weight_DN;
		if (flavor == BTagEntry::FLAV_UDSG) {
                  btagprob_err_light_UP += abserr_UP/weight_cent;
                  btagprob_err_light_DN += abserr_DN/weight_cent;
		} else {
                  btagprob_err_heavy_UP += abserr_UP/weight_cent;
                  btagprob_err_heavy_DN += abserr_DN/weight_cent;
                }
              }
              if (jet_pt[njet] > 25.0) nBJet25++; 
              if (jet_pt[njet] > 30.0) {
                nBJet30++; 
                if (jet_pt[njet] > 40.0) nBJet40++; 
                float mt = MT(jet_pt[njet],jet_phi[njet],met_pt,met_phi);
                if (mt < minMTBMet) minMTBMet = mt;
                if (nlep == 2) {
                  float zllmt = MT(jet_pt[njet],jet_phi[njet],zll_met_pt,zll_met_phi);
                  if (zllmt < zll_minMTBMet) zll_minMTBMet = zllmt;
                }
                if (nlep == 1) {
                  float rlmt = MT(jet_pt[njet],jet_phi[njet],rl_met_pt,rl_met_phi);
                  if (rlmt < rl_minMTBMet) rl_minMTBMet = rlmt;
                }
              } // pt 30
            } // pass med btag
            else if (!isData && applyBtagSFs) { // fail med btag -- needed for SF event weights
              float eff = getBtagEffFromFile(jet_pt[njet], jet_eta[njet], jet_mcFlavour[njet], isFastsim);
	      BTagEntry::JetFlavor flavor = BTagEntry::FLAV_UDSG;
	      if (abs(jet_mcFlavour[njet]) == 5) flavor = BTagEntry::FLAV_B;
	      else if (abs(jet_mcFlavour[njet]) == 4) flavor = BTagEntry::FLAV_C;
	      float pt_cutoff = std::max(30.,std::min(669.,double(jet_pt[njet])));
	      float eta_cutoff = std::min(2.39,fabs(double(jet_eta[njet])));
	      float weight_cent(1.), weight_UP(1.), weight_DN(1.);
	      if (flavor == BTagEntry::FLAV_UDSG) {
		weight_cent = reader_light->eval(flavor, eta_cutoff, pt_cutoff);
		weight_UP = reader_light_UP->eval(flavor, eta_cutoff, pt_cutoff);
		weight_DN = reader_light_DN->eval(flavor, eta_cutoff, pt_cutoff);
	      } else {
		weight_cent = reader_heavy->eval(flavor, eta_cutoff, pt_cutoff);
		weight_UP = reader_heavy_UP->eval(flavor, eta_cutoff, pt_cutoff);
		weight_DN = reader_heavy_DN->eval(flavor, eta_cutoff, pt_cutoff);
	      }
	      // extra SF for fastsim
	      if (isFastsim) {
		weight_cent *= reader_fastsim->eval(flavor, eta_cutoff, pt_cutoff);
		weight_UP *= reader_fastsim_UP->eval(flavor, eta_cutoff, pt_cutoff);
		weight_DN *= reader_fastsim_DN->eval(flavor, eta_cutoff, pt_cutoff);
	      }
              btagprob_data *= (1. - weight_cent * eff);
              btagprob_mc *= (1. - eff);
              float abserr_UP = weight_UP - weight_cent;
              float abserr_DN = weight_cent - weight_DN;
	      if (flavor == BTagEntry::FLAV_UDSG) {
                btagprob_err_light_UP += (-eff * abserr_UP)/(1 - eff * weight_cent);
                btagprob_err_light_DN += (-eff * abserr_DN)/(1 - eff * weight_cent);
              } else {
                btagprob_err_heavy_UP += (-eff * abserr_UP)/(1 - eff * weight_cent);
                btagprob_err_heavy_DN += (-eff * abserr_DN)/(1 - eff * weight_cent);
              }
            } // fail med btag
          } // pt 20 eta 2.5
          // accept jets out to eta 4.7 for dphi
          else if ( (jet_pt[njet] > 30.0) && (fabs(jet_eta[njet]) < 4.7) ) {
            p4sForDphi.push_back(p4sCorrJets.at(iJet));
            p4sForDphiZll.push_back(p4sCorrJets.at(iJet));
          }

          // fill gamma_XXX variables before checking for lepton overlap. Why? Let's keep them consistent with the lepton-overlapped jets
          if( ( p4sCorrJets.at(iJet).pt() > 20.0) && (fabs(p4sCorrJets.at(iJet).eta()) < 2.5) ) { 
            if(!isOverlapJetGamma) {
              if (p4sCorrJets.at(iJet).pt() > 30.0) {
                // store leading/subleading central jet pt.
                //  jets should be pt-ordered before entering this loop
                if (gamma_jet1_pt < 0.1) gamma_jet1_pt = p4sCorrJets.at(iJet).pt();
                else if (gamma_jet2_pt < 0.1) gamma_jet2_pt = p4sCorrJets.at(iJet).pt();
                p4sForHemsGamma.push_back(p4sCorrJets.at(iJet));
                p4sForDphiGamma.push_back(p4sCorrJets.at(iJet));
                gamma_nJet30++;
                if (p4sCorrJets.at(iJet).pt() > 40.0) gamma_nJet40++;
              } // pt40
              if(cms3.pfjets_pfCombinedInclusiveSecondaryVertexV2BJetTag().at(iJet) >= 0.890) { //CSVv2IVFM
                gamma_nBJet20++; 
                if (p4sCorrJets.at(iJet).pt() > 25.0) gamma_nBJet25++; 
                if (p4sCorrJets.at(iJet).pt() > 30.0) {
                  gamma_nBJet30++;
                  if (p4sCorrJets.at(iJet).pt() > 40.0) gamma_nBJet40++; 
                  float mt = MT( p4sCorrJets.at(iJet).pt(),p4sCorrJets.at(iJet).phi(),gamma_met_pt,gamma_met_phi);
                  if (mt < gamma_minMTBMet) gamma_minMTBMet = mt;	 
                } // pt30
              } // pass med btag  
            } // not overlap with photon
          } // pt 20 eta 2.5 
          // accept jets out to eta 4.7 for dphi
          else if ( (p4sCorrJets.at(iJet).pt() > 30.0) && (fabs(p4sCorrJets.at(iJet).eta()) < 4.7) ) {
            //check against list of jets that overlap with a photon
            bool isOverlapJetGamma = false;
            for(unsigned int j=0; j<removedJetsGamma.size(); j++){
              if(iJet == removedJetsGamma.at(j)){
                isOverlapJetGamma = true;
                break;
              }
            }

            if(!isOverlapJetGamma) {
              p4sForDphiGamma.push_back(p4sCorrJets.at(iJet));
            }
          }


          njet++;
        } // pt 20 eta 4.7
      }

      // compute event level btag weights
      if (!isData && applyBtagSFs) {
        weight_btagsf = btagprob_data / btagprob_mc;
        weight_btagsf_UP = weight_btagsf + (sqrt(pow(btagprob_err_heavy_UP,2) + pow(btagprob_err_light_UP,2)) * weight_btagsf);
        weight_btagsf_DN = weight_btagsf - (sqrt(pow(btagprob_err_heavy_DN,2) + pow(btagprob_err_light_DN,2)) * weight_btagsf);
      }

      if (verbose) cout << "before hemispheres" << endl;

      // sort vectors by pt for hemisphere calculation
      sort(p4sForHems.begin(), p4sForHems.end(), sortByPt);
      sort(p4sForDphi.begin(), p4sForDphi.end(), sortByPt);
      sort(p4sForHemsGamma.begin(), p4sForHemsGamma.end(), sortByPt);
      sort(p4sForDphiGamma.begin(), p4sForDphiGamma.end(), sortByPt);
      sort(p4sForHemsZll.begin(), p4sForHemsZll.end(), sortByPt);
      sort(p4sForDphiZll.begin(), p4sForDphiZll.end(), sortByPt);
      sort(p4sForHemsZllMT.begin(), p4sForHemsZllMT.end(), sortByPt);
      sort(p4sForDphiZllMT.begin(), p4sForDphiZllMT.end(), sortByPt);
      sort(p4sForHemsRl.begin(), p4sForHemsRl.end(), sortByPt);
      sort(p4sForDphiRl.begin(), p4sForDphiRl.end(), sortByPt);

      ht = 0;
      deltaPhiMin = 999;
      LorentzVector sumMhtp4 = LorentzVector(0,0,0,0);

      // HT, MT2 and MHT

      // compute HT, MHT using same objects as MT2 inputs
      for (unsigned int ip4 = 0; ip4 < p4sForHems.size(); ++ip4) {
        ht += p4sForHems.at(ip4).pt();
        sumMhtp4 -= p4sForHems.at(ip4);
      }

      // min(dphi) of 4 leading objects
      for (unsigned int ip4 = 0; ip4 < p4sForDphi.size(); ++ip4) {
        if(ip4 < 4) deltaPhiMin = min(deltaPhiMin, DeltaPhi( met_phi, p4sForDphi.at(ip4).phi() ));
      }

      vector<LorentzVector> hemJets;
      if(p4sForHems.size() > 1){
        //Hemispheres used in MT2 calculation
        hemJets = getHemJets(p4sForHems);  

        mt2 = HemMT2(met_pt, met_phi, hemJets.at(0), hemJets.at(1));

        // order hemispheres by pt for saving
        int idx_lead = 0;
        int idx_subl = 1;
        if (hemJets.at(1).pt() > hemJets.at(0).pt()) {
          idx_lead = 1;
          idx_subl = 0;
        }

        pseudoJet1_pt   = hemJets.at(idx_lead).pt();
        pseudoJet1_eta  = hemJets.at(idx_lead).eta();
        pseudoJet1_phi  = hemJets.at(idx_lead).phi();
        pseudoJet1_mass = hemJets.at(idx_lead).mass();
        pseudoJet2_pt   = hemJets.at(idx_subl).pt();
        pseudoJet2_eta  = hemJets.at(idx_subl).eta();
        pseudoJet2_phi  = hemJets.at(idx_subl).phi();
        pseudoJet2_mass = hemJets.at(idx_subl).mass();
      }

      mht_pt  = sumMhtp4.pt();
      mht_phi = sumMhtp4.phi();

      TVector2 mhtVector = TVector2(mht_pt*cos(mht_phi), mht_pt*sin(mht_phi));
      TVector2 metVector = TVector2(met_pt*cos(met_phi), met_pt*sin(met_phi));
      diffMetMht = (mhtVector - metVector).Mod();

      // HT, MT2 and MHT for photon+jets regions
      //  note that leptons are NOT included in this MT2 calculation
      //  would need to do lepton/photon overlap to include them

      gamma_ht = 0;
      gamma_deltaPhiMin = 999;
      LorentzVector sumMhtp4Gamma = LorentzVector(0,0,0,0);

      // compute HT, MHT using same objects as MT2 inputs
      for (unsigned int ip4 = 0; ip4 < p4sForHemsGamma.size(); ++ip4) {
        gamma_ht += p4sForHemsGamma.at(ip4).pt();
        sumMhtp4Gamma -= p4sForHemsGamma.at(ip4);
      }

      // min(dphi) of 4 leading objects
      for (unsigned int ip4 = 0; ip4 < p4sForDphiGamma.size(); ++ip4) {
        if(ip4 < 4) gamma_deltaPhiMin = min(gamma_deltaPhiMin, DeltaPhi( gamma_met_phi, p4sForDphiGamma.at(ip4).phi() ));
      }

      vector<LorentzVector> hemJetsGamma;
      if(p4sForHemsGamma.size() > 1){
        //Hemispheres used in MT2 calculation
        hemJetsGamma = getHemJets(p4sForHemsGamma);  

        gamma_mt2 = HemMT2(gamma_met_pt, gamma_met_phi, hemJetsGamma.at(0), hemJetsGamma.at(1));
      }

      gamma_mht_pt  = sumMhtp4Gamma.pt();
      gamma_mht_phi = sumMhtp4Gamma.phi();

      TVector2 mhtVectorGamma = TVector2(gamma_mht_pt*cos(gamma_mht_phi), gamma_mht_pt*sin(gamma_mht_phi));
      TVector2 metVectorGamma = TVector2(gamma_met_pt*cos(gamma_met_phi), gamma_met_pt*sin(gamma_met_phi));
      gamma_diffMetMht = (mhtVectorGamma - metVectorGamma).Mod();

      // MT2, MHT for Z-->ll control region
      zll_ht = 0;
      zllmt_ht = 0;
      if (nlep == 2) {
        zll_deltaPhiMin = 999;
        LorentzVector sumMhtp4Zll = LorentzVector(0,0,0,0);

        // compute MHT using same objects as MT2 inputs
        for (unsigned int ip4 = 0; ip4 < p4sForHemsZll.size(); ++ip4) {
          zll_ht += p4sForHemsZll.at(ip4).pt();
          sumMhtp4Zll -= p4sForHemsZll.at(ip4);
        }

        // min(dphi) of 4 leading objects
        for (unsigned int ip4 = 0; ip4 < p4sForDphiZll.size(); ++ip4) {
          if(ip4 < 4) zll_deltaPhiMin = min(zll_deltaPhiMin, DeltaPhi( zll_met_phi, p4sForDphiZll.at(ip4).phi() ));
        }

        vector<LorentzVector> hemJetsZll;
        if(p4sForHemsZll.size() > 1){
          //Hemispheres used in MT2 calculation
          hemJetsZll = getHemJets(p4sForHemsZll);  

          zll_mt2 = HemMT2(zll_met_pt, zll_met_phi, hemJetsZll.at(0), hemJetsZll.at(1));
        }	  

        zll_mht_pt  = sumMhtp4Zll.pt();
        zll_mht_phi = sumMhtp4Zll.phi();

        TVector2 mhtVectorZll = TVector2(zll_mht_pt*cos(zll_mht_phi), zll_mht_pt*sin(zll_mht_phi));
        TVector2 metVectorZll = TVector2(zll_met_pt*cos(zll_met_phi), zll_met_pt*sin(zll_met_phi));
        zll_diffMetMht = (mhtVectorZll - metVectorZll).Mod();


        // same for Zll MT region test
        zllmt_deltaPhiMin = 999;
        LorentzVector sumMhtp4ZllMT = LorentzVector(0,0,0,0);

        // compute MHT using same objects as MT2 inputs
        for (unsigned int ip4 = 0; ip4 < p4sForHemsZllMT.size(); ++ip4) {
          zllmt_ht += p4sForHemsZllMT.at(ip4).pt();
          sumMhtp4ZllMT -= p4sForHemsZllMT.at(ip4);
        }

        // min(dphi) of 4 leading objects
        for (unsigned int ip4 = 0; ip4 < p4sForDphiZllMT.size(); ++ip4) {
          if(ip4 < 4) zllmt_deltaPhiMin = min(zllmt_deltaPhiMin, DeltaPhi( zllmt_met_phi, p4sForDphiZllMT.at(ip4).phi() ));
        }

        vector<LorentzVector> hemJetsZllMT;
        if(p4sForHemsZllMT.size() > 1){
          //Hemispheres used in MT2 calculation
          hemJetsZllMT = getHemJets(p4sForHemsZllMT);  

          zllmt_mt2 = HemMT2(zllmt_met_pt, zllmt_met_phi, hemJetsZllMT.at(0), hemJetsZllMT.at(1));
        }	  

        zllmt_mht_pt  = sumMhtp4ZllMT.pt();
        zllmt_mht_phi = sumMhtp4ZllMT.phi();

        TVector2 mhtVectorZllMT = TVector2(zllmt_mht_pt*cos(zllmt_mht_phi), zllmt_mht_pt*sin(zllmt_mht_phi));
        TVector2 metVectorZllMT = TVector2(zllmt_met_pt*cos(zllmt_met_phi), zllmt_met_pt*sin(zllmt_met_phi));
        zllmt_diffMetMht = (mhtVectorZllMT - metVectorZllMT).Mod();


      }

      // MT2, MHT for removed lepton control region
      rl_ht = 0;
      if (nlep == 1) {
        rl_deltaPhiMin = 999;
        LorentzVector sumMhtp4Rl = LorentzVector(0,0,0,0);

        // compute MHT using same objects as MT2 inputs
        for (unsigned int ip4 = 0; ip4 < p4sForHemsRl.size(); ++ip4) {
          rl_ht += p4sForHemsRl.at(ip4).pt();
          sumMhtp4Rl -= p4sForHemsRl.at(ip4);
        }

        // min(dphi) of 4 leading objects
        for (unsigned int ip4 = 0; ip4 < p4sForDphiRl.size(); ++ip4) {
          if(ip4 < 4) rl_deltaPhiMin = min(rl_deltaPhiMin, DeltaPhi( rl_met_phi, p4sForDphiRl.at(ip4).phi() ));
        }

        vector<LorentzVector> hemJetsRl;
        if(p4sForHemsRl.size() > 1){
          //Hemispheres used in MT2 calculation
          hemJetsRl = getHemJets(p4sForHemsRl);  

          rl_mt2 = HemMT2(rl_met_pt, rl_met_phi, hemJetsRl.at(0), hemJetsRl.at(1));
        }	  

        rl_mht_pt  = sumMhtp4Rl.pt();
        rl_mht_phi = sumMhtp4Rl.phi();

        TVector2 mhtVectorRl = TVector2(rl_mht_pt*cos(rl_mht_phi), rl_mht_pt*sin(rl_mht_phi));
        TVector2 metVectorRl = TVector2(rl_met_pt*cos(rl_met_phi), rl_met_pt*sin(rl_met_phi));
        rl_diffMetMht = (mhtVectorRl - metVectorRl).Mod();

      }
      if (!isData) {
        //GEN MT2
        vector<LorentzVector> goodGenJets;
        for(unsigned int iGenJet=0; iGenJet < cms3.genjets_p4NoMuNoNu().size(); iGenJet++){
          if(cms3.genjets_p4NoMuNoNu().at(iGenJet).pt() < 40.0) continue;
          if(fabs(cms3.genjets_p4NoMuNoNu().at(iGenJet).eta()) > 2.5) continue;
          goodGenJets.push_back(cms3.genjets_p4NoMuNoNu().at(iGenJet));
        }
        if(goodGenJets.size() > 1){
          hemJets = getHemJets(goodGenJets);  
          mt2_gen = HemMT2(met_genPt, met_genPhi, hemJets.at(0), hemJets.at(1));
        }
      } // !isData

      if (verbose) cout << "before taus" << endl;

      //TAUS
      ntau = 0;
      nTaus20 = 0;
      for(unsigned int iTau = 0; iTau < cms3.taus_pf_p4().size(); iTau++){
        if(cms3.taus_pf_p4().at(iTau).pt() < 20.0) continue; 
        if(fabs(cms3.taus_pf_p4().at(iTau).eta()) > 2.3) continue; 
        if (!cms3.passTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits", iTau)) continue; // HPS3 hits taus

        if (ntau >= max_ntau) {
          std::cout << "WARNING: attempted to fill more than " << max_ntau << " taus" << std::endl;
          break;
        }

        tau_pt[ntau]   = cms3.taus_pf_p4().at(iTau).pt();
        tau_eta[ntau]  = cms3.taus_pf_p4().at(iTau).eta();
        tau_phi[ntau]  = cms3.taus_pf_p4().at(iTau).phi();
        tau_mass[ntau] = cms3.taus_pf_mass().at(iTau);
        tau_charge[ntau] = cms3.taus_pf_charge().at(iTau);
        tau_dxy[ntau] = 0; // could use the tau->dxy() function instead, but not sure what it does
        tau_dz[ntau] = 0; // not sure how to get this. 
        tau_isoCI3hit[ntau] = cms3.passTauID("byCombinedIsolationDeltaBetaCorrRaw3Hits", iTau);
        int temp = 0;
        if (cms3.passTauID("byLooseCombinedIsolationDeltaBetaCorr3Hits", iTau)) temp = 1;
        if (cms3.passTauID("byMediumCombinedIsolationDeltaBetaCorr3Hits", iTau)) temp = 2;
        if (cms3.passTauID("byTightCombinedIsolationDeltaBetaCorr3Hits", iTau)) temp = 3;
        tau_idCI3hit[ntau] = temp;
        if(tau_pt[ntau] > 20) nTaus20++;
        //tau_mcMatchId[ntau] = ; // Have to do this by hand unless we want to add tau_mc branches in CMS3 through the CandToGenAssMaker

        ntau++;
      }

      if (!isData && applyDummyWeights) {
        // dummy version of generator scales variation
        if (mt2 < 600.) {
          weight_scales_UP = 1.05;
          weight_scales_DN = 0.95;
        }
        else if (mt2 > 600. && mt2 < 1000.) {
          weight_scales_UP = 1.10;
          weight_scales_DN = 0.90;
        }
        else if (mt2 > 1000.) {
          weight_scales_UP = 1.30;
          weight_scales_DN = 0.70;
        }
      }

      if (verbose) cout << "before fill" << endl;

      FillBabyNtuple();

    }//end loop on events in a file

    delete tree;
    f.Close();
    }//end loop on files

    cout << "Processed " << nEventsTotal << " events" << endl;
    if ( nEventsChain != nEventsTotal ) {
      std::cout << "ERROR: number of events from files is not equal to total number of events" << std::endl;
    }

    cout << nDuplicates << " duplicate events were skipped." << endl;
    cout << nFailJSON << " events were removed by JSON." << endl;
    cout << nFailRunNumber << " events were removed due to run number." << endl;

    CloseBabyNtuple();

    if (applyBtagSFs) {
      delete calib;
      delete reader_heavy;
      delete reader_heavy_UP;
      delete reader_heavy_DN;
      delete reader_light;
      delete reader_light_UP;
      delete reader_light_DN;
      if (isFastsim) {
	delete calib_fastsim;
	delete reader_fastsim;
	delete reader_fastsim_UP;
	delete reader_fastsim_DN;
      }
    }

    if (applyJECfromFile) delete jet_corrector_pfL1FastJetL2L3;
    if (!isDataFromFileName && applyJECfromFile && applyJECunc != 0) delete jetcorr_uncertainty;

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

  void babyMaker::MakeBabyNtuple(const char *BabyFilename){

    BabyFile_ = new TFile(Form("%s", BabyFilename), "RECREATE");
    BabyFile_->cd();
    BabyTree_ = new TTree("mt2", "A Baby Ntuple");

    BabyTree_->Branch("run", &run );
    BabyTree_->Branch("lumi", &lumi );
    BabyTree_->Branch("evt", &evt, "evt/l" );
    BabyTree_->Branch("isData", &isData );
    BabyTree_->Branch("evt_scale1fb", &evt_scale1fb);
    BabyTree_->Branch("evt_xsec", &evt_xsec );
    BabyTree_->Branch("evt_kfactor", &evt_kfactor );
    BabyTree_->Branch("evt_filter", &evt_filter );
    BabyTree_->Branch("evt_nEvts", &evt_nEvts, "evt_nEvts/l" );
    BabyTree_->Branch("evt_id", &evt_id );
    BabyTree_->Branch("genWeight", &genWeight );
    BabyTree_->Branch("puWeight", &puWeight );
    BabyTree_->Branch("nVert", &nVert );
    BabyTree_->Branch("nTrueInt", &nTrueInt );
    BabyTree_->Branch("rho", &rho );
    BabyTree_->Branch("nJet30", &nJet30 );
    BabyTree_->Branch("nJet40", &nJet40 );
    BabyTree_->Branch("nBJet20", &nBJet20 );
    BabyTree_->Branch("nBJet25", &nBJet25 );
    BabyTree_->Branch("nBJet30", &nBJet30 );
    BabyTree_->Branch("nBJet40", &nBJet40 );
    BabyTree_->Branch("nJet30FailId", &nJet30FailId );
    BabyTree_->Branch("nJet100FailId", &nJet100FailId );
    BabyTree_->Branch("nMuons10", &nMuons10 );
    BabyTree_->Branch("nElectrons10", &nElectrons10 );
    BabyTree_->Branch("nLepLowMT", &nLepLowMT );
    BabyTree_->Branch("nTaus20", &nTaus20 );
    BabyTree_->Branch("nGammas20", &nGammas20 );
    BabyTree_->Branch("nPFCHCand3", &nPFCHCand3 );
    BabyTree_->Branch("deltaPhiMin", &deltaPhiMin );
    BabyTree_->Branch("diffMetMht", &diffMetMht );
    BabyTree_->Branch("minMTBMet", &minMTBMet );
    BabyTree_->Branch("zll_minMTBMet", &zll_minMTBMet );
    BabyTree_->Branch("gamma_minMTBMet", &gamma_minMTBMet );
    BabyTree_->Branch("ht", &ht );
    BabyTree_->Branch("mt2", &mt2 );
    BabyTree_->Branch("mt2_gen", &mt2_gen );
    BabyTree_->Branch("jet1_pt", &jet1_pt );
    BabyTree_->Branch("jet2_pt", &jet2_pt );
    BabyTree_->Branch("gamma_jet1_pt", &gamma_jet1_pt );
    BabyTree_->Branch("gamma_jet2_pt", &gamma_jet2_pt );
    BabyTree_->Branch("pseudoJet1_pt", &pseudoJet1_pt );
    BabyTree_->Branch("pseudoJet1_eta", &pseudoJet1_eta );
    BabyTree_->Branch("pseudoJet1_phi", &pseudoJet1_phi );
    BabyTree_->Branch("pseudoJet1_mass", &pseudoJet1_mass );
    BabyTree_->Branch("pseudoJet2_pt", &pseudoJet2_pt );
    BabyTree_->Branch("pseudoJet2_eta", &pseudoJet2_eta );
    BabyTree_->Branch("pseudoJet2_phi", &pseudoJet2_phi );
    BabyTree_->Branch("pseudoJet2_mass", &pseudoJet2_mass );
    BabyTree_->Branch("mht_pt", &mht_pt );
    BabyTree_->Branch("mht_phi", &mht_phi );
    BabyTree_->Branch("met_pt", &met_pt );
    BabyTree_->Branch("met_phi", &met_phi );
    BabyTree_->Branch("met_rawPt",  &met_rawPt );
    BabyTree_->Branch("met_rawPhi", &met_rawPhi );
    BabyTree_->Branch("met_caloPt",  &met_caloPt );
    BabyTree_->Branch("met_caloPhi", &met_caloPhi );
    BabyTree_->Branch("met_trkPt",  &met_trkPt );
    BabyTree_->Branch("met_trkPhi", &met_trkPhi );
    BabyTree_->Branch("met_genPt",  &met_genPt );
    BabyTree_->Branch("met_genPhi", &met_genPhi );
    BabyTree_->Branch("Flag_EcalDeadCellTriggerPrimitiveFilter", &Flag_EcalDeadCellTriggerPrimitiveFilter );
    BabyTree_->Branch("Flag_trkPOG_manystripclus53X", &Flag_trkPOG_manystripclus53X );
    BabyTree_->Branch("Flag_ecalLaserCorrFilter", &Flag_ecalLaserCorrFilter );
    BabyTree_->Branch("Flag_trkPOG_toomanystripclus53X", &Flag_trkPOG_toomanystripclus53X );
    BabyTree_->Branch("Flag_hcalLaserEventFilter", &Flag_hcalLaserEventFilter );
    BabyTree_->Branch("Flag_trkPOG_logErrorTooManyClusters", &Flag_trkPOG_logErrorTooManyClusters );
    BabyTree_->Branch("Flag_trkPOGFilters", &Flag_trkPOGFilters );
    BabyTree_->Branch("Flag_trackingFailureFilter", &Flag_trackingFailureFilter );
    BabyTree_->Branch("Flag_CSCTightHaloFilter", &Flag_CSCTightHaloFilter );
    BabyTree_->Branch("Flag_HBHENoiseFilter", &Flag_HBHENoiseFilter );
    BabyTree_->Branch("Flag_HBHEIsoNoiseFilter", &Flag_HBHEIsoNoiseFilter );
    BabyTree_->Branch("Flag_goodVertices", &Flag_goodVertices );
    BabyTree_->Branch("Flag_eeBadScFilter", &Flag_eeBadScFilter );
    BabyTree_->Branch("Flag_METFilters", &Flag_METFilters );
    BabyTree_->Branch("HLT_PFHT800", &HLT_PFHT800 );
    BabyTree_->Branch("HLT_PFHT900", &HLT_PFHT900 );
    BabyTree_->Branch("HLT_PFMET170", &HLT_PFMET170 );
    BabyTree_->Branch("HLT_PFHT350_PFMET100", &HLT_PFHT350_PFMET100 );
    BabyTree_->Branch("HLT_PFHT350_PFMET120", &HLT_PFHT350_PFMET120 );
    BabyTree_->Branch("HLT_PFMETNoMu90_PFMHTNoMu90", &HLT_PFMETNoMu90_PFMHTNoMu90 );
    BabyTree_->Branch("HLT_MonoCentralPFJet80_PFMETNoMu90_PFMHTNoMu90", &HLT_MonoCentralPFJet80_PFMETNoMu90_PFMHTNoMu90 );
    BabyTree_->Branch("HLT_PFMETNoMu120_PFMHTNoMu120", &HLT_PFMETNoMu120_PFMHTNoMu120 );
    BabyTree_->Branch("HLT_PFMET90_PFMHT90", &HLT_PFMET90_PFMHT90 );
    BabyTree_->Branch("HLT_SingleMu", &HLT_SingleMu );
    BabyTree_->Branch("HLT_SingleEl", &HLT_SingleEl );
    BabyTree_->Branch("HLT_DoubleEl", &HLT_DoubleEl );
    BabyTree_->Branch("HLT_DoubleEl33", &HLT_DoubleEl33 );
    BabyTree_->Branch("HLT_MuX_Ele12", &HLT_MuX_Ele12 );
    BabyTree_->Branch("HLT_Mu8_EleX", &HLT_Mu8_EleX );
    BabyTree_->Branch("HLT_DoubleMu", &HLT_DoubleMu );
    BabyTree_->Branch("HLT_Photon120", &HLT_Photon120 );
    BabyTree_->Branch("HLT_Photon165_HE10", &HLT_Photon165_HE10 );
    BabyTree_->Branch("HLT_PFHT200_Prescale", &HLT_PFHT200_Prescale );
    BabyTree_->Branch("HLT_PFHT300_Prescale", &HLT_PFHT300_Prescale );
    BabyTree_->Branch("HLT_PFHT350_Prescale", &HLT_PFHT350_Prescale );
    BabyTree_->Branch("HLT_PFHT475_Prescale", &HLT_PFHT475_Prescale );
    BabyTree_->Branch("HLT_PFHT600_Prescale", &HLT_PFHT600_Prescale );
    BabyTree_->Branch("HLT_DiCentralPFJet70_PFMET120", &HLT_DiCentralPFJet70_PFMET120 );
    BabyTree_->Branch("HLT_DiCentralPFJet55_PFMET110", &HLT_DiCentralPFJet55_PFMET110 );
    BabyTree_->Branch("nlep", &nlep, "nlep/I" );
    BabyTree_->Branch("lep_pt", lep_pt, "lep_pt[nlep]/F");
    BabyTree_->Branch("lep_eta", lep_eta, "lep_eta[nlep]/F" );
    BabyTree_->Branch("lep_phi", lep_phi, "lep_phi[nlep]/F" );
    BabyTree_->Branch("lep_mass", lep_mass, "lep_mass[nlep]/F" );
    BabyTree_->Branch("lep_charge", lep_charge, "lep_charge[nlep]/I" );
    BabyTree_->Branch("lep_pdgId", lep_pdgId, "lep_pdgId[nlep]/I" );
    BabyTree_->Branch("lep_dxy", lep_dxy, "lep_dxy[nlep]/F" );
    BabyTree_->Branch("lep_dz", lep_dz, "lep_dz[nlep]/F" );
    BabyTree_->Branch("lep_tightId", lep_tightId, "lep_tightId[nlep]/I" );
    BabyTree_->Branch("lep_heepId", lep_heepId, "lep_heepId[nlep]/I" );
    BabyTree_->Branch("lep_relIso03", lep_relIso03, "lep_relIso03[nlep]/F" );
    BabyTree_->Branch("lep_relIso04", lep_relIso04, "lep_relIso04[nlep]/F" );
    BabyTree_->Branch("lep_miniRelIso", lep_miniRelIso, "lep_miniRelIso[nlep]/F" );
    BabyTree_->Branch("lep_relIsoAn04", lep_relIsoAn04, "lep_relIsoAn04[nlep]/F" );
    BabyTree_->Branch("lep_mcMatchId", lep_mcMatchId, "lep_mcMatchId[nlep]/I" );
    BabyTree_->Branch("lep_lostHits", lep_lostHits, "lep_lostHits[nlep]/I" );
    BabyTree_->Branch("lep_convVeto", lep_convVeto, "lep_convVeto[nlep]/I" );
    BabyTree_->Branch("lep_tightCharge", lep_tightCharge, "lep_tightCharge[nlep]/I" );
    BabyTree_->Branch("nisoTrack", &nisoTrack, "nisoTrack/I" );
    BabyTree_->Branch("isoTrack_pt", isoTrack_pt, "isoTrack_pt[nisoTrack]/F" );
    BabyTree_->Branch("isoTrack_eta", isoTrack_eta, "isoTrack_eta[nisoTrack]/F" );
    BabyTree_->Branch("isoTrack_phi", isoTrack_phi, "isoTrack_phi[nisoTrack]/F" );
    BabyTree_->Branch("isoTrack_mass", isoTrack_mass, "isoTrack_mass[nisoTrack]/F" );
    BabyTree_->Branch("isoTrack_absIso", isoTrack_absIso, "isoTrack_absIso[nisoTrack]/F" );
    BabyTree_->Branch("isoTrack_relIsoAn04", isoTrack_relIsoAn04, "isoTrack_relIsoAn04[nisoTrack]/F" );
    BabyTree_->Branch("isoTrack_dz", isoTrack_dz, "isoTrack_dz[nisoTrack]/F" );
    BabyTree_->Branch("isoTrack_pdgId", isoTrack_pdgId, "isoTrack_pdgId[nisoTrack]/I" );
    BabyTree_->Branch("isoTrack_mcMatchId", isoTrack_mcMatchId, "isoTrack_mcMatchId[nisoTrack]/I" );
    BabyTree_->Branch("nPFLep5LowMT", &nPFLep5LowMT, "nPFLep5LowMT/I" );
    BabyTree_->Branch("nPFHad10LowMT", &nPFHad10LowMT, "nPFHad10LowMT/I" );
    BabyTree_->Branch("ntau", &ntau, "ntau/I" );
    BabyTree_->Branch("tau_pt", tau_pt, "tau_pt[ntau]/F" );
    BabyTree_->Branch("tau_eta", tau_eta, "tau_eta[ntau]/F" );
    BabyTree_->Branch("tau_phi", tau_phi, "tau_phi[ntau]/F" );
    BabyTree_->Branch("tau_mass", tau_mass, "tau_mass[ntau]/F" );
    BabyTree_->Branch("tau_charge", tau_charge, "tau_charge[ntau]/I" );
    BabyTree_->Branch("tau_dxy", tau_dxy, "tau_dxy[ntau]/F" );
    BabyTree_->Branch("tau_dz", tau_dz, "tau_dz[ntau]/F" );
    BabyTree_->Branch("tau_idCI3hit", tau_idCI3hit, "tau_idCI3hit[ntau]/I" );
    BabyTree_->Branch("tau_isoCI3hit", tau_isoCI3hit, "tau_isoCI3hit[ntau]/F" );
    BabyTree_->Branch("tau_mcMatchId", tau_mcMatchId, "tau_mcMatchId[ntau]/I" );
    BabyTree_->Branch("ngamma", &ngamma, "ngamma/I" );
    BabyTree_->Branch("gamma_pt", gamma_pt, "gamma_pt[ngamma]/F" );
    BabyTree_->Branch("gamma_eta", gamma_eta, "gamma_eta[ngamma]/F" );
    BabyTree_->Branch("gamma_phi", gamma_phi, "gamma_phi[ngamma]/F" );
    BabyTree_->Branch("gamma_mass", gamma_mass, "gamma_mass[ngamma]/F" );
    BabyTree_->Branch("gamma_mcMatchId", gamma_mcMatchId, "gamma_mcMatchId[ngamma]/I" );
    BabyTree_->Branch("gamma_genIso04", gamma_genIso04, "gamma_genIso04[ngamma]/F" );
    BabyTree_->Branch("gamma_drMinParton", gamma_drMinParton, "gamma_drMinParton[ngamma]/F" );
    BabyTree_->Branch("gamma_chHadIso", gamma_chHadIso, "gamma_chHadIso[ngamma]/F" );
    BabyTree_->Branch("gamma_neuHadIso", gamma_neuHadIso, "gamma_neuHadIso[ngamma]/F" );
    BabyTree_->Branch("gamma_phIso", gamma_phIso, "gamma_phIso[ngamma]/F" );
    BabyTree_->Branch("gamma_sigmaIetaIeta", gamma_sigmaIetaIeta, "gamma_sigmaIetaIeta[ngamma]/F" );
    BabyTree_->Branch("gamma_r9", gamma_r9, "gamma_r9[ngamma]/F" );
    BabyTree_->Branch("gamma_hOverE", gamma_hOverE, "gamma_hOverE[ngamma]/F" );
    BabyTree_->Branch("gamma_idCutBased", gamma_idCutBased, "gamma_idCutBased[ngamma]/I" );
    BabyTree_->Branch("gamma_mt2", &gamma_mt2 );
    BabyTree_->Branch("gamma_nJet30", &gamma_nJet30 );
    BabyTree_->Branch("gamma_nJet40", &gamma_nJet40 );
    BabyTree_->Branch("gamma_nJet30FailId", &gamma_nJet30FailId );
    BabyTree_->Branch("gamma_nJet100FailId", &gamma_nJet100FailId );
    BabyTree_->Branch("gamma_nBJet20", &gamma_nBJet20 );
    BabyTree_->Branch("gamma_nBJet25", &gamma_nBJet25 );
    BabyTree_->Branch("gamma_nBJet30", &gamma_nBJet30 );
    BabyTree_->Branch("gamma_nBJet40", &gamma_nBJet40 );
    BabyTree_->Branch("gamma_ht", &gamma_ht );
    BabyTree_->Branch("gamma_deltaPhiMin", &gamma_deltaPhiMin );
    BabyTree_->Branch("gamma_diffMetMht", &gamma_diffMetMht );
    BabyTree_->Branch("gamma_mht_pt", &gamma_mht_pt );
    BabyTree_->Branch("gamma_mht_phi", &gamma_mht_phi );
    BabyTree_->Branch("gamma_met_pt", &gamma_met_pt );
    BabyTree_->Branch("gamma_met_phi", &gamma_met_phi );
    BabyTree_->Branch("zll_mt2", &zll_mt2 );
    BabyTree_->Branch("zll_deltaPhiMin", &zll_deltaPhiMin );
    BabyTree_->Branch("zll_diffMetMht", &zll_diffMetMht );
    BabyTree_->Branch("zll_met_pt", &zll_met_pt );
    BabyTree_->Branch("zll_met_phi", &zll_met_phi );
    BabyTree_->Branch("zll_mht_pt", &zll_mht_pt );
    BabyTree_->Branch("zll_mht_phi", &zll_mht_phi );
    BabyTree_->Branch("zll_mass", &zll_mass );
    BabyTree_->Branch("zll_pt", &zll_pt );
    BabyTree_->Branch("zll_eta", &zll_eta );
    BabyTree_->Branch("zll_phi", &zll_phi );
    BabyTree_->Branch("zll_ht", &zll_ht );
    BabyTree_->Branch("zllmt_mt2", &zllmt_mt2 );
    BabyTree_->Branch("zllmt_deltaPhiMin", &zllmt_deltaPhiMin );
    BabyTree_->Branch("zllmt_diffMetMht", &zllmt_diffMetMht );
    BabyTree_->Branch("zllmt_met_pt", &zllmt_met_pt );
    BabyTree_->Branch("zllmt_met_phi", &zllmt_met_phi );
    BabyTree_->Branch("zllmt_mht_pt", &zllmt_mht_pt );
    BabyTree_->Branch("zllmt_mht_phi", &zllmt_mht_phi );
    BabyTree_->Branch("zllmt_ht", &zllmt_ht );
    BabyTree_->Branch("zllmt_mt", &zllmt_mt );
    BabyTree_->Branch("rl_mt2", &rl_mt2 );
    BabyTree_->Branch("rl_deltaPhiMin", &rl_deltaPhiMin );
    BabyTree_->Branch("rl_diffMetMht", &rl_diffMetMht );
    BabyTree_->Branch("rl_met_pt", &rl_met_pt );
    BabyTree_->Branch("rl_met_phi", &rl_met_phi );
    BabyTree_->Branch("rl_mht_pt", &rl_mht_pt );
    BabyTree_->Branch("rl_mht_phi", &rl_mht_phi );
    BabyTree_->Branch("rl_mass", &rl_mass );
    BabyTree_->Branch("rl_pt", &rl_pt );
    BabyTree_->Branch("rl_eta", &rl_eta );
    BabyTree_->Branch("rl_phi", &rl_phi );
    BabyTree_->Branch("rl_ht", &rl_ht );
    if (!isDataFromFileName) {
      if (saveGenParticles) {
        BabyTree_->Branch("ngenPart", &ngenPart, "ngenPart/I" );
        BabyTree_->Branch("genPart_pt", genPart_pt, "genPart_pt[ngenPart]/F" );
        BabyTree_->Branch("genPart_eta", genPart_eta, "genPart_eta[ngenPart]/F" );
        BabyTree_->Branch("genPart_phi", genPart_phi, "genPart_phi[ngenPart]/F" );
        BabyTree_->Branch("genPart_mass", genPart_mass, "genPart_mass[ngenPart]/F" );
        BabyTree_->Branch("genPart_pdgId", genPart_pdgId, "genPart_pdgId[ngenPart]/I" );
        BabyTree_->Branch("genPart_status", genPart_status, "genPart_status[ngenPart]/I" );
        BabyTree_->Branch("genPart_charge", genPart_charge, "genPart_charge[ngenPart]/F" );
        BabyTree_->Branch("genPart_motherId", genPart_motherId, "genPart_motherId[ngenPart]/I" );
        BabyTree_->Branch("genPart_grandmotherId", genPart_grandmotherId, "genPart_grandmotherId[ngenPart]/I" );
      }
      BabyTree_->Branch("ngenLep", &ngenLep, "ngenLep/I" );
      BabyTree_->Branch("genLep_pt", genLep_pt, "genLep_pt[ngenLep]/F" );
      BabyTree_->Branch("genLep_eta", genLep_eta, "genLep_eta[ngenLep]/F" );
      BabyTree_->Branch("genLep_phi", genLep_phi, "genLep_phi[ngenLep]/F" );
      BabyTree_->Branch("genLep_mass", genLep_mass, "genLep_mass[ngenLep]/F" );
      BabyTree_->Branch("genLep_pdgId", genLep_pdgId, "genLep_pdgId[ngenLep]/I" );
      BabyTree_->Branch("genLep_status", genLep_status, "genLep_status[ngenLep]/I" );
      BabyTree_->Branch("genLep_charge", genLep_charge, "genLep_charge[ngenLep]/F" );
      BabyTree_->Branch("genLep_sourceId", genLep_sourceId, "genLep_sourceId[ngenLep]/I" );
      BabyTree_->Branch("ngenStat23", &ngenStat23, "ngenStat23/I" );
      BabyTree_->Branch("genStat23_pt", genStat23_pt, "genStat23_pt[ngenStat23]/F" );
      BabyTree_->Branch("genStat23_eta", genStat23_eta, "genStat23_eta[ngenStat23]/F" );
      BabyTree_->Branch("genStat23_phi", genStat23_phi, "genStat23_phi[ngenStat23]/F" );
      BabyTree_->Branch("genStat23_mass", genStat23_mass, "genStat23_mass[ngenStat23]/F" );
      BabyTree_->Branch("genStat23_pdgId", genStat23_pdgId, "genStat23_pdgId[ngenStat23]/I" );
      BabyTree_->Branch("genStat23_status", genStat23_status, "genStat23_status[ngenStat23]/I" );
      BabyTree_->Branch("genStat23_charge", genStat23_charge, "genStat23_charge[ngenStat23]/F" );
      BabyTree_->Branch("genStat23_sourceId", genStat23_sourceId, "genStat23_sourceId[ngenStat23]/I" );
      BabyTree_->Branch("ngenGamma", &ngenGamma, "ngenGamma/I" );
      BabyTree_->Branch("genGamma_pt", genGamma_pt, "genGamma_pt[ngenGamma]/F" );
      BabyTree_->Branch("genGamma_eta", genGamma_eta, "genGamma_eta[ngenGamma]/F" );
      BabyTree_->Branch("genGamma_phi", genGamma_phi, "genGamma_phi[ngenGamma]/F" );
      BabyTree_->Branch("genGamma_motherId", genGamma_motherId, "genGamma_motherId[ngenGamma]/I" );
      BabyTree_->Branch("ngenTau", &ngenTau, "ngenTau/I" );
      BabyTree_->Branch("ngenTau1Prong", &ngenTau1Prong, "ngenTau1Prong/I" );
      BabyTree_->Branch("ngenTau3Prong", &ngenTau3Prong, "ngenTau3Prong/I" );
      BabyTree_->Branch("genTau_pt", genTau_pt, "genTau_pt[ngenTau]/F" );
      BabyTree_->Branch("genTau_eta", genTau_eta, "genTau_eta[ngenTau]/F" );
      BabyTree_->Branch("genTau_phi", genTau_phi, "genTau_phi[ngenTau]/F" );
      BabyTree_->Branch("genTau_mass", genTau_mass, "genTau_mass[ngenTau]/F" );
      BabyTree_->Branch("genTau_pdgId", genTau_pdgId, "genTau_pdgId[ngenTau]/I" );
      BabyTree_->Branch("genTau_status", genTau_status, "genTau_status[ngenTau]/I" );
      BabyTree_->Branch("genTau_charge", genTau_charge, "genTau_charge[ngenTau]/F" );
      BabyTree_->Branch("genTau_sourceId", genTau_sourceId, "genTau_sourceId[ngenTau]/I" );
      BabyTree_->Branch("genTau_decayMode", genTau_decayMode, "genTau_decayMode[ngenTau]/I" );
      BabyTree_->Branch("genTau_leadTrackPt", genTau_leadTrackPt, "genTau_leadTrackPt[ngenTau]/F" );
      BabyTree_->Branch("genTau_neutralDaughters", genTau_neutralDaughters, "genTau_neutralDaughters[ngenTau]/I" );
      BabyTree_->Branch("ngenLepFromTau", &ngenLepFromTau, "ngenLepFromTau/I" );
      BabyTree_->Branch("genLepFromTau_pt", genLepFromTau_pt, "genLepFromTau_pt[ngenLepFromTau]/F" );
      BabyTree_->Branch("genLepFromTau_eta", genLepFromTau_eta, "genLepFromTau_eta[ngenLepFromTau]/F" );
      BabyTree_->Branch("genLepFromTau_phi", genLepFromTau_phi, "genLepFromTau_phi[ngenLepFromTau]/F" );
      BabyTree_->Branch("genLepFromTau_mass", genLepFromTau_mass, "genLepFromTau_mass[ngenLepFromTau]/F" );
      BabyTree_->Branch("genLepFromTau_pdgId", genLepFromTau_pdgId, "genLepFromTau_pdgId[ngenLepFromTau]/I" );
      BabyTree_->Branch("genLepFromTau_status", genLepFromTau_status, "genLepFromTau_status[ngenLepFromTau]/I" );
      BabyTree_->Branch("genLepFromTau_charge", genLepFromTau_charge, "genLepFromTau_charge[ngenLepFromTau]/F" );
      BabyTree_->Branch("genLepFromTau_sourceId", genLepFromTau_sourceId, "genLepFromTau_sourceId[ngenLepFromTau]/I" );
      BabyTree_->Branch("GenSusyMScan1", &GenSusyMScan1 );
      BabyTree_->Branch("GenSusyMScan2", &GenSusyMScan2 );
      BabyTree_->Branch("GenSusyMScan3", &GenSusyMScan3 );
      BabyTree_->Branch("GenSusyMScan4", &GenSusyMScan4 );
      if (saveLHEweights || saveLHEweightsScaleOnly) {
	BabyTree_->Branch("nLHEweight", &nLHEweight, "nLHEweight/I" );
	BabyTree_->Branch("LHEweight_wgt", LHEweight_wgt, "LHEweight_wgt[nLHEweight]/F" );
      }
    }
    BabyTree_->Branch("njet", &njet, "njet/I" );
    BabyTree_->Branch("jet_pt", jet_pt, "jet_pt[njet]/F" );
    BabyTree_->Branch("jet_eta", jet_eta, "jet_eta[njet]/F" );
    BabyTree_->Branch("jet_phi", jet_phi, "jet_phi[njet]/F" );
    BabyTree_->Branch("jet_mass", jet_mass, "jet_mass[njet]/F" );
    BabyTree_->Branch("jet_btagCSV", jet_btagCSV, "jet_btagCSV[njet]/F" );
    BabyTree_->Branch("jet_rawPt", jet_rawPt, "jet_rawPt[njet]/F" );
    BabyTree_->Branch("jet_mcPt", jet_mcPt, "jet_mcPt[njet]/F" );
    BabyTree_->Branch("jet_mcFlavour", jet_mcFlavour, "jet_mcFlavour[njet]/I" );
    BabyTree_->Branch("jet_qgl", jet_qgl, "jet_qgl[njet]/F" );
    BabyTree_->Branch("jet_area", jet_area, "jet_area[njet]/F" );
    BabyTree_->Branch("jet_id", jet_id, "jet_id[njet]/I" );
    BabyTree_->Branch("jet_puId", jet_puId, "jet_puId[njet]/I" );
    BabyTree_->Branch("weight_lepsf", &weight_lepsf );
    BabyTree_->Branch("weight_lepsf_UP", &weight_lepsf_UP );
    BabyTree_->Branch("weight_lepsf_DN", &weight_lepsf_DN );
    BabyTree_->Branch("weight_btagsf", &weight_btagsf );
    BabyTree_->Branch("weight_btagsf_UP", &weight_btagsf_UP );
    BabyTree_->Branch("weight_btagsf_DN", &weight_btagsf_DN );
    BabyTree_->Branch("weight_sigtrigsf", &weight_sigtrigsf );
    BabyTree_->Branch("weight_dileptrigsf", &weight_dileptrigsf );
    BabyTree_->Branch("weight_phottrigsf", &weight_phottrigsf );
    BabyTree_->Branch("weight_pu", &weight_pu );
    BabyTree_->Branch("weight_isr", &weight_isr );
    BabyTree_->Branch("weight_scales_UP", &weight_scales_UP );
    BabyTree_->Branch("weight_scales_DN", &weight_scales_DN );
    BabyTree_->Branch("weight_pdfs_UP", &weight_pdfs_UP );
    BabyTree_->Branch("weight_pdfs_DN", &weight_pdfs_DN );
    BabyTree_->Branch("weight_toppt", &weight_toppt );
    BabyTree_->Branch("genRecoil_pt", &genRecoil_pt );
    BabyTree_->Branch("genTop_pt", &genTop_pt );
    BabyTree_->Branch("genTbar_pt", &genTbar_pt );

    // also make counter histogram
    count_hist_ = new TH1D("Count","Count",1,0,2);

    return;
  }

  void babyMaker::InitBabyNtuple () {

    run    = -999;
    lumi   = -999;
    evt    = -999;
    isData = -999;
    evt_scale1fb = 0;
    evt_xsec = -999.0;
    evt_kfactor = -999.0;
    evt_filter = -999.0;
    evt_nEvts = 0;
    //evt_id = -1; // don't reset every event
    genWeight = 0;
    puWeight = -999.0;
    nVert = -999;
    nTrueInt = -999;
    rho = -999.0;
    nJet30 = -999;
    nJet40 = -999;
    nBJet20 = -999;
    nBJet25 = -999;
    nBJet30 = -999;
    nBJet40 = -999;
    nJet30FailId = -999;
    nJet100FailId = -999;
    nMuons10 = -999;
    nElectrons10 = -999;
    nLepLowMT = -999;
    nTaus20 = -999;
    nGammas20 = -999;
    nPFCHCand3 = -999;
    deltaPhiMin = -999.0;
    diffMetMht = -999.0;
    minMTBMet = -999.0;
    zll_minMTBMet = -999.0;
    rl_minMTBMet = -999.0;
    gamma_minMTBMet = -999.0;
    ht = -999.0;
    mt2 = -999.0;
    mt2_gen = -999.0;
    jet1_pt = 0.0;
    jet2_pt = 0.0;
    gamma_jet1_pt = 0.0;
    gamma_jet2_pt = 0.0;
    pseudoJet1_pt = 0.0;
    pseudoJet1_eta = 0.0;
    pseudoJet1_phi = 0.0;
    pseudoJet1_mass = 0.0;
    pseudoJet2_pt = 0.0;
    pseudoJet2_eta = 0.0;
    pseudoJet2_phi = 0.0;
    pseudoJet2_mass = 0.0;
    mht_pt = -999.0;
    mht_phi = -999.0;
    met_pt = -999.0;
    met_phi = -999.0;
    met_rawPt = -999.0;
    met_rawPhi = -999.0;
    met_caloPt = -999.0;
    met_caloPhi = -999.0;
    met_trkPt = -999.0;
    met_trkPhi = -999.0;
    met_genPt = -999.0;
    met_genPhi = -999.0;
    Flag_EcalDeadCellTriggerPrimitiveFilter = -999;
    Flag_trkPOG_manystripclus53X = -999;
    Flag_ecalLaserCorrFilter = -999;
    Flag_trkPOG_toomanystripclus53X = -999;
    Flag_hcalLaserEventFilter = -999;
    Flag_trkPOG_logErrorTooManyClusters = -999;
    Flag_trkPOGFilters = -999;
    Flag_trackingFailureFilter = -999;
    Flag_CSCTightHaloFilter = -999;
    Flag_HBHENoiseFilter = -999;
    Flag_HBHEIsoNoiseFilter = -999;
    Flag_goodVertices = -999;
    Flag_eeBadScFilter = -999;
    Flag_METFilters = -999;
    HLT_PFHT800 = -999;
    HLT_PFHT900 = -999;
    HLT_PFMET170 = -999;
    HLT_PFHT350_PFMET100 = -999;
    HLT_PFHT350_PFMET120 = -999;
    HLT_PFMETNoMu90_PFMHTNoMu90 = -999;
    HLT_MonoCentralPFJet80_PFMETNoMu90_PFMHTNoMu90 = -999;
    HLT_PFMETNoMu120_PFMHTNoMu120 = -999;
    HLT_PFMET90_PFMHT90 = -999;
    HLT_SingleMu = -999;   
    HLT_SingleEl = -999;   
    HLT_DoubleEl = -999;   
    HLT_DoubleEl33 = -999;   
    HLT_MuX_Ele12 = -999;   
    HLT_Mu8_EleX = -999;   
    HLT_DoubleMu = -999;   
    HLT_Photon120 = -999;   
    HLT_Photon165_HE10 = -999;   
    HLT_PFHT200_Prescale = -999;
    HLT_PFHT300_Prescale = -999;
    HLT_PFHT350_Prescale = -999;
    HLT_PFHT475_Prescale = -999;
    HLT_PFHT600_Prescale = -999;
    HLT_DiCentralPFJet70_PFMET120 = -999;
    HLT_DiCentralPFJet55_PFMET110 = -999;
    nlep = -999;
    nisoTrack = -999;
    nPFLep5LowMT = -999;
    nPFHad10LowMT = -999;
    ntau = -999;
    ngamma = -999;
    ngenPart = -999;
    ngenLep = -999;
    ngenStat23 = -999;
    ngenGamma = -999;
    ngenTau = -999;
    ngenTau1Prong = -999;
    ngenTau3Prong = -999;
    ngenLepFromTau = -999;
    njet = -999;
    gamma_mt2 = -999.0;
    gamma_nJet30 = -999;
    gamma_nJet40 = -999;
    gamma_nJet30FailId = -999;
    gamma_nJet100FailId = -999;
    gamma_nBJet20 = -999;
    gamma_nBJet25 = -999;
    gamma_nBJet30 = -999;
    gamma_nBJet40 = -999;
    gamma_ht = -999.0;
    gamma_deltaPhiMin = -999.0;
    gamma_diffMetMht = -999.0;
    gamma_mht_pt = -999.0;
    gamma_mht_phi = -999.0;
    gamma_met_pt = -999.0;
    gamma_met_phi = -999.0;
    zll_mt2 = -999.0;
    zll_deltaPhiMin = -999.0;
    zll_diffMetMht = -999.0;
    zll_met_pt = -999.0;
    zll_met_phi = -999.0;
    zll_mht_pt = -999.0;
    zll_mht_phi = -999.0;
    zll_mass = -999.0;
    zll_pt = -999.0;
    zll_eta = -999.0;
    zll_phi = -999.0;
    zll_ht = -999.0;
    zllmt_mt2 = -999.0;
    zllmt_deltaPhiMin = -999.0;
    zllmt_diffMetMht = -999.0;
    zllmt_met_pt = -999.0;
    zllmt_met_phi = -999.0;
    zllmt_mht_pt = -999.0;
    zllmt_mht_phi = -999.0;
    zllmt_ht = -999.0;
    zllmt_mt = -999.0;
    rl_mt2 = -999.0;
    rl_deltaPhiMin = -999.0;
    rl_diffMetMht = -999.0;
    rl_met_pt = -999.0;
    rl_met_phi = -999.0;
    rl_mht_pt = -999.0;
    rl_mht_phi = -999.0;
    rl_mass = -999.0;
    rl_pt = -999.0;
    rl_eta = -999.0;
    rl_phi = -999.0;
    rl_ht = -999.0;
    GenSusyMScan1 = 0;
    GenSusyMScan2 = 0;
    GenSusyMScan3 = 0;
    GenSusyMScan4 = 0;
    weight_lepsf = 1.;
    weight_lepsf_UP = 1.;
    weight_lepsf_DN = 1.;
    weight_btagsf = 1.;
    weight_btagsf_UP = 1.;
    weight_btagsf_DN = 1.;
    weight_sigtrigsf = 1.;
    weight_dileptrigsf = 1.;
    weight_phottrigsf = 1.;
    weight_pu = 1.;
    weight_isr = 1.;
    weight_scales_UP = 1.;
    weight_scales_DN = 1.;
    weight_pdfs_UP = 1.;
    weight_pdfs_DN = 1.;
    weight_toppt = 1.;
    genRecoil_pt = -999.;
    genTop_pt = -999.;
    genTbar_pt = -999.;
    nLHEweight = -999;

    for(int i=0; i < max_nlep; i++){
      lep_pt[i] = -999;
      lep_eta[i] = -999;
      lep_phi[i] = -999;
      lep_mass[i] = -999;
      lep_charge[i] = -999;
      lep_pdgId[i] = -999;
      lep_dxy[i] = -999;
      lep_dz[i] = -999;
      lep_tightId[i] = -999;
      lep_heepId[i] = -999;
      lep_relIso03[i] = -999;
      lep_relIso04[i] = -999;
      lep_miniRelIso[i] = -999;
      lep_relIsoAn04[i] = -999;
      lep_mcMatchId[i] = -999;
      lep_lostHits[i] = -999;
      lep_convVeto[i] = -999;
      lep_tightCharge[i] = -999;
    }

    for(int i=0; i < max_nisoTrack; i++){
      isoTrack_pt[i] = -999;
      isoTrack_eta[i] = -999;
      isoTrack_phi[i] = -999;
      isoTrack_mass[i] = -999;
      isoTrack_absIso[i] = -999;
      isoTrack_relIsoAn04[i] = -999;
      isoTrack_dz[i] = -999;
      isoTrack_pdgId[i] = -999;
      isoTrack_mcMatchId[i] = -999;
    }

    for(int i=0; i < max_ntau; i++){
      tau_pt[i] = -999;
      tau_eta[i] = -999;
      tau_phi[i] = -999;
      tau_mass[i] = -999;
      tau_charge[i] = -999;
      tau_dxy[i] = -999;
      tau_dz[i] = -999;
      tau_idCI3hit[i] = -999;
      tau_isoCI3hit[i] = -999;
      tau_mcMatchId[i] = -999;
    }

    for(int i=0; i < max_ngamma; i++){
      gamma_pt[i] = -999;
      gamma_eta[i] = -999;
      gamma_phi[i] = -999;
      gamma_mass[i] = -999;
      gamma_mcMatchId[i] = -999;
      gamma_genIso04[i] = -999;
      gamma_drMinParton[i] = -999;
      gamma_chHadIso[i] = -999;
      gamma_neuHadIso[i] = -999;
      gamma_phIso[i] = -999;
      gamma_sigmaIetaIeta[i] = -999;
      gamma_r9[i] = -999;
      gamma_hOverE[i] = -999;
      gamma_idCutBased[i] = -999;
    }

    if (saveGenParticles) {
      for(int i=0; i < max_ngenPart; i++){
        genPart_pt[i] = -999;
        genPart_eta[i] = -999;
        genPart_phi[i] = -999;
        genPart_mass[i] = -999;
        genPart_pdgId[i] = -999;
        genPart_status[i] = -999;
        genPart_charge[i] = -999;
        genPart_motherId[i] = -999;
        genPart_grandmotherId[i] = -999;
      }
    }

    for(int i=0; i < max_ngenLep; i++){
      genLep_pt[i] = -999;
      genLep_eta[i] = -999;
      genLep_phi[i] = -999;
      genLep_mass[i] = -999;
      genLep_pdgId[i] = -999;
      genLep_status[i] = -999;
      genLep_charge[i] = -999;
      genLep_sourceId[i] = -999;
    }

    for(int i=0; i < max_ngenStat23; i++){
      genStat23_pt[i] = -999;
      genStat23_eta[i] = -999;
      genStat23_phi[i] = -999;
      genStat23_mass[i] = -999;
      genStat23_pdgId[i] = -999;
      genStat23_status[i] = -999;
      genStat23_charge[i] = -999;
      genStat23_sourceId[i] = -999;
    }

    for(int i=0; i < max_ngenGamma; i++){
      genGamma_pt[i] = -999;
      genGamma_eta[i] = -999;
      genGamma_phi[i] = -999;
      genGamma_motherId[i] = -999;
    }

    for(int i=0; i < max_ngenTau; i++){
      genTau_pt[i] = -999;
      genTau_eta[i] = -999;
      genTau_phi[i] = -999;
      genTau_mass[i] = -999;
      genTau_pdgId[i] = -999;
      genTau_status[i] = -999;
      genTau_charge[i] = -999;
      genTau_sourceId[i] = -999;
      genTau_decayMode[i] = -999;
      genTau_leadTrackPt[i] = -999;
      genTau_neutralDaughters[i] = -999;
    }

    for(int i=0; i < max_ngenLepFromTau; i++){
      genLepFromTau_pt[i] = -999;
      genLepFromTau_eta[i] = -999;
      genLepFromTau_phi[i] = -999;
      genLepFromTau_mass[i] = -999;
      genLepFromTau_pdgId[i] = -999;
      genLepFromTau_status[i] = -999;
      genLepFromTau_charge[i] = -999;
      genLepFromTau_sourceId[i] = -999;
    }

    for(int i=0; i < max_njet; i++){
      jet_pt[i] = -999;
      jet_eta[i] = -999;
      jet_phi[i] = -999;
      jet_mass[i] = -999;
      jet_btagCSV[i] = -999;
      jet_rawPt[i] = -999;
      jet_mcPt[i] = -999;
      jet_mcFlavour[i] = -999;
      jet_qgl[i] = -999;
      jet_area[i] = -999;
      jet_id[i] = -999;
      jet_puId[i] = -999;
    }

    if (saveLHEweights || saveLHEweightsScaleOnly) {
      for(int i=0; i < max_nLHEweight; i++){
	LHEweight_wgt[i] = -999;
      }
    }
    
    return;
  }

  void babyMaker::FillBabyNtuple(){
    BabyTree_->Fill();
    return;
  }

  void babyMaker::CloseBabyNtuple(){
    BabyFile_->cd();
    BabyTree_->Write();
    count_hist_->Write();
    BabyFile_->Close();
    return;
  }

  float babyMaker::getBtagEffFromFile(float pt, float eta, int mcFlavour, bool isFastsim){
    if(!h_btag_eff_b || !h_btag_eff_c || !h_btag_eff_udsg) {
      std::cout << "babyMaker::getBtagEffFromFile: ERROR: missing input hists" << std::endl;
      return 1.;
    }

    if(isFastsim && (!h_btag_eff_b_fastsim || !h_btag_eff_c_fastsim || !h_btag_eff_udsg_fastsim)) {
      std::cout << "babyMaker::getBtagEffFromFile: ERROR: missing input hists" << std::endl;
      return 1.;
    }

    // only use pt bins up to 400 GeV for charm and udsg
    float pt_cutoff = std::max(20.,std::min(399.,double(pt)));
    TH2D* h(0);
    if (abs(mcFlavour) == 5) {
      h = isFastsim ? h_btag_eff_b_fastsim : h_btag_eff_b;
      // use pt bins up to 600 GeV for b
      pt_cutoff = std::max(20.,std::min(599.,double(pt)));
    }
    else if (abs(mcFlavour) == 4) {
      h = isFastsim ? h_btag_eff_c_fastsim : h_btag_eff_c;
    }
    else {
      h = isFastsim ? h_btag_eff_udsg_fastsim : h_btag_eff_udsg;
    }
    
    int binx = h->GetXaxis()->FindBin(pt_cutoff);
    int biny = h->GetYaxis()->FindBin(fabs(eta));
    return h->GetBinContent(binx,biny);
  }


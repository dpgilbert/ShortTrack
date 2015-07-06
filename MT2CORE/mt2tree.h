//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Aug  7 07:02:38 2014 by ROOT version 5.34/10
// from TTree treeProducerSusyFullHad/A Baby Ntuple
// found on file: sntMT2Baby.root
//////////////////////////////////////////////////////////

#ifndef mt2tree_h
#define mt2tree_h

#include "TROOT.h"
#include "TChain.h"
#include "TFile.h"

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class mt2tree {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types
   Int_t           run;
   Int_t           lumi;
   ULong64_t       evt;
   Int_t           isData;
   Float_t         evt_scale1fb;
   Float_t         evt_xsec;
   Float_t         evt_kfactor;
   Float_t         evt_filter;
   ULong64_t       evt_nEvts;
   Int_t           evt_id;
   Float_t         puWeight;
   Int_t           nVert;
   Int_t           nTrueInt;
   Float_t         rho;
   Float_t         rho25;
   Int_t           nJet40;
   Int_t           nBJet20;
   Int_t           nBJet25;
   Int_t           nBJet40;
   Int_t           nMuons10;
   Int_t           nElectrons10;
   Int_t           nLepLowMT;
   Int_t           nTaus20;
   Int_t           nGammas20;
   Float_t         deltaPhiMin;
   Float_t         diffMetMht;
   Float_t         minMTBMet;
   Float_t         ht;
   Float_t         mt2;
   Float_t         mt2_gen;
   Float_t         jet1_pt;
   Float_t         jet2_pt;
   Float_t         gamma_jet1_pt;
   Float_t         gamma_jet2_pt;
   Float_t         pseudoJet1_pt;
   Float_t         pseudoJet1_eta;
   Float_t         pseudoJet1_phi;
   Float_t         pseudoJet1_mass;
   Float_t         pseudoJet2_pt;
   Float_t         pseudoJet2_eta;
   Float_t         pseudoJet2_phi;
   Float_t         pseudoJet2_mass;
   Float_t         mht_pt;
   Float_t         mht_phi;
   Float_t         met_pt;
   Float_t         met_phi;
   Float_t         met_rawPt;
   Float_t         met_rawPhi;
   Float_t         met_caloPt;
   Float_t         met_caloPhi;
   Float_t         met_genPt;
   Float_t         met_genPhi;
   Int_t           Flag_EcalDeadCellTriggerPrimitiveFilter;
   Int_t           Flag_trkPOG_manystripclus53X;
   Int_t           Flag_ecalLaserCorrFilter;
   Int_t           Flag_trkPOG_toomanystripclus53X;
   Int_t           Flag_hcalLaserEventFilter;
   Int_t           Flag_trkPOG_logErrorTooManyClusters;
   Int_t           Flag_trkPOGFilters;
   Int_t           Flag_trackingFailureFilter;
   Int_t           Flag_CSCTightHaloFilter;
   Int_t           Flag_HBHENoiseFilter;
   Int_t           Flag_goodVertices;
   Int_t           Flag_eeBadScFilter;
   Int_t           Flag_METFilters;
   Int_t           HLT_HT900;
   Int_t           HLT_MET170;
   Int_t           HLT_ht350met120;
   Int_t           HLT_SingleMu;   
   Int_t           HLT_DoubleEl;   
   Int_t           HLT_MuEG;   
   Int_t           HLT_DoubleMu;   
   Int_t           HLT_Photons;   
   Int_t           nlep;
   Float_t         lep_pt[50];   //[nlep]
   Float_t         lep_eta[50];   //[nlep]
   Float_t         lep_phi[50];   //[nlep]
   Float_t         lep_mass[50];   //[nlep]
   Int_t           lep_charge[50];   //[nlep]
   Int_t           lep_pdgId[50];   //[nlep]
   Float_t         lep_dxy[50];   //[nlep]
   Float_t         lep_dz[50];   //[nlep]
   Int_t           lep_tightId[50];   //[nlep]
   Float_t         lep_relIso03[50];   //[nlep]
   Float_t         lep_relIso04[50];   //[nlep]
   Float_t         lep_miniRelIso[50];   //[nlep]
   Int_t           lep_mcMatchId[50];   //[nlep]
   Int_t           lep_lostHits[50];   //[nlep]
   Int_t           lep_convVeto[50];   //[nlep]
   Int_t           lep_tightCharge[50];   //[nlep]
   Int_t           nisoTrack;
   Float_t         isoTrack_pt[50];   //[nisoTrack]
   Float_t         isoTrack_eta[50];   //[nisoTrack]
   Float_t         isoTrack_phi[50];   //[nisoTrack]
   Float_t         isoTrack_mass[50];   //[nisoTrack]
   Float_t         isoTrack_absIso[50];   //[nisoTrack]
   Float_t         isoTrack_dz[50];   //[nisoTrack]
   Int_t           isoTrack_pdgId[50];   //[nisoTrack]
   Int_t           isoTrack_mcMatchId[50];   //[nisoTrack]
   Int_t           nPFLep5LowMT;
   Int_t           nPFHad10LowMT;
   Int_t           ntau;
   Float_t         tau_pt[50];   //[ntau]
   Float_t         tau_eta[50];   //[ntau]
   Float_t         tau_phi[50];   //[ntau]
   Float_t         tau_mass[50];   //[ntau]
   Int_t           tau_charge[50];   //[ntau]
   Float_t         tau_dxy[50];   //[ntau]
   Float_t         tau_dz[50];   //[ntau]
   Float_t         tau_isoMVA2[50];   //[ntau]
   Int_t           tau_idCI3hit[50];   //[ntau]
   Float_t         tau_isoCI3hit[50];   //[ntau]
   Int_t           tau_idMVA2[50];   //[ntau]
   Int_t           tau_mcMatchId[50];   //[ntau]
   Int_t           ngamma;
   Float_t         gamma_pt[50];   //[ngamma]
   Float_t         gamma_eta[50];   //[ngamma]
   Float_t         gamma_phi[50];   //[ngamma]
   Float_t         gamma_mass[50];   //[ngamma]
   Int_t           gamma_mcMatchId[50];   //[ngamma]
   Float_t         gamma_genIso[50];   //[ngamma]
   Float_t         gamma_genIso04[50];   //[ngamma]
   Float_t         gamma_drMinParton[50];   //[ngamma]
   Float_t         gamma_chHadIso[50];   //[ngamma]
   Float_t         gamma_neuHadIso[50];   //[ngamma]
   Float_t         gamma_phIso[50];   //[ngamma]
   Float_t         gamma_sigmaIetaIeta[50];   //[ngamma]
   Float_t         gamma_r9[50];   //[ngamma]
   Float_t         gamma_hOverE[50];   //[ngamma]
   Int_t           gamma_idCutBased[50];   //[ngamma]
   Float_t         gamma_mt2;
   Int_t           gamma_nJet40;
   Int_t           gamma_nBJet20;
   Int_t           gamma_nBJet25;
   Int_t           gamma_nBJet40;
   Float_t         gamma_ht;
   Float_t         gamma_minMTBMet;
   Float_t         gamma_met_pt;
   Float_t         gamma_met_phi;
   Float_t         gamma_mht_pt;
   Float_t         gamma_mht_phi;
   Float_t         gamma_deltaPhiMin;
   Float_t         gamma_diffMetMht;
   Float_t         zll_mt2;
   Float_t         zll_met_pt;
   Float_t         zll_met_phi;
   Float_t         zll_mht_pt;
   Float_t         zll_mht_phi;
   Float_t         zll_deltaPhiMin;
   Float_t         zll_diffMetMht;
   Float_t         zll_mass;
   Float_t         zll_pt;
   Float_t         zll_eta;
   Float_t         zll_phi;
   Float_t         zll_ht;
   Float_t         zll_minMTBMet;
   Float_t         zllmt_mt2;
   Float_t         zllmt_deltaPhiMin;
   Float_t         zllmt_diffMetMht;
   Float_t         zllmt_met_pt;
   Float_t         zllmt_met_phi;
   Float_t         zllmt_mht_pt;
   Float_t         zllmt_mht_phi;
   Float_t         zllmt_ht;
   Float_t         zllmt_mt;
   Float_t         rl_mt2;
   Float_t         rl_met_pt;
   Float_t         rl_met_phi;
   Float_t         rl_mht_pt;
   Float_t         rl_mht_phi;
   Float_t         rl_deltaPhiMin;
   Float_t         rl_diffMetMht;
   Float_t         rl_mass;
   Float_t         rl_pt;
   Float_t         rl_eta;
   Float_t         rl_phi;
   Float_t         rl_ht;
   Float_t         rl_minMTBMet;
   Int_t           ngenPart;
   Float_t         genPart_pt[200];   //[ngenPart]
   Float_t         genPart_eta[200];   //[ngenPart]
   Float_t         genPart_phi[200];   //[ngenPart]
   Float_t         genPart_mass[200];   //[ngenPart]
   Int_t           genPart_pdgId[200];   //[ngenPart]
   Int_t           genPart_status[200];   //[ngenPart]
   Float_t         genPart_charge[200];   //[ngenPart]
   Int_t           genPart_motherId[200];   //[ngenPart]
   Int_t           genPart_grandmaId[200];   //[ngenPart]
   Int_t           ngenLep;
   Float_t         genLep_pt[10];   //[ngenLep]
   Float_t         genLep_eta[10];   //[ngenLep]
   Float_t         genLep_phi[10];   //[ngenLep]
   Float_t         genLep_mass[10];   //[ngenLep]
   Int_t           genLep_pdgId[10];   //[ngenLep]
   Int_t           genLep_status[10];   //[ngenLep]
   Float_t         genLep_charge[10];   //[ngenLep]
   Int_t           genLep_sourceId[10];   //[ngenLep]
   Int_t           ngenStat23;
   Float_t         genStat23_pt[10];   //[ngenStat23]
   Float_t         genStat23_eta[10];   //[ngenStat23]
   Float_t         genStat23_phi[10];   //[ngenStat23]
   Float_t         genStat23_mass[10];   //[ngenStat23]
   Int_t           genStat23_pdgId[10];   //[ngenStat23]
   Int_t           genStat23_status[10];   //[ngenStat23]
   Float_t         genStat23_charge[10];   //[ngenStat23]
   Int_t           genStat23_sourceId[10];   //[ngenStat23]
   Int_t           ngenGamma;
   Float_t         genGamma_pt[10];   //[ngenGamma]
   Float_t         genGamma_eta[10];   //[ngenGamma]
   Float_t         genGamma_phi[10];   //[ngenGamma]
   Int_t           genGamma_motherId[10];   //[ngenGamma]
   Int_t           ngenLepFromTau;
   Float_t         genLepFromTau_pt[10];   //[ngenLepFromTau]
   Float_t         genLepFromTau_eta[10];   //[ngenLepFromTau]
   Float_t         genLepFromTau_phi[10];   //[ngenLepFromTau]
   Float_t         genLepFromTau_mass[10];   //[ngenLepFromTau]
   Int_t           genLepFromTau_pdgId[10];   //[ngenLepFromTau]
   Int_t           genLepFromTau_status[10];   //[ngenLepFromTau]
   Float_t         genLepFromTau_charge[10];   //[ngenLepFromTau]
   Int_t           genLepFromTau_sourceId[10];   //[ngenLepFromTau]
   Int_t           ngenTau;
   Float_t         genTau_pt[10];   //[ngenTau]
   Float_t         genTau_eta[10];   //[ngenTau]
   Float_t         genTau_phi[10];   //[ngenTau]
   Float_t         genTau_mass[10];   //[ngenTau]
   Int_t           genTau_pdgId[10];   //[ngenTau]
   Int_t           genTau_status[10];   //[ngenTau]
   Float_t         genTau_charge[10];   //[ngenTau]
   Int_t           genTau_sourceId[10];   //[ngenTau]
   Int_t           njet;
   Float_t         jet_pt[100];   //[njet]
   Float_t         jet_eta[100];   //[njet]
   Float_t         jet_phi[100];   //[njet]
   Float_t         jet_mass[100];   //[njet]
   Float_t         jet_btagCSV[100];   //[njet]
   Float_t         jet_rawPt[100];   //[njet]
   Float_t         jet_mcPt[100];   //[njet]
   Int_t           jet_mcFlavour[100];   //[njet]
   Float_t         jet_qgl[100];   //[njet]
   Float_t         jet_area[100];   //[njet]
   Int_t           jet_id[100];   //[njet]
   Int_t           jet_puId[100];   //[njet]
   Int_t           GenSusyMScan1;
   Int_t           GenSusyMScan2;
   Int_t           GenSusyMScan3;
   Int_t           GenSusyMScan4;
   Float_t         weight_lepsf;
   Float_t         weight_lepsf_UP;
   Float_t         weight_lepsf_DN;
   Float_t         weight_btagsf;
   Float_t         weight_btagsf_UP;
   Float_t         weight_btagsf_DN;
   Float_t         weight_sigtrigsf;
   Float_t         weight_dileptrigsf;
   Float_t         weight_phottrigsf;
   Float_t         weight_pu;
   Float_t         weight_isr;
   Float_t         weight_scales_UP;
   Float_t         weight_scales_DN;
   Float_t         weight_pdfs_UP;
   Float_t         weight_pdfs_DN;

   // List of branches
   TBranch        *b_run;   //!
   TBranch        *b_lumi;   //!
   TBranch        *b_evt;   //!
   TBranch        *b_isData;   //!
   TBranch        *b_evt_scale1fb;   //!
   TBranch        *b_evt_xsec;   //!
   TBranch        *b_evt_kfactor;   //!
   TBranch        *b_evt_filter;   //!
   TBranch        *b_evt_nEvts;   //!
   TBranch        *b_evt_id;   //!
   TBranch        *b_puWeight;   //!
   TBranch        *b_nVert;   //!
   TBranch        *b_nTrueInt;   //!
   TBranch        *b_rho;   //!
   TBranch        *b_rho25;   //!
   TBranch        *b_nJet40;   //!
   TBranch        *b_nBJet20;   //!
   TBranch        *b_nBJet25;   //!
   TBranch        *b_nBJet40;   //!
   TBranch        *b_nMuons10;   //!
   TBranch        *b_nElectrons10;   //!
   TBranch        *b_nLepLowMT;   //!
   TBranch        *b_nTaus20;   //!
   TBranch        *b_nGammas20;   //!
   TBranch        *b_deltaPhiMin;   //!
   TBranch        *b_diffMetMht;   //!
   TBranch        *b_minMTBMet;   //!
   TBranch        *b_ht;   //!
   TBranch        *b_mt2;   //!
   TBranch        *b_mt2_gen;   //!
   TBranch        *b_jet1_pt;   //!
   TBranch        *b_jet2_pt;   //!
   TBranch        *b_gamma_jet1_pt;   //!
   TBranch        *b_gamma_jet2_pt;   //!
   TBranch        *b_pseudoJet1_pt;   //!
   TBranch        *b_pseudoJet1_eta;   //!
   TBranch        *b_pseudoJet1_phi;   //!
   TBranch        *b_pseudoJet1_mass;   //!
   TBranch        *b_pseudoJet2_pt;   //!
   TBranch        *b_pseudoJet2_eta;   //!
   TBranch        *b_pseudoJet2_phi;   //!
   TBranch        *b_pseudoJet2_mass;   //!
   TBranch        *b_mht_pt;   //!
   TBranch        *b_mht_phi;   //!
   TBranch        *b_met_pt;   //!
   TBranch        *b_met_phi;   //!
   TBranch        *b_met_rawPt;   //!
   TBranch        *b_met_rawPhi;   //!
   TBranch        *b_met_caloPt;   //!
   TBranch        *b_met_caloPhi;   //!
   TBranch        *b_met_genPt;   //!
   TBranch        *b_met_genPhi;   //!
   TBranch        *b_Flag_EcalDeadCellTriggerPrimitiveFilter;   //!
   TBranch        *b_Flag_trkPOG_manystripclus53X;   //!
   TBranch        *b_Flag_ecalLaserCorrFilter;   //!
   TBranch        *b_Flag_trkPOG_toomanystripclus53X;   //!
   TBranch        *b_Flag_hcalLaserEventFilter;   //!
   TBranch        *b_Flag_trkPOG_logErrorTooManyClusters;   //!
   TBranch        *b_Flag_trkPOGFilters;   //!
   TBranch        *b_Flag_trackingFailureFilter;   //!
   TBranch        *b_Flag_CSCTightHaloFilter;   //!
   TBranch        *b_Flag_HBHENoiseFilter;   //!
   TBranch        *b_Flag_goodVertices;   //!
   TBranch        *b_Flag_eeBadScFilter;   //!
   TBranch        *b_Flag_METFilters;   //!
   TBranch        *b_HLT_HT900;   //!
   TBranch        *b_HLT_MET170;   //!
   TBranch        *b_HLT_ht350met120;   //!
   TBranch        *b_HLT_SingleMu;   //!
   TBranch        *b_HLT_DoubleEl;   //!
   TBranch        *b_HLT_MuEG;   //!
   TBranch        *b_HLT_DoubleMu;   //!
   TBranch        *b_HLT_Photons;   //!
   TBranch        *b_nlep;   //!
   TBranch        *b_lep_pt;   //!
   TBranch        *b_lep_eta;   //!
   TBranch        *b_lep_phi;   //!
   TBranch        *b_lep_mass;   //!
   TBranch        *b_lep_charge;   //!
   TBranch        *b_lep_pdgId;   //!
   TBranch        *b_lep_dxy;   //!
   TBranch        *b_lep_dz;   //!
   TBranch        *b_lep_tightId;   //!
   TBranch        *b_lep_relIso03;   //!
   TBranch        *b_lep_relIso04;   //!
   TBranch        *b_lep_miniRelIso;   //!
   TBranch        *b_lep_mcMatchId;   //!
   TBranch        *b_lep_lostHits;   //!
   TBranch        *b_lep_convVeto;   //!
   TBranch        *b_lep_tightCharge;   //!
   TBranch        *b_nisoTrack;   //!
   TBranch        *b_isoTrack_pt;   //!
   TBranch        *b_isoTrack_eta;   //!
   TBranch        *b_isoTrack_phi;   //!
   TBranch        *b_isoTrack_mass;   //!
   TBranch        *b_isoTrack_absIso;   //!
   TBranch        *b_isoTrack_dz;   //!
   TBranch        *b_isoTrack_pdgId;   //!
   TBranch        *b_isoTrack_mcMatchId;   //!
   TBranch        *b_nPFLep5LowMT;   //!
   TBranch        *b_nPFHad10LowMT;   //!
   TBranch        *b_ntau;   //!
   TBranch        *b_tau_pt;   //!
   TBranch        *b_tau_eta;   //!
   TBranch        *b_tau_phi;   //!
   TBranch        *b_tau_mass;   //!
   TBranch        *b_tau_charge;   //!
   TBranch        *b_tau_dxy;   //!
   TBranch        *b_tau_dz;   //!
   TBranch        *b_tau_isoMVA2;   //!
   TBranch        *b_tau_idCI3hit;   //!
   TBranch        *b_tau_isoCI3hit;   //!
   TBranch        *b_tau_idMVA2;   //!
   TBranch        *b_tau_mcMatchId;   //!
   TBranch        *b_ngamma;   //!
   TBranch        *b_gamma_pt;   //!
   TBranch        *b_gamma_eta;   //!
   TBranch        *b_gamma_phi;   //!
   TBranch        *b_gamma_mass;   //!
   TBranch        *b_gamma_mcMatchId;   //!
   TBranch        *b_gamma_genIso;   //!
   TBranch        *b_gamma_genIso04;   //!
   TBranch        *b_gamma_drMinParton;   //!
   TBranch        *b_gamma_chHadIso;   //!
   TBranch        *b_gamma_neuHadIso;   //!
   TBranch        *b_gamma_phIso;   //!
   TBranch        *b_gamma_sigmaIetaIeta;   //!
   TBranch        *b_gamma_r9;   //!
   TBranch        *b_gamma_hOverE;   //!
   TBranch        *b_gamma_idCutBased;   //!
   TBranch        *b_gamma_mt2;
   TBranch        *b_gamma_nJet40;
   TBranch        *b_gamma_nBJet20;
   TBranch        *b_gamma_nBJet25;
   TBranch        *b_gamma_nBJet40;
   TBranch        *b_gamma_ht;
   TBranch        *b_gamma_minMTBMet;
   TBranch        *b_gamma_met_pt;
   TBranch        *b_gamma_met_phi;
   TBranch        *b_gamma_mht_pt;
   TBranch        *b_gamma_mht_phi;
   TBranch        *b_gamma_deltaPhiMin;
   TBranch        *b_gamma_diffMetMht;
   TBranch        *b_zll_mt2;
   TBranch        *b_zll_met_pt;
   TBranch        *b_zll_met_phi;
   TBranch        *b_zll_mht_pt;
   TBranch        *b_zll_mht_phi;
   TBranch        *b_zll_deltaPhiMin;
   TBranch        *b_zll_diffMetMht;
   TBranch        *b_zll_mass;
   TBranch        *b_zll_pt;
   TBranch        *b_zll_eta;
   TBranch        *b_zll_phi;
   TBranch        *b_zll_ht;
   TBranch        *b_zll_minMTBMet;
   TBranch        *b_zllmt_mt2;
   TBranch        *b_zllmt_deltaPhiMin;
   TBranch        *b_zllmt_diffMetMht;
   TBranch        *b_zllmt_met_pt;
   TBranch        *b_zllmt_met_phi;
   TBranch        *b_zllmt_mht_pt;
   TBranch        *b_zllmt_mht_phi;
   TBranch        *b_zllmt_ht;
   TBranch        *b_zllmt_mt;
   TBranch        *b_rl_mt2;
   TBranch        *b_rl_met_pt;
   TBranch        *b_rl_met_phi;
   TBranch        *b_rl_mht_pt;
   TBranch        *b_rl_mht_phi;
   TBranch        *b_rl_deltaPhiMin;
   TBranch        *b_rl_diffMetMht;
   TBranch        *b_rl_mass;
   TBranch        *b_rl_pt;
   TBranch        *b_rl_eta;
   TBranch        *b_rl_phi;
   TBranch        *b_rl_ht;
   TBranch        *b_rl_minMTBMet;
   TBranch        *b_ngenPart;   //!
   TBranch        *b_genPart_pt;   //!
   TBranch        *b_genPart_eta;   //!
   TBranch        *b_genPart_phi;   //!
   TBranch        *b_genPart_mass;   //!
   TBranch        *b_genPart_pdgId;   //!
   TBranch        *b_genPart_status;   //!
   TBranch        *b_genPart_charge;   //!
   TBranch        *b_genPart_motherId;   //!
   TBranch        *b_genPart_grandmaId;   //!
   TBranch        *b_ngenLep;   //!
   TBranch        *b_genLep_pt;   //!
   TBranch        *b_genLep_eta;   //!
   TBranch        *b_genLep_phi;   //!
   TBranch        *b_genLep_mass;   //!
   TBranch        *b_genLep_pdgId;   //!
   TBranch        *b_genLep_status;   //!
   TBranch        *b_genLep_charge;   //!
   TBranch        *b_genLep_sourceId;   //!
   TBranch        *b_ngenStat23;   //!
   TBranch        *b_genStat23_pt;   //!
   TBranch        *b_genStat23_eta;   //!
   TBranch        *b_genStat23_phi;   //!
   TBranch        *b_genStat23_mass;   //!
   TBranch        *b_genStat23_pdgId;   //!
   TBranch        *b_genStat23_status;   //!
   TBranch        *b_genStat23_charge;   //!
   TBranch        *b_genStat23_sourceId;   //!
   TBranch        *b_ngenGamma;   //!
   TBranch        *b_genGamma_pt;   //!
   TBranch        *b_genGamma_eta;   //!
   TBranch        *b_genGamma_phi;   //!
   TBranch        *b_genGamma_motherId;   //!
   TBranch        *b_ngenLepFromTau;   //!
   TBranch        *b_genLepFromTau_pt;   //!
   TBranch        *b_genLepFromTau_eta;   //!
   TBranch        *b_genLepFromTau_phi;   //!
   TBranch        *b_genLepFromTau_mass;   //!
   TBranch        *b_genLepFromTau_pdgId;   //!
   TBranch        *b_genLepFromTau_status;   //!
   TBranch        *b_genLepFromTau_charge;   //!
   TBranch        *b_genLepFromTau_sourceId;   //!
   TBranch        *b_ngenTau;   //!
   TBranch        *b_genTau_pt;   //!
   TBranch        *b_genTau_eta;   //!
   TBranch        *b_genTau_phi;   //!
   TBranch        *b_genTau_mass;   //!
   TBranch        *b_genTau_pdgId;   //!
   TBranch        *b_genTau_status;   //!
   TBranch        *b_genTau_charge;   //!
   TBranch        *b_genTau_sourceId;   //!
   TBranch        *b_njet;   //!
   TBranch        *b_jet_pt;   //!
   TBranch        *b_jet_eta;   //!
   TBranch        *b_jet_phi;   //!
   TBranch        *b_jet_mass;   //!
   TBranch        *b_jet_btagCSV;   //!
   TBranch        *b_jet_rawPt;   //!
   TBranch        *b_jet_mcPt;   //!
   TBranch        *b_jet_mcFlavour;   //!
   TBranch        *b_jet_qgl;   //!
   TBranch        *b_jet_area;   //!
   TBranch        *b_jet_id;   //!
   TBranch        *b_jet_puId;   //!
   TBranch        *b_GenSusyMScan1;   //!
   TBranch        *b_GenSusyMScan2;   //!
   TBranch        *b_GenSusyMScan3;   //!
   TBranch        *b_GenSusyMScan4;   //!
   TBranch        *b_weight_lepsf;   //!
   TBranch        *b_weight_lepsf_UP;   //!
   TBranch        *b_weight_lepsf_DN;   //!
   TBranch        *b_weight_btagsf;   //!
   TBranch        *b_weight_btagsf_UP;   //!
   TBranch        *b_weight_btagsf_DN;   //!
   TBranch        *b_weight_sigtrigsf;   //!
   TBranch        *b_weight_dileptrigsf;   //!
   TBranch        *b_weight_phottrigsf;   //!
   TBranch        *b_weight_pu;   //!
   TBranch        *b_weight_isr;   //!
   TBranch        *b_weight_scales_UP;   //!
   TBranch        *b_weight_scales_DN;   //!
   TBranch        *b_weight_pdfs_UP;   //!
   TBranch        *b_weight_pdfs_DN;   //!

   mt2tree(TTree *tree=0);
   virtual ~mt2tree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef mt2tree_cxx
mt2tree::mt2tree(TTree *tree) : fChain(0) 
{
   Init(tree);
}

mt2tree::~mt2tree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t mt2tree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t mt2tree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void mt2tree::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("run", &run, &b_run);
   fChain->SetBranchAddress("lumi", &lumi, &b_lumi);
   fChain->SetBranchAddress("evt", &evt, &b_evt);
   fChain->SetBranchAddress("isData", &isData, &b_isData);
   fChain->SetBranchAddress("evt_scale1fb", &evt_scale1fb, &b_evt_scale1fb);
   fChain->SetBranchAddress("evt_xsec", &evt_xsec, &b_evt_xsec);
   fChain->SetBranchAddress("evt_kfactor", &evt_kfactor, &b_evt_kfactor);
   fChain->SetBranchAddress("evt_filter", &evt_filter, &b_evt_filter);
   fChain->SetBranchAddress("evt_nEvts", &evt_nEvts, &b_evt_nEvts);
   fChain->SetBranchAddress("evt_id", &evt_id, &b_evt_id);
   fChain->SetBranchAddress("puWeight", &puWeight, &b_puWeight);
   fChain->SetBranchAddress("nVert", &nVert, &b_nVert);
   fChain->SetBranchAddress("nTrueInt", &nTrueInt, &b_nTrueInt);
   fChain->SetBranchAddress("rho", &rho, &b_rho);
   fChain->SetBranchAddress("rho25", &rho25, &b_rho25);
   fChain->SetBranchAddress("nJet40", &nJet40, &b_nJet40);
   fChain->SetBranchAddress("nBJet20", &nBJet20, &b_nBJet20);
   fChain->SetBranchAddress("nBJet25", &nBJet25, &b_nBJet25);
   fChain->SetBranchAddress("nBJet40", &nBJet40, &b_nBJet40);
   fChain->SetBranchAddress("nMuons10", &nMuons10, &b_nMuons10);
   fChain->SetBranchAddress("nElectrons10", &nElectrons10, &b_nElectrons10);
   fChain->SetBranchAddress("nLepLowMT", &nLepLowMT, &b_nLepLowMT);
   fChain->SetBranchAddress("nTaus20", &nTaus20, &b_nTaus20);
   fChain->SetBranchAddress("nGammas20", &nGammas20, &b_nGammas20);
   fChain->SetBranchAddress("deltaPhiMin", &deltaPhiMin, &b_deltaPhiMin);
   fChain->SetBranchAddress("diffMetMht", &diffMetMht, &b_diffMetMht);
   fChain->SetBranchAddress("minMTBMet", &minMTBMet, &b_minMTBMet);
   fChain->SetBranchAddress("ht", &ht, &b_ht);
   fChain->SetBranchAddress("mt2", &mt2, &b_mt2);
   fChain->SetBranchAddress("mt2_gen", &mt2_gen, &b_mt2_gen);
   fChain->SetBranchAddress("jet1_pt", &jet1_pt, &b_jet1_pt);
   fChain->SetBranchAddress("jet2_pt", &jet2_pt, &b_jet2_pt);
   fChain->SetBranchAddress("gamma_jet1_pt", &gamma_jet1_pt, &b_gamma_jet1_pt);
   fChain->SetBranchAddress("gamma_jet2_pt", &gamma_jet2_pt, &b_gamma_jet2_pt);
   fChain->SetBranchAddress("pseudoJet1_pt", &pseudoJet1_pt, &b_pseudoJet1_pt);
   fChain->SetBranchAddress("pseudoJet1_eta", &pseudoJet1_eta, &b_pseudoJet1_eta);
   fChain->SetBranchAddress("pseudoJet1_phi", &pseudoJet1_phi, &b_pseudoJet1_phi);
   fChain->SetBranchAddress("pseudoJet1_mass", &pseudoJet1_mass, &b_pseudoJet1_mass);
   fChain->SetBranchAddress("pseudoJet2_pt", &pseudoJet2_pt, &b_pseudoJet2_pt);
   fChain->SetBranchAddress("pseudoJet2_eta", &pseudoJet2_eta, &b_pseudoJet2_eta);
   fChain->SetBranchAddress("pseudoJet2_phi", &pseudoJet2_phi, &b_pseudoJet2_phi);
   fChain->SetBranchAddress("pseudoJet2_mass", &pseudoJet2_mass, &b_pseudoJet2_mass);
   fChain->SetBranchAddress("mht_pt", &mht_pt, &b_mht_pt);
   fChain->SetBranchAddress("mht_phi", &mht_phi, &b_mht_phi);
   fChain->SetBranchAddress("met_pt", &met_pt, &b_met_pt);
   fChain->SetBranchAddress("met_phi", &met_phi, &b_met_phi);
   fChain->SetBranchAddress("met_rawPt", &met_rawPt, &b_met_rawPt);
   fChain->SetBranchAddress("met_rawPhi", &met_rawPhi, &b_met_rawPhi);
   fChain->SetBranchAddress("met_caloPt", &met_caloPt, &b_met_caloPt);
   fChain->SetBranchAddress("met_caloPhi", &met_caloPhi, &b_met_caloPhi);
   fChain->SetBranchAddress("met_genPt", &met_genPt, &b_met_genPt);
   fChain->SetBranchAddress("met_genPhi", &met_genPhi, &b_met_genPhi);
   fChain->SetBranchAddress("Flag_EcalDeadCellTriggerPrimitiveFilter", &Flag_EcalDeadCellTriggerPrimitiveFilter, &b_Flag_EcalDeadCellTriggerPrimitiveFilter);
   fChain->SetBranchAddress("Flag_trkPOG_manystripclus53X", &Flag_trkPOG_manystripclus53X, &b_Flag_trkPOG_manystripclus53X);
   fChain->SetBranchAddress("Flag_ecalLaserCorrFilter", &Flag_ecalLaserCorrFilter, &b_Flag_ecalLaserCorrFilter);
   fChain->SetBranchAddress("Flag_trkPOG_toomanystripclus53X", &Flag_trkPOG_toomanystripclus53X, &b_Flag_trkPOG_toomanystripclus53X);
   fChain->SetBranchAddress("Flag_hcalLaserEventFilter", &Flag_hcalLaserEventFilter, &b_Flag_hcalLaserEventFilter);
   fChain->SetBranchAddress("Flag_trkPOG_logErrorTooManyClusters", &Flag_trkPOG_logErrorTooManyClusters, &b_Flag_trkPOG_logErrorTooManyClusters);
   fChain->SetBranchAddress("Flag_trkPOGFilters", &Flag_trkPOGFilters, &b_Flag_trkPOGFilters);
   fChain->SetBranchAddress("Flag_trackingFailureFilter", &Flag_trackingFailureFilter, &b_Flag_trackingFailureFilter);
   fChain->SetBranchAddress("Flag_CSCTightHaloFilter", &Flag_CSCTightHaloFilter, &b_Flag_CSCTightHaloFilter);
   fChain->SetBranchAddress("Flag_HBHENoiseFilter", &Flag_HBHENoiseFilter, &b_Flag_HBHENoiseFilter);
   fChain->SetBranchAddress("Flag_goodVertices", &Flag_goodVertices, &b_Flag_goodVertices);
   fChain->SetBranchAddress("Flag_eeBadScFilter", &Flag_eeBadScFilter, &b_Flag_eeBadScFilter);
   fChain->SetBranchAddress("Flag_METFilters", &Flag_METFilters, &b_Flag_METFilters);
   fChain->SetBranchAddress("HLT_HT900", &HLT_HT900, &b_HLT_HT900);
   fChain->SetBranchAddress("HLT_MET170", &HLT_MET170, &b_HLT_MET170);
   fChain->SetBranchAddress("HLT_ht350met120", &HLT_ht350met120, &b_HLT_ht350met120);
   fChain->SetBranchAddress("HLT_SingleMu", &HLT_SingleMu, &b_HLT_SingleMu);
   fChain->SetBranchAddress("HLT_DoubleEl", &HLT_DoubleEl, &b_HLT_DoubleEl);
   fChain->SetBranchAddress("HLT_MuEG", &HLT_MuEG, &b_HLT_MuEG);
   fChain->SetBranchAddress("HLT_DoubleMu", &HLT_DoubleMu, &b_HLT_DoubleMu);
   fChain->SetBranchAddress("HLT_Photons", &HLT_Photons, &b_HLT_Photons);
   fChain->SetBranchAddress("nlep", &nlep, &b_nlep);
   fChain->SetBranchAddress("lep_pt", lep_pt, &b_lep_pt);
   fChain->SetBranchAddress("lep_eta", lep_eta, &b_lep_eta);
   fChain->SetBranchAddress("lep_phi", lep_phi, &b_lep_phi);
   fChain->SetBranchAddress("lep_mass", lep_mass, &b_lep_mass);
   fChain->SetBranchAddress("lep_charge", lep_charge, &b_lep_charge);
   fChain->SetBranchAddress("lep_pdgId", lep_pdgId, &b_lep_pdgId);
   fChain->SetBranchAddress("lep_dxy", lep_dxy, &b_lep_dxy);
   fChain->SetBranchAddress("lep_dz", lep_dz, &b_lep_dz);
   fChain->SetBranchAddress("lep_tightId", lep_tightId, &b_lep_tightId);
   fChain->SetBranchAddress("lep_relIso03", lep_relIso03, &b_lep_relIso03);
   fChain->SetBranchAddress("lep_relIso04", lep_relIso04, &b_lep_relIso04);
   fChain->SetBranchAddress("lep_miniRelIso", lep_miniRelIso, &b_lep_miniRelIso);
   fChain->SetBranchAddress("lep_mcMatchId", lep_mcMatchId, &b_lep_mcMatchId);
   fChain->SetBranchAddress("lep_lostHits", lep_lostHits, &b_lep_lostHits);
   fChain->SetBranchAddress("lep_convVeto", lep_convVeto, &b_lep_convVeto);
   fChain->SetBranchAddress("lep_tightCharge", lep_tightCharge, &b_lep_tightCharge);
   fChain->SetBranchAddress("nisoTrack", &nisoTrack, &b_nisoTrack);
   fChain->SetBranchAddress("isoTrack_pt", isoTrack_pt, &b_isoTrack_pt);
   fChain->SetBranchAddress("isoTrack_eta", isoTrack_eta, &b_isoTrack_eta);
   fChain->SetBranchAddress("isoTrack_phi", isoTrack_phi, &b_isoTrack_phi);
   fChain->SetBranchAddress("isoTrack_mass", isoTrack_mass, &b_isoTrack_mass);
   fChain->SetBranchAddress("isoTrack_absIso", isoTrack_absIso, &b_isoTrack_absIso);
   fChain->SetBranchAddress("isoTrack_dz", isoTrack_dz, &b_isoTrack_dz);
   fChain->SetBranchAddress("isoTrack_pdgId", isoTrack_pdgId, &b_isoTrack_pdgId);
   fChain->SetBranchAddress("isoTrack_mcMatchId", isoTrack_mcMatchId, &b_isoTrack_mcMatchId);
   fChain->SetBranchAddress("nPFLep5LowMT", &nPFLep5LowMT, &b_nPFLep5LowMT);
   fChain->SetBranchAddress("nPFHad10LowMT", &nPFHad10LowMT, &b_nPFHad10LowMT);
   fChain->SetBranchAddress("ntau", &ntau, &b_ntau);
   fChain->SetBranchAddress("tau_pt", tau_pt, &b_tau_pt);
   fChain->SetBranchAddress("tau_eta", tau_eta, &b_tau_eta);
   fChain->SetBranchAddress("tau_phi", tau_phi, &b_tau_phi);
   fChain->SetBranchAddress("tau_mass", tau_mass, &b_tau_mass);
   fChain->SetBranchAddress("tau_charge", tau_charge, &b_tau_charge);
   fChain->SetBranchAddress("tau_dxy", tau_dxy, &b_tau_dxy);
   fChain->SetBranchAddress("tau_dz", tau_dz, &b_tau_dz);
   fChain->SetBranchAddress("tau_isoMVA2", tau_isoMVA2, &b_tau_isoMVA2);
   fChain->SetBranchAddress("tau_idCI3hit", tau_idCI3hit, &b_tau_idCI3hit);
   fChain->SetBranchAddress("tau_isoCI3hit", tau_isoCI3hit, &b_tau_isoCI3hit);
   fChain->SetBranchAddress("tau_idMVA2", tau_idMVA2, &b_tau_idMVA2);
   fChain->SetBranchAddress("tau_mcMatchId", tau_mcMatchId, &b_tau_mcMatchId);
   fChain->SetBranchAddress("ngamma", &ngamma, &b_ngamma);
   fChain->SetBranchAddress("gamma_pt", gamma_pt, &b_gamma_pt);
   fChain->SetBranchAddress("gamma_eta", gamma_eta, &b_gamma_eta);
   fChain->SetBranchAddress("gamma_phi", gamma_phi, &b_gamma_phi);
   fChain->SetBranchAddress("gamma_mass", gamma_mass, &b_gamma_mass);
   fChain->SetBranchAddress("gamma_mcMatchId", gamma_mcMatchId, &b_gamma_mcMatchId);
   fChain->SetBranchAddress("gamma_genIso", gamma_genIso, &b_gamma_genIso);
   fChain->SetBranchAddress("gamma_genIso04", gamma_genIso04, &b_gamma_genIso04);
   fChain->SetBranchAddress("gamma_drMinParton", gamma_drMinParton, &b_gamma_drMinParton);
   fChain->SetBranchAddress("gamma_chHadIso", gamma_chHadIso, &b_gamma_chHadIso);
   fChain->SetBranchAddress("gamma_neuHadIso", gamma_neuHadIso, &b_gamma_neuHadIso);
   fChain->SetBranchAddress("gamma_phIso", gamma_phIso, &b_gamma_phIso);
   fChain->SetBranchAddress("gamma_sigmaIetaIeta", gamma_sigmaIetaIeta, &b_gamma_sigmaIetaIeta);
   fChain->SetBranchAddress("gamma_r9", gamma_r9, &b_gamma_r9);
   fChain->SetBranchAddress("gamma_hOverE", gamma_hOverE, &b_gamma_hOverE);
   fChain->SetBranchAddress("gamma_idCutBased", gamma_idCutBased, &b_gamma_idCutBased);
   fChain->SetBranchAddress("gamma_mt2", &gamma_mt2, &b_gamma_mt2);
   fChain->SetBranchAddress("gamma_nJet40", &gamma_nJet40, &b_gamma_nJet40);
   fChain->SetBranchAddress("gamma_nBJet20", &gamma_nBJet20, &b_gamma_nBJet20);
   fChain->SetBranchAddress("gamma_nBJet25", &gamma_nBJet25, &b_gamma_nBJet25);
   fChain->SetBranchAddress("gamma_nBJet40", &gamma_nBJet40, &b_gamma_nBJet40);
   fChain->SetBranchAddress("gamma_ht", &gamma_ht, &b_gamma_ht);
   fChain->SetBranchAddress("gamma_minMTBMet", &gamma_minMTBMet, &b_gamma_minMTBMet);
   fChain->SetBranchAddress("gamma_met_pt", &gamma_met_pt, &b_gamma_met_pt);
   fChain->SetBranchAddress("gamma_met_phi", &gamma_met_phi, &b_gamma_met_phi);
   fChain->SetBranchAddress("gamma_mht_pt", &gamma_mht_pt, &b_gamma_mht_pt);
   fChain->SetBranchAddress("gamma_mht_phi", &gamma_mht_phi, &b_gamma_mht_phi);
   fChain->SetBranchAddress("gamma_deltaPhiMin", &gamma_deltaPhiMin, &b_gamma_deltaPhiMin);
   fChain->SetBranchAddress("gamma_diffMetMht", &gamma_diffMetMht, &b_gamma_diffMetMht);
   fChain->SetBranchAddress("zll_mt2", &zll_mt2, &b_zll_mt2);
   fChain->SetBranchAddress("zll_met_pt", &zll_met_pt, &b_zll_met_pt);
   fChain->SetBranchAddress("zll_met_phi", &zll_met_phi, &b_zll_met_phi);
   fChain->SetBranchAddress("zll_mht_pt", &zll_mht_pt, &b_zll_mht_pt);
   fChain->SetBranchAddress("zll_mht_phi", &zll_mht_phi, &b_zll_mht_phi);
   fChain->SetBranchAddress("zll_deltaPhiMin", &zll_deltaPhiMin, &b_zll_deltaPhiMin);
   fChain->SetBranchAddress("zll_diffMetMht", &zll_diffMetMht, &b_zll_diffMetMht);
   fChain->SetBranchAddress("zll_mass", &zll_mass, &b_zll_mass);
   fChain->SetBranchAddress("zll_pt", &zll_pt, &b_zll_pt);
   fChain->SetBranchAddress("zll_eta", &zll_eta, &b_zll_eta);
   fChain->SetBranchAddress("zll_phi", &zll_phi, &b_zll_phi);
   fChain->SetBranchAddress("zll_ht", &zll_ht, &b_zll_ht);
   fChain->SetBranchAddress("zll_minMTBMet", &zll_minMTBMet, &b_zll_minMTBMet);
   fChain->SetBranchAddress("zllmt_mt2", &zllmt_mt2, &b_zllmt_mt2);
   fChain->SetBranchAddress("zllmt_deltaPhiMin", &zllmt_deltaPhiMin, &b_zllmt_deltaPhiMin);
   fChain->SetBranchAddress("zllmt_diffMetMht", &zllmt_diffMetMht, &b_zllmt_diffMetMht);
   fChain->SetBranchAddress("zllmt_met_pt", &zllmt_met_pt, &b_zllmt_met_pt);
   fChain->SetBranchAddress("zllmt_met_phi", &zllmt_met_phi, &b_zllmt_met_phi);
   fChain->SetBranchAddress("zllmt_mht_pt", &zllmt_mht_pt, &b_zllmt_mht_pt);
   fChain->SetBranchAddress("zllmt_mht_phi", &zllmt_mht_phi, &b_zllmt_mht_phi);
   fChain->SetBranchAddress("zllmt_ht", &zllmt_ht, &b_zllmt_ht);
   fChain->SetBranchAddress("zllmt_mt", &zllmt_mt, &b_zllmt_mt);
   fChain->SetBranchAddress("rl_mt2", &rl_mt2, &b_rl_mt2);
   fChain->SetBranchAddress("rl_met_pt", &rl_met_pt, &b_rl_met_pt);
   fChain->SetBranchAddress("rl_met_phi", &rl_met_phi, &b_rl_met_phi);
   fChain->SetBranchAddress("rl_mht_pt", &rl_mht_pt, &b_rl_mht_pt);
   fChain->SetBranchAddress("rl_mht_phi", &rl_mht_phi, &b_rl_mht_phi);
   fChain->SetBranchAddress("rl_deltaPhiMin", &rl_deltaPhiMin, &b_rl_deltaPhiMin);
   fChain->SetBranchAddress("rl_diffMetMht", &rl_diffMetMht, &b_rl_diffMetMht);
   fChain->SetBranchAddress("rl_mass", &rl_mass, &b_rl_mass);
   fChain->SetBranchAddress("rl_pt", &rl_pt, &b_rl_pt);
   fChain->SetBranchAddress("rl_eta", &rl_eta, &b_rl_eta);
   fChain->SetBranchAddress("rl_phi", &rl_phi, &b_rl_phi);
   fChain->SetBranchAddress("rl_ht", &rl_ht, &b_rl_ht);
   fChain->SetBranchAddress("rl_minMTBMet", &rl_minMTBMet, &b_rl_minMTBMet);
   fChain->SetBranchAddress("ngenPart", &ngenPart, &b_ngenPart);
   fChain->SetBranchAddress("genPart_pt", genPart_pt, &b_genPart_pt);
   fChain->SetBranchAddress("genPart_eta", genPart_eta, &b_genPart_eta);
   fChain->SetBranchAddress("genPart_phi", genPart_phi, &b_genPart_phi);
   fChain->SetBranchAddress("genPart_mass", genPart_mass, &b_genPart_mass);
   fChain->SetBranchAddress("genPart_pdgId", genPart_pdgId, &b_genPart_pdgId);
   fChain->SetBranchAddress("genPart_status", genPart_status, &b_genPart_status);
   fChain->SetBranchAddress("genPart_charge", genPart_charge, &b_genPart_charge);
   fChain->SetBranchAddress("genPart_motherId", genPart_motherId, &b_genPart_motherId);
   fChain->SetBranchAddress("genPart_grandmaId", genPart_grandmaId, &b_genPart_grandmaId);
   fChain->SetBranchAddress("ngenLep", &ngenLep, &b_ngenLep);
   fChain->SetBranchAddress("genLep_pt", genLep_pt, &b_genLep_pt);
   fChain->SetBranchAddress("genLep_eta", genLep_eta, &b_genLep_eta);
   fChain->SetBranchAddress("genLep_phi", genLep_phi, &b_genLep_phi);
   fChain->SetBranchAddress("genLep_mass", genLep_mass, &b_genLep_mass);
   fChain->SetBranchAddress("genLep_pdgId", genLep_pdgId, &b_genLep_pdgId);
   fChain->SetBranchAddress("genLep_status", genLep_status, &b_genLep_status);
   fChain->SetBranchAddress("genLep_charge", genLep_charge, &b_genLep_charge);
   fChain->SetBranchAddress("genLep_sourceId", genLep_sourceId, &b_genLep_sourceId);
   fChain->SetBranchAddress("ngenStat23", &ngenStat23, &b_ngenStat23);
   fChain->SetBranchAddress("genStat23_pt", genStat23_pt, &b_genStat23_pt);
   fChain->SetBranchAddress("genStat23_eta", genStat23_eta, &b_genStat23_eta);
   fChain->SetBranchAddress("genStat23_phi", genStat23_phi, &b_genStat23_phi);
   fChain->SetBranchAddress("genStat23_mass", genStat23_mass, &b_genStat23_mass);
   fChain->SetBranchAddress("genStat23_pdgId", genStat23_pdgId, &b_genStat23_pdgId);
   fChain->SetBranchAddress("genStat23_status", genStat23_status, &b_genStat23_status);
   fChain->SetBranchAddress("genStat23_charge", genStat23_charge, &b_genStat23_charge);
   fChain->SetBranchAddress("genStat23_sourceId", genStat23_sourceId, &b_genStat23_sourceId);
   fChain->SetBranchAddress("ngenGamma", &ngenGamma, &b_ngenGamma);
   fChain->SetBranchAddress("genGamma_pt", genGamma_pt, &b_genGamma_pt);
   fChain->SetBranchAddress("genGamma_eta", genGamma_eta, &b_genGamma_eta);
   fChain->SetBranchAddress("genGamma_phi", genGamma_phi, &b_genGamma_phi);
   fChain->SetBranchAddress("genGamma_motherId", genGamma_motherId, &b_genGamma_motherId);
   fChain->SetBranchAddress("ngenLepFromTau", &ngenLepFromTau, &b_ngenLepFromTau);
   fChain->SetBranchAddress("genLepFromTau_pt", genLepFromTau_pt, &b_genLepFromTau_pt);
   fChain->SetBranchAddress("genLepFromTau_eta", genLepFromTau_eta, &b_genLepFromTau_eta);
   fChain->SetBranchAddress("genLepFromTau_phi", genLepFromTau_phi, &b_genLepFromTau_phi);
   fChain->SetBranchAddress("genLepFromTau_mass", genLepFromTau_mass, &b_genLepFromTau_mass);
   fChain->SetBranchAddress("genLepFromTau_pdgId", genLepFromTau_pdgId, &b_genLepFromTau_pdgId);
   fChain->SetBranchAddress("genLepFromTau_status", genLepFromTau_status, &b_genLepFromTau_status);
   fChain->SetBranchAddress("genLepFromTau_charge", genLepFromTau_charge, &b_genLepFromTau_charge);
   fChain->SetBranchAddress("genLepFromTau_sourceId", genLepFromTau_sourceId, &b_genLepFromTau_sourceId);
   fChain->SetBranchAddress("ngenTau", &ngenTau, &b_ngenTau);
   fChain->SetBranchAddress("genTau_pt", genTau_pt, &b_genTau_pt);
   fChain->SetBranchAddress("genTau_eta", genTau_eta, &b_genTau_eta);
   fChain->SetBranchAddress("genTau_phi", genTau_phi, &b_genTau_phi);
   fChain->SetBranchAddress("genTau_mass", genTau_mass, &b_genTau_mass);
   fChain->SetBranchAddress("genTau_pdgId", genTau_pdgId, &b_genTau_pdgId);
   fChain->SetBranchAddress("genTau_status", genTau_status, &b_genTau_status);
   fChain->SetBranchAddress("genTau_charge", genTau_charge, &b_genTau_charge);
   fChain->SetBranchAddress("genTau_sourceId", genTau_sourceId, &b_genTau_sourceId);
   fChain->SetBranchAddress("njet", &njet, &b_njet);
   fChain->SetBranchAddress("jet_pt", jet_pt, &b_jet_pt);
   fChain->SetBranchAddress("jet_eta", jet_eta, &b_jet_eta);
   fChain->SetBranchAddress("jet_phi", jet_phi, &b_jet_phi);
   fChain->SetBranchAddress("jet_mass", jet_mass, &b_jet_mass);
   fChain->SetBranchAddress("jet_btagCSV", jet_btagCSV, &b_jet_btagCSV);
   fChain->SetBranchAddress("jet_rawPt", jet_rawPt, &b_jet_rawPt);
   fChain->SetBranchAddress("jet_mcPt", jet_mcPt, &b_jet_mcPt);
   fChain->SetBranchAddress("jet_mcFlavour", jet_mcFlavour, &b_jet_mcFlavour);
   fChain->SetBranchAddress("jet_qgl", jet_qgl, &b_jet_qgl);
   fChain->SetBranchAddress("jet_area", jet_area, &b_jet_area);
   fChain->SetBranchAddress("jet_id", jet_id, &b_jet_id);
   fChain->SetBranchAddress("jet_puId", jet_puId, &b_jet_puId);
   fChain->SetBranchAddress("GenSusyMScan1", &GenSusyMScan1, &b_GenSusyMScan1);
   fChain->SetBranchAddress("GenSusyMScan2", &GenSusyMScan2, &b_GenSusyMScan2);
   fChain->SetBranchAddress("GenSusyMScan3", &GenSusyMScan3, &b_GenSusyMScan3);
   fChain->SetBranchAddress("GenSusyMScan4", &GenSusyMScan4, &b_GenSusyMScan4);
   fChain->SetBranchAddress("weight_lepsf", &weight_lepsf, &b_weight_lepsf);
   fChain->SetBranchAddress("weight_lepsf_UP", &weight_lepsf_UP, &b_weight_lepsf_UP);
   fChain->SetBranchAddress("weight_lepsf_DN", &weight_lepsf_DN, &b_weight_lepsf_DN);
   fChain->SetBranchAddress("weight_btagsf", &weight_btagsf, &b_weight_btagsf);
   fChain->SetBranchAddress("weight_btagsf_UP", &weight_btagsf_UP, &b_weight_btagsf_UP);
   fChain->SetBranchAddress("weight_btagsf_DN", &weight_btagsf_DN, &b_weight_btagsf_DN);
   fChain->SetBranchAddress("weight_sigtrigsf", &weight_sigtrigsf, &b_weight_sigtrigsf);
   fChain->SetBranchAddress("weight_dileptrigsf", &weight_dileptrigsf, &b_weight_dileptrigsf);
   fChain->SetBranchAddress("weight_phottrigsf", &weight_phottrigsf, &b_weight_phottrigsf);
   fChain->SetBranchAddress("weight_pu", &weight_pu, &b_weight_pu);
   fChain->SetBranchAddress("weight_isr", &weight_isr, &b_weight_isr);
   fChain->SetBranchAddress("weight_scales_UP", &weight_scales_UP, &b_weight_scales_UP);
   fChain->SetBranchAddress("weight_scales_DN", &weight_scales_DN, &b_weight_scales_DN);
   fChain->SetBranchAddress("weight_pdfs_UP", &weight_pdfs_UP, &b_weight_pdfs_UP);
   fChain->SetBranchAddress("weight_pdfs_DN", &weight_pdfs_DN, &b_weight_pdfs_DN);
   Notify();
}

Bool_t mt2tree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void mt2tree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t mt2tree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef mt2tree_cxx

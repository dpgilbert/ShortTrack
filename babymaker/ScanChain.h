#ifndef ScanChain_h
#define ScanChain_h

// C++ includes
//#include <string>
//#include <vector>

// ROOT includes
#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
//#include "TH1F.h"
#include "Math/LorentzVector.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;

class babyMaker {

 public:

  babyMaker() {};
  ~babyMaker() {
    delete BabyFile_;
    delete BabyTree_;
  };

  void ScanChain(TChain*, std::string = "testSample");

  void MakeBabyNtuple(const char *);
  void InitBabyNtuple();
  void FillBabyNtuple();
  void CloseBabyNtuple();

 private:

  TFile *BabyFile_;
  TTree *BabyTree_;


  //baby ntuple variables

  Int_t           run;
  Int_t           lumi;
  Int_t           evt;
  Int_t           isData;

  Float_t         crossSection;
  Float_t         puWeight;
  Int_t           nVert;
  Int_t           nTrueInt;
  Float_t         rho;
  Float_t         rho25;

  Int_t           nJet40;
  Int_t           nBJet40;
  Int_t           nMuons10;
  Int_t           nElectrons10;

  Float_t         deltaPhiMin;
  Float_t         diffMetMht;

  Float_t         ht;
  Float_t         mt2;

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
  Float_t         rawmet_pt;
  Float_t         rawmet_phi;
  Float_t         calomet_pt;
  Float_t         calomet_phi;
  Float_t         genmet_pt;
  Float_t         genmet_phi;

//----- TRIGGER (to be better defined)
  Int_t           HLT_HT650;   
  Int_t           HLT_MET150;
  Int_t           HLT_ht350met100;   

//----- LEPTONS
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
  Int_t           lep_mcMatchId[50];   //[nlep]
  Int_t           lep_lostHits[50];   //[nlep]
  Int_t           lep_convVeto[50];   //[nlep]
  Int_t           lep_tightCharge[50];   //[nlep]

//----- ISOLATED TRACK
  Int_t             nisoTrack;
  Float_t           isoTrack_pt[50];
  Float_t           isoTrack_eta[50];
  Float_t           isoTrack_phi[50];
  Float_t           isoTrack_mass[50];
  Float_t           isoTrack_absIso[50];
  Float_t           isoTrack_dz[50];
  Int_t             isoTrack_pdgId[50];

//----- TAUS
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

//----- PHOTONS
  Int_t           ngamma;
  Float_t         gamma_pt[50];   //[ngamma]
  Float_t         gamma_eta[50];   //[ngamma]
  Float_t         gamma_phi[50];   //[ngamma]
  Float_t         gamma_mass[50];   //[ngamma]
  Int_t           gamma_mcMatchId[50];   //[ngamma]
  Float_t         gamma_chHadIso[50];   //[ngamma]
  Float_t         gamma_neuHadIso[50];   //[ngamma]
  Float_t         gamma_phIso[50];   //[ngamma]
  Float_t         gamma_sigmaIetaIeta[50];   //[ngamma]
  Float_t         gamma_r9[50];   //[ngamma]
  Float_t         gamma_hOverE[50];   //[ngamma]
  Int_t           gamma_idCutBased[50];   //[ngamma]

//----- GEN PARTICLES
  Int_t           ngenPart;
  Float_t         genPart_pt[200];   //[ngenPart]
  Float_t         genPart_eta[200];   //[ngenPart]
  Float_t         genPart_phi[200];   //[ngenPart]
  Float_t         genPart_mass[200];   //[ngenPart]
  Int_t           genPart_pdgId[200];   //[ngenPart]
  Float_t         genPart_charge[200];   //[ngenPart]
  Int_t           genPart_motherId[200];   //[ngenPart]
  Int_t           genPart_grandmaId[200];   //[ngenPart]

//----- JETS
   Int_t           njet;
   Float_t         jet_pt[100];   //[njet]
   Float_t         jet_eta[100];   //[njet]
   Float_t         jet_phi[100];   //[njet]
   Float_t         jet_mass[100];   //[njet]
   Float_t         jet_btagCSV[100];   //[njet]
   Float_t         jet_rawPt[100];   //[njet]
   Float_t         jet_mcPt[100];   //[njet]
   Int_t           jet_mcFlavour[100];   //[njet]
   Float_t         jet_quarkGluonID[100];   //[njet]
   Float_t         jet_area[100];   //[njet]
   Int_t           jet_id[100];   //[njet]
   Int_t           jet_puId[100];   //[njet]

};

#endif

void babyMaker::MakeBabyNtuple(const char *BabyFilename){

  //
  TDirectory *rootdir = gDirectory->GetDirectory("Rint:");
  rootdir->cd();
  BabyFile_ = new TFile(Form("%s", BabyFilename), "RECREATE");
  BabyFile_->cd();
  BabyTree_ = new TTree("tree", "A Baby Ntuple");

  BabyTree_->Branch("run", &run );
  BabyTree_->Branch("lumi", &lumi );
  BabyTree_->Branch("evt", &evt );
  BabyTree_->Branch("isData", &isData );
  BabyTree_->Branch("crossSection", &crossSection );
  BabyTree_->Branch("puWeight", &puWeight );
  BabyTree_->Branch("nVert", &nVert );
  BabyTree_->Branch("nTrueInt", &nTrueInt );
  BabyTree_->Branch("rho", &rho );
  BabyTree_->Branch("rho25", &rho25 );
  BabyTree_->Branch("nJet40", &nJet40 );
  BabyTree_->Branch("nBJet40", &nBJet40 );
  BabyTree_->Branch("nMuons10", &nMuons10 );
  BabyTree_->Branch("nElectrons10", &nElectrons10 );
  BabyTree_->Branch("deltaPhiMin", &deltaPhiMin );
  BabyTree_->Branch("diffMetMht", &diffMetMht );
  BabyTree_->Branch("ht", &ht );
  BabyTree_->Branch("mt2", &mt2 );
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
  BabyTree_->Branch("rawmet_pt", &rawmet_pt );
  BabyTree_->Branch("rawmet_phi", &rawmet_phi );
  BabyTree_->Branch("calomet_pt", &calomet_pt );
  BabyTree_->Branch("calomet_phi", &calomet_phi );
  BabyTree_->Branch("genmet_pt", &genmet_pt );
  BabyTree_->Branch("genmet_phi", &genmet_phi );
  BabyTree_->Branch("HLT_HT650", &HLT_HT650 );
  BabyTree_->Branch("HLT_MET150", &HLT_MET150 );
  BabyTree_->Branch("HLT_ht350met100", &HLT_ht350met100 );
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
  BabyTree_->Branch("lep_relIso03", lep_relIso03, "lep_relIso03[nlep]/F" );
  BabyTree_->Branch("lep_relIso04", lep_relIso04, "lep_relIso04[nlep]/F" );
  BabyTree_->Branch("lep_mcMatchId", lep_mcMatchId, "lep_mcMatchId[nlep]/I" );
  BabyTree_->Branch("lep_lostHits", lep_lostHits, "lep_lostHits[nlep]/I" );
  BabyTree_->Branch("lep_convVeto", lep_convVeto, "lep_convVeto[nlep]/I" );
  BabyTree_->Branch("lep_tightCharge", lep_tightCharge, "lep_tightCharge[nlep]/I" );
  BabyTree_->Branch("nisoTrack", &nisoTrack, "nisoTrack/I" );
  BabyTree_->Branch("isoTrack_pt", isoTrack_pt, "isoTrack_pt[nisoTrack]/F" );
  BabyTree_->Branch("isoTrack_eta", isoTrack_eta, "isoTrack_eta[nisoTrack]/F" );
  BabyTree_->Branch("isoTrack_phi", isoTrack_phi, "isoTrack_phi[nisoTrack]/F" );
  BabyTree_->Branch("isoTrack_mass", isoTrack_mass, "isoTrack_mass[nisoTrack]/F" );
  BabyTree_->Branch("isoTrackabssIso", isoTrack_absIso, "isoTrack_absIso[nisoTrack]/F" );
  BabyTree_->Branch("isoTrack_dz", isoTrack_dz, "isoTrack_dz[nisoTrack]/F" );
  BabyTree_->Branch("isoTrack_pdgId", isoTrack_pdgId, "isoTrack_pdgId[nisoTrack]/I" );
  BabyTree_->Branch("ntau", &ntau, "ntau/I" );
  BabyTree_->Branch("tau_pt", tau_pt, "tau_pt[ntau]/F" );
  BabyTree_->Branch("tau_eta", tau_eta, "tau_eta[ntau]/F" );
  BabyTree_->Branch("tau_phi", tau_phi, "tau_phi[ntau]/F" );
  BabyTree_->Branch("tau_mass", tau_mass, "tau_mass[ntau]/F" );
  BabyTree_->Branch("tau_charge", tau_charge, "tau_charge[ntau]/I" );
  BabyTree_->Branch("tau_dxy", tau_dxy, "tau_dxy[ntau]/F" );
  BabyTree_->Branch("tau_dz", tau_dz, "tau_dz[ntau]/F" );
  BabyTree_->Branch("tau_isoMVA2", tau_isoMVA2, "tau_isoMVA2[ntau]/F" );
  BabyTree_->Branch("tau_idCI3hit", tau_idCI3hit, "tau_idCI3hit[ntau]/I" );
  BabyTree_->Branch("tau_isoCI3hit", tau_isoCI3hit, "tau_isoCI3hit[ntau]/F" );
  BabyTree_->Branch("tau_idMVA2", tau_idMVA2, "tau_idMVA2[ntau]/I" );
  BabyTree_->Branch("tau_mcMatchId", tau_mcMatchId, "tau_mcMatchId[ntau]/I" );
  BabyTree_->Branch("ngamma", &ngamma, "ngamma/I" );
  BabyTree_->Branch("gamma_pt", gamma_pt, "gamma_pt[ngamma]/F" );
  BabyTree_->Branch("gamma_eta", gamma_eta, "gamma_eta[ngamma]/F" );
  BabyTree_->Branch("gamma_phi", gamma_phi, "gamma_phi[ngamma]/F" );
  BabyTree_->Branch("gamma_mass", gamma_mass, "gamma_mass[ngamma]/F" );
  BabyTree_->Branch("gamma_mcMatchId", gamma_mcMatchId, "gamma_eta[ngamma]/I" );
  BabyTree_->Branch("gamma_chHadIso", gamma_chHadIso, "gamma_chHadIso[ngamma]/F" );
  BabyTree_->Branch("gamma_neuHadIso", gamma_neuHadIso, "gamma_neuHadIso[ngamma]/F" );
  BabyTree_->Branch("gamma_phIso", gamma_phIso, "gamma_phIso[ngamma]/F" );
  BabyTree_->Branch("gamma_sigmaIetaIeta", gamma_sigmaIetaIeta, "gamma_sigmaIetaIeta[ngamma]/F" );
  BabyTree_->Branch("gamma_r9", gamma_r9, "gamma_r9[ngamma]/F" );
  BabyTree_->Branch("gamma_hOverE", gamma_hOverE, "gamma_hOverE[ngamma]/F" );
  BabyTree_->Branch("gamma_idCutBased", gamma_idCutBased, "gamma_idCutBased[ngamma]/I" );
  BabyTree_->Branch("ngenPart", &ngenPart, "ngenPart/I" );
  BabyTree_->Branch("genPart_pt", genPart_pt, "genPart_pt[ngenPart]/F" );
  BabyTree_->Branch("genPart_eta", genPart_eta, "genPart_eta[ngenPart]/F" );
  BabyTree_->Branch("genPart_phi", genPart_phi, "genPart_phi[ngenPart]/F" );
  BabyTree_->Branch("genPart_mass", genPart_mass, "genPart_mass[ngenPart]/F" );
  BabyTree_->Branch("genPart_pdgId", genPart_pdgId, "genPart_pdgId[ngenPart]/I" );
  BabyTree_->Branch("genPart_charge", genPart_charge, "genPart_charge[ngenPart]/I" );
  BabyTree_->Branch("genPart_motherId", genPart_motherId, "genPart_motherId[ngenPart]/I" );
  BabyTree_->Branch("genPart_grandmaId", genPart_grandmaId, "genPart_grandmaId[ngenPart]/I" );
  BabyTree_->Branch("njet", &njet, "njet/I" );
  BabyTree_->Branch("jet_pt", jet_pt, "jet_pt[njet]/F" );
  BabyTree_->Branch("jet_eta", jet_eta, "jet_eta[njet]/F" );
  BabyTree_->Branch("jet_phi", jet_phi, "jet_phi[njet]/F" );
  BabyTree_->Branch("jet_mass", jet_mass, "jet_mass[njet]/F" );
  BabyTree_->Branch("jet_btagCSV", jet_btagCSV, "jet_btagCSV[njet]/F" );
  BabyTree_->Branch("jet_rawPt", jet_rawPt, "jet_rawPt[njet]/F" );
  BabyTree_->Branch("jet_mcPt", jet_mcPt, "jet_mcPt[njet]/F" );
  BabyTree_->Branch("jet_mcFlavour", jet_mcFlavour, "jet_mcFlavor[njet]/I" );
  BabyTree_->Branch("jet_quarkGluonID", jet_quarkGluonID, "jet_quarkGluonID[njet]/F" );
  BabyTree_->Branch("jet_area", jet_area, "jet_area[njet]/F" );
  BabyTree_->Branch("jet_id", jet_id, "jet_id[njet]/I" );
  BabyTree_->Branch("jet_puId", jet_puId, "jet_puId[njet]/I" );

  return;
}

void babyMaker::InitBabyNtuple () {

  run    = -999;
  lumi   = -999;
  evt    = -999;
  isData = -999;
  crossSection = -999.0;
  puWeight = -999.0;
  nVert = -999;
  nTrueInt = -999;
  rho = -999.0;
  rho25 = -999.0;
  nJet40 = -999;
  nBJet40 = -999;
  nMuons10 = -999;
  nElectrons10 = -999;
  deltaPhiMin = -999.0;
  diffMetMht = -999.0;
  ht = -999.0;
  mt2 = -999.0;
  pseudoJet1_pt = -999.0;
  pseudoJet1_eta = -999.0;
  pseudoJet1_phi = -999.0;
  pseudoJet1_mass = -999.0;
  pseudoJet2_pt = -999.0;
  pseudoJet2_eta = -999.0;
  pseudoJet2_phi = -999.0;
  pseudoJet2_mass = -999.0;
  mht_pt = -999.0;
  mht_phi = -999.0;
  met_pt = -999.0;
  met_phi = -999.0;
  rawmet_pt = -999.0;
  rawmet_phi = -999.0;
  calomet_pt = -999.0;
  calomet_phi = -999.0;
  genmet_pt = -999.0;
  genmet_phi = -999.0;
  HLT_HT650 = -999;
  HLT_MET150 = -999;
  HLT_ht350met100 = -999;
  nlep = -999;
  nisoTrack = -999;
  ntau = -999;
  ngamma = -999;
  ngenPart = -999;
  njet = -999;

  
  return;
}

void babyMaker::FillBabyNtuple(){
  BabyTree_->Fill();
  return;
}

void babyMaker::CloseBabyNtuple(){
  BabyFile_->cd();
  BabyTree_->Write();
  BabyFile_->Close();
  return;
}

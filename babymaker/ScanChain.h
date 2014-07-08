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

  Float_t         pseudojet1_pt;
  Float_t         pseudojet1_eta;
  Float_t         pseudojet1_phi;
  Float_t         pseudojet1_mass;
  Float_t         pseudojet2_pt;
  Float_t         pseudojet2_eta;
  Float_t         pseudojet2_phi;
  Float_t         pseudojet2_mass;

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
  Int_t           hlt_HT650;   
  Int_t           hlt_MET150;
  Int_t           hlt_ht350met100;   

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
  Float_t           IsoTrack_pt;
  Float_t           IsoTrack_relIso;
  Float_t           IsoTrack_dz;
  Int_t             IsoTrack_pdgId;
  Int_t             IsoTrack_mtw;

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
  Int_t           tau_idMVA2[50];   //[ntau]
  Int_t           tau_mcMatchId[50];   //[ntau]

//----- PHOTONS
  Int_t           ngamma;
  Float_t         gamma_pt[50];   //[ngamma]
  Float_t         gamma_eta[50];   //[ngamma]
  Float_t         gamma_phi[50];   //[ngamma]
  Float_t         gamma_mass[50];   //[ngamma]
  Int_t           gamma_mcMatchId[50];   //[ngamma]
  Float_t         gamma_chadiso[50];   //[ngamma]
  Float_t         gamma_nhadiso[50];   //[ngamma]
  Float_t         gamma_photiso[50];   //[ngamma]
  Float_t         gamma_sigmaietaieta[50];   //[ngamma]
  Int_t           gamma_id[50];   //[ngamma]

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
   Int_t           nJet;
   Float_t         Jet_pt[100];   //[nJet]
   Float_t         Jet_eta[100];   //[nJet]
   Float_t         Jet_phi[100];   //[nJet]
   Float_t         Jet_mass[100];   //[nJet]
   Float_t         Jet_btagCSV[100];   //[nJet]
   Float_t         Jet_rawPt[100];   //[nJet]
   Float_t         Jet_mcPt[100];   //[nJet]
   Int_t           Jet_mcFlavour[100];   //[nJet]
   Float_t         Jet_quarkGluonID[100];   //[nJet]
   Float_t         Jet_area[100];   //[nJet]
   Int_t           Jet_id[100];   //[nJet]
   Int_t           Jet_puId[100];   //[nJet]

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
  BabyTree_->Branch("pseudojet1_pt", &pseudojet1_pt );
  BabyTree_->Branch("pseudojet1_eta", &pseudojet1_eta );
  BabyTree_->Branch("pseudojet1_phi", &pseudojet1_phi );
  BabyTree_->Branch("pseudojet1_mass", &pseudojet1_mass );
  BabyTree_->Branch("pseudojet2_pt", &pseudojet2_pt );
  BabyTree_->Branch("pseudojet2_eta", &pseudojet2_eta );
  BabyTree_->Branch("pseudojet2_phi", &pseudojet2_phi );
  BabyTree_->Branch("pseudojet2_mass", &pseudojet2_mass );
  BabyTree_->Branch("mht_pt", &met_pt );
  BabyTree_->Branch("mht_phi", &met_phi );
  BabyTree_->Branch("met_pt", &met_pt );
  BabyTree_->Branch("met_phi", &met_phi );
  BabyTree_->Branch("rawmet_pt", &rawmet_pt );
  BabyTree_->Branch("rawmet_phi", &rawmet_phi );
  BabyTree_->Branch("calomet_pt", &calomet_pt );
  BabyTree_->Branch("calomet_phi", &calomet_phi );
  BabyTree_->Branch("genmet_pt", &genmet_pt );
  BabyTree_->Branch("genmet_phi", &genmet_phi );
  BabyTree_->Branch("hlt_HT650", &hlt_HT650 );
  BabyTree_->Branch("hlt_MET150", &hlt_MET150 );
  BabyTree_->Branch("hlt_ht350met100", &hlt_ht350met100 );
  BabyTree_->Branch("nlep", &nlep, "nlep/I" );
  BabyTree_->Branch("lep_pt", lep_pt, "lep_pt[nlep]/F");
  BabyTree_->Branch("lep_eta", lep_eta, "lep_eta[nlep]/F" );
  BabyTree_->Branch("lep_phi", lep_phi, "lep_phi[nlep]/F" );
  BabyTree_->Branch("lep_mass", lep_mass, "lep_mass[nlep]/F" );
  BabyTree_->Branch("lep_charge", lep_charge, "lep_charge[nlep]/F" );
  BabyTree_->Branch("lep_pdgId", lep_pdgId, "lep_pdgId[nlep]/F" );
  BabyTree_->Branch("lep_dxy", lep_dxy, "lep_dxy[nlep]/F" );
  BabyTree_->Branch("lep_dz", lep_dz, "lep_dz[nlep]/F" );
  BabyTree_->Branch("lep_tightId", lep_tightId, "lep_tightId[nlep]/F" );
  BabyTree_->Branch("lep_relIso03", lep_relIso03, "lep_relIso03[nlep]/F" );
  BabyTree_->Branch("lep_relIso04", lep_relIso04, "lep_relIso04[nlep]/F" );
  BabyTree_->Branch("lep_mcMatchId", lep_mcMatchId, "lep_mcMatchId[nlep]/F" );
  BabyTree_->Branch("lep_lostHits", lep_lostHits, "lep_lostHits[nlep]/F" );
  BabyTree_->Branch("lep_convVeto", lep_convVeto, "lep_convVeto[nlep]/F" );
  BabyTree_->Branch("lep_tightCharge", lep_tightCharge, "lep_tightCharge[nlep]/F" );
  BabyTree_->Branch("IsoTrack_pt", &IsoTrack_pt );
  BabyTree_->Branch("IsoTrack_relIso", &IsoTrack_relIso );
  BabyTree_->Branch("IsoTrack_dz", &IsoTrack_dz );
  BabyTree_->Branch("IsoTrack_pdgId", &IsoTrack_pdgId );
  BabyTree_->Branch("IsoTrack_mtw", &IsoTrack_mtw );
  BabyTree_->Branch("ntau", &ntau, "ntau/I" );
  BabyTree_->Branch("tau_pt", tau_pt, "tau_pt[ntau]/F" );
  BabyTree_->Branch("tau_eta", tau_eta, "tau_eta[ntau]/F" );
  BabyTree_->Branch("tau_phi", tau_phi, "tau_phi[ntau]/F" );
  BabyTree_->Branch("tau_mass", tau_mass, "tau_mass[ntau]/F" );
  BabyTree_->Branch("tau_charge", tau_charge, "tau_charge[ntau]/F" );
  BabyTree_->Branch("tau_dxy", tau_dxy, "tau_dxy[ntau]/F" );
  BabyTree_->Branch("tau_dz", tau_dz, "tau_dz[ntau]/F" );
  BabyTree_->Branch("tau_isoMVA2", tau_isoMVA2, "tau_isoMVA2[ntau]/F" );
  BabyTree_->Branch("tau_idCI3hit", tau_idCI3hit, "tau_idCI3hit[ntau]/F" );
  BabyTree_->Branch("tau_idMVA2", tau_idMVA2, "tau_idMVA2[ntau]/F" );
  BabyTree_->Branch("tau_mcMatchId", tau_mcMatchId, "tau_mcMatchId[ntau]/F" );
  BabyTree_->Branch("ngamma", &ngamma, "ngamma/I" );
  BabyTree_->Branch("gamma_pt", gamma_pt, "gamma_pt[ngamma]/F" );
  BabyTree_->Branch("gamma_eta", gamma_eta, "gamma_eta[ngamma]/F" );
  BabyTree_->Branch("gamma_phi", gamma_phi, "gamma_phi[ngamma]/F" );
  BabyTree_->Branch("gamma_mass", gamma_mass, "gamma_mass[ngamma]/F" );
  BabyTree_->Branch("gamma_mcMatchId", gamma_mcMatchId, "gamma_eta[ngamma]/F" );
  BabyTree_->Branch("gamma_chadiso", gamma_chadiso, "gamma_chadiso[ngamma]/F" );
  BabyTree_->Branch("gamma_nhadiso", gamma_nhadiso, "gamma_nhadiso[ngamma]/F" );
  BabyTree_->Branch("gamma_photiso", gamma_photiso, "gamma_photiso[ngamma]/F" );
  BabyTree_->Branch("gamma_sigmaietaieta", gamma_sigmaietaieta, "gamma_sigmaietaieta[ngamma]/F" );
  BabyTree_->Branch("gamma_id", gamma_id, "gamma_id[ngamma]/F" );
  BabyTree_->Branch("ngenPart", &ngenPart, "ngenPart/I" );
  BabyTree_->Branch("genPart_pt", genPart_pt, "genPart_pt[ngenPart]/F" );
  BabyTree_->Branch("genPart_eta", genPart_eta, "genPart_eta[ngenPart]/F" );
  BabyTree_->Branch("genPart_phi", genPart_phi, "genPart_phi[ngenPart]/F" );
  BabyTree_->Branch("genPart_mass", genPart_mass, "genPart_mass[ngenPart]/F" );
  BabyTree_->Branch("genPart_pdgId", genPart_pdgId, "genPart_pdgId[ngenPart]/F" );
  BabyTree_->Branch("genPart_charge", genPart_charge, "genPart_charge[ngenPart]/F" );
  BabyTree_->Branch("genPart_motherId", genPart_motherId, "genPart_motherId[ngenPart]/F" );
  BabyTree_->Branch("genPart_grandmaId", genPart_grandmaId, "genPart_grandmaId[ngenPart]/F" );
  BabyTree_->Branch("nJet", &nJet, "nJet/I" );
  BabyTree_->Branch("Jet_pt", Jet_pt, "Jet_pt[nJet]/F" );
  BabyTree_->Branch("Jet_eta", Jet_eta, "Jet_eta[nJet]/F" );
  BabyTree_->Branch("Jet_phi", Jet_phi, "Jet_phi[nJet]/F" );
  BabyTree_->Branch("Jet_mass", Jet_mass, "Jet_mass[nJet]/F" );
  BabyTree_->Branch("Jet_btagCSV", Jet_btagCSV, "Jet_btagCSV[nJet]/F" );
  BabyTree_->Branch("Jet_rawPt", Jet_rawPt, "Jet_rawPt[nJet]/F" );
  BabyTree_->Branch("Jet_mcFlavour", Jet_mcFlavour, "Jet_mcFlavor[nJet]/F" );
  BabyTree_->Branch("Jet_quarkGluonID", Jet_quarkGluonID, "Jet_quarkGluonID[nJet]/F" );
  BabyTree_->Branch("Jet_area", Jet_area, "Jet_area[nJet]/F" );
  BabyTree_->Branch("Jet_id", Jet_id, "Jet_id[nJet]/F" );
  BabyTree_->Branch("Jet_puId", Jet_puId, "Jet_puId[nJet]/F" );

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
  pseudojet1_pt = -999.0;
  pseudojet1_eta = -999.0;
  pseudojet1_phi = -999.0;
  pseudojet1_mass = -999.0;
  pseudojet2_pt = -999.0;
  pseudojet2_eta = -999.0;
  pseudojet2_phi = -999.0;
  pseudojet2_mass = -999.0;
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
  hlt_HT650 = -999;
  hlt_MET150 = -999;
  hlt_ht350met100 = -999;
  nlep = -999;
  IsoTrack_pt = -999.0;
  IsoTrack_relIso = -999.0;
  IsoTrack_dz = -999.0;
  IsoTrack_pdgId = -999;
  IsoTrack_mtw = -999;
  ntau = -999;
  ngamma = -999;
  ngenPart = -999;
  nJet = -999;

  
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

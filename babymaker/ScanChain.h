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

  Int_t           nevents;
  Float_t         scale1fb;
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
  Int_t           nTaus20;
  Int_t           nGammas20;

  Float_t         deltaPhiMin;
  Float_t         diffMetMht;

  Float_t         ht;
  Float_t         mt2;
  Float_t         mt2_gen;

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
  Int_t             isoTrack_mcMatchId[50];

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


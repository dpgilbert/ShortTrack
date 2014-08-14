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
  static const int max_nlep = 50;
  Int_t           nlep;
  Float_t         lep_pt[max_nlep];   //[nlep]
  Float_t         lep_eta[max_nlep];   //[nlep]
  Float_t         lep_phi[max_nlep];   //[nlep]
  Float_t         lep_mass[max_nlep];   //[nlep]
  Int_t           lep_charge[max_nlep];   //[nlep]
  Int_t           lep_pdgId[max_nlep];   //[nlep]
  Float_t         lep_dxy[max_nlep];   //[nlep]
  Float_t         lep_dz[max_nlep];   //[nlep]
  Int_t           lep_tightId[max_nlep];   //[nlep]
  Float_t         lep_relIso03[max_nlep];   //[nlep]
  Float_t         lep_relIso04[max_nlep];   //[nlep]
  Int_t           lep_mcMatchId[max_nlep];   //[nlep]
  Int_t           lep_lostHits[max_nlep];   //[nlep]
  Int_t           lep_convVeto[max_nlep];   //[nlep]
  Int_t           lep_tightCharge[max_nlep];   //[nlep]

//----- ISOLATED TRACK
  static const int max_nisoTrack = 50;
  Int_t             nisoTrack;
  Float_t           isoTrack_pt[max_nisoTrack];
  Float_t           isoTrack_eta[max_nisoTrack];
  Float_t           isoTrack_phi[max_nisoTrack];
  Float_t           isoTrack_mass[max_nisoTrack];
  Float_t           isoTrack_absIso[max_nisoTrack];
  Float_t           isoTrack_dz[max_nisoTrack];
  Int_t             isoTrack_pdgId[max_nisoTrack];
  Int_t             isoTrack_mcMatchId[max_nisoTrack];

//----- TAUS
  static const int max_ntau = 50;
  Int_t           ntau;
  Float_t         tau_pt[max_ntau];   //[ntau]
  Float_t         tau_eta[max_ntau];   //[ntau]
  Float_t         tau_phi[max_ntau];   //[ntau]
  Float_t         tau_mass[max_ntau];   //[ntau]
  Int_t           tau_charge[max_ntau];   //[ntau]
  Float_t         tau_dxy[max_ntau];   //[ntau]
  Float_t         tau_dz[max_ntau];   //[ntau]
  Float_t         tau_isoMVA2[max_ntau];   //[ntau]
  Int_t           tau_idCI3hit[max_ntau];   //[ntau]
  Float_t         tau_isoCI3hit[max_ntau];   //[ntau]
  Int_t           tau_idMVA2[max_ntau];   //[ntau]
  Int_t           tau_mcMatchId[max_ntau];   //[ntau]

//----- PHOTONS
  static const int max_ngamma = 50;
  Int_t           ngamma;
  Float_t         gamma_pt[max_ngamma];   //[ngamma]
  Float_t         gamma_eta[max_ngamma];   //[ngamma]
  Float_t         gamma_phi[max_ngamma];   //[ngamma]
  Float_t         gamma_mass[max_ngamma];   //[ngamma]
  Int_t           gamma_mcMatchId[max_ngamma];   //[ngamma]
  Float_t         gamma_chHadIso[max_ngamma];   //[ngamma]
  Float_t         gamma_neuHadIso[max_ngamma];   //[ngamma]
  Float_t         gamma_phIso[max_ngamma];   //[ngamma]
  Float_t         gamma_sigmaIetaIeta[max_ngamma];   //[ngamma]
  Float_t         gamma_r9[max_ngamma];   //[ngamma]
  Float_t         gamma_hOverE[max_ngamma];   //[ngamma]
  Int_t           gamma_idCutBased[max_ngamma];   //[ngamma]

//----- GEN PARTICLES
  static const int max_ngenPart = 200;
  Int_t           ngenPart;
  Float_t         genPart_pt[max_ngenPart];   //[ngenPart]
  Float_t         genPart_eta[max_ngenPart];   //[ngenPart]
  Float_t         genPart_phi[max_ngenPart];   //[ngenPart]
  Float_t         genPart_mass[max_ngenPart];   //[ngenPart]
  Int_t           genPart_pdgId[max_ngenPart];   //[ngenPart]
  Float_t         genPart_charge[max_ngenPart];   //[ngenPart]
  Int_t           genPart_motherId[max_ngenPart];   //[ngenPart]
  Int_t           genPart_grandmaId[max_ngenPart];   //[ngenPart]

//----- JETS
  static const int max_njet = 100;
  Int_t           njet;
  Float_t         jet_pt[max_njet];   //[njet]
  Float_t         jet_eta[max_njet];   //[njet]
  Float_t         jet_phi[max_njet];   //[njet]
  Float_t         jet_mass[max_njet];   //[njet]
  Float_t         jet_btagCSV[max_njet];   //[njet]
  Float_t         jet_rawPt[max_njet];   //[njet]
  Float_t         jet_mcPt[max_njet];   //[njet]
  Int_t           jet_mcFlavour[max_njet];   //[njet]
  Float_t         jet_quarkGluonID[max_njet];   //[njet]
  Float_t         jet_area[max_njet];   //[njet]
  Int_t           jet_id[max_njet];   //[njet]
  Int_t           jet_puId[max_njet];   //[njet]

};

#endif


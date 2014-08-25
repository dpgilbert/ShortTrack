#ifndef MINIBABYISOTRACKLOOPER_h
#define MINIBABYISOTRACKLOOPER_h

// C++ includes
//#include <string>
//#include <vector>

// ROOT includes
#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TH1F.h"
#include "Math/LorentzVector.h"

//MT2
#include "../MT2CORE/mt2tree.h"
#include "../MT2CORE/sigSelections.h"


typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;
using namespace mt2;
class MiniBabyIsoTrackLooper {

 public:

  MiniBabyIsoTrackLooper();
  ~MiniBabyIsoTrackLooper();

  void loop(TChain* chain, std::string output_name = "test.root");

  void initBaby();
  void makeTree(TChain *chain);

 private:

  TFile * outfile_;
  TTree * outtree_;
  mt2tree t;

  // minibaby vars
  float weight_;

  int passevt_;
  int sigbase_;
  int sigjets_;
  int sightmet_;

  // isoTrack vars
  float isoTrack_lead_pt_;
  float isoTrack_lead_eta_;
  float isoTrack_lead_phi_;
  float isoTrack_lead_absIso_;
  float isoTrack_lead_dz_;
  int isoTrack_lead_pdgId_;

  float isoTrack_mostiso_pt_;
  float isoTrack_mostiso_eta_;
  float isoTrack_mostiso_phi_;
  float isoTrack_mostiso_absIso_;
  float isoTrack_mostiso_dz_;
  int isoTrack_mostiso_pdgId_;

  // reco lep vars
  float lep1_pt_;
  float lep1_eta_;
  float lep1_phi_;

  float tau1_pt_;
  float tau1_eta_;
  float tau1_phi_;

  // gen vars
  float genlep1_pt_;
  float genlep1_eta_;
  float genlep1_phi_;
  int genlep1_pdgId_;

  float genlep2_pt_;
  float genlep2_eta_;
  float genlep2_phi_;
  int genlep2_pdgId_;

  int ngenel_;
  int ngenmu_;
  int ngentaulep_;
  int ngentauhad_;
  int ngenlep_;

};

#endif


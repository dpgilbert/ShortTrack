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
  int sigbase_nolepveto_;
  int sigjets_;
  int sightmet_;

  // isoTrack vars
  float isoTrackLepPt5_pt_;
  float isoTrackLepPt5_eta_;
  float isoTrackLepPt5_phi_;
  float isoTrackLepPt5_absIso_;
  float isoTrackLepPt5_dz_;
  int isoTrackLepPt5_pdgId_;

  float isoTrackLepPt10_pt_;
  float isoTrackLepPt10_eta_;
  float isoTrackLepPt10_phi_;
  float isoTrackLepPt10_absIso_;
  float isoTrackLepPt10_dz_;
  int isoTrackLepPt10_pdgId_;

  float isoTrackHadPt5_pt_;
  float isoTrackHadPt5_eta_;
  float isoTrackHadPt5_phi_;
  float isoTrackHadPt5_absIso_;
  float isoTrackHadPt5_dz_;

  float isoTrackHadPt10_pt_;
  float isoTrackHadPt10_eta_;
  float isoTrackHadPt10_phi_;
  float isoTrackHadPt10_absIso_;
  float isoTrackHadPt10_dz_;

  float isoTrackHadPt15_pt_;
  float isoTrackHadPt15_eta_;
  float isoTrackHadPt15_phi_;
  float isoTrackHadPt15_absIso_;
  float isoTrackHadPt15_dz_;

  float isoTrackHadPt20_pt_;
  float isoTrackHadPt20_eta_;
  float isoTrackHadPt20_phi_;
  float isoTrackHadPt20_absIso_;
  float isoTrackHadPt20_dz_;

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


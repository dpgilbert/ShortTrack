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
#include "../MT2CORE/SR.h"


typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;
using namespace mt2;
class MiniBabyIsoTrackLooper {

 public:

  MiniBabyIsoTrackLooper();
  ~MiniBabyIsoTrackLooper();

  void loop(TChain* chain, std::string output_name = "test.root");
  void SetSignalRegions();

  void initBaby();
  void makeTree(TChain *chain);

 private:

  TFile * outfile_;
  TTree * outtree_;
  mt2tree t;

  SR SRBase;

  // minibaby vars
  float weight_;

  int passevt_;
  int sigbase_;
  int sigbase_nolepveto_;

  // veto decisions: reco leptons
  int veto_mu10_ ;
  int veto_mu15_ ;
  int veto_mu20_ ;
  int veto_mu10mt100_ ;
  int veto_mu15mt100_ ;
  int veto_mu20mt100_ ;

  int veto_el10_ ;
  int veto_el15_ ;
  int veto_el20_ ;
  int veto_el10mt100_ ;
  int veto_el15mt100_ ;
  int veto_el20mt100_ ;

  // veto decisions: pfmuon
  int veto_pfmu5reliso20_;
  int veto_pfmu5reliso15_;
  int veto_pfmu5reliso10_;
  int veto_pfmu5reliso20mt100_;
  int veto_pfmu5reliso15mt100_;
  int veto_pfmu5reliso10mt100_;

  int veto_pfmu5absiso10_;
  int veto_pfmu5absiso5_;
  int veto_pfmu5absiso3_;
  int veto_pfmu5absiso10mt100_;
  int veto_pfmu5absiso5mt100_;
  int veto_pfmu5absiso3mt100_;

  // veto decisions: pfelectron
  int veto_pfel5reliso20_;
  int veto_pfel5reliso15_;
  int veto_pfel5reliso10_;
  int veto_pfel5reliso20mt100_;
  int veto_pfel5reliso15mt100_;
  int veto_pfel5reliso10mt100_;

  int veto_pfel5absiso10_;
  int veto_pfel5absiso5_;
  int veto_pfel5absiso3_;
  int veto_pfel5absiso10mt100_;
  int veto_pfel5absiso5mt100_;
  int veto_pfel5absiso3mt100_;

  // veto decisions: pfhadron
  int veto_pfhad5reliso20_;
  int veto_pfhad5reliso15_;
  int veto_pfhad5reliso10_;
  int veto_pfhad5reliso20mt100_;
  int veto_pfhad5reliso15mt100_;
  int veto_pfhad5reliso10mt100_;

  int veto_pfhad5absiso10_;
  int veto_pfhad5absiso5_;
  int veto_pfhad5absiso3_;
  int veto_pfhad5absiso10mt100_;
  int veto_pfhad5absiso5mt100_;
  int veto_pfhad5absiso3mt100_;

  int veto_pfhad10reliso20_;
  int veto_pfhad10reliso15_;
  int veto_pfhad10reliso10_;
  int veto_pfhad10reliso20mt100_;
  int veto_pfhad10reliso15mt100_;
  int veto_pfhad10reliso10mt100_;

  int veto_pfhad10absiso10_;
  int veto_pfhad10absiso5_;
  int veto_pfhad10absiso3_;
  int veto_pfhad10absiso10mt100_;
  int veto_pfhad10absiso5mt100_;
  int veto_pfhad10absiso3mt100_;

  // reco lep vars
  float lep1_pt_;
  float lep1_eta_;
  float lep1_phi_;
  float lep1_mt_;

  float tau1_pt_;
  float tau1_eta_;
  float tau1_phi_;
  float tau1_mt_;

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

};

#endif


#ifndef SINGLELEPLOOPER_h
#define SINGLELEPLOOPER_h

// C++ includes
//#include <string>
//#include <vector>

// ROOT includes
#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TH1.h"
#include "TH1D.h"
#include "TH2D.h"
#include "Math/LorentzVector.h"

//MT2
#include "../MT2CORE/mt2tree.h"
#include "../MT2CORE/sigSelections.h"
#include "../MT2CORE/SR.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;
using namespace mt2;
class SingleLepLooper {

 public:

  SingleLepLooper();
  ~SingleLepLooper();

  void SetSignalRegions();
  void loop(TChain* chain, std::string output_name = "test.root");
  void fillHistos(std::map<std::string, TH1*>& h_1d, const std::string& dirname, const std::string& s = "");

 private:

  TFile * outfile_;
  mt2tree t;
  float evtweight_;
  int nJet30Eta3_;
  std::map<std::string, TH1*> h_1d_global;
  SR CRSLNoCut;
  SR CRSLMET50;
  SR CRSLMET50Nj2;
  SR CRSLMET50Nj2Nb0;
  SR CRSLMET50Nj2Nb2;
  SR CRSLMET80MT30Nj2;
  SR CRSLMET80MT30Nj2Nb0;
  SR CRSLMET80MT30Nj2Nb2;
  TH1D* h_nvtx_weights_;
};

#endif


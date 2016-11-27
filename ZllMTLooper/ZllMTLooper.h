#ifndef ZLLMTLOOPER_h
#define ZLLMTLOOPER_h

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
class ZllMTLooper {

 public:

  ZllMTLooper();
  ~ZllMTLooper();

  void SetSignalRegions();
  void loop(TChain* chain, std::string sample, std::string output_dir);
  void fillHistosMT(std::map<std::string, TH1*>& h_1d, const std::string& dirname, const std::string& s = "");

 private:

  TFile * outfile_;
  mt2tree t;
  float evtweight_;
  std::map<std::string, TH1*> h_1d_global;
  SR CRMTBase;
  SR CRMTNj2Nb0;
  SR CRMTHT250;
  SR CRMTHT450;
  TH1D* h_nvtx_weights_;
};

#endif


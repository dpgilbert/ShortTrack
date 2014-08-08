#ifndef MT2LOOPER_h
#define MT2LOOPER_h

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

class MT2Looper {

 public:

  MT2Looper();
  ~MT2Looper();

  void loop(TChain*, std::string = "testSample");



 private:

  TFile * outfile_;

  //  mt2tree mt2t;

};

#endif


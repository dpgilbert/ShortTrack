#ifndef REBALANCELOOPER_h
#define REBALANCELOOPER_h

// C++ includes
//#include <string>
//#include <vector>

// ROOT includes
#include "TROOT.h"
#include "TFile.h"
#include "TChain.h"
#include "TTree.h"
#include "TH1D.h"
#include "Math/LorentzVector.h"

// include a reader class for the templates
#include "../../CMSSW_8_0_5/src/JetResponseTemplates/JetResponseTemplates/interface/JRTreader.h"
#include "../../CMSSW_8_0_5/src/JetResponseTemplates/JetResponseTemplates/interface/JRTTree.h"

//MT2
#include "../../MT2CORE/mt2tree.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;
class RebalanceLooper {

 public:


    RebalanceLooper();
    ~RebalanceLooper();

    void MakeNtuple(const char *);
    void InitNtuple();
    void FillNtuple();
    void CloseNtuple();

    void loop(TChain* chain, std::string output_name = "test.root");
 private:

    TFile * outfile_;
    TTree * outTree_;
    mt2tree t;
    float evtweight_;

    std::vector<float> rebalanceFactors;
    std::vector<int> useJet;
    int status;
    int prescale;
    float new_met;
};

#endif


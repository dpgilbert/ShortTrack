#ifndef RSTEST_h
#define RSTEST_h

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
#include "../../CMSSW_8_0_5/src/JetResponseTemplates/JetResponseTemplates/interface/JRTrebalTree.h"

//MT2
#include "../../MT2CORE/mt2tree.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;
class RStest {

 public:


    RStest();
    ~RStest();

    void MakeNtuple(const char *);
    void InitNtuple();
    void FillNtuple();
    void CloseNtuple();

    void loop(TChain* chain, JRTrebalTree rebalTree, JRTreader reader, std::string output_name = "test.root");
    void computeJetVars(string jettype, float met_pt, float met_phi, float *results);

 private:

    TFile * outfile_;
    TTree * outTree_;
    JRTrebalTree rebalTree;
    mt2tree t;
    float evtweight_;

    bool doRebalance = true;
    float metCut = 0;
    float smearjet_pt[100];
    float smearjet_eta[100];
    float smearjet_phi[100];
    float rebaljet_pt[100];
    float rebaljet_eta[100];
    float rebaljet_phi[100];
    float rebaljet_btagCSV[100];
    int nrebaljet = 0;
    float RSjet_pt[100];
    float RSjet_eta[100];
    float RSjet_phi[100];
    
};

#endif


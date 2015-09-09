#ifndef LEPEFFLOOPER_h
#define LEPEFFLOOPER_h

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
class LepEffLooper {

 public:

  struct genlepcand {
    float pt;
    float eta;
    float phi;
    int pdgId;
    bool isFromTau;
    float minDRrecojet;
    float recojetpt;
  };

  LepEffLooper();
  ~LepEffLooper();

  void SetSignalRegions();
  void loop(TChain* chain, std::string output_name = "test.root");
  void fillHistosGen(std::map<std::string, TH1*>& h_1d, const std::string& dirname, const genlepcand& genlep, const std::string& s = "");
  void fillHistosReco(std::map<std::string, TH1*>& h_1d, const std::string& dirname, const unsigned int& ilep, const std::string& s = "");
  void fillHistosPF(std::map<std::string, TH1*>& h_1d, const std::string& dirname, const unsigned int& ipf, const std::string& s = "");
  void makeEfficiencyHist(std::map<std::string, TH1*>& h_1d, const std::string& dirname, const std::string& num_name, const std::string& denom_name, const std::string& eff_name = "");
  void makeEfficiencyHist(std::map<std::string, TH1*>& h_1d, const std::string& dirname, TH1D* num, TH1D* denom, const std::string& eff_name = "");
  void makeEfficiencyHist2D(std::map<std::string, TH1*>& h_1d, const std::string& dirname, const std::string& num_name, const std::string& denom_name, const std::string& eff_name = "");
  void makeEfficiencyHist2D(std::map<std::string, TH1*>& h_1d, const std::string& dirname, TH2D* num, TH2D* denom, const std::string& eff_name = "");
  float err_eff(float eff,  float denom);
  
 private:

  TFile * outfile_;
  mt2tree t;
  float evtweight_;
  std::map<std::string, TH1*> h_1d_global;
  SR SRBase;
  TH2D* h_eff_el_pt_activity_zjets_;
  TH2D* h_eff_mu_pt_activity_zjets_;
  TH2D* h_eff_el_pt_activity_ttbar_;
  TH2D* h_eff_mu_pt_activity_ttbar_;
  TH2D* h_eff_el_pt_activityra2_zjets_;
  TH2D* h_eff_mu_pt_activityra2_zjets_;
  TH2D* h_eff_el_pt_activityra2_ttbar_;
  TH2D* h_eff_mu_pt_activityra2_ttbar_;
  TH1D* h_eff_el_pt_zjets_;
  TH1D* h_eff_mu_pt_zjets_;
  TH1D* h_eff_el_pt_ttbar_;
  TH1D* h_eff_mu_pt_ttbar_;
};

#endif


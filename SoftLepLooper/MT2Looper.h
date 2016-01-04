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
#include "TH1.h"
#include "TH2.h"
#include "Math/LorentzVector.h"

#include "RooDataSet.h"

//MT2
#include "../MT2CORE/mt2tree.h"
#include "../MT2CORE/sigSelections.h"
#include "../MT2CORE/sigSelectionsLep.h"
#include "../MT2CORE/SR.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;
using namespace mt2;
class MT2Looper {

 public:

  struct lepcand {
    float pt;
    float eta;
    float phi;
    int pdgId;
    float mt;
    bool isPFCand;
  };

  MT2Looper();
  ~MT2Looper();

  void SetSignalRegions();
  void storeHistWithCutValues(std::vector<SR> & srvector, TString SR);
  void loop(TChain* chain, std::string sample, std::string output_dir);
  void fillHistosSRBase();
  void fillHistosInclusive();
  void fillHistosSignalRegion(const std::string& prefix = "", const std::string& suffix = "");
  void fillHistosLepSignalRegions(const std::string& prefix = "", const std::string& suffix = "");
  void fillHistosSoftL(const std::string& prefix = "", const std::string& suffix = "");
  void fillHistosDoubleL(const std::string& prefix = "", const std::string& suffix = "");
  void fillHistosCRSL(const std::string& prefix = "", const std::string& suffix = "");
  void fillHistosCRGJ(const std::string& prefix = "", const std::string& suffix = "");
  void fillHistosCRDY(const std::string& prefix = "", const std::string& suffix = "");
  void fillHistosCRRL(const std::string& prefix = "", const std::string& suffix = "");
  void fillHistosCRQCD(const std::string& prefix = "", const std::string& suffix = "");
  void fillHistos(std::map<std::string, TH1*>& h_1d, int n_mt2bins, float* mt2bins,
		  const std::string& dir = "", const std::string& suffix = "");
  void fillHistosSingleSoftLepton(std::map<std::string, TH1*>& h_1d, int n_mt2bins, float* mt2bins,
			      const std::string& dir = "", const std::string& suffix = ""); 
  void fillHistosDoubleLepton(std::map<std::string, TH1*>& h_1d, int n_mt2bins, float* mt2bins,
			      const std::string& dir = "", const std::string& suffix = ""); 
  void fillHistosSingleLepton(std::map<std::string, TH1*>& h_1d, int n_mt2bins, float* mt2bins,
			      const std::string& dir = "", const std::string& suffix = ""); 
  void fillHistosGammaJets(std::map<std::string, TH1*>& h_1d, std::map<std::string, RooDataSet*>& datasets, int n_mt2bins, float* mt2bins, 
			      const std::string& dir = "", const std::string& suffix = ""); 
  void fillHistosRemovedLepton(std::map<std::string, TH1*>& h_1d, int n_mt2bins, float* mt2bins, 
			      const std::string& dir = "", const std::string& suffix = ""); 
  void fillHistosDY(std::map<std::string, TH1*>& h_1d, int n_mt2bins, float* mt2bins, 
			      const std::string& dir = "", const std::string& suffix = ""); 
  void fillHistosQCD(std::map<std::string, TH1*>& h_1d, int n_mt2bins, float* mt2bins,
			      const std::string& dir = "", const std::string& suffix = "");
  void fillLepUncSR();
  void fillLepCorSRfastsim();
  
 private:

  TFile * outfile_;
  mt2tree t;
  float evtweight_;
  int nlepveto_;
  float leppt_;
  float mt_;

  //soft lep variables
  int nUniqueLep_;
  float softleppt_;
  float softlepeta_;
  float softlepphi_;
  float softlepM_;
  int softlepId_;
  bool softlepElId_;
  float softlepmt_;
  float softlepmt2_;
  float softlepDPhiMin_;

  //2-lep softLep CR variables
  float lep1pt_;
  float lep1eta_;
  float lep1phi_;
  float lep1M_;
  float lep2pt_;
  float lep2eta_;
  float lep2phi_;
  float lep2M_;
  float dilepmll_;
  
  int nJet30Eta3_;
  bool isSignal_;
  bool passMonojetId_;
  std::map<std::string, TH1*> h_1d_global;
  std::vector<SR> SRVec;
  std::vector<SR> SRVecMonojet;
  std::vector<SR> InclusiveRegions;
  std::vector<SR> InclusiveRegionsMonojet;
  std::vector<SR> SRVecLep;
  SR SRBase;
  SR SRBaseIncl;
  SR SRBaseMonojet;
  SR SRBaseMonojet0B;
  SR SRBaseMonojet1B;
  SR SRNoCut;
  SR CRSL_WJets;
  SR CRSL_TTbar;
  TH1D* h_nvtx_weights_;
  TH2D* h_sig_nevents_;
  TH2D* h_sig_avgweight_btagsf_;
  TH2D* h_sig_avgweight_btagsf_heavy_UP_;
  TH2D* h_sig_avgweight_btagsf_light_UP_;
  TH2D* h_sig_avgweight_btagsf_heavy_DN_;
  TH2D* h_sig_avgweight_btagsf_light_DN_;
  TH2D* h_sig_avgweight_isr_;
  float cor_lepeff_sr_;
  float unc_lepeff_sr_;
  
};

#endif


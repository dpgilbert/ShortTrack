#ifndef SmearLooper_h
#define SmearLooper_h

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

#include "RooDataSet.h"

//MT2
#include "../../MT2CORE/mt2tree.h"
#include "../../MT2CORE/RebalSmear/sigSelections_RebalSmear.h"
#include "../../MT2CORE/RebalSmear/SR_RebalSmear.h"

#include "../../MT2CORE/RebalSmear/JRTreader.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;
using namespace mt2;
class SmearLooper {

 public:

  struct lepcand {
    float pt;
    float eta;
    float phi;
    int pdgId;
    float mt;
    bool isPFCand;
  };

  SmearLooper();
  ~SmearLooper();

  void SetSignalRegions();
  void loop(TChain* chain, std::string output_name = "test.root");
  void fillHistosSRBase();
  void fillHistosInclusive();
  void fillHistosSignalRegion(const std::string& prefix = "", const std::string& suffix = "");
  void fillHistosCRSL(const std::string& prefix = "", const std::string& suffix = "");
  void fillHistosCRGJ(const std::string& prefix = "", const std::string& suffix = "");
  void fillHistosCRDY(const std::string& prefix = "", const std::string& suffix = "");
  void fillHistosCRRL(const std::string& prefix = "", const std::string& suffix = "");
  void fillHistosCRRSInvertDPhi(const std::string& prefix = "crRSInvertDPhi", const std::string& suffix = "");
  void fillHistosCRRSMT2SideBand(const std::string& prefix = "crRSMT2SideBand", const std::string& suffix = "");
  void fillHistosCRRSDPhiMT2(const std::string& prefix = "crRSDPhiMT2", const std::string& suffix = "");
  void fillHistos(std::map<std::string, TH1*>& h_1d, int n_mt2bins, float* mt2bins,
		  const std::string& dir = "", const std::string& suffix = ""); 
  void fillHistosSingleLepton(std::map<std::string, TH1*>& h_1d, int n_mt2bins, float* mt2bins,
			      const std::string& dir = "", const std::string& suffix = ""); 
  void fillHistosGammaJets(std::map<std::string, TH1*>& h_1d, std::map<std::string, RooDataSet*>& datasets, int n_mt2bins, float* mt2bins, 
			      const std::string& dir = "", const std::string& suffix = ""); 
  void fillHistosRemovedLepton(std::map<std::string, TH1*>& h_1d, int n_mt2bins, float* mt2bins, 
			      const std::string& dir = "", const std::string& suffix = ""); 
  void fillHistosDY(std::map<std::string, TH1*>& h_1d, int n_mt2bins, float* mt2bins, 
			      const std::string& dir = "", const std::string& suffix = ""); 
  void setupResponseHists();
  std::vector<float> GetResponseVector(float, float);
  void plotRS(std::map<std::string, TH1*>& inmap, std::map<std::string, TH1*>& outmap, std::string outdir);
  void resetHistmap(std::map<std::string, TH1*>& inmap, std::string outdir);
 private:

  JRTreader reader;

  TFile * outfile_;
  mt2tree t;
  float evtweight_;
  int nlepveto_;
  float leppt_;
  float mt_;
  int nJet30Eta3_;
  bool isSignal_;
  std::map<std::string, TH1*> h_1d_global;
  std::vector<SRRS> SRVec;
  std::vector<SRRS> InclusiveRegions;
  SRRS SRBase;
  SRRS SRNoCut;
  SRRS SRJustHT1;
  SRRS SRJustHT2;
  SRRS CRSL_WJets;
  SRRS CRSL_TTbar;
  TH1D* h_nvtx_weights_;
  std::map<std::string, float> RS_vars_; 

  std::vector<SRRS> SRVec_temp;
  std::vector<SRRS> InclusiveRegions_temp;
  SRRS SRBase_temp;
  SRRS SRNoCut_temp;
  SRRS SRJustHT1_temp;
  SRRS SRJustHT2_temp;
};

#endif


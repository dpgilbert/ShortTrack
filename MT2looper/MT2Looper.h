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
#include "TH1D.h"
#include "Math/LorentzVector.h"

//MT2
#include "../MT2CORE/mt2tree.h"
#include "../MT2CORE/sigSelections.h"

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

  void loop(TChain* chain, std::string output_name = "test.root");
  void fillHistosSignalRegion(std::map<std::string, TH1D*>& h_1d, 
			      const SignalRegionJets::value_type& signal_region = SignalRegionJets::nocut, 
			      const SignalRegionHtMet::value_type& signal_region_type = SignalRegionHtMet::nocut,
			      const std::string& dirname = "", const std::string& suffix = "");
  void fillHistosCRSL(std::map<std::string, TH1D*>& h_1d, 
		      const SignalRegionJets::value_type& signal_region = SignalRegionJets::nocut, 
		      const SignalRegionHtMet::value_type& signal_region_type = SignalRegionHtMet::nocut,
		      const std::string& dirname = "", const std::string& suffix = "");
  void fillHistosCRGJ(std::map<std::string, TH1D*>& h_1d, 
		      const SignalRegionJets::value_type& signal_region = SignalRegionJets::nocut, 
		      const SignalRegionHtMet::value_type& signal_region_type = SignalRegionHtMet::nocut,
		      const std::string& dirname = "", const std::string& suffix = "", const int jetIdx0 = 0, const int jetIdx1 = 1);
  void fillHistosCRDY(std::map<std::string, TH1D*>& h_1d, 
		      const SignalRegionJets::value_type& signal_region = SignalRegionJets::nocut, 
		      const SignalRegionHtMet::value_type& signal_region_type = SignalRegionHtMet::nocut,
		      const std::string& dirname = "", const std::string& suffix = "");
  void fillHistos(std::map<std::string, TH1D*>& h_1d, 
		  const std::string& dir = "", const std::string& suffix = ""); 
  void fillHistosSingleLepton(std::map<std::string, TH1D*>& h_1d, 
			      const std::string& dir = "", const std::string& suffix = ""); 
  void fillHistosGammaJets(std::map<std::string, TH1D*>& h_1d, 
			      const std::string& dir = "", const std::string& suffix = ""); 
  void fillHistosDY(std::map<std::string, TH1D*>& h_1d, 
			      const std::string& dir = "", const std::string& suffix = ""); 
 private:

  TFile * outfile_;
  mt2tree t;
  float evtweight_;
  int nlepveto_;
  float leppt_;
  float mt_;
  std::map<std::string, TH1D*> h_1d_global;
};

#endif


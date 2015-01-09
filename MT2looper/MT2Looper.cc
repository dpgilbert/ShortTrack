// C++
#include <iostream>
#include <vector>
#include <set>

// ROOT
#include "TDirectory.h"
#include "TTreeCache.h"
#include "Math/VectorUtil.h"
#include "TVector2.h"
#include "TBenchmark.h"

// CMS2
//#include "../MT2CORE/CMS2.h"
#include "../MT2CORE/tools.h"
//#include "../MT2CORE/selections.h"
//#include "../MT2CORE/hemJet.h"
//#include "../MT2CORE/MT2/MT2.h"
//#include "../MT2CORE/IsoTrackVeto.h"

// header
#include "MT2Looper.h"

//MT2
#include "../MT2CORE/Plotting/PlotUtilities.h"


using namespace std;
using namespace mt2;

class mt2tree;

// mt2 binning for results
const int n_mt2bins = 5;
const float mt2bins[n_mt2bins+1] = {200., 300., 400., 600., 1000., 1500.};
const std::string mt2binsname[n_mt2bins+1] = {"200", "300", "400", "600", "1000", "1500"};

MT2Looper::MT2Looper(){
  // set some stuff here
}
MT2Looper::~MT2Looper(){

};

void MT2Looper::loop(TChain* chain, std::string output_name){

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  gROOT->cd();
  cout << "[MT2Looper::loop] creating output file: " << output_name << endl;

  outfile_ = new TFile(output_name.c_str(),"RECREATE") ; 

  cout << "[MT2Looper::loop] setting up histos" << endl;

  std::vector< std::string > regions;
  std::vector< SignalRegionJets::value_type > srJets;
  std::vector< SignalRegionHtMet::value_type > srHtMet;
  regions.push_back("1H");  srJets.push_back(SignalRegionJets::sr1);  srHtMet.push_back(SignalRegionHtMet::h_ht);
  regions.push_back("2H");  srJets.push_back(SignalRegionJets::sr2);  srHtMet.push_back(SignalRegionHtMet::h_ht);
  regions.push_back("3H");  srJets.push_back(SignalRegionJets::sr3);  srHtMet.push_back(SignalRegionHtMet::h_ht);
  regions.push_back("4H");  srJets.push_back(SignalRegionJets::sr4);  srHtMet.push_back(SignalRegionHtMet::h_ht);
  regions.push_back("5H");  srJets.push_back(SignalRegionJets::sr5);  srHtMet.push_back(SignalRegionHtMet::h_ht);
  regions.push_back("6H");  srJets.push_back(SignalRegionJets::sr6);  srHtMet.push_back(SignalRegionHtMet::h_ht);
  regions.push_back("7H");  srJets.push_back(SignalRegionJets::sr7);  srHtMet.push_back(SignalRegionHtMet::h_ht);
  regions.push_back("8H");  srJets.push_back(SignalRegionJets::sr8);  srHtMet.push_back(SignalRegionHtMet::h_ht);
  regions.push_back("9H");  srJets.push_back(SignalRegionJets::sr9);  srHtMet.push_back(SignalRegionHtMet::h_ht);
  regions.push_back("10H"); srJets.push_back(SignalRegionJets::sr10); srHtMet.push_back(SignalRegionHtMet::h_ht);
  regions.push_back("1M");  srJets.push_back(SignalRegionJets::sr1);  srHtMet.push_back(SignalRegionHtMet::m_ht);
  regions.push_back("2M");  srJets.push_back(SignalRegionJets::sr2);  srHtMet.push_back(SignalRegionHtMet::m_ht);
  regions.push_back("3M");  srJets.push_back(SignalRegionJets::sr3);  srHtMet.push_back(SignalRegionHtMet::m_ht);
  regions.push_back("4M");  srJets.push_back(SignalRegionJets::sr4);  srHtMet.push_back(SignalRegionHtMet::m_ht);
  regions.push_back("5M");  srJets.push_back(SignalRegionJets::sr5);  srHtMet.push_back(SignalRegionHtMet::m_ht);
  regions.push_back("6M");  srJets.push_back(SignalRegionJets::sr6);  srHtMet.push_back(SignalRegionHtMet::m_ht);
  regions.push_back("7M");  srJets.push_back(SignalRegionJets::sr7);  srHtMet.push_back(SignalRegionHtMet::m_ht);
  regions.push_back("8M");  srJets.push_back(SignalRegionJets::sr8);  srHtMet.push_back(SignalRegionHtMet::m_ht);
  regions.push_back("9M");  srJets.push_back(SignalRegionJets::sr9);  srHtMet.push_back(SignalRegionHtMet::m_ht);
  regions.push_back("10M"); srJets.push_back(SignalRegionJets::sr10); srHtMet.push_back(SignalRegionHtMet::m_ht);
  regions.push_back("1L");  srJets.push_back(SignalRegionJets::sr1);  srHtMet.push_back(SignalRegionHtMet::l_ht);
  regions.push_back("2L");  srJets.push_back(SignalRegionJets::sr2);  srHtMet.push_back(SignalRegionHtMet::l_ht);
  regions.push_back("3L");  srJets.push_back(SignalRegionJets::sr3);  srHtMet.push_back(SignalRegionHtMet::l_ht);
  regions.push_back("4L");  srJets.push_back(SignalRegionJets::sr4);  srHtMet.push_back(SignalRegionHtMet::l_ht);
  regions.push_back("5L");  srJets.push_back(SignalRegionJets::sr5);  srHtMet.push_back(SignalRegionHtMet::l_ht);
  regions.push_back("6L");  srJets.push_back(SignalRegionJets::sr6);  srHtMet.push_back(SignalRegionHtMet::l_ht);
  regions.push_back("7L");  srJets.push_back(SignalRegionJets::sr7);  srHtMet.push_back(SignalRegionHtMet::l_ht);
  regions.push_back("8L");  srJets.push_back(SignalRegionJets::sr8);  srHtMet.push_back(SignalRegionHtMet::l_ht);
  regions.push_back("9L");  srJets.push_back(SignalRegionJets::sr9);  srHtMet.push_back(SignalRegionHtMet::l_ht);
  regions.push_back("10L"); srJets.push_back(SignalRegionJets::sr10); srHtMet.push_back(SignalRegionHtMet::l_ht);

  std::map<std::string, TH1D*> h_1d_nocut;
  std::map<std::string, TH1D*> h_1d_srbase;
  std::map<std::string, TH1D*> h_1d_srH;
  std::map<std::string, TH1D*> h_1d_srM;
  std::map<std::string, TH1D*> h_1d_srL;

  std::vector< std::map<std::string, TH1D*> > h_1d_sr_v;
  std::map<std::string, TH1D*> h_1d_sr1H;  h_1d_sr_v.push_back(h_1d_sr1H);  
  std::map<std::string, TH1D*> h_1d_sr2H;  h_1d_sr_v.push_back(h_1d_sr2H);  
  std::map<std::string, TH1D*> h_1d_sr3H;  h_1d_sr_v.push_back(h_1d_sr3H);  
  std::map<std::string, TH1D*> h_1d_sr4H;  h_1d_sr_v.push_back(h_1d_sr4H);  
  std::map<std::string, TH1D*> h_1d_sr5H;  h_1d_sr_v.push_back(h_1d_sr5H);  
  std::map<std::string, TH1D*> h_1d_sr6H;  h_1d_sr_v.push_back(h_1d_sr6H);  
  std::map<std::string, TH1D*> h_1d_sr7H;  h_1d_sr_v.push_back(h_1d_sr7H);  
  std::map<std::string, TH1D*> h_1d_sr8H;  h_1d_sr_v.push_back(h_1d_sr8H);  
  std::map<std::string, TH1D*> h_1d_sr9H;  h_1d_sr_v.push_back(h_1d_sr9H);  
  std::map<std::string, TH1D*> h_1d_sr10H; h_1d_sr_v.push_back(h_1d_sr10H); 
  std::map<std::string, TH1D*> h_1d_sr1M;  h_1d_sr_v.push_back(h_1d_sr1M);  
  std::map<std::string, TH1D*> h_1d_sr2M;  h_1d_sr_v.push_back(h_1d_sr2M);  
  std::map<std::string, TH1D*> h_1d_sr3M;  h_1d_sr_v.push_back(h_1d_sr3M);  
  std::map<std::string, TH1D*> h_1d_sr4M;  h_1d_sr_v.push_back(h_1d_sr4M);  
  std::map<std::string, TH1D*> h_1d_sr5M;  h_1d_sr_v.push_back(h_1d_sr5M);  
  std::map<std::string, TH1D*> h_1d_sr6M;  h_1d_sr_v.push_back(h_1d_sr6M);  
  std::map<std::string, TH1D*> h_1d_sr7M;  h_1d_sr_v.push_back(h_1d_sr7M);  
  std::map<std::string, TH1D*> h_1d_sr8M;  h_1d_sr_v.push_back(h_1d_sr8M);  
  std::map<std::string, TH1D*> h_1d_sr9M;  h_1d_sr_v.push_back(h_1d_sr9M);  
  std::map<std::string, TH1D*> h_1d_sr10M; h_1d_sr_v.push_back(h_1d_sr10M); 
  std::map<std::string, TH1D*> h_1d_sr1L;  h_1d_sr_v.push_back(h_1d_sr1L);  
  std::map<std::string, TH1D*> h_1d_sr2L;  h_1d_sr_v.push_back(h_1d_sr2L);  
  std::map<std::string, TH1D*> h_1d_sr3L;  h_1d_sr_v.push_back(h_1d_sr3L);  
  std::map<std::string, TH1D*> h_1d_sr4L;  h_1d_sr_v.push_back(h_1d_sr4L);  
  std::map<std::string, TH1D*> h_1d_sr5L;  h_1d_sr_v.push_back(h_1d_sr5L);  
  std::map<std::string, TH1D*> h_1d_sr6L;  h_1d_sr_v.push_back(h_1d_sr6L);  
  std::map<std::string, TH1D*> h_1d_sr7L;  h_1d_sr_v.push_back(h_1d_sr7L);  
  std::map<std::string, TH1D*> h_1d_sr8L;  h_1d_sr_v.push_back(h_1d_sr8L);  
  std::map<std::string, TH1D*> h_1d_sr9L;  h_1d_sr_v.push_back(h_1d_sr9L);  
  std::map<std::string, TH1D*> h_1d_sr10L; h_1d_sr_v.push_back(h_1d_sr10L); 

  std::vector< std::map<std::string, TH1D*> > h_1d_crgj_v;
  std::map<std::string, TH1D*> h_1d_crgj1H;  h_1d_crgj_v.push_back(h_1d_crgj1H);  
  std::map<std::string, TH1D*> h_1d_crgj2H;  h_1d_crgj_v.push_back(h_1d_crgj2H);  
  std::map<std::string, TH1D*> h_1d_crgj3H;  h_1d_crgj_v.push_back(h_1d_crgj3H);  
  std::map<std::string, TH1D*> h_1d_crgj4H;  h_1d_crgj_v.push_back(h_1d_crgj4H);  
  std::map<std::string, TH1D*> h_1d_crgj5H;  h_1d_crgj_v.push_back(h_1d_crgj5H);  
  std::map<std::string, TH1D*> h_1d_crgj6H;  h_1d_crgj_v.push_back(h_1d_crgj6H);  
  std::map<std::string, TH1D*> h_1d_crgj7H;  h_1d_crgj_v.push_back(h_1d_crgj7H);  
  std::map<std::string, TH1D*> h_1d_crgj8H;  h_1d_crgj_v.push_back(h_1d_crgj8H);  
  std::map<std::string, TH1D*> h_1d_crgj9H;  h_1d_crgj_v.push_back(h_1d_crgj9H);  
  std::map<std::string, TH1D*> h_1d_crgj10H; h_1d_crgj_v.push_back(h_1d_crgj10H); 
  std::map<std::string, TH1D*> h_1d_crgj1M;  h_1d_crgj_v.push_back(h_1d_crgj1M);  
  std::map<std::string, TH1D*> h_1d_crgj2M;  h_1d_crgj_v.push_back(h_1d_crgj2M);  
  std::map<std::string, TH1D*> h_1d_crgj3M;  h_1d_crgj_v.push_back(h_1d_crgj3M);  
  std::map<std::string, TH1D*> h_1d_crgj4M;  h_1d_crgj_v.push_back(h_1d_crgj4M);  
  std::map<std::string, TH1D*> h_1d_crgj5M;  h_1d_crgj_v.push_back(h_1d_crgj5M);  
  std::map<std::string, TH1D*> h_1d_crgj6M;  h_1d_crgj_v.push_back(h_1d_crgj6M);  
  std::map<std::string, TH1D*> h_1d_crgj7M;  h_1d_crgj_v.push_back(h_1d_crgj7M);  
  std::map<std::string, TH1D*> h_1d_crgj8M;  h_1d_crgj_v.push_back(h_1d_crgj8M);  
  std::map<std::string, TH1D*> h_1d_crgj9M;  h_1d_crgj_v.push_back(h_1d_crgj9M);  
  std::map<std::string, TH1D*> h_1d_crgj10M; h_1d_crgj_v.push_back(h_1d_crgj10M); 
  std::map<std::string, TH1D*> h_1d_crgj1L;  h_1d_crgj_v.push_back(h_1d_crgj1L);  
  std::map<std::string, TH1D*> h_1d_crgj2L;  h_1d_crgj_v.push_back(h_1d_crgj2L);  
  std::map<std::string, TH1D*> h_1d_crgj3L;  h_1d_crgj_v.push_back(h_1d_crgj3L);  
  std::map<std::string, TH1D*> h_1d_crgj4L;  h_1d_crgj_v.push_back(h_1d_crgj4L);  
  std::map<std::string, TH1D*> h_1d_crgj5L;  h_1d_crgj_v.push_back(h_1d_crgj5L);  
  std::map<std::string, TH1D*> h_1d_crgj6L;  h_1d_crgj_v.push_back(h_1d_crgj6L);  
  std::map<std::string, TH1D*> h_1d_crgj7L;  h_1d_crgj_v.push_back(h_1d_crgj7L);  
  std::map<std::string, TH1D*> h_1d_crgj8L;  h_1d_crgj_v.push_back(h_1d_crgj8L);  
  std::map<std::string, TH1D*> h_1d_crgj9L;  h_1d_crgj_v.push_back(h_1d_crgj9L);  
  std::map<std::string, TH1D*> h_1d_crgj10L; h_1d_crgj_v.push_back(h_1d_crgj10L); 

  std::map<std::string, TH1D*> h_1d_crgjbase;

  std::vector< std::map<std::string, TH1D*> > h_1d_crdy_v;
  std::map<std::string, TH1D*> h_1d_crdy1H;  h_1d_crdy_v.push_back(h_1d_crdy1H);  
  std::map<std::string, TH1D*> h_1d_crdy2H;  h_1d_crdy_v.push_back(h_1d_crdy2H);  
  std::map<std::string, TH1D*> h_1d_crdy3H;  h_1d_crdy_v.push_back(h_1d_crdy3H);  
  std::map<std::string, TH1D*> h_1d_crdy4H;  h_1d_crdy_v.push_back(h_1d_crdy4H);  
  std::map<std::string, TH1D*> h_1d_crdy5H;  h_1d_crdy_v.push_back(h_1d_crdy5H);  
  std::map<std::string, TH1D*> h_1d_crdy6H;  h_1d_crdy_v.push_back(h_1d_crdy6H);  
  std::map<std::string, TH1D*> h_1d_crdy7H;  h_1d_crdy_v.push_back(h_1d_crdy7H);  
  std::map<std::string, TH1D*> h_1d_crdy8H;  h_1d_crdy_v.push_back(h_1d_crdy8H);  
  std::map<std::string, TH1D*> h_1d_crdy9H;  h_1d_crdy_v.push_back(h_1d_crdy9H);  
  std::map<std::string, TH1D*> h_1d_crdy10H; h_1d_crdy_v.push_back(h_1d_crdy10H); 
  std::map<std::string, TH1D*> h_1d_crdy1M;  h_1d_crdy_v.push_back(h_1d_crdy1M);  
  std::map<std::string, TH1D*> h_1d_crdy2M;  h_1d_crdy_v.push_back(h_1d_crdy2M);  
  std::map<std::string, TH1D*> h_1d_crdy3M;  h_1d_crdy_v.push_back(h_1d_crdy3M);  
  std::map<std::string, TH1D*> h_1d_crdy4M;  h_1d_crdy_v.push_back(h_1d_crdy4M);  
  std::map<std::string, TH1D*> h_1d_crdy5M;  h_1d_crdy_v.push_back(h_1d_crdy5M);  
  std::map<std::string, TH1D*> h_1d_crdy6M;  h_1d_crdy_v.push_back(h_1d_crdy6M);  
  std::map<std::string, TH1D*> h_1d_crdy7M;  h_1d_crdy_v.push_back(h_1d_crdy7M);  
  std::map<std::string, TH1D*> h_1d_crdy8M;  h_1d_crdy_v.push_back(h_1d_crdy8M);  
  std::map<std::string, TH1D*> h_1d_crdy9M;  h_1d_crdy_v.push_back(h_1d_crdy9M);  
  std::map<std::string, TH1D*> h_1d_crdy10M; h_1d_crdy_v.push_back(h_1d_crdy10M); 
  std::map<std::string, TH1D*> h_1d_crdy1L;  h_1d_crdy_v.push_back(h_1d_crdy1L);  
  std::map<std::string, TH1D*> h_1d_crdy2L;  h_1d_crdy_v.push_back(h_1d_crdy2L);  
  std::map<std::string, TH1D*> h_1d_crdy3L;  h_1d_crdy_v.push_back(h_1d_crdy3L);  
  std::map<std::string, TH1D*> h_1d_crdy4L;  h_1d_crdy_v.push_back(h_1d_crdy4L);  
  std::map<std::string, TH1D*> h_1d_crdy5L;  h_1d_crdy_v.push_back(h_1d_crdy5L);  
  std::map<std::string, TH1D*> h_1d_crdy6L;  h_1d_crdy_v.push_back(h_1d_crdy6L);  
  std::map<std::string, TH1D*> h_1d_crdy7L;  h_1d_crdy_v.push_back(h_1d_crdy7L);  
  std::map<std::string, TH1D*> h_1d_crdy8L;  h_1d_crdy_v.push_back(h_1d_crdy8L);  
  std::map<std::string, TH1D*> h_1d_crdy9L;  h_1d_crdy_v.push_back(h_1d_crdy9L);  
  std::map<std::string, TH1D*> h_1d_crdy10L; h_1d_crdy_v.push_back(h_1d_crdy10L); 

  std::map<std::string, TH1D*> h_1d_crslbase;
  std::map<std::string, TH1D*> h_1d_crslw;
  std::map<std::string, TH1D*> h_1d_crsltt;

  std::vector< std::map<std::string, TH1D*> > h_1d_crsl_v;
  std::map<std::string, TH1D*> h_1d_crsl1H;  h_1d_crsl_v.push_back(h_1d_crsl1H);  
  std::map<std::string, TH1D*> h_1d_crsl2H;  h_1d_crsl_v.push_back(h_1d_crsl2H);  
  std::map<std::string, TH1D*> h_1d_crsl3H;  h_1d_crsl_v.push_back(h_1d_crsl3H);  
  std::map<std::string, TH1D*> h_1d_crsl4H;  h_1d_crsl_v.push_back(h_1d_crsl4H);  
  std::map<std::string, TH1D*> h_1d_crsl5H;  h_1d_crsl_v.push_back(h_1d_crsl5H);  
  std::map<std::string, TH1D*> h_1d_crsl6H;  h_1d_crsl_v.push_back(h_1d_crsl6H);  
  std::map<std::string, TH1D*> h_1d_crsl7H;  h_1d_crsl_v.push_back(h_1d_crsl7H);  
  std::map<std::string, TH1D*> h_1d_crsl8H;  h_1d_crsl_v.push_back(h_1d_crsl8H);  
  std::map<std::string, TH1D*> h_1d_crsl9H;  h_1d_crsl_v.push_back(h_1d_crsl9H);  
  std::map<std::string, TH1D*> h_1d_crsl10H; h_1d_crsl_v.push_back(h_1d_crsl10H); 
  std::map<std::string, TH1D*> h_1d_crsl1M;  h_1d_crsl_v.push_back(h_1d_crsl1M);  
  std::map<std::string, TH1D*> h_1d_crsl2M;  h_1d_crsl_v.push_back(h_1d_crsl2M);  
  std::map<std::string, TH1D*> h_1d_crsl3M;  h_1d_crsl_v.push_back(h_1d_crsl3M);  
  std::map<std::string, TH1D*> h_1d_crsl4M;  h_1d_crsl_v.push_back(h_1d_crsl4M);  
  std::map<std::string, TH1D*> h_1d_crsl5M;  h_1d_crsl_v.push_back(h_1d_crsl5M);  
  std::map<std::string, TH1D*> h_1d_crsl6M;  h_1d_crsl_v.push_back(h_1d_crsl6M);  
  std::map<std::string, TH1D*> h_1d_crsl7M;  h_1d_crsl_v.push_back(h_1d_crsl7M);  
  std::map<std::string, TH1D*> h_1d_crsl8M;  h_1d_crsl_v.push_back(h_1d_crsl8M);  
  std::map<std::string, TH1D*> h_1d_crsl9M;  h_1d_crsl_v.push_back(h_1d_crsl9M);  
  std::map<std::string, TH1D*> h_1d_crsl10M; h_1d_crsl_v.push_back(h_1d_crsl10M); 
  std::map<std::string, TH1D*> h_1d_crsl1L;  h_1d_crsl_v.push_back(h_1d_crsl1L);  
  std::map<std::string, TH1D*> h_1d_crsl2L;  h_1d_crsl_v.push_back(h_1d_crsl2L);  
  std::map<std::string, TH1D*> h_1d_crsl3L;  h_1d_crsl_v.push_back(h_1d_crsl3L);  
  std::map<std::string, TH1D*> h_1d_crsl4L;  h_1d_crsl_v.push_back(h_1d_crsl4L);  
  std::map<std::string, TH1D*> h_1d_crsl5L;  h_1d_crsl_v.push_back(h_1d_crsl5L);  
  std::map<std::string, TH1D*> h_1d_crsl6L;  h_1d_crsl_v.push_back(h_1d_crsl6L);  
  std::map<std::string, TH1D*> h_1d_crsl7L;  h_1d_crsl_v.push_back(h_1d_crsl7L);  
  std::map<std::string, TH1D*> h_1d_crsl8L;  h_1d_crsl_v.push_back(h_1d_crsl8L);  
  std::map<std::string, TH1D*> h_1d_crsl9L;  h_1d_crsl_v.push_back(h_1d_crsl9L);  
  std::map<std::string, TH1D*> h_1d_crsl10L; h_1d_crsl_v.push_back(h_1d_crsl10L); 

  std::map<std::string, TH1D*> h_1d_crslmubase;
  std::map<std::string, TH1D*> h_1d_crslmuw;
  std::map<std::string, TH1D*> h_1d_crslmutt;

  std::vector< std::map<std::string, TH1D*> > h_1d_crslmu_v;
  std::map<std::string, TH1D*> h_1d_crslmu1H;  h_1d_crslmu_v.push_back(h_1d_crslmu1H);  
  std::map<std::string, TH1D*> h_1d_crslmu2H;  h_1d_crslmu_v.push_back(h_1d_crslmu2H);  
  std::map<std::string, TH1D*> h_1d_crslmu3H;  h_1d_crslmu_v.push_back(h_1d_crslmu3H);  
  std::map<std::string, TH1D*> h_1d_crslmu4H;  h_1d_crslmu_v.push_back(h_1d_crslmu4H);  
  std::map<std::string, TH1D*> h_1d_crslmu5H;  h_1d_crslmu_v.push_back(h_1d_crslmu5H);  
  std::map<std::string, TH1D*> h_1d_crslmu6H;  h_1d_crslmu_v.push_back(h_1d_crslmu6H);  
  std::map<std::string, TH1D*> h_1d_crslmu7H;  h_1d_crslmu_v.push_back(h_1d_crslmu7H);  
  std::map<std::string, TH1D*> h_1d_crslmu8H;  h_1d_crslmu_v.push_back(h_1d_crslmu8H);  
  std::map<std::string, TH1D*> h_1d_crslmu9H;  h_1d_crslmu_v.push_back(h_1d_crslmu9H);  
  std::map<std::string, TH1D*> h_1d_crslmu10H; h_1d_crslmu_v.push_back(h_1d_crslmu10H); 
  std::map<std::string, TH1D*> h_1d_crslmu1M;  h_1d_crslmu_v.push_back(h_1d_crslmu1M);  
  std::map<std::string, TH1D*> h_1d_crslmu2M;  h_1d_crslmu_v.push_back(h_1d_crslmu2M);  
  std::map<std::string, TH1D*> h_1d_crslmu3M;  h_1d_crslmu_v.push_back(h_1d_crslmu3M);  
  std::map<std::string, TH1D*> h_1d_crslmu4M;  h_1d_crslmu_v.push_back(h_1d_crslmu4M);  
  std::map<std::string, TH1D*> h_1d_crslmu5M;  h_1d_crslmu_v.push_back(h_1d_crslmu5M);  
  std::map<std::string, TH1D*> h_1d_crslmu6M;  h_1d_crslmu_v.push_back(h_1d_crslmu6M);  
  std::map<std::string, TH1D*> h_1d_crslmu7M;  h_1d_crslmu_v.push_back(h_1d_crslmu7M);  
  std::map<std::string, TH1D*> h_1d_crslmu8M;  h_1d_crslmu_v.push_back(h_1d_crslmu8M);  
  std::map<std::string, TH1D*> h_1d_crslmu9M;  h_1d_crslmu_v.push_back(h_1d_crslmu9M);  
  std::map<std::string, TH1D*> h_1d_crslmu10M; h_1d_crslmu_v.push_back(h_1d_crslmu10M); 
  std::map<std::string, TH1D*> h_1d_crslmu1L;  h_1d_crslmu_v.push_back(h_1d_crslmu1L);  
  std::map<std::string, TH1D*> h_1d_crslmu2L;  h_1d_crslmu_v.push_back(h_1d_crslmu2L);  
  std::map<std::string, TH1D*> h_1d_crslmu3L;  h_1d_crslmu_v.push_back(h_1d_crslmu3L);  
  std::map<std::string, TH1D*> h_1d_crslmu4L;  h_1d_crslmu_v.push_back(h_1d_crslmu4L);  
  std::map<std::string, TH1D*> h_1d_crslmu5L;  h_1d_crslmu_v.push_back(h_1d_crslmu5L);  
  std::map<std::string, TH1D*> h_1d_crslmu6L;  h_1d_crslmu_v.push_back(h_1d_crslmu6L);  
  std::map<std::string, TH1D*> h_1d_crslmu7L;  h_1d_crslmu_v.push_back(h_1d_crslmu7L);  
  std::map<std::string, TH1D*> h_1d_crslmu8L;  h_1d_crslmu_v.push_back(h_1d_crslmu8L);  
  std::map<std::string, TH1D*> h_1d_crslmu9L;  h_1d_crslmu_v.push_back(h_1d_crslmu9L);  
  std::map<std::string, TH1D*> h_1d_crslmu10L; h_1d_crslmu_v.push_back(h_1d_crslmu10L); 

  std::map<std::string, TH1D*> h_1d_crslelbase;
  std::map<std::string, TH1D*> h_1d_crslelw;
  std::map<std::string, TH1D*> h_1d_crsleltt;

  std::vector< std::map<std::string, TH1D*> > h_1d_crslel_v;
  std::map<std::string, TH1D*> h_1d_crslel1H;  h_1d_crslel_v.push_back(h_1d_crslel1H);  
  std::map<std::string, TH1D*> h_1d_crslel2H;  h_1d_crslel_v.push_back(h_1d_crslel2H);  
  std::map<std::string, TH1D*> h_1d_crslel3H;  h_1d_crslel_v.push_back(h_1d_crslel3H);  
  std::map<std::string, TH1D*> h_1d_crslel4H;  h_1d_crslel_v.push_back(h_1d_crslel4H);  
  std::map<std::string, TH1D*> h_1d_crslel5H;  h_1d_crslel_v.push_back(h_1d_crslel5H);  
  std::map<std::string, TH1D*> h_1d_crslel6H;  h_1d_crslel_v.push_back(h_1d_crslel6H);  
  std::map<std::string, TH1D*> h_1d_crslel7H;  h_1d_crslel_v.push_back(h_1d_crslel7H);  
  std::map<std::string, TH1D*> h_1d_crslel8H;  h_1d_crslel_v.push_back(h_1d_crslel8H);  
  std::map<std::string, TH1D*> h_1d_crslel9H;  h_1d_crslel_v.push_back(h_1d_crslel9H);  
  std::map<std::string, TH1D*> h_1d_crslel10H; h_1d_crslel_v.push_back(h_1d_crslel10H); 
  std::map<std::string, TH1D*> h_1d_crslel1M;  h_1d_crslel_v.push_back(h_1d_crslel1M);  
  std::map<std::string, TH1D*> h_1d_crslel2M;  h_1d_crslel_v.push_back(h_1d_crslel2M);  
  std::map<std::string, TH1D*> h_1d_crslel3M;  h_1d_crslel_v.push_back(h_1d_crslel3M);  
  std::map<std::string, TH1D*> h_1d_crslel4M;  h_1d_crslel_v.push_back(h_1d_crslel4M);  
  std::map<std::string, TH1D*> h_1d_crslel5M;  h_1d_crslel_v.push_back(h_1d_crslel5M);  
  std::map<std::string, TH1D*> h_1d_crslel6M;  h_1d_crslel_v.push_back(h_1d_crslel6M);  
  std::map<std::string, TH1D*> h_1d_crslel7M;  h_1d_crslel_v.push_back(h_1d_crslel7M);  
  std::map<std::string, TH1D*> h_1d_crslel8M;  h_1d_crslel_v.push_back(h_1d_crslel8M);  
  std::map<std::string, TH1D*> h_1d_crslel9M;  h_1d_crslel_v.push_back(h_1d_crslel9M);  
  std::map<std::string, TH1D*> h_1d_crslel10M; h_1d_crslel_v.push_back(h_1d_crslel10M); 
  std::map<std::string, TH1D*> h_1d_crslel1L;  h_1d_crslel_v.push_back(h_1d_crslel1L);  
  std::map<std::string, TH1D*> h_1d_crslel2L;  h_1d_crslel_v.push_back(h_1d_crslel2L);  
  std::map<std::string, TH1D*> h_1d_crslel3L;  h_1d_crslel_v.push_back(h_1d_crslel3L);  
  std::map<std::string, TH1D*> h_1d_crslel4L;  h_1d_crslel_v.push_back(h_1d_crslel4L);  
  std::map<std::string, TH1D*> h_1d_crslel5L;  h_1d_crslel_v.push_back(h_1d_crslel5L);  
  std::map<std::string, TH1D*> h_1d_crslel6L;  h_1d_crslel_v.push_back(h_1d_crslel6L);  
  std::map<std::string, TH1D*> h_1d_crslel7L;  h_1d_crslel_v.push_back(h_1d_crslel7L);  
  std::map<std::string, TH1D*> h_1d_crslel8L;  h_1d_crslel_v.push_back(h_1d_crslel8L);  
  std::map<std::string, TH1D*> h_1d_crslel9L;  h_1d_crslel_v.push_back(h_1d_crslel9L);  
  std::map<std::string, TH1D*> h_1d_crslel10L; h_1d_crslel_v.push_back(h_1d_crslel10L); 

  std::map<std::string, TH1D*> h_1d_crslhadbase;
  std::map<std::string, TH1D*> h_1d_crslhadw;
  std::map<std::string, TH1D*> h_1d_crslhadtt;

  std::vector< std::map<std::string, TH1D*> > h_1d_crslhad_v;
  std::map<std::string, TH1D*> h_1d_crslhad1H;  h_1d_crslhad_v.push_back(h_1d_crslhad1H);  
  std::map<std::string, TH1D*> h_1d_crslhad2H;  h_1d_crslhad_v.push_back(h_1d_crslhad2H);  
  std::map<std::string, TH1D*> h_1d_crslhad3H;  h_1d_crslhad_v.push_back(h_1d_crslhad3H);  
  std::map<std::string, TH1D*> h_1d_crslhad4H;  h_1d_crslhad_v.push_back(h_1d_crslhad4H);  
  std::map<std::string, TH1D*> h_1d_crslhad5H;  h_1d_crslhad_v.push_back(h_1d_crslhad5H);  
  std::map<std::string, TH1D*> h_1d_crslhad6H;  h_1d_crslhad_v.push_back(h_1d_crslhad6H);  
  std::map<std::string, TH1D*> h_1d_crslhad7H;  h_1d_crslhad_v.push_back(h_1d_crslhad7H);  
  std::map<std::string, TH1D*> h_1d_crslhad8H;  h_1d_crslhad_v.push_back(h_1d_crslhad8H);  
  std::map<std::string, TH1D*> h_1d_crslhad9H;  h_1d_crslhad_v.push_back(h_1d_crslhad9H);  
  std::map<std::string, TH1D*> h_1d_crslhad10H; h_1d_crslhad_v.push_back(h_1d_crslhad10H); 
  std::map<std::string, TH1D*> h_1d_crslhad1M;  h_1d_crslhad_v.push_back(h_1d_crslhad1M);  
  std::map<std::string, TH1D*> h_1d_crslhad2M;  h_1d_crslhad_v.push_back(h_1d_crslhad2M);  
  std::map<std::string, TH1D*> h_1d_crslhad3M;  h_1d_crslhad_v.push_back(h_1d_crslhad3M);  
  std::map<std::string, TH1D*> h_1d_crslhad4M;  h_1d_crslhad_v.push_back(h_1d_crslhad4M);  
  std::map<std::string, TH1D*> h_1d_crslhad5M;  h_1d_crslhad_v.push_back(h_1d_crslhad5M);  
  std::map<std::string, TH1D*> h_1d_crslhad6M;  h_1d_crslhad_v.push_back(h_1d_crslhad6M);  
  std::map<std::string, TH1D*> h_1d_crslhad7M;  h_1d_crslhad_v.push_back(h_1d_crslhad7M);  
  std::map<std::string, TH1D*> h_1d_crslhad8M;  h_1d_crslhad_v.push_back(h_1d_crslhad8M);  
  std::map<std::string, TH1D*> h_1d_crslhad9M;  h_1d_crslhad_v.push_back(h_1d_crslhad9M);  
  std::map<std::string, TH1D*> h_1d_crslhad10M; h_1d_crslhad_v.push_back(h_1d_crslhad10M); 
  std::map<std::string, TH1D*> h_1d_crslhad1L;  h_1d_crslhad_v.push_back(h_1d_crslhad1L);  
  std::map<std::string, TH1D*> h_1d_crslhad2L;  h_1d_crslhad_v.push_back(h_1d_crslhad2L);  
  std::map<std::string, TH1D*> h_1d_crslhad3L;  h_1d_crslhad_v.push_back(h_1d_crslhad3L);  
  std::map<std::string, TH1D*> h_1d_crslhad4L;  h_1d_crslhad_v.push_back(h_1d_crslhad4L);  
  std::map<std::string, TH1D*> h_1d_crslhad5L;  h_1d_crslhad_v.push_back(h_1d_crslhad5L);  
  std::map<std::string, TH1D*> h_1d_crslhad6L;  h_1d_crslhad_v.push_back(h_1d_crslhad6L);  
  std::map<std::string, TH1D*> h_1d_crslhad7L;  h_1d_crslhad_v.push_back(h_1d_crslhad7L);  
  std::map<std::string, TH1D*> h_1d_crslhad8L;  h_1d_crslhad_v.push_back(h_1d_crslhad8L);  
  std::map<std::string, TH1D*> h_1d_crslhad9L;  h_1d_crslhad_v.push_back(h_1d_crslhad9L);  
  std::map<std::string, TH1D*> h_1d_crslhad10L; h_1d_crslhad_v.push_back(h_1d_crslhad10L); 


  
  // These will be set to true if any SL GJ or DY control region plots are produced
  bool saveGJplots = false;
  bool saveDYplots = false;
  bool saveSLplots = false;
  bool saveSLMUplots = false;
  bool saveSLELplots = false;
  bool saveSLHADplots = false;

  // File Loop
  int nDuplicates = 0;
  int nEvents = chain->GetEntries();
  unsigned int nEventsChain = nEvents;
  cout << "[MT2Looper::loop] running on " << nEventsChain << " events" << endl;
  unsigned int nEventsTotal = 0;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;
  while ( (currentFile = (TFile*)fileIter.Next()) ) {
    cout << "[MT2Looper::loop] running on file: " << currentFile->GetTitle() << endl;

    // Get File Content
    TFile f( currentFile->GetTitle() );
    TTree *tree = (TTree*)f.Get("mt2");
    TTreeCache::SetLearnEntries(10);
    tree->SetCacheSize(128*1024*1024);
    //mt2tree t(tree);
    
    // Use this to speed things up when not looking at genParticles
    //tree->SetBranchStatus("genPart_*", 0); 

    t.Init(tree);

    // Event Loop
    unsigned int nEventsTree = tree->GetEntriesFast();
    for( unsigned int event = 0; event < nEventsTree; ++event) {
    //    for( unsigned int event = 0; event < nEventsTree/10.; ++event) {
    //    for( unsigned int event = 0; event < 100.; ++event) {
      
      t.GetEntry(event);

      //---------------------
      // bookkeeping and progress report
      //---------------------
      ++nEventsTotal;
      if (nEventsTotal%10000==0) {
	ULong64_t i_permille = (int)floor(1000 * nEventsTotal / float(nEventsChain));
	if (isatty(1)) {
	  printf("\015\033[32m ---> \033[1m\033[31m%4.1f%%"
		 "\033[0m\033[32m <---\033[0m\015", i_permille/10.);
	  fflush(stdout);
	}
      }

      //---------------------
      // skip duplicates -- will need this eventually
      //---------------------
      //if( isData ) {
      //  DorkyEventIdentifier id = {stopt.run(), stopt.event(), stopt.lumi() };
      //  if (is_duplicate(id, already_seen) ){
      //    continue;
      //  }
      //}

      //---------------------
      // basic event selection and cleaning
      //---------------------

      if (!PassesEventSelection(SignalRegionVersion::sel2012, t.nVert)) continue;

      // remove low pt QCD samples 
      if (t.evt_id >= 100 && t.evt_id < 108) continue;

      //---------------------
      // set weights and start making plots
      //---------------------
      outfile_->cd();
      const float lumi = 5.;
      evtweight_ = t.evt_scale1fb * lumi;

      // ***************************************
      // ***** HACK FOR CSA14 ttbar SAMPLE *****
      // ***************************************
      // fix scale1fb weight
      if (t.evt_id == 300) evtweight_ *= 25394255./25031921.;

      plot1D("h_nvtx",       t.nVert,       evtweight_, h_1d_global, ";N(vtx)", 80, 0, 80);
      plot1D("h_mt2",       t.mt2,       evtweight_, h_1d_global, ";M_{T2} [GeV]", 80, 0, 800);

      // note: this will double count some leptons, since reco leptons can appear as PFcands
      nlepveto_ = 0;
      int nlepveto_overlap = t.nMuons10 + t.nElectrons10 + t.nPFLep5LowMT + t.nPFHad10LowMT;

      // variables for single lep control region
      bool doSLplots = false;
      bool doSLMUplots = false;
      bool doSLELplots = false;
      bool doSLHADplots = false;
      leppt_ = -1.;
      mt_ = -1.;
      // do lepton overlap removal and 1L CR selections
      if (nlepveto_overlap >= 1) {
	std::vector<MT2Looper::lepcand> all_cands;
	std::vector<MT2Looper::lepcand> unique_cands;
	// require MT < 100 for reco leptons
	//  require at most 1 reco lepton at this point, do overlap with PFcands below
	if ( (t.nMuons10 == 1 && t.nElectrons10 == 0) || (t.nMuons10 == 0 && t.nElectrons10 == 1) ) {
	  MT2Looper::lepcand cand;
	  cand.pt = t.lep_pt[0];
	  cand.phi = t.lep_phi[0];
	  cand.mt = sqrt( 2 * t.met_pt * cand.pt * ( 1 - cos( t.met_phi - cand.phi) ) );
	  if (cand.mt > 100.) continue;

	  // cand passes cuts: add to vector
	  cand.eta = t.lep_eta[0];
	  cand.pdgId = t.lep_pdgId[0];
	  cand.isPFCand = false;
	  all_cands.push_back(cand);
	  //doSLplots = true;
	} 
	// pf leptons: need to find cand passing selection
	else if (t.nPFLep5LowMT == 1) {
	  for (int itrk = 0; itrk < t.nisoTrack; ++itrk) {
	    MT2Looper::lepcand cand;
	    cand.pt = t.isoTrack_pt[itrk];
	    cand.phi = t.isoTrack_phi[itrk];
	    cand.pdgId = t.isoTrack_pdgId[itrk];
	    if (cand.pt < 5.) continue;
	    if (abs(cand.pdgId) != 11 && abs(cand.pdgId) != 13) continue;
	    float absiso = t.isoTrack_absIso[itrk];
	    if (absiso/cand.pt > 0.2) continue;
	    cand.mt = sqrt( 2 * t.met_pt * cand.pt * ( 1 - cos( t.met_phi - cand.phi) ) );
	    if (cand.mt > 100.) continue;

	    // cand passes cuts: add to vector
	    cand.eta = t.isoTrack_eta[itrk];
	    cand.isPFCand = true;
	    all_cands.push_back(cand);
	  } // loop on isoTracks
	}
	// pf hadrons: need to find cand passing selection
	else if (t.nPFHad10LowMT == 1) {
	  for (int itrk = 0; itrk < t.nisoTrack; ++itrk) {
	    MT2Looper::lepcand cand;
	    cand.pt = t.isoTrack_pt[itrk];
	    cand.phi = t.isoTrack_phi[itrk];
	    cand.pdgId = t.isoTrack_pdgId[itrk];
	    if (cand.pt < 10.) continue;
	    if (abs(cand.pdgId) != 211) continue;
	    float absiso = t.isoTrack_absIso[itrk];
	    if (absiso/cand.pt > 0.1) continue;
	    cand.mt = sqrt( 2 * t.met_pt * cand.pt * ( 1 - cos( t.met_phi - cand.phi) ) );
	    if (cand.mt > 100.) continue;

	    // cand passes cuts: add to vector
	    cand.eta = t.isoTrack_eta[itrk];
	    cand.isPFCand = true;
	    all_cands.push_back(cand);
	  } // loop on isoTracks
	}

	// check all_cands for overlaps
	for (unsigned int icand = 0; icand < all_cands.size(); ++icand) {
	  bool keep = true;
	  for (unsigned int jcand = 0; jcand < all_cands.size(); ++jcand) {
	    float dr = DeltaR(all_cands.at(icand).eta, all_cands.at(jcand).eta, all_cands.at(icand).phi, all_cands.at(jcand).phi);
	    if (dr < 0.1) {
	      // if overlap, check whether the cands have the same pdgId
	      // keep the reco lepton in case of overlap with PF lepton
	      if (all_cands.at(icand).pdgId != all_cands.at(jcand).pdgId) {
		std::cout << "WARNING: leptons with different pdgIds overlap." << std::endl
			  << "  lep 1: pt: " << all_cands.at(icand).pt << ", pdgId: " << all_cands.at(icand).pdgId
			  << ", isPFCand: " << all_cands.at(icand).isPFCand << std::endl
			  << "  lep 2: pt: " << all_cands.at(jcand).pt << ", pdgId: " << all_cands.at(jcand).pdgId
			  << ", isPFCand: " << all_cands.at(jcand).isPFCand << std::endl;
	      }
	      if (all_cands.at(icand).isPFCand && !all_cands.at(jcand).isPFCand) keep = false;
	    }
	  }
	  if (keep) unique_cands.push_back(all_cands.at(icand));
	}

	// check size of unique cands, and if == 1, fill 1L CR plots
	if (unique_cands.size() == 1) {
	  leppt_ = unique_cands.at(0).pt;
	  mt_ = unique_cands.at(0).mt;
	  doSLplots = true;
	  if (abs(unique_cands.at(0).pdgId) == 13) doSLMUplots = true;
	  else if (abs(unique_cands.at(0).pdgId) == 11) doSLELplots = true;
	  else if (abs(unique_cands.at(0).pdgId) == 211) doSLHADplots = true;
	}

	nlepveto_ = (int) unique_cands.size();

      } // for 1L control region

      // Variables for gamma+jets control region
      bool doGJplots = false;
      int jetIdx0 = 0;
      int jetIdx1 = 1;
      if (t.evt_id < 300) {
      	if (t.ngamma > 0) {
      	  if ( (t.evt_id < 200 && t.gamma_mcMatchId[0]>0  /*&& t.gamma_genIso[0]<5*/)    // Reject true photons from QCD (iso is always 0 for now)
      	       || (t.evt_id >=200 && t.gamma_mcMatchId[0]==0 )                           // Reject unmatched photons from Gamma+Jets
      	       || (t.evt_id >=200 && t.gamma_mcMatchId[0] >0 && t.gamma_genIso[0]>5) )   // Reject non-iso photons from Gamma+Jets
      	    { doGJplots = false; }
      	  else {
      	    // Redefine leading two jets after jet/photon overlap
      	    float minDR = 0.4;
      	    int gammaJet = -1;
      	    for (int i = 0; i < t.njet; i++) {
      	      float thisDR = DeltaR(t.jet_eta[i], t.gamma_eta[0], t.jet_phi[i], t.gamma_phi[0]);
      	      if(thisDR < minDR){
      		minDR = thisDR; 
      		gammaJet = i;
      	      }
      	    } 
      	    if (gammaJet==0) { jetIdx0++; jetIdx1++;}
      	    if (gammaJet==1) { jetIdx1++;} 
      	    doGJplots = true;
      	  }
      	} // ngamma > 0
      }// evt_id < 300

      // Variables for Zll (DY) control region
      bool doDYplots = false;
      if (t.evt_id >= 700 && t.evt_id < 800) {
      	if (t.nlep == 2) {
      	  if ( (t.lep_charge[0] * t.lep_charge[1] != -1)
      	       || (abs(t.lep_pdgId[0]) != abs(t.lep_pdgId[1]) )
      	       || (fabs(t.zll_mass - 90) > 20 ) ) {
      		 nlepveto_ = nlepveto_ - 2;
      		 doDYplots = true;
      	       }
      	} // nlep == 2
      }// evt_id 



      fillHistos(h_1d_nocut, "nocut");

      fillHistosSignalRegion(h_1d_srbase, SignalRegionJets::nocut, SignalRegionHtMet::nocut, "srbase");
      fillHistosSignalRegion(h_1d_srH, SignalRegionJets::nocut, SignalRegionHtMet::h_ht, "srH");
      fillHistosSignalRegion(h_1d_srM, SignalRegionJets::nocut, SignalRegionHtMet::m_ht, "srM");
      fillHistosSignalRegion(h_1d_srL, SignalRegionJets::nocut, SignalRegionHtMet::l_ht, "srL");

      for (unsigned int imap = 0; imap < h_1d_sr_v.size(); imap++) {
	fillHistosSignalRegion(h_1d_sr_v.at(imap), srJets.at(imap), srHtMet.at(imap), "sr"+regions.at(imap));
      } 

      if (doGJplots) {
      	saveGJplots = true;
      	fillHistosCRGJ(h_1d_crgjbase, SignalRegionJets::nocut, SignalRegionHtMet::nocut, "crgjbase","", jetIdx0, jetIdx1);
      	for (unsigned int imap = 0; imap < h_1d_crgj_v.size(); imap++) {
      	  fillHistosCRGJ(h_1d_crgj_v.at(imap), srJets.at(imap), srHtMet.at(imap), "crgj"+regions.at(imap),"", jetIdx0, jetIdx1);
      	} 
      }

      if (doDYplots) {
      	saveDYplots = true;
      	for (unsigned int imap = 0; imap < h_1d_crdy_v.size(); imap++) {
      	  fillHistosCRDY(h_1d_crdy_v.at(imap), srJets.at(imap), srHtMet.at(imap), "crdy"+regions.at(imap),"");
      	} 
      }

      if (doSLplots) {
      	saveSLplots = true;
        fillHistosCRSL(h_1d_crslbase, SignalRegionJets::nocut, SignalRegionHtMet::nocut, "crslbase");
        fillHistosCRSL(h_1d_crslw, SignalRegionJets::crw, SignalRegionHtMet::nocut, "crslw");
        fillHistosCRSL(h_1d_crsltt, SignalRegionJets::crtt, SignalRegionHtMet::nocut, "crsltt");
      	for (unsigned int imap = 0; imap < h_1d_crsl_v.size(); imap++) {
      	  fillHistosCRSL(h_1d_crsl_v.at(imap), srJets.at(imap), srHtMet.at(imap), "crsl"+regions.at(imap));
      	} 

	if (doSLMUplots) {
	  fillHistosCRSL(h_1d_crslmubase, SignalRegionJets::nocut, SignalRegionHtMet::nocut, "crslmubase");
	  fillHistosCRSL(h_1d_crslmuw, SignalRegionJets::crw, SignalRegionHtMet::nocut, "crslmuw");
	  fillHistosCRSL(h_1d_crslmutt, SignalRegionJets::crtt, SignalRegionHtMet::nocut, "crslmutt");
	  for (unsigned int imap = 0; imap < h_1d_crslmu_v.size(); imap++) {
	    fillHistosCRSL(h_1d_crslmu_v.at(imap), srJets.at(imap), srHtMet.at(imap), "crslmu"+regions.at(imap));
	  } 
	}

	if (doSLELplots) {
	  fillHistosCRSL(h_1d_crslelbase, SignalRegionJets::nocut, SignalRegionHtMet::nocut, "crslelbase");
	  fillHistosCRSL(h_1d_crslelw, SignalRegionJets::crw, SignalRegionHtMet::nocut, "crslelw");
	  fillHistosCRSL(h_1d_crsleltt, SignalRegionJets::crtt, SignalRegionHtMet::nocut, "crsleltt");
	  for (unsigned int imap = 0; imap < h_1d_crslel_v.size(); imap++) {
	    fillHistosCRSL(h_1d_crslel_v.at(imap), srJets.at(imap), srHtMet.at(imap), "crslel"+regions.at(imap));
	  } 
	}

	if (doSLHADplots) {
	  fillHistosCRSL(h_1d_crslhadbase, SignalRegionJets::nocut, SignalRegionHtMet::nocut, "crslhadbase");
	  fillHistosCRSL(h_1d_crslhadw, SignalRegionJets::crw, SignalRegionHtMet::nocut, "crslhadw");
	  fillHistosCRSL(h_1d_crslhadtt, SignalRegionJets::crtt, SignalRegionHtMet::nocut, "crslhadtt");
	  for (unsigned int imap = 0; imap < h_1d_crslhad_v.size(); imap++) {
	    fillHistosCRSL(h_1d_crslhad_v.at(imap), srJets.at(imap), srHtMet.at(imap), "crslhad"+regions.at(imap));
	  } 
	}

      }

   }//end loop on events in a file
  
    delete tree;
    f.Close();
  }//end loop on files
  
  cout << "[MT2Looper::loop] processed " << nEventsTotal << " events" << endl;
  if ( nEventsChain != nEventsTotal ) {
    std::cout << "ERROR: number of events from files is not equal to total number of events" << std::endl;
  }

  cout << nDuplicates << " duplicate events were skipped." << endl;

  //---------------------
  // Save Plots
  //---------------------

  outfile_->cd();
  savePlotsDir(h_1d_global,outfile_,"");
  savePlotsDir(h_1d_nocut,outfile_,"nocut");
  savePlotsDir(h_1d_srbase,outfile_,"srbase");
  savePlotsDir(h_1d_srH,outfile_,"srH");
  savePlotsDir(h_1d_srM,outfile_,"srM");
  savePlotsDir(h_1d_srL,outfile_,"srL");

  for (unsigned int imap = 0; imap < h_1d_sr_v.size(); imap++) {
    savePlotsDir(h_1d_sr_v.at(imap),outfile_,("sr"+regions.at(imap)).c_str());
  } 

  if (saveGJplots) {
    for (unsigned int imap = 0; imap < h_1d_crgj_v.size(); imap++) {
      savePlotsDir(h_1d_crgj_v.at(imap),outfile_,("crgj"+regions.at(imap)).c_str());
    } 
    savePlotsDir(h_1d_crgjbase,outfile_,"crgjbase");
  }

  if (saveDYplots) {
    for (unsigned int imap = 0; imap < h_1d_crdy_v.size(); imap++) {
      savePlotsDir(h_1d_crdy_v.at(imap),outfile_,("crdy"+regions.at(imap)).c_str());
    } 
  }

  if (saveSLplots) {
    savePlotsDir(h_1d_crslbase,outfile_,"crslbase");
    savePlotsDir(h_1d_crslw,outfile_,"crslw");
    savePlotsDir(h_1d_crsltt,outfile_,"crsltt");
    for (unsigned int imap = 0; imap < h_1d_crsl_v.size(); imap++) {
      savePlotsDir(h_1d_crsl_v.at(imap),outfile_,("crsl"+regions.at(imap)).c_str());
    } 

    if (saveSLMUplots) {
      savePlotsDir(h_1d_crslmubase,outfile_,"crslmubase");
      savePlotsDir(h_1d_crslmuw,outfile_,"crslmuw");
      savePlotsDir(h_1d_crslmutt,outfile_,"crslmutt");
      for (unsigned int imap = 0; imap < h_1d_crslmu_v.size(); imap++) {
	savePlotsDir(h_1d_crslmu_v.at(imap),outfile_,("crslmu"+regions.at(imap)).c_str());
      } 
    }

    if (saveSLELplots) {
      savePlotsDir(h_1d_crslelbase,outfile_,"crslelbase");
      savePlotsDir(h_1d_crslelw,outfile_,"crslelw");
      savePlotsDir(h_1d_crsleltt,outfile_,"crsleltt");
      for (unsigned int imap = 0; imap < h_1d_crslel_v.size(); imap++) {
	savePlotsDir(h_1d_crslel_v.at(imap),outfile_,("crslel"+regions.at(imap)).c_str());
      } 
    }

    if (saveSLHADplots) {
      savePlotsDir(h_1d_crslhadbase,outfile_,"crslhadbase");
      savePlotsDir(h_1d_crslhadw,outfile_,"crslhadw");
      savePlotsDir(h_1d_crslhadtt,outfile_,"crslhadtt");
      for (unsigned int imap = 0; imap < h_1d_crslhad_v.size(); imap++) {
	savePlotsDir(h_1d_crslhad_v.at(imap),outfile_,("crslhad"+regions.at(imap)).c_str());
      } 
    }
  }


  //---------------------
  // Write and Close file
  //---------------------
  outfile_->Write();
  outfile_->Close();
  delete outfile_;

  bmark->Stop("benchmark");
  cout << endl;
  cout << nEventsTotal << " Events Processed" << endl;
  cout << "------------------------------" << endl;
  cout << "CPU  Time:	" << Form( "%.01f s", bmark->GetCpuTime("benchmark")  ) << endl;
  cout << "Real Time:	" << Form( "%.01f s", bmark->GetRealTime("benchmark") ) << endl;
  cout << endl;
  delete bmark;

  return;
}

void MT2Looper::fillHistosSignalRegion(std::map<std::string, TH1D*>& h_1d, const SignalRegionJets::value_type& sr_jets, const SignalRegionHtMet::value_type& sr_htmet, const std::string& dirname, const std::string& suffix) {

  if ( !PassesSignalRegion(SignalRegionVersion::sel2015LowLumi, t.mt2, t.met_pt, t.ht, t.nJet40, t.nBJet40, t.deltaPhiMin, t.diffMetMht, 
			   t.minMTBMet, nlepveto_, t.jet_pt[0], t.jet_pt[1], sr_jets, sr_htmet) ) return;

  plot1D("h_SignalRegion",  sr_jets+sr_htmet,   evtweight_, h_1d_global, ";Signal Region", 100, 0, 100);
  fillHistos( h_1d, dirname, suffix);

  return;
}

// hists for single lepton control region
void MT2Looper::fillHistosCRSL(std::map<std::string, TH1D*>& h_1d, const SignalRegionJets::value_type& sr_jets, const SignalRegionHtMet::value_type& sr_htmet, const std::string& dirname, const std::string& suffix) {

  if ( !PassesSignalRegionNoLepVeto(SignalRegionVersion::sel2015LowLumi, t.mt2, t.met_pt, t.ht, t.nJet40, t.nBJet40, t.deltaPhiMin, t.diffMetMht,
				    t.minMTBMet, t.jet_pt[0], t.jet_pt[1], sr_jets, sr_htmet) ) return;
  if (nlepveto_ != 1) return;

  // fill hists
  fillHistos( h_1d, dirname, suffix);
  fillHistosSingleLepton( h_1d, dirname, suffix);

  // // fill CR summary hist
  // outfile_->cd();
  // plot1D("h_SingleLeptonControlRegion",  sr_jets+sr_htmet,   evtweight_, h_1d_global, ";Single Lepton Control Region", 100, 0, 100);

  return;
}

// hists for Gamma+Jets control region
void MT2Looper::fillHistosCRGJ(std::map<std::string, TH1D*>& h_1d, const SignalRegionJets::value_type& sr_jets, const SignalRegionHtMet::value_type& sr_htmet, const std::string& dirname, const std::string& suffix, const int jetIdx0, const int jetIdx1) {

  if (t.ngamma==0) return;


  if ( !PassesSignalRegion(SignalRegionVersion::sel2015LowLumi, t.gamma_mt2, t.gamma_met_pt, t.gamma_ht, t.gamma_nJet40, t.gamma_nBJet40, t.gamma_deltaPhiMin, t.gamma_diffMetMht,
				    t.gamma_minMTBMet, nlepveto_, t.jet_pt[jetIdx0], t.jet_pt[jetIdx1], sr_jets, sr_htmet) ) return;

  bool passSieie = t.gamma_idCutBased[0] ? true : false; // just deal with the standard case now. Worry later about sideband in sieie

  // fill hists
  if (passSieie) {
    fillHistosGammaJets( h_1d, dirname, suffix);
  }
  return;
}

// hists for Zll control region
void MT2Looper::fillHistosCRDY(std::map<std::string, TH1D*>& h_1d, const SignalRegionJets::value_type& sr_jets, const SignalRegionHtMet::value_type& sr_htmet, const std::string& dirname, const std::string& suffix) {

  if (t.nlep!=2) return;

  if ( !PassesSignalRegion(SignalRegionVersion::sel2015LowLumi, t.zll_mt2, t.zll_met_pt, t.zll_ht, t.nJet40, t.nBJet40, t.zll_deltaPhiMin, t.zll_diffMetMht,
				    t.zll_minMTBMet, nlepveto_, t.jet_pt[0], t.jet_pt[1], sr_jets, sr_htmet) ) return;

  fillHistosDY( h_1d, dirname, suffix);
  return;
}


void MT2Looper::fillHistos(std::map<std::string, TH1D*>& h_1d, const std::string& dirname, const std::string& s) {
  TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
  if (dir == 0) {
    dir = outfile_->mkdir(dirname.c_str());
  } 
  dir->cd();

  plot1D("h_Events"+s,  1, 1, h_1d, ";Events, Unweighted", 1, 0, 2);
  plot1D("h_Events_w"+s,  1,   evtweight_, h_1d, ";Events, Weighted", 1, 0, 2);
  plot1D("h_mt2"+s,       t.mt2,   evtweight_, h_1d, "; M_{T2} [GeV]", 150, 0, 1500);
  plot1D("h_met"+s,       t.met_pt,   evtweight_, h_1d, ";E_{T}^{miss} [GeV]", 150, 0, 1500);
  plot1D("h_ht"+s,       t.ht,   evtweight_, h_1d, ";H_{T} [GeV]", 120, 0, 3000);
  plot1D("h_nJet40"+s,       t.nJet40,   evtweight_, h_1d, ";N(jets)", 15, 0, 15);
  plot1D("h_nBJet40"+s,      t.nBJet40,   evtweight_, h_1d, ";N(bjets)", 6, 0, 6);
  plot1D("h_deltaPhiMin"+s,  t.deltaPhiMin,   evtweight_, h_1d, ";#Delta#phi_{min}", 32, 0, 3.2);
  plot1D("h_diffMetMht"+s,   t.diffMetMht,   evtweight_, h_1d, ";|E_{T}^{miss} - MHT| [GeV]", 120, 0, 300);
  plot1D("h_diffMetMhtOverMet"+s,   t.diffMetMht/t.met_pt,   evtweight_, h_1d, ";|E_{T}^{miss} - MHT| / E_{T}^{miss}", 100, 0, 2.);
  plot1D("h_minMTBMet"+s,   t.minMTBMet,   evtweight_, h_1d, ";min M_{T}(b, E_{T}^{miss}) [GeV]", 150, 0, 1500);
  plot1D("h_nlepveto"+s,     nlepveto_,   evtweight_, h_1d, ";N(leps)", 10, 0, 10);
  plot1D("h_J0pt"+s,       t.jet_pt[0],   evtweight_, h_1d, ";p_{T}(jet1) [GeV]", 150, 0, 1500);
  plot1D("h_J1pt"+s,       t.jet_pt[1],   evtweight_, h_1d, ";p_{T}(jet2) [GeV]", 150, 0, 1500);

  plot1D("h_mt2bins"+s,       t.mt2,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);

  outfile_->cd();
  return;
}

void MT2Looper::fillHistosSingleLepton(std::map<std::string, TH1D*>& h_1d, const std::string& dirname, const std::string& s) {
  TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
  if (dir == 0) {
    dir = outfile_->mkdir(dirname.c_str());
  } 
  dir->cd();

  plot1D("h_leppt"+s,      leppt_,   evtweight_, h_1d, ";p_{T}(lep) [GeV]", 200, 0, 1000);
  plot1D("h_mt"+s,            mt_,   evtweight_, h_1d, ";M_{T} [GeV]", 200, 0, 1000);

  outfile_->cd();
  return;
}


void MT2Looper::fillHistosGammaJets(std::map<std::string, TH1D*>& h_1d, const std::string& dirname, const std::string& s) {
  TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
  if (dir == 0) {
    dir = outfile_->mkdir(dirname.c_str());
  } 
  dir->cd();
  float iso = t.gamma_chHadIso[0] + t.gamma_neuHadIso[0];

  plot1D("h_iso"+s,      iso,   evtweight_, h_1d, ";iso [GeV]", 100, 0, 50);

  for (unsigned int i = 0; i < n_mt2bins; i++) {
    if ( t.gamma_mt2 > mt2bins[i] &&  t.gamma_mt2 < mt2bins[i+1]) 
      plot1D("h_iso_mt2bin"+mt2binsname[i]+s,  iso,  evtweight_, h_1d, "; iso", 100, 0, 50);
  }

  if (iso < 10) 
    plot1D("h_mt2bins"+s,       t.gamma_mt2,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);

  outfile_->cd();
  return;
}

void MT2Looper::fillHistosDY(std::map<std::string, TH1D*>& h_1d, const std::string& dirname, const std::string& s) {
  TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
  if (dir == 0) {
    dir = outfile_->mkdir(dirname.c_str());
  } 
  dir->cd();

  plot1D("h_mt2bins"+s,       t.zll_mt2,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);

  outfile_->cd();
  return;
}

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

  std::map<std::string, TH1D*> h_1d_nocut;
  std::map<std::string, TH1D*> h_1d_srbase;
  std::map<std::string, TH1D*> h_1d_srH;
  std::map<std::string, TH1D*> h_1d_srM;
  std::map<std::string, TH1D*> h_1d_srL;
  std::map<std::string, TH1D*> h_1d_sr1H;
  std::map<std::string, TH1D*> h_1d_sr2H;
  std::map<std::string, TH1D*> h_1d_sr3H;
  std::map<std::string, TH1D*> h_1d_sr4H;
  std::map<std::string, TH1D*> h_1d_sr5H;
  std::map<std::string, TH1D*> h_1d_sr6H;
  std::map<std::string, TH1D*> h_1d_sr7H;
  std::map<std::string, TH1D*> h_1d_sr8H;
  std::map<std::string, TH1D*> h_1d_sr9H;
  std::map<std::string, TH1D*> h_1d_sr10H;
  std::map<std::string, TH1D*> h_1d_sr1M;
  std::map<std::string, TH1D*> h_1d_sr2M;
  std::map<std::string, TH1D*> h_1d_sr3M;
  std::map<std::string, TH1D*> h_1d_sr4M;
  std::map<std::string, TH1D*> h_1d_sr5M;
  std::map<std::string, TH1D*> h_1d_sr6M;
  std::map<std::string, TH1D*> h_1d_sr7M;
  std::map<std::string, TH1D*> h_1d_sr8M;
  std::map<std::string, TH1D*> h_1d_sr9M;
  std::map<std::string, TH1D*> h_1d_sr10M;
  std::map<std::string, TH1D*> h_1d_sr1L;
  std::map<std::string, TH1D*> h_1d_sr2L;
  std::map<std::string, TH1D*> h_1d_sr3L;
  std::map<std::string, TH1D*> h_1d_sr4L;
  std::map<std::string, TH1D*> h_1d_sr5L;
  std::map<std::string, TH1D*> h_1d_sr6L;
  std::map<std::string, TH1D*> h_1d_sr7L;
  std::map<std::string, TH1D*> h_1d_sr8L;
  std::map<std::string, TH1D*> h_1d_sr9L;
  std::map<std::string, TH1D*> h_1d_sr10L;

  std::map<std::string, TH1D*> h_1d_crslbase_nomt;
  std::map<std::string, TH1D*> h_1d_crslH_nomt;
  std::map<std::string, TH1D*> h_1d_crslM_nomt;
  std::map<std::string, TH1D*> h_1d_crslL_nomt;

  std::map<std::string, TH1D*> h_1d_crslbase;
  std::map<std::string, TH1D*> h_1d_crslH;
  std::map<std::string, TH1D*> h_1d_crslM;
  std::map<std::string, TH1D*> h_1d_crslL;

  // std::map<std::string, TH1D*> h_1d_crsl1H;
  // std::map<std::string, TH1D*> h_1d_crsl2H;
  // std::map<std::string, TH1D*> h_1d_crsl3H;
  // std::map<std::string, TH1D*> h_1d_crsl4H;
  // std::map<std::string, TH1D*> h_1d_crsl5H;
  // std::map<std::string, TH1D*> h_1d_crsl6H;
  // std::map<std::string, TH1D*> h_1d_crsl7H;
  // std::map<std::string, TH1D*> h_1d_crsl8H;
  // std::map<std::string, TH1D*> h_1d_crsl9H;
  // std::map<std::string, TH1D*> h_1d_crsl1M;
  // std::map<std::string, TH1D*> h_1d_crsl2M;
  // std::map<std::string, TH1D*> h_1d_crsl3M;
  // std::map<std::string, TH1D*> h_1d_crsl4M;
  // std::map<std::string, TH1D*> h_1d_crsl5M;
  // std::map<std::string, TH1D*> h_1d_crsl6M;
  // std::map<std::string, TH1D*> h_1d_crsl7M;
  // std::map<std::string, TH1D*> h_1d_crsl8M;
  // std::map<std::string, TH1D*> h_1d_crsl9M;
  // std::map<std::string, TH1D*> h_1d_crsl1L;
  // std::map<std::string, TH1D*> h_1d_crsl2L;
  // std::map<std::string, TH1D*> h_1d_crsl3L;
  // std::map<std::string, TH1D*> h_1d_crsl4L;
  // std::map<std::string, TH1D*> h_1d_crsl5L;
  // std::map<std::string, TH1D*> h_1d_crsl6L;
  // std::map<std::string, TH1D*> h_1d_crsl7L;
  // std::map<std::string, TH1D*> h_1d_crsl8L;
  // std::map<std::string, TH1D*> h_1d_crsl9L;


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


  
  // These will be set to true if any GJ or DY control region plots are produced
  bool saveGJplots = false;
  bool saveDYplots = false;

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

      plot1D("h_nvtx",       t.nVert,       evtweight_, h_1d_global, ";N(vtx)", 80, 0, 80);
      plot1D("h_mt2",       t.mt2,       evtweight_, h_1d_global, ";M_{T2} [GeV]", 80, 0, 800);

      nlepveto_ = t.nMuons10 + t.nElectrons10 + t.nPFLep5LowMT + t.nPFHad10LowMT;

      // variables for single lep control region
      leppt_ = -1.;
      mt_ = -1.;
      if (nlepveto_ == 1) {
	float lepphi;
	if (t.nMuons10 == 1 || t.nElectrons10 == 1) {
	  leppt_ = t.lep_pt[0];
	  lepphi = t.lep_phi[0];
	} 
	else if (t.nTaus20 == 1) {
	  leppt_ = t.tau_pt[0];
	  lepphi = t.tau_phi[0];
	}

	// mt with lep1
	mt_ = sqrt( 2 * t.met_pt * leppt_ * ( 1 - cos( t.met_phi - lepphi) ) );
      }

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
      fillHistosSignalRegion(h_1d_sr1H, SignalRegionJets::sr1, SignalRegionHtMet::h_ht, "sr1H");
      fillHistosSignalRegion(h_1d_sr2H, SignalRegionJets::sr2, SignalRegionHtMet::h_ht, "sr2H");
      fillHistosSignalRegion(h_1d_sr3H, SignalRegionJets::sr3, SignalRegionHtMet::h_ht, "sr3H");
      fillHistosSignalRegion(h_1d_sr4H, SignalRegionJets::sr4, SignalRegionHtMet::h_ht, "sr4H");
      fillHistosSignalRegion(h_1d_sr5H, SignalRegionJets::sr5, SignalRegionHtMet::h_ht, "sr5H");
      fillHistosSignalRegion(h_1d_sr6H, SignalRegionJets::sr6, SignalRegionHtMet::h_ht, "sr6H");
      fillHistosSignalRegion(h_1d_sr7H, SignalRegionJets::sr7, SignalRegionHtMet::h_ht, "sr7H");
      fillHistosSignalRegion(h_1d_sr8H, SignalRegionJets::sr8, SignalRegionHtMet::h_ht, "sr8H");
      fillHistosSignalRegion(h_1d_sr9H, SignalRegionJets::sr9, SignalRegionHtMet::h_ht, "sr9H");
      fillHistosSignalRegion(h_1d_sr10H, SignalRegionJets::sr10, SignalRegionHtMet::h_ht, "sr10H");
      fillHistosSignalRegion(h_1d_sr1M, SignalRegionJets::sr1, SignalRegionHtMet::m_ht, "sr1M");
      fillHistosSignalRegion(h_1d_sr2M, SignalRegionJets::sr2, SignalRegionHtMet::m_ht, "sr2M");
      fillHistosSignalRegion(h_1d_sr3M, SignalRegionJets::sr3, SignalRegionHtMet::m_ht, "sr3M");
      fillHistosSignalRegion(h_1d_sr4M, SignalRegionJets::sr4, SignalRegionHtMet::m_ht, "sr4M");
      fillHistosSignalRegion(h_1d_sr5M, SignalRegionJets::sr5, SignalRegionHtMet::m_ht, "sr5M");
      fillHistosSignalRegion(h_1d_sr6M, SignalRegionJets::sr6, SignalRegionHtMet::m_ht, "sr6M");
      fillHistosSignalRegion(h_1d_sr7M, SignalRegionJets::sr7, SignalRegionHtMet::m_ht, "sr7M");
      fillHistosSignalRegion(h_1d_sr8M, SignalRegionJets::sr8, SignalRegionHtMet::m_ht, "sr8M");
      fillHistosSignalRegion(h_1d_sr9M, SignalRegionJets::sr9, SignalRegionHtMet::m_ht, "sr9M");
      fillHistosSignalRegion(h_1d_sr10M, SignalRegionJets::sr10, SignalRegionHtMet::m_ht, "sr10M");
      fillHistosSignalRegion(h_1d_sr1L, SignalRegionJets::sr1, SignalRegionHtMet::l_ht, "sr1L");
      fillHistosSignalRegion(h_1d_sr2L, SignalRegionJets::sr2, SignalRegionHtMet::l_ht, "sr2L");
      fillHistosSignalRegion(h_1d_sr3L, SignalRegionJets::sr3, SignalRegionHtMet::l_ht, "sr3L");
      fillHistosSignalRegion(h_1d_sr4L, SignalRegionJets::sr4, SignalRegionHtMet::l_ht, "sr4L");
      fillHistosSignalRegion(h_1d_sr5L, SignalRegionJets::sr5, SignalRegionHtMet::l_ht, "sr5L");
      fillHistosSignalRegion(h_1d_sr6L, SignalRegionJets::sr6, SignalRegionHtMet::l_ht, "sr6L");
      fillHistosSignalRegion(h_1d_sr7L, SignalRegionJets::sr7, SignalRegionHtMet::l_ht, "sr7L");
      fillHistosSignalRegion(h_1d_sr8L, SignalRegionJets::sr8, SignalRegionHtMet::l_ht, "sr8L");
      fillHistosSignalRegion(h_1d_sr9L, SignalRegionJets::sr9, SignalRegionHtMet::l_ht, "sr9L");
      fillHistosSignalRegion(h_1d_sr10L, SignalRegionJets::sr10, SignalRegionHtMet::l_ht, "sr10L");

      fillHistosCRSL(h_1d_crslbase_nomt, SignalRegionJets::nocut, SignalRegionHtMet::nocut, "crslbase_nomt");
      fillHistosCRSL(h_1d_crslH_nomt, SignalRegionJets::nocut, SignalRegionHtMet::h_ht, "crslH_nomt");
      fillHistosCRSL(h_1d_crslM_nomt, SignalRegionJets::nocut, SignalRegionHtMet::m_ht, "crslM_nomt");
      fillHistosCRSL(h_1d_crslL_nomt, SignalRegionJets::nocut, SignalRegionHtMet::l_ht, "crslL_nomt");

      fillHistosCRSL(h_1d_crslbase, SignalRegionJets::nocut, SignalRegionHtMet::nocut, "crslbase","",100.);
      fillHistosCRSL(h_1d_crslH, SignalRegionJets::nocut, SignalRegionHtMet::h_ht, "crslH","",100.);
      fillHistosCRSL(h_1d_crslM, SignalRegionJets::nocut, SignalRegionHtMet::m_ht, "crslM","",100.);
      fillHistosCRSL(h_1d_crslL, SignalRegionJets::nocut, SignalRegionHtMet::l_ht, "crslL","",100.);

      // fillHistosCRSL(h_1d_crsl1H, SignalRegionJets::sr1, SignalRegionHtMet::h_ht, "crsl1H");
      // fillHistosCRSL(h_1d_crsl2H, SignalRegionJets::sr2, SignalRegionHtMet::h_ht, "crsl2H");
      // fillHistosCRSL(h_1d_crsl3H, SignalRegionJets::sr3, SignalRegionHtMet::h_ht, "crsl3H");
      // fillHistosCRSL(h_1d_crsl4H, SignalRegionJets::sr4, SignalRegionHtMet::h_ht, "crsl4H");
      // fillHistosCRSL(h_1d_crsl5H, SignalRegionJets::sr5, SignalRegionHtMet::h_ht, "crsl5H");
      // fillHistosCRSL(h_1d_crsl6H, SignalRegionJets::sr6, SignalRegionHtMet::h_ht, "crsl6H");
      // fillHistosCRSL(h_1d_crsl7H, SignalRegionJets::sr7, SignalRegionHtMet::h_ht, "crsl7H");
      // fillHistosCRSL(h_1d_crsl8H, SignalRegionJets::sr8, SignalRegionHtMet::h_ht, "crsl8H");
      // fillHistosCRSL(h_1d_crsl9H, SignalRegionJets::sr9, SignalRegionHtMet::h_ht, "crsl9H");
      // fillHistosCRSL(h_1d_crsl1M, SignalRegionJets::sr1, SignalRegionHtMet::m_ht, "crsl1M");
      // fillHistosCRSL(h_1d_crsl2M, SignalRegionJets::sr2, SignalRegionHtMet::m_ht, "crsl2M");
      // fillHistosCRSL(h_1d_crsl3M, SignalRegionJets::sr3, SignalRegionHtMet::m_ht, "crsl3M");
      // fillHistosCRSL(h_1d_crsl4M, SignalRegionJets::sr4, SignalRegionHtMet::m_ht, "crsl4M");
      // fillHistosCRSL(h_1d_crsl5M, SignalRegionJets::sr5, SignalRegionHtMet::m_ht, "crsl5M");
      // fillHistosCRSL(h_1d_crsl6M, SignalRegionJets::sr6, SignalRegionHtMet::m_ht, "crsl6M");
      // fillHistosCRSL(h_1d_crsl7M, SignalRegionJets::sr7, SignalRegionHtMet::m_ht, "crsl7M");
      // fillHistosCRSL(h_1d_crsl8M, SignalRegionJets::sr8, SignalRegionHtMet::m_ht, "crsl8M");
      // fillHistosCRSL(h_1d_crsl9M, SignalRegionJets::sr9, SignalRegionHtMet::m_ht, "crsl9M");
      // fillHistosCRSL(h_1d_crsl1L, SignalRegionJets::sr1, SignalRegionHtMet::l_ht, "crsl1L");
      // fillHistosCRSL(h_1d_crsl2L, SignalRegionJets::sr2, SignalRegionHtMet::l_ht, "crsl2L");
      // fillHistosCRSL(h_1d_crsl3L, SignalRegionJets::sr3, SignalRegionHtMet::l_ht, "crsl3L");
      // fillHistosCRSL(h_1d_crsl4L, SignalRegionJets::sr4, SignalRegionHtMet::l_ht, "crsl4L");
      // fillHistosCRSL(h_1d_crsl5L, SignalRegionJets::sr5, SignalRegionHtMet::l_ht, "crsl5L");
      // fillHistosCRSL(h_1d_crsl6L, SignalRegionJets::sr6, SignalRegionHtMet::l_ht, "crsl6L");
      // fillHistosCRSL(h_1d_crsl7L, SignalRegionJets::sr7, SignalRegionHtMet::l_ht, "crsl7L");
      // fillHistosCRSL(h_1d_crsl8L, SignalRegionJets::sr8, SignalRegionHtMet::l_ht, "crsl8L");
      // fillHistosCRSL(h_1d_crsl9L, SignalRegionJets::sr9, SignalRegionHtMet::l_ht, "crsl9L");

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
	  fillHistosCRDY(h_1d_crdy_v.at(imap), srJets.at(imap), srHtMet.at(imap), "crgj"+regions.at(imap),"");
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
  savePlotsDir(h_1d_sr1H,outfile_,"sr1H");
  savePlotsDir(h_1d_sr2H,outfile_,"sr2H");
  savePlotsDir(h_1d_sr3H,outfile_,"sr3H");
  savePlotsDir(h_1d_sr4H,outfile_,"sr4H");
  savePlotsDir(h_1d_sr5H,outfile_,"sr5H");
  savePlotsDir(h_1d_sr6H,outfile_,"sr6H");
  savePlotsDir(h_1d_sr7H,outfile_,"sr7H");
  savePlotsDir(h_1d_sr8H,outfile_,"sr8H");
  savePlotsDir(h_1d_sr9H,outfile_,"sr9H");
  savePlotsDir(h_1d_sr10H,outfile_,"sr10H");
  savePlotsDir(h_1d_sr1M,outfile_,"sr1M");
  savePlotsDir(h_1d_sr2M,outfile_,"sr2M");
  savePlotsDir(h_1d_sr3M,outfile_,"sr3M");
  savePlotsDir(h_1d_sr4M,outfile_,"sr4M");
  savePlotsDir(h_1d_sr5M,outfile_,"sr5M");
  savePlotsDir(h_1d_sr6M,outfile_,"sr6M");
  savePlotsDir(h_1d_sr7M,outfile_,"sr7M");
  savePlotsDir(h_1d_sr8M,outfile_,"sr8M");
  savePlotsDir(h_1d_sr9M,outfile_,"sr9M");
  savePlotsDir(h_1d_sr10M,outfile_,"sr10M");
  savePlotsDir(h_1d_sr1L,outfile_,"sr1L");
  savePlotsDir(h_1d_sr2L,outfile_,"sr2L");
  savePlotsDir(h_1d_sr3L,outfile_,"sr3L");
  savePlotsDir(h_1d_sr4L,outfile_,"sr4L");
  savePlotsDir(h_1d_sr5L,outfile_,"sr5L");
  savePlotsDir(h_1d_sr6L,outfile_,"sr6L");
  savePlotsDir(h_1d_sr7L,outfile_,"sr7L");
  savePlotsDir(h_1d_sr8L,outfile_,"sr8L");
  savePlotsDir(h_1d_sr9L,outfile_,"sr9L");
  savePlotsDir(h_1d_sr10L,outfile_,"sr10L");

  savePlotsDir(h_1d_crslbase_nomt,outfile_,"crslbase_nomt");
  savePlotsDir(h_1d_crslH_nomt,outfile_,"crslH_nomt");
  savePlotsDir(h_1d_crslM_nomt,outfile_,"crslM_nomt");
  savePlotsDir(h_1d_crslL_nomt,outfile_,"crslL_nomt");

  savePlotsDir(h_1d_crslbase,outfile_,"crslbase");
  savePlotsDir(h_1d_crslH,outfile_,"crslH");
  savePlotsDir(h_1d_crslM,outfile_,"crslM");
  savePlotsDir(h_1d_crslL,outfile_,"crslL");

  // savePlotsDir(h_1d_crsl1H,outfile_,"crsl1H");
  // savePlotsDir(h_1d_crsl2H,outfile_,"crsl2H");
  // savePlotsDir(h_1d_crsl3H,outfile_,"crsl3H");
  // savePlotsDir(h_1d_crsl4H,outfile_,"crsl4H");
  // savePlotsDir(h_1d_crsl5H,outfile_,"crsl5H");
  // savePlotsDir(h_1d_crsl6H,outfile_,"crsl6H");
  // savePlotsDir(h_1d_crsl7H,outfile_,"crsl7H");
  // savePlotsDir(h_1d_crsl8H,outfile_,"crsl8H");
  // savePlotsDir(h_1d_crsl9H,outfile_,"crsl9H");
  // savePlotsDir(h_1d_crsl1M,outfile_,"crsl1M");
  // savePlotsDir(h_1d_crsl2M,outfile_,"crsl2M");
  // savePlotsDir(h_1d_crsl3M,outfile_,"crsl3M");
  // savePlotsDir(h_1d_crsl4M,outfile_,"crsl4M");
  // savePlotsDir(h_1d_crsl5M,outfile_,"crsl5M");
  // savePlotsDir(h_1d_crsl6M,outfile_,"crsl6M");
  // savePlotsDir(h_1d_crsl7M,outfile_,"crsl7M");
  // savePlotsDir(h_1d_crsl8M,outfile_,"crsl8M");
  // savePlotsDir(h_1d_crsl9M,outfile_,"crsl9M");
  // savePlotsDir(h_1d_crsl1L,outfile_,"crsl1L");
  // savePlotsDir(h_1d_crsl2L,outfile_,"crsl2L");
  // savePlotsDir(h_1d_crsl3L,outfile_,"crsl3L");
  // savePlotsDir(h_1d_crsl4L,outfile_,"crsl4L");
  // savePlotsDir(h_1d_crsl5L,outfile_,"crsl5L");
  // savePlotsDir(h_1d_crsl6L,outfile_,"crsl6L");
  // savePlotsDir(h_1d_crsl7L,outfile_,"crsl7L");
  // savePlotsDir(h_1d_crsl8L,outfile_,"crsl8L");
  // savePlotsDir(h_1d_crsl9L,outfile_,"crsl9L");

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
void MT2Looper::fillHistosCRSL(std::map<std::string, TH1D*>& h_1d, const SignalRegionJets::value_type& sr_jets, const SignalRegionHtMet::value_type& sr_htmet, const std::string& dirname, const std::string& suffix, const float mtcut) {

  if ( !PassesSignalRegionNoLepVeto(SignalRegionVersion::sel2015LowLumi, t.mt2, t.met_pt, t.ht, t.nJet40, t.nBJet40, t.deltaPhiMin, t.diffMetMht,
				    t.minMTBMet, t.jet_pt[0], t.jet_pt[1], sr_jets, sr_htmet) ) return;
  if (nlepveto_ != 1) return;

  // apply mtcut if requested
  if (mtcut > 0. && mt_ > mtcut) return;

  // fill hists
  fillHistos( h_1d, dirname, suffix);
  fillHistosSingleLepton( h_1d, dirname, suffix);

  // fill CR summary hist only if mt cut was applied
  if (mtcut > 0.) {
    outfile_->cd();
    plot1D("h_SingleLeptonControlRegion",  sr_jets+sr_htmet,   evtweight_, h_1d_global, ";Single Lepton Control Region", 100, 0, 100);
  }

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

  // mt2 binning for results
  const int n_mt2bins = 5;
  const float mt2bins[n_mt2bins+1] = {200., 300., 400., 600., 1000., 1500.};
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
  // mt2 binning for results
  const int n_mt2bins = 5;
  const float mt2bins[n_mt2bins+1] = {200., 300., 400., 600., 1000., 1500.};
  std::string mt2binsname[n_mt2bins+1] = {"200", "300", "400", "600", "1000", "1500"};
  plot1D("h_mt2bins"+s,       t.gamma_mt2,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
  for (unsigned int i = 0; i < n_mt2bins; i++) {
    if ( t.gamma_mt2 > mt2bins[n_mt2bins] &&  t.gamma_mt2 < mt2bins[n_mt2bins+1]) 
      plot1D("h_iso_mt2bin"+mt2binsname[i]+s,  iso,  evtweight_, h_1d, "; iso", 100, 0, 50);
  }

  outfile_->cd();
  return;
}

void MT2Looper::fillHistosDY(std::map<std::string, TH1D*>& h_1d, const std::string& dirname, const std::string& s) {
  TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
  if (dir == 0) {
    dir = outfile_->mkdir(dirname.c_str());
  } 
  dir->cd();

  const int n_mt2bins = 5;
  const float mt2bins[n_mt2bins+1] = {200., 300., 400., 600., 1000., 1500.};
  plot1D("h_mt2bins"+s,       t.zll_mt2,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);

  outfile_->cd();
  return;
}

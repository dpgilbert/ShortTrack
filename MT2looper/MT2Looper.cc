// C++
#include <iostream>
#include <vector>
#include <set>

// ROOT
#include "TDirectory.h"
#include "TTreeCache.h"
#include "Math/VectorUtil.h"
#include "TVector2.h"

// CMS2
//#include "../MT2CORE/CMS2.h"
//#include "../MT2CORE/tools.h"
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

  gROOT->cd();
  cout << "[MT2Looper::loop] creating output file: " << output_name << endl;

  outfile_ = new TFile(output_name.c_str(),"RECREATE") ; 

  cout << "[MT2Looper::loop] setting up histos" << endl;

  std::map<std::string, TH1F*> h_1d_nocut;
  std::map<std::string, TH1F*> h_1d_sr1H;
  std::map<std::string, TH1F*> h_1d_sr2H;
  std::map<std::string, TH1F*> h_1d_sr3H;
  std::map<std::string, TH1F*> h_1d_sr4H;
  std::map<std::string, TH1F*> h_1d_sr5H;
  std::map<std::string, TH1F*> h_1d_sr6H;
  std::map<std::string, TH1F*> h_1d_sr7H;
  std::map<std::string, TH1F*> h_1d_sr8H;
  std::map<std::string, TH1F*> h_1d_sr9H;
  std::map<std::string, TH1F*> h_1d_sr1M;
  std::map<std::string, TH1F*> h_1d_sr2M;
  std::map<std::string, TH1F*> h_1d_sr3M;
  std::map<std::string, TH1F*> h_1d_sr4M;
  std::map<std::string, TH1F*> h_1d_sr5M;
  std::map<std::string, TH1F*> h_1d_sr6M;
  std::map<std::string, TH1F*> h_1d_sr7M;
  std::map<std::string, TH1F*> h_1d_sr8M;
  std::map<std::string, TH1F*> h_1d_sr9M;
  std::map<std::string, TH1F*> h_1d_sr1L;
  std::map<std::string, TH1F*> h_1d_sr2L;
  std::map<std::string, TH1F*> h_1d_sr3L;
  std::map<std::string, TH1F*> h_1d_sr4L;
  std::map<std::string, TH1F*> h_1d_sr5L;
  std::map<std::string, TH1F*> h_1d_sr6L;
  std::map<std::string, TH1F*> h_1d_sr7L;
  std::map<std::string, TH1F*> h_1d_sr8L;
  std::map<std::string, TH1F*> h_1d_sr9L;
  
  
  // File Loop
  int nDuplicates = 0;
  int nEvents = chain->GetEntries();
  unsigned int nEventsChain = nEvents;
  unsigned int nEventsTotal = 0;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;
  while ( (currentFile = (TFile*)fileIter.Next()) ) {
    cout << "[MT2Looper::loop] running on file: " << currentFile->GetTitle() << endl;

    // Get File Content
    TFile f( currentFile->GetTitle() );
    TTree *tree = (TTree*)f.Get("treeProducerSusyFullHad");
    TTreeCache::SetLearnEntries(10);
    tree->SetCacheSize(128*1024*1024);
    //mt2tree t(tree);
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

      if (!PassesEventSelection(t.nVert)) continue;

      //---------------------
      // set weights and start making plots
      //---------------------
      outfile_->cd();
      const float lumi = 1.;
      evtweight_ = t.scale1fb * lumi;
      plot1D("h_nvtx",       t.nVert,       evtweight_, h_1d_global, 80, 0, 80);
      plot1D("h_mt2",       t.mt2,       evtweight_, h_1d_global, 80, 0, 800);

      nlepveto_ = t.nMuons10 + t.nElectrons10 + t.nTaus20;
      //cout <<"t.met_pt, t.ht, t.nJet40, t.nBJet40, t.deltaPhiMin, t.diffMetMht, nlepveto_, t.jet_pt[0], t.jet_pt[1]"<<endl;
      //cout<<t.met_pt<<" "<<t.ht<<" "<<t.nJet40<<" "<<t.nBJet40<<" "<<t.deltaPhiMin<<" "<<t.diffMetMht<<" "<<nlepveto_<<" "<<t.jet_pt[0]<<" "<<t.jet_pt[1]<<endl;

      fillHistosSignalRegion(h_1d_nocut, SignalRegionJets::nocut, SignalRegionHtMet::nocut, "nocut");

      fillHistosSignalRegion(h_1d_sr1H, SignalRegionJets::sr1, SignalRegionHtMet::h_ht, "sr1H");
      fillHistosSignalRegion(h_1d_sr2H, SignalRegionJets::sr2, SignalRegionHtMet::h_ht, "sr2H");
      fillHistosSignalRegion(h_1d_sr3H, SignalRegionJets::sr3, SignalRegionHtMet::h_ht, "sr3H");
      fillHistosSignalRegion(h_1d_sr4H, SignalRegionJets::sr4, SignalRegionHtMet::h_ht, "sr4H");
      fillHistosSignalRegion(h_1d_sr5H, SignalRegionJets::sr5, SignalRegionHtMet::h_ht, "sr5H");
      fillHistosSignalRegion(h_1d_sr6H, SignalRegionJets::sr6, SignalRegionHtMet::h_ht, "sr6H");
      fillHistosSignalRegion(h_1d_sr7H, SignalRegionJets::sr7, SignalRegionHtMet::h_ht, "sr7H");
      fillHistosSignalRegion(h_1d_sr8H, SignalRegionJets::sr8, SignalRegionHtMet::h_ht, "sr8H");
      fillHistosSignalRegion(h_1d_sr9H, SignalRegionJets::sr9, SignalRegionHtMet::h_ht, "sr9H");
      fillHistosSignalRegion(h_1d_sr1M, SignalRegionJets::sr1, SignalRegionHtMet::m_ht, "sr1M");
      fillHistosSignalRegion(h_1d_sr2M, SignalRegionJets::sr2, SignalRegionHtMet::m_ht, "sr2M");
      fillHistosSignalRegion(h_1d_sr3M, SignalRegionJets::sr3, SignalRegionHtMet::m_ht, "sr3M");
      fillHistosSignalRegion(h_1d_sr4M, SignalRegionJets::sr4, SignalRegionHtMet::m_ht, "sr4M");
      fillHistosSignalRegion(h_1d_sr5M, SignalRegionJets::sr5, SignalRegionHtMet::m_ht, "sr5M");
      fillHistosSignalRegion(h_1d_sr6M, SignalRegionJets::sr6, SignalRegionHtMet::m_ht, "sr6M");
      fillHistosSignalRegion(h_1d_sr7M, SignalRegionJets::sr7, SignalRegionHtMet::m_ht, "sr7M");
      fillHistosSignalRegion(h_1d_sr8M, SignalRegionJets::sr8, SignalRegionHtMet::m_ht, "sr8M");
      fillHistosSignalRegion(h_1d_sr9M, SignalRegionJets::sr9, SignalRegionHtMet::m_ht, "sr9M");
      fillHistosSignalRegion(h_1d_sr1L, SignalRegionJets::sr1, SignalRegionHtMet::l_ht, "sr1L");
      fillHistosSignalRegion(h_1d_sr2L, SignalRegionJets::sr2, SignalRegionHtMet::l_ht, "sr2L");
      fillHistosSignalRegion(h_1d_sr3L, SignalRegionJets::sr3, SignalRegionHtMet::l_ht, "sr3L");
      fillHistosSignalRegion(h_1d_sr4L, SignalRegionJets::sr4, SignalRegionHtMet::l_ht, "sr4L");
      fillHistosSignalRegion(h_1d_sr5L, SignalRegionJets::sr5, SignalRegionHtMet::l_ht, "sr5L");
      fillHistosSignalRegion(h_1d_sr6L, SignalRegionJets::sr6, SignalRegionHtMet::l_ht, "sr6L");
      fillHistosSignalRegion(h_1d_sr7L, SignalRegionJets::sr7, SignalRegionHtMet::l_ht, "sr7L");
      fillHistosSignalRegion(h_1d_sr8L, SignalRegionJets::sr8, SignalRegionHtMet::l_ht, "sr8L");
      fillHistosSignalRegion(h_1d_sr9L, SignalRegionJets::sr9, SignalRegionHtMet::l_ht, "sr9L");

   }//end loop on events in a file
  
    delete tree;
    f.Close();
  }//end loop on files
  
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
  savePlotsDir(h_1d_sr1H,outfile_,"sr1H");
  savePlotsDir(h_1d_sr2H,outfile_,"sr2H");
  savePlotsDir(h_1d_sr3H,outfile_,"sr3H");
  savePlotsDir(h_1d_sr4H,outfile_,"sr4H");
  savePlotsDir(h_1d_sr5H,outfile_,"sr5H");
  savePlotsDir(h_1d_sr6H,outfile_,"sr6H");
  savePlotsDir(h_1d_sr7H,outfile_,"sr7H");
  savePlotsDir(h_1d_sr8H,outfile_,"sr8H");
  savePlotsDir(h_1d_sr9H,outfile_,"sr9H");
  savePlotsDir(h_1d_sr1M,outfile_,"sr1M");
  savePlotsDir(h_1d_sr2M,outfile_,"sr2M");
  savePlotsDir(h_1d_sr3M,outfile_,"sr3M");
  savePlotsDir(h_1d_sr4M,outfile_,"sr4M");
  savePlotsDir(h_1d_sr5M,outfile_,"sr5M");
  savePlotsDir(h_1d_sr6M,outfile_,"sr6M");
  savePlotsDir(h_1d_sr7M,outfile_,"sr7M");
  savePlotsDir(h_1d_sr8M,outfile_,"sr8M");
  savePlotsDir(h_1d_sr9M,outfile_,"sr9M");
  savePlotsDir(h_1d_sr1L,outfile_,"sr1L");
  savePlotsDir(h_1d_sr2L,outfile_,"sr2L");
  savePlotsDir(h_1d_sr3L,outfile_,"sr3L");
  savePlotsDir(h_1d_sr4L,outfile_,"sr4L");
  savePlotsDir(h_1d_sr5L,outfile_,"sr5L");
  savePlotsDir(h_1d_sr6L,outfile_,"sr6L");
  savePlotsDir(h_1d_sr7L,outfile_,"sr7L");
  savePlotsDir(h_1d_sr8L,outfile_,"sr8L");
  savePlotsDir(h_1d_sr9L,outfile_,"sr9L");


  //---------------------
  // Write and Close file
  //---------------------
  outfile_->Write();
  outfile_->Close();
  delete outfile_;


  return;
}

void MT2Looper::fillHistosSignalRegion(std::map<std::string, TH1F*>& h_1d, const SignalRegionJets::value_type& sr_jets, const SignalRegionHtMet::value_type& sr_htmet, const std::string& dir, const std::string& suffix) {

 if ( PassesSignalRegion(t.met_pt, t.ht, t.nJet40, t.nBJet40, t.deltaPhiMin, t.diffMetMht, 
			 nlepveto_, t.jet_pt[0], t.jet_pt[1], sr_jets, sr_htmet) ) 
   {
     plot1D("h_SignalRegion",  sr_jets+sr_htmet,   evtweight_, h_1d_global, 100, 0, 100);
     fillHistos( h_1d, sr_jets, sr_htmet, dir, suffix);
   }
 return;
}

void MT2Looper::fillHistos(std::map<std::string, TH1F*>& h_1d, const SignalRegionJets::value_type& sr_jets, const SignalRegionHtMet::value_type& sr_htmet, const std::string& dirname, const std::string& s) {
  TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
  if (dir == 0) {
    dir = outfile_->mkdir(dirname.c_str());
  } 
  dir->cd();

  plot1D("h_Events"+s,  1, 1, h_1d, 1, 0, 2);
  plot1D("h_Events_w"+s,  1,   evtweight_, h_1d, 1, 0, 2);
  plot1D("h_mt2"+s,       t.mt2,   evtweight_, h_1d, 80, 0, 800);
  plot1D("h_met"+s,       t.met_pt,   evtweight_, h_1d, 80, 0, 800);
  plot1D("h_ht"+s,       t.ht,   evtweight_, h_1d, 80, 0, 2000);
  plot1D("h_nJet40"+s,       t.nJet40,   evtweight_, h_1d, 15, 0, 15);
  plot1D("h_nBJet40"+s,      t.nBJet40,   evtweight_, h_1d, 6, 0, 6);
  plot1D("h_deltaPhiMin"+s,  t.deltaPhiMin,   evtweight_, h_1d, 32, 0, 3.2);
  plot1D("h_diffMetMht"+s,   t.diffMetMht,   evtweight_, h_1d, 80, 0, 200);
  plot1D("h_nlepveto"+s,     nlepveto_,   evtweight_, h_1d, 10, 0, 10);
  plot1D("h_J0pt"+s,       t.jet_pt[0],   evtweight_, h_1d, 80, 0, 800);
  plot1D("h_J1pt"+s,       t.jet_pt[1],   evtweight_, h_1d, 80, 0, 800);
  outfile_->cd();
  return;
}

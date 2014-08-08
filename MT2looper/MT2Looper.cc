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
#include "Plotting/PlotUtilities.h"


using namespace std;
using namespace mt2;

class mt2tree;

MT2Looper::MT2Looper(){
  // set some stuff here
}
MT2Looper::~MT2Looper(){

};

void MT2Looper::loop(TChain* chain, std::string baby_name){

  gROOT->cd();
  TString m_outfilename_ = "test.root";
  cout << "[MT2Looper::loop] creating output file: " << m_outfilename_ << endl;

  outfile_ = new TFile(m_outfilename_,"RECREATE") ; 

  cout << "[MT2Looper::loop] setting up histos" << endl;
  std::map<std::string, TH1F*> h_1d;
  
  
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
      // set weights and start making plots
      //---------------------
      outfile_->cd();
      evtweight = 1;
      plot1D("h_nvtx",       t.nVert,       evtweight, h_1d, 40, 0, 40);
      plot1D("h_mt2",       t.mt2,       evtweight, h_1d, 80, 0, 800);

      nlep = t.nMuons10 + t.nElectrons10 + t.nTaus20;
      //cout <<"t.met_pt, t.ht, t.nJet40, t.nBJet40, t.deltaPhiMin, t.diffMetMht, nlep, t.jet_pt[0], t.jet_pt[1]"<<endl;
      //cout<<t.met_pt<<" "<<t.ht<<" "<<t.nJet40<<" "<<t.nBJet40<<" "<<t.deltaPhiMin<<" "<<t.diffMetMht<<" "<<nlep<<" "<<t.jet_pt[0]<<" "<<t.jet_pt[1]<<endl;

      fillHistos(h_1d, SignalRegionJets::sr0, SignalRegionHtMet::inclusive, "sr0");

     

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
  savePlotsDir(h_1d,outfile_,"");
 
  
  //---------------------
  // Write and Close file
  //---------------------
  outfile_->Write();
  outfile_->Close();
  delete outfile_;


  return;
}

void MT2Looper::fillHistos(std::map<std::string, TH1F*>& h_1d, const SignalRegionJets::value_type& sr_jets, const SignalRegionHtMet::value_type& sr_htmet, std::string name) {

 if ( PassesSignalRegion(t.met_pt, t.ht, t.nJet40, t.nBJet40, t.deltaPhiMin, t.diffMetMht, nlep, t.jet_pt[0], t.jet_pt[1], 
			 sr_jets, sr_htmet) ) 
	plot1D("h_mt2_"+name,       t.mt2,   evtweight, h_1d, 80, 0, 800);
}

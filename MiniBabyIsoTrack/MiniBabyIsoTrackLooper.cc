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
#include "../CORE/Tools/utils.h"

// header
#include "MiniBabyIsoTrackLooper.h"

using namespace std;
using namespace mt2;

class mt2tree;

MiniBabyIsoTrackLooper::MiniBabyIsoTrackLooper(){
  // set some stuff here
}
MiniBabyIsoTrackLooper::~MiniBabyIsoTrackLooper(){

};

void MiniBabyIsoTrackLooper::SetSignalRegions(){

  SRBase.SetName("srbase");
  SRBase.SetVar("mt2", 200, -1);
  SRBase.SetVar("j1pt", 100, -1);
  SRBase.SetVar("j2pt", 100, -1);
  SRBase.SetVar("deltaPhiMin", 0.3, -1);
  SRBase.SetVar("diffMetMhtOverMet", 0, 0.5);
  SRBase.SetVar("nlep", 0, 1);
  SRBase.SetVar("passesHtMet", 1, 2);

}

void MiniBabyIsoTrackLooper::loop(TChain* chain, std::string output_name){

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  gROOT->cd();
  cout << "[MiniBabyIsoTrackLooper::loop] creating output file: " << output_name << endl;

  outfile_ = new TFile(output_name.c_str(),"RECREATE") ; 
  makeTree(chain);

  SetSignalRegions();

  // File Loop
  int nDuplicates = 0;
  int nEvents = chain->GetEntries();
  unsigned int nEventsChain = nEvents;
  cout << "[MiniBabyIsoTrackLooper::loop] running on " << nEventsChain << " events" << endl;
  unsigned int nEventsTotal = 0;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;
  while ( (currentFile = (TFile*)fileIter.Next()) ) {
    cout << "[MiniBabyIsoTrackLooper::loop] running on file: " << currentFile->GetTitle() << endl;

    // Get File Content
    TFile f( currentFile->GetTitle() );
    TTree *tree = (TTree*)f.Get("mt2");
    TTreeCache::SetLearnEntries(10);
    tree->SetCacheSize(128*1024*1024);
    //mt2tree t(tree);
    t.Init(tree);

    tree->CopyAddresses(outtree_);

    // Event Loop
    unsigned int nEventsTree = tree->GetEntriesFast();
    for( unsigned int event = 0; event < nEventsTree; ++event) {
    //    for( unsigned int event = 0; event < nEventsTree/10.; ++event) {
    //    for( unsigned int event = 0; event < 100.; ++event) {

      t.GetEntry(event);

      initBaby();

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
      // event and signal region selection, weights
      //---------------------

      const float lumi = 1.;
      weight_ = t.evt_scale1fb * lumi;

      passevt_ = (int)(t.nVert > 0);
      int nlepveto = t.nMuons10 + t.nElectrons10 + t.nPFLep5LowMT + t.nPFHad10LowMT;

      std::map<std::string, float> values;
      values["deltaPhiMin"] = t.deltaPhiMin;
      values["diffMetMhtOverMet"]  = t.diffMetMht/t.met_pt;
      values["nlep"]        = nlepveto;
      values["j1pt"]        = t.jet_pt[0];
      values["j2pt"]        = t.jet_pt[1];
      values["mt2"]         = t.mt2;
      values["passesHtMet"] = ( (t.ht > 450. && t.met_pt > 200.) || (t.ht > 1000. && t.met_pt > 30.) );

      sigbase_ = (int) SRBase.PassesSelection(values);

      // set nlep to 0 to check SRbase without lepton veto
      values["nlep"]        = 0;
      sigbase_nolepveto_ = (int) SRBase.PassesSelection(values);

      // need to save: 
      //  - gen match for iso tracks?

      for (int itrk = 0; itrk < t.nisoTrack; ++itrk) {
	int pdgId = abs(t.isoTrack_pdgId[itrk]);
	float pt = t.isoTrack_pt[itrk];
	float absIso = t.isoTrack_absIso[itrk];
	float relIso = absIso/pt;
	float mt = MT(pt, t.isoTrack_phi[itrk], t.met_pt, t.met_phi);

	// pf leptons
	if (pdgId == 11 || pdgId == 13) {

	  if (pdgId == 11) {
	    if (pt > 5. && relIso < 0.20) veto_pfel5reliso20_++;
	    if (pt > 5. && relIso < 0.15) veto_pfel5reliso15_++;
	    if (pt > 5. && relIso < 0.10) veto_pfel5reliso10_++;
	    if (pt > 5. && relIso < 0.20 && mt < 100.) veto_pfel5reliso20mt100_++;
	    if (pt > 5. && relIso < 0.15 && mt < 100.) veto_pfel5reliso15mt100_++;
	    if (pt > 5. && relIso < 0.10 && mt < 100.) veto_pfel5reliso10mt100_++;

	    if (pt > 5. && absIso < 10.) veto_pfel5absiso10_++;
	    if (pt > 5. && absIso < 5.) veto_pfel5absiso5_++;
	    if (pt > 5. && absIso < 3.) veto_pfel5absiso3_++;
	    if (pt > 5. && absIso < 10. && mt < 100.) veto_pfel5absiso10mt100_++;
	    if (pt > 5. && absIso < 5. && mt < 100.) veto_pfel5absiso5mt100_++;
	    if (pt > 5. && absIso < 3. && mt < 100.) veto_pfel5absiso3mt100_++;

	  }
	  else if (pdgId == 13) {
	    if (pt > 5. && relIso < 0.20) veto_pfmu5reliso20_++;
	    if (pt > 5. && relIso < 0.15) veto_pfmu5reliso15_++;
	    if (pt > 5. && relIso < 0.10) veto_pfmu5reliso10_++;
	    if (pt > 5. && relIso < 0.20 && mt < 100.) veto_pfmu5reliso20mt100_++;
	    if (pt > 5. && relIso < 0.15 && mt < 100.) veto_pfmu5reliso15mt100_++;
	    if (pt > 5. && relIso < 0.10 && mt < 100.) veto_pfmu5reliso10mt100_++;

	    if (pt > 5. && absIso < 10.) veto_pfmu5absiso10_++;
	    if (pt > 5. && absIso < 5.) veto_pfmu5absiso5_++;
	    if (pt > 5. && absIso < 3.) veto_pfmu5absiso3_++;
	    if (pt > 5. && absIso < 10. && mt < 100.) veto_pfmu5absiso10mt100_++;
	    if (pt > 5. && absIso < 5. && mt < 100.) veto_pfmu5absiso5mt100_++;
	    if (pt > 5. && absIso < 3. && mt < 100.) veto_pfmu5absiso3mt100_++;

	  }

	} // pf leptons

	// pf hadrons
	else if (pdgId == 211) {

	  if (pt > 5. && relIso < 0.20) veto_pfhad5reliso20_++;
	  if (pt > 5. && relIso < 0.15) veto_pfhad5reliso15_++;
	  if (pt > 5. && relIso < 0.10) veto_pfhad5reliso10_++;
	  if (pt > 5. && relIso < 0.20 && mt < 100.) veto_pfhad5reliso20mt100_++;
	  if (pt > 5. && relIso < 0.15 && mt < 100.) veto_pfhad5reliso15mt100_++;
	  if (pt > 5. && relIso < 0.10 && mt < 100.) veto_pfhad5reliso10mt100_++;

	  if (pt > 5. && absIso < 10.) veto_pfhad5absiso10_++;
	  if (pt > 5. && absIso < 5.) veto_pfhad5absiso5_++;
	  if (pt > 5. && absIso < 3.) veto_pfhad5absiso3_++;
	  if (pt > 5. && absIso < 10. && mt < 100.) veto_pfhad5absiso10mt100_++;
	  if (pt > 5. && absIso < 5. && mt < 100.) veto_pfhad5absiso5mt100_++;
	  if (pt > 5. && absIso < 3. && mt < 100.) veto_pfhad5absiso3mt100_++;

	  if (pt > 10. && relIso < 0.20) veto_pfhad10reliso20_++;
	  if (pt > 10. && relIso < 0.15) veto_pfhad10reliso15_++;
	  if (pt > 10. && relIso < 0.10) veto_pfhad10reliso10_++;
	  if (pt > 10. && relIso < 0.20 && mt < 100.) veto_pfhad10reliso20mt100_++;
	  if (pt > 10. && relIso < 0.15 && mt < 100.) veto_pfhad10reliso15mt100_++;
	  if (pt > 10. && relIso < 0.10 && mt < 100.) veto_pfhad10reliso10mt100_++;

	  if (pt > 10. && absIso < 10.) veto_pfhad10absiso10_++;
	  if (pt > 10. && absIso < 5.) veto_pfhad10absiso5_++;
	  if (pt > 10. && absIso < 3.) veto_pfhad10absiso3_++;
	  if (pt > 10. && absIso < 10. && mt < 100.) veto_pfhad10absiso10mt100_++;
	  if (pt > 10. && absIso < 5. && mt < 100.) veto_pfhad10absiso5mt100_++;
	  if (pt > 10. && absIso < 3. && mt < 100.) veto_pfhad10absiso3mt100_++;

	} // pf hadrons

      } // isoTrack loop

      // leading reco lepton
      if (t.nlep > 0) {
	lep1_pt_ = t.lep_pt[0];
	lep1_eta_ = t.lep_eta[0];
	lep1_phi_ = t.lep_phi[0];
	lep1_mt_ = MT(lep1_pt_, lep1_phi_, t.met_pt, t.met_phi);
      }

      // loop over reco leptons
      for (int ilep = 0; ilep < t.nlep; ++ilep) {
	float pt = t.lep_pt[ilep];
	float mt = MT(pt, t.lep_phi[ilep], t.met_pt, t.met_phi);
	// electrons
	if (abs(t.lep_pdgId[ilep]) == 11) {

	  // apply some isolation cut here..

	  // electron passes sel - save decision
	  if (pt > 10.) veto_el10_++;
	  if (pt > 15.) veto_el15_++;
	  if (pt > 20.) veto_el20_++;
	  if (pt > 10. && mt < 100.) veto_el10mt100_++;
	  if (pt > 15. && mt < 100.) veto_el15mt100_++;
	  if (pt > 20. && mt < 100.) veto_el20mt100_++;
	}

	// muons
	else if (abs(t.lep_pdgId[ilep]) == 13) {

	  // apply some isolation cut here..

	  if (pt > 10.) veto_mu10_++;
	  if (pt > 15.) veto_mu15_++;
	  if (pt > 20.) veto_mu20_++;
	  if (pt > 10. && mt < 100.) veto_mu10mt100_++;
	  if (pt > 15. && mt < 100.) veto_mu15mt100_++;
	  if (pt > 20. && mt < 100.) veto_mu20mt100_++;
	}

      } // loop on reco leps


      // leading reco tau
      if (t.ntau > 0) {
	tau1_pt_ = t.tau_pt[0];
	tau1_eta_ = t.tau_eta[0];
	tau1_phi_ = t.tau_phi[0];
	tau1_mt_ = MT(tau1_pt_, tau1_phi_, t.met_pt, t.met_phi);
      }

      // fill output tree
      outtree_->Fill();

   }//end loop on events in a file
  
    delete tree;
    f.Close();
  }//end loop on files
  
  cout << "[MiniBabyIsoTrackLooper::loop] processed " << nEventsTotal << " events" << endl;
  if ( nEventsChain != nEventsTotal ) {
    std::cout << "ERROR: number of events from files is not equal to total number of events" << std::endl;
  }

  cout << nDuplicates << " duplicate events were skipped." << endl;

  //---------------------
  // Write and Close file
  //---------------------
  outfile_->cd();
  outtree_->Write();
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


//--------------------------------------------
// create the tree and set branch addresses
//--------------------------------------------

void MiniBabyIsoTrackLooper::makeTree(TChain* chain){

  outfile_->cd();
  outtree_ = chain->CloneTree(0);

  // define minibaby-specific branches
  outtree_->Branch("mini_weight"        , &weight_        ,  "mini_weight/F"	 );
  outtree_->Branch("mini_passevt"       , &passevt_       ,  "mini_passevt/I"	 );
  outtree_->Branch("mini_sigbase"       , &sigbase_       ,  "mini_sigbase/I"	 );
  outtree_->Branch("mini_sigbase_nolepveto"       , &sigbase_nolepveto_       ,  "mini_sigbase_nolepveto/I"	 );

  outtree_->Branch("mini_veto_mu10"      , &veto_mu10_      ,  "mini_veto_mu10/I"	 );
  outtree_->Branch("mini_veto_mu15"      , &veto_mu15_      ,  "mini_veto_mu15/I"	 );
  outtree_->Branch("mini_veto_mu20"      , &veto_mu20_      ,  "mini_veto_mu20/I"	 );
  outtree_->Branch("mini_veto_mu10mt100"      , &veto_mu10mt100_      ,  "mini_veto_mu10mt100/I"	 );
  outtree_->Branch("mini_veto_mu15mt100"      , &veto_mu15mt100_      ,  "mini_veto_mu15mt100/I"	 );
  outtree_->Branch("mini_veto_mu20mt100"      , &veto_mu20mt100_      ,  "mini_veto_mu20mt100/I"	 );

  outtree_->Branch("mini_veto_el10"      , &veto_el10_      ,  "mini_veto_el10/I"	 );
  outtree_->Branch("mini_veto_el15"      , &veto_el15_      ,  "mini_veto_el15/I"	 );
  outtree_->Branch("mini_veto_el20"      , &veto_el20_      ,  "mini_veto_el20/I"	 );
  outtree_->Branch("mini_veto_el10mt100"      , &veto_el10mt100_      ,  "mini_veto_el10mt100/I"	 );
  outtree_->Branch("mini_veto_el15mt100"      , &veto_el15mt100_      ,  "mini_veto_el15mt100/I"	 );
  outtree_->Branch("mini_veto_el20mt100"      , &veto_el20mt100_      ,  "mini_veto_el20mt100/I"	 );

  outtree_->Branch("mini_veto_pfmu5reliso20"      , &veto_pfmu5reliso20_      ,  "mini_veto_pfmu5reliso20/I"	 );
  outtree_->Branch("mini_veto_pfmu5reliso15"      , &veto_pfmu5reliso15_      ,  "mini_veto_pfmu5reliso15/I"	 );
  outtree_->Branch("mini_veto_pfmu5reliso10"      , &veto_pfmu5reliso10_      ,  "mini_veto_pfmu5reliso10/I"	 );
  outtree_->Branch("mini_veto_pfmu5reliso20mt100"      , &veto_pfmu5reliso20mt100_      ,  "mini_veto_pfmu5reliso20mt100/I"	 );
  outtree_->Branch("mini_veto_pfmu5reliso15mt100"      , &veto_pfmu5reliso15mt100_      ,  "mini_veto_pfmu5reliso15mt100/I"	 );
  outtree_->Branch("mini_veto_pfmu5reliso10mt100"      , &veto_pfmu5reliso10mt100_      ,  "mini_veto_pfmu5reliso10mt100/I"	 );

  outtree_->Branch("mini_veto_pfmu5absiso10"      , &veto_pfmu5absiso10_      ,  "mini_veto_pfmu5absiso10/I"	 );
  outtree_->Branch("mini_veto_pfmu5absiso5"      , &veto_pfmu5absiso5_      ,  "mini_veto_pfmu5absiso5/I"	 );
  outtree_->Branch("mini_veto_pfmu5absiso3"      , &veto_pfmu5absiso3_      ,  "mini_veto_pfmu5absiso3/I"	 );
  outtree_->Branch("mini_veto_pfmu5absiso10mt100"      , &veto_pfmu5absiso10mt100_      ,  "mini_veto_pfmu5absiso10mt100/I"	 );
  outtree_->Branch("mini_veto_pfmu5absiso5mt100"      , &veto_pfmu5absiso5mt100_      ,  "mini_veto_pfmu5absiso5mt100/I"	 );
  outtree_->Branch("mini_veto_pfmu5absiso3mt100"      , &veto_pfmu5absiso3mt100_      ,  "mini_veto_pfmu5absiso3mt100/I"	 );

  outtree_->Branch("mini_veto_pfel5reliso20"      , &veto_pfel5reliso20_      ,  "mini_veto_pfel5reliso20/I"	 );
  outtree_->Branch("mini_veto_pfel5reliso15"      , &veto_pfel5reliso15_      ,  "mini_veto_pfel5reliso15/I"	 );
  outtree_->Branch("mini_veto_pfel5reliso10"      , &veto_pfel5reliso10_      ,  "mini_veto_pfel5reliso10/I"	 );
  outtree_->Branch("mini_veto_pfel5reliso20mt100"      , &veto_pfel5reliso20mt100_      ,  "mini_veto_pfel5reliso20mt100/I"	 );
  outtree_->Branch("mini_veto_pfel5reliso15mt100"      , &veto_pfel5reliso15mt100_      ,  "mini_veto_pfel5reliso15mt100/I"	 );
  outtree_->Branch("mini_veto_pfel5reliso10mt100"      , &veto_pfel5reliso10mt100_      ,  "mini_veto_pfel5reliso10mt100/I"	 );

  outtree_->Branch("mini_veto_pfel5absiso10"      , &veto_pfel5absiso10_      ,  "mini_veto_pfel5absiso10/I"	 );
  outtree_->Branch("mini_veto_pfel5absiso5"      , &veto_pfel5absiso5_      ,  "mini_veto_pfel5absiso5/I"	 );
  outtree_->Branch("mini_veto_pfel5absiso3"      , &veto_pfel5absiso3_      ,  "mini_veto_pfel5absiso3/I"	 );
  outtree_->Branch("mini_veto_pfel5absiso10mt100"      , &veto_pfel5absiso10mt100_      ,  "mini_veto_pfel5absiso10mt100/I"	 );
  outtree_->Branch("mini_veto_pfel5absiso5mt100"      , &veto_pfel5absiso5mt100_      ,  "mini_veto_pfel5absiso5mt100/I"	 );
  outtree_->Branch("mini_veto_pfel5absiso3mt100"      , &veto_pfel5absiso3mt100_      ,  "mini_veto_pfel5absiso3mt100/I"	 );

  outtree_->Branch("mini_veto_pfhad5reliso20"      , &veto_pfhad5reliso20_      ,  "mini_veto_pfhad5reliso20/I"	 );
  outtree_->Branch("mini_veto_pfhad5reliso15"      , &veto_pfhad5reliso15_      ,  "mini_veto_pfhad5reliso15/I"	 );
  outtree_->Branch("mini_veto_pfhad5reliso10"      , &veto_pfhad5reliso10_      ,  "mini_veto_pfhad5reliso10/I"	 );
  outtree_->Branch("mini_veto_pfhad5reliso20mt100"      , &veto_pfhad5reliso20mt100_      ,  "mini_veto_pfhad5reliso20mt100/I"	 );
  outtree_->Branch("mini_veto_pfhad5reliso15mt100"      , &veto_pfhad5reliso15mt100_      ,  "mini_veto_pfhad5reliso15mt100/I"	 );
  outtree_->Branch("mini_veto_pfhad5reliso10mt100"      , &veto_pfhad5reliso10mt100_      ,  "mini_veto_pfhad5reliso10mt100/I"	 );

  outtree_->Branch("mini_veto_pfhad5absiso10"      , &veto_pfhad5absiso10_      ,  "mini_veto_pfhad5absiso10/I"	 );
  outtree_->Branch("mini_veto_pfhad5absiso5"      , &veto_pfhad5absiso5_      ,  "mini_veto_pfhad5absiso5/I"	 );
  outtree_->Branch("mini_veto_pfhad5absiso3"      , &veto_pfhad5absiso3_      ,  "mini_veto_pfhad5absiso3/I"	 );
  outtree_->Branch("mini_veto_pfhad5absiso10mt100"      , &veto_pfhad5absiso10mt100_      ,  "mini_veto_pfhad5absiso10mt100/I"	 );
  outtree_->Branch("mini_veto_pfhad5absiso5mt100"      , &veto_pfhad5absiso5mt100_      ,  "mini_veto_pfhad5absiso5mt100/I"	 );
  outtree_->Branch("mini_veto_pfhad5absiso3mt100"      , &veto_pfhad5absiso3mt100_      ,  "mini_veto_pfhad5absiso3mt100/I"	 );

  outtree_->Branch("mini_veto_pfhad10reliso20"      , &veto_pfhad10reliso20_      ,  "mini_veto_pfhad10reliso20/I"	 );
  outtree_->Branch("mini_veto_pfhad10reliso15"      , &veto_pfhad10reliso15_      ,  "mini_veto_pfhad10reliso15/I"	 );
  outtree_->Branch("mini_veto_pfhad10reliso10"      , &veto_pfhad10reliso10_      ,  "mini_veto_pfhad10reliso10/I"	 );
  outtree_->Branch("mini_veto_pfhad10reliso20mt100"      , &veto_pfhad10reliso20mt100_      ,  "mini_veto_pfhad10reliso20mt100/I"	 );
  outtree_->Branch("mini_veto_pfhad10reliso15mt100"      , &veto_pfhad10reliso15mt100_      ,  "mini_veto_pfhad10reliso15mt100/I"	 );
  outtree_->Branch("mini_veto_pfhad10reliso10mt100"      , &veto_pfhad10reliso10mt100_      ,  "mini_veto_pfhad10reliso10mt100/I"	 );

  outtree_->Branch("mini_veto_pfhad10absiso10"      , &veto_pfhad10absiso10_      ,  "mini_veto_pfhad10absiso10/I"	 );
  outtree_->Branch("mini_veto_pfhad10absiso5"      , &veto_pfhad10absiso5_      ,  "mini_veto_pfhad10absiso5/I"	 );
  outtree_->Branch("mini_veto_pfhad10absiso3"      , &veto_pfhad10absiso3_      ,  "mini_veto_pfhad10absiso3/I"	 );
  outtree_->Branch("mini_veto_pfhad10absiso10mt100"      , &veto_pfhad10absiso10mt100_      ,  "mini_veto_pfhad10absiso10mt100/I"	 );
  outtree_->Branch("mini_veto_pfhad10absiso5mt100"      , &veto_pfhad10absiso5mt100_      ,  "mini_veto_pfhad10absiso5mt100/I"	 );
  outtree_->Branch("mini_veto_pfhad10absiso3mt100"      , &veto_pfhad10absiso3mt100_      ,  "mini_veto_pfhad10absiso3mt100/I"	 );

  outtree_->Branch("mini_lep1_pt"        , &lep1_pt_        ,  "mini_lep1_pt/F"	 );
  outtree_->Branch("mini_lep1_eta"        , &lep1_eta_        ,  "mini_lep1_eta/F"	 );
  outtree_->Branch("mini_lep1_phi"        , &lep1_phi_        ,  "mini_lep1_phi/F"	 );
  outtree_->Branch("mini_lep1_mt"        , &lep1_mt_        ,  "mini_lep1_mt/F"	 );

  outtree_->Branch("mini_tau1_pt"        , &tau1_pt_        ,  "mini_tau1_pt/F"	 );
  outtree_->Branch("mini_tau1_eta"        , &tau1_eta_        ,  "mini_tau1_eta/F"	 );
  outtree_->Branch("mini_tau1_phi"        , &tau1_phi_        ,  "mini_tau1_phi/F"	 );
  outtree_->Branch("mini_tau1_mt"        , &tau1_mt_        ,  "mini_tau1_mt/F"	 );

  cout << "Removing all branches and only storing the needed ones" << endl;

  // enable only wanted branches
  outtree_->SetBranchStatus("*",0); 

  outtree_->SetBranchStatus("evt_*",1); 
  outtree_->SetBranchStatus("run",1); 
  outtree_->SetBranchStatus("lumi",1); 
  outtree_->SetBranchStatus("evt",1); 
  outtree_->SetBranchStatus("isData",1); 
  outtree_->SetBranchStatus("puWeight",1); 
  outtree_->SetBranchStatus("nVert",1); 
  outtree_->SetBranchStatus("nTrueInt",1); 
  outtree_->SetBranchStatus("nJet40",1); 
  outtree_->SetBranchStatus("nBJet40",1); 
  outtree_->SetBranchStatus("nMuons10",1); 
  outtree_->SetBranchStatus("nElectrons10",1); 
  outtree_->SetBranchStatus("nTaus20",1); 
  outtree_->SetBranchStatus("nPFLep5LowMT",1); 
  outtree_->SetBranchStatus("nPFHad10LowMT",1); 
  outtree_->SetBranchStatus("deltaPhiMin",1); 
  outtree_->SetBranchStatus("diffMetMht",1); 
  outtree_->SetBranchStatus("ht",1); 
  outtree_->SetBranchStatus("mt2",1); 
  outtree_->SetBranchStatus("met_pt",1); 
  outtree_->SetBranchStatus("met_phi",1); 
  outtree_->SetBranchStatus("HLT_HT900",1); 
  outtree_->SetBranchStatus("HLT_MET170",1); 
  outtree_->SetBranchStatus("HLT_ht350met120",1); 
  outtree_->SetBranchStatus("nlep",1); 
  outtree_->SetBranchStatus("nisoTrack",1); 
  outtree_->SetBranchStatus("ntau",1); 
  outtree_->SetBranchStatus("ngenLep",1); 
  outtree_->SetBranchStatus("ngenTau",1); 
  outtree_->SetBranchStatus("ngenLepFromTau",1); 

  // enable mini branches
  outtree_->SetBranchStatus("mini_*",1); 

}

void MiniBabyIsoTrackLooper::initBaby(){

  weight_     = 1.;

  passevt_    = -1;
  sigbase_    = -1;
  sigbase_nolepveto_    = -1;

  // veto bits are 0 by default
  veto_mu10_ = 0;
  veto_mu15_ = 0;
  veto_mu20_ = 0;
  veto_mu10mt100_ = 0;
  veto_mu15mt100_ = 0;
  veto_mu20mt100_ = 0;

  veto_el10_ = 0;
  veto_el15_ = 0;
  veto_el20_ = 0;
  veto_el10mt100_ = 0;
  veto_el15mt100_ = 0;
  veto_el20mt100_ = 0;

  veto_pfmu5reliso20_ = 0;
  veto_pfmu5reliso15_ = 0;
  veto_pfmu5reliso10_ = 0;
  veto_pfmu5reliso20mt100_ = 0;
  veto_pfmu5reliso15mt100_ = 0;
  veto_pfmu5reliso10mt100_ = 0;

  veto_pfmu5absiso10_ = 0;
  veto_pfmu5absiso5_ = 0;
  veto_pfmu5absiso3_ = 0;
  veto_pfmu5absiso10mt100_ = 0;
  veto_pfmu5absiso5mt100_ = 0;
  veto_pfmu5absiso3mt100_ = 0;

  veto_pfel5reliso20_ = 0;
  veto_pfel5reliso15_ = 0;
  veto_pfel5reliso10_ = 0;
  veto_pfel5reliso20mt100_ = 0;
  veto_pfel5reliso15mt100_ = 0;
  veto_pfel5reliso10mt100_ = 0;

  veto_pfel5absiso10_ = 0;
  veto_pfel5absiso5_ = 0;
  veto_pfel5absiso3_ = 0;
  veto_pfel5absiso10mt100_ = 0;
  veto_pfel5absiso5mt100_ = 0;
  veto_pfel5absiso3mt100_ = 0;

  veto_pfhad5reliso20_ = 0;
  veto_pfhad5reliso15_ = 0;
  veto_pfhad5reliso10_ = 0;
  veto_pfhad5reliso20mt100_ = 0;
  veto_pfhad5reliso15mt100_ = 0;
  veto_pfhad5reliso10mt100_ = 0;

  veto_pfhad5absiso10_ = 0;
  veto_pfhad5absiso5_ = 0;
  veto_pfhad5absiso3_ = 0;
  veto_pfhad5absiso10mt100_ = 0;
  veto_pfhad5absiso5mt100_ = 0;
  veto_pfhad5absiso3mt100_ = 0;

  veto_pfhad10reliso20_ = 0;
  veto_pfhad10reliso15_ = 0;
  veto_pfhad10reliso10_ = 0;
  veto_pfhad10reliso20mt100_ = 0;
  veto_pfhad10reliso15mt100_ = 0;
  veto_pfhad10reliso10mt100_ = 0;

  veto_pfhad10absiso10_ = 0;
  veto_pfhad10absiso5_ = 0;
  veto_pfhad10absiso3_ = 0;
  veto_pfhad10absiso10mt100_ = 0;
  veto_pfhad10absiso5mt100_ = 0;
  veto_pfhad10absiso3mt100_ = 0;

  lep1_pt_ = -1.;
  lep1_eta_ = -1.;
  lep1_phi_ = -1.;
  lep1_mt_ = -1.;

  tau1_pt_ = -1.;
  tau1_eta_ = -1.;
  tau1_phi_ = -1.;
  tau1_mt_ = -1.;

}


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
#include "MiniBabyIsoTrackLooper.h"

using namespace std;
using namespace mt2;

class mt2tree;

MiniBabyIsoTrackLooper::MiniBabyIsoTrackLooper(){
  // set some stuff here
}
MiniBabyIsoTrackLooper::~MiniBabyIsoTrackLooper(){

};

void MiniBabyIsoTrackLooper::loop(TChain* chain, std::string output_name){

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  gROOT->cd();
  cout << "[MiniBabyIsoTrackLooper::loop] creating output file: " << output_name << endl;

  outfile_ = new TFile(output_name.c_str(),"RECREATE") ; 
  makeTree(chain);


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

      passevt_ = (int)PassesEventSelection(t.nVert);
      int nlepveto = t.nMuons10 + t.nElectrons10 + t.nTaus20;
      sigbase_ = (int) PassesSignalRegionBase(t.met_pt, t.ht, t.nJet40, t.deltaPhiMin, t.diffMetMht, 
					      nlepveto, t.jet_pt[0], t.jet_pt[1]);
      sigjets_ = (int) PassesSignalRegionJets(t.nJet40, t.nBJet40);
      sightmet_ = (int) PassesSignalRegionHtMet(t.met_pt, t.ht);

      // need to save: 
      //  - gen match for iso tracks?

      // loop over isolated tracks to find highest pt and most isolated
      int isoTrack_lead_idx = -1;
      float isoTrack_lead_pt = -1.;
      int isoTrack_mostiso_idx = -1;
      float isoTrack_mostiso_iso = 99.;

      for (int itrk = 0; itrk < t.nisoTrack; ++itrk) {
	if (t.isoTrack_pt[itrk] > isoTrack_lead_pt) {
	  isoTrack_lead_idx = itrk;
	  isoTrack_lead_pt = t.isoTrack_pt[itrk];
	}
	if (t.isoTrack_absIso[itrk] < isoTrack_mostiso_iso) {
	  isoTrack_mostiso_idx = itrk;
	  isoTrack_mostiso_iso = t.isoTrack_absIso[itrk];
	}
      }

      // store isolated tracks
      if (isoTrack_lead_idx >= 0) {
	isoTrack_lead_pt_ = t.isoTrack_pt[isoTrack_lead_idx];
	isoTrack_lead_eta_ = t.isoTrack_eta[isoTrack_lead_idx];
	isoTrack_lead_phi_ = t.isoTrack_phi[isoTrack_lead_idx];
	isoTrack_lead_absIso_ = t.isoTrack_absIso[isoTrack_lead_idx];
	isoTrack_lead_dz_ = t.isoTrack_dz[isoTrack_lead_idx];
	isoTrack_lead_pdgId_ = t.isoTrack_pdgId[isoTrack_lead_idx];
      }
      if (isoTrack_mostiso_idx >= 0) {
	isoTrack_mostiso_pt_ = t.isoTrack_pt[isoTrack_mostiso_idx];
	isoTrack_mostiso_eta_ = t.isoTrack_eta[isoTrack_mostiso_idx];
	isoTrack_mostiso_phi_ = t.isoTrack_phi[isoTrack_mostiso_idx];
	isoTrack_mostiso_absIso_ = t.isoTrack_absIso[isoTrack_mostiso_idx];
	isoTrack_mostiso_dz_ = t.isoTrack_dz[isoTrack_mostiso_idx];
	isoTrack_mostiso_pdgId_ = t.isoTrack_pdgId[isoTrack_mostiso_idx];
      }

      // leading reco lepton
      if (t.nlep > 0) {
	lep1_pt_ = t.lep_pt[0];
	lep1_eta_ = t.lep_eta[0];
	lep1_phi_ = t.lep_phi[0];
      }

      // leading reco tau
      if (t.ntau > 0) {
	tau1_pt_ = t.tau_pt[0];
	tau1_eta_ = t.tau_eta[0];
	tau1_phi_ = t.tau_phi[0];
      }

      ngenel_ = 0;
      ngenmu_ = 0;
      ngentaulep_ = 0;
      ngentauhad_ = 0;
      int ngentau = 0;
      std::vector<int> gen_idx_save;

      // loop over gen particles to store lepton info
      for (int igen = 0; igen < t.ngenPart; ++igen) {
	// only care about leptons for now
	int pdgId = abs(t.genPart_pdgId[igen]);
	if ((pdgId != 11) && (pdgId != 13) && (pdgId != 15)) continue;

	int motherId = abs(t.genPart_motherId[igen]);
	int grandmaId = abs(t.genPart_grandmaId[igen]);

	bool save = false;
	// electrons, muons: status 1 and mother or grandmother W/Z or tau from W/Z
	if (((pdgId == 11) || (pdgId == 13)) && (t.genPart_status[igen] == 1)) {
	  // leptons from taus
	  if (motherId == 15 && (grandmaId == 24 || grandmaId == 23 || grandmaId == 15)) {
	    ++ngentaulep_;
	    if (pdgId == 11) ++ngenel_;
	    if (pdgId == 13) ++ngenmu_;
	    save = true;
	  } 
	  // leptons from W/Z
	  else if (motherId == 24 || grandmaId == 24 || motherId == 23 || grandmaId == 23) {
	    if (pdgId == 11) ++ngenel_;
	    if (pdgId == 13) ++ngenmu_;
	    save = true;
	  }
	}

	// taus: status 2. just count leptonic taus for now and infer number of hadronic taus.  Would need daughter info to separate
	//  one and three prong taus - could do in babyMaker if needed.
	if (pdgId == 15 && t.genPart_status[igen] == 2) {
	  if (motherId == 24 || grandmaId == 24 || motherId == 23 || grandmaId == 23) {
	    // look for decay products.. double loop is not optimal..
	    int lep = 0;
	    //	    int had = 0;
	    for (int jgen = 0; jgen < t.ngenPart; ++jgen) {
	      if (t.genPart_status[jgen] != 1) continue;
	      if ((abs(t.genPart_pdgId[jgen]) == 11 || abs(t.genPart_pdgId[jgen]) == 13) && (abs(t.genPart_motherId[jgen]) == 15)) {
		// check dR
		float dr = DeltaR(t.genPart_eta[igen], t.genPart_eta[jgen], t.genPart_phi[igen], t.genPart_phi[jgen]);
		// cout << "- found tau lepton, id: " << t.genPart_pdgId[jgen] << ", pt: " << t.genPart_pt[jgen] 
		//      << ", dr: " << dr << endl;
		if (dr > 0.5) continue;
		++lep;
		break;
	      }
	      // // hadronic taus: pion or kaon decays
	      // else if ((abs(t.genPart_pdgId[jgen]) == 211 || abs(t.genPart_pdgId[jgen]) == 321) && (abs(t.genPart_motherId[jgen]) == 15 || abs(t.genPart_grandmaId[jgen]) == 15)) {
	      // 	// check dR
	      // 	float dr = DeltaR(t.genPart_eta[igen], t.genPart_eta[jgen], t.genPart_phi[igen], t.genPart_phi[jgen]);
	      // 	if (dr > 0.5) continue;
	      // 	++had;
	      // 	if (had == 3) break;
	      // }
	    } // 2nd loop on genPart
	    // if (lep == 0 && had == 0) cout << "WARNING: couldn't classify tau decays, event: " << t.evt << endl;
	    //	    if (lep > 0) ++ngentaulep_;
	    if (lep == 0) {
	      //	      ++ngentauhad_;
	      save = true;
	    }
	    ++ngentau;
	  }
	}

	// good gen lepton
	if (save) {
	  // compare to leading gen lep
	  if (t.genPart_pt[igen] > genlep1_pt_) {
	    genlep2_pt_ = genlep1_pt_;
	    genlep2_eta_ = genlep1_eta_;
	    genlep2_phi_ = genlep1_phi_;
	    genlep2_pdgId_ = genlep1_pdgId_;

	    genlep1_pt_ = t.genPart_pt[igen];
	    genlep1_eta_ = t.genPart_eta[igen];
	    genlep1_phi_ = t.genPart_phi[igen];
	    genlep1_pdgId_ = t.genPart_pdgId[igen];
	  } else if (t.genPart_pt[igen] > genlep2_pt_) {
	    genlep2_pt_ = t.genPart_pt[igen];
	    genlep2_eta_ = t.genPart_eta[igen];
	    genlep2_phi_ = t.genPart_phi[igen];
	    genlep2_pdgId_ = t.genPart_pdgId[igen];
	  }
	}// if save

      } // loop over genPart

      ngentauhad_ = ngentau - ngentaulep_;
      ngenlep_ = ngentauhad_ + ngenel_ + ngenmu_;

      // if (ngentaulep_ != ngentaulep_daughters) {
      // 	cout << "WARNING: disagree on leptonic taus! from leps: " << ngentaulep_ << ", from taus: " << ngentaulep_daughters << endl;
      // }

      // if (ngentauhad_ != ngentauhad_daughters) {
      // 	cout << "WARNING: disagree on hadronic taus! from leps: " << ngentauhad_ << ", from taus: " << ngentauhad_daughters << endl;
      // }

      // // save some gen lepton info
      // if (ngenlep_ > 0) {
      // 	// only save hadronically decaying taus: 
      // }

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
  TDirectory *rootdir = gDirectory->GetDirectory("Rint:");
  rootdir->cd();

  outfile_->cd();
  outtree_ = chain->CloneTree(0);

  // define minibaby-specific branches
  outtree_->Branch("mini_weight"        , &weight_        ,  "mini_weight/F"	 );
  outtree_->Branch("mini_ngenlep"       , &ngenlep_       ,  "mini_ngenlep/I"	 );
  outtree_->Branch("mini_passevt"       , &passevt_       ,  "mini_passevt/I"	 );
  outtree_->Branch("mini_sigbase"       , &sigbase_       ,  "mini_sigbase/I"	 );
  outtree_->Branch("mini_sigjets"       , &sigjets_       ,  "mini_sigjets/I"	 );
  outtree_->Branch("mini_sightmet"      , &sightmet_      ,  "mini_sightmet/I"	 );

  outtree_->Branch("mini_isoTrack_lead_pt"        , &isoTrack_lead_pt_        ,  "mini_isoTrack_lead_pt/F"	 );
  outtree_->Branch("mini_isoTrack_lead_eta"        , &isoTrack_lead_eta_        ,  "mini_isoTrack_lead_eta/F"	 );
  outtree_->Branch("mini_isoTrack_lead_phi"        , &isoTrack_lead_phi_        ,  "mini_isoTrack_lead_phi/F"	 );
  outtree_->Branch("mini_isoTrack_lead_absIso"        , &isoTrack_lead_absIso_        ,  "mini_isoTrack_lead_absIso/F"	 );
  outtree_->Branch("mini_isoTrack_lead_dz"        , &isoTrack_lead_dz_        ,  "mini_isoTrack_lead_dz/F"	 );
  outtree_->Branch("mini_isoTrack_lead_pdgId"        , &isoTrack_lead_pdgId_        ,  "mini_isoTrack_lead_pdgId/I"	 );

  outtree_->Branch("mini_isoTrack_mostiso_pt"        , &isoTrack_mostiso_pt_        ,  "mini_isoTrack_mostiso_pt/F"	 );
  outtree_->Branch("mini_isoTrack_mostiso_eta"        , &isoTrack_mostiso_eta_        ,  "mini_isoTrack_mostiso_eta/F"	 );
  outtree_->Branch("mini_isoTrack_mostiso_phi"        , &isoTrack_mostiso_phi_        ,  "mini_isoTrack_mostiso_phi/F"	 );
  outtree_->Branch("mini_isoTrack_mostiso_absIso"        , &isoTrack_mostiso_absIso_        ,  "mini_isoTrack_mostiso_absIso/F"	 );
  outtree_->Branch("mini_isoTrack_mostiso_dz"        , &isoTrack_mostiso_dz_        ,  "mini_isoTrack_mostiso_dz/F"	 );
  outtree_->Branch("mini_isoTrack_mostiso_pdgId"        , &isoTrack_mostiso_pdgId_        ,  "mini_isoTrack_mostiso_pdgId/I"	 );

  outtree_->Branch("mini_lep1_pt"        , &lep1_pt_        ,  "mini_lep1_pt/F"	 );
  outtree_->Branch("mini_lep1_eta"        , &lep1_eta_        ,  "mini_lep1_eta/F"	 );
  outtree_->Branch("mini_lep1_phi"        , &lep1_phi_        ,  "mini_lep1_phi/F"	 );

  outtree_->Branch("mini_tau1_pt"        , &tau1_pt_        ,  "mini_tau1_pt/F"	 );
  outtree_->Branch("mini_tau1_eta"        , &tau1_eta_        ,  "mini_tau1_eta/F"	 );
  outtree_->Branch("mini_tau1_phi"        , &tau1_phi_        ,  "mini_tau1_phi/F"	 );

  outtree_->Branch("mini_genlep1_pt"        , &genlep1_pt_        ,  "mini_genlep1_pt/F"	 );
  outtree_->Branch("mini_genlep1_eta"        , &genlep1_eta_        ,  "mini_genlep1_eta/F"	 );
  outtree_->Branch("mini_genlep1_phi"        , &genlep1_phi_        ,  "mini_genlep1_phi/F"	 );
  outtree_->Branch("mini_genlep1_pdgId"        , &genlep1_pdgId_        ,  "mini_genlep1_pdgId/I"	 );

  outtree_->Branch("mini_genlep2_pt"        , &genlep2_pt_        ,  "mini_genlep2_pt/F"	 );
  outtree_->Branch("mini_genlep2_eta"        , &genlep2_eta_        ,  "mini_genlep2_eta/F"	 );
  outtree_->Branch("mini_genlep2_phi"        , &genlep2_phi_        ,  "mini_genlep2_phi/F"	 );
  outtree_->Branch("mini_genlep2_pdgId"        , &genlep2_pdgId_        ,  "mini_genlep2_pdgId/I"	 );

  outtree_->Branch("mini_ngenel"      , &ngenel_      ,  "mini_ngenel/I"	 );
  outtree_->Branch("mini_ngenmu"      , &ngenmu_      ,  "mini_ngenmu/I"	 );
  outtree_->Branch("mini_ngentauelp"      , &ngentaulep_      ,  "mini_ngentaulep/I"	 );
  outtree_->Branch("mini_ngentauhad"      , &ngentauhad_      ,  "mini_ngentauhad/I"	 );
  outtree_->Branch("mini_ngenlep"      , &ngenlep_      ,  "mini_ngenlep/I"	 );

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
  outtree_->SetBranchStatus("deltaPhiMin",1); 
  outtree_->SetBranchStatus("diffMetMht",1); 
  outtree_->SetBranchStatus("ht",1); 
  outtree_->SetBranchStatus("mt2",1); 
  outtree_->SetBranchStatus("met_pt",1); 
  outtree_->SetBranchStatus("met_phi",1); 
  outtree_->SetBranchStatus("HLT_HT650",1); 
  outtree_->SetBranchStatus("HLT_MET150",1); 
  outtree_->SetBranchStatus("HLT_ht350met100",1); 
  outtree_->SetBranchStatus("nlep",1); 
  outtree_->SetBranchStatus("nisoTrack",1); 
  outtree_->SetBranchStatus("ntau",1); 

  // enable mini branches
  outtree_->SetBranchStatus("mini_*",1); 

}

void MiniBabyIsoTrackLooper::initBaby(){

  weight_     = 1.;
  ngenlep_    = -1;

  passevt_    = -1;
  sigbase_    = -1;
  sigjets_    = -1;
  sightmet_   = -1;

  isoTrack_lead_pt_ = -1.;
  isoTrack_lead_eta_ = -1.;
  isoTrack_lead_phi_ = -1.;
  isoTrack_lead_absIso_ = -1.;
  isoTrack_lead_dz_ = -1.;
  isoTrack_lead_pdgId_ = -99;

  isoTrack_mostiso_pt_ = -1.;
  isoTrack_mostiso_eta_ = -1.;
  isoTrack_mostiso_phi_ = -1.;
  isoTrack_mostiso_absIso_ = -1.;
  isoTrack_mostiso_dz_ = -1.;
  isoTrack_mostiso_pdgId_ = -99;

  lep1_pt_ = -1.;
  lep1_eta_ = -1.;
  lep1_phi_ = -1.;

  tau1_pt_ = -1.;
  tau1_eta_ = -1.;
  tau1_phi_ = -1.;

  genlep1_pt_ = -1.;
  genlep1_eta_ = -1.;
  genlep1_phi_ = -1.;
  genlep1_pdgId_ = -99;

  genlep2_pt_ = -1.;
  genlep2_eta_ = -1.;
  genlep2_phi_ = -1.;
  genlep2_pdgId_ = -99;

  ngenel_ = -1;
  ngenmu_ = -1;
  ngentaulep_ = -1;
  ngentauhad_ = -1;
  ngenlep_ = -1;
}


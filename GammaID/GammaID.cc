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
#include "TLorentzVector.h"
// CMS2
//#include "../MT2CORE/CMS2.h"
#include "../MT2CORE/tools.h"
//#include "../MT2CORE/selections.h"
//#include "../MT2CORE/hemJet.h"
//#include "../MT2CORE/MT2/MT2.h"
//#include "../MT2CORE/IsoTrackVeto.h"

// header
#include "GammaID.h"

//MT2
#include "../MT2CORE/Plotting/PlotUtilities.h"


using namespace std;
using namespace mt2;

class mt2tree;

GammaID::GammaID(){
  // set some stuff here
}
GammaID::~GammaID(){

};

void GammaID::loop(TChain* chain, std::string output_name){

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  gROOT->cd();
  cout << "[GammaID::loop] creating output file: " << output_name << endl;

  outfile_ = new TFile(output_name.c_str(),"RECREATE") ; 

  cout << "[GammaID::loop] setting up histos" << endl;

  std::map<std::string, TH1D*> h_1d_nocut;
  std::map<std::string, TH1D*> h_1d_all;
  std::map<std::string, TH1D*> h_1d_true;
  std::map<std::string, TH1D*> h_1d_fake;
  std::map<std::string, TH1D*> h_1d_true700;
  std::map<std::string, TH1D*> h_1d_fake700;
  std::map<std::string, TH1D*> h_1d_trueL;
  std::map<std::string, TH1D*> h_1d_fakeL;
  std::map<std::string, TH1D*> h_1d_trueFailL;
  std::map<std::string, TH1D*> h_1d_fakeFailL;
  std::map<std::string, TH1D*> h_1d_trueL2;
  std::map<std::string, TH1D*> h_1d_fakeL2;
  std::map<std::string, TH1D*> h_1d_trueL2700;
  std::map<std::string, TH1D*> h_1d_fakeL2700;
  std::map<std::string, TH1D*> h_1d_trueL700;
  std::map<std::string, TH1D*> h_1d_fakeL700;
  
  
  // File Loop
  int nDuplicates = 0;
  int nEvents = chain->GetEntries();
  unsigned int nEventsChain = nEvents;
  cout << "[GammaID::loop] running on " << nEventsChain << " events" << endl;
  unsigned int nEventsTotal = 0;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;
  while ( (currentFile = (TFile*)fileIter.Next()) ) {
    cout << "[GammaID::loop] running on file: " << currentFile->GetTitle() << endl;

    // Get File Content
    TFile f( currentFile->GetTitle() );
    TTree *tree = (TTree*)f.Get("mt2");
    TTreeCache::SetLearnEntries(10);
    tree->SetCacheSize(128*1024*1024);
    //mt2tree t(tree);
    t.Init(tree);

    // Event Loop
    unsigned int nEventsTree = tree->GetEntriesFast();
    for( unsigned int event = 0; event < nEventsTree; ++event) {
    //    for( unsigned int event = 0; event < nEventsTree/10.; ++event) {
    //for( unsigned int event = 0; event < 10000.; ++event) {

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
      const float lumi = 1.;
      evtweight_ = t.evt_scale1fb * lumi;

      //---------------------
      // Generator level plots
      //---------------------
      float genVphi;
      float genVeta;
      float posLepEta;
      float posLepPhi;
      float negLepEta;
      float negLepPhi;
      bool ismu = false;
      bool isel = false;
      bool istau = false;
      bool isZ = false;
      bool isG = false;
      partonHT_ = 0;
      for (int i = 0; i < t.ngenPart; i++) {
	//	if (t.genPart_pdgId[i]!=23 && t.genPart_pdgId[i]!=22 && t.genPart_status[i]==23) { 
	if ((t.genPart_pdgId[i]<10 || t.genPart_pdgId[i]==21) && (t.genPart_status[i]==23 || t.genPart_status[i]==22)) { 
	  // sum final state quarks and gluons
	  partonHT_ += t.genPart_pt[i];
	}
	if (  t.evt_id >= 700 && t.evt_id < 800 ) { // dilepton
	  if (t.genPart_pdgId[i]==23 && t.genPart_status[i]==22)  {	    
	    genVpt_  = t.genPart_pt[i];
	    genVphi = t.genPart_phi[i];
	    genVeta = t.genPart_eta[i];
	  }
	  float pdgId = t.genPart_pdgId[i];
	  float momId =  t.genPart_motherId[i];
	  if (fabs(pdgId) == 13 && momId==23) ismu=true;
	  if (fabs(pdgId) == 11 && momId==23) isel=true;
	  if (fabs(pdgId) == 15 && momId==23) istau=true;
	  if (fabs(pdgId)>0 && (ismu || isel)) { posLepEta = t.genPart_eta[i]; posLepPhi = t.genPart_phi[i]; }
	  if (fabs(pdgId)<0 && (ismu || isel)) { negLepEta = t.genPart_eta[i]; negLepPhi = t.genPart_phi[i]; }
	  
	  isZ=true;
	}
	else if (  t.evt_id >= 200 && t.evt_id < 300 ) { // gamma
	  if (t.genPart_pdgId[i]==22 && t.genPart_status[i]==23) {
	    genVpt_  = t.genPart_pt[i];
	    genVphi = t.genPart_phi[i];
	    genVeta = t.genPart_eta[i];
	  }
	  isG=true;
	}	
      }
      if (!isG && !isZ) continue; // this should never happen
      if (!PassesEventSelection(t.nVert)) continue;


      float real = false;
      if (genVpt_ > 70 && fabs(genVeta) < 2.5) {
	
	for (int i = 0; i < t.nGammas20; i++) {
	  if (t.gamma_pt[i] < 70) continue;
	  float thisDR = DeltaR(genVeta, t.gamma_eta[i], genVphi, t.gamma_phi[i]);
	  if (thisDR < 0.1) real = true;
	  else real = false;
	  fillHistosPh(h_1d_all, "all",i);
	  if (real) fillHistosPh(h_1d_true, "true",i);
	  else fillHistosPh(h_1d_fake, "fake",i);
	  if (t.gamma_pt[i] > 700) {
	    if (real) fillHistosPh(h_1d_true700, "true700",i);
	    else fillHistosPh(h_1d_fake700, "fake700",i);
	  }
	  if (t.gamma_idCutBased[i] > 0) {
	    if (real) fillHistosPh(h_1d_trueL, "trueL",i);
	    else fillHistosPh(h_1d_fakeL, "fakeL",i);
	  }
	  else {
	    if (real) fillHistosPh(h_1d_trueFailL, "trueFailL",i);
	    else fillHistosPh(h_1d_fakeFailL, "fakeFailL",i);
	  }
	  float pt = t.gamma_pt[i];
	  float eta = t.gamma_eta[i];
	  if (t.gamma_chHadIso[i] / pt > 0.1) continue;
	  if (t.gamma_neuHadIso[i] / pt > 0.1) continue;
	  //if (t.gamma_phIso[i] / pt > 0.2) continue;
	  if (t.gamma_hOverE[i]  > 0.05) continue;
	  if (t.gamma_r9[i] < 0.8) continue;
	  if (t.gamma_sigmaIetaIeta[i] > 0.013 && fabs(eta)< 1.479) continue;
	  if (t.gamma_sigmaIetaIeta[i] > 0.03 && fabs(eta) >= 1.479) continue;
	  if (real) fillHistosPh(h_1d_trueL2, "trueL2",i);
	  else fillHistosPh(h_1d_fakeL2, "fakeL2",i);
	  if (t.gamma_pt[i] > 700) {
	    if (real) fillHistosPh(h_1d_trueL2700, "trueL2700",i);
	    else fillHistosPh(h_1d_fakeL2700, "fakeL2700",i);
	  }
	  if (t.gamma_phIso[i] / pt > 0.2) continue;
	  if (t.gamma_pt[i] > 700) {
	    if (real) fillHistosPh(h_1d_trueL700, "trueL700",i);
	    else fillHistosPh(h_1d_fakeL700, "fakeL700",i);
	  }


	}
	 
      } 



   }//end loop on events in a file
  
    delete tree;
    f.Close();
  }//end loop on files
  
  cout << "[GammaID::loop] processed " << nEventsTotal << " events" << endl;
  if ( nEventsChain != nEventsTotal ) {
    std::cout << "ERROR: number of events from files is not equal to total number of events" << std::endl;
  }

  cout << nDuplicates << " duplicate events were skipped." << endl;

  //---------------------
  // Save Plots
  //---------------------

  outfile_->cd();
  savePlotsDir(h_1d_global,outfile_,"");
  savePlotsDir(h_1d_all,outfile_,"all");
  savePlotsDir(h_1d_true,outfile_,"true");
  savePlotsDir(h_1d_fake,outfile_,"fake");
  savePlotsDir(h_1d_true700,outfile_,"true700");
  savePlotsDir(h_1d_fake700,outfile_,"fake700");
  savePlotsDir(h_1d_trueL,outfile_,"trueL");
  savePlotsDir(h_1d_fakeL,outfile_,"fakeL");
  savePlotsDir(h_1d_trueFailL,outfile_,"trueFailL");
  savePlotsDir(h_1d_fakeFailL,outfile_,"fakeFailL");
  savePlotsDir(h_1d_trueL2,outfile_,"trueL2");
  savePlotsDir(h_1d_fakeL2,outfile_,"fakeL2");
  savePlotsDir(h_1d_trueL2700,outfile_,"trueL2700");
  savePlotsDir(h_1d_fakeL2700,outfile_,"fakeL2700");
  savePlotsDir(h_1d_trueL700,outfile_,"trueL700");
  savePlotsDir(h_1d_fakeL700,outfile_,"fakeL700");

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

void GammaID::fillHistosSignalRegion(std::map<std::string, TH1D*>& h_1d, const SignalRegionJets::value_type& sr_jets, const SignalRegionHtMet::value_type& sr_htmet, const std::string& dir, const std::string& suffix) {
  
 if ( PassesSignalRegion(met_pt_Zinv_, ht_Zinv_, nJet40_Zinv_, nBJet40_Zinv_, deltaPhiMin_Zinv_, diffMetMht_Zinv_, 
			 nlepveto_, t.jet_pt[jetIdx0_], t.jet_pt[jetIdx1_], sr_jets, sr_htmet) ) 
   {
     plot1D("h_SignalRegion",  sr_jets+sr_htmet,   evtweight_, h_1d_global, "", 100, 0, 100);
     //     fillHistos( h_1d, dir, suffix);
   }
 return;
}

void GammaID::fillHistosPh(std::map<std::string, TH1D*>& h_1d, const std::string& dirname, int i) {
  TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
  if (dir == 0) {
    dir = outfile_->mkdir(dirname.c_str());
  } 
  dir->cd();


  plot1D("h_truth_Vpt",       genVpt_,   evtweight_, h_1d, "", 400, 0, 2000);
  plot1D("h_pt", t.gamma_pt[i] ,   evtweight_, h_1d, "", 200, 0, 1000);
  plot1D("h_eta", t.gamma_eta[i] ,   evtweight_, h_1d, "", 100, -5, 5);
  plot1D("h_hOverE", t.gamma_hOverE[i] ,   evtweight_, h_1d, "", 100, 0, 1);
  plot1D("h_r9", t.gamma_r9[i] ,   evtweight_, h_1d, "", 100, -1, 1);
  plot1D("h_sigmaIetaIeta", t.gamma_sigmaIetaIeta[i] ,   evtweight_, h_1d, "", 100, 0, 0.05);
  plot1D("h_chHadIso" , t.gamma_chHadIso[i] ,   evtweight_, h_1d, "", 200, 0, 20);
  plot1D("h_neuHadIso", t.gamma_neuHadIso[i] ,   evtweight_, h_1d, "", 200, 0, 20);
  plot1D("h_phIso", t.gamma_phIso[i] ,   evtweight_, h_1d, "", 200, 0, 20);
  plot1D("h_chHadIsoRel" , t.gamma_chHadIso[i] / t.gamma_pt[i] ,   evtweight_, h_1d, "", 100, 0, 1);
  plot1D("h_neuHadIsoRel", t.gamma_neuHadIso[i] / t.gamma_pt[i] ,   evtweight_, h_1d, "", 100, 0, 1);
  plot1D("h_phIsoRel", t.gamma_phIso[i] / t.gamma_pt[i],   evtweight_, h_1d, "", 100, 0, 1);

  
  outfile_->cd();
  return;
}

// C++
#include <iostream>
#include <vector>
#include <set>
#include <stdexcept>
#include <array>

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

  std::map<std::string, TH1D*> h_1d_truePh;
  std::map<std::string, TH1D*> h_1d_truePhNoMom;
  std::map<std::string, TH1D*> h_1d_truePhMom;
  std::map<std::string, TH1D*> h_1d_fakePh;
  std::map<std::string, TH1D*> h_1d_veryfakePh;
  std::map<std::string, TH1D*> h_1d_stat1Mom22;
  std::map<std::string, TH1D*> h_1d_stat1MomGreater22;
  std::map<std::string, TH1D*> h_1d_stat1MomLess22;
  std::map<std::string, TH1D*> h_1d_stat23;
  
  
  std::map<std::string, TH1D*> h_1d_truePh_70t100;
  std::map<std::string, TH1D*> h_1d_truePhNoMom_70t100;
  std::map<std::string, TH1D*> h_1d_truePhMom_70t100;
  std::map<std::string, TH1D*> h_1d_fakePh_70t100;
  std::map<std::string, TH1D*> h_1d_veryfakePh_70t100;
  std::map<std::string, TH1D*> h_1d_stat1Mom22_70t100;
  std::map<std::string, TH1D*> h_1d_stat1MomGreater22_70t100;
  std::map<std::string, TH1D*> h_1d_stat1MomLess22_70t100;
  std::map<std::string, TH1D*> h_1d_stat23_70t100;

  std::map<std::string, TH1D*> h_1d_truePh_100t150;
  std::map<std::string, TH1D*> h_1d_truePhNoMom_100t150;
  std::map<std::string, TH1D*> h_1d_truePhMom_100t150;
  std::map<std::string, TH1D*> h_1d_fakePh_100t150;
  std::map<std::string, TH1D*> h_1d_veryfakePh_100t150;
  std::map<std::string, TH1D*> h_1d_stat1Mom22_100t150;
  std::map<std::string, TH1D*> h_1d_stat1MomGreater22_100t150;
  std::map<std::string, TH1D*> h_1d_stat1MomLess22_100t150;
  std::map<std::string, TH1D*> h_1d_stat23_100t150;
  
  std::map<std::string, TH1D*> h_1d_truePh_150t200;
  std::map<std::string, TH1D*> h_1d_truePhNoMom_150t200;
  std::map<std::string, TH1D*> h_1d_truePhMom_150t200;
  std::map<std::string, TH1D*> h_1d_fakePh_150t200;
  std::map<std::string, TH1D*> h_1d_veryfakePh_150t200;
  std::map<std::string, TH1D*> h_1d_stat1Mom22_150t200;
  std::map<std::string, TH1D*> h_1d_stat1MomGreater22_150t200;
  std::map<std::string, TH1D*> h_1d_stat1MomLess22_150t200;
  std::map<std::string, TH1D*> h_1d_stat23_150t200;
  
  std::map<std::string, TH1D*> h_1d_truePh_200plus;
  std::map<std::string, TH1D*> h_1d_truePhNoMom_200plus;
  std::map<std::string, TH1D*> h_1d_truePhMom_200plus;
  std::map<std::string, TH1D*> h_1d_fakePh_200plus;
  std::map<std::string, TH1D*> h_1d_veryfakePh_200plus;
  std::map<std::string, TH1D*> h_1d_stat1Mom22_200plus;
  std::map<std::string, TH1D*> h_1d_stat1MomGreater22_200plus;
  std::map<std::string, TH1D*> h_1d_stat1MomLess22_200plus;
  std::map<std::string, TH1D*> h_1d_stat23_200plus;
  
  //book histograms
  TH1D* h_stat23DR = new TH1D("h_stat23DR","Status 23 dR",60,0,0.3);
  TH1D* h_stat1DR = new TH1D("h_stat1DR","Status 1 dR",60,0,0.3);
  TH1D* h_stat23DR_long = new TH1D("h_stat23DR_long","Status 23 dR",60,0,5);
  TH1D* h_stat1DR_long = new TH1D("h_stat1DR_long","Status 1 dR",60,0,5);
  
  // TH1D* h_jetDRtrue = new TH1D("h_jetDRtrue","jet dR",60,0,0.4);
  // TH1D* h_jetDRtrueNoMom = new TH1D("h_jetDRtrueNoMom","jet dR",60,0,0.4);
  // TH1D* h_jetDRtrueMom = new TH1D("h_jetDRtrueMom","jet dR",60,0,0.4);
  // TH1D* h_jetDRveryfake = new TH1D("h_jetDRveryfake","jet dR",60,0,0.4);
  // TH1D* h_jetDRstat1Mom22 = new TH1D("h_jetDRstat1Mom22","jet dR",60,0,0.4);
  // TH1D* h_jetDRstat1MomOther = new TH1D("h_jetDRstat1MomOther","jet dR",60,0,0.4); 
  
  TH1D* h_jetDR2true = new TH1D("h_jetDR2true","jet dR",60,0,0.4);
  TH1D* h_jetDR2trueNoMom = new TH1D("h_jetDR2trueNoMom","jet dR",60,0,0.4);
  TH1D* h_jetDR2trueMom = new TH1D("h_jetDR2trueMom","jet dR",60,0,0.4);
  TH1D* h_jetDR2veryfake = new TH1D("h_jetDR2veryfake","jet dR",60,0,0.4);
  TH1D* h_jetDR2stat1Mom22 = new TH1D("h_jetDR2stat1Mom22","jet dR",60,0,0.4);
  TH1D* h_jetDR2stat1MomOther = new TH1D("h_jetDR2stat1MomOther","jet dR",60,0,0.4);
  
  // TH1D* h_jetDRtrueLong = new TH1D("h_jetDRtrueLong","jet dR",60,0,5);
  // TH1D* h_jetDRtrueNoMomLong = new TH1D("h_jetDRtrueNoMomLong","jet dR",60,0,5);
  // TH1D* h_jetDRtrueMomLong = new TH1D("h_jetDRtrueMomLong","jet dR",60,0,5);
  // TH1D* h_jetDRveryfakeLong = new TH1D("h_jetDRveryfakeLong","jet dR",60,0,5);
  // TH1D* h_jetDRstat1Mom22Long = new TH1D("h_jetDRstat1Mom22Long","jet dR",60,0,5);
  // TH1D* h_jetDRstat1MomOtherLong = new TH1D("h_jetDRstat1MomOtherLong","jet dR",60,0,5);
  
  TH1D* h_jetDR2trueLong = new TH1D("h_jetDR2trueLong","jet dR",60,0,5);
  TH1D* h_jetDR2trueNoMomLong = new TH1D("h_jetDR2trueNoMomLong","jet dR",60,0,5);
  TH1D* h_jetDR2trueMomLong = new TH1D("h_jetDR2trueMomLong","jet dR",60,0,5);
  TH1D* h_jetDR2veryfakeLong = new TH1D("h_jetDR2veryfakeLong","jet dR",60,0,5);
  TH1D* h_jetDR2stat1Mom22Long = new TH1D("h_jetDR2stat1Mom22Long","jet dR",60,0,5);
  TH1D* h_jetDR2stat1MomOtherLong = new TH1D("h_jetDR2stat1MomOtherLong","jet dR",60,0,5);

  TH1D* h_jetDRlowIso = new TH1D("h_jetDRlowIso", "jet dR", 60, 0, 5);
  TH1D* h_jetDRhighIso = new TH1D("h_jetDRhighIso", "jet dR", 60, 0, 5);
  TH1D* h_jetDR2lowIso = new TH1D("h_jetDR2lowIso", "jet dR", 60, 0, 5);
  TH1D* h_jetDR2highIso = new TH1D("h_jetDR2highIso", "jet dR", 60, 0, 5);
  
  TH1D * h_genIsotrue = new TH1D("h_genIsotrue","gen Iso", 80,0,80);
  TH1D * h_genIsostat1Mom22 = new TH1D("h_genIsostat1Mom22","gen Iso", 80,0,80);
  TH1D * h_genIsostat1MomGreater22 = new TH1D("h_genIsostat1MomGreater22","gen Iso", 80,0,80);
  TH1D * h_genIsostat1MomLess22 = new TH1D("h_genIsostat1MomLess22","gen Iso", 80,0,80);
  TH1D * h_genIsotrueShort = new TH1D("h_genIsotrueShort","gen Iso", 15,0,15);
  TH1D * h_genIsostat1Mom22Short = new TH1D("h_genIsostat1Mom22Short","gen Iso", 15,0,15);
  TH1D * h_genIsostat1MomGreater22Short = new TH1D("h_genIsostat1MomGreater22Short","gen Iso", 15,0,15);
  TH1D * h_genIsostat1MomLess22Short = new TH1D("h_genIsostat1MomLess22Short","gen Iso", 15,0,15);

  TH1D* h_genPartMomId = new TH1D("h_genPartMomId","mother ID",500,0,500);
  TH1D* h_trueNoMomId = new TH1D("h_trueNoMomId","true w/ no mother ID",500,0,500);
  TH1D* h_trueMomId = new TH1D("h_trueMomId","true w/ mother ID",500,0,500);
  
  // h_jetDRtrue->Sumw2();
  // h_jetDRtrueNoMom->Sumw2();
  // h_jetDRtrueMom->Sumw2();
  // h_jetDRveryfake->Sumw2();
  // h_jetDRstat1Mom22->Sumw2();
  // h_jetDRstat1MomOther->Sumw2();
  // h_jetDRtrueLong->Sumw2();
  // h_jetDRtrueNoMomLong->Sumw2();
  // h_jetDRtrueMomLong->Sumw2();
  // h_jetDRveryfakeLong->Sumw2();
  // h_jetDRstat1Mom22Long->Sumw2();
  // h_jetDRstat1MomOtherLong->Sumw2();

  h_jetDR2true->Sumw2();
  h_jetDR2trueNoMom->Sumw2();
  h_jetDR2trueMom->Sumw2();
  h_jetDR2veryfake->Sumw2();
  h_jetDR2stat1Mom22->Sumw2();
  h_jetDR2stat1MomOther->Sumw2();
  h_jetDR2trueLong->Sumw2();
  h_jetDR2trueNoMomLong->Sumw2();
  h_jetDR2trueMomLong->Sumw2();
  h_jetDR2veryfakeLong->Sumw2();
  h_jetDR2stat1Mom22Long->Sumw2();
  h_jetDR2stat1MomOtherLong->Sumw2();

  h_genIsotrue->Sumw2();
  h_genIsostat1Mom22->Sumw2();
  h_genIsostat1MomGreater22->Sumw2();
  h_genIsostat1MomLess22->Sumw2();
  h_genIsotrueShort->Sumw2();
  h_genIsostat1Mom22Short->Sumw2();
  h_genIsostat1MomGreater22Short->Sumw2();
  h_genIsostat1MomLess22Short->Sumw2();

  h_jetDRlowIso->Sumw2();
  h_jetDRhighIso->Sumw2();
  h_jetDR2lowIso->Sumw2();
  h_jetDR2highIso->Sumw2();

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
      float gen23Vphi;
      float gen23Veta;
      int   gen23motherId;
      vector<float> gen1Vphi;
      vector<float> gen1Veta;
      vector<int>   gen1motherId;
      vector<int>   gen1idx;
      float posLepEta;
      float posLepPhi;
      float negLepEta;
      float negLepPhi;
      
      int gamma23_counter = 0;
      int gamma1_counter  = 0;

      bool ismu = false;
      bool isel = false;
      bool istau = false;
      bool isZ = false;
      bool isG = false;
      partonHT_ = 0;
      
      for (int i = 0; i < t.ngenPart; i++) {
	//	if (t.genPart_pdgId[i]!=23 && t.genPart_pdgId[i]!=22 && t.genPart_status[i]==23) {   
	if (t.genPart_pdgId[i]==22 && t.genPart_status[i]==23) {
	  genVpt_  = t.genPart_pt[i];
	  gen23Vphi = t.genPart_phi[i];
	  gen23Veta = t.genPart_eta[i];
	  gen23motherId = t.genPart_motherId[i];
	  gamma23_counter++;
	}
	if (t.genPart_pdgId[i]==22 && t.genPart_status[i]==1) {
	  gen1Vphi.push_back(t.genPart_phi[i]);
	  gen1Veta.push_back(t.genPart_eta[i]);
	  gen1motherId.push_back(t.genPart_motherId[i]);
	  gen1idx.push_back(i);
	  gamma1_counter++;
	}
	
	
      }

      //cout statements for debugging
      // if (gamma23_counter >1) cout << "No. of status 23 Gammas: " << gamma23_counter << endl;
      // if (gamma1_counter >1) cout  << "No. of status 1 Gammas: "  << gamma1_counter  << endl; 

      for (int i = 0; i < t.ngamma; i++) {
	  
	//gamma id cuts, loose working point
	if (t.gamma_pt[i] < 70) continue;
	if (abs(t.gamma_eta[i]) > 1.4442) continue;
	if (t.gamma_sigmaIetaIeta[i] > 0.01) continue;
	if (t.gamma_hOverE[i] > 0.0559) continue;
	
	//initialize 
	float totalIso = t.gamma_chHadIso[i]+t.gamma_neuHadIso[i]+t.gamma_phIso[i];
	float real = false;
	float fake = false;
	float real1 = false;
	float fake1 = false;
	int momId   = -9999;
	float mom   = false;
	int stat1idx = -1;
	float stat1iso = 0;
	
	//dR for status 23 photons
	float thisDR23 = 9999;
	if (gamma23_counter != 0){
	  thisDR23 = DeltaR(gen23Veta, t.gamma_eta[i], gen23Vphi, t.gamma_phi[i]);
	}
	int momId23 = abs(gen23motherId);
	
	//compute smallest dR for status 1 photons
	float thisDR1 = 9999;
	for (int j = 0; j < gen1Vphi.size(); j++) {
	  float thisDR = DeltaR(gen1Veta[j], t.gamma_eta[i], gen1Vphi[j], t.gamma_phi[i]);
	  if (thisDR < thisDR1) {
	    thisDR1 = thisDR;
	    momId = gen1motherId[j];
	    stat1idx = j;
	  }
	}

	
	//compute gen level iso for stat1 photons, using closest stat1 photon in dR cone of 0.4
	if(thisDR1<0.4){ //only for photons matched to stat1 photon
	  for (int genIdx = 0; genIdx < t.ngenPart; genIdx++) {
	    
	    //only loop over stat 1111 gen part
	    if (t.genPart_status[genIdx]!=1111) continue;

	    const int ID = t.genPart_pdgId[genIdx];

	    //skip neutrinos
	    if ((ID==12) || (ID==14) || (ID==16)) continue;

	    
	    //compute dR between gen part and stat1 photon
	    float thisDR = DeltaR(gen1Veta[stat1idx], t.genPart_eta[genIdx], gen1Vphi[stat1idx], t.genPart_phi[genIdx]);
	    
	    //skip the photon itself
	    //if (genIdx == gen1idx[stat1idx]) continue; //this doesnt work, stat 1 idx != stat 1111 idx, duh
	    if (thisDR < 0.01 && t.genPart_pdgId[genIdx]==22) continue; //skip if its a photon within dR<0.1

	    //check if gen part is within dR<0.4, and add to the stat1iso
	    if (thisDR<0.4){
	      stat1iso += t.genPart_pt[genIdx];
	    }
	  }
	}

	//compute smallest dR to jet
	float thisDRjet = 9999; //dr to closest jet
	float thisDRjet2 = 9999; //dr to second closest jet
	for (int j = 0; j < t.njet; j++) {
	  if (t.jet_pt[j] < 40) continue; //min jet pT
	  float thisDR = DeltaR(t.jet_eta[j], t.gamma_eta[i], t.jet_phi[j], t.gamma_phi[i]);
	  if (thisDR < thisDRjet2){
	    thisDRjet2 = thisDR;
	  }
	  if (thisDR < thisDRjet) {
	    thisDRjet2 = thisDRjet;
	    thisDRjet = thisDR;
	  }
	}
	
	//define dR cut
	if (thisDR23 < 0.4) real = true;
	if (thisDR23 >= 0.4)   fake = true;
	if (thisDR1 < 0.4) real1 = true;
	if (thisDR1 >= 0.4) fake1 = true;
	
	if (momId23 == 0) mom = false;
	else mom = true;

	//fill all
	fillHistosPh(h_1d_all, "all",i);

	//dR histograms
	h_stat23DR->Fill(thisDR23);
	h_stat1DR->Fill(thisDR1);
	h_stat23DR_long->Fill(thisDR23);
	h_stat1DR_long->Fill(thisDR1);

	//genIso histograms
	if (thisDR1<0.4){
	  if (real) {
	    h_genIsotrue->Fill(stat1iso, evtweight_);
	    h_genIsotrueShort->Fill(stat1iso, evtweight_);
	  }
	  if (real1 && fake && abs(momId)==22) {
	    h_genIsostat1Mom22->Fill(stat1iso, evtweight_);
	    h_genIsostat1Mom22Short->Fill(stat1iso, evtweight_);
	  }
	  if (real1 && fake && abs(momId)>22) {
	    h_genIsostat1MomGreater22->Fill(stat1iso, evtweight_);
	    h_genIsostat1MomGreater22Short->Fill(stat1iso, evtweight_);
	  }
	  if (real1 && fake && abs(momId)<22) {
	    h_genIsostat1MomLess22->Fill(stat1iso, evtweight_);
	    h_genIsostat1MomLess22Short->Fill(stat1iso, evtweight_);
	  }
	}

	//jetdR histograms plot dR of closest jet
	/*
	  if (real) {
	  h_jetDRtrue->Fill(thisDRjet, evtweight_);
	  h_jetDRtrueLong->Fill(thisDRjet, evtweight_);
	}
	if (real && !mom) {
	  h_jetDRtrueNoMom->Fill(thisDRjet, evtweight_);
	  h_jetDRtrueNoMomLong->Fill(thisDRjet, evtweight_);
	}
	if (real && mom) {
	  h_jetDRtrueMom->Fill(thisDRjet, evtweight_);
	  h_jetDRtrueMomLong->Fill(thisDRjet, evtweight_);
	}
	if (fake && fake1) {
	  h_jetDRveryfake->Fill(thisDRjet, evtweight_);
	  h_jetDRveryfakeLong->Fill(thisDRjet, evtweight_);
	}
	if (real1 && fake && abs(momId)<=22) {
	  h_jetDRstat1Mom22->Fill(thisDRjet, evtweight_);
	  h_jetDRstat1Mom22Long->Fill(thisDRjet, evtweight_);
	}
	if (real1 && fake && abs(momId)>22) {
	  h_jetDRstat1MomOther->Fill(thisDRjet, evtweight_);
	  h_jetDRstat1MomOtherLong->Fill(thisDRjet, evtweight_);
	}
	if (real && totalIso < 20) {
	  h_jetDRlowIso->Fill(thisDRjet, evtweight_);
	}
	if (real && totalIso > 20) {
	  h_jetDRhighIso->Fill(thisDRjet, evtweight_);
	}
	*/

	//jetDR2 histograms plot jetDR ignoring the cloest jet in DR of 0.4
	if (thisDRjet<0.4) {
	  if (real) {
	    h_jetDR2true->Fill(thisDRjet2, evtweight_);
	    h_jetDR2trueLong->Fill(thisDRjet2, evtweight_);
	  }
	  if (real && !mom) {	  
	    h_jetDR2trueNoMom->Fill(thisDRjet2, evtweight_);
	    h_jetDR2trueNoMomLong->Fill(thisDRjet2, evtweight_);
	  }
	  if (real && mom) {
	    h_jetDR2trueMom->Fill(thisDRjet2, evtweight_);
	    h_jetDR2trueMomLong->Fill(thisDRjet2, evtweight_);
	  }
	  if (fake && fake1) {
	    h_jetDR2veryfake->Fill(thisDRjet2, evtweight_);
	    h_jetDR2veryfakeLong->Fill(thisDRjet2, evtweight_);
	  }
	  if (real1 && fake && abs(momId)<=22) {
	    h_jetDR2stat1Mom22->Fill(thisDRjet2, evtweight_);
	    h_jetDR2stat1Mom22Long->Fill(thisDRjet2, evtweight_);
	  }
	  if (real1 && fake && abs(momId)>22) {
	    h_jetDR2stat1MomOther->Fill(thisDRjet2, evtweight_);
	    h_jetDR2stat1MomOtherLong->Fill(thisDRjet2, evtweight_);
	  }
	  if (real && totalIso < 20) {
	    h_jetDR2lowIso->Fill(thisDRjet2, evtweight_);
	  }
	  if (real && totalIso > 20) {
	    h_jetDR2highIso->Fill(thisDRjet2, evtweight_);
	  }
	  
	}
	
	else {
	  if (real) {
	    h_jetDR2true->Fill(thisDRjet, evtweight_);
	    h_jetDR2trueLong->Fill(thisDRjet, evtweight_);
	  }
	  if (real && !mom) {	  
	    h_jetDR2trueNoMom->Fill(thisDRjet, evtweight_);
	    h_jetDR2trueNoMomLong->Fill(thisDRjet, evtweight_);
	  }
	  if (real && mom) {
	    h_jetDR2trueMom->Fill(thisDRjet, evtweight_);
	    h_jetDR2trueMomLong->Fill(thisDRjet, evtweight_);
	  }
	  if (fake && fake1) {
	    h_jetDR2veryfake->Fill(thisDRjet, evtweight_);
	    h_jetDR2veryfakeLong->Fill(thisDRjet, evtweight_);
	  }
	  if (real1 && fake && abs(momId)<=22) {
	    h_jetDR2stat1Mom22->Fill(thisDRjet, evtweight_);
	    h_jetDR2stat1Mom22Long->Fill(thisDRjet, evtweight_);
	  }
	  if (real1 && fake && abs(momId)>22) {
	    h_jetDR2stat1MomOther->Fill(thisDRjet, evtweight_);
	    h_jetDR2stat1MomOtherLong->Fill(thisDRjet, evtweight_);
	  }
	  if (real && totalIso < 20) {
	    h_jetDR2lowIso->Fill(thisDRjet, evtweight_);
	  }
	  if (real && totalIso > 20) {
	    h_jetDR2highIso->Fill(thisDRjet, evtweight_);
	  }
	}

	//mom id histograms
	if (real1) h_genPartMomId->Fill(abs(momId));
	if (real && mom) h_trueMomId->Fill(abs(momId23));
	if (real && !mom) h_trueNoMomId->Fill(abs(momId23));
	
	  
	//no pT bin
	if (real) fillHistosPh(h_1d_truePh, "truePh",i);
	if (real && !mom) fillHistosPh(h_1d_truePhNoMom, "truePhNoMom",i);
	if (real &&  mom) fillHistosPh(h_1d_truePhMom, "truePhMom",i);
	if (fake) fillHistosPh(h_1d_fakePh, "fakePh",i);
	if (fake && fake1) fillHistosPh(h_1d_veryfakePh, "veryfakePh",i);
	if (real1 && fake && abs(momId)==22) fillHistosPh(h_1d_stat1Mom22, "stat1Mom22",i);
	if (real1 && fake && abs(momId)>22) fillHistosPh(h_1d_stat1MomGreater22, "stat1MomGreater22",i);
	if (real1 && fake && abs(momId)<22) fillHistosPh(h_1d_stat1MomLess22, "stat1MomLess22",i);
	if (!real1 && real) fillHistosPh(h_1d_stat23, "stat23",i);

	// 70< pT <100
	if ((t.gamma_pt[i] > 70) && (t.gamma_pt[i] < 100)) {
	  if (real) {
	    fillHistosPh(h_1d_truePh_70t100, "truePh_70t100",i);
	    fillGenIso(h_1d_truePh_70t100, "truePh_70t100", i, stat1iso);
	  }
	  if (real && !mom) fillHistosPh(h_1d_truePhNoMom_70t100, "truePhNoMom_70t100",i);
	  if (real && mom) fillHistosPh(h_1d_truePhMom_70t100, "truePhMom_70t100",i);
	  if (fake) fillHistosPh(h_1d_fakePh_70t100, "fakePh_70t100",i);
	  if (fake && fake1) fillHistosPh(h_1d_veryfakePh_70t100, "veryfakePh_70t100",i);
	  if (real1 && fake && abs(momId)==22) {
	    fillHistosPh(h_1d_stat1Mom22_70t100, "stat1Mom22_70t100",i);
	    fillGenIso(h_1d_stat1Mom22_70t100, "stat1Mom22_70t100", i, stat1iso);
	  }
	  if (real1 && fake && abs(momId)>22) {
	    fillHistosPh(h_1d_stat1MomGreater22_70t100, "stat1MomGreater22_70t100",i);
	    fillGenIso(h_1d_stat1MomGreater22_70t100, "stat1MomGreater22_70t100", i, stat1iso);
	  }
	  if (real1 && fake && abs(momId)<22) {
	    fillHistosPh(h_1d_stat1MomLess22_70t100, "stat1MomLess22_70t100",i);
	    fillGenIso(h_1d_stat1MomLess22_70t100, "stat1MomLess22_70t100", i, stat1iso);
	  }
	  if (!real1 && real) fillHistosPh(h_1d_stat23_70t100, "stat23_70t100",i);
	}

	// 100< pT <150
	if ((t.gamma_pt[i] > 100) && (t.gamma_pt[i] < 150)) {
	  if (real) {
	    fillHistosPh(h_1d_truePh_100t150, "truePh_100t150",i);
	    fillGenIso(h_1d_truePh_100t150, "truePh_100t150", i, stat1iso);
	  }
	  if (real && !mom) fillHistosPh(h_1d_truePhNoMom_100t150, "truePhNoMom_100t150",i);
	  if (real && mom) fillHistosPh(h_1d_truePhMom_100t150, "truePhMom_100t150",i);
	  if (fake) fillHistosPh(h_1d_fakePh_100t150, "fakePh_100t150",i);
	  if (fake && fake1) fillHistosPh(h_1d_veryfakePh_100t150, "veryfakePh_100t150",i);
	  if (real1 && fake && abs(momId)==22) {
	    fillHistosPh(h_1d_stat1Mom22_100t150, "stat1Mom22_100t150",i);
	    fillGenIso(h_1d_stat1Mom22_100t150, "stat1Mom22_100t150", i, stat1iso);
	  }
	  if (real1 && fake && abs(momId)>22) {
	    fillHistosPh(h_1d_stat1MomGreater22_100t150, "stat1MomGreater22_100t150",i);
	    fillGenIso(h_1d_stat1MomGreater22_100t150, "stat1MomGreater22_100t150", i, stat1iso);
	  }
	  if (real1 && fake && abs(momId)<22) {
	    fillHistosPh(h_1d_stat1MomLess22_100t150, "stat1MomLess22_100t150",i);
	    fillGenIso(h_1d_stat1MomLess22_100t150, "stat1MomLess22_100t150", i, stat1iso);
	  }
	  if (!real1 && real) fillHistosPh(h_1d_stat23_100t150, "stat23_100t150",i);
	}

	// 150< pT <200
	if ((t.gamma_pt[i] > 150) && (t.gamma_pt[i] < 200)) {
	  if (real) {
	    fillHistosPh(h_1d_truePh_150t200, "truePh_150t200",i);
	    fillGenIso(h_1d_truePh_150t200, "truePh_150t200", i, stat1iso);
	  }
	  if (real && !mom) fillHistosPh(h_1d_truePhNoMom_150t200, "truePhNoMom_150t200",i);
	  if (real && mom) fillHistosPh(h_1d_truePhMom_150t200, "truePhMom_150t200",i);
	  if (fake) fillHistosPh(h_1d_fakePh_150t200, "fakePh_150t200",i);
	  if (fake && fake1) fillHistosPh(h_1d_veryfakePh_150t200, "veryfakePh_150t200",i);
	  if (real1 && fake && abs(momId)==22) {
	    fillHistosPh(h_1d_stat1Mom22_150t200, "stat1Mom22_150t200",i);
	    fillGenIso(h_1d_stat1Mom22_150t200, "stat1Mom22_150t200", i, stat1iso);
	  }
	  if (real1 && fake && abs(momId)>22) {
	    fillHistosPh(h_1d_stat1MomGreater22_150t200, "stat1MomGreater22_150t200",i);
	    fillGenIso(h_1d_stat1MomGreater22_150t200, "stat1MomGreater22_150t200", i, stat1iso);
	  }
	  if (real1 && fake && abs(momId)<22) {
	    fillHistosPh(h_1d_stat1MomLess22_150t200, "stat1MomLess22_150t200",i);
	    fillGenIso(h_1d_stat1MomLess22_150t200, "stat1MomLess22_150t200", i, stat1iso);
	  }
	  if (!real1 && real) fillHistosPh(h_1d_stat23_150t200, "stat23_150t200",i);
	}

	// 200< pT
	if (t.gamma_pt[i] > 200) {
	  if (real) {
	    fillHistosPh(h_1d_truePh_200plus, "truePh_200plus",i);
	    fillGenIso(h_1d_truePh_200plus, "truePh_200plus", i, stat1iso);
	  }
	  if (real && !mom) fillHistosPh(h_1d_truePhNoMom_200plus, "truePhNoMom_200plus",i);
	  if (real && mom) fillHistosPh(h_1d_truePhMom_200plus, "truePhMom_200plus",i);
	  if (fake) fillHistosPh(h_1d_fakePh_200plus, "fakePh_200plus",i);
	  if (fake && fake1) fillHistosPh(h_1d_veryfakePh_200plus, "veryfakePh_200plus",i);
	  if (real1 && fake && abs(momId)==22) {
	    fillHistosPh(h_1d_stat1Mom22_200plus, "stat1Mom22_200plus",i);
	    fillGenIso(h_1d_stat1Mom22_200plus, "stat1Mom22_200plus", i, stat1iso);
	  }
	  if (real1 && fake && abs(momId)>22) {
	    fillHistosPh(h_1d_stat1MomGreater22_200plus, "stat1MomGreater22_200plus",i);
	    fillGenIso(h_1d_stat1MomGreater22_200plus, "stat1MomGreater22_200plus", i, stat1iso);
	  }
	  if (real1 && fake && abs(momId)<22) {
	    fillHistosPh(h_1d_stat1MomLess22_200plus, "stat1MomLess22_200plus",i);
	    fillGenIso(h_1d_stat1MomLess22_200plus, "stat1MomLess22_200plus", i, stat1iso);
	  }
	  if (!real1 && real) fillHistosPh(h_1d_stat23_200plus, "stat23_200plus",i);
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
  
  h_stat23DR->Write();
  h_stat1DR->Write();
  h_stat23DR_long->Write();
  h_stat1DR_long->Write();
  //h_jetDRtrue->Write();
  //h_jetDRtrueNoMom->Write();
  //h_jetDRtrueMom->Write();
  //h_jetDRveryfake->Write();
  //h_jetDRstat1Mom22->Write();
  //h_jetDRstat1MomOther->Write();
  h_jetDR2true->Write();
  h_jetDR2trueNoMom->Write();
  h_jetDR2trueMom->Write();
  h_jetDR2veryfake->Write();
  h_jetDR2stat1Mom22->Write();
  h_jetDR2stat1MomOther->Write();
  //h_jetDRtrueLong->Write();
  //h_jetDRtrueNoMomLong->Write();
  //h_jetDRtrueMomLong->Write();
  //h_jetDRveryfakeLong->Write();
  //h_jetDRstat1Mom22Long->Write();
  //h_jetDRstat1MomOtherLong->Write();
  h_jetDR2trueLong->Write();
  h_jetDR2trueNoMomLong->Write();
  h_jetDR2trueMomLong->Write();
  h_jetDR2veryfakeLong->Write();
  h_jetDR2stat1Mom22Long->Write();
  h_jetDR2stat1MomOtherLong->Write();
  h_jetDRlowIso->Write();
  h_jetDRhighIso->Write();
  h_jetDR2lowIso->Write();
  h_jetDR2highIso->Write();

  h_genIsotrue->Write();
  h_genIsostat1Mom22->Write();
  h_genIsostat1MomGreater22->Write();
  h_genIsostat1MomLess22->Write();
  h_genIsotrueShort->Write();
  h_genIsostat1Mom22Short->Write();
  h_genIsostat1MomGreater22Short->Write();
  h_genIsostat1MomLess22Short->Write();
  
  //h_genPartMomId->Write();
  //h_trueNoMomId->Write();
  //h_trueMomId->Write();

  savePlotsDir(h_1d_global,outfile_,"");
  savePlotsDir(h_1d_all,outfile_,"all");

  savePlotsDir(h_1d_truePh,outfile_,"truePh");
  savePlotsDir(h_1d_truePhNoMom,outfile_,"truePhNoMom");
  savePlotsDir(h_1d_truePhMom,outfile_,"truePhMom");
  savePlotsDir(h_1d_fakePh,outfile_,"fakePh");
  savePlotsDir(h_1d_veryfakePh,outfile_,"veryfakePh");
  savePlotsDir(h_1d_stat1Mom22,outfile_,"stat1Mom22");
  savePlotsDir(h_1d_stat1MomGreater22,outfile_,"stat1MomGreater22");
  savePlotsDir(h_1d_stat1MomLess22,outfile_,"stat1MomLess22");
  savePlotsDir(h_1d_stat23,outfile_,"stat23");

  savePlotsDir(h_1d_truePh_70t100,outfile_,"truePh_70t100");
  savePlotsDir(h_1d_truePhNoMom_70t100,outfile_,"truePhNoMom_70t100");
  savePlotsDir(h_1d_truePhMom_70t100,outfile_,"truePhMom_70t100");
  savePlotsDir(h_1d_fakePh_70t100,outfile_,"fakePh_70t100");
  savePlotsDir(h_1d_veryfakePh_70t100,outfile_,"veryfakePh_70t100");
  savePlotsDir(h_1d_stat1Mom22_70t100,outfile_,"stat1Mom22_70t100");
  savePlotsDir(h_1d_stat1MomGreater22_70t100,outfile_,"stat1MomGreater22_70t100");
  savePlotsDir(h_1d_stat1MomLess22_70t100,outfile_,"stat1MomLess22_70t100");
  savePlotsDir(h_1d_stat23_70t100,outfile_,"stat23_70t100");

  savePlotsDir(h_1d_truePh_100t150,outfile_,"truePh_100t150");
  savePlotsDir(h_1d_truePhNoMom_100t150,outfile_,"truePhNoMom_100t150");
  savePlotsDir(h_1d_truePhMom_100t150,outfile_,"truePhMom_100t150");
  savePlotsDir(h_1d_fakePh_100t150,outfile_,"fakePh_100t150");
  savePlotsDir(h_1d_veryfakePh_100t150,outfile_,"veryfakePh_100t150");
  savePlotsDir(h_1d_stat1Mom22_100t150,outfile_,"stat1Mom22_100t150");
  savePlotsDir(h_1d_stat1MomGreater22_100t150,outfile_,"stat1MomGreater22_100t150");
  savePlotsDir(h_1d_stat1MomLess22_100t150,outfile_,"stat1MomLess22_100t150");
  savePlotsDir(h_1d_stat23_100t150,outfile_,"stat23_100t150");

  savePlotsDir(h_1d_truePh_150t200,outfile_,"truePh_150t200");
  savePlotsDir(h_1d_truePhNoMom_150t200,outfile_,"truePhNoMom_150t200");
  savePlotsDir(h_1d_truePhMom_150t200,outfile_,"truePhMom_150t200");
  savePlotsDir(h_1d_fakePh_150t200,outfile_,"fakePh_150t200");
  savePlotsDir(h_1d_veryfakePh_150t200,outfile_,"veryfakePh_150t200");
  savePlotsDir(h_1d_stat1Mom22_150t200,outfile_,"stat1Mom22_150t200");
  savePlotsDir(h_1d_stat1MomGreater22_150t200,outfile_,"stat1MomGreater22_150t200");
  savePlotsDir(h_1d_stat1MomLess22_150t200,outfile_,"stat1MomLess22_150t200");
  savePlotsDir(h_1d_stat23_150t200,outfile_,"stat23_150t200");

  savePlotsDir(h_1d_truePh_200plus,outfile_,"truePh_200plus");
  savePlotsDir(h_1d_truePhNoMom_200plus,outfile_,"truePhNoMom_200plus");
  savePlotsDir(h_1d_truePhMom_200plus,outfile_,"truePhMom_200plus");
  savePlotsDir(h_1d_fakePh_200plus,outfile_,"fakePh_200plus");
  savePlotsDir(h_1d_veryfakePh_200plus,outfile_,"veryfakePh_200plus");
  savePlotsDir(h_1d_stat1Mom22_200plus,outfile_,"stat1Mom22_200plus");
  savePlotsDir(h_1d_stat1MomGreater22_200plus,outfile_,"stat1MomGreater22_200plus");
  savePlotsDir(h_1d_stat1MomLess22_200plus,outfile_,"stat1MomLess22_200plus");
  savePlotsDir(h_1d_stat23_200plus,outfile_,"stat23_200plus");

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
  plot1D("h_hOverE", t.gamma_hOverE[i] ,   evtweight_, h_1d, "", 100, 0, 0.1);
  plot1D("h_r9", t.gamma_r9[i] ,   evtweight_, h_1d, "", 100, -1, 1);
  plot1D("h_sigmaIetaIeta", t.gamma_sigmaIetaIeta[i] ,   evtweight_, h_1d, "", 100, 0, 0.015);
  plot1D("h_chHadIso" , t.gamma_chHadIso[i] ,   evtweight_, h_1d, "", 200, 0, 20);
  plot1D("h_neuHadIso", t.gamma_neuHadIso[i] ,   evtweight_, h_1d, "", 200, 0, 20);
  plot1D("h_phIso", t.gamma_phIso[i] ,   evtweight_, h_1d, "", 200, 0, 20);
  plot1DUnderOverFlow("h_totalIso", t.gamma_chHadIso[i]+t.gamma_neuHadIso[i]+t.gamma_phIso[i] ,   evtweight_, h_1d, 100, 0, 300);
  plot1DUnderOverFlow("h_totalIsoLong", t.gamma_chHadIso[i]+t.gamma_neuHadIso[i]+t.gamma_phIso[i] ,   evtweight_, h_1d, 100, 0, 600);
  plot1D("h_totalIsoShort", t.gamma_chHadIso[i]+t.gamma_neuHadIso[i]+t.gamma_phIso[i] ,   evtweight_, h_1d, "", 80, 0, 80);
  plot1D("h_chHadIsoRel" , t.gamma_chHadIso[i] / t.gamma_pt[i] ,   evtweight_, h_1d, "", 100, 0, 1);
  plot1D("h_neuHadIsoRel", t.gamma_neuHadIso[i] / t.gamma_pt[i] ,   evtweight_, h_1d, "", 100, 0, 1);
  plot1D("h_phIsoRel", t.gamma_phIso[i] / t.gamma_pt[i],   evtweight_, h_1d, "", 100, 0, 1);
  plot1DUnderOverFlow("h_totalIsoRel", (t.gamma_chHadIso[i]+t.gamma_neuHadIso[i]+t.gamma_phIso[i])/t.gamma_pt[i] ,   evtweight_, h_1d, 30, 0, 3);

  
  outfile_->cd();
  return;
}

void GammaID::fillGenIso(std::map<std::string, TH1D*>& h_1d, const std::string& dirname, int i, float genIso) {
  TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
  if (dir == 0) {
    dir = outfile_->mkdir(dirname.c_str());
  } 
  dir->cd();

  const float totalIso = t.gamma_chHadIso[i]+t.gamma_neuHadIso[i]+t.gamma_phIso[i];

  plot1D("h_genIso", genIso,   evtweight_, h_1d, "", 80, 0, 80);
  plot1D("h_genIsoShort", genIso,   evtweight_, h_1d, "", 100, 0, 15);
  if (genIso<5){
    plot1D("h_totalIso_genIsolatedShort", totalIso , evtweight_, h_1d, "", 80, 0, 80);
    plot1D("h_totalIso_genIsolated", totalIso, evtweight_, h_1d, "", 100, 0, 300);
    if (totalIso<80) plot1D("h_sigmaIetaIeta_genIsolated", t.gamma_sigmaIetaIeta[i] ,   evtweight_, h_1d, "", 100, 0, 0.015);
  }
  else {
    plot1D("h_totalIso_genNonIsolatedShort", totalIso, evtweight_, h_1d, "", 80, 0, 80);
    plot1D("h_totalIso_genNonIsolated", totalIso, evtweight_, h_1d, "", 100, 0, 300);
    if (totalIso<80) plot1D("h_sigmaIetaIeta_genNonIsolated", t.gamma_sigmaIetaIeta[i] ,   evtweight_, h_1d, "", 100, 0, 0.015);
  }

  outfile_->cd();
  return;
}

// C++
#include <iostream>
#include <vector>
#include <set>
#include <cmath>

// ROOT
#include "TDirectory.h"
#include "TTreeCache.h"
#include "Math/VectorUtil.h"
#include "TVector2.h"
#include "TBenchmark.h"

#include "../Tools/utils.h"

// header
#include "MT2Looper.h"

//MT2
#include "../MT2CORE/Plotting/PlotUtilities.h"


using namespace std;
using namespace mt2;

class mt2tree;
class SR;

// mt2 binning for results
const int n_mt2bins = 5;
const float mt2bins[n_mt2bins+1] = {200., 300., 400., 600., 1000., 1500.};
const std::string mt2binsname[n_mt2bins+1] = {"200", "300", "400", "600", "1000", "1500"};

MT2Looper::MT2Looper(){
}
MT2Looper::~MT2Looper(){

};

void MT2Looper::SetSignalRegions(){

  SRVec = getSignalRegions2015LowLumi();
  //SRVec = getSignalRegions2015ExtendedNJets();

  //store histograms with cut values for all variables
  for(unsigned int i = 0; i < SRVec.size(); i++){
    std::vector<std::string> vars = SRVec.at(i).GetListOfVariables();
    TDirectory * dir = (TDirectory*)outfile_->Get(("sr"+SRVec.at(i).GetName()).c_str());
    if (dir == 0) {
      dir = outfile_->mkdir(("sr"+SRVec.at(i).GetName()).c_str());
    } 
    dir->cd();
    for(unsigned int j = 0; j < vars.size(); j++){
      plot1D("h_"+vars.at(j)+"_"+"LOW",  1, SRVec.at(i).GetLowerBound(vars.at(j)), SRVec.at(i).srHistMap, "", 1, 0, 2);
      plot1D("h_"+vars.at(j)+"_"+"UP",   1, SRVec.at(i).GetUpperBound(vars.at(j)), SRVec.at(i).srHistMap, "", 1, 0, 2);
    }
    outfile_->cd();
  }

}


void MT2Looper::loop(TChain* chain, std::string output_name){

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  gROOT->cd();
  cout << "[MT2Looper::loop] creating output file: " << output_name << endl;

  outfile_ = new TFile(output_name.c_str(),"RECREATE") ; 

  cout << "[MT2Looper::loop] setting up histos" << endl;

  SetSignalRegions();

/*
  std::map<std::string, TH1D*> h_1d_nocut;
  std::map<std::string, TH1D*> h_1d_srbase;
  std::map<std::string, TH1D*> h_1d_crgjbase;
  std::map<std::string, TH1D*> h_1d_crdybase;
  std::map<std::string, TH1D*> h_1d_crslbase;
  std::map<std::string, TH1D*> h_1d_crslw;
  std::map<std::string, TH1D*> h_1d_crsltt;
  std::map<std::string, TH1D*> h_1d_crslmubase;
  std::map<std::string, TH1D*> h_1d_crslmuw;
  std::map<std::string, TH1D*> h_1d_crslmutt;
  std::map<std::string, TH1D*> h_1d_crslelbase;
  std::map<std::string, TH1D*> h_1d_crslelw;
  std::map<std::string, TH1D*> h_1d_crsleltt;
  std::map<std::string, TH1D*> h_1d_crslhadbase;
  std::map<std::string, TH1D*> h_1d_crslhadw;
  std::map<std::string, TH1D*> h_1d_crslhadtt;
*/


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

      if (t.nVert == 0) continue;

      // remove low pt QCD samples 
      if (t.evt_id >= 100 && t.evt_id < 108) continue;

      //---------------------
      // set weights and start making plots
      //---------------------
      outfile_->cd();
      const float lumi = 4.;
      evtweight_ = t.evt_scale1fb * lumi;

      plot1D("h_nvtx",       t.nVert,       evtweight_, h_1d_global, ";N(vtx)", 80, 0, 80);
      plot1D("h_mt2",       t.mt2,       evtweight_, h_1d_global, ";M_{T2} [GeV]", 80, 0, 800);

      // note: this will double count some leptons, since reco leptons can appear as PFcands
      nlepveto_ = t.nMuons10 + t.nElectrons10 + t.nPFLep5LowMT + t.nPFHad10LowMT;
      int nlep_unique = nlepveto_;

      // variables for single lep control region
      bool doSLplots = false;
      bool doSLMUplots = false;
      bool doSLELplots = false;
      bool doSLHADplots = false;
      leppt_ = -1.;
      mt_ = -1.;
      // do lepton overlap removal and 1L CR selections
      if (nlepveto_ >= 1) {
	std::vector<MT2Looper::lepcand> all_cands;
	std::vector<MT2Looper::lepcand> unique_cands;
	// check reco leptons - apply MT cut later
	// do overlap with PFcands below
	if ( t.nMuons10 > 0 || t.nElectrons10 > 0) {
	  for (int ilep = 0; ilep < t.nlep; ++ilep) {
	    MT2Looper::lepcand cand;
	    cand.pt = t.lep_pt[ilep];
	    cand.phi = t.lep_phi[ilep];
	    cand.mt = sqrt( 2 * t.met_pt * cand.pt * ( 1 - cos( t.met_phi - cand.phi) ) );
	    cand.eta = t.lep_eta[ilep];
	    cand.pdgId = t.lep_pdgId[ilep];
	    cand.isPFCand = false;

	    // add cand to vector
	    all_cands.push_back(cand);
	  } // loop over reco leps
	} 
	// pf leptons: need to find cands passing selection. 
	else if (t.nPFLep5LowMT > 0) {
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
	    cand.eta = t.isoTrack_eta[itrk];
	    cand.isPFCand = true;

	    // cand passes cuts: add to vector
	    if (cand.mt > 100.) continue;
	    all_cands.push_back(cand);
	  } // loop on isoTracks
	}
	// pf hadrons: need to find cands passing selection. 
	else if (t.nPFHad10LowMT > 0) {
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
	    cand.eta = t.isoTrack_eta[itrk];
	    cand.isPFCand = true;

	    // cand passes cuts: add to vector
	    if (cand.mt > 100.) continue;
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
	      if (all_cands.at(icand).pdgId == all_cands.at(jcand).pdgId && 
		  all_cands.at(icand).isPFCand && !all_cands.at(jcand).isPFCand) 
		keep = false;
	    }
	  }
	  if (keep) unique_cands.push_back(all_cands.at(icand));
	}
	
	nlep_unique = unique_cands.size() ; // useful counter (for DYCR)

	// check size of unique cands. if size == 1 and MT < 100, fill 1L CR plots
	if (unique_cands.size() == 1) {
	  leppt_ = unique_cands.at(0).pt;
	  mt_ = unique_cands.at(0).mt;
	  if (mt_ < 100.) {
	    doSLplots = true;
	    if (abs(unique_cands.at(0).pdgId) == 13) doSLMUplots = true;
	    else if (abs(unique_cands.at(0).pdgId) == 11) doSLELplots = true;
	    else if (abs(unique_cands.at(0).pdgId) == 211) doSLHADplots = true;
	  } // mt < 100
	} // 1 unique cand

      } // for 1L control region

      // Variables for gamma+jets control region
      bool doGJplots = false;
      int jetIdx0 = 0;
      int jetIdx1 = 1;
      if (t.evt_id < 300 && t.evt_id!=120) { // remove lowest qcd_ht sample
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
      	  if ( (t.lep_charge[0] * t.lep_charge[1] == -1)
      	       && (abs(t.lep_pdgId[0]) == abs(t.lep_pdgId[1]) )
      	       && (fabs(t.zll_mass - 90) < 20 ) 
	       && t.lep_pt[0] > 20 && t.lep_pt[1] > 20) {
      		 nlepveto_ = nlep_unique - 2;
      		 doDYplots = true;
      	       }
      	} // nlep == 2
      }// evt_id 

      ////////////////////////////////////
      /// done with overall selection  /// 
      ////////////////////////////////////
      ///   time to fill histograms    /// 
      ////////////////////////////////////

      //fillHistos(h_1d_nocut, "nocut");

      //fillHistosSignalRegion(h_1d_srbase, "srbase");

      fillHistosSignalRegion("sr");

      if (doGJplots) {
        saveGJplots = true;
        fillHistosCRGJ("crgjbase", jetIdx0, jetIdx1);
      }
      if (doDYplots) {
        saveDYplots = true;
        fillHistosCRDY("crdy");
      }
      if (doSLplots) {
        saveSLplots = true;
        fillHistosCRSL("crsl");
      }
      if (doSLMUplots) {
        fillHistosCRSL("crslmu");
      }
      if (doSLELplots) {
        fillHistosCRSL("crslel");
      }
      if (doSLHADplots) {
        fillHistosCRSL("crslhad");
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
  //savePlotsDir(h_1d_global,outfile_,"");
  //savePlotsDir(h_1d_nocut,outfile_,"nocut");
  //savePlotsDir(h_1d_srbase,outfile_,"srbase");

  for(unsigned int srN = 0; srN < SRVec.size(); srN++){
    if(!SRVec.at(srN).srHistMap.empty()){
      savePlotsDir(SRVec.at(srN).srHistMap, outfile_, ("sr"+SRVec.at(srN).GetName()).c_str());
    }
  }
  if (saveGJplots) {
    for(unsigned int srN = 0; srN < SRVec.size(); srN++){
      if(!SRVec.at(srN).crgjHistMap.empty()){
        savePlotsDir(SRVec.at(srN).crgjHistMap, outfile_, ("crgj"+SRVec.at(srN).GetName()).c_str());
      }
    }
  }
  if (saveDYplots) {
    for(unsigned int srN = 0; srN < SRVec.size(); srN++){
      if(!SRVec.at(srN).crdyHistMap.empty()){
        savePlotsDir(SRVec.at(srN).crdyHistMap, outfile_, ("crdy"+SRVec.at(srN).GetName()).c_str());
      }
    }
  }
  if (saveSLplots) {
    for(unsigned int srN = 0; srN < SRVec.size(); srN++){
      if(!SRVec.at(srN).crslHistMap.empty()){
        savePlotsDir(SRVec.at(srN).crslHistMap, outfile_, ("crsl"+SRVec.at(srN).GetName()).c_str());
      }
    }
  }
  if (saveSLMUplots) {
    for(unsigned int srN = 0; srN < SRVec.size(); srN++){
      if(!SRVec.at(srN).crslmuHistMap.empty()){
        savePlotsDir(SRVec.at(srN).crslmuHistMap, outfile_, ("crslmu"+SRVec.at(srN).GetName()).c_str());
      }
    }
  }
  if (saveSLELplots) {
    for(unsigned int srN = 0; srN < SRVec.size(); srN++){
      if(!SRVec.at(srN).crslelHistMap.empty()){
        savePlotsDir(SRVec.at(srN).crslelHistMap, outfile_, ("crslel"+SRVec.at(srN).GetName()).c_str());
      }
    }
  }
  if (saveSLHADplots) {
    for(unsigned int srN = 0; srN < SRVec.size(); srN++){
      if(!SRVec.at(srN).crslhadHistMap.empty()){
        savePlotsDir(SRVec.at(srN).crslhadHistMap, outfile_, ("crslhad"+SRVec.at(srN).GetName()).c_str());
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

void MT2Looper::fillHistosSignalRegion(const std::string& prefix, const std::string& suffix) {

  std::map<std::string, float> values;
  values["deltaPhiMin"] = t.deltaPhiMin;
  values["diffMetMhtOverMet"]  = t.diffMetMht/t.met_pt;
  values["nlep"]        = nlepveto_;
  values["j1pt"]        = t.jet_pt[0];
  values["j2pt"]        = t.jet_pt[1];
  values["njets"]       = t.nJet40;
  values["nbjets"]      = t.nBJet40;
  values["lowMT"]       = (t.minMTBMet < 200. && t.mt2 < 400.);
  values["mt2"]         = t.mt2;
  values["ht"]          = t.ht;
  values["met"]         = t.met_pt;
  //values["passesHtMet"] = ( (t.ht > 450. && t.met_pt > 200.) || (t.ht > 1000. && t.met_pt > 30.) );


  for(unsigned int srN = 0; srN < SRVec.size(); srN++){
    if(SRVec.at(srN).PassesSelection(values)){
      fillHistos(SRVec.at(srN).srHistMap, prefix+SRVec.at(srN).GetName(), suffix);
      break;//signal regions are orthogonal, event cannot be in more than one
    }
  }

  //plot1D("h_SignalRegion",  sr_jets+sr_htmet,   evtweight_, h_1d_global, ";Signal Region", 100, 0, 100);

  return;
}

// hists for single lepton control region
void MT2Looper::fillHistosCRSL(const std::string& prefix, const std::string& suffix) {

  std::map<std::string, float> values;
  values["deltaPhiMin"] = t.deltaPhiMin;
  values["diffMetMhtOverMet"]  = t.diffMetMht/t.met_pt;
  values["nlep"]        = 0; //dummy value
  values["j1pt"]        = t.jet_pt[0];
  values["j2pt"]        = t.jet_pt[1];
  values["njets"]       = t.nJet40;
  values["nbjets"]      = t.nBJet40;
  values["lowMT"]       = (t.minMTBMet < 200. && t.mt2 < 400.);
  values["mt2"]         = t.mt2;
  values["ht"]          = t.ht;
  values["met"]         = t.met_pt;

  for(unsigned int srN = 0; srN < SRVec.size(); srN++){
    if(SRVec.at(srN).PassesSelection(values)){
      if(prefix=="crsl")    fillHistosSingleLepton(SRVec.at(srN).crslHistMap,    prefix+SRVec.at(srN).GetName(), suffix);
      if(prefix=="crslmu")  fillHistosSingleLepton(SRVec.at(srN).crslmuHistMap,  prefix+SRVec.at(srN).GetName(), suffix);
      if(prefix=="crslel")  fillHistosSingleLepton(SRVec.at(srN).crslelHistMap,  prefix+SRVec.at(srN).GetName(), suffix);
      if(prefix=="crslhad") fillHistosSingleLepton(SRVec.at(srN).crslhadHistMap, prefix+SRVec.at(srN).GetName(), suffix);
      break;//control regions are orthogonal, event cannot be in more than one
    }
  }

  return;
}

// hists for Gamma+Jets control region
void MT2Looper::fillHistosCRGJ(const std::string& prefix, const int jetIdx0, const int jetIdx1, const std::string& suffix) {

  if (t.ngamma==0) return;

  bool passSieie = t.gamma_idCutBased[0] ? true : false; // just deal with the standard case now. Worry later about sideband in sieie

  // fill hists
  if (!passSieie) return;

  std::map<std::string, float> values;
  values["deltaPhiMin"] = t.gamma_deltaPhiMin;
  values["diffMetMhtOverMet"]  = t.gamma_diffMetMht/t.gamma_met_pt;
  values["nlep"]        = nlepveto_;
  values["j1pt"]        = t.jet_pt[jetIdx0];
  values["j2pt"]        = t.jet_pt[jetIdx1];
  values["njets"]       = t.gamma_nJet40;
  values["nbjets"]      = t.gamma_nBJet40;
  values["lowMT"]       = (t.gamma_minMTBMet < 200. && t.gamma_mt2 < 400.);
  values["mt2"]         = t.gamma_mt2;
  values["ht"]          = t.gamma_ht;
  values["met"]         = t.gamma_met_pt;

  for(unsigned int srN = 0; srN < SRVec.size(); srN++){
    if(SRVec.at(srN).PassesSelection(values)){
      fillHistosGammaJets(SRVec.at(srN).crgjHistMap, prefix+SRVec.at(srN).GetName(), suffix);
      break;//control regions are orthogonal, event cannot be in more than one
    }
  }

  return;
}

// hists for Zll control region
void MT2Looper::fillHistosCRDY(const std::string& prefix, const std::string& suffix) {

  if (t.nlep!=2) return;

  std::map<std::string, float> values;
  values["deltaPhiMin"] = t.zll_deltaPhiMin;
  values["diffMetMhtOverMet"]  = t.zll_diffMetMht/t.zll_met_pt;
  values["nlep"]        = nlepveto_;
  values["j1pt"]        = t.jet_pt[0];
  values["j2pt"]        = t.jet_pt[1];
  values["njets"]       = t.nJet40;
  values["nbjets"]      = t.nBJet40;
  values["lowMT"]       = (t.zll_minMTBMet < 200. && t.zll_mt2 < 400.);
  values["mt2"]         = t.zll_mt2;
  values["ht"]          = t.zll_ht;
  values["met"]         = t.zll_met_pt;

  for(unsigned int srN = 0; srN < SRVec.size(); srN++){
    if(SRVec.at(srN).PassesSelection(values)){
      fillHistosGammaJets(SRVec.at(srN).crdyHistMap, prefix+SRVec.at(srN).GetName(), suffix);
      break;//control regions are orthogonal, event cannot be in more than one
    }
  }

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

  fillHistos(h_1d, dirname, s);
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

  if (iso < 10)  {
    plot1D("h_mt2bins"+s,       t.gamma_mt2,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);
  if (dirname=="crgjbase" || dirname=="crgjL" || dirname=="crgjM" || dirname=="crgjH") {
      plot1D("h_Events"+s,  1, 1, h_1d, ";Events, Unweighted", 1, 0, 2);
      plot1D("h_Events_w"+s,  1,   evtweight_, h_1d, ";Events, Weighted", 1, 0, 2);
      plot1D("h_mt2"+s,       t.gamma_mt2,   evtweight_, h_1d, "; M_{T2} [GeV]", 150, 0, 1500);
      plot1D("h_met"+s,       t.gamma_met_pt,   evtweight_, h_1d, ";E_{T}^{miss} [GeV]", 150, 0, 1500);
      plot1D("h_ht"+s,       t.gamma_ht,   evtweight_, h_1d, ";H_{T} [GeV]", 120, 0, 3000);
      plot1D("h_nJet40"+s,       t.gamma_nJet40,   evtweight_, h_1d, ";N(jets)", 15, 0, 15);
      plot1D("h_nBJet40"+s,      t.gamma_nBJet40,   evtweight_, h_1d, ";N(bjets)", 6, 0, 6);
      plot1D("h_deltaPhiMin"+s,  t.gamma_deltaPhiMin,   evtweight_, h_1d, ";#Delta#phi_{min}", 32, 0, 3.2);
      plot1D("h_diffMetMht"+s,   t.gamma_diffMetMht,   evtweight_, h_1d, ";|E_{T}^{miss} - MHT| [GeV]", 120, 0, 300);
      plot1D("h_diffMetMhtOverMet"+s,   t.gamma_diffMetMht/t.gamma_met_pt,   evtweight_, h_1d, ";|E_{T}^{miss} - MHT| / E_{T}^{miss}", 100, 0, 2.);
      plot1D("h_minMTBMet"+s,   t.gamma_minMTBMet,   evtweight_, h_1d, ";min M_{T}(b, E_{T}^{miss}) [GeV]", 150, 0, 1500);
      plot1D("h_nlepveto"+s,     nlepveto_,   evtweight_, h_1d, ";N(leps)", 10, 0, 10);
    }
    
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

  plot1D("h_mt2bins"+s,       t.zll_mt2,   evtweight_, h_1d, "; M_{T2} [GeV]", n_mt2bins, mt2bins);

  if (dirname=="crdybase" || dirname=="crdyL" || dirname=="crdyM" || dirname=="crdyH") {
    plot1D("h_Events"+s,  1, 1, h_1d, ";Events, Unweighted", 1, 0, 2);
    plot1D("h_Events_w"+s,  1,   evtweight_, h_1d, ";Events, Weighted", 1, 0, 2);
    plot1D("h_mt2"+s,       t.zll_mt2,   evtweight_, h_1d, "; M_{T2} [GeV]", 150, 0, 1500);
    plot1D("h_met"+s,       t.zll_met_pt,   evtweight_, h_1d, ";E_{T}^{miss} [GeV]", 150, 0, 1500);
    plot1D("h_ht"+s,       t.zll_ht,   evtweight_, h_1d, ";H_{T} [GeV]", 120, 0, 3000);
    plot1D("h_nJet40"+s,       t.nJet40,   evtweight_, h_1d, ";N(jets)", 15, 0, 15);
    plot1D("h_nBJet40"+s,      t.nBJet40,   evtweight_, h_1d, ";N(bjets)", 6, 0, 6);
    plot1D("h_deltaPhiMin"+s,  t.zll_deltaPhiMin,   evtweight_, h_1d, ";#Delta#phi_{min}", 32, 0, 3.2);
    plot1D("h_diffMetMht"+s,   t.zll_diffMetMht,   evtweight_, h_1d, ";|E_{T}^{miss} - MHT| [GeV]", 120, 0, 300);
    plot1D("h_diffMetMhtOverMet"+s,   t.zll_diffMetMht/t.zll_met_pt,   evtweight_, h_1d, ";|E_{T}^{miss} - MHT| / E_{T}^{miss}", 100, 0, 2.);
    plot1D("h_minMTBMet"+s,   t.zll_minMTBMet,   evtweight_, h_1d, ";min M_{T}(b, E_{T}^{miss}) [GeV]", 150, 0, 1500);
    plot1D("h_nlepveto"+s,     nlepveto_,   evtweight_, h_1d, ";N(leps)", 10, 0, 10);
  }

  outfile_->cd();
  return;
}

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

#include "../CORE/Tools/utils.h"

// header
#include "LepEffLooperMinimal.h"

//MT2
#include "../MT2CORE/Plotting/PlotUtilities.h"


using namespace std;

class mt2tree;

bool applyEffMaps_zjets = false;
bool applyEffMaps_ttbar = false;
bool scaleErrorsForLumi = false;
bool requireGen1L = false;
bool doEMu = true;
bool doHadTau = true;
bool doPFCands = true;

// pt binning for hists
const int n_ptbins = 62;
const double ptbins[n_ptbins+1] = {0, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
				  22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50,
				  55, 60, 65, 70, 75, 80, 85, 90, 95, 100,
				  110, 120, 130, 140, 150, 160, 170, 180, 190, 200,
				  250, 300, 350, 400, 450, 500,
				  600, 700, 800, 900, 1000};

// coarse pt binning for hists
const int n_ptbins_coarse = 7;
const double ptbins_coarse[n_ptbins_coarse+1] = {10, 20, 30, 40, 50, 70, 100, 200};
const float ptbins_coarse_float[n_ptbins_coarse+1] = {10, 20, 30, 40, 50, 70, 100, 200};

// coarse pt binning for hists
const int n_ptbins_coarse_pf = 8;
const double ptbins_coarse_pf[n_ptbins_coarse_pf+1] = {5, 10, 20, 30, 40, 50, 70, 100, 200};
const float ptbins_coarse_float_pf[n_ptbins_coarse_pf+1] = {5, 10, 20, 30, 40, 50, 70, 100, 200};

// iso binning for efficiency hists
const int n_isobins = 12;
const double isobins[n_isobins+1] = {0.0, 0.01, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 1.0, 2.0, 5.0, 10.0};
const float isobins_float[n_isobins+1] = {0.0, 0.01, 0.05, 0.1, 0.15, 0.2, 0.3, 0.4, 0.5, 1.0, 2.0, 5.0, 10.0};

LepEffLooperMinimal::LepEffLooperMinimal(){
}
LepEffLooperMinimal::~LepEffLooperMinimal(){

};

//____________________________________________________
void LepEffLooperMinimal::loop(TChain* chain, std::string output_name){

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  gROOT->cd();
  cout << "[LepEffLooperMinimal::loop] creating output file: " << output_name << endl;

  outfile_ = new TFile(output_name.c_str(),"RECREATE") ; 

  if (applyEffMaps_zjets) {
    TFile* f_effmap = new TFile("output/V00-01-05_25ns_nolepjetcleaning_noiso_ht200_reco03_pfMiniIso_an05_1match_morevars/dyjetsll_ht.root");
    TH1D* h_eff_el_reco_pt_zjets_temp = (TH1D*) f_effmap->Get("h_eff_reco_pt_iso_el");
    TH1D* h_eff_mu_reco_pt_zjets_temp = (TH1D*) f_effmap->Get("h_eff_reco_pt_iso_mu");
    TH2D* h_eff_el_reco_pt_activity_zjets_temp = (TH2D*) f_effmap->Get("h_eff_reco_pt_relIsoAn04_iso_el");
    TH2D* h_eff_mu_reco_pt_activity_zjets_temp = (TH2D*) f_effmap->Get("h_eff_reco_pt_relIsoAn04_iso_mu");
    TH1D* h_eff_el_pf_pt_zjets_temp = (TH1D*) f_effmap->Get("h_eff_pf_pt_iso_el");
    TH1D* h_eff_mu_pf_pt_zjets_temp = (TH1D*) f_effmap->Get("h_eff_pf_pt_iso_mu");
    TH2D* h_eff_el_pf_pt_activity_zjets_temp = (TH2D*) f_effmap->Get("h_eff_pf_pt_relIsoAn04_iso_el");
    TH2D* h_eff_mu_pf_pt_activity_zjets_temp = (TH2D*) f_effmap->Get("h_eff_pf_pt_relIsoAn04_iso_mu");

    outfile_->cd();
    h_eff_el_reco_pt_zjets_ = (TH1D*) h_eff_el_reco_pt_zjets_temp->Clone("h_eff_el_reco_pt_");
    h_eff_mu_reco_pt_zjets_ = (TH1D*) h_eff_mu_reco_pt_zjets_temp->Clone("h_eff_mu_reco_pt_");
    h_eff_el_reco_pt_activity_zjets_ = (TH2D*) h_eff_el_reco_pt_activity_zjets_temp->Clone("h_eff_el_reco_pt_activity_zjets_");
    h_eff_mu_reco_pt_activity_zjets_ = (TH2D*) h_eff_mu_reco_pt_activity_zjets_temp->Clone("h_eff_mu_reco_pt_activity_zjets_");
    h_eff_el_pf_pt_zjets_ = (TH1D*) h_eff_el_pf_pt_zjets_temp->Clone("h_eff_el_pf_pt_");
    h_eff_mu_pf_pt_zjets_ = (TH1D*) h_eff_mu_pf_pt_zjets_temp->Clone("h_eff_mu_pf_pt_");
    h_eff_el_pf_pt_activity_zjets_ = (TH2D*) h_eff_el_pf_pt_activity_zjets_temp->Clone("h_eff_el_pf_pt_activity_zjets_");
    h_eff_mu_pf_pt_activity_zjets_ = (TH2D*) h_eff_mu_pf_pt_activity_zjets_temp->Clone("h_eff_mu_pf_pt_activity_zjets_");
    f_effmap->Close();
  }
  
  if (applyEffMaps_ttbar) {
    TFile* f_effmap = new TFile("output/V00-01-05_25ns_nolepjetcleaning_noiso_ht200_reco03_pfMiniIso_an05_1match_morevars/ttall_mg_lo.root");
    TH1D* h_eff_el_reco_pt_ttbar_temp = (TH1D*) f_effmap->Get("h_eff_reco_pt_iso_el");
    TH1D* h_eff_mu_reco_pt_ttbar_temp = (TH1D*) f_effmap->Get("h_eff_reco_pt_iso_mu");
    TH2D* h_eff_el_reco_pt_activity_ttbar_temp = (TH2D*) f_effmap->Get("h_eff_reco_pt_relIsoAn04_iso_el");
    TH2D* h_eff_mu_reco_pt_activity_ttbar_temp = (TH2D*) f_effmap->Get("h_eff_reco_pt_relIsoAn04_iso_mu");
    TH1D* h_eff_el_pf_pt_ttbar_temp = (TH1D*) f_effmap->Get("h_eff_pf_pt_iso_el");
    TH1D* h_eff_mu_pf_pt_ttbar_temp = (TH1D*) f_effmap->Get("h_eff_pf_pt_iso_mu");
    TH2D* h_eff_el_pf_pt_activity_ttbar_temp = (TH2D*) f_effmap->Get("h_eff_pf_pt_relIsoAn04_iso_el");
    TH2D* h_eff_mu_pf_pt_activity_ttbar_temp = (TH2D*) f_effmap->Get("h_eff_pf_pt_relIsoAn04_iso_mu");

    outfile_->cd();
    h_eff_el_reco_pt_ttbar_ = (TH1D*) h_eff_el_reco_pt_ttbar_temp->Clone("h_eff_el_reco_pt_");
    h_eff_mu_reco_pt_ttbar_ = (TH1D*) h_eff_mu_reco_pt_ttbar_temp->Clone("h_eff_mu_reco_pt_");
    h_eff_el_reco_pt_activity_ttbar_ = (TH2D*) h_eff_el_reco_pt_activity_ttbar_temp->Clone("h_eff_el_reco_pt_activity_ttbar_");
    h_eff_mu_reco_pt_activity_ttbar_ = (TH2D*) h_eff_mu_reco_pt_activity_ttbar_temp->Clone("h_eff_mu_reco_pt_activity_ttbar_");
    h_eff_el_pf_pt_ttbar_ = (TH1D*) h_eff_el_pf_pt_ttbar_temp->Clone("h_eff_el_pf_pt_");
    h_eff_mu_pf_pt_ttbar_ = (TH1D*) h_eff_mu_pf_pt_ttbar_temp->Clone("h_eff_mu_pf_pt_");
    h_eff_el_pf_pt_activity_ttbar_ = (TH2D*) h_eff_el_pf_pt_activity_ttbar_temp->Clone("h_eff_el_pf_pt_activity_ttbar_");
    h_eff_mu_pf_pt_activity_ttbar_ = (TH2D*) h_eff_mu_pf_pt_activity_ttbar_temp->Clone("h_eff_mu_pf_pt_activity_ttbar_");
    f_effmap->Close();
  }
  

  // File Loop
  int nDuplicates = 0;
  int nEvents = chain->GetEntries();
  unsigned int nEventsChain = nEvents;
  cout << "[LepEffLooperMinimal::loop] running on " << nEventsChain << " events" << endl;
  unsigned int nEventsTotal = 0;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;
  while ( (currentFile = (TFile*)fileIter.Next()) ) {
    cout << "[LepEffLooperMinimal::loop] running on file: " << currentFile->GetTitle() << endl;

    // Get File Content
    TFile f( currentFile->GetTitle() );
    TTree *tree = (TTree*)f.Get("mt2");
    TTreeCache::SetLearnEntries(10);
    tree->SetCacheSize(128*1024*1024);
    
    // Use this to speed things up a little when not looking at some branches
    tree->SetBranchStatus("gamma_*", 0); 
    tree->SetBranchStatus("zll_*", 0); 
    tree->SetBranchStatus("zll_ht", 1); 
    tree->SetBranchStatus("tau_*", 0); 
    tree->SetBranchStatus("jet_*", 0); 
    tree->SetBranchStatus("lep_mass", 0); 
    tree->SetBranchStatus("lep_charge", 0); 
    tree->SetBranchStatus("lep_dxy", 0); 
    tree->SetBranchStatus("lep_dz", 0); 
    tree->SetBranchStatus("lep_mcMatchId", 0); 
    tree->SetBranchStatus("lep_lostHits", 0); 
    tree->SetBranchStatus("lep_convVeto", 0); 
    tree->SetBranchStatus("lep_tightCharge", 0); 

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

      //if (t.ngenLep+t.ngenLepFromTau < 1) continue;
      if (t.nVert == 0) continue;

      // remove low pt QCD samples 
      if (t.evt_id >= 100 && t.evt_id < 108) continue;

      // cut on HT: different var for Z samples
      if (t.evt_id >= 702 && t.evt_id <= 705) {
	if (t.zll_ht < 200.) continue;
      }
      else if (t.ht < 200.) continue;

      //---------------------
      // set weights and start making plots
      //---------------------
      outfile_->cd();
      const float lumi = 4.;
      //const float lumi = 1.;
      evtweight_ = t.evt_scale1fb * lumi;

      if ( requireGen1L && !((t.ngenLep == 1 && t.ngenTau == 0) || (t.ngenLep == 0 && t.ngenTau == 1 && t.ngenLepFromTau == 1)) ) continue; 

      // -------------------------------------------------
      //           loop over gen leptons
      // -------------------------------------------------

      if (doEMu) {
	//   do e,mu from W and from tau in same loop
	int ngenLepTotal = t.ngenLep + t.ngenLepFromTau;
	plot1D("h_ngenLepTotal",      ngenLepTotal,       evtweight_, h_1d_global, ";N(gen lep)", 5, 0, 5);
	for (int igen = 0; igen < ngenLepTotal; ++igen) {

	  genlepcand genlep;
	  int genidx = igen;
	  if (igen < t.ngenLep) {
	    genlep.pt = t.genLep_pt[genidx];
	    genlep.eta = t.genLep_eta[genidx];
	    genlep.phi = t.genLep_phi[genidx];
	    genlep.pdgId = t.genLep_pdgId[genidx];
	    genlep.isFromTau = false;
	  }
	  else {
	    genidx = igen - t.ngenLep;
	    genlep.pt = t.genLepFromTau_pt[genidx];
	    genlep.eta = t.genLepFromTau_eta[genidx];
	    genlep.phi = t.genLepFromTau_phi[genidx];
	    genlep.pdgId = t.genLepFromTau_pdgId[genidx];
	    genlep.isFromTau = true;
	  }

	  // gen level acceptance cuts
	  if (genlep.pt < 5. || fabs(genlep.eta) > 2.4) continue;

	  // -- loop over reco leptons, make plots to get iso eff vs pt, activity, etc
	  for (int ilep=0; ilep < t.nlep; ++ilep) {
	    // check flavor first
	    if (abs(genlep.pdgId) != abs(t.lep_pdgId[ilep])) continue;
	    // check dR match
	    float dr = DeltaR(genlep.eta,t.lep_eta[ilep],genlep.phi,t.lep_phi[ilep]);
	    if (dr > 0.1) continue;
	  
	    // plots by lepton flavor
	    if (abs(genlep.pdgId) == 11) fillHistosReco(h_1d_global, "", ilep, "_el");
	    else if (abs(genlep.pdgId) == 13) fillHistosReco(h_1d_global, "", ilep, "_mu");

	    // reco leptons: mini iso cut
	    if ((abs(genlep.pdgId) == 11 && t.lep_miniRelIso[ilep] < 0.10) || 
	        (abs(genlep.pdgId) == 13 && t.lep_miniRelIso[ilep] < 0.20) ) {

	    // // reco leptons: abs mini iso cut
	    // if ((abs(genlep.pdgId) == 11 && t.lep_miniRelIso[ilep]*t.lep_pt[ilep] < 2.) || 
	    //     (abs(genlep.pdgId) == 13 && t.lep_miniRelIso[ilep]*t.lep_pt[ilep] < 2.) ) {

	      // // reco leptons: reliso03  cut
	      // if (t.lep_relIso03[ilep] < 0.15) {

	      // isolated plots by lepton flavor
	      if (abs(genlep.pdgId) == 11) fillHistosReco(h_1d_global, "", ilep, "_iso_el");
	      else if (abs(genlep.pdgId) == 13) fillHistosReco(h_1d_global, "", ilep, "_iso_mu");
	    } // passes iso

	    // take only first match
	    break; 
	  } // loop over reco leps

	  if (doPFCands) {
	    // -- loop over PF cands, make plots to get iso eff vs pt, activity, etc
	    for (int ipf=0; ipf < t.nisoTrack; ++ipf) {
	      // // check pt
	      // if (t.isoTrack_pt[ipf] < 10.) continue;
	      // check flavor first
	      if (abs(genlep.pdgId) != abs(t.isoTrack_pdgId[ipf])) continue;
	      // check dR match
	      float dr = DeltaR(genlep.eta,t.isoTrack_eta[ipf],genlep.phi,t.isoTrack_phi[ipf]);
	      if (dr > 0.1) continue;

	      // plots by lepton flavor
	      if (abs(genlep.pdgId) == 11) fillHistosPF(h_1d_global, "", ipf, "_el");
	      else if (abs(genlep.pdgId) == 13) fillHistosPF(h_1d_global, "", ipf, "_mu");

	      // --- use 0.1 cut when comparing directly to PF hadron efficiency for taus
	      //if (t.isoTrack_absIso[ipf]/t.isoTrack_pt[ipf] < 0.2) {
	      if (t.isoTrack_absIso[ipf]/t.isoTrack_pt[ipf] < 0.1) {

		// iso plots by lepton flavor
		if (abs(genlep.pdgId) == 11) fillHistosPF(h_1d_global, "", ipf, "_iso_el");
		else if (abs(genlep.pdgId) == 13) fillHistosPF(h_1d_global, "", ipf, "_iso_mu");
		
	      } // passes iso
	    
	      // take only first match
	      break;
	    } // loop over pf leps
	  } // if doPFCands

	} // loop over gen leps

      } // if doEMu

      //--------------------------------------------------------------
      //------------  now loop on gen hadronic taus ------------------
      //--------------------------------------------------------------

      if (doHadTau) {
	for (int igen = 0; igen < t.ngenTau; ++igen) {
	  // only consider 1-prong and 3-prong taus
	  int decayMode = t.genTau_decayMode[igen];
	  int neutralDaughters = t.genTau_neutralDaughters[igen];
	  if (decayMode != 1 && decayMode != 3) continue;
	  if (fabs(t.genTau_eta[igen]) > 2.5) continue;

	  if (decayMode == 1) {
	    plot1D("h_gen_leadTrackPt_tau1p",  t.genTau_leadTrackPt[igen],   evtweight_, h_1d_global, ";gen lead track p_{T} [GeV]", 100, 0., 200.);
	    if (t.genTau_leadTrackPt[igen] > 10.) plot1D("h_gen_tauEta_tau1p",  t.genTau_eta[igen],   evtweight_, h_1d_global, ";gen #tau #eta", 60, -3, 3);
	  }
	  
	  if (doPFCands) {
	    // -- loop over PF cands, make plots to get iso eff vs pt, activity, etc
	    for (int ipf=0; ipf < t.nisoTrack; ++ipf) {
	      // // check pt
	      // if (t.isoTrack_pt[ipf] < 10.) continue;
	      // check flavor first
	      //if (abs(t.isoTrack_pdgId[ipf]) != 211) continue;
	      // check dR match
	      float dr = DeltaR(t.genTau_eta[igen],t.isoTrack_eta[ipf],t.genTau_phi[igen],t.isoTrack_phi[ipf]);
	      //plot1D("h_dr_pfgen_tau1p",  dr,   evtweight_, h_1d_global, ";#Delta R(gen #tau, PF cand)", 600, 0., 6.);
	      if (dr > 0.1) continue;

	      // plots before iso
	      if (decayMode == 1) {
		plot1D("h_gen_leadTrackPt_match_tau1p",  t.genTau_leadTrackPt[igen],   evtweight_, h_1d_global, ";gen lead track p_{T} [GeV]", 100, 0., 200.);
		if (t.genTau_leadTrackPt[igen] > 10.) plot1D("h_gen_tauEta_match_tau1p",  t.genTau_eta[igen],   evtweight_, h_1d_global, ";gen #tau #eta", 60, -3, 3);
		fillHistosPF(h_1d_global, "", ipf, "_tau1p");
		if (neutralDaughters == 0) fillHistosPF(h_1d_global, "", ipf, "_tau1p0n");
		else if (neutralDaughters == 1) fillHistosPF(h_1d_global, "", ipf, "_tau1p1n");
		else if (neutralDaughters == 2) fillHistosPF(h_1d_global, "", ipf, "_tau1p2n");
		else if (neutralDaughters >= 3) fillHistosPF(h_1d_global, "", ipf, "_tau1p3n");
		if (neutralDaughters >= 1) fillHistosPF(h_1d_global, "", ipf, "_tau1pgt1n");
	      }
	      else if (decayMode == 3) fillHistosPF(h_1d_global, "", ipf, "_tau3p");

	      if (t.isoTrack_absIso[ipf]/t.isoTrack_pt[ipf] < 0.1) {

		// iso plots 
		if (decayMode == 1) {
		  fillHistosPF(h_1d_global, "", ipf, "_iso_tau1p");
		  if (neutralDaughters == 0) fillHistosPF(h_1d_global, "", ipf, "_iso_tau1p0n");
		  else if (neutralDaughters == 1) fillHistosPF(h_1d_global, "", ipf, "_iso_tau1p1n");
		  else if (neutralDaughters == 2) fillHistosPF(h_1d_global, "", ipf, "_iso_tau1p2n");
		  else if (neutralDaughters >= 3) fillHistosPF(h_1d_global, "", ipf, "_iso_tau1p3n");
		  if (neutralDaughters >= 1) fillHistosPF(h_1d_global, "", ipf, "_iso_tau1pgt1n");
		}
		else if (decayMode == 3) fillHistosPF(h_1d_global, "", ipf, "_iso_tau3p");
	      } // passes iso
	      
	      // take only first match
	      break;
	    } // loop over pf cands
	  } // if doPFCands

	} // loop on gen taus

      } // if doHadTau
      
    }//end loop on events in a file
  
    delete tree;
    f.Close();
  }//end loop on files
  
  cout << "[LepEffLooperMinimal::loop] processed " << nEventsTotal << " events" << endl;
  if ( nEventsChain != nEventsTotal ) {
    std::cout << "ERROR: number of events from files is not equal to total number of events" << std::endl;
  }

  cout << nDuplicates << " duplicate events were skipped." << endl;

  //---------------------
  // Make Efficiency Hists
  //---------------------

  if (doEMu) {
    // reco leptons

    makeEfficiencyHist(h_1d_global,"","h_reco_pt_iso_el","h_reco_pt_el");
    makeEfficiencyHist(h_1d_global,"","h_reco_eta_iso_el","h_reco_eta_el");
    makeEfficiencyHist(h_1d_global,"","h_reco_relIsoAn04_iso_el","h_reco_relIsoAn04_el");
    makeEfficiencyHist(h_1d_global,"","h_reco_nPFCHCand3_iso_el","h_reco_nPFCHCand3_el");
    makeEfficiencyHist2D(h_1d_global,"","h_reco_pt_relIsoAn04_iso_el","h_reco_pt_relIsoAn04_el");

    makeEfficiencyHist(h_1d_global,"","h_reco_pt_iso_mu","h_reco_pt_mu");
    makeEfficiencyHist(h_1d_global,"","h_reco_eta_iso_mu","h_reco_eta_mu");
    makeEfficiencyHist(h_1d_global,"","h_reco_relIsoAn04_iso_mu","h_reco_relIsoAn04_mu");
    makeEfficiencyHist(h_1d_global,"","h_reco_nPFCHCand3_iso_mu","h_reco_nPFCHCand3_mu");
    makeEfficiencyHist2D(h_1d_global,"","h_reco_pt_relIsoAn04_iso_mu","h_reco_pt_relIsoAn04_mu");

    if (applyEffMaps_zjets) {
      makeEfficiencyHist(h_1d_global,"","h_reco_nPFCHCand3_effmap1d_zjets_el","h_reco_nPFCHCand3_el", "h_effmap1d_zjets_reco_nPFCHCand3_el");
      makeEfficiencyHist(h_1d_global,"","h_reco_nPFCHCand3_effmap2d_zjets_el","h_reco_nPFCHCand3_el", "h_effmap2d_zjets_reco_nPFCHCand3_el");

      makeEfficiencyHist(h_1d_global,"","h_reco_nPFCHCand3_effmap1d_zjets_mu","h_reco_nPFCHCand3_mu", "h_effmap1d_zjets_reco_nPFCHCand3_mu");
      makeEfficiencyHist(h_1d_global,"","h_reco_nPFCHCand3_effmap2d_zjets_mu","h_reco_nPFCHCand3_mu", "h_effmap2d_zjets_reco_nPFCHCand3_mu");
    }

    if (applyEffMaps_ttbar) {
      makeEfficiencyHist(h_1d_global,"","h_reco_nPFCHCand3_effmap1d_ttbar_el","h_reco_nPFCHCand3_el", "h_effmap1d_ttbar_reco_nPFCHCand3_el");
      makeEfficiencyHist(h_1d_global,"","h_reco_nPFCHCand3_effmap2d_ttbar_el","h_reco_nPFCHCand3_el", "h_effmap2d_ttbar_reco_nPFCHCand3_el");

      makeEfficiencyHist(h_1d_global,"","h_reco_nPFCHCand3_effmap1d_ttbar_mu","h_reco_nPFCHCand3_mu", "h_effmap1d_ttbar_reco_nPFCHCand3_mu");
      makeEfficiencyHist(h_1d_global,"","h_reco_nPFCHCand3_effmap2d_ttbar_mu","h_reco_nPFCHCand3_mu", "h_effmap2d_ttbar_reco_nPFCHCand3_mu");
    }

    // pf cands
    if (doPFCands) {
      makeEfficiencyHist(h_1d_global,"","h_pf_pt_iso_el","h_pf_pt_el");
      makeEfficiencyHist(h_1d_global,"","h_pf_eta_iso_el","h_pf_eta_el");
      makeEfficiencyHist(h_1d_global,"","h_pf_relIsoAn04_iso_el","h_pf_relIsoAn04_el");
      makeEfficiencyHist(h_1d_global,"","h_pf_nPFCHCand3_iso_el","h_pf_nPFCHCand3_el");
      makeEfficiencyHist(h_1d_global,"","h_pf_mt2_iso_el","h_pf_mt2_el");
      makeEfficiencyHist(h_1d_global,"","h_pf_ht_iso_el","h_pf_ht_el");
      makeEfficiencyHist(h_1d_global,"","h_pf_nJet30_iso_el","h_pf_nJet30_el");
      makeEfficiencyHist2D(h_1d_global,"","h_pf_pt_relIsoAn04_iso_el","h_pf_pt_relIsoAn04_el");

      makeEfficiencyHist(h_1d_global,"","h_pf_pt_iso_mu","h_pf_pt_mu");
      makeEfficiencyHist(h_1d_global,"","h_pf_eta_iso_mu","h_pf_eta_mu");
      makeEfficiencyHist(h_1d_global,"","h_pf_relIsoAn04_iso_mu","h_pf_relIsoAn04_mu");
      makeEfficiencyHist(h_1d_global,"","h_pf_nPFCHCand3_iso_mu","h_pf_nPFCHCand3_mu");
      makeEfficiencyHist(h_1d_global,"","h_pf_mt2_iso_mu","h_pf_mt2_mu");
      makeEfficiencyHist(h_1d_global,"","h_pf_ht_iso_mu","h_pf_ht_mu");
      makeEfficiencyHist(h_1d_global,"","h_pf_nJet30_iso_mu","h_pf_nJet30_mu");
      makeEfficiencyHist2D(h_1d_global,"","h_pf_pt_relIsoAn04_iso_mu","h_pf_pt_relIsoAn04_mu");

      if (applyEffMaps_zjets) {
	makeEfficiencyHist(h_1d_global,"","h_pf_nPFCHCand3_effmap1d_zjets_el","h_pf_nPFCHCand3_el", "h_effmap1d_zjets_pf_nPFCHCand3_el");
	makeEfficiencyHist(h_1d_global,"","h_pf_nPFCHCand3_effmap2d_zjets_el","h_pf_nPFCHCand3_el", "h_effmap2d_zjets_pf_nPFCHCand3_el");

	makeEfficiencyHist(h_1d_global,"","h_pf_nPFCHCand3_effmap1d_zjets_mu","h_pf_nPFCHCand3_mu", "h_effmap1d_zjets_pf_nPFCHCand3_mu");
	makeEfficiencyHist(h_1d_global,"","h_pf_nPFCHCand3_effmap2d_zjets_mu","h_pf_nPFCHCand3_mu", "h_effmap2d_zjets_pf_nPFCHCand3_mu");
      }

      if (applyEffMaps_ttbar) {
	makeEfficiencyHist(h_1d_global,"","h_pf_nPFCHCand3_effmap1d_ttbar_el","h_pf_nPFCHCand3_el", "h_effmap1d_ttbar_pf_nPFCHCand3_el");
	makeEfficiencyHist(h_1d_global,"","h_pf_nPFCHCand3_effmap2d_ttbar_el","h_pf_nPFCHCand3_el", "h_effmap2d_ttbar_pf_nPFCHCand3_el");

	makeEfficiencyHist(h_1d_global,"","h_pf_nPFCHCand3_effmap1d_ttbar_mu","h_pf_nPFCHCand3_mu", "h_effmap1d_ttbar_pf_nPFCHCand3_mu");
	makeEfficiencyHist(h_1d_global,"","h_pf_nPFCHCand3_effmap2d_ttbar_mu","h_pf_nPFCHCand3_mu", "h_effmap2d_ttbar_pf_nPFCHCand3_mu");
      }

    } // if doPFCands
  } // if doEMu

  if (doHadTau && doPFCands) {

    makeEfficiencyHist(h_1d_global,"","h_gen_leadTrackPt_match_tau1p","h_gen_leadTrackPt_tau1p");
    makeEfficiencyHist(h_1d_global,"","h_gen_tauEta_match_tau1p","h_gen_tauEta_tau1p");

    makeEfficiencyHist(h_1d_global,"","h_pf_pt_iso_tau1p","h_pf_pt_tau1p");
    makeEfficiencyHist(h_1d_global,"","h_pf_eta_iso_tau1p","h_pf_eta_tau1p");
    makeEfficiencyHist(h_1d_global,"","h_pf_relIsoAn04_iso_tau1p","h_pf_relIsoAn04_tau1p");
    makeEfficiencyHist(h_1d_global,"","h_pf_nPFCHCand3_iso_tau1p","h_pf_nPFCHCand3_tau1p");
    makeEfficiencyHist(h_1d_global,"","h_pf_mt2_iso_tau1p","h_pf_mt2_tau1p");
    makeEfficiencyHist(h_1d_global,"","h_pf_ht_iso_tau1p","h_pf_ht_tau1p");
    makeEfficiencyHist(h_1d_global,"","h_pf_nJet30_iso_tau1p","h_pf_nJet30_tau1p");
    makeEfficiencyHist2D(h_1d_global,"","h_pf_pt_relIsoAn04_iso_tau1p","h_pf_pt_relIsoAn04_tau1p");

    makeEfficiencyHist(h_1d_global,"","h_pf_pt_iso_tau1p0n","h_pf_pt_tau1p0n");
    makeEfficiencyHist(h_1d_global,"","h_pf_eta_iso_tau1p0n","h_pf_eta_tau1p0n");
    makeEfficiencyHist(h_1d_global,"","h_pf_relIsoAn04_iso_tau1p0n","h_pf_relIsoAn04_tau1p0n");
    makeEfficiencyHist(h_1d_global,"","h_pf_nPFCHCand3_iso_tau1p0n","h_pf_nPFCHCand3_tau1p0n");
    makeEfficiencyHist(h_1d_global,"","h_pf_mt2_iso_tau1p0n","h_pf_mt2_tau1p0n");
    makeEfficiencyHist(h_1d_global,"","h_pf_ht_iso_tau1p0n","h_pf_ht_tau1p0n");
    makeEfficiencyHist(h_1d_global,"","h_pf_nJet30_iso_tau1p0n","h_pf_nJet30_tau1p0n");
    makeEfficiencyHist2D(h_1d_global,"","h_pf_pt_relIsoAn04_iso_tau1p0n","h_pf_pt_relIsoAn04_tau1p0n");

    makeEfficiencyHist(h_1d_global,"","h_pf_pt_iso_tau1p1n","h_pf_pt_tau1p1n");
    makeEfficiencyHist(h_1d_global,"","h_pf_eta_iso_tau1p1n","h_pf_eta_tau1p1n");
    makeEfficiencyHist(h_1d_global,"","h_pf_relIsoAn04_iso_tau1p1n","h_pf_relIsoAn04_tau1p1n");
    makeEfficiencyHist(h_1d_global,"","h_pf_nPFCHCand3_iso_tau1p1n","h_pf_nPFCHCand3_tau1p1n");
    makeEfficiencyHist(h_1d_global,"","h_pf_mt2_iso_tau1p1n","h_pf_mt2_tau1p1n");
    makeEfficiencyHist(h_1d_global,"","h_pf_ht_iso_tau1p1n","h_pf_ht_tau1p1n");
    makeEfficiencyHist(h_1d_global,"","h_pf_nJet30_iso_tau1p1n","h_pf_nJet30_tau1p1n");
    makeEfficiencyHist2D(h_1d_global,"","h_pf_pt_relIsoAn04_iso_tau1p1n","h_pf_pt_relIsoAn04_tau1p1n");

    makeEfficiencyHist(h_1d_global,"","h_pf_pt_iso_tau1p2n","h_pf_pt_tau1p2n");
    makeEfficiencyHist(h_1d_global,"","h_pf_eta_iso_tau1p2n","h_pf_eta_tau1p2n");
    makeEfficiencyHist(h_1d_global,"","h_pf_relIsoAn04_iso_tau1p2n","h_pf_relIsoAn04_tau1p2n");
    makeEfficiencyHist(h_1d_global,"","h_pf_nPFCHCand3_iso_tau1p2n","h_pf_nPFCHCand3_tau1p2n");
    makeEfficiencyHist(h_1d_global,"","h_pf_mt2_iso_tau1p2n","h_pf_mt2_tau1p2n");
    makeEfficiencyHist(h_1d_global,"","h_pf_ht_iso_tau1p2n","h_pf_ht_tau1p2n");
    makeEfficiencyHist(h_1d_global,"","h_pf_nJet30_iso_tau1p2n","h_pf_nJet30_tau1p2n");
    makeEfficiencyHist2D(h_1d_global,"","h_pf_pt_relIsoAn04_iso_tau1p2n","h_pf_pt_relIsoAn04_tau1p2n");

    makeEfficiencyHist(h_1d_global,"","h_pf_pt_iso_tau1p3n","h_pf_pt_tau1p3n");
    makeEfficiencyHist(h_1d_global,"","h_pf_eta_iso_tau1p3n","h_pf_eta_tau1p3n");
    makeEfficiencyHist(h_1d_global,"","h_pf_relIsoAn04_iso_tau1p3n","h_pf_relIsoAn04_tau1p3n");
    makeEfficiencyHist(h_1d_global,"","h_pf_nPFCHCand3_iso_tau1p3n","h_pf_nPFCHCand3_tau1p3n");
    makeEfficiencyHist(h_1d_global,"","h_pf_mt2_iso_tau1p3n","h_pf_mt2_tau1p3n");
    makeEfficiencyHist(h_1d_global,"","h_pf_ht_iso_tau1p3n","h_pf_ht_tau1p3n");
    makeEfficiencyHist(h_1d_global,"","h_pf_nJet30_iso_tau1p3n","h_pf_nJet30_tau1p3n");
    makeEfficiencyHist2D(h_1d_global,"","h_pf_pt_relIsoAn04_iso_tau1p3n","h_pf_pt_relIsoAn04_tau1p3n");

    makeEfficiencyHist(h_1d_global,"","h_pf_pt_iso_tau3p","h_pf_pt_tau3p");
    makeEfficiencyHist(h_1d_global,"","h_pf_eta_iso_tau3p","h_pf_eta_tau3p");
    makeEfficiencyHist(h_1d_global,"","h_pf_relIsoAn04_iso_tau3p","h_pf_relIsoAn04_tau3p");
    makeEfficiencyHist(h_1d_global,"","h_pf_nPFCHCand3_iso_tau3p","h_pf_nPFCHCand3_tau3p");
    makeEfficiencyHist(h_1d_global,"","h_pf_mt2_iso_tau3p","h_pf_mt2_tau3p");
    makeEfficiencyHist(h_1d_global,"","h_pf_ht_iso_tau3p","h_pf_ht_tau3p");
    makeEfficiencyHist(h_1d_global,"","h_pf_nJet30_iso_tau3p","h_pf_nJet30_tau3p");
    makeEfficiencyHist2D(h_1d_global,"","h_pf_pt_relIsoAn04_iso_tau3p","h_pf_pt_relIsoAn04_tau3p");

    makeEfficiencyHist(h_1d_global,"","h_pf_pt_iso_tau1pgt1n","h_pf_pt_tau1pgt1n");
    makeEfficiencyHist(h_1d_global,"","h_pf_eta_iso_tau1pgt1n","h_pf_eta_tau1pgt1n");
    makeEfficiencyHist(h_1d_global,"","h_pf_relIsoAn04_iso_tau1pgt1n","h_pf_relIsoAn04_tau1pgt1n");
    makeEfficiencyHist(h_1d_global,"","h_pf_nPFCHCand3_iso_tau1pgt1n","h_pf_nPFCHCand3_tau1pgt1n");
    makeEfficiencyHist(h_1d_global,"","h_pf_mt2_iso_tau1pgt1n","h_pf_mt2_tau1pgt1n");
    makeEfficiencyHist(h_1d_global,"","h_pf_ht_iso_tau1pgt1n","h_pf_ht_tau1pgt1n");
    makeEfficiencyHist(h_1d_global,"","h_pf_nJet30_iso_tau1pgt1n","h_pf_nJet30_tau1pgt1n");
    makeEfficiencyHist2D(h_1d_global,"","h_pf_pt_relIsoAn04_iso_tau1pgt1n","h_pf_pt_relIsoAn04_tau1pgt1n");

  }

  //---------------------
  // Save Plots
  //---------------------

  outfile_->cd();
  savePlotsDir(h_1d_global,outfile_,"");

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

//____________________________________________________
void LepEffLooperMinimal::fillHistosReco(std::map<std::string, TH1*>& h_1d, const std::string& dirname, const unsigned int& ilep, const std::string& s) {

  if (dirname.size()) {
    TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
    if (dir == 0) {
      dir = outfile_->mkdir(dirname.c_str());
    } 
    dir->cd();
  } else {
    outfile_->cd();
  }

  plot1D("h_reco_pt"+s,       t.lep_pt[ilep],   evtweight_, h_1d, ";p_{T}(reco lep) [GeV]", 1000, 0., 1000.);
  plot1D("h_reco_eta"+s,      t.lep_eta[ilep],  evtweight_, h_1d, ";#eta(reco lep)", 60, -3.0, 3.0);
  plot1D("h_reco_relIsoAn04"+s,      t.lep_relIsoAn04[ilep],  evtweight_, h_1d, ";Rel PFIso dR Mini to 0.4", 250, 0., 5.);
  plot1D("h_reco_nPFCHCand3"+s,      t.nPFCHCand3,  evtweight_, h_1d, ";N(tracks, pt > 3)", 10, 0., 100.);

  plot2D("h_reco_pt_relIsoAn04"+s,      t.lep_pt[ilep], t.lep_relIsoAn04[ilep],  evtweight_, h_1d, ";p_{T}(reco lep) [GeV];Activity / p_{T}", n_ptbins_coarse, ptbins_coarse_float, n_isobins, isobins_float);
  //plot2D("h_reco_miniRelIso_relIsoAn04"+s,      t.lep_miniRelIso[ilep], t.lep_relIsoAn04[ilep],  evtweight_, h_1d, ";Mini Iso / p_{T};Activity / p_{T} ", 200, 0, 2., 200, 0, 2);

  if (applyEffMaps_zjets) {
    float effweight1D = 1.;
    float effweight2D = 1.;
    float pt = std::min(t.lep_pt[ilep],float(199.));
    float an04 = std::min(t.lep_relIsoAn04[ilep],float(4.99));
    if (abs(t.lep_pdgId[ilep]) == 11) {
      effweight1D = h_eff_el_reco_pt_zjets_->GetBinContent(h_eff_el_reco_pt_zjets_->FindBin(pt));
      effweight2D = h_eff_el_reco_pt_activity_zjets_->GetBinContent(h_eff_el_reco_pt_activity_zjets_->FindBin(pt,an04));
    }
    else if (abs(t.lep_pdgId[ilep]) == 13) {
      effweight1D = h_eff_mu_reco_pt_zjets_->GetBinContent(h_eff_mu_reco_pt_zjets_->FindBin(pt));
      effweight2D = h_eff_mu_reco_pt_activity_zjets_->GetBinContent(h_eff_mu_reco_pt_activity_zjets_->FindBin(pt,an04));
    }
    plot1D("h_reco_nPFCHCand3_effmap1d_zjets"+s,      t.nPFCHCand3,  evtweight_*effweight1D, h_1d, ";N(tracks, pt > 3)", 10, 0., 100.);
    plot1D("h_reco_nPFCHCand3_effmap2d_zjets"+s,      t.nPFCHCand3,  evtweight_*effweight2D, h_1d, ";N(tracks, pt > 3)", 10, 0., 100.);
  }
  
  if (applyEffMaps_ttbar) {
    float effweight1D = 1.;
    float effweight2D = 1.;
    float pt = std::min(t.lep_pt[ilep],float(199.));
    float an04 = std::min(t.lep_relIsoAn04[ilep],float(4.99));
    if (abs(t.lep_pdgId[ilep]) == 11) {
      effweight1D = h_eff_el_reco_pt_ttbar_->GetBinContent(h_eff_el_reco_pt_ttbar_->FindBin(pt));
      effweight2D = h_eff_el_reco_pt_activity_ttbar_->GetBinContent(h_eff_el_reco_pt_activity_ttbar_->FindBin(pt,an04));
    }
    else if (abs(t.lep_pdgId[ilep]) == 13) {
      effweight1D = h_eff_mu_reco_pt_ttbar_->GetBinContent(h_eff_mu_reco_pt_ttbar_->FindBin(pt));
      effweight2D = h_eff_mu_reco_pt_activity_ttbar_->GetBinContent(h_eff_mu_reco_pt_activity_ttbar_->FindBin(pt,an04));
    }
    plot1D("h_reco_nPFCHCand3_effmap1d_ttbar"+s,      t.nPFCHCand3,  evtweight_*effweight1D, h_1d, ";N(tracks, pt > 3)", 10, 0., 100.);
    plot1D("h_reco_nPFCHCand3_effmap2d_ttbar"+s,      t.nPFCHCand3,  evtweight_*effweight2D, h_1d, ";N(tracks, pt > 3)", 10, 0., 100.);
    plot2D("h_reco_nPFCHCand3_effweight_effmap2d_ttbar"+s,      t.nPFCHCand3, effweight2D,  evtweight_, h_1d, ";N(tracks, pt > 3);Eff. Weight ", 10, 0, 100., 50, 0, 1);
  }
  
  outfile_->cd();
  return;
}

//____________________________________________________
void LepEffLooperMinimal::fillHistosPF(std::map<std::string, TH1*>& h_1d, const std::string& dirname, const unsigned int& ipf, const std::string& s) {

  if (dirname.size()) {
    TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
    if (dir == 0) {
      dir = outfile_->mkdir(dirname.c_str());
    } 
    dir->cd();
  } else {
    outfile_->cd();
  }

  plot1D("h_pf_pt"+s,       t.isoTrack_pt[ipf],   evtweight_, h_1d, ";p_{T}(PF cand) [GeV]", 1000, 0., 1000.);
  plot1D("h_pf_eta"+s,      t.isoTrack_eta[ipf],  evtweight_, h_1d, ";#eta(pf lep)", 60, -3.0, 3.0);
  plot1D("h_pf_absIso"+s,      t.isoTrack_absIso[ipf],  evtweight_, h_1d, ";Track Iso [GeV]", 200, 0., 10.);
  plot1D("h_pf_relIsoAn04"+s,      t.isoTrack_relIsoAn04[ipf],  evtweight_, h_1d, ";Rel PFIso dR 0.3 to 0.4", 250, 0., 5.);
  plot1D("h_pf_nPFCHCand3"+s,      t.nPFCHCand3,  evtweight_, h_1d, ";N(tracks, pt > 3)", 10, 0., 100.);
  plot1D("h_pf_mt2"+s,             t.mt2,  evtweight_, h_1d, ";M_{T2} [GeV]", 100, 0., 500.);
  plot1D("h_pf_ht"+s,              t.ht,  evtweight_, h_1d, ";H_{T} [GeV]", 150, 0., 1500.);
  plot1D("h_pf_nJet30"+s,          t.nJet30,  evtweight_, h_1d, ";N(jets)", 10, 0., 10.);

  plot2D("h_pf_pt_relIsoAn04"+s,      t.isoTrack_pt[ipf], t.isoTrack_relIsoAn04[ipf],  evtweight_, h_1d, ";p_{T}(pf lep) [GeV];Acivity / p_{T}", n_ptbins_coarse_pf, ptbins_coarse_float_pf, n_isobins, isobins_float);

  if (applyEffMaps_zjets) {
    float effweight1D = 1.;
    float effweight2D = 1.;
    float pt = std::min(t.isoTrack_pt[ipf],float(199.));
    float an04 = std::min(t.isoTrack_relIsoAn04[ipf],float(4.99));
    if (abs(t.isoTrack_pdgId[ipf]) == 11) {
      effweight1D = h_eff_el_pf_pt_zjets_->GetBinContent(h_eff_el_pf_pt_zjets_->FindBin(pt));
      effweight2D = h_eff_el_pf_pt_activity_zjets_->GetBinContent(h_eff_el_pf_pt_activity_zjets_->FindBin(pt,an04));
    }
    else if (abs(t.isoTrack_pdgId[ipf]) == 13) {
      effweight1D = h_eff_mu_pf_pt_zjets_->GetBinContent(h_eff_mu_pf_pt_zjets_->FindBin(pt));
      effweight2D = h_eff_mu_pf_pt_activity_zjets_->GetBinContent(h_eff_mu_pf_pt_activity_zjets_->FindBin(pt,an04));
    }
    plot1D("h_pf_nPFCHCand3_effmap1d_zjets"+s,      t.nPFCHCand3,  evtweight_*effweight1D, h_1d, ";N(tracks, pt > 3)", 10, 0., 100.);
    plot1D("h_pf_nPFCHCand3_effmap2d_zjets"+s,      t.nPFCHCand3,  evtweight_*effweight2D, h_1d, ";N(tracks, pt > 3)", 10, 0., 100.);
  }
  
  if (applyEffMaps_ttbar) {
    float effweight1D = 1.;
    float effweight2D = 1.;
    float pt = std::min(t.isoTrack_pt[ipf],float(199.));
    float an04 = std::min(t.isoTrack_relIsoAn04[ipf],float(4.99));
    if (abs(t.isoTrack_pdgId[ipf]) == 11) {
      effweight1D = h_eff_el_pf_pt_ttbar_->GetBinContent(h_eff_el_pf_pt_ttbar_->FindBin(pt));
      effweight2D = h_eff_el_pf_pt_activity_ttbar_->GetBinContent(h_eff_el_pf_pt_activity_ttbar_->FindBin(pt,an04));
    }
    else if (abs(t.isoTrack_pdgId[ipf]) == 13) {
      effweight1D = h_eff_mu_pf_pt_ttbar_->GetBinContent(h_eff_mu_pf_pt_ttbar_->FindBin(pt));
      effweight2D = h_eff_mu_pf_pt_activity_ttbar_->GetBinContent(h_eff_mu_pf_pt_activity_ttbar_->FindBin(pt,an04));
    }
    plot1D("h_pf_nPFCHCand3_effmap1d_ttbar"+s,      t.nPFCHCand3,  evtweight_*effweight1D, h_1d, ";N(tracks, pt > 3)", 10, 0., 100.);
    plot1D("h_pf_nPFCHCand3_effmap2d_ttbar"+s,      t.nPFCHCand3,  evtweight_*effweight2D, h_1d, ";N(tracks, pt > 3)", 10, 0., 100.);
    plot2D("h_pf_nPFCHCand3_effweight_effmap2d_ttbar"+s,      t.nPFCHCand3, effweight2D,  evtweight_, h_1d, ";N(tracks, pt > 3);Eff. Weight ", 10, 0, 100., 50, 0, 1);
  }

  outfile_->cd();
  return;
}

//____________________________________________________
void LepEffLooperMinimal::makeEfficiencyHist(std::map<std::string, TH1*>& h_1d, const std::string& dirname, const std::string& num_name, const std::string& denom_name, const std::string& eff_name) {

  TH1D* num = getHist1D(num_name,h_1d,1,0,1);
  TH1D* denom = getHist1D(denom_name,h_1d,1,0,1);

  return makeEfficiencyHist(h_1d,dirname,num,denom,eff_name);

}

//____________________________________________________
void LepEffLooperMinimal::makeEfficiencyHist(std::map<std::string, TH1*>& h_1d, const std::string& dirname, TH1D* num, TH1D* denom, const std::string& eff_name) {

  // protect against empty hists
  if (num->GetEntries() == 0 || denom->GetEntries() == 0) return;

  if (dirname.size()) {
    TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
    if (dir == 0) {
      dir = outfile_->mkdir(dirname.c_str());
    } 
    dir->cd();
  } else {
    outfile_->cd();
  }

  TString eff_tname(eff_name.c_str());
  // no name given - make automatic name
  if (eff_tname.Length() == 0) {
    eff_tname = num->GetName();
    eff_tname.Remove(0,2);
    eff_tname = TString("h_eff_") + eff_tname;
  }
  // pt or dR hists - rebin
  TH1D* eff = 0;
  if (eff_tname.Contains("_pt_")) {
    eff = (TH1D*) num->Rebin(n_ptbins,eff_tname.Data(),ptbins);
  } else if (eff_tname.Contains("_relIso")) {
    eff = (TH1D*) num->Rebin(n_isobins,eff_tname.Data(),isobins);
  } else {
    eff = (TH1D*) num->Clone(eff_tname.Data());
  }

  TString denom_name = TString(denom->GetName()) + "_clone";
  TH1D* denom_clone = 0;
  if (eff_tname.Contains("_pt_")) {
    denom_clone = (TH1D*) denom->Rebin(n_ptbins,denom_name.Data(),ptbins);
  } else if (eff_tname.Contains("_relIso")) {
    denom_clone = (TH1D*) denom->Rebin(n_isobins,denom_name.Data(),isobins);
  } else {
    denom_clone = (TH1D*) denom->Clone(denom_name.Data());
  }

  if (scaleErrorsForLumi) {
    for (int ibinx=1; ibinx <= eff->GetNbinsX(); ++ibinx) {
      float num = eff->GetBinContent(ibinx);
      float denom = denom_clone->GetBinContent(ibinx);
      float effval(0.);
      float errval(0.);
      if (denom > 0.) {
	effval = num/denom;
	errval = err_eff(effval,denom);
      }
      eff->SetBinContent(ibinx,effval);
      eff->SetBinError(ibinx,errval);
    }
  }
  // otherwise use normal binomial errors
  else {
    eff->Divide(eff,denom_clone,1.0,1.0,"B");
  }

  insertHist1D(eff,h_1d);

  delete denom_clone;

  outfile_->cd();
  return;
}

//____________________________________________________
void LepEffLooperMinimal::makeEfficiencyHist2D(std::map<std::string, TH1*>& h_1d, const std::string& dirname, const std::string& num_name, const std::string& denom_name, const std::string& eff_name) {

  TH2D* num = getHist2D(num_name,h_1d,1,0,1,1,0,1);
  TH2D* denom = getHist2D(denom_name,h_1d,1,0,1,1,0,1);

  return makeEfficiencyHist2D(h_1d,dirname,num,denom,eff_name);

}

//____________________________________________________
void LepEffLooperMinimal::makeEfficiencyHist2D(std::map<std::string, TH1*>& h_1d, const std::string& dirname, TH2D* num, TH2D* denom, const std::string& eff_name) {

  // protect against empty hists
  if (num->GetEntries() == 0 || denom->GetEntries() == 0) return;

  if (dirname.size()) {
    TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
    if (dir == 0) {
      dir = outfile_->mkdir(dirname.c_str());
    } 
    dir->cd();
  } else {
    outfile_->cd();
  }

  TString eff_tname(eff_name.c_str());
  // no name given - make automatic name
  if (eff_tname.Length() == 0) {
    eff_tname = num->GetName();
    eff_tname.Remove(0,2);
    eff_tname = TString("h_eff_") + eff_tname;
  }

  TH2D* eff = (TH2D*) num->Clone(eff_tname.Data());

  TString denom_name = TString(denom->GetName()) + "_clone";
  TH2D* denom_clone = (TH2D*) denom->Clone(denom_name.Data());

  if (scaleErrorsForLumi) {
    for (int ibinx=1; ibinx <= eff->GetNbinsX(); ++ibinx) {
      for (int ibiny=1; ibiny <= eff->GetNbinsY(); ++ibiny) {
	float num = eff->GetBinContent(ibinx,ibiny);
	float denom = denom_clone->GetBinContent(ibinx,ibiny);
	float effval(0.);
	float errval(0.);
	if (denom > 0.) {
	  effval = num/denom;
	  errval = err_eff(effval,denom);
	}
	eff->SetBinContent(ibinx,ibiny,effval);
	eff->SetBinError(ibinx,ibiny,errval);
      }
    }
  }
  // otherwise use normal binomial errors
  else {
    eff->Divide(eff,denom_clone,1.0,1.0,"B");
  }

  insertHist2D(eff,h_1d);

  delete denom_clone;

  outfile_->cd();
  return;
}

//______________________________________________________________________________
// returns the error on eff = num/denom: sqrt((eff * (1-eff)) / denom)
float LepEffLooperMinimal::err_eff(float eff,  float denom) {
  return sqrt((eff * (1-eff)) / denom);
}


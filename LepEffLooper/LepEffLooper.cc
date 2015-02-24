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
#include "LepEffLooper.h"

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

// pt binning for hists
const int n_ptbins = 62;
const double ptbins[n_ptbins+1] = {0, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
				  22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 46, 48, 50,
				  55, 60, 65, 70, 75, 80, 85, 90, 95, 100,
				  110, 120, 130, 140, 150, 160, 170, 180, 190, 200,
				  250, 300, 350, 400, 450, 500,
				  600, 700, 800, 900, 1000};

LepEffLooper::LepEffLooper(){
}
LepEffLooper::~LepEffLooper(){

};

void LepEffLooper::SetSignalRegions(){

  // SR base, NO LEPTON VETO
  SRBase.SetName("srbase");
   SRBase.SetVar("mt2", 200, -1);
  SRBase.SetVar("j1pt", 100, -1);
  SRBase.SetVar("j2pt", 100, -1);
  SRBase.SetVar("deltaPhiMin", 0.3, -1);
  SRBase.SetVar("diffMetMhtOverMet", 0, 0.5);
   SRBase.SetVar("passesHtMet", 1, 2);
  // SRBase.SetVar("nJet40", 2, -1);
  // SRBase.SetVar("ht", 450, -1);

}


void LepEffLooper::loop(TChain* chain, std::string output_name){

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  gROOT->cd();
  cout << "[LepEffLooper::loop] creating output file: " << output_name << endl;

  outfile_ = new TFile(output_name.c_str(),"RECREATE") ; 

  cout << "[LepEffLooper::loop] setting up histos" << endl;

  SetSignalRegions();

  // File Loop
  int nDuplicates = 0;
  int nEvents = chain->GetEntries();
  unsigned int nEventsChain = nEvents;
  cout << "[LepEffLooper::loop] running on " << nEventsChain << " events" << endl;
  unsigned int nEventsTotal = 0;
  TObjArray *listOfFiles = chain->GetListOfFiles();
  TIter fileIter(listOfFiles);
  TFile *currentFile = 0;
  while ( (currentFile = (TFile*)fileIter.Next()) ) {
    cout << "[LepEffLooper::loop] running on file: " << currentFile->GetTitle() << endl;

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

      // check SR base cuts (without lepton veto)
      std::map<std::string, float> values;
      values["deltaPhiMin"] = t.deltaPhiMin;
      values["diffMetMhtOverMet"]  = t.diffMetMht/t.met_pt;
      values["j1pt"]        = t.jet_pt[0];
      values["j2pt"]        = t.jet_pt[1];
      values["mt2"]         = t.mt2;
      values["passesHtMet"] = ( (t.ht > 450. && t.met_pt > 200.) || (t.ht > 1000. && t.met_pt > 30.) );
      // values["ht"]         = t.ht;
      // values["nJet40"]         = t.nJet40;

      if(!SRBase.PassesSelection(values)) continue;

      // keep track of which reco leptons are matched to gen
      vector<int> lep_idx_genmatch;
      vector<int> pflep_idx_genmatch;
      vector<int> pfhad_idx_genmatch;

      // -------------------------------------------------
      //           loop over gen leptons
      // -------------------------------------------------

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

	// compute dR to nearest reco jet
	float minDR = 99.;
	for (int ijet = 0; ijet < t.njet; ++ijet) {
	  float dr = DeltaR(genlep.eta,t.jet_eta[ijet],genlep.phi,t.jet_phi[ijet]);
	  if (dr < minDR) minDR = dr;
	}

	genlep.minDRrecojet = minDR;

	// gen level plots
	fillHistosGen(h_1d_global,"",genlep,"");
	// by lepton flavor
	if (abs(genlep.pdgId) == 11) fillHistosGen(h_1d_global,"",genlep,"_el");
	else if (abs(genlep.pdgId) == 13) fillHistosGen(h_1d_global,"",genlep,"_mu");

	// -- loop over reco leptons
	//    -- plot gen, reco quantities for passing objects
	//    -- could also do reco efficiencies for additional cuts/requirements
	bool recomatch = false;
	bool recoisomatch = false;
	for (int ilep=0; ilep < t.nlep; ++ilep) {
	  // check flavor first
	  if (abs(genlep.pdgId) != abs(t.lep_pdgId[ilep])) continue;
	  // check dR match
	  float dr = DeltaR(genlep.eta,t.lep_eta[ilep],genlep.phi,t.lep_phi[ilep]);
	  plot1D("h_genreco_dR",      dr,       evtweight_, h_1d_global, ";#Delta R(gen lep,reco lep)", 600, 0, 6);
	  if (dr > 0.1) continue;

	  // note genmatch
	  lep_idx_genmatch.push_back(ilep);

	  // only fill hists once, in case there are multiple reco matches
	  if (!recomatch) {
	    // match: plot gen quantities again to get eff
	    fillHistosGen(h_1d_global,"",genlep,"_recomatch");
	    plot2D("h_met_genmet_recomatch", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	    plot2D("h_mt2_genpt_recomatch", genlep.pt, t.mt2, evtweight_, h_2d_global, ";pt(gen lep) [GeV]; MT2 [GeV]", 50, 0., 1000., 50., 0., 1000.);
	    if (genlep.pt > 200.) plot2D("h_met_genmet_recomatch_pt200", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	    // by lepton flavor
	    if (abs(genlep.pdgId) == 11) fillHistosGen(h_1d_global,"",genlep,"_recomatch_el");
	    else if (abs(genlep.pdgId) == 13) fillHistosGen(h_1d_global,"",genlep,"_recomatch_mu");
	    recomatch = true;
	  }

	  if (t.lep_relIso03[ilep] > 0.15) continue;

	  // only fill hists once, in case there are multiple reco matches
	  if (!recoisomatch) {
	    // match: plot gen quantities again to get eff
	    fillHistosGen(h_1d_global,"",genlep,"_recoisomatch");
	    plot2D("h_met_genmet_recoisomatch", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	    plot2D("h_mt2_genpt_recoisomatch", genlep.pt, t.mt2, evtweight_, h_2d_global, ";pt(gen lep) [GeV]; MT2 [GeV]", 50, 0., 1000., 50., 0., 1000.);
	    if (genlep.pt > 200.) plot2D("h_met_genmet_recoisomatch_pt200", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	    // by lepton flavor
	    if (abs(genlep.pdgId) == 11) fillHistosGen(h_1d_global,"",genlep,"_recoisomatch_el");
	    else if (abs(genlep.pdgId) == 13) fillHistosGen(h_1d_global,"",genlep,"_recoisomatch_mu");
	    recoisomatch = true;
	  }

	} // loop over reco leps

	// plots for gen leptons with no reco match
	if (!recomatch) {
	  // no match: plot gen quantities again
	  fillHistosGen(h_1d_global,"",genlep,"_NOrecomatch");
	  plot2D("h_met_genmet_NOrecomatch", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	  plot2D("h_mt2_genpt_NOrecomatch", genlep.pt, t.mt2, evtweight_, h_2d_global, ";pt(gen lep) [GeV]; MT2 [GeV]", 50, 0., 1000., 50., 0., 1000.);
	  // by lepton flavor
	  if (abs(genlep.pdgId) == 11) fillHistosGen(h_1d_global,"",genlep,"_NOrecomatch_el");
	  else if (abs(genlep.pdgId) == 13) fillHistosGen(h_1d_global,"",genlep,"_NOrecomatch_mu");
	  if (genlep.pt > 200.) {
	    fillHistosGen(h_1d_global,"",genlep,"_NOrecomatch_pt200");
	    plot2D("h_met_genmet_NOrecomatch_pt200", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	    if (abs(genlep.pdgId) == 11) fillHistosGen(h_1d_global,"",genlep,"_NOrecomatch_pt200_el");
	    else if (abs(genlep.pdgId) == 13) fillHistosGen(h_1d_global,"",genlep,"_NOrecomatch_pt200_mu");
	  }
	}

	// plots for gen leptons with reco match but reco fails iso
	if (recomatch && !recoisomatch) {
	  // no match: plot gen quantities again
	  fillHistosGen(h_1d_global,"",genlep,"_recoNOiso");
	  plot2D("h_met_genmet_recoNOiso", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	  plot2D("h_mt2_genpt_recoNOiso", genlep.pt, t.mt2, evtweight_, h_2d_global, ";pt(gen lep) [GeV]; MT2 [GeV]", 50, 0., 1000., 50., 0., 1000.);
	  // by lepton flavor
	  if (abs(genlep.pdgId) == 11) fillHistosGen(h_1d_global,"",genlep,"_recoNOiso_el");
	  else if (abs(genlep.pdgId) == 13) fillHistosGen(h_1d_global,"",genlep,"_recoNOiso_mu");
	  if (genlep.pt > 200.) {
	    fillHistosGen(h_1d_global,"",genlep,"_recoNOiso_pt200");
	    plot2D("h_met_genmet_recoNOiso_pt200", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	    if (abs(genlep.pdgId) == 11) fillHistosGen(h_1d_global,"",genlep,"_recoNOiso_pt200_el");
	    else if (abs(genlep.pdgId) == 13) fillHistosGen(h_1d_global,"",genlep,"_recoNOiso_pt200_mu");
	  }
	}

	// -- loop over PF cands
	//    -- plot gen, reco quantities for passing objects
	bool pfmatch = false;
	bool pfisomatch = false;
	for (int ipf=0; ipf < t.nisoTrack; ++ipf) {
	  // check flavor first
	  if (abs(genlep.pdgId) != abs(t.isoTrack_pdgId[ipf])) continue;
	  // check dR match
	  float dr = DeltaR(genlep.eta,t.isoTrack_eta[ipf],genlep.phi,t.isoTrack_phi[ipf]);
	  plot1D("h_genpf_dR",      dr,       evtweight_, h_1d_global, ";#Delta R(gen lep,PF lep)", 600, 0, 6);
	  if (dr > 0.1) continue;

	  // note genmatch
	  pflep_idx_genmatch.push_back(ipf);

	  // only fill hists once, in case there are multiple reco matches
	  if (!pfmatch) {
	    // match: plot gen quantities again to get eff
	    fillHistosGen(h_1d_global,"",genlep,"_pfmatch");
	    plot2D("h_met_genmet_pfmatch", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	    if (genlep.pt > 200.) plot2D("h_met_genmet_pfmatch_pt200", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	    // by lepton flavor
	    if (abs(genlep.pdgId) == 11) fillHistosGen(h_1d_global,"",genlep,"_pfmatch_el");
	    else if (abs(genlep.pdgId) == 13) fillHistosGen(h_1d_global,"",genlep,"_pfmatch_mu");
	    pfmatch = true;
	  }

	  if (t.isoTrack_absIso[ipf]/t.isoTrack_pt[ipf] > 0.2) continue;

	  // only fill hists once, in case there are multiple reco matches
	  if (!pfisomatch) {
	    // match: plot gen quantities again to get eff
	    fillHistosGen(h_1d_global,"",genlep,"_pfisomatch");
	    plot2D("h_met_genmet_pfisomatch", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	    if (genlep.pt > 200.) plot2D("h_met_genmet_pfisomatch_pt200", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	    // by lepton flavor
	    if (abs(genlep.pdgId) == 11) fillHistosGen(h_1d_global,"",genlep,"_pfisomatch_el");
	    else if (abs(genlep.pdgId) == 13) fillHistosGen(h_1d_global,"",genlep,"_pfisomatch_mu");
	    pfisomatch = true;
	  }

	} // loop over pf leps

	// plots for gen leptons with no pf match
	if (!pfmatch) {
	  // no match: plot gen quantities again
	  fillHistosGen(h_1d_global,"",genlep,"_NOpfmatch");
	  plot2D("h_met_genmet_NOpfmatch", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	  if (genlep.pt > 200.) plot2D("h_met_genmet_NOpfmatch_pt200", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	  // by lepton flavor
	  if (abs(genlep.pdgId) == 11) fillHistosGen(h_1d_global,"",genlep,"_NOpfmatch_el");
	  else if (abs(genlep.pdgId) == 13) fillHistosGen(h_1d_global,"",genlep,"_NOpfmatch_mu");
	}

	// plots for gen leptons with pf match but failing iso
	if (pfmatch && !pfisomatch) {
	  // no match: plot gen quantities again
	  fillHistosGen(h_1d_global,"",genlep,"_pfNOiso");
	  plot2D("h_met_genmet_pfNOiso", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	  if (genlep.pt > 200.) plot2D("h_met_genmet_pfNOiso_pt200", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	  // by lepton flavor
	  if (abs(genlep.pdgId) == 11) fillHistosGen(h_1d_global,"",genlep,"_pfNOiso_el");
	  else if (abs(genlep.pdgId) == 13) fillHistosGen(h_1d_global,"",genlep,"_pfNOiso_mu");
	}

	// plots for gen leptons in NOT-vetoed events
	if (t.nMuons10 + t.nElectrons10 + t.nPFLep5LowMT + t.nPFHad10LowMT == 0) {
	  fillHistosGen(h_1d_global,"",genlep,"_NOTvetoed");
	  plot2D("h_met_genmet_NOTvetoed", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	  plot2D("h_mt2_genpt_NOTvetoed", genlep.pt, t.mt2, evtweight_, h_2d_global, ";pt(gen lep) [GeV]; MT2 [GeV]", 50, 0., 1000., 50., 0., 1000.);
	  if (genlep.pt > 200.) plot2D("h_met_genmet_NOTvetoed_pt200", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	  // by lepton flavor
	  if (abs(genlep.pdgId) == 11) fillHistosGen(h_1d_global,"",genlep,"_NOTvetoed_el");
	  else if (abs(genlep.pdgId) == 13) fillHistosGen(h_1d_global,"",genlep,"_NOTvetoed_mu");
	}
	// plots for gen leptons in vetoed events
	else {
	  fillHistosGen(h_1d_global,"",genlep,"_vetoed");
	  plot2D("h_met_genmet_vetoed", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	  if (genlep.pt > 200.) plot2D("h_met_genmet_vetoed_pt200", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	  // by lepton flavor
	  if (abs(genlep.pdgId) == 11) fillHistosGen(h_1d_global,"",genlep,"_vetoed_el");
	  else if (abs(genlep.pdgId) == 13) fillHistosGen(h_1d_global,"",genlep,"_vetoed_mu");
	}

      } // loop over gen leps


      // -------------------------------------------------
      //          loop over reco leptons
      // -------------------------------------------------

      // reco lepton loop
      for (int ilep=0; ilep < t.nlep; ++ilep) {
	int pdgId = abs(t.lep_pdgId[ilep]);
	float pt = t.lep_pt[ilep];
	fillHistosReco(h_1d_global, "", ilep, "");
	if (pdgId == 11) fillHistosReco(h_1d_global, "", ilep, "_el");
	else if (pdgId == 13) fillHistosReco(h_1d_global, "", ilep, "_mu");
	if (pt < 20.) fillHistosReco(h_1d_global, "", ilep, "_ptlt20");

	// check for genmatch
	bool genmatch = false;
	for (unsigned int imatch=0; imatch < lep_idx_genmatch.size(); ++imatch) {
	  if (lep_idx_genmatch.at(imatch) == ilep) {
	    genmatch = true;
	    break;
	  }
	}

	if (genmatch) {
	  fillHistosReco(h_1d_global, "", ilep, "_genmatch");
	  if (pdgId == 11) fillHistosReco(h_1d_global, "", ilep, "_genmatch_el");
	  else if (pdgId == 13) fillHistosReco(h_1d_global, "", ilep, "_genmatch_mu");
	  if (pt < 20.) fillHistosReco(h_1d_global, "", ilep, "_genmatch_ptlt20");
	}
	// not gen matched
	else {
	  fillHistosReco(h_1d_global, "", ilep, "_NOTgenmatch");
	  if (pdgId == 11) fillHistosReco(h_1d_global, "", ilep, "_NOTgenmatch_el");
	  else if (pdgId == 13) fillHistosReco(h_1d_global, "", ilep, "_NOTgenmatch_mu");
	  if (pt < 20.) fillHistosReco(h_1d_global, "", ilep, "_NOTgenmatch_ptlt20");
	}

      } // loop over reco leps


      // -------------------------------------------------
      //          loop over pf leptons
      // -------------------------------------------------

      // pf lepton loop
      for (int ipf=0; ipf < t.nisoTrack; ++ipf) {
	// select pf leps
	int pdgId = abs(t.isoTrack_pdgId[ipf]);
	if ( (pdgId != 11) && (pdgId != 13) ) continue;

	float pt = t.isoTrack_pt[ipf];
	fillHistosPF(h_1d_global, "", ipf, "");
	if (pdgId == 11) fillHistosPF(h_1d_global, "", ipf, "_el");
	else if (pdgId == 13) fillHistosPF(h_1d_global, "", ipf, "_mu");
	if (pt < 20.) fillHistosPF(h_1d_global, "", ipf, "_ptlt20");

	// check for genmatch
	bool genmatch = false;
	for (unsigned int imatch=0; imatch < pflep_idx_genmatch.size(); ++imatch) {
	  if (pflep_idx_genmatch.at(imatch) == ipf) {
	    genmatch = true;
	    break;
	  }
	}

	if (genmatch) {
	  fillHistosPF(h_1d_global, "", ipf, "_genmatch");
	  if (pdgId == 11) fillHistosPF(h_1d_global, "", ipf, "_genmatch_el");
	  else if (pdgId == 13) fillHistosPF(h_1d_global, "", ipf, "_genmatch_mu");
	  if (pt < 20.) fillHistosPF(h_1d_global, "", ipf, "_genmatch_ptlt20");
	}
	// not gen matched
	else {
	  fillHistosPF(h_1d_global, "", ipf, "_NOTgenmatch");
	  if (pdgId == 11) fillHistosPF(h_1d_global, "", ipf, "_NOTgenmatch_el");
	  else if (pdgId == 13) fillHistosPF(h_1d_global, "", ipf, "_NOTgenmatch_mu");
	  if (pt < 20.) fillHistosPF(h_1d_global, "", ipf, "_NOTgenmatch_ptlt20");
	}
      } // loop over PF leps


      // -------------------------------------------------
      //          loop over gen hadronic taus
      // -------------------------------------------------

      // only look at events with 0 genLepFromTaus to make sure the tau is hadronic
      if (t.ngenTau > 0 && t.ngenLepFromTau == 0 && t.ngenLep == 0) {

	// loop over gen taus
	for (int igen = 0; igen < t.ngenTau; ++igen) {

	  genlepcand genlep;
	  genlep.pt = t.genTau_pt[igen];
	  genlep.eta = t.genTau_eta[igen];
	  genlep.phi = t.genTau_phi[igen];
	  genlep.pdgId = t.genTau_pdgId[igen];
	  genlep.isFromTau = false;
	  genlep.minDRrecojet = 99.; // every tau is a jet, don't bother with this

	  // gen level acceptance cuts
	  if (genlep.pt < 5. || fabs(genlep.eta) > 2.5) continue;

	  // gen level plots
	  fillHistosGen(h_1d_global,"",genlep,"_tau");


	  // -- loop over PF cands
	  //    -- plot gen, reco quantities for passing objects
	  bool pfmatch = false;
	  bool pfisomatch = false;
	  for (int ipf=0; ipf < t.nisoTrack; ++ipf) {
	    // check flavor first
	    if (abs(t.isoTrack_pdgId[ipf]) != 211) continue;
	    // check dR match
	    float dr = DeltaR(genlep.eta,t.isoTrack_eta[ipf],genlep.phi,t.isoTrack_phi[ipf]);
	    plot1D("h_genpf_dR_tau",      dr,       evtweight_, h_1d_global, ";#Delta R(gen lep,PF lep)", 600, 0, 6);
	    if (dr > 0.2) continue;

	    // note genmatch
	    pfhad_idx_genmatch.push_back(ipf);

	    // only fill hists once, in case there are multiple reco matches
	    if (!pfmatch) {
	      // match: plot gen quantities again to get eff
	      fillHistosGen(h_1d_global,"",genlep,"_tau_pfmatch");
	      plot2D("h_met_genmet_tau_pfmatch", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	      plot2D("h_recopt_genpt_tau_pfmatch", genlep.pt, t.isoTrack_pt[ipf], evtweight_, h_2d_global, ";gen tau pT [GeV]; reco PF cand pT [GeV]", 200, 0., 200., 200., 0., 200.);
	      if (genlep.pt > 200.) plot2D("h_met_genmet_tau_pfmatch_pt200", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	      pfmatch = true;
	    }

	    if (t.isoTrack_absIso[ipf]/t.isoTrack_pt[ipf] > 0.1) continue;

	    // only fill hists once, in case there are multiple reco matches
	    if (!pfisomatch) {
	      // match: plot gen quantities again to get eff
	      fillHistosGen(h_1d_global,"",genlep,"_tau_pfisomatch");
	      plot2D("h_met_genmet_tau_pfisomatch", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	      plot2D("h_recopt_genpt_tau_pfisomatch", genlep.pt, t.isoTrack_pt[ipf], evtweight_, h_2d_global, ";gen tau pT [GeV]; reco PF cand pT [GeV]", 200, 0., 200., 200., 0., 200.);
	      if (genlep.pt > 200.) plot2D("h_met_genmet_tau_pfisomatch_pt200", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	      pfisomatch = true;
	    }

	  } // loop over pf leps

	  // plots for gen leptons with no pf match
	  if (!pfmatch) {
	    // no match: plot gen quantities again
	    fillHistosGen(h_1d_global,"",genlep,"_tau_NOpfmatch");
	    plot2D("h_met_genmet_tau_NOpfmatch", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	    if (genlep.pt > 200.) plot2D("h_met_genmet_tau_NOpfmatch_pt200", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	  }

	  // plots for gen leptons with pf match but failing iso
	  if (pfmatch && !pfisomatch) {
	    // no match: plot gen quantities again
	    fillHistosGen(h_1d_global,"",genlep,"_tau_pfNOiso");
	    plot2D("h_met_genmet_tau_pfNOiso", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	    if (genlep.pt > 200.) plot2D("h_met_genmet_tau_pfNOiso_pt200", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	  }

	  // plots for gen leptons in NOT-vetoed events
	  if (t.nMuons10 + t.nElectrons10 + t.nPFLep5LowMT + t.nPFHad10LowMT == 0) {
	    fillHistosGen(h_1d_global,"",genlep,"_tau_NOTvetoed");
	    plot2D("h_met_genmet_tau_NOTvetoed", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	    plot2D("h_mt2_genpt_tau_NOTvetoed", genlep.pt, t.mt2, evtweight_, h_2d_global, ";pt(gen lep) [GeV]; MT2 [GeV]", 50, 0., 1000., 50., 0., 1000.);
	    if (genlep.pt > 200.) plot2D("h_met_genmet_tau_NOTvetoed_pt200", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	  }
	  // plots for gen leptons in vetoed events
	  else {
	    fillHistosGen(h_1d_global,"",genlep,"_tau_vetoed");
	    plot2D("h_met_genmet_tau_vetoed", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	    if (genlep.pt > 200.) plot2D("h_met_genmet_tau_vetoed_pt200", t.met_genPt, t.met_pt, evtweight_, h_2d_global, ";gen MET [GeV]; MET [GeV]", 50, 0., 1000., 50., 0., 1000.);
	  }


	} // loop over gen taus


      } // if gen hadronic taus only


      // -------------------------------------------------
      //          loop over pf hadrons
      // -------------------------------------------------

      // pf hadron loop
      for (int ipf=0; ipf < t.nisoTrack; ++ipf) {
	// select pf leps
	int pdgId = abs(t.isoTrack_pdgId[ipf]);
	if ( pdgId != 211 ) continue;

	float pt = t.isoTrack_pt[ipf];
	fillHistosPF(h_1d_global, "", ipf, "_had");
	if (pt < 20.) fillHistosPF(h_1d_global, "", ipf, "_ptlt20_had");

	// check for genmatch
	bool genmatch = false;
	for (unsigned int imatch=0; imatch < pfhad_idx_genmatch.size(); ++imatch) {
	  if (pfhad_idx_genmatch.at(imatch) == ipf) {
	    genmatch = true;
	    break;
	  }
	}

	if (genmatch) {
	  fillHistosPF(h_1d_global, "", ipf, "_genmatch_had");
	  if (pt < 20.) fillHistosPF(h_1d_global, "", ipf, "_genmatch_ptlt20_had");
	}
	// not gen matched
	else {
	  fillHistosPF(h_1d_global, "", ipf, "_NOTgenmatch_had");
	  if (pt < 20.) fillHistosPF(h_1d_global, "", ipf, "_NOTgenmatch_ptlt20_had");
	}
      } // loop over PF hads



   }//end loop on events in a file
  
    delete tree;
    f.Close();
  }//end loop on files
  
  cout << "[LepEffLooper::loop] processed " << nEventsTotal << " events" << endl;
  if ( nEventsChain != nEventsTotal ) {
    std::cout << "ERROR: number of events from files is not equal to total number of events" << std::endl;
  }

  cout << nDuplicates << " duplicate events were skipped." << endl;

  //---------------------
  // Make Efficiency Hists
  //---------------------

  // reco leptons

  makeEfficiencyHist(h_1d_global,"","h_gen_pt_recomatch","h_gen_pt");
  makeEfficiencyHist(h_1d_global,"","h_gen_eta_recomatch","h_gen_eta");
  makeEfficiencyHist(h_1d_global,"","h_gen_phi_recomatch","h_gen_phi");

  makeEfficiencyHist(h_1d_global,"","h_gen_pt_recoisomatch","h_gen_pt_recomatch","h_eff_wrtreco_gen_pt_recoisomatch");
  makeEfficiencyHist(h_1d_global,"","h_gen_eta_recoisomatch","h_gen_eta_recomatch","h_eff_wrtreco_gen_eta_recoisomatch");
  makeEfficiencyHist(h_1d_global,"","h_gen_phi_recoisomatch","h_gen_phi_recomatch","h_eff_wrtreco_gen_phi_recoisomatch");

  makeEfficiencyHist(h_1d_global,"","h_gen_pt_recoisomatch","h_gen_pt","h_eff_wrtgen_gen_pt_recoisomatch");
  makeEfficiencyHist(h_1d_global,"","h_gen_eta_recoisomatch","h_gen_eta","h_eff_wrtgen_gen_eta_recoisomatch");
  makeEfficiencyHist(h_1d_global,"","h_gen_phi_recoisomatch","h_gen_phi","h_eff_wrtgen_gen_phi_recoisomatch");

  makeEfficiencyHist(h_1d_global,"","h_gen_pt_recomatch_el","h_gen_pt_el");
  makeEfficiencyHist(h_1d_global,"","h_gen_eta_recomatch_el","h_gen_eta_el");
  makeEfficiencyHist(h_1d_global,"","h_gen_phi_recomatch_el","h_gen_phi_el");

  makeEfficiencyHist(h_1d_global,"","h_gen_pt_recoisomatch_el","h_gen_pt_recomatch_el","h_eff_wrtreco_gen_pt_recoisomatch_el");
  makeEfficiencyHist(h_1d_global,"","h_gen_eta_recoisomatch_el","h_gen_eta_recomatch_el","h_eff_wrtreco_gen_eta_recoisomatch_el");
  makeEfficiencyHist(h_1d_global,"","h_gen_phi_recoisomatch_el","h_gen_phi_recomatch_el","h_eff_wrtreco_gen_phi_recoisomatch_el");

  makeEfficiencyHist(h_1d_global,"","h_gen_pt_recoisomatch_el","h_gen_pt_el","h_eff_wrtgen_gen_pt_recoisomatch_el");
  makeEfficiencyHist(h_1d_global,"","h_gen_eta_recoisomatch_el","h_gen_eta_el","h_eff_wrtgen_gen_eta_recoisomatch_el");
  makeEfficiencyHist(h_1d_global,"","h_gen_phi_recoisomatch_el","h_gen_phi_el","h_eff_wrtgen_gen_phi_recoisomatch_el");

  makeEfficiencyHist(h_1d_global,"","h_gen_pt_recomatch_mu","h_gen_pt_mu");
  makeEfficiencyHist(h_1d_global,"","h_gen_eta_recomatch_mu","h_gen_eta_mu");
  makeEfficiencyHist(h_1d_global,"","h_gen_phi_recomatch_mu","h_gen_phi_mu");

  makeEfficiencyHist(h_1d_global,"","h_gen_pt_recoisomatch_mu","h_gen_pt_recomatch_mu","h_eff_wrtreco_gen_pt_recoisomatch_mu");
  makeEfficiencyHist(h_1d_global,"","h_gen_eta_recoisomatch_mu","h_gen_eta_recomatch_mu","h_eff_wrtreco_gen_eta_recoisomatch_mu");
  makeEfficiencyHist(h_1d_global,"","h_gen_phi_recoisomatch_mu","h_gen_phi_recomatch_mu","h_eff_wrtreco_gen_phi_recoisomatch_mu");

  makeEfficiencyHist(h_1d_global,"","h_gen_pt_recoisomatch_mu","h_gen_pt_mu","h_eff_wrtgen_gen_pt_recoisomatch_mu");
  makeEfficiencyHist(h_1d_global,"","h_gen_eta_recoisomatch_mu","h_gen_eta_mu","h_eff_wrtgen_gen_eta_recoisomatch_mu");
  makeEfficiencyHist(h_1d_global,"","h_gen_phi_recoisomatch_mu","h_gen_phi_mu","h_eff_wrtgen_gen_phi_recoisomatch_mu");



  // PF leptons

  makeEfficiencyHist(h_1d_global,"","h_gen_pt_pfmatch","h_gen_pt");
  makeEfficiencyHist(h_1d_global,"","h_gen_eta_pfmatch","h_gen_eta");
  makeEfficiencyHist(h_1d_global,"","h_gen_phi_pfmatch","h_gen_phi");

  makeEfficiencyHist(h_1d_global,"","h_gen_pt_pfisomatch","h_gen_pt_pfmatch","h_eff_wrtpf_gen_pt_pfisomatch");
  makeEfficiencyHist(h_1d_global,"","h_gen_eta_pfisomatch","h_gen_eta_pfmatch","h_eff_wrtpf_gen_eta_pfisomatch");
  makeEfficiencyHist(h_1d_global,"","h_gen_phi_pfisomatch","h_gen_phi_pfmatch","h_eff_wrtpf_gen_phi_pfisomatch");

  makeEfficiencyHist(h_1d_global,"","h_gen_pt_pfisomatch","h_gen_pt","h_eff_wrtgen_gen_pt_pfisomatch");
  makeEfficiencyHist(h_1d_global,"","h_gen_eta_pfisomatch","h_gen_eta","h_eff_wrtgen_gen_eta_pfisomatch");
  makeEfficiencyHist(h_1d_global,"","h_gen_phi_pfisomatch","h_gen_phi","h_eff_wrtgen_gen_phi_pfisomatch");

  makeEfficiencyHist(h_1d_global,"","h_gen_pt_pfmatch_el","h_gen_pt_el");
  makeEfficiencyHist(h_1d_global,"","h_gen_eta_pfmatch_el","h_gen_eta_el");
  makeEfficiencyHist(h_1d_global,"","h_gen_phi_pfmatch_el","h_gen_phi_el");

  makeEfficiencyHist(h_1d_global,"","h_gen_pt_pfisomatch_el","h_gen_pt_pfmatch_el","h_eff_wrtpf_gen_pt_pfisomatch_el");
  makeEfficiencyHist(h_1d_global,"","h_gen_eta_pfisomatch_el","h_gen_eta_pfmatch_el","h_eff_wrtpf_gen_eta_pfisomatch_el");
  makeEfficiencyHist(h_1d_global,"","h_gen_phi_pfisomatch_el","h_gen_phi_pfmatch_el","h_eff_wrtpf_gen_phi_pfisomatch_el");

  makeEfficiencyHist(h_1d_global,"","h_gen_pt_pfisomatch_el","h_gen_pt_el","h_eff_wrtgen_gen_pt_pfisomatch_el");
  makeEfficiencyHist(h_1d_global,"","h_gen_eta_pfisomatch_el","h_gen_eta_el","h_eff_wrtgen_gen_eta_pfisomatch_el");
  makeEfficiencyHist(h_1d_global,"","h_gen_phi_pfisomatch_el","h_gen_phi_el","h_eff_wrtgen_gen_phi_pfisomatch_el");

  makeEfficiencyHist(h_1d_global,"","h_gen_pt_pfmatch_mu","h_gen_pt_mu");
  makeEfficiencyHist(h_1d_global,"","h_gen_eta_pfmatch_mu","h_gen_eta_mu");
  makeEfficiencyHist(h_1d_global,"","h_gen_phi_pfmatch_mu","h_gen_phi_mu");

  makeEfficiencyHist(h_1d_global,"","h_gen_pt_pfisomatch_mu","h_gen_pt_pfmatch_mu","h_eff_wrtpf_gen_pt_pfisomatch_mu");
  makeEfficiencyHist(h_1d_global,"","h_gen_eta_pfisomatch_mu","h_gen_eta_pfmatch_mu","h_eff_wrtpf_gen_eta_pfisomatch_mu");
  makeEfficiencyHist(h_1d_global,"","h_gen_phi_pfisomatch_mu","h_gen_phi_pfmatch_mu","h_eff_wrtpf_gen_phi_pfisomatch_mu");

  makeEfficiencyHist(h_1d_global,"","h_gen_pt_pfisomatch_mu","h_gen_pt_mu","h_eff_wrtgen_gen_pt_pfisomatch_mu");
  makeEfficiencyHist(h_1d_global,"","h_gen_eta_pfisomatch_mu","h_gen_eta_mu","h_eff_wrtgen_gen_eta_pfisomatch_mu");
  makeEfficiencyHist(h_1d_global,"","h_gen_phi_pfisomatch_mu","h_gen_phi_mu","h_eff_wrtgen_gen_phi_pfisomatch_mu");


  // PF hadrons for taus

  makeEfficiencyHist(h_1d_global,"","h_gen_pt_tau_pfmatch","h_gen_pt_tau");
  makeEfficiencyHist(h_1d_global,"","h_gen_eta_tau_pfmatch","h_gen_eta_tau");
  makeEfficiencyHist(h_1d_global,"","h_gen_phi_tau_pfmatch","h_gen_phi_tau");

  makeEfficiencyHist(h_1d_global,"","h_gen_pt_tau_pfisomatch","h_gen_pt_tau_pfmatch","h_eff_wrtpf_gen_pt_tau_pfisomatch");
  makeEfficiencyHist(h_1d_global,"","h_gen_eta_tau_pfisomatch","h_gen_eta_tau_pfmatch","h_eff_wrtpf_gen_eta_tau_pfisomatch");
  makeEfficiencyHist(h_1d_global,"","h_gen_phi_tau_pfisomatch","h_gen_phi_tau_pfmatch","h_eff_wrtpf_gen_phi_tau_pfisomatch");

  makeEfficiencyHist(h_1d_global,"","h_gen_pt_tau_pfisomatch","h_gen_pt_tau","h_eff_wrtgen_gen_pt_tau_pfisomatch");
  makeEfficiencyHist(h_1d_global,"","h_gen_eta_tau_pfisomatch","h_gen_eta_tau","h_eff_wrtgen_gen_eta_tau_pfisomatch");
  makeEfficiencyHist(h_1d_global,"","h_gen_phi_tau_pfisomatch","h_gen_phi_tau","h_eff_wrtgen_gen_phi_tau_pfisomatch");

  // event level quantities

  makeEfficiencyHist(h_1d_global,"","h_reco_nJet40_vetoed","h_reco_nJet40");
  makeEfficiencyHist(h_1d_global,"","h_reco_nJet40_vetoed_el","h_reco_nJet40_el");
  makeEfficiencyHist(h_1d_global,"","h_reco_nJet40_vetoed_mu","h_reco_nJet40_mu");
  makeEfficiencyHist(h_1d_global,"","h_reco_nJet40_tau_vetoed","h_reco_nJet40_tau");

  makeEfficiencyHist(h_1d_global,"","h_reco_ht_vetoed","h_reco_ht");
  makeEfficiencyHist(h_1d_global,"","h_reco_ht_vetoed_el","h_reco_ht_el");
  makeEfficiencyHist(h_1d_global,"","h_reco_ht_vetoed_mu","h_reco_ht_mu");
  makeEfficiencyHist(h_1d_global,"","h_reco_ht_tau_vetoed","h_reco_ht_tau");

  makeEfficiencyHist(h_1d_global,"","h_reco_mt2_vetoed","h_reco_mt2");
  makeEfficiencyHist(h_1d_global,"","h_reco_mt2_vetoed_el","h_reco_mt2_el");
  makeEfficiencyHist(h_1d_global,"","h_reco_mt2_vetoed_mu","h_reco_mt2_mu");
  makeEfficiencyHist(h_1d_global,"","h_reco_mt2_tau_vetoed","h_reco_mt2_tau");


  //---------------------
  // Save Plots
  //---------------------

  outfile_->cd();
  savePlotsDir(h_1d_global,outfile_,"");
  savePlots2Dir(h_2d_global,outfile_,"");

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

void LepEffLooper::fillHistosGen(std::map<std::string, TH1D*>& h_1d, const std::string& dirname, const genlepcand& genlep, const std::string& s) {

  if (dirname.size()) {
    TDirectory * dir = (TDirectory*)outfile_->Get(dirname.c_str());
    if (dir == 0) {
      dir = outfile_->mkdir(dirname.c_str());
    } 
    dir->cd();
  } else {
    outfile_->cd();
  }

  plot1D("h_gen_pt"+s,       genlep.pt,   evtweight_, h_1d, ";p_{T}(gen lep) [GeV]", 1000, 0., 1000.);
  plot1D("h_gen_eta"+s,      genlep.eta,  evtweight_, h_1d, ";#eta(gen lep)", 300, -3.0, 3.0);
  plot1D("h_gen_phi"+s,      genlep.phi,  evtweight_, h_1d, ";#phi(gen lep)", 340, -3.2, 3.2);
  plot1D("h_gen_minDRrecojet"+s,      genlep.minDRrecojet,  evtweight_, h_1d, ";min #Delta R(gen lep, reco jet)", 300, 0., 6.);

  // reco plots, to plot gen efficiency vs these
  plot1D("h_reco_nJet40"+s,      t.nJet40,  evtweight_, h_1d, ";N(jets)", 10, 0., 10.);
  plot1D("h_reco_ht"+s,      t.ht,  evtweight_, h_1d, ";H_{T} [GeV]", 50, 0., 1500.);
  plot1D("h_reco_mt2"+s,      t.mt2,  evtweight_, h_1d, ";M_{T2} [GeV]", 30, 0., 1500.);

  outfile_->cd();
  return;
}

void LepEffLooper::fillHistosReco(std::map<std::string, TH1D*>& h_1d, const std::string& dirname, const unsigned int& ilep, const std::string& s) {

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
  // plot1D("h_reco_eta"+s,      t.lep_eta[ilep],  evtweight_, h_1d, ";#eta(reco lep)", 60, -3.0, 3.0);
  // plot1D("h_reco_phi"+s,      t.lep_phi[ilep],  evtweight_, h_1d, ";#phi(reco lep)", 68, -3.2, 3.2);
  plot1D("h_reco_relIso"+s,      t.lep_relIso03[ilep],  evtweight_, h_1d, ";Rel PFIso", 200, 0., 2.);
  plot1D("h_reco_absIso"+s,      t.lep_relIso03[ilep]*t.lep_pt[ilep],  evtweight_, h_1d, ";Abs PFIso [GeV]", 200, 0., 10.);
  // plot1D("h_reco_miniRelIso"+s,      t.lep_miniRelIso[ilep],  evtweight_, h_1d, ";Mini Rel PFIso", 200, 0., 2.);
  // plot1D("h_reco_miniAbsIso"+s,      t.lep_miniRelIso[ilep]*t.lep_pt[ilep],  evtweight_, h_1d, ";Mini Abs PFIso [GeV]", 200, 0., 10.);
  // plot1D("h_reco_ptRelv0"+s,      t.lep_ptRelv0[ilep],  evtweight_, h_1d, ";p_{T}Rel v0 [GeV]", 200, 0., 20.);
  // plot1D("h_reco_ptRelv1"+s,      t.lep_ptRelv1[ilep],  evtweight_, h_1d, ";p_{T}Rel v1 [GeV]", 200, 0., 20.);

  outfile_->cd();
  return;
}

void LepEffLooper::fillHistosPF(std::map<std::string, TH1D*>& h_1d, const std::string& dirname, const unsigned int& ipf, const std::string& s) {

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
  // plot1D("h_pf_eta"+s,      t.isoTrack_eta[ipf],  evtweight_, h_1d, ";#eta(PF lep)", 300, -3.0, 3.0);
  // plot1D("h_pf_phi"+s,      t.isoTrack_phi[ipf],  evtweight_, h_1d, ";#phi(PF lep)", 340, -3.2, 3.2);
  plot1D("h_pf_relIso"+s,      t.isoTrack_absIso[ipf]/t.isoTrack_pt[ipf],  evtweight_, h_1d, ";Rel TrkIso", 200, 0., 2.);
  plot1D("h_pf_absIso"+s,      t.isoTrack_absIso[ipf],  evtweight_, h_1d, ";Abs TrkIso [GeV]", 200, 0., 10.);
  // plot1D("h_pf_miniRelIso"+s,      t.isoTrack_miniAbsIso[ipf]/t.isoTrack_pt[ipf],  evtweight_, h_1d, ";Mini Rel TrkIso", 200, 0., 2.);
  // plot1D("h_pf_miniAbsIso"+s,      t.isoTrack_miniAbsIso[ipf],  evtweight_, h_1d, ";Mini Abs TrkIso [GeV]", 200, 0., 10.);
  // plot1D("h_pf_ptRelv0"+s,      t.isoTrack_ptRelv0[ipf],  evtweight_, h_1d, ";p_{T}Rel v0 [GeV]", 200, 0., 20.);
  // plot1D("h_pf_ptRelv1"+s,      t.isoTrack_ptRelv1[ipf],  evtweight_, h_1d, ";p_{T}Rel v1 [GeV]", 200, 0., 20.);
  float mt = MT(t.isoTrack_pt[ipf],t.isoTrack_phi[ipf],t.met_pt,t.met_phi);
  plot1D("h_pf_mt"+s,      mt,  evtweight_, h_1d, ";M_{T} [GeV]", 200, 0., 200.);

  outfile_->cd();
  return;
}

void LepEffLooper::makeEfficiencyHist(std::map<std::string, TH1D*>& h_1d, const std::string& dirname, const std::string& num_name, const std::string& denom_name, const std::string& eff_name) {

  TH1D* num = getHist1D(num_name,h_1d,1,0,1);
  TH1D* denom = getHist1D(denom_name,h_1d,1,0,1);

  return makeEfficiencyHist(h_1d,dirname,num,denom,eff_name);

}

void LepEffLooper::makeEfficiencyHist(std::map<std::string, TH1D*>& h_1d, const std::string& dirname, TH1D* num, TH1D* denom, const std::string& eff_name) {

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
  // pt hist - rebin
  TH1D* eff = 0;
  if (eff_tname.Contains("_pt_")) {
    eff = (TH1D*) num->Rebin(n_ptbins,eff_tname.Data(),ptbins);
  } else {
    eff = (TH1D*) num->Clone(eff_tname.Data());
  }

  TString denom_name = TString(denom->GetName()) + "_clone";
  TH1D* denom_clone = 0;
  if (eff_tname.Contains("_pt_")) {
    denom_clone = (TH1D*) denom->Rebin(n_ptbins,denom_name.Data(),ptbins);
  } else {
    denom_clone = (TH1D*) denom->Clone(denom_name.Data());
  }

  eff->Divide(eff,denom_clone,1.0,1.0,"B");

  insertHist1D(eff,h_1d);

  delete denom_clone;

  outfile_->cd();
  return;
}

#include "ShortTrackCRSL.h"

using namespace std;

class mt2tree;
class sttree;

const float ht_limit = 100;
const float mt2_limit = 100;
const float met_limit = 100;

const int maskHT = 1;
const int maskMET = 1<<1;
const int maskDphiDiff = 1<<2;
const int maskMT2 = 1<<3;
const int maskLep = 1<<4;
const int maskNj = 1<<5;
const int maskEta = 1<<6;
const int maskNt = 1<<7;

bool doIncl = false;
bool doNj = false;
bool doHT = false;
bool doMET = false;
bool doDphiDiff = false;
bool doMT2 = false;
bool doLep = false;
bool doEta = false;
bool doNt = false;

int ShortTrackCRSL::loop (char * indir, char * sample, int selection) {
  doIncl = selection == 0;
  doHT = maskHT & selection;
  doMET = maskMET & selection;
  doDphiDiff = maskDphiDiff & selection;
  doMT2 = maskMT2 & selection;
  doLep = maskLep & selection;
  doNj = maskNj & selection;
  doEta = maskEta & selection;
  doNt = maskNt & selection;

  string outdir = "";
  if (doIncl) outdir = "incl";
  else {
    if (doHT) outdir += "HT";
    if (doMET) outdir += "MET";
    if (doDphiDiff) outdir += "DphiDiff";
    if (doMT2) outdir += "MT2";
    if (doLep) outdir += "Lep";
    if (doNj) outdir += "Nj";
    if (doEta) outdir += "Eta";
    if (doNt) outdir += "Nt";
  }

  string mkdir = "mkdir -p output_CRSL/" + outdir;
  system(mkdir.c_str());
  TString output_name = Form("output_CRSL/%s/%s.root",outdir.c_str(),sample);

  TChain* ch_mt2 = new TChain("mt2st");
  TChain* ch_st = new TChain("st");

  TString infile_name = Form("%s/%s.root",indir,sample);
  cout << "Input file: " << infile_name.Data() << endl;
  ch_mt2->Add( infile_name );
  ch_st->Add( infile_name );

  // Get File Content
  TTree *tree_mt2 = (TTree*)ch_mt2->Clone("mt2st");
  TTree *tree_st = (TTree*)ch_st->Clone("st");

  string wjets_incl = "wjets_incl";
  string sample_str = sample;
  bool isWjetsIncl = ! wjets_incl.compare(sample_str);

  // sttree is a subtype of mt2tree. mt2tree functions can be called on an sttree, but not the reverse.
  // This line incorporates branches from tree_mt2 into tree_st.
  tree_st->AddFriend(tree_mt2);

  sttree t;

  // Our baby tree is an sttree so that we can call both mt2 and st methods mindlessly.
  t.Init(tree_st); // The Init function from mt2tree, for mt2 branches.
  t.Init_ST(tree_st); // The Init function from sttree, for st branches.

  // Event Loop
  // Sanity check:
  const unsigned int nEventsTree = tree_mt2->GetEntries();
  if (nEventsTree != tree_st->GetEntries()) {
    cout << "st and mt2 trees have different numbers of entries. This should not be possible if the input file was produced by treefriend.py and never edited. Aborting..." << endl;
    return 2;
  }

  // Book Histograms
  TH1::SetDefaultSumw2(true);
  TH1F h_unskimmed("h_unskimmed","Events in Unskimmed File",1,0,1);

  // all
  TH1F h_el_pix("h_el_pix","Electron STs, Pixel Layers",2,2,4);
  TH1F h_el_ext("h_el_ext","Electron STs, Non-Pixel Layers",8,0,8);
  TH1F h_el_tot("h_el_tot","Electron STs, Total Layers",11,0,11);
  TH1F h_fake_pix("h_fake_pix","(Mostly) Fake STs, Pixel Layers",2,2,4);
  TH1F h_fake_ext("h_fake_ext","(Mostly) Fake STs, Non-Pixel Layers",8,0,8);
  TH1F h_fake_tot("h_fake_tot","(Mostly) Fake STs, Total Layers",11,0,11);
  TH2F h_el_etaphi("h_el_etaphi","Electron STs, #eta and #phi",1200,-2.4,2.4,1200,-3.14,3.14);
  TH2F h_fake_etaphi("h_fake_etaphi","(Mostly) Fake STs, #eta and #phi",1200,-2.4,2.4,1200,-3.14,3.14);
  TH1F h_el_ratio("h_el_ratio","Gen e / ST p_T Ratio",15,0.5,2.0);
  TH1F h_el_met("h_el_met","Electron STs, MET",20,0,300);
  TH1F h_fake_met("h_fake_met","(Mostly) Fake STs, MET",20,0,300);

  TH1F h_all_nt("h_all_nt","N_{t}",10,0,10);
  TH1F h_el_nt("h_el_nt","N_{t}",10,0,10);
  TH1F h_fake_nt("h_fake_nt","N_{t}",10,0,10);

  // isshort
  TH1F h_el_pix_s("h_el_pix_s","Electron STs, Pixel Layers",2,2,4);
  TH1F h_el_ext_s("h_el_ext_s","Electron STs, Non-Pixel Layers",8,0,8);
  TH1F h_el_tot_s("h_el_tot_s","Electron STs, Total Layers",11,0,11);
  TH1F h_fake_pix_s("h_fake_pix_s","(Mostly) Fake STs, Pixel Layers",2,2,4);
  TH1F h_fake_ext_s("h_fake_ext_s","(Mostly) Fake STs, Non-Pixel Layers",8,0,8);
  TH1F h_fake_tot_s("h_fake_tot_s","(Mostly) Fake STs, Total Layers",11,0,11);
  TH2F h_el_etaphi_s("h_el_etaphi_s","Electron STs, #eta and #phi",1200,-2.4,2.4,1200,-3.14,3.14);
  TH2F h_fake_etaphi_s("h_fake_etaphi_s","(Mostly) Fake STs, #eta and #phi",1200,-2.4,2.4,1200,-3.14,3.14);
  TH1F h_el_ratio_s("h_el_ratio_s","Gen e / ST p_T Ratio",15,0.5,2.0);
  TH1F h_el_met_s("h_el_met_s","Electron STs, MET",20,0,300);
  TH1F h_fake_met_s("h_fake_met_s","(Mostly) Fake STs, MET",20,0,300);

  TH1F h_all_chDR_s("h_all_chDR_s","All STs, Nearest Charged DR",20,0,1.0);
  TH1F h_all_anyDR_s("h_all_anyDR_s","All STs, Nearest DR",20,0,1.0);
  TH1F h_all_chID_s("h_all_chID_s","All STs, Nearest Charged pdgID",250,0,250);
  TH1F h_all_anyID_s("h_all_anyID_s","All STs, Nearest pdgID",250,0,250);
  TH1F h_all_nearby_s("h_all_nearby_s","All STs, #DeltaR of Nearest Long Track",20,0,1.0);

  // istrkshort
  TH1F h_el_pix_ts("h_el_pix_ts","Electron STs, Pixel Layers",2,2,4);
  TH1F h_el_ext_ts("h_el_ext_ts","Electron STs, Non-Pixel Layers",8,0,8);
  TH1F h_el_tot_ts("h_el_tot_ts","Electron STs, Total Layers",11,0,11);
  TH1F h_fake_pix_ts("h_fake_pix_ts","(Mostly) Fake STs, Pixel Layers",2,2,4);
  TH1F h_fake_ext_ts("h_fake_ext_ts","(Mostly) Fake STs, Non-Pixel Layers",8,0,8);
  TH1F h_fake_tot_ts("h_fake_tot_ts","(Mostly) Fake STs, Total Layers",11,0,11);
  TH2F h_el_etaphi_ts("h_el_etaphi_ts","Electron STs, #eta and #phi",1200,-2.4,2.4,1200,-3.14,3.14);
  TH2F h_fake_etaphi_ts("h_fake_etaphi_ts","(Mostly) Fake STs, #eta and #phi",1200,-2.4,2.4,1200,-3.14,3.14);
  TH1F h_el_ratio_ts("h_el_ratio_ts","Gen e / ST p_T Ratio",15,0.5,2.0);
  TH1F h_el_met_ts("h_el_met_ts","Electron STs, MET",20,0,300);
  TH1F h_fake_met_ts("h_fake_met_ts","(Mostly) Fake STs, MET",20,0,300);

  // istrklong
  TH1F h_el_pix_tl("h_el_pix_tl","Electron STs, Pixel Layers",2,2,4);
  TH1F h_el_ext_tl("h_el_ext_tl","Electron STs, Non-Pixel Layers",8,0,8);
  TH1F h_el_tot_tl("h_el_tot_tl","Electron STs, Total Layers",11,0,11);
  TH1F h_fake_pix_tl("h_fake_pix_tl","(Mostly) Fake STs, Pixel Layers",2,2,4);
  TH1F h_fake_ext_tl("h_fake_ext_tl","(Mostly) Fake STs, Non-Pixel Layers",8,0,8);
  TH1F h_fake_tot_tl("h_fake_tot_tl","(Mostly) Fake STs, Total Layers",11,0,11);
  TH2F h_el_etaphi_tl("h_el_etaphi_tl","Electron STs, #eta and #phi",1200,-2.4,2.4,1200,-3.14,3.14);
  TH2F h_fake_etaphi_tl("h_fake_etaphi_tl","(Mostly) Fake STs, #eta and #phi",1200,-2.4,2.4,1200,-3.14,3.14);
  TH1F h_el_ratio_tl("h_el_ratio_tl","Gen e / ST p_T Ratio",15,0.5,2.0);
  TH1F h_el_met_tl("h_el_met_tl","Electron STs, MET",20,0,300);
  TH1F h_fake_met_tl("h_fake_met_tl","(Mostly) Fake STs, MET",20,0,300);

  for( unsigned int event = 0; event < nEventsTree; ++event) {
    
    // AddFriend above makes this pull both mt2 and st branches for the same event, assuming the trees were properly (identically) sorted. 
    t.GetEntry(event); 
    
    //    const float w_ = t.evt_xsec * 1000 / nEventsTree;
    const float w_ = 1.0; // Just count occurences without wrinkle of event weight

    //---------------------
    // basic event selection and cleaning
    //---------------------
    
    if (t.nVert == 0) continue;
    
    // random events with ht or met=="inf" or "nan" that don't get caught by the filters...
    if(isinf(t.met_pt) || isnan(t.met_pt) || isinf(t.ht) || isnan(t.ht)){
      cout << "WARNING: bad event with infinite MET/HT! " << t.run << ":" << t.lumi << ":" << t.evt
	   << ", met=" << t.met_pt << ", ht=" << t.ht << endl;
      continue;
    }
    
    // catch events with unphysical jet pt
    if(t.jet_pt[0] > 13000.){
      cout << "WARNING: bad event with unphysical jet pt! " << t.run << ":" << t.lumi << ":" << t.evt
	   << ", met=" << t.met_pt << ", ht=" << t.ht << ", jet_pt=" << t.jet_pt[0] << endl;
      continue;
    }

    if (isWjetsIncl) {
      bool foundGoodW = false;
      for (int i = 0; i < t.ngenStat23 && ! foundGoodW; i++) {
	if (abs(t.genStat23_pdgId[i]) != 24) continue;
	if (t.genStat23_pt[i] > 100) foundGoodW = true;      
      }
      if (!foundGoodW) continue;
    }

    if (!doIncl && t.nJet30 == 0) continue;

    //    if (doEta && st_eta > 1.4) continue;

    if (doNj && t.nJet30 < 2) continue;

    if (doNt && t.ntracks > 5) continue;

    // Recalculate ht manually to omit leptons
    float ht = 0;
    for (int ij = 0; ij < t.njet; ij++) {
      ht += t.jet_pt[ij];
    }

    if (doMET) {
      if ((ht < 1000 && t.met_pt < met_limit) || t.met_pt < 30 || ht < ht_limit ) continue;
    }
    else if (doHT && ht < ht_limit) continue;

    if (doDphiDiff && (t.deltaPhiMin < 0.3 || t.diffMetMht / t.met_pt > 0.5) ) continue;

    if (doMT2 && t.mt2 < mt2_limit) continue;

    // In some cases, useful to store the index of a track matched to a lepton
    int i_lowMT_trk = -1;
    int i_lowMT_lep = -1;
    if (doLep && t.nLepLowMT != 1) continue;

    // This event passes all non-short track selections
    h_unskimmed.Fill(0.5,w_);
    if (t.nshorttracks == 0) continue;

    // From here on, there's a short track in this event
    // Collect some short track info
    int st_idx = -1;
    double st_phi = -999;
    double st_eta = -999;
    double st_pt = -1;
    int st_pix = -1;
    int st_ext = -1;
    int st_tot = -1;
    for (int i_trk = 0; i_trk < t.ntracks; i_trk++) {
      if (t.track_isshort[i_trk]) {
	st_idx = i_trk;
	st_phi = t.track_phi[i_trk];
	st_eta = t.track_eta[i_trk];
	st_pt = t.track_pt[i_trk];
	st_pix = t.track_pixelLayersWithMeasurement[i_trk];
	st_tot = t.track_trackerLayersWithMeasurement[i_trk];
	st_ext = st_tot - st_pix;
	break;
      }
    }

    if (st_idx < 0 || fabs(st_phi) > M_PI || fabs(st_eta) > 2.4 || st_pt < 0 || st_pix < 0 || st_ext < 0 || st_tot < 0) {
      cout << "Something went wrong reading from the short track in event index " << event << endl;
      cout << "st_idx = " << st_idx << endl;
      cout << "st_phi = " << st_phi << endl;
      cout << "st_eta = " << st_eta << endl;
      cout << "st_pt = " << st_pt << endl;
      cout << "st_pix = " << st_pix << endl;
      cout << "st_tot = " << st_tot << endl;
      t.Show(event);
      return 3;
    }
    
    // Now we need to exclude the event if its short track is the LowMT lepton's track
    float lmt_phi = -999;
    float lmt_eta = -999;
    float met_pt = t.met_pt;
    float met_phi = t.met_phi;      
    for (int ilep = 0; ilep < t.nlep; ilep++) {
      float lep_pt = t.lep_pt[ilep];
      float lep_phi = t.lep_phi[ilep];
      float lep_eta = t.lep_eta[ilep];
      float mt = MT(lep_pt,lep_phi,met_pt,met_phi);
      if (mt < 100) {
	// This guy is the LowMT lepton.
	lmt_phi = lep_phi;
	lmt_eta = lep_eta;
	i_lowMT_lep = ilep;
	break;
      }
    }
    // If the closest track is the short track, don't count this event as a short track event
    float smallestDR = 10;       
    for (int itrk = 0; itrk < t.ntracks; itrk++) {
      float trk_eta = t.track_eta[itrk];
      float trk_phi = t.track_phi[itrk];
      float thisDR = DeltaR(lmt_eta,lmt_phi,trk_eta,trk_phi);
      if (thisDR < smallestDR) {
	smallestDR = thisDR;
	i_lowMT_trk = itrk;	      
      }
    }
    if (i_lowMT_trk == st_idx) {
      cout << "Disqualifed short track: Low MT lepton." << endl;
      continue; // Best match is the short track, disqualified!
    }
    
    // Search for disappearing electron close to track.
    // Else assume track is fake since we don't have non-lep gen particles in babies.
    bool e_match = false;
    for (int i_lep = 0; i_lep < t.ngenLep; i_lep++) {
      if (abs(t.genLep_pdgId[i_lep]) != 11) continue;
      float lep_eta = t.genLep_eta[i_lep];
      float lep_phi = t.genLep_phi[i_lep];
      if (sqrt( pow(lep_eta - st_eta,2) + pow(lep_phi - st_phi,2) ) > 0.1 ) continue;
      float ratio_pt = t.genLep_pt[i_lep] / st_pt;
      if (ratio_pt < 0.5 || ratio_pt > 2.0) continue;
      h_el_ratio.Fill(ratio_pt,w_);
      // Matched to electron, probably disappearing. Don't have the calorimeter reading needed to confirm but...
      e_match = true;
      break;
    }

    if (t.track_isshort[st_idx]) {
      h_all_chDR_s.Fill(abs(t.track_pfDR[st_idx]),w_);
      h_all_anyDR_s.Fill(abs(t.track_anypfDR[st_idx]),w_);
      h_all_chID_s.Fill(abs(t.track_pfPdgId[st_idx]),w_);
      h_all_anyID_s.Fill(abs(t.track_anypfPdgId[st_idx]),w_);
      float nearestDR = 10;
      for (int i_trk = 0; i_trk < t.ntracks; i_trk++) {
	if (t.track_isshort[i_trk]) continue;
	float track_eta = t.track_eta[i_trk];
	float track_phi = t.track_phi[i_trk];
	float DR = DeltaR(track_eta,track_phi,st_eta,st_phi);
	if (DR < nearestDR) {
	  nearestDR = DR;
	}
      }
      h_all_nearby_s.Fill(nearestDR,w_);
    }

    if (e_match) {
      h_el_pix.Fill(st_pix,w_);
      h_el_ext.Fill(st_ext,w_);
      h_el_tot.Fill(st_tot,w_);
      h_el_etaphi.Fill(st_eta,st_phi,w_);
      h_el_met.Fill(t.met_pt);
      if (t.track_isshort[st_idx]) {
	h_el_pix_s.Fill(st_pix,w_);
	h_el_ext_s.Fill(st_ext,w_);
	h_el_tot_s.Fill(st_tot,w_);
	h_el_etaphi_s.Fill(st_eta,st_phi,w_);
	h_el_met_s.Fill(t.met_pt);
      }
      else if (t.track_istrkshort[st_idx]) {
	h_el_pix_ts.Fill(st_pix,w_);
	h_el_ext_ts.Fill(st_ext,w_);
	h_el_tot_ts.Fill(st_tot,w_);
	h_el_etaphi_ts.Fill(st_eta,st_phi,w_);
	h_el_met_ts.Fill(t.met_pt);
      }
      else if (t.track_istrklong[st_idx]) {
	h_el_pix_tl.Fill(st_pix,w_);
	h_el_ext_tl.Fill(st_ext,w_);
	h_el_tot_tl.Fill(st_tot,w_);
	h_el_etaphi_tl.Fill(st_eta,st_phi,w_);
	h_el_met_tl.Fill(t.met_pt);
      } else {
	cout << "A short track is neither short, trkshort, or trklong. Something went wrong: evt = " << t.evt << endl;
      }

    } else {
      //cout << t.run << ":" << t.lumi << ":" << t.evt << endl;
      h_fake_pix.Fill(st_pix,w_);
      h_fake_ext.Fill(st_ext,w_);
      h_fake_tot.Fill(st_tot,w_);
      h_fake_etaphi.Fill(st_eta,st_phi,w_);
      h_fake_met.Fill(t.met_pt);
      
      // The LowMT lepton's track can be as soft as 5 GeV, but we only store tracks > 10 GeV (and short tracks need 15 GeV)
      // So, there is a chance, but not guaranteed, that the lowMT lepton's track is contributing to our 10 GeV track count.
      // Subtract it if so.
      int ntracks = t.ntracks;
      if (doLep) {
	float trk_eta = t.track_eta[i_lowMT_trk];
	float trk_phi = t.track_phi[i_lowMT_trk];
	float lep_eta = t.lep_phi[i_lowMT_lep];
	float lep_phi = t.lep_phi[i_lowMT_lep];
	float DR = DeltaR(trk_eta,trk_phi,lep_eta,lep_phi);
	if (DR < 0.1) {
	  ntracks--; // If the lowMT lep is matched to a track in the collection, remove from ntracks
	  // check for plausibility
	  if (t.lep_pt[i_lowMT_lep] < 10) {
	    cout << "Track removed from count despite LowMT lepton having pT < 10 GeV" << endl;
	    cout << "Track had pT = " << t.track_pt[i_lowMT_trk] << endl;
	  }
	  
	}
      } // Shouldn't need to adjust CRDY since its leptons ("nlep") are pT > 10 GeV

      h_fake_nt.Fill(ntracks);

      if (t.track_isshort[st_idx]) {
	h_fake_pix_s.Fill(st_pix,w_);
	h_fake_ext_s.Fill(st_ext,w_);
	h_fake_tot_s.Fill(st_tot,w_);
	h_fake_etaphi_s.Fill(st_eta,st_phi,w_);
	h_fake_met_s.Fill(t.met_pt);
      }
      else if (t.track_istrkshort[st_idx]) {
	h_fake_pix_ts.Fill(st_pix,w_);
	h_fake_ext_ts.Fill(st_ext,w_);
	h_fake_tot_ts.Fill(st_tot,w_);
	h_fake_etaphi_ts.Fill(st_eta,st_phi,w_);
	h_fake_met_ts.Fill(t.met_pt);
      }
      else if (t.track_istrklong[st_idx]) {
	h_fake_pix_tl.Fill(st_pix,w_);
	h_fake_ext_tl.Fill(st_ext,w_);
	h_fake_tot_tl.Fill(st_tot,w_);
	h_fake_etaphi_tl.Fill(st_eta,st_phi,w_);
	h_fake_met_tl.Fill(t.met_pt);
      } else{
	cout << "A short track is neither short, trkshort, or trklong. Something went wrong: evt = " << t.evt << endl;
      }
    }

  }//end loop on events in a file

  cout << "About to write" << endl;

  TFile outfile_(output_name,"RECREATE"); 
  h_unskimmed.Write();

  h_el_pix.Write();
  h_el_ext.Write();
  h_el_tot.Write();
  h_el_etaphi.Write();
  h_el_ratio.Write();
  h_fake_pix.Write();
  h_fake_ext.Write();
  h_fake_tot.Write();
  h_fake_etaphi.Write();
  h_el_met.Write();
  h_fake_met.Write();

  h_all_nt.Write();
  h_el_nt.Write();
  h_fake_nt.Write();

  h_el_pix_s.Write();
  h_el_ext_s.Write();
  h_el_tot_s.Write();
  h_el_etaphi_s.Write();
  h_el_ratio_s.Write();
  h_fake_pix_s.Write();
  h_fake_ext_s.Write();
  h_fake_tot_s.Write();
  h_fake_etaphi_s.Write();
  h_el_met_s.Write();
  h_fake_met_s.Write();

  h_all_chDR_s.Write();
  h_all_anyDR_s.Write();
  h_all_chID_s.Write();
  h_all_anyID_s.Write();
  h_all_nearby_s.Write();

  h_el_pix_ts.Write();
  h_el_ext_ts.Write();
  h_el_tot_ts.Write();
  h_el_etaphi_ts.Write();
  h_el_ratio_ts.Write();
  h_fake_pix_ts.Write();
  h_fake_ext_ts.Write();
  h_fake_tot_ts.Write();
  h_fake_etaphi_ts.Write();
  h_el_met_ts.Write();
  h_fake_met_ts.Write();

  h_el_pix_tl.Write();
  h_el_ext_tl.Write();
  h_el_tot_tl.Write();
  h_el_etaphi_tl.Write();
  h_el_ratio_tl.Write();
  h_fake_pix_tl.Write();
  h_fake_ext_tl.Write();
  h_fake_tot_tl.Write();
  h_fake_etaphi_tl.Write();
  h_el_met_tl.Write();
  h_fake_met_tl.Write();

  outfile_.Close();

  cout << "Wrote everything" << endl;
  
  return 0;
}

int main (int argc, char ** argv) {

  if (argc < 4) {
    cout << "Usage: ./ShortTrackCRSL.exe <indir> <sample> <selection>" << endl;
    return 1;
  }

  const int selection = strtol(argv[3],NULL,2);
  cout << "selection: " << selection << endl;
  ShortTrackCRSL * stt = new ShortTrackCRSL();
  stt->loop(argv[1],argv[2],selection);
  return 0;
}

ShortTrackCRSL::ShortTrackCRSL() {}

ShortTrackCRSL::~ShortTrackCRSL() {};

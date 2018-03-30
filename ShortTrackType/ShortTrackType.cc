#include "ShortTrackType.h"

using namespace std;

class mt2tree;
class sttree;

const int maskHT = 1;
const int maskMET = 1<<1;
const int maskDphiDiff = 1<<2;
const int maskMT2 = 1<<3;
const int maskLep = 1<<4;
const int maskNj = 1<<5;

const int SR = 0;
const int CRQCD = 1;
const int CRSL = 2;
const int CRDY = 3;

bool doIncl = false;
bool doNj = false;
bool doHT = false;
bool doMET = false;
bool doDphiDiff = false;
bool doMT2 = false;
bool doLep = false;

string ShortTrackType::StringSelection(int selection, int region) {
  vector<string> selections;
  if (doHT && ! doMET) {
    switch (region) {
    case SR : 
    case CRQCD : 
    case CRSL : selections.push_back("(ht > 100)"); break; 
    case CRDY : selections.push_back("(zll_ht > 100)"); break;
    }
  }
  if (doMET) {
    switch (region) {
    case SR : 
    case CRQCD : 
    case CRSL : selections.push_back("( (met_pt > 250) || (met_pt > 30 && ht > 1000) )"); break; 
    case CRDY : selections.push_back("( (zll_met_pt > 250) || (zll_met_pt > 30 && zll_ht > 1000) )"); break;
    }    
  }
  if (doDphiDiff) {
    switch (region) {
    case SR : 
    case CRSL : selections.push_back("( deltaPhiMin > 0.3 && diffMetMht / met_pt < 0.5 )"); break;
    case CRDY : selections.push_back("( zll_deltaPhiMin > 0.3 && zll_diffMetMht / zll_met_pt < 0.5 )"); break;
    case CRQCD : selections.push_back("( deltaPhiMin < 0.3 && diffMetMht / met_pt < 0.5 )"); break;
    }
  }
  if (doMT2) {
    switch (region) {
    case SR :
    case CRQCD :
    case CRSL : selections.push_back(" (mt2 > 200) "); break;
    case CRDY : selections.push_back(" (zll_mt2 > 200) "); break;
    }
  }
  if (doLep) {
    switch(region) {
    case SR :
    case CRQCD : selections.push_back("( nElectrons10 + nMuons10 + nPFLep5LowMT + nPFHad10LowMT == 0)"); break;
    case CRSL : selections.push_back("( nLepLowMT == 1 )"); break;
    case CRDY : selections.push_back("( nlep == 2 )"); break;
    }
  }
  if (doNj) {
    selections.push_back("( nJet30 > 1 )");
  } else if (!doIncl) {
    selections.push_back("( nJet30 > 0 )");
  }
  
  if (selections.size() == 0) return "";
  if (selections.size() == 1) return selections.at(0);
  string toReturn = selections.at(0);
  for (unsigned int i = 1; i < selections.size(); i++) {
    toReturn += " && " + selections.at(i);    
  }
  return toReturn;
}

int ShortTrackType::loop (char * indir, char * sample, char * unskimmed_dir, int selection, int region) {
  doIncl = selection == 0;
  doHT = maskHT & selection;
  doMET = maskMET & selection;
  doDphiDiff = maskDphiDiff & selection;
  doMT2 = maskMT2 & selection;
  doLep = maskLep & selection;
  doNj = maskNj & selection;

  string outdir = "";
  if (doIncl) outdir = "incl";
  else {
    if (doHT) outdir += "HT";
    if (doMET) outdir += "MET";
    if (doDphiDiff) outdir += "DphiDiff";
    if (doMT2) outdir += "MT2";
    if (doLep) outdir += "Lep";
    if (doNj) outdir += "Nj";
  }

  string output_prefix;
  switch(region) {
  case SR : output_prefix = "output_SR"; break;
  case CRQCD : output_prefix = "output_CRQCD"; break;
  case CRSL : output_prefix = "output_CRSL"; break;
  case CRDY : output_prefix = "output_CRDY"; break;
  }
  string fulloutdir = output_prefix+"/"+outdir;
  string mkdir = "mkdir -p " + fulloutdir;
  system(mkdir.c_str());
  TString output_name = Form("%s/%s.root",fulloutdir.c_str(),sample);

  TChain* ch_mt2 = new TChain("mt2st");
  TChain* ch_st = new TChain("st");

  TChain* ch_us = new TChain("mt2st");

  TString infile_name = Form("%s/%s_incl_skim_friend.root",indir,sample);
  cout << "Skimmed file: " << infile_name.Data() << endl;
  ch_mt2->Add( infile_name );
  ch_st->Add( infile_name );

  TString unskimmed_name = Form("%s/%s.root",unskimmed_dir,sample);
  cout << "Unskimmed file: " << unskimmed_name.Data() << endl;
  ch_us->Add( unskimmed_name );

  // Get File Content
  TTree *tree_mt2 = (TTree*)ch_mt2->Clone("mt2st");
  TTree *tree_st = (TTree*)ch_st->Clone("st");

  int total_events = ch_us->GetEntries((this->StringSelection(selection,region)).c_str());
  cout << "Total events in unskimmed file: " << total_events << endl;
  cout << "Total events is subject to adjustment if short tracks affect region definition." << endl;
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
  TH1F h_el_pix("h_el_pix","Electron STs, Pixel Layers",2,2,4);
  TH1F h_el_ext("h_el_ext","Electron STs, Non-Pixel Layers",8,0,8);
  TH1F h_el_tot("h_el_tot","Electron STs, Total Layers",11,0,11);
  TH1F h_fake_pix("h_fake_pix","(Mostly) Fake STs, Pixel Layers",2,2,4);
  TH1F h_fake_ext("h_fake_ext","(Mostly) Fake STs, Non-Pixel Layers",8,0,8);
  TH1F h_fake_tot("h_fake_tot","(Mostly) Fake STs, Total Layers",11,0,11);
  TH2F h_el_etaphi("h_el_etaphi","Electron STs, #eta and #phi",1200,-2.4,2.4,1200,-3.14,3.14);
  TH2F h_fake_etaphi("h_fake_etaphi","(Mostly) Fake STs, #eta and #phi",1200,-2.4,2.4,1200,-3.14,3.14);
  TH1F h_el_ratio("h_el_ratio","Gen e / ST p_T Ratio",15,0.5,2.0);

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


    if (!doIncl && t.nJet30 == 0) continue;

    if (doNj && t.nJet30 < 2) continue;

    if (doMET) {
      if (region != CRDY && ((t.ht < 1000 && t.met_pt < 250) || t.met_pt < 30) ) continue;
      // CRDY
      if (region == CRDY && ((t.zll_ht < 1000 && t.zll_met_pt < 250) || t.zll_met_pt < 30) ) continue;
    }
    else if (region != CRDY && doHT && t.ht < 100) continue;
    else if (region == CRDY && doHT && t.zll_ht < 100) continue;

    // SR and CRSL
    if (region != CRQCD && region != CRDY && doDphiDiff && (t.deltaPhiMin < 0.3 || t.diffMetMht / t.met_pt > 0.5) ) continue;
    // CRQCD
    if (region == CRQCD && doDphiDiff && (t.deltaPhiMin > 0.3 || t.diffMetMht / t.met_pt > 0.5) ) continue;
    // CRDY
    if (region == CRDY && doDphiDiff && (t.zll_deltaPhiMin < 0.3 || t.zll_diffMetMht / t.zll_met_pt > 0.5) ) continue;

    // Not CRDY
    if (region != CRDY && doMT2 && t.mt2 < 100) continue;
    // CRDY
    if (region == CRDY && doMT2 && t.zll_mt2 < 100) continue;

    // IMPORTANT:
    // We perform lepton checks last so that any event reaching this point is known to pass all other selections. So, if
    // short tracks force an adjustment to the lepton count, we know that that same adjustment would remove/add the event 
    // from/to the denominator as well. DO NOT reorder the checks without altering the logic. Leptons MUST BE LAST.

    // SR
    if ( (region == SR || region == CRQCD) && doLep && (t.nElectrons10 + t.nMuons10 + t.nPFLep5LowMT + t.nPFHad10LowMT > 0)) continue;
    // CRSL
    if (region == CRSL && doLep) {
      if (t.nLepLowMT != 1) continue;
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
	  break;
	}
      }
      // If the closest track is the short track, don't count this event as a short track event
      float smallestDR = 10; int i_best_matched_trk = -1;
      for (int itrk = 0; itrk < t.ntracks; itrk++) {
	float trk_eta = t.track_eta[itrk];
	float trk_phi = t.track_phi[itrk];
	float thisDR = DeltaR(lmt_eta,lmt_phi,trk_eta,trk_phi);
	if (thisDR < smallestDR) {
	  smallestDR = thisDR;
	  i_best_matched_trk = itrk;	      
	}
      }
      if (i_best_matched_trk == st_idx) {
	cout << "Disqualifed short track: Low MT lepton." << endl;
	continue; // Best match is the short track, disqualified!
      }

    }
    // CRDY
    if (region == CRDY && doLep) {
      if (t.nlep != 2) continue;
      // If short track is associated to lepton, it doesn't count
      bool matched_st = false;
      for (int ilep = 0; ilep < t.nlep && ! matched_st; ilep++) {
	float lep_phi = t.lep_phi[ilep];
	float lep_eta = t.lep_eta[ilep];
	float smallestDR = 10; int i_best_matched_trk = -1;
	for (int itrk = 0; itrk < t.ntracks; itrk++) {
	  float trk_eta = t.track_eta[itrk];
	  float trk_phi = t.track_phi[itrk];
	  float thisDR = DeltaR(lep_eta,lep_phi,trk_eta,trk_phi);
	  if (thisDR < smallestDR) {
	    smallestDR = thisDR;
	    i_best_matched_trk = itrk;	      
	  }
	}
	matched_st = i_best_matched_trk == st_idx; // Best match is short track, disqualified!
      }
      if (matched_st) {
	cout << "Disqualified short track: lepton" << endl;
	continue;
      }
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

    if (e_match) {
      h_el_pix.Fill(st_pix,w_);
      h_el_ext.Fill(st_ext,w_);
      h_el_tot.Fill(st_tot,w_);
      h_el_etaphi.Fill(st_eta,st_phi,w_);
    } else {
      h_fake_pix.Fill(st_pix,w_);
      h_fake_ext.Fill(st_ext,w_);
      h_fake_tot.Fill(st_tot,w_);
      h_fake_etaphi.Fill(st_eta,st_phi,w_);
    }

  }//end loop on events in a file
  
  h_unskimmed.SetBinContent(1,total_events);

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
  outfile_.Close();

  cout << "Wrote everything" << endl;
  
  return 0;
}

int main (int argc, char ** argv) {

  if (argc < 6) {
    cout << "Usage: ./ShortTrackType.exe <indir> <sample> <unskimmed_dir> <selection> <region>" << endl;
    return 1;
  }

  const int selection = strtol(argv[4],NULL,2);
  const int region = atoi(argv[5]);
  cout << "selection: " << selection << endl;
  cout << "region: " << region << endl;
  ShortTrackType * stt = new ShortTrackType();
  stt->loop(argv[1],argv[2],argv[3],selection,region);
  return 0;
}

ShortTrackType::ShortTrackType() {}

ShortTrackType::~ShortTrackType() {};

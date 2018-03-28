#include "ShortTrackType.h"

using namespace std;

class mt2tree;
class sttree;

const int maskHT = 1;
const int maskMET = 1<<1;
const int maskDphiDiff = 1<<2;
const int maskMT2 = 1<<3;

int ShortTrackType::loop (char * indir, char * sample, char * outdir, char * unskimmed_dir, int selection) {
  bool doNj = selection > 0;
  bool doHT = maskHT & selection;
  bool doMET = maskMET & selection;
  bool doDphiDiff = maskDphiDiff & selection;
  bool doMT2 = maskMT2 & selection;
  
  TString output_name = Form("%s/%s.root",outdir,sample);

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

  const int total_events = ch_us->GetEntries();
  cout << "Total events in unskimmed file: " << total_events << endl;
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
  TH2F h_el_etaphi("h_el_etaphi","Electron STs, #eta and #phi",12,-2.4,2.4,12,-3.14,3.14);
  TH2F h_fake_etaphi("h_fake_etaphi","(Mostly) Fake STs, #eta and #phi",12,-2.4,2.4,12,-3.14,3.14);
  TH1F h_el_ratio("h_el_ratio","Gen e / ST p_T Ratio",15,0.5,2.0);

  h_unskimmed.SetBinContent(1,total_events);

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

    if (doNj && t.nJet30 == 0) continue;

    if (doMET) {
      if (t.ht < 250 || (t.ht < 1000 && t.met_pt < 250) || t.met_pt < 30) continue;
    }
    else if (doHT && t.ht < 250) continue;

    if (doDphiDiff && (t.deltaPhiMin < 0.3 || t.diffMetMht / t.met_pt > 0.5) ) continue;

    if (doMT2 && t.mt2 < 200) continue;
    
    // Search for disappearing electron close to track.
    // Else assume track is fake since we don't have non-lep gen particles in babies.
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
    cout << "Usage: ./ShortTrackType.exe <indir> <sample> <outdir> <unskimmed_dir>" << endl;
    return 1;
  }

  const int selection = strtol(argv[5],NULL,2);
  cout << selection << endl;
  ShortTrackType * stt = new ShortTrackType();
  stt->loop(argv[1],argv[2],argv[3],argv[4],selection);
  return 0;
}

ShortTrackType::ShortTrackType() {}

ShortTrackType::~ShortTrackType() {};

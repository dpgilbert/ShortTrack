// C++
#include <iostream>
#include <cmath>
#include <sstream>

// ROOT
#include "TMath.h"
#include "TLorentzVector.h"
#include "TH1.h"
#include "TH2.h"

#include "../MT2CORE/mt2tree.h"
#include "../MT2CORE/sttree.h"

using namespace std;

class mt2tree;
class sttree;

int main (int argc, char ** argv) {

  if (argc < 4) {
    cout << "Usage: ./ShortTrackType.exe <input file> <sample> <outdir>" << endl;
    return 1;
  }

  TFile input( Form("%s/%s_incl_skim.root",argv[1],argv[2]) );
  
  TString output_name = Form("%s/%s.root",argv[3],argv[2]);
  cout << "[ShortTrackType::loop] creating output file: " << output_name << endl;

  TFile * outfile_ = TFile::Open(output_name.Data(),"RECREATE") ; 

  cout << "[ShortTrackType::loop] running on file: " << input.GetTitle() << endl;

  // Get File Content
  TTree *tree_mt2 = (TTree*)input.Get("mt2st");
  TTree *tree_st = (TTree*)input.Get("st");
    
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
  if (nEventsTree != tree_st->GetEntriesFast()) {
    cout << "st and mt2 trees have different numbers of entries. This should not be possible if the input file was produced by treefriend.py and never edited. Aborting..." << endl;
    return 2;
  }

  // Book Histograms
  TH1F h_el_pix("h_el_pix","Electron STs, Pixel Layers",2,2,4);
  TH1F h_el_ext("h_el_ext","Electron STs, Non-Pixel Layers",8,0,8);
  TH1F h_el_tot("h_el_tot","Electron STs, Total Layers",11,0,11);
  TH1F h_fake_pix("h_fake_pix","(Mostly) Fake STs, Pixel Layers",2,2,4);
  TH1F h_fake_ext("h_fake_ext","(Mostly) Fake STs, Non-Pixel Layers",8,0,8);
  TH1F h_fake_tot("h_fake_tot","(Mostly) Fake STs, Total Layers",11,0,11);
  TH2F h_el_etaphi("h_el_etaphi","Electron STs, #eta and #phi",12,-2.4,2.4,12,-3.14,3.14);
  TH2F h_fake_etaphi("h_fake_etaphi","(Mostly) Fake STs, #eta and #phi",12,-2.4,2.4,12,-3.14,3.14);
  TH1F h_el_ratio("h_el_ratio","Gen e / ST p_T Ratio",15,0.5,2.0);

  for( unsigned int event = 0; event < nEventsTree; ++event) {
    
    cout << "Processing event " << event << endl;

    // AddFriend above makes this pull both mt2 and st branches for the same event, assuming the trees were properly (identically) sorted. 
    t.GetEntry(event); 
    
    const float w_ = t.evt_xsec * 1000 / nEventsTree;
    
    //---------------------
    // bookkeeping and progress report
    //---------------------
    if (event%10000==0) {
      ULong64_t i_permille = (int)floor(1000 * event / float(nEventsTree));
      if (isatty(1)) {
	printf("\015\033[32m ---> \033[1m\033[31m%4.1f%%"
	       "\033[0m\033[32m <---\033[0m\015", i_permille/10.);
	fflush(stdout);
      }
      else {
	cout<<i_permille/10.<<" ";
	if (event % 100000==0) cout<<endl;
      }
    }
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
    
    // Search for disappearing electron close to track.
    // Else assume track is fake since we don't have non-lep gen particles in babies.
    int st_idx = -1;
    float st_phi = -999;
    float st_eta = -999;
    float st_pt = -1;
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
      continue;
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
  
    cout << "About to delete trees for file " << input.GetTitle() << endl;
    delete tree_st;
    delete tree_mt2;
    input.Close();

    outfile_->cd();
    h_el_pix.Write();
    h_el_ext.Write();
    h_el_tot.Write();
    h_el_etaphi.Write();
    h_el_ratio.Write();
    h_fake_pix.Write();
    h_fake_ext.Write();
    h_fake_tot.Write();
    h_fake_etaphi.Write();
    outfile_->Close();
    
    return 0;
}


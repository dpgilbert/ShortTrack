#include "ToyLooper.h"

using namespace std;

class mt2tree;
class sttree;

int ToyLooper::loop (char * indir, char * sample) {
  string mkdir = "mkdir -p output";
  system(mkdir.c_str());
  TString output_name = Form("output/%s.root",sample);

  // Containers for our trees
  TChain* ch_mt2 = new TChain("mt2st");
  TChain* ch_st = new TChain("st");

  TString infile_name = Form("%s/%s.root",indir,sample);
  cout << "Input file: " << infile_name.Data() << endl;
  ch_mt2->Add( infile_name ); // Load mt2 inputs
  ch_st->Add( infile_name ); // Load short track inputs

  // Get file Content
  TTree *tree_mt2 = (TTree*)ch_mt2->Clone("mt2st");
  TTree *tree_st = (TTree*)ch_st->Clone("st");

  // sttree is a subtype of mt2tree. mt2tree functions can be called on an sttree, but not the reverse.
  // This line incorporates branches from tree_mt2 into tree_st.
  tree_st->AddFriend(tree_mt2);

  sttree t; // See MT2CORE/sttree.h

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
  TH1::SetDefaultSumw2(true); // Makes histograms do proper error calculation automatically
  //  TH1F name_in_cpp("name_in_output_root_file","Title of Histogram",number_of_bins,lower_limit,upper_limit);
  TH1F h_example("h_example","An Example Histogram",1,0,1);

  for( unsigned int event = 0; event < nEventsTree; ++event) {
    
    // AddFriend above makes this pull both mt2 and st branches for the same event, assuming the trees were properly (identically) sorted. 
    t.GetEntry(event); 
    
    //    const float w_ = t.evt_xsec * 1000 / nEventsTree; // weight events to 1 fb^-1 luminosity
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

    // Analysis code

    float thing_you_want_to_fill = 0;
    h_example.Fill( thing_you_want_to_fill , w_);

  }//end loop on events in a file

  cout << "About to write" << endl;

  TFile outfile_(output_name,"RECREATE"); 
  h_example.Write();

  outfile_.Close();

  cout << "Wrote everything" << endl;
  
  return 0;
}

int main (int argc, char ** argv) {

  if (argc < 3) {
    cout << "Usage: ./ToyLooper.exe <indir> <sample>" << endl;
    return 1;
  }

  ToyLooper * tl = new ToyLooper();
  tl->loop(argv[1],argv[2]);
  return 0;
}

ToyLooper::ToyLooper() {}

ToyLooper::~ToyLooper() {};

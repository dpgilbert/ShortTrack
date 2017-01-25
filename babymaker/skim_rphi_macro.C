#include <fstream>
#include <sstream>
#include <iostream>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>

using namespace std;

void skim_rphi_macro(string inpath = "./", string outpath = "./", string intag = "output", string outtag = "skim_rphi") {
  
  //--------------------------------------------------
  // cut for output files
  //--------------------------------------------------

  // cuts from: skim_base_mt2_Zinv_v6_JECs  
  const char* sel = "mt2>50 && ht>250 && nlep==0 && diffMetMht/met_pt<0.5";

  //cout << "Skimming with selection : "<<sel<<endl;

  //--------------------------------------------------
  // input and output file
  //--------------------------------------------------
  
  const char* infilename = Form("%s/%s*.root",inpath.c_str(),intag.c_str());
  const char* outfilename = Form("%s/%s.root",outpath.c_str(),outtag.c_str());
  
  //--------------------------------------------------
  // cout stuff
  //--------------------------------------------------
  
  cout << "Reading in : " << infilename << endl;
  cout << "Writing to : " << outfilename << endl;
  cout << "Selection : " << sel << endl;
  
  //--------------------------------------------------
  // read input file, write to output files
  //--------------------------------------------------
  
  //long long max_tree_size = 5000000000LL; // 5GB
  //long long max_tree_size = 2500000000LL; // 2.5GB
  //TTree::SetMaxTreeSize(max_tree_size);
  
  TChain *chain = new TChain("mt2");
  chain->Add(infilename);

  unsigned int input_entries = chain->GetEntries();
  cout << "Input tree has entries: " << input_entries << endl;
  if (input_entries == 0) {
    cout << "ERROR: no entries in input tree, exiting.." << endl;
    return;
  }
  
  //-------------------
  // skim
  //-------------------
  
  TFile *outfile = TFile::Open(outfilename, "RECREATE");
  assert( outfile != 0 );
  TTree* outtree = chain->CopyTree( sel );
  unsigned int output_entries = outtree->GetEntries();
  cout << "Output tree has entries: " << output_entries << endl
       << "Reduction factor of: " << double(input_entries)/double(output_entries) << endl;
  outtree->Write();
  outfile->Close();

}

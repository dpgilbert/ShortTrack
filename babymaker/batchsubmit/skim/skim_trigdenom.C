#include <fstream>
#include <sstream>
#include <iostream>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>
#include <TString.h>
#include <TCut.h>

using namespace std;

void skim_trigdenom(string inpath = "/nfs-6/userdata/mt2/V00-00-03", string outpath = "/nfs-6/userdata/mt2/V00-00-03_skim_nj2_ht450_mt2gt50", string tag = "ttall_msdecays") {
  
  //--------------------------------------------------
  // cut for output files
  //--------------------------------------------------
 
  TCut sel;
  if (tag.find("JetHT") != std::string::npos) sel = "((gamma_pt[0]>150 && gamma_chHadIso[0]<20.) || (HLT_PFHT475_Prescale || HLT_PFHT600_Prescale))";
  // else if (tag.find("SingleElectron") != std::string::npos) sel = "(nJet30 > 1 && ht > 200. && met_pt > 100.)";
  // else if (tag.find("MET") != std::string::npos) sel = "(nJet30 > 1 && met_pt > 250.)";
  else if (tag.find("SingleElectron") != std::string::npos) sel = "(nJet30 > 0 && (ht > 200. || met_pt > 100.))";
  else if (tag.find("MET") != std::string::npos) sel = "(nJet30 > 0 && met_pt > 250.)";
  else {
    cout << "no skim defined for sample: " << tag << ", exiting" << endl;
    return;
  }

  cout << "Skimming with selection : "<<sel.GetTitle()<<endl;

  //--------------------------------------------------
  // input and output file
  //--------------------------------------------------
  
  const char* infilename = Form("%s/%s*.root",inpath.c_str(),tag.c_str());
  const char* outfilename = Form("%s/%s.root",outpath.c_str(),tag.c_str());
  
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
  long long max_tree_size = 2500000000LL; // 2.5GB
  TTree::SetMaxTreeSize(max_tree_size);
  
  TChain *chain = new TChain("mt2");
  chain->Add(infilename);

  unsigned int input_entries = chain->GetEntries();
  cout << "Input tree has entries: " << input_entries << endl;
  
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

#include <fstream>
#include <sstream>
#include <iostream>
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TChain.h>

using namespace std;

void print_nevents(string inpath = "/nfs-6/userdata/mt2/V00-00-03", string tag = "ttall_msdecays") {
  
  //--------------------------------------------------
  // input and output file
  //--------------------------------------------------
  
  const char* infilename = Form("%s/%s*.root",inpath.c_str(),tag.c_str());
  
  //--------------------------------------------------
  // cout stuff
  //--------------------------------------------------
  
  cout << "Reading in : " << infilename << endl;
  
  TChain *chain = new TChain("t");
  chain->Add(infilename);

  int input_entries = (int) chain->GetEntries();
  cout << "Input tree has entries: " << input_entries << endl;

  ULong64_t nEvts(0);
  Float_t xsec(0.);
  Float_t kfactor(0.);
  Float_t filter(0.);
  Float_t scale1fb(0.);

  chain->SetBranchAddress("evt_nEvts",&nEvts);
  chain->SetBranchAddress("evt_xsec",&xsec);
  chain->SetBranchAddress("evt_kfactor",&kfactor);
  chain->SetBranchAddress("evt_filter",&filter);
  chain->SetBranchAddress("evt_scale1fb",&scale1fb);
  chain->GetEntry(0);

  float scale1fb_recalc = xsec*kfactor*filter*1000/double(input_entries);

  if (nEvts != input_entries) {
    cout << "!! WARNING: branch evt_nEvts doesn't agree with GetEntries()" << endl;
  }
  cout << "-- Branch values: " << endl
       << "   nEvts: " << nEvts << endl
       << "   xsec: " << xsec << endl
       << "   kfactor: " << kfactor << endl
       << "   filter: " << filter << endl
       << "   scale1fb: " << scale1fb << endl
       << "   1/scale1fb: " << 1./scale1fb << endl
       << "   scale1fb recalculated: " << scale1fb_recalc << endl
       << endl;

  unsigned int badevts = chain->GetEntries("evt_nEvts == 0");

  if (badevts > 0) cout << "!! WARNING: " << badevts << " events have evt_nEvts == 0" << endl;

  //  cout << tag << ": " << nEvts << ", " << scale1fb << endl;
  
}

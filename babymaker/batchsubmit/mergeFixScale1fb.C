#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include "TFile.h"
#include "TTree.h"
#include "TBranch.h"
#include "TString.h"
#include "TChain.h"
#include "TBenchmark.h"

using namespace std;

// merges a sample and corrects scale1fb for the number of events in the merged sample
// - can be used to merge ext stats versions of samples
// - does NOT take into account negative weights for NLO samples!!
int mergeFixScale1fb(const TString& indir, const TString& sample, const TString& outdir){
  
  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");
  
  cout << "Input directory: " << indir << endl
       << "Input sample: " << sample << endl
       << "Output directory: " << outdir << endl;

  TString infiles = Form("%s/%s*.root",indir.Data(),sample.Data());
  TString outfile = Form("%s/%s.root",outdir.Data(),sample.Data());
  
  TChain *chain = new TChain("mt2");
  chain->Add(infiles);

  ULong64_t nevents_input = chain->GetEntries();
  
  if (nevents_input == 0) {
    cout << "ERROR: no events in input chain! aborting.." << endl;
    return 1;
  }

  float xsec_input_max = chain->GetMaximum("evt_xsec");
  float xsec_input_min = chain->GetMinimum("evt_xsec");
  if (fabs(xsec_input_max - xsec_input_min) > 1e-4 * xsec_input_max) {
    cout << "ERROR: xsec varies across input trees: max value: " << xsec_input_max
	 << ", min value: " << xsec_input_min << ", aborting.." << endl;
    return 2;
  }
  float kfactor_input = chain->GetMaximum("evt_kfactor");
  float filter_input = chain->GetMaximum("evt_filter");

  Float_t scale1fb_input = xsec_input_max * kfactor_input * filter_input * 1000. / float(nevents_input);

  cout << "input events: " << nevents_input << endl
       << "xsec*kfactor*filter: " << xsec_input_max * kfactor_input * filter_input << endl
       << "new scale1fb: " << scale1fb_input << endl;

  //-------------------------------------------------------------
  // Removes branches to be replaced
  //-------------------------------------------------------------`
  chain->SetBranchStatus("*", 1);
  chain->SetBranchStatus("evt_scale1fb", 0);
  chain->SetBranchStatus("evt_nEvts", 0);

  TFile *out = TFile::Open(outfile.Data(), "RECREATE");
  TTree *clone;
  clone = chain->CloneTree(-1, "fast");

  cout << "Merging to file: " << outfile << endl;

  //-------------------------------------------------------------

  // "new" branches to overwrite old ones
  TBranch* b_evt_scale1fb = clone->Branch("evt_scale1fb", &scale1fb_input, "evt_scale1fb/F");
  TBranch* b_evt_nEvts = clone->Branch("evt_nEvts", &nevents_input, "evt_nEvts/l");
   
  for(ULong64_t i = 0; i < nevents_input; ++i) {
    clone->GetEntry(i);

    // fill new branches
    b_evt_scale1fb->Fill();
    b_evt_nEvts->Fill();
  }
  //-------------------------------------------------------------

  clone->SetMaxTreeSize(5000000000LL); // ~5 GB
  clone->Write(); 
  out->Close();

  cout << "Processed events: " << nevents_input << endl;
  bmark->Stop("benchmark");
  cout << "------------------------------" << endl;
  cout << "CPU  Time:	" << Form( "%.01f s", bmark->GetCpuTime("benchmark")  ) << endl;
  cout << "Real Time:	" << Form( "%.01f s", bmark->GetRealTime("benchmark") ) << endl;
  cout << endl;
  delete bmark;

  return 0;
}







#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"

#include <iostream>
#include <fstream>
#include <vector>

void printLimit(std::string sample = "T1tttt_1500_100", std::string version = "LowLumi_GAMMA"){

  TFile *limit_file = new TFile(Form("higgsCombine_%s.HybridNew.mH120.quant0.500.root", sample.c_str()), "READ");
  TTree *limit_tree = (TTree*)limit_file->Get("limit");

  double value = -1.0;
  limit_tree->SetBranchAddress("limit", &value);
  limit_tree->GetEntry(0);
  
  std::cout << "limit = " << value << std::endl;

  ofstream outfile;
  outfile.open (Form("limits_%s.txt", version.c_str()), std::ios::app);

  outfile << sample << ": " << value << std::endl;

}

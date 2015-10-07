#include "TFile.h"
#include "TChain.h"
#include "TH2.h"
#include "TString.h"

void make_scan_nsig(TString dir, TString sample) {

  TChain* t = new TChain("mt2");
  t->Add(Form("%s/%s*.root",dir.Data(),sample.Data()));

  TFile* fout = new TFile(Form("nsig_%s.root",sample.Data()),"RECREATE");
  
  // assuming here: 25 GeV binning, m1 from 400-2000, m2 from 0-1600
  TH2D* h_nsig = new TH2D("h_nsig",";mass1 [GeV];mass2 [GeV]", 65, 387.5, 2012.5, 65, -12.5, 1612.5);
  t->Draw("GenSusyMScan2:GenSusyMScan1>>h_nsig");

  fout->Write();
  fout->Close();

}

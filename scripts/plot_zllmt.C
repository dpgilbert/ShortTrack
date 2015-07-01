#include "TFile.h"
#include "TChain.h"
#include "TString.h"
#include "TH1.h"
#include "TDirectory.h"
#include "TCut.h"

void plot_zllmt(TString indir = "/nfs-6/userdata/mt2/V00-00-12_zllmt/", TString sample = "dyjetsll_ht") {

  // inputs
  TChain* mt2 = new TChain("mt2");
  mt2->Add(indir+"/"+sample+"*.root");

  // cuts
  TCut basecuts = "(nVert > 0) && (nJet40 > 1) && zllmt_ht > 450. && nBJet20 == 0";
  TCut lepcuts = "nlep == 2 && lep_pt[0] > 20. && lep_pt[1] > 20. && (lep_charge[0] * lep_charge[1] == -1) && (abs(lep_pdgId[0]) == abs(lep_pdgId[1])) && (abs(zll_mass - 90) < 20) ";
  TCut weight = "evt_scale1fb";

  // output file
  TFile* fout = new TFile(sample+".root","RECREATE");
  fout->cd();
  TDirectory* dir = fout->mkdir("crmtbase");
  dir->cd();

  // define and fill plots
  TH1D* h_zllmt = new TH1D("h_zllmt",";Transformed M_{T} [GeV]",250,0,500);
  TH1D* h_zllmt_mt2gt50 = (TH1D*) h_zllmt->Clone("h_zllmt_mt2gt50");
  TH1D* h_zllmt_mt2gt100 = (TH1D*) h_zllmt->Clone("h_zllmt_mt2gt100");
  TH1D* h_zllmt_mt2gt150 = (TH1D*) h_zllmt->Clone("h_zllmt_mt2gt150");
  TH1D* h_zllmt_mt2gt200 = (TH1D*) h_zllmt->Clone("h_zllmt_mt2gt200");

  mt2->Draw("zllmt_mt>>h_zllmt",(basecuts+lepcuts)*weight);
  mt2->Draw("zllmt_mt>>h_zllmt_mt2gt50",(basecuts+lepcuts+"zllmt_mt2 > 50.")*weight);
  mt2->Draw("zllmt_mt>>h_zllmt_mt2gt100",(basecuts+lepcuts+"zllmt_mt2 > 100.")*weight);
  mt2->Draw("zllmt_mt>>h_zllmt_mt2gt150",(basecuts+lepcuts+"zllmt_mt2 > 150.")*weight);
  mt2->Draw("zllmt_mt>>h_zllmt_mt2gt200",(basecuts+lepcuts+"zllmt_mt2 > 200.")*weight);

  h_zllmt->Write();
  h_zllmt_mt2gt50->Write();
  h_zllmt_mt2gt100->Write();
  h_zllmt_mt2gt150->Write();
  h_zllmt_mt2gt200->Write();

  fout->Close();

  
}

#include "TFile.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TString.h"

void make_nsig_weight_hists(TString dir, TString sample) {

  TChain* t = new TChain("mt2");
  t->Add(Form("%s/%s*.root",dir.Data(),sample.Data()));

  TFile* fout = new TFile(Form("nsig_weights_%s.root",sample.Data()),"RECREATE");
  
  // assuming here: 25 GeV binning, m1 from 0-2000, m2 from 0-1600
  TH2D* h_nsig = new TH2D("h_nsig",";mass1 [GeV];mass2 [GeV]", 81, -12.5, 2012.5, 65, -12.5, 1612.5);
  TH2D* h_avg_weight_btagsf = (TH2D*) h_nsig->Clone("h_avg_weight_btagsf");
  TH2D* h_avg_weight_btagsf_heavy_UP = (TH2D*) h_nsig->Clone("h_avg_weight_btagsf_heavy_UP");
  TH2D* h_avg_weight_btagsf_light_UP = (TH2D*) h_nsig->Clone("h_avg_weight_btagsf_light_UP");
  TH2D* h_avg_weight_btagsf_heavy_DN = (TH2D*) h_nsig->Clone("h_avg_weight_btagsf_heavy_DN");
  TH2D* h_avg_weight_btagsf_light_DN = (TH2D*) h_nsig->Clone("h_avg_weight_btagsf_light_DN");
  TH2D* h_avg_weight_isr = (TH2D*) h_nsig->Clone("h_avg_weight_isr");

  t->Draw("GenSusyMScan2:GenSusyMScan1>>h_nsig");
  t->Draw("GenSusyMScan2:GenSusyMScan1>>h_avg_weight_btagsf","(met_pt > -1.)*weight_btagsf" );
  t->Draw("GenSusyMScan2:GenSusyMScan1>>h_avg_weight_btagsf_heavy_UP","(met_pt > -1.)*weight_btagsf_heavy_UP" );
  t->Draw("GenSusyMScan2:GenSusyMScan1>>h_avg_weight_btagsf_light_UP","(met_pt > -1.)*weight_btagsf_light_UP" );
  t->Draw("GenSusyMScan2:GenSusyMScan1>>h_avg_weight_btagsf_heavy_DN","(met_pt > -1.)*weight_btagsf_heavy_DN" );
  t->Draw("GenSusyMScan2:GenSusyMScan1>>h_avg_weight_btagsf_light_DN","(met_pt > -1.)*weight_btagsf_light_DN" );
  t->Draw("GenSusyMScan2:GenSusyMScan1>>h_avg_weight_isr","(met_pt > -1.)*weight_isr" );

  h_avg_weight_btagsf->Divide(h_nsig);
  h_avg_weight_btagsf_heavy_UP->Divide(h_nsig);
  h_avg_weight_btagsf_light_UP->Divide(h_nsig);
  h_avg_weight_btagsf_heavy_DN->Divide(h_nsig);
  h_avg_weight_btagsf_light_DN->Divide(h_nsig);
  h_avg_weight_isr->Divide(h_nsig);
  
  fout->Write();
  fout->Close();

}

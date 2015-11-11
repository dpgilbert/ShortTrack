#include "TFile.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TLegend.h"
#include "TString.h"
#include "TCanvas.h"
#include "TCut.h"
#include "TEfficiency.h"

void plot_trigeff_Photon165_HE10 (const TString& indir = "/nfs-6/userdata/mt2/V00-01-05_25ns_json_246908-258159_v3_Summer15_25nsV5_phot120/") {

  TH1::SetDefaultSumw2();
  
  TChain* t_phot = new TChain("mt2");

  t_phot->Add(Form("%s/*Run2015*SinglePhoton*.root", indir.Data()));

  TFile* f_out = new TFile("trigeff_Photon165_HE10.root","RECREATE");
  
  TH1D* h_pt_denom_phot120 = new TH1D("h_pt_denom_phot120",";photon p_{T} [GeV]",40,150,350);
  TH1D* h_pt_denom_eb_phot120 = (TH1D*) h_pt_denom_phot120->Clone("h_pt_denom_eb_phot120");
  TH1D* h_pt_denom_ee_phot120 = (TH1D*) h_pt_denom_phot120->Clone("h_pt_denom_ee_phot120");
  TH1D* h_pt_num_phot120 = (TH1D*) h_pt_denom_phot120->Clone("h_pt_num_phot120");
  TH1D* h_pt_num_eb_phot120 = (TH1D*) h_pt_denom_phot120->Clone("h_pt_num_eb_phot120");
  TH1D* h_pt_num_ee_phot120 = (TH1D*) h_pt_denom_phot120->Clone("h_pt_num_ee_phot120");

  TCanvas* c = new TCanvas("c","c");
  c->SetGrid(1,1);
  c->cd();

  TCut base = "nVert > 0 && ngamma > 0 && gamma_idCutBased[0] == 1 && gamma_chHadIso[0] < 2.5";
  TCut eb = base+"abs(gamma_eta[0]) < 1.4";
  TCut ee = base+"abs(gamma_eta[0]) > 1.6";

  t_phot->Draw("gamma_pt[0]>>h_pt_denom_phot120",base+"HLT_Photon120");
  t_phot->Draw("gamma_pt[0]>>h_pt_num_phot120",base+"HLT_Photon120 && HLT_Photon165_HE10");

  t_phot->Draw("gamma_pt[0]>>h_pt_denom_eb_phot120",base+eb+"HLT_Photon120");
  t_phot->Draw("gamma_pt[0]>>h_pt_num_eb_phot120",base+eb+"HLT_Photon120 && HLT_Photon165_HE10");

  t_phot->Draw("gamma_pt[0]>>h_pt_denom_ee_phot120",base+ee+"HLT_Photon120");
  t_phot->Draw("gamma_pt[0]>>h_pt_num_ee_phot120",base+ee+"HLT_Photon120 && HLT_Photon165_HE10");

  TH2F* h_axis = new TH2F("h_axis",";photon p_{T} [GeV];Efficiency of HLT_Photon165_HE10",40,150,350,20,0,1);
  h_axis->Draw();
  
  TEfficiency* h_pt_eff_phot120 = new TEfficiency(*h_pt_num_phot120, *h_pt_denom_phot120);
  h_pt_eff_phot120->SetLineColor(kBlue);
  h_pt_eff_phot120->SetMarkerColor(kBlue);

  h_pt_eff_phot120->Draw("pe same");

  TLegend* leg = new TLegend(0.6,0.2,0.9,0.5);
  leg->AddEntry(h_pt_eff_phot120,"HLT_Photon120","pe");
  leg->Draw("same");

  c->SaveAs("trigeff_Photon165_HE10.pdf");
  c->SaveAs("trigeff_Photon165_HE10.eps");

  // split into EB and EE
  
  TCanvas* c_ebee = new TCanvas("c_ebee","c_ebee");
  c_ebee->SetGrid(1,1);
  c_ebee->cd();

  h_axis->Draw();

  TEfficiency* h_pt_eff_eb_phot120 = new TEfficiency(*h_pt_num_eb_phot120, *h_pt_denom_eb_phot120);
  h_pt_eff_eb_phot120->SetLineColor(kGreen+2);
  h_pt_eff_eb_phot120->SetMarkerColor(kGreen+2);

  TEfficiency* h_pt_eff_ee_phot120 = new TEfficiency(*h_pt_num_ee_phot120, *h_pt_denom_ee_phot120);
  h_pt_eff_ee_phot120->SetLineColor(kMagenta);
  h_pt_eff_ee_phot120->SetMarkerColor(kMagenta);

  h_pt_eff_eb_phot120->Draw("pe same");
  h_pt_eff_ee_phot120->Draw("pe same");

  TLegend* leg_ebee = new TLegend(0.6,0.2,0.9,0.5);
  leg_ebee->AddEntry(h_pt_eff_eb_phot120,"HLT_Photon120, EB only","pe");
  leg_ebee->AddEntry(h_pt_eff_ee_phot120,"HLT_Photon120, EE only","pe");
  leg_ebee->Draw("same");

  c_ebee->SaveAs("trigeff_Photon165_HE10_EBEE.pdf");
  c_ebee->SaveAs("trigeff_Photon165_HE10_EBEE.eps");

  f_out->Write();
  f_out->Close();
  
  return;
}

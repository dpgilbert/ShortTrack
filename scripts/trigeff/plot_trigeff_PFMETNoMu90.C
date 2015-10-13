#include "TFile.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TLegend.h"
#include "TString.h"
#include "TCanvas.h"
#include "TCut.h"
#include "TEfficiency.h"

void plot_trigeff_PFMETNoMu90 (const TString& indir = "/nfs-6/userdata/mt2/V00-01-05_25ns_json_246908-258159_v3_Summer15_25nsV5_met90/") {

  TH1::SetDefaultSumw2();
  
  TChain* t_jetht = new TChain("mt2");
  TChain* t_muon = new TChain("mt2");
  TChain* t_ele = new TChain("mt2");

  t_jetht->Add(Form("%s/*Run2015*JetHT*.root", indir.Data()));
  t_muon->Add(Form("%s/*Run2015*SingleMuon*.root", indir.Data()));
  t_ele->Add(Form("%s/*Run2015*SingleElectron*.root", indir.Data()));

  TFile* f_out = new TFile("trigeff_PFMETNoMu90.root","RECREATE");
  
  TH1D* h_met_denom_ht800 = new TH1D("h_met_denom_ht800",";E_{T}^{miss} [GeV]",30,100,400);
  TH1D* h_met_num_ht800 = (TH1D*) h_met_denom_ht800->Clone("h_met_num_ht800");
  TH1D* h_met_denom_mu20 = (TH1D*) h_met_denom_ht800->Clone("h_met_denom_mu20");
  TH1D* h_met_num_mu20 = (TH1D*) h_met_denom_ht800->Clone("h_met_num_mu20");
  TH1D* h_met_denom_ele23 = (TH1D*) h_met_denom_ht800->Clone("h_met_denom_ele23");
  TH1D* h_met_num_ele23 = (TH1D*) h_met_denom_ht800->Clone("h_met_num_ele23");

  TCanvas* c = new TCanvas("c","c");
  c->SetGrid(1,1);
  c->cd();

  TCut base = "nVert > 0 && nJet30 > 1 && Flag_CSCTightHaloFilter && Flag_eeBadScFilter && Flag_HBHENoiseFilter && Flag_HBHEIsoNoiseFilter";
  TCut had = base + "nElectrons10+nMuons10==0 && ht > 1000. && deltaPhiMin > 0.3";
  TCut ele = base + "nElectrons10 > 0 && abs(lep_pdgId[0]) == 11 && lep_pt[0] > 25.";
  TCut mu = base + "nMuons10 > 0 && abs(lep_pdgId[0]) == 13 && lep_pt[0] > 25.";

  t_jetht->Draw("met_pt>>h_met_denom_ht800",had+"HLT_PFHT800");
  t_jetht->Draw("met_pt>>h_met_num_ht800",had+"HLT_PFHT800 && HLT_PFMETNoMu90");

  t_muon->Draw("met_pt>>h_met_denom_mu20",mu+"HLT_SingleMu");
  t_muon->Draw("met_pt>>h_met_num_mu20",mu+"HLT_SingleMu && HLT_PFMETNoMu90");

  t_ele->Draw("met_pt>>h_met_denom_ele23",ele+"HLT_SingleEl");
  t_ele->Draw("met_pt>>h_met_num_ele23",ele+"HLT_SingleEl && HLT_PFMETNoMu90");

  TH2F* h_axis = new TH2F("h_axis",";E_{T}^{miss} [GeV];Efficiency of HLT_PFMETNoMu90_etc",30,100,400,20,0,1);
  h_axis->Draw();
  
  TEfficiency* h_met_eff_ht800 = new TEfficiency(*h_met_num_ht800, *h_met_denom_ht800);
  h_met_eff_ht800->SetLineColor(kRed);
  h_met_eff_ht800->SetMarkerColor(kRed);
  
  TEfficiency* h_met_eff_mu20 = new TEfficiency(*h_met_num_mu20, *h_met_denom_mu20);
  h_met_eff_mu20->SetLineColor(kBlue);
  h_met_eff_mu20->SetMarkerColor(kBlue);

  TEfficiency* h_met_eff_ele23 = new TEfficiency(*h_met_num_ele23, *h_met_denom_ele23);
  h_met_eff_ele23->SetLineColor(kGreen+2);
  h_met_eff_ele23->SetMarkerColor(kGreen+2);

  //h_met_eff_ht800->Draw("pe same");
  h_met_eff_mu20->Draw("pe same");
  h_met_eff_ele23->Draw("pe same");

  TLegend* leg = new TLegend(0.55,0.15,0.95,0.45);
  //leg->AddEntry(h_met_eff_ht800,"HLT_PFHT800","pe");
  leg->AddEntry(h_met_eff_mu20,"HLT_IsoMu20 || HLT_IsoTkMu20","pe");
  leg->AddEntry(h_met_eff_ele23,"HLT_Ele23_WPLoose_Gsf","pe");
  leg->Draw("same");

  c->SaveAs("trigeff_PFMETNoMu90.pdf");
  c->SaveAs("trigeff_PFMETNoMu90.eps");

  f_out->Write();
  f_out->Close();
  
  return;
}

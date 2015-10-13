#include "TFile.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TLegend.h"
#include "TString.h"
#include "TCanvas.h"
#include "TCut.h"
#include "TEfficiency.h"

void plot_trigeff_PFHT800 (const TString& indir = "/nfs-6/userdata/mt2/V00-01-05_25ns_json_246908-258159_v3_Summer15_25nsV5/") {

  TH1::SetDefaultSumw2();
  
  TChain* t_jetht = new TChain("mt2");
  TChain* t_met = new TChain("mt2");
  TChain* t_ele = new TChain("mt2");

  t_jetht->Add(Form("%s/*Run2015*JetHT*.root", indir.Data()));
  t_met->Add(Form("%s/*Run2015*MET*.root", indir.Data()));
  t_ele->Add(Form("%s/*Run2015*SingleElectron*.root", indir.Data()));

  TFile* f_out = new TFile("trigeff_PFHT800.root","RECREATE");
  
  TH1D* h_ht_denom_ht475 = new TH1D("h_ht_denom_ht475",";H_{T} [GeV]",28,600,1300);
  TH1D* h_ht_num_ht475 = (TH1D*) h_ht_denom_ht475->Clone("h_ht_num_ht475");
  TH1D* h_ht_denom_met170 = (TH1D*) h_ht_denom_ht475->Clone("h_ht_denom_met170");
  TH1D* h_ht_num_met170 = (TH1D*) h_ht_denom_ht475->Clone("h_ht_num_met170");
  TH1D* h_ht_denom_ele23 = (TH1D*) h_ht_denom_ht475->Clone("h_ht_denom_ele23");
  TH1D* h_ht_num_ele23 = (TH1D*) h_ht_denom_ht475->Clone("h_ht_num_ele23");

  TCanvas* c = new TCanvas("c","c");
  c->SetGrid(1,1);
  c->cd();

  TCut base = "nVert > 0 && nJet30 > 1 && Flag_CSCTightHaloFilter && Flag_eeBadScFilter && Flag_HBHENoiseFilter";
  TCut had = base + "nElectrons10+nMuons10==0";
  TCut ele = base + "nElectrons10 > 0 && abs(lep_pdgId[0]) == 11 && lep_pt[0] > 25.";

  t_jetht->Draw("ht>>h_ht_denom_ht475",had+"HLT_PFHT475_Prescale");
  t_jetht->Draw("ht>>h_ht_num_ht475",had+"HLT_PFHT475_Prescale && HLT_PFHT800");

  t_met->Draw("ht>>h_ht_denom_met170",had+"met_pt > 200. && HLT_PFMET170");
  t_met->Draw("ht>>h_ht_num_met170",had+"met_pt > 200. && HLT_PFMET170 && HLT_PFHT800");

  t_ele->Draw("ht>>h_ht_denom_ele23",ele+"HLT_SingleEl");
  t_ele->Draw("ht>>h_ht_num_ele23",ele+"HLT_SingleEl && HLT_PFHT800");

  TH2F* h_axis = new TH2F("h_axis",";H_{T} [GeV];Efficiency of HLT_PFHT800",14,600,1300,20,0,1);
  h_axis->Draw();
  
  TEfficiency* h_ht_eff_ht475 = new TEfficiency(*h_ht_num_ht475, *h_ht_denom_ht475);
  h_ht_eff_ht475->SetLineColor(kRed);
  h_ht_eff_ht475->SetMarkerColor(kRed);
  //h_ht_eff_ht475->SetMarkerStyle(7);
  
  TEfficiency* h_ht_eff_met170 = new TEfficiency(*h_ht_num_met170, *h_ht_denom_met170);
  h_ht_eff_met170->SetLineColor(kBlue);
  h_ht_eff_met170->SetMarkerColor(kBlue);

  TEfficiency* h_ht_eff_ele23 = new TEfficiency(*h_ht_num_ele23, *h_ht_denom_ele23);
  h_ht_eff_ele23->SetLineColor(kGreen+2);
  h_ht_eff_ele23->SetMarkerColor(kGreen+2);

  h_ht_eff_met170->Draw("pe same");
  h_ht_eff_ht475->Draw("pe same");
  //  h_ht_eff_ele23->Draw("pe same");

  TLegend* leg = new TLegend(0.6,0.2,0.9,0.5);
  leg->AddEntry(h_ht_eff_met170,"HLT_PFMET170","pe");
  leg->AddEntry(h_ht_eff_ht475,"HLT_PFHT475","pe");
  //  leg->AddEntry(h_ht_eff_ele23,"HLT_Ele23_WPLoose_Gsf","pe");
  leg->Draw("same");

  c->SaveAs("trigeff_PFHT800.pdf");
  c->SaveAs("trigeff_PFHT800.eps");

  f_out->Write();
  f_out->Close();
  
  return;
}

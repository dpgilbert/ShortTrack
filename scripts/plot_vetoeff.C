#include "TFile.h"
#include "TChain.h"
#include "TString.h"
#include "TH1.h"
#include "TDirectory.h"
#include "TCut.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "TEfficiency.h"

TChain* mt2 = 0;

void make_plots_var(TString var, TH1D* h_tau1p_all, TH1D* h_tau1p_veto, TH1D* h_tau3p_all, TH1D* h_tau3p_veto);

void plot_vetoeff(TString indir = "/nfs-6/userdata/mt2/V00-08-05_nojson_skim_base_mt2gt200_ZinvV4/") {

  TH1::SetDefaultSumw2();
  
  // inputs
  mt2 = new TChain("mt2");
  mt2->Add(indir+"/ttsl_mg_lo*.root");
  mt2->Add(indir+"/wjets_ht*.root");

  // output file
  TFile* fout = new TFile("vetoeff_taus.root","RECREATE");
  fout->cd();

  // define and fill plots
  
  // ht
  const int nbins_ht = 5;
  const float bins_ht[nbins_ht+1] = {200., 450., 575., 1000., 1500., 2000.};
  TH1D* h_ht_tau1p_all = new TH1D("h_ht_tau1p_all",";H_{T} [GeV]",nbins_ht,bins_ht);
  TH1D* h_ht_tau1p_veto = (TH1D*) h_ht_tau1p_all->Clone("h_ht_tau1p_veto");
  TH1D* h_ht_tau3p_all = (TH1D*) h_ht_tau1p_all->Clone("h_ht_tau3p_all");
  TH1D* h_ht_tau3p_veto = (TH1D*) h_ht_tau1p_all->Clone("h_ht_tau3p_veto");

  make_plots_var("ht", h_ht_tau1p_all, h_ht_tau1p_veto, h_ht_tau3p_all, h_ht_tau3p_veto);

  // nJet30
  const int nbins_nJet30 = 3;
  const float bins_nJet30[nbins_ht+1] = {2., 4., 7., 10.};
  TH1D* h_nJet30_tau1p_all = new TH1D("h_nJet30_tau1p_all",";N(jets)",nbins_nJet30,bins_nJet30);
  TH1D* h_nJet30_tau1p_veto = (TH1D*) h_nJet30_tau1p_all->Clone("h_nJet30_tau1p_veto");
  TH1D* h_nJet30_tau3p_all = (TH1D*) h_nJet30_tau1p_all->Clone("h_nJet30_tau3p_all");
  TH1D* h_nJet30_tau3p_veto = (TH1D*) h_nJet30_tau1p_all->Clone("h_nJet30_tau3p_veto");

  make_plots_var("nJet30", h_nJet30_tau1p_all, h_nJet30_tau1p_veto, h_nJet30_tau3p_all, h_nJet30_tau3p_veto);

  // nBJet20
  const int nbins_nBJet20 = 4;
  const float bins_nBJet20[nbins_ht+1] = {0., 1., 2., 3., 4.};
  TH1D* h_nBJet20_tau1p_all = new TH1D("h_nBJet20_tau1p_all",";N(b jets)",nbins_nBJet20,bins_nBJet20);
  TH1D* h_nBJet20_tau1p_veto = (TH1D*) h_nBJet20_tau1p_all->Clone("h_nBJet20_tau1p_veto");
  TH1D* h_nBJet20_tau3p_all = (TH1D*) h_nBJet20_tau1p_all->Clone("h_nBJet20_tau3p_all");
  TH1D* h_nBJet20_tau3p_veto = (TH1D*) h_nBJet20_tau1p_all->Clone("h_nBJet20_tau3p_veto");

  make_plots_var("nBJet20", h_nBJet20_tau1p_all, h_nBJet20_tau1p_veto, h_nBJet20_tau3p_all, h_nBJet20_tau3p_veto);


  // mt2
  const int nbins_mt2 = 6;
  const float bins_mt2[nbins_mt2+1] = {200, 300, 400, 600, 800, 1000, 1500};
  TH1D* h_mt2_tau1p_all = new TH1D("h_mt2_tau1p_all",";M_{T2} [GeV]",nbins_mt2,bins_mt2);
  TH1D* h_mt2_tau1p_veto = (TH1D*) h_mt2_tau1p_all->Clone("h_mt2_tau1p_veto");
  TH1D* h_mt2_tau3p_all = (TH1D*) h_mt2_tau1p_all->Clone("h_mt2_tau3p_all");
  TH1D* h_mt2_tau3p_veto = (TH1D*) h_mt2_tau1p_all->Clone("h_mt2_tau3p_veto");

  make_plots_var("mt2", h_mt2_tau1p_all, h_mt2_tau1p_veto, h_mt2_tau3p_all, h_mt2_tau3p_veto);

  fout->Write();
  fout->Close();

  return;
}

void make_plots_var(TString var, TH1D* h_tau1p_all, TH1D* h_tau1p_veto, TH1D* h_tau3p_all, TH1D* h_tau3p_veto) {

  // cuts
  TCut srbase = "(nVert > 0) && (nJet30 > 1) && (deltaPhiMin > 0.3) && (diffMetMht/met_pt < 0.5) && (mt2 > 200) && ( (ht > 200 && met_pt > 200) || (ht > 1000 && met_pt > 30))";
  //TCut lepveto = "nMuons10+nElectrons10+nPFLep5LowMT+nPFHad10LowMT == 0"; // actual veto
  TCut lepveto = "nMuons10+nElectrons10+nPFLep5LowMT+nPFHad10LowMT > 0"; // inverted veto
  TCut gentau1p = "ngenTau1Prong == 1 && ngenTau3Prong == 0 && ngenLep+ngenLepFromTau == 0 && genTau_leadTrackPt[0] > 10.";
  TCut gentau3p = "ngenTau1Prong == 0 && ngenTau3Prong == 1 && ngenLep+ngenLepFromTau == 0 && genTau_leadTrackPt[0] > 10.";
  TCut weight = "evt_scale1fb";

  mt2->Draw(Form("%s>>h_%s_tau1p_all",var.Data(),var.Data()),(srbase+gentau1p)*weight);
  mt2->Draw(Form("%s>>h_%s_tau1p_veto",var.Data(),var.Data()),(srbase+gentau1p+lepveto)*weight);
  mt2->Draw(Form("%s>>h_%s_tau3p_all",var.Data(),var.Data()),(srbase+gentau3p)*weight);
  mt2->Draw(Form("%s>>h_%s_tau3p_veto",var.Data(),var.Data()),(srbase+gentau3p+lepveto)*weight);

  TH1D* h_tau1p_eff = (TH1D*) h_tau1p_veto->Clone("h_tau1p_eff");
  h_tau1p_eff->Divide(h_tau1p_eff,h_tau1p_all,1.0,1.0,"B");
  h_tau1p_eff->SetLineColor(kBlue);
  h_tau1p_eff->SetMarkerColor(kBlue);

  TH1D* h_tau3p_eff = (TH1D*) h_tau3p_veto->Clone("h_tau3p_eff");
  h_tau3p_eff->Divide(h_tau3p_eff,h_tau3p_all,1.0,1.0,"B");
  h_tau3p_eff->SetLineColor(kMagenta);
  h_tau3p_eff->SetMarkerColor(kMagenta);

  TCanvas* c = new TCanvas(Form("c_%s",var.Data()),Form("c_%s",var.Data()));
  c->cd();
  c->SetGrid(1,1);

  h_tau1p_eff->Draw("histe");
  h_tau1p_eff->GetYaxis()->SetRangeUser(0,1);
  h_tau1p_eff->GetYaxis()->SetTitle("Total Veto Efficiency");
  h_tau3p_eff->Draw("histe same");
  gPad->Modified();

  TLegend* leg = new TLegend(0.6,0.7,0.9,0.9);
  leg->AddEntry(h_tau1p_eff,"1-prong #tau","pe");
  leg->AddEntry(h_tau3p_eff,"3-prong #tau","pe");
  leg->Draw("same");

  c->SaveAs(Form("c_lepvetoeff_%s.eps",var.Data()));
  c->SaveAs(Form("c_lepvetoeff_%s.pdf",var.Data()));

  return;
}


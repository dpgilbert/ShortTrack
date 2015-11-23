#include "TFile.h"
#include "TChain.h"
#include "TH1.h"
#include "TH2.h"
#include "TLegend.h"
#include "TString.h"
#include "TCanvas.h"
#include "TCut.h"
#include "TEfficiency.h"
#include "TStyle.h"

#include "../CMS_lumi.C"

// -- for CMS_lumi label

const int iPeriod = 4; // 13 tev

// iPos drives the position of the CMS logo in the plot
// iPos=11 : top-left, left-aligned
// iPos=33 : top-right, right-aligned
// iPos=22 : center, centered
// mode generally : 
//   iPos = 10*(alignement 1/2/3) + position (1/2/3 = left/center/right)
const int iPos = 3;


void plot_trigeff_PFHT350_PFMET100 (const TString& indir = "/nfs-6/userdata/mt2/V00-01-07_25ns_miniaodv2_Summer15_25nsV6_2p1fb/") {

  cmsText = "CMS Preliminary";
  cmsTextSize = 0.5;
  lumiTextSize = 0.4;
  writeExtraText = false;
  lumi_13TeV = "2.1 fb^{-1}";
  
  gStyle->SetPadTopMargin(0.08);
  gStyle->SetPadBottomMargin(0.12);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadRightMargin(0.05);

  TH1::SetDefaultSumw2();

  TString suffix = "";
  // TString suffix = "_2j";
  
  TChain* t_jetht = new TChain("mt2");
  TChain* t_met = new TChain("mt2");
  TChain* t_ele = new TChain("mt2");

  t_jetht->Add(Form("%s/*Run2015*JetHT*.root", indir.Data()));
  t_met->Add(Form("%s/*Run2015*MET*.root", indir.Data()));
  t_ele->Add(Form("%s/*Run2015*SingleElectron*.root", indir.Data()));

  TFile* f_out = new TFile(Form("trigeff_PFHT350_PFMET100%s.root",suffix.Data()),"RECREATE");
  
  TH1D* h_ht_denom_met170 = new TH1D("h_ht_denom_met170",";H_{T} [GeV]",32,200,1000);
  TH1D* h_ht_num_met170 = (TH1D*) h_ht_denom_met170->Clone("h_ht_num_met170");
  TH1D* h_ht_denom_ele23 = (TH1D*) h_ht_denom_met170->Clone("h_ht_denom_ele23");
  TH1D* h_ht_num_ele23 = (TH1D*) h_ht_denom_met170->Clone("h_ht_num_ele23");

  TH1D* h_met_denom_ht475 = new TH1D("h_met_denom_ht475",";E_{T}^{miss} [GeV]",40,100,500);
  TH1D* h_met_num_ht475 = (TH1D*) h_met_denom_ht475->Clone("h_met_num_ht475");
  TH1D* h_met_denom_ele23 = (TH1D*) h_met_denom_ht475->Clone("h_met_denom_ele23");
  TH1D* h_met_num_ele23 = (TH1D*) h_met_denom_ht475->Clone("h_met_num_ele23");

  TCut base = "nVert > 0 && nJet30 > 1 && Flag_CSCTightHaloFilter && Flag_eeBadScFilter && Flag_HBHENoiseFilter && Flag_HBHEIsoNoiseFilter";
  //TCut base = "nVert > 0 && nJet30 > 3 && Flag_CSCTightHaloFilter && Flag_eeBadScFilter && Flag_HBHENoiseFilter && Flag_HBHEIsoNoiseFilter";
  TCut had = base + "nElectrons10+nMuons10==0 && deltaPhiMin > 0.3";
  TCut ele = base + "nElectrons10 > 0 && abs(lep_pdgId[0]) == 11 && lep_pt[0] > 25.";

  // t_jetht->Draw("met_pt>>h_met_denom_ht475",had+"ht > 500. && HLT_PFHT475_Prescale");
  // t_jetht->Draw("met_pt>>h_met_num_ht475",had+"ht > 500. && HLT_PFHT475_Prescale && HLT_PFHT350_PFMET100");

  t_ele->Draw("met_pt>>h_met_denom_ele23",ele+"ht > 500. && HLT_SingleEl");
  t_ele->Draw("met_pt>>h_met_num_ele23",ele+"ht > 500. && HLT_SingleEl && HLT_PFHT350_PFMET100");

  t_met->Draw("ht>>h_ht_denom_met170",had+"met_pt > 250. && HLT_PFMET170");
  t_met->Draw("ht>>h_ht_num_met170",had+"met_pt > 250. && HLT_PFMET170 && HLT_PFHT350_PFMET100");

  t_ele->Draw("ht>>h_ht_denom_ele23",ele+"met_pt > 250. && HLT_SingleEl");
  t_ele->Draw("ht>>h_ht_num_ele23",ele+"met_pt > 250. && HLT_SingleEl && HLT_PFHT350_PFMET100");

  TH2F* h_met_axis = new TH2F("h_met_axis",";E_{T}^{miss} [GeV];Efficiency of PFMET100 leg",40,100,500,20,0,1);
  h_met_axis->GetYaxis()->SetTitleOffset(0.98);
  
  // TEfficiency* h_met_eff_ht475 = new TEfficiency(*h_met_num_ht475, *h_met_denom_ht475);
  // h_met_eff_ht475->SetLineColor(kRed);
  // h_met_eff_ht475->SetMarkerColor(kRed);
  
  TEfficiency* h_met_eff_ele23 = new TEfficiency(*h_met_num_ele23, *h_met_denom_ele23);
  h_met_eff_ele23->SetLineColor(kGreen+2);
  h_met_eff_ele23->SetMarkerColor(kGreen+2);

  int bin_met200 = h_met_denom_ele23->FindBin(200.);
  float num_met200 = h_met_num_ele23->Integral(bin_met200,-1);
  float denom_met200 = h_met_denom_ele23->Integral(bin_met200,-1);
  float eff_met200_ele23 = num_met200/denom_met200;
  float eff_UP_met200_ele23 = TEfficiency::ClopperPearson(denom_met200, num_met200, 0.68, 1);
  float eff_DN_met200_ele23 = TEfficiency::ClopperPearson(denom_met200, num_met200, 0.68, 0);

  std::cout << "efficiency for MET > 200 using Ele23: " << eff_met200_ele23 << " +" << (eff_UP_met200_ele23 - eff_met200_ele23)
	    << " -" << (eff_met200_ele23 - eff_DN_met200_ele23) << std::endl;
  
  TH2F* h_ht_axis = new TH2F("h_ht_axis",";H_{T} [GeV];Efficiency of PFHT350 leg",16,200,1000,20,0,1);
  h_ht_axis->GetYaxis()->SetTitleOffset(0.98);

  TEfficiency* h_ht_eff_met170 = new TEfficiency(*h_ht_num_met170, *h_ht_denom_met170);
  h_ht_eff_met170->SetLineColor(kBlue);
  h_ht_eff_met170->SetMarkerColor(kBlue);

  TEfficiency* h_ht_eff_ele23 = new TEfficiency(*h_ht_num_ele23, *h_ht_denom_ele23);
  h_ht_eff_ele23->SetLineColor(kGreen+2);
  h_ht_eff_ele23->SetMarkerColor(kGreen+2);

  TCanvas* c_met = new TCanvas("c_met","c_met");
  c_met->SetGrid(1,1);
  c_met->cd();

  h_met_axis->Draw();
  //h_met_eff_ht475->Draw("pe same");
  h_met_eff_ele23->Draw("pe same");

  TLegend* leg_met = new TLegend(0.6,0.2,0.95,0.5);
  //leg_met->AddEntry(h_met_eff_ht475,"HLT_PFHT475","pe");
  leg_met->AddEntry(h_met_eff_ele23,"HLT_Ele23_WPLoose_Gsf","pe");
  leg_met->Draw("same");

  CMS_lumi( c_met, iPeriod, iPos );
  c_met->SaveAs(Form("trigeff_PFMET100_leg%s.pdf",suffix.Data()));
  c_met->SaveAs(Form("trigeff_PFMET100_leg%s.eps",suffix.Data()));

  TCanvas* c_ht = new TCanvas("c_ht","c_ht");
  c_ht->SetGrid(1,1);
  c_ht->cd();

  h_ht_axis->Draw();
  h_ht_eff_ele23->Draw("pe same");
  h_ht_eff_met170->Draw("pe same");

  TLegend* leg_ht = new TLegend(0.6,0.2,0.95,0.5);
  leg_ht->AddEntry(h_ht_eff_ele23,"HLT_Ele23_WPLoose_Gsf","pe");
  leg_ht->AddEntry(h_ht_eff_met170,"HLT_PFMET170","pe");
  leg_ht->Draw("same");

  CMS_lumi( c_ht, iPeriod, iPos );
  c_ht->SaveAs(Form("trigeff_PFHT350_leg%s.pdf",suffix.Data()));
  c_ht->SaveAs(Form("trigeff_PFHT350_leg%s.eps",suffix.Data()));

  f_out->Write();
  f_out->Close();
  
  return;
}

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

void plot_trigeff_PFMETOR (const TString& indir = "/nfs-6/userdata/mt2/V00-08-12_json_271036-284044_23Sep2016ReReco_36p26fb/trigdenom/") {

  cmsText = "CMS Preliminary";
  cmsTextSize = 0.5;
  lumiTextSize = 0.4;
  writeExtraText = false;
  lumi_13TeV = "36.5 fb^{-1}";
  //lumi_13TeV = "589 pb^{-1}";
  
  gStyle->SetPadTopMargin(0.08);
  gStyle->SetPadBottomMargin(0.12);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadRightMargin(0.05);

  TH1::SetDefaultSumw2();

  //  TString suffix = "";
  TString suffix = "_36p5fb_jetpt450_1j";
  
  TChain* t_jetht = new TChain("mt2");
  // TChain* t_muon = new TChain("mt2");
  TChain* t_ele = new TChain("mt2");

  t_jetht->Add(Form("%s/*Run2016*JetHT*.root", indir.Data()));
  // t_muon->Add(Form("%s/*Run2016*SingleMuon*.root", indir.Data()));
  t_ele->Add(Form("%s/*Run2016*SingleElectron*.root", indir.Data()));

  TFile* f_out = new TFile(Form("trigeff_OR_of_METtrigs%s.root",suffix.Data()),"RECREATE");
  
  TH1D* h_met_denom_ht800 = new TH1D("h_met_denom_ht800",";E_{T}^{miss} [GeV]",40,100,500);
  TH1D* h_metor_num_ht800 = (TH1D*) h_met_denom_ht800->Clone("h_metor_num_ht800");
  TH1D* h_met120_num_ht800 = (TH1D*) h_met_denom_ht800->Clone("h_met120_num_ht800");
  TH1D* h_htmet_num_ht800 = (TH1D*) h_met_denom_ht800->Clone("h_htmet_num_ht800");
  TH1D* h_met_denom_mu20 = (TH1D*) h_met_denom_ht800->Clone("h_met_denom_mu20");
  TH1D* h_met_num_mu20 = (TH1D*) h_met_denom_ht800->Clone("h_met_num_mu20");
  TH1D* h_met_denom_ele23 = (TH1D*) h_met_denom_ht800->Clone("h_met_denom_ele23");
  TH1D* h_metor_num_ele23 = (TH1D*) h_met_denom_ht800->Clone("h_metor_num_ele23");
  TH1D* h_met120_num_ele23 = (TH1D*) h_met_denom_ht800->Clone("h_met120_num_ele23");
  TH1D* h_htmet_num_ele23 = (TH1D*) h_met_denom_ht800->Clone("h_htmet_num_ele23");

  TCanvas* c = new TCanvas("c","c");
  c->SetGrid(1,1);
  c->cd();

  //  TCut base = "nVert > 0 && nJet30 > 1 && Flag_globalTightHalo2016Filter && Flag_eeBadScFilter && Flag_HBHENoiseFilter && Flag_HBHENoiseIsoFilter && Flag_EcalDeadCellTriggerPrimitiveFilter && Flag_goodVertices && Flag_badMuonFilter && Flag_badChargedHadronFilter && ht > 250. && isGolden";
  // TCut had = base + "nElectrons10+nMuons10==0 && ht > 1000. && deltaPhiMin > 0.3";
  //  TCut base = "nVert > 0 && nJet30 > 1 && Flag_globalTightHalo2016Filter && Flag_eeBadScFilter && Flag_HBHENoiseFilter && Flag_HBHENoiseIsoFilter && Flag_EcalDeadCellTriggerPrimitiveFilter && Flag_goodVertices && Flag_badMuonFilter && Flag_badChargedHadronFilter && ht > 250. && isGolden && deltaPhiMin < 0.3";
  TCut base = "nVert > 0 && nJet30 == 1 && Flag_globalTightHalo2016Filter && Flag_eeBadScFilter && Flag_HBHENoiseFilter && Flag_HBHENoiseIsoFilter && Flag_EcalDeadCellTriggerPrimitiveFilter && Flag_goodVertices && Flag_badMuonFilter && Flag_badChargedHadronFilter && jet1_pt > 450. && isGolden";
  TCut ele = base + "nElectrons10 > 0 && abs(lep_pdgId[0]) == 11 && lep_pt[0] > 30. && lep_tightId[0] > 2 && lep_relIso03[0] < 0.1";
  TCut mu = base + "nMuons10 > 0 && abs(lep_pdgId[0]) == 13 && lep_pt[0] > 25. && lep_tightId[0] > 0 && lep_relIso03[0] < 0.1";
  //  TCut had = base + "nElectrons10+nMuons10==0 && (HLT_PFHT475_Prescale || HLT_PFHT600_Prescale) && ht > 600. && deltaPhiMin < 0.3 && mt2 < 200";
  TCut had = base + "nElectrons10+nMuons10==0 && (HLT_PFHT125_Prescale || HLT_PFHT200_Prescale || HLT_PFHT300_Prescale) && ht > 250 && ht < 575. && deltaPhiMin < 0.3 && mt2 < 200";

  // t_jetht->Draw("met_pt>>h_met_denom_ht800",had);
  // t_jetht->Draw("met_pt>>h_metor_num_ht800",had+"(HLT_PFMET120_PFMHT120 || HLT_PFHT300_PFMET110)");
  // t_jetht->Draw("met_pt>>h_met120_num_ht800",had+"(HLT_PFMET120_PFMHT120)");
  // t_jetht->Draw("met_pt>>h_htmet_num_ht800",had+"(HLT_PFHT300_PFMET110)");

  // t_muon->Draw("met_pt>>h_met_denom_mu20",mu+"HLT_SingleMu");
  // t_muon->Draw("met_pt>>h_met_num_mu20",mu+"HLT_SingleMu && HLT_PFMET100_PFMHT100");

  // t_ele->Draw("met_pt>>h_met_denom_ele23",ele+"ht > 450. && HLT_SingleEl");
  // t_ele->Draw("met_pt>>h_metor_num_ele23",ele+"ht > 450. && HLT_SingleEl && (HLT_PFMET120_PFMHT120 || HLT_PFHT300_PFMET110)");
  // t_ele->Draw("met_pt>>h_met120_num_ele23",ele+"ht > 450. && HLT_SingleEl && (HLT_PFMET120_PFMHT120)");
  // t_ele->Draw("met_pt>>h_htmet_num_ele23",ele+"ht > 450. && HLT_SingleEl && (HLT_PFHT300_PFMET110)");

  t_ele->Draw("met_pt>>h_met_denom_ele23",ele+"jet1_pt > 450. && HLT_SingleEl");
  t_ele->Draw("met_pt>>h_metor_num_ele23",ele+"jet1_pt > 450. && HLT_SingleEl && (HLT_PFMET120_PFMHT120 || HLT_PFHT300_PFMET110)");
  t_ele->Draw("met_pt>>h_met120_num_ele23",ele+"jet1_pt > 450. && HLT_SingleEl && (HLT_PFMET120_PFMHT120)");
  t_ele->Draw("met_pt>>h_htmet_num_ele23",ele+"jet1_pt > 450. && HLT_SingleEl && (HLT_PFHT300_PFMET110)");

  TH2F* h_axis = new TH2F("h_axis",";E_{T}^{miss} [GeV];Efficiency",40,100,500,20,0,1);
  h_axis->GetYaxis()->SetTitleOffset(0.98);
  h_axis->Draw();
  
  // TEfficiency* h_met_eff_ht800 = new TEfficiency(*h_met_num_ht800, *h_met_denom_ht800);
  // h_met_eff_ht800->SetLineColor(kRed);
  // h_met_eff_ht800->SetMarkerColor(kRed);
  
  // TEfficiency* h_met_eff_mu20 = new TEfficiency(*h_met_num_mu20, *h_met_denom_mu20);
  // h_met_eff_mu20->SetLineColor(kBlue);
  // h_met_eff_mu20->SetMarkerColor(kBlue);

  // TEfficiency* h_metor_eff_ht800 = new TEfficiency(*h_metor_num_ht800, *h_met_denom_ht800);
  // h_metor_eff_ht800->SetLineColor(kBlue);
  // h_metor_eff_ht800->SetMarkerColor(kBlue);

  // TEfficiency* h_met120_eff_ht800 = new TEfficiency(*h_met120_num_ht800, *h_met_denom_ht800);
  // h_met120_eff_ht800->SetLineColor(kRed);
  // h_met120_eff_ht800->SetMarkerColor(kRed);

  // TEfficiency* h_htmet_eff_ht800 = new TEfficiency(*h_htmet_num_ht800, *h_met_denom_ht800);
  // h_htmet_eff_ht800->SetLineColor(kGreen+2);
  // h_htmet_eff_ht800->SetMarkerColor(kGreen+2);

  TEfficiency* h_metor_eff_ele23 = new TEfficiency(*h_metor_num_ele23, *h_met_denom_ele23);
  h_metor_eff_ele23->SetLineColor(kBlue);
  h_metor_eff_ele23->SetMarkerColor(kBlue);

  TEfficiency* h_met120_eff_ele23 = new TEfficiency(*h_met120_num_ele23, *h_met_denom_ele23);
  h_met120_eff_ele23->SetLineColor(kRed);
  h_met120_eff_ele23->SetMarkerColor(kRed);

  TEfficiency* h_htmet_eff_ele23 = new TEfficiency(*h_htmet_num_ele23, *h_met_denom_ele23);
  h_htmet_eff_ele23->SetLineColor(kGreen+2);
  h_htmet_eff_ele23->SetMarkerColor(kGreen+2);

  int bin_met250 = h_met_denom_ele23->FindBin(250.);
  float num_met250 = h_metor_num_ele23->Integral(bin_met250,-1);
  float denom_met250 = h_met_denom_ele23->Integral(bin_met250,-1);
  float eff_met250_ele23 = num_met250/denom_met250;
  float eff_UP_met250_ele23 = TEfficiency::ClopperPearson(denom_met250, num_met250, 0.68, 1);
  float eff_DN_met250_ele23 = TEfficiency::ClopperPearson(denom_met250, num_met250, 0.68, 0);

  std::cout << "efficiency for MET > 250 using Ele23: " << eff_met250_ele23 << " +" << (eff_UP_met250_ele23 - eff_met250_ele23)
  	    << " -" << (eff_met250_ele23 - eff_DN_met250_ele23) << std::endl;
  
  //h_met_eff_ht800->Draw("pe same");
  //h_met_eff_mu20->Draw("pe same");
  // h_met120_eff_ht800->Draw("pe same");
  // h_htmet_eff_ht800->Draw("pe same");
  // h_metor_eff_ht800->Draw("pe same");
  h_met120_eff_ele23->Draw("pe same");
  h_htmet_eff_ele23->Draw("pe same");
  h_metor_eff_ele23->Draw("pe same");

  TLegend* leg = new TLegend(0.35,0.15,0.95,0.45);
  leg->SetHeader("Ref: Ele23 || Ele27_eta2p1 || Ele32_eta2p1");
  // leg->AddEntry(h_metor_eff_ht800,"OR of PFMET trigs","pe");
  // leg->AddEntry(h_htmet_eff_ht800,"PFHT300_PFMET110","pe");
  // leg->AddEntry(h_met120_eff_ht800,"PFMET120_PFMHT120_IDTight","pe");
  leg->AddEntry(h_metor_eff_ele23,"OR of PFMET trigs","pe");
  leg->AddEntry(h_htmet_eff_ele23,"PFHT300_PFMET110","pe");
  leg->AddEntry(h_met120_eff_ele23,"PFMET120_PFMHT120_IDTight","pe");
  leg->Draw("same");

  CMS_lumi( c, iPeriod, iPos );
  c->SaveAs(Form("trigeff_OR_of_METtrigs%s.pdf",suffix.Data()));
  c->SaveAs(Form("trigeff_OR_of_METtrigs%s.eps",suffix.Data()));
  c->SaveAs(Form("trigeff_OR_of_METtrigs%s.png",suffix.Data()));

  f_out->Write();
  f_out->Close();
  
  return;
}

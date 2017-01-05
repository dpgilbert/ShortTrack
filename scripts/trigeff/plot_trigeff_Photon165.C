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

void plot_trigeff_Photon165 (const TString& indir = "/nfs-6/userdata/mt2/V00-08-12_json_271036-284044_23Sep2016ReReco_36p26fb/trigdenom") {

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

  //TString suffix = "_hovere001";
  TString suffix = "_36p5fb";
  //  TString suffix = "";
  
  TChain* t_jetht = new TChain("mt2");
  TChain* t_muon = new TChain("mt2");

  t_jetht->Add(Form("%s/*Run2016*JetHT*.root", indir.Data()));
  //  t_muon->Add(Form("%s/*Run2016*SingleMuon*.root", indir.Data()));

  TFile* f_out = new TFile(Form("trigeff_Photon165%s.root",suffix.Data()),"RECREATE");

  const int n_ptbins = 32;
  const float ptbins[n_ptbins+1] = {100, 110, 120, 130, 140, 150, 160, 170, 180, 190,
				    200, 210, 220, 230, 240, 250, 260, 270, 280, 290,
				    300, 320, 340, 360, 380,
				    400, 450, 500, 550, 600, 700, 800, 1000};
  
  TH1D* h_pt_denom_jetht = new TH1D("h_pt_denom_jetht",";#gamma p_{T} [GeV]",n_ptbins,ptbins);
  TH1D* h_pt_num_jetht = (TH1D*) h_pt_denom_jetht->Clone("h_pt_num_jetht");
  TH1D* h_pt_eb_denom_jetht = (TH1D*) h_pt_denom_jetht->Clone("h_pt_eb_denom_jetht");
  TH1D* h_pt_eb_num_jetht = (TH1D*) h_pt_num_jetht->Clone("h_pt_eb_num_jetht");
  TH1D* h_pt_ee_denom_jetht = (TH1D*) h_pt_denom_jetht->Clone("h_pt_ee_denom_jetht");
  TH1D* h_pt_ee_num_jetht = (TH1D*) h_pt_num_jetht->Clone("h_pt_ee_num_jetht");

  TCanvas* c = new TCanvas("c","c");
  c->SetGrid(1,1);
  c->cd();

  TCut base = "nVert > 0 && Flag_CSCTightHalo2015Filter && Flag_eeBadScFilter && Flag_HBHENoiseFilter && Flag_HBHENoiseIsoFilter && Flag_EcalDeadCellTriggerPrimitiveFilter && Flag_goodVertices && isGolden && ngamma > 0 && gamma_idCutBased[0] > 0 && gamma_chHadIso[0] < 2.5 && gamma_hOverE015[0] < 0.1 ";
  //  TCut base = "nVert > 0 && Flag_CSCTightHalo2015Filter && Flag_eeBadScFilter && Flag_HBHENoiseFilter && Flag_HBHENoiseIsoFilter && Flag_EcalDeadCellTriggerPrimitiveFilter && Flag_goodVertices && ngamma > 0 && gamma_idCutBased[0] > 0 && gamma_chHadIso[0] < 2.5 && gamma_hOverE[0] < 0.01 ";
  // -- cuts for ZMET 2016, or as close as I can get..
  //  TCut base = "nVert > 0 && Flag_CSCTightHalo2015Filter && Flag_eeBadScFilter && Flag_HBHENoiseFilter && Flag_HBHENoiseIsoFilter && Flag_EcalDeadCellTriggerPrimitiveFilter && Flag_goodVertices && nElectrons10 == 0 && ngamma > 0 && gamma_idCutBased[0] > 0 && ( (fabs(gamma_eta[0]) < 1.442 && gamma_chHadIso[0] < 3.32 && (gamma_neuHadIso[0] < 1.92 + 0.014  * gamma_pt[0] + 0.000019*pow(gamma_pt[0],2)) && gamma_phIso[0] < 0.81 + 0.0053 * gamma_pt[0]) || ((fabs(gamma_eta[0]) > 1.566 && fabs(gamma_eta[0]) < 2.4 && gamma_chHadIso[0] < 1.97 && (gamma_neuHadIso[0] < 11.86 + 0.0139  * gamma_pt[0] + 0.000025*pow(gamma_pt[0],2)) && gamma_phIso[0] < 0.83 + 0.0034 * gamma_pt[0]) ) ) ";
  TCut eb = "abs(gamma_eta[0]) < 1.479";
  TCut ee = "abs(gamma_eta[0]) > 1.479 && abs(gamma_eta[0]) < 2.4";
  TCut had = base + "nJet30 > 0 && gamma_ht > 1000. && (HLT_PFHT900 || HLT_PFJet450)";
  //  TCut had = base + "ht > 900";
  TCut muon = base + "nMuons10 > 0 && abs(lep_pdgId[0]) == 13 && lep_pt[0] > 25.";

  t_jetht->Draw("gamma_pt[0]>>h_pt_denom_jetht",had);
  t_jetht->Draw("gamma_pt[0]>>h_pt_num_jetht",had+"HLT_Photon165_HE10");
  t_jetht->Draw("gamma_pt[0]>>h_pt_eb_denom_jetht",had+eb);
  t_jetht->Draw("gamma_pt[0]>>h_pt_eb_num_jetht",had+eb+"HLT_Photon165_HE10");
  t_jetht->Draw("gamma_pt[0]>>h_pt_ee_denom_jetht",had+ee);
  t_jetht->Draw("gamma_pt[0]>>h_pt_ee_num_jetht",had+ee+"HLT_Photon165_HE10");

  TH2F* h_axis = new TH2F("h_axis",";#gamma p_{T} [GeV];Efficiency of HLT_Photon165_HE10",45,100,1000,20,0,1);
  //TH2F* h_axis = new TH2F("h_axis",";H_{T} [GeV];Efficiency of HLT_PFJETHT",38,600,2500,20,0,1);
  h_axis->GetYaxis()->SetTitleOffset(0.98);
  h_axis->Draw();
  
  TEfficiency* h_pt_eff_jetht = new TEfficiency(*h_pt_num_jetht, *h_pt_denom_jetht);
  h_pt_eff_jetht->SetName("h_pt_eff_jetht");
  h_pt_eff_jetht->SetLineColor(kBlue);
  h_pt_eff_jetht->SetMarkerColor(kBlue);

  TEfficiency* h_pt_eb_eff_jetht = new TEfficiency(*h_pt_eb_num_jetht, *h_pt_eb_denom_jetht);
  h_pt_eb_eff_jetht->SetName("h_pt_eb_eff_jetht");
  h_pt_eb_eff_jetht->SetLineColor(kBlue);
  h_pt_eb_eff_jetht->SetMarkerColor(kBlue);

  TEfficiency* h_pt_ee_eff_jetht = new TEfficiency(*h_pt_ee_num_jetht, *h_pt_ee_denom_jetht);
  h_pt_ee_eff_jetht->SetName("h_pt_ee_eff_jetht");
  h_pt_ee_eff_jetht->SetLineColor(kGreen+3);
  h_pt_ee_eff_jetht->SetMarkerColor(kGreen+3);

  h_pt_eff_jetht->Draw("pe same");

  TLegend* leg = new TLegend(0.6,0.2,0.9,0.5);
  leg->AddEntry(h_pt_eff_jetht,"PFHT900 || PFJet450","pe");
  leg->Draw("same");

  CMS_lumi( c, iPeriod, iPos );
  c->SaveAs(Form("trigeff_Photon165%s.pdf",suffix.Data()));
  c->SaveAs(Form("trigeff_Photon165%s.eps",suffix.Data()));
  c->SaveAs(Form("trigeff_Photon165%s.png",suffix.Data()));

  // ----- EB -----------
  
  TCanvas* c_eb = new TCanvas("c_eb","c_eb");
  c_eb->SetGrid(1,1);
  c_eb->cd();

  h_axis->Draw();
  h_pt_eb_eff_jetht->Draw("pe same");
  h_pt_ee_eff_jetht->Draw("pe same");
  
  TLegend* leg_eb = new TLegend(0.6,0.2,0.9,0.5);
  leg_eb->SetHeader("Ref: PFHT900 || PFJet450");
  leg_eb->AddEntry(h_pt_eb_eff_jetht,"EB Photons","pe");
  leg_eb->AddEntry(h_pt_ee_eff_jetht,"EE Photons","pe");
  leg_eb->Draw("same");

  CMS_lumi( c_eb, iPeriod, iPos );
  c_eb->SaveAs(Form("trigeff_Photon165_EBEE%s.pdf",suffix.Data()));
  c_eb->SaveAs(Form("trigeff_Photon165_EBEE%s.eps",suffix.Data()));
  c_eb->SaveAs(Form("trigeff_Photon165_EBEE%s.png",suffix.Data()));

  // // ----- EE -----------

  // TCanvas* c_ee = new TCanvas("c_ee","c_ee");
  // c_ee->SetGrid(1,1);
  // c_ee->cd();

  // h_axis->Draw();
  // h_pt_ee_eff_jetht->Draw("pe same");
  
  // TLegend* leg_ee = new TLegend(0.6,0.2,0.9,0.5);
  // leg_ee->SetHeader("EE only");
  // leg_ee->AddEntry(h_pt_eff_jetht,"PFHT900 || PFJet450","pe");
  // leg_ee->Draw("same");

  // CMS_lumi( c_ee, iPeriod, iPos );
  // c_ee->SaveAs(Form("trigeff_Photon165_EE%s.pdf",suffix.Data()));
  // c_ee->SaveAs(Form("trigeff_Photon165_EE%s.eps",suffix.Data()));
  // c_ee->SaveAs(Form("trigeff_Photon165_EE%s.png",suffix.Data()));

  f_out->Write();
  h_pt_eff_jetht->Write();
  h_pt_eb_eff_jetht->Write();
  h_pt_ee_eff_jetht->Write();
  f_out->Close();
  
  return;
}

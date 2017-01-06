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

void plot_trigeff_dilep (const TString& indir = "/nfs-6/userdata/mt2/V00-08-12_json_271036-284044_23Sep2016ReReco_36p26fb/skim_dilep/") {

  cmsText = "CMS Preliminary";
  cmsTextSize = 0.5;
  lumiTextSize = 0.4;
  writeExtraText = false;
  lumi_13TeV = "36.5 fb^{-1}";
  //  lumi_13TeV = "Period 2016H";
  
  gStyle->SetPadTopMargin(0.08);
  gStyle->SetPadBottomMargin(0.12);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadRightMargin(0.05);

  gStyle->SetPaintTextFormat(".2f");  

  TH1::SetDefaultSumw2();
  
  TString suffix = "_36p5fb_nologx";
  
  TChain* t_ht = new TChain("mt2");
  TChain* t_met = new TChain("mt2");

  // t_ht->Add(Form("%s/*Run2016H*JetHT*.root", indir.Data()));
  // t_met->Add(Form("%s/*Run2016H*MET*.root", indir.Data()));

  t_ht->Add(Form("%s/*Run2016*JetHT*.root", indir.Data()));
  t_met->Add(Form("%s/*Run2016*MET*.root", indir.Data()));

  TFile* f_out = new TFile(Form("trigeff_dilep%s.root",suffix.Data()),"RECREATE");

  TCut base = "nVert > 0 && nJet30 >= 1 && Flag_globalTightHalo2016Filter && Flag_eeBadScFilter && Flag_HBHENoiseFilter && Flag_HBHENoiseIsoFilter && Flag_EcalDeadCellTriggerPrimitiveFilter && Flag_goodVertices && Flag_badMuonFilter && Flag_badChargedHadronFilter && isGolden";
  TCut dilep = base + "nlep == 2 && lep_charge[0] * lep_charge[1] == -1 && (abs(lep_pdgId[0]) == 13 ||  lep_tightId[0] > 0 ) && (abs(lep_pdgId[1]) == 13 ||  lep_tightId[1] > 0 ) && lep_pt[1] > 20.";
  TCut ee = dilep + "abs(lep_pdgId[0]) == 11 && abs(lep_pdgId[1]) == 11";
  TCut mm = dilep + "abs(lep_pdgId[0]) == 13 && abs(lep_pdgId[1]) == 13";
  TCut em = dilep + "abs(lep_pdgId[0]) != abs(lep_pdgId[1])";

  TCut trigs_ht = "HLT_PFHT900 || HLT_PFHT800 || HLT_PFHT600_Prescale || HLT_PFHT475_Prescale || HLT_PFHT350_Prescale || HLT_PFHT300_Prescale || HLT_PFHT200_Prescale || HLT_PFHT125_Prescale";
  TCut trigs_met = "HLT_PFMETNoMu90_PFMHTNoMu90 || HLT_PFMETNoMu100_PFMHTNoMu100 || HLT_PFMETNoMu110_PFMHTNoMu110 || HLT_PFMETNoMu120_PFMHTNoMu120 || HLT_PFMET90_PFMHT90 || HLT_PFMET100_PFMHT100 || HLT_PFMET110_PFMHT110 || HLT_PFMET120_PFMHT120";
  TCut trigs_ee = "HLT_DoubleEl || HLT_Photon165_HE10 || HLT_DoubleEl33";
  TCut trigs_mm = "HLT_DoubleMu || HLT_DoubleMu_NonIso || HLT_SingleMu_NonIso";
  //  TCut trigs_em = "HLT_MuX_Ele12 || HLT_Mu8_EleX || HLT_Mu30_Ele30_NonIso || HLT_Mu33_Ele33_NonIso";
  TCut trigs_em = "HLT_MuX_Ele12 || HLT_Mu8_EleX || HLT_Mu30_Ele30_NonIso || HLT_Mu33_Ele33_NonIso || HLT_SingleMu_NonIso || HLT_Photon165_HE10";
  
  // const int nbins_lead = 12;
  // const Double_t bins_lead[nbins+1] = {20, 25, 30, 35, 40, 
  // 				       50, 60, 80, 100, 120, 150, 200, 300};
  // const int nbins_subl = 12;
  // const Double_t bins_subl[nbins+1] = {20, 25, 30, 35, 40, 
  // 				       50, 60, 80, 100, 120, 150, 200, 300};
  const int nbins_lead = 2;
  const Double_t bins_lead[nbins_lead+1] = {100, 180, 300};
  
  const int nbins_subl = 4;
  const Double_t bins_subl[nbins_subl+1] = {30, 35, 100, 180, 300};
  
  // TH1D* h_ee_leadpt_denom_ht = new TH1D("h_ee_leadpt_denom_ht",";lead p_{T} [GeV]",nbins, bins);
  // TH1D* h_ee_leadpt_num_ht = (TH1D*) h_ee_leadpt_denom_ht->Clone("h_ee_leadpt_num_ht");
  // TH1D* h_ee_sublpt_denom_ht = (TH1D*) h_ee_leadpt_denom_ht->Clone("h_ee_sublpt_denom_ht");
  // TH1D* h_ee_sublpt_num_ht = (TH1D*) h_ee_leadpt_denom_ht->Clone("h_ee_sublpt_num_ht");
  // TH1D* h_ee_leadpt_denom_met = (TH1D*) h_ee_leadpt_denom_ht->Clone("h_ee_leadpt_denom_met");
  // TH1D* h_ee_leadpt_num_met = (TH1D*) h_ee_leadpt_denom_ht->Clone("h_ee_leadpt_num_met");
  // TH1D* h_ee_sublpt_denom_met = (TH1D*) h_ee_leadpt_denom_ht->Clone("h_ee_sublpt_denom_met");
  // TH1D* h_ee_sublpt_num_met = (TH1D*) h_ee_leadpt_denom_ht->Clone("h_ee_sublpt_num_met");

  TH2D* h_ee_pt_2d_denom_ht = new TH2D("h_ee_pt_2d_denom_ht",";lead p_{T} [GeV]; subl p_{T} [GeV]",nbins_lead, bins_lead,nbins_subl, bins_subl);
  TH2D* h_ee_pt_2d_num_ht = (TH2D*) h_ee_pt_2d_denom_ht->Clone("h_ee_pt_2d_num_ht");
  TH2D* h_ee_pt_2d_denom_met = (TH2D*) h_ee_pt_2d_denom_ht->Clone("h_ee_pt_2d_denom_met");
  TH2D* h_ee_pt_2d_num_met = (TH2D*) h_ee_pt_2d_denom_ht->Clone("h_ee_pt_2d_num_met");
  
  TH2D* h_mm_pt_2d_denom_ht = (TH2D*) h_ee_pt_2d_denom_ht->Clone("h_mm_pt_2d_denom_ht");
  TH2D* h_mm_pt_2d_num_ht = (TH2D*) h_ee_pt_2d_denom_ht->Clone("h_mm_pt_2d_num_ht");
  TH2D* h_mm_pt_2d_denom_met = (TH2D*) h_ee_pt_2d_denom_ht->Clone("h_mm_pt_2d_denom_met");
  TH2D* h_mm_pt_2d_num_met = (TH2D*) h_ee_pt_2d_denom_ht->Clone("h_mm_pt_2d_num_met");
  
  TH2D* h_em_pt_2d_denom_ht = (TH2D*) h_ee_pt_2d_denom_ht->Clone("h_em_pt_2d_denom_ht");
  TH2D* h_em_pt_2d_num_ht = (TH2D*) h_ee_pt_2d_denom_ht->Clone("h_em_pt_2d_num_ht");
  TH2D* h_em_pt_2d_denom_met = (TH2D*) h_ee_pt_2d_denom_ht->Clone("h_em_pt_2d_denom_met");
  TH2D* h_em_pt_2d_num_met = (TH2D*) h_ee_pt_2d_denom_ht->Clone("h_em_pt_2d_num_met");
  
  // ------ dielectron

  // TCanvas* c_ee = new TCanvas("c_ee","c_ee");
  // c_ee->SetGrid(1,1);
  // c_ee->cd();

  // t_met->Draw("lep_pt[0]>>h_ee_leadpt_denom_met",ee+trigs_met+"met_pt > 250.");
  // t_met->Draw("lep_pt[0]>>h_ee_leadpt_num_met",ee+trigs_met+"met_pt > 250."+trigs_ee);

  // t_met->Draw("lep_pt[1]>>h_ee_sublpt_denom_met",ee+trigs_met+"met_pt > 250.");
  // t_met->Draw("lep_pt[1]>>h_ee_sublpt_num_met",ee+trigs_met+"met_pt > 250."+trigs_ee);

  // t_ht->Draw("lep_pt[0]>>h_ee_leadpt_denom_ht",ee+trigs_ht+"ht > 200.");
  // t_ht->Draw("lep_pt[0]>>h_ee_leadpt_num_ht",ee+trigs_ht+"ht > 200."+trigs_ee);

  // t_ht->Draw("lep_pt[1]>>h_ee_sublpt_denom_ht",ee+trigs_ht+"ht > 200.");
  // t_ht->Draw("lep_pt[1]>>h_ee_sublpt_num_ht",ee+trigs_ht+"ht > 200."+trigs_ee);

  // TH2F* h_axis_ee = new TH2F("h_axis",";p_{T} [GeV];Trigger Efficiency",50,20,300,50,0,1);
  // h_axis_ee->GetYaxis()->SetTitleOffset(0.98);
  // h_axis_ee->Draw();
  
  // TEfficiency* h_ee_leadpt_eff_met = new TEfficiency(*h_ee_leadpt_num_met, *h_ee_leadpt_denom_met);
  // h_ee_leadpt_eff_met->SetLineColor(kBlue);
  // h_ee_leadpt_eff_met->SetMarkerColor(kBlue);

  // TEfficiency* h_ee_sublpt_eff_met = new TEfficiency(*h_ee_sublpt_num_met, *h_ee_sublpt_denom_met);
  // h_ee_sublpt_eff_met->SetLineColor(kRed);;
  // h_ee_sublpt_eff_met->SetMarkerColor(kRed);

  // TEfficiency* h_ee_leadpt_eff_ht = new TEfficiency(*h_ee_leadpt_num_ht, *h_ee_leadpt_denom_ht);
  // h_ee_leadpt_eff_ht->SetLineColor(kGreen+2);
  // h_ee_leadpt_eff_ht->SetMarkerColor(kGreen+2);

  // TEfficiency* h_ee_sublpt_eff_ht = new TEfficiency(*h_ee_sublpt_num_ht, *h_ee_sublpt_denom_ht);
  // h_ee_sublpt_eff_ht->SetLineColor(kMagenta);
  // h_ee_sublpt_eff_ht->SetMarkerColor(kMagenta);

  // h_ee_leadpt_eff_met->Draw("pe same");
  // h_ee_sublpt_eff_met->Draw("pe same");
  // h_ee_leadpt_eff_ht->Draw("pe same");
  // h_ee_sublpt_eff_ht->Draw("pe same");

  // TLegend* leg_ee = new TLegend(0.45,0.2,0.9,0.5);
  // leg_ee->SetHeader("ee triggers");
  // leg_ee->AddEntry(h_ee_leadpt_eff_met,"lead p_{T} from MET","pe");
  // leg_ee->AddEntry(h_ee_sublpt_eff_met,"subl p_{T} from MET","pe");
  // leg_ee->AddEntry(h_ee_leadpt_eff_ht,"lead p_{T} from HT","pe");
  // leg_ee->AddEntry(h_ee_sublpt_eff_ht,"subl p_{T} from HT","pe");
  // leg_ee->Draw("same");

  // CMS_lumi( c_ee, iPeriod, iPos );
  // c_ee->SaveAs(Form("trigeff_dilep_ee%s.pdf",suffix.Data()));
  // c_ee->SaveAs(Form("trigeff_dilep_ee%s.eps",suffix.Data()));
  // c_ee->SaveAs(Form("trigeff_dilep_ee%s.png",suffix.Data()));

  t_ht->Draw("lep_pt[1]:lep_pt[0]>>h_ee_pt_2d_denom_ht",ee+trigs_ht+"ht > 200.");
  t_ht->Draw("lep_pt[1]:lep_pt[0]>>h_ee_pt_2d_num_ht",ee+trigs_ht+"ht > 200."+trigs_ee);

  t_met->Draw("lep_pt[1]:lep_pt[0]>>h_ee_pt_2d_denom_met",ee+trigs_met+"met_pt > 250.");
  t_met->Draw("lep_pt[1]:lep_pt[0]>>h_ee_pt_2d_num_met",ee+trigs_met+"met_pt > 250."+trigs_ee);

  TCanvas* c_ee_2d_ht = new TCanvas("c_ee_2d_ht","c_ee_2d_ht");
  //  c_ee_2d_ht->SetLogx();
  c_ee_2d_ht->SetLogy();
  c_ee_2d_ht->cd();

  TH2D* h_ee_pt_2d_eff_ht = (TH2D*) h_ee_pt_2d_num_ht->Clone("h_ee_pt_2d_eff_ht");
  h_ee_pt_2d_eff_ht->Divide(h_ee_pt_2d_denom_ht);
  h_ee_pt_2d_eff_ht->GetXaxis()->SetMoreLogLabels();
  h_ee_pt_2d_eff_ht->GetYaxis()->SetMoreLogLabels();
  h_ee_pt_2d_eff_ht->GetYaxis()->SetTitleOffset(1.0);
  h_ee_pt_2d_eff_ht->Draw("col texte");
  
  CMS_lumi( c_ee_2d_ht, iPeriod, iPos );
  c_ee_2d_ht->SaveAs(Form("trigeff_dilep_ee_2d_ht%s.pdf",suffix.Data()));
  c_ee_2d_ht->SaveAs(Form("trigeff_dilep_ee_2d_ht%s.eps",suffix.Data()));
  c_ee_2d_ht->SaveAs(Form("trigeff_dilep_ee_2d_ht%s.png",suffix.Data()));

  // TCanvas* c_ee_2d_met = new TCanvas("c_ee_2d_met","c_ee_2d_met");
  // c_ee_2d_met->SetLogx();
  // c_ee_2d_met->SetLogy();
  // c_ee_2d_met->cd();

  // TH2D* h_ee_pt_2d_eff_met = (TH2D*) h_ee_pt_2d_num_met->Clone("h_ee_pt_2d_eff_met");
  // h_ee_pt_2d_eff_met->Divide(h_ee_pt_2d_denom_met);
  // h_ee_pt_2d_eff_met->GetXaxis()->SetMoreLogLabels();
  // h_ee_pt_2d_eff_met->GetYaxis()->SetMoreLogLabels();
  // h_ee_pt_2d_eff_met->GetYaxis()->SetTitleOffset(1.0);
  // h_ee_pt_2d_eff_met->Draw("col text");
  
  // CMS_lumi( c_ee_2d_met, iPeriod, iPos );
  // c_ee_2d_met->SaveAs(Form("trigeff_dilep_ee_2d_met%s.pdf",suffix.Data()));
  // c_ee_2d_met->SaveAs(Form("trigeff_dilep_ee_2d_met%s.eps",suffix.Data()));
  // c_ee_2d_met->SaveAs(Form("trigeff_dilep_ee_2d_met%s.png",suffix.Data()));


  
  // --------- dimuons
  
  t_ht->Draw("lep_pt[1]:lep_pt[0]>>h_mm_pt_2d_denom_ht",mm+trigs_ht+"ht > 200.");
  t_ht->Draw("lep_pt[1]:lep_pt[0]>>h_mm_pt_2d_num_ht",mm+trigs_ht+"ht > 200."+trigs_mm);

  t_met->Draw("lep_pt[1]:lep_pt[0]>>h_mm_pt_2d_denom_met",mm+trigs_met+"met_pt > 250.");
  t_met->Draw("lep_pt[1]:lep_pt[0]>>h_mm_pt_2d_num_met",mm+trigs_met+"met_pt > 250."+trigs_mm);

  TCanvas* c_mm_2d_ht = new TCanvas("c_mm_2d_ht","c_mm_2d_ht");
  //  c_mm_2d_ht->SetLogx();
  c_mm_2d_ht->SetLogy();
  c_mm_2d_ht->cd();

  TH2D* h_mm_pt_2d_eff_ht = (TH2D*) h_mm_pt_2d_num_ht->Clone("h_mm_pt_2d_eff_ht");
  h_mm_pt_2d_eff_ht->Divide(h_mm_pt_2d_denom_ht);
  h_mm_pt_2d_eff_ht->GetXaxis()->SetMoreLogLabels();
  h_mm_pt_2d_eff_ht->GetYaxis()->SetMoreLogLabels();
  h_mm_pt_2d_eff_ht->GetYaxis()->SetTitleOffset(1.0);
  h_mm_pt_2d_eff_ht->Draw("col texte");
  
  CMS_lumi( c_mm_2d_ht, iPeriod, iPos );
  c_mm_2d_ht->SaveAs(Form("trigeff_dilep_mm_2d_ht%s.pdf",suffix.Data()));
  c_mm_2d_ht->SaveAs(Form("trigeff_dilep_mm_2d_ht%s.eps",suffix.Data()));
  c_mm_2d_ht->SaveAs(Form("trigeff_dilep_mm_2d_ht%s.png",suffix.Data()));

  // TCanvas* c_mm_2d_met = new TCanvas("c_mm_2d_met","c_mm_2d_met");
  // c_mm_2d_met->SetLogx();
  // c_mm_2d_met->SetLogy();
  // c_mm_2d_met->cd();

  // TH2D* h_mm_pt_2d_eff_met = (TH2D*) h_mm_pt_2d_num_met->Clone("h_mm_pt_2d_eff_met");
  // h_mm_pt_2d_eff_met->Divide(h_mm_pt_2d_denom_met);
  // h_mm_pt_2d_eff_met->GetXaxis()->SetMoreLogLabels();
  // h_mm_pt_2d_eff_met->GetYaxis()->SetMoreLogLabels();
  // h_mm_pt_2d_eff_met->GetYaxis()->SetTitleOffset(1.0);
  // h_mm_pt_2d_eff_met->Draw("col text");
  
  // CMS_lumi( c_mm_2d_met, iPeriod, iPos );
  // c_mm_2d_met->SaveAs(Form("trigeff_dilep_mm_2d_met%s.pdf",suffix.Data()));
  // c_mm_2d_met->SaveAs(Form("trigeff_dilep_mm_2d_met%s.eps",suffix.Data()));
  // c_mm_2d_met->SaveAs(Form("trigeff_dilep_mm_2d_met%s.png",suffix.Data()));
  

  // --------- emu
  
  t_ht->Draw("lep_pt[1]:lep_pt[0]>>h_em_pt_2d_denom_ht",em+trigs_ht+"ht > 200.");
  t_ht->Draw("lep_pt[1]:lep_pt[0]>>h_em_pt_2d_num_ht",em+trigs_ht+"ht > 200."+trigs_em);

  t_met->Draw("lep_pt[1]:lep_pt[0]>>h_em_pt_2d_denom_met",em+trigs_met+"met_pt > 250.");
  t_met->Draw("lep_pt[1]:lep_pt[0]>>h_em_pt_2d_num_met",em+trigs_met+"met_pt > 250."+trigs_em);

  TCanvas* c_em_2d_ht = new TCanvas("c_em_2d_ht","c_em_2d_ht");
  //  c_em_2d_ht->SetLogx();
  c_em_2d_ht->SetLogy();
  c_em_2d_ht->cd();

  TH2D* h_em_pt_2d_eff_ht = (TH2D*) h_em_pt_2d_num_ht->Clone("h_em_pt_2d_eff_ht");
  h_em_pt_2d_eff_ht->Divide(h_em_pt_2d_denom_ht);
  h_em_pt_2d_eff_ht->GetXaxis()->SetMoreLogLabels();
  h_em_pt_2d_eff_ht->GetYaxis()->SetMoreLogLabels();
  h_em_pt_2d_eff_ht->GetYaxis()->SetTitleOffset(1.0);
  h_em_pt_2d_eff_ht->Draw("col texte");
  
  CMS_lumi( c_em_2d_ht, iPeriod, iPos );
  c_em_2d_ht->SaveAs(Form("trigeff_dilep_em_2d_ht%s.pdf",suffix.Data()));
  c_em_2d_ht->SaveAs(Form("trigeff_dilep_em_2d_ht%s.eps",suffix.Data()));
  c_em_2d_ht->SaveAs(Form("trigeff_dilep_em_2d_ht%s.png",suffix.Data()));

  // TCanvas* c_em_2d_met = new TCanvas("c_em_2d_met","c_em_2d_met");
  // c_em_2d_met->SetLogx();
  // c_em_2d_met->SetLogy();
  // c_em_2d_met->cd();

  // TH2D* h_em_pt_2d_eff_met = (TH2D*) h_em_pt_2d_num_met->Clone("h_em_pt_2d_eff_met");
  // h_em_pt_2d_eff_met->Divide(h_em_pt_2d_denom_met);
  // h_em_pt_2d_eff_met->GetXaxis()->SetMoreLogLabels();
  // h_em_pt_2d_eff_met->GetYaxis()->SetMoreLogLabels();
  // h_em_pt_2d_eff_met->GetYaxis()->SetTitleOffset(1.0);
  // h_em_pt_2d_eff_met->Draw("col text");
  
  // CMS_lumi( c_em_2d_met, iPeriod, iPos );
  // c_em_2d_met->SaveAs(Form("trigeff_dilep_em_2d_met%s.pdf",suffix.Data()));
  // c_em_2d_met->SaveAs(Form("trigeff_dilep_em_2d_met%s.eps",suffix.Data()));
  // c_em_2d_met->SaveAs(Form("trigeff_dilep_em_2d_met%s.png",suffix.Data()));

  f_out->Write();
  f_out->Close();
  
  return;
}

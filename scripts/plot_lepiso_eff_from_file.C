#include "TFile.h"

void plot_lepiso_eff_from_file() {

  gStyle->SetOptStat("");
  gStyle->SetCanvasColor(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetFrameBorderMode(0);

  gStyle->SetPadTopMargin(0.08);
  gStyle->SetPadBottomMargin(0.12);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadRightMargin(0.05);
  
  TFile* fin = new TFile("/home/olivito/cms3/MT2Analysis/LepEffLooperMinimal/output/V00-08-06_nojson_nolepjetcleaning_pfreliso01_allgenleps/ttsl_mg_lo.root");

  TH1D* h_eff_pf_pt_iso_el_temp = (TH1D*) fin->Get("h_eff_pf_pt_iso_el");
  TH1D* h_eff_pf_pt_iso_mu_temp = (TH1D*) fin->Get("h_eff_pf_pt_iso_mu");
  TH1D* h_eff_pf_pt_iso_tau1p_temp = (TH1D*) fin->Get("h_eff_pf_pt_iso_tau1p");
  TH1D* h_eff_pf_pt_iso_tau1p0n_temp = (TH1D*) fin->Get("h_eff_pf_pt_iso_tau1p0n");
  TH1D* h_eff_pf_pt_iso_tau1pgt1n_temp = (TH1D*) fin->Get("h_eff_pf_pt_iso_tau1pgt1n");

  TH1D* h_eff_pf_pt_iso_el = (TH1D*) h_eff_pf_pt_iso_el_temp->Clone("h_eff_pf_pt_iso_el");
  TH1D* h_eff_pf_pt_iso_mu = (TH1D*) h_eff_pf_pt_iso_mu_temp->Clone("h_eff_pf_pt_iso_mu");
  TH1D* h_eff_pf_pt_iso_tau1p = (TH1D*) h_eff_pf_pt_iso_tau1p_temp->Clone("h_eff_pf_pt_iso_tau1p");
  TH1D* h_eff_pf_pt_iso_tau1p0n = (TH1D*) h_eff_pf_pt_iso_tau1p0n_temp->Clone("h_eff_pf_pt_iso_tau1p0n");
  TH1D* h_eff_pf_pt_iso_tau1pgt1n = (TH1D*) h_eff_pf_pt_iso_tau1pgt1n_temp->Clone("h_eff_pf_pt_iso_tau1pgt1n");

  h_eff_pf_pt_iso_el->SetLineColor(kGreen+2);
  h_eff_pf_pt_iso_el->SetMarkerColor(kGreen+2);
  h_eff_pf_pt_iso_mu->SetLineColor(kRed);
  h_eff_pf_pt_iso_mu->SetMarkerColor(kRed);
  h_eff_pf_pt_iso_tau1p->SetLineColor(kBlack);
  h_eff_pf_pt_iso_tau1p->SetMarkerColor(kBlack);
  h_eff_pf_pt_iso_tau1p0n->SetLineColor(kBlue);
  h_eff_pf_pt_iso_tau1p0n->SetMarkerColor(kBlue);
  h_eff_pf_pt_iso_tau1pgt1n->SetLineColor(kMagenta);
  h_eff_pf_pt_iso_tau1pgt1n->SetMarkerColor(kMagenta);
  
  TCanvas* c = new TCanvas("c","c");
  c->SetLogx();
  c->SetGrid(1,1);

  TH2D* h_axis = new TH2D("h_axis",";p_{T} [GeV]; Iso Efficiency wrt PF",195,5,200,100,0,1);
  h_axis->GetXaxis()->SetMoreLogLabels();
  h_axis->GetYaxis()->SetTitleOffset(1.10);
  h_axis->Draw("axis");
  
  //h_eff_pf_pt_iso_el->Draw("histe");
  h_eff_pf_pt_iso_el->Draw("histe same");
  h_eff_pf_pt_iso_mu->Draw("histe same");
  h_eff_pf_pt_iso_tau1p->Draw("histe same");
  h_eff_pf_pt_iso_tau1p0n->Draw("histe same");
  h_eff_pf_pt_iso_tau1pgt1n->Draw("histe same");

  TLegend* leg = new TLegend(0.3,0.15,0.83,0.46);
  leg->AddEntry(h_eff_pf_pt_iso_el,"e","le");
  leg->AddEntry(h_eff_pf_pt_iso_mu,"#mu","le");
  leg->AddEntry(h_eff_pf_pt_iso_tau1p,"1-prong #tau","le");
  leg->AddEntry(h_eff_pf_pt_iso_tau1p0n,"1-prong #tau, 0 neutrals","le");
  leg->AddEntry(h_eff_pf_pt_iso_tau1pgt1n,"1-prong #tau, #geq 1 neutral","le");
  leg->Draw("same");

  c->SaveAs("eff_iso_wrt_pf.pdf");
  c->SaveAs("eff_iso_wrt_pf.eps");

  return;
}

#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TGraph2D.h"
#include "TGraph.h"
#include "TString.h"

#include <iostream>
#include <vector>

float GetValue(float xvalue, TH1F &hist){
  float xmax = hist.GetXaxis()->GetXmax()-0.01;
  int xbin = hist.GetXaxis()->FindBin(min(xvalue, xmax));
  return hist.GetBinContent(xbin);
}

void makeXsecLimits(TString model){

  TFile *f_xsec = new TFile("xsec_susy_13tev.root", "READ");
  TH1F *h_xsec = 0;
  if (model.Contains("TChiWH")) h_xsec = (TH1F*)f_xsec->Get("h_xsec_c1n2");
  else if (model.Contains("T1")) h_xsec = (TH1F*)f_xsec->Get("h_xsec_gluino");
  else if (model.Contains("T2qq")) h_xsec = (TH1F*)f_xsec->Get("h_xsec_squark");
  else if (model.Contains("T2")) h_xsec = (TH1F*)f_xsec->Get("h_xsec_stop");

  TFile *f = new TFile(Form("r-values_%s.root", model.Data()));
  f->cd();

  TH2 *hExp   = (TH2*)f->Get("hExp");
  TH2 *hObs   = (TH2*)f->Get("hObs");
  TH2 *hExp1m = (TH2*)f->Get("hExp1m");
  TH2 *hExp2m = (TH2*)f->Get("hExp2m");
  TH2 *hExp1p = (TH2*)f->Get("hExp1p");
  TH2 *hExp2p = (TH2*)f->Get("hExp2p");

  if(!hExp) std::cout << "hist not found" << std::endl;

  TFile *fout = new TFile(Form("xsec_limits_%s.root", model.Data()), "RECREATE");
  fout->cd();
  
  TH2* hXsec_obs = (TH2*)hObs->Clone("hXsec_obs");
  hXsec_obs->GetXaxis()->SetTitle("m_{#tilde{#chi}^{#pm}_{1}} [GeV]");
  hXsec_obs->GetYaxis()->SetTitle("m_{#tilde{#chi}^{0}_{1}} [GeV]");
  hXsec_obs->GetZaxis()->SetTitle("#sigma limit [pb]");
  TH1D* proj_x_obs = hXsec_obs->ProjectionX();

  for(int i=1; i<=hXsec_obs->GetNbinsX(); i++){
    for(int j=1; j<=hXsec_obs->GetNbinsY(); j++){
      float xsec_limit = hXsec_obs->GetBinContent(i,j);
      if(xsec_limit == 0){
        continue;
      }
      xsec_limit *= GetValue(proj_x_obs->GetBinCenter(i), *h_xsec);
      hXsec_obs->SetBinContent(i,j,xsec_limit);
    }
  } 
  
  TH2* hXsec_exp = (TH2*)hExp->Clone("hXsec_exp"); 
  hXsec_exp->GetXaxis()->SetTitle("m_{#tilde{#chi}^{#pm}_{1}} [GeV]");
  hXsec_exp->GetYaxis()->SetTitle("m_{#tilde{#chi}^{0}_{1}} [GeV]");
  hXsec_exp->GetZaxis()->SetTitle("#sigma limit [pb]");
  TH1D* proj_x_exp = hXsec_exp->ProjectionX();

  for(int i=1; i<=hXsec_exp->GetNbinsX(); i++){
    for(int j=1; j<=hXsec_exp->GetNbinsY(); j++){
      float xsec_limit = hXsec_exp->GetBinContent(i,j);
      if(xsec_limit == 0){
        continue;
      }
      xsec_limit *= GetValue(proj_x_exp->GetBinCenter(i), *h_xsec);
      hXsec_exp->SetBinContent(i,j,xsec_limit);
    }
  } 

  TH2* hXsec_exp1p = (TH2*)hExp1p->Clone("hXsec_exp1p"); 
  hXsec_exp1p->GetXaxis()->SetTitle("m_{#tilde{#chi}^{#pm}_{1}} [GeV]");
  hXsec_exp1p->GetYaxis()->SetTitle("m_{#tilde{#chi}^{0}_{1}} [GeV]");
  hXsec_exp1p->GetZaxis()->SetTitle("#sigma limit [pb]");
  TH1D* proj_x_exp1p = hXsec_exp1p->ProjectionX();

  for(int i=1; i<=hXsec_exp1p->GetNbinsX(); i++){
    for(int j=1; j<=hXsec_exp1p->GetNbinsY(); j++){
      float xsec_limit = hXsec_exp1p->GetBinContent(i,j);
      if(xsec_limit == 0){
        continue;
      }
      xsec_limit *= GetValue(proj_x_exp1p->GetBinCenter(i), *h_xsec);
      hXsec_exp1p->SetBinContent(i,j,xsec_limit);
    }
  } 

  TH2* hXsec_exp1m = (TH2*)hExp1m->Clone("hXsec_exp1m"); 
  hXsec_exp1m->GetXaxis()->SetTitle("m_{#tilde{#chi}^{#pm}_{1}} [GeV]");
  hXsec_exp1m->GetYaxis()->SetTitle("m_{#tilde{#chi}^{0}_{1}} [GeV]");
  hXsec_exp1m->GetZaxis()->SetTitle("#sigma limit [pb]");
  TH1D* proj_x_exp1m = hXsec_exp1m->ProjectionX();

  for(int i=1; i<=hXsec_exp1m->GetNbinsX(); i++){
    for(int j=1; j<=hXsec_exp1m->GetNbinsY(); j++){
      float xsec_limit = hXsec_exp1m->GetBinContent(i,j);
      if(xsec_limit == 0){
        continue;
      }
      xsec_limit *= GetValue(proj_x_exp1m->GetBinCenter(i), *h_xsec);
      hXsec_exp1m->SetBinContent(i,j,xsec_limit);
    }
  } 


  TH2* hXsec_exp2p = (TH2*)hExp2p->Clone("hXsec_exp2p"); 
  hXsec_exp2p->GetXaxis()->SetTitle("m_{#tilde{#chi}^{#pm}_{1}} [GeV]");
  hXsec_exp2p->GetYaxis()->SetTitle("m_{#tilde{#chi}^{0}_{1}} [GeV]");
  hXsec_exp2p->GetZaxis()->SetTitle("#sigma limit [pb]");
  TH1D* proj_x_exp2p = hXsec_exp2p->ProjectionX();

  for(int i=1; i<=hXsec_exp2p->GetNbinsX(); i++){
    for(int j=1; j<=hXsec_exp2p->GetNbinsY(); j++){
      float xsec_limit = hXsec_exp2p->GetBinContent(i,j);
      if(xsec_limit == 0){
        continue;
      }
      xsec_limit *= GetValue(proj_x_exp2p->GetBinCenter(i), *h_xsec);
      hXsec_exp2p->SetBinContent(i,j,xsec_limit);
    }
  } 

  TH2* hXsec_exp2m = (TH2*)hExp2m->Clone("hXsec_exp2m"); 
  hXsec_exp2m->GetXaxis()->SetTitle("m_{#tilde{#chi}^{#pm}_{1}} [GeV]");
  hXsec_exp2m->GetYaxis()->SetTitle("m_{#tilde{#chi}^{0}_{1}} [GeV]");
  hXsec_exp2m->GetZaxis()->SetTitle("#sigma limit [pb]");
  TH1D* proj_x_exp2m = hXsec_exp2m->ProjectionX();

  for(int i=1; i<=hXsec_exp2m->GetNbinsX(); i++){
    for(int j=1; j<=hXsec_exp2m->GetNbinsY(); j++){
      float xsec_limit = hXsec_exp2m->GetBinContent(i,j);
      if(xsec_limit == 0){
        continue;
      }
      xsec_limit *= GetValue(proj_x_exp2m->GetBinCenter(i), *h_xsec);
      hXsec_exp2m->SetBinContent(i,j,xsec_limit);
    }
  } 

  hXsec_obs->Write();
  hXsec_exp->Write();
  hXsec_exp1p->Write();
  hXsec_exp1m->Write();
  hXsec_exp2p->Write();
  hXsec_exp2m->Write();
  //  fout->Write();
  fout->Close();
  delete fout;

  f->Close();
  delete f;

  f_xsec->Close();
  delete f_xsec;

}

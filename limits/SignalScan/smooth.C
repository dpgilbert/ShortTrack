#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TGraph2D.h"
#include "TGraph.h"

#include <iostream>
#include <vector>

float GetValue(float xvalue, TH1F &hist){
  float xmax = hist.GetXaxis()->GetXmax()-0.01;
  int xbin = hist.GetXaxis()->FindBin(min(xvalue, xmax));
  return hist.GetBinContent(xbin);
}

float GetValuePlus(float xvalue, TH1F &hist){
  float xmax = hist.GetXaxis()->GetXmax()-0.01;
  int xbin = hist.GetXaxis()->FindBin(min(xvalue, xmax));
  return hist.GetBinContent(xbin) + hist.GetBinError(xbin);
}

float GetValueMinus(float xvalue, TH1F &hist){
  float xmax = hist.GetXaxis()->GetXmax()-0.01;
  int xbin = hist.GetXaxis()->FindBin(min(xvalue, xmax));
  return hist.GetBinContent(xbin) - hist.GetBinError(xbin);
}

void smooth(std::string model){

  TFile *f_xsec = new TFile("xsec_susy_13tev.root", "READ");
  TH1F *h_xsec = (TH1F*)f_xsec->Get("h_xsec_gluino");//FIXME for Gluino models
  //TH1F *h_xsec = (TH1F*)f_xsec->Get("h_xsec_stop");// for stop/sbottom models

  TFile *f = new TFile(Form("r-values_%s.root", model.c_str()), "UPDATE");
  f->cd();

  TH2 *hExp   = (TH2*)f->Get("hExp");
  TH2 *hObs   = (TH2*)f->Get("hObs");
  TH2 *hExp1m = (TH2*)f->Get("hExp1m");
  TH2 *hExp2m = (TH2*)f->Get("hExp2m");
  TH2 *hExp1p = (TH2*)f->Get("hExp1p");
  TH2 *hExp2p = (TH2*)f->Get("hExp2p");

  if(!hExp) std::cout << "hist not found" << std::endl;

  TH2* hXsec_obs = (TH2*)hObs->Clone("hXsec_obs"); 
  TH1D* proj_x = hXsec_obs->ProjectionX();

  for(int i=1; i<=hXsec_obs->GetNbinsX(); i++){
    for(int j=1; j<=hXsec_obs->GetNbinsY(); j++){
      float xsec_limit = hXsec_obs->GetBinContent(i,j);
      if(xsec_limit == 0){
        continue;
      }
      xsec_limit *= GetValue(proj_x->GetBinCenter(i), *h_xsec);
      hXsec_obs->SetBinContent(i,j,xsec_limit);
    }
  } 


  TH2* hObs1m = (TH2*)hObs->Clone("hObs1m"); 
  TH1D* proj_x_1m = hObs1m->ProjectionX();

  for(int i=1; i<=hObs1m->GetNbinsX(); i++){
    for(int j=1; j<=hObs1m->GetNbinsY(); j++){
      float xsec_limit = hObs1m->GetBinContent(i,j);
      if(xsec_limit == 0){
        continue;
      }
      xsec_limit *= (GetValueMinus(proj_x_1m->GetBinCenter(i), *h_xsec)/GetValue(proj_x_1m->GetBinCenter(i), *h_xsec));
      hObs1m->SetBinContent(i,j,xsec_limit);
    }
  } 

  TH2* hObs1p = (TH2*)hObs->Clone("hObs1p"); 
  TH1D* proj_x_1p = hObs1p->ProjectionX();

  for(int i=1; i<=hObs1p->GetNbinsX(); i++){
    for(int j=1; j<=hObs1p->GetNbinsY(); j++){
      float xsec_limit = hObs1p->GetBinContent(i,j);
      if(xsec_limit == 0){
        continue;
      }
      xsec_limit *= (GetValuePlus(proj_x_1p->GetBinCenter(i), *h_xsec)/GetValue(proj_x_1p->GetBinCenter(i), *h_xsec));
      hObs1p->SetBinContent(i,j,xsec_limit);
    }
  } 


  TGraph2D *hExp_graph   = new TGraph2D(hExp);
  TGraph2D *hObs_graph   = new TGraph2D(hObs);
  TGraph2D *hObs1m_graph   = new TGraph2D(hObs1m);
  TGraph2D *hObs1p_graph   = new TGraph2D(hObs1p);
  TGraph2D *hExp1m_graph = new TGraph2D(hExp1m);
  TGraph2D *hExp2m_graph = new TGraph2D(hExp2m);
  TGraph2D *hExp1p_graph = new TGraph2D(hExp1p);
  TGraph2D *hExp2p_graph = new TGraph2D(hExp2p);
  TGraph2D *hXsec_obs_graph = new TGraph2D(hXsec_obs);

  //hExp_graph->SetNpx(2*hExp_graph->GetNpx());
  //hObs_graph->SetNpx(2*hObs_graph->GetNpx());
  //hObs1m_graph->SetNpx(2*hObs1m_graph->GetNpx());
  //hObs1p_graph->SetNpx(2*hObs1p_graph->GetNpx());
  //hExp1m_graph->SetNpx(2*hExp1m_graph->GetNpx());
  //hExp2m_graph->SetNpx(2*hExp2m_graph->GetNpx());
  //hExp1p_graph->SetNpx(2*hExp1p_graph->GetNpx());
  //hExp2p_graph->SetNpx(2*hExp2p_graph->GetNpx());
  //hXsec_obs_graph->SetNpx(2*hXsec_obs_graph->GetNpx());

  //hExp_graph->SetNpy(2*hExp_graph->GetNpy());
  //hObs_graph->SetNpy(2*hObs_graph->GetNpy());
  //hObs1m_graph->SetNpy(2*hObs1m_graph->GetNpy());
  //hObs1p_graph->SetNpy(2*hObs1p_graph->GetNpy());
  //hExp1m_graph->SetNpy(2*hExp1m_graph->GetNpy());
  //hExp2m_graph->SetNpy(2*hExp2m_graph->GetNpy());
  //hExp1p_graph->SetNpy(2*hExp1p_graph->GetNpy());
  //hExp2p_graph->SetNpy(2*hExp2p_graph->GetNpy());
  //hXsec_obs_graph->SetNpy(2*hXsec_obs_graph->GetNpy());

  TH2D *hExp_smooth   = hExp_graph->GetHistogram();
  TH2D *hObs_smooth   = hObs_graph->GetHistogram();
  TH2D *hObs1m_smooth   = hObs1m_graph->GetHistogram();
  TH2D *hObs1p_smooth   = hObs1p_graph->GetHistogram();
  TH2D *hExp1m_smooth = hExp1m_graph->GetHistogram();
  TH2D *hExp2m_smooth = hExp2m_graph->GetHistogram();
  TH2D *hExp1p_smooth = hExp1p_graph->GetHistogram();
  TH2D *hExp2p_smooth = hExp2p_graph->GetHistogram();
  TH2D *hXsec_obs_smooth = hXsec_obs_graph->GetHistogram();

  hExp_smooth->Write("hExp_smooth",TObject::kOverwrite);
  hObs_smooth->Write("hObs_smooth",TObject::kOverwrite);
  hObs1m_smooth->Write("hObs1m_smooth",TObject::kOverwrite);
  hObs1p_smooth->Write("hObs1p_smooth",TObject::kOverwrite);
  hExp1m_smooth->Write("hExp1m_smooth",TObject::kOverwrite);
  hExp2m_smooth->Write("hExp2m_smooth",TObject::kOverwrite);
  hExp1p_smooth->Write("hExp1p_smooth",TObject::kOverwrite);
  hExp2p_smooth->Write("hExp2p_smooth",TObject::kOverwrite);
  hXsec_obs_smooth->Write("hXsec_obs_smooth",TObject::kOverwrite);
  f->Close();
  delete f;

  f_xsec->Close();
  delete f_xsec;

}

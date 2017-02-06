#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TGraph2D.h"
#include "TGraph.h"

#include <iostream>
#include <vector>

const int ybinsfirstxbin=30;

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

void fillHorizontalBelowZero(TH2 *hist){
  for (int i = 1; i < hist->GetNbinsX()+1; i++) {
    hist->SetBinContent( i,1,hist->GetBinContent(i,2) );
  }
}

void smoothDiagonal(TH2* hist){
  for (int ix = 1; ix < hist->GetNbinsX()+1; ix++) {
    for (int iy = hist->GetNbinsY()-2; iy > 0; iy--) {
      //    if (iy>ix+ybinsfirstxbin) hist->SetBinContent(ix,iy,hist->GetBinContent(ix,iy-1));
      if (hist->GetBinContent(ix,iy)==0) {
	for (int iyy = iy; iyy < hist->GetNbinsY()-2; iyy++) {
	  hist->SetBinContent(ix,iyy,hist->GetBinContent(ix,iy-1));
	}
      }
      else break;
    }
  }
}

void cleanDiagonal(TH2* hist, TH2* histUnsmooth, bool isR){
  for (int ix = 1; ix < hist->GetNbinsX()+1; ix++) {
    for (int iy = hist->GetNbinsY()-2; iy > 0; iy--) {
      //if (iy>ix+ybinsfirstxbin) hist->SetBinContent(ix,iy,0);
      if (histUnsmooth->GetBinContent(ix,iy)==0) {
  	if (isR) hist->SetBinContent(ix,iy,1.1);
  	else hist->SetBinContent(ix,iy,0);
      }
      else break;
    }
  }
}

void smooth(std::string model){

  TFile *f_xsec = new TFile("xsec_susy_13tev.root", "READ");
  TH1F *h_xsec = (TH1F*)f_xsec->Get("h_xsec_gluino");//FIXME for Gluino models
  if ( model.find("T2-4bd") != string::npos || model.find("T2tt") != string::npos)
    h_xsec = (TH1F*)f_xsec->Get("h_xsec_stop");// for stop/sbottom models

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
      xsec_limit *= (GetValue(proj_x_1m->GetBinCenter(i), *h_xsec)/GetValueMinus(proj_x_1m->GetBinCenter(i), *h_xsec));
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
      xsec_limit *= (GetValue(proj_x_1p->GetBinCenter(i), *h_xsec)/GetValuePlus(proj_x_1p->GetBinCenter(i), *h_xsec));
      hObs1p->SetBinContent(i,j,xsec_limit);
    }
  } 

  
  fillHorizontalBelowZero(hExp);
  fillHorizontalBelowZero(hObs);
  fillHorizontalBelowZero(hObs1m);
  fillHorizontalBelowZero(hObs1p);
  fillHorizontalBelowZero(hExp1m);
  fillHorizontalBelowZero(hExp2m);
  fillHorizontalBelowZero(hExp1p);
  fillHorizontalBelowZero(hExp2p);
  fillHorizontalBelowZero(hXsec_obs);
  
  TGraph2D *hExp_graph   = new TGraph2D(hExp);
  TGraph2D *hObs_graph   = new TGraph2D(hObs);
  TGraph2D *hObs1m_graph   = new TGraph2D(hObs1m);
  TGraph2D *hObs1p_graph   = new TGraph2D(hObs1p);
  TGraph2D *hExp1m_graph = new TGraph2D(hExp1m);
  TGraph2D *hExp2m_graph = new TGraph2D(hExp2m);
  TGraph2D *hExp1p_graph = new TGraph2D(hExp1p);
  TGraph2D *hExp2p_graph = new TGraph2D(hExp2p);
  TGraph2D *hXsec_obs_graph = new TGraph2D(hXsec_obs);

  const float width = 12.5;
  hExp_graph->SetNpx((hExp_graph->GetXmax()-hExp_graph->GetXmin())/width);
  hObs_graph->SetNpx((hObs_graph->GetXmax()-hObs_graph->GetXmin())/width);
  hObs1m_graph->SetNpx((hObs1m_graph->GetXmax()-hObs1m_graph->GetXmin())/width);
  hObs1p_graph->SetNpx((hObs1p_graph->GetXmax()-hObs1p_graph->GetXmin())/width);
  hExp1m_graph->SetNpx((hExp1m_graph->GetXmax()-hExp1m_graph->GetXmin())/width);
  hExp2m_graph->SetNpx((hExp2m_graph->GetXmax()-hExp2m_graph->GetXmin())/width);
  hExp1p_graph->SetNpx((hExp1p_graph->GetXmax()-hExp1p_graph->GetXmin())/width);
  hExp2p_graph->SetNpx((hExp2p_graph->GetXmax()-hExp2p_graph->GetXmin())/width);
  hXsec_obs_graph->SetNpx((hXsec_obs_graph->GetXmax()-hXsec_obs_graph->GetXmin())/width);

  hExp_graph->SetNpy((hExp_graph->GetYmax()-hExp_graph->GetYmin())/width);
  hObs_graph->SetNpy((hObs_graph->GetYmax()-hObs_graph->GetYmin())/width);
  hObs1m_graph->SetNpy((hObs1m_graph->GetYmax()-hObs1m_graph->GetYmin())/width);
  hObs1p_graph->SetNpy((hObs1p_graph->GetYmax()-hObs1p_graph->GetYmin())/width);
  hExp1m_graph->SetNpy((hExp1m_graph->GetYmax()-hExp1m_graph->GetYmin())/width);
  hExp2m_graph->SetNpy((hExp2m_graph->GetYmax()-hExp2m_graph->GetYmin())/width);
  hExp1p_graph->SetNpy((hExp1p_graph->GetYmax()-hExp1p_graph->GetYmin())/width);
  hExp2p_graph->SetNpy((hExp2p_graph->GetYmax()-hExp2p_graph->GetYmin())/width);
  hXsec_obs_graph->SetNpy((hXsec_obs_graph->GetYmax()-hXsec_obs_graph->GetYmin())/width);

  TH2D *hExp_smooth   = hExp_graph->GetHistogram();
  TH2D *hObs_smooth   = hObs_graph->GetHistogram();
  TH2D *hObs1m_smooth   = hObs1m_graph->GetHistogram();
  TH2D *hObs1p_smooth   = hObs1p_graph->GetHistogram();
  TH2D *hExp1m_smooth = hExp1m_graph->GetHistogram();
  TH2D *hExp2m_smooth = hExp2m_graph->GetHistogram();
  TH2D *hExp1p_smooth = hExp1p_graph->GetHistogram();
  TH2D *hExp2p_smooth = hExp2p_graph->GetHistogram();
  TH2D *hXsec_obs_smooth = hXsec_obs_graph->GetHistogram();

  TH2D * unsmoothedHist = (TH2D*) hExp_smooth->Clone();
  
  smoothDiagonal(hExp_smooth);
  smoothDiagonal(hObs_smooth);
  smoothDiagonal(hObs1m_smooth);
  smoothDiagonal(hObs1p_smooth);
  smoothDiagonal(hExp1m_smooth);
  smoothDiagonal(hExp2m_smooth);
  smoothDiagonal(hExp1p_smooth);
  smoothDiagonal(hExp2p_smooth);
  smoothDiagonal(hXsec_obs_smooth);
  
  const int nSmooth = 3;
  cout << "Smoothing..." << endl;
  for (int i = 0; i < nSmooth; i++) {
    hExp_smooth->Smooth(1,"k3a");
    hObs_smooth->Smooth(1,"k3a");
    hObs1m_smooth->Smooth(1,"k3a");
    hObs1p_smooth->Smooth(1,"k3a");
    hExp1m_smooth->Smooth(1,"k3a");
    hExp2m_smooth->Smooth(1,"k3a");
    hExp1p_smooth->Smooth(1,"k3a");
    hExp2p_smooth->Smooth(1,"k3a");
    hXsec_obs_smooth->Smooth(1,"k3a");
  }

  cleanDiagonal(hExp_smooth,unsmoothedHist,1);
  cleanDiagonal(hObs_smooth,unsmoothedHist,1);
  cleanDiagonal(hObs1m_smooth,unsmoothedHist,1);
  cleanDiagonal(hObs1p_smooth,unsmoothedHist,1);
  cleanDiagonal(hExp1m_smooth,unsmoothedHist,1);
  cleanDiagonal(hExp2m_smooth,unsmoothedHist,1);
  cleanDiagonal(hExp1p_smooth,unsmoothedHist,1);
  cleanDiagonal(hExp2p_smooth,unsmoothedHist,1);
  cleanDiagonal(hXsec_obs_smooth,unsmoothedHist,0);
  
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

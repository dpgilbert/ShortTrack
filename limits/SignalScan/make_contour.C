#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"
#include "TGraph2D.h"
#include "TGraph.h"

#include <iostream>
#include <vector>

TGraph DrawContours(TGraph2D &g2, int color, int style){
  TGraph out;
  TList *l = g2.GetContourList(1.);
  //l->Print();
  //TList *l = g2.GetContourList(20000.);
  if(!l){
    std::cout << "no contour" << std::endl;
    return out;
  }
  bool added = false;
  int max_points = -1;
  for(int i = 0; i < l->GetSize(); ++i){
    TGraph *g = static_cast<TGraph*>(l->At(i));
    if(!g){
      std::cout << "no graph" << std::endl;
      continue;
    }
    int n_points = g->GetN();
    std::cout << "n_points = " << n_points << std::endl;
    if(n_points > max_points){
      out = *g;
      max_points = n_points;
    }
    g->SetLineColor(color);
    g->SetLineStyle(style);
    g->SetLineWidth(5);
    g->Draw("L same");
  }
  return out;
}

void make_contour(std::string model){

  TFile *f = new TFile(Form("r-values_%s.root", model.c_str()), "UPDATE");
  f->cd();

  TH2 *hExp_smooth   = (TH2*)f->Get("hExp_smooth");
  TH2 *hObs_smooth   = (TH2*)f->Get("hObs_smooth");
  TH2 *hObs1m_smooth   = (TH2*)f->Get("hObs1m_smooth");
  TH2 *hObs1p_smooth   = (TH2*)f->Get("hObs1p_smooth");
  TH2 *hExp1m_smooth = (TH2*)f->Get("hExp1m_smooth");
  TH2 *hExp2m_smooth = (TH2*)f->Get("hExp2m_smooth");
  TH2 *hExp1p_smooth = (TH2*)f->Get("hExp1p_smooth");
  TH2 *hExp2p_smooth = (TH2*)f->Get("hExp2p_smooth");

  if(!hExp_smooth) std::cout << "hist not found" << std::endl;

  TGraph2D *hExp_graph   = new TGraph2D(hExp_smooth);
  TGraph2D *hObs_graph   = new TGraph2D(hObs_smooth);
  TGraph2D *hObs1m_graph   = new TGraph2D(hObs1m_smooth);
  TGraph2D *hObs1p_graph   = new TGraph2D(hObs1p_smooth);
  TGraph2D *hExp1m_graph = new TGraph2D(hExp1m_smooth);
  TGraph2D *hExp2m_graph = new TGraph2D(hExp2m_smooth);
  TGraph2D *hExp1p_graph = new TGraph2D(hExp1p_smooth);
  TGraph2D *hExp2p_graph = new TGraph2D(hExp2p_smooth);

  TCanvas* c1 = new TCanvas();
  hExp_graph->Draw("COLZ");
  c1->Update();
  hObs_graph->Draw("COLZ");
  c1->Update();
  hObs1m_graph->Draw("COLZ");
  c1->Update();
  hObs1p_graph->Draw("COLZ");
  c1->Update();
  hExp1m_graph->Draw("COLZ");
  c1->Update();
  hExp2m_graph->Draw("COLZ");
  c1->Update();
  hExp1p_graph->Draw("COLZ");
  c1->Update();
  hExp2p_graph->Draw("COLZ");
  c1->Update();

  TLegend* l = 0;

  TGraph cup = DrawContours(*hExp1p_graph, 2, 2);
  TGraph cdown = DrawContours(*hExp1m_graph, 2, 2);
  TGraph cexp = DrawContours(*hExp_graph, 2, 1);
  TGraph cobs = DrawContours(*hObs_graph, 1, 1);
  TGraph cobsm = DrawContours(*hObs1m_graph, 1, 1);
  TGraph cobsp = DrawContours(*hObs1p_graph, 1, 1);

  cup.Write("graph_smoothed_ExpP",TObject::kOverwrite);
  cdown.Write("graph_smoothed_ExpM",TObject::kOverwrite);
  cexp.Write("graph_smoothed_Exp",TObject::kOverwrite);
  cobs.Write("graph_smoothed_Obs",TObject::kOverwrite);
  cobsm.Write("graph_smoothed_ObsM",TObject::kOverwrite);
  cobsp.Write("graph_smoothed_ObsP",TObject::kOverwrite);

  f->Close();
  delete f;

}

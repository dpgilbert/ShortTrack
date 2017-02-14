#include <string>
#include <iostream>
#include <algorithm>

#include "TFile.h"
#include "TH1D.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TString.h"
#include "TLegend.h"

#include "CMS_lumi.C"
#include <vector>

using namespace std;

TCanvas *canv;
TString outname = "prova.root";


TH1D* h_Pull = new TH1D("pull", "pull", 20, -4, 4);

/////////////////////////

// USE: root -b -L compareOrig.C

/////////////////////////


TH1D* offsetSlightly(TH1D* h, float offset) {
  
  double *oldbins = h->GetXaxis()->GetXbins()->fArray;
  double newbins[h->GetXaxis()->GetNbins()];
  for (int ibin=1; ibin <= h->GetNbinsX() + 1; ++ibin) {
    newbins[ibin-1] = oldbins[ibin-1] + offset;
    //cout<<oldbins[ibin-1]<<" "<<newbins[ibin-1]<<endl;
  }
  
  TH1D * hnew = new TH1D("h2","h2",h->GetXaxis()->GetNbins(), newbins);
  hnew->SetDirectory(0);

  for (int ibin=1; ibin <= h->GetNbinsX(); ++ibin) {
    hnew->SetBinContent(ibin, h->GetBinContent(ibin));
    hnew->SetBinError(ibin, h->GetBinError(ibin));
  }
  
  return hnew;
}


void makeCMSPlot(  vector<TString> files,  vector<TString> labels, vector<TString> nplots , TString saveas, const string& xtitle , const string& ytitle , float xmin , float xmax , int rebin = 1 , bool logplot = false, float norm = -1., bool doRatio = false ) {
  
  cout << "-- plotting hist: " << nplots[0] << " etc "<<endl;
  
  bool mconly = true;
  for( unsigned int i = 0 ; i < labels.size() ; ++i ){
    if (labels[i].Contains("data") || labels[i].Contains("Data")) mconly = false;
  }
  cmsText = "CMS Preliminary";
//  cmsText = "CMS";
  lumi_13TeV = "36.8 fb^{-1}";
  if (mconly) {
    cmsText = "CMS Preliminary";
    lumi_13TeV = "";
  }

  
  gStyle->SetOptStat("");
  gStyle->SetCanvasColor(0);
  gStyle->SetPadGridX(0);
  //gStyle->SetPadGridY(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetFrameBorderMode(0);
  
  // if ratio was requested, check if data is present.  If not, turn ratio off again
  //  const unsigned int n = samples.size();
  //  if (doRatio) {
  //    bool foundData = false;
  //    for( unsigned int i = 0 ; i < n ; ++i ) {
  //      if( TString(names.at(i)).Contains("data")  ) {
  //        foundData = true;
  //        break;
  //      }
  //    }
  //    if (!foundData) {
  //      cout << "ratio requested but no data hist found.  Not plotting ratio" << endl;
  //      doRatio = false;
  //    }
  //  } // if doRatio
  
  if (!doRatio) {
    //these 4 lines shift plot to make room for axis labels
    gStyle->SetPadTopMargin(0.08);
    gStyle->SetPadBottomMargin(0.12);
    gStyle->SetPadLeftMargin(0.15);
    gStyle->SetPadRightMargin(0.05);
  }
  else {
    gStyle->SetPadTopMargin();
    gStyle->SetPadBottomMargin();
    gStyle->SetPadLeftMargin();
    gStyle->SetPadRightMargin();
  }
  
  TString canvas_name = saveas;
  TCanvas* can = new TCanvas(canvas_name,canvas_name, 600, 600);
  can->cd();
  if (!doRatio) {
    if (logplot) can->SetLogy();
    gPad->SetRightMargin(0.05);
    gPad->Modified();
  }
  
  // splitting canvas for ratio plots
  TPad* fullpad(0);
  TPad* plotpad(0);
  
  if (doRatio) {
    // master pad
    fullpad = new TPad("fullpad","fullpad",0,0,1,1);
    fullpad->Draw();
    fullpad->cd();
    
    // main plot pad, for ratio on bottom
    plotpad = new TPad("plotpad","plotpad",0,0.2,1,0.99);
    plotpad->SetTopMargin(0.05);
    plotpad->SetRightMargin(0.05);
    plotpad->SetBottomMargin(0.05);
    plotpad->Draw();
    plotpad->cd();
    if( logplot ) plotpad->SetLogy();
  }
  
  //TLegend* leg = new TLegend(0.55,0.6,0.85,0.92);
  TLegend* leg = new TLegend(0.55,0.70,0.85,0.90);
  if (saveas.Contains("urity")) leg = new TLegend(0.55,0.25,0.85,0.40);

  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.032);
  if (doRatio) leg->SetTextSize(0.05);
  if (saveas.Contains("purity")) {
    leg->SetX1(0.4);
    leg->SetY1(0.4);
    leg->SetY2(0.55);
    leg->SetTextSize(0.042);
  
  }
  
  TH1D* data_hist(0);
  //  string data_name;
  //  for( unsigned int i = 0 ; i < n ; ++i ) {
  //    if( !TString(names.at(i)).Contains("data")  ) continue;
  //    TString fullhistname = Form("%s/%s",histdir.c_str(),histname.c_str());
  //    if (histdir.size() == 0) fullhistname = TString(histname);
  //    TString newhistname = Form("%s_%s_%s",histname.c_str(),histdir.c_str(),names.at(i).c_str());
  //    TH1D* h_temp = (TH1D*) samples.at(i)->Get(fullhistname);
  //    if (h_temp == 0) continue;
  //    data_hist = (TH1D*) h_temp->Clone(newhistname);
  //    data_name = names.at(i);
  //    //    h->Sumw2();
  //    data_hist->SetLineColor(kBlack);
  //    data_hist->SetMarkerColor(kBlack);
  //    if (rebin > 1) data_hist->Rebin(rebin);
  //
  //    // fake data -> set error bars to correspond to data stats
  //    if (TString(data_name).Contains("fakedata")) {
  //      for (int ibin = 0; ibin <= data_hist->GetNbinsX(); ++ibin) {
  //        data_hist->SetBinError( ibin, sqrt(data_hist->GetBinContent(ibin)) );
  //      }
  //    } // if fakedata
  //
  //    // expect to only find 1 data hist
  //    break;
  //  }
  
  //  if (data_hist) leg->AddEntry(data_hist,getLegendName(data_name).c_str(),"pe");
  
  //  THStack* t = new THStack(Form("stack_%s_%s",histdir.c_str(),histname.c_str()),Form("stack_%s_%s",histdir.c_str(),histname.c_str()));
  TH1D* h_bgtot = 0;
  //
  //  // to make legend and find max yvalue
  //  vector<TH1D*> bg_hists;
  //  vector<string> bg_names;
  vector<TH1D*> sig_hists;
  vector<TString> sig_names;
  
  //  // background hists
  //  for( unsigned int i = 0 ; i < n ; ++i ) {
  //    if( TString(names.at(i)).Contains("data")  ) continue;
  //    if( TString(names.at(i)).Contains("sig")  ) continue;
  //    TString fullhistname = Form("%s/%s",histdir.c_str(),histname.c_str());
  //    if (histdir.size() == 0) fullhistname = TString(histname);
  //    TString newhistname = Form("%s_%s_%s",histname.c_str(),histdir.c_str(),names.at(i).c_str());
  //    TH1D* h_temp = (TH1D*) samples.at(i)->Get(fullhistname);
  //    if (h_temp == 0) continue;
  //    TH1D* h = (TH1D*) h_temp->Clone(newhistname);
  //    //    h->Sumw2();
  //    h->SetFillColor(getColor(names.at(i)));
  //    h->SetLineColor(getColor(names.at(i)));
  //    if (rebin > 1) h->Rebin(rebin);
  //    t->Add(h);
  //    if( h_bgtot==0 ) h_bgtot = (TH1D*) h->Clone(Form("%s_%s_bgtot",histname.c_str(),histdir.c_str()));
  //    else h_bgtot->Add(h);
  //
  //    bg_hists.push_back(h);
  //    bg_names.push_back(names.at(i));
  //  }
  
  //  // loop backwards to add to legend
  //  for (int ibg = (int) bg_hists.size()-1; ibg >= 0; --ibg) {
  //    leg->AddEntry(bg_hists.at(ibg),getLegendName(bg_names.at(ibg)).c_str(),"f");
  //  }
  //
  // signal hists - all samples must have "sig" in the name
  float intFirstSample = 0;
  int icolor = 0;
  int countOffset = 0;
  for( unsigned int i = 0 ; i < files.size() ; ++i ){
    
    if (files[i]=="") {
      leg->SetHeader(labels[i]);
      continue;
    }
    
    TFile f1( files[i], "READ");
    if(!f1.IsOpen()){
      std::cout<<"File "<<files[i]<<" can't be found, will not be used for histogram "<<nplots[i]<<std::endl;
      continue;
    }
    if(f1.Get(nplots[i].Data())==0){
      std::cout<<"Histogram "<<nplots[i]<<" not found in file "<<files[i]<<std::endl;
      continue;
    }
    
    TH1D* h_temp = (TH1D*) f1.Get(nplots[i]);
    if (h_temp == 0) continue;
    // don't draw signal if the total yield in the plot is < 0.1 events
    //if (h_temp->Integral(0,-1) < 0.1) continue;
    TH1D* h = (TH1D*) h_temp->Clone("h1");
    h->SetDirectory(0);
    
//    if (sig_hists.size()>0  && saveas.Contains("MT2") && saveas.Contains("W_GJ")) {
    if (saveas.Contains("MT2") && saveas.Contains("W_GJ")) {
      h = offsetSlightly(h, 20*countOffset);
      if (countOffset > 0) countOffset *= -1;
      else if (countOffset < 0) { countOffset *= -1; countOffset++;}
      else countOffset++;
    }
    
    //    h->Sumw2();
    h->SetLineColor(1+icolor);
    h->SetMarkerColor(1+icolor);
    if (1+icolor==3) {
      h->SetLineColor(kGreen+1);
      h->SetMarkerColor(kGreen+1);
    }
    h->SetMarkerStyle(21+icolor);
    h->SetMarkerSize(1);
    h->SetLineWidth(2);
    h->GetXaxis()->SetTitle(xtitle.c_str());

    
    if (rebin > 1) h->Rebin(rebin);
    if (norm) {
      h->Scale(1/h->Integral(0, -1));
    }
    sig_hists.push_back(h);
    sig_names.push_back(labels[i]);
    icolor++;
  }

  // Error bars on Zinv MC shape
  bool drawBand = false;
  if (sig_names.at(0).Contains("Zinv MC") || sig_names.at(0).Contains("Z #rightarrow #nu#bar{#nu}")) drawBand = true;
  if (drawBand) {
    int lastNonZeroBin = 1;
    for (int ibin=1; ibin <= sig_hists.at(0)->GetNbinsX(); ++ibin)
      if ( sig_hists.at(0)->GetBinContent(ibin) > 0) lastNonZeroBin = ibin;
    float firstBinUnc = 0;
    for (int ibin=1; ibin <= sig_hists.at(0)->GetNbinsX(); ++ibin) {
      float cont = sig_hists.at(0)->GetBinContent(ibin);
      float percenterr = 0.4 / (lastNonZeroBin - 1) * (ibin -1);
      if (ibin>1) {
        sig_hists.at(0)->SetBinError(ibin, cont*percenterr);
        firstBinUnc += cont*percenterr;
      }
    }
    if (firstBinUnc > sig_hists.at(0)->GetBinContent(1)) firstBinUnc = sig_hists.at(0)->GetBinContent(1); // max to 100%
    sig_hists.at(0)->SetBinError( 1,  firstBinUnc);
  }

  float ymax = 0;
  float ymin = 0.11;
  //if(h_bgtot) ymax = h_bgtot->GetMaximum();
  // also check signals for max val
  for (unsigned int isig = 0; isig < sig_hists.size(); ++isig) {
    if (sig_hists.at(isig)->GetMaximum() > ymax) ymax = sig_hists.at(isig)->GetMaximum();
    if (sig_hists.at(isig)->GetMinimum() < ymin) ymin = sig_hists.at(isig)->GetMinimum();
  }
//  if( logplot ) ymax*=30;
  if( logplot ) ymax*=2;
  else          ymax*=1.5;
  if (norm) ymin = 0;
  if (saveas.Contains("urity")) { ymin = 0.7; ymax = 1;}
  if (norm && logplot ) ymin = 0.00011;
  
  if ( xmin==0 ) xmin = sig_hists.at(0)->GetXaxis()->GetXmin();
  if ( xmax==0 ) xmax = sig_hists.at(0)->GetXaxis()->GetXmax();

  
  TH2F* h_axes = new TH2F(nplots[0],"",1000,xmin,xmax,1000,ymin,ymax);
  h_axes->GetXaxis()->SetTitle(xtitle.c_str());
  h_axes->GetXaxis()->SetLabelSize(0.04);
  h_axes->GetXaxis()->SetTitleSize(0.05);
  h_axes->GetYaxis()->SetTitle(ytitle.c_str());
  h_axes->GetYaxis()->SetLabelSize(0.04);
//  h_axes->GetYaxis()->SetTitleOffset(1.5);
  h_axes->GetYaxis()->SetTitleOffset(1);
  h_axes->GetYaxis()->SetTitleSize(0.05);
  if (doRatio) {
    h_axes->GetXaxis()->SetLabelSize(0.);
    h_axes->GetXaxis()->SetTitleSize(0.);
  }
  
  
  if (nplots[0].Contains("dijetflav")) {
    h_axes->GetXaxis()->SetBinLabel(100, "jj");
    h_axes->GetXaxis()->SetBinLabel(250, "cj");
    h_axes->GetXaxis()->SetBinLabel(420, "bj");
    h_axes->GetXaxis()->SetBinLabel(590, "cc");
    h_axes->GetXaxis()->SetBinLabel(760, "bc");
    h_axes->GetXaxis()->SetBinLabel(900, "bb");
    h_axes->GetXaxis()->SetLabelSize(0.05);
    h_axes->GetXaxis()->LabelsOption("h");

    //h1->GetXaxis()->SetLabelOffset(0.02);
    
  }
  
  h_axes->Draw();
  
  //t->Draw("hist same");
  
  // add signal hists
  for (unsigned int isig = 0; isig < sig_hists.size(); ++isig) {
    //sig_hists.at(isig)->Draw("hist same");
    if (saveas.Contains("MT2") && saveas.Contains("W_GJ") && isig > 0) {
      sig_hists.at(isig)->Draw("same EX0");

    }
    else sig_hists.at(isig)->Draw("same");
    leg->AddEntry(sig_hists.at(isig),sig_names.at(isig),"lp");
  }
  
  //if (data_hist) data_hist->Draw("pe same");
  
  TLatex label;
  label.SetNDC();
  label.SetTextSize(0.032);
  float label_y_start = 0.82;
  float label_y_spacing = 0.04;
  if (doRatio) {
    label.SetTextSize(0.039);
    label_y_start = 0.84;
    label_y_spacing = 0.04;
  }
  
//  FOR SUPPLEMENTARY PLOTS
//  TLatex labelA;
//  labelA.SetNDC();
//  labelA.SetTextSize(0.039);
//  labelA.SetTextFont(42);
//  labelA.DrawLatex(0.5, 0.96, "arXiv:1603.04053");
  

  
  //  //TString ht_label = getHTPlotLabel(histdir);
  //  TString ht_label = getHTPlotLabel(samples.at(0), histdir);
  //  TString region_label = getJetBJetPlotLabel(samples.at(0), histdir);
  //  TString region_label_line2 = getMT2PlotLabel(samples.at(0), histdir);
  //  //label.DrawLatex(0.2,0.85,ht_label);
  //  label.DrawLatex(0.187,label_y_start,ht_label);
  //  // base region plots all have at least 2 jets
  //  if ((histdir.find("base") != std::string::npos)) region_label = "#geq 2j";
  //  // minMT plot always requires at least 2 bjets
  //  if ((histdir.find("srbase") != std::string::npos) && (histname.find("minMTBMet") != std::string::npos)) region_label = "#geq 2j, #geq 2b";
  //  // lostlepton CR
  //  if ((histdir.find("crsl") != std::string::npos)) region_label += ", 1 lepton";
  //
  //  if (region_label.Length() > 0) label.DrawLatex(0.187,label_y_start - label_y_spacing,region_label);
  //  if (region_label_line2.Length() > 0) label.DrawLatex(0.187,label_y_start - 2 * label_y_spacing,region_label_line2);
  
  leg->Draw();
  //h_axes->Draw("axissame");
  
  
  // -- for CMS_lumi label
  
  const int iPeriod = 4; // 13 tev
  
  // iPos drives the position of the CMS logo in the plot
  // iPos=11 : top-left, left-aligned
  // iPos=33 : top-right, right-aligned
  // iPos=22 : center, centered
  // mode generally :
  //   iPos = 10*(alignement 1/2/3) + position (1/2/3 = left/center/right)
  //const int iPos = 11;
  const int iPos = 3; // try out of frame
  
  
  if (doRatio) {
    lumiTextSize     = 0.8;
    cmsTextSize      = 1.0;
    CMS_lumi( plotpad, iPeriod, iPos );
  }
  else {
    lumiTextSize     = 0.45;
    cmsTextSize      = 0.55;
    CMS_lumi( can, iPeriod, iPos );
  }
  
  // make ratio pad and plot
  if (doRatio) {
    // draw ratio pad
    fullpad->cd();
    //plotpad = new TPad("plotpad","plotpad",0,0.2,1,0.99);
    //plotpad->SetTopMargin(0.05);
    //plotpad->SetRightMargin(0.05);
    //plotpad->SetBottomMargin(0.05);
    TPad* ratiopad = new TPad("ratiopad","ratiopad",0.,0.,1,0.23);
    ratiopad->SetLeftMargin(0.1);//(0.16);
    ratiopad->SetRightMargin(0.05);
    ratiopad->SetTopMargin(0.04);
    ratiopad->SetBottomMargin(0.44);
    ratiopad->SetGridy(1);
    ratiopad->Draw();
    ratiopad->cd();
    
    
    // draw axis only
    TH1F* h_axis_ratio = new TH1F(Form("%s_axes",sig_hists[0]->GetName()),"",100,xmin,xmax);
    h_axis_ratio->GetYaxis()->SetTitleOffset(0.25);
    h_axis_ratio->GetYaxis()->SetTitleSize(0.18);
    h_axis_ratio->GetYaxis()->SetNdivisions(5);
    h_axis_ratio->GetYaxis()->SetLabelSize(0.15);
    //h_axis_ratio->GetYaxis()->SetRangeUser(0.01,1.99);
    h_axis_ratio->GetYaxis()->SetRangeUser(0.5,1.5);
    //h_axis_ratio->GetYaxis()->SetRangeUser(0.001,1.);
//    h_axis_ratio->GetYaxis()->SetTitle("Ratio to Z");
    h_axis_ratio->GetYaxis()->SetTitle("Ratio");
    h_axis_ratio->GetXaxis()->SetTitle(sig_hists[1]->GetXaxis()->GetTitle());
    h_axis_ratio->GetXaxis()->SetTitleSize(0.17);
    h_axis_ratio->GetXaxis()->SetLabelSize(0.17);
    h_axis_ratio->GetXaxis()->SetTitleOffset(1);
    h_axis_ratio->GetXaxis()->SetTickLength(0.07);
    h_axis_ratio->Draw("axis");
    
    if (drawBand) {
      TH1D* h_band = (TH1D*) sig_hists[0]->Clone("band");
      for (int ibin=1; ibin <= h_band->GetNbinsX(); ++ibin) {
        if (h_band->GetBinContent(ibin)>0.) h_band->SetBinError(ibin, h_band->GetBinError(ibin)/h_band->GetBinContent(ibin));
        h_band->SetBinContent(ibin, 1.);
      }
      h_band->SetMarkerSize(0);
      h_band->SetFillColor (kGray+2);
      h_band->SetFillStyle (3344);
      h_band->Draw("E2,same");
    }
      
    for (unsigned int isig = 1; isig < sig_hists.size(); ++isig) {
      //    TH1D* h_ratio = (TH1D*) data_hist->Clone(Form("ratio_%s",data_hist->GetName()));
      TH1D* h_ratio = (TH1D*) sig_hists[isig]->Clone(Form("ratio_%s",sig_hists[isig]->GetName()));
      h_ratio->Sumw2();
      //sig_hists[1]->Sumw2();
      if (drawBand) { // don't put uncertainty in ratio, since we'll have the band
        TH1D* h_tmp = (TH1D*) sig_hists[0]->Clone("tmp");
        for (int ibin=1; ibin <= h_ratio->GetNbinsX(); ++ibin)  h_tmp->SetBinError(ibin, 0.);
        h_ratio->Divide(h_tmp);
      }
      else {
        if ( h_ratio->GetNbinsX() == sig_hists[0]->GetNbinsX() ) h_ratio->Divide(sig_hists[0]); 
        else {
          cout<<h_ratio->GetNbinsX()<<endl;
          double *TopoRegionBins = h_ratio->GetXaxis()->GetXbins()->fArray;  
          int nTopoRegionBins    = h_ratio->GetXaxis()->GetNbins();        
          TH1D* h_Rebinned = (TH1D*) sig_hists[0]->Rebin(nTopoRegionBins, "h_RebinnedTemplate", TopoRegionBins);
          h_ratio->Divide(h_Rebinned); 
        }
      }
    
      for (int ibin=1; ibin <= h_ratio->GetNbinsX(); ++ibin) {
//        cout<<"Difference "<< h_ratio->GetBinContent(ibin) - 1 <<", uncertainty "<<h_ratio->GetBinError(ibin)<<endl;
//        cout<<"Pull "<<(h_ratio->GetBinContent(ibin) - 1)/h_ratio->GetBinError(ibin)<<endl;
        h_Pull->Fill((h_ratio->GetBinContent(ibin) - 1)/h_ratio->GetBinError(ibin));
      }
      
      TGraphErrors* g_ratio = new TGraphErrors(h_ratio);
      g_ratio->SetName(Form("%s_graph",h_ratio->GetName()));
      for (int ibin=0; ibin < h_ratio->GetNbinsX(); ++ibin) {
        if (saveas.Contains("MT2") && saveas.Contains("W_GJ")) {
          g_ratio->SetPointError(ibin, 0, h_ratio->GetBinError(ibin+1));
        }
        else g_ratio->SetPointError(ibin, h_ratio->GetBinWidth(ibin+1)/2., h_ratio->GetBinError(ibin+1));
      }
      //g_ratio->SetLineColor(kBlack);
      //g_ratio->SetMarkerColor(kBlack);
      //g_ratio->SetMarkerStyle(20);
      g_ratio->Draw("p0same");
    }
    
  } // if (doRatio)
  
  gPad->Modified();
  
  can->Print(Form("compareMultiPlot/%s.pdf",canvas_name.Data()));
  
  
  return;
}


void makeCMSPlotRatios(  vector<TString> files,  vector<TString> labels, vector<TString> nplots , TString saveas, const string& xtitle , const string& ytitle , float xmin , float xmax , int rebin = 1 , bool logplot = false, float norm = -1., bool doRatio = false ) {
  
  cout << "-- plotting hist: " << nplots[0] << " etc "<<endl;
  
  bool mconly = true;
  for( unsigned int i = 0 ; i < labels.size() ; ++i ){
    if (labels[i].Contains("data") || labels[i].Contains("Data")) mconly = false;
  }
  cmsText = "CMS Preliminary";
  lumi_13TeV = "36.8 fb^{-1}";
  if (mconly) {
    cmsText = "CMS Preliminary";
    lumi_13TeV = "";
  }
  
  
  gStyle->SetOptStat("");
  gStyle->SetCanvasColor(0);
  gStyle->SetPadGridX(0);
  gStyle->SetPadGridY(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetFrameBorderMode(0);
  
  // if ratio was requested, check if data is present.  If not, turn ratio off again
  //  const unsigned int n = samples.size();
  //  if (doRatio) {
  //    bool foundData = false;
  //    for( unsigned int i = 0 ; i < n ; ++i ) {
  //      if( TString(names.at(i)).Contains("data")  ) {
  //        foundData = true;
  //        break;
  //      }
  //    }
  //    if (!foundData) {
  //      cout << "ratio requested but no data hist found.  Not plotting ratio" << endl;
  //      doRatio = false;
  //    }
  //  } // if doRatio
  
  if (!doRatio) {
    //these 4 lines shift plot to make room for axis labels
    gStyle->SetPadTopMargin(0.08);
    gStyle->SetPadBottomMargin(0.12);
    gStyle->SetPadLeftMargin(0.15);
    gStyle->SetPadRightMargin(0.05);
  }
  else {
    gStyle->SetPadTopMargin();
    gStyle->SetPadBottomMargin();
    gStyle->SetPadLeftMargin();
    gStyle->SetPadRightMargin();
  }
  
  TString canvas_name = saveas;
  TCanvas* can = new TCanvas(canvas_name,canvas_name, 600, 600);
  can->cd();
  if (!doRatio) {
    if (logplot) can->SetLogy();
    gPad->SetRightMargin(0.05);
    gPad->Modified();
  }
  
  // splitting canvas for ratio plots
  TPad* fullpad(0);
  TPad* plotpad(0);
  
  if (doRatio) {
    // master pad
    fullpad = new TPad("fullpad","fullpad",0,0,1,1);
    fullpad->Draw();
    fullpad->cd();
    
    // main plot pad, for ratio on bottom
    plotpad = new TPad("plotpad","plotpad",0,0.2,1,0.99);
    plotpad->SetTopMargin(0.05);
    plotpad->SetRightMargin(0.05);
    plotpad->SetLeftMargin(0.15);
    plotpad->SetBottomMargin(0.05);
    plotpad->Draw();
    plotpad->cd();
    if( logplot ) plotpad->SetLogy();
  }
  
  //TLegend* leg = new TLegend(0.55,0.6,0.85,0.92);
  TLegend* leg = new TLegend(0.55,0.7,0.85,0.90);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.032);
  if (doRatio) leg->SetTextSize(0.05);
  
  TH1D* data_hist(0);
  //  string data_name;
  //  for( unsigned int i = 0 ; i < n ; ++i ) {
  //    if( !TString(names.at(i)).Contains("data")  ) continue;
  //    TString fullhistname = Form("%s/%s",histdir.c_str(),histname.c_str());
  //    if (histdir.size() == 0) fullhistname = TString(histname);
  //    TString newhistname = Form("%s_%s_%s",histname.c_str(),histdir.c_str(),names.at(i).c_str());
  //    TH1D* h_temp = (TH1D*) samples.at(i)->Get(fullhistname);
  //    if (h_temp == 0) continue;
  //    data_hist = (TH1D*) h_temp->Clone(newhistname);
  //    data_name = names.at(i);
  //    //    h->Sumw2();
  //    data_hist->SetLineColor(kBlack);
  //    data_hist->SetMarkerColor(kBlack);
  //    if (rebin > 1) data_hist->Rebin(rebin);
  //
  //    // fake data -> set error bars to correspond to data stats
  //    if (TString(data_name).Contains("fakedata")) {
  //      for (int ibin = 0; ibin <= data_hist->GetNbinsX(); ++ibin) {
  //        data_hist->SetBinError( ibin, sqrt(data_hist->GetBinContent(ibin)) );
  //      }
  //    } // if fakedata
  //
  //    // expect to only find 1 data hist
  //    break;
  //  }
  
  //  if (data_hist) leg->AddEntry(data_hist,getLegendName(data_name).c_str(),"pe");
  
  //  THStack* t = new THStack(Form("stack_%s_%s",histdir.c_str(),histname.c_str()),Form("stack_%s_%s",histdir.c_str(),histname.c_str()));
  TH1D* h_bgtot = 0;
  //
  //  // to make legend and find max yvalue
  //  vector<TH1D*> bg_hists;
  //  vector<string> bg_names;
  vector<TH1D*> sig_hists;
  vector<TString> sig_names;
  
  //  // background hists
  //  for( unsigned int i = 0 ; i < n ; ++i ) {
  //    if( TString(names.at(i)).Contains("data")  ) continue;
  //    if( TString(names.at(i)).Contains("sig")  ) continue;
  //    TString fullhistname = Form("%s/%s",histdir.c_str(),histname.c_str());
  //    if (histdir.size() == 0) fullhistname = TString(histname);
  //    TString newhistname = Form("%s_%s_%s",histname.c_str(),histdir.c_str(),names.at(i).c_str());
  //    TH1D* h_temp = (TH1D*) samples.at(i)->Get(fullhistname);
  //    if (h_temp == 0) continue;
  //    TH1D* h = (TH1D*) h_temp->Clone(newhistname);
  //    //    h->Sumw2();
  //    h->SetFillColor(getColor(names.at(i)));
  //    h->SetLineColor(getColor(names.at(i)));
  //    if (rebin > 1) h->Rebin(rebin);
  //    t->Add(h);
  //    if( h_bgtot==0 ) h_bgtot = (TH1D*) h->Clone(Form("%s_%s_bgtot",histname.c_str(),histdir.c_str()));
  //    else h_bgtot->Add(h);
  //
  //    bg_hists.push_back(h);
  //    bg_names.push_back(names.at(i));
  //  }
  
  //  // loop backwards to add to legend
  //  for (int ibg = (int) bg_hists.size()-1; ibg >= 0; --ibg) {
  //    leg->AddEntry(bg_hists.at(ibg),getLegendName(bg_names.at(ibg)).c_str(),"f");
  //  }
  //
  // signal hists - all samples must have "sig" in the name
  float intFirstSample = 0;
  int icolor = 0;
  for( unsigned int i = 0 ; i < files.size() ; ++i ){
    if (files[i]=="") {
      leg->SetHeader(labels[i]);
      continue;
    }
    
    TFile f1( files[i], "READ");
    TFile f2( files[i+1], "READ");
    if(!f1.IsOpen()){
      std::cout<<"File "<<files[i]<<" can't be found, will not be used for histogram "<<nplots[i]<<std::endl;
      continue;
    }
    if(!f2.IsOpen()){
      std::cout<<"File "<<files[i+1]<<" can't be found, will not be used for histogram "<<nplots[i]<<std::endl;
      continue;
    }
    if(f1.Get(nplots[i].Data())==0){
      std::cout<<"Histogram "<<nplots[i]<<" not found in file "<<files[i]<<std::endl;
      continue;
    }
    if(f2.Get(nplots[i+1].Data())==0){
      std::cout<<"Histogram "<<nplots[i]<<" not found in file "<<files[i+1]<<std::endl;
      continue;
    }
    
    
    TH1D* h_temp = (TH1D*) f1.Get(nplots[i])->Clone();
    TH1D* h_temp2 = (TH1D*) f2.Get(nplots[i+1])->Clone();
    if (h_temp == 0) {cout<<"No histogram in f1 "<<nplots[i]<<endl; continue;}
    if (h_temp2 == 0) {cout<<"No histogram in f2 "<<nplots[i+1]<<endl; continue;}
    h_temp2->Sumw2();h_temp->Sumw2();

    // Patch to add fakes to MC
//    if (files[i+1].Contains("qcdplusgjet")) {
//      TH1D* h_temp3 = (TH1D*) f2.Get(nplots[i+1]+"Fake");
//      if (h_temp3 == 0) {cout<<"No histogram in qcdplusjet "<<nplots[i+1]<<"Fake"<<endl; continue;}
//      h_temp2->Add(h_temp3);
//    }
    // Patch to subtract data-driven fakes from MC plots of htbins, njbins, nbjbins
    if (files[i+1].Contains("data_Run2016")  && labels[i+1].Contains("bkg subtr") && nplots[i+1].Contains("crgjbase")
          && ( nplots[i+1].Contains("htbins") || nplots[i+1].Contains("njbins") || nplots[i+1].Contains("nbjbins") || nplots[i+1].Contains("bosonptbins"))) {
      TString purityfile = files[i+1].ReplaceAll("data_Run2016", "purity");
      cout<<"Taking fakes from file "<<purityfile<<endl;
      TFile f3(purityfile,"READ");
      TString fakesplot = nplots[i+1];
      fakesplot.ReplaceAll("crgj","sr");
      if (fakesplot.Contains("htbins")) fakesplot.ReplaceAll("h_htbins","h_predhtbinsFailSieieData");
      if (fakesplot.Contains("njbins")) fakesplot.ReplaceAll("h_njbins","h_prednjbinsFailSieieData");
      if (fakesplot.Contains("nbjbins")) fakesplot.ReplaceAll("h_nbjbins","h_prednbjbinsFailSieieData");
      if (fakesplot.Contains("bosonptbins")) fakesplot.ReplaceAll("h_bosonptbins","h_predbosonptbinsFailSieieData");
      TH1D* h_temp3 = (TH1D*) f3.Get(fakesplot);
      if (h_temp3 == 0) {cout<<"No histogram "<<fakesplot<<" in "<<purityfile<<endl; return;}
      h_temp2->Add(h_temp3, -1);
      for (int ibin=1; ibin <= h_temp2->GetNbinsX(); ++ibin) {if (h_temp2->GetBinContent(ibin)<0) {h_temp2->SetBinContent(ibin,0);h_temp2->SetBinError(ibin,0);}}
      h_temp2->Scale(0.92);
      //cout<<"____________________________________Adjusting purity by 2% to match ETH_______________________________________"<<endl;
      //h_temp2->Scale(1.02);
      // Add 10% uncertainty for p and f
      for (int ibin=1; ibin <= h_temp2->GetNbinsX(); ++ibin) {
        float tenpercent = 0.1*h_temp2->GetBinContent(ibin);
        float err = h_temp2->GetBinError(ibin);
        h_temp2->SetBinError(ibin, TMath::Sqrt(err*err + tenpercent*tenpercent) );
      }
      h_temp2->Print("all");

    }
    //subtrac ttbar from DY
    if (files[i].Contains("data_Run2016")  && labels[i].Contains("bkg subtr") && nplots[i].Contains("crdybase")
        && ( nplots[i].Contains("htbins") || nplots[i].Contains("njbins") || nplots[i].Contains("nbjbins") || nplots[i+1].Contains("bosonptbins") || nplots[i].Contains("mt2bins"))) {
      TString topfile = files[i].ReplaceAll("data_Run2016", "top");
      cout<<"Taking ttbar from file "<<topfile<<endl;
      TFile f3(topfile,"READ");
      TString topplot = nplots[i];
      TH1D* h_temp3 = (TH1D*) f3.Get(topplot);
      if (h_temp3 == 0) {cout<<"No histogram "<<topplot<<" in "<<topfile<<endl; return;}
      h_temp->Add(h_temp3, -1);
      for (int ibin=1; ibin <= h_temp->GetNbinsX(); ++ibin) {if (h_temp->GetBinContent(ibin)<0) {h_temp->SetBinContent(ibin,0);h_temp->SetBinError(ibin,0);}}
      // Add 50% uncertainty on ttbar
      h_temp->Print("all");
      for (int ibin=1; ibin <= h_temp->GetNbinsX(); ++ibin) {
        float fiftypercent = 0.5*h_temp3->GetBinContent(ibin);
        float err = h_temp->GetBinError(ibin);
        h_temp->SetBinError(ibin, TMath::Sqrt(err*err + fiftypercent*fiftypercent) );
      }
      h_temp->Print("all");
    }

    
    // don't draw signal if the total yield in the plot is < 0.1 events
    //if (h_temp->Integral(0,-1) < 0.1) continue;
    if (rebin > 1) h_temp->Rebin(rebin);
    if (norm) {
      h_temp->Scale(1/h_temp->Integral(0, -1));
    }
    if (rebin > 1) h_temp2->Rebin(rebin);
    if (norm) {
      h_temp2->Scale(1/h_temp2->Integral(0, -1));
    }
    if (files[i+1].Contains("gjet") && saveas.Contains("DoubleRatio")) {
      h_temp2->Scale(1.23);
      cout<<"Scaled GJet by 1.23 to invert Z kFactor"<<endl;
    }

    TH1D* h = (TH1D*) h_temp->Clone("h1");

    h->Divide(h_temp2);
    h->SetDirectory(0);
    h->SetLineColor(1+icolor);
    h->SetMarkerColor(1+icolor);
    h->SetMarkerStyle(21+icolor);
    h->SetMarkerSize(1);
    h->SetLineWidth(2);
    h->GetXaxis()->SetTitle(xtitle.c_str());
    sig_hists.push_back(h);
    sig_names.push_back(labels[i]);
    i++;
    icolor++;
  }
  
  float ymax = 0;
  //if(h_bgtot) ymax = h_bgtot->GetMaximum();
  // also check signals for max val
  for (unsigned int isig = 0; isig < sig_hists.size(); ++isig) {
    if (sig_hists.at(isig)->GetMaximum() > ymax) ymax = sig_hists.at(isig)->GetMaximum();
  }
  //  if( logplot ) ymax*=30;
  if( logplot ) ymax*=2;
  else          ymax*=1.5;
  float ymin = 0.01;
  if (norm) ymin = 0;
  
  
  if ( xmin==0 ) xmin = sig_hists.at(0)->GetXaxis()->GetXmin();
  if ( xmax==0 ) xmax = sig_hists.at(0)->GetXaxis()->GetXmax();
  
  
  TH2F* h_axes = new TH2F(nplots[0],"",1000,xmin,xmax,1000,ymin,ymax);
  h_axes->GetXaxis()->SetTitle(xtitle.c_str());
  h_axes->GetXaxis()->SetLabelSize(0.04);
  h_axes->GetXaxis()->SetTitleSize(0.05);
  h_axes->GetYaxis()->SetTitle(ytitle.c_str());
  h_axes->GetYaxis()->SetLabelSize(0.04);
  //  h_axes->GetYaxis()->SetTitleOffset(1.5);
  h_axes->GetYaxis()->SetTitleOffset(1);
  h_axes->GetYaxis()->SetTitleSize(0.05);
  if (doRatio) {
    h_axes->GetXaxis()->SetLabelSize(0.);
    h_axes->GetXaxis()->SetTitleSize(0.);
  }
  h_axes->Draw();
  
  //t->Draw("hist same");
  
  // add signal hists
  for (unsigned int isig = 0; isig < sig_hists.size(); ++isig) {
    //sig_hists.at(isig)->Draw("hist same");
    sig_hists.at(isig)->Draw("same");
    leg->AddEntry(sig_hists.at(isig),sig_names.at(isig),"lp");
  }
  
  //if (data_hist) data_hist->Draw("pe same");

  TLatex label;
  label.SetNDC();
  label.SetTextSize(0.032);
  float label_y_start = 0.82;
  float label_y_spacing = 0.04;
  if (doRatio) {
    label.SetTextSize(0.039);
    label_y_start = 0.84;
    label_y_spacing = 0.04;
  }
  
  //  //TString ht_label = getHTPlotLabel(histdir);
  //  TString ht_label = getHTPlotLabel(samples.at(0), histdir);
  //  TString region_label = getJetBJetPlotLabel(samples.at(0), histdir);
  //  TString region_label_line2 = getMT2PlotLabel(samples.at(0), histdir);
  //  //label.DrawLatex(0.2,0.85,ht_label);
  //  label.DrawLatex(0.187,label_y_start,ht_label);
  //  // base region plots all have at least 2 jets
  //  if ((histdir.find("base") != std::string::npos)) region_label = "#geq 2j";
  //  // minMT plot always requires at least 2 bjets
  //  if ((histdir.find("srbase") != std::string::npos) && (histname.find("minMTBMet") != std::string::npos)) region_label = "#geq 2j, #geq 2b";
  //  // lostlepton CR
  //  if ((histdir.find("crsl") != std::string::npos)) region_label += ", 1 lepton";
  //
  //  if (region_label.Length() > 0) label.DrawLatex(0.187,label_y_start - label_y_spacing,region_label);
  //  if (region_label_line2.Length() > 0) label.DrawLatex(0.187,label_y_start - 2 * label_y_spacing,region_label_line2);
  
  leg->Draw();
  //h_axes->Draw("axissame");
  
  
  // -- for CMS_lumi label
  
  const int iPeriod = 4; // 13 tev
  
  // iPos drives the position of the CMS logo in the plot
  // iPos=11 : top-left, left-aligned
  // iPos=33 : top-right, right-aligned
  // iPos=22 : center, centered
  // mode generally :
  //   iPos = 10*(alignement 1/2/3) + position (1/2/3 = left/center/right)
  //const int iPos = 11;
  const int iPos = 3; // try out of frame
  
  
  if (doRatio) {
    lumiTextSize     = 0.8;
    cmsTextSize      = 1.0;
    CMS_lumi( plotpad, iPeriod, iPos );
  }
  else {
    lumiTextSize     = 0.45;
    cmsTextSize      = 0.55;
    CMS_lumi( can, iPeriod, iPos );
  }
  
  // make ratio pad and plot
  if (doRatio) {
    // draw ratio pad
    fullpad->cd();
    //plotpad = new TPad("plotpad","plotpad",0,0.2,1,0.99);
    //plotpad->SetTopMargin(0.05);
    //plotpad->SetRightMargin(0.05);
    //plotpad->SetBottomMargin(0.05);
    TPad* ratiopad = new TPad("ratiopad","ratiopad",0.,0.,1,0.23);
    ratiopad->SetLeftMargin(0.15);//(0.16);
    ratiopad->SetRightMargin(0.05);
    ratiopad->SetTopMargin(0.04);
    ratiopad->SetBottomMargin(0.44);
    //ratiopad->SetGridy();
    ratiopad->SetGridy();
    ratiopad->Draw();
    ratiopad->cd();

    
    // draw axis only
    TH1F* h_axis_ratio = new TH1F(Form("%s_axes",sig_hists[0]->GetName()),"",100,xmin,xmax);
    h_axis_ratio->GetYaxis()->SetTitleOffset(0.25);
    h_axis_ratio->GetYaxis()->SetTitleSize(0.18);
    h_axis_ratio->GetYaxis()->SetNdivisions(5);
    h_axis_ratio->GetYaxis()->SetLabelSize(0.15);
    h_axis_ratio->GetYaxis()->SetRangeUser(0.5,1.5);
    //h_axis_ratio->GetYaxis()->SetRangeUser(0.001,1.);
    h_axis_ratio->GetYaxis()->SetTitle("Ratio");
    h_axis_ratio->GetXaxis()->SetTitle(sig_hists[0]->GetXaxis()->GetTitle());
    h_axis_ratio->GetXaxis()->SetTitleSize(0.17);
    h_axis_ratio->GetXaxis()->SetLabelSize(0.17);
    h_axis_ratio->GetXaxis()->SetTitleOffset(1);
    h_axis_ratio->GetXaxis()->SetTickLength(0.07);
    h_axis_ratio->Draw("axis");
    
    TLine* lHT_b;
    lHT_b = new TLine(xmin,1, xmax ,1);
    lHT_b->SetLineColor(kBlack);
    lHT_b->SetLineStyle(2);
    lHT_b->SetLineWidth(1);
    lHT_b->Draw("same");


    
    for (unsigned int isig = 1; isig < sig_hists.size(); ++isig) {
      //    TH1D* h_ratio = (TH1D*) data_hist->Clone(Form("ratio_%s",data_hist->GetName()));
      TH1D* h_ratio = (TH1D*) sig_hists[isig]->Clone(Form("ratio_%s",sig_hists[isig]->GetName()));
      h_ratio->Sumw2();
      //sig_hists[1]->Sumw2();
      h_ratio->Divide(sig_hists[0]);
      for (int ibin=0; ibin < h_ratio->GetNbinsX(); ++ibin) {
        if (h_ratio->GetBinContent(ibin+1)<=0) {h_ratio->SetBinContent(ibin+1,0.);h_ratio->SetBinError(ibin+1,0.);}
      }
      h_ratio->Print("all");
      TGraphErrors* g_ratio = new TGraphErrors(h_ratio);
      g_ratio->SetName(Form("%s_graph",h_ratio->GetName()));
      for (int ibin=0; ibin < h_ratio->GetNbinsX(); ++ibin) {
        g_ratio->SetPointError(ibin, h_ratio->GetBinWidth(ibin+1)/2., h_ratio->GetBinError(ibin+1));
      }
      //g_ratio->SetLineColor(kBlack);
      //g_ratio->SetMarkerColor(kBlack);
      //g_ratio->SetMarkerStyle(20);
      gStyle->SetOptFit(111);
      gStyle->SetFitFormat("3.2g");
      gStyle->SetStatX(0.8);
      gStyle->SetStatY(0.95);
      gStyle->SetStatH(0.3);
      TF1* f=new TF1("f", "[0]", xmin, xmax);
//      TF1* f=new TF1("f", "[0]+[1]*x", xmin, xmax);
      f->SetLineColor(kRed);
      f->SetParameter(0, 1.0);
      f->SetParameter(1, 0.0);
      g_ratio->Print("all");
      g_ratio->Fit(f, "0");
      g_ratio->Draw("p0same");
      f->Draw("same");

      
    }
    
  } // if (doRatio)
  
  gPad->Modified();
  
  can->Print(Form("compareMultiPlot/%s.pdf",canvas_name.Data()));
  
  
  return;
}




void Superimp(TString file1, TString file2, TString la1, TString la2, TString nplot, TString xname, int reb =1, TString fix="")
{
  TFile f1( file1, "READ");
  TFile f2( file2, "READ");

  TH1D* h1= (TH1D*) f1.Get(nplot)->Clone("h1");
  TH1D* h2= (TH1D*) f2.Get(nplot+fix)->Clone("h2");
  h1->Rebin(reb);
  h2->Rebin(reb);
  TLegend *l = new TLegend(0.80,0.75, 0.88,0.88);
//  TLegend *l = new TLegend(0.15,0.60, 0.45,0.85);
  l->AddEntry(h1, la1, "lp");
  l->AddEntry(h2, la2, "lp");
  l->SetShadowColor(0);
  l->SetFillColor(0);
  l->SetBorderSize(0);
  l->SetTextFont(22);
  l->SetTextSize(0.04);

  canv = new TCanvas();
  canv->cd();
  gStyle->SetOptStat(0);

  h1->SetMarkerStyle(23);
  h2->SetMarkerStyle(22);
  h2->SetLineColor(2);
  h2->SetMarkerColor(2);
  h1->GetXaxis()->SetTitle(xname);
  h2->GetXaxis()->SetTitle(xname);
  h1->GetYaxis()->SetRangeUser(0, h1->GetMaximum()*1.3);

  if ( xname.Contains("ntrk") )h1->GetXaxis()->SetRangeUser(0,20);
  if ( xname.Contains("Mass") )h1->GetXaxis()->SetRangeUser(0,7);
  h1->SetTitle("");
  h2->SetTitle("");
  h1->DrawNormalized();
  h2->DrawNormalized("same");
  //h1->Draw();
  //h2->Draw("same");
  l->Draw("same");

  canv->Print("compare/" + nplot + fix + ".pdf"); 
  delete canv;
  delete l   ;
  return;
}

void SuperimpVec(vector<TString> files, vector<TString> labels, vector<TString> nplots, TString xname, float maxY, TString saveas, bool norm=0, int X1 = 30, int X2 = 150, bool doLog = false, int reb=1)
{
  
  canv = new TCanvas("c1_n48", "c1_n48");
  canv->SetBottomMargin(0.14);
  canv->cd();
  gStyle->SetOptStat(0);
  TLegend *l = new TLegend(0.50,0.65, 0.88,0.88);
  l->SetShadowColor(0);
  l->SetFillColor(0);
  l->SetBorderSize(0);
  l->SetTextFont(22);
  l->SetTextSize(0.04);
  //float maxY = 0;
  bool firstToDraw = true;
  int skipped = 0;
  for (unsigned int i = 0; i < files.size(); i++) {
    if (files[i]=="") {
      l->SetHeader(labels[i]);
      skipped++;
    }
    TFile f1( files[i], "READ");
    if(!f1.IsOpen()){
      std::cout<<"File "<<files[i]<<" can't be found, will not be used for histogram "<<nplots[i]<<std::endl;
      continue;
    }
    if((TH1D*) f1.Get(nplots[i])==0){
      std::cout<<"Histogram "<<nplots[i]<<" not found in file "<<files[i]<<std::endl;
      continue;
    }
    TH1D* h1= (TH1D*) f1.Get(nplots[i])->Clone("h1");
    
    TString nameTitle = files[i] + nplots[i] + labels[i];
    h1->SetNameTitle("","");
    h1->SetDirectory(0);
    h1->Rebin(reb);
    if ( labels[i].Contains("True Z inv") ) l->AddEntry(h1, labels[i], "f");
    else l->AddEntry(h1, labels[i], "lpe");
    h1->SetMarkerStyle(21+i-skipped);
    h1->SetLineColor(i+1-skipped);
    h1->SetMarkerColor(i+1-skipped);
    h1->GetXaxis()->SetTitle(xname);
    h1->GetXaxis()->SetTitleOffset(0.9);
    //    if (maxY < h1->GetMaximum()*1.3) maxY = h1->GetMaximum()*1.3;
    //  h1->GetYaxis()->SetRangeUser(0, 0.15);
    h1->GetXaxis()->SetRangeUser(X1, X2);
    
    
    if (norm) {
      h1->Scale(1./h1->Integral());
    }
    
    TString drawing = "";
    
    if ( labels[i].Contains("True Z inv") ) {
      h1->SetFillColor(i+1);
      h1->SetLineStyle(0);
      h1->SetMarkerSize(0);
      drawing = "E2";
    }
    if (firstToDraw) {h1->Draw(drawing); firstToDraw=false;}
    else h1->Draw("same");
    if (maxY==0) maxY = h1->GetMaximum()*1.3;
    if (doLog) {  
      canv->SetLogy(1);   
      h1->GetYaxis()->SetRangeUser(0.1, maxY);
    }
    else h1->GetYaxis()->SetRangeUser(0, maxY);
    
    if (nplots[i].Contains("dijetflav")) {
      h1->GetXaxis()->SetBinLabel(1, "jj");
      h1->GetXaxis()->SetBinLabel(2, "cj");
      h1->GetXaxis()->SetBinLabel(3, "bj");
      h1->GetXaxis()->SetBinLabel(4, "cc");
      h1->GetXaxis()->SetBinLabel(5, "bc");      
      h1->GetXaxis()->SetBinLabel(6, "bb"); 
      h1->GetXaxis()->SetLabelSize(0.05);
      //h1->GetXaxis()->SetLabelOffset(0.02);
      
    }
    
    
    f1.Close();
  }
  l->Draw("same");
  canv->Print("compareMultiPlot/" + saveas+ ".pdf"); 
  //delete canv;
  //delete l   ;
  return;

}


void SuperimpRatios(vector<TString> files, vector<TString> labels, vector<TString> nplots, TString xname, float maxY, TString saveas, bool norm=0, int X1 = 30, int X2 = 150, bool unsetSumW2 = false, int rebin = 1, TString yname="")
{
  
  canv = new TCanvas("c1_n48", "c1_n48");
  canv->SetBottomMargin(0.14);
  canv->cd();
  gStyle->SetOptStat(0);
  TLegend *l = new TLegend(0.50,0.65, 0.88,0.88);
  l->SetShadowColor(0);
  l->SetFillColor(0);
  l->SetBorderSize(0);
  l->SetTextFont(22);
  l->SetTextSize(0.04);
  //float maxY = 0;
  bool firstToDraw = true;
  int skipped = 0;
  for (unsigned int i = 0; i < files.size(); i++) {
    if (files[i]=="") {
      l->SetHeader(labels[i]);
      skipped++;
    }
    
    TFile f1( files[i], "READ");
    if(!f1.IsOpen()){
      std::cout<<"File "<<files[i]<<" can't be found, will not be used for histogram "<<nplots[i]<<std::endl;
      continue;
    }
    if((TH1D*) f1.Get(nplots[i])==0){
      std::cout<<"Histogram "<<nplots[i]<<" not found in file "<<files[i]<<std::endl;
      continue;
    }
    TH1D* h1= (TH1D*) f1.Get(nplots[i])->Clone("h1");
    
    TFile f2( files[i+1], "READ");
    if(!f2.IsOpen()){
      std::cout<<"File "<<files[i+1]<<" can't be found, will not be used for histogram "<<nplots[i]<<std::endl;
      continue;
    }
    if((TH1D*) f2.Get(nplots[i+1])==0){
      std::cout<<"Histogram "<<nplots[i+1]<<" not found in file "<<files[i+1]<<std::endl;
      continue;
    }
    TH1D* h2= (TH1D*) f2.Get(nplots[i+1])->Clone("h2");
    h1->Rebin(rebin);
    h2->Rebin(rebin);
    
    if (unsetSumW2) {
      h1->GetSumw2()->Set(0);   
      h2->GetSumw2()->Set(0); 
      h1->Sumw2(); // need to reset these so that the division is done correctly
      h2->Sumw2();
    }

    h1->Divide(h2);
    
    TString nameTitle = files[i] + nplots[i] + labels[i];
    h1->SetNameTitle("","");
    h1->SetDirectory(0);
    //h1->Rebin(reb);
    l->AddEntry(h1, labels[i], "lp");
    h1->SetMarkerStyle(21+i-skipped);
    h1->SetLineColor(i+1-skipped);
    h1->SetMarkerColor(i+1-skipped);
    h1->GetXaxis()->SetTitle(xname);
    h1->GetXaxis()->SetTitleOffset(0.9);
    //    if (maxY < h1->GetMaximum()*1.3) maxY = h1->GetMaximum()*1.3;
    //  h1->GetYaxis()->SetRangeUser(0, 0.15);
    h1->GetXaxis()->SetRangeUser(X1, X2);
    h1->GetYaxis()->SetTitleOffset(0.83);
    h1->GetYaxis()->SetTitle(yname);
    
    TString drawing = "";

    if (unsetSumW2) drawing = "E1";
    
    if (firstToDraw) {h1->Draw(drawing); firstToDraw=false;}
    else h1->Draw(drawing+"same");
    


    
    if (maxY==0) maxY = h1->GetMaximum()*1.3;
    
    h1->GetYaxis()->SetRangeUser(0, maxY);
    
    f1.Close();
    f2.Close();
    i++;
  }
  l->Draw("same");
  canv->Print("compareMultiPlot/" + saveas+ ".pdf"); 
  //delete canv;
  //delete l   ;
  return;
  
}



void SuperimpRatiosFlat(vector<TString> files, vector<TString> labels, vector<TString> nplots, TString xname, float maxY, TString saveas, bool norm=0, int X1 = 30, int X2 = 150, bool unsetSumW2 = false, int rebin = 1, TString yname="")
{
  
  canv = new TCanvas("c1_n48", "c1_n48");
  canv->SetBottomMargin(0.14);
  canv->cd();
  gStyle->SetOptStat(0);
  TLegend *l = new TLegend(0.50,0.65, 0.88,0.88);
  l->SetShadowColor(0);
  l->SetFillColor(0);
  l->SetBorderSize(0);
  l->SetTextFont(22);
  l->SetTextSize(0.04);
  //float maxY = 0;
  bool firstToDraw = true;
  int skipped = 0;
  for (unsigned int i = 0; i < files.size(); i++) {
    if (files[i]=="") {
      l->SetHeader(labels[i]);
      skipped++;
    }
    
    TFile f1( files[i], "READ");
    if(!f1.IsOpen()){
      std::cout<<"File "<<files[i]<<" can't be found, will not be used for histogram "<<nplots[i]<<std::endl;
      continue;
    }
    if((TH1D*) f1.Get(nplots[i])==0){
      std::cout<<"Histogram "<<nplots[i]<<" not found in file "<<files[i]<<std::endl;
      continue;
    }
    TH1D* h1= (TH1D*) f1.Get(nplots[i])->Clone("h1");
    
    TFile f2( files[i+1], "READ");
    if(!f2.IsOpen()){
      std::cout<<"File "<<files[i+1]<<" can't be found, will not be used for histogram "<<nplots[i]<<std::endl;
      continue;
    }
    if((TH1D*) f2.Get(nplots[i+1])==0){
      std::cout<<"Histogram "<<nplots[i+1]<<" not found in file "<<files[i+1]<<std::endl;
      continue;
    }
    TH1D* h2= (TH1D*) f2.Get(nplots[i+1])->Clone("h2");
    h1->Rebin(rebin);
    h2->Rebin(rebin);
    
    if (unsetSumW2) {
      h1->GetSumw2()->Set(0);   
      h2->GetSumw2()->Set(0); 
      h1->Sumw2(); // need to reset these so that the division is done correctly
      h2->Sumw2();
    }
    
    h1->Divide(h2);
    
    // Now we make the magic: set bin content to 1, and bin error to relative error of h1
    for (int ib = 1; ib <= h1->GetNbinsX(); ib++) {
      if (h1->GetBinContent(ib)!=0) {
        float relerror = h1->GetBinError(ib)/h1->GetBinContent(ib);
        h1->SetBinContent(ib, 1.);
        h1->SetBinError(ib, relerror);
      }
    }    
    
    TString nameTitle = files[i] + nplots[i] + labels[i];
    h1->SetNameTitle("","");
    h1->SetDirectory(0);
    //h1->Rebin(reb);
    l->AddEntry(h1, labels[i], "lp");
    h1->SetMarkerStyle(21+i-skipped);
    h1->SetLineColor(i+1-skipped);
    h1->SetMarkerColor(i+1-skipped);
    h1->GetXaxis()->SetTitle(xname);
    h1->GetXaxis()->SetTitleOffset(0.9);
    //    if (maxY < h1->GetMaximum()*1.3) maxY = h1->GetMaximum()*1.3;
    //  h1->GetYaxis()->SetRangeUser(0, 0.15);
    h1->GetXaxis()->SetRangeUser(X1, X2);
    h1->GetYaxis()->SetTitleOffset(0.83);
    h1->GetYaxis()->SetTitle(yname);
    gPad->SetGridy();
    
    TString drawing = "";
    
    if (unsetSumW2) drawing = "E1";
    
    if (firstToDraw) {h1->Draw(drawing); firstToDraw=false;}
    else h1->Draw(drawing+"same");
    
    
    
    
    if (maxY==0) maxY = h1->GetMaximum()*1.3;
    
    h1->GetYaxis()->SetRangeUser(1+ -1. * maxY, 1+maxY);
    
    f1.Close();
    f2.Close();
    i++;
  }
  l->Draw("same");
  canv->Print("compareMultiPlot/" + saveas+ ".pdf"); 
  //delete canv;
  //delete l   ;
  return;
  
}

void SuperimpVecRatio(vector<TString> files, vector<TString> labels, vector<TString> nplots, TString xname, float maxY, TString saveas, bool norm=0, int X1 = 30, int X2 = 150, bool doLog = false)
{
  
  canv = new TCanvas("c1_n48", "c1_n48",10,32,787,573);
  canv->cd();
  gStyle->SetOptStat(0);
  TLegend *l = new TLegend(0.60,0.65, 0.88,0.88);
  l->SetShadowColor(0);
  l->SetFillColor(0);
  l->SetBorderSize(0);
  l->SetTextFont(22);
  l->SetTextSize(0.04);
  //float maxY = 0;
  
  
  TPad* p01=new TPad("p01","p01",0,0.25,1,1);
  p01->SetFillStyle(4000);
  p01->SetFillColor(0);
  p01->SetFrameFillStyle(4000);
  p01->SetFrameFillColor(0);
  p01->SetFrameLineColor(0);
  p01->Range(-70.32233,-8.217202,421.3903,1071.598);
  p01->SetBorderMode(0);
  p01->SetBorderSize(2);
  p01->SetTickx(1);
  p01->SetTicky(1);
  p01->SetLeftMargin(0.1430151);
  p01->SetRightMargin(0.04350161);
  p01->SetTopMargin(0.05269226);
  p01->SetBottomMargin(0.007609824);
  p01->SetFrameBorderMode(0);
  p01->SetFrameBorderMode(0);
  p01->Draw();
  p01->cd();

  float autoMaxY = -999;
  bool firstToDraw = true;
  for (unsigned int i = 0; i < files.size(); i++) {
    TFile f1( files[i], "READ");
    if(!f1.IsOpen()){
      std::cout<<"File "<<files[i]<<" can't be found, will not be used for histogram "<<nplots[i]<<std::endl;
      continue;
    }
    if(f1.GetKey(nplots[i].Data())==0){
      std::cout<<"Histogram "<<nplots[i]<<" not found in file "<<files[i]<<std::endl;
      continue;
    }
    TH1D* h1= (TH1D*) f1.Get(nplots[i])->Clone("h1");                
    
    TString nameTitle = files[i] + nplots[i] + labels[i];
    h1->SetNameTitle("","");
    h1->SetDirectory(0);
    //h1->Rebin(reb);
    l->AddEntry(h1, labels[i], "lp");
    h1->SetMarkerStyle(21+i);
    h1->SetLineColor(i+1);
    h1->SetMarkerColor(i+1);
    h1->GetXaxis()->SetTitle(xname);
    //    if (maxY < h1->GetMaximum()*1.3) maxY = h1->GetMaximum()*1.3;
    //  h1->GetYaxis()->SetRangeUser(0, 0.15);
    h1->GetYaxis()->SetTitleSize(0.05);
    h1->GetXaxis()->SetRangeUser(X1, X2);
    if (norm) {
      h1->Scale(1./h1->Integral());
    }
    if (firstToDraw) {h1->Draw(); firstToDraw=false;}
    else h1->Draw("same");
    
    if (autoMaxY < h1->GetMaximum()*1.3 ) autoMaxY = h1->GetMaximum()*1.3;
    if (doLog) {  
      p01->SetLogy(1);   
      h1->GetYaxis()->SetRangeUser(1, maxY==0 ? autoMaxY*2 : maxY*2);
    }
    else h1->GetYaxis()->SetRangeUser(0, maxY==0 ? autoMaxY : maxY); // this doesn't quite work. Only first histogram is taken into account
    

    f1.Close();
  }
  l->Draw("same");
  
  canv->cd();
  TPad* p02=new TPad("p02","p02",0,0,1,0.25);
  p02->SetFillStyle(4000);
  p02->SetFillColor(0);
  p02->SetFrameFillStyle(4000);
  p02->SetFrameFillColor(0);
  p02->SetFrameLineColor(0);
  p02->Range(-69.27659,-17.79753,420.5338,19.06095);
  p02->SetLeftMargin(0.1414355);
  p02->SetRightMargin(0.04192203);
  p02->SetTopMargin(0.01176064);
  p02->SetBottomMargin(0.39156);
  p02->SetTickx(1);
  p02->SetTicky(1);
  p02->SetGridy();
  p02->SetBorderMode(0);
  p02->SetFrameBorderMode(0);
  p02->SetFrameBorderMode(0);
  p02->Draw();
  p02->cd();
  
  TFile f( files[0], "READ");
  if(!f.IsOpen()){
    std::cout<<"File "<<files[0]<<" can't be found, will not be used for ratio histogram "<<nplots[0]<<std::endl;
    return;
  }
  if(f.GetKey(nplots[0].Data())==0){
    std::cout<<"Histogram "<<nplots[0]<<" for ratio not found in file "<<files[0]<<std::endl;
    return;
  }
  TH1D* h0= (TH1D*) f.Get(nplots[0])->Clone("h0"); 
  TString nameTitle = files[0] + nplots[0] + labels[0];
  h0->SetNameTitle("","");
  h0->SetDirectory(0);
  f.Close();
  
   firstToDraw = true;
  for (unsigned int i = 1; i < files.size(); i++) {
    TFile f1( files[i], "READ");
    if(!f1.IsOpen()){
      std::cout<<"File "<<files[i]<<" can't be found, will not be used for ratio histogram "<<nplots[i]<<std::endl;
      continue;
    }
    if(f1.GetKey(nplots[i].Data())==0){
      std::cout<<"Histogram "<<nplots[i]<<" not found in file "<<files[i]<<std::endl;
      continue;
    }
    TH1D* h1= (TH1D*) f1.Get(nplots[i])->Clone("h1");                
    h1->Divide(h0);
    
    TString nameTitle = files[i] + nplots[i] + labels[i];
    h1->SetNameTitle("","");
    h1->SetDirectory(0);
    //h1->Rebin(reb);
    //l->AddEntry(h1, labels[i], "lp");
    h1->SetMarkerStyle(21+i);
    h1->SetLineColor(i+1);
    h1->SetMarkerColor(i+1);
    h1->GetXaxis()->SetTitle(xname);
    
    h1->GetXaxis()->SetLabelSize(0.16);
    h1->GetXaxis()->SetTitleSize(0.17);
    //diff->GetXaxis()->SetTitleFont(42);
    h1->GetYaxis()->SetTitle("/ black");
    h1->GetYaxis()->SetNdivisions(505);
    //diff->GetYaxis()->SetLabelFont(42);
    h1->GetYaxis()->SetLabelSize(0.1);
    h1->GetYaxis()->SetTitleSize(0.12);
    h1->GetYaxis()->SetTitleOffset(0.38);
    //diff->GetYaxis()->SetTitleFont(42);
    if (norm || nplots[i].Contains("eff") || nplots[i].Contains("fr"))     
      h1->GetYaxis()->SetRangeUser(0.2, 1.8);

    if ( saveas.Contains("Ratio_FOpt") )
      h1->GetYaxis()->SetRangeUser(-0.1, 0.5);
    
    //    if (maxY < h1->GetMaximum()*1.3) maxY = h1->GetMaximum()*1.3;
    //  h1->GetYaxis()->SetRangeUser(0, 0.15);
    h1->GetXaxis()->SetRangeUser(X1, X2);
    if (nplots[i].Contains("fo_ID")) {
      h1->GetXaxis()->SetBinLabel(1, "");
      h1->GetXaxis()->SetBinLabel(2, "other");
      h1->GetXaxis()->SetBinLabel(3, "C-had");
      h1->GetXaxis()->SetBinLabel(4, "B-had");      
      h1->GetXaxis()->SetBinLabel(5, "fake"); 
      h1->GetXaxis()->SetLabelSize(0.20);
      h1->GetXaxis()->SetLabelOffset(0.02);

    }

    if (firstToDraw) {h1->Draw(); firstToDraw=false;}
    else h1->Draw("same");
    //h1->GetYaxis()->SetRangeUser(0, maxY);
    
    f1.Close();
  }
  //l->Draw("same");

  
  
  
  canv->Print("compareMultiPlot/" + saveas+ ".pdf"); 
  //delete canv;
  //delete l   ;
  return;
  
}

void compareMultiPlot()
{


//  TString dir = "/Users/giovannizevidellaporta/UCSD/MT2_2016/MT2looperHistograms/V00-08-08_nojson_skim_base_mt2gt200_ZinvV6/";
//  TString dir = "/Users/giovannizevidellaporta/UCSD/MT2_2016/MT2looperHistograms/FromMark_zinvHW_oct30/";
//  TString dir = "/Users/giovannizevidellaporta/UCSD/MT2_2016/MT2looperHistograms/FromMark_zinvHW_nhIso_nov2/";
//  TString dir = "/Users/giovannizevidellaporta/UCSD/MT2_2016/MT2looperHistograms/FromMark_zinvHW_HOverE015_nov2/";
//  TString dir = "/Users/giovannizevidellaporta/UCSD/MT2_2016/MT2looperHistograms/V00-08-08_nojson_skim_base_mt2gt200_ZinvV6_DRcorr/";
//  TString dir = "/Users/giovannizevidellaporta/UCSD/MT2/Zinvisible/MT2babies/V00-01-10_25ns_data_json_246908-260627_v2_skim_base_V4_mt2gt200_2p26fb_newtxtfilters_jet1ptqcd/";
//  TString dir = "/Users/giovannizevidellaporta/UCSD/MT2_2016/MT2looperHistograms/FromMark_sep29_V00-08-09_json_271036-280385_NoL1T_24p6fb_skim_base_mt2gt200_ZinvV6/";
//  TString dir = "/Users/giovannizevidellaporta/UCSD/MT2_2016/MT2looperHistograms/FromMark_31p24_oldMC_nov7/";
//  TString dir = "/Users/giovannizevidellaporta/UCSD/MT2_2016/MT2looperHistograms/FromMark_36p26_v10MC_nov29/";
  TString dir = "/Users/giovannizevidellaporta/UCSD/MT2_2016/MT2looperHistograms/V00-08-15/";
  TString dir2 = "/Users/giovannizevidellaporta/UCSD/MT2_2016/MT2looperHistograms/V00-08-09_16Dec16/";

  
  
  vector<TString> files; vector<TString> labels; vector<TString> nplots;
  
  
  TString dir4 = "/Users/giovannizevidellaporta/UCSD/MT2_2016/MT2looperHistograms/CheckEGMissue2/";
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir4+"data_Run2016.root");       labels.push_back("MM");            nplots.push_back("crdybaseIncl/h_bosonptbinsMuMu");
  files.push_back( dir4+"data_Run2016.root");       labels.push_back("EE");            nplots.push_back("crdybaseIncl/h_bosonptbinsEE");
  makeCMSPlot(  files, labels, nplots, "ZpT_eeVSmm_Data", /*xtitle*/ "Z pt [GeV]" , /*ytitle*/ "" , /*xmin*/ 207, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 1, /*doRatio*/ 1 );
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir4+"dyjetsll_ht.root");       labels.push_back("MM");            nplots.push_back("crdybaseIncl/h_bosonptbinsMuMu");
  files.push_back( dir4+"dyjetsll_ht.root");       labels.push_back("EE");            nplots.push_back("crdybaseIncl/h_bosonptbinsEE");
  makeCMSPlot(  files, labels, nplots, "ZpT_eeVSmm_MC", /*xtitle*/ "Z pt [GeV]" , /*ytitle*/ "" , /*xmin*/ 207, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 1, /*doRatio*/ 1 );
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir4+"data_Run2016.root");       labels.push_back("MM");            nplots.push_back("crdybaseIncl/h_mt2binsMuMu");
  files.push_back( dir4+"data_Run2016.root");       labels.push_back("EE");            nplots.push_back("crdybaseIncl/h_mt2binsEE");
  makeCMSPlot(  files, labels, nplots, "Zmt2_eeVSmm_Data", /*xtitle*/ "MT2 [GeV]" , /*ytitle*/ "" , /*xmin*/ 0, /*xmax*/1199 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir4+"dyjetsll_ht.root");       labels.push_back("MM");            nplots.push_back("crdybaseIncl/h_mt2binsMuMu");
  files.push_back( dir4+"dyjetsll_ht.root");       labels.push_back("EE");            nplots.push_back("crdybaseIncl/h_mt2binsEE");
  makeCMSPlot(  files, labels, nplots, "Zmt2_eeVSmm_MC", /*xtitle*/ "MT2 [GeV]" , /*ytitle*/ "" , /*xmin*/ 0, /*xmax*/1199 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir4+"data_Run2016.root");       labels.push_back("MM");            nplots.push_back("crdybaseIncl/h_metMuMu");
  files.push_back( dir4+"data_Run2016.root");       labels.push_back("EE");            nplots.push_back("crdybaseIncl/h_metEE");
  makeCMSPlot(  files, labels, nplots, "Zmet_eeVSmm_Data", /*xtitle*/ "MT2 [GeV]" , /*ytitle*/ "" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 5 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir4+"dyjetsll_ht.root");       labels.push_back("MM");            nplots.push_back("crdybaseIncl/h_metMuMu");
  files.push_back( dir4+"dyjetsll_ht.root");       labels.push_back("EE");            nplots.push_back("crdybaseIncl/h_metEE");
  makeCMSPlot(  files, labels, nplots, "Zmet_eeVSmm_MC", /*xtitle*/ "MT2 [GeV]" , /*ytitle*/ "" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 5 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );


  // STUDY HIGH PT PF CANDIDATES
//  TString dir3 = "/Users/giovannizevidellaporta/UCSD/MT2_2016/METfilter/data_Run2016H_noQCD.root";
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir3);            labels.push_back("All");            nplots.push_back("srbase/h_pfCand_pt");
//  files.push_back( dir3);            labels.push_back("NeutralEM");            nplots.push_back("srbase/h_pfCand_22_pt");
//  files.push_back( dir3);            labels.push_back("Muon");            nplots.push_back("srbase/h_pfCand_13_pt");
//  makeCMSPlot(  files, labels, nplots, "PFCandPt", /*xtitle*/ "pt [GeV]" , /*ytitle*/ "" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 0 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir3);            labels.push_back("All");            nplots.push_back("srbaseIncl/h_pfCand_pt");
//  files.push_back( dir3);            labels.push_back("NeutralEM");            nplots.push_back("srbaseIncl/h_pfCand_22_pt");
//  files.push_back( dir3);            labels.push_back("Muon");            nplots.push_back("srbaseIncl/h_pfCand_13_pt");
//  makeCMSPlot(  files, labels, nplots, "PFCandPtIncl", /*xtitle*/ "pt [GeV]" , /*ytitle*/ "" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 0 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir3);            labels.push_back("All");            nplots.push_back("srbase/h_pfCand_deltaPFMET");
//  files.push_back( dir3);            labels.push_back("NeutralEM");            nplots.push_back("srbase/h_pfCand_22_deltaPFMET");
//  files.push_back( dir3);            labels.push_back("Muon");            nplots.push_back("srbase/h_pfCand_13_deltaPFMET");
//  makeCMSPlot(  files, labels, nplots, "PFCandDPt", /*xtitle*/ "pt [GeV]" , /*ytitle*/ "" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 0 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir3);            labels.push_back("All");            nplots.push_back("srbaseIncl/h_pfCand_deltaPFMET");
//  files.push_back( dir3);            labels.push_back("NeutralEM");            nplots.push_back("srbaseIncl/h_pfCand_22_deltaPFMET");
//  files.push_back( dir3);            labels.push_back("Muon");            nplots.push_back("srbaseIncl/h_pfCand_13_deltaPFMET");
//  makeCMSPlot(  files, labels, nplots, "PFCandDPtIncl", /*xtitle*/ "pt [GeV]" , /*ytitle*/ "" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 0 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir3);            labels.push_back("All");            nplots.push_back("srbase/h_pfCand_deltaPhiPFMET");
//  files.push_back( dir3);            labels.push_back("NeutralEM");            nplots.push_back("srbase/h_pfCand_22_deltaPhiPFMET");
//  files.push_back( dir3);            labels.push_back("Muon");            nplots.push_back("srbase/h_pfCand_13_deltaPhiPFMET");
//  makeCMSPlot(  files, labels, nplots, "PFCandDPhi", /*xtitle*/ "pt [GeV]" , /*ytitle*/ "" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 0 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir3);            labels.push_back("All");            nplots.push_back("srbaseIncl/h_pfCand_deltaPhiPFMET");
//  files.push_back( dir3);            labels.push_back("NeutralEM");            nplots.push_back("srbaseIncl/h_pfCand_22_deltaPhiPFMET");
//  files.push_back( dir3);            labels.push_back("Muon");            nplots.push_back("srbaseIncl/h_pfCand_13_deltaPhiPFMET");
//  makeCMSPlot(  files, labels, nplots, "PFCandDPhiIncl", /*xtitle*/ "pt [GeV]" , /*ytitle*/ "" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 0 );
//  
  
  
  
  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir+"dyjetsll_ht.root");           labels.push_back("Z");                 nplots.push_back("crdy6M/h_mt2bins");
//  files.push_back( dir+"ttz.root");            labels.push_back("TTZ");            nplots.push_back("crdy6M/h_mt2bins");
//  //  makeCMSPlot(  files, labels, nplots, "Monojet_W_GJ", /*xtitle*/ "HT [GeV]" , /*ytitle*/ "Events / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1 );
//  //  makeCMSPlot(  files, labels, nplots, "Monojet_W_GJ_norm", /*xtitle*/ "HT [GeV]" , /*ytitle*/ "AU " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 1, /*doRatio*/ 1 );
//  makeCMSPlot(  files, labels, nplots, "TTZshape46j2bMHT", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  //makeCMSPlot(  files, labels, nplots, "truthPtVratio", /*xtitle*/ "p_{T}^{V} [GeV]" , /*ytitle*/ "Events / 40 GeV" , /*xmin*/ 0, /*xmax*/1200 , /*rebin*/ 4 , /*logplot*/ 1, /*scalesig*/ -1., /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir+"dyjetsll_ht.root");           labels.push_back("Z");                 nplots.push_back("crdy6M/h_mt2bins");
//  files.push_back( dir+"ttz.root");            labels.push_back("TTZ");            nplots.push_back("crdy6M/h_mt2bins");
//  //  makeCMSPlot(  files, labels, nplots, "Monojet_W_GJ", /*xtitle*/ "HT [GeV]" , /*ytitle*/ "Events / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1 );
//  //  makeCMSPlot(  files, labels, nplots, "Monojet_W_GJ_norm", /*xtitle*/ "HT [GeV]" , /*ytitle*/ "AU " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 1, /*doRatio*/ 1 );
//  makeCMSPlot(  files, labels, nplots, "TTZ46j2bMHT", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Events / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 0, /*doRatio*/ 1 );
//  //makeCMSPlot(  files, labels, nplots, "truthPtVratio", /*xtitle*/ "p_{T}^{V} [GeV]" , /*ytitle*/ "Events / 40 GeV" , /*xmin*/ 0, /*xmax*/1200 , /*rebin*/ 4 , /*logplot*/ 1, /*scalesig*/ -1., /*doRatio*/ 1 );
//  
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir+"data_Run2016.root");            labels.push_back("MC");            nplots.push_back("crdybaseIncl/h_nBJet20LowPt");
//  files.push_back( dir+"data_Run2016.root");                labels.push_back("MC");            nplots.push_back("crdybaseIncl/h_nBJet20LowPtemu");
//  makeCMSPlotRatios(  files, labels, nplots, "RSFOF_NB", /*xtitle*/ "N(bjets)" , /*ytitle*/ "R(SF/OF)" , /*xmin*/ 0, /*xmax*/4 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1);
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir+"data_Run2016.root");            labels.push_back("MC");            nplots.push_back("crdybaseIncl/h_nBJet20LLowPt");
//  files.push_back( dir+"data_Run2016.root");                labels.push_back("MC");            nplots.push_back("crdybaseIncl/h_nBJet20LLowPtemu");
//  makeCMSPlotRatios(  files, labels, nplots, "RSFOF_NBL", /*xtitle*/ "N(bjets)" , /*ytitle*/ "R(SF/OF)" , /*xmin*/ 0, /*xmax*/4 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1);
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir+"data_Run2016.root");            labels.push_back("MC");            nplots.push_back("crdybaseIncl/h_nBJet20TLowPt");
//  files.push_back( dir+"data_Run2016.root");                labels.push_back("MC");            nplots.push_back("crdybaseIncl/h_nBJet20TLowPtemu");
//  makeCMSPlotRatios(  files, labels, nplots, "RSFOF_NBT", /*xtitle*/ "N(bjets)" , /*ytitle*/ "R(SF/OF)" , /*xmin*/ 0, /*xmax*/4 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1);
//  
//  
  
//  // VL
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("23j 0b");            nplots.push_back("sr1VL/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("23j 1b");            nplots.push_back("sr2VL/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("23j 2b");            nplots.push_back("sr3VL/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_1VL", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("23j 0b");            nplots.push_back("crdy1VL/h_mt2bins");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("23j 1b");            nplots.push_back("crdy2VL/h_mt2bins");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("23j 2b");            nplots.push_back("crdy3VL/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_1VLDY", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("4+j 0b");            nplots.push_back("sr12VL/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("4+j 1b");            nplots.push_back("sr13VL/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("4+j 2b");            nplots.push_back("sr14VL/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_4VL", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("4+j 0b");            nplots.push_back("crdy12VL/h_mt2bins");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("4+j 1b");            nplots.push_back("crdy13VL/h_mt2bins");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("4+j 2b");            nplots.push_back("crdy14VL/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_4VLDY", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  // L
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("23j 0b");            nplots.push_back("sr1L/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("23j 1b");            nplots.push_back("sr2L/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("23j 2b");            nplots.push_back("sr3L/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_1L", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("23j 0b");            nplots.push_back("crdy1L/h_mt2bins");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("23j 1b");            nplots.push_back("crdy2L/h_mt2bins");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("23j 2b");            nplots.push_back("crdy3L/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_1LDY", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("46j 0b");            nplots.push_back("sr4L/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("46j 1b");            nplots.push_back("sr5L/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("46j 2b");            nplots.push_back("sr6L/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_4L", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("46j 0b");            nplots.push_back("crdy4L/h_mt2bins");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("46j 1b");            nplots.push_back("crdy5L/h_mt2bins");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("46j 2b");            nplots.push_back("crdy6L/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_4LDY", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("7j 0b");            nplots.push_back("sr7L/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("7j 1b");            nplots.push_back("sr8L/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("7j 2b");            nplots.push_back("sr9L/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("7j 3b");            nplots.push_back("sr11L/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_7L", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("7j 0b");            nplots.push_back("crdy7L/h_mt2bins");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("7j 1b");            nplots.push_back("crdy8L/h_mt2bins_1B");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("7j 2b");            nplots.push_back("crdy9L/h_mt2bins_2B");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("7j 3b");            nplots.push_back("crdy11L/h_mt2bins_3B");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_7LDY", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  // M
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("23j 0b");            nplots.push_back("sr1M/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("23j 1b");            nplots.push_back("sr2M/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("23j 2b");            nplots.push_back("sr3M/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_1M", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("23j 0b");            nplots.push_back("crdy1M/h_mt2bins");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("23j 1b");            nplots.push_back("crdy2M/h_mt2bins");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("23j 2b");            nplots.push_back("crdy3M/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_1MDY", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("46j 0b");            nplots.push_back("sr4M/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("46j 1b");            nplots.push_back("sr5M/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("46j 2b");            nplots.push_back("sr6M/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_4M", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("46j 0b");            nplots.push_back("crdy4M/h_mt2bins");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("46j 1b");            nplots.push_back("crdy5M/h_mt2bins");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("46j 2b");            nplots.push_back("crdy6M/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_4MDY", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("7j 0b");            nplots.push_back("sr7M/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("7j 1b");            nplots.push_back("sr8M/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("7j 2b");            nplots.push_back("sr9M/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("7j 3b");            nplots.push_back("sr11M/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_7M", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("7j 0b");            nplots.push_back("crdy7M/h_mt2bins");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("7j 1b");            nplots.push_back("crdy8M/h_mt2bins_1B");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("7j 2b");            nplots.push_back("crdy9M/h_mt2bins_2B");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("7j 3b");            nplots.push_back("crdy11M/h_mt2bins_3B");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_7MDY", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  
//  // H
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("23j 0b");            nplots.push_back("sr1H/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("23j 1b");            nplots.push_back("sr2H/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("23j 2b");            nplots.push_back("sr3H/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_1H", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("23j 0b");            nplots.push_back("crdy1H/h_mt2bins");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("23j 1b");            nplots.push_back("crdy2H/h_mt2bins");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("23j 2b");            nplots.push_back("crdy3H/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_1HDY", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("46j 0b");            nplots.push_back("sr4H/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("46j 1b");            nplots.push_back("sr5H/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("46j 2b");            nplots.push_back("sr6H/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_4H", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("46j 0b");            nplots.push_back("crdy4H/h_mt2bins");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("46j 1b");            nplots.push_back("crdy5H/h_mt2bins");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("46j 2b");            nplots.push_back("crdy6H/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_4HDY", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("7j 0b");            nplots.push_back("sr7H/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("7j 1b");            nplots.push_back("sr8H/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("7j 2b");            nplots.push_back("sr9H/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("7j 3b");            nplots.push_back("sr11H/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_7H", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("7j 0b");            nplots.push_back("crdy7H/h_mt2bins");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("7j 1b");            nplots.push_back("crdy8H/h_mt2bins_1B");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("7j 2b");            nplots.push_back("crdy9H/h_mt2bins_2B");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("7j 3b");            nplots.push_back("crdy11H/h_mt2bins_3B");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_7HDY", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  // UH
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("23j 0b");            nplots.push_back("sr1UH/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("23j 1b");            nplots.push_back("sr2UH/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("23j 2b");            nplots.push_back("sr3UH/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_1UH", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("23j 0b");            nplots.push_back("crdy1UH/h_mt2bins");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("23j 1b");            nplots.push_back("crdy2UH/h_mt2bins");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("23j 2b");            nplots.push_back("crdy3UH/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_1UHDY", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("46j 0b");            nplots.push_back("sr4UH/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("46j 1b");            nplots.push_back("sr5UH/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("46j 2b");            nplots.push_back("sr6UH/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_4UH", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("46j 0b");            nplots.push_back("crdy4UH/h_mt2bins");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("46j 1b");            nplots.push_back("crdy5UH/h_mt2bins");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("46j 2b");            nplots.push_back("crdy6UH/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_4UHDY", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("7j 0b");            nplots.push_back("sr7UH/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("7j 1b");            nplots.push_back("sr8UH/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("7j 2b");            nplots.push_back("sr9UH/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("7j 3b");            nplots.push_back("sr11UH/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_7UH", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("7j 0b");            nplots.push_back("crdy7UH/h_mt2bins");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("7j 1b");            nplots.push_back("crdy8UH/h_mt2bins_1B");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("7j 2b");            nplots.push_back("crdy9UH/h_mt2bins_2B");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("7j 3b");            nplots.push_back("crdy11UH/h_mt2bins_3B");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_7UHDY", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  h_Pull->SaveAs("pull.root");
//
//  
//  // UH vs NJ
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("23j 0b");            nplots.push_back("sr1UH/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("46j 0b");            nplots.push_back("sr4UH/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("7j 0b");            nplots.push_back("sr7UH/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNJ_UH", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("23j 0b");            nplots.push_back("crdy1UH/h_mt2bins");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("46j 0b");            nplots.push_back("crdy4UH/h_mt2bins");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("7j 0b");            nplots.push_back("crdy7UH/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNJ_UHDY", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  // 26j 3b
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("3j");            nplots.push_back("srbaseVL/h_mt2bins3J");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("2j 3b");            nplots.push_back("sr15VL/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_26VL", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("3j");            nplots.push_back("crdybaseVL/h_mt2bins3J");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("2j 3b");            nplots.push_back("crdy15VL/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_26VLDY", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("36j");            nplots.push_back("srbaseL/h_mt2bins36J");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("26j 3b");            nplots.push_back("sr10L/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_26L", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("36j");            nplots.push_back("crdybaseL/h_mt2bins36J");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("26j 3b");            nplots.push_back("crdy10L/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_26LDY", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("36j");            nplots.push_back("srbaseM/h_mt2bins36J");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("26j 3b");            nplots.push_back("sr10M/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_26M", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("36j");            nplots.push_back("crdybaseM/h_mt2bins36J");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("26j 3b");            nplots.push_back("crdy10M/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_26MDY", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("36j");            nplots.push_back("srbaseH/h_mt2bins36J");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("26j 3b");            nplots.push_back("sr10H/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_26H", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("36j");            nplots.push_back("crdybaseH/h_mt2bins36J");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("26j 3b");            nplots.push_back("crdy10H/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_26HDY", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("Inclusive");            nplots.push_back("srbaseUH/h_mt2bins");
//  files.push_back( dir2+"zinv_ht.root");            labels.push_back("26j 3b");            nplots.push_back("sr10UH/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_26UH", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("Inclusive");            nplots.push_back("crdybaseUH/h_mt2bins");
//  files.push_back( dir2+"dyjetsll_ht.root");            labels.push_back("26j 3b");            nplots.push_back("crdy10UH/h_mt2bins");
//  makeCMSPlot(  files, labels, nplots, "MT2vsNB_26UHDY", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
//  
//  
//  
//  
//  return;
  
//
//
//files.clear(); labels.clear(); nplots.clear();
//files.push_back( dir+"data_Run2016.root");            labels.push_back("Data");            nplots.push_back("h_nvtx");
//files.push_back( dir+"dyjetsll_ht.root");           labels.push_back("DY Full");                 nplots.push_back("h_nvtx");
//files.push_back( dir+"T1bbbb_1500_100.root");           labels.push_back("T1bbbb Full");                 nplots.push_back("h_nvtx");
//files.push_back( dir3+"T1bbbb_mGluino-1600.root");           labels.push_back("T1bbbb Fast");                 nplots.push_back("h_nvtx");
//makeCMSPlot(  files, labels, nplots, "NVTX", /*xtitle*/ "Number of Reconstructed Vertices" , /*ytitle*/ "Events / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 1, /*doRatio*/ 1 );
////makeCMSPlot(  files, labels, nplots, "truthPtVratio", /*xtitle*/ "p_{T}^

  files.clear(); labels.clear(); nplots.clear();
  files.push_back( "");       labels.push_back("200 < H_{T} < 450 GeV");          nplots.push_back("");

  files.push_back( dir+"zinv_ht.root");           labels.push_back("Z #rightarrow #nu#bar{#nu} (MC)");                 nplots.push_back("srbaseVL/h_mt2bins");
  files.push_back( dir+"purity.root");            labels.push_back("#gamma estimate (Data)");            nplots.push_back("srbaseVL/h_mt2binspredZFailSieieData");
  files.push_back( dir+"purityRL.root");          labels.push_back("W estimate (Data)");            nplots.push_back("srbaseVL/h_predZ");
  files.push_back( dir+"zinvFromDY.root");          labels.push_back("DY estimate (Data)");            nplots.push_back("srbaseVL/h_mt2bins");
  makeCMSPlot(  files, labels, nplots, "MT2VL_W_GJ_log", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 200, /*xmax*/1000 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
  //makeCMSPlot(  files, labels, nplots, "truthPtVratio", /*xtitle*/ "p_{T}^{V} [GeV]" , /*ytitle*/ "Events / 40 GeV" , /*xmin*/ 0, /*xmax*/1200 , /*rebin*/ 4 , /*logplot*/ 1, /*scalesig*/ -1., /*doRatio*/ 1 );
  
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( "");       labels.push_back("450 < H_{T} < 575 GeV");          nplots.push_back("");

  files.push_back( dir+"zinv_ht.root");           labels.push_back("Z #rightarrow #nu#bar{#nu} (MC)");                 nplots.push_back("srbaseL/h_mt2bins");
  files.push_back( dir+"purity.root");            labels.push_back("#gamma estimate (Data)");            nplots.push_back("srbaseL/h_mt2binspredZFailSieieData");
  files.push_back( dir+"purityRL.root");          labels.push_back("W estimate (Data)");            nplots.push_back("srbaseL/h_predZ");
  files.push_back( dir+"zinvFromDY.root");          labels.push_back("DY estimate (Data)");            nplots.push_back("srbaseL/h_mt2bins");
  makeCMSPlot(  files, labels, nplots, "MT2L_W_GJ_log", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
  //makeCMSPlot(  files, labels, nplots, "truthPtVratio", /*xtitle*/ "p_{T}^{V} [GeV]" , /*ytitle*/ "Events / 40 GeV" , /*xmin*/ 0, /*xmax*/1200 , /*rebin*/ 4 , /*logplot*/ 1, /*scalesig*/ -1., /*doRatio*/ 1 );
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( "");       labels.push_back("575 < H_{T} < 1000 GeV");          nplots.push_back("");
  files.push_back( dir+"zinv_ht.root");           labels.push_back("Z #rightarrow #nu#bar{#nu} (MC)");                 nplots.push_back("srbaseM/h_mt2bins");
  files.push_back( dir+"purity.root");            labels.push_back("#gamma estimate (Data)");            nplots.push_back("srbaseM/h_mt2binspredZFailSieieData");
  files.push_back( dir+"purityRL.root");          labels.push_back("W estimate (Data)");            nplots.push_back("srbaseM/h_predZ");
  files.push_back( dir+"zinvFromDY.root");          labels.push_back("DY estimate (Data)");            nplots.push_back("srbaseM/h_mt2bins");
  makeCMSPlot(  files, labels, nplots, "MT2M_W_GJ_log", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
  //makeCMSPlot(  files, labels, nplots, "truthPtVratio", /*xtitle*/ "p_{T}^{V} [GeV]" , /*ytitle*/ "Events / 40 GeV" , /*xmin*/ 0, /*xmax*/1200 , /*rebin*/ 4 , /*logplot*/ 1, /*scalesig*/ -1., /*doRatio*/ 1 );
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( "");       labels.push_back("1000 < H_{T} < 1500 GeV");          nplots.push_back("");
  files.push_back( dir+"zinv_ht.root");           labels.push_back("Z #rightarrow #nu#bar{#nu} (MC)");                 nplots.push_back("srbaseH/h_mt2bins");
  files.push_back( dir+"purity.root");            labels.push_back("#gamma estimate (Data)");            nplots.push_back("srbaseH/h_mt2binspredZFailSieieData");
  files.push_back( dir+"purityRL.root");          labels.push_back("W estimate (Data)");            nplots.push_back("srbaseH/h_predZ");
//  files.push_back( dir+"zinvFromDY.root");          labels.push_back("DY (Data)");            nplots.push_back("srbaseH/h_mt2bins");
  files.push_back( dir+"zinvFromDY.root");          labels.push_back("DY estimate (Data)");            nplots.push_back("srbaseH/h_mt2bins");
  makeCMSPlot(  files, labels, nplots, "MT2H_W_GJ_log", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
  //makeCMSPlot(  files, labels, nplots, "truthPtVratio", /*xtitle*/ "p_{T}^{V} [GeV]" , /*ytitle*/ "Events / 40 GeV" , /*xmin*/ 0, /*xmax*/1200 , /*rebin*/ 4 , /*logplot*/ 1, /*scalesig*/ -1., /*doRatio*/ 1 );
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( "");       labels.push_back("H_{T} > 1500 GeV");          nplots.push_back("");
  files.push_back( dir+"zinv_ht.root");           labels.push_back("Z #rightarrow #nu#bar{#nu} (MC)");                 nplots.push_back("srbaseUH/h_mt2bins");
  files.push_back( dir+"purity.root");            labels.push_back("#gamma estimate (Data)");            nplots.push_back("srbaseUH/h_mt2binspredZFailSieieData");
  files.push_back( dir+"purityRL.root");          labels.push_back("W estimate (Data)");            nplots.push_back("srbaseUH/h_predZ");
  files.push_back( dir+"zinvFromDY.root");          labels.push_back("DY estimate (Data)");            nplots.push_back("srbaseUH/h_mt2bins");
  makeCMSPlot(  files, labels, nplots, "MT2UH_W_GJ_log", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
  //makeCMSPlot(  files, labels, nplots, "truthPtVratio", /*xtitle*/ "p_{T}^{V} [GeV]" , /*ytitle*/ "Events / 40 GeV" , /*xmin*/ 0, /*xmax*/1200 , /*rebin*/ 4 , /*logplot*/ 1, /*scalesig*/ -1., /*doRatio*/ 1 );
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"zinv_ht.root");           labels.push_back("Z #rightarrow #nu#bar{#nu} (MC)");                 nplots.push_back("srbase/h_mt2bins");
  files.push_back( dir+"purity.root");            labels.push_back("#gamma estimate (Data)");            nplots.push_back("srbase/h_mt2binspredZFailSieieData");
  files.push_back( dir+"purityRL.root");          labels.push_back("W estimate (Data)");            nplots.push_back("srbase/h_predZ");
  files.push_back( dir+"zinvFromDY.root");          labels.push_back("DY estimate (Data)");            nplots.push_back("srbase/h_mt2bins");
  makeCMSPlot(  files, labels, nplots, "MT2_W_GJ", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Events / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1 );
  makeCMSPlot(  files, labels, nplots, "MT2_W_GJ_norm", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "AU " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 1, /*doRatio*/ 1 );
  makeCMSPlot(  files, labels, nplots, "MT2_W_GJ_log", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
  //makeCMSPlot(  files, labels, nplots, "truthPtVratio", /*xtitle*/ "p_{T}^{V} [GeV]" , /*ytitle*/ "Events / 40 GeV" , /*xmin*/ 0, /*xmax*/1200 , /*rebin*/ 4 , /*logplot*/ 1, /*scalesig*/ -1., /*doRatio*/ 1 );
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"zinv_ht.root");           labels.push_back("Z #rightarrow #nu#bar{#nu} (MC)");                 nplots.push_back("srbaseJ/h_mt2bins");
  files.push_back( dir+"purity.root");            labels.push_back("#gamma estimate (Data)");            nplots.push_back("srbaseJ/h_mt2binspredZFailSieieData");
  files.push_back( dir+"purityRL.root");          labels.push_back("W estimate (Data)");            nplots.push_back("srbaseJ/h_predZ");
//  makeCMSPlot(  files, labels, nplots, "Monojet_W_GJ", /*xtitle*/ "HT [GeV]" , /*ytitle*/ "Events / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1 );
//  makeCMSPlot(  files, labels, nplots, "Monojet_W_GJ_norm", /*xtitle*/ "HT [GeV]" , /*ytitle*/ "AU " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 1, /*doRatio*/ 1 );
  makeCMSPlot(  files, labels, nplots, "Monojet_W_GJ_log", /*xtitle*/ "p_{T}(jet1) [GeV]" , /*ytitle*/ "Events / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
  //makeCMSPlot(  files, labels, nplots, "truthPtVratio", /*xtitle*/ "p_{T}^{V} [GeV]" , /*ytitle*/ "Events / 40 GeV" , /*xmin*/ 0, /*xmax*/1200 , /*rebin*/ 4 , /*logplot*/ 1, /*scalesig*/ -1., /*doRatio*/ 1 );
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"purity.root");          labels.push_back("Data (FR)");            nplots.push_back("srbase/h_mt2binspurityFailSieieData");
  files.push_back( dir+"purity.root");          labels.push_back("MC (FR)");            nplots.push_back("srbase/h_mt2binspurityFailSieie");
  files.push_back( dir+"purity.root");          labels.push_back("MC (truth)");            nplots.push_back("srbase/h_mt2binspurityTrue");
  makeCMSPlot(  files, labels, nplots, "PurityMT2", /*xtitle*/ "MT2 [GeV]" , /*ytitle*/ "Purity" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 0);
  
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"purity.root");            labels.push_back("Data (FR)");            nplots.push_back("srbaseJ/h_mt2binspurityFailSieieData");
  files.push_back( dir+"purity.root");          labels.push_back("MC (FR)");            nplots.push_back("srbaseJ/h_mt2binspurityFailSieie");
  files.push_back( dir+"purity.root");          labels.push_back("MC (truth)");            nplots.push_back("srbaseJ/h_mt2binspurityTrue");
  makeCMSPlot(  files, labels, nplots, "PurityMonojetHT", /*xtitle*/ "HT [GeV]" , /*ytitle*/ "Purity" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 0);
  
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"purity.root");            labels.push_back("Data (FR)");            nplots.push_back("srbaseIncl/h_njbinspurityFailSieieData");
  files.push_back( dir+"purity.root");            labels.push_back("MC (FR)");            nplots.push_back("srbaseIncl/h_njbinspurityFailSieie");
  files.push_back( dir+"purity.root");            labels.push_back("MC (truth)");            nplots.push_back("srbaseIncl/h_njbinspurityTrue");
  makeCMSPlot(  files, labels, nplots, "PurityNJ", /*xtitle*/ "N(jets)" , /*ytitle*/ "Photon Purity" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 0 );
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"purity.root");            labels.push_back("Data (FR)");            nplots.push_back("srbaseIncl/h_nbjbinspurityFailSieieData");
  files.push_back( dir+"purity.root");            labels.push_back("MC (FR)");            nplots.push_back("srbaseIncl/h_nbjbinspurityFailSieie");
  files.push_back( dir+"purity.root");            labels.push_back("MC (truth)");            nplots.push_back("srbaseIncl/h_nbjbinspurityTrue");
  makeCMSPlot(  files, labels, nplots, "PurityNBJ", /*xtitle*/ "N(b-jets)" , /*ytitle*/ "Photon Purity" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 0 );
  
  files.clear(); labels.clear(); nplots.clear();

  files.push_back( dir+"purity.root");            labels.push_back("Data (FR)");            nplots.push_back("srbaseIncl/h_bosonptbinspurityFailSieieData");
  files.push_back( dir+"purity.root");            labels.push_back("MC (FR)");            nplots.push_back("srbaseIncl/h_bosonptbinspurityFailSieie");
  files.push_back( dir+"purity.root");            labels.push_back("MC (truth)");            nplots.push_back("srbaseIncl/h_bosonptbinspurityTrue");
  makeCMSPlot(  files, labels, nplots, "PurityPTV", /*xtitle*/ "Boson p_T [GeV]" , /*ytitle*/ "Photon Purity" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 0 );
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"purity.root");            labels.push_back("Data (FR)");            nplots.push_back("srbaseIncl/h_htbinspurityFailSieieData");
  files.push_back( dir+"purity.root");            labels.push_back("MC (FR)");            nplots.push_back("srbaseIncl/h_htbinspurityFailSieie");
  files.push_back( dir+"purity.root");            labels.push_back("MC (truth)");            nplots.push_back("srbaseIncl/h_htbinspurityTrue");
  makeCMSPlot(  files, labels, nplots, "PurityHT", /*xtitle*/ "HT [GeV]" , /*ytitle*/ "Photon Purity" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 0 );
  
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"dyjetsll_ht.root");            labels.push_back("MC");            nplots.push_back("crdybase/h_mt2bins");
  files.push_back( dir+"gjets_dr0p05_ht.root");          labels.push_back("MC");            nplots.push_back("crgjbase/h_mt2bins");
  files.push_back( dir+"data_Run2016.root");            labels.push_back("Data (bkg subtr)");            nplots.push_back("crdybase/h_mt2bins");
  files.push_back( dir+"purity.root");          labels.push_back("Data (bkg subtr)");            nplots.push_back("srbase/h_mt2binsphotonestimateFailSieieData");
//    files.push_back( dir+"data_Run2016.root");            labels.push_back("Data (full)");            nplots.push_back("crdybase/h_mt2bins");
//    files.push_back( dir+"data_Run2016.root");          labels.push_back("Data (full)");            nplots.push_back("crgjbase/h_mt2bins");
  makeCMSPlotRatios(  files, labels, nplots, "DoubleRatioMT2", /*xtitle*/ "MT2 [GeV]" , /*ytitle*/ "R(Z_{ll} / #gamma)" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1);
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"wjets_ht.root");            labels.push_back("MC");            nplots.push_back("crrlbase/h_mt2bins");
  files.push_back( dir+"gjets_dr0p05_ht.root");          labels.push_back("MC");            nplots.push_back("crgjbase/h_mt2bins");
  files.push_back( dir+"purityRL.root");            labels.push_back("Data (bkg subtr)");            nplots.push_back("srbase/h_predW");
  files.push_back( dir+"purity.root");          labels.push_back("Data (bkg subtr)");            nplots.push_back("srbase/h_mt2binsphotonestimateFailSieieData");
  //    files.push_back( dir+"data_Run2016.root");            labels.push_back("Data (full)");            nplots.push_back("crdybase/h_mt2bins");
  //    files.push_back( dir+"data_Run2016.root");          labels.push_back("Data (full)");            nplots.push_back("crgjbase/h_mt2bins");
  makeCMSPlotRatios(  files, labels, nplots, "DoubleRatioMT2_WG", /*xtitle*/ "MT2 [GeV]" , /*ytitle*/ "R(W_{l#nu} / #gamma)" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1);
  
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"dyjetsll_ht.root");            labels.push_back("MC");            nplots.push_back("crdybaseJ/h_mt2bins");
  files.push_back( dir+"gjets_dr0p05_ht.root");          labels.push_back("MC");            nplots.push_back("crgjbaseJ/h_mt2bins");
  files.push_back( dir+"data_Run2016.root");            labels.push_back("Data (bkg subtr)");            nplots.push_back("crdybaseJ/h_mt2bins");
  files.push_back( dir+"purity.root");          labels.push_back("Data (bkg subtr)");            nplots.push_back("srbaseJ/h_mt2binsphotonestimateFailSieieData");
  makeCMSPlotRatios(  files, labels, nplots, "DoubleRatioMonojet", /*xtitle*/ "p_{T}(jet1)  [GeV]" , /*ytitle*/ "R(Z_{ll} / #gamma)" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1);
  

  
  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir+"dyjetsll_ht.root");            labels.push_back("MC");            nplots.push_back("crdybase/h_mt2bins");
//  files.push_back( dir+"gjets_ht.root");          labels.push_back("MC");            nplots.push_back("crgjbase/h_mt2bins");
//  files.push_back( dir+"data_Run2016.root");            labels.push_back("Data (bkg subtr)");            nplots.push_back("crdybase/h_mt2bins");
//  files.push_back( dir+"purity.root");          labels.push_back("Data (bkg subtr)");            nplots.push_back("srbase/h_estimateFailSieieData");
//  files.push_back( dir+"data_Run2016.root");            labels.push_back("Data (full)");            nplots.push_back("crdybase/h_mt2bins");
//  files.push_back( dir+"data_Run2016.root");          labels.push_back("Data (full)");            nplots.push_back("crgjbase/h_mt2bins");
//  makeCMSPlotRatios(  files, labels, nplots, "DoubleRatioMT2", /*xtitle*/ "MT2 [GeV]" , /*ytitle*/ "DY/#gamma Ratio" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1);
  

//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir+"dyjetsll_ht.root");            labels.push_back("MC+bkg");            nplots.push_back("crdybase/h_mt2bins");
//  files.push_back( dir+"purityMC.root");          labels.push_back("MC+bkg");            nplots.push_back("srbase/h_estimateFailSieieData");
//  files.push_back( dir+"data_Run2016.root");            labels.push_back("Data (full)");            nplots.push_back("crdybase/h_mt2bins");
//  files.push_back( dir+"data_Run2016.root");          labels.push_back("Data (full)");            nplots.push_back("crgjbase/h_mt2bins");
//  makeCMSPlotRatios(  files, labels, nplots, "DoubleRatioOutOfBox", /*xtitle*/ "MT2 [GeV]" , /*ytitle*/ "DY/#gamma Ratio" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1);
//  
//  files.clear(); labels.clear(); nplots.clear();
//
//  files.push_back( dir+"dyjetsll_ht.root");            labels.push_back("MC+bkg");            nplots.push_back("crdybase/h_htbins");
//  files.push_back( dir+"qcdplusgjet.root");          labels.push_back("MC+bkg");            nplots.push_back("crgjbase/h_htbins");
//  files.push_back( dir+"data_Run2016.root");            labels.push_back("Data (full)");            nplots.push_back("crdybase/h_htbins");
//  files.push_back( dir+"data_Run2016.root");          labels.push_back("Data (full)");            nplots.push_back("crgjbase/h_htbins");
//  files.push_back( dir+"dyjetsll_ht.root");            labels.push_back("MC");            nplots.push_back("crdybase/h_htbins");
//  files.push_back( dir+"gjets_ht.root");          labels.push_back("MC");            nplots.push_back("crgjbase/h_htbins");
//  makeCMSPlotRatios(  files, labels, nplots, "DoubleRatioHTOutOfBox", /*xtitle*/ "HT [GeV]" , /*ytitle*/ "DY/#gamma Ratio" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1);
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"dyjetsll_ht.root");            labels.push_back("MC");            nplots.push_back("crdybase/h_htbins");
  files.push_back( dir+"gjets_dr0p05_ht.root");          labels.push_back("MC");            nplots.push_back("crgjbase/h_htbins");
  files.push_back( dir+"data_Run2016.root");            labels.push_back("Data (bkg subtr)");            nplots.push_back("crdybase/h_htbins");
  files.push_back( dir+"purity.root");          labels.push_back("Data (bkg subtr)");            nplots.push_back("srbase/h_htbinsphotonestimateFailSieieData");
//    files.push_back( dir+"data_Run2016.root");            labels.push_back("Data (full)");            nplots.push_back("crdybase/h_htbins");
//    files.push_back( dir+"data_Run2016.root");          labels.push_back("Data (full)");            nplots.push_back("crgjbase/h_htbins");
  makeCMSPlotRatios(  files, labels, nplots, "DoubleRatioHT", /*xtitle*/ "HT [GeV]" , /*ytitle*/ "R(Z_{ll} / #gamma)" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1);
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"dyjetsll_ht.root");            labels.push_back("MC");            nplots.push_back("crdybase/h_njbins");
  files.push_back( dir+"gjets_dr0p05_ht.root");          labels.push_back("MC");            nplots.push_back("crgjbase/h_njbins");
  files.push_back( dir+"data_Run2016.root");            labels.push_back("Data (bkg subtr)");            nplots.push_back("crdybase/h_njbins");
  files.push_back( dir+"purity.root");          labels.push_back("Data (bkg subtr)");            nplots.push_back("srbase/h_njbinsphotonestimateFailSieieData");
  makeCMSPlotRatios(  files, labels, nplots, "DoubleRatioNJ", /*xtitle*/ "N(jets)" , /*ytitle*/ "R(Z_{ll} / #gamma)" , /*xmin*/ 2, /*xmax*/12 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1);
  
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"dyjetsll_ht.root");            labels.push_back("MC");            nplots.push_back("crdybase/h_nbjbins");
  files.push_back( dir+"gjets_dr0p05_ht.root");                labels.push_back("MC");            nplots.push_back("crgjbase/h_nbjbins");
  files.push_back( dir+"data_Run2016.root");            labels.push_back("Data (bkg subtr)");            nplots.push_back("crdybase/h_nbjbins");
  files.push_back( dir+"purity.root");                   labels.push_back("Data (bkg subtr)");            nplots.push_back("srbase/h_nbjbinsphotonestimateFailSieieData");
//  files.push_back( dir+"data_Run2016.root");            labels.push_back("Data (full)");            nplots.push_back("crdybase/h_nbjbins");
//  files.push_back( dir+"data_Run2016.root");          labels.push_back("Data (full)");            nplots.push_back("crgjbase/h_nbjbins");
  makeCMSPlotRatios(  files, labels, nplots, "DoubleRatioNB", /*xtitle*/ "N(bjets)" , /*ytitle*/ "R(Z_{ll} / #gamma)" , /*xmin*/ 0, /*xmax*/3 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1);
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"dyjetsll_ht.root");            labels.push_back("MC");            nplots.push_back("crdybaseIncl/h_htbins");
  files.push_back( dir+"gjets_dr0p05_ht.root");          labels.push_back("MC");            nplots.push_back("crgjbaseIncl/h_htbins");
  files.push_back( dir+"data_Run2016.root");            labels.push_back("Data (bkg subtr)");            nplots.push_back("crdybaseIncl/h_htbins");
  files.push_back( dir+"purity.root");          labels.push_back("Data (bkg subtr)");            nplots.push_back("srbaseIncl/h_htbinsphotonestimateFailSieieData");
  makeCMSPlotRatios(  files, labels, nplots, "DoubleRatioHTincl", /*xtitle*/ "HT [GeV]" , /*ytitle*/ "R(Z_{ll} / #gamma)" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1);
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"dyjetsll_ht.root");            labels.push_back("MC");            nplots.push_back("crdybaseIncl/h_njbins");
  files.push_back( dir+"gjets_dr0p05_ht.root");          labels.push_back("MC");            nplots.push_back("crgjbaseIncl/h_njbins");
  files.push_back( dir+"data_Run2016.root");            labels.push_back("Data (bkg subtr)");            nplots.push_back("crdybaseIncl/h_njbins");
  files.push_back( dir+"purity.root");          labels.push_back("Data (bkg subtr)");            nplots.push_back("srbaseIncl/h_njbinsphotonestimateFailSieieData");
  makeCMSPlotRatios(  files, labels, nplots, "DoubleRatioNJincl", /*xtitle*/ "N(jets)" , /*ytitle*/ "R(Z_{ll} / #gamma)" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1);
  
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"dyjetsll_ht.root");            labels.push_back("MC");            nplots.push_back("crdybaseIncl/h_nbjbins");
  files.push_back( dir+"gjets_dr0p05_ht.root");                labels.push_back("MC");            nplots.push_back("crgjbaseIncl/h_nbjbins");
  files.push_back( dir+"data_Run2016.root");            labels.push_back("Data (bkg subtr)");            nplots.push_back("crdybaseIncl/h_nbjbins");
  files.push_back( dir+"purity.root");                   labels.push_back("Data (bkg subtr)");            nplots.push_back("srbaseIncl/h_nbjbinsphotonestimateFailSieieData");
  makeCMSPlotRatios(  files, labels, nplots, "DoubleRatioNBincl", /*xtitle*/ "N(bjets)" , /*ytitle*/ "R(Z_{ll} / #gamma)" , /*xmin*/ 0, /*xmax*/3 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1);
  

  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"dyjetsll_ht.root");            labels.push_back("MC");            nplots.push_back("crdybaseIncl/h_bosonptbins");
  files.push_back( dir+"gjets_dr0p05_ht.root");        labels.push_back("MC");            nplots.push_back("crgjbaseIncl/h_bosonptbins");
  files.push_back( dir+"data_Run2016.root");            labels.push_back("Data (bkg subtr)");            nplots.push_back("crdybaseIncl/h_bosonptbins");
  files.push_back( dir+"purity.root");                   labels.push_back("Data (bkg subtr)");            nplots.push_back("srbaseIncl/h_bosonptbinsphotonestimateFailSieieData");
  makeCMSPlotRatios(  files, labels, nplots, "DoubleRatioVPTincl", /*xtitle*/ "Boson p_T [GeV]" , /*ytitle*/ "R(Z_{ll} / #gamma)" , /*xmin*/ 200, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1);
  
  

   dir2 = "/Users/giovannizevidellaporta/UCSD/MT2/Zinvisible/MT2babies/V00-01-07_signalSyst/";

//
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"T1bbbb_mGluino-1600.root");           labels.push_back("T1bbbb 1600 1");                 nplots.push_back("srbase/h_ht");
//  files.push_back( dir2+"/NVTX/T1bbbb_mGluino-1600.root");           labels.push_back("T1bbbb 1600 1, nvtx");                 nplots.push_back("srbase/h_ht");
//  makeCMSPlot(  files, labels, nplots, "NVTX_HT", /*xtitle*/ "HT" , /*ytitle*/ "Events / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 4 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1 );
//
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"T1bbbb_mGluino-1600.root");           labels.push_back("T1bbbb 1600 1");                 nplots.push_back("srbase/h_nJet30");
//  files.push_back( dir2+"/NVTX/T1bbbb_mGluino-1600.root");           labels.push_back("T1bbbb 1600 1, nvtx");                 nplots.push_back("srbase/h_nJet30");
//  makeCMSPlot(  files, labels, nplots, "NVTX_NJ", /*xtitle*/ "NJ" , /*ytitle*/ "Events / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"T1bbbb_mGluino-1600.root");           labels.push_back("T1bbbb 1600 1");                 nplots.push_back("srbase/h_nBJet20");
//  files.push_back( dir2+"/NVTX/T1bbbb_mGluino-1600.root");           labels.push_back("T1bbbb 1600 1, nvtx");                 nplots.push_back("srbase/h_nBJet20");
//  makeCMSPlot(  files, labels, nplots, "NVTX_NB", /*xtitle*/ "NB" , /*ytitle*/ "Events / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"T1bbbb_mGluino-1600.root");           labels.push_back("T1bbbb 1600 1");                 nplots.push_back("srbase/h_met");
//  files.push_back( dir2+"/NVTX/T1bbbb_mGluino-1600.root");           labels.push_back("T1bbbb 1600 1, nvtx");                 nplots.push_back("srbase/h_met");
//  makeCMSPlot(  files, labels, nplots, "NVTX_MET", /*xtitle*/ "MET" , /*ytitle*/ "Events / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 4 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir2+"T1bbbb_mGluino-1600.root");           labels.push_back("T1bbbb 1600 1");                 nplots.push_back("srbase/h_mt2");
//  files.push_back( dir2+"/NVTX/T1bbbb_mGluino-1600.root");           labels.push_back("T1bbbb 1600 1, nvtx");                 nplots.push_back("srbase/h_mt2");
//  makeCMSPlot(  files, labels, nplots, "NVTX_MT2", /*xtitle*/ "MT2" , /*ytitle*/ "Events / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 4 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1 );
  
  
    files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir2+"T1bbbb_mGluino-875-900-925.root");           labels.push_back("0 ExtraPartons");                 nplots.push_back("nocut/h_nJet30With0ExtraPartons");
  files.push_back( dir2+"T1bbbb_mGluino-875-900-925.root");           labels.push_back("1 ExtraPartons");                 nplots.push_back("nocut/h_nJet30With1ExtraPartons");
  files.push_back( dir2+"T1bbbb_mGluino-875-900-925.root");           labels.push_back("2 ExtraPartons");                 nplots.push_back("nocut/h_nJet30With2ExtraPartons");
    makeCMSPlot(  files, labels, nplots, "CompressedNJetsPerParton", /*xtitle*/ "N(jets)" , /*ytitle*/ "Events / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 0 );
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir2+"T1bbbb_mGluino-875-900-925.root");           labels.push_back("0 ExtraPartons");                 nplots.push_back("nocut/h_JetPartonFlavorWith0ExtraPartons");
  files.push_back( dir2+"T1bbbb_mGluino-875-900-925.root");           labels.push_back("1 ExtraPartons");                 nplots.push_back("nocut/h_JetPartonFlavorWith1ExtraPartons");
  files.push_back( dir2+"T1bbbb_mGluino-875-900-925.root");           labels.push_back("2 ExtraPartons");                 nplots.push_back("nocut/h_JetPartonFlavorWith2ExtraPartons");
  makeCMSPlot(  files, labels, nplots, "CompressedPartonFlavor", /*xtitle*/ "Parton Flavor" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 1, /*doRatio*/ 0 );
 
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir2+"T1bbbb_mGluino-875-900-925.root");           labels.push_back("0 ExtraPartons");                 nplots.push_back("nocut/h_JetHadronFlavorWith0ExtraPartons");
  files.push_back( dir2+"T1bbbb_mGluino-875-900-925.root");           labels.push_back("1 ExtraPartons");                 nplots.push_back("nocut/h_JetHadronFlavorWith1ExtraPartons");
  files.push_back( dir2+"T1bbbb_mGluino-875-900-925.root");           labels.push_back("2 ExtraPartons");                 nplots.push_back("nocut/h_JetHadronFlavorWith2ExtraPartons");
  makeCMSPlot(  files, labels, nplots, "CompressedHadronFlavor", /*xtitle*/ "Hadron Flavor" , /*ytitle*/ "Fraction / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 1, /*doRatio*/ 0 );

  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir2+"T1bbbb_mGluino-875-900-925.root");           labels.push_back("1st ExtraParton");                 nplots.push_back("nocut/h_extra1pt");
  files.push_back( dir2+"T1bbbb_mGluino-875-900-925.root");           labels.push_back("2nd ExtraParton");                 nplots.push_back("nocut/h_extra2pt");
  makeCMSPlot(  files, labels, nplots, "ExtraPartonPt", /*xtitle*/ "Extra Parton p_{T}" , /*ytitle*/ "Events / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 0 );

  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir2+"T1bbbb_mGluino-875-900-925.root");           labels.push_back("1st Jet");                 nplots.push_back("nocut/h_J0pt");
  files.push_back( dir2+"T1bbbb_mGluino-875-900-925.root");           labels.push_back("2nd Jet");                 nplots.push_back("nocut/h_J1pt");
  makeCMSPlot(  files, labels, nplots, "JetPt", /*xtitle*/ "Jet p_{T}" , /*ytitle*/ "Events / Bin " , /*xmin*/ 0, /*xmax*/600 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 0 );
  makeCMSPlot(  files, labels, nplots, "JetPt_log", /*xtitle*/ "Jet p_{T}" , /*ytitle*/ "Events / Bin " , /*xmin*/ 0, /*xmax*/600 , /*rebin*/ 1 , /*logplot*/ 1, /*norm*/ 0, /*doRatio*/ 0 );

  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir2+"ttsl_mg_lo.root");           labels.push_back("1st ExtraGluon");                 nplots.push_back("nocut/h_extrag1pt");
  files.push_back( dir2+"ttsl_mg_lo.root");           labels.push_back("2nd ExtraGluon");                 nplots.push_back("nocut/h_extrag2pt");
  makeCMSPlot(  files, labels, nplots, "ExtraPartonPtTtbar", /*xtitle*/ "Extra Parton p_{T}" , /*ytitle*/ "Events / Bin " , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 0 );

  
  
//  
//  
//  TString dir4 = "~/UCSD/MT2/MT2AnalysisOldSep2015/MT2looper/output/V00-00-08recoIsoskim/";
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir4+"zinv_ht.root");            labels.push_back("Z");          nplots.push_back("srbase/h_jetflavor");
//  files.push_back( dir4+"gjets_ht.root");            labels.push_back("#gamma");      nplots.push_back("crgjbase/h_jetflavor");
//  makeCMSPlot(  files, labels, nplots, "ratioJetID", /*xtitle*/ "PDG ID" , /*ytitle*/ "A.U." , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 1, /*doRatio*/ 0 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir4+"zinv_ht.root");            labels.push_back("Z (450 < HT < 575)");          nplots.push_back("srL/h_jetflavor");
//  files.push_back( dir4+"gjets_ht.root");            labels.push_back("#gamma (450 < HT < 575)");      nplots.push_back("crgjL/h_jetflavor");
//  makeCMSPlot(  files, labels, nplots, "ratioJetIDL", /*xtitle*/ "PDG ID" , /*ytitle*/ "A.U." , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 1, /*doRatio*/ 0 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir4+"zinv_ht.root");            labels.push_back("Z (575 < HT < 1000)");          nplots.push_back("srM/h_jetflavor");
//  files.push_back( dir4+"gjets_ht.root");            labels.push_back("#gamma (575 < HT < 1000)");      nplots.push_back("crgjM/h_jetflavor");
//  makeCMSPlot(  files, labels, nplots, "ratioJetIDM", /*xtitle*/ "PDG ID" , /*ytitle*/ "A.U." , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 1, /*doRatio*/ 0 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir4+"zinv_ht.root");            labels.push_back("Z (HT > 1000)");          nplots.push_back("srH/h_jetflavor");
//  files.push_back( dir4+"gjets_ht.root");            labels.push_back("#gamma (HT > 1000)");      nplots.push_back("crgjH/h_jetflavor");
//  makeCMSPlot(  files, labels, nplots, "ratioJetIDH", /*xtitle*/ "PDG ID" , /*ytitle*/ "A.U." , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 1, /*doRatio*/ 0 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( dir4+"zinv_ht.root");            labels.push_back("Z");          nplots.push_back("srbase/h_dijetflavor");
//  files.push_back( dir4+"gjets_ht.root");            labels.push_back("#gamma");      nplots.push_back("crgjbase/h_dijetflavor");
//  makeCMSPlot(  files, labels, nplots, "ratioDiJetID", /*xtitle*/ "Jet Flavor combination ", /*ytitle*/ "A.U." , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 1, /*doRatio*/ 0 );
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( "");       labels.push_back("Low HT (450-575 GeV)");          nplots.push_back("");
//  files.push_back( dir4+"zinv_ht.root");       labels.push_back("Z 2-3j");          nplots.push_back("sr2L/h_mt2bins");
//  files.push_back( dir4+"zinv_ht.root");       labels.push_back("Z 2-3j");          nplots.push_back("sr1L/h_mt2bins");
//  files.push_back( dir4+"zinv_ht.root");       labels.push_back("Z 4j+");          nplots.push_back("sr6L/h_mt2bins");
//  files.push_back( dir4+"zinv_ht.root");       labels.push_back("Z 4j+");          nplots.push_back("sr5L/h_mt2bins");
//  files.push_back( dir4+"gjets_ht.root");       labels.push_back("Gamma 2-3j");          nplots.push_back("crgj2L/h_mt2bins");
//  files.push_back( dir4+"gjets_ht.root");       labels.push_back("Gamma 2-3j");          nplots.push_back("crgj1L/h_mt2bins");
//  files.push_back( dir4+"gjets_ht.root");       labels.push_back("Gamma 4j+");          nplots.push_back("crgj6L/h_mt2bins");
//  files.push_back( dir4+"gjets_ht.root");       labels.push_back("Gamma 4j+");          nplots.push_back("crgj5L/h_mt2bins");
//  makeCMSPlotRatios(  files, labels, nplots, "ratioZbZ0bL", /*xtitle*/ "MT2" , /*ytitle*/ "R(1b/0b)" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 0);
//
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( "");       labels.push_back("Medium HT (575-1000 GeV)");          nplots.push_back("");
//  files.push_back( dir4+"zinv_ht.root");       labels.push_back("Z 2-3j");          nplots.push_back("sr2M/h_mt2bins");
//  files.push_back( dir4+"zinv_ht.root");       labels.push_back("Z 2-3j");          nplots.push_back("sr1M/h_mt2bins");
//  files.push_back( dir4+"zinv_ht.root");       labels.push_back("Z 4j+");          nplots.push_back("sr6M/h_mt2bins");
//  files.push_back( dir4+"zinv_ht.root");       labels.push_back("Z 4j+");          nplots.push_back("sr5M/h_mt2bins");
//  files.push_back( dir4+"gjets_ht.root");       labels.push_back("Gamma 2-3j");          nplots.push_back("crgj2M/h_mt2bins");
//  files.push_back( dir4+"gjets_ht.root");       labels.push_back("Gamma 2-3j");          nplots.push_back("crgj1M/h_mt2bins");
//  files.push_back( dir4+"gjets_ht.root");       labels.push_back("Gamma 4j+");          nplots.push_back("crgj6M/h_mt2bins");
//  files.push_back( dir4+"gjets_ht.root");       labels.push_back("Gamma 4j+");          nplots.push_back("crgj5M/h_mt2bins");
//  makeCMSPlotRatios(  files, labels, nplots, "ratioZbZ0bM", /*xtitle*/ "MT2" , /*ytitle*/ "R(1b/0b)" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 0);
//
//  
//  files.clear(); labels.clear(); nplots.clear();
//  files.push_back( "");       labels.push_back("High HT (#geq 1000 GeV)");          nplots.push_back("");
//  files.push_back( dir4+"zinv_ht.root");       labels.push_back("Z 2-3j");          nplots.push_back("sr2H/h_mt2bins");
//  files.push_back( dir4+"zinv_ht.root");       labels.push_back("Z 2-3j");          nplots.push_back("sr1H/h_mt2bins");
//  files.push_back( dir4+"zinv_ht.root");       labels.push_back("Z 4j+");          nplots.push_back("sr6H/h_mt2bins");
//  files.push_back( dir4+"zinv_ht.root");       labels.push_back("Z 4j+");          nplots.push_back("sr5H/h_mt2bins");
//  files.push_back( dir4+"gjets_ht.root");       labels.push_back("Gamma 2-3j");          nplots.push_back("crgj2H/h_mt2bins");
//  files.push_back( dir4+"gjets_ht.root");       labels.push_back("Gamma 2-3j");          nplots.push_back("crgj1H/h_mt2bins");
//  files.push_back( dir4+"gjets_ht.root");       labels.push_back("Gamma 4j+");          nplots.push_back("crgj6H/h_mt2bins");
//  files.push_back( dir4+"gjets_ht.root");       labels.push_back("Gamma 4j+");          nplots.push_back("crgj5H/h_mt2bins");
//  makeCMSPlotRatios(  files, labels, nplots, "ratioZbZ0bH", /*xtitle*/ "MT2" , /*ytitle*/ "R(1b/0b)" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 0);
//
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"gjets_dr0p05_ht.root");            labels.push_back("0.05 ");            nplots.push_back("crgjbaseIncl/h_htbins");
  files.push_back( dir+"gjets_dr0p4_ht.root");            labels.push_back("0.4");            nplots.push_back("crgjbaseIncl/h_htbins");
  makeCMSPlot(  files, labels, nplots, "GJets04vs005_HT", /*xtitle*/ "HT [GeV]" , /*ytitle*/ "Events" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1 );
  makeCMSPlot(  files, labels, nplots, "GJets04vs005_HTnorm", /*x*/  "HT [GeV]" , /*ytitle*/ "Events" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 1, /*doRatio*/ 1 );
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"gjets_dr0p05_ht.root");            labels.push_back("0.05 ");            nplots.push_back("crgjbase/h_mt2bins");
  files.push_back( dir+"gjets_dr0p4_ht.root");            labels.push_back("0.4");            nplots.push_back("crgjbase/h_mt2bins");
  makeCMSPlot(  files, labels, nplots, "GJets04vs005_MT2", /*xtitle*/ "MT2 [GeV]" , /*ytitle*/ "Events" , /*xmin*/ 0, /*xmax*/995 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1 );
  makeCMSPlot(  files, labels, nplots, "GJets04vs005_MT2norm", /*xt*/ "MT2 [GeV]" , /*ytitle*/ "Events" , /*xmin*/ 0, /*xmax*/995 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 1, /*doRatio*/ 1 );
  
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"gjets_dr0p05_ht.root");            labels.push_back("0.05 ");            nplots.push_back("crgjbaseIncl/h_njbins");
  files.push_back( dir+"gjets_dr0p4_ht.root");            labels.push_back("0.4");            nplots.push_back("crgjbaseIncl/h_njbins");
  makeCMSPlot(  files, labels, nplots, "GJets04vs005_NJ", /*xtitle*/ "N(jets)" , /*ytitle*/ "Events" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1 );
  makeCMSPlot(  files, labels, nplots, "GJets04vs005_NJnorm", /*xe*/ "N(jets)" , /*ytitle*/ "Events" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 1, /*doRatio*/ 1 );

  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"gjets_dr0p05_ht.root");            labels.push_back("0.05 ");            nplots.push_back("crgjbaseIncl/h_nbjbins");
  files.push_back( dir+"gjets_dr0p4_ht.root");            labels.push_back("0.4");            nplots.push_back("crgjbaseIncl/h_nbjbins");
  makeCMSPlot(  files, labels, nplots, "GJets04vs005_NBJ", /*xtitle*/ "N(b-jets)" , /*ytitle*/ "Events" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1 );
  makeCMSPlot(  files, labels, nplots, "GJets04vs005_NBJnorm", /*xe*/ "N(b-jets)" , /*ytitle*/ "Events" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 1, /*doRatio*/ 1 );
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"gjets_dr0p05_ht.root");            labels.push_back("0.05 ");            nplots.push_back("crgjbaseIncl/h_drMinParton");
  files.push_back( dir+"gjets_dr0p4_ht.root");            labels.push_back("0.4");            nplots.push_back("crgjbaseIncl/h_drMinParton");
  makeCMSPlot(  files, labels, nplots, "GJets04vs005_DR", /*xtitle*/ "minDR(photon, parton)" , /*ytitle*/ "Events" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 0, /*doRatio*/ 1 );
  makeCMSPlot(  files, labels, nplots, "GJets04vs005_DRnorm", /*xe*/ "minDR(photon, parton)" , /*ytitle*/ "Events" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 1 , /*logplot*/ 0, /*norm*/ 1, /*doRatio*/ 1 );
  
  
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"zinv_ht.root");            labels.push_back("HT VL, NJ 2-3, NB 0");            nplots.push_back("sr1VL/h_mt2");
  files.push_back( dir+"zinv_ht.root");            labels.push_back("HT VL, NJ 2-3, NB 1");            nplots.push_back("sr2VL/h_mt2");
  makeCMSPlot(  files, labels, nplots, "ZinvMT2vsNB_sr1VL", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Events" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 5 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"zinv_ht.root");            labels.push_back("HT L, NJ 2-3, NB 0");            nplots.push_back("sr1L/h_mt2");
  files.push_back( dir+"zinv_ht.root");            labels.push_back("HT L, NJ 2-3, NB 1");            nplots.push_back("sr2L/h_mt2");
  makeCMSPlot(  files, labels, nplots, "ZinvMT2vsNB_sr1L", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Events" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 5 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"zinv_ht.root");            labels.push_back("HT M, NJ 2-3, NB 0");            nplots.push_back("sr1M/h_mt2");
  files.push_back( dir+"zinv_ht.root");            labels.push_back("HT M, NJ 2-3, NB 1");            nplots.push_back("sr2M/h_mt2");
  makeCMSPlot(  files, labels, nplots, "ZinvMT2vsNB_sr1M", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Events" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 10 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"zinv_ht.root");            labels.push_back("HT H, NJ 2-3, NB 0");            nplots.push_back("sr1H/h_mt2");
  files.push_back( dir+"zinv_ht.root");            labels.push_back("HT H, NJ 2-3, NB 1");            nplots.push_back("sr2H/h_mt2");
  makeCMSPlot(  files, labels, nplots, "ZinvMT2vsNB_sr1H", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Events" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 10 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"zinv_ht.root");            labels.push_back("HT UH, NJ 2-3, NB 0");            nplots.push_back("sr1UH/h_mt2");
  files.push_back( dir+"zinv_ht.root");            labels.push_back("HT UH, NJ 2-3, NB 1");            nplots.push_back("sr2UH/h_mt2");
  makeCMSPlot(  files, labels, nplots, "ZinvMT2vsNB_sr1UH", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Events" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 20 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );

  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"zinv_ht.root");            labels.push_back("HT VL, NJ 4-6, NB 0");            nplots.push_back("sr4VL/h_mt2");
  files.push_back( dir+"zinv_ht.root");            labels.push_back("HT VL, NJ 4-6, NB 1");            nplots.push_back("sr5VL/h_mt2");
  makeCMSPlot(  files, labels, nplots, "ZinvMT2vsNB_sr4VL", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Events" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 5 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"zinv_ht.root");            labels.push_back("HT L, NJ 4-6, NB 0");            nplots.push_back("sr4L/h_mt2");
  files.push_back( dir+"zinv_ht.root");            labels.push_back("HT L, NJ 4-6, NB 1");            nplots.push_back("sr5L/h_mt2");
  makeCMSPlot(  files, labels, nplots, "ZinvMT2vsNB_sr4L", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Events" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 5 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"zinv_ht.root");            labels.push_back("HT M, NJ 4-6, NB 0");            nplots.push_back("sr4M/h_mt2");
  files.push_back( dir+"zinv_ht.root");            labels.push_back("HT M, NJ 4-6, NB 1");            nplots.push_back("sr5M/h_mt2");
  makeCMSPlot(  files, labels, nplots, "ZinvMT2vsNB_sr4M", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Events" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 5 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"zinv_ht.root");            labels.push_back("HT H, NJ 4-6, NB 0");            nplots.push_back("sr4H/h_mt2");
  files.push_back( dir+"zinv_ht.root");            labels.push_back("HT H, NJ 4-6, NB 1");            nplots.push_back("sr5H/h_mt2");
  makeCMSPlot(  files, labels, nplots, "ZinvMT2vsNB_sr4H", /*xtitle*/ "M_{T2} [GeV]" , /*ytitle*/ "Events" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 10 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
  
  files.clear(); labels.clear(); nplots.clear();
  files.push_back( dir+"zinv_ht.root");            labels.push_back("HT UH, NJ 4-6, NB 0");            nplots.push_back("sr4UH/h_mt2");
  files.push_back( dir+"zinv_ht.root");            labels.push_back("HT UH, NJ 4-6, NB 1");            nplots.push_back("sr5UH/h_mt2");
  makeCMSPlot(  files, labels, nplots, "ZinvMT2vsNB_sr4UH", /*xtitle*/ "HT [GeV]" , /*ytitle*/ "Events" , /*xmin*/ 0, /*xmax*/0 , /*rebin*/ 20 , /*logplot*/ 1, /*norm*/ 1, /*doRatio*/ 1 );
  
  

   
  return;
}




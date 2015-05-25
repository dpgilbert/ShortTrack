#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <sstream>
#include <cmath>

#include "TROOT.h"
#include "TLatex.h"
#include "TString.h"
#include "TH2.h"
#include "THStack.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TPaveText.h"
#include "TStyle.h"
#include "TKey.h"

#include "plotUtilities.C"
#include "CMS_lumi.C"

using namespace std;

// -- for CMS_lumi label

const int iPeriod = 4; // 13 tev

// iPos drives the position of the CMS logo in the plot
// iPos=11 : top-left, left-aligned
// iPos=33 : top-right, right-aligned
// iPos=22 : center, centered
// mode generally : 
//   iPos = 10*(alignement 1/2/3) + position (1/2/3 = left/center/right)
const int iPos = 11; 

//_______________________________________________________________________________
TCanvas* makePlot( const vector<TFile*>& samples , const vector<string>& names , const string& histdir , const string& histname , const string& xtitle , const string& ytitle , float xmin , float xmax , int rebin = 1 , bool logplot = true, bool printplot = false, float scalesig = -1. ) {

  cout << "-- plotting histdir: " << histdir << ", histname: " << histname << endl;

  gStyle->SetOptStat("");
  gStyle->SetCanvasColor(0);
  gStyle->SetPadGridX(0);
  gStyle->SetPadGridY(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetFrameBorderMode(0);

  //these 4 lines shift plot to make room for axis labels
  gStyle->SetPadTopMargin(0.08);
  gStyle->SetPadBottomMargin(0.12);
  gStyle->SetPadLeftMargin(0.15);
  gStyle->SetPadRightMargin(0.05);

  TString canvas_name = Form("c_%s_%s",histdir.c_str(),histname.c_str());
  TCanvas* can = new TCanvas(canvas_name,canvas_name, 600, 600);
  can->cd();
  if (logplot) can->SetLogy();
  gPad->SetRightMargin(0.05);
  gPad->Modified();



  THStack* t = new THStack(Form("stack_%s_%s",histdir.c_str(),histname.c_str()),Form("stack_%s_%s",histdir.c_str(),histname.c_str()));
  TH1D* h_bgtot = 0;

  //TLegend* leg = new TLegend(0.55,0.6,0.85,0.92);
  TLegend* leg = new TLegend(0.55,0.6,0.85,0.90);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.032);

  // to make legend and find max yvalue
  vector<TH1D*> bg_hists;
  vector<string> bg_names;
  vector<TH1D*> sig_hists;
  vector<string> sig_names;

  const unsigned int n = samples.size();
  // background hists
  for( unsigned int i = 0 ; i < n ; ++i ) {
    if( TString(names.at(i)).Contains("sig")  ) continue;
    TString fullhistname = Form("%s/%s",histdir.c_str(),histname.c_str());
    if (histdir.size() == 0) fullhistname = TString(histname);
    TString newhistname = Form("%s_%s_%s",histname.c_str(),histdir.c_str(),names.at(i).c_str());
    TH1D* h_temp = (TH1D*) samples.at(i)->Get(fullhistname);
    if (h_temp == 0) continue;
    TH1D* h = (TH1D*) h_temp->Clone(newhistname);
    //    h->Sumw2();
    h->SetFillColor(getColor(names.at(i)));
    h->SetLineColor(getColor(names.at(i)));
    if (rebin > 1) h->Rebin(rebin);
    t->Add(h);
    if( h_bgtot==0 ) h_bgtot = (TH1D*) h->Clone(Form("%s_%s_bgtot",histname.c_str(),histdir.c_str()));
    else h_bgtot->Add(h);

    bg_hists.push_back(h);
    bg_names.push_back(names.at(i));
  }

  // loop backwards to add to legend
  for (int ibg = (int) bg_hists.size()-1; ibg >= 0; --ibg) {
    leg->AddEntry(bg_hists.at(ibg),getLegendName(bg_names.at(ibg)).c_str(),"f");
  }

  // signal hists - all samples must have "sig" in the name
  for( unsigned int i = 0 ; i < n ; ++i ){
    if( !TString(names.at(i)).Contains("sig") ) continue;
    TString fullhistname = Form("%s/%s",histdir.c_str(),histname.c_str());
    if (histdir.size() == 0) fullhistname = TString(histname);
    TString newhistname = Form("%s_%s_%s",histname.c_str(),histdir.c_str(),names.at(i).c_str());
    TH1D* h_temp = (TH1D*) samples.at(i)->Get(fullhistname);
    if (h_temp == 0) continue;
    // don't draw signal if the total yield in the plot is < 0.1 events
    if (h_temp->Integral(0,-1) < 0.1) continue;
    TH1D* h = (TH1D*) h_temp->Clone(newhistname);
    //    h->Sumw2();
    h->SetLineColor(getColor(names.at(i)));
    h->SetLineWidth(2);
    if (rebin > 1) h->Rebin(rebin);
    if (scalesig > 0.) h->Scale(scalesig);
    sig_hists.push_back(h);
    sig_names.push_back(names.at(i));
  }

  float ymax = 0;
  if(h_bgtot) ymax = h_bgtot->GetMaximum();
  // also check signals for max val
  for (unsigned int isig = 0; isig < sig_hists.size(); ++isig) {
    if (sig_hists.at(isig)->GetMaximum() > ymax) ymax = sig_hists.at(isig)->GetMaximum();
  }
  if( logplot ) ymax*=15;
  else          ymax*=1.5;
  float ymin = 0.1;

  TH2F* h_axes = new TH2F(Form("%s_%s_axes",histname.c_str(),histdir.c_str()),"",1000,xmin,xmax,1000,ymin,ymax);
  h_axes->GetXaxis()->SetTitle(xtitle.c_str());
  h_axes->GetXaxis()->SetLabelSize(0.04);
  h_axes->GetXaxis()->SetTitleSize(0.05);
  h_axes->GetYaxis()->SetTitle(ytitle.c_str());
  h_axes->GetYaxis()->SetLabelSize(0.04);
  h_axes->GetYaxis()->SetTitleOffset(1.5);
  h_axes->GetYaxis()->SetTitleSize(0.05);
  h_axes->Draw();

  t->Draw("hist same");

  // add signal hists
  for (unsigned int isig = 0; isig < sig_hists.size(); ++isig) {
    sig_hists.at(isig)->Draw("hist same");
    TString legend_name = getLegendName(sig_names.at(isig));
    if (scalesig > 0.) legend_name += Form(" x %.0f",scalesig);
    leg->AddEntry(sig_hists.at(isig),legend_name,"l");
  }

  TLatex label;
  label.SetNDC();
  label.SetTextSize(0.032);
  //TString ht_label = getHTPlotLabel(histdir);
  TString ht_label = getHTPlotLabel(samples.at(0), histdir);
  TString region_label = getJetBJetPlotLabel(samples.at(0), histdir);
  TString region_label_line2 = getMT2PlotLabel(samples.at(0), histdir);
  //label.DrawLatex(0.2,0.85,ht_label);
  label.DrawLatex(0.187,0.82,ht_label);
  // minMT plot always requires at least 2 bjets
  if ((histdir.find("srbase") != std::string::npos) && (histname.find("minMTBMet") != std::string::npos)) region_label = "#geq 2j, #geq 2b";
  //if (region_label.Length() > 0) label.DrawLatex(0.2,0.81,region_label);
  if (region_label.Length() > 0) label.DrawLatex(0.187,0.78,region_label);
  //if (region_label.Length() > 0) label.DrawLatex(0.187,0.78,"#geq 2j");//hack for srbase for now
  //if (region_label_line2.Length() > 0) label.DrawLatex(0.2,0.77,region_label_line2);
  if (region_label_line2.Length() > 0) label.DrawLatex(0.187,0.74,region_label_line2);

  leg->Draw();
  h_axes->Draw("axissame");

  CMS_lumi( can, iPeriod, iPos );

  gPad->Modified();

  if( printplot ) {
    can->Print(Form("plots/%s.pdf",canvas_name.Data()));
    //can->Print(Form("plots/%s.eps",canvas_name.Data()));
  }

  return can;
}

//_______________________________________________________________________________
void printTable( vector<TFile*> samples , vector<string> names , vector<string> dirs, int mt2bin = -1 ) {

  // read off yields from h_mt2bins hist in each topological region

  const unsigned int n = samples.size();
  const unsigned int ndirs = dirs.size();

  vector<double> bgtot(ndirs,0.);
  vector<double> bgerr(ndirs,0.);

  std::cout << "\\begin{table}[!ht]" << std::endl;
  std::cout << "\\scriptsize" << std::endl;
  std::cout << "\\centering" << std::endl;
  std::cout << "\\begin{tabular}{r";
  for (unsigned int idir=0; idir < ndirs; ++idir) std::cout << "|c";
  std::cout << "}" << std::endl;
  std::cout << "\\hline" << std::endl;

  cout << endl << "\\hline" << endl
    << "Sample";

  // header
  for (unsigned int idir = 0; idir < ndirs; ++idir) {
    //cout << " & " << getRegionName(dirs.at(idir));
    cout << " & " << getJetBJetTableLabel(samples.at(0), dirs.at(idir));
  }
  cout << " \\\\" << endl
    << "\\hline\\hline" << endl;

  // backgrounds first -- loop backwards
  for( int i = n-1 ; i >= 0 ; --i ){
    if( TString(names.at(i)).Contains("sig")  ) continue;
    cout << getTableName(names.at(i));
    for ( unsigned int idir = 0; idir < ndirs; ++idir ) {
      TString fullhistname = Form("%s/h_mt2bins",dirs.at(idir).c_str());
      TH1D* h = (TH1D*) samples.at(i)->Get(fullhistname);
      double yield = 0.;
      double err = 0.;
      if (h) {
        // use all bins
        if (mt2bin < 0) {
          yield = h->IntegralAndError(0,-1,err);
          bgtot.at(idir) += yield;
          bgerr.at(idir) = sqrt(pow(bgerr.at(idir),2) + pow(err,2));
        }
        // last bin: include overflow
        else if (mt2bin == h->GetXaxis()->GetNbins()) {
          yield = h->IntegralAndError(mt2bin,-1,err);
          bgtot.at(idir) += yield;
          bgerr.at(idir) = sqrt(pow(bgerr.at(idir),2) + pow(err,2));
        }
        // single bin, not last bin
        else {
          yield = h->GetBinContent(mt2bin);
          err = h->GetBinError(mt2bin);
          bgtot.at(idir) += yield;
          bgerr.at(idir) = sqrt(pow(bgerr.at(idir),2) + pow(err,2));
        }
      }
      if (yield > 10.) {
        //  	cout << "  &  " << Form("%.0f $\\pm$ %.0f",yield,err);
        cout << "  &  " << Form("%.1f $\\pm$ %.1f",yield,err);
      } else {
        //  	cout << "  &  " << Form("%.1f $\\pm$ %.1f",yield,err);
        cout << "  &  " << Form("%.2f $\\pm$ %.2f",yield,err);
      }
    }
    cout << " \\\\" << endl;
  } // loop over samples

  // print bg totals
  cout << "\\hline" << endl;
  cout << "Total SM";
  for ( unsigned int idir = 0; idir < ndirs; ++idir ) {
    double yield = bgtot.at(idir);
    double err = bgerr.at(idir);
    if (yield > 10.) {
      //  	cout << "  &  " << Form("%.0f $\\pm$ %.0f",yield,err);
      cout << "  &  " << Form("%.1f $\\pm$ %.1f",yield,err);
    } else {
      //  	cout << "  &  " << Form("%.1f $\\pm$ %.1f",yield,err);
      cout << "  &  " << Form("%.2f $\\pm$ %.2f",yield,err);
    }
  }
  cout << " \\\\" << endl;
  cout << "\\hline" << endl;

  for( unsigned int i = 0 ; i < n ; i++ ){
    if( !TString(names.at(i)).Contains("sig") ) continue;
    cout << getTableName(names.at(i));
    for ( unsigned int idir = 0; idir < ndirs; ++idir ) {
      TString fullhistname = Form("%s/h_mt2bins",dirs.at(idir).c_str());
      TH1D* h = (TH1D*) samples.at(i)->Get(fullhistname);
      double yield = 0.;
      double err = 0.;
      if (h) {
        // use all bins
        if (mt2bin < 0) {
          yield = h->IntegralAndError(0,-1,err);
        }
        // last bin: include overflow
        else if (mt2bin == h->GetXaxis()->GetNbins()) {
          yield = h->IntegralAndError(mt2bin,-1,err);
        }
        // single bin, not last bin
        else {
          yield = h->GetBinContent(mt2bin);
          err = h->GetBinError(mt2bin);
        }
      }
      if (yield > 10.) {
        //  	cout << "  &  " << Form("%.0f $\\pm$ %.0f",yield,err);
        cout << "  &  " << Form("%.1f $\\pm$ %.1f",yield,err);
      } else {
        //  	cout << "  &  " << Form("%.1f $\\pm$ %.1f",yield,err);
        cout << "  &  " << Form("%.2f $\\pm$ %.2f",yield,err);
      }
    }
    cout << " \\\\" << endl;
  } // loop over samples

  std::cout << "\\end{tabular}" << std::endl;
  std::cout << "\\caption{}" << std::endl;
  std::cout << "\\end{table}" << std::endl;

  cout << endl;
  return;
}

//_______________________________________________________________________________
void printDetailedTable( vector<TFile*> samples , vector<string> names , string dir) {

  // read off yields from h_mt2bins hist in each topological region

  const unsigned int n = samples.size();


  TString nbinshistname = Form("%s/h_n_mt2bins",dir.c_str());
  TH1D* h_n_mt2bins = (TH1D*) samples.at(0)->Get(nbinshistname);
  int n_mt2bins;
  if(h_n_mt2bins){
    n_mt2bins = h_n_mt2bins->GetBinContent(1);
  }
  else{
    std::cout << "Couldn't get number of mt2 bins" << std::endl;
    return;
  }

  vector<double> bgtot(n_mt2bins,0.);
  vector<double> bgerr(n_mt2bins,0.);

  TString binshistname = Form("%s/h_mt2bins",dir.c_str());
  TH1D* h_mt2bins(0);
  for(unsigned int i=0; i<samples.size(); i++){//need to find a sample that has this hist filled
    h_mt2bins = (TH1D*) samples.at(i)->Get(binshistname);
    if(h_mt2bins) break;
  }
  
  if(!h_mt2bins) std::cout << "Couldn't get the mt2 binning" << std::endl;

  std::cout << "\\begin{table}[htbp]" << std::endl;
  std::cout << "\\caption{Background and signal yields in bins of \\mttwo for ";
  std::cout << getHTTableLabel(samples.at(0), dir) << ", ";
  std::cout << getJetBJetTableLabel(samples.at(0), dir) << ". ";
  std::cout << "The yields are normalized to \\lumival.}" << std::endl;
  std::cout << "\\scriptsize" << std::endl;
  std::cout << "\\centering" << std::endl;
  std::cout << "\\makebox[\\textwidth][c]{" << std::endl;
  std::cout << "\\begin{tabular}{r";
  for (int ibin=0; ibin < n_mt2bins; ++ibin) std::cout << "|c";
  std::cout << "}" << std::endl;
  std::cout << "\\hline" << std::endl;
  std::cout << "\\multicolumn{" << n_mt2bins+1 << "}{c}{";
  std::cout << getHTTableLabel(samples.at(0), dir) << ", ";
  std::cout << getMETTableLabel(samples.at(0), dir) << ", ";
  std::cout << getJetBJetTableLabel(samples.at(0), dir);
  std::cout << "} \\\\" << std::endl;
  std::cout << "\\hline" << std::endl;
  std::cout << "\\hline" << endl
    << "Sample";

  // header
  for (int ibin = 1; ibin < n_mt2bins; ++ibin) {
    cout << " & " << h_mt2bins->GetXaxis()->GetBinLowEdge(ibin) << " $<$ \\mttwo $<$ " << h_mt2bins->GetXaxis()->GetBinLowEdge(ibin+1) << " GeV";
  }
  cout << " & \\mttwo $>$ " << h_mt2bins->GetXaxis()->GetBinLowEdge(n_mt2bins) << " GeV";
  cout << " \\\\" << endl
    << "\\hline\\hline" << endl;

  // backgrounds first -- loop backwards
  for( int isamp = n-1 ; isamp >= 0 ; --isamp ){
    if( TString(names.at(isamp)).Contains("sig")  ) continue;
    cout << getTableName(names.at(isamp));
    for (int ibin = 1; ibin <= n_mt2bins; ++ibin) {
      TString fullhistname = Form("%s/h_mt2bins",dir.c_str());
      TH1D* h = (TH1D*) samples.at(isamp)->Get(fullhistname);
      double yield = 0.;
      double err = 0.;
      if (h) {
        // not last bin
        if(ibin != n_mt2bins) {
          yield = h->GetBinContent(ibin);
          err = h->GetBinError(ibin);
          bgtot.at(ibin-1) += yield;
          bgerr.at(ibin-1) = sqrt(pow(bgerr.at(ibin-1),2) + pow(err,2));
        }
        // last bin: include overflow
        else if (ibin == h->GetXaxis()->GetNbins()) {
          yield = h->IntegralAndError(ibin,-1,err);
          bgtot.at(ibin-1) += yield;
          bgerr.at(ibin-1) = sqrt(pow(bgerr.at(ibin-1),2) + pow(err,2));
        }
        else {
          std::cout << "Shouldn't get here" << std::endl;
          return;
        }
      }
      if (yield > 10.) {
        //  	cout << "  &  " << Form("%.0f $\\pm$ %.0f",yield,err);
        cout << "  &  " << Form("%.1f $\\pm$ %.1f",yield,err);
      } else {
        //  	cout << "  &  " << Form("%.1f $\\pm$ %.1f",yield,err);
        cout << "  &  " << Form("%.2f $\\pm$ %.2f",yield,err);
      }
    }
    cout << " \\\\" << endl;
  } // loop over samples

  // print bg totals
  cout << "\\hline" << endl;
  cout << "Total SM";
  for ( int ibin = 0; ibin < n_mt2bins; ++ibin ) {
    double yield = bgtot.at(ibin);
    double err = bgerr.at(ibin);
    if (yield > 10.) {
      //  	cout << "  &  " << Form("%.0f $\\pm$ %.0f",yield,err);
      cout << "  &  " << Form("%.1f $\\pm$ %.1f",yield,err);
    } else {
      //  	cout << "  &  " << Form("%.1f $\\pm$ %.1f",yield,err);
      cout << "  &  " << Form("%.2f $\\pm$ %.2f",yield,err);
    }
  }
  cout << " \\\\" << endl;
  cout << "\\hline" << endl;

  for( unsigned int jsamp = 0 ; jsamp < n ; jsamp++ ){
    if( !TString(names.at(jsamp)).Contains("sig") ) continue;
    cout << getTableName(names.at(jsamp));
    for (int ibin = 1; ibin <= n_mt2bins; ++ibin) {
      TString fullhistname = Form("%s/h_mt2bins",dir.c_str());
      TH1D* h = (TH1D*) samples.at(jsamp)->Get(fullhistname);
      double yield = 0.;
      double err = 0.;
      if (h) {
        // not last bin
        if(ibin != n_mt2bins) {
          yield = h->GetBinContent(ibin);
          err = h->GetBinError(ibin);
        }
        // last bin: include overflow
        else if (ibin == h->GetXaxis()->GetNbins()) {
          yield = h->IntegralAndError(ibin,-1,err);
        }
        else {
          std::cout << "Shouldn't get here" << std::endl;
          return;
        }
      }
      if (yield > 10.) {
        //  	cout << "  &  " << Form("%.0f $\\pm$ %.0f",yield,err);
        cout << "  &  " << Form("%.1f $\\pm$ %.1f",yield,err);
      } else {
        //  	cout << "  &  " << Form("%.1f $\\pm$ %.1f",yield,err);
        cout << "  &  " << Form("%.2f $\\pm$ %.2f",yield,err);
      }
    }
    cout << " \\\\" << endl;
  } // loop over samples


  std::cout << "\\hline" << std::endl;
  std::cout << "\\end{tabular}}" << std::endl;
  std::cout << "\\end{table}" << std::endl;
  //std::cout << "\\pagebreak" << std::endl;
  std::cout << std::endl;

  return;
}


//_______________________________________________________________________________
void plotMaker(){

  //  gROOT->LoadMacro("CMS_lumi.C");
  cmsText = "CMS Simulation";
  cmsTextSize = 0.5;
  lumiTextSize = 0.4;
  writeExtraText = false;
  lumi_13TeV = "4 fb^{-1}";

  //string input_dir = "/home/olivito/cms3/MT2Analysis/MT2looper/output/V00-00-12_test/";
  string input_dir = "/home/users/jgran/temp/update/MT2Analysis/MT2looper/output/V00-00-12/";

  // ----------------------------------------
  //  samples definition
  // ----------------------------------------

  // get input files

  TFile* f_ttbar = new TFile(Form("%s/ttall_msdecays.root",input_dir.c_str()));
  TFile* f_zinv = new TFile(Form("%s/zinv_ht.root",input_dir.c_str()));
  TFile* f_gjet = new TFile(Form("%s/gjet_ht.root",input_dir.c_str()));
  TFile* f_wjets = new TFile(Form("%s/wjets_ht.root",input_dir.c_str()));
  TFile* f_qcd = new TFile(Form("%s/qcd_pt.root",input_dir.c_str()));
  TFile* f_tth = new TFile(Form("%s/tth.root",input_dir.c_str()));
  TFile* f_ttw = new TFile(Form("%s/ttwjets.root",input_dir.c_str()));
  TFile* f_ttz = new TFile(Form("%s/ttzjets.root",input_dir.c_str()));
  TFile* f_singletop = new TFile(Form("%s/singletop.root",input_dir.c_str()));
  TFile* f_top = new TFile(Form("%s/top.root",input_dir.c_str())); //hadd'ing of ttbar, ttw, ttz, tth, singletop

  TFile* f_T1tttt_1500_100 = new TFile(Form("%s/T1tttt_1500_100.root",input_dir.c_str()));
  TFile* f_T1tttt_1200_800 = new TFile(Form("%s/T1tttt_1200_800.root",input_dir.c_str()));
  TFile* f_T1bbbb_1500_100 = new TFile(Form("%s/T1bbbb_1500_100.root",input_dir.c_str()));
  TFile* f_T1bbbb_1000_900 = new TFile(Form("%s/T1bbbb_1000_900.root",input_dir.c_str()));
  TFile* f_T1qqqq_1400_100 = new TFile(Form("%s/T1qqqq_1400_100.root",input_dir.c_str()));
  TFile* f_T1qqqq_1000_800 = new TFile(Form("%s/T1qqqq_1000_800.root",input_dir.c_str()));

  TFile* f_T2tt_425_325 = new TFile(Form("%s/T2tt_425_325.root",input_dir.c_str()));
  TFile* f_T2tt_500_325 = new TFile(Form("%s/T2tt_500_325.root",input_dir.c_str()));
  TFile* f_T2tt_650_325 = new TFile(Form("%s/T2tt_650_325.root",input_dir.c_str()));
  TFile* f_T2tt_850_100 = new TFile(Form("%s/T2tt_850_100.root",input_dir.c_str()));

  TFile* f_T2bb_900_100 = new TFile(Form("%s/T2bb_900_100.root",input_dir.c_str()));
  TFile* f_T2bb_600_580 = new TFile(Form("%s/T2bb_600_580.root",input_dir.c_str()));
  TFile* f_T2qq_1200_100 = new TFile(Form("%s/T2qq_1200_100.root",input_dir.c_str()));
  TFile* f_T2qq_600_550 = new TFile(Form("%s/T2qq_600_550.root",input_dir.c_str()));

  //TFile* f_zinv_ht100to200 = new TFile(Form("%s/zinv_ht100to200.root",input_dir.c_str()));
  //TFile* f_zinv_ht200to400 = new TFile(Form("%s/zinv_ht200to400.root",input_dir.c_str()));
  //TFile* f_zinv_ht400to600 = new TFile(Form("%s/zinv_ht400to600.root",input_dir.c_str()));
  //TFile* f_zinv_ht600toInf = new TFile(Form("%s/zinv_ht600toInf.root",input_dir.c_str()));

  //TFile* f_wjets_ht100to200 = new TFile(Form("%s/wjets_ht100to200.root",input_dir.c_str()));
  //TFile* f_wjets_ht200to400 = new TFile(Form("%s/wjets_ht200to400.root",input_dir.c_str()));
  //TFile* f_wjets_ht400to600 = new TFile(Form("%s/wjets_ht400to600.root",input_dir.c_str()));
  //TFile* f_wjets_ht600toInf = new TFile(Form("%s/wjets_ht600toInf.root",input_dir.c_str()));

  vector<TFile*> samples;
  vector<string>  names;

  samples.push_back(f_qcd); names.push_back("qcd");
  samples.push_back(f_wjets); names.push_back("wjets");
  samples.push_back(f_zinv);  names.push_back("zinv");
  //samples.push_back(f_gjet);  names.push_back("gjet");
  //samples.push_back(f_tth); names.push_back("tth");
  //samples.push_back(f_ttw); names.push_back("ttw");
  //samples.push_back(f_ttz); names.push_back("ttz");
  //samples.push_back(f_singletop); names.push_back("singletop");
  //samples.push_back(f_ttbar); names.push_back("ttbar");
  //samples.push_back(f_wjets); names.push_back("wjets");
  samples.push_back(f_top); names.push_back("top");

  samples.push_back(f_T1tttt_1500_100); names.push_back("sig_T1tttt_1500_100");
  samples.push_back(f_T1tttt_1200_800); names.push_back("sig_T1tttt_1200_800");
  samples.push_back(f_T1bbbb_1500_100); names.push_back("sig_T1bbbb_1500_100");
  samples.push_back(f_T1bbbb_1000_900); names.push_back("sig_T1bbbb_1000_900");
  samples.push_back(f_T1qqqq_1400_100); names.push_back("sig_T1qqqq_1400_100");
  samples.push_back(f_T1qqqq_1000_800); names.push_back("sig_T1qqqq_1000_800");

  samples.push_back(f_T2tt_850_100); names.push_back("sig_T2tt_850_100");
  samples.push_back(f_T2tt_650_325); names.push_back("sig_T2tt_650_325");
  samples.push_back(f_T2tt_500_325); names.push_back("sig_T2tt_500_325");
  samples.push_back(f_T2tt_425_325); names.push_back("sig_T2tt_425_325");

  samples.push_back(f_T2bb_900_100); names.push_back("sig_T2bb_900_100");
  samples.push_back(f_T2bb_600_580); names.push_back("sig_T2bb_600_580");
  samples.push_back(f_T2qq_1200_100); names.push_back("sig_T2qq_1200_100");
  samples.push_back(f_T2qq_600_550); names.push_back("sig_T2qq_600_550");

  // ----------------------------------------
  //  plots definitions
  // ----------------------------------------

  float scalesig = -1.;
  //float scalesig = 50.;
  bool printplots = false;
  //bool printplots = true;

  if(printplots){
    TIter it(f_ttbar->GetListOfKeys());
    TKey* k;
    std::string cr_skip = "cr";
    std::string sr_skip = "sr";
    while ((k = (TKey *)it())) {
      if (strncmp (k->GetTitle(), cr_skip.c_str(), cr_skip.length()) == 0) continue; //skip control regions
      //if (strncmp (k->GetTitle(), sr_skip.c_str(), sr_skip.length()) == 0) continue; //skip signal regions and srbase
      std::string dir_name = k->GetTitle();
      if(dir_name == "") continue;
      if(dir_name != "srbase") continue; //to do only srbase
      //if(dir_name != "sr1H") continue; //for testing
      makePlot( samples , names , dir_name , "h_ht"  , "H_{T} [GeV]" , "Events / 25 GeV" , 0 , 2000 , 1 , true, printplots, scalesig );
      makePlot( samples , names , dir_name , "h_mt2" , "M_{T2} [GeV]" , "Events / 10 GeV" , 0 , 1000 , 1 , true, printplots, scalesig );
      makePlot( samples , names , dir_name , "h_met"  , "E_{T}^{miss} [GeV]" , "Events / 10 GeV" , 0 , 800 , 1 , true, printplots );
      makePlot( samples , names , dir_name , "h_nlepveto" , "N(leptons)" , "Events" , 0 , 10 , 1 , false, printplots );
      makePlot( samples , names , dir_name , "h_nJet40" , "N(jets)" , "Events" , 0 , 15 , 1 , false, printplots, scalesig );
      makePlot( samples , names , dir_name , "h_nBJet20" , "N(b jets)" , "Events" , 0 , 6 , 1 , false, printplots, scalesig );
      makePlot( samples , names , dir_name , "h_minMTBMet"  , "min M_{T}(b,MET) [GeV]" , "Events / 10 GeV" , 0 , 800 , 1 , true, printplots );
      makePlot( samples , names , dir_name , "h_mt2bins" , "M_{T2} [GeV]" , "Events / Bin" , 200 , 1500 , 1 , true, printplots, scalesig );
    }
  }

  std::cout << "\\documentclass[landscape, 10pt]{article}" << std::endl;
  std::cout << "\\usepackage{amsmath}" << std::endl;
  std::cout << "\\usepackage{amssymb}" << std::endl;
  std::cout << "\\usepackage{graphicx}" << std::endl;
  std::cout << "\\usepackage[left=.1in,top=1in,right=.1in,bottom=.1in,nohead]{geometry}" << std::endl;
  std::cout << "\\begin{document}" << std::endl;

  vector<string> dirs;
  dirs.push_back("sr1L");
  // dirs.push_back("sr2L");
  // dirs.push_back("sr3L");
  // dirs.push_back("sr4L");
  // dirs.push_back("sr5L");
  // dirs.push_back("sr6L");
  // dirs.push_back("sr7L");
  // dirs.push_back("sr8L");
  // dirs.push_back("sr9L");
  // dirs.push_back("sr10L");
  // dirs.push_back("sr11L");
  // dirs.push_back("sr1M");
  // dirs.push_back("sr2M");
  // dirs.push_back("sr3M");
  // dirs.push_back("sr4M");
  // dirs.push_back("sr5M");
  // dirs.push_back("sr6M");
  // dirs.push_back("sr7M");
  // dirs.push_back("sr8M");
  // dirs.push_back("sr9M");
  // dirs.push_back("sr10M");
  // dirs.push_back("sr11M");
  // dirs.push_back("sr1H");
  // dirs.push_back("sr2H");
  // dirs.push_back("sr3H");
  // dirs.push_back("sr4H");
  // dirs.push_back("sr5H");
  // dirs.push_back("sr6H");
  // dirs.push_back("sr7H");
  // dirs.push_back("sr8H");
  // dirs.push_back("sr9H");
  // dirs.push_back("sr10H");
  // dirs.push_back("sr11H");
  // dirs.push_back("sr1UH");
  // dirs.push_back("sr2UH");
  // dirs.push_back("sr3UH");
  // dirs.push_back("sr4UH");
  // dirs.push_back("sr5UH");
  // dirs.push_back("sr6UH");
  // dirs.push_back("sr7UH");
  // dirs.push_back("sr8UH");
  // dirs.push_back("sr9UH");
  // dirs.push_back("sr10UH");
  // dirs.push_back("sr11UH");

  for(unsigned int i=0; i<dirs.size(); i++){
    printDetailedTable(samples, names, dirs.at(i));
    if(i % 2 != 0) std::cout << "\\pagebreak" << std::endl; //two tables per page
  }

  /*
     vector<string> dirsH;
     dirsH.push_back("sr1L");
     dirsH.push_back("sr2L");
     dirsH.push_back("sr3L");
     dirsH.push_back("sr4L");
     dirsH.push_back("sr5L");
     dirsH.push_back("sr6L");
     printTable(samples, names, dirsH);
     dirsH.clear();

     dirsH.push_back("sr7L");
     dirsH.push_back("sr8L");
     dirsH.push_back("sr9L");
     dirsH.push_back("sr10L");
     dirsH.push_back("sr11L");
     printTable(samples, names, dirsH);
     dirsH.clear();

     dirsH.push_back("sr1M");
     dirsH.push_back("sr2M");
     dirsH.push_back("sr3M");
     dirsH.push_back("sr4M");
     dirsH.push_back("sr5M");
     dirsH.push_back("sr6M");
     printTable(samples, names, dirsH);
     dirsH.clear();

     dirsH.push_back("sr7M");
     dirsH.push_back("sr8M");
     dirsH.push_back("sr9M");
     dirsH.push_back("sr10M");
     dirsH.push_back("sr11M");
     printTable(samples, names, dirsH);
     dirsH.clear();

     dirsH.push_back("sr1H");
     dirsH.push_back("sr2H");
     dirsH.push_back("sr3H");
     dirsH.push_back("sr4H");
     dirsH.push_back("sr5H");
     dirsH.push_back("sr6H");
     printTable(samples, names, dirsH);
     dirsH.clear();

     dirsH.push_back("sr7H");
     dirsH.push_back("sr8H");
     dirsH.push_back("sr9H");
     dirsH.push_back("sr10H");
     dirsH.push_back("sr11H");
     printTable(samples, names, dirsH);
     dirsH.clear();

     dirsH.push_back("sr1UH");
     dirsH.push_back("sr2UH");
     dirsH.push_back("sr3UH");
     dirsH.push_back("sr4UH");
     dirsH.push_back("sr5UH");
     dirsH.push_back("sr6UH");
     printTable(samples, names, dirsH);
     dirsH.clear();

     dirsH.push_back("sr7UH");
     dirsH.push_back("sr8UH");
     dirsH.push_back("sr9UH");
     dirsH.push_back("sr10UH");
     dirsH.push_back("sr11UH");
     printTable(samples, names, dirsH);
     dirsH.clear();
     */

  std::cout << "\\end{document}" << std::endl;

}

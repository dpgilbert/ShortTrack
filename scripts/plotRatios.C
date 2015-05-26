#include <iostream>
#include <utility>
#include <vector>
#include <string>
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
const int iPos = 33; 

//_______________________________________________________________________________
TCanvas* makeRatio( TFile* f , const string& dir, const string& cent_name, const vector<string>& var_names, const string& xtitle , const string& ytitle , bool logplot = true, bool printplot = false, bool fixerrors = false, bool fixnorm = false ) {

  TString canvas_name = Form("c_%s_%s_vars",dir.c_str(),cent_name.c_str());
  TCanvas* can = new TCanvas(canvas_name,canvas_name, 600, 600);
  can->cd();
  gPad->SetRightMargin(0.05);
  gPad->Modified();

  string histname = dir + "/" + cent_name;

  TH1D* hcent = (TH1D*) f->Get(histname.c_str());
  if (!hcent) {
    cout << "WARNING: didn't find hist with name: " << histname << endl;
    return can;
  }

  vector<TH1D*> hvars;

  for (unsigned int ivar=0; ivar < var_names.size(); ++ivar) {
    string histname_var = histname + "_" + var_names.at(ivar);
    TH1D* hvar = (TH1D*) f->Get(histname_var.c_str());
    if (!hvar) cout << "WARNING: didn't find hist with name: " << histname_var << endl;
    hvars.push_back(hvar);
  }

  gStyle->SetOptStat("");
  gStyle->SetCanvasColor(0);
  gStyle->SetPadGridX(0);
  gStyle->SetPadGridY(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetFrameBorderMode(0);

  // //these 4 lines shift plot to make room for axis labels
  // gStyle->SetPadTopMargin(0.08);
  // gStyle->SetPadBottomMargin(0.12);
  // gStyle->SetPadLeftMargin(0.15);
  // gStyle->SetPadRightMargin(0.05);

  // master pad
  TPad* fullpad = new TPad("fullpad","fullpad",0,0,1,1);
  fullpad->Draw();
  fullpad->cd();

  // main plot pad, for ratio on bottom
  TPad* plotpad = new TPad("plotpad","plotpad",0,0.2,1,0.99);
  plotpad->SetTopMargin(0.05);
  plotpad->SetRightMargin(0.05);
  plotpad->SetBottomMargin(0.05);
  plotpad->Draw();
  plotpad->cd();
  if( logplot ) plotpad->SetLogy();

  //TLegend* leg = new TLegend(0.55,0.6,0.85,0.92);
  TLegend* leg = new TLegend(0.63,0.5,0.88,0.71);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.050);

  std::vector<int> colors;
  colors.push_back(kBlue);
  colors.push_back(kRed);
  colors.push_back(kGreen+2);
  colors.push_back(kMagenta);

  vector<TH1D*> hratios;

  hcent->Sumw2();
  hcent->Draw("histe");
  // remove labels - put them on the ratio hist
  hcent->GetXaxis()->SetTitleSize(0.0);
  hcent->GetXaxis()->SetLabelSize(0.0);

  double cent_norm = hcent->Integral(0,-1);

  leg->AddEntry(hcent,"Nominal","pe");

  for (unsigned int ivar=0; ivar < hvars.size(); ++ivar) {
    hvars.at(ivar)->Sumw2();
    hvars.at(ivar)->SetLineColor(colors.at(ivar));
    hvars.at(ivar)->SetMarkerColor(colors.at(ivar));
    if (fixnorm) {
      double integral = hvars.at(ivar)->Integral(0,-1);
      hvars.at(ivar)->Scale(cent_norm/integral);
    }
    hvars.at(ivar)->Draw("histe same");

    TH1D* hratio = (TH1D*) hvars.at(ivar)->Clone(Form("%s_ratio",hvars.at(ivar)->GetName()));
    hratio->Divide(hcent);
    // fix errors: loop through bins and set relative error to that of hcent
    if (fixerrors) {
      for (int ibin=1; ibin <= hcent->GetNbinsX(); ++ibin) {
	double relerr = hcent->GetBinError(ibin)/hcent->GetBinContent(ibin);
	hratio->SetBinError(ibin,hratio->GetBinContent(ibin)*relerr);
      } // loop on bins
    }
    hratios.push_back(hratio);

    leg->AddEntry(hratio,var_names.at(ivar).c_str(),"pe");
  } // loop on variations

  leg->Draw("same");

  TLatex label;
  label.SetNDC();
  label.SetTextSize(0.04);
  TString ht_label = getHTPlotLabel(f, dir);
  TString region_label = getJetBJetPlotLabel(f, dir);
  TString region_label_line2 = getMT2PlotLabel(f, dir);
  label.DrawLatex(0.65,0.82,ht_label);
  if (region_label.Length() > 0) label.DrawLatex(0.65,0.78,region_label);
  if (region_label_line2.Length() > 0) label.DrawLatex(0.65,0.74,region_label_line2);

  // need to fix the size of this somehow..
  CMS_lumi( plotpad, iPeriod, iPos );

  // draw ratio pad
  fullpad->cd();
  TPad* ratiopad = new TPad("ratiopad","ratiopad",0.,0.,1,0.23);
  ratiopad->SetLeftMargin(0.16);
  ratiopad->SetRightMargin(0.05);
  ratiopad->SetTopMargin(0.08);
  ratiopad->SetBottomMargin(0.44);
  ratiopad->SetGridy();
  ratiopad->Draw();
  ratiopad->cd();
  //  gPad->SetGridy();

  for (unsigned int ivar=0; ivar < hvars.size(); ++ivar) {
    if (ivar == 0) {
      // draw axis only
      hratios.at(ivar)->GetYaxis()->SetTitleOffset(0.3);
      hratios.at(ivar)->GetYaxis()->SetTitleSize(0.18);
      hratios.at(ivar)->GetYaxis()->SetNdivisions(5);
      hratios.at(ivar)->GetYaxis()->SetLabelSize(0.15);
      hratios.at(ivar)->GetYaxis()->SetRangeUser(0.5,1.5);
      //hratios.at(ivar)->GetYaxis()->SetRangeUser(0.001,2.0);
      hratios.at(ivar)->GetYaxis()->SetTitle("Var/Central");
      hratios.at(ivar)->GetXaxis()->SetTitle(hcent->GetXaxis()->GetTitle());
      hratios.at(ivar)->GetXaxis()->SetTitleSize(0.17);
      hratios.at(ivar)->GetXaxis()->SetLabelSize(0.17);
      hratios.at(ivar)->GetXaxis()->SetTitleOffset(1.0);
      hratios.at(ivar)->GetXaxis()->SetTickLength(0.07);
      hratios.at(ivar)->Draw("p");
    }

    TGraphErrors* gratio = new TGraphErrors(hratios.at(ivar));
    gratio->SetName(Form("%s_graph",hratios.at(ivar)->GetName()));
    for (int ibin=0; ibin < hratios.at(ivar)->GetNbinsX(); ++ibin) {
      gratio->SetPointError(ibin, hratios.at(ivar)->GetBinWidth(ibin+1)/2., hratios.at(ivar)->GetBinError(ibin+1));
    }
    gratio->SetLineColor(colors.at(ivar));
    gratio->SetMarkerColor(colors.at(ivar));
    gratio->SetMarkerStyle(20);
    gratio->Draw("p0same");

  }

  return can;
}

//_______________________________________________________________________________
void plotRatios() {

  //  gROOT->LoadMacro("CMS_lumi.C");
  cmsText = "CMS Simulation";
  cmsTextSize = 0.5;
  lumiTextSize = 0.4;
  writeExtraText = false;
  lumi_13TeV = "4 fb^{-1}";

  string input_dir = "/home/olivito/cms3/MT2Analysis/MT2looper/output/V00-00-12_weights_test/";

  //  TFile* f_top = new TFile(Form("%s/top.root",input_dir.c_str())); //hadd'ing of ttbar, ttw, ttz, tth, singletop
  TFile* f_top = new TFile(Form("%s/ttall_msdecays.root",input_dir.c_str())); 

  string cent_name = "h_mt2bins";
  vector<string> var_names;
  // var_names.push_back("isr_UP");
  // var_names.push_back("isr_DN");
  // var_names.push_back("btagsf_UP");
  // var_names.push_back("btagsf_DN");
  var_names.push_back("scales_UP");
  var_names.push_back("scales_DN");

  bool printplots = false;

  vector<string> dirs;
  dirs.push_back("sr1L");
  dirs.push_back("sr2L");
  dirs.push_back("sr3L");
  dirs.push_back("sr4L");
  dirs.push_back("sr5L");
  dirs.push_back("sr6L");
  dirs.push_back("sr7L");
  dirs.push_back("sr8L");
  dirs.push_back("sr9L");
  dirs.push_back("sr10L");
  dirs.push_back("sr11L");
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

  //  makeRatio(f_top,"sr5L",cent_name,var_names,"M_{T2} [GeV]", "Events" , true, printplots, true, true);

  for (unsigned int idir=0; idir < dirs.size(); ++idir) {
    makeRatio(f_top,dirs.at(idir),cent_name,var_names,"M_{T2} [GeV]", "Events" , true, printplots, true, true);
  }

}

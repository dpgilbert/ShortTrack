#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

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
const int iPos = 3;

//______________________________________________________________________________
// returns the error on C = A*B (or C = A/B)
float err_mult(float A, float B, float errA, float errB, float C) {
  return sqrt(C*C*(pow(errA/A,2) + pow(errB/B,2)));
}

void addOverflow(TH1* h) {
  Double_t err = 0;
  float lastBinPlusOverflow = h->IntegralAndError(h->GetXaxis()->GetNbins(), -1, err);
  h->SetBinContent(h->GetXaxis()->GetNbins(), lastBinPlusOverflow);
  h->SetBinError(h->GetXaxis()->GetNbins(), err);
  return;
}

//_______________________________________________________________________________
TCanvas* makePlot( const vector<TFile*>& samples , const vector<string>& names , const string& histdir , const string& histname , const string& xtitle , const string& ytitle , float xmin , float xmax , int rebin = 1 , bool logplot = true, bool printplot = false, float scalesig = -1., bool doRatio = false, bool scaleBGtoData = false ) {

  
  cout << "-- plotting histdir: " << histdir << ", histname: " << histname << endl;

  gStyle->SetOptStat("");
  gStyle->SetCanvasColor(0);
  gStyle->SetPadGridX(0);
  gStyle->SetPadGridY(0);
  gStyle->SetPadTickX(1);
  gStyle->SetPadTickY(1);
  gStyle->SetFrameBorderMode(0);

  // if ratio was requested, check if data is present.  If not, turn ratio off again
  const unsigned int n = samples.size();
  if (doRatio) {
    bool foundData = false;
    for( unsigned int i = 0 ; i < n ; ++i ) {
      if( TString(names.at(i)).Contains("data")  ) {
	foundData = true;
	break;
      }
    }
    if (!foundData) {
      cout << "ratio requested but no data hist found.  Not plotting ratio" << endl;
      doRatio = false;
    }
  } // if doRatio

  if (!doRatio) {
    //these 4 lines shift plot to make room for axis labels
    gStyle->SetPadTopMargin(0.08);
    gStyle->SetPadBottomMargin(0.12);
    gStyle->SetPadLeftMargin(0.15);
    gStyle->SetPadRightMargin(0.05);
  }
  
  TString canvas_name = Form("c_%s_%s",histdir.c_str(),histname.c_str());
  if (logplot) canvas_name += "_log";
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
    plotpad->SetRightMargin(0.05);
    plotpad->SetBottomMargin(0.12);
    plotpad->Draw();
    plotpad->cd();
    if( logplot ) plotpad->SetLogy();
  }
  
  TLegend* leg = new TLegend(0.55,0.57,0.85,0.87);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.032);
  if (doRatio) leg->SetTextSize(0.06);

  TH1D* data_hist(0);
  string data_name;
  for( unsigned int i = 0 ; i < n ; ++i ) {
    if( !TString(names.at(i)).Contains("data")  ) continue;
    TString fullhistname = Form("%s/%s",histdir.c_str(),histname.c_str());
    if (histdir.size() == 0) fullhistname = TString(histname);
    TString newhistname = Form("%s_%s_%s",histname.c_str(),histdir.c_str(),names.at(i).c_str());
    TH1D* h_temp = (TH1D*) samples.at(i)->Get(fullhistname);
    if (h_temp == 0) continue;
    data_hist = (TH1D*) h_temp->Clone(newhistname);
    data_name = names.at(i);
    data_hist->SetLineColor(kBlack);
    data_hist->SetMarkerColor(kBlack);
    data_hist->SetMarkerStyle(20);
    addOverflow(data_hist); // Add Overflow
    if (rebin > 1) data_hist->Rebin(rebin);

    // fake data -> set error bars to correspond to data stats
    if (TString(data_name).Contains("fakedata")) {
      for (int ibin = 0; ibin <= data_hist->GetNbinsX(); ++ibin) {
	data_hist->SetBinError( ibin, sqrt(data_hist->GetBinContent(ibin)) );
      }
    } // if fakedata
    
    // expect to only find 1 data hist
    break;
  }

  if (data_hist) leg->AddEntry(data_hist,getLegendName(data_name).c_str(),"pe1");
  
  THStack* t = new THStack(Form("stack_%s_%s",histdir.c_str(),histname.c_str()),Form("stack_%s_%s",histdir.c_str(),histname.c_str()));
  TH1D* h_bgtot = 0;

  // to make legend and find max yvalue
  vector<TH1D*> bg_hists;
  vector<string> bg_names;
  vector<TH1D*> sig_hists;
  vector<string> sig_names;

  // background hists
  for( unsigned int i = 0 ; i < n ; ++i ) {
    if( TString(names.at(i)).Contains("data")  ) continue;
    if( TString(names.at(i)).Contains("sig")  ) continue;
    TString fullhistname = Form("%s/%s",histdir.c_str(),histname.c_str());
    // Make sure we take the "Fake" labeled histograms from MC
    if (TString(names.at(i)).Contains("fakephotonMC") ) {
      if (!fullhistname.Contains("Loose") ) fullhistname+="Fake";
      else if (fullhistname.Contains("LooseSieieSB")) fullhistname.ReplaceAll("LooseSieieSB", "FakeLooseSieieSB");
      else if (fullhistname.Contains("Loose")) fullhistname.ReplaceAll("Loose", "FakeLoose");
    }
    if (TString(names.at(i)).Contains("fakeLep") ) {
      fullhistname+="Fake";
    }
    // Try data-driven fake estimates: LooseNotTight * FR / (1 - FR)
    bool DataDrivenQCD = false;
    if (TString(names.at(i)).Contains("fakephotonData") && !fullhistname.Contains("Loose")) DataDrivenQCD = true;
    
    if (DataDrivenQCD) fullhistname+="LooseNotTight";

    if (histdir.size() == 0) fullhistname = TString(histname);
    TString newhistname = Form("%s_%s_%s",histname.c_str(),histdir.c_str(),names.at(i).c_str());
    TH1D* h_temp = (TH1D*) samples.at(i)->Get(fullhistname);
    if (h_temp == 0) continue;
    if (DataDrivenQCD)  {
      TH1D* h_prompt = (TH1D*) samples.at(i+1)->Get(fullhistname);
      if (h_prompt == 0) cout<<"Can't do prompt subtraction for "<<fullhistname<<endl;
      else h_temp->Add(h_prompt, -1.5); // 50% Data/MC scale factor
      h_temp->Scale( 0.168 / (1 - 0.168) ); // 0.168 Data FR
    }
    TH1D* h = (TH1D*) h_temp->Clone(newhistname);
    h->SetFillColor(getColor(names.at(i)));
    h->SetLineColor(kBlack);
    addOverflow(h); // Add Overflow
    if (rebin > 1) h->Rebin(rebin);
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
    h->SetLineColor(getColor(names.at(i)));
    h->SetLineWidth(2);
    addOverflow(h); // Add Overflow
    if (rebin > 1) h->Rebin(rebin);
    if (scalesig > 0.) h->Scale(scalesig);
    sig_hists.push_back(h);
    sig_names.push_back(names.at(i));
  }

  // loop through backgrounds to add hists to stack
  Double_t bg_integral_err = 0.;
  float bg_integral = h_bgtot->IntegralAndError(0,h_bgtot->GetXaxis()->GetNbins(),bg_integral_err);
  float data_integral = 1.;
  float bg_sf = 1.;
  float bg_sf_err = 0.;
  if (data_hist) {
    data_integral = data_hist->Integral(0,data_hist->GetXaxis()->GetNbins());
    bg_sf = data_integral/bg_integral;
    bg_sf_err = err_mult(data_integral,bg_integral,sqrt(data_integral),bg_integral_err,bg_sf);
    std::cout << "Data/MC is: " << bg_sf << " +/- " << bg_sf_err << std::endl;
  }
  for (unsigned int ibg = 0; ibg < bg_hists.size(); ++ibg) {
    if (scaleBGtoData && data_hist) bg_hists.at(ibg)->Scale(bg_sf);
    t->Add(bg_hists.at(ibg));
  }
  if (scaleBGtoData && data_hist) {
    h_bgtot->Scale(bg_sf);
    std::cout << "Scaled background by: " << bg_sf << " +/- " << bg_sf_err << std::endl;
  }
  
  float ymax = 0;
  if(h_bgtot) ymax = h_bgtot->GetMaximum();
  // also check signals for max val
  for (unsigned int isig = 0; isig < sig_hists.size(); ++isig) {
    ymax = std::max(ymax,(float)sig_hists.at(isig)->GetMaximum());
  }
  if (data_hist) ymax = std::max(ymax,(float)data_hist->GetMaximum());

  if( logplot ) ymax*=30;
  else          ymax*=1.5;
  float ymin = 0.1;

  TH2F* h_axes = new TH2F(Form("%s_%s_axes",histname.c_str(),histdir.c_str()),"",1000,xmin,xmax,1000,ymin,ymax);
  h_axes->GetXaxis()->SetTitle(xtitle.c_str());
  h_axes->GetXaxis()->SetLabelSize(0.04);
  h_axes->GetXaxis()->SetTitleSize(0.05);
  h_axes->GetYaxis()->SetTitle(ytitle.c_str());
  h_axes->GetYaxis()->SetLabelSize(0.04);
  h_axes->GetYaxis()->SetTitleOffset(0.95);
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

  if (data_hist) data_hist->Draw("pe1 x0 same");

  TLatex label;
  label.SetNDC();
  label.SetTextSize(0.032);
  float label_x_start = 0.2;
  float label_y_start = 0.82;
  float label_y_spacing = 0.04;
  if (doRatio) {
    label.SetTextSize(0.039);
    label_y_start = 0.84;
    label_y_spacing = 0.04;
  }
  
  //TString ht_label = getHTPlotLabel(histdir);
  TString ht_label = getHTPlotLabel(samples.at(0), histdir);
  TString region_label = getJetBJetPlotLabel(samples.at(0), histdir);
  TString region_label_line2 = getMT2PlotLabel(samples.at(0), histdir);
  //label.DrawLatex(0.2,0.85,ht_label);

  // ------- some hardcoded labels..
  // base region plots all have at least 2 jets
  if ((histdir.find("base") != std::string::npos)) region_label = "#geq 2j";
  if ((histdir.find("baseJ") != std::string::npos)) {
    region_label = "1j";
    region_label_line2 = "";
  }
  if ((histdir.find("nocut") != std::string::npos)) {
    ht_label = "HT #geq 0";
    region_label = "#geq 0j";
    region_label_line2 = "";
  }

  
  // bkg control regions
  if ((histdir.find("crslwjets") != std::string::npos)) region_label = "#geq 2j, 0b";
  if ((histdir.find("crslttbar") != std::string::npos)) region_label = "#geq 2j, #geq 2b";
  // minMT plot always requires at least 2 bjets
  if ((histdir.find("srbase") != std::string::npos) && (histname.find("minMTBMet") != std::string::npos)) region_label = "#geq 2j, #geq 2b";
  // lostlepton CR
  if ((histdir.find("crsl") != std::string::npos)) {
    ht_label = "";
    if (histdir.find("J") != std::string::npos) region_label_line2 = "E_{T}^{miss} > 200 GeV";
    if (histdir.find("met50") != std::string::npos) region_label_line2 = "E_{T}^{miss} > 50 GeV";
    else if (histdir.find("met80") != std::string::npos) region_label_line2 = "E_{T}^{miss} > 80 GeV";
    if (histdir.find("mt30") != std::string::npos) region_label_line2 += ", M_{T} > 30 GeV";
    if (histdir.find("J") != std::string::npos) region_label = "exactly 1j";
    else if (histdir.find("VL") != std::string::npos) {region_label = "#geq 2j"; ht_label = "200 < H_{T} < 450 GeV";}
    else if (histdir.find("L") != std::string::npos) {region_label = "#geq 2j"; ht_label = "450 < H_{T} < 575 GeV";}
    else if (histdir.find("M") != std::string::npos) {region_label = "#geq 2j"; ht_label = "575 < H_{T} < 1000 GeV";}
    else if (histdir.find("UH") != std::string::npos) {region_label = "#geq 2j"; ht_label = "H_{T} > 1500 GeV";}
    else if (histdir.find("H") != std::string::npos) {region_label = "#geq 2j"; ht_label = "1000 < H_{T} < 1500 GeV";}
    if (histdir.find("nj2") != std::string::npos) region_label = "#geq 2j";
    if (histdir.find("nb0") != std::string::npos) region_label += ", 0b";
    else if (histdir.find("nb2") != std::string::npos) region_label += ", #geq 2b";
  }
  if ((histdir.find("crslel") != std::string::npos)) region_label += ", 1 electron";
  else if ((histdir.find("crslmu") != std::string::npos)) region_label += ", 1 muon";
  else if ((histdir.find("crsl") != std::string::npos)) region_label += ", 1 lepton";

  // zll mt CR
  if (histdir.find("crmt") != std::string::npos) {
    if (histdir.find("base") != std::string::npos) region_label = "";
    else region_label = "#geq 2j, ";
    if ((histdir.find("base") != std::string::npos) || (histdir.find("nj2nb0") != std::string::npos)) ht_label = "";
    else if (histdir.find("ht200") != std::string::npos) ht_label = "H_{T} > 200 GeV";
    else if (histdir.find("ht450") != std::string::npos) ht_label = "H_{T} > 450 GeV";
    if (histdir.find("crmtel") != std::string::npos) region_label += "Z(ee) w/ removed lepton";
    else if (histdir.find("crmtmu") != std::string::npos) region_label += "Z(#mu#mu) w/ removed lepton";
    else region_label += "Z(ll) w/ removed lepton";
    if (histname.find("mt2gt50") != std::string::npos) region_label_line2 = "M_{T2} > 50 GeV";
    else if (histname.find("mt2gt100") != std::string::npos) region_label_line2 = "M_{T2} > 100 GeV";
    else if (histname.find("mt2gt150") != std::string::npos) region_label_line2 = "M_{T2} > 150 GeV";
    else if (histname.find("mt2gt200") != std::string::npos) region_label_line2 = "M_{T2} > 200 GeV";
    else region_label_line2 = "";
  }

  //soft lepton regions
  if (histdir.find("softl") != std::string::npos) {
    if (histdir.find("base") != std::string::npos){
      ht_label = "";
      region_label = "";
      region_label_line2 = "";
      if (histdir.find("softlmu") != std::string::npos){
      region_label = "Single Muon";
      }
      if (histdir.find("softlel") != std::string::npos){
      region_label = "Single Electron";
      }
    }
  }
  
  if (ht_label.Length() > 0) label.DrawLatex(label_x_start,label_y_start,ht_label);
  if (region_label.Length() > 0) label.DrawLatex(label_x_start,label_y_start - label_y_spacing,region_label);
  if (region_label_line2.Length() > 0) label.DrawLatex(label_x_start,label_y_start - 2 * label_y_spacing,region_label_line2);

  if (scaleBGtoData && data_hist) {
    TString scale_label = Form("MC scaled by %.2f #pm %.2f",bg_sf,bg_sf_err);
    label.DrawLatex(0.6,0.55,scale_label);
  }

  
  leg->Draw();
  h_axes->Draw("axissame");

  if (doRatio) {
    lumiTextSize     = 0.4;
    cmsTextSize      = 0.5;
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
    TPad* ratiopad = new TPad("ratiopad","ratiopad",0.,0.,1,0.21);
    ratiopad->SetTopMargin(0.05);
    ratiopad->SetBottomMargin(0.1);
    ratiopad->SetRightMargin(0.05);
    ratiopad->SetGridy();  // doesn't actually appear for some reason..
    ratiopad->Draw();
    ratiopad->cd();

    TH1D* h_ratio = (TH1D*) data_hist->Clone(Form("ratio_%s",data_hist->GetName()));
    h_ratio->Sumw2();
    h_bgtot->Sumw2();
    h_ratio->Divide(h_bgtot);
    
    // draw axis only
    TH1F* h_axis_ratio = new TH1F(Form("%s_axes",h_ratio->GetName()),"",100,xmin,xmax);
    h_axis_ratio->GetYaxis()->SetTitleOffset(0.25);
    h_axis_ratio->GetYaxis()->SetTitleSize(0.18);
    h_axis_ratio->GetYaxis()->SetNdivisions(5);
    h_axis_ratio->GetYaxis()->SetLabelSize(0.15);
    //h_axis_ratio->GetYaxis()->SetRangeUser(0.5,1.5);
    h_axis_ratio->GetYaxis()->SetRangeUser(0.001,2.0);
    h_axis_ratio->GetYaxis()->SetTitle("Data/MC");
    h_axis_ratio->GetXaxis()->SetTickLength(0.07);
    h_axis_ratio->GetXaxis()->SetTitleSize(0.);
    h_axis_ratio->GetXaxis()->SetLabelSize(0.);
    h_axis_ratio->Draw("axis");

    // draw line at 1
    TLine* line1 = new TLine(xmin,1,xmax,1);
    line1->SetLineStyle(2);
    line1->Draw("same");

    TGraphErrors* g_ratio = new TGraphErrors(h_ratio);
    g_ratio->SetName(Form("%s_graph",h_ratio->GetName()));
    for (int ibin=0; ibin < h_ratio->GetNbinsX(); ++ibin) {
      //      g_ratio->SetPointError(ibin, h_ratio->GetBinWidth(ibin+1)/2., h_ratio->GetBinError(ibin+1));
      g_ratio->SetPointError(ibin, 0., h_ratio->GetBinError(ibin+1));
    }
    g_ratio->SetLineColor(kBlack);
    g_ratio->SetMarkerColor(kBlack);
    g_ratio->SetMarkerStyle(20);
    g_ratio->Draw("p same");

  } // if (doRatio)
  
  gPad->Modified();
  gPad->Update();

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

  bool found_data = false;
  // backgrounds first -- loop backwards
  for( int i = n-1 ; i >= 0 ; --i ){
    if( TString(names.at(i)).Contains("data")  ) {found_data = true; continue;}
    if( TString(names.at(i)).Contains("sig")  ) continue;
    cout << getTableName(names.at(i));
    for ( unsigned int idir = 0; idir < ndirs; ++idir ) {
      TString fullhistname = Form("%s/h_mt2bins",dirs.at(idir).c_str());
      // if (names.at(i)=="gjets" || names.at(i)=="gjetsqcd") fullhistname.ReplaceAll("sr","crgj");
      // if (names.at(i)=="dyjets") fullhistname.ReplaceAll("sr","crdy");
      // if (names.at(i)=="wjets") fullhistname.ReplaceAll("sr","crrl");
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

  // next print data, if it exists
  if (found_data) {
    for( unsigned int i = 0 ; i < n ; i++ ){
      if( !TString(names.at(i)).Contains("data") ) continue;
      cout << getTableName(names.at(i));
      for ( unsigned int idir = 0; idir < ndirs; ++idir ) {
	TString fullhistname = Form("%s/h_mt2bins",dirs.at(idir).c_str());
	TH1D* h = (TH1D*) samples.at(i)->Get(fullhistname);
	double yield = 0.;
	if (h) {
	  // use all bins
	  if (mt2bin < 0) {
	    yield = h->Integral(0,-1);
	  }
	  // last bin: include overflow
	  else if (mt2bin == h->GetXaxis()->GetNbins()) {
	    yield = h->Integral(mt2bin,-1);
	  }
	  // single bin, not last bin
	  else {
	    yield = h->GetBinContent(mt2bin);
	  }
	}
	cout << "  &  " << Form("%d",(int)yield);
      }
      cout << " \\\\" << endl;
    } // loop over samples
    cout << "\\hline" << endl;
  } // if found_data
  
  // finally print signals
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
void printRatioTable( vector<TFile*> samples , vector<string> names , vector<string> dirs_num, vector<string> dirs_denom, int mt2bin = -1 ) {

  // read off yields from h_mt2bins hist in each topological region

  const unsigned int n = samples.size();
  const unsigned int ndirs = dirs_num.size();

  if (dirs_num.size() != dirs_denom.size()) {
    std::cout << "ERROR: mismatch in number of directories" << std::endl;
    return;
  }
  
  vector<double> bgtot_num(ndirs,0.);
  vector<double> bgerr_num(ndirs,0.);
  vector<double> bgtot_denom(ndirs,0.);
  vector<double> bgerr_denom(ndirs,0.);
  
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
    cout << " & " << getJetBJetTableLabel(samples.at(0), dirs_num.at(idir));
  }
  cout << " \\\\" << endl
    << "\\hline\\hline" << endl;

  // backgrounds first -- loop backwards
  for( int i = n-1 ; i >= 0 ; --i ){
    if( TString(names.at(i)).Contains("data")  ) continue;
    if( TString(names.at(i)).Contains("sig")  ) continue;
    cout << getTableName(names.at(i));
    for ( unsigned int idir = 0; idir < ndirs; ++idir ) {
      TString fullhistname_num = Form("%s/h_mt2bins",dirs_num.at(idir).c_str());
      TH1D* h_num = (TH1D*) samples.at(i)->Get(fullhistname_num);
      double yield_num = 0.;
      double err_num = 0.;
      if (h_num) {
        // use all bins
        if (mt2bin < 0) {
          yield_num = h_num->IntegralAndError(0,-1,err_num);
          bgtot_num.at(idir) += yield_num;
          bgerr_num.at(idir) = sqrt(pow(bgerr_num.at(idir),2) + pow(err_num,2));
        }
        // last bin: include overflow
        else if (mt2bin == h_num->GetXaxis()->GetNbins()) {
          yield_num = h_num->IntegralAndError(mt2bin,-1,err_num);
          bgtot_num.at(idir) += yield_num;
          bgerr_num.at(idir) = sqrt(pow(bgerr_num.at(idir),2) + pow(err_num,2));
        }
        // single bin, not last bin
        else {
          yield_num = h_num->GetBinContent(mt2bin);
          err_num = h_num->GetBinError(mt2bin);
          bgtot_num.at(idir) += yield_num;
          bgerr_num.at(idir) = sqrt(pow(bgerr_num.at(idir),2) + pow(err_num,2));
        }
      }

      TString fullhistname_denom = Form("%s/h_mt2bins",dirs_denom.at(idir).c_str());
      TH1D* h_denom = (TH1D*) samples.at(i)->Get(fullhistname_denom);
      double yield_denom = 0.;
      double err_denom = 0.;
      if (h_denom) {
        // use all bins
        if (mt2bin < 0) {
          yield_denom = h_denom->IntegralAndError(0,-1,err_denom);
          bgtot_denom.at(idir) += yield_denom;
          bgerr_denom.at(idir) = sqrt(pow(bgerr_denom.at(idir),2) + pow(err_denom,2));
        }
        // last bin: include overflow
        else if (mt2bin == h_denom->GetXaxis()->GetNbins()) {
          yield_denom = h_denom->IntegralAndError(mt2bin,-1,err_denom);
          bgtot_denom.at(idir) += yield_denom;
          bgerr_denom.at(idir) = sqrt(pow(bgerr_denom.at(idir),2) + pow(err_denom,2));
        }
        // single bin, not last bin
        else {
          yield_denom = h_denom->GetBinContent(mt2bin);
          err_denom = h_denom->GetBinError(mt2bin);
          bgtot_denom.at(idir) += yield_denom;
          bgerr_denom.at(idir) = sqrt(pow(bgerr_denom.at(idir),2) + pow(err_denom,2));
        }
      }

      if (yield_denom == 0.) {
        cout << "  &  - ";
      } else {
	double ratio = yield_num / yield_denom;
	double ratio_err = ratio * sqrt(pow(err_num/yield_num,2) + pow(err_denom/yield_denom,2));
        cout << "  &  " << Form("%.2f $\\pm$ %.2f",ratio,ratio_err);
      }
    }
    cout << " \\\\" << endl;
  } // loop over samples

  // print bg totals
  cout << "\\hline" << endl;
  cout << "Total SM";
  for ( unsigned int idir = 0; idir < ndirs; ++idir ) {
    double yield_num = bgtot_num.at(idir);
    double err_num = bgerr_num.at(idir);
    double yield_denom = bgtot_denom.at(idir);
    double err_denom = bgerr_denom.at(idir);
    if (yield_denom == 0.) {
      cout << "  &  - ";
    } else {
      double ratio = yield_num / yield_denom;
      double ratio_err = ratio * sqrt(pow(err_num/yield_num,2) + pow(err_denom/yield_denom,2));
      cout << "  &  " << Form("%.2f $\\pm$ %.2f",ratio,ratio_err);
    }
  }
  cout << " \\\\" << endl;
  cout << "\\hline" << endl;

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
    if( TString(names.at(isamp)).Contains("data")  ) continue;
    if( TString(names.at(isamp)).Contains("sig")  ) continue;
    cout << getTableName(names.at(isamp));
    for (int ibin = 1; ibin <= n_mt2bins; ++ibin) {
      TString fullhistname = Form("%s/h_mt2bins",dir.c_str());
      if (names.at(isamp)=="gjets" || names.at(isamp)=="gjetsqcd") fullhistname.ReplaceAll("sr","crgj");
      if (names.at(isamp)=="dyjets") fullhistname.ReplaceAll("sr","crdy");
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
// requires output files from ZllMTLooper
void plotMakerZllMT(){

  //  gROOT->LoadMacro("CMS_lumi.C");
  cmsText = "CMS Preliminary";
  cmsTextSize = 0.5;
  lumiTextSize = 0.4;
  writeExtraText = false;
  lumi_13TeV = "1.3 fb^{-1}";

  string input_dir = "/home/olivito/cms3/MT2Analysis/ZllMTLooper/output/V00-01-07_25ns_miniaodv2_1p26fb_mt2gt200_crht200/";

  
  // ----------------------------------------
  //  samples definition
  // ----------------------------------------

  // get input files

  TFile* f_ttbar = new TFile(Form("%s/top.root",input_dir.c_str())); //hadd'ing of ttbar, ttw, ttz, tth, singletop
  TFile* f_dyjets = new TFile(Form("%s/dyjetsll_ht.root",input_dir.c_str()));
  //TFile* f_dyjets = new TFile(Form("%s/zjets_mg_lo.root",input_dir.c_str()));
  TFile* f_data = new TFile(Form("%s/data_Run2015D.root",input_dir.c_str())); 

  vector<TFile*> samples;
  vector<string>  names;

  samples.push_back(f_ttbar); names.push_back("top");
  samples.push_back(f_dyjets); names.push_back("dyjets");
  samples.push_back(f_data); names.push_back("data");

  // ----------------------------------------
  //  plots definitions
  // ----------------------------------------

  float scalesig = -1.;
  //float scalesig = 50.;
  bool printplots = true;
  bool doRatio = true;
  bool scaleBGtoData = true;

  if(printplots){
    TIter it(f_ttbar->GetListOfKeys());
    TKey* k;
    std::string cr_skip = "cr";
    std::string sr_skip = "sr";
    while ((k = (TKey *)it())) {
      //if (strncmp (k->GetTitle(), cr_skip.c_str(), cr_skip.length()) == 0) continue; //skip control regions
      //if (strncmp (k->GetTitle(), sr_skip.c_str(), sr_skip.length()) == 0) continue; //skip signal regions and srbase
      std::string dir_name = k->GetTitle();
      //if(dir_name != "crmtbase") continue; //to do only this dir
      //if(dir_name != "crmtnj2nb0") continue; //to do only this dir
      if(dir_name != "crmtht200") continue; //to do only this dir
      //if(dir_name != "crmtht450") continue; //to do only this dir
      //if(dir_name != "crmtmuht200" && dir_name != "crmtelht200") continue; //to do only this dir

      makePlot( samples , names , dir_name , "h_zll_mass" , "Dilepton Mass [GeV]" , "Events / 1 GeV" , 0 , 200 , 1 , false , printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_nVert" , "N(vertices)" , "Events" , 0 , 50 , 1 , false , printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_kill_lep_pt" , "Removed Lepton p_{T} [GeV]" , "Events / 5 GeV" , 0 , 500 , 5 , true , printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_keep_lep_pt" , "Kept Lepton p_{T} [GeV]" , "Events / 5 GeV" , 0 , 500 , 5 , true , printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_zll_pt" , "Dilepton p_{T} [GeV]" , "Events / 5 GeV" , 0 , 500 , 5 , true , printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_zllmt_met_pt" , "Transformed E_{T}^{miss} [GeV]" , "Events / 10 GeV" , 0 , 500 , 5 , true, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_zllmt_mt2" , "Transformed M_{T2} [GeV]" , "Events / 10 GeV" , 0 , 500 , 5 , true, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_zllmt_mt" , "Transformed M_{T} [GeV]" , "Events / 10 GeV" , 0 , 300 , 5 , true, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_zllmt_mt_mt2gt50" , "Transformed M_{T} [GeV]" , "Events / 10 GeV" , 0 , 300 , 5 , true, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_zllmt_mt_mt2gt100" , "Transformed M_{T} [GeV]" , "Events / 10 GeV" , 0 , 300 , 5 , true, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_zllmt_mt_mt2gt150" , "Transformed M_{T} [GeV]" , "Events / 10 GeV" , 0 , 300 , 5 , true, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_zllmt_mt_mt2gt200" , "Transformed M_{T} [GeV]" , "Events / 10 GeV" , 0 , 300 , 5 , true, printplots, scalesig, doRatio, scaleBGtoData );

    }
  }
}
  
//_______________________________________________________________________________
void plotMakerGJets(){
  
  //  gROOT->LoadMacro("CMS_lumi.C");
  cmsText = "CMS Preliminary";
  cmsTextSize = 0.5;
  lumiTextSize = 0.4;
  writeExtraText = false;
  lumi_13TeV = "1.26 fb^{-1}";
  
  string input_dir = "/Users/giovannizevidellaporta/UCSD/MT2/Zinvisible/MT2babies/V00-01-06_25ns_miniaodv1_skim_base_mt2gt200_ZinvV3_1fb";
  
  
  // ----------------------------------------
  //  samples definition
  // ----------------------------------------
  
  // get input files
  
//  TFile* f_ttbar = new TFile(Form("%s/top.root",input_dir.c_str()));
  TFile* f_ttbar = new TFile(Form("%s/ttsl_mg_lo.root",input_dir.c_str()));
  TFile* f_gjet = new TFile(Form("%s/gjet_ht.root",input_dir.c_str()));
  TFile* f_qcd = new TFile(Form("%s/qcd_ht.root",input_dir.c_str()));
  TFile* f_data = new TFile(Form("%s/data_Run2015D.root",input_dir.c_str()));
  
  vector<TFile*> samples;
  vector<string>  names;
  samples.push_back(f_data); names.push_back("data");
  samples.push_back(f_qcd);   names.push_back("fragphoton");
  //samples.push_back(f_qcd);   names.push_back("fakephotonMC");
  samples.push_back(f_data);   names.push_back("fakephotonData");
  samples.push_back(f_gjet);  names.push_back("gjet"); //this needs to be right after fakephotonData, for prompt subtraction!
  
  vector<TFile*> samples2;
  vector<string>  names2;
  samples2.push_back(f_data); names2.push_back("data");
  samples2.push_back(f_qcd);   names2.push_back("fragphoton");
  samples2.push_back(f_qcd);   names2.push_back("fakephotonMC");
  samples2.push_back(f_gjet);  names2.push_back("gjet");
  
  // ----------------------------------------
  //  plots definitions
  // ----------------------------------------
  
  float scalesig = -1.;
  //float scalesig = 50.;
  bool printplots = true;
  bool doRatio = true;
  bool scaleBGtoData = true;
  
  if(printplots){
    TIter it(f_ttbar->GetListOfKeys());
    TKey* k;
    std::string cr_skip = "cr";
    std::string sr_skip = "sr";
    while ((k = (TKey *)it())) {
      //if (strncmp (k->GetTitle(), cr_skip.c_str(), cr_skip.length()) == 0) continue; //skip control regions
      //if (strncmp (k->GetTitle(), sr_skip.c_str(), sr_skip.length()) == 0) continue; //skip signal regions and srbase
      std::string dir_name = k->GetTitle();
      if(dir_name == "") continue;
      //if(dir_name != "srbase") continue; //to do only this dir
      //      if(dir_name != "crrlmubase") continue; //for testing
      if(dir_name != "crgjbaseJ" && dir_name != "crgjbase" && dir_name != "crgjnocut") continue; //for testing

      bool dolog = false;
      string s = "";
      for (int j = 0; j < 1; j++) {
        if (j>0) dolog = true;
        makePlot( samples , names , dir_name , "h_ht"  , "H_{T} [GeV]" , "Events / 50 GeV" , 200 , 1500 , 2 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples , names , dir_name , "h_htbins"  , "H_{T} [GeV]" , "Events / 50 GeV" , 0 , 1500 , 1 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples2, names2, dir_name , "h_chisoLoose"  , "Charged Iso [GeV]" , "Events " , 0 , 20 , 1 , true, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples2, names2, dir_name , "h_chisoEBLoose"  , "Charged Iso [GeV]" , "Events " , 0 , 20 , 2 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples2, names2, dir_name , "h_chisoEELoose"  , "Charged Iso [GeV]" , "Events " , 0 , 20 , 2 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples2, names2, dir_name , "h_chisoLooseSieieSB"  , "Charged Iso [GeV]" , "Events " , 0 , 20 , 5 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples2, names2, dir_name , "h_chisoEBLooseSieieSB"  , "Charged Iso [GeV]" , "Events " , 0 , 20 , 5 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples2, names2, dir_name , "h_chisoEELooseSieieSB"  , "Charged Iso [GeV]" , "Events " , 0 , 20 , 5 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples , names , dir_name , "h_mt2" , "M_{T2} [GeV]" , "Events / 50 GeV" , 0 , 1000 , 5 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples , names , dir_name , "h_met"  , "E_{T}^{miss} [GeV]" , "Events / 50 GeV" , 0 , 1000 , 5 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples , names , dir_name , "h_simplemet"  , "Untransformed E_{T}^{miss} [GeV]" , "Events / 50 GeV" , 0 , 500 , 5 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples , names , dir_name , "h_gammaPt"  , "Photon p_{T} [GeV]" , "Events / 50 GeV" , 179 , 1000 , 10 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples , names , dir_name , "h_gammaPtEB"  , "Photon p_{T} [GeV]" , "Events / 50 GeV" , 179 , 1000 , 10 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples , names , dir_name , "h_gammaPtEE"  , "Photon p_{T} [GeV]" , "Events / 50 GeV" , 179 , 1000 , 10 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples , names , dir_name , "h_gammaEta"  , "Photon Eta" , "Events" , -2.5 , 2.5 , 5 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples , names , dir_name , "h_nlepveto" , "N(leptons)" , "Events" , 0 , 10 , 1 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples , names , dir_name , "h_nJet30" , "N(jets)" , "Events" , 0 , 15 , 1 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples , names , dir_name , "h_nBJet20" , "N(b jets)" , "Events" , 0 , 6 , 1 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples , names , dir_name , "h_mt2bins" , "M_{T2} [GeV]" , "Events / Bin" , 200 , 1500 , 1 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        //makePlot( samples , names , dir_name , "h_nJet30Eta3" , "N(jets, |#eta| > 3.0)" , "Events" , 0 , 5 , 1 , false, printplots, scalesig, doRatio, scaleBGtoData );
        //makePlot( samples , names , dir_name , "h_leppt"  , "Lepton p_{T} [GeV]" , "Events" , 0 , 1000 , 10 , false, printplots, scalesig, doRatio, scaleBGtoData );
      }
      
    }
    
  }
  
}
//_______________________________________________________________________________
void plotMakerDY(){
  
  //  gROOT->LoadMacro("CMS_lumi.C");
  cmsText = "CMS Preliminary";
  cmsTextSize = 0.5;
  lumiTextSize = 0.4;
  writeExtraText = false;
  lumi_13TeV = "1.26 fb^{-1}";
  
  string input_dir = "/Users/giovannizevidellaporta/UCSD/MT2/Zinvisible/MT2babies/V00-01-06_25ns_miniaodv1_skim_base_mt2gt200_ZinvV3_1fb";
//  string input_dir = "/Users/giovannizevidellaporta/UCSD/MT2/Zinvisible/MT2babies/V00-01-06_25ns_miniaodv1";
  
  
  // ----------------------------------------
  //  samples definition
  // ----------------------------------------
  
  // get input files
  
  TFile* f_ttbar = new TFile(Form("%s/top.root",input_dir.c_str()));
  //TFile* f_wjets = new TFile(Form("%s/wjets_ht.root",input_dir.c_str()));
  TFile* f_dyjets = new TFile(Form("%s/dyjetsll_ht.root",input_dir.c_str()));
//  TFile* f_dyjets = new TFile(Form("%s/zjets_amcatnlo.root",input_dir.c_str()));
  //TFile* f_qcd = new TFile(Form("%s/qcd_ht.root",input_dir.c_str()));
  TFile* f_data = new TFile(Form("%s/data_Run2015D.root",input_dir.c_str()));
  
  vector<TFile*> samples;
  vector<string>  names;
  samples.push_back(f_data); names.push_back("data");
  //samples.push_back(f_qcd); names.push_back("qcd");
  samples.push_back(f_ttbar); names.push_back("top");
  //samples.push_back(f_wjets); names.push_back("wjets");
  samples.push_back(f_dyjets); names.push_back("dyjets");
  
  
  // ----------------------------------------
  //  plots definitions
  // ----------------------------------------
  
  float scalesig = -1.;
  //float scalesig = 50.;
  bool printplots = true;
  bool doRatio = true;
  bool scaleBGtoData = true;
  
  if(printplots){
    TIter it(f_ttbar->GetListOfKeys());
    TKey* k;
    std::string cr_skip = "cr";
    std::string sr_skip = "sr";
    while ((k = (TKey *)it())) {
      //if (strncmp (k->GetTitle(), cr_skip.c_str(), cr_skip.length()) == 0) continue; //skip control regions
      //if (strncmp (k->GetTitle(), sr_skip.c_str(), sr_skip.length()) == 0) continue; //skip signal regions and srbase
      std::string dir_name = k->GetTitle();
      if(dir_name == "") continue;
      //if(dir_name != "srbase") continue; //to do only this dir
      //      if(dir_name != "crrlmubase") continue; //for testing
      if(dir_name != "crdybaseJ" && dir_name != "crdybase") continue; //for testing
      //if(dir_name != "crdynocut") continue; //for testing
      
      bool dolog = false;
      string s = "";
      for (int j = 0; j < 1; j++) {
        if (j>0) dolog = true;
        makePlot( samples , names , dir_name , "h_ht"  , "H_{T} [GeV]" , "Events / 50 GeV" , 200 , 1500 , 2 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples , names , dir_name , "h_htbins"  , "H_{T} [GeV]" , "Events / 50 GeV" , 200 , 1500 , 1 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples , names , dir_name , "h_mt2" , "M_{T2} [GeV]" , "Events / 50 GeV" , 0 , 1000 , 5 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples , names , dir_name , "h_met"  , "E_{T}^{miss} [GeV]" , "Events / 50 GeV" , 0 , 1000 , 5 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples , names , dir_name , "h_nlepveto" , "N(leptons)" , "Events" , 0 , 10 , 1 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples , names , dir_name , "h_nJet30" , "N(jets)" , "Events" , 0 , 15 , 1 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples , names , dir_name , "h_nBJet20" , "N(b jets)" , "Events" , 0 , 6 , 1 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples , names , dir_name , "h_mt2bins" , "M_{T2} [GeV]" , "Events / Bin" , 200 , 1500 , 1 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples , names , dir_name , "h_zllmass" , "M_{ll} [GeV]" , "Events / Bin" , 60 , 120 , 1 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        //makePlot( samples , names , dir_name , "h_nJet30Eta3" , "N(jets, |#eta| > 3.0)" , "Events" , 0 , 5 , 1 , false, printplots, scalesig, doRatio, scaleBGtoData );
        //makePlot( samples , names , dir_name , "h_leppt"  , "Lepton p_{T} [GeV]" , "Events" , 0 , 1000 , 10 , false, printplots, scalesig, doRatio, scaleBGtoData );
      }
      
    }
  }

  
}

//_______________________________________________________________________________
void plotMakerRemovedLep(){
  
  //  gROOT->LoadMacro("CMS_lumi.C");
  cmsText = "CMS Preliminary";
  cmsTextSize = 0.5;
  lumiTextSize = 0.4;
  writeExtraText = false;
  lumi_13TeV = "1.26 fb^{-1}";
  
  string input_dir = "/Users/giovannizevidellaporta/UCSD/MT2/Zinvisible/MT2babies/V00-01-06_25ns_miniaodv1_skim_base_mt2gt200_ZinvV3_1fb";
  
  
  // ----------------------------------------
  //  samples definition
  // ----------------------------------------
  
  // get input files
  
  TFile* f_ttbar = new TFile(Form("%s/top.root",input_dir.c_str()));
  TFile* f_wjets = new TFile(Form("%s/wjets_ht.root",input_dir.c_str()));
//  TFile* f_dyjets = new TFile(Form("%s/dyjetsll_ht.root",input_dir.c_str()));
  TFile* f_qcd = new TFile(Form("%s/qcd_ht.root",input_dir.c_str()));
  TFile* f_data = new TFile(Form("%s/data_Run2015D.root",input_dir.c_str()));
  
  vector<TFile*> samples;
  vector<string>  names;
  samples.push_back(f_data); names.push_back("data");
  samples.push_back(f_qcd); names.push_back("qcd");
  samples.push_back(f_ttbar); names.push_back("top");
  samples.push_back(f_wjets); names.push_back("wjets");
//  samples.push_back(f_dyjets); names.push_back("dyjets");

  
  // ----------------------------------------
  //  plots definitions
  // ----------------------------------------
  
  float scalesig = -1.;
  //float scalesig = 50.;
  bool printplots = true;
  bool doRatio = true;
  bool scaleBGtoData = true;
  
  if(printplots){
    TIter it(f_ttbar->GetListOfKeys());
    TKey* k;
    std::string cr_skip = "cr";
    std::string sr_skip = "sr";
    while ((k = (TKey *)it())) {
      //if (strncmp (k->GetTitle(), cr_skip.c_str(), cr_skip.length()) == 0) continue; //skip control regions
      //if (strncmp (k->GetTitle(), sr_skip.c_str(), sr_skip.length()) == 0) continue; //skip signal regions and srbase
      std::string dir_name = k->GetTitle();
      if(dir_name == "") continue;
      //if(dir_name != "srbase") continue; //to do only this dir
      //      if(dir_name != "crrlmubase") continue; //for testing
      if(dir_name != "crrlbaseJ" && dir_name != "crrlbase") continue; //for testing
      
      bool dolog = false;
      string s = "";
      for (int j = 0; j < 1; j++) {
        if (j>0) dolog = true;
        makePlot( samples , names , dir_name , "h_ht"  , "H_{T} [GeV]" , "Events / 50 GeV" , 200 , 1500 , 2 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        //makePlot( samples , names , dir_name , "h_lepabsiso"  , "Abs Iso [GeV]" , "Events " , 0 , 50 , 2 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        //makePlot( samples , names , dir_name , "h_lepreliso"  , "Rel Iso" , "Events " , 0 , 1 , 2 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples , names , dir_name , "h_mt2" , "M_{T2} [GeV]" , "Events / 50 GeV" , 0 , 1000 , 5 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples , names , dir_name , "h_mt" , "M_{T} (lep, MET) [GeV]" , "Events " , 0 , 200 , 5 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples , names , dir_name , "h_met"  , "E_{T}^{miss} [GeV]" , "Events / 50 GeV" , 200 , 1000 , 5 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        //makePlot( samples , names , dir_name , "h_simplemet"  , "Untransformed E_{T}^{miss} [GeV]" , "Events / 50 GeV" , 0 , 1000 , 10 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples , names , dir_name , "h_leppt"  , "Lepton p_{T} [GeV]" , "Events / 50 GeV" , 0 , 1000 , 10 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples , names , dir_name , "h_lepeta"  , "Lepton #eta " , "Events " , -2.5 , 2.5 , 5 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples , names , dir_name , "h_nlepveto" , "N(leptons)" , "Events" , 0 , 10 , 1 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples , names , dir_name , "h_nJet30" , "N(jets)" , "Events" , 0 , 15 , 1 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples , names , dir_name , "h_nBJet20" , "N(b jets)" , "Events" , 0 , 6 , 1 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        makePlot( samples , names , dir_name , "h_mt2bins" , "M_{T2} [GeV]" , "Events / Bin" , 200 , 1500 , 1 , dolog, printplots, scalesig, doRatio, scaleBGtoData );
        //makePlot( samples , names , dir_name , "h_nJet30Eta3" , "N(jets, |#eta| > 3.0)" , "Events" , 0 , 5 , 1 , false, printplots, scalesig, doRatio, scaleBGtoData );
        //makePlot( samples , names , dir_name , "h_leppt"  , "Lepton p_{T} [GeV]" , "Events" , 0 , 1000 , 10 , false, printplots, scalesig, doRatio, scaleBGtoData );
      }
      
    }
  }
  
//  std::cout << "\\documentclass[landscape, 10pt]{article}" << std::endl;
//  std::cout << "\\usepackage{amsmath}" << std::endl;
//  std::cout << "\\usepackage{amssymb}" << std::endl;
//  std::cout << "\\usepackage{graphicx}" << std::endl;
//  std::cout << "\\usepackage[left=.1in,top=1in,right=.1in,bottom=.1in,nohead]{geometry}" << std::endl;
//  std::cout << "\\begin{document}" << std::endl;
//  vector<string> dirs;
//  dirs.push_back("crrl1L");
//  dirs.push_back("crrl4L");
//  dirs.push_back("crrl7L");
//  printTable(samples, names, dirs);
//  dirs.clear();
//  dirs.push_back("crrl1M");
//  dirs.push_back("crrl4M");
//  dirs.push_back("crrl7M");
//  printTable(samples, names, dirs);
//  dirs.clear();
//  dirs.push_back("crrl1H");
//  dirs.push_back("crrl4H");
//  dirs.push_back("crrl7H");
//  printTable(samples, names, dirs);
//  dirs.clear();
//  dirs.push_back("crrl1UH");
//  dirs.push_back("crrl4UH");
//  dirs.push_back("crrl7UH");
//  printTable(samples, names, dirs);
//  dirs.clear();
//  dirs.push_back("crrlbase");
//  for(unsigned int i=0; i<dirs.size(); i++){
//    printDetailedTable(samples, names, dirs.at(i));
//    if(i % 2 != 0) std::cout << "\\pagebreak" << std::endl; //two tables per page
//  }
//  std::cout << "\\end{document}" << std::endl;

  
}

void tableMakerZinvCR(){
  
  string input_dir = "/Users/giovannizevidellaporta/UCSD/MT2/Zinvisible/MT2babies/V00-01-06_25ns_miniaodv1_skim_base_mt2gt200_ZinvV3_1fb";
  
  
  // ----------------------------------------
  //  samples definition
  // ----------------------------------------
  
  // get input files
  
  TFile* f_zinv = new TFile(Form("%s/zinv_ht.root",input_dir.c_str()));
  TFile* f_dyjets = new TFile(Form("%s/dyjetsll_ht.root",input_dir.c_str()));
  TFile* f_gjet = new TFile(Form("%s/gjet_ht.root",input_dir.c_str()));
  TFile* f_wjet = new TFile(Form("%s/wjet_ht.root",input_dir.c_str()));
  TFile* f_gjetqcd = new TFile(Form("%s/qcdplusgjet.root",input_dir.c_str()));
  
  vector<TFile*> samples;
  vector<string>  names;
  samples.push_back(f_zinv); names.push_back("zinv");
  samples.push_back(f_dyjets); names.push_back("dyjets");
  samples.push_back(f_gjet); names.push_back("gjets");
  samples.push_back(f_wjet); names.push_back("wjets");
  samples.push_back(f_gjetqcd); names.push_back("gjetsqcd");

  
    std::cout << "\\documentclass[landscape, 10pt]{article}" << std::endl;
    std::cout << "\\usepackage{amsmath}" << std::endl;
    std::cout << "\\usepackage{amssymb}" << std::endl;
    std::cout << "\\usepackage{graphicx}" << std::endl;
    std::cout << "\\usepackage[left=.1in,top=1in,right=.1in,bottom=.1in,nohead]{geometry}" << std::endl;
    std::cout << "\\begin{document}" << std::endl;
    vector<string> dirs;
    dirs.push_back("sr1VL");
    dirs.push_back("sr2VL");
    dirs.push_back("sr4VL");
    dirs.push_back("sr5VL");
    dirs.push_back("sr7VL");
    dirs.push_back("sr8VL");
    printTable(samples, names, dirs);
    dirs.clear();
    dirs.push_back("srbaseJ");
    for(unsigned int i=0; i<dirs.size(); i++){
      printDetailedTable(samples, names, dirs.at(i));
      if(i % 2 != 0) std::cout << "\\pagebreak" << std::endl; //two tables per page
    }
    std::cout << "\\end{document}" << std::endl;
  
  
}

//_______________________________________________________________________________
void plotMakerSoftLepSR(){

  //  gROOT->LoadMacro("CMS_lumi.C");
  cmsText = "CMS Preliminary";
  cmsTextSize = 0.5;
  lumiTextSize = 0.4;
  writeExtraText = false;
  lumi_13TeV = "4 fb^{-1}";

  string input_dir = "../SoftLepLooper/output/softLepSR";
  
  // ----------------------------------------
  //  samples definition
  // ----------------------------------------

  // get input files

  TFile* f_ttbar = new TFile(Form("%s/top.root",input_dir.c_str())); //hadd'ing of ttbar, ttw, ttz, tth, singletop
  TFile* f_tt1l = new TFile(Form("%s/ttsl.root",input_dir.c_str()));
  TFile* f_tt2l = new TFile(Form("%s/ttdl.root",input_dir.c_str())); 
  TFile* f_st = new TFile(Form("%s/singletop.root",input_dir.c_str())); 
  TFile* f_wjets = new TFile(Form("%s/wjets_ht.root",input_dir.c_str()));
  TFile* f_qcd = new TFile(Form("%s/qcd_ht.root",input_dir.c_str()));
  TFile* f_zinv = new TFile(Form("%s/zinv_ht.root",input_dir.c_str()));
  //TFile* f_data = new TFile(Form("%s/data_Run2015D.root",input_dir.c_str())); 

  vector<TFile*> samples;
  vector<string>  names;

  samples.push_back(f_qcd); names.push_back("qcd");
  samples.push_back(f_tt2l); names.push_back("tt+2l fakeLep");
  samples.push_back(f_zinv); names.push_back("zinv fakeLep");
  samples.push_back(f_st); names.push_back("st fakeLep");
  samples.push_back(f_tt1l); names.push_back("tt+1l fakeLep");
  samples.push_back(f_wjets); names.push_back("wjets fakeLep");
  samples.push_back(f_tt2l); names.push_back("tt+2l");
  samples.push_back(f_st); names.push_back("st");
  //samples.push_back(f_ttbar); names.push_back("top");
  samples.push_back(f_tt1l); names.push_back("tt+1l");
  samples.push_back(f_wjets); names.push_back("wjets");
  // samples.push_back(f_data); names.push_back("data");

  
  // ----------------------------------------
  //  plots definitions
  // ----------------------------------------

  float scalesig = -1.;
  //float scalesig = 50.;
  bool printplots = true;
  bool doRatio = true;
  bool scaleBGtoData = false;

  if(printplots){
    TIter it(f_wjets->GetListOfKeys());
    TKey* k;
    std::string cr_skip = "cr";
    std::string sr_skip = "sr";
    while ((k = (TKey *)it())) {
      //if (strncmp (k->GetTitle(), cr_skip.c_str(), cr_skip.length()) == 0) continue; //skip control regions
      //if (strncmp (k->GetTitle(), sr_skip.c_str(), sr_skip.length()) == 0) continue; //skip signal regions and srbase
      std::string dir_name = k->GetTitle();
      //if(dir_name != "srsoftlbase") continue; //to do only this dir
      if(dir_name != "srsoftlmubase" && dir_name != "srsoftlelbase") continue; //to do only this dir

      makePlot( samples , names , dir_name , "h_lepptshort" , "Lepton p_{T} [GeV]" , "Events / 1 GeV" , 0 , 25 , 1 , false , printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_ht"  , "H_{T} [GeV]" , "Events / 25 GeV" , 0 , 2000 , 1 , false, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_mt2" , "M_{T2} [GeV]" , "Events / 10 GeV" , 0 , 1000 , 1 , false, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_met"  , "E_{T}^{miss} [GeV]" , "Events / 10 GeV" , 0 , 800 , 1 , false, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_nJet30" , "N(jets)" , "Events" , 0 , 15 , 1 , false, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_nBJet20" , "N(b jets)" , "Events" , 0 , 6 , 1 , false, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_J0pt" , "p_{T}(jet1) [GeV]" , "Events / 20 GeV" , 0 , 1000 , 1 , false , printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_J1pt" , "p_{T}(jet2) [GeV]" , "Events / 20 GeV" , 0 , 1000 , 1 , false , printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_mt" , "M_{T} [GeV]" , "Events / 5 GeV" , 0 , 500 , 5 , false , printplots, scalesig, doRatio, scaleBGtoData );

      //log plots
      makePlot( samples , names , dir_name , "h_lepptshort" , "Lepton p_{T} [GeV]" , "Events / 1 GeV" , 0 , 25 , 1 , true , printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_ht"  , "H_{T} [GeV]" , "Events / 25 GeV" , 0 , 2000 , 1 , true, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_mt2" , "M_{T2} [GeV]" , "Events / 10 GeV" , 0 , 1000 , 1 , true, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_met"  , "E_{T}^{miss} [GeV]" , "Events / 10 GeV" , 0 , 800 , 1 , true, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_nJet30" , "N(jets)" , "Events" , 0 , 15 , 1 , true, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_nBJet20" , "N(b jets)" , "Events" , 0 , 6 , 1 , true, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_J0pt" , "p_{T}(jet1) [GeV]" , "Events / 20 GeV" , 0 , 1000 , 1 , true , printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_J1pt" , "p_{T}(jet2) [GeV]" , "Events / 20 GeV" , 0 , 1000 , 1 , true , printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_mt" , "M_{T} [GeV]" , "Events / 5 GeV" , 0 , 500 , 5 , true , printplots, scalesig, doRatio, scaleBGtoData );

    }
  }
}


void plotMakerSoftLepCR(){

  //  gROOT->LoadMacro("CMS_lumi.C");
  cmsText = "CMS Preliminary";
  cmsTextSize = 0.5;
  lumiTextSize = 0.4;
  writeExtraText = false;
  lumi_13TeV = "4 fb^{-1}";

  string input_dir = "../SoftLepLooper/output/softLepCR";
  
  // ----------------------------------------
  //  samples definition
  // ----------------------------------------

  // get input files
  
  TFile* f_ttbar = new TFile(Form("%s/top.root",input_dir.c_str())); //hadd'ing of ttbar, ttw, ttz, tth, singletop
  TFile* f_tt1l = new TFile(Form("%s/ttsl.root",input_dir.c_str()));
  TFile* f_tt2l = new TFile(Form("%s/ttdl.root",input_dir.c_str())); 
  TFile* f_st = new TFile(Form("%s/singletop.root",input_dir.c_str())); 
  TFile* f_wjets = new TFile(Form("%s/wjets_ht.root",input_dir.c_str()));
  TFile* f_qcd = new TFile(Form("%s/qcd_ht.root",input_dir.c_str()));
  TFile* f_zinv = new TFile(Form("%s/zinv_ht.root",input_dir.c_str()));
  //TFile* f_data = new TFile(Form("%s/data_Run2015D.root",input_dir.c_str())); 

  vector<TFile*> samples;
  vector<string>  names;

  samples.push_back(f_qcd); names.push_back("qcd");
  samples.push_back(f_tt2l); names.push_back("tt+2l fakeLep");
  samples.push_back(f_zinv); names.push_back("zinv fakeLep");
  samples.push_back(f_st); names.push_back("st fakeLep");
  samples.push_back(f_tt1l); names.push_back("tt+1l fakeLep");
  samples.push_back(f_wjets); names.push_back("wjets fakeLep");
  samples.push_back(f_tt2l); names.push_back("tt+2l");
  samples.push_back(f_st); names.push_back("st");
  //samples.push_back(f_ttbar); names.push_back("top");
  samples.push_back(f_tt1l); names.push_back("tt+1l");
  samples.push_back(f_wjets); names.push_back("wjets");
  // samples.push_back(f_data); names.push_back("data");

  // ----------------------------------------
  //  plots definitions
  // ----------------------------------------

  float scalesig = -1.;
  //float scalesig = 50.;
  bool printplots = true;
  bool doRatio = true;
  bool scaleBGtoData = false;

  if(printplots){
    TIter it(f_wjets->GetListOfKeys());
    TKey* k;
    std::string cr_skip = "cr";
    std::string sr_skip = "sr";
    while ((k = (TKey *)it())) {
      //if (strncmp (k->GetTitle(), cr_skip.c_str(), cr_skip.length()) == 0) continue; //skip control regions
      //if (strncmp (k->GetTitle(), sr_skip.c_str(), sr_skip.length()) == 0) continue; //skip signal regions and srbase
      std::string dir_name = k->GetTitle();
      //if(dir_name != "srsoftlbase") continue; //to do only this dir
      if(dir_name != "crsoftlmubase" && dir_name != "crsoftlelbase") continue; //to do only this dir

      makePlot( samples , names , dir_name , "h_leppt" , "Lepton p_{T} [GeV]" , "Events / 5 GeV" , 200 , 500 , 1 , false , printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_softlepht"  , "H_{T} [GeV]" , "Events / 25 GeV" , 0 , 2000 , 1 , false, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_mt2" , "M_{T2} [GeV]" , "Events / 10 GeV" , 0 , 1000 , 1 , false, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_softlepmt2" , "M_{T2} [GeV]" , "Events / 10 GeV" , 0 , 1000 , 1 , false, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_met"  , "E_{T}^{miss} [GeV]" , "Events / 10 GeV" , 0 , 800 , 1 , false, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_nJet30" , "N(jets)" , "Events" , 0 , 15 , 1 , false, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_nBJet20" , "N(b jets)" , "Events" , 0 , 6 , 1 , false, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_J0pt" , "p_{T}(jet1) [GeV]" , "Events / 20 GeV" , 0 , 1000 , 1 , false , printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_J1pt" , "p_{T}(jet2) [GeV]" , "Events / 20 GeV" , 0 , 1000 , 1 , false , printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_mt" , "M_{T} [GeV]" , "Events / 5 GeV" , 0 , 500 , 5 , false , printplots, scalesig, doRatio, scaleBGtoData );

      //log plots
      makePlot( samples , names , dir_name , "h_leppt" , "Lepton p_{T} [GeV]" , "Events / 5 GeV" , 200 , 500 , 1 , true , printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_softlepht"  , "H_{T} [GeV]" , "Events / 25 GeV" , 0 , 2000 , 1 , true, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_mt2" , "M_{T2} [GeV]" , "Events / 10 GeV" , 0 , 1000 , 1 , true, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_softlepmt2" , "M_{T2} [GeV]" , "Events / 10 GeV" , 0 , 1000 , 1 , true, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_met"  , "E_{T}^{miss} [GeV]" , "Events / 10 GeV" , 0 , 800 , 1 , true, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_nJet30" , "N(jets)" , "Events" , 0 , 15 , 1 , true, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_nBJet20" , "N(b jets)" , "Events" , 0 , 6 , 1 , true, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_J0pt" , "p_{T}(jet1) [GeV]" , "Events / 20 GeV" , 0 , 1000 , 1 , true , printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_J1pt" , "p_{T}(jet2) [GeV]" , "Events / 20 GeV" , 0 , 1000 , 1 , true , printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_mt" , "M_{T} [GeV]" , "Events / 5 GeV" , 0 , 500 , 5 , true , printplots, scalesig, doRatio, scaleBGtoData );

    }
  }
}

void plotMakerDoubleLepCR(){

  //  gROOT->LoadMacro("CMS_lumi.C");
  cmsText = "CMS Preliminary";
  cmsTextSize = 0.5;
  lumiTextSize = 0.4;
  writeExtraText = false;
  lumi_13TeV = "4 fb^{-1}";

  string input_dir = "../SoftLepLooper/output/softLepSR";
  
  // ----------------------------------------
  //  samples definition
  // ----------------------------------------

  // get input files
  
  TFile* f_tt1l = new TFile(Form("%s/ttsl.root",input_dir.c_str()));
  TFile* f_tt2l = new TFile(Form("%s/ttdl.root",input_dir.c_str())); 
  TFile* f_st = new TFile(Form("%s/singletop.root",input_dir.c_str())); 
  TFile* f_wjets = new TFile(Form("%s/wjets_ht.root",input_dir.c_str()));
  //TFile* f_qcd = new TFile(Form("%s/qcd_ht.root",input_dir.c_str()));
  TFile* f_zinv = new TFile(Form("%s/zinv_ht.root",input_dir.c_str()));
  TFile* f_dy = new TFile(Form("%s/dyjetsll.root",input_dir.c_str()));
  //TFile* f_data = new TFile(Form("%s/data_Run2015D.root",input_dir.c_str())); 

  vector<TFile*> samples;
  vector<string>  names;

  //samples.push_back(f_qcd); names.push_back("qcd");
  samples.push_back(f_st); names.push_back("st");
  samples.push_back(f_dy); names.push_back("dyjets");
  samples.push_back(f_wjets); names.push_back("wjets");
  samples.push_back(f_tt1l); names.push_back("tt+1l");
  samples.push_back(f_tt2l); names.push_back("tt+2l");
  samples.push_back(f_zinv); names.push_back("zinv");
  // samples.push_back(f_data); names.push_back("data");

  // ----------------------------------------
  //  plots definitions
  // ----------------------------------------

  float scalesig = -1.;
  //float scalesig = 50.;
  bool printplots = true;
  bool doRatio = true;
  bool scaleBGtoData = false;

  if(printplots){
    TIter it(f_tt2l->GetListOfKeys());
    TKey* k;
    std::string cr_skip = "cr";
    std::string sr_skip = "sr";
    while ((k = (TKey *)it())) {
      //if (strncmp (k->GetTitle(), cr_skip.c_str(), cr_skip.length()) == 0) continue; //skip control regions
      //if (strncmp (k->GetTitle(), sr_skip.c_str(), sr_skip.length()) == 0) continue; //skip signal regions and srbase
      std::string dir_name = k->GetTitle();
      //if(dir_name != "srsoftlbase") continue; //to do only this dir
      if(dir_name != "crdoublelbase") continue; //to do only this dir

      makePlot( samples , names , dir_name , "h_lowleppt" , "Lepton p_{T} [GeV]" , "Events / 1 GeV" , 0 , 30 , 1 , false , printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_highleppt" , "Lepton p_{T} [GeV]" , "Events / 5 GeV" , 0 , 500 , 1 , false , printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_ht"  , "H_{T} [GeV]" , "Events / 25 GeV" , 0 , 2000 , 1 , false, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_met"  , "E_{T}^{miss} [GeV]" , "Events / 10 GeV" , 0 , 800 , 1 , false, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_nJet30" , "N(jets)" , "Events" , 0 , 15 , 1 , false, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_nBJet20" , "N(b jets)" , "Events" , 0 , 6 , 1 , false, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_J0pt" , "p_{T}(jet1) [GeV]" , "Events / 20 GeV" , 0 , 1000 , 1 , false , printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_J1pt" , "p_{T}(jet2) [GeV]" , "Events / 20 GeV" , 0 , 1000 , 1 , false , printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_mt" , "M_{T} [GeV]" , "Events / 5 GeV" , 0 , 500 , 5 , false , printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_type" , "type" , "Events" , 0 , 3 , 1 , false , printplots, scalesig, doRatio, scaleBGtoData );

      //log plots
      makePlot( samples , names , dir_name , "h_lowleppt" , "Lepton p_{T} [GeV]" , "Events / 1 GeV" , 0 , 30 , 1 , true , printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_highleppt" , "Lepton p_{T} [GeV]" , "Events / 5 GeV" , 0 , 500 , 1 , true , printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_ht"  , "H_{T} [GeV]" , "Events / 25 GeV" , 0 , 2000 , 1 , true, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_met"  , "E_{T}^{miss} [GeV]" , "Events / 10 GeV" , 0 , 800 , 1 , true, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_nJet30" , "N(jets)" , "Events" , 0 , 15 , 1 , true, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_nBJet20" , "N(b jets)" , "Events" , 0 , 6 , 1 , true, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_J0pt" , "p_{T}(jet1) [GeV]" , "Events / 20 GeV" , 0 , 1000 , 1 , true , printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_J1pt" , "p_{T}(jet2) [GeV]" , "Events / 20 GeV" , 0 , 1000 , 1 , true , printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_mt" , "M_{T} [GeV]" , "Events / 5 GeV" , 0 , 500 , 5 , true , printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_type" , "type" , "Events" , 0 , 3 , 1 , true , printplots, scalesig, doRatio, scaleBGtoData );



    }
  }
}

//_______________________________________________________________________________
void plotMakerCRSL(){

  //  gROOT->LoadMacro("CMS_lumi.C");
  cmsText = "CMS Preliminary";
  cmsTextSize = 0.5;
  lumiTextSize = 0.4;
  writeExtraText = false;
  lumi_13TeV = "1.26 fb^{-1}";

//  string input_dir = "/home/olivito/cms3/MT2Analysis/MT2looper/output/V00-01-05_25ns_json_246908-256869_Summer15_25nsV2_skim_107pb_mt2gt100/";
  string input_dir = "/Users/giovannizevidellaporta/UCSD/MT2/Zinvisible/MT2babies/V00-01-05_25ns_json_246908-257599_skimV3";

  
  // ----------------------------------------
  //  samples definition
  // ----------------------------------------

  // get input files

  TFile* f_ttbar = new TFile(Form("%s/top.root",input_dir.c_str())); //hadd'ing of ttbar, ttw, ttz, tth, singletop
  TFile* f_wjets = new TFile(Form("%s/wjets_ht.root",input_dir.c_str()));
  //TFile* f_qcd = new TFile(Form("%s/qcd_ht.root",input_dir.c_str()));
  TFile* f_data = new TFile(Form("%s/data_Run2015D.root",input_dir.c_str())); 

  vector<TFile*> samples;
  vector<string>  names;

  //samples.push_back(f_qcd); names.push_back("qcd");
  samples.push_back(f_wjets); names.push_back("wjets");
  samples.push_back(f_ttbar); names.push_back("top");
  samples.push_back(f_data); names.push_back("data");

  // ----------------------------------------
  //  plots definitions
  // ----------------------------------------

  float scalesig = -1.;
  //float scalesig = 50.;
  bool printplots = true;
  bool doRatio = true;
  bool scaleBGtoData = false;

  if(printplots){
    TIter it(f_ttbar->GetListOfKeys());
    TKey* k;
    std::string cr_skip = "cr";
    std::string sr_skip = "sr";
    while ((k = (TKey *)it())) {
      //if (strncmp (k->GetTitle(), cr_skip.c_str(), cr_skip.length()) == 0) continue; //skip control regions
      //if (strncmp (k->GetTitle(), sr_skip.c_str(), sr_skip.length()) == 0) continue; //skip signal regions and srbase
      std::string dir_name = k->GetTitle();
      //if(dir_name != "crslbase") continue; //to do only this dir
      if(dir_name != "crslmubase" && dir_name != "crslelbase") continue; //to do only this dir
      //if(dir_name != "crslttbar" && dir_name != "crslwjets") continue; //to do only this dir

      makePlot( samples , names , dir_name , "h_leppt" , "Lepton p_{T} [GeV]" , "Events / 20 GeV" , 0 , 500 , 4 , true , printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_ht"  , "H_{T} [GeV]" , "Events / 50 GeV" , 0 , 2000 , 2 , false, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_mt2" , "M_{T2} [GeV]" , "Events / 50 GeV" , 100 , 1000 , 5 , false, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_met"  , "E_{T}^{miss} [GeV]" , "Events / 50 GeV" , 0 , 800 , 5 , false, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_nJet30" , "N(jets)" , "Events" , 0 , 15 , 1 , false, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_nBJet20" , "N(b jets)" , "Events" , 0 , 6 , 1 , false, printplots, scalesig, doRatio, scaleBGtoData );

    }
  }
}

//_______________________________________________________________________________
void plotMakerSoft(){
  //tableMakerZinvCR(); return;
  //plotMakerDY(); return;
  //plotMakerZllMT(); return;
  //plotMakerGJets(); return;
  //plotMakerRemovedLep(); return;
  //plotMakerCRSL(); return;
  //plotMakerSoftLepSR(); return;
  //plotMakerSoftLepCR(); return;
  plotMakerDoubleLepCR(); return;

  //  gROOT->LoadMacro("CMS_lumi.C");
  cmsText = "CMS Preliminary";
  cmsTextSize = 0.5;
  lumiTextSize = 0.4;
  writeExtraText = false;
  //lumi_13TeV = "42 pb^{-1}";
  lumi_13TeV = "8.6 pb^{-1}";

  string input_dir = "/home/olivito/cms3/MT2Analysis/MT2looper/output/V00-01-04_25ns_skim_8p6pb_mt2gt100_metfilt/";

  
  // ----------------------------------------
  //  samples definition
  // ----------------------------------------

  // get input files

  TFile* f_ttbar = new TFile(Form("%s/ttall_mg_lo.root",input_dir.c_str()));
  //TFile* f_ttbar = new TFile(Form("%s/ttall_powheg.root",input_dir.c_str()));
  //TFile* f_ttbar = new TFile(Form("%s/top.root",input_dir.c_str())); //hadd'ing of ttbar, ttw, ttz, tth, singletop
  //TFile* f_zjets = new TFile(Form("%s/zjets_amcatnlo.root",input_dir.c_str()));
  // TFile* f_dyjets = new TFile(Form("%s/dyjetsll_ht.root",input_dir.c_str()));
  //TFile* f_zinv = new TFile(Form("%s/zinv_ht.root",input_dir.c_str()));
  // TFile* f_gjet = new TFile(Form("%s/gjet_ht.root",input_dir.c_str()));
  //TFile* f_wjets = new TFile(Form("%s/wjets_ht.root",input_dir.c_str()));
  TFile* f_wjets = new TFile(Form("%s/wjets_amcatnlo.root",input_dir.c_str()));
  //TFile* f_qcd = new TFile(Form("%s/qcd_pt.root",input_dir.c_str()));
  // TFile* f_tth = new TFile(Form("%s/tth.root",input_dir.c_str()));
  // TFile* f_ttw = new TFile(Form("%s/ttwjets.root",input_dir.c_str()));
  // TFile* f_ttz = new TFile(Form("%s/ttzjets.root",input_dir.c_str()));
  // TFile* f_singletop = new TFile(Form("%s/singletop.root",input_dir.c_str()));
  // //  TFile* f_singletop_wrong = new TFile(Form("%s/singletop_wrong_scale1fb.root",input_dir.c_str()));
  //TFile* f_fakedata = new TFile(Form("%s/lostlep.root",input_dir.c_str())); //hadd'ing of ttbar, ttw, ttz, tth, singletop, wjets
  //  TFile* f_fakedata = new TFile(Form("%s/fakedata.root",input_dir.c_str())); 
  //TFile* f_data = new TFile(Form("%s/data_Run2015B.root",input_dir.c_str())); 
  TFile* f_data = new TFile(Form("%s/data_Run2015C.root",input_dir.c_str())); 

  // TFile* f_T1tttt_1500_100 = new TFile(Form("%s/T1tttt_1500_100.root",input_dir.c_str()));
  // TFile* f_T1tttt_1200_800 = new TFile(Form("%s/T1tttt_1200_800.root",input_dir.c_str()));
  // TFile* f_T1bbbb_1500_100 = new TFile(Form("%s/T1bbbb_1500_100.root",input_dir.c_str()));
  // TFile* f_T1bbbb_1000_900 = new TFile(Form("%s/T1bbbb_1000_900.root",input_dir.c_str()));
  // TFile* f_T1qqqq_1400_100 = new TFile(Form("%s/T1qqqq_1400_100.root",input_dir.c_str()));
  // TFile* f_T1qqqq_1000_800 = new TFile(Form("%s/T1qqqq_1000_800.root",input_dir.c_str()));

  // TFile* f_T2tt_425_325 = new TFile(Form("%s/T2tt_425_325.root",input_dir.c_str()));
  // TFile* f_T2tt_500_325 = new TFile(Form("%s/T2tt_500_325.root",input_dir.c_str()));
  // TFile* f_T2tt_650_325 = new TFile(Form("%s/T2tt_650_325.root",input_dir.c_str()));
  // TFile* f_T2tt_850_100 = new TFile(Form("%s/T2tt_850_100.root",input_dir.c_str()));

  // TFile* f_T2bb_900_100 = new TFile(Form("%s/T2bb_900_100.root",input_dir.c_str()));
  // TFile* f_T2bb_600_580 = new TFile(Form("%s/T2bb_600_580.root",input_dir.c_str()));
  // TFile* f_T2qq_1200_100 = new TFile(Form("%s/T2qq_1200_100.root",input_dir.c_str()));
  // TFile* f_T2qq_600_550 = new TFile(Form("%s/T2qq_600_550.root",input_dir.c_str()));

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

  //samples.push_back(f_qcd); names.push_back("qcd");
  //samples.push_back(f_wjets); names.push_back("wjets");
  //samples.push_back(f_zinv);  names.push_back("zinv");
  //samples.push_back(f_gjet);  names.push_back("gjet");
  // samples.push_back(f_tth); names.push_back("tth");
  // samples.push_back(f_ttw); names.push_back("ttw");
  // samples.push_back(f_ttz); names.push_back("ttz");
  // samples.push_back(f_singletop); names.push_back("singletop");
  //  samples.push_back(f_ttbar); names.push_back("ttbar");
  samples.push_back(f_wjets); names.push_back("wjets");
  samples.push_back(f_ttbar); names.push_back("ttbar");
  //samples.push_back(f_ttbar); names.push_back("top");
  //samples.push_back(f_zjets); names.push_back("zjets");
  //samples.push_back(f_dyjets); names.push_back("dyjets");
  samples.push_back(f_data); names.push_back("data");

  // samples.push_back(f_singletop_wrong); names.push_back("singletop_wrong");
  // samples.push_back(f_singletop); names.push_back("singletop");

  // samples.push_back(f_T1tttt_1500_100); names.push_back("sig_T1tttt_1500_100");
  // samples.push_back(f_T1tttt_1200_800); names.push_back("sig_T1tttt_1200_800");
  // samples.push_back(f_T1bbbb_1500_100); names.push_back("sig_T1bbbb_1500_100");
  // samples.push_back(f_T1bbbb_1000_900); names.push_back("sig_T1bbbb_1000_900");
  // samples.push_back(f_T1qqqq_1400_100); names.push_back("sig_T1qqqq_1400_100");
  // samples.push_back(f_T1qqqq_1000_800); names.push_back("sig_T1qqqq_1000_800");

  // samples.push_back(f_T2tt_850_100); names.push_back("sig_T2tt_850_100");
  // samples.push_back(f_T2tt_650_325); names.push_back("sig_T2tt_650_325");
  // samples.push_back(f_T2tt_500_325); names.push_back("sig_T2tt_500_325");
  // samples.push_back(f_T2tt_425_325); names.push_back("sig_T2tt_425_325");

  // samples.push_back(f_T2bb_900_100); names.push_back("sig_T2bb_900_100");
  // samples.push_back(f_T2bb_600_580); names.push_back("sig_T2bb_600_580");
  // samples.push_back(f_T2qq_1200_100); names.push_back("sig_T2qq_1200_100");
  // samples.push_back(f_T2qq_600_550); names.push_back("sig_T2qq_600_550");

  // ----------------------------------------
  //  plots definitions
  // ----------------------------------------

  float scalesig = -1.;
  //float scalesig = 50.;
  bool printplots = false;
  bool doRatio = true;
  bool scaleBGtoData = false;

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
      if(dir_name != "srbase") continue; //to do only this dir
      //if(dir_name != "sr1H") continue; //for testing

      makePlot( samples , names , dir_name , "h_ht"  , "H_{T} [GeV]" , "Events / 50 GeV" , 0 , 1500 , 2 , false, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_mt2" , "M_{T2} [GeV]" , "Events / 50 GeV" , 100 , 1000 , 5 , false, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_met"  , "E_{T}^{miss} [GeV]" , "Events / 50 GeV" , 0 , 800 , 5 , false, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_nlepveto" , "N(leptons)" , "Events" , 0 , 10 , 1 , false, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_nJet30" , "N(jets)" , "Events" , 0 , 15 , 1 , false, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_nBJet20" , "N(b jets)" , "Events" , 0 , 6 , 1 , false, printplots, scalesig, doRatio, scaleBGtoData );
      makePlot( samples , names , dir_name , "h_mt2bins" , "M_{T2} [GeV]" , "Events / Bin" , 200 , 1500 , 1 , true, printplots, scalesig, doRatio, scaleBGtoData );
      //makePlot( samples , names , dir_name , "h_nJet30Eta3" , "N(jets, |#eta| > 3.0)" , "Events" , 0 , 5 , 1 , false, printplots, scalesig, doRatio, scaleBGtoData );

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

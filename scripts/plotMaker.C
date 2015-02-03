#include <iostream>
#include <utility>
#include <vector>

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
int getColor(const string& sample) {
  if (sample.find("ttbar") != string::npos) return kBlue;
  if (sample.find("wjets") != string::npos) return kGreen+1;
  if (sample.find("zinv") != string::npos) return kGreen-1;
  if (sample.find("qcd") != string::npos) return kYellow+1;
  if (sample.find("T1tttt") != string::npos) return kRed;
  if (sample.find("T1bbbb") != string::npos) return kMagenta;
  if (sample.find("T1qqqq") != string::npos) return kOrange;

  cout << "getColor: WARNING: didn't recognize sample: " << sample << endl;
  return kBlack;
}

//_______________________________________________________________________________
string getLegendName(const string& sample) {
  if (sample.find("ttbar") != string::npos) return "Top";
  if (sample.find("wjets") != string::npos) return "W+jets";
  if (sample.find("zinv") != string::npos) return "Z+jets";
  if (sample.find("qcd") != string::npos) return "QCD";
  if (sample.find("T1tttt_1500_100") != string::npos) return "T1tttt 1500, 100";
  if (sample.find("T1tttt_1200_800") != string::npos) return "T1tttt 1200, 800";
  if (sample.find("T1bbbb_1000_900") != string::npos) return "T1bbbb 1000, 900";
  if (sample.find("T1bbbb_1500_100") != string::npos) return "T1bbbb 1500, 100";
  if (sample.find("T1qqqq_1400_100") != string::npos) return "T1qqqq 1400, 100";
  if (sample.find("T1qqqq_1000_800") != string::npos) return "T1qqqq 1000, 800";
  if (sample.find("T2tt_425_325") != string::npos) return "T2tt 425, 325";
  if (sample.find("T2tt_500_325") != string::npos) return "T2tt 500, 325";
  if (sample.find("T2tt_650_325") != string::npos) return "T2tt 650, 325";
  if (sample.find("T2tt_850_100") != string::npos) return "T2tt 850, 100";
  if (sample.find("T2bb_900_100") != string::npos) return "T2bb 900, 100";
  if (sample.find("T2bb_600_580") != string::npos) return "T2bb 600, 580";
  if (sample.find("T2qq_1200_100") != string::npos) return "T2qq 1200, 100";
  if (sample.find("T2qq_600_550") != string::npos) return "T2qq 600, 550";

  cout << "getLegendName: WARNING: didn't recognize sample: " << sample << endl;
  return sample;
}

//_______________________________________________________________________________
string getTableName(const string& sample) {
  if (sample.find("ttbar") != string::npos) return "Top";
  if (sample.find("wjets_ht100to200") != string::npos) return "W+jets HT100to200";
  if (sample.find("wjets_ht200to400") != string::npos) return "W+jets HT200to400";
  if (sample.find("wjets_ht400to600") != string::npos) return "W+jets HT400to600";
  if (sample.find("wjets_ht600toInf") != string::npos) return "W+jets HT600toInf";
  if (sample.find("wjets") != string::npos) return "W+jets";
  if (sample.find("zinv_ht100to200") != string::npos) return "Z+jets HT100to200";
  if (sample.find("zinv_ht200to400") != string::npos) return "Z+jets HT200to400";
  if (sample.find("zinv_ht400to600") != string::npos) return "Z+jets HT400to600";
  if (sample.find("zinv_ht600toInf") != string::npos) return "Z+jets HT600toInf";
  if (sample.find("zinv") != string::npos) return "Z+jets";
  if (sample.find("qcd") != string::npos) return "QCD";
  if (sample.find("T1tttt_1500_100") != string::npos) return "T1tttt 1500, 100";
  if (sample.find("T1tttt_1200_800") != string::npos) return "T1tttt 1200, 800";
  if (sample.find("T1bbbb_1000_900") != string::npos) return "T1bbbb 1000, 900";
  if (sample.find("T1bbbb_1500_100") != string::npos) return "T1bbbb 1500, 100";
  if (sample.find("T1qqqq_1400_100") != string::npos) return "T1qqqq 1400, 100";
  if (sample.find("T1qqqq_1000_800") != string::npos) return "T1qqqq 1000, 800";
  if (sample.find("T2tt_425_325") != string::npos) return "T2tt 425, 325";
  if (sample.find("T2tt_500_325") != string::npos) return "T2tt 500, 325";
  if (sample.find("T2tt_650_325") != string::npos) return "T2tt 650, 325";
  if (sample.find("T2tt_850_100") != string::npos) return "T2tt 850, 100";
  if (sample.find("T2bb_900_100") != string::npos) return "T2bb 900, 100";
  if (sample.find("T2bb_600_580") != string::npos) return "T2bb 600, 580";
  if (sample.find("T2qq_1200_100") != string::npos) return "T2qq 1200, 100";
  if (sample.find("T2qq_600_550") != string::npos) return "T2qq 600, 550";

  cout << "getTableName: WARNING: didn't recognize sample: " << sample << endl;
  return sample;
}

//_______________________________________________________________________________
string getRegionName(const string& dir) {
  if (dir.find("sr10") != string::npos) return "$\\geq$2j, $\\geq$3b, high minMT,MT2";
  if (dir.find("sr1") != string::npos) return "2-3j, 0b";
  if (dir.find("sr2") != string::npos) return "2-3j, 1b";
  if (dir.find("sr3") != string::npos) return "2-3j, 2b, low minMT,MT2";
  if (dir.find("sr4") != string::npos) return "2-3j, 2b, high minMT,MT2";
  if (dir.find("sr5") != string::npos) return "$\\geq$4j, 0b";
  if (dir.find("sr6") != string::npos) return "$\\geq$4j, 1b";
  if (dir.find("sr7") != string::npos) return "$\\geq$4j, 2b, low minMT,MT2";
  if (dir.find("sr8") != string::npos) return "$\\geq$4j, 2b, high minMT,MT2";
  if (dir.find("sr9") != string::npos) return "$\\geq$2j, $\\geq$3b, low minMT,MT2";
  if (dir.find("sr") != string::npos) return "Signal";
  if (dir.find("nomt") != string::npos) return "CRSL, No $M_{T}$ Cut";
  if (dir.find("crsl") != string::npos) return "CRSL";

  cout << "getRegionName: WARNING: didn't recognize dir: " << dir << endl;
  return dir;
}

//_______________________________________________________________________________
TCanvas* makePlot( const vector<TFile*>& samples , const vector<string>& names , const string& histdir , const string& histname , const string& xtitle , const string& ytitle , float xmin , float xmax , int rebin = 1 , bool logplot = true, bool printplot = false, float scalesig = -1. ) {

  cout << "-- plotting histdir: " << histdir << ", histname: " << histname << endl;

  TString canvas_name = Form("c_%s_%s",histdir.c_str(),histname.c_str());
  TCanvas* can = new TCanvas(canvas_name,canvas_name);
  can->cd();
  if (logplot) can->SetLogy();
  gPad->SetRightMargin(0.05);
  gPad->Modified();

  THStack* t = new THStack(Form("stack_%s_%s",histdir.c_str(),histname.c_str()),Form("stack_%s_%s",histdir.c_str(),histname.c_str()));
  TH1D* h_bgtot = 0;

  TLegend* leg = new TLegend(0.55,0.6,0.85,0.92);
  leg->SetFillColor(0);
  leg->SetBorderSize(0);
  leg->SetTextSize(0.032);

  // to make legend
  vector<TH1D*> bg_hists;
  vector<string> bg_names;

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

  float ymax = h_bgtot->GetMaximum();
  if( logplot ) ymax*=10;
  else          ymax*=1.1;
  float ymin = 0.1;

  TH2F* h_axes = new TH2F(Form("%s_%s_axes",histname.c_str(),histdir.c_str()),"",100,xmin,xmax,100,ymin,ymax);
  h_axes->GetXaxis()->SetTitle(xtitle.c_str());
  h_axes->GetXaxis()->SetLabelSize(0.04);
  h_axes->GetYaxis()->SetTitle(ytitle.c_str());
  h_axes->GetYaxis()->SetLabelSize(0.04);
  h_axes->Draw();

  t->Draw("hist same");

  for( unsigned int i = 0 ; i < n ; ++i ){
    if( !TString(names.at(i)).Contains("sig") ) continue;
    TString fullhistname = Form("%s/%s",histdir.c_str(),histname.c_str());
    if (histdir.size() == 0) fullhistname = TString(histname);
    TString newhistname = Form("%s_%s_%s",histname.c_str(),histdir.c_str(),names.at(i).c_str());
    TH1D* h_temp = (TH1D*) samples.at(i)->Get(fullhistname);
    if (h_temp == 0) continue;
    TH1D* h = (TH1D*) h_temp->Clone(newhistname);
    //    h->Sumw2();
    h->SetLineColor(getColor(names.at(i)));
    h->SetLineWidth(2);
    if (rebin > 1) h->Rebin(rebin);
    if (scalesig > 0.) h->Scale(scalesig);
    h->Draw("hist same");
    TString legend_name = getLegendName(names.at(i));
    if (scalesig > 0.) legend_name += Form(" x %.0f",scalesig);
    leg->AddEntry(h,legend_name,"l");
  }

  leg->Draw();
  h_axes->Draw("axissame");

  CMS_lumi( can, iPeriod, iPos );

  gPad->Modified();

  if( printplot ) {
    can->Print(Form("plots/%s.pdf",canvas_name.Data()));
    can->Print(Form("plots/%s.eps",canvas_name.Data()));
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

  cout << endl << "\\hline" << endl
       << "Sample";

  // header
  for (unsigned int idir = 0; idir < ndirs; ++idir) {
    cout << " & " << getRegionName(dirs.at(idir));
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
	  bgerr.at(idir) = sqrt(bgerr.at(idir)**2 + err**2);
	}
	// last bin: include overflow
	else if (mt2bin == h->GetXaxis()->GetNbins()) {
	  yield = h->IntegralAndError(mt2bin,-1,err);
	  bgtot.at(idir) += yield;
	  bgerr.at(idir) = sqrt(bgerr.at(idir)**2 + err**2);
	}
	// single bin, not last bin
	else {
	  yield = h->GetBinContent(mt2bin);
	  err = h->GetBinError(mt2bin);
	  bgtot.at(idir) += yield;
	  bgerr.at(idir) = sqrt(bgerr.at(idir)**2 + err**2);
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

  for( int i = 0 ; i < n ; i++ ){
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

  cout << endl;
  return;
}

//_______________________________________________________________________________
void plotMaker(){

  gROOT->LoadMacro("CMS_lumi.C");
  cmsText = "CMS Simulation";
  writeExtraText = false;
  lumi_13TeV = "4 fb^{-1}";

  string input_dir = "/home/olivito/cms3/MT2Analysis/MT2looper/output/V00-00-08_4fb/";

  // ----------------------------------------
  //  samples definition
  // ----------------------------------------

  // get input files

  TFile* f_ttbar = new TFile(Form("%s/ttall_msdecays.root",input_dir.c_str()));
  TFile* f_zinv = new TFile(Form("%s/zinv_ht.root",input_dir.c_str()));
  TFile* f_wjets = new TFile(Form("%s/wjets_ht.root",input_dir.c_str()));
  TFile* f_qcd = new TFile(Form("%s/qcd_ht.root",input_dir.c_str()));

  // TFile* f_T1tttt_1500_100 = new TFile(Form("%s/T1tttt_1500_100.root",input_dir.c_str()));
  // TFile* f_T1tttt_1200_800 = new TFile(Form("%s/T1tttt_1200_800.root",input_dir.c_str()));
  // TFile* f_T1bbbb_1000_900 = new TFile(Form("%s/T1bbbb_1000_900.root",input_dir.c_str()));
  // TFile* f_T1bbbb_1500_100 = new TFile(Form("%s/T1bbbb_1500_100.root",input_dir.c_str()));
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

  // TFile* f_zinv_ht100to200 = new TFile(Form("%s/zinv_ht100to200.root",input_dir.c_str()));
  // TFile* f_zinv_ht200to400 = new TFile(Form("%s/zinv_ht200to400.root",input_dir.c_str()));
  // TFile* f_zinv_ht400to600 = new TFile(Form("%s/zinv_ht400to600.root",input_dir.c_str()));
  // TFile* f_zinv_ht600toInf = new TFile(Form("%s/zinv_ht600toInf.root",input_dir.c_str()));

  // TFile* f_wjets_ht100to200 = new TFile(Form("%s/wjets_ht100to200.root",input_dir.c_str()));
  // TFile* f_wjets_ht200to400 = new TFile(Form("%s/wjets_ht200to400.root",input_dir.c_str()));
  // TFile* f_wjets_ht400to600 = new TFile(Form("%s/wjets_ht400to600.root",input_dir.c_str()));
  // TFile* f_wjets_ht600toInf = new TFile(Form("%s/wjets_ht600toInf.root",input_dir.c_str()));

  vector<TFile*> samples;
  vector<string>  names;

  samples.push_back(f_qcd); names.push_back("qcd");
  samples.push_back(f_wjets); names.push_back("wjets");
  samples.push_back(f_zinv);  names.push_back("zinv");
  samples.push_back(f_ttbar); names.push_back("ttbar");

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

  // samples.push_back(f_wjets_ht600toInf);  names.push_back("wjets_ht600toInf");
  // samples.push_back(f_wjets_ht400to600);  names.push_back("wjets_ht400to600");
  // samples.push_back(f_wjets_ht200to400);  names.push_back("wjets_ht200to400");
  // samples.push_back(f_wjets_ht100to200);  names.push_back("wjets_ht100to200");

  // samples.push_back(f_zinv_ht600toInf);  names.push_back("zinv_ht600toInf");
  // samples.push_back(f_zinv_ht400to600);  names.push_back("zinv_ht400to600");
  // samples.push_back(f_zinv_ht200to400);  names.push_back("zinv_ht200to400");
  // samples.push_back(f_zinv_ht100to200);  names.push_back("zinv_ht100to200");

  // ----------------------------------------
  //  plots definitions
  // ----------------------------------------

  float scalesig = 50.;
  bool printplots = false;

  // makePlot( samples , names , "" , "h_SignalRegion"  , "Signal Region" , "Events" , 0 , 100 , 1 , true, printplots );
  // makePlot( samples , names , "nocut" , "h_ht"  , "H_{T} [GeV]" , "Events / 25 GeV" , 0 , 2000 , 1 , true, printplots );
  // makePlot( samples , names , "nocut" , "h_mt2" , "M_{T2} [GeV]" , "Events / 10 GeV" , 0 , 800 , 1 , true, printplots );
  // makePlot( samples , names , "nocut" , "h_met"  , "E_{T}^{miss} [GeV]" , "Events / 10 GeV" , 0 , 800 , 1 , true, printplots );
  // makePlot( samples , names , "nocut" , "h_nlepveto" , "N(leptons)" , "Events" , 0 , 10 , 1 , false, printplots );
  // makePlot( samples , names , "nocut" , "h_nJet40" , "N(jets)" , "Events" , 0 , 15 , 1 , false, printplots );
  // makePlot( samples , names , "nocut" , "h_nBJet40" , "N(b jets)" , "Events" , 0 , 6 , 1 , false, printplots );
  // makePlot( samples , names , "nocut" , "h_minMTBMet"  , "min M_{T}(b,MET) [GeV]" , "Events / 10 GeV" , 0 , 800 , 1 , true, printplots );

  makePlot( samples , names , "srbase" , "h_ht"  , "H_{T} [GeV]" , "Events / 25 GeV" , 0 , 2000 , 1 , true, printplots, scalesig );
  makePlot( samples , names , "srbase" , "h_mt2" , "M_{T2} [GeV]" , "Events / 10 GeV" , 0 , 1000 , 1 , true, printplots, scalesig );
  makePlot( samples , names , "srbase" , "h_nJet40" , "N(jets)" , "Events" , 0 , 15 , 1 , false, printplots, scalesig );
  makePlot( samples , names , "srbase" , "h_nBJet40" , "N(b jets)" , "Events" , 0 , 6 , 1 , false, printplots, scalesig );
  makePlot( samples , names , "srbase" , "h_minMTBMet"  , "min M_{T}(b,MET) [GeV]" , "Events / 10 GeV" , 0 , 800 , 1 , false, printplots, scalesig );

  // makePlot( samples , names , "crslbase" , "h_ht"  , "H_{T} [GeV]" , "Events / 25 GeV" , 0 , 2000 , 1 , true, printplots );
  // makePlot( samples , names , "crslbase" , "h_mt2" , "M_{T2} [GeV]" , "Events / 10 GeV" , 0 , 1000 , 1 , true, printplots );
  // makePlot( samples , names , "crslbase" , "h_met"  , "E_{T}^{miss} [GeV]" , "Events / 10 GeV" , 0 , 800 , 1 , true, printplots );
  // makePlot( samples , names , "crslbase" , "h_nJet40" , "N(jets)" , "Events" , 0 , 15 , 1 , false, printplots );
  // makePlot( samples , names , "crslbase" , "h_nBJet40" , "N(b jets)" , "Events" , 0 , 6 , 1 , false, printplots );

  // makePlot( samples , names , "crslw" , "h_ht"  , "H_{T} [GeV]" , "Events / 25 GeV" , 0 , 2000 , 1 , true, printplots );
  // makePlot( samples , names , "crslw" , "h_mt2" , "M_{T2} [GeV]" , "Events / 10 GeV" , 0 , 1000 , 1 , true, printplots );
  // makePlot( samples , names , "crslw" , "h_met"  , "E_{T}^{miss} [GeV]" , "Events / 10 GeV" , 0 , 800 , 1 , true, printplots );
  // makePlot( samples , names , "crslw" , "h_nJet40" , "N(jets)" , "Events" , 0 , 15 , 1 , false, printplots );
  // makePlot( samples , names , "crslw" , "h_nBJet40" , "N(b jets)" , "Events" , 0 , 6 , 1 , false, printplots );

  // makePlot( samples , names , "crsltt" , "h_ht"  , "H_{T} [GeV]" , "Events / 25 GeV" , 0 , 2000 , 1 , true, printplots );
  // makePlot( samples , names , "crsltt" , "h_mt2" , "M_{T2} [GeV]" , "Events / 10 GeV" , 0 , 1000 , 1 , true, printplots );
  // makePlot( samples , names , "crsltt" , "h_met"  , "E_{T}^{miss} [GeV]" , "Events / 10 GeV" , 0 , 800 , 1 , true, printplots );
  // makePlot( samples , names , "crsltt" , "h_nJet40" , "N(jets)" , "Events" , 0 , 15 , 1 , false, printplots );
  // makePlot( samples , names , "crsltt" , "h_nBJet40" , "N(b jets)" , "Events" , 0 , 6 , 1 , false, printplots );
  // makePlot( samples , names , "crsltt" , "h_minMTBMet"  , "min M_{T}(b,MET) [GeV]" , "Events / 10 GeV" , 0 , 800 , 1 , true, printplots );

  // ----------------------------------------
  //  tables definitions
  // ----------------------------------------

  //  vector<string> dirs;
  // //  dirs.push_back("nocut");
  // dirs.push_back("crslbase");
  // dirs.push_back("crslelbase");
  // dirs.push_back("crslmubase");
  // dirs.push_back("crslhadbase");
  // dirs.push_back("crslw");
  // dirs.push_back("crsltt");

  // printTable(samples, names, dirs);

  // dirs.clear();
  // dirs.push_back("srbase");
  // dirs.push_back("srL");
  // dirs.push_back("srM");
  // dirs.push_back("srH");

  // printTable(samples, names, dirs);

  // dirs.clear();
  // dirs.push_back("crslelw");
  // dirs.push_back("crsleltt");
  // dirs.push_back("crslmuw");
  // dirs.push_back("crslmutt");
  // dirs.push_back("crslhadw");
  // dirs.push_back("crslhadtt");

  // printTable(samples, names, dirs);

  // vector<string> dirsL;
  // dirsL.push_back("sr1L");
  // dirsL.push_back("sr2L");
  // dirsL.push_back("sr3L");
  // dirsL.push_back("sr4L");
  // dirsL.push_back("sr5L");
  // printTable(samples, names, dirsL);

  // dirsL.clear();
  // dirsL.push_back("sr6L");
  // dirsL.push_back("sr7L");
  // dirsL.push_back("sr8L");
  // dirsL.push_back("sr9L");
  // dirsL.push_back("sr10L");
  // printTable(samples, names, dirsL);

  // vector<string> dirsM;
  // dirsM.push_back("sr1M");
  // dirsM.push_back("sr2M");
  // dirsM.push_back("sr3M");
  // dirsM.push_back("sr4M");
  // dirsM.push_back("sr5M");
  // printTable(samples, names, dirsM);

  // dirsM.clear();
  // dirsM.push_back("sr6M");
  // dirsM.push_back("sr7M");
  // dirsM.push_back("sr8M");
  // dirsM.push_back("sr9M");
  // dirsM.push_back("sr10M");
  // printTable(samples, names, dirsM);

  // vector<string> dirsH;
  // dirsH.push_back("sr1H");
  // dirsH.push_back("sr2H");
  // dirsH.push_back("sr3H");
  // dirsH.push_back("sr4H");
  // dirsH.push_back("sr5H");
  // printTable(samples, names, dirsH);

  // dirsH.clear();
  // dirsH.push_back("sr6H");
  // dirsH.push_back("sr7H");
  // dirsH.push_back("sr8H");
  // dirsH.push_back("sr9H");
  // dirsH.push_back("sr10H");
  // printTable(samples, names, dirsH);

  // vector<string> dirsL;
  // dirsL.push_back("crsl1L");
  // dirsL.push_back("crsl2L");
  // dirsL.push_back("crsl3L4L");
  // dirsL.push_back("crsl5L");
  // dirsL.push_back("crsl6L");
  // dirsL.push_back("crsl7L8L");
  // dirsL.push_back("crsl9L10L");

  // vector<string> dirsM;
  // dirsM.push_back("crsl1M");
  // dirsM.push_back("crsl2M");
  // dirsM.push_back("crsl3M4M");
  // dirsM.push_back("crsl5M");
  // dirsM.push_back("crsl6M");
  // dirsM.push_back("crsl7M8M");
  // dirsM.push_back("crsl9M10M");

  // vector<string> dirsH;
  // dirsH.push_back("crsl1H");
  // dirsH.push_back("crsl2H");
  // dirsH.push_back("crsl3H4H");
  // dirsH.push_back("crsl5H");
  // dirsH.push_back("crsl6H");
  // dirsH.push_back("crsl7H8H");
  // dirsH.push_back("crsl9H10H");


  // printTable(samples, names, dirsH, 1);
  // printTable(samples, names, dirsH, 2);
  // printTable(samples, names, dirsH, 3);
  // printTable(samples, names, dirsH, 4);
  // printTable(samples, names, dirsH, 5);

  // // inclusive in mt2
  // printTable(samples, names, dirsL);
  // printTable(samples, names, dirsM);
  // printTable(samples, names, dirsH);

  // // mt2 bin 1
  // printTable(samples, names, dirsL, 1);
  // printTable(samples, names, dirsM, 1);
  // printTable(samples, names, dirsH, 1);

  // // mt2 bin 2
  // printTable(samples, names, dirsL, 2);
  // printTable(samples, names, dirsM, 2);
  // printTable(samples, names, dirsH, 2);

  // // mt2 bin 3
  // printTable(samples, names, dirsL, 3);
  // printTable(samples, names, dirsM, 3);
  // printTable(samples, names, dirsH, 3);

  // // mt2 bin 4
  // printTable(samples, names, dirsL, 4);
  // printTable(samples, names, dirsM, 4);
  // printTable(samples, names, dirsH, 4);

  // // mt2 bin 5
  // printTable(samples, names, dirsL, 5);
  // printTable(samples, names, dirsM, 5);
  // printTable(samples, names, dirsH, 5);


}

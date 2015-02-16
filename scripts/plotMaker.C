#include <iostream>
#include <utility>
#include <vector>
#include <string>


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

std::string toString(int in){
  stringstream ss;
  ss << in;
  return ss.str();
}

void ReplaceString(std::string& subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}


//_______________________________________________________________________________
int getColor(const string& sample) {
  if (sample.find("ttbar") != string::npos) return kBlue;
  if (sample.find("wjets") != string::npos) return kGreen+1;
  if (sample.find("zinv") != string::npos) return kGreen-1;
  if (sample.find("qcd") != string::npos) return kYellow+1;
  if (sample.find("T1tttt") != string::npos) return kRed;
  if (sample.find("T1bbbb") != string::npos) return kMagenta;
  if (sample.find("T1qqqq") != string::npos) return kOrange;
  if (sample.find("T2tt") != string::npos) return kCyan;
  if (sample.find("T2bb") != string::npos) return kMagenta+3;

  cout << "getColor: WARNING: didn't recognize sample: " << sample << endl;
  return kBlack;
}

//_______________________________________________________________________________
string getLegendName(const string& sample) {
  if (sample.find("ttbar") != string::npos) return "Top";
  if (sample.find("wjets") != string::npos) return "W+jets";
  if (sample.find("zinv") != string::npos) return "Z+jets";
  if (sample.find("gjet") != string::npos) return "Gamma+jets";
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
  if (sample.find("gjet") != string::npos) return "Gamma+jets";
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
string getRegionPlotLabel(const string& dir) {
  if (dir.find("sr10") != string::npos) return "#geq 2j, #geq 3b";
  if (dir.find("sr1") != string::npos) return "2-3j, 0b";
  if (dir.find("sr2") != string::npos) return "2-3j, 1b";
  if (dir.find("sr3") != string::npos) return "2-3j, 2b";
  if (dir.find("sr4") != string::npos) return "2-3j, 2b";
  if (dir.find("sr5") != string::npos) return "#geq 4j, 0b";
  if (dir.find("sr6") != string::npos) return "#geq 4j, 1b";
  if (dir.find("sr7") != string::npos) return "#geq 4j, 2b";
  if (dir.find("sr8") != string::npos) return "#geq 4j, 2b";
  if (dir.find("sr9") != string::npos) return "#geq 2j, #geq 3b";
  if (dir.find("srbase") != string::npos) return "#geq 2j";
  //  if (dir.find("crsl") != string::npos) return "CRSL";

  cout << "getRegionPlotLabel: WARNING: didn't recognize dir: " << dir << endl;
  return dir;
}

string getJetBJetPlotLabel(const TFile* f, std::string dir_str) {

  TString dir= TString(dir_str);

  TH1D* h_njets_LOW = (TH1D*) f->Get(dir+"/h_njets_LOW");
  TH1D* h_njets_UP = (TH1D*) f->Get(dir+"/h_njets_UP");
  int njets_LOW;
  int njets_UP;
  if(h_njets_LOW && h_njets_UP){
    njets_LOW = h_njets_LOW->GetBinContent(1);
    njets_UP = h_njets_UP->GetBinContent(1);
  }
  else{
    njets_LOW = 0;
    njets_UP = -1;
  }

  TH1D* h_nbjets_LOW = (TH1D*) f->Get(dir+"/h_nbjets_LOW");
  TH1D* h_nbjets_UP = (TH1D*) f->Get(dir+"/h_nbjets_UP");
  int nbjets_LOW;
  int nbjets_UP;
  if(h_nbjets_LOW && h_nbjets_UP){
    nbjets_LOW = h_nbjets_LOW->GetBinContent(1);
    nbjets_UP = h_nbjets_UP->GetBinContent(1);
  }
  else{
    nbjets_LOW = 0;
    nbjets_UP = -1;
  }

  if(njets_UP != -1) njets_UP--;
  if(nbjets_UP != -1) nbjets_UP--;
  
  std::string jet_string; 
  std::string bjet_string; 

  if( (njets_UP - njets_LOW) == 0) jet_string = toString(njets_LOW) + "j";
  else if( njets_UP != -1) jet_string = toString(njets_LOW) + "-" + toString(njets_UP) + "j";
  else jet_string = "#geq " + toString(njets_LOW) + "j";

  if( (nbjets_UP - nbjets_LOW) == 0) bjet_string = toString(nbjets_LOW) + "b";
  else if( nbjets_UP != -1) bjet_string = toString(nbjets_LOW) + "-" + toString(nbjets_UP) + "b";
  else bjet_string = "#geq " + toString(nbjets_LOW) + "b";

  return jet_string + ", " + bjet_string;

}

string getHTPlotLabel(const TFile* f, std::string dir_str) {

  TString dir= TString(dir_str);

  TH1D* h_ht_LOW = (TH1D*) f->Get(dir+"/h_ht_LOW");
  TH1D* h_ht_UP = (TH1D*) f->Get(dir+"/h_ht_UP");
  int ht_LOW;
  int ht_UP;
  if(h_ht_LOW && h_ht_UP){
    ht_LOW = h_ht_LOW->GetBinContent(1);
    ht_UP = h_ht_UP->GetBinContent(1);
  }
  else return "H_{T} > 450 GeV";

  if(ht_UP != -1) return toString(ht_LOW) + " < H_{T} < " + toString(ht_UP) + " GeV"; 
  else  return "H_{T} > " + toString(ht_LOW) + " GeV";

}

string getMT2MinMTPlotLabel(const TFile* f, std::string dir_str) {

  TString dir= TString(dir_str);

  TH1D* h_lowMT_LOW = (TH1D*) f->Get(dir+"/h_lowMT_LOW");
  TH1D* h_lowMT_UP = (TH1D*) f->Get(dir+"/h_lowMT_UP");
  int lowMT_LOW;
  int lowMT_UP;
  if(h_lowMT_LOW && h_lowMT_UP){
    lowMT_LOW = h_lowMT_LOW->GetBinContent(1);
    lowMT_UP = h_lowMT_UP->GetBinContent(1);
  }
  else return "";

  if(lowMT_LOW == 1) return "minM_{T} < 200 GeV && M_{T2} < 400 GeV";
  else if(lowMT_UP == 1) return "minM_{T} > 200 GeV || M_{T2} > 400 GeV";
  else return "M_{T2} > 200 GeV";

}

//_______________________________________________________________________________
string getRegionPlotLabelLine2(const string& dir) {
  if (dir.find("sr10") != string::npos) return "minM_{T} > 200 GeV ||";
  if (dir.find("sr3") != string::npos) return "minM_{T} < 200 GeV &&";
  if (dir.find("sr4") != string::npos) return "minM_{T} > 200 GeV ||";
  if (dir.find("sr7") != string::npos) return "minM_{T} < 200 GeV &&";
  if (dir.find("sr8") != string::npos) return "minM_{T} > 200 GeV ||";
  if (dir.find("sr9") != string::npos) return "minM_{T} < 200 GeV &&";
  //  if (dir.find("crsl") != string::npos) return "CRSL";

  //  cout << "getRegionPlotLabelLine2: WARNING: didn't recognize dir: " << dir << endl;
  return "";
}

//_______________________________________________________________________________
string getRegionPlotLabelLine3(const string& dir) {
  if (dir.find("sr10") != string::npos) return "M_{T2} > 400 GeV";
  if (dir.find("sr3") != string::npos) return "M_{T2} < 400 GeV";
  if (dir.find("sr4") != string::npos) return "M_{T2} > 400 GeV";
  if (dir.find("sr7") != string::npos) return "M_{T2} < 400 GeV";
  if (dir.find("sr8") != string::npos) return "M_{T2} > 400 GeV";
  if (dir.find("sr9") != string::npos) return "M_{T2} < 400 GeV";
  //  if (dir.find("crsl") != string::npos) return "CRSL";

  //  cout << "getRegionPlotLabelLine2: WARNING: didn't recognize dir: " << dir << endl;
  return "";
}

//_______________________________________________________________________________
string getHTPlotLabel(const string& dir) {
  if (dir.find("H") != string::npos) return "H_{T} > 1000 GeV";
  if (dir.find("M") != string::npos) return "575 < H_{T} < 1000 GeV";
  if (dir.find("L") != string::npos) return "450 < H_{T} < 575 GeV";
  if (dir.find("srbase") != string::npos) return "H_{T} > 450 GeV";

  cout << "getHTPlotLabel: WARNING: didn't recognize dir: " << dir << endl;
  return dir;
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

string getJetBJetTableLabel(const TFile* f, std::string dir_str) {

  TString dir= TString(dir_str);

  TH1D* h_njets_LOW = (TH1D*) f->Get(dir+"/h_njets_LOW");
  TH1D* h_njets_UP = (TH1D*) f->Get(dir+"/h_njets_UP");
  int njets_LOW;
  int njets_UP;
  if(h_njets_LOW && h_njets_UP){
    njets_LOW = h_njets_LOW->GetBinContent(1);
    njets_UP = h_njets_UP->GetBinContent(1);
  }
  else{
    njets_LOW = 0;
    njets_UP = -1;
  }

  TH1D* h_nbjets_LOW = (TH1D*) f->Get(dir+"/h_nbjets_LOW");
  TH1D* h_nbjets_UP = (TH1D*) f->Get(dir+"/h_nbjets_UP");
  int nbjets_LOW;
  int nbjets_UP;
  if(h_nbjets_LOW && h_nbjets_UP){
    nbjets_LOW = h_nbjets_LOW->GetBinContent(1);
    nbjets_UP = h_nbjets_UP->GetBinContent(1);
  }
  else{
    nbjets_LOW = 0;
    nbjets_UP = -1;
  }

  if(njets_UP != -1) njets_UP--;
  if(nbjets_UP != -1) nbjets_UP--;
  
  std::string jet_string; 
  std::string bjet_string; 

  if( (njets_UP - njets_LOW) == 0) jet_string = toString(njets_LOW) + "j";
  else if( njets_UP != -1) jet_string = toString(njets_LOW) + "-" + toString(njets_UP) + "j";
  else jet_string = "$\\geq$" + toString(njets_LOW) + "j";

  if( (nbjets_UP - nbjets_LOW) == 0) bjet_string = toString(nbjets_LOW) + "b";
  else if( nbjets_UP != -1) bjet_string = toString(nbjets_LOW) + "-" + toString(nbjets_UP) + "b";
  else bjet_string = "$\\geq$" + toString(nbjets_LOW) + "b";


  //check if bin is split by minMT
  bool splitByMinMT = false;
  std::string minMT_string;
  TH1D* h_lowMT_LOW = (TH1D*) f->Get(dir+"/h_lowMT_LOW");
  TH1D* h_lowMT_UP = (TH1D*) f->Get(dir+"/h_lowMT_UP");
  int lowMT_LOW = h_lowMT_LOW->GetBinContent(1);
  int lowMT_UP = h_lowMT_UP->GetBinContent(1);

  if(lowMT_LOW == 0 && lowMT_UP == 1){
    splitByMinMT = true; 
    minMT_string = "hiMT";
  }

  if(lowMT_LOW == 1 && lowMT_UP == 2){
    splitByMinMT = true; 
    minMT_string = "loMT";
  }


  if(!splitByMinMT) return jet_string + ", " + bjet_string;
  else return jet_string + ", " + bjet_string + " " + minMT_string;

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
  h_axes->GetYaxis()->SetTitle(ytitle.c_str());
  h_axes->GetYaxis()->SetLabelSize(0.04);
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
  TString region_label_line2 = getMT2MinMTPlotLabel(samples.at(0), histdir);
  label.DrawLatex(0.2,0.85,ht_label);
  // minMT plot always requires at least 2 bjets
  if ((histdir.find("srbase") != std::string::npos) && (histname.find("minMTBMet") != std::string::npos)) region_label = "#geq 2j, #geq 2b";
  if (region_label.Length() > 0) label.DrawLatex(0.2,0.81,region_label);
  if (region_label_line2.Length() > 0) label.DrawLatex(0.2,0.77,region_label_line2);

  leg->Draw();
  h_axes->Draw("axissame");

  //CMS_lumi( can, iPeriod, iPos );

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
//std::cout << "\\begin{tabular}{r|c|c|c|c|c|c}" << std::endl;
std::cout << "\\begin{tabular}{r|c|c|c|c|c|c|c|c|c|c|c}" << std::endl;
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

  std::cout << "\\end{tabular}" << std::endl;
  std::cout << "\\caption{}" << std::endl;
  std::cout << "\\end{table}" << std::endl;

  cout << endl;
  return;
}

//_______________________________________________________________________________
void plotMaker(){

  gROOT->LoadMacro("CMS_lumi.C");
  cmsText = "CMS Simulation";
  writeExtraText = false;
  lumi_13TeV = "4 fb^{-1}";

  //string input_dir = "/home/olivito/cms3/MT2Analysis/MT2looper/output/V00-00-08_4fb/";
  string input_dir = "/home/users/jgran/temp/new_datacards/MT2Analysis/MT2looper/output/2015ExtendedNJets_UltraHighHT/";

  // ----------------------------------------
  //  samples definition
  // ----------------------------------------

  // get input files

  TFile* f_ttbar = new TFile(Form("%s/ttall_msdecays.root",input_dir.c_str()));
  TFile* f_zinv = new TFile(Form("%s/zinv_ht.root",input_dir.c_str()));
  TFile* f_gjet = new TFile(Form("%s/gjet_ht.root",input_dir.c_str()));
  TFile* f_wjets = new TFile(Form("%s/wjets_ht.root",input_dir.c_str()));
  TFile* f_qcd = new TFile(Form("%s/qcd_ht.root",input_dir.c_str()));

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

  //samples.push_back(f_qcd); names.push_back("qcd");
  samples.push_back(f_wjets); names.push_back("wjets");
  samples.push_back(f_zinv);  names.push_back("zinv");
  //samples.push_back(f_gjet);  names.push_back("gjet");
  samples.push_back(f_ttbar); names.push_back("ttbar");

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

  //samples.push_back(f_wjets_ht600toInf);  names.push_back("wjets_ht600toInf");
  //samples.push_back(f_wjets_ht400to600);  names.push_back("wjets_ht400to600");
  //samples.push_back(f_wjets_ht200to400);  names.push_back("wjets_ht200to400");
  //samples.push_back(f_wjets_ht100to200);  names.push_back("wjets_ht100to200");

  //samples.push_back(f_zinv_ht600toInf);  names.push_back("zinv_ht600toInf");
  //samples.push_back(f_zinv_ht400to600);  names.push_back("zinv_ht400to600");
  //samples.push_back(f_zinv_ht200to400);  names.push_back("zinv_ht200to400");
  //samples.push_back(f_zinv_ht100to200);  names.push_back("zinv_ht100to200");

  // ----------------------------------------
  //  plots definitions
  // ----------------------------------------

  float scalesig = -1.;
  //bool printplots = false;
  bool printplots = true;

/*
  TIter it(f_ttbar->GetListOfKeys());
  TKey* k;
  std::string cr_skip = "cr";
  std::string sr_skip = "sr";
  while (k = (TKey *)it()) {
    if (strncmp (k->GetTitle(), cr_skip.c_str(), cr_skip.length()) == 0) continue; //skip control regions
    //if (strncmp (k->GetTitle(), sr_skip.c_str(), sr_skip.length()) == 0) continue; //skip signal regions and srbase
    std::string dir_name = k->GetTitle();
    if(dir_name == "") continue;
    //if(dir_name != "srbase") continue; //to do only srbase
    //if(dir_name != "sr1H") continue; //for testing
    //makePlot( samples , names , dir_name , "h_ht"  , "H_{T} [GeV]" , "Events / 25 GeV" , 0 , 2000 , 1 , true, printplots );
    //makePlot( samples , names , dir_name , "h_mt2" , "M_{T2} [GeV]" , "Events / 10 GeV" , 0 , 800 , 1 , true, printplots );
    //makePlot( samples , names , dir_name , "h_met"  , "E_{T}^{miss} [GeV]" , "Events / 10 GeV" , 0 , 800 , 1 , true, printplots );
    //makePlot( samples , names , dir_name , "h_nlepveto" , "N(leptons)" , "Events" , 0 , 10 , 1 , false, printplots );
    //makePlot( samples , names , dir_name , "h_nJet40" , "N(jets)" , "Events" , 0 , 15 , 1 , false, printplots );
    //makePlot( samples , names , dir_name , "h_nBJet40" , "N(b jets)" , "Events" , 0 , 6 , 1 , false, printplots );
    //makePlot( samples , names , dir_name , "h_minMTBMet"  , "min M_{T}(b,MET) [GeV]" , "Events / 10 GeV" , 0 , 800 , 1 , true, printplots );
    //makePlot( samples , names , dir_name , "h_mt2bins" , "M_{T2} [GeV]" , "Events / Bin" , 200 , 1500 , 1 , true, printplots, scalesig );
  }
*/

  std::cout << "\\documentclass[landscape, 10pt]{article}" << std::endl;
  std::cout << "\\usepackage{amsmath}" << std::endl;
  std::cout << "\\usepackage{amssymb}" << std::endl;
  std::cout << "\\usepackage{graphicx}" << std::endl;
  std::cout << "\\usepackage[left=.1in,top=1in,right=.1in,bottom=.1in,nohead]{geometry}" << std::endl;
  std::cout << "\\begin{document}" << std::endl;

  vector<string> dirsH;
  dirsH.push_back("sr1H");
  dirsH.push_back("sr2H");
  dirsH.push_back("sr3H");
  dirsH.push_back("sr4H");
  dirsH.push_back("sr5H");
  dirsH.push_back("sr6H");
  dirsH.push_back("sr7H");
  dirsH.push_back("sr8H");
  dirsH.push_back("sr9H");
  dirsH.push_back("sr10H");
  printTable(samples, names, dirsH);

  dirsH.clear();
  dirsH.push_back("sr11H");
  dirsH.push_back("sr12H");
  dirsH.push_back("sr13H");
  dirsH.push_back("sr14H");
  dirsH.push_back("sr15H");
  dirsH.push_back("sr16H");
  dirsH.push_back("sr17H");
  dirsH.push_back("sr18H");
  dirsH.push_back("sr19H");
  dirsH.push_back("sr20H");
  printTable(samples, names, dirsH);

/*
  vector<string> dirsH;
  dirsH.push_back("sr1UH");
  dirsH.push_back("sr2UH");
  dirsH.push_back("sr3UH");
  dirsH.push_back("sr4UH");
  dirsH.push_back("sr5UH");
  dirsH.push_back("sr6UH");
  dirsH.push_back("sr7UH");
  dirsH.push_back("sr8UH");
  dirsH.push_back("sr9UH");
  dirsH.push_back("sr10UH");
  printTable(samples, names, dirsH);

  dirsH.clear();
  dirsH.push_back("sr11UH");
  dirsH.push_back("sr12UH");
  dirsH.push_back("sr13UH");
  dirsH.push_back("sr14UH");
  dirsH.push_back("sr15UH");
  dirsH.push_back("sr16UH");
  dirsH.push_back("sr17UH");
  dirsH.push_back("sr18UH");
  dirsH.push_back("sr19UH");
  dirsH.push_back("sr20UH");
  printTable(samples, names, dirsH);
*/

/*
  vector<string> dirsH;
  dirsH.push_back("crsl1H");
  dirsH.push_back("crsl2H");
  dirsH.push_back("crsl3H");
  dirsH.push_back("crsl4H");
  dirsH.push_back("crsl5H");
  dirsH.push_back("crsl6H");
  dirsH.push_back("crsl7H");
  dirsH.push_back("crsl8H");
  dirsH.push_back("crsl9H");
  dirsH.push_back("crsl10H");
  printTable(samples, names, dirsH);

  dirsH.clear();
  dirsH.push_back("crsl11H");
  dirsH.push_back("crsl12H");
  dirsH.push_back("crsl13H");
  dirsH.push_back("crsl14H");
  dirsH.push_back("crsl15H");
  dirsH.push_back("crsl16H");
  dirsH.push_back("crsl17H");
  dirsH.push_back("crsl18H");
  dirsH.push_back("crsl19H");
  dirsH.push_back("crsl20H");
  printTable(samples, names, dirsH);
*/

/*
  vector<string> dirsH;
  dirsH.push_back("crsl1UH");
  dirsH.push_back("crsl2UH");
  dirsH.push_back("crsl3UH");
  dirsH.push_back("crsl4UH");
  dirsH.push_back("crsl5UH");
  dirsH.push_back("crsl6UH");
  dirsH.push_back("crsl7UH");
  dirsH.push_back("crsl8UH");
  dirsH.push_back("crsl9UH");
  dirsH.push_back("crsl10UH");
  printTable(samples, names, dirsH);

  dirsH.clear();
  dirsH.push_back("crsl11UH");
  dirsH.push_back("crsl12UH");
  dirsH.push_back("crsl13UH");
  dirsH.push_back("crsl14UH");
  dirsH.push_back("crsl15UH");
  dirsH.push_back("crsl16UH");
  dirsH.push_back("crsl17UH");
  dirsH.push_back("crsl18UH");
  dirsH.push_back("crsl19UH");
  dirsH.push_back("crsl20UH");
  printTable(samples, names, dirsH);
*/

/*
  vector<string> dirsH;
  dirsH.push_back("crgj1H");
  dirsH.push_back("crgj2H");
  dirsH.push_back("crgj3H");
  dirsH.push_back("crgj4H");
  dirsH.push_back("crgj5H");
  dirsH.push_back("crgj6H");
  dirsH.push_back("crgj7H");
  dirsH.push_back("crgj8H");
  dirsH.push_back("crgj9H");
  dirsH.push_back("crgj10H");
  printTable(samples, names, dirsH);

  dirsH.clear();
  dirsH.push_back("crgj11H");
  dirsH.push_back("crgj12H");
  dirsH.push_back("crgj13H");
  dirsH.push_back("crgj14H");
  dirsH.push_back("crgj15H");
  dirsH.push_back("crgj16H");
  dirsH.push_back("crgj17H");
  dirsH.push_back("crgj18H");
  dirsH.push_back("crgj19H");
  dirsH.push_back("crgj20H");
  printTable(samples, names, dirsH);

*/

/*
  vector<string> dirsH;
  dirsH.clear();
  dirsH.push_back("crgj1UH");
  dirsH.push_back("crgj2UH");
  dirsH.push_back("crgj3UH");
  dirsH.push_back("crgj4UH");
  dirsH.push_back("crgj5UH");
  dirsH.push_back("crgj6UH");
  dirsH.push_back("crgj7UH");
  dirsH.push_back("crgj8UH");
  dirsH.push_back("crgj9UH");
  dirsH.push_back("crgj10UH");
  printTable(samples, names, dirsH);

  dirsH.clear();
  dirsH.push_back("crgj11UH");
  dirsH.push_back("crgj12UH");
  dirsH.push_back("crgj13UH");
  dirsH.push_back("crgj14UH");
  dirsH.push_back("crgj15UH");
  dirsH.push_back("crgj16UH");
  dirsH.push_back("crgj17UH");
  dirsH.push_back("crgj18UH");
  dirsH.push_back("crgj19UH");
  dirsH.push_back("crgj20UH");
  printTable(samples, names, dirsH);
*/

  std::cout << "\\end{document}" << std::endl;

}

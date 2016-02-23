#include <iostream>
#include <utility>
#include <string>
#include <sstream>

#include "TROOT.h"
#include "TString.h"
#include "TFile.h"
#include "TH1.h"

#include "plotUtilities.h"

using namespace std;

//_______________________________________________________________________________
std::string toString(int in){
  stringstream ss;
  ss << in;
  return ss.str();
}

//_______________________________________________________________________________
void ReplaceString(std::string& subject, const std::string& search, const std::string& replace) {
  size_t pos = 0;
  while((pos = subject.find(search, pos)) != std::string::npos) {
    subject.replace(pos, search.length(), replace);
    pos += replace.length();
  }
}

//_______________________________________________________________________________
int getColor(const string& sample) {

  //softLep fakes
  if (sample.find("wjets fakeLep") != string::npos) return 417+3;
  if (sample.find("zinv fakeLep") != string::npos) return 419+3;
  if (sample.find("tt+1l fakeLep") != string::npos) return kAzure+2+3;
  if (sample.find("tt+2l fakeLep") != string::npos) return kOrange-1+3;
  if (sample.find("st fakeLep") != string::npos) return kViolet+3;
  
  if (sample.find("data") != string::npos) return kBlack;
  if (sample.find("ttbar") != string::npos) return 855;
  if (sample.find("top") != string::npos) return 855;
  if (sample.find("wjets") != string::npos) return 417;
  if (sample.find("zinv") != string::npos) return 418;
  if (sample.find("qcd") != string::npos) return 401;
  if (sample.find("multijet") != string::npos) return 402;
  if (sample.find("gjet") != string::npos) return 18;
  if (sample.find("fakephoton") != string::npos) return 46;
  if (sample.find("fragphoton") != string::npos) return 38;
  if (sample.find("dyjets") != string::npos) return 430;
  if (sample.find("zjets") != string::npos) return 430;
  if (sample.find("lostl") != string::npos) return 430;
  if (sample.find("T1tttt") != string::npos) return kRed;
  if (sample.find("T1bbbb") != string::npos) return kMagenta;
  if (sample.find("T1qqqq") != string::npos) return kOrange;
  if (sample.find("T2tt") != string::npos) return kCyan;
  if (sample.find("T2bb") != string::npos) return kMagenta+3;
  if (sample.find("tt+1l") != string::npos) return kAzure+2;
  if (sample.find("tt+2l") != string::npos) return kOrange-1;
  if (sample.find("st") != string::npos) return kViolet;

  cout << "getColor: WARNING: didn't recognize sample: " << sample << endl;
  return kBlack;
}

//_______________________________________________________________________________
string getLegendName(const string& sample) {

  //softLep fakes
  if (sample.find("wjets fakeLep") != string::npos) return "Fake W+jets";
  if (sample.find("st fakeLep") != string::npos) return "Fake singletop";
  if (sample.find("zinv fakeLep") != string::npos) return "Fake Z #rightarrow #nu#nu";
  if (sample.find("tt+1l fakeLep") != string::npos) return "Fake tt + 1l";
  if (sample.find("tt+2l fakeLep") != string::npos) return "Fake tt + 2l";

  if (sample.find("fakedata") != string::npos) return "Fake Data";
  if (sample.find("data") != string::npos) return "Data";
  if (sample.find("ttbar") != string::npos) return "ttbar";
  if (sample.find("top") != string::npos) return "Top quark";
  if (sample.find("wjets") != string::npos) return "W+jets";
  if (sample.find("zinv") != string::npos) return "Z #rightarrow #nu#bar{#nu}";
  if (sample.find("gjet") != string::npos) return "Prompt #gamma";
  if (sample.find("fakephotonMC") != string::npos) return "Fake #gamma";
  if (sample.find("fakephotonData") != string::npos) return "Fake #gamma (data)";
  if (sample.find("fragphoton") != string::npos) return "Frag. #gamma";
  if (sample.find("qcd") != string::npos) return "Multijet";
  if (sample.find("multijet") != string::npos) return "Multijet";
  if (sample.find("dyjets") != string::npos) return "Z+jets";
  if (sample.find("zjets") != string::npos) return "Z+jets";
  if (sample.find("lostl") != string::npos) return "Lost lepton";
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
  if (sample.find("tt+1l") != string::npos) return "tt + 1l";
  if (sample.find("tt+2l") != string::npos) return "tt + 2l";
  if (sample.find("st") != string::npos) return "singletop";

  cout << "getLegendName: WARNING: didn't recognize sample: " << sample << endl;
  return sample;
}

//_______________________________________________________________________________
string getTableName(const string& sample) {
  if (sample.find("fakedata") != string::npos) return "Fake Data";
  if (sample.find("data") != string::npos) return "Data";
  if (sample.find("ttbar") != string::npos) return "ttbar";
  if (sample.find("ttw") != string::npos) return "ttw";
  if (sample.find("ttz") != string::npos) return "ttz";
  if (sample.find("tth") != string::npos) return "tth";
  if (sample.find("singletop") != string::npos) return "single top";
  if (sample.find("top") != string::npos) return "Top";
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
  if (sample.find("singletop") != string::npos) return "Single Top";
  if (sample.find("ttv") != string::npos) return "ttV";
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
string getJetBJetPlotLabel(TFile* f, std::string dir_str) {

  TString dir= TString(dir_str);

  TH1D* h_njets_LOW = (TH1D*) f->Get(dir+"/h_njets_LOW");
  TH1D* h_njets_HI = (TH1D*) f->Get(dir+"/h_njets_HI");
  int njets_LOW;
  int njets_HI;
  if(h_njets_LOW && h_njets_HI){
    njets_LOW = h_njets_LOW->GetBinContent(1);
    njets_HI = h_njets_HI->GetBinContent(1);
  }
  else{
    njets_LOW = 0;
    njets_HI = -1;
  }

  TH1D* h_nbjets_LOW = (TH1D*) f->Get(dir+"/h_nbjets_LOW");
  TH1D* h_nbjets_HI = (TH1D*) f->Get(dir+"/h_nbjets_HI");
  int nbjets_LOW;
  int nbjets_HI;
  if(h_nbjets_LOW && h_nbjets_HI){
    nbjets_LOW = h_nbjets_LOW->GetBinContent(1);
    nbjets_HI = h_nbjets_HI->GetBinContent(1);
  }
  else{
    nbjets_LOW = 0;
    nbjets_HI = -1;
  }

  if(njets_HI != -1) njets_HI--;
  if(nbjets_HI != -1) nbjets_HI--;

  std::string jet_string; 
  std::string bjet_string; 

  if( (njets_HI - njets_LOW) == 0) jet_string = toString(njets_LOW) + "j";
  else if( njets_HI != -1) jet_string = toString(njets_LOW) + "-" + toString(njets_HI) + "j";
  else jet_string = "#geq " + toString(njets_LOW) + "j";

  if( (nbjets_HI - nbjets_LOW) == 0) bjet_string = toString(nbjets_LOW) + "b";
  else if( nbjets_HI != -1) bjet_string = toString(nbjets_LOW) + "-" + toString(nbjets_HI) + "b";
  else bjet_string = "#geq " + toString(nbjets_LOW) + "b";

  return jet_string + ", " + bjet_string;

}

//_______________________________________________________________________________
string getHTPlotLabel(TFile* f, std::string dir_str) {

  TString dir= TString(dir_str);

  TH1D* h_ht_LOW = (TH1D*) f->Get(dir+"/h_ht_LOW");
  TH1D* h_ht_HI = (TH1D*) f->Get(dir+"/h_ht_HI");
  int ht_LOW;
  int ht_HI;
  if(h_ht_LOW && h_ht_HI){
    ht_LOW = h_ht_LOW->GetBinContent(1);
    ht_HI = h_ht_HI->GetBinContent(1);
  }
  else return "H_{T} > 200 GeV";

  if(ht_HI != -1) return toString(ht_LOW) + " < H_{T} < " + toString(ht_HI) + " GeV"; 
  else  return "H_{T} > " + toString(ht_LOW) + " GeV";

}

//_______________________________________________________________________________
string getHTTableLabel(TFile* f, std::string dir_str) {

  TString dir= TString(dir_str);

  TH1D* h_ht_LOW = (TH1D*) f->Get(dir+"/h_ht_LOW");
  TH1D* h_ht_HI = (TH1D*) f->Get(dir+"/h_ht_HI");
  int ht_LOW;
  int ht_HI;
  if(h_ht_LOW && h_ht_HI){
    ht_LOW = h_ht_LOW->GetBinContent(1);
    ht_HI = h_ht_HI->GetBinContent(1);
  }
  else return "$H_{T} > 200$~GeV";

  if(ht_HI != -1) return "$" + toString(ht_LOW) + " < H_{T} < " + toString(ht_HI) + "$~GeV"; 
  else  return "$H_{T} > " + toString(ht_LOW) + "$~GeV";

}

//_______________________________________________________________________________
string getMETTableLabel(TFile* f, std::string dir_str) {

  TString dir= TString(dir_str);

  TH1D* h_met_LOW = (TH1D*) f->Get(dir+"/h_met_LOW");
  int met_LOW(0);
  if(h_met_LOW){
    met_LOW = h_met_LOW->GetBinContent(1);
  }

  return "$\\MET > " + toString(met_LOW) + " $~GeV ";

}

//_______________________________________________________________________________
string getMT2PlotLabel(TFile* f, std::string dir_str) {

  TString dir= TString(dir_str);

  TH1D* h_mt2_LOW = (TH1D*) f->Get(dir+"/h_mt2_LOW");
  TH1D* h_mt2_HI = (TH1D*) f->Get(dir+"/h_mt2_HI");
  int mt2_LOW(0);
  int mt2_HI(0);
  if(h_mt2_LOW && h_mt2_HI){
    mt2_LOW = h_mt2_LOW->GetBinContent(1);
    mt2_HI = h_mt2_HI->GetBinContent(1);
  }
  else return "M_{T2} > 200 GeV";

  return "M_{T2} > " + toString(mt2_LOW) + " GeV";

}

//_______________________________________________________________________________
string getJetBJetTableLabel(TFile* f, std::string dir_str) {

  TString dir= TString(dir_str);

  TH1D* h_njets_LOW = (TH1D*) f->Get(dir+"/h_njets_LOW");
  TH1D* h_njets_HI = (TH1D*) f->Get(dir+"/h_njets_HI");
  int njets_LOW;
  int njets_HI;
  if(h_njets_LOW && h_njets_HI){
    njets_LOW = h_njets_LOW->GetBinContent(1);
    njets_HI = h_njets_HI->GetBinContent(1);
  }
  else{
    njets_LOW = 0;
    njets_HI = -1;
  }

  TH1D* h_nbjets_LOW = (TH1D*) f->Get(dir+"/h_nbjets_LOW");
  TH1D* h_nbjets_HI = (TH1D*) f->Get(dir+"/h_nbjets_HI");
  int nbjets_LOW;
  int nbjets_HI;
  if(h_nbjets_LOW && h_nbjets_HI){
    nbjets_LOW = h_nbjets_LOW->GetBinContent(1);
    nbjets_HI = h_nbjets_HI->GetBinContent(1);
  }
  else{
    nbjets_LOW = 0;
    nbjets_HI = -1;
  }

  if(njets_HI != -1) njets_HI--;
  if(nbjets_HI != -1) nbjets_HI--;

  std::string jet_string; 
  std::string bjet_string; 

  if( (njets_HI - njets_LOW) == 0) jet_string = toString(njets_LOW) + "j";
  else if( njets_HI != -1) jet_string = toString(njets_LOW) + "-" + toString(njets_HI) + "j";
  else jet_string = "$\\geq$" + toString(njets_LOW) + "j";

  if( (nbjets_HI - nbjets_LOW) == 0) bjet_string = toString(nbjets_LOW) + "b";
  else if( nbjets_HI != -1) bjet_string = toString(nbjets_LOW) + "-" + toString(nbjets_HI) + "b";
  else bjet_string = "$\\geq$" + toString(nbjets_LOW) + "b";

  return jet_string + ", " + bjet_string;

}

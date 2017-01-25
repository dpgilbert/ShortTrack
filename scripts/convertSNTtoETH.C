#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>
#include <regex>

#include "TROOT.h"
#include "TLatex.h"
#include "TString.h"
#include "TObjArray.h"
#include "TH2.h"
#include "THStack.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TH1.h"
#include "TH3.h"
#include "TPaveText.h"
#include "TStyle.h"
#include "TKey.h"


//_______________________________________________________________________________
void ReplaceString(std::string& subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}

//__________________________________________________
// signal region names 
TString getETHname(TFile* fin, TString directory) {

    // retrieve relevant factors and uncertainties from ETH input file -- need to convert name
    TH1D* h_ht_LOW = (TH1D*) fin->Get(directory+"/h_ht_LOW");
    TH1D* h_ht_HI = (TH1D*) fin->Get(directory+"/h_ht_HI");
    int ht_LOW = h_ht_LOW->GetBinContent(1);
    int ht_HI = h_ht_HI->GetBinContent(1);
  
    TH1D* h_nbjets_LOW = (TH1D*) fin->Get(directory+"/h_nbjets_LOW");
    TH1D* h_nbjets_HI = (TH1D*) fin->Get(directory+"/h_nbjets_HI");
    int nbjets_LOW = h_nbjets_LOW->GetBinContent(1);
    int nbjets_HI = h_nbjets_HI->GetBinContent(1);

    TH1D* h_njets_LOW = (TH1D*) fin->Get(directory+"/h_njets_LOW");
    TH1D* h_njets_HI = (TH1D*) fin->Get(directory+"/h_njets_HI");
    int njets_LOW = h_njets_LOW->GetBinContent(1);
    int njets_HI = h_njets_HI->GetBinContent(1);

    int nbjets_HI_mod = nbjets_HI;
    int njets_HI_mod = njets_HI;
    if(nbjets_HI != -1) nbjets_HI_mod--;
    if(njets_HI != -1) njets_HI_mod--;

    std::string ht_str = "HT" + std::to_string(ht_LOW) + "to" + std::to_string(ht_HI);
    std::string jet_str = (njets_HI_mod == njets_LOW) ? "j" + std::to_string(njets_LOW) : "j" + std::to_string(njets_LOW) + "to" + std::to_string(njets_HI_mod);
    std::string bjet_str = (nbjets_HI_mod == nbjets_LOW) ? "b" + std::to_string(nbjets_LOW) : "b" + std::to_string(nbjets_LOW) + "to" + std::to_string(nbjets_HI_mod);
  
    //Replace instances of "-1" with "inf" for variables with no upper bound.
    ReplaceString(ht_str, "-1", "Inf");
    ReplaceString(jet_str, "-1", "Inf");
    ReplaceString(bjet_str, "-1", "Inf");

    std::string channel = ht_str + "_" + jet_str + "_" + bjet_str;

    return TString(channel.c_str());
}

//__________________________________________________
void convertSNTtoETH(TString dir, TString sample) {

  TFile* fin = new TFile(dir+"/"+sample+".root","READ");
  TFile* fout = new TFile(Form("%s/%s_eth.root",dir.Data(),sample.Data()),"RECREATE");
  TDirectory* dout_base = fout->mkdir(sample);
  
  TIter dir_it(fin->GetListOfKeys());
  TKey* dir_k;
  while ((dir_k = (TKey *)dir_it())) {
    if (TString(dir_k->GetClassName()) != "TDirectoryFile") continue;
    TString dir_name = dir_k->GetTitle();
    if(dir_name == "") continue;
    if(!dir_name.Contains("sr")) continue;
    if(dir_name.Contains("base")) continue;
    
    // check to see if this is a super signal region (20 and up)
    int sr_number = -1;
    std::string dir_str = dir_k->GetTitle();
    std::regex sr_number_regex("[^0-9]*([0-9]+).*"); // finds an integer of any length
    std::smatch sr_number_match;
    if (std::regex_match(dir_str,sr_number_match, sr_number_regex)) {
      sr_number = stoi(sr_number_match[1].str());
      if (sr_number >= 20) continue;
    }
    else {
      cout << "WARNING: couldn't get signal region number for region: " << dir_str << endl;
    }
  
    TDirectory* din = (TDirectory*) dir_k->ReadObj();
    if(din == 0) continue;
    //std::cout << "dir_name: " << dir_name << std::endl;
    TString dir_name_eth = getETHname(fin,dir_name);
    //std::cout << "dir_name_eth: " << dir_name_eth << std::endl;
    TDirectory* dout = dout_base->mkdir(dir_name_eth);

    TIter hist_it(din->GetListOfKeys());
    TKey* hist_k;
    while ((hist_k = (TKey *)hist_it())) {
      TString hist_name = (hist_k->GetName());
      TString hist_name_eth(hist_name);
      // 3d signal hists
      if (TString(hist_k->GetClassName()) == "TH3D") {
	hist_name_eth.ReplaceAll("h_mt2bins_","yield3d");
	hist_name_eth.ReplaceAll("sigscan","");
	hist_name_eth += "_" + sample + "_" + dir_name_eth;
	TH3D* h = (TH3D*)hist_k->ReadObj();
	h->SetName(hist_name_eth);
	dout->WriteObject(h,hist_name_eth);
      }
      // 1d yield hist
      else if (hist_name == "h_mt2bins" && TString(hist_k->GetClassName()) == "TH1D") {
	hist_name_eth = "yield_" + sample + "_" + dir_name_eth;
	TH1D* h = (TH1D*)hist_k->ReadObj();
	h->SetName(hist_name_eth);
	dout->WriteObject(h,hist_name_eth);
      }
      // 1d crsl yield hist
      else if (hist_name == "h_mt2bins_crsl" && TString(hist_k->GetClassName()) == "TH1D") {
	hist_name_eth = "yield_crsl_" + sample + "_" + dir_name_eth;
	TH1D* h = (TH1D*)hist_k->ReadObj();
	h->SetName(hist_name_eth);
	dout->WriteObject(h,hist_name_eth);
      }
      // 1d transfer factor hist
      else if (hist_name == "h_mt2bins_alpha" && TString(hist_k->GetClassName()) == "TH1D") {
	hist_name_eth = "yield_alpha_" + sample + "_" + dir_name_eth;
	TH1D* h = (TH1D*)hist_k->ReadObj();
	h->SetName(hist_name_eth);
	dout->WriteObject(h,hist_name_eth);
      }
    }// loop on hists
      
  }// loop on dirs

  fin->Close();
  fout->Close();

  delete fin;
  delete fout;
  
  return;
}

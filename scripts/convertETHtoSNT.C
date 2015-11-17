#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

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
#include "TPaveText.h"
#include "TStyle.h"
#include "TKey.h"

//__________________________________________________
// signal region names based on getSignalRegionsJamboree
TString getSNTname(TString dirname) {

  //std::cout << "getting snt name for dirname: " << dirname << std::endl;
  TObjArray* tokens = (TObjArray*) dirname.Tokenize("_");

  // first token is HT name
  TString ht_string = ((TObjString*) (*tokens)[0])->GetString();
  TString nj_string = ((TObjString*) (*tokens)[1])->GetString();
  TString nb_string = ((TObjString*) (*tokens)[2])->GetString();

  //std::cout << "  tokenized to: " << ht_string << ", " << nj_string << ", " << nb_string  << std::endl;

  TString output_ht_name = "";
  if (ht_string == "HT200to450") output_ht_name = "VL";
  else if (ht_string == "HT450to575") output_ht_name = "L";
  else if (ht_string == "HT575to1000") output_ht_name = "M";
  else if (ht_string == "HT1000to1500") output_ht_name = "H";
  else if (ht_string == "HT1500toInf") output_ht_name = "UH";
  else if (ht_string == "HT200toInf") output_ht_name = "J";

  int output_region_number = 0;
  if (output_ht_name == "J") {
    if (nb_string == "b0") return "srbaseJ0B";
    else if (nb_string == "b1toInf") return "srbaseJ1B";
  }
  // not monojet region
  else {
    if (nj_string == "j2to3" && nb_string == "b0") output_region_number = 1;
    else if (nj_string == "j2to3" && nb_string == "b1") output_region_number = 2;
    else if (nj_string == "j2to3" && nb_string == "b2") output_region_number = 3;
    else if (nj_string == "j4to6" && nb_string == "b0") output_region_number = 4;
    else if (nj_string == "j4to6" && nb_string == "b1") output_region_number = 5;
    else if (nj_string == "j4to6" && nb_string == "b2") output_region_number = 6;
    else if (nj_string == "j7toInf" && nb_string == "b0") output_region_number = 7;
    else if (nj_string == "j7toInf" && nb_string == "b1") output_region_number = 8;
    else if (nj_string == "j7toInf" && nb_string == "b2") output_region_number = 9;
    else if (nj_string == "j2to6" && nb_string == "b3toInf") output_region_number = 10;
    else if (nj_string == "j7toInf" && nb_string == "b3toInf") output_region_number = 11;
  }

  return Form("sr%d%s",output_region_number,output_ht_name.Data());
}

//__________________________________________________
void convertETHtoSNT(TString filename) {

  TFile* fin = new TFile(filename,"READ");
  TString filename_noext(filename);
  filename_noext.ReplaceAll(".root","");
  TFile* fout = new TFile(Form("%s_snt.root",filename_noext.Data()),"RECREATE");
  
  TIter dir_it(fin->GetListOfKeys());
  TKey* dir_k;
  while ((dir_k = (TKey *)dir_it())) {
    if (TString(dir_k->GetClassName()) != "TDirectoryFile") continue;
    TString dir_name = dir_k->GetTitle();
    if(dir_name == "") continue;
    TDirectory* din = (TDirectory*) dir_k->ReadObj();
    if(din == 0) continue;
    TDirectory* dout = fout->mkdir(dir_name);

    TIter subdir_it(din->GetListOfKeys());
    TKey* subdir_k;
    while ((subdir_k = (TKey *)subdir_it())) {
      std::string subdir_name = subdir_k->GetTitle();
      TDirectory* subdin = (TDirectory*) subdir_k->ReadObj();
      TString snt_dirname = getSNTname(subdir_name);
      //std::cout << "  snt_dirname: " << snt_dirname << std::endl;
      TDirectory* subdout = dout->mkdir(snt_dirname);
      
      TIter hist_it(subdin->GetListOfKeys());
      TKey* hist_k;
      while ((hist_k = (TKey *)hist_it())) {
	if (TString(hist_k->GetClassName()) != "TH1D") continue;
	std::string hist_name = (hist_k->GetTitle());
	TH1D* h = (TH1D*)hist_k->ReadObj();
	subdout->WriteObject(h,h->GetName());
      }// loop on hists
      
    }// loop on subdirs
    
  }// loop on dirs

  fin->Close();
  fout->Close();

  delete fin;
  delete fout;
  
  return;
}

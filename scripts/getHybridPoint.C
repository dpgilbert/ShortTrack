#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

#include "TROOT.h"
#include "TLatex.h"
#include "TString.h"
#include "TFile.h"
#include "TH1.h"
#include "TH3.h"
#include "TList.h"
#include "TCollection.h"
#include "TKey.h"


void getHybridPoint();
void getHybridTables();
void getZinvTable();
void setRegionsLepAll();
void setRegionsZinvAll();
void setRegionsZinvTemplates();

vector<string> lepSRs;
vector<string> lepHTs;
vector<string> lepNJs;
vector<string> lepNBs;


vector<string> zinvSRs;
vector<string> zinvHTs;
vector<string> zinvNJs;
vector<string> zinvNBs;

TString dir = "/home/users/mderdzinski/winter2017/clean_master/MT2looper/output/full2016_v16fixed_Feb6/";

void setRegionsLepAll() {

  lepSRs.push_back("sr1VL"); lepHTs.push_back("[250,450]"); lepNJs.push_back("2-3"); lepNBs.push_back("0");
  lepSRs.push_back("sr2VL"); lepHTs.push_back("[250,450]"); lepNJs.push_back("2-3"); lepNBs.push_back("1");
  lepSRs.push_back("sr3VL"); lepHTs.push_back("[250,450]"); lepNJs.push_back("2-3"); lepNBs.push_back("2");
  lepSRs.push_back("sr12VL"); lepHTs.push_back("[250,450]"); lepNJs.push_back("4+"); lepNBs.push_back("0");
  lepSRs.push_back("sr13VL"); lepHTs.push_back("[250,450]"); lepNJs.push_back("4+"); lepNBs.push_back("1");
  lepSRs.push_back("sr14VL"); lepHTs.push_back("[250,450]"); lepNJs.push_back("4+"); lepNBs.push_back("2");
  lepSRs.push_back("sr15VL"); lepHTs.push_back("[250,450]"); lepNJs.push_back("2-3"); lepNBs.push_back("3+");
  
  lepSRs.push_back("sr1L"); lepHTs.push_back("[450,575]"); lepNJs.push_back("2-3"); lepNBs.push_back("0");
  lepSRs.push_back("sr2L"); lepHTs.push_back("[450,575]"); lepNJs.push_back("2-3"); lepNBs.push_back("1");
  lepSRs.push_back("sr3L"); lepHTs.push_back("[450,575]"); lepNJs.push_back("2-3"); lepNBs.push_back("2");
  lepSRs.push_back("sr4L"); lepHTs.push_back("[450,575]"); lepNJs.push_back("4-6"); lepNBs.push_back("0");
  lepSRs.push_back("sr5L"); lepHTs.push_back("[450,575]"); lepNJs.push_back("4-6"); lepNBs.push_back("1");
  lepSRs.push_back("sr6L"); lepHTs.push_back("[450,575]"); lepNJs.push_back("4-6"); lepNBs.push_back("2");
  lepSRs.push_back("sr7L"); lepHTs.push_back("[450,575]"); lepNJs.push_back("7+"); lepNBs.push_back("0");
  lepSRs.push_back("sr8L"); lepHTs.push_back("[450,575]"); lepNJs.push_back("7+"); lepNBs.push_back("1-2");
  lepSRs.push_back("sr10L"); lepHTs.push_back("[450,575]"); lepNJs.push_back("2-6"); lepNBs.push_back("3+");
  
  lepSRs.push_back("sr1M"); lepHTs.push_back("[575,1000]"); lepNJs.push_back("2-3"); lepNBs.push_back("0");
  lepSRs.push_back("sr2M"); lepHTs.push_back("[575,1000]"); lepNJs.push_back("2-3"); lepNBs.push_back("1");
  lepSRs.push_back("sr3M"); lepHTs.push_back("[575,1000]"); lepNJs.push_back("2-3"); lepNBs.push_back("2");
  lepSRs.push_back("sr4M"); lepHTs.push_back("[575,1000]"); lepNJs.push_back("4-6"); lepNBs.push_back("0");
  lepSRs.push_back("sr5M"); lepHTs.push_back("[575,1000]"); lepNJs.push_back("4-6"); lepNBs.push_back("1");
  lepSRs.push_back("sr6M"); lepHTs.push_back("[575,1000]"); lepNJs.push_back("4-6"); lepNBs.push_back("2");
  lepSRs.push_back("sr7M"); lepHTs.push_back("[575,1000]"); lepNJs.push_back("7+"); lepNBs.push_back("0");
  lepSRs.push_back("sr8M"); lepHTs.push_back("[575,1000]"); lepNJs.push_back("7+"); lepNBs.push_back("1-2");
  lepSRs.push_back("sr10M"); lepHTs.push_back("[575,1000]"); lepNJs.push_back("2-6"); lepNBs.push_back("3+");
  
  lepSRs.push_back("sr1H"); lepHTs.push_back("[1000,1500]"); lepNJs.push_back("2-3"); lepNBs.push_back("0");
  lepSRs.push_back("sr2H"); lepHTs.push_back("[1000,1500]"); lepNJs.push_back("2-3"); lepNBs.push_back("1");
  lepSRs.push_back("sr3H"); lepHTs.push_back("[1000,1500]"); lepNJs.push_back("2-3"); lepNBs.push_back("2");
  lepSRs.push_back("sr4H"); lepHTs.push_back("[1000,1500]"); lepNJs.push_back("4-6"); lepNBs.push_back("0");
  lepSRs.push_back("sr5H"); lepHTs.push_back("[1000,1500]"); lepNJs.push_back("4-6"); lepNBs.push_back("1");
  lepSRs.push_back("sr6H"); lepHTs.push_back("[1000,1500]"); lepNJs.push_back("4-6"); lepNBs.push_back("2");
  lepSRs.push_back("sr7H"); lepHTs.push_back("[1000,1500]"); lepNJs.push_back("7+"); lepNBs.push_back("0");
  lepSRs.push_back("sr8H"); lepHTs.push_back("[1000,1500]"); lepNJs.push_back("7+"); lepNBs.push_back("1-2");
  lepSRs.push_back("sr10H"); lepHTs.push_back("[1000,1500]"); lepNJs.push_back("2-6"); lepNBs.push_back("3+");

  lepSRs.push_back("sr1UH"); lepHTs.push_back("[1500,\\infty]"); lepNJs.push_back("2-3"); lepNBs.push_back("0");
  lepSRs.push_back("sr2UH"); lepHTs.push_back("[1500,\\infty]"); lepNJs.push_back("2-3"); lepNBs.push_back("1");
  lepSRs.push_back("sr3UH"); lepHTs.push_back("[1500,\\infty]"); lepNJs.push_back("2-3"); lepNBs.push_back("2");
  lepSRs.push_back("sr4UH"); lepHTs.push_back("[1500,\\infty]"); lepNJs.push_back("4-6"); lepNBs.push_back("0");
  lepSRs.push_back("sr5UH"); lepHTs.push_back("[1500,\\infty]"); lepNJs.push_back("4-6"); lepNBs.push_back("1");
  lepSRs.push_back("sr6UH"); lepHTs.push_back("[1500,\\infty]"); lepNJs.push_back("4-6"); lepNBs.push_back("2");
  lepSRs.push_back("sr7UH"); lepHTs.push_back("[1500,\\infty]"); lepNJs.push_back("7+"); lepNBs.push_back("0");
  lepSRs.push_back("sr8UH"); lepHTs.push_back("[1500,\\infty]"); lepNJs.push_back("7+"); lepNBs.push_back("1-2");
  lepSRs.push_back("sr10UH"); lepHTs.push_back("[1500,\\infty]"); lepNJs.push_back("2-6"); lepNBs.push_back("3+");

}

void setRegionsZinvAll() {
 
  zinvSRs.push_back("sr1VL"); zinvHTs.push_back("[250,450]"); zinvNJs.push_back("2-3"); zinvNBs.push_back("0");
  zinvSRs.push_back("sr2VL"); zinvHTs.push_back("[250,450]"); zinvNJs.push_back("2-3"); zinvNBs.push_back("1");
  zinvSRs.push_back("sr3VL"); zinvHTs.push_back("[250,450]"); zinvNJs.push_back("2-3"); zinvNBs.push_back("2");
  zinvSRs.push_back("sr12VL"); zinvHTs.push_back("[250,450]"); zinvNJs.push_back("4+"); zinvNBs.push_back("0");
  zinvSRs.push_back("sr13VL"); zinvHTs.push_back("[250,450]"); zinvNJs.push_back("4+"); zinvNBs.push_back("1");
  zinvSRs.push_back("sr14VL"); zinvHTs.push_back("[250,450]"); zinvNJs.push_back("4+"); zinvNBs.push_back("2");
  zinvSRs.push_back("sr15VL"); zinvHTs.push_back("[250,450]"); zinvNJs.push_back("2-3"); zinvNBs.push_back("3+");
  
  zinvSRs.push_back("sr1L"); zinvHTs.push_back("[450,575]"); zinvNJs.push_back("2-3"); zinvNBs.push_back("0");
  zinvSRs.push_back("sr2L"); zinvHTs.push_back("[450,575]"); zinvNJs.push_back("2-3"); zinvNBs.push_back("1");
  zinvSRs.push_back("sr3L"); zinvHTs.push_back("[450,575]"); zinvNJs.push_back("2-3"); zinvNBs.push_back("2");
  zinvSRs.push_back("sr4L"); zinvHTs.push_back("[450,575]"); zinvNJs.push_back("4-6"); zinvNBs.push_back("0");
  zinvSRs.push_back("sr5L"); zinvHTs.push_back("[450,575]"); zinvNJs.push_back("4-6"); zinvNBs.push_back("1");
  zinvSRs.push_back("sr6L"); zinvHTs.push_back("[450,575]"); zinvNJs.push_back("4-6"); zinvNBs.push_back("2");
  zinvSRs.push_back("sr7L"); zinvHTs.push_back("[450,575]"); zinvNJs.push_back("7+"); zinvNBs.push_back("0");
  zinvSRs.push_back("sr8L"); zinvHTs.push_back("[450,575]"); zinvNJs.push_back("7+"); zinvNBs.push_back("1");
  zinvSRs.push_back("sr9L"); zinvHTs.push_back("[450,575]"); zinvNJs.push_back("7+"); zinvNBs.push_back("2");
  zinvSRs.push_back("sr10L"); zinvHTs.push_back("[450,575]"); zinvNJs.push_back("2-6"); zinvNBs.push_back("3+");
  zinvSRs.push_back("sr11L"); zinvHTs.push_back("[450,575]"); zinvNJs.push_back("7+"); zinvNBs.push_back("3+");
  
  zinvSRs.push_back("sr1M"); zinvHTs.push_back("[575,1000]"); zinvNJs.push_back("2-3"); zinvNBs.push_back("0");
  zinvSRs.push_back("sr2M"); zinvHTs.push_back("[575,1000]"); zinvNJs.push_back("2-3"); zinvNBs.push_back("1");
  zinvSRs.push_back("sr3M"); zinvHTs.push_back("[575,1000]"); zinvNJs.push_back("2-3"); zinvNBs.push_back("2");
  zinvSRs.push_back("sr4M"); zinvHTs.push_back("[575,1000]"); zinvNJs.push_back("4-6"); zinvNBs.push_back("0");
  zinvSRs.push_back("sr5M"); zinvHTs.push_back("[575,1000]"); zinvNJs.push_back("4-6"); zinvNBs.push_back("1");
  zinvSRs.push_back("sr6M"); zinvHTs.push_back("[575,1000]"); zinvNJs.push_back("4-6"); zinvNBs.push_back("2");
  zinvSRs.push_back("sr7M"); zinvHTs.push_back("[575,1000]"); zinvNJs.push_back("7+"); zinvNBs.push_back("0");
  zinvSRs.push_back("sr8M"); zinvHTs.push_back("[575,1000]"); zinvNJs.push_back("7+"); zinvNBs.push_back("1");
  zinvSRs.push_back("sr9M"); zinvHTs.push_back("[575,1000]"); zinvNJs.push_back("7+"); zinvNBs.push_back("2");
  zinvSRs.push_back("sr10M"); zinvHTs.push_back("[575,1000]"); zinvNJs.push_back("2-6"); zinvNBs.push_back("3+");
  zinvSRs.push_back("sr11M"); zinvHTs.push_back("[575,1000]"); zinvNJs.push_back("7+"); zinvNBs.push_back("3+");
  
  zinvSRs.push_back("sr1H"); zinvHTs.push_back("[1000,1500]"); zinvNJs.push_back("2-3"); zinvNBs.push_back("0");
  zinvSRs.push_back("sr2H"); zinvHTs.push_back("[1000,1500]"); zinvNJs.push_back("2-3"); zinvNBs.push_back("1");
  zinvSRs.push_back("sr3H"); zinvHTs.push_back("[1000,1500]"); zinvNJs.push_back("2-3"); zinvNBs.push_back("2");
  zinvSRs.push_back("sr4H"); zinvHTs.push_back("[1000,1500]"); zinvNJs.push_back("4-6"); zinvNBs.push_back("0");
  zinvSRs.push_back("sr5H"); zinvHTs.push_back("[1000,1500]"); zinvNJs.push_back("4-6"); zinvNBs.push_back("1");
  zinvSRs.push_back("sr6H"); zinvHTs.push_back("[1000,1500]"); zinvNJs.push_back("4-6"); zinvNBs.push_back("2");
  zinvSRs.push_back("sr7H"); zinvHTs.push_back("[1000,1500]"); zinvNJs.push_back("7+"); zinvNBs.push_back("0");
  zinvSRs.push_back("sr8H"); zinvHTs.push_back("[1000,1500]"); zinvNJs.push_back("7+"); zinvNBs.push_back("1");
  zinvSRs.push_back("sr9H"); zinvHTs.push_back("[1000,1500]"); zinvNJs.push_back("7+"); zinvNBs.push_back("2");
  zinvSRs.push_back("sr10H"); zinvHTs.push_back("[1000,1500]"); zinvNJs.push_back("2-6"); zinvNBs.push_back("3+");
  zinvSRs.push_back("sr11H"); zinvHTs.push_back("[1000,1500]"); zinvNJs.push_back("7+"); zinvNBs.push_back("3+");

  zinvSRs.push_back("sr1UH"); zinvHTs.push_back("[1500,\\infty]"); zinvNJs.push_back("2-3"); zinvNBs.push_back("0");
  zinvSRs.push_back("sr2UH"); zinvHTs.push_back("[1500,\\infty]"); zinvNJs.push_back("2-3"); zinvNBs.push_back("1");
  //zinvSRs.push_back("sr3UH"); zinvHTs.push_back("[1500,\\infty]"); zinvNJs.push_back("2-3"); zinvNBs.push_back("2");
  zinvSRs.push_back("sr4UH"); zinvHTs.push_back("[1500,\\infty]"); zinvNJs.push_back("4-6"); zinvNBs.push_back("0");
  zinvSRs.push_back("sr5UH"); zinvHTs.push_back("[1500,\\infty]"); zinvNJs.push_back("4-6"); zinvNBs.push_back("1");
  zinvSRs.push_back("sr6UH"); zinvHTs.push_back("[1500,\\infty]"); zinvNJs.push_back("4-6"); zinvNBs.push_back("2");
  zinvSRs.push_back("sr7UH"); zinvHTs.push_back("[1500,\\infty]"); zinvNJs.push_back("7+"); zinvNBs.push_back("0");
  zinvSRs.push_back("sr8UH"); zinvHTs.push_back("[1500,\\infty]"); zinvNJs.push_back("7+"); zinvNBs.push_back("1");
  zinvSRs.push_back("sr9UH"); zinvHTs.push_back("[1500,\\infty]"); zinvNJs.push_back("7+"); zinvNBs.push_back("2");
  zinvSRs.push_back("sr10UH"); zinvHTs.push_back("[1500,\\infty]"); zinvNJs.push_back("2-6"); zinvNBs.push_back("3+");
  zinvSRs.push_back("sr11UH"); zinvHTs.push_back("[1500,\\infty]"); zinvNJs.push_back("7+"); zinvNBs.push_back("3+");

}

void setRegionsZinvTemplates() {

  zinvSRs.push_back("sr1VL"); zinvHTs.push_back("[250,450]"); zinvNJs.push_back("2-3");
  zinvSRs.push_back("sr15VL"); zinvHTs.push_back("[250,450]"); zinvNJs.push_back("2+");
  zinvSRs.push_back("sr14VL"); zinvHTs.push_back("[250,450]"); zinvNJs.push_back("4+");
  zinvSRs.push_back("sr1L"); zinvHTs.push_back("[450,575]"); zinvNJs.push_back("2-3");
  zinvSRs.push_back("sr10L"); zinvHTs.push_back("[450,575]"); zinvNJs.push_back("2-6");
  zinvSRs.push_back("sr4L"); zinvHTs.push_back("[450,575]"); zinvNJs.push_back("4-6");
  zinvSRs.push_back("sr8L"); zinvHTs.push_back("[450,575]"); zinvNJs.push_back("7+");
  zinvSRs.push_back("sr1M"); zinvHTs.push_back("[575,1000]"); zinvNJs.push_back("2-3");
  zinvSRs.push_back("sr10M"); zinvHTs.push_back("[575,1000]"); zinvNJs.push_back("2-6");
  zinvSRs.push_back("sr4M"); zinvHTs.push_back("[575,1000]"); zinvNJs.push_back("4-6");
  zinvSRs.push_back("sr8M"); zinvHTs.push_back("[575,1000]"); zinvNJs.push_back("7+");
  zinvSRs.push_back("sr1H"); zinvHTs.push_back("[1000,1500]"); zinvNJs.push_back("2-3");
  zinvSRs.push_back("sr10H"); zinvHTs.push_back("[1000,1500]"); zinvNJs.push_back("2-6");
  zinvSRs.push_back("sr4H"); zinvHTs.push_back("[1000,1500]"); zinvNJs.push_back("4-6");
  zinvSRs.push_back("sr8H"); zinvHTs.push_back("[1000,1500]"); zinvNJs.push_back("7+");
  zinvSRs.push_back("sr1UH"); zinvHTs.push_back("[1500,\\infty]"); zinvNJs.push_back("2+");

}

void getHybridTables() {
  
  TFile* lostlepFile = new TFile(dir+"/lostlepFromCRs.root");
  TFile* zinvFile = new TFile(dir+"/zinvFromDY.root");

  setRegionsLepAll();
  setRegionsZinvAll();
  
  cout << "-------------------------------" << endl;
  cout << "\\begin{tabular}{c|c|c|c|c}" << endl;
  cout << "\\hline \\hline" << endl;
  // cout << "\\multicolumn{5}{c}{Lost Lepton} \\\\" << endl;
  // cout << "\\hline" << endl;
  cout << "\\multicolumn{3}{c|}{Control Region} & Extrapolation Point & Last $\\text{M}_{T2}$ bin  \\\\" << endl;
  cout << "$\\text{H}_{T}$ [GeV] & $\\text{N}_{j}$ & $\\text{N}_{b}$ & $\\text{M}_{T2}$ [GeV] & $\\text{M}_{T2}$ [GeV]\\\\" << endl;
  cout << "\\hline" << endl;														
			       
  // TIter it(lostlepFile->GetListOfKeys());
  // TKey* k;
  // while ((k = (TKey *)it())) {
  //     string hist_name = (k->GetTitle());
  //     cout << "SR: " << hist_name;
  //     hist_name += "/h_lastmt2Hybrid";
  //     TH1D* h_lastmt2 = (TH1D*) lostlepFile->Get(TString(hist_name));
  //     int theBin = h_lastmt2->GetBinContent(1);
  //     cout << " mt2: " << theBin << endl;
  // }

  for (unsigned int i = 0; i<lepSRs.size(); i++) {
    string hist_name = lepSRs[i];
    string hist_name_lastmt2 = hist_name + "/h_lastmt2Hybrid";
    string hist_name_mt2bins = hist_name + "/h_mt2bins";
    TH1D* h_lastmt2 = (TH1D*) lostlepFile->Get(TString(hist_name_lastmt2));
    TH1D* h_mt2bins = (TH1D*) lostlepFile->Get(TString(hist_name_mt2bins));
    int theBin = h_lastmt2->GetBinContent(1);
    int highestMT2bin = h_mt2bins->GetBinLowEdge(h_mt2bins->GetNbinsX());
    cout << "$" << lepHTs[i] << "$ &" << lepNJs[i] << "&" << lepNBs[i] << "& $>$ " << theBin << "& $>$ " << highestMT2bin << "\\\\" << endl;
  }
  cout << "\\hline \\hline" << endl;
  cout << "\\end{tabular}" << endl;
  cout << "-------------------------------" << endl;

  cout << "-------------------------------" << endl;
  cout << "\\begin{tabular}{c|c|c|c}" << endl;
  cout << "\\hline \\hline" << endl;
  cout << "\\multicolumn{4}{c}{Invisible Z} \\\\" << endl;
  cout << "\\hline" << endl;
  cout << "\\multicolumn{3}{c|}{Control Region} & Extrapolation Point\\\\" << endl;
  cout << "$\\text{H}_{T}$ [GeV] & $\\text{N}_{j}$ & $\\text{N}_{b}$ & $\\text{M}_{T2}$ [GeV]\\\\" << endl;
  cout << "\\hline" << endl;														

  for (unsigned int i = 0; i<zinvSRs.size(); i++) {
    string hist_name = zinvSRs[i];
    hist_name += "/h_lastmt2Hybrid";
    TH1D* h_lastmt2 = (TH1D*) zinvFile->Get(TString(hist_name));
    int theBin = h_lastmt2->GetBinContent(1);
    cout << "$" << zinvHTs[i] << "$ &" << zinvNJs[i] << "&" << zinvNBs[i] << "&" << theBin << "\\\\" << endl;
  }
  cout << "\\hline \\hline" << endl;
  cout << "\\end{tabular}" << endl;
  cout << "-------------------------------" << endl;
 
}

void getZinvTable() {
  
  TFile* lostlepFile = new TFile(dir+"/lostlepFromCRs.root");
  TFile* zinvFile = new TFile(dir+"/zinvFromDY.root");

  setRegionsZinvAll();
  
  cout << "-------------------------------" << endl;
  cout << "\\begin{tabular}{c|c|c|c|c|c|c|c}" << endl;
  cout << "\\hline \\hline" << endl;
  cout << "\\multicolumn{8}{c}{Invisible Z} \\\\" << endl;
  cout << "\\hline" << endl;
  cout << "\\multicolumn{3}{c|}{Region} & DY Yield & SF Yield & OF Yield & Purity & Ratio\\\\" << endl;
  cout << "$\\text{H}_{T}$ [GeV] & $\\text{N}_{j}$ & $\\text{N}_{b}$ & & & &  \\\\" << endl;
  cout << "\\hline" << endl;														
  cout << std::fixed;
  
  for (unsigned int i = 0; i<zinvSRs.size(); i++) {    
    string srName = zinvSRs[i];
    bool isHighNjNb = (srName.find("8") !=std::string::npos) || (srName.find("9") !=std::string::npos) || (srName.find("11") !=std::string::npos);
    string asterisk = "";
    if (isHighNjNb) asterisk = "$^{(*)}$";
      
    string cr_hist_name = srName+"/CRyieldCard";
    string sf_hist_name = srName+"/h_mt2binsCRyield";
    string of_hist_name = srName+"/h_mt2binsCRyieldEM";
    string purity_hist_name = srName+"/purityCard";
    string ratio_hist_name = srName+"/ratioCard";
    TH1D* h_cr = (TH1D*) zinvFile->Get(TString(cr_hist_name));
    TH1D* h_sf = (TH1D*) zinvFile->Get(TString(sf_hist_name));
    TH1D* h_of = (TH1D*) zinvFile->Get(TString(of_hist_name));
    TH1D* h_purity = (TH1D*) zinvFile->Get(TString(purity_hist_name));
    TH1D* h_ratio = (TH1D*) zinvFile->Get(TString(ratio_hist_name));
    float crBin = 0;
    int sfBin = 0;
    int ofBin = 0;
    if (h_cr) crBin = h_cr->GetBinContent(1);
    if (h_sf) sfBin = h_sf->Integral(0);
    if (h_of) ofBin = h_of->Integral(0);
    float purityBin = h_purity->GetBinContent(1);
    float ratioBin = h_ratio->Integral();
    float purityBinErr = h_purity->GetBinError(1);
    //float ratioBinErr = h_ratio->GetBinError(1);

    //no histogram with DY yield, calculate using purity
    crBin = sfBin*purityBin;
    if (purityBin == 0) crBin = 1; //exception for edge case with 1 SF, 1 OF event in cr10H
    
    cout << "$" << zinvHTs[i] << "$ &" << zinvNJs[i] << "&" << zinvNBs[i] << "&";
    cout << std::setprecision(1) << crBin << "&";
    cout << sfBin << "&";
    cout << ofBin << "&";
    // cout << std::setprecision(3) << purityBin << "&";
    cout << std::setprecision(3) << purityBin << "$\\pm$" << purityBinErr << "&";
    cout << std::setprecision(2) << ratioBin << asterisk << "\\\\" << endl;
    // cout << std::setprecision(2) << ratioBin << "$\\pm$" << ratioBinErr << "\\\\" << endl;
  }
  cout << "\\hline \\hline" << endl;
  cout << "\\end{tabular}" << endl;
  cout << "-------------------------------" << endl;
 
}

void getHybridPoint() {

  getZinvTable();
  //getHybridTables();
  
}

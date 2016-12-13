#include <iostream>
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
void setRegions();

vector<string> lepSRs;
vector<string> lepHTs;
vector<string> lepNJs;
vector<string> lepNBs;


vector<string> zinvSRs;
vector<string> zinvHTs;
vector<string> zinvNJs;

void getHybridPoint() {

  TString dir = "/home/users/mderdzinski/summer2016/current_master_mt2/MT2looper/output/36p26_v10MC_dec5/";
  
  TFile* lostlepFile = new TFile(dir+"/lostlepFromCRs.root");
  TFile* zinvFile = new TFile(dir+"/zinvFromDY.root");

  setRegions();
  
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
  cout << "\\begin{tabular}{c|c|c}" << endl;
  cout << "\\hline \\hline" << endl;
  cout << "\\multicolumn{3}{c}{Invisible Z} \\\\" << endl;
  cout << "\\hline" << endl;
  cout << "\\multicolumn{2}{c|}{Template Region} & Extrapolation Point\\\\" << endl;
  cout << "$\\text{H}_{T}$ [GeV] & $\\text{N}_{j}$ & $\\text{M}_{T2}$ [GeV]\\\\" << endl;
  cout << "\\hline" << endl;														

  for (unsigned int i = 0; i<zinvSRs.size(); i++) {
    string hist_name = zinvSRs[i];
    hist_name += "/h_lastmt2Hybrid";
    TH1D* h_lastmt2 = (TH1D*) zinvFile->Get(TString(hist_name));
    int theBin = h_lastmt2->GetBinContent(1);
    cout << "$" << zinvHTs[i] << "$ &" << zinvNJs[i] << "&" << theBin << "\\\\" << endl;
  }
  cout << "\\hline \\hline" << endl;
  cout << "\\end{tabular}" << endl;
  cout << "-------------------------------" << endl;
 
}

void setRegions() {

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

//script to make purity.root file
//which contains purity histograms and predicitions for Zinv
//using w->lnu w/ removed lepton

#include <string>
#include <iostream>
#include <algorithm>
#include <map>
#include <sstream>
#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TStyle.h"
#include "TH1D.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TString.h"
#include "TLegend.h"
#include "TKey.h"
#include "TEfficiency.h"

#include "../MT2CORE/mt2tree.h"
#include "../MT2CORE/sigSelections.h"
#include "../MT2CORE/SR.h"
#include "../MT2CORE/sigSelections.cc"
#include "../MT2CORE/SR.cc"

using namespace std;
using namespace mt2;

void makePred(TFile* f_out, TFile* f_w, TFile* f_bkg, TFile* f_z, SR sr)
{
  
  //sr name
  TString srName = sr.GetName();

  cout << "Signal Region " << srName << "..." << endl;
  
  TDirectory * dir = (TDirectory*)f_out->Get("sr"+srName);
  if (dir == 0) {
    dir = f_out->mkdir("sr"+srName);
  } 
  dir->cd();

  //if no crrl directory, exit
  if (!f_w->GetDirectory("crrl"+srName)) return;
  if (!f_bkg->GetDirectory("crrl"+srName)) cout << "no h_bkg" << endl;
  
  //get hists for FR calc
  TH1D* h_w = (TH1D*) f_w->Get("crrl"+srName+"/h_mt2bins");
  TH1D* h_bkg = (TH1D*) f_bkg->Get("crrl"+srName+"/h_mt2bins");
  h_w->SetName("h_w");
  h_bkg->SetName("h_bkg");\

  //if no w->lnu, exit
  if (h_w->GetEntries() == 0 ) return;

  //add all to make total hist
  TH1D* h_total = (TH1D*) h_w->Clone();
  h_total->Reset();
  h_total->SetName("h_total");
  h_total->Add(h_w,h_bkg);

  //make another total hist with 150% bkg, for syst.
  TH1D* h_totalplus50 = (TH1D*) h_w->Clone();
  h_totalplus50->Reset();
  h_totalplus50->SetName("h_totalplus50");
  h_totalplus50->Add(h_w,h_bkg,1,1.5);

  //get h_data hist
  //in future, replace with data
  TH1D* h_data = (TH1D*) h_total->Clone();
  h_data->GetSumw2()->Set(0);
  h_data->Sumw2();

  //make purity plot, straight from MC
  TH1D* h_purity = (TH1D*) h_w->Clone();
  h_purity->Reset();
  h_purity->SetName("h_purity");
  h_purity->Divide(h_w,h_total,1,1,"B");

  //make syst plot (with +-50% bkg)
  TH1D* h_syst = (TH1D*) h_w->Clone();
  h_syst->Reset();
  h_syst->SetName("h_syst");
  h_syst->Divide(h_w,h_totalplus50,1,1,"B");
  
  //recalculate purity errors, summing stat and syst.
  for (int bin = 0; bin <= h_purity->GetNbinsX(); bin++){
    double stat = h_purity->GetBinError(bin);
    double syst = h_syst-> GetBinError(bin);
    h_purity->SetBinError(bin,pow(pow(stat,2)+pow(syst,2),0.5));
  }

  //make R(Z/W) histogram
  TH1D* h_z = (TH1D*) f_z->Get("sr"+srName+"/h_mt2bins");
  h_z->SetName("h_z");

  TH1D* h_ratio = (TH1D*) h_w->Clone();
  h_ratio->Reset();
  h_ratio->SetName("h_ratio");
  h_ratio->Divide(h_z,h_w,1,1,"B");

  //make predicition for this SR
  //predicted number of real w->lnu
  TH1D* h_predW = (TH1D*) h_w->Clone();
  h_predW->Reset();
  h_predW->SetName("h_predW");
  h_predW->Multiply(h_data,h_purity,1,1,"B");

  //predicted number of zinv
  TH1D* h_predZ = (TH1D*) h_w->Clone();
  h_predZ->Reset();
  h_predZ->SetName("h_predZ");
  h_predZ->Multiply(h_predW,h_ratio,1,1,"B");

  //save it all
  h_w->Write();
  h_bkg->Write();
  h_z->Write();
  h_data->Write();
  h_purity->Write();
  h_ratio->Write();
  h_predW->Write();
  h_predZ->Write();   

  return ;
}


void purityRL(string input_dir = "/nfs-5/users/mderdzinski/spring2015/mt2/MT2Analysis/MT2looper/output/CRRLmuonskims")
{
  
  //load signal regions
  vector<SR> SRVec =  getSignalRegionsZurich_jetpt30();

  //open files
  TFile* f_w = new TFile(Form("%s/wjets_ht.root",input_dir.c_str())); //wjet file
  // TFile* f_q = new TFile(Form("%s/qcd_pt.root",input_dir.c_str())); //qcd file
  // TFile* f_t = new TFile(Form("%s/ttall.root",input_dir.c_str())); //ttbar file
  // TFile* f_s = new TFile(Form("%s/singletop.root",input_dir.c_str())); //singletop file
  TFile* f_z = new TFile(Form("%s/zinv_ht.root",input_dir.c_str())); //zinv file
  TFile* f_bkg = new TFile(Form("%s/CRRLbkg.root",input_dir.c_str())); //qcd+ttall+singletop file
  
  if(f_w->IsZombie() || f_bkg->IsZombie() ||f_z->IsZombie()) {
    std::cerr << "Input file does not exist" << std::endl;
    return;
  }

  TFile* f_out = new TFile(Form("%s/purity.root",input_dir.c_str()),"RECREATE");
  
  cout << "Making Purity Histograms..." << endl;

  for(int i = 0; i< (int) SRVec.size(); i++){
    makePred(f_out, f_w, f_bkg, f_z, SRVec[i]);
  }

  cout << "Saving and closing..." << endl;
  f_out->Close();
  
  return ;
}


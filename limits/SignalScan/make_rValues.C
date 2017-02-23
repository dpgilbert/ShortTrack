#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"

#include <iostream>
#include <vector>



void Fill2d(TH2F *&hist, double w, double x, double y){   
  x = std::min(hist->GetXaxis()->GetBinCenter(hist->GetXaxis()->GetLast()) , x); 
  x = std::max(hist->GetXaxis()->GetBinCenter(hist->GetXaxis()->GetFirst()), x); 
  y = std::min(hist->GetYaxis()->GetBinCenter(hist->GetYaxis()->GetLast()) , y); 
  y = std::max(hist->GetYaxis()->GetBinCenter(hist->GetYaxis()->GetFirst()), y); 
  hist->Fill(x, y, w); 
  cout<<"Filling "<<x<<", "<<y<<" with weight "<<w<<endl;
}

void make_rValues(std::string indir, std::string model, int m1, int m2){
  //setup file to hold r-values if it doesn't already exist
  TFile *f_temp = new TFile(Form("r-values_%s.root", model.c_str()), "NEW");
  if(f_temp){
    int xlow = 000;
    int xhigh = 2200;
    int ylow = 0;
    int yhigh = 1600;
    int stepsizeX = 25;
    int stepsizeY = 25;
    int nbinsx = (xhigh-xlow+stepsizeX)/stepsizeX;
    int nbinsy = (yhigh-ylow+2*stepsizeY)/stepsizeY;
    if ( model.find("T2-4bd") != std::string::npos) {
      nbinsy = 160;
      ylow = 0;
      yhigh = 800;
    }
    if ( model.find("T2cc") != std::string::npos ) {
      stepsizeY = 5;
      ylow = 0;
      yhigh = 800;
      nbinsy = (yhigh-ylow+2*stepsizeY)/stepsizeY;
    }
    
    // TH2F *hExp   = new TH2F("hExp",   "hExp"  , nbinsx, xlow, xhigh, nbinsy, ylow, yhigh);
    // TH2F *hObs   = new TH2F("hObs",   "hObs"  , nbinsx, xlow, xhigh, nbinsy, ylow, yhigh);
    // TH2F *hExp1m = new TH2F("hExp1m", "hExp1m", nbinsx, xlow, xhigh, nbinsy, ylow, yhigh);
    // TH2F *hExp2m = new TH2F("hExp2m", "hExp2m", nbinsx, xlow, xhigh, nbinsy, ylow, yhigh);
    // TH2F *hExp1p = new TH2F("hExp1p", "hExp1p", nbinsx, xlow, xhigh, nbinsy, ylow, yhigh);
    // TH2F *hExp2p = new TH2F("hExp2p", "hExp2p", nbinsx, xlow, xhigh, nbinsy, ylow, yhigh);
    
    TH2F *hExp   = new TH2F("hExp",   "hExp"  , nbinsx, xlow-stepsizeX/2, xhigh+stepsizeX/2, nbinsy, ylow-3*stepsizeY/2, yhigh+stepsizeY/2);
    TH2F *hObs   = new TH2F("hObs",   "hObs"  , nbinsx, xlow-stepsizeX/2, xhigh+stepsizeX/2, nbinsy, ylow-3*stepsizeY/2, yhigh+stepsizeY/2);
    TH2F *hExp1m = new TH2F("hExp1m", "hExp1m", nbinsx, xlow-stepsizeX/2, xhigh+stepsizeX/2, nbinsy, ylow-3*stepsizeY/2, yhigh+stepsizeY/2);
    TH2F *hExp2m = new TH2F("hExp2m", "hExp2m", nbinsx, xlow-stepsizeX/2, xhigh+stepsizeX/2, nbinsy, ylow-3*stepsizeY/2, yhigh+stepsizeY/2);
    TH2F *hExp1p = new TH2F("hExp1p", "hExp1p", nbinsx, xlow-stepsizeX/2, xhigh+stepsizeX/2, nbinsy, ylow-3*stepsizeY/2, yhigh+stepsizeY/2);
    TH2F *hExp2p = new TH2F("hExp2p", "hExp2p", nbinsx, xlow-stepsizeX/2, xhigh+stepsizeX/2, nbinsy, ylow-3*stepsizeY/2, yhigh+stepsizeY/2);

    f_temp->Write();
    f_temp->Close();
  }
  delete f_temp;

  //This file is created earlier by running combine
  TFile *limit_file = new TFile(Form("%s/limit_%s_%d_%d.root", indir.c_str(), model.c_str(), m1, m2), "READ");
  if (limit_file->IsZombie()) {
    cout<<"File not found, exiting: "<<Form("%s/limit_%s_%d_%d.root", indir.c_str(), model.c_str(), m1, m2)<<endl;
    return;
  }
  TTree *limit_tree = (TTree*)limit_file->Get("limit");



  //This file is created earlier by running combine
  //TFile *significance_file = new TFile(Form("significance_%s_%d_%d.root", model.c_str(), m1, m2), "READ");
  //TTree *significance_tree = (TTree*)significance_file->Get("limit");

  //This file will hold 2d histograms with limit r-values and discovery significance
  TFile *f = new TFile(Form("r-values_%s.root", model.c_str()), "UPDATE");

  double value = -1.0;
  limit_tree->SetBranchAddress("limit", &value);

  double limit   = -1.0; //observed limit
  double rm2s    = -1.0; //expected - 2 sigma
  double rm1s    = -1.0; //expected - 1 sigma
  double rmedian = -1.0; //expected limit
  double rp1s    = -1.0; //expected + 1 sigma
  double rp2s    = -1.0; //expected + 2 sigma

  for(int i=0; i< 6; i++){
    limit_tree->GetEntry(i);
    if(i==0)      rm2s    = value;
    else if(i==1) rm1s    = value;
    else if(i==2) rmedian = value;
    else if(i==3) rp1s    = value;
    else if(i==4) rp2s    = value;
    else if(i==5) limit   = value;
  }

  //double sig = -1.0;
  //significance_tree->SetBranchAddress("limit", &sig);
  //significance_tree->GetEntry(0);

  delete limit_tree;
  limit_file->Close();
  delete limit_file;

  //delete significance_tree;
  //significance_file->Close();
  //delete significance_file;

  f->cd();

  TH2F *hExp   = (TH2F*)f->Get("hExp");
  TH2F *hObs   = (TH2F*)f->Get("hObs");
  TH2F *hExp1m = (TH2F*)f->Get("hExp1m");
  TH2F *hExp2m = (TH2F*)f->Get("hExp2m");
  TH2F *hExp1p = (TH2F*)f->Get("hExp1p");
  TH2F *hExp2p = (TH2F*)f->Get("hExp2p");
  //TH2F *hSig   = (TH2F*)f->Get("hSig");

  Fill2d(hExp  , rmedian, m1, m2);    
  Fill2d(hObs  , limit  , m1, m2);  
  Fill2d(hExp1m, rm1s   , m1, m2);  
  Fill2d(hExp2m, rm2s   , m1, m2);  
  Fill2d(hExp1p, rp1s   , m1, m2);  
  Fill2d(hExp2p, rp2s   , m1, m2);  
  //Fill2d(hSig  , sig    , m1, m2);  

  hExp  ->Write("",TObject::kOverwrite);
  hObs  ->Write("",TObject::kOverwrite);
  hExp1m->Write("",TObject::kOverwrite);
  hExp2m->Write("",TObject::kOverwrite);
  hExp1p->Write("",TObject::kOverwrite);
  hExp2p->Write("",TObject::kOverwrite);
  //hSig  ->Write("",TObject::kOverwrite);
  f->Close();
  delete f;

}

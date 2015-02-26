#include "TFile.h"
#include "TH1F.h"
#include "TH2F.h"
#include "TTree.h"

#include <iostream>
#include <fstream>
#include <vector>


float GetValue(float xvalue, float yvalue, TH2F *&hist){
  float xmax = hist->GetXaxis()->GetXmax()-0.01;
  int xbin   = hist->GetXaxis()->FindBin(min(xvalue, xmax));
  int ybin   = hist->GetYaxis()->FindBin(yvalue);
  return hist->GetBinContent(xbin, ybin);
}


void getGridPoints(std::string model, int m1, int m2){

  TFile *f = new TFile(Form("r-values_%s.root", model.c_str()), "READ");

  TH2F *hp1 = (TH2F*)f->Get("hExp1p");
  TH2F *hm1 = (TH2F*)f->Get("hExp1m");

  float upper = GetValue(m1, m2, hp1);
  float lower = GetValue(m1, m2, hm1);

  float nDiv = 10;
  float delta = (upper - lower)/(nDiv-1);

  ofstream outfile;
  outfile.open (Form("gridPoints_%s_%d_%d.txt", model.c_str(), m1, m2));

  for(int i=0; i<nDiv; i++){
    outfile << std::setprecision(3) << lower + i*delta << std::endl;
  }

  outfile.close();

}

#include "TFile.h"
#include "TString.h"
#include <fstream>
#include <iostream>

void make_xsec_hist() {

  float sf = 1.;
  TString sparticle = "gluino";

  //float sf = 0.8;
  //TString sparticle = "squark";

  //float sf = 1.;
  //TString sparticle = "stop";

  TFile* f = new TFile("xsec_susy_13tev.root","RECREATE");
  //TFile* f = new TFile("xsec_susy_13tev.root","UPDATE");
  TH1F* h = new TH1F(Form("h_xsec_%s",sparticle.Data()),Form(";M(%s) [GeV];xsec [pb]",sparticle.Data()), 561, 197.5, 3002.5);
  //TH1F* h = new TH1F(Form("h_xsec_%s",sparticle.Data()),Form(";M(%s) [GeV];xsec [pb]",sparticle.Data()), 381, 97.5, 2002.5);

  int nlines = 0;
  std::ifstream xsecifs(Form("xsec_%s_13tev.dat",sparticle.Data()));
  float mass;
  float xsec,err;
  while (xsecifs >> mass >> xsec >> err) {
    int bin = h->FindBin(mass);
    h->SetBinContent(bin,xsec * sf);
    h->SetBinError(bin,err/100. * xsec * sf);
    ++nlines;
  }
  xsecifs.close();
  f->Write();
  f->Close();

  std::cout << "read " << nlines << " masses" << std::endl;

}

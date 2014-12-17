#include <iostream>
#include <utility>
#include <vector>
#include <fstream>

#include "TROOT.h"
#include "TLatex.h"
#include "TString.h"
#include "TFile.h"
#include "TH1.h"

using namespace std;

TFile* f_lostlep = 0;
TFile* f_zinv = 0;
TFile* f_qcd = 0;
TFile* f_sig = 0;

string input_dir = "/home/olivito/cms3/MT2Analysis/MT2looper/output/V00-00-07_sel2015LowLumi/";

string output_dir = "./cards/";

string signame = "";

//_______________________________________________________________________________
void printCard( int sr , string htbin , int mt2bin ) {

  // read off yields from h_mt2bins hist in each topological region

  TString dir = Form("sr%d%s",sr,htbin.c_str());
  TString fullhistname = dir + "/h_mt2bins";

  TString cardname = Form("%s/card_%s_m%d_%s.txt",output_dir.c_str(),dir.Data(),mt2bin,signame.c_str());

  double n_lostlep(0.);
  double n_lostlep_tr(0.);
  double n_zinv(0.);
  double n_qcd(0.);
  double n_bkg(0.);

  double n_sig(0.);

  // get yields for each sample
  TH1D* h_lostlep = (TH1D*) f_lostlep->Get(fullhistname);
  if (h_lostlep != 0) {
    n_lostlep = h_lostlep->GetBinContent(mt2bin);
    n_lostlep_tr = h_lostlep->Integral(0,-1);
  } 
  TH1D* h_zinv = (TH1D*) f_zinv->Get(fullhistname);
  if (h_zinv != 0) n_zinv = h_zinv->GetBinContent(mt2bin);
  TH1D* h_qcd = (TH1D*) f_qcd->Get(fullhistname);
  if (h_qcd != 0) n_qcd = h_qcd->GetBinContent(mt2bin);
  TH1D* h_sig = (TH1D*) f_sig->Get(fullhistname);
  if (h_sig != 0) n_sig = h_sig->GetBinContent(mt2bin);

  n_bkg = n_lostlep+n_zinv+n_qcd;

  // ----- lost lepton bkg uncertainties
  // uncorrelated across TRs and MT2 bins
  double lostlep_shape = 1.075;
  TString name_lostlep_shape = Form("LL_SHAPE_%s_m%d",dir.Data(),mt2bin);
  // correlated for MT2 bins in a TR
  double lostlep_crstat = 1. + sqrt( pow(0.15,2) + pow(1./sqrt(n_lostlep_tr),2));
  TString name_lostlep_crstat = Form("LL_CRSTAT_%s",dir.Data());

  // ----- zinv bkg uncertainties - depend on signal region, b selection
  // uncorrelated across TRs and MT2 bins
  double zinv_crstat = -1.;
  TString name_zinv_crstat = Form("ZINV_CRSTAT_%s_m%d",dir.Data(),mt2bin);
  // correlated for all bins with 0-1b
  double zinv_zgamma = -1.;
  TString name_zinv_zgamma = "ZINV_ZGAMMA";
  // correlated for MT2 bins in a TR
  double zinv_bratio = -1.;
  TString name_zinv_bratio = Form("ZINV_BRATIO_%s",dir.Data());
  // uncorrelated across TRs and MT2 bins
  double zinv_mcsyst = -1.;
  TString name_zinv_mcsyst = Form("ZINV_MCSYST_%s_m%d",dir.Data(),mt2bin);
  // 2+b: pure MC estimate
  if (sr == 3 || sr == 4 || sr >= 7) {
    zinv_mcsyst = 2.;
  }
  // 1b: data CR with 0->1b sf
  else if (sr == 2 || sr == 6) {
    zinv_crstat = 1. + 1./sqrt(20. * n_zinv);
    zinv_zgamma = 1.20;
    zinv_bratio = 1.30;
  }
  // 0b: data CR
  else if (sr == 1 || sr == 5) {
    zinv_crstat = 1. + 1./sqrt(2. * n_zinv);
    zinv_zgamma = 1.20;
  }

  // ----- qcd bkg uncertainties: uncorrelated for all bins
  double qcd_syst = 2.00;
  TString name_qcd_syst = Form("QCD_SYST_%s_m%d",dir.Data(),mt2bin);

  // ----- sig uncertainties: correlated for all bins
  double sig_syst = 1.10;

  ofstream* ofile = new ofstream();
  ofile->open(cardname);

  *ofile <<  "imax 1  number of channels"                                                    << endl;
  *ofile <<  "jmax 3  number of backgrounds"                                                 << endl;
  //  *ofile <<  "kmax 2  number of nuisance parameters (sources of systematical uncertainties)" << endl;
  *ofile <<  "------------"                                                                  << endl;
  *ofile <<  "bin         1"                                                                 << endl;
  *ofile <<  Form("observation %.0f",n_bkg)                                                  << endl;
  *ofile <<  "------------"                                                                  << endl;
  *ofile <<  "bin             1      1        1      1"                                      << endl;
  *ofile <<  "process       signal  lostlep  zinv   qcd"                                     << endl; 
  *ofile <<  "process         0      1        2      3"                                      << endl;
  *ofile <<  Form("rate            %.1f    %.1f     %.1f    %.1f",n_sig,n_lostlep,n_zinv,n_qcd) << endl;
  *ofile <<  "------------"                                                                  << endl;
  *ofile <<  Form("SIG_SYST              lnN   %.2f    -      -     -     uncertainty on signal",sig_syst)  << endl;
  *ofile <<  Form("%s     lnN    -   %.3f     -     -     lost lepton shape uncert",name_lostlep_shape.Data(),lostlep_shape)  << endl;
  *ofile <<  Form("%s       lnN    -   %.3f     -     -     lost lepton CR stats/lep eff",name_lostlep_crstat.Data(),lostlep_crstat)  << endl;
  if (zinv_crstat > 0.) {
    *ofile <<  Form("%s  lnN    -      -    %.3f   -     zinv CR stats",name_zinv_crstat.Data(),zinv_crstat)  << endl;
  }
  if (zinv_zgamma > 0.) {
    *ofile <<  Form("%s          lnN    -      -    %.2f    -     zinv Z/gamma ratio",name_zinv_zgamma.Data(),zinv_zgamma)  << endl;
  }
  if (zinv_bratio > 0.) {
    *ofile <<  Form("%s     lnN    -      -    %.2f    -     zinv 0/1b ratio",name_zinv_bratio.Data(),zinv_bratio)  << endl;
  }
  if (zinv_mcsyst > 0.) {
    *ofile <<  Form("%s  lnN    -      -    %.2f    -     zinv MC syst",name_zinv_mcsyst.Data(),zinv_mcsyst)  << endl;
  }
  *ofile <<  Form("%s     lnN    -      -       -   %.2f    QCD syst",name_qcd_syst.Data(),qcd_syst)  << endl;

  ofile->close();

  std::cout << "Wrote card: " << cardname << std::endl;

  return;
}

//_______________________________________________________________________________
void cardMaker(string signal = "T1tttt_1500_100"){

  signame = signal;

  // ----------------------------------------
  //  samples definition
  // ----------------------------------------

  // set input files

  f_lostlep = new TFile(Form("%s/lostlep.root",input_dir.c_str()));
  f_zinv = new TFile(Form("%s/zinv_ht.root",input_dir.c_str()));
  f_qcd = new TFile(Form("%s/qcd_pt.root",input_dir.c_str()));

  f_sig = new TFile(Form("%s/%s.root",input_dir.c_str(),signame.c_str()));

  // ----------------------------------------
  //  cards definitions
  // ----------------------------------------

  // printCard(1, "L",1);
  // printCard(2, "L",1);
  // printCard(3, "L",1);

  // loop over SRs and MT2 bins, print all cards
  const unsigned int n_sr = 10;
  const unsigned int n_mt2bins = 5;
  for (unsigned int isr = 1; isr <= n_sr; ++isr) {
    for (unsigned int imt2 = 1; imt2 <= n_mt2bins; ++imt2) {
      // only do lowest 2 mt2 bins for regions with low minMT
      if (imt2 > 2 && (isr == 3 || isr == 7 || isr == 9)) continue;
      printCard(isr, "L", imt2);
      printCard(isr, "M", imt2);
      printCard(isr, "H", imt2);
    }
  }

}

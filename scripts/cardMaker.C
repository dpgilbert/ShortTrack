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

bool suppressZeroBins = true;

bool iteration1 = false; // Here we try to get realistic statistical errors from control region statistics
                         // To use this option, first run the ZinvMaker.C and lostlepMaker.C

//_______________________________________________________________________________
void printCard( int sr , string htbin , int mt2bin , string signal, string output_dir) {

  // read off yields from h_mt2bins hist in each topological region

  TString dir = Form("sr%d%s",sr,htbin.c_str());
  TString fullhistname = dir + "/h_mt2bins";
  TString fullhistnameStat  = fullhistname+"Stat";

  TString cardname = Form("%s/card_%s_m%d_%s.txt",output_dir.c_str(),dir.Data(),mt2bin,signal.c_str());

  double n_lostlep(0.);
  double n_lostlep_tr(0.);
  double err_lostlep_stat(0.);
  double n_zinv(0.);
  double err_zinv_stat(0.);
  double n_qcd(0.);
  double n_bkg(0.);

  double n_sig(0.);

  // get yields for each sample
  // !!!!! HACK: set zero bins to 0.01 for now to make combine happy
  TH1D* h_lostlep = (TH1D*) f_lostlep->Get(fullhistname);
  if (h_lostlep != 0) {
    n_lostlep = h_lostlep->GetBinContent(mt2bin);
    n_lostlep_tr = h_lostlep->Integral(0,-1);
  } 
  else {
    n_lostlep = 0.01;
    n_lostlep_tr = 0.01;
  }
  if (iteration1) {
    // stat error comes from CR -> CR stats already integrated over MT2 bins in input
    TH1D* h_lostlep_stat = (TH1D*) f_lostlep->Get(fullhistnameStat);
    if (h_lostlep_stat != 0 && h_lostlep_stat->GetBinContent(mt2bin) != 0) 
      err_lostlep_stat = h_lostlep_stat->GetBinError(mt2bin)/h_lostlep_stat->GetBinContent(mt2bin);
  }
  TH1D* h_zinv = (TH1D*) f_zinv->Get(fullhistname);
  if (h_zinv != 0) n_zinv = h_zinv->GetBinContent(mt2bin);
  else n_zinv = 0.01;
  if (iteration1) {
    // For 1b regions, use the statistical uncertainty from corresponding 0b region
    // if (sr == 2) fullhistnameStat.ReplaceAll("sr2", "sr1");
    // if (sr == 6) fullhistnameStat.ReplaceAll("sr6", "sr5");
    TH1D* h_zinv_stat = (TH1D*) f_zinv->Get(fullhistnameStat);
    if (h_zinv_stat != 0 && h_zinv_stat->GetBinContent(mt2bin) != 0) 
      err_zinv_stat = h_zinv_stat->GetBinError(mt2bin)/h_zinv_stat->GetBinContent(mt2bin);
  }
  TH1D* h_qcd = (TH1D*) f_qcd->Get(fullhistname);
  if (h_qcd != 0) n_qcd = h_qcd->GetBinContent(mt2bin);
  else n_qcd = 0.01;
  TH1D* h_sig = (TH1D*) f_sig->Get(fullhistname);
  if (h_sig != 0) n_sig = h_sig->GetBinContent(mt2bin);

  n_bkg = n_lostlep+n_zinv+n_qcd;

  if (suppressZeroBins && n_sig < 0.01) {
    std::cout << "Zero signal, card not printed: " << cardname << std::endl;
    return;
  }

  int n_syst = 0;

  // ----- lost lepton bkg uncertainties
  // uncorrelated across TRs and MT2 bins
  double lostlep_shape = 1.075;
  TString name_lostlep_shape = Form("LL_SHAPE_%s_m%d",dir.Data(),mt2bin);
  ++n_syst;
  // correlated for MT2 bins in a TR
  double lostlep_crstat = 1.;
  if (iteration1) { // iteration1: use CR stats
    if (err_lostlep_stat > 0.) lostlep_crstat = 1. + sqrt( pow(0.15,2) + pow(err_lostlep_stat,2));
    else lostlep_crstat = 1. + sqrt( pow(0.15,2) + 1.);
  }
  else { // iteration0: emulate CR stats
    if (n_lostlep_tr > 0.) lostlep_crstat = 1. + sqrt( pow(0.15,2) + pow(1./sqrt(n_lostlep_tr),2));
    else lostlep_crstat = 1. + sqrt( pow(0.15,2) + 1.);
  }
  TString name_lostlep_crstat = Form("LL_CRSTAT_%s",dir.Data());
  if (iteration1) {
    TString crdir = getCorrelatedSLCRs(dir);
    name_lostlep_crstat = Form("LL_CRSTAT_%s",crdir.Data());
  }
  ++n_syst;

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
    ++n_syst;
  }
  // 1b: data CR with 0->1b sf
  else if (sr == 2 || sr == 6) {
    if (n_zinv > 0.) {
      if (iteration1) zinv_crstat = 1. + 1.*err_zinv_stat;
      else zinv_crstat = 1. + 1./sqrt(20. * n_zinv);
    }
    else zinv_crstat = 2.;
    zinv_zgamma = 1.20;
    if (iteration1) zinv_bratio = 1.00;
    else zinv_bratio = 1.30;
    n_syst += 3;
  }
  // 0b: data CR
  else if (sr == 1 || sr == 5) {
    if (n_zinv > 0.) {
      if (iteration1) zinv_crstat = 1. + 1.*err_zinv_stat;
      else zinv_crstat = 1. + 1./sqrt(2. * n_zinv);
    }
    else zinv_crstat = 2.;
    zinv_zgamma = 1.20;
    n_syst += 2;
  }

  // ----- qcd bkg uncertainties: uncorrelated for all bins
  double qcd_syst = 2.00;
  TString name_qcd_syst = Form("QCD_SYST_%s_m%d",dir.Data(),mt2bin);
  ++n_syst;

  // ----- sig uncertainties: correlated for all bins
  double sig_syst = 1.10;
  ++n_syst;

  ofstream* ofile = new ofstream();
  ofile->open(cardname);

  *ofile <<  "imax 1  number of channels"                                                    << endl;
  *ofile <<  "jmax 3  number of backgrounds"                                                 << endl;
  *ofile <<  Form("kmax %d  number of nuisance parameters",n_syst)                           << endl;
  *ofile <<  "------------"                                                                  << endl;
  *ofile <<  "bin         1"                                                                 << endl;
  *ofile <<  Form("observation %.0f",n_bkg)                                                  << endl;
  *ofile <<  "------------"                                                                  << endl;
  *ofile <<  "bin             1      1        1      1"                                      << endl;
  *ofile <<  "process       signal  lostlep  zinv   qcd"                                     << endl; 
  *ofile <<  "process         0      1        2      3"                                      << endl;
  *ofile <<  Form("rate            %.2f    %.2f     %.2f    %.2f",n_sig,n_lostlep,n_zinv,n_qcd) << endl;
  *ofile <<  "------------"                                                                  << endl;
  *ofile <<  Form("SIG_SYST              lnN   %.2f    -      -     -     uncertainty on signal",sig_syst)  << endl;
  *ofile <<  Form("%s     lnN    -   %.3f     -     -     lost lepton shape uncert",name_lostlep_shape.Data(),lostlep_shape)  << endl;
  *ofile <<  Form("%s       lnN    -   %.3f     -     -     lost lepton CR stats/lep eff",name_lostlep_crstat.Data(),lostlep_crstat)  << endl;
  if (sr == 1 || sr == 5 || sr == 2 || sr == 6) {
    *ofile <<  Form("%s  lnN    -      -    %.3f   -     zinv CR stats",name_zinv_crstat.Data(),zinv_crstat)  << endl;
  }
  if (sr == 1 || sr == 5 || sr == 2 || sr == 6) {
    *ofile <<  Form("%s          lnN    -      -    %.2f    -     zinv Z/gamma ratio",name_zinv_zgamma.Data(),zinv_zgamma)  << endl;
  }
  if (sr == 2 || sr == 6) {
    *ofile <<  Form("%s     lnN    -      -    %.2f    -     zinv 0/1b ratio",name_zinv_bratio.Data(),zinv_bratio)  << endl;
  }
  if (sr == 3 || sr == 4 || sr >= 7) {
    *ofile <<  Form("%s  lnN    -      -    %.2f    -     zinv MC syst",name_zinv_mcsyst.Data(),zinv_mcsyst)  << endl;
  }
  *ofile <<  Form("%s     lnN    -      -       -   %.2f    QCD syst",name_qcd_syst.Data(),qcd_syst)  << endl;

  ofile->close();

  std::cout << "Wrote card: " << cardname << std::endl;

  return;
}

//_______________________________________________________________________________
TString getCorrelatedSLCRs(const TString& dir) {

  // combine loMT and hiMT bins for regions split by that
  if (dir.Contains("3L") || dir.Contains("4L")) {
    return "sr3L4L";
  }
  else if (dir.Contains("3M") || dir.Contains("4M")) {
    return "sr3M4M";
  }
  else if (dir.Contains("3H") || dir.Contains("4H")) {
    return "sr3H4H";
  }

  // combine loMT and hiMT bins for regions split by that
  if (dir.Contains("7L") || dir.Contains("8L")) {
    return "sr7L8L";
  }
  else if (dir.Contains("7M") || dir.Contains("8M")) {
    return "sr7M8M";
  }
  else if (dir.Contains("7H") || dir.Contains("8H")) {
    return "sr7H8H";
  }

  // combine loMT and hiMT bins for regions split by that
  if (dir.Contains("9L") || dir.Contains("10L")) {
    return "sr9L10L";
  }
  else if (dir.Contains("9M") || dir.Contains("10M")) {
    return "sr9M10M";
  }
  else if (dir.Contains("9H") || dir.Contains("10H")) {
    return "sr9H10H";
  }

  // default: use only this TR
  return dir;
}

//_______________________________________________________________________________
void cardMaker(string signal, string input_dir, string output_dir){

  // ----------------------------------------
  //  samples definition
  // ----------------------------------------

  // set input files

  if (iteration1) {
    f_lostlep = new TFile(Form("%s/lostlepFromCRs.root",input_dir.c_str()));
    f_zinv = new TFile(Form("%s/zinvFromGJ.root",input_dir.c_str()));
  } else { 
    f_lostlep = new TFile(Form("%s/lostlep.root",input_dir.c_str()));
    f_zinv = new TFile(Form("%s/zinv_ht.root",input_dir.c_str()));
  }
  f_qcd = new TFile(Form("%s/qcd_ht.root",input_dir.c_str()));

  f_sig = new TFile(Form("%s/%s.root",input_dir.c_str(),signal.c_str()));

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
      printCard(isr, "L", imt2, signal, output_dir);
      printCard(isr, "M", imt2, signal, output_dir);
      printCard(isr, "H", imt2, signal, output_dir);
    }
  }

}

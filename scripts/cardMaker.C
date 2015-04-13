#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <string>

#include "TROOT.h"
#include "TLatex.h"
#include "TString.h"
#include "TFile.h"
#include "TH1.h"
#include "TList.h"
#include "TCollection.h"
#include "TKey.h"

using namespace std;

TFile* f_lostlep = 0;
TFile* f_zinv = 0;
TFile* f_qcd = 0;
TFile* f_sig = 0;

bool suppressZeroBins = true;

bool iteration1 = true; // Here we try to get realistic statistical errors from control region statistics
                         // To use this option, first run the ZinvMaker.C and lostlepMaker.C

bool useGammaFunction = true; // use gamma function for CR stat nuisance params in iteration1

const float dummy_alpha = 1.; // dummy value for gmN when there are no SR events

bool uncorrelatedZGratio = false; // treat ZGratio uncertainty as fully uncorrelated
bool previousBinZGprediction = false; // Get prediction from previous bin if nGJ < previousBinZGprediction_min
float previousBinZGprediction_min = 5.;

std::string toString(int in){
  stringstream ss;
  ss << in;
  return ss.str();
}

void ReplaceString(std::string& subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}

double round(double d)
{
  return floor(d + 0.5);
}

//_______________________________________________________________________________
void printCard( string dir_str , int mt2bin , string signal, string output_dir) {

  // read off yields from h_mt2bins hist in each topological region

  TString dir = TString(dir_str);
  TString fullhistname = dir + "/h_mt2bins";
  TString fullhistnameStat  = fullhistname+"Stat";
  TString fullhistnameMCStat  = fullhistname+"MCStat";
  TString fullhistnameCRyield  = fullhistname+"CRyield";
  TString fullhistnamePreviousBinRatio  = fullhistname+"PreviousBinRatio";

  double n_lostlep(0.);
  double n_lostlep_tr(0.);
  double n_lostlep_cr(0.);
  double err_lostlep_mcstat(0.);
  double err_lostlep_stat(0.);
  double n_zinv(0.);
  double n_zinv_cr(0.);
  double n_zinv_cr_prevBin(0.);
  double zinv_prevBinRatio(0.);
  double err_zinv_stat(0.);
  double err_zinv_mcstat(0.);
  double n_qcd(0.);
  double n_bkg(0.);

  double n_sig(0.);

  TH1D* h_sig = (TH1D*) f_sig->Get(fullhistname);
  if (h_sig != 0) n_sig = h_sig->GetBinContent(mt2bin);
  else return;

  //Get variable boundaries for signal region.
  //Used to create datacard name.
  TH1D* h_ht_LOW = (TH1D*) f_sig->Get(dir+"/h_ht_LOW");
  TH1D* h_ht_UP = (TH1D*) f_sig->Get(dir+"/h_ht_UP");
  int ht_LOW = h_ht_LOW->GetBinContent(1);
  int ht_UP = h_ht_UP->GetBinContent(1);
  
  TH1D* h_nbjets_LOW = (TH1D*) f_sig->Get(dir+"/h_nbjets_LOW");
  TH1D* h_nbjets_UP = (TH1D*) f_sig->Get(dir+"/h_nbjets_UP");
  int nbjets_LOW = h_nbjets_LOW->GetBinContent(1);
  int nbjets_UP = h_nbjets_UP->GetBinContent(1);

  TH1D* h_njets_LOW = (TH1D*) f_sig->Get(dir+"/h_njets_LOW");
  TH1D* h_njets_UP = (TH1D*) f_sig->Get(dir+"/h_njets_UP");
  int njets_LOW = h_njets_LOW->GetBinContent(1);
  int njets_UP = h_njets_UP->GetBinContent(1);

  int mt2_LOW = h_sig->GetBinLowEdge(mt2bin);
  int mt2_UP = mt2_LOW + h_sig->GetBinWidth(mt2bin);
  // hardcode the current edge of our highest bin..
  if (mt2_UP == 1500) mt2_UP = -1;

  int nbjets_UP_mod = nbjets_UP;
  int njets_UP_mod = njets_UP;
  if(nbjets_UP != -1) nbjets_UP_mod--;
  if(njets_UP != -1) njets_UP_mod--;

  std::string ht_str = "HT" + toString(ht_LOW) + "to" + toString(ht_UP);
  std::string jet_str = (njets_UP_mod == njets_LOW) ? "j" + toString(njets_LOW) : "j" + toString(njets_LOW) + "to" + toString(njets_UP_mod);
  std::string bjet_str = (nbjets_UP_mod == nbjets_LOW) ? "b" + toString(nbjets_LOW) : "b" + toString(nbjets_LOW) + "to" + toString(nbjets_UP_mod);
  std::string mt2_str = "m" + toString(mt2_LOW) + "to" + toString(mt2_UP);
  
  //Replace instances of "-1" with "inf" for variables with no upper bound.
  ReplaceString(ht_str, "-1", "Inf");
  ReplaceString(jet_str, "-1", "Inf");
  ReplaceString(bjet_str, "-1", "Inf");
  ReplaceString(mt2_str, "-1", "Inf");

  std::string channel = ht_str + "_" + jet_str + "_" + bjet_str + "_" + mt2_str;

  TString cardname = Form("%s/datacard_%s_%s.txt",output_dir.c_str(),channel.c_str(),signal.c_str());

  // get yields for each sample
  // !!!!! HACK: set zero bins to 0.01 for now to make combine happy
  TH1D* h_lostlep = (TH1D*) f_lostlep->Get(fullhistname);
  if (h_lostlep != 0) {
    n_lostlep = h_lostlep->GetBinContent(mt2bin);
    n_lostlep_tr = h_lostlep->Integral(0,-1);
  } 
  if (n_lostlep < 0.01 && !useGammaFunction) n_lostlep = 0.01;
  if (n_lostlep_tr < 0.01 && !useGammaFunction) n_lostlep_tr = 0.01;
  if (iteration1) {
    // stat error comes from CR -> CR stats already integrated over MT2 bins in input
    TH1D* h_lostlep_stat = (TH1D*) f_lostlep->Get(fullhistnameStat);
    if (h_lostlep_stat != 0 && h_lostlep_stat->GetBinContent(mt2bin) != 0) 
      err_lostlep_stat = h_lostlep_stat->GetBinError(mt2bin)/h_lostlep_stat->GetBinContent(mt2bin);
    TH1D* h_lostlep_mcstat = (TH1D*) f_lostlep->Get(fullhistnameMCStat);
    if (h_lostlep_mcstat != 0 && h_lostlep_mcstat->GetBinContent(mt2bin) != 0) 
      err_lostlep_mcstat = h_lostlep_mcstat->GetBinError(mt2bin)/h_lostlep_mcstat->GetBinContent(mt2bin);
    TH1D* h_lostlep_cryield = (TH1D*) f_lostlep->Get(fullhistnameCRyield);
    if (h_lostlep_cryield != 0) 
      n_lostlep_cr = round(h_lostlep_cryield->Integral(0,-1));
  }


  TH1D* h_zinv = (TH1D*) f_zinv->Get(fullhistname);
  if (h_zinv != 0) n_zinv = h_zinv->GetBinContent(mt2bin);
  if (n_zinv < 0.01 && !useGammaFunction) n_zinv = 0.01;
  if (iteration1) {
    // For 1b regions, use the statistical uncertainty from corresponding 0b region
    // if (sr == 2) fullhistnameStat.ReplaceAll("sr2", "sr1");
    // if (sr == 6) fullhistnameStat.ReplaceAll("sr6", "sr5");
    TH1D* h_zinv_stat = (TH1D*) f_zinv->Get(fullhistnameStat);
    if (h_zinv_stat != 0 && h_zinv_stat->GetBinContent(mt2bin) != 0) 
      err_zinv_stat = h_zinv_stat->GetBinError(mt2bin)/h_zinv_stat->GetBinContent(mt2bin);
    TH1D* h_zinv_mcstat = (TH1D*) f_zinv->Get(fullhistnameMCStat);
    if (h_zinv_mcstat != 0 && h_zinv_mcstat->GetBinContent(mt2bin) != 0) 
      err_zinv_mcstat = h_zinv_mcstat->GetBinError(mt2bin)/h_zinv_mcstat->GetBinContent(mt2bin);
    TH1D* h_zinv_cryield = (TH1D*) f_zinv->Get(fullhistnameCRyield);
    if (h_zinv_cryield != 0 && h_zinv_cryield->GetBinContent(mt2bin) != 0) {
      n_zinv_cr = round(h_zinv_cryield->GetBinContent(mt2bin));
      // Use previousBinZGprediction:
      // Extrapolate from earlier bin if "yield(bin i) < min" while "yield(earlier bin) >= min". Otherwise it's not worth it. 
      if (previousBinZGprediction && mt2bin > 2 && h_zinv_cryield->GetBinContent(mt2bin) < previousBinZGprediction_min) {
	TH1D* h_zinv_prevbinratio = (TH1D*) f_zinv->Get(fullhistnamePreviousBinRatio);
	zinv_prevBinRatio = h_zinv_prevbinratio->GetBinContent(mt2bin);
	n_zinv_cr = h_zinv_cryield->GetBinContent(mt2bin); // remove rounding from n_zinv_cr
	if ( h_zinv_cryield->GetBinContent(mt2bin-1) >= previousBinZGprediction_min ) { // extrapolate from one bin back
	  n_zinv_cr_prevBin = round(h_zinv_cryield->GetBinContent(mt2bin-1));
	}
	else if ( mt2bin > 3 && h_zinv_cryield->GetBinContent(mt2bin-2) >= previousBinZGprediction_min) { // extrapolate from two bin back
	  zinv_prevBinRatio *= h_zinv_prevbinratio->GetBinContent(mt2bin-1);
	  n_zinv_cr_prevBin = round(h_zinv_cryield->GetBinContent(mt2bin-2));
	}
	else if ( mt2bin > 4 && h_zinv_cryield->GetBinContent(mt2bin-3) >= previousBinZGprediction_min) { // extrapolate from three bin back
	  zinv_prevBinRatio *= h_zinv_prevbinratio->GetBinContent(mt2bin-1)*h_zinv_prevbinratio->GetBinContent(mt2bin-2);
	  n_zinv_cr_prevBin = round(h_zinv_cryield->GetBinContent(mt2bin-3));
	}
	else { // give up and just put everything back the way it was
	  n_zinv_cr = round(h_zinv_cryield->GetBinContent(mt2bin));
	}
      }
    }
  }


  TH1D* h_qcd = (TH1D*) f_qcd->Get(fullhistname);
  if (h_qcd != 0) n_qcd = h_qcd->GetBinContent(mt2bin);
  //  if (n_qcd < 0.01) n_qcd = 0.01;

  n_bkg = n_lostlep+n_zinv+n_qcd;
  if (n_bkg < 0.001) n_qcd = 0.01;

  if (suppressZeroBins && ((n_sig < 0.1) || (n_sig/n_bkg < 0.02))) {
    std::cout << "Zero signal, card not printed: " << cardname << std::endl;
    return;
  }

  int n_syst = 0;

  // ----- lost lepton bkg uncertainties
  // uncorrelated across TRs and MT2 bins
  //  for iter1+gamma, only apply if n_lostlep > 0.
  double lostlep_shape = 1.075;
  TString name_lostlep_shape = Form("llep_shape_%s", channel.c_str());
  if (n_lostlep > 0. || (!iteration1 && !useGammaFunction))  ++n_syst;
  // correlated for MT2 bins in a TR
  double lostlep_crstat = 1.;
  bool lostlep_decorrelate_bin = false;
  double lostlep_lepeff = 1.;
  // uncorrelated across TRs and MT2 bins
  //  only use for now for iter1+gamma, and if SR pred is nonzero
  double lostlep_mcstat = 1. + err_lostlep_mcstat;
  if (iteration1) { // iteration1: use CR stats

    if (useGammaFunction) {
      lostlep_lepeff = 1.15;

      // set lostlep_crstat to transfer factor from CR to SR
      if (n_lostlep > 0. && n_lostlep_cr > 0.) {
	lostlep_crstat = n_lostlep / n_lostlep_cr;
	n_syst += 3;
      }
      else if (n_lostlep > 0.) {
	// 0 CR, nonzero SR: use SR with 100% lnN. Decorrelate bin
	lostlep_crstat = 2.;
	lostlep_decorrelate_bin = true;
	n_syst += 3;
      }
      else if (n_lostlep_cr > 0.) {
	// nonzero CR, 0 SR: use 0 for CR and SR, dummy_alpha. Decorrelate bin
	lostlep_crstat = dummy_alpha;
	n_lostlep_cr = 0.;
	lostlep_decorrelate_bin = true;
	n_syst += 2;
      }
      else {
	// 0 CR, 0 SR: use 0 for CR and SR, dummy_alpha
	lostlep_crstat = dummy_alpha;
	n_syst += 2;
      }
    } // if (useGammaFunction)
    else { // don't use gamma function
      if (err_lostlep_stat > 0.) lostlep_crstat = 1. + sqrt( pow(0.15,2) + pow(err_lostlep_stat,2));
      else lostlep_crstat = 1. + sqrt( pow(0.15,2) + 1.);
      ++n_syst;
    }
  }
  else { // iteration0: emulate CR stats
    if (n_lostlep_tr > 0.) lostlep_crstat = 1. + sqrt( pow(0.15,2) + pow(1./sqrt(n_lostlep_tr),2));
    else lostlep_crstat = 1. + sqrt( pow(0.15,2) + 1.);
    ++n_syst;
  }
  TString name_lostlep_crstat = Form("llep_CRstat_%s_%s_%s", ht_str.c_str(), jet_str.c_str(), bjet_str.c_str());
  if (lostlep_decorrelate_bin) name_lostlep_crstat = Form("llep_CRstat_%s", channel.c_str());
  TString name_lostlep_lepeff = Form("llep_lepeff_%s_%s_%s", ht_str.c_str(), jet_str.c_str(), bjet_str.c_str());
  TString name_lostlep_mcstat = Form("llep_MCstat_%s", channel.c_str());

  // ----- zinv bkg uncertainties - depend on signal region, b selection
  // uncorrelated across TRs and MT2 bins
  double zinv_crstat = -1.;
  TString name_zinv_crstat = Form("zinv_CRstat_%s",channel.c_str());
  double zinv_alphaerr = 1. + err_zinv_mcstat;
  TString name_zinv_alphaerr = Form("zinv_alphaErr_%s",channel.c_str());
  // correlated for all bins with 0-1b
  double zinv_zgamma = -1.;
  TString name_zinv_zgamma = "zinv_ZGratio";
  if (uncorrelatedZGratio) name_zinv_zgamma = Form("zinv_ZGratio_%s",channel.c_str());
  // correlated for MT2 bins in a TR
  double zinv_bratio = -1.;
  TString name_zinv_bratio = Form("zinv_Bratio_%s_%s_%s", ht_str.c_str(), jet_str.c_str(), bjet_str.c_str());
  // uncorrelated across TRs and MT2 bins
  double zinv_mcsyst = -1.;
  TString name_zinv_mcsyst = Form("zinv_MC_%s",channel.c_str());

  // 2+b: pure MC estimate
  if (nbjets_LOW >= 2) {
    zinv_mcsyst = 2.;
    ++n_syst;
  }
  // 1b: data CR with 0->1b sf
  else if (nbjets_LOW == 1 && nbjets_UP == 2) {
    if (iteration1) {
      if (useGammaFunction) {
	if (n_zinv_cr > 0. && n_zinv > 0.) {
	  zinv_crstat =  n_zinv / n_zinv_cr;
	  n_syst += 2;
	}
	else if (n_zinv > 0.) {
	  // 0 CR, nonzero SR: use SR with 100% lnN
	  zinv_crstat = 2.;
	  ++n_syst;
	}
	else if (n_zinv_cr > 0.) {
	  // nonzero CR, 0 SR: use 0 for CR and SR, dummy_alpha
	  zinv_crstat = dummy_alpha;
	  n_zinv_cr = 0.;
	  ++n_syst;
	}
	else {
	  // 0 CR, 0 SR: use 0 for CR and SR, dummy_alpha
	  zinv_crstat = dummy_alpha;
	  ++n_syst;
	}
      } // if (useGammaFunction)
      else { // iter1, don't use gamma function
	if (n_zinv > 0.) zinv_crstat = 1. + 1.*err_zinv_stat;
	else zinv_crstat = 2.;
	  ++n_syst;
      }
    }
    // else: iter0
    else {
      if (n_zinv > 0.) zinv_crstat = 1. + 1./sqrt(20. * n_zinv);
      else zinv_crstat = 2.;
    }
    zinv_zgamma = 1.20;
    ++n_syst;
    if (iteration1) zinv_bratio = 1.00;
    else { // iteration0
      zinv_bratio = 1.30;
      ++n_syst;
    }
  } // end of 1b
  // 0b: data CR
  else if (nbjets_UP == 1) {
    if (iteration1) {
      if (useGammaFunction) {
	if (n_zinv_cr > 0. && n_zinv > 0.) {
	  zinv_crstat =  n_zinv / n_zinv_cr;
	  n_syst += 2;
	}
	else if (n_zinv > 0.) {
	  // 0 CR, nonzero SR: use SR with 100% lnN
	  zinv_crstat = 2.;
	  ++n_syst;
	}
	else if (n_zinv_cr > 0.) {
	  // nonzero CR, 0 SR: use 0 for CR and SR, dummy_alpha
	  zinv_crstat = dummy_alpha;
	  n_zinv_cr = 0.;
	  ++n_syst;
	}
	else {
	  // 0 CR, 0 SR: use 0 for CR and SR, dummy_alpha
	  zinv_crstat = dummy_alpha;
	  ++n_syst;
	}
      } // if (useGammaFunction)
      else { // iter1, don't use gamma function
	if (n_zinv > 0.) zinv_crstat = 1. + 1.*err_zinv_stat;
	else zinv_crstat = 2.;
  ++n_syst;
      }
    }
    // else: iter0
    else {
      if (n_zinv > 0.) zinv_crstat = 1. + 1./sqrt(2. * n_zinv);
      else zinv_crstat = 2.;
      ++n_syst;
    }
    zinv_zgamma = 1.20;
    ++n_syst;
  }

  // ----- qcd bkg uncertainties: uncorrelated for all bins
  double qcd_syst = 2.00;
  TString name_qcd_syst = Form("qcd_syst_%s", channel.c_str());
  if (n_qcd > 0.) ++n_syst;

  // ----- sig uncertainties: correlated for all bins
  double sig_syst = 1.10;
  ++n_syst;

  ofstream* ofile = new ofstream();
  ofile->open(cardname);

  *ofile <<  "imax 1  number of channels"                                                    << endl;
  *ofile <<  "jmax 3  number of backgrounds"                                                 << endl;
  *ofile <<  Form("kmax %d  number of nuisance parameters",n_syst)                           << endl;
  *ofile <<  "------------"                                                                  << endl;
  *ofile <<  Form("bin         %s",channel.c_str())                                          << endl;
  *ofile <<  Form("observation %.0f",n_bkg)                                                  << endl;
  *ofile <<  "------------"                                                                  << endl;
  *ofile <<  Form("bin             %s   %s   %s   %s",channel.c_str(),channel.c_str(),channel.c_str(),channel.c_str()) << endl;
  *ofile <<  "process          sig       zinv        llep      qcd"                                      << endl; 
  *ofile <<  "process           0         1           2         3"                                      << endl;
  *ofile <<  Form("rate            %.3f    %.3f      %.3f      %.3f",n_sig,n_zinv,n_lostlep,n_qcd) << endl;
  *ofile <<  "------------"                                                                  << endl;
  *ofile <<  Form("sig_syst                                            lnN   %.3f    -      -     - ",sig_syst)  << endl;
  if (nbjets_UP == 1 || nbjets_UP == 2) {
    *ofile <<  Form("%s                                   lnN    -   %.3f    -    - ",name_zinv_zgamma.Data(),zinv_zgamma)  << endl;
  }
  if (nbjets_UP == 1 || nbjets_UP == 2) {
    if (iteration1 && useGammaFunction) {
      if (n_zinv > 0. && n_zinv_cr == 0.) {
	// taking MC directly - use lnN uncertainty
	*ofile <<  Form("%s     lnN    -   %.3f   -   - ",name_zinv_crstat.Data(),zinv_crstat)  << endl;
      } else {
	if (previousBinZGprediction && n_zinv_cr_prevBin > 0.) {
	  *ofile <<  Form("%s     gmN %.0f    -  %.5f   -   - ",name_zinv_crstat.Data(),n_zinv_cr_prevBin,zinv_crstat * zinv_prevBinRatio)  << endl;
	  cout <<  Form("%s     gmN %.0f    -  %.5f   -   - ",name_zinv_crstat.Data(),n_zinv_cr_prevBin,zinv_crstat * zinv_prevBinRatio)  << endl;
	}
	else 
	  *ofile <<  Form("%s     gmN %.0f    -  %.5f   -   - ",name_zinv_crstat.Data(),n_zinv_cr,zinv_crstat)  << endl;
      }
      if (n_zinv > 0. && n_zinv_cr > 0.) {
	// MC stats for Z/g ratio, if both samples have events
	*ofile <<  Form("%s       lnN    -   %.3f   -    - ",name_zinv_alphaerr.Data(),zinv_alphaerr)  << endl;
      }
    } else {
      *ofile <<  Form("%s     lnN    -   %.3f   -   - ",name_zinv_crstat.Data(),zinv_crstat)  << endl;
    }
  }
  if (!iteration1 && nbjets_LOW == 1 && nbjets_UP == 2) {
    *ofile <<  Form("%s               lnN    -     %.3f   -    - ",name_zinv_bratio.Data(),zinv_bratio)  << endl;
  }
  if (nbjets_LOW >= 2) {
    *ofile <<  Form("%s       lnN    -   %.3f   -    - ",name_zinv_mcsyst.Data(),zinv_mcsyst)  << endl;
  }
  if (iteration1 && useGammaFunction) {
    if (lostlep_decorrelate_bin && n_lostlep > 0.) {
      // taking MC directly - use lnN uncertainty
      *ofile <<  Form("%s                 lnN    -    -    %.3f    - ",name_lostlep_crstat.Data(),lostlep_crstat)  << endl;
    } else {
      *ofile <<  Form("%s                 gmN %.0f    -    -    %.5f     - ",name_lostlep_crstat.Data(),n_lostlep_cr,lostlep_crstat)  << endl;
    }
    *ofile <<  Form("%s                 lnN    -    -    %.3f    - ",name_lostlep_lepeff.Data(),lostlep_lepeff)  << endl;
    if (n_lostlep > 0.) {
      *ofile <<  Form("%s        lnN    -    -    %.3f    - ",name_lostlep_mcstat.Data(),lostlep_mcstat)  << endl;
    }
  } else {
    *ofile <<  Form("%s                 lnN    -    -    %.3f    - ",name_lostlep_crstat.Data(),lostlep_crstat)  << endl;
  }
  if (n_lostlep > 0. || (!iteration1 && !useGammaFunction)) {
    *ofile <<  Form("%s    lnN    -    -   %.3f     - ",name_lostlep_shape.Data(),lostlep_shape)  << endl;
  }
  if (n_qcd > 0.) *ofile <<  Form("%s     lnN    -      -       -   %.3f ",name_qcd_syst.Data(),qcd_syst)  << endl;

  ofile->close();

  std::cout << "Wrote card: " << cardname << std::endl;

  return;
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

  if( f_lostlep->IsZombie() || f_zinv->IsZombie() || f_qcd->IsZombie() || f_sig->IsZombie()) {
    std::cout << "Input file does not exist" << std::endl;
    return;
  }

  // ----------------------------------------
  //  cards definitions
  // ----------------------------------------

  const unsigned int n_mt2bins = 5;

  //Loop through list of every directory in the signal file.
  //if directory begins with "sr", excluding "srbase", make cards for it.
  TIter it(f_sig->GetListOfKeys());
  TKey* k;
  std::string keep = "sr";
  std::string skip = "srbase";
  while (k = (TKey *)it()) {
    if (k->GetTitle() == "srbase") continue;
    if (strncmp (k->GetTitle(), skip.c_str(), skip.length()) == 0) continue;
    if (strncmp (k->GetTitle(), keep.c_str(), keep.length()) == 0) {//it is a signal region
      for (unsigned int imt2 = 1; imt2 <= n_mt2bins; ++imt2) {//Make a separate card for each MT2 bin.
        printCard(k->GetTitle(), imt2, signal, output_dir);   //MT2 bins with no entries are handled by printCard function.
      }
    }
  }

}

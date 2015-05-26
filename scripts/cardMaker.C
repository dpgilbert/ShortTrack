#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <sstream>
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
TFile* f_purity = 0;
TFile* f_qcd = 0;
TFile* f_sig = 0;

bool suppressZeroBins = true;

const float dummy_alpha = 1.; // dummy value for gmN when there are no SR events

bool uncorrelatedZGratio = false; // treat ZGratio uncertainty as fully uncorrelated

//_______________________________________________________________________________
std::string toString(int in){
  stringstream ss;
  ss << in;
  return ss.str();
}

//_______________________________________________________________________________
void ReplaceString(std::string& subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}

//_______________________________________________________________________________
double round(double d)
{
  return floor(d + 0.5);
}

//_______________________________________________________________________________
void printCard( string dir_str , int mt2bin , string signal, string output_dir) {

  // read off yields from h_mt2bins hist in each topological region

  TString dir = TString(dir_str);
  TString fullhistname = dir + "/h_mt2bins";
  //  TString fullhistnameStat  = fullhistname+"Stat";
  TString fullhistnameMCStat  = fullhistname+"MCStat";
  TString fullhistnameCRyield  = fullhistname+"CRyield";
  TString fullhistnameRatio  = fullhistname+"Ratio";
  TString fullhistnamePurity = dir + "/h_puritySieieSB";

  double n_lostlep(0.);
  double n_lostlep_cr(0.);
  double err_lostlep_mcstat(0.);
  double n_zinv(0.);
  double n_zinv_cr(0.);
  double err_zinv_mcstat(0.);
  double zinv_ratio_zg(0.);
  double zinv_purity(0.);
  double err_zinv_purity(0.);
  double n_qcd(0.);
  double n_bkg(0.);

  double n_sig(0.);

  TH1D* h_sig = (TH1D*) f_sig->Get(fullhistname);
  if (h_sig != 0) n_sig = h_sig->GetBinContent(mt2bin);
  else return;

  //Get variable boundaries for signal region.
  //Used to create datacard name.
  TH1D* h_ht_LOW = (TH1D*) f_sig->Get(dir+"/h_ht_LOW");
  TH1D* h_ht_HI = (TH1D*) f_sig->Get(dir+"/h_ht_HI");
  int ht_LOW = h_ht_LOW->GetBinContent(1);
  int ht_HI = h_ht_HI->GetBinContent(1);
  
  TH1D* h_nbjets_LOW = (TH1D*) f_sig->Get(dir+"/h_nbjets_LOW");
  TH1D* h_nbjets_HI = (TH1D*) f_sig->Get(dir+"/h_nbjets_HI");
  int nbjets_LOW = h_nbjets_LOW->GetBinContent(1);
  int nbjets_HI = h_nbjets_HI->GetBinContent(1);

  TH1D* h_njets_LOW = (TH1D*) f_sig->Get(dir+"/h_njets_LOW");
  TH1D* h_njets_HI = (TH1D*) f_sig->Get(dir+"/h_njets_HI");
  int njets_LOW = h_njets_LOW->GetBinContent(1);
  int njets_HI = h_njets_HI->GetBinContent(1);

  int mt2_LOW = h_sig->GetBinLowEdge(mt2bin);
  int mt2_HI = mt2_LOW + h_sig->GetBinWidth(mt2bin);
  // hardcode the current edge of our highest bin..
  if (mt2_HI == 1500) mt2_HI = -1;

  int nbjets_HI_mod = nbjets_HI;
  int njets_HI_mod = njets_HI;
  if(nbjets_HI != -1) nbjets_HI_mod--;
  if(njets_HI != -1) njets_HI_mod--;

  std::string ht_str = "HT" + toString(ht_LOW) + "to" + toString(ht_HI);
  std::string jet_str = (njets_HI_mod == njets_LOW) ? "j" + toString(njets_LOW) : "j" + toString(njets_LOW) + "to" + toString(njets_HI_mod);
  std::string bjet_str = (nbjets_HI_mod == nbjets_LOW) ? "b" + toString(nbjets_LOW) : "b" + toString(nbjets_LOW) + "to" + toString(nbjets_HI_mod);
  std::string mt2_str = "m" + toString(mt2_LOW) + "to" + toString(mt2_HI);
  
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
  } 
  TH1D* h_lostlep_mcstat = (TH1D*) f_lostlep->Get(fullhistnameMCStat);
  if (h_lostlep_mcstat != 0 && h_lostlep_mcstat->GetBinContent(mt2bin) != 0) 
    err_lostlep_mcstat = h_lostlep_mcstat->GetBinError(mt2bin)/h_lostlep_mcstat->GetBinContent(mt2bin);
  TH1D* h_lostlep_cryield = (TH1D*) f_lostlep->Get(fullhistnameCRyield);
  if (h_lostlep_cryield != 0) 
    n_lostlep_cr = round(h_lostlep_cryield->Integral(0,-1));


  TH1D* h_zinv = (TH1D*) f_zinv->Get(fullhistname);
  if (h_zinv != 0) n_zinv = h_zinv->GetBinContent(mt2bin);
  // MC stat unc based on #Z/#g
  TH1D* h_zinv_mcstat = (TH1D*) f_zinv->Get(fullhistnameRatio);
  if (h_zinv_mcstat != 0 && h_zinv_mcstat->GetBinContent(mt2bin) != 0) {
    err_zinv_mcstat = h_zinv_mcstat->GetBinError(mt2bin)/h_zinv_mcstat->GetBinContent(mt2bin);
    zinv_ratio_zg = h_zinv_mcstat->GetBinContent(mt2bin);
  }
  else { // catch zeroes (there shouldn't be any)
    err_zinv_mcstat = 1.;
    zinv_ratio_zg = 0.4;
  }
  // Photon yield (includes GJetPrompt+QCDPrompt+QCDFake)
  TH1D* h_zinv_cryield = (TH1D*) f_purity->Get(fullhistname);
  if (h_zinv_cryield != 0 && h_zinv_cryield->GetBinContent(mt2bin) != 0)
    n_zinv_cr = round(h_zinv_cryield->GetBinContent(mt2bin));
  // Purity and uncertainty
  TH1D* h_zinv_purity = (TH1D*) f_purity->Get(fullhistnamePurity);
  zinv_purity = 1.;
  if (h_zinv_purity != 0 && h_zinv_purity->GetBinContent(mt2bin) != 0) {
    zinv_purity *= h_zinv_purity->GetBinContent(mt2bin);
    err_zinv_purity = h_zinv_purity->GetBinError(mt2bin)/h_zinv_purity->GetBinContent(mt2bin);
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
  if (n_lostlep > 0.)  ++n_syst;
  // correlated for MT2 bins in a TR
  double lostlep_crstat = 1.;
  bool lostlep_decorrelate_bin = false;
  double lostlep_lepeff = 1.;
  // uncorrelated across TRs and MT2 bins
  //  only use for now for iter1+gamma, and if SR pred is nonzero
  double lostlep_mcstat = 1. + err_lostlep_mcstat;
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
  TString name_lostlep_crstat = Form("llep_CRstat_%s_%s_%s", ht_str.c_str(), jet_str.c_str(), bjet_str.c_str());
  if (lostlep_decorrelate_bin) name_lostlep_crstat = Form("llep_CRstat_%s", channel.c_str());
  TString name_lostlep_lepeff = Form("llep_lepeff_%s_%s_%s", ht_str.c_str(), jet_str.c_str(), bjet_str.c_str());
  TString name_lostlep_mcstat = Form("llep_MCstat_%s", channel.c_str());

  // ----- zinv bkg uncertainties - depend on signal region, b selection
  // uncorrelated across TRs and MT2 bins
  TString name_zinv_crstat = Form("zinv_CRstat_%s",channel.c_str());
  double zinv_alpha = 1.;
  double zinv_alphaerr = 1. + err_zinv_mcstat;
  TString name_zinv_alphaerr = Form("zinv_alphaErr_%s",channel.c_str());
  double zinv_purityerr = 1. + err_zinv_purity;
  TString name_zinv_purityerr = Form("zinv_purity_%s",channel.c_str());
  // correlated for all bins with 0-1b
  double zinv_zgamma = -1.;
  TString name_zinv_zgamma = "zinv_ZGratio";
  if (uncorrelatedZGratio) name_zinv_zgamma = Form("zinv_ZGratio_%s",channel.c_str());
  // uncorrelated across TRs and MT2 bins
  double zinv_mcsyst = -1.;
  TString name_zinv_mcsyst = Form("zinv_MC_%s",channel.c_str());

  // 2+b: pure MC estimate
  if (nbjets_LOW >= 2) {
    zinv_mcsyst = 2.;
    ++n_syst;
  }
  else { // 0-1b. always use gamma function
    // err_zinv_mcstat, zinv_ratio_zg, n_zinv_cr
    zinv_alpha =  zinv_ratio_zg * zinv_purity * 0.92; // 0.92 is a fixed factor for "f = GJetPrompt / (GJetPrompt+QCDPrompt)"
    n_zinv = n_zinv_cr * zinv_alpha; // don't use Zinv as central value any more!
    zinv_zgamma = 1.20;
    n_syst += 4; // 1: zinv_alphaerr (stat on ratio). 2: zinv_zgamma (R, p, f) 3: gamma function. 4: purity stat unc.
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

  // ---- sig systs
  *ofile <<  Form("sig_syst                                            lnN   %.3f    -      -     - ",sig_syst)  << endl;

  // ---- Zinv systs
  if (nbjets_HI == 1 || nbjets_HI == 2) {
    *ofile <<  Form("%s     gmN %.0f    -  %.5f   -   - ",name_zinv_crstat.Data(),n_zinv_cr,zinv_alpha)  << endl;
    *ofile <<  Form("%s       lnN    -   %.3f   -    - ",name_zinv_alphaerr.Data(),zinv_alphaerr)  << endl;
    *ofile <<  Form("%s                                   lnN    -   %.3f    -    - ",name_zinv_zgamma.Data(),zinv_zgamma)  << endl;     
    *ofile <<  Form("%s       lnN    -   %.3f   -    - ",name_zinv_purityerr.Data(),zinv_purityerr)  << endl;
  }
  if (nbjets_LOW >= 2) {
    *ofile <<  Form("%s       lnN    -   %.3f   -    - ",name_zinv_mcsyst.Data(),zinv_mcsyst)  << endl;
  }

  // ---- lostlep systs
  if (lostlep_decorrelate_bin && n_lostlep > 0.) {
    // taking MC directly - use lnN uncertainty
    *ofile <<  Form("%s                 lnN    -    -    %.3f    - ",name_lostlep_crstat.Data(),lostlep_crstat)  << endl;
  } else {
    *ofile <<  Form("%s                 gmN %.0f    -    -    %.5f     - ",name_lostlep_crstat.Data(),n_lostlep_cr,lostlep_crstat)  << endl;
  }
  *ofile <<  Form("%s                 lnN    -    -    %.3f    - ",name_lostlep_lepeff.Data(),lostlep_lepeff)  << endl;
  if (n_lostlep > 0.) {
    *ofile <<  Form("%s        lnN    -    -    %.3f    - ",name_lostlep_mcstat.Data(),lostlep_mcstat)  << endl;
    *ofile <<  Form("%s    lnN    -    -   %.3f     - ",name_lostlep_shape.Data(),lostlep_shape)  << endl;
  }

  // ---- QCD systs
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

  f_lostlep = new TFile(Form("%s/lostlepFromCRs.root",input_dir.c_str()));
  f_zinv = new TFile(Form("%s/zinvFromGJ.root",input_dir.c_str()));
  f_purity = new TFile(Form("%s/purity.root",input_dir.c_str()));
  f_qcd = new TFile(Form("%s/qcd_ht.root",input_dir.c_str()));

  f_sig = new TFile(Form("%s/%s.root",input_dir.c_str(),signal.c_str()));

  if( f_lostlep->IsZombie() || f_zinv->IsZombie() || f_purity->IsZombie() || f_qcd->IsZombie() || f_sig->IsZombie()) {
    std::cout << "Input file does not exist" << std::endl;
    return;
  }

  // ----------------------------------------
  //  cards definitions
  // ----------------------------------------

  
  //Loop through list of every directory in the signal file.
  //if directory begins with "sr", excluding "srbase", make cards for it.
  TIter it(f_sig->GetListOfKeys());
  TKey* k;
  std::string keep = "sr";
  std::string skip = "srbase";
  while ((k = (TKey *)it())) {
    if (strncmp (k->GetTitle(), skip.c_str(), skip.length()) == 0) continue;
    if (strncmp (k->GetTitle(), keep.c_str(), keep.length()) == 0) {//it is a signal region
      std::string mt2_hist_name = (k->GetTitle());
      mt2_hist_name += "/h_n_mt2bins";
      TH1D* h_n_mt2bins = (TH1D*) f_sig->Get(TString(mt2_hist_name));
      int n_mt2bins = h_n_mt2bins->GetBinContent(1);
      for (int imt2 = 1; imt2 <= n_mt2bins; ++imt2) {//Make a separate card for each MT2 bin.
        printCard(k->GetTitle(), imt2, signal, output_dir);   //MT2 bins with no entries are handled by printCard function.
      }
    }
  }

}

#include <iostream>
#include <utility>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <set>

#include "TROOT.h"
#include "TLatex.h"
#include "TString.h"
#include "TFile.h"
#include "TH1.h"
#include "TH3.h"
#include "TList.h"
#include "TCollection.h"
#include "TKey.h"
#include "TBenchmark.h"

using namespace std;

TFile* f_lostlep = 0;
TFile* f_zinv = 0;
TFile* f_purity = 0;
TFile* f_qcd = 0;
TFile* f_sig = 0;
TFile* f_data = 0;

const bool verbose = true;

const bool suppressZeroBins = true;

const float dummy_alpha = 1.; // dummy value for gmN when there are no SR events

const bool uncorrelatedZGratio = false; // treat ZGratio uncertainty as fully uncorrelated

const bool fourNuisancesPerBinZGratio = true;

const bool integratedZinvEstimate = true;

double last_zinv_ratio = 0.5;
double last_lostlep_transfer = 2.;

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
int printCard( string dir_str , int mt2bin , string signal, string output_dir, int scanM1 = -1, int scanM2 = -1) {

  // read off yields from h_mt2bins hist in each topological region
  cout<<"Looking at region "<<dir_str<<", mt2 bin "<<mt2bin<<endl;
  TString dir = TString(dir_str);
  TString fullhistname = dir + "/h_mt2bins";
  TString fullhistnameScan  = fullhistname+"_sigscan";
  //  TString fullhistnameStat  = fullhistname+"Stat";
  TString fullhistnameMCStat  = fullhistname+"MCStat";
  TString fullhistnameCRyield  = fullhistname+"CRyield";
  TString fullhistnameRatio  = fullhistname+"Ratio";
  TString fullhistnameRatioInt  = fullhistname+"RatioInt";
  TString fullhistnamePurity = dir + "/h_mt2binspurityFailSieieData";
  TString fullhistnamePurityInt = dir + "/h_mt2binspurityIntFailSieieData";
  TString fullhistnameAlpha  = fullhistname+"Alpha";

  TString signame(signal);
  if (scanM1 >= 0 && scanM2 >= 0) {
    signame = Form("%s_%d_%d",signal.c_str(),scanM1,scanM2);
  }

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
  double n_qcd_cr(0.);
  double qcd_alpha(0.);
  double err_qcd_alpha(0.);
  double n_bkg(0.);
  double n_data(0.);

  double n_sig(0.);

  TH1D* h_sig(0);
  // pick point out of signal scan
  if (scanM1 >= 0 && scanM2 >= 0) {
    TH3D* h_sigscan = (TH3D*) f_sig->Get(fullhistnameScan);
    if (!h_sigscan) return 0;
    int binx = h_sigscan->GetXaxis()->FindBin(scanM1);
    int biny = h_sigscan->GetYaxis()->FindBin(scanM2);
    h_sig = h_sigscan->ProjectionZ(Form("h_mt2bins_%d_%d_%s",scanM1,scanM2,dir_str.c_str()),binx,binx,biny,biny);
  }
  // single point sample
  else {
    h_sig = (TH1D*) f_sig->Get(fullhistname);
  }

  if (!h_sig) {
    if (verbose) cout<<"No signal found in this region"<<endl;
    return 0;
  }
  n_sig = h_sig->GetBinContent(mt2bin);
  
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
  std::string topologicalR = ht_str + "_" + jet_str + "_" + bjet_str;

  
  // bin boundaries for CRSL, for lostlep systematic correlations
  TH1D* h_ht_LOW_crsl = (TH1D*) f_lostlep->Get(dir+"/h_ht_LOW");
  TH1D* h_ht_HI_crsl = (TH1D*) f_lostlep->Get(dir+"/h_ht_HI");
  int ht_LOW_crsl = h_ht_LOW_crsl->GetBinContent(1);
  int ht_HI_crsl = h_ht_HI_crsl->GetBinContent(1);
  
  TH1D* h_nbjets_LOW_crsl = (TH1D*) f_lostlep->Get(dir+"/h_nbjets_LOW");
  TH1D* h_nbjets_HI_crsl = (TH1D*) f_lostlep->Get(dir+"/h_nbjets_HI");
  int nbjets_LOW_crsl = h_nbjets_LOW_crsl->GetBinContent(1);
  int nbjets_HI_crsl = h_nbjets_HI_crsl->GetBinContent(1);

  TH1D* h_njets_LOW_crsl = (TH1D*) f_lostlep->Get(dir+"/h_njets_LOW");
  TH1D* h_njets_HI_crsl = (TH1D*) f_lostlep->Get(dir+"/h_njets_HI");
  int njets_LOW_crsl = h_njets_LOW_crsl->GetBinContent(1);
  int njets_HI_crsl = h_njets_HI_crsl->GetBinContent(1);

  int nbjets_HI_crsl_mod = nbjets_HI_crsl;
  int njets_HI_crsl_mod = njets_HI_crsl;
  if(nbjets_HI_crsl != -1) nbjets_HI_crsl_mod--;
  if(njets_HI_crsl != -1) njets_HI_crsl_mod--;

  std::string ht_str_crsl = "HT" + toString(ht_LOW_crsl) + "to" + toString(ht_HI_crsl);
  std::string jet_str_crsl = (njets_HI_crsl_mod == njets_LOW_crsl) ? "j" + toString(njets_LOW_crsl) : "j" + toString(njets_LOW_crsl) + "to" + toString(njets_HI_crsl_mod);
  std::string bjet_str_crsl = (nbjets_HI_crsl_mod == nbjets_LOW_crsl) ? "b" + toString(nbjets_LOW_crsl) : "b" + toString(nbjets_LOW_crsl) + "to" + toString(nbjets_HI_crsl_mod);
  
  //Replace instances of "-1" with "inf" for variables with no upper bound.
  ReplaceString(ht_str_crsl, "-1", "Inf");
  ReplaceString(jet_str_crsl, "-1", "Inf");
  ReplaceString(bjet_str_crsl, "-1", "Inf");


  TString cardname = Form("%s/datacard_%s_%s.txt",output_dir.c_str(),channel.c_str(),signame.Data());

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
  int n_mt2bins = 1;
  if (h_zinv != 0) n_zinv = h_zinv->GetBinContent(mt2bin);
  if (h_zinv != 0) n_mt2bins = h_zinv->GetNbinsX();

  // MC stat unc based on #Z/#g
  TH1D* h_zinv_mcstat = (TH1D*) f_zinv->Get(fullhistnameRatio);
  if (integratedZinvEstimate) h_zinv_mcstat = (TH1D*) f_zinv->Get(fullhistnameRatioInt);
  if (h_zinv_mcstat != 0 && h_zinv_mcstat->GetBinContent(mt2bin) != 0) {
    err_zinv_mcstat = h_zinv_mcstat->GetBinError(mt2bin)/h_zinv_mcstat->GetBinContent(mt2bin);
    zinv_ratio_zg = h_zinv_mcstat->GetBinContent(mt2bin);
    last_zinv_ratio = zinv_ratio_zg;
  }
  else { // catch zeroes (there shouldn't be any)
    err_zinv_mcstat = 1.;
    zinv_ratio_zg = last_zinv_ratio;
  }
  // Pure GJet yield (useful when extracting G_i/G_int for integral estimate)
  TH1D* h_gjetyield = (TH1D*) f_zinv->Get(fullhistnameCRyield);

  // Photon yield (includes GJetPrompt+QCDPrompt+QCDFake, or Data)
  TH1D* h_zinv_cryield = (TH1D*) f_purity->Get(fullhistname);
  if (h_zinv_cryield != 0 && h_zinv_cryield->GetBinContent(mt2bin) != 0) {
    n_zinv_cr = round(h_zinv_cryield->GetBinContent(mt2bin));
    if (integratedZinvEstimate)  n_zinv_cr = round(h_zinv_cryield->Integral(0,-1));
  }
 
  // Purity and uncertainty (bin-by-bin estimate)
  TH1D* h_zinv_purity = (TH1D*) f_purity->Get(fullhistnamePurity);
  zinv_purity = 1.;
  int mt2bin_tmp = mt2bin;
  if (integratedZinvEstimate) {
    // When using the integrated estimate (over MT2), should use the integrated purity.
    h_zinv_purity = (TH1D*) f_purity->Get(fullhistnamePurityInt);
    mt2bin_tmp = 1;
  }
 
  if (h_zinv_purity != 0 && h_zinv_purity->GetBinContent(mt2bin_tmp) != 0) {
    zinv_purity *= h_zinv_purity->GetBinContent(mt2bin_tmp);
    err_zinv_purity = h_zinv_purity->GetBinError(mt2bin_tmp)/h_zinv_purity->GetBinContent(mt2bin_tmp);
  }
 
  float zllgamma_nj = 1., zllgamma_nb = 1., zllgamma_ht = 1., zllgamma_mt2 = 1.;
  TString notFound = "";
  if (fourNuisancesPerBinZGratio) {
    // Load NJ, NB, HT, MT2 histograms of R(Z/Gamma)
    TH1D* h_zllgamma_nj  = (TH1D*) f_zinv->Get("h_njbinsRatio");
    TH1D* h_zllgamma_nb  = (TH1D*) f_zinv->Get("h_nbjbinsRatio");
    TH1D* h_zllgamma_ht  = (TH1D*) f_zinv->Get("h_htbinsRatio");
    TH1D* h_zllgamma_mt2 = (TH1D*) f_zinv->Get("h_mt2binsRatio");
    // Extract values corresponding to this bin
    if (h_zllgamma_nj == 0 || h_zllgamma_nb == 0 || h_zllgamma_ht == 0 || h_zllgamma_mt2 == 0) {
      cout<<"Trying fourNuisancesPerBinZGratio, but could not find inclusive Zll/Gamma ratio plots for nuisance parameters"<<endl;
      return 0;
    }
 
    int bin_nj   = h_zllgamma_nj ->FindBin(njets_LOW + 0.5);     
    int bin_nb   = h_zllgamma_nb ->FindBin(nbjets_LOW + 0.5);
    if (nbjets_LOW >= 3) bin_nb   = h_zllgamma_nb ->FindBin(nbjets_LOW - 1 + 0.5);
    int bin_ht   = h_zllgamma_ht ->FindBin(ht_LOW + 1);      
    int bin_mt2  = h_zllgamma_mt2->FindBin(mt2_LOW + 1);     
    // (set to 100% if the ratio doesn't exist)
    zllgamma_nj  = h_zllgamma_nj ->GetBinContent( bin_nj  ) > 0 ? h_zllgamma_nj ->GetBinError( bin_nj  ) / h_zllgamma_nj ->GetBinContent( bin_nj  ) : 1.;
    zllgamma_nb  = h_zllgamma_nb ->GetBinContent( bin_nb  ) > 0 ? h_zllgamma_nb ->GetBinError( bin_nb  ) / h_zllgamma_nb ->GetBinContent( bin_nb  ) : 1.;
    if (nbjets_LOW >= 3) zllgamma_nb *= 2;
    zllgamma_ht  = h_zllgamma_ht ->GetBinContent( bin_ht  ) > 0 ? h_zllgamma_ht ->GetBinError( bin_ht  ) / h_zllgamma_ht ->GetBinContent( bin_ht  ) : 1.;
    zllgamma_mt2 = h_zllgamma_mt2->GetBinContent( bin_mt2 ) > 0 ? h_zllgamma_mt2->GetBinError( bin_mt2 ) / h_zllgamma_mt2->GetBinContent( bin_mt2 ) : 1.;
    //cout<<"Looking at bin "<<channel<<endl;
    //cout<<"Corresponding bins are: "<<bin_nj<<", "<<bin_nb<<", "<<bin_ht<<", "<<bin_mt2<<endl;
    //cout<<"Corresponding Zll/Gamma uncertainties are: "<<zllgamma_nj<<", "<<zllgamma_nb<<", "<<zllgamma_ht<<", "<<zllgamma_mt2<<endl;
  }

 
  TH1D* h_qcd = (TH1D*) f_qcd->Get(fullhistname);
  if (h_qcd != 0) n_qcd = h_qcd->GetBinContent(mt2bin);
  //  if (n_qcd < 0.01) n_qcd = 0.01;
  TH1D* h_qcd_cryield = (TH1D*) f_qcd->Get(fullhistnameCRyield);
  if (h_qcd_cryield != 0) 
    n_qcd_cr = round(h_qcd_cryield->GetBinContent(mt2bin));
  TH1D* h_qcd_alpha = (TH1D*) f_qcd->Get(fullhistnameAlpha);
  if (h_qcd_alpha != 0) {
    qcd_alpha = h_qcd_alpha->GetBinContent(mt2bin);
    err_qcd_alpha = h_qcd_alpha->GetBinError(mt2bin);
  }

  n_bkg = n_lostlep+n_zinv+n_qcd;
  if (n_bkg < 0.001) n_qcd = 0.01;

  //  if (suppressZeroBins && ((n_sig < 0.1) || (n_sig/n_bkg < 0.02))) {
  if (suppressZeroBins && (n_sig < 0)) {
    if (verbose) std::cout << "Zero signal, card not printed: " << cardname << std::endl;
    return 0;
  }
 
  if (f_data) {
    // n_data is 0 by default
    TH1D* h_data = (TH1D*) f_data->Get(fullhistname);
    if (h_data != 0) {
      n_data = h_data->GetBinContent(mt2bin);
    } 
  } else {
    // using only MC: take observation == total bkg
    n_data = n_bkg;
  }

  int n_syst = 0;
  // ----- lost lepton bkg uncertainties
  double lostlep_shape = 1.0;
  double lostlep_crstat = 1; // transfer factor
  double lostlep_mcstat = 1. + err_lostlep_mcstat; // transfer factor stat uncertainty
  double lostlep_alphaerr = 1. + 0.05; // transfer factor syst uncertainty
  double lostlep_lepeff = 1.15;
  double lostlep_bTag = 1.2; // special for 7jets with b-tags
 
  // want this to be correlated either (1) among all bins or (2) for all bins sharing the same CR bin
  TString name_lostlep_shape = Form("llep_shape_%s_%s_%s", ht_str_crsl.c_str(), jet_str_crsl.c_str(), bjet_str_crsl.c_str());
  TString name_lostlep_crstat = Form("llep_CRstat_%s_%s_%s", ht_str_crsl.c_str(), jet_str_crsl.c_str(), bjet_str_crsl.c_str());
  TString name_lostlep_mcstat = Form("llep_MCstat_%s", channel.c_str());
  TString name_lostlep_alphaerr = Form("llep_alpha_%s_%s_%s", ht_str_crsl.c_str(), jet_str_crsl.c_str(), bjet_str_crsl.c_str());
  TString name_lostlep_lepeff = Form("llep_lepeff_%s_%s_%s", ht_str_crsl.c_str(), jet_str_crsl.c_str(), bjet_str_crsl.c_str());
  TString name_lostlep_bTag = Form("llep_bTag_%s_%s_%s", ht_str_crsl.c_str(), jet_str_crsl.c_str(), bjet_str_crsl.c_str());

  if (n_lostlep_cr > 0.) {
    lostlep_crstat = n_lostlep / n_lostlep_cr > 0 ? n_lostlep / n_lostlep_cr : last_lostlep_transfer;
    if (lostlep_crstat > 2.) lostlep_crstat = 2.; // hard bound to avoid statistical fluctuations
    if (lostlep_crstat > 0.) last_lostlep_transfer = lostlep_crstat;
  }
  else {
    lostlep_crstat = last_lostlep_transfer;
  }
  n_syst += 4; // lostlep_crstat, lostlep_mcstat, lostlep_alphaerr, lostlep_lepeff

  if (n_mt2bins > 1) {
    if (mt2bin == 1 && n_lostlep > 0.) {
      // first bin needs to compensate normalization from the rest
      float increment = 0.;
      for (int ibin=1; ibin<h_lostlep->GetNbinsX(); ibin++) 
	increment += 0.4 / (n_mt2bins - 1) * (ibin - 1) * h_lostlep->GetBinContent(ibin);
      lostlep_shape = 1. - increment/n_zinv;
    }
    else
      lostlep_shape = 1. + 0.4 / (n_mt2bins - 1) * (mt2bin - 1);
    n_syst++;  // lostlep_shape
  }
 
  if ( njets_LOW == 7 && nbjets_LOW >= 1) {
    cout<<"special case with 7jets 1btag"<<endl;
    n_syst++; // nBtag extrapolation
  }


  // ----- zinv bkg uncertainties - depend on signal region, b selection
  TString perChannel(channel.c_str());
  TString perTopoRegion(topologicalR.c_str());
  
  TString name_zinv_crstat     = integratedZinvEstimate ? "zinv_CRstat_"    +perTopoRegion : "zinv_CRstat_"+perChannel    ;
  TString name_zinv_alphaerr   = integratedZinvEstimate ? "zinv_alphaErr_"  +perTopoRegion : "zinv_alphaErr_"+perChannel  ;
  TString name_zinv_purityerr  = integratedZinvEstimate ? "zinv_purity_"    +perTopoRegion : "zinv_purity_"+perChannel    ;
  TString name_zinv_puritysyst = integratedZinvEstimate ? "zinv_puritySyst_"+perTopoRegion : "zinv_puritySyst_"+perChannel;
  TString name_zinv_zgamma = "zinv_ZGratio";
  if (uncorrelatedZGratio) name_zinv_zgamma = Form("zinv_ZGratio_%s",channel.c_str());
  TString name_zinv_zgamma_nj  = Form("zinv_ZGratio_nj_%s" , jet_str.c_str()  );
  TString name_zinv_zgamma_nb  = Form("zinv_ZGratio_nb_%s" , bjet_str.c_str() );
  TString name_zinv_zgamma_ht  = Form("zinv_ZGratio_ht_%s" , ht_str.c_str()   );
  // Only a low edge for MT2, since we want to maintain correlations between differently sized MT2 bins with the same lower edge
  TString name_zinv_zgamma_mt2 = "zinv_ZGratio_m" + toString(mt2_LOW); 
  TString name_zinv_doubleRatioOffset = "zinv_doubleRatioOffset";
  TString name_zinv_mcsyst = Form("zinv_MC_%s",channel.c_str());
  TString name_zinv_shape = "zinv_shape_"+perTopoRegion;
 
  double zinv_alpha = 1.;
  double zinv_alphaerr = 1. + err_zinv_mcstat;
  double zinv_purityerr = 1. + err_zinv_purity;
  double zinv_puritysyst = 1.10;
  double zinv_doubleRatioOffset = 1.07;
  double zinv_zgamma = -1.;
  double zinv_zgamma_nj = 1., zinv_zgamma_nb = 1., zinv_zgamma_ht = 1., zinv_zgamma_mt2 = 1.;
  double zinv_mcsyst = -1.;
  double zinv_shape = 1.;
  

  // 2+b: pure MC estimate (not used when using integrated estimate)
  if ( !integratedZinvEstimate && nbjets_LOW >= 2 ) {
    zinv_mcsyst = 2.;
    ++n_syst;
  }
  else {
    // err_zinv_mcstat, zinv_ratio_zg, n_zinv_cr
    zinv_alpha =  zinv_ratio_zg * zinv_purity * 0.92; // 0.92 is a fixed factor for "f = GJetPrompt / (GJetPrompt+QCDPrompt)"
    if (f_data) zinv_alpha *= 0.93; // data-driven correction, based on the double-ratio R(Zll/Gamma)
    if (zinv_alpha > 0.5) zinv_alpha = 0.5; // Hard bound to avoid statistical fluctuations
    n_syst += 5; // 1: zinv_alphaerr (stat on ratio). 2: gamma function. 3: purity stat unc. 4: puritySyst. 5: doubleRatioOffset
    if (integratedZinvEstimate && n_mt2bins > 1) {
      if (mt2bin == 1 && n_zinv > 0) {
	// first bin needs to compensate normalization from the rest
	float increment = 0.;
	for (int ibin=1; ibin<h_zinv->GetNbinsX(); ibin++) 
	  increment += 0.4 / (n_mt2bins - 1) * (ibin - 1) * h_zinv->GetBinContent(ibin);
	zinv_shape = 1. - increment/n_zinv;
      }
      else
	zinv_shape = 1. + 0.4 / (n_mt2bins - 1) * (mt2bin - 1);
      n_syst++;
    }
    if (fourNuisancesPerBinZGratio) {
      zinv_zgamma_nj  =  1. + zllgamma_nj ;     
      zinv_zgamma_nb  =  1. + zllgamma_nb ;
      zinv_zgamma_ht  =  1. + zllgamma_ht ; // not used in 1-jet bin
      if (njets_LOW == 1) n_syst += 2;
      else n_syst += 3;
      if (!integratedZinvEstimate) {
	zinv_zgamma_mt2 =  1. + zllgamma_mt2;	
	n_syst++;
      }
    }
    else {
      zinv_zgamma = 1.20; // old 20% uncertainty
      n_syst++;
    }
    n_zinv = n_zinv_cr * zinv_alpha; // don't use Zinv MC as central value any more!
  }
 
  // ----- qcd bkg uncertainties from data driven rphi method
  //       monojet and multijet cases should be the same at this point
  double qcd_crstat = qcd_alpha; // transfer factor
  double qcd_alphaerr = 1. + (err_qcd_alpha / qcd_alpha); // transfer factor syst uncertainty
 
  // fully uncorrelated for now, to match what ETH does
  TString name_qcd_crstat = "qcd_CRstat_"+perChannel;
  TString name_qcd_alphaerr = "qcd_alphaerr_"+perChannel;
  
  n_syst += 2;

  // ----- sig uncertainties: correlated for all bins
  double sig_syst = 1.10;
  ++n_syst;

  ofstream ofile;
  ofile.open(cardname);

  ofile <<  "imax 1  number of channels"                                                    << endl;
  ofile <<  "jmax 3  number of backgrounds"                                                 << endl;
  ofile <<  Form("kmax %d  number of nuisance parameters",n_syst)                           << endl;
  ofile <<  "------------"                                                                  << endl;
  ofile <<  Form("bin         %s",channel.c_str())                                          << endl;
  ofile <<  Form("observation %.0f",n_data)                                                 << endl;
  ofile <<  "------------"                                                                  << endl;
  ofile <<  Form("bin             %s   %s   %s   %s",channel.c_str(),channel.c_str(),channel.c_str(),channel.c_str()) << endl;
  ofile <<  "process          sig       zinv        llep      qcd"                                      << endl; 
  ofile <<  "process           0         1           2         3"                                      << endl;
  ofile <<  Form("rate            %.3f    %.3f      %.3f      %.3f",n_sig,n_zinv,n_lostlep,n_qcd) << endl;
  ofile <<  "------------"                                                                  << endl;
 
  // ---- sig systs
  ofile <<  Form("sig_syst                                            lnN   %.3f    -      -     - ",sig_syst)  << endl;

  // ---- Zinv systs
  if ( !integratedZinvEstimate && nbjets_LOW >= 2) {
    ofile <<  Form("%s       lnN    -   %.3f   -    - ",name_zinv_mcsyst.Data(),zinv_mcsyst)  << endl;
  }
  else {
    ofile <<  Form("%s       lnN    -   %.3f   -    - ",name_zinv_doubleRatioOffset.Data(),zinv_doubleRatioOffset )  << endl;
    if (fourNuisancesPerBinZGratio) {
      ofile <<  Form("%s      lnN    -   %.3f    -    - ",name_zinv_zgamma_nj.Data() ,zinv_zgamma_nj )  << endl;
      ofile <<  Form("%s      lnN    -   %.3f    -    - ",name_zinv_zgamma_nb.Data() ,zinv_zgamma_nb )  << endl;
      if (njets_LOW != 1) ofile <<  Form("%s      lnN    -   %.3f    -    - ",name_zinv_zgamma_ht.Data() ,zinv_zgamma_ht )  << endl;
      if (!integratedZinvEstimate)
        ofile <<  Form("%s      lnN    -   %.3f    -    - ",name_zinv_zgamma_mt2.Data(),zinv_zgamma_mt2)  << endl;
    }
    else {
      ofile <<  Form("%s      lnN    -   %.3f    -    - ",name_zinv_zgamma.Data(),zinv_zgamma)  << endl;     
    }
    ofile <<  Form("%s       lnN    -   %.3f   -    - ",name_zinv_puritysyst.Data(),zinv_puritysyst)  << endl;
    ofile <<  Form("%s       lnN    -   %.3f   -    - ",name_zinv_purityerr.Data(),zinv_purityerr)  << endl;
    ofile <<  Form("%s     gmN %.0f    -  %.5f   -   - ",name_zinv_crstat.Data(),n_zinv_cr,zinv_alpha)  << endl;
    ofile <<  Form("%s       lnN    -   %.3f   -    - ",name_zinv_alphaerr.Data(),zinv_alphaerr)  << endl;
    if (integratedZinvEstimate && n_mt2bins > 1)
      ofile <<  Form("%s       lnN    -   %.3f   -    - ",name_zinv_shape.Data(),zinv_shape)  << endl;
  }

  // ---- lostlep systs
  ofile <<  Form("%s        lnN    -    -    %.3f    - ",name_lostlep_lepeff.Data(),lostlep_lepeff)  << endl;
  ofile <<  Form("%s        gmN %.0f    -    -    %.5f     - ",name_lostlep_crstat.Data(),n_lostlep_cr,lostlep_crstat)  << endl;
  if ( njets_LOW == 7 && nbjets_LOW >= 1) 
    ofile <<  Form("%s    lnN    -    -   %.3f     - ",name_lostlep_bTag.Data(),lostlep_bTag)  << endl;
  ofile <<  Form("%s        lnN    -    -    %.3f    - ",name_lostlep_mcstat.Data(),lostlep_mcstat)  << endl;
  if (n_mt2bins > 1)
    ofile <<  Form("%s    lnN    -    -   %.3f     - ",name_lostlep_shape.Data(),lostlep_shape)  << endl;
  ofile <<  Form("%s        lnN    -    -    %.3f    - ",name_lostlep_alphaerr.Data(),lostlep_alphaerr)  << endl;


  // ---- QCD systs
  ofile <<  Form("%s        gmN %.0f    -    -    %.5f     - ",name_qcd_crstat.Data(),n_qcd_cr,qcd_crstat)  << endl;
  ofile <<  Form("%s        lnN    -    -    %.3f    - ",name_qcd_alphaerr.Data(),qcd_alphaerr)  << endl;

  ofile.close();

  if (verbose) std::cout << "Wrote card: " << cardname << std::endl;

  return 1;
}

//_______________________________________________________________________________
void cardMaker(string signal, string input_dir, string output_dir, bool isScan = false, bool doData = false){

  // Benchmark
  TBenchmark *bmark = new TBenchmark();
  bmark->Start("benchmark");

  // ----------------------------------------
  //  samples definition
  // ----------------------------------------

  // set input files

  f_lostlep = new TFile(Form("%s/lostlepFromCRs.root",input_dir.c_str()));
  f_zinv = new TFile(Form("%s/zinvFromGJ.root",input_dir.c_str()));
  f_purity = new TFile(Form("%s/purity.root",input_dir.c_str()));
  f_qcd = new TFile(Form("%s/qcdFromCRs.root",input_dir.c_str()));

  f_sig = new TFile(Form("%s/%s.root",input_dir.c_str(),signal.c_str()));

  if (doData) f_data = new TFile(Form("%s/data_Run2015D.root",input_dir.c_str()));

  if( f_lostlep->IsZombie() || f_zinv->IsZombie() || f_purity->IsZombie() || f_qcd->IsZombie() || f_sig->IsZombie() || (doData && f_data->IsZombie()) ) {
    std::cout << "Input file does not exist" << std::endl;
    return;
  }

  // ----------------------------------------
  //  cards definitions
  // ----------------------------------------

  set<pair<int, int> > signal_points;  
  //Loop through list of every directory in the signal file.
  //if directory begins with "sr", excluding "srbase", make cards for it.
  TIter it(f_sig->GetListOfKeys());
  TKey* k;
  string keep = "sr";
  string skip = "srbase";
  while ((k = (TKey *)it())) {
    if (strncmp (k->GetTitle(), skip.c_str(), skip.length()) == 0) continue;
    if (strncmp (k->GetTitle(), keep.c_str(), keep.length()) == 0) {//it is a signal region
      string mt2_hist_name = (k->GetTitle());
      mt2_hist_name += "/h_n_mt2bins";
      TH1D* h_n_mt2bins = (TH1D*) f_sig->Get(TString(mt2_hist_name));
      int n_mt2bins = h_n_mt2bins->GetBinContent(1);
      for (int imt2 = 1; imt2 <= n_mt2bins; ++imt2) {//Make a separate card for each MT2 bin.
	if (isScan) {
	  for (int im1 = 400; im1 <= 2000; im1 += 25) {
	    for (int im2 = 0; im2 <= 1600; im2 += 25) {
	      int result = printCard(k->GetTitle(), imt2, signal, output_dir, im1, im2);   //MT2 and scan bins with no entries are handled by printCard function.
	      if (result > 0) signal_points.insert( make_pair(im1,im2) ); 
	    } // scanM2 loop
	  } // scanM1 loop
	} // if isScan
        else {
	  printCard(k->GetTitle(), imt2, signal, output_dir);   //MT2 bins with no entries are handled by printCard function.
	}
      }
    }
  }

  if (isScan) {
    TString filename = Form("%s/points_%s.txt",output_dir.c_str(),signal.c_str());
    ofstream ofile;
    ofile.open(filename);
    cout << "--------------------------------------------" << endl
	 << "- saw nonzero signal entries for the following points: " << endl;
    for (set<pair<int,int> >::iterator it=signal_points.begin(); it!=signal_points.end(); ++it) {
      cout << signal << "_" << (*it).first << "_" << (*it).second << endl;
      ofile << signal << "_" << (*it).first << "_" << (*it).second << endl;
    }
    ofile.close();
  }
  
  bmark->Stop("benchmark");
  cout << endl;
  cout << "------------------------------" << endl;
  cout << "CPU  Time:	" << Form( "%.01f s", bmark->GetCpuTime("benchmark")  ) << endl;
  cout << "Real Time:	" << Form( "%.01f s", bmark->GetRealTime("benchmark") ) << endl;
  cout << endl;
  delete bmark;

}

#include <iostream>
#include <utility>
#include <vector>
#include <map>

#include "TROOT.h"
#include "TLatex.h"
#include "TString.h"
#include "TH2.h"
#include "THStack.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TH1.h"
#include "TPaveText.h"
#include "TKey.h"

using namespace std;

//options
const bool verbose = false;
const bool doHybrid = true; // hybrid estimate: uses CR MT2 binning until the MC CR integral is less than the threshold below
const float hybrid_nevent_threshold = 50.;
const float last_bin_relerr = 0.4; // relative extrapolation error in last bin

const int n_htbins = 5;
const float htbins[n_htbins+1] = {200, 450., 575., 1000., 1500., 3000.};
const int n_njbins = 4;
const float njbins[n_njbins+1] = {1, 2, 4, 7, 12};
const int n_nbjbins = 4;
const float nbjbins[n_nbjbins+1] = {0, 1, 2, 3, 6};

//______________________________________________________________________________
// returns the error on C = A*B (or C = A/B)
float err_mult(float A, float B, float errA, float errB, float C) {
  return sqrt(C*C*(pow(errA/A,2) + pow(errB/B,2)));
}

//_______________________________________________________________________________
void makeLostLepFromCRs( TFile* f_data , TFile* f_lostlep , vector<string> dirs, string output_name ) {

  // Generate histogram file with lostlep prediction based on 1L CRs

  TFile * outfile = new TFile(output_name.c_str(),"RECREATE") ; 
  outfile->cd();
  const unsigned int ndirs = dirs.size();
  
  for ( unsigned int idir = 0; idir < ndirs; ++idir ) {

    if (verbose) cout << "Running on dir " << dirs.at(idir) << endl;
    
    TString directory = "sr"+dirs.at(idir);
    TString fullhistname = directory + "/h_mt2bins";
    TString fullhistnameFinebin = directory + "/h_mt2";
    TString n_mt2bins_name = directory + "/h_n_mt2bins";
    TString crdir = "crsl"+TString(dirs.at(idir));
    TString fullhistnameSL = crdir+"/h_mt2bins";
    TString fullhistnameSLallbins = crdir+"/h_mt2binsAll";
    TString fullhistnameSLfinebin = crdir+"/h_mt2";
    TString fullhistnameSLHT = crdir+"/h_htbins";
    TString fullhistnameSLNj = crdir+"/h_njbins";
    TString fullhistnameSLNb = crdir+"/h_nbjbins";

    //define map of alpha variations here
    std::map<string, TH1D*> histMap;
    histMap["h_lostlepMC_sr"]                 = (TH1D*) f_lostlep->Get(fullhistname);
    histMap["h_lostlepMC_sr_lepeff_UP"]       = (TH1D*) f_lostlep->Get(fullhistname+"_lepeff_UP");
    histMap["h_lostlepMC_sr_lepeff_DN"]       = (TH1D*) f_lostlep->Get(fullhistname+"_lepeff_DN");
    histMap["h_lostlepMC_sr_jec_UP"]          = (TH1D*) f_lostlep->Get(fullhistname+"_jec_UP");
    histMap["h_lostlepMC_sr_jec_DN"]          = (TH1D*) f_lostlep->Get(fullhistname+"_jec_DN");
    histMap["h_lostlepMC_sr_renorm_UP"]       = (TH1D*) f_lostlep->Get(fullhistname+"_renorm_UP");
    histMap["h_lostlepMC_sr_renorm_DN"]       = (TH1D*) f_lostlep->Get(fullhistname+"_renorm_DN");
    histMap["h_lostlepMC_sr_mtcut"]           = (TH1D*) f_lostlep->Get(fullhistname+"_mtcut");
    histMap["h_lostlepMC_sr_tau1p_UP"]        = (TH1D*) f_lostlep->Get(fullhistname+"_tau1p_UP");
    histMap["h_lostlepMC_sr_tau1p_DN"]        = (TH1D*) f_lostlep->Get(fullhistname+"_tau1p_DN");
    histMap["h_lostlepMC_sr_tau3p_UP"]        = (TH1D*) f_lostlep->Get(fullhistname+"_tau3p_UP");
    histMap["h_lostlepMC_sr_tau3p_DN"]        = (TH1D*) f_lostlep->Get(fullhistname+"_tau3p_DN");
    histMap["h_lostlepMC_sr_btagsf_heavy_UP"] = (TH1D*) f_lostlep->Get(fullhistname+"_btagsf_heavy_UP");
    histMap["h_lostlepMC_sr_btagsf_heavy_DN"] = (TH1D*) f_lostlep->Get(fullhistname+"_btagsf_heavy_DN");
    histMap["h_lostlepMC_sr_btagsf_light_UP"] = (TH1D*) f_lostlep->Get(fullhistname+"_btagsf_light_UP");
    histMap["h_lostlepMC_sr_btagsf_light_DN"] = (TH1D*) f_lostlep->Get(fullhistname+"_btagsf_light_DN");
    
    //check for missing or empty hists
    for ( std::map<string, TH1D*>::iterator iter = histMap.begin(); iter != histMap.end(); ++iter ) {
      if (!(iter->second)){
	if (verbose) cerr << "WARNING: hist " << iter->first << " does not exist! Setting to nominal..." << endl;
	iter->second = (TH1D*)  f_lostlep->Get(fullhistname);
      }
      iter->second->SetName(iter->first.c_str());
    }
    
    TH1D* h_n_mt2bins = (TH1D*) f_lostlep->Get(n_mt2bins_name);
    const int n_mt2bins = h_n_mt2bins->GetBinContent(1);
    float* mt2bins = new float[n_mt2bins+1];
    for(int i=0; i<=(n_mt2bins); i++){
      //std::cout << "bin edge = " << h_lostlep_sr->GetBinLowEdge(i+1) << std::endl;
      mt2bins[i] = histMap["h_lostlepMC_sr"]->GetBinLowEdge(i+1);  
    }

    TH1D* h_lostlepDD_sr = 0;
    std::map<string, TH1D*> alphaMap;
    if(histMap["h_lostlepMC_sr"]) {
      h_lostlepDD_sr = (TH1D*) histMap["h_lostlepMC_sr"]->Clone("h_mt2binsSR");
      for ( std::map<string, TH1D*>::iterator iter = histMap.begin(); iter != histMap.end(); ++iter ) {
	TString name = TString(iter->first).ReplaceAll("h_lostlepMC_sr","alphaHist");
	TString nameHist = TString(iter->first).ReplaceAll("h_lostlepMC_sr","h_mt2binsAlpha");
	alphaMap[name.Data()] = (TH1D*) iter->second->Clone(nameHist);
      }
    } else {
      cout<<"couldn't find lostlep MC SR hist: "<<fullhistname<<endl;
      // make empty histogram
      h_lostlepDD_sr = new TH1D("h_mt2binsSR", "h_mt2binsSR", n_mt2bins, mt2bins);
      for ( std::map<string, TH1D*>::iterator iter = histMap.begin(); iter != histMap.end(); ++iter ) {
	TString name = TString(iter->first).ReplaceAll("h_lostlepMC_sr","alphaHist");
	TString nameHist = TString(iter->first).ReplaceAll("h_lostlepMC_sr","h_mt2binsAlpha");
	alphaMap[name.Data()] = new TH1D(nameHist.Data(), nameHist.Data(), n_mt2bins, mt2bins);
      }
    }
    TH1D* h_lostlepMC_sr_finebin = (TH1D*) f_lostlep->Get(fullhistnameFinebin);
    TH1D* h_lostlepDD_sr_finebin = 0;
    if(h_lostlepMC_sr_finebin) {
      h_lostlepDD_sr_finebin = (TH1D*) h_lostlepMC_sr_finebin->Clone("h_mt2SR");
    } else {
      cout<<"couldn't find lostlep MC SR finebin hist: "<<fullhistnameFinebin<<endl;
      // make empty histogram
      h_lostlepDD_sr_finebin = new TH1D("h_mt2SR", "h_mt2SR", 150, 0, 1500);
    }
    std::map<string, TH1D*> histMapCR;
    for ( std::map<string, TH1D*>::iterator iter = histMap.begin(); iter != histMap.end(); ++iter ) {
      TString nameCR = TString(iter->first).ReplaceAll("h_lostlepMC_sr","h_lostlepMC_cr");
      TString nameSuf = TString(iter->first).ReplaceAll("h_lostlepMC_sr","");
      histMapCR[nameCR.Data()] = (TH1D*) f_lostlep->Get(fullhistnameSL+nameSuf);
    }

    //check for missing or empty hists
    for ( std::map<string, TH1D*>::iterator iter = histMapCR.begin(); iter != histMapCR.end(); ++iter ) {
      if (!(iter->second)){
	if (verbose) cerr << "WARNING: hist " << iter->first << " does not exist! Setting to nominal..." << endl;
	iter->second = (TH1D*)  f_lostlep->Get(fullhistnameSL);
      }
      iter->second->SetName(iter->first.c_str());
    }

    int lastbin_hybrid = 1; // hybrid: will integrate all MT2 bins INCLUDING this one
    float lastmt2val_hybrid = 200.;
    // check that histograms exist
    TH1D* h_data_cr = (TH1D*) f_data->Get(fullhistnameSL);
    if (!histMapCR["h_lostlepMC_cr"] || !h_data_cr) {
      if (!histMapCR["h_lostlepMC_cr"]) cout << "couldn't find lostlep MC CR hist: " << fullhistnameSL << endl;
      if (!h_data_cr) cout << "couldn't find lostlep data CR hist: " << fullhistnameSL << endl;
    } else {

      if (doHybrid) {
	// hybrid method: use MC CR yield histogram to determine how many MT2 bins to use
	//  by default: use all MT2 bins integrated (no bin-by-bin).
	//  start from the last bin and add bins at lower MT2 until we get above the hybrid_nevent_threshold
	for ( int ibin = histMapCR["h_lostlepMC_cr"]->GetNbinsX()+1; ibin >= 1; --ibin ) {
	  if (histMapCR["h_lostlepMC_cr"]->Integral(ibin,-1) < hybrid_nevent_threshold) continue;
	  lastbin_hybrid = ibin;
	  lastmt2val_hybrid = h_data_cr->GetBinLowEdge(ibin);
	  break;
	}
      }

      for ( std::map<string, TH1D*>::iterator iter = histMap.begin(); iter != histMap.end(); ++iter ) {
	TString name   = TString(iter->first).ReplaceAll("h_lostlepMC_sr","alphaHist");
	TString nameCR = TString(iter->first).ReplaceAll("h_lostlepMC_sr","h_lostlepMC_cr");
	if (doHybrid) {
	  for ( int ibin=1; ibin <= histMapCR[nameCR.Data()]->GetNbinsX(); ++ibin ) {
	    double cr_yield = 0.;
	    double err_cr_yield = 0.;
	    if (ibin < lastbin_hybrid) {
	      cr_yield = histMapCR[nameCR.Data()]->GetBinContent(ibin);
	      err_cr_yield = histMapCR[nameCR.Data()]->GetBinError(ibin);
	    }
	    else cr_yield = histMapCR[nameCR.Data()]->IntegralAndError(lastbin_hybrid,-1,err_cr_yield);
	    double bin_val = alphaMap[name.Data()]->GetBinContent(ibin) / cr_yield;
	    double bin_err = err_mult(cr_yield, alphaMap[name.Data()]->GetBinContent(ibin), err_cr_yield, alphaMap[name.Data()]->GetBinError(ibin), bin_val);
	    alphaMap[name.Data()]->SetBinContent( ibin, bin_val );
	    alphaMap[name.Data()]->SetBinError( ibin, bin_err );
	  }
	}
	else {
	  // not doing hybrid: can just scale entire hist by 1/cr_yield, with cr_yield integrated over full topological region
	  //  note that this is neglecting the stat error on cr_yield
	  float cr_yield = histMapCR[nameCR.Data()]->Integral(0,-1);
	  alphaMap[name.Data()]->Scale(1./cr_yield);
	}
      }
    }
    
    //hack to scale certain variations by fixed amount
    //can remove once weights added to looper
    alphaMap["alphaHist_mtcut"]->Scale(1.03);

    double data_cr_totalyield = 0;
    TH1D* h_lostlepDD_cr = 0;
    TH1D* h_lostlepDD_cr_datacard = 0;
    if (h_data_cr) {
      h_lostlepDD_cr = (TH1D*) h_data_cr->Clone("h_mt2binsCRyield"); // actual number of CR events in each MT2 bin
      h_lostlepDD_cr_datacard = (TH1D*) h_data_cr->Clone("h_mt2binsCRyieldDatacard"); // CR event yields, integrated above some MT2 bin
      // if not doHybrid, this will integrate all the MT2 bins
      double data_cr_totalyield = h_lostlepDD_cr->Integral(0,-1);
      double err_cr_yield = 0.;
      double cr_yield = h_lostlepDD_cr->IntegralAndError(lastbin_hybrid,-1,err_cr_yield);
      for ( int ibin=lastbin_hybrid; ibin <= h_lostlepDD_cr_datacard->GetNbinsX(); ++ibin ) {
	h_lostlepDD_cr_datacard->SetBinContent(ibin, cr_yield);
	h_lostlepDD_cr_datacard->SetBinError(ibin, err_cr_yield);
      }
    } else {
      cout << "couldn't find data CR hist: " << fullhistnameSL << endl;
      // make empty histogram
      h_lostlepDD_cr = new TH1D("h_mt2binsCRyield", "h_mt2binsCRyield", n_mt2bins, mt2bins);
      h_lostlepDD_cr_datacard = new TH1D("h_mt2binsCRyieldDatacard", "h_mt2binsCRyieldDatacard", n_mt2bins, mt2bins);
    }

    // ------------------------------------------
    //  added to compare after normalizing MC -- MAY NEED TO FIX FOR HYBRID APPROACH
    
    TH1D* h_lostlepMC_cr_finebin = (TH1D*) f_lostlep->Get(fullhistnameSLfinebin);
    TH1D* h_lostlepMC_rescaled_cr_finebin = 0;
    if(h_lostlepMC_cr_finebin) {
      h_lostlepMC_rescaled_cr_finebin = (TH1D*) h_lostlepMC_cr_finebin->Clone("h_mt2CRMCrescaled");
    } else {
      cout<<"couldn't find lostlep MC CR finebin hist: "<<fullhistnameSLfinebin<<endl;
      // make empty histogram
      h_lostlepMC_rescaled_cr_finebin = new TH1D("h_mt2", "h_mt2", 150, 0, 1500);
    }

    TH1D* h_lostlepMC_cr_allbins = (TH1D*) f_lostlep->Get(fullhistnameSLallbins);
    TH1D* h_lostlepMC_rescaled_cr_allbins = 0;
    if(h_lostlepMC_cr_allbins) {
      h_lostlepMC_rescaled_cr_allbins = (TH1D*) h_lostlepMC_cr_allbins->Clone("h_mt2binsAllCRMChybrid");
    } else {
      cout<<"couldn't find lostlep MC CR allbins hist: "<<fullhistnameSLallbins<<endl;
      // make empty histogram - will be incompatible with other copies of this hist..
      h_lostlepMC_rescaled_cr_allbins = new TH1D("h_mt2binsAll", "h_mt2binsAll", 150, 0, 1500);
    }

    TH1D* h_htbins_lostlepMC_cr = (TH1D*) f_lostlep->Get(fullhistnameSLHT);
    TH1D* h_htbins_lostlepMC_rescaled_cr = 0;
    if(h_htbins_lostlepMC_cr) {
      h_htbins_lostlepMC_rescaled_cr = (TH1D*) h_htbins_lostlepMC_cr->Clone("h_htbinsCRMCrescaled");
    } else {
      cout<<"couldn't find lostlep MC CR finebin hist: "<<fullhistnameSLHT<<endl;
      // make empty histogram
      h_htbins_lostlepMC_rescaled_cr = new TH1D("h_htbins", "h_htbins", n_htbins, htbins);
    }

    TH1D* h_njbins_lostlepMC_cr = (TH1D*) f_lostlep->Get(fullhistnameSLNj);
    TH1D* h_njbins_lostlepMC_rescaled_cr = 0;
    if(h_njbins_lostlepMC_cr) {
      h_njbins_lostlepMC_rescaled_cr = (TH1D*) h_njbins_lostlepMC_cr->Clone("h_njbinsCRMCrescaled");
    } else {
      cout<<"couldn't find lostlep MC CR finebin hist: "<<fullhistnameSLNj<<endl;
      // make empty histogram
      h_njbins_lostlepMC_rescaled_cr = new TH1D("h_njbins", "h_njbins", n_njbins, njbins);
    }

    TH1D* h_nbjbins_lostlepMC_cr = (TH1D*) f_lostlep->Get(fullhistnameSLNb);
    TH1D* h_nbjbins_lostlepMC_rescaled_cr = 0;
    if(h_nbjbins_lostlepMC_cr) {
      h_nbjbins_lostlepMC_rescaled_cr = (TH1D*) h_nbjbins_lostlepMC_cr->Clone("h_nbjbinsCRMCrescaled");
    } else {
      cout<<"couldn't find lostlep MC CR finebin hist: "<<fullhistnameSLNb<<endl;
      // make empty histogram
      h_nbjbins_lostlepMC_rescaled_cr = new TH1D("h_nbjbins", "h_nbjbins", n_nbjbins, nbjbins);
    }


    TH1D* h_data_cr_finebin = (TH1D*) f_data->Get(fullhistnameSLfinebin);
    TH1D* h_data_cr_finebin_save = 0;
    if (h_data_cr_finebin) {
      h_data_cr_finebin_save = (TH1D*) h_data_cr_finebin->Clone("h_mt2CRyield");
    } else {
      cout << "couldn't find data CR finebin hist: " << fullhistnameSLfinebin << endl;
      // make empty histogram
      h_data_cr_finebin_save = new TH1D("h_mt2CRyield", "h_mt2CRyield", 150, 0, 1500);
    }

    TH1D* h_htbins_data_cr = (TH1D*) f_data->Get(fullhistnameSLHT);
    TH1D* h_htbins_data_cr_save = 0;
    if (h_htbins_data_cr) {
      h_htbins_data_cr_save = (TH1D*) h_htbins_data_cr->Clone("h_htbinsCRyield");
    } else {
      cout << "couldn't find data CR finebin hist: " << fullhistnameSLHT << endl;
      // make empty histogram
      h_htbins_data_cr_save = new TH1D("h_htbinsCRyield", "h_htbinsCRyield", n_htbins, htbins);
    }
    
    TH1D* h_njbins_data_cr = (TH1D*) f_data->Get(fullhistnameSLNj);
    TH1D* h_njbins_data_cr_save = 0;
    if (h_njbins_data_cr) {
      h_njbins_data_cr_save = (TH1D*) h_njbins_data_cr->Clone("h_njbinsCRyield");
    } else {
      cout << "couldn't find data CR finebin hist: " << fullhistnameSLNj << endl;
      // make empty histogram
      h_njbins_data_cr_save = new TH1D("h_njbinsCRyield", "h_njbinsCRyield", n_njbins, njbins);
    }
    
    TH1D* h_nbjbins_data_cr = (TH1D*) f_data->Get(fullhistnameSLNb);
    TH1D* h_nbjbins_data_cr_save = 0;
    if (h_nbjbins_data_cr) {
      h_nbjbins_data_cr_save = (TH1D*) h_nbjbins_data_cr->Clone("h_nbjbinsCRyield");
    } else {
      cout << "couldn't find data CR finebin hist: " << fullhistnameSLNb << endl;
      // make empty histogram
      h_nbjbins_data_cr_save = new TH1D("h_nbjbinsCRyield", "h_nbjbinsCRyield", n_nbjbins, nbjbins);
    }
    
    // ------------------------------------------

    // only needed if we have to create an empty histogram for the prediction
    delete mt2bins;

    if (h_lostlepDD_sr->GetNbinsX() != h_lostlepDD_cr->GetNbinsX() ) {
      cout<<"WARNING: different binning for histograms "<<fullhistname<<endl;
      continue;
    }
    
    // data-driven part: use data to normalize MC SR prediction
    if (h_lostlepDD_cr_datacard && alphaMap["alphaHist"]) {
      // multiply CR yields by alpha to get prediction
      h_lostlepDD_sr->Multiply(h_lostlepDD_cr_datacard,alphaMap["alphaHist"],1.,1.);
    }

    // other histograms: just normalize based on total yield in topological region
    double norm = 1.;
    if (histMapCR["h_lostlepMC_cr"]) {
      norm = data_cr_totalyield/histMapCR["h_lostlepMC_cr"]->Integral(0,-1);
    }
    h_lostlepDD_sr_finebin->Scale(norm);
    h_lostlepMC_rescaled_cr_finebin->Scale(norm);
    h_htbins_lostlepMC_rescaled_cr->Scale(norm);
    h_njbins_lostlepMC_rescaled_cr->Scale(norm);
    h_nbjbins_lostlepMC_rescaled_cr->Scale(norm);

    // mt2binsAll: normalize MC to data below lastmt2val_hybrid, then MC to the data integral above
    TH1D* h_data_cr_allbins = (TH1D*) f_data->Get(fullhistnameSLallbins);
    if (h_data_cr_allbins) {
      int lastbin_hybrid_allbins = h_data_cr_allbins->FindBin(lastmt2val_hybrid);
      for (int ibin = 1; ibin <= h_data_cr_allbins->GetNbinsX(); ++ibin) {
	// using bin-by-bin: set MC to data yield
	if (ibin < lastbin_hybrid_allbins) {
	  double data_val = h_data_cr_allbins->GetBinContent(ibin);
	  double mc_val = h_lostlepMC_cr_allbins->GetBinContent(ibin);
	  double mc_err = h_lostlepMC_cr_allbins->GetBinError(ibin);
	  h_lostlepMC_rescaled_cr_allbins->SetBinContent(ibin, data_val);
	  if (mc_val > 0.) h_lostlepMC_rescaled_cr_allbins->SetBinError(ibin, mc_err * data_val/mc_val);
	}
	else {
	  double data_int = h_data_cr_allbins->Integral(lastbin_hybrid_allbins,-1);
	  double mc_int = h_lostlepMC_cr_allbins->Integral(lastbin_hybrid_allbins,-1);
	  double mc_val = h_lostlepMC_cr_allbins->GetBinContent(ibin);
	  double mc_err = h_lostlepMC_cr_allbins->GetBinError(ibin);
	  if (mc_int > 0.) {
	    h_lostlepMC_rescaled_cr_allbins->SetBinContent(ibin, mc_val * data_int/mc_int);
	    h_lostlepMC_rescaled_cr_allbins->SetBinError(ibin, mc_err * data_int/mc_int);
	  }
	}
      }
    } else {
      cout << "WARNING: didn't find histogram in data: " << fullhistnameSLallbins << endl;
    }

    // Make directory and plot(s) in the output file
    TDirectory* dir = 0;
    dir = (TDirectory*)outfile->Get(directory.Data());
    if (dir == 0) {
      dir = outfile->mkdir(directory.Data());
    } 
    dir->cd();
    
    // dummy: no systs for now
    TH1D* Syst = (TH1D*) h_lostlepDD_sr->Clone("h_mt2binsSyst");

    // MC stats needed for syst
    TH1D* MCStat = (TH1D*) h_lostlepDD_sr->Clone("h_mt2binsMCStat");
    
    // don't do anything special for errors for now on pred hist
    TH1D* pred = (TH1D*) h_lostlepDD_sr->Clone("h_mt2bins");
    TH1D* pred_finebin = (TH1D*) h_lostlepDD_sr_finebin->Clone("h_mt2");
    // for ( int ibin = 0; ibin <= Stat->GetNbinsX(); ++ibin) { 
    //   Syst->SetBinError(ibin, 0.);
    //   double quadrature = Stat->GetBinError(ibin)*Stat->GetBinError(ibin) + Syst->GetBinError(ibin)*Syst->GetBinError(ibin);
    //   pred->SetBinError(ibin, sqrt(quadrature));
    // }
    //pred->Print("all");
    
    // also save bin boundary hists for CR
    TH1D* h_ht_LOW = (TH1D*) f_lostlep->Get(crdir+"/h_ht_LOW");
    TH1D* h_ht_HI = (TH1D*) f_lostlep->Get(crdir+"/h_ht_HI");
    TH1D* h_nbjets_LOW = (TH1D*) f_lostlep->Get(crdir+"/h_nbjets_LOW");
    TH1D* h_nbjets_HI = (TH1D*) f_lostlep->Get(crdir+"/h_nbjets_HI");
    TH1D* h_njets_LOW = (TH1D*) f_lostlep->Get(crdir+"/h_njets_LOW");
    TH1D* h_njets_HI = (TH1D*) f_lostlep->Get(crdir+"/h_njets_HI");

    int ht_LOW = -999;
    int ht_HI = -999;
    int njets_LOW = -999;
    int njets_HI = -999;
    int nbjets_LOW = -999;
    int nbjets_HI = -999;
    if (h_ht_LOW)  ht_LOW = h_ht_LOW->GetBinContent(1);
    else {
      if (verbose) cout << "h_ht_LOW does not exist! setting to -1." << endl;
      h_ht_LOW = (TH1D*) 0;
      ht_LOW = -1;
    }
    if (h_ht_HI)  ht_HI = h_ht_HI->GetBinContent(1);
    else {
      if (verbose) cout << "h_ht_HI does not exist! setting to -1." << endl;
      h_ht_HI = (TH1D*) 0;
      ht_HI = -1;
    }
    if (h_njets_LOW)  njets_LOW = h_njets_LOW->GetBinContent(1);
    else {
      if (verbose) cout << "h_njets_LOW does not exist! setting to -1." << endl;
      h_njets_LOW = (TH1D*) 0;
      njets_LOW = -1;
    }
    if (h_njets_HI)  njets_HI = h_njets_HI->GetBinContent(1);
    else {
      if (verbose) cout << "h_njets_HI does not exist! setting to -1." << endl;
      h_njets_HI = (TH1D*) 0;
      njets_HI = -1;
    }
    if (h_nbjets_LOW) nbjets_LOW = h_nbjets_LOW->GetBinContent(1);
    else {
      if (verbose) cout << "h_nbjets_LOW does not exist! setting to -1." << endl;
      h_nbjets_LOW = (TH1D*) 0;
      nbjets_LOW = -1;
    }
    if (h_nbjets_HI) nbjets_HI = h_nbjets_HI->GetBinContent(1);
    else {
      if (verbose) cout << "h_nbjets_HI does not exist! setting to -1." << endl;
      h_nbjets_HI = (TH1D*) 0;
      nbjets_HI = -1;
    }
    
    // set uncertainties on finebin prediction hist
    // - MC stat (already included)
    // - data CR stat -> use just sqrt(N) instead of (correct) poisson uncertainties
    // - 15% for lepton efficiency
    // - 5% transfer factor error for most bins, additional 20% for 7j,>=1b
    // - not including MT2 shape uncertainty at this point
    // NOTE that this sets uncertainty to 0 for bins with 0 data CR stats (i.e. pred = 0)... 
    for ( int ibin = 1; ibin <= pred_finebin->GetNbinsX(); ++ibin) {
      float val = pred_finebin->GetBinContent(ibin);
      if (val <= 0.) continue;
      float err_mcstat = h_lostlepMC_sr_finebin->GetBinError(ibin)/h_lostlepMC_sr_finebin->GetBinContent(ibin);
      float err_datastat = (data_cr_totalyield > 0) ? sqrt(data_cr_totalyield)/data_cr_totalyield : 0.; // should never get 0 data CR yield and nonzero pred
      float quadrature = err_mcstat*err_mcstat + err_datastat*err_datastat + 0.15*0.15 + 0.05*0.05;
      if (njets_LOW >= 7 && nbjets_LOW >= 1) quadrature += 0.2*0.2;
      pred_finebin->SetBinError(ibin,val*sqrt(quadrature));
    }
    //    pred_finebin->Print("all");

    //    pred->Print("all");

    // MC extrapolation systematic uncertainty: goes up to last_bin_relerr in last bin
    int n_extrap_bins = n_mt2bins - lastbin_hybrid;
    TH1D* MCExtrap = (TH1D*) h_lostlepDD_sr->Clone("h_mt2binsMCExtrap");
    for ( int ibin = 1; ibin <= pred->GetNbinsX(); ++ibin ) {
      if (ibin < lastbin_hybrid) continue; // no uncertainty if bin-by-bin
      else if (ibin == lastbin_hybrid) {
	// first bin needs to compensate normalization from the rest
	float increment = 0.;
	for (int jbin=lastbin_hybrid+1; jbin<=pred->GetNbinsX(); ++jbin) 
	  increment += last_bin_relerr / (n_extrap_bins) * (jbin - lastbin_hybrid) * pred->GetBinContent(jbin);
	MCExtrap->SetBinContent(ibin, pred->GetBinContent(ibin) - increment);
      }
      else {
	float relvar = last_bin_relerr / (n_extrap_bins) * (ibin - lastbin_hybrid);
	MCExtrap->SetBinContent(ibin, pred->GetBinContent(ibin) * (1. + relvar));
      }
    }
  
    // mt2binsAll with extrapolation error
    TH1D* h_lostlepMC_rescaled_cr_allbins_MCExtrapErr = (TH1D*) h_lostlepMC_cr_allbins->Clone("h_mt2binsAllCRMChybridExtrapErr");
    for (int ibin = 1; ibin <= h_lostlepMC_cr_allbins->GetNbinsX(); ++ibin) {
      // scale by relative error from the standard MT2 binning plot
      int sig_mt2bin = pred->FindBin(h_lostlepMC_cr_allbins->GetBinLowEdge(ibin));
      float relvar = (pred->GetBinContent(sig_mt2bin) > 0.) ? (MCExtrap->GetBinContent(sig_mt2bin) / pred->GetBinContent(sig_mt2bin)) : 0.;
      h_lostlepMC_rescaled_cr_allbins_MCExtrapErr->SetBinContent( ibin, h_lostlepMC_rescaled_cr_allbins->GetBinContent(ibin) * relvar );
    }
    
    // store info on which bin / MT2 threshold is the last used in hybrid method
    TH1D* h_lastbinHybrid = new TH1D("h_lastbinHybrid",";last bin",1,0,1);
    h_lastbinHybrid->SetBinContent(1,lastbin_hybrid);
    TH1D* h_lastmt2Hybrid = new TH1D("h_lastmt2Hybrid",";last M_{T2} value",1,0,1);
    h_lastmt2Hybrid->SetBinContent(1,lastmt2val_hybrid);
    
    pred->Write();
    pred_finebin->Write();
    Syst->Write();
    h_lostlepDD_cr->Write();
    h_lostlepDD_cr_datacard->Write();
    MCStat->Write();
    MCExtrap->Write();
    for ( std::map<string, TH1D*>::iterator iter = alphaMap.begin(); iter != alphaMap.end(); ++iter ) {
      iter->second->Write();
    }
    for ( std::map<string, TH1D*>::iterator iter = histMap.begin(); iter != histMap.end(); ++iter ) {
      iter->second->Write();
    }
    for ( std::map<string, TH1D*>::iterator iter = histMapCR.begin(); iter != histMapCR.end(); ++iter ) {
      iter->second->Write();
    }
    h_lostlepMC_rescaled_cr_finebin->Write();
    h_data_cr_finebin_save->Write();
    h_lostlepMC_rescaled_cr_allbins->Write();
    h_lostlepMC_rescaled_cr_allbins_MCExtrapErr->Write();
    h_htbins_lostlepMC_rescaled_cr->Write();
    h_njbins_lostlepMC_rescaled_cr->Write();
    h_nbjbins_lostlepMC_rescaled_cr->Write();
    h_htbins_data_cr_save->Write();
    h_njbins_data_cr_save->Write();
    h_nbjbins_data_cr_save->Write();    
    h_ht_LOW->Write();
    h_ht_HI->Write();
    h_nbjets_LOW->Write();
    h_nbjets_HI->Write();
    h_njets_LOW->Write();
    h_njets_HI->Write();
    h_lastbinHybrid->Write();
    h_lastmt2Hybrid->Write();
    
  } // loop over signal regions

  outfile->Close("R");
  
  return;
}

//_______________________________________________________________________________
void lostlepMaker(string input_dir = "/home/users/jgran/temp/update/MT2Analysis/MT2looper/output/V00-00-12/", string dataname = "lostlep"){

  string output_name = input_dir+"/lostlepFromCRs.root";
  std::cout << "Writing to file: " << output_name << std::endl;

  // get input files -- default to faking data with same MC file
  TFile* f_data = new TFile(Form("%s/%s.root",input_dir.c_str(),dataname.c_str()));
  TFile* f_lostlep = new TFile(Form("%s/lostlep.root",input_dir.c_str()));
  
  if(f_data->IsZombie() || f_lostlep->IsZombie()) {
    std::cerr << "Input file does not exist" << std::endl;
    return;
  }

  vector<string> dirs;

  //Loop through list of every directory in the signal file.
  //if directory begins with "sr", excluding "srbase", add it to vector signal regions.
  TIter it(f_lostlep->GetListOfKeys());
  TKey* k;
  std::string keep = "sr";
  std::string skip = "srbase";
  while ((k = (TKey *)it())) {
    //if ( (strncmp (k->GetTitle(), skip.c_str(), skip.length()) == 0) && k->GetTitle() != "srbaseJ0B" &&  k->GetTitle() != "srbaseJ1B" ) continue;
    if (strncmp (k->GetTitle(), skip.c_str(), skip.length()) == 0) continue;
    if (strncmp (k->GetTitle(), keep.c_str(), keep.length()) == 0) {//it is a signal region
      std::string sr_string = k->GetTitle();
      sr_string.erase(0, 2);//remove "sr" from front of string
      dirs.push_back(sr_string);
    }
  }

  makeLostLepFromCRs( f_data , f_lostlep , dirs, output_name );

}

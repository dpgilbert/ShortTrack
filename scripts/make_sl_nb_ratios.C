#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TString.h"
#include "TLegend.h"
#include <vector>
#include <string>

int colors[4] = {2,4,3,6};

TFile* fdata(0);
TFile* fwjets(0);
TFile* ftop(0);

//______________________________________________________________________________
// returns the error on C = A*B (or C = A/B)
float err_mult(float A, float B, float errA, float errB, float C) {
  return sqrt(C*C*(pow(errA/A,2) + pow(errB/B,2)));
}

//_______________________________________________________________________________
void make_sl_nb_ratios() {

  TH1::SetDefaultSumw2();

  TString basedir = "/home/olivito/cms3/MT2Analysis/SingleLepLooper/output/V00-08-08_nojson_12p9fb_fixsel/";
  
  fdata = new TFile(Form("%s/data_Run2016.root",basedir.Data()), "READ");
  fwjets = new TFile(Form("%s/wjets_incl.root",basedir.Data()), "READ");
  ftop = new TFile(Form("%s/top.root",basedir.Data()), "READ"); // hadd'ed version of ttsl, singletop
  std::string histname = "h_nBJet20";

  std::vector<std::string> regions;

  const int nbcut_bin = 3; // < 3 b-tags

  regions.push_back("crslmet50mt30to100nj7nb1");

  // loop over regions
  for (unsigned int j=0; j<regions.size(); ++j) {

    TString fullhistname = Form("%s/%s", regions.at(j).c_str(),histname.c_str());
    TH1D* h_data = (TH1D*) fdata->Get(fullhistname);
    TH1D* h_wjets = (TH1D*) fwjets->Get(fullhistname);
    TH1D* h_top = (TH1D*) ftop->Get(fullhistname);
    if (!h_data) std::cout << "WARNING: couldn't find hist for data" << std::endl;
    if (!h_wjets) std::cout << "WARNING: couldn't find hist for wjets" << std::endl;
    if (!h_top) std::cout << "WARNING: couldn't find hist for top" << std::endl;

    Double_t err_data_below;
    Double_t yield_data_below = h_data->IntegralAndError(0, nbcut_bin, err_data_below);
    Double_t err_data_above;
    Double_t yield_data_above = h_data->IntegralAndError(nbcut_bin+1, -1, err_data_above);
      
    Double_t err_wjets_below;
    Double_t yield_wjets_below = h_wjets->IntegralAndError(0, nbcut_bin, err_wjets_below);
    Double_t err_wjets_above;
    Double_t yield_wjets_above = h_wjets->IntegralAndError(nbcut_bin+1, -1, err_wjets_above);
      
    Double_t err_top_below;
    Double_t yield_top_below = h_top->IntegralAndError(0, nbcut_bin, err_top_below);
    Double_t err_top_above;
    Double_t yield_top_above = h_top->IntegralAndError(nbcut_bin+1, -1, err_top_above);

    Double_t yield_mc_below = yield_wjets_below + yield_top_below;
    Double_t err_mc_below = sqrt(pow(err_wjets_below,2) + pow(err_top_below,2));
    Double_t yield_mc_above = yield_wjets_above + yield_top_above;
    Double_t err_mc_above = sqrt(pow(err_wjets_above,2) + pow(err_top_above,2));

    Double_t ratio_mc = yield_mc_above / yield_mc_below;
    Double_t err_ratio_mc = err_mult(yield_mc_above, yield_mc_below, err_mc_above, err_mc_below, ratio_mc);
    Double_t ratio_data = yield_data_above / yield_data_below;
    Double_t err_ratio_data = err_mult(yield_data_above, yield_data_below, err_data_above, err_data_below, ratio_data);

    Double_t ratio_data_mc = ratio_data / ratio_mc;
    Double_t err_ratio_data_mc = err_mult(ratio_data, ratio_mc, err_ratio_data, err_ratio_mc, ratio_data_mc);

    std::cout << regions.at(j) << " & " << Form("%.4f $\\pm$ %.4f",ratio_data,err_ratio_data)
	      << " & " << Form("%.4f $\\pm$ %.4f",ratio_mc,err_ratio_mc)
	      << " & " << Form("%.4f $\\pm$ %.4f",ratio_data_mc,err_ratio_data_mc) << endl;
  }

  fdata->Close();
  fwjets->Close();
  ftop->Close();
}


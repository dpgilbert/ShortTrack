#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TString.h"
#include "TLegend.h"
#include <vector>
#include <string>

int colors[4] = {2,4,3,6};

TFile* fdata(0);
TFile* fzjets(0);
TFile* ftop(0);

//______________________________________________________________________________
// returns the error on C = A/(A+B) 
//  note that if A and B are integers, simplifies to sqrt((C * (1-C)) / (A+B))
//  or thinking of an efficiency, sqrt((eff * (1-eff)) / N)
Double_t err_binomial(float A, float B, float errA, float errB) {
  return (1/pow(A+B,2)) * sqrt(pow(B*errA,2) + pow(A*errB,2));
}

//_______________________________________________________________________________
void make_mt_tail_fractions() {

  TH1::SetDefaultSumw2();

  TString basedir = "/home/olivito/cms3/MT2Analysis/ZllMTLooper/output/V00-08-08_nojson_12p9fb_sfs/";
  
  fdata = new TFile(Form("%s/data_Run2016.root",basedir.Data()), "READ");
  fzjets = new TFile(Form("%s/dyjetsll_incl.root",basedir.Data()), "READ");
  //  ftop = new TFile(Form("%s/ttdl_mg_lo.root",basedir.Data()), "READ");
  ftop = new TFile(Form("%s/top.root",basedir.Data()), "READ"); // hadded version of ttdl, singletop
  std::string histname = "h_zllmt_mt";
  std::vector<std::string> vars;
  vars.push_back("");
  vars.push_back("_mt2gt50");
  vars.push_back("_mt2gt100");
  vars.push_back("_mt2gt150");
  vars.push_back("_mt2gt200");

  std::vector<std::string> regions;

  const int mtcut_bin = 55; // < 110 GeV

  regions.push_back("crmtht200");
  regions.push_back("crmtht450");

  // loop over regions
  for (unsigned int j=0; j<regions.size(); ++j) {

    // loop over variations
    for (unsigned int ivar = 0; ivar < vars.size(); ++ivar) {
      TString fullhistname = Form("%s/%s%s", regions.at(j).c_str(),histname.c_str(),vars.at(ivar).c_str());
      TH1D* h_data = (TH1D*) fdata->Get(fullhistname);
      TH1D* h_zjets = (TH1D*) fzjets->Get(fullhistname);
      TH1D* h_top = (TH1D*) ftop->Get(fullhistname);
      if (!h_data) std::cout << "WARNING: couldn't find hist for data" << std::endl;
      if (!h_zjets) std::cout << "WARNING: couldn't find hist for zjets" << std::endl;
      if (!h_top) std::cout << "WARNING: couldn't find hist for top" << std::endl;

      Double_t err_data_tot;
      Double_t yield_data_tot = h_data->IntegralAndError(0, -1, err_data_tot);
      Double_t err_data_pass;
      Double_t yield_data_pass = h_data->IntegralAndError(0, mtcut_bin, err_data_pass);
      
      Double_t err_zjets_tot;
      Double_t yield_zjets_tot = h_zjets->IntegralAndError(0, -1, err_zjets_tot);
      Double_t err_zjets_pass;
      Double_t yield_zjets_pass = h_zjets->IntegralAndError(0, mtcut_bin, err_zjets_pass);
      Double_t err_zjets_fail;
      Double_t yield_zjets_fail = h_zjets->IntegralAndError(mtcut_bin+1, -1, err_zjets_fail);
      
      Double_t err_top_tot;
      Double_t yield_top_tot = h_top->IntegralAndError(0, -1, err_top_tot);
      Double_t err_top_pass;
      Double_t yield_top_pass = h_top->IntegralAndError(0, mtcut_bin, err_top_pass);
      Double_t err_top_fail;
      Double_t yield_top_fail = h_top->IntegralAndError(mtcut_bin+1, -1, err_top_fail);

      Double_t yield_data_tot_cor = yield_data_tot - yield_top_tot;
      Double_t yield_data_pass_cor = yield_data_pass - yield_top_pass;
      Double_t yield_data_fail_cor = yield_data_tot_cor - yield_data_pass_cor;

      Double_t frac_data_cor = yield_data_pass_cor / yield_data_tot_cor;
      Double_t err_frac_data_cor = err_binomial(yield_data_pass_cor, yield_data_fail_cor, sqrt(yield_data_pass_cor), sqrt(yield_data_fail_cor));
      //      Double_t err_frac_data_cor = err_binomial(yield_data_pass_cor, yield_data_fail_cor, sqrt(yield_data_pass_cor + pow(0.1 * yield_top_pass,2)), sqrt(yield_data_fail_cor + pow(0.1 * yield_top_fail,2)));

      Double_t frac_zjets = yield_zjets_pass / yield_zjets_tot;
      Double_t err_frac_zjets = err_binomial(yield_zjets_pass, yield_zjets_fail, err_zjets_pass, err_zjets_fail);

      // std::cout << "region: " << regions.at(j) << ", variation: " << vars.at(ivar) << std::endl
      // 		<< "  - data yield MT < 110 GeV: " << yield_data_pass << ", MT > 110 GeV: " << yield_data_tot - yield_data_pass << std::endl
      // 		<< "  - zjets yield MT < 110 GeV: " << yield_zjets_pass << ", MT > 110 GeV: " << yield_zjets_fail << std::endl
      // 		<< "  - top yield MT < 110 GeV: " << yield_top_pass << ", MT > 110 GeV: " << yield_top_fail << std::endl
      // 		<< "  - data fraction passing MT < 110 GeV: " << frac_data_cor << " +/- " << err_frac_data_cor << std::endl
      // 		<< "  - zjets fraction passing MT < 110 GeV: " << frac_zjets << " +/- " << err_frac_zjets << std::endl;
      
      std::cout << vars.at(ivar) << " & " << Form("%.3f $\\pm$ %.3f",frac_data_cor,err_frac_data_cor)
      		<< " & " << Form("%.3f $\\pm$ %.3f",frac_zjets,err_frac_zjets) << " \\\\" << std::endl;
    }
std::cout << std::endl << std::endl;
  }

  fdata->Close();
  fzjets->Close();
  ftop->Close();
}


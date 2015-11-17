#include "TFile.h"
#include "TCanvas.h"
#include "TH1.h"
#include "TString.h"
#include "TLegend.h"
#include <vector>
#include <string>

int colors[4] = {2,4,3,6};

//std::string savedir = "btagsf_ttbar_errcomp_ext1/";
//std::string savedir = "btagsf_wjets_errcomp_ext1/";
std::string savedir = "btagsf_zinv_errcomp_ext1/";

TFile* f1(0);

void make_plot(std::string region, TH1D* h, std::vector<TH1D*> hists_vars, std::vector<std::string> vars);
std::string getRegionName(TString dir);

//_______________________________________________________________________________
void ReplaceString(std::string& subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}

//_______________________________________________________________________________
void compare_mt2bins() {

  TH1::SetDefaultSumw2();
  
  //  f1 = new TFile("/home/olivito/cms3/MT2Analysis/MT2looper/output/V00-01-06_25ns_skim_base_3fb_mt2gt200_monojet_btagsf_ignore0weight/ttsl_mg_lo.root", "READ");
  //f1 = new TFile("/home/olivito/cms3/MT2Analysis/MT2looper/output/V00-01-06_25ns_skim_base_3fb_mt2gt200_monojet_int_btagsf_vars/wjets_ht.root", "READ");
  //  f1 = new TFile("/home/olivito/cms3/MT2Analysis/MT2looper/output/V00-01-07_25ns_miniaodv2_skim_base_1p26fb_mt2gt200_btagsf_vars/ttsl_mg_lo.root", "READ");
  //  f1 = new TFile("/home/olivito/cms3/MT2Analysis/MT2looper/output/V00-01-07_25ns_miniaodv2_skim_base_1p26fb_mt2gt200_btagsf_vars/wjets_ht.root", "READ");
  f1 = new TFile("/home/olivito/cms3/MT2Analysis/MT2looper/output/V00-01-07_25ns_miniaodv2_skim_base_1p26fb_mt2gt200_btagsf_vars/ttsl_mg_lo.root", "READ");
  std::string central_histname = "h_mt2bins";
  std::vector<std::string> vars;
  vars.push_back("btagsf_UP");
  vars.push_back("btagsf_DN");

  std::vector<std::string> regions;

  regions.push_back("srbaseJ0B");
  regions.push_back("srbaseJ1B");
  
  regions.push_back("sr1VL");
  regions.push_back("sr2VL");
  regions.push_back("sr3VL");
  regions.push_back("sr4VL");
  regions.push_back("sr5VL");
  regions.push_back("sr6VL");
  regions.push_back("sr7VL");
  regions.push_back("sr8VL");
  regions.push_back("sr9VL");
  regions.push_back("sr10VL");
  regions.push_back("sr11VL");
  regions.push_back("sr1L");
  regions.push_back("sr2L");
  regions.push_back("sr3L");
  regions.push_back("sr4L");
  regions.push_back("sr5L");
  regions.push_back("sr6L");
  regions.push_back("sr7L");
  regions.push_back("sr8L");
  regions.push_back("sr9L");
  regions.push_back("sr10L");
  regions.push_back("sr11L");
  regions.push_back("sr1M");
  regions.push_back("sr2M");
  regions.push_back("sr3M");
  regions.push_back("sr4M");
  regions.push_back("sr5M");
  regions.push_back("sr6M");
  regions.push_back("sr7M");
  regions.push_back("sr8M");
  regions.push_back("sr9M");
  regions.push_back("sr10M");
  regions.push_back("sr11M");
  regions.push_back("sr1H");
  regions.push_back("sr2H");
  regions.push_back("sr3H");
  regions.push_back("sr4H");
  regions.push_back("sr5H");
  regions.push_back("sr6H");
  regions.push_back("sr7H");
  regions.push_back("sr8H");
  regions.push_back("sr9H");
  regions.push_back("sr10H");
  regions.push_back("sr11H");
  regions.push_back("sr1UH");
  regions.push_back("sr2UH");
  regions.push_back("sr3UH");
  regions.push_back("sr4UH");
  regions.push_back("sr5UH");
  regions.push_back("sr6UH");
  regions.push_back("sr7UH");
  regions.push_back("sr8UH");
  regions.push_back("sr9UH");
  regions.push_back("sr10UH");
  regions.push_back("sr11UH");

  // loop over regions
  for (unsigned int j=0; j<regions.size(); ++j) {
    TString fullhistname = Form("%s/%s", regions.at(j).c_str(),central_histname.c_str());
    TH1D* h = (TH1D*) f1->Get(fullhistname);
    if (h == 0) continue;

    std::vector<TH1D*> hists_vars;
    // loop over variations
    for (unsigned int ivar = 0; ivar < vars.size(); ++ivar) {
      TString fullhistname_var = Form("%s_%s",fullhistname.Data(),vars.at(ivar).c_str());
      TH1D* h_var = (TH1D*) f1->Get(fullhistname_var);
      hists_vars.push_back(h_var);
    }

    make_plot(regions.at(j), h, hists_vars, vars);

  }

  f1->Close();
}

//_______________________________________________________________________________
void make_plot(std::string region, TH1D* h, std::vector<TH1D*> hists_vars, std::vector<std::string> vars) {

  std::string region_name = getRegionName(TString(region.c_str()));
  
  TCanvas* c2 = new TCanvas("c2", "", 600, 600);
  c2->cd();
  
  TPad *pad1 = new TPad("pad1","pad1",0,0.2,1,0.99);
  pad1->SetBottomMargin(0.12);
  pad1->SetRightMargin(0.05);
  //  pad1->SetLeftMargin(0.05);
  pad1->Draw();
  pad1->cd();

  pad1->SetLogy();

  h->SetLineColor(kBlack);
  if (region_name.find("j1") != std::string::npos) h->GetXaxis()->SetTitle("p_{T}(jet1) [GeV]");
  h->Draw("histe");
  double norm = h->Integral(0,-1);

  TLegend* legend = new TLegend( 0.6, 0.6, 0.85, 0.8 );
  legend->SetBorderSize(0);
  legend->SetTextSize(0.038);
  legend->SetTextFont(42);
  legend->SetFillColor(0);
  legend->AddEntry( h, "central", "l" );

  for (unsigned int i=0; i<hists_vars.size(); ++i) {
    TH1D* h_var = hists_vars.at(i);
    h_var->SetLineColor(colors[i]);
    double norm_var = h_var->Integral(0,-1);
    h_var->Scale(norm/norm_var);
    h_var->Draw("histe same");
    legend->AddEntry(h_var, vars.at(i).c_str(), "l");
  }

  legend->Draw("same");


  //  gPad->RedrawAxis();
  
  c2->cd();
  TPad *pad2 = new TPad("pad2","pad2",0.,0.,1,0.21);
  pad2->SetTopMargin(0.05);
  pad2->SetBottomMargin(0.1);
  pad2->SetRightMargin(0.05);
  //  pad2->SetLeftMargin(0.05);
  pad2->Draw();
  pad2->cd();
  pad2->SetGridy();

  TH2D* h2_axes_ratio = new TH2D("axes_ratio", "", 130, 200, 1500, 20, 0.5, 1.5 );
  h2_axes_ratio->SetStats(0);
  h2_axes_ratio->GetXaxis()->SetLabelSize(0.00);
  h2_axes_ratio->GetXaxis()->SetTickLength(0.09);
  h2_axes_ratio->GetYaxis()->SetNdivisions(5,5,0);
  h2_axes_ratio->GetYaxis()->SetTitleSize(0.15);
  h2_axes_ratio->GetYaxis()->SetTitleOffset(0.27);
  h2_axes_ratio->GetYaxis()->SetLabelSize(0.17);
  h2_axes_ratio->GetYaxis()->SetTitle("Var/Central");

  TLine* LineCentral = new TLine(200, 1., 1500., 1.);
  //TLine* LineCentral = new TLine(0, 1., 44, 1.);
  LineCentral->SetLineColor(1);

  h2_axes_ratio->Draw("");
  //  LineCentral->Draw("same");

  for (unsigned int i=0; i<hists_vars.size(); ++i) {
    TH1D* h_ratio = (TH1D*) hists_vars.at(i)->Clone(Form("h_ratio_%s",vars.at(i).c_str()));
    h_ratio->Divide(h);
    h_ratio->SetMarkerColor(colors[i]);
    h_ratio->SetLineColor(colors[i]);
    h_ratio->SetStats(0);
    h_ratio->SetMarkerStyle(20);
    h_ratio->SetMarkerSize(1.2);
    h_ratio->Draw("hist same");
  }

  gPad->RedrawAxis();

  c2->cd();
  c2->SaveAs( Form("%s/%s.pdf", savedir.c_str(), region_name.c_str() ) );
  c2->SaveAs( Form("%s/%s.eps", savedir.c_str(), region_name.c_str() ) );
  c2->SaveAs( Form("%s/%s.png", savedir.c_str(), region_name.c_str() ) );
  delete h2_axes_ratio;
  delete c2;
  
}

//_______________________________________________________________________________
std::string getRegionName(TString dir) {

  if (dir == "srbaseJ0B") return "HT200toInf_j1_b0";
  else if (dir == "srbaseJ1B") return "HT200toInf_j1_b1toInf";

  //Get variable boundaries for signal region.
  //Used to create datacard name.
  TH1D* h_ht_LOW = (TH1D*) f1->Get(dir+"/h_ht_LOW");
  TH1D* h_ht_HI = (TH1D*) f1->Get(dir+"/h_ht_HI");
  int ht_LOW = h_ht_LOW->GetBinContent(1);
  int ht_HI = h_ht_HI->GetBinContent(1);
  
  TH1D* h_nbjets_LOW = (TH1D*) f1->Get(dir+"/h_nbjets_LOW");
  TH1D* h_nbjets_HI = (TH1D*) f1->Get(dir+"/h_nbjets_HI");
  int nbjets_LOW = h_nbjets_LOW->GetBinContent(1);
  int nbjets_HI = h_nbjets_HI->GetBinContent(1);

  TH1D* h_njets_LOW = (TH1D*) f1->Get(dir+"/h_njets_LOW");
  TH1D* h_njets_HI = (TH1D*) f1->Get(dir+"/h_njets_HI");
  int njets_LOW = h_njets_LOW->GetBinContent(1);
  int njets_HI = h_njets_HI->GetBinContent(1);

  int nbjets_HI_mod = nbjets_HI;
  int njets_HI_mod = njets_HI;
  if(nbjets_HI != -1) nbjets_HI_mod--;
  if(njets_HI != -1) njets_HI_mod--;

  std::string ht_str = "HT" + std::to_string(ht_LOW) + "to" + std::to_string(ht_HI);
  std::string jet_str = (njets_HI_mod == njets_LOW) ? "j" + std::to_string(njets_LOW) : "j" + std::to_string(njets_LOW) + "to" + std::to_string(njets_HI_mod);
  std::string bjet_str = (nbjets_HI_mod == nbjets_LOW) ? "b" + std::to_string(nbjets_LOW) : "b" + std::to_string(nbjets_LOW) + "to" + std::to_string(nbjets_HI_mod);
  
  //Replace instances of "-1" with "inf" for variables with no upper bound.
  ReplaceString(ht_str, "-1", "Inf");
  ReplaceString(jet_str, "-1", "Inf");
  ReplaceString(bjet_str, "-1", "Inf");

  std::string channel = ht_str + "_" + jet_str + "_" + bjet_str;

  return channel;

}

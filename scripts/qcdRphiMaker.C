#include <iostream>
#include <utility>
#include <vector>

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

//_______________________________________________________________________________
void ReplaceString(std::string& subject, const std::string& search, const std::string& replace) {
    size_t pos = 0;
    while((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
}

//_______________________________________________________________________________
void makeQCDFromCRs( TFile* f_data , TFile* f_qcd , TFile* f_lostlep , TFile* f_zinv , vector<string> dirs, string output_name ) {

  // Generate histogram file with qcd prediction based on low dphi CRs
  // 2 cases: multijet and monojet

  TFile * outfile = new TFile(output_name.c_str(),"RECREATE") ; 
  outfile->cd();
  const unsigned int ndirs = dirs.size();
  
  for ( unsigned int idir = 0; idir < ndirs; ++idir ) {
    TString directory = "sr"+dirs.at(idir);
    TString fullhistname = directory + "/h_mt2bins";
    TString n_mt2bins_name = directory + "/h_n_mt2bins";
    TString crdir = "crqcd"+TString(dirs.at(idir));
    TString fullhistnameCR = crdir+"/h_mt2bins";

    //std::cout << "making estimate for region: " << directory << ", from CR: " << crdir << std::endl;

    TH1D* h_qcd_cr_data = (TH1D*) f_data->Get(fullhistnameCR);
    
    TH1D* h_n_mt2bins = (TH1D*) f_data->Get(n_mt2bins_name);
    const int n_mt2bins = h_n_mt2bins->GetBinContent(1);
    float* mt2bins = new float[n_mt2bins+1];
    for(int i=0; i<=(n_mt2bins); i++){
      //std::cout << "bin edge = " << h_lostlep_sr->GetBinLowEdge(i+1) << std::endl;
      mt2bins[i] = h_qcd_cr_data->GetBinLowEdge(i+1);  
    }

    //std::cout << "got stuff from data" << std::endl;

    // retrieve relevant factors and uncertainties from ETH input file -- need to convert name
    TH1D* h_ht_LOW = (TH1D*) f_data->Get(directory+"/h_ht_LOW");
    TH1D* h_ht_HI = (TH1D*) f_data->Get(directory+"/h_ht_HI");
    int ht_LOW = h_ht_LOW->GetBinContent(1);
    int ht_HI = h_ht_HI->GetBinContent(1);
  
    TH1D* h_nbjets_LOW = (TH1D*) f_data->Get(directory+"/h_nbjets_LOW");
    TH1D* h_nbjets_HI = (TH1D*) f_data->Get(directory+"/h_nbjets_HI");
    int nbjets_LOW = h_nbjets_LOW->GetBinContent(1);
    int nbjets_HI = h_nbjets_HI->GetBinContent(1);

    TH1D* h_njets_LOW = (TH1D*) f_data->Get(directory+"/h_njets_LOW");
    TH1D* h_njets_HI = (TH1D*) f_data->Get(directory+"/h_njets_HI");
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
    std::string channel_njonly = std::string("HT200toInf_") + jet_str + "_b0toInf";
    // special case for j2to6_b3toInf: use j4to6
    if (nbjets_LOW == 3 && njets_LOW == 2) channel_njonly = "HT200toInf_j4to6_b0toInf";
    std::string channel_htonly = ht_str + "_j2toInf_b0toInf";

    //std::cout << "channel is: " << channel << std::endl;

    // Make directory and plot(s) in the output file
    TDirectory* dir = 0;
    dir = (TDirectory*)outfile->Get(directory.Data());
    if (dir == 0) {
      dir = outfile->mkdir(directory.Data());
    } 
    dir->cd();

    // special case for monojet
    if (njets_LOW == 1) {

      // get CR hists for data and for EWK backgrounds, to do subtraction
      TH1D* h_qcd_cr_lostlep = (TH1D*) f_lostlep->Get(fullhistnameCR);
      TH1D* h_qcd_cr_zinv = (TH1D*) f_zinv->Get(fullhistnameCR);

      // get subtracted plot -- first clone original
      TH1D* h_qcd_sr_pred = 0;
      if(h_qcd_cr_data) {
	h_qcd_sr_pred = (TH1D*) h_qcd_cr_data->Clone("h_mt2bins");
      } else {
	cout<<"couldn't find qcd data CR hist: "<<fullhistnameCR<<endl;
	// make empty histogram
	h_qcd_sr_pred = new TH1D("h_mt2bins", "h_mt2bins", n_mt2bins, mt2bins);
      }

      // do subtraction
      if (h_qcd_cr_lostlep) h_qcd_sr_pred->Add(h_qcd_cr_lostlep,-1);
      if (h_qcd_cr_zinv) h_qcd_sr_pred->Add(h_qcd_cr_zinv,-1);

      // check for negative bins, set them to 0. No transfer factor to apply for monojet
      for (int ibin=1; ibin <= h_qcd_sr_pred->GetNbinsX(); ++ibin) {
	if (h_qcd_sr_pred->GetBinContent(ibin) < 0.) h_qcd_sr_pred->SetBinContent(ibin,0.);
      }

      dir->cd();
      h_qcd_sr_pred->Write();
      
      //std::cout << "done for monojet bin " << std::endl;

    } // monojet
    
    // multijet regions
    else if (njets_LOW >= 2) {

      // clone original to become prediction plot
      TH1D* h_qcd_sr_pred = 0;
      if(h_qcd_cr_data) {
	h_qcd_sr_pred = (TH1D*) h_qcd_cr_data->Clone("h_mt2bins");
      } else {
	cout<<"couldn't find qcd data CR hist: "<<fullhistnameCR<<endl;
	// make empty histogram
	h_qcd_sr_pred = new TH1D("h_mt2bins", "h_mt2bins", n_mt2bins, mt2bins);
      }
      // for gamma function
      TH1D* CRstats = (TH1D*) h_qcd_sr_pred->Clone("h_mt2binsCRyield");
      TH1D* alphaHist = (TH1D*) h_qcd_sr_pred->Clone("h_mt2binsAlpha");

      //std::cout << "cloned data hist " << std::endl;
      
      // lookup transfer factors from ETH input file
      TH1D* h_reff = (TH1D*) f_qcd->Get(Form("r_effective/%s/yield_r_effective_%s",channel.c_str(),channel.c_str()));
      string f_jets_dir = "f_jets_data";
      if (ht_LOW == 200) f_jets_dir = "f_jets_data_noPS";
      TH1D* h_fjets = (TH1D*) f_qcd->Get(Form("%s/%s/yield_%s_%s",f_jets_dir.c_str(),channel_htonly.c_str(),f_jets_dir.c_str(),channel_htonly.c_str()));
      TH1D* h_rb = (TH1D*) f_qcd->Get(Form("r_hat_data/%s/yield_r_hat_data_%s",channel_njonly.c_str(),channel_njonly.c_str()));

      float fjets = h_fjets->GetBinContent( h_fjets->FindBin(njets_LOW) );
      float fjets_err = h_fjets->GetBinError( h_fjets->FindBin(njets_LOW) );
      float rb = h_rb->GetBinContent( h_rb->FindBin(nbjets_LOW) );
      float rb_err = h_rb->GetBinError( h_rb->FindBin(njets_LOW) );

      //std::cout << "got fjets and rb " << std::endl;
	
      // have to combine bins for j2to6, b3toInf
      if (nbjets_LOW == 3 && njets_LOW == 2) {
	//std::cout << "need to get extra bin for fjets " << std::endl;
	int otherFJetsBin  = h_fjets->FindBin(4);
	fjets += h_fjets->GetBinContent( otherFJetsBin );
	fjets_err *= fjets_err;
	fjets_err += h_fjets->GetBinError( otherFJetsBin )*h_fjets->GetBinError( otherFJetsBin );
	fjets_err  = sqrt(fjets_err);
      }

      // convert to relative error
      fjets_err = fjets > 0. ? fjets_err/fjets : 0.;
      rb_err = rb > 0. ? rb_err/rb : 0.;

      //std::cout << " fjets: " << fjets << " +/- " << fjets_err << std::endl;
      //std::cout << " rb: " << rb << " +/- " << rb_err << std::endl;
      
      //std::cout << "going to scale yields " << std::endl;
      
      for (int ibin=1; ibin <= h_qcd_sr_pred->GetNbinsX(); ++ibin) {
	//std::cout << " bin " << ibin << std::endl;
	float reff = h_reff->GetBinContent(ibin);
	float reff_err = h_reff->GetBinError(ibin);
	// convert to relative error
	reff_err = reff > 0. ? reff_err/reff : 0.;
	//std::cout << " reff: " << reff << " +/- " << reff_err << std::endl;
	float alpha = reff * fjets * rb;
	float alpha_err = sqrt(fjets_err*fjets_err + rb_err*rb_err + reff_err*reff_err); 
	//std::cout << " alpha: " << alpha << " +/- " << alpha_err << std::endl;

	float pred = h_qcd_sr_pred->GetBinContent(ibin);
	pred *= alpha;
	// convert back to absolute err
	float pred_err = pred * alpha_err;
	//std::cout << " pred: " << pred << " +/- " << pred_err << " (abs err)" << std::endl;

	h_qcd_sr_pred->SetBinContent(ibin,pred);
	h_qcd_sr_pred->SetBinError(ibin,pred_err);
	
	alphaHist->SetBinContent(ibin,alpha);
	alphaHist->SetBinError(ibin,alpha*alpha_err);
      } // loop on mt2 bins

      dir->cd();
      h_qcd_sr_pred->Write();
      CRstats->Write();
      alphaHist->Write();

      //std::cout << "done for multijet bin " << std::endl;

    } // multijets

    delete mt2bins;
    
  } // loop over signal regions

  outfile->Close();
  delete outfile;

  return;
}

//_______________________________________________________________________________
void qcdRphiMaker(string input_dir = "/home/users/jgran/temp/update/MT2Analysis/MT2looper/output/V00-00-12/", string dataname = "data_Run2015D", string qcdname = "qcdEstimateData") {

  string output_name = input_dir+"/qcdFromCRs.root";
  std::cout << "Writing to file: " << output_name << std::endl;

  // get input files -- default to faking data with same MC file
  TFile* f_data = new TFile(Form("%s/%s.root",input_dir.c_str(),dataname.c_str()));
  // inputs with rphi, fj, rb and uncertainties from ETH.  Assume this will live in scripts dir
  TFile* f_qcd = new TFile(Form("./%s.root",qcdname.c_str()));
  // other MC backgrounds needed for EWK subtraction in monojet prediction region
  TFile* f_lostlep = new TFile(Form("%s/lostlep.root",input_dir.c_str()));
  TFile* f_zinv = new TFile(Form("%s/zinv_ht.root",input_dir.c_str()));
  
  if(f_data->IsZombie() || f_qcd->IsZombie() || f_lostlep->IsZombie() || f_zinv->IsZombie()) {
    std::cerr << "Input file does not exist" << std::endl;
    return;
  }

  vector<string> dirs;

  //Loop through list of every directory in the signal file.
  //if directory begins with "sr", excluding "srbase", add it to vector signal regions.
  TIter it(f_data->GetListOfKeys());
  TKey* k;
  std::string keep = "sr";
  std::string skip = "srbase";
  while ((k = (TKey *)it())) {
    if (strncmp (k->GetTitle(), skip.c_str(), skip.length()) == 0) continue;
    if (strncmp (k->GetTitle(), keep.c_str(), keep.length()) == 0) {//it is a signal region
      std::string sr_string = k->GetTitle();
      sr_string.erase(0, 2);//remove "sr" from front of string
      dirs.push_back(sr_string);
    }
  }

  makeQCDFromCRs( f_data , f_qcd , f_lostlep , f_zinv , dirs, output_name );

}

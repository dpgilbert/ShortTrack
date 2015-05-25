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
void makeLostLepFromCRs( TFile* ftop , TFile* fwjets , vector<string> dirs, string output_name, int method = 0 ) {

  // Generate histogram file with lostlep prediction based on 1L CRs
  // Method 0.  Central value from SRs, Gaussian stat err from 1L CRs

  TFile * outfile = new TFile(output_name.c_str(),"RECREATE") ; 
  outfile->cd();
  const unsigned int ndirs = dirs.size();
  
  for ( unsigned int idir = 0; idir < ndirs; ++idir ) {
    TString directory = "sr"+dirs.at(idir);
    TString fullhistname = directory + "/h_mt2bins";
    TString n_mt2bins_name = directory + "/h_n_mt2bins";

    TString fullhistnameSL = "crsl"+TString(dirs.at(idir))+"/h_mt2bins";

    TH1D* htop_sr = (TH1D*) ftop->Get(fullhistname);
    TH1D* hwjets_sr = (TH1D*) fwjets->Get(fullhistname);
    
    // If either top or wjets SR histograms are not filled, just leave (shouldn't happen when running on full stat MC)
    if(!htop_sr || !hwjets_sr){
      cout<<"could not find histogram "<<fullhistname<<endl;
      //continue; //Actually, don't skip. Still need CR yields even if SR yields are 0.
    }

    //const int n_mt2bins = 5;
    //const float mt2bins[n_mt2bins+1] = {200., 300., 400., 600., 1000., 1500.};
    TH1D* h_n_mt2bins = (TH1D*) ftop->Get(n_mt2bins_name);
    const int n_mt2bins = h_n_mt2bins->GetBinContent(1);
    float* mt2bins = new float[n_mt2bins+1];
    for(int i=0; i<=(n_mt2bins); i++){
      //std::cout << "bin edge = " << htop_sr->GetBinLowEdge(i+1) << std::endl;
      mt2bins[i] = htop_sr->GetBinLowEdge(i+1);  
    }

    TH1D* hlostlep_sr = 0;
    if(htop_sr) {
      hlostlep_sr = (TH1D*) htop_sr->Clone("h_mt2binsSR");
      if(hwjets_sr) hlostlep_sr->Add(hwjets_sr);
    } else if(hwjets_sr) {
      hlostlep_sr = (TH1D*) hwjets_sr->Clone("h_mt2binsSR");
    } else {
      hlostlep_sr = new TH1D("h_mt2binsSR", "h_mt2binsSR", n_mt2bins, mt2bins);
    }

    TH1D* hlostlep_cr = 0;
    TH1D* htop_cr = (TH1D*) ftop->Get(fullhistnameSL);
    TH1D* hwjets_cr = (TH1D*) fwjets->Get(fullhistnameSL);
    // check that histograms exist
    if (!htop_cr) {
      cout << "couldn't find top CR hist: " << fullhistnameSL << endl;
    }
    if (!hwjets_cr) {
      cout << "couldn't find wjets CR hist: " << fullhistnameSL << endl;
    }
    if (!hlostlep_cr && htop_cr) {
      hlostlep_cr = (TH1D*) htop_cr->Clone("h_mt2binsCRyield");
    } else if (htop_cr) {
      hlostlep_cr->Add(htop_cr);
    }
    if (!hlostlep_cr && hwjets_cr) {
      hlostlep_cr = (TH1D*) hwjets_cr->Clone("h_mt2binsCRyield");
    } else if (hwjets_cr) {
      hlostlep_cr->Add(hwjets_cr);
    }

    //std::cout << "n_mt2bins = " << n_mt2bins << std::endl;
    //std::cout << "Binning =" << std::endl;
    //for(int i=0; i<=(n_mt2bins); i++){
      //std::cout << mt2bins[i] << std::endl;
    //}
    //std::cout << "Binning has been shown" << std::endl;

    //std::cout << "It should mess up just below this" << std::endl;
    if(!hlostlep_cr) hlostlep_cr = new TH1D("h_mt2binsCRyield", "h_mt2binsCRyield", n_mt2bins, mt2bins);
    //std::cout << "Did it mess up?" << std::endl;
    delete mt2bins;

    if (hlostlep_sr->GetNbinsX() != hlostlep_cr->GetNbinsX() ) {
      cout<<"different binning for histograms "<<fullhistname<<endl;
      continue;
    }
    
    // Make directory and plot(s) in the output file
    TDirectory* dir = 0;
    dir = (TDirectory*)outfile->Get(directory.Data());
    if (dir == 0) {
      dir = outfile->mkdir(directory.Data());
    } 
    dir->cd();

    TH1D* Stat = (TH1D*) hlostlep_sr->Clone("h_mt2binsStat");
    cout<<"Looking at histo "<<fullhistname<<endl;
    if (method==0) { // --- Simple: lostlep_sr +/- lostlep_sr/sqrt(lostlep_cr)
      // use full CR stats integrated over MT2
      double n_cr = hlostlep_cr->Integral(0,-1);
      for ( int ibin = 0; ibin <= Stat->GetNbinsX(); ++ibin) { // "<=" to deal with overflow bin
	if (n_cr > 0)
	  Stat->SetBinError(ibin, hlostlep_sr->GetBinContent(ibin)/sqrt(n_cr));
	else Stat->SetBinError(ibin, hlostlep_sr->GetBinContent(ibin));
      }
    }

    TH1D* Syst = (TH1D*) Stat->Clone("h_mt2binsSyst");
    TH1D* pred = (TH1D*) Stat->Clone("h_mt2bins");
    for ( int ibin = 0; ibin <= Stat->GetNbinsX(); ++ibin) { 
      Syst->SetBinError(ibin, 0.);
      double quadrature = Stat->GetBinError(ibin)*Stat->GetBinError(ibin) + Syst->GetBinError(ibin)*Syst->GetBinError(ibin);
      pred->SetBinError(ibin, sqrt(quadrature));
    }
    //pred->Print("all");

    TH1D* MCStat = (TH1D*) hlostlep_sr->Clone("h_mt2binsMCStat");

    pred->Write();
    Stat->Write();
    Syst->Write();
    hlostlep_cr->Write();
    MCStat->Write();

  } // loop over signal regions


  return;
}

//_______________________________________________________________________________
void lostlepMaker(string input_dir = "/home/users/jgran/temp/update/MT2Analysis/MT2looper/output/V00-00-12/"){

  //string input_dir = "/home/users/olivito/MT2Analysis/MT2looper/output/V00-00-08_fullstats/";
  //  string input_dir = "/home/users/jgran/temp/update/MT2Analysis/MT2looper/output/test/";
  //string input_dir = "../MT2looper/output/2015LowLumi/";
  //  string input_dir = "../MT2looper/output/test/";

  string output_name = input_dir+"lostlepFromCRs.root";
  // ----------------------------------------
  //  samples definition
  // ----------------------------------------

  std::cout << "Writing to file: " << output_name << std::endl;

  // get input files
  TFile* f_top = new TFile(Form("%s/top.root",input_dir.c_str()));
  TFile* f_wjets = new TFile(Form("%s/wjets_ht.root",input_dir.c_str()));
  //TFile* f_qcd = new TFile(Form("%s/qcd_pt.root",input_dir.c_str()));
  
  if(f_top->IsZombie() || f_wjets->IsZombie()) {
    std::cerr << "Input file does not exist" << std::endl;
    return;
  }

  vector<string> dirs;

  //Loop through list of every directory in the signal file.
  //if directory begins with "sr", excluding "srbase", add it to vector signal regions.
  TIter it(f_top->GetListOfKeys());
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

  makeLostLepFromCRs( f_top , f_wjets , dirs, output_name, 0 );

}

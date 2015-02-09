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

using namespace std;


//_______________________________________________________________________________
void makeLostLepFromCRs( TFile* fttbar , TFile* fwjets , vector<string> dirs, string output_name, int method = 0 ) {

  // Generate histogram file with lostlep prediction based on 1L CRs
  // Method 0.  Central value from SRs, Gaussian stat err from 1L CRs

  TFile * outfile = new TFile(output_name.c_str(),"RECREATE") ; 
  outfile->cd();
  const unsigned int ndirs = dirs.size();
  
  for ( unsigned int idir = 0; idir < ndirs; ++idir ) {
    TString directory = "sr"+dirs.at(idir);
    TString fullhistname = directory + "/h_mt2bins";

    vector<TString> directoriesSL_temp = getCRsToCombine(dirs.at(idir));
    vector<TString> directoriesSL;
    vector<TString> fullhistnamesSL;
    for (unsigned int icr = 0; icr < directoriesSL_temp.size(); ++icr) {
      directoriesSL.push_back("crsl"+directoriesSL_temp.at(icr));
      fullhistnamesSL.push_back(directoriesSL.at(icr)+"/h_mt2bins");
    }

    TH1D* httbar_sr = (TH1D*) fttbar->Get(fullhistname);
    TH1D* hwjets_sr = (TH1D*) fwjets->Get(fullhistname);
    
    // If both ttbar and wjets SR histograms are not filled, just leave (shouldn't happen when running on full stat MC)
    if(!httbar_sr && !hwjets_sr){
      cout<<"could not find histogram "<<fullhistname<<endl;
      continue;
    }


    TH1D* hlostlep_sr = (TH1D*) httbar_sr->Clone("h_mt2binsSR");
    hlostlep_sr->Add(hwjets_sr);

    TH1D* hlostlep_cr = 0;
    for (unsigned int icr = 0; icr < fullhistnamesSL.size(); ++icr) {
      TH1D* httbar_cr = (TH1D*) fttbar->Get(fullhistnamesSL.at(icr));
      TH1D* hwjets_cr = (TH1D*) fwjets->Get(fullhistnamesSL.at(icr));
      // check that histograms exist
      if (!httbar_cr || !hwjets_cr) {
	cout << "couldn't find CR hist: " << fullhistnamesSL.at(icr) << endl;
      }
      if (icr == 0) {
	hlostlep_cr = (TH1D*) httbar_cr->Clone("h_mt2binsCR");
      } else {
	hlostlep_cr->Add(httbar_cr);
      }
      hlostlep_cr->Add(hwjets_cr);
    }

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
      for ( unsigned int ibin = 0; ibin <= Stat->GetNbinsX(); ++ibin) { // "<=" to deal with overflow bin
	if (n_cr > 0)
	  Stat->SetBinError(ibin, hlostlep_sr->GetBinContent(ibin)/sqrt(n_cr));
	else Stat->SetBinError(ibin, hlostlep_sr->GetBinContent(ibin));
      }
    }

    TH1D* Syst = (TH1D*) Stat->Clone("h_mt2binsSyst");
    TH1D* pred = (TH1D*) Stat->Clone("h_mt2bins");
    for ( unsigned int ibin = 0; ibin <= Stat->GetNbinsX(); ++ibin) { 
      Syst->SetBinError(ibin, 0.);
      double quadrature = Stat->GetBinError(ibin)*Stat->GetBinError(ibin) + Syst->GetBinError(ibin)*Syst->GetBinError(ibin);
      pred->SetBinError(ibin, sqrt(quadrature));
    }
    //pred->Print("all");

    pred->Write();
    Stat->Write();
    Syst->Write();


  } // loop over signal regions


  return;
}

//_______________________________________________________________________________
vector<TString> getCRsToCombine(const string& dir) {

  vector<TString> crs;

  // combine loMT and hiMT bins for regions split by that
  if ((dir.find("3L") != string::npos) || (dir.find("4L") != string::npos)) {
    crs.push_back("3L");
    crs.push_back("4L");
  }
  else if ((dir.find("3M") != string::npos) || (dir.find("4M") != string::npos)) {
    crs.push_back("3M");
    crs.push_back("4M");
  }
  else if ((dir.find("3H") != string::npos) || (dir.find("4H") != string::npos)) {
    crs.push_back("3H");
    crs.push_back("4H");
  }

  // combine loMT and hiMT bins for regions split by that
  else if ((dir.find("7L") != string::npos) || (dir.find("8L") != string::npos)) {
    crs.push_back("7L");
    crs.push_back("8L");
  }
  else if ((dir.find("7M") != string::npos) || (dir.find("8M") != string::npos)) {
    crs.push_back("7M");
    crs.push_back("8M");
  }
  else if ((dir.find("7H") != string::npos) || (dir.find("8H") != string::npos)) {
    crs.push_back("7H");
    crs.push_back("8H");
  }

  // combine loMT and hiMT bins for regions split by that
  else if ((dir.find("9L") != string::npos) || (dir.find("10L") != string::npos)) {
    crs.push_back("9L");
    crs.push_back("10L");
  }
  else if ((dir.find("9M") != string::npos) || (dir.find("10M") != string::npos)) {
    crs.push_back("9M");
    crs.push_back("10M");
  }
  else if ((dir.find("9H") != string::npos) || (dir.find("10H") != string::npos)) {
    crs.push_back("9H");
    crs.push_back("10H");
  }

  // default: use only this TR
  else {
    crs.push_back(TString(dir));
  }

  return crs;
}


//_______________________________________________________________________________
void lostlepMaker(){

  string input_dir = "../MT2looper/output/2015ExtendedNJets/";
  //string input_dir = "../MT2looper/output/2015LowLumi/";
  //  string input_dir = "../MT2looper/output/test/";
  string output_name = input_dir+"lostlepFromCRs.root";
  // ----------------------------------------
  //  samples definition
  // ----------------------------------------

  // get input files
  TFile* f_ttbar = new TFile(Form("%s/ttall_msdecays.root",input_dir.c_str()));
  TFile* f_wjets = new TFile(Form("%s/wjets_ht.root",input_dir.c_str()));
  //TFile* f_qcd = new TFile(Form("%s/qcd_pt.root",input_dir.c_str()));

  vector<string> dirs;

  //Loop through list of every directory in the signal file.
  //if directory begins with "sr", excluding "srbase", add it to vector signal regions.
  TIter it(f_ttbar->GetListOfKeys());
  TKey* k;
  std::string keep = "sr";
  std::string skip = "srbase";
  while (k = (TKey *)it()) {
    if (k->GetTitle() == "srbase") continue;
    if (strncmp (k->GetTitle(), skip.c_str(), skip.length()) == 0) continue;
    if (strncmp (k->GetTitle(), keep.c_str(), keep.length()) == 0) {//it is a signal region
      std::string sr_string = k->GetTitle();
      sr_string.erase(0, 2);//remove "sr" from front of string
      dirs.push_back(sr_string);
    }
  }

  makeLostLepFromCRs( f_ttbar , f_wjets , dirs, output_name, 0 );

}

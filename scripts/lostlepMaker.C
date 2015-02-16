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

    vector<TString> directoriesSL_temp = getCRsToCombine(fttbar, TString(dirs.at(idir)));
    vector<TString> directoriesSL;
    vector<TString> fullhistnamesSL;
    for (unsigned int icr = 0; icr < directoriesSL_temp.size(); ++icr) {
      directoriesSL.push_back("crsl"+directoriesSL_temp.at(icr));
      fullhistnamesSL.push_back(directoriesSL.at(icr)+"/h_mt2bins");
    }

    TH1D* httbar_sr = (TH1D*) fttbar->Get(fullhistname);
    TH1D* hwjets_sr = (TH1D*) fwjets->Get(fullhistname);
    
    // If either ttbar or wjets SR histograms are not filled, just leave (shouldn't happen when running on full stat MC)
    if(!httbar_sr || !hwjets_sr){
      cout<<"could not find histogram "<<fullhistname<<endl;
      //continue; //Actually, don't skip. Still need CR yields even if SR yields are 0.
    }

    const int n_mt2bins = 5;
    const float mt2bins[n_mt2bins+1] = {200., 300., 400., 600., 1000., 1500.};

    TH1D* hlostlep_sr = 0;
    if(httbar_sr) {
      hlostlep_sr = (TH1D*) httbar_sr->Clone("h_mt2binsSR");
      if(hwjets_sr) hlostlep_sr->Add(hwjets_sr);
    } else if(hwjets_sr) {
      hlostlep_sr = (TH1D*) hwjets_sr->Clone("h_mt2binsSR");
    } else {
      hlostlep_sr = new TH1D("h_mt2binsSR", "h_mt2binsSR", n_mt2bins, mt2bins);
    }

    TH1D* hlostlep_cr = 0;
    for (unsigned int icr = 0; icr < fullhistnamesSL.size(); ++icr) {
      TH1D* httbar_cr = (TH1D*) fttbar->Get(fullhistnamesSL.at(icr));
      TH1D* hwjets_cr = (TH1D*) fwjets->Get(fullhistnamesSL.at(icr));
      // check that histograms exist
      if (!httbar_cr) {
        cout << "couldn't find ttbar CR hist: " << fullhistnamesSL.at(icr) << endl;
      }
      if (!hwjets_cr) {
        cout << "couldn't find wjets CR hist: " << fullhistnamesSL.at(icr) << endl;
      }
      if (!hlostlep_cr && httbar_cr) {
        hlostlep_cr = (TH1D*) httbar_cr->Clone("h_mt2binsCRyield");
      } else if (httbar_cr) {
        hlostlep_cr->Add(httbar_cr);
      }
      if (!hlostlep_cr && hwjets_cr) {
        hlostlep_cr = (TH1D*) hwjets_cr->Clone("h_mt2binsCRyield");
      } else if (hwjets_cr) {
        hlostlep_cr->Add(hwjets_cr);
      }
    }

    if(!hlostlep_cr) hlostlep_cr = new TH1D("h_mt2binsCRyield", "h_mt2binsCRyield", n_mt2bins, mt2bins);

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
vector<TString> getCRsToCombine(TFile* f, const TString& dir) {

  vector<TString> crs;  

  //get minMTBMet boundaries
  TH1D* h_lowMT_LOW = (TH1D*) f->Get("sr"+dir+"/h_lowMT_LOW");
  TH1D* h_lowMT_UP = (TH1D*) f->Get("sr"+dir+"/h_lowMT_UP");
  int lowMT_LOW;
  int lowMT_UP;

  if(h_lowMT_LOW) lowMT_LOW = h_lowMT_LOW->GetBinContent(1);
  else std::cout << "Bad Pointer" << std::endl;

  if(h_lowMT_UP)  lowMT_UP = h_lowMT_UP->GetBinContent(1);
  else std::cout << "Bad Pointer" << std::endl;

  if(lowMT_LOW == 0 && lowMT_UP == -1){//this SR is not split by minMTBMet
    crs.push_back(dir);
    return crs;
  }

  char* str = dir; 
  std::string first;//this piece is just "sr"
  std::string second;//this piece is the SR number
  std::string third;//this piece is "L", "M", or "H"
  bool found_num = false;
  
  //find the SR number so we can get the SR before and after this one
  while (*str){
    if(!isdigit(*str) && !found_num) first += *str;
    else if(!isdigit(*str)) third += *str;
    else {
      second += *str;
      found_num = true;
    }
    str++;
  }

  std::string srA = second;
  int sr_before = atoi(second.c_str());
  int sr_after  = atoi(second.c_str());

  sr_before--;
  sr_after++;

  //assume that lowMT region comes before highMT region
  stringstream ss;
  if(lowMT_LOW == 1) ss << sr_after;
  else ss << sr_before;

  std::string srB = ss.str();

  crs.push_back(TString(srA + third));
  crs.push_back(TString(srB + third));

  return crs;

}


//_______________________________________________________________________________
void lostlepMaker(){

  string input_dir = "/home/users/olivito/MT2Analysis/MT2looper/output/V00-00-08_fullstats/";
  //string input_dir = "../MT2looper/output/2015LowLumi/";
  //  string input_dir = "../MT2looper/output/test/";
  string output_name = input_dir+"lostlepFromCRs.root";
  // ----------------------------------------
  //  samples definition
  // ----------------------------------------

  std::cout << "Writing to file: " << output_name << std::endl;

  // get input files
  TFile* f_ttbar = new TFile(Form("%s/ttall_msdecays.root",input_dir.c_str()));
  TFile* f_wjets = new TFile(Form("%s/wjets_ht.root",input_dir.c_str()));
  //TFile* f_qcd = new TFile(Form("%s/qcd_pt.root",input_dir.c_str()));
  
  if(f_ttbar->IsZombie() || f_wjets->IsZombie()) {
    std::cout << "Input file does not exist" << std::endl;
    return;
  }

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

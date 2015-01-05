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
void makeZinvFromGJets( TFile* fZinv , TFile* fGJet /*, TFile* QCD */, vector<string> dirs, vector<string> dirsGJ, string output_name, int method = 0 ) {

  // Generate histogram file with Zinv prediction based on GJetsData * R(Zinv/GJ)
  // Method 0.  Just Poisson from GJet: Zinv +/- 1/sqrt(GJet)
  // Method 1.  also MC stats on ratio: ( GJet +/- 1/sqrt(GJet) ) * ( R(Zinv/GJet) +/- MCstat ) 

  TFile * outfile = new TFile(output_name.c_str(),"RECREATE") ; 
  outfile->cd();
  const unsigned int ndirs = dirs.size();
  
  for ( unsigned int idir = 0; idir < ndirs; ++idir ) {
    TString fullhistname = Form("%s/h_mt2bins",dirs.at(idir).c_str());
    TString fullhistnameGJ = Form("%s/h_mt2bins",dirsGJ.at(idir).c_str());
    // If Zinv or GJet histograms are not filled, just leave (shouldn't happen when running on full stat MC)
    if(fGJet->GetDirectory(dirsGJ.at(idir).c_str())->GetKey("h_mt2bins")==0 ||
       fZinv->GetDirectory(dirs.at(idir).c_str())->GetKey("h_mt2bins")==0 ) {
      cout<<"could not find histogram "<<fullhistname<<endl;
      continue;
    }

    TH1D* hGJet = (TH1D*) fGJet->Get(fullhistnameGJ);    
    TH1D* hZinv = (TH1D*) fZinv->Get(fullhistname);    
    
    if (hGJet->GetNbinsX() != hZinv->GetNbinsX() ) {
      cout<<"different binning for histograms "<<fullhistname<<endl;
      continue;
    }
    
    // Make directory and plot(s) in the output file
    TDirectory* dir = 0;
    dir = (TDirectory*)outfile->Get(dirs.at(idir).c_str());
    if (dir == 0) {
      dir = outfile->mkdir(dirs.at(idir).c_str());
    } 
    dir->cd();

    TH1D* Stat = hZinv->Clone("h_mt2binsStat");
    cout<<"Looking at histo "<<fullhistname<<endl;
    if (method==0) { // --- Simple: Zinv +/- Zinv/sqrt(GJet)
      for ( unsigned int ibin = 0; ibin <= Stat->GetNbinsX(); ++ibin) { // "<=" to deal with overflow bin
	if (hGJet->GetBinContent(ibin) > 0)
	  Stat->SetBinError(ibin, hZinv->GetBinContent(ibin)/sqrt( hGJet->GetBinContent(ibin) ));
	else Stat->SetBinError(ibin, hZinv->GetBinContent(ibin));
      }
    }

    if (method==1) { // --- More advanced: ( GJet +/- sqrt(GJet) ) * ( R(Zinv/GJet) +/- MCstat ) 
      // Poisson uncertainty on GJet, then MC statistical uncertainty on R 
      TH1D* ratio = hZinv->Clone("ratio");
      ratio->Divide(hGJet);
      Stat = (TH1D*) hGJet->Clone("h_mt2binsStat");
      for ( unsigned int ibin = 0; ibin <= Stat->GetNbinsX(); ++ibin) { // Set Poisson errors
	Stat->SetBinError(ibin, sqrt( hGJet->GetBinContent(ibin) ));
      }
      Stat->Multiply(ratio);
    }

    TH1D* Syst = Stat->Clone("h_mt2binsSyst");
    TH1D* pred = Stat->Clone("h_mt2bins");
    for ( unsigned int ibin = 0; ibin <= Stat->GetNbinsX(); ++ibin) { 
      Syst->SetBinError(ibin, 0.);
      double quadrature = Stat->GetBinError(ibin)*Stat->GetBinError(ibin) + Syst->GetBinError(ibin)*Syst->GetBinError(ibin);
      pred->SetBinError(ibin, sqrt(quadrature));
    }
    pred->Print("all");

    pred->Write();
    Stat->Write();
    Syst->Write();


  } // loop over signal regions


  return;
}


//_______________________________________________________________________________
void ZinvMaker(){

  //  string input_dir = "/home/olivito/cms3/MT2Analysis/MT2looper/output/V00-00-07_sel2015LowLumi/";
  string input_dir = "../MT2looper/output/test/";
  string output_name = input_dir+"zinvFromGJ.root";
  // ----------------------------------------
  //  samples definition
  // ----------------------------------------

  // get input files
  TFile* f_zinv = new TFile(Form("%s/zinv_ht.root",input_dir.c_str()));
  TFile* f_gjet = new TFile(Form("%s/gjet_ht.root",input_dir.c_str()));

  //TFile* f_zinv = new TFile(Form("%s/zinv_ht.root",input_dir.c_str()));
  //TFile* f_gjet = new TFile(Form("%s/gjets_ht.root",input_dir.c_str()));
  //  TFile* f_qcd = new TFile(Form("%s/qcd_pt.root",input_dir.c_str()));



  vector<string> dirs;
  dirs.push_back("sr1L");
  dirs.push_back("sr2L");
  dirs.push_back("sr3L");
  dirs.push_back("sr4L");
  dirs.push_back("sr5L");
  dirs.push_back("sr6L");
  dirs.push_back("sr7L");
  dirs.push_back("sr8L");
  dirs.push_back("sr9L");
  dirs.push_back("sr10L");
  dirs.push_back("sr1M");
  dirs.push_back("sr2M");
  dirs.push_back("sr3M");
  dirs.push_back("sr4M");
  dirs.push_back("sr5M");
  dirs.push_back("sr6M");
  dirs.push_back("sr7M");
  dirs.push_back("sr8M");
  dirs.push_back("sr9M");
  dirs.push_back("sr10M");
  dirs.push_back("sr1H");
  dirs.push_back("sr2H");
  dirs.push_back("sr3H");
  dirs.push_back("sr4H");
  dirs.push_back("sr5H");
  dirs.push_back("sr6H");
  dirs.push_back("sr7H");
  dirs.push_back("sr8H");
  dirs.push_back("sr9H");
  dirs.push_back("sr10H");

  vector<string> dirsGJ;
  dirsGJ.push_back("crgj1L");
  dirsGJ.push_back("crgj2L");
  dirsGJ.push_back("crgj3L");
  dirsGJ.push_back("crgj4L");
  dirsGJ.push_back("crgj5L");
  dirsGJ.push_back("crgj6L");
  dirsGJ.push_back("crgj7L");
  dirsGJ.push_back("crgj8L");
  dirsGJ.push_back("crgj9L");
  dirsGJ.push_back("crgj10L");
  dirsGJ.push_back("crgj1M");
  dirsGJ.push_back("crgj2M");
  dirsGJ.push_back("crgj3M");
  dirsGJ.push_back("crgj4M");
  dirsGJ.push_back("crgj5M");
  dirsGJ.push_back("crgj6M");
  dirsGJ.push_back("crgj7M");
  dirsGJ.push_back("crgj8M");
  dirsGJ.push_back("crgj9M");
  dirsGJ.push_back("crgj10M");
  dirsGJ.push_back("crgj1H");
  dirsGJ.push_back("crgj2H");
  dirsGJ.push_back("crgj3H");
  dirsGJ.push_back("crgj4H");
  dirsGJ.push_back("crgj5H");
  dirsGJ.push_back("crgj6H");
  dirsGJ.push_back("crgj7H");
  dirsGJ.push_back("crgj8H");
  dirsGJ.push_back("crgj9H");
  dirsGJ.push_back("crgj10H");

  makeZinvFromGJets( f_zinv , f_gjet /*, TFile* f_qcd */, dirs, dirsGJ, output_name, 0 );

}

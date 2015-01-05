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
void makeZinvFromGJets( TFile* fZinv , TFile* fGJet , TFile* fQCD, vector<string> dirs, string output_name, int method = 0 ) {

  // Generate histogram file with Zinv prediction based on GJetsData * R(Zinv/GJ)
  // Method 0.  Just Poisson from GJet: Zinv +/- Zinv/sqrt(GJet)
  // Method 1.  also MC stats on ratio: ( GJet +/- sqrt(GJet) ) * ( R(Zinv/GJet) +/- MCstat ) 
  // Method 2.  Same as method 0, add +/- QCD (100% systematic uncertainty based on QCD)

  TFile * outfile = new TFile(output_name.c_str(),"RECREATE") ; 
  outfile->cd();
  const unsigned int ndirs = dirs.size();
  
  for ( unsigned int idir = 0; idir < ndirs; ++idir ) {
    TString directory = "sr"+dirs.at(idir);
    TString directoryGJ = "crgj"+dirs.at(idir);

    TString fullhistname = directory + "/h_mt2bins";
    TString fullhistnameGJ = directoryGJ + "/h_mt2bins";
    // If Zinv or GJet histograms are not filled, just leave (shouldn't happen when running on full stat MC)
    if(fGJet->GetDirectory(directoryGJ.Data())->GetKey("h_mt2bins")==0 ||
       fZinv->GetDirectory(directory.Data())->GetKey("h_mt2bins")==0 ) {
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
    dir = (TDirectory*)outfile->Get(directory.Data());
    if (dir == 0) {
      dir = outfile->mkdir(directory.Data());
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
    //pred->Print("all");

    pred->Write();
    Stat->Write();
    Syst->Write();


  } // loop over signal regions


  return;
}


//_______________________________________________________________________________
void makeZinvFromDY( TFile* fZinv , TFile* fDY ,vector<string> dirs, string output_name, int method = 0 ) {

  // Generate histogram file with Zinv prediction based on DYData * R(Zinv/DY)
  // Method 0.  Just Poisson from DY: Zinv +/- Zinv/sqrt(DY)
  // Method 1.  also MC stats on ratio: ( DY +/- sqrt(DY) ) * ( R(Zinv/DY) +/- MCstat ) 

  TFile * outfile = new TFile(output_name.c_str(),"RECREATE") ; 
  outfile->cd();
  const unsigned int ndirs = dirs.size();
  
  for ( unsigned int idir = 0; idir < ndirs; ++idir ) {
    TString directory = "sr"+dirs.at(idir);
    TString directoryDY = "crdy"+dirs.at(idir);

    TString fullhistname = directory + "/h_mt2bins";
    TString fullhistnameDY = directoryDY + "/h_mt2bins";
    // If Zinv or DY histograms are not filled, just leave (shouldn't happen when running on full stat MC)
    if(fDY->GetDirectory(directoryDY.Data())->GetKey("h_mt2bins")==0 ||
       fZinv->GetDirectory(directory.Data())->GetKey("h_mt2bins")==0 ) {
      cout<<"could not find histogram "<<fullhistname<<endl;
      continue;
    }

    TH1D* hDY = (TH1D*) fDY->Get(fullhistnameDY);    
    TH1D* hZinv = (TH1D*) fZinv->Get(fullhistname);    
    
    if (hDY->GetNbinsX() != hZinv->GetNbinsX() ) {
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

    TH1D* Stat = hZinv->Clone("h_mt2binsStat");
    cout<<"Looking at histo "<<fullhistname<<endl;
    if (method==0) { // --- Simple: Zinv +/- Zinv/sqrt(DY)
      for ( unsigned int ibin = 0; ibin <= Stat->GetNbinsX(); ++ibin) { // "<=" to deal with overflow bin
	if (hDY->GetBinContent(ibin) > 0)
	  Stat->SetBinError(ibin, hZinv->GetBinContent(ibin)/sqrt( hDY->GetBinContent(ibin) ));
	else Stat->SetBinError(ibin, hZinv->GetBinContent(ibin));
      }
    }

    if (method==1) { // --- More advanced: ( DY +/- sqrt(DY) ) * ( R(Zinv/GJet) +/- MCstat ) 
      // Poisson uncertainty on DY, MC statistical uncertainty on R 
      TH1D* ratio = hZinv->Clone("ratio");
      ratio->Divide(hDY);
      Stat = (TH1D*) hDY->Clone("h_mt2binsStat");
      for ( unsigned int ibin = 0; ibin <= Stat->GetNbinsX(); ++ibin) { // Set Poisson errors
	Stat->SetBinError(ibin, sqrt( hDY->GetBinContent(ibin) ));
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
    //pred->Print("all");

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
  //TFile* f_qcd = new TFile(Form("%s/qcd_pt.root",input_dir.c_str()));

  vector<string> dirs;
  dirs.push_back("1L");
  dirs.push_back("2L");
  dirs.push_back("3L");
  dirs.push_back("4L");
  dirs.push_back("5L");
  dirs.push_back("6L");
  dirs.push_back("7L");
  dirs.push_back("8L");
  dirs.push_back("9L");
  dirs.push_back("10L");
  dirs.push_back("1M");
  dirs.push_back("2M");
  dirs.push_back("3M");
  dirs.push_back("4M");
  dirs.push_back("5M");
  dirs.push_back("6M");
  dirs.push_back("7M");
  dirs.push_back("8M");
  dirs.push_back("9M");
  dirs.push_back("10M");
  dirs.push_back("1H");
  dirs.push_back("2H");
  dirs.push_back("3H");
  dirs.push_back("4H");
  dirs.push_back("5H");
  dirs.push_back("6H");
  dirs.push_back("7H");
  dirs.push_back("8H");
  dirs.push_back("9H");
  dirs.push_back("10H");


  //  makeZinvFromGJets( f_zinv , f_gjet , f_qcd, dirs, dirsGJ, output_name, 0 );
  makeZinvFromGJets( f_zinv , f_gjet , f_zinv, dirs, output_name, 0 ); // not using QCD for now

  TFile* f_dy = new TFile(Form("%s/dyjetsll_ht.root",input_dir.c_str()));
   output_name = input_dir+"zinvFromDY.root";

  makeZinvFromDY( f_zinv , f_dy , dirs, output_name, 0 ); 


}

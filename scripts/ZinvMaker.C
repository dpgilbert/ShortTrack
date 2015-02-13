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
  // Method 2.  Same as method 0, add +/- QCD (100% systematic uncertainty based on QCD)

  TFile * outfile = new TFile(output_name.c_str(),"RECREATE") ; 
  outfile->cd();
  const unsigned int ndirs = dirs.size();
  
  for ( unsigned int idir = 0; idir < ndirs; ++idir ) {
    TString directory = "sr"+dirs.at(idir);
    TString directoryGJ = "crgj"+dirs.at(idir);

    TString fullhistname = directory + "/h_mt2bins";
    TString fullhistnameGJ = directoryGJ + "/h_mt2bins";

    TH1D* hGJet = (TH1D*) fGJet->Get(fullhistnameGJ);    
    TH1D* hZinv = (TH1D*) fZinv->Get(fullhistname);    

    // If Zinv or GJet histograms are not filled, just leave (shouldn't happen when running on full stat MC)
    if(!hGJet || !hZinv){
      cout<<"could not find histogram "<<fullhistname<<endl;
      continue;
    }
    
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

    // Zgamma ratio in each MT2bin -> to get MC stat error on ratio
    TH1D* ratio = hZinv->Clone("ratio");
    ratio->Divide(hGJet);

    // MCStat: use relative bin error from ratio hist, normalized to Zinv MC prediction
    TH1D* MCStat = (TH1D*) hZinv->Clone("h_mt2binsMCStat");
    for ( unsigned int ibin = 0; ibin <= Stat->GetNbinsX(); ++ibin) { 
      MCStat->SetBinError(ibin, MCStat->GetBinContent(ibin) * ratio->GetBinError(ibin) / ratio->GetBinContent(ibin) );
    }

    TH1D* Syst = Stat->Clone("h_mt2binsSyst");
    TH1D* pred = Stat->Clone("h_mt2bins");
    for ( unsigned int ibin = 0; ibin <= Stat->GetNbinsX(); ++ibin) { 
      Syst->SetBinError(ibin, 0.);
      double quadrature = Stat->GetBinError(ibin)*Stat->GetBinError(ibin) + Syst->GetBinError(ibin)*Syst->GetBinError(ibin);
      pred->SetBinError(ibin, sqrt(quadrature));
    }
    //pred->Print("all");

    TH1D* CRyield = hGJet->Clone("h_mt2binsCRyield");

    pred->Write();
    Stat->Write();
    Syst->Write();
    CRyield->Write();
    MCStat->Write();

  } // loop over signal regions


  return;
}


//_______________________________________________________________________________
void makeZinvFromDY( TFile* fZinv , TFile* fDY ,vector<string> dirs, string output_name, int method = 0 ) {

  // Generate histogram file with Zinv prediction based on DYData * R(Zinv/DY)
  // Method 0.  Just Poisson from DY: Zinv +/- Zinv/sqrt(DY)

  TFile * outfile = new TFile(output_name.c_str(),"RECREATE") ; 
  outfile->cd();
  const unsigned int ndirs = dirs.size();
  
  for ( unsigned int idir = 0; idir < ndirs; ++idir ) {
    TString directory = "sr"+dirs.at(idir);
    TString directoryDY = "crdy"+dirs.at(idir);

    TString fullhistname = directory + "/h_mt2bins";
    TString fullhistnameDY = directoryDY + "/h_mt2bins";

    TH1D* hDY = (TH1D*) fDY->Get(fullhistnameDY);    
    TH1D* hZinv = (TH1D*) fZinv->Get(fullhistname);    
    
    // If Zinv or DY histograms are not filled, just leave (shouldn't happen when running on full stat MC)
    if(!hDY || !hZinv){
      cout<<"could not find histogram "<<fullhistname<<endl;
      continue;
    }
    
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

    TH1D* ratio = hZinv->Clone("ratio");
    ratio->Divide(hDY);

    // MCStat: use relative bin error from ratio hist, normalized to Zinv MC prediction
    TH1D* MCStat = (TH1D*) hZinv->Clone("h_mt2binsMCStat");
    for ( unsigned int ibin = 0; ibin <= Stat->GetNbinsX(); ++ibin) { 
      MCStat->SetBinError(ibin, MCStat->GetBinContent(ibin) * ratio->GetBinError(ibin) / ratio->GetBinContent(ibin) );
    }


    TH1D* Syst = Stat->Clone("h_mt2binsSyst");
    TH1D* pred = Stat->Clone("h_mt2bins");
    for ( unsigned int ibin = 0; ibin <= Stat->GetNbinsX(); ++ibin) { 
      Syst->SetBinError(ibin, 0.);
      double quadrature = Stat->GetBinError(ibin)*Stat->GetBinError(ibin) + Syst->GetBinError(ibin)*Syst->GetBinError(ibin);
      pred->SetBinError(ibin, sqrt(quadrature));
    }
    //pred->Print("all");

    TH1D* CRyield = hDY->Clone("h_mt2binsCRyield");

    pred->Write();
    Stat->Write();
    Syst->Write();
    CRyield->Write();
    MCStat->Write();


  } // loop over signal regions


  return;
}



//_______________________________________________________________________________
void ZinvMaker(){

  string input_dir = "/home/users/olivito/MT2Analysis/MT2looper/output/V00-00-08_fullstats/";
  //  string input_dir = "../MT2looper/output/2015ExtendedNJets/";
  //string input_dir = "../MT2looper/output/2015LowLumi/";
  string output_name = input_dir+"zinvFromGJ.root";
  // ----------------------------------------
  //  samples definition
  // ----------------------------------------

  std::cout << "Writing to file: " << output_name << std::endl;

  // get input files
  TFile* f_zinv = new TFile(Form("%s/zinv_ht.root",input_dir.c_str()));
  TFile* f_gjet = new TFile(Form("%s/gjet_ht.root",input_dir.c_str()));
  //TFile* f_qcd = new TFile(Form("%s/qcd_pt.root",input_dir.c_str()));

  vector<string> dirs;

  //Loop through list of every directory in the signal file.
  //if directory begins with "sr", excluding "srbase", add it to vector signal regions.
  TIter it(f_zinv->GetListOfKeys());
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

  //makeZinvFromGJets( f_zinv , f_gjet , f_qcd, dirs, dirsGJ, output_name, 0 );
  makeZinvFromGJets( f_zinv , f_gjet , f_zinv, dirs, output_name, 0 ); // not using QCD for now

  TFile* f_dy = new TFile(Form("%s/dyjetsll_ht.root",input_dir.c_str()));
   output_name = input_dir+"zinvFromDY.root";

  makeZinvFromDY( f_zinv , f_dy , dirs, output_name, 0 ); 


}

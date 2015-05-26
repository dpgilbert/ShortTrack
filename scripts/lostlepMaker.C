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
void makeLostLepFromCRs( TFile* f_data , TFile* f_lostlep , vector<string> dirs, string output_name ) {

  // Generate histogram file with lostlep prediction based on 1L CRs

  TFile * outfile = new TFile(output_name.c_str(),"RECREATE") ; 
  outfile->cd();
  const unsigned int ndirs = dirs.size();
  
  for ( unsigned int idir = 0; idir < ndirs; ++idir ) {
    TString directory = "sr"+dirs.at(idir);
    TString fullhistname = directory + "/h_mt2bins";
    TString n_mt2bins_name = directory + "/h_n_mt2bins";
    TString fullhistnameSL = "crsl"+TString(dirs.at(idir))+"/h_mt2bins";

    TH1D* h_lostlepMC_sr = (TH1D*) f_lostlep->Get(fullhistname);
    
    TH1D* h_n_mt2bins = (TH1D*) f_lostlep->Get(n_mt2bins_name);
    const int n_mt2bins = h_n_mt2bins->GetBinContent(1);
    float* mt2bins = new float[n_mt2bins+1];
    for(int i=0; i<=(n_mt2bins); i++){
      //std::cout << "bin edge = " << h_lostlep_sr->GetBinLowEdge(i+1) << std::endl;
      mt2bins[i] = h_lostlepMC_sr->GetBinLowEdge(i+1);  
    }

    TH1D* h_lostlepDD_sr = 0;
    if(h_lostlepMC_sr) {
      h_lostlepDD_sr = (TH1D*) h_lostlepMC_sr->Clone("h_mt2binsSR");
    } else {
      cout<<"couldn't find lostlep MC SR hist: "<<fullhistname<<endl;
      // make empty histogram
      h_lostlepDD_sr = new TH1D("h_mt2binsSR", "h_mt2binsSR", n_mt2bins, mt2bins);
    }

    TH1D* h_lostlepMC_cr = (TH1D*) f_lostlep->Get(fullhistnameSL);
    // check that histograms exist
    if (!h_lostlepMC_cr) {
      cout << "couldn't find lostlep MC CR hist: " << fullhistnameSL << endl;
    }

    TH1D* h_lostlepDD_cr = 0;
    TH1D* h_data_cr = (TH1D*) f_data->Get(fullhistnameSL);
    if (h_data_cr) {
      h_lostlepDD_cr = (TH1D*) h_data_cr->Clone("h_mt2binsCRyield");
    } else {
      cout << "couldn't find data CR hist: " << fullhistnameSL << endl;
      // make empty histogram
      h_lostlepDD_cr = new TH1D("h_mt2binsCRyield", "h_mt2binsCRyield", n_mt2bins, mt2bins);
    }

    // only needed if we have to create an empty histogram for the prediction
    delete mt2bins;

    if (h_lostlepDD_sr->GetNbinsX() != h_lostlepDD_cr->GetNbinsX() ) {
      cout<<"WARNING: different binning for histograms "<<fullhistname<<endl;
      continue;
    }
    
    // data-driven part: use data to normalize MC SR prediction
    //   need to do something fancier if we normalize to different TRs
    double norm = 1.;
    if (h_data_cr && h_lostlepMC_cr) norm = h_data_cr->Integral(0,-1)/h_lostlepMC_cr->Integral(0,-1);
    h_lostlepDD_sr->Scale(norm);

    // Make directory and plot(s) in the output file
    TDirectory* dir = 0;
    dir = (TDirectory*)outfile->Get(directory.Data());
    if (dir == 0) {
      dir = outfile->mkdir(directory.Data());
    } 
    dir->cd();

    // dummy: no systs for now
    TH1D* Syst = (TH1D*) h_lostlepDD_sr->Clone("h_mt2binsSyst");

    // MC stats needed for syst
    TH1D* MCStat = (TH1D*) h_lostlepDD_sr->Clone("h_mt2binsMCStat");

    // don't do anything special for errors for now on pred hist
    TH1D* pred = (TH1D*) h_lostlepDD_sr->Clone("h_mt2bins");
    // for ( int ibin = 0; ibin <= Stat->GetNbinsX(); ++ibin) { 
    //   Syst->SetBinError(ibin, 0.);
    //   double quadrature = Stat->GetBinError(ibin)*Stat->GetBinError(ibin) + Syst->GetBinError(ibin)*Syst->GetBinError(ibin);
    //   pred->SetBinError(ibin, sqrt(quadrature));
    // }
    //pred->Print("all");

    pred->Write();
    Syst->Write();
    h_lostlepDD_cr->Write();
    MCStat->Write();

  } // loop over signal regions


  return;
}

//_______________________________________________________________________________
void lostlepMaker(string input_dir = "/home/users/jgran/temp/update/MT2Analysis/MT2looper/output/V00-00-12/"){

  string output_name = input_dir+"lostlepFromCRs.root";
  std::cout << "Writing to file: " << output_name << std::endl;

  // get input files -- faking data for now with same MC file
  TFile* f_data = new TFile(Form("%s/lostlep.root",input_dir.c_str()));
  TFile* f_lostlep = new TFile(Form("%s/lostlep.root",input_dir.c_str()));
  
  if(f_data->IsZombie() || f_lostlep->IsZombie()) {
    std::cerr << "Input file does not exist" << std::endl;
    return;
  }

  vector<string> dirs;

  //Loop through list of every directory in the signal file.
  //if directory begins with "sr", excluding "srbase", add it to vector signal regions.
  TIter it(f_lostlep->GetListOfKeys());
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

  makeLostLepFromCRs( f_data , f_lostlep , dirs, output_name );

}

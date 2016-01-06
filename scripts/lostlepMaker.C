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

const int n_htbins = 5;
const float htbins[n_htbins+1] = {200, 450., 575., 1000., 1500., 3000.};
const int n_njbins = 4;
const float njbins[n_njbins+1] = {1, 2, 4, 7, 12};
const int n_nbjbins = 4;
const float nbjbins[n_nbjbins+1] = {0, 1, 2, 3, 6};

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
    TString crdir = "crsl"+TString(dirs.at(idir));
    TString fullhistnameSL = crdir+"/h_mt2bins";
    TString fullhistnameSLfinebin = crdir+"/h_mt2";
    TString fullhistnameSLHT = crdir+"/h_htbins";
    TString fullhistnameSLNj = crdir+"/h_njbins";
    TString fullhistnameSLNb = crdir+"/h_nbjbins";

    TH1D* h_lostlepMC_sr = (TH1D*) f_lostlep->Get(fullhistname);
    
    TH1D* h_n_mt2bins = (TH1D*) f_lostlep->Get(n_mt2bins_name);
    const int n_mt2bins = h_n_mt2bins->GetBinContent(1);
    float* mt2bins = new float[n_mt2bins+1];
    for(int i=0; i<=(n_mt2bins); i++){
      //std::cout << "bin edge = " << h_lostlep_sr->GetBinLowEdge(i+1) << std::endl;
      mt2bins[i] = h_lostlepMC_sr->GetBinLowEdge(i+1);  
    }

    TH1D* h_lostlepDD_sr = 0;
    TH1D* alphaHist = 0;
    if(h_lostlepMC_sr) {
      h_lostlepDD_sr = (TH1D*) h_lostlepMC_sr->Clone("h_mt2binsSR");
      alphaHist = (TH1D*) h_lostlepMC_sr->Clone("h_mt2binsAlpha");
    } else {
      cout<<"couldn't find lostlep MC SR hist: "<<fullhistname<<endl;
      // make empty histogram
      h_lostlepDD_sr = new TH1D("h_mt2binsSR", "h_mt2binsSR", n_mt2bins, mt2bins);
      alphaHist = new TH1D("h_mt2binsAlpha", "h_mt2binsAlpha", n_mt2bins, mt2bins);
    }

    TH1D* h_lostlepMC_cr = (TH1D*) f_lostlep->Get(fullhistnameSL);
    // check that histograms exist
    if (!h_lostlepMC_cr) {
      cout << "couldn't find lostlep MC CR hist: " << fullhistnameSL << endl;
    } else {
      alphaHist->Divide(h_lostlepMC_cr);
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
    
    // ------------------------------------------
    //  added to compare after normalizing MC
    
    TH1D* h_lostlepMC_cr_finebin = (TH1D*) f_lostlep->Get(fullhistnameSLfinebin);
    TH1D* h_lostlepMC_rescaled_cr_finebin = 0;
    if(h_lostlepMC_cr_finebin) {
      h_lostlepMC_rescaled_cr_finebin = (TH1D*) h_lostlepMC_cr_finebin->Clone("h_mt2CRMCrescaled");
    } else {
      cout<<"couldn't find lostlep MC CR finebin hist: "<<fullhistnameSLfinebin<<endl;
      // make empty histogram
      h_lostlepMC_rescaled_cr_finebin = new TH1D("h_mt2", "h_mt2", 150, 0, 1500);
    }

    TH1D* h_htbins_lostlepMC_cr = (TH1D*) f_lostlep->Get(fullhistnameSLHT);
    TH1D* h_htbins_lostlepMC_rescaled_cr = 0;
    if(h_htbins_lostlepMC_cr) {
      h_htbins_lostlepMC_rescaled_cr = (TH1D*) h_htbins_lostlepMC_cr->Clone("h_htbinsCRMCrescaled");
    } else {
      cout<<"couldn't find lostlep MC CR finebin hist: "<<fullhistnameSLHT<<endl;
      // make empty histogram
      h_htbins_lostlepMC_rescaled_cr = new TH1D("h_htbins", "h_htbins", n_htbins, htbins);
    }

    TH1D* h_njbins_lostlepMC_cr = (TH1D*) f_lostlep->Get(fullhistnameSLNj);
    TH1D* h_njbins_lostlepMC_rescaled_cr = 0;
    if(h_njbins_lostlepMC_cr) {
      h_njbins_lostlepMC_rescaled_cr = (TH1D*) h_njbins_lostlepMC_cr->Clone("h_njbinsCRMCrescaled");
    } else {
      cout<<"couldn't find lostlep MC CR finebin hist: "<<fullhistnameSLNj<<endl;
      // make empty histogram
      h_njbins_lostlepMC_rescaled_cr = new TH1D("h_njbins", "h_njbins", n_njbins, njbins);
    }

    TH1D* h_nbjbins_lostlepMC_cr = (TH1D*) f_lostlep->Get(fullhistnameSLNb);
    TH1D* h_nbjbins_lostlepMC_rescaled_cr = 0;
    if(h_nbjbins_lostlepMC_cr) {
      h_nbjbins_lostlepMC_rescaled_cr = (TH1D*) h_nbjbins_lostlepMC_cr->Clone("h_nbjbinsCRMCrescaled");
    } else {
      cout<<"couldn't find lostlep MC CR finebin hist: "<<fullhistnameSLNb<<endl;
      // make empty histogram
      h_nbjbins_lostlepMC_rescaled_cr = new TH1D("h_nbjbins", "h_nbjbins", n_nbjbins, nbjbins);
    }


    TH1D* h_data_cr_finebin = (TH1D*) f_data->Get(fullhistnameSLfinebin);
    TH1D* h_data_cr_finebin_save = 0;
    if (h_data_cr_finebin) {
      h_data_cr_finebin_save = (TH1D*) h_data_cr_finebin->Clone("h_mt2CRyield");
    } else {
      cout << "couldn't find data CR finebin hist: " << fullhistnameSLfinebin << endl;
      // make empty histogram
      h_data_cr_finebin_save = new TH1D("h_mt2CRyield", "h_mt2CRyield", 150, 0, 1500);
    }

    TH1D* h_htbins_data_cr = (TH1D*) f_data->Get(fullhistnameSLHT);
    TH1D* h_htbins_data_cr_save = 0;
    if (h_htbins_data_cr) {
      h_htbins_data_cr_save = (TH1D*) h_htbins_data_cr->Clone("h_htbinsCRyield");
    } else {
      cout << "couldn't find data CR finebin hist: " << fullhistnameSLHT << endl;
      // make empty histogram
      h_htbins_data_cr_save = new TH1D("h_htbinsCRyield", "h_htbinsCRyield", n_htbins, htbins);
    }
    
    TH1D* h_njbins_data_cr = (TH1D*) f_data->Get(fullhistnameSLNj);
    TH1D* h_njbins_data_cr_save = 0;
    if (h_njbins_data_cr) {
      h_njbins_data_cr_save = (TH1D*) h_njbins_data_cr->Clone("h_njbinsCRyield");
    } else {
      cout << "couldn't find data CR finebin hist: " << fullhistnameSLNj << endl;
      // make empty histogram
      h_njbins_data_cr_save = new TH1D("h_njbinsCRyield", "h_njbinsCRyield", n_njbins, njbins);
    }
    
    TH1D* h_nbjbins_data_cr = (TH1D*) f_data->Get(fullhistnameSLNb);
    TH1D* h_nbjbins_data_cr_save = 0;
    if (h_nbjbins_data_cr) {
      h_nbjbins_data_cr_save = (TH1D*) h_nbjbins_data_cr->Clone("h_nbjbinsCRyield");
    } else {
      cout << "couldn't find data CR finebin hist: " << fullhistnameSLNb << endl;
      // make empty histogram
      h_nbjbins_data_cr_save = new TH1D("h_nbjbinsCRyield", "h_nbjbinsCRyield", n_nbjbins, nbjbins);
    }
    
    // ------------------------------------------

    // only needed if we have to create an empty histogram for the prediction
    delete mt2bins;

    if (h_lostlepDD_sr->GetNbinsX() != h_lostlepDD_cr->GetNbinsX() ) {
      cout<<"WARNING: different binning for histograms "<<fullhistname<<endl;
      continue;
    }
    
    // data-driven part: use data to normalize MC SR prediction
    double norm = 1.;
    if (h_data_cr && h_lostlepMC_cr) norm = h_data_cr->Integral(0,-1)/h_lostlepMC_cr->Integral(0,-1);
    else if (!h_data_cr) norm = 0;
    h_lostlepDD_sr->Scale(norm);
    h_lostlepMC_rescaled_cr_finebin->Scale(norm);
    h_htbins_lostlepMC_rescaled_cr->Scale(norm);
    h_njbins_lostlepMC_rescaled_cr->Scale(norm);
    h_nbjbins_lostlepMC_rescaled_cr->Scale(norm);

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
    alphaHist->Write();
    h_lostlepMC_rescaled_cr_finebin->Write();
    h_data_cr_finebin_save->Write();
    
    h_htbins_lostlepMC_rescaled_cr->Write();
    h_njbins_lostlepMC_rescaled_cr->Write();
    h_nbjbins_lostlepMC_rescaled_cr->Write();
    h_htbins_data_cr_save->Write();
    h_njbins_data_cr_save->Write();
    h_nbjbins_data_cr_save->Write();

    // also save bin boundary hists for CR
    TH1D* h_ht_LOW = (TH1D*) f_lostlep->Get(crdir+"/h_ht_LOW");
    TH1D* h_ht_HI = (TH1D*) f_lostlep->Get(crdir+"/h_ht_HI");
    TH1D* h_nbjets_LOW = (TH1D*) f_lostlep->Get(crdir+"/h_nbjets_LOW");
    TH1D* h_nbjets_HI = (TH1D*) f_lostlep->Get(crdir+"/h_nbjets_HI");
    TH1D* h_njets_LOW = (TH1D*) f_lostlep->Get(crdir+"/h_njets_LOW");
    TH1D* h_njets_HI = (TH1D*) f_lostlep->Get(crdir+"/h_njets_HI");

    h_ht_LOW->Write();
    h_ht_HI->Write();
    h_nbjets_LOW->Write();
    h_nbjets_HI->Write();
    h_njets_LOW->Write();
    h_njets_HI->Write();
    
  } // loop over signal regions


  return;
}

//_______________________________________________________________________________
void lostlepMaker(string input_dir = "/home/users/jgran/temp/update/MT2Analysis/MT2looper/output/V00-00-12/", string dataname = "lostlep"){

  string output_name = input_dir+"/lostlepFromCRs.root";
  std::cout << "Writing to file: " << output_name << std::endl;

  // get input files -- default to faking data with same MC file
  TFile* f_data = new TFile(Form("%s/%s.root",input_dir.c_str(),dataname.c_str()));
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

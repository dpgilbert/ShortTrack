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
void makeZinvFromGJets( TFile* fZinv , TFile* fGJet , TFile* fZll , vector<string> dirs, string output_name, float kFactorGJetForRatio = 1.0 ) {

  // Generate histogram file with Zinv prediction based on GJetsData * R(Zinv/GJ)


  TFile * outfile = new TFile(output_name.c_str(),"RECREATE") ; 
  outfile->cd();
  const unsigned int ndirs = dirs.size();
  
  // Do the inclusive ones
  vector<TString> inclPlots;
  inclPlots.push_back("h_njbins");
  inclPlots.push_back("h_nbjbins");
  inclPlots.push_back("h_htbins");
  inclPlots.push_back("h_mt2bins");

  for ( unsigned int incl = 0; incl < inclPlots.size(); ++incl ) {
    
    TH1D* hGJetIncl = (TH1D*) fGJet->Get("crgjbase/"+inclPlots[incl])->Clone();
    TH1D* hZllIncl  = (TH1D*)  fZll->Get("crdybase/"+inclPlots[incl])->Clone();

    if(!hGJetIncl || !hZllIncl){
      cout<<"could not find histogram "<<inclPlots[incl]<<endl;
      continue;
    }
    if (hGJetIncl->GetNbinsX() != hZllIncl->GetNbinsX() ) {
      cout<<"different binning for histograms "<<inclPlots[incl]<<endl;
      continue;
    }
    outfile->cd();

    hGJetIncl->Scale(kFactorGJetForRatio); // The goal is LO(Z) / LO(gamma)

    // Since we're working on MC, let's set poissionian errors by hand
    hZllIncl->Sumw2(0); hZllIncl->Sumw2(1);
    hGJetIncl->Sumw2(0); hGJetIncl->Sumw2(1);
    TH1D* ratioIncl = (TH1D*) hZllIncl->Clone(inclPlots[incl]+"Ratio");
    ratioIncl->Divide(hGJetIncl);

    ratioIncl->Write();

  } // end of inclusive plots

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
    //hGJet->Print("all");
    hGJet->Scale(kFactorGJetForRatio); // The goal is LO(Z) / LO(gamma)

    // Make directory and plot(s) in the output file
    TDirectory* dir = 0;
    dir = (TDirectory*)outfile->Get(directory.Data());
    if (dir == 0) {
      dir = outfile->mkdir(directory.Data());
    } 
    dir->cd();

    TH1D* Stat = (TH1D*) hZinv->Clone("h_mt2binsStat");
    cout<<"Looking at histo "<<fullhistname<<endl;
    for ( int ibin = 0; ibin <= Stat->GetNbinsX(); ++ibin) { // "<=" to deal with overflow bin
      if (hGJet->GetBinContent(ibin) > 0)
        Stat->SetBinError(ibin, hZinv->GetBinContent(ibin)/sqrt( hGJet->GetBinContent(ibin) ));
      else Stat->SetBinError(ibin, hZinv->GetBinContent(ibin));
    }
    

    // Zgamma ratio in each MT2bin -> to get MC stat error on ratio
    TH1D* ratio = (TH1D*) hZinv->Clone("h_mt2binsRatio");
    //ratio->Print("all");
    //hGJet->Print("all");
    ratio->Divide(hGJet);
    //ratio->Print("all");

    // MCStat: use relative bin error from ratio hist, normalized to Zinv MC prediction
    TH1D* MCStat = (TH1D*) hZinv->Clone("h_mt2binsMCStat");
    for ( int ibin = 0; ibin <= Stat->GetNbinsX(); ++ibin) { 
      MCStat->SetBinError(ibin, MCStat->GetBinContent(ibin) * ratio->GetBinError(ibin) / ratio->GetBinContent(ibin) );
    }

    TH1D* Syst = (TH1D*) Stat->Clone("h_mt2binsSyst");
    TH1D* pred = (TH1D*) Stat->Clone("h_mt2bins");
    for ( int ibin = 0; ibin <= Stat->GetNbinsX(); ++ibin) { 
      Syst->SetBinError(ibin, 0.);
      double quadrature = Stat->GetBinError(ibin)*Stat->GetBinError(ibin) + Syst->GetBinError(ibin)*Syst->GetBinError(ibin);
      pred->SetBinError(ibin, sqrt(quadrature));
    }
    //pred->Print("all");

    TH1D* CRyield = (TH1D*) hGJet->Clone("h_mt2binsCRyield");

    // Extrapolation to next bin: just a ratio of GJet_i/GJet_i-1, so that we can later obtain bin i prediction from bin i-1 yield
    // Instead of : GJet_i * R(Zinv_i/GJet_i), we will do GJet_i-1 * R(GJet_i/GJet_i-1) * R(Zinv_i/GJet_i)
    TH1D* PreviousBinRatio = (TH1D*) hGJet->Clone("h_mt2binsPreviousBinRatio");
    for ( int ibin = 0; ibin <= Stat->GetNbinsX(); ++ibin) { 
      if (ibin<=1) PreviousBinRatio->SetBinContent(ibin, 1.);
      else {
	PreviousBinRatio->SetBinContent(ibin, hGJet->GetBinContent(ibin)/hGJet->GetBinContent(ibin-1));
	PreviousBinRatio->SetBinContent(ibin, hGJet->GetBinContent(ibin)/hGJet->GetBinContent(ibin-1));
      }
      PreviousBinRatio->SetBinError(ibin, 0.); // Ignore uncertainty (just MC anyway)
    }
    

    pred->Write();
    Stat->Write();
    Syst->Write();
    CRyield->Write();
    MCStat->Write();
    PreviousBinRatio->Write();
    ratio->Write();
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

    TH1D* Stat = (TH1D*) hZinv->Clone("h_mt2binsStat");
    cout<<"Looking at histo "<<fullhistname<<endl;
    if (method==0) { // --- Simple: Zinv +/- Zinv/sqrt(DY)
      for ( int ibin = 0; ibin <= Stat->GetNbinsX(); ++ibin) { // "<=" to deal with overflow bin
	if (hDY->GetBinContent(ibin) > 0)
	  Stat->SetBinError(ibin, hZinv->GetBinContent(ibin)/sqrt( hDY->GetBinContent(ibin) ));
	else Stat->SetBinError(ibin, hZinv->GetBinContent(ibin));
      }
    }

    TH1D* ratio = (TH1D*) hZinv->Clone("ratio");
    ratio->Divide(hDY);

    // MCStat: use relative bin error from ratio hist, normalized to Zinv MC prediction
    TH1D* MCStat = (TH1D*) hZinv->Clone("h_mt2binsMCStat");
    for ( int ibin = 0; ibin <= Stat->GetNbinsX(); ++ibin) { 
      MCStat->SetBinError(ibin, MCStat->GetBinContent(ibin) * ratio->GetBinError(ibin) / ratio->GetBinContent(ibin) );
    }


    TH1D* Syst = (TH1D*) Stat->Clone("h_mt2binsSyst");
    TH1D* pred = (TH1D*) Stat->Clone("h_mt2bins");
    for ( int ibin = 0; ibin <= Stat->GetNbinsX(); ++ibin) { 
      Syst->SetBinError(ibin, 0.);
      double quadrature = Stat->GetBinError(ibin)*Stat->GetBinError(ibin) + Syst->GetBinError(ibin)*Syst->GetBinError(ibin);
      pred->SetBinError(ibin, sqrt(quadrature));
    }
    //pred->Print("all");

    TH1D* CRyield = (TH1D*) hDY->Clone("h_mt2binsCRyield");

    pred->Write();
    Stat->Write();
    Syst->Write();
    CRyield->Write();
    MCStat->Write();


  } // loop over signal regions


  return;
}



//_______________________________________________________________________________
void ZinvMaker(string input_dir = "/home/users/gzevi/MT2/MT2Analysis/MT2looper/output/V00-00-11skim/"){

  //  string input_dir = "/home/users/olivito/MT2Analysis/MT2looper/output/V00-00-08_fullstats/";
  //  string input_dir = "../MT2looper/output/2015ExtendedNJets/";
  //string input_dir = "../MT2looper/output/2015LowLumi/";
  string output_name = input_dir+"/zinvFromGJ.root";
  // ----------------------------------------
  //  samples definition
  // ----------------------------------------

  std::cout << "Writing to file: " << output_name << std::endl;

  // get input files
  TFile* f_zinv = new TFile(Form("%s/zinv_ht.root",input_dir.c_str()));
  TFile* f_gjet = new TFile(Form("%s/gjet_ht.root",input_dir.c_str()));
  //TFile* f_qcd = new TFile(Form("%s/qcd_pt.root",input_dir.c_str()));
  TFile* f_dy = new TFile(Form("%s/dyjetsll_ht.root",input_dir.c_str()));


  if(f_zinv->IsZombie() || f_gjet->IsZombie()) {
    std::cerr << "Input file does not exist" << std::endl;
    return;
  }

  vector<string> dirs;

  //Loop through list of every directory in the signal file.
  //if directory begins with "sr", excluding "srbase", add it to vector signal regions.
  TIter it(f_zinv->GetListOfKeys());
  TKey* k;
  std::string keep = "sr";
  std::string skip = "srbase";
  while ((k = (TKey *)it())) {
//    if (strncmp (k->GetTitle(), skip.c_str(), skip.length()) == 0) continue;
    if (strncmp (k->GetTitle(), keep.c_str(), keep.length()) == 0) {//it is a signal region
      std::string sr_string = k->GetTitle();
      sr_string.erase(0, 2);//remove "sr" from front of string
      dirs.push_back(sr_string);
    }
  }

  //makeZinvFromGJets( f_zinv , f_gjet , f_qcd, dirs, dirsGJ, output_name, 0 );
  makeZinvFromGJets( f_zinv , f_gjet , f_dy ,dirs, output_name, 1.23 ); // not using QCD for now

   output_name = input_dir+"/zinvFromDY.root";

  makeZinvFromDY( f_zinv , f_dy , dirs, output_name, 0 ); 


}

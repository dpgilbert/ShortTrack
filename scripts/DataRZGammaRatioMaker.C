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

bool isData = false;

//_______________________________________________________________________________
void makeDataRatio( TFile* fData , TFile* fPurity , TFile* fTop, string output_name, float kFactorGJetForRatio = 1.0 ) {

  // Generate histogram file with Zinv prediction based on GJetsData * R(Zinv/GJ)
  
  // Here we want two sets of histograms, one with purity/frag uncertainty and one without
  // Uncertainties are: crdy stat, crgj stat, top syst
  // Inputs are: data (or sum of DY, GJ, QCD, TOP MC) for stat uncertainties,
  //       purity.root for actual photon yield (h_htbinsphotonestimateFailSieieData, but without its uncertainty),
  //       top.root for top subtraction


  TFile * outfile = new TFile(output_name.c_str(),"RECREATE") ; 
  outfile->cd();
  
  // Do the inclusive ones
  vector<TString> inclPlots;
  inclPlots.push_back("h_njbins");
  inclPlots.push_back("h_nbjbins");
  inclPlots.push_back("h_htbins");
  inclPlots.push_back("h_htbins2");
  inclPlots.push_back("h_mt2bins");

  for ( unsigned int incl = 0; incl < inclPlots.size(); ++incl ) {
    
    TH1D* hGJetStat  = (TH1D*) fData->Get("crgjbaseIncl/"+inclPlots[incl]);
    TH1D* hZllStat   = (TH1D*) fData->Get("crdybaseIncl/"+inclPlots[incl]);
    TH1D* hGJetYield = (TH1D*) fPurity->Get("srbaseIncl/"+inclPlots[incl]+"photonestimateFailSieieData");
    TH1D* hTop       = (TH1D*) fTop->Get("crdybaseIncl/"+inclPlots[incl]);
    if(!hGJetStat || !hGJetYield || !hZllStat || !hTop){
      cout<<"could not find histogram "<<inclPlots[incl]<<endl;
      continue;
    }
    if (hGJetYield->GetNbinsX() != hZllStat->GetNbinsX() ) {
      cout<<"different binning for histograms "<<inclPlots[incl]<<endl;
      continue;
    }
    outfile->cd();

    if (!isData) hGJetYield->Scale(kFactorGJetForRatio); // The goal is LO(Z) / LO(gamma)

    TH1D* ratioIncl = (TH1D*) hZllStat->Clone(inclPlots[incl]+"Ratio");
    ratioIncl->Add(hTop, -1.);
    ratioIncl->Divide(hGJetYield);
    for ( int ibin = 0; ibin <= ratioIncl->GetNbinsX(); ++ibin) {
      if (ratioIncl->GetBinContent(ibin) > 0) {
        float zllStatErr =  hZllStat->GetBinError(ibin)/hZllStat->GetBinContent(ibin)  * ratioIncl->GetBinContent(ibin);
        float gjStatErr  =  hGJetStat->GetBinError(ibin)/hGJetStat->GetBinContent(ibin) * ratioIncl->GetBinContent(ibin);
        float topSystErr =  0.5 * hTop->GetBinContent(ibin) / hZllStat->GetBinContent(ibin)  * ratioIncl->GetBinContent(ibin);
        float relativeErrorSquared = zllStatErr*zllStatErr + gjStatErr*gjStatErr + topSystErr*topSystErr;
        ratioIncl->SetBinError(ibin, sqrt(relativeErrorSquared));
      }
      else ratioIncl->SetBinError(ibin, 0.);
    }
    TH1D* ratioInclWithPurityUnc = (TH1D*) ratioIncl->Clone(inclPlots[incl]+"RatioWithPurityUnc");
    for ( int ibin = 0; ibin <= ratioInclWithPurityUnc->GetNbinsX(); ++ibin) {
      float tenpercent = 0.1 * ratioInclWithPurityUnc->GetBinContent(ibin);
      float rest = ratioInclWithPurityUnc->GetBinError(ibin);
      ratioInclWithPurityUnc->SetBinError(ibin, sqrt (rest*rest + tenpercent*tenpercent));
    }
    
    
    //    hGJetStat->Print("all");
    //    hGJetYield->Print("all");
    //    hZllStat->Print("all");
    //    hTop->Print("all");
    //    ratioIncl->Print("all");
    ratioIncl->Write();
    ratioInclWithPurityUnc->Write();

  } // end of inclusive plots

  return;
}




//_______________________________________________________________________________
void DataRZGammaRatioMaker(string input_dir = "/home/users/gzevi/MT2/MT2Analysis/MT2looper/output/V00-00-11skim/", string dataname = "data"){


  string output_name = input_dir+"/doubleRatio.root";
  // ----------------------------------------
  //  samples definition
  // ----------------------------------------

  std::cout << "Writing to file: " << output_name << std::endl;

  // get input files
  TString datanamestring(dataname);

  if (datanamestring.Contains("Data") || datanamestring.Contains("data")) isData = true;
  TFile* f_data = new TFile(Form("%s/%s.root",input_dir.c_str(),dataname.c_str())); //data or qcd+gjets file
  TFile* f_purity = new TFile(Form("%s/purity.root",input_dir.c_str()));
  TFile* f_top = new TFile(Form("%s/top.root",input_dir.c_str()));


  if(f_data->IsZombie() || f_purity->IsZombie() || f_top->IsZombie()) {
    std::cerr << "Input file does not exist" << std::endl;
    return;
  }


  makeDataRatio( f_data , f_purity , f_top , output_name, 1.23 );


}

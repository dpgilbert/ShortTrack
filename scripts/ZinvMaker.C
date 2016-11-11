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
bool doHybrid = true; // hybrid estimate: uses CR MT2 binning until the (MC) integral is less than the threshold below
float hybrid_nevent_threshold = 20.;

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
  inclPlots.push_back("h_htbins2");
  inclPlots.push_back("h_mt2bins");
  inclPlots.push_back("h_bosonptbins");

  for ( unsigned int incl = 0; incl < inclPlots.size(); ++incl ) {

    TH1D* hGJetIncl = (TH1D*) fGJet->Get("crgjbaseIncl/"+inclPlots[incl])->Clone();
    TH1D* hZllIncl  = (TH1D*)  fZll->Get("crdybaseIncl/"+inclPlots[incl])->Clone();

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

    cout<<"Looking at directory "<<directory<<endl;

    TString fullhistname = directory + "/h_mt2bins";
    TString fullhistnameHT = directory + "/h_htbins";
    TString fullhistnameHT2 = directory + "/h_htbins2";
    TString fullhistnameNJ = directory + "/h_njbins";
    TString fullhistnameNB = directory + "/h_nbjbins";
    //    TString fullhistnamePT = directory + "/h_bosonptbins";
    TString fullhistnameGJ = directoryGJ + "/h_mt2bins";
      
    TH1D* hGJet = (TH1D*) fGJet->Get(fullhistnameGJ);    
    TH1D* hZinv = (TH1D*) fZinv->Get(fullhistname);
    TH1D* hZinvHT = (TH1D*) fZinv->Get(fullhistnameHT);
    TH1D* hZinvHT2 = (TH1D*) fZinv->Get(fullhistnameHT2);
    TH1D* hZinvNJ = (TH1D*) fZinv->Get(fullhistnameNJ);
    TH1D* hZinvNB = (TH1D*) fZinv->Get(fullhistnameNB);
    //    TH1D* hZinvPT = (TH1D*) fZinv->Get(fullhistnamePT);
    
    // If Zinv or GJet histograms are not filled, just leave (shouldn't happen when running on full stat MC)
    if(!hGJet ){
      cout<<"could not find histogram "<<fullhistnameGJ<<endl;
      continue;
    }
    if(!hZinv){
      cout<<"could not find histogram "<<fullhistname<<endl;
      continue;
    }
//    if(!hZinvPT){
//      cout<<"could not find histogram "<<fullhistnamePT<<" in file "<<fZinv->GetName()<<endl;
//      continue;
//    }
    cout<<"Looking at histo "<<fullhistname<<endl;      
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

    TH1D* ratioInt = (TH1D*) hZinv->Clone("h_mt2binsRatioInt");
    double nGammaErr = 0;
    double nGamma = hGJet->IntegralAndError(0, -1, nGammaErr);
    for ( int ibin = 0; ibin <= ratioInt->GetNbinsX(); ++ibin) {
      if (nGamma>0) {
        ratioInt->SetBinContent(ibin, ratioInt->GetBinContent(ibin)/nGamma);
        float errOld = ratioInt->GetBinError(ibin)/nGamma;
        float errNew = (nGammaErr/nGamma) * ratioInt->GetBinContent(ibin);
        cout<<nGamma<<" "<<nGammaErr<<" "<<errOld<<" "<<errNew<<endl;
        ratioInt->SetBinError(ibin, sqrt( errOld*errOld + errNew*errNew ) );
      }
      else {
        ratioInt->SetBinContent(ibin, 0);
        ratioInt->SetBinError(ibin, 0);
      }
    }
    ratioInt->Print("all");

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
    ratioInt->Write();
    hZinvHT->Write();
    hZinvHT2->Write();
    hZinvNJ->Write();
    hZinvNB->Write();
    //    hZinvPT->Write();
  } // loop over signal regions

  return;
}


//_______________________________________________________________________________
void makeZinvFromDY( TFile* fData , TFile* fZinv , TFile* fDY ,TFile* fTop, vector<string> dirs, string output_name ) {

  // Generate histogram file with Zinv prediction based on DYData * R(Zinv/DY)

  TFile * outfile = new TFile(output_name.c_str(),"RECREATE") ; 
  outfile->cd();
  const unsigned int ndirs = dirs.size();
  
  for ( unsigned int idir = 0; idir < ndirs; ++idir ) {
    TString directory = "sr"+dirs.at(idir);
    TString directoryDY = "crdy"+dirs.at(idir);

    TString fullhistname = directory + "/h_mt2bins";
    TString fullhistnameDY = directoryDY + "/h_mt2bins";
    TString fullhistnameEM = directoryDY + "/h_mt2binsemu";

    TH1D* hData = (TH1D*) fData->Get(fullhistnameDY);    
    TH1D* hDY   = (TH1D*) fDY->Get(fullhistnameDY);    
    TH1D* hDataEM   = (TH1D*) fDY->Get(fullhistnameEM);    
    TH1D* hZinv = (TH1D*) fZinv->Get(fullhistname);    
    TH1D* hTop  = (TH1D*) fTop->Get(fullhistnameDY);    
    
    // If Zinv or DY histograms are not filled, just leave (shouldn't happen when running on full stat MC)
    if(!hDY || !hZinv || !hData){
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

    cout<<"Looking at histo "<<fullhistname<<endl;

    int lastbin_hybrid = 1;
    if (doHybrid) {
      // hybrid method: use nominal MC CR yield histogram to determine how many MT2 bins to use
      //  by default: use all MT2 bins integrated (no bin-by-bin).
      //  choose the last bin to try to have at least hybrid_nevent_threshold integrated events
      for ( int ibin=1; ibin <= hData->GetNbinsX(); ++ibin ) {
	float top = 0;
	if (hDataEM) top = hDataEM->Integral(ibin,-1);
	if (hData->Integral(ibin,-1) - top < hybrid_nevent_threshold) {
	  if (ibin == 1) lastbin_hybrid = 1;
	  else lastbin_hybrid = ibin-1;

	  break;
	}
      }
      cout<<"lastbin_hybrid "<<lastbin_hybrid<<endl;
    }
    

    TH1D* ratio = (TH1D*) hZinv->Clone("ratio");
    ratio->Divide(hDY);

    TH1D* CRyield = (TH1D*) hData->Clone("h_mt2binsCRyield");

    TH1D* purityMC = (TH1D*) hDY->Clone("h_mt2binsPurityMC");
    if (hTop) purityMC->Add(hTop, -1);
    purityMC->Divide(hDY);

    TH1D* purityData = (TH1D*) hData->Clone("h_mt2binsPurityData");
    if (hDataEM) purityData->Add(hDataEM, -1);
    purityData->Divide(purityData, hData, 1, 1, "B");
    
    TH1D* Stat = (TH1D*) CRyield->Clone("h_mt2binsStat");
    Stat->Multiply(purityData);
    Stat->Multiply(ratio);

    TH1D* Syst = (TH1D*) Stat->Clone("h_mt2binsSyst");
    TH1D* pred = (TH1D*) Stat->Clone("h_mt2bins");
    for ( int ibin = 0; ibin <= Stat->GetNbinsX(); ++ibin) { 
      Syst->SetBinError(ibin, (1-purityData->GetBinContent(ibin))*0.2*Stat->GetBinContent(ibin));
      double quadrature = Stat->GetBinError(ibin)*Stat->GetBinError(ibin) + Syst->GetBinError(ibin)*Syst->GetBinError(ibin);
      pred->SetBinError(ibin, sqrt(quadrature));
    }
    //pred->Print("all");

    // Inputs to cardMaker
    TH1D* ratioCard  = (TH1D*) ratio->Clone("ratioCard");
    TH1D* purityCard  = (TH1D*) purityData->Clone("purityCard");   
    TH1D* CRyieldCard  = (TH1D*) CRyield->Clone("CRyieldCard");

    if (doHybrid) {
      // purity needs to be mofidied so the last N bins all describe the purity of the integrated yield
      // ratio needs to be modified so that the last N bins include kMT2
      // CRyield needs to be modified so that the last N bins have the same yield (which is the integral over those N bins)
      for ( int ibin=1; ibin <= hZinv->GetNbinsX(); ++ibin ) {

	if (ibin < lastbin_hybrid) continue;
	
	double integratedYieldErr = 0;
	float integratedYield = CRyield->IntegralAndError(lastbin_hybrid,-1,integratedYieldErr);
	CRyieldCard->SetBinContent(ibin, integratedYield);
	CRyieldCard->SetBinError(ibin, integratedYieldErr);

	float integratedDen = integratedYield;
	float EM = 0;
	if (hDataEM) EM =  hDataEM->Integral(lastbin_hybrid, -1);
	float integratedNum = integratedDen - EM;
	if (integratedNum < 0) integratedNum = 0;
	float integratedPurity = integratedNum/integratedDen;
	float integratedPurityErr = sqrt(integratedPurity*(1-integratedPurity)/integratedDen);// sqrt(e(1-e)/N)
	purityCard->SetBinContent(ibin, integratedPurity);
	purityCard->SetBinError(ibin, integratedPurityErr);

	float integratedZinv = hZinv->Integral(lastbin_hybrid, -1);
	float kMT2 = hZinv->GetBinContent(ibin) / integratedZinv;
	ratioCard->SetBinContent(ibin, ratioCard->GetBinContent(ibin) * kMT2);
	ratioCard->SetBinError(ibin, ratioCard->GetBinError(ibin) * kMT2 ); // just rescale the error by the same amount

      }

    }

    TH1D* hybridEstimate  = (TH1D*) CRyieldCard->Clone("hybridEstimate");
    hybridEstimate->Multiply(purityCard);
    hybridEstimate->Multiply(ratioCard);
    
    
    TH1D* h_lastbinHybrid = new TH1D("h_lastbinHybrid",";last bin",1,0,1);
    h_lastbinHybrid->SetBinContent(1,lastbin_hybrid);



    pred->Write();
    Stat->Write();
    Syst->Write();
    purityMC->Write();
    purityData->Write();
    ratio->Write();
    CRyield->Write();

    ratioCard->Write();
    purityCard->Write();
    CRyieldCard->Write();
    h_lastbinHybrid->Write();
    hybridEstimate->Write();


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
  TFile* f_data = new TFile(Form("%s/data_Run2016.root",input_dir.c_str()));
  TFile* f_zinv = new TFile(Form("%s/zinv_ht.root",input_dir.c_str()));
  TFile* f_gjet = new TFile(Form("%s/gjets_dr0p05_ht.root",input_dir.c_str()));
  //TFile* f_qcd = new TFile(Form("%s/qcd_pt.root",input_dir.c_str()));
  TFile* f_dy = new TFile(Form("%s/dyjetsll_ht.root",input_dir.c_str()));
  TFile* f_top = new TFile(Form("%s/top.root",input_dir.c_str()));
  //TFile* f_dy = new TFile(Form("%s/dyjetsll_incl.root",input_dir.c_str()));


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
    if (strncmp (k->GetTitle(), keep.c_str(), keep.length()) == 0 
) {//it is a signal region
      std::string sr_string = k->GetTitle();
      sr_string.erase(0, 2);//remove "sr" from front of string
      dirs.push_back(sr_string);
    }
  }

  //makeZinvFromGJets( f_zinv , f_gjet , f_qcd, dirs, dirsGJ, output_name, 0 );
  makeZinvFromGJets( f_zinv , f_gjet , f_dy ,dirs, output_name, 1.23 ); // not using QCD for now

   output_name = input_dir+"/zinvFromDY.root";

   makeZinvFromDY( f_data, f_zinv , f_dy , f_top, dirs, output_name ); 


}

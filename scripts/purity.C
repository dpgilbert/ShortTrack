//script to make purity.root file
//which contains purity histograms
//calculated from fake-rate predictions

#include <string>
#include <iostream>
#include <algorithm>
#include <map>
#include <sstream>
#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TStyle.h"
#include "TH1F.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TString.h"
#include "TLegend.h"
#include "TKey.h"
#include "TEfficiency.h"
#include "TMath.h"

#include "../MT2CORE/mt2tree.h"
#include "../MT2CORE/sigSelections.h"
#include "../MT2CORE/SR.h"
#include "../MT2CORE/sigSelections.cc"
#include "../MT2CORE/SR.cc"

using namespace std;
using namespace mt2;

bool verbose = true;
bool realData = false;


//converts float to string
std::string toString(float in){
  std::stringstream ss;
  ss << in;
  return ss.str();
}

//returns an empty hist with same binning
inline TH1F* sameBin(TH1F* h_in)
{
  TH1F* h_out = (TH1F*) h_in->Clone();
  h_out->Reset();
  return h_out;
}


//takes a FR histogram and signal region and calculates the predicted number of fakes
//in the signal region 'sr' , adding fragScale*qcdPrompt to LooseNotTight
void makePred(TFile* f_out, TFile* f_in, TFile* f_qcd, TFile* f_gjet, TString srName, TH2D* h_FR, const float fragScale = 1, TString s = "")
{

  //sr name
  bool realDataLocal = false;
  if (s.Contains("Data") || s.Contains("data")) realDataLocal = true;


  if (verbose) cout<<" ------ Looking at SR "<<srName<<". Fake rate histo has "<<h_FR->GetSize()<<" bins"<<endl;
  
  //Single bin or not
  TString bin = "";
  if(s.Contains("FailSieie")) bin = "SingleBin";
  
  //get LooseNotTight hist (this only contains Fakes, there are no Prompts, so no subtraction is needed for MC)
  TH1F* h_LooseNotTight = (TH1F*) f_in->Get("crgj"+srName+"/h_mt2binsLooseNotTight");

  //check for empty hists
  if(!h_FR){
    cout << "FR hist is null!" << endl;
    return;
  }
  if(!h_LooseNotTight){
    cout << "SR" << srName << " LooseNotTight hist is empty! Skipping signal region..." << endl;
    return;
  }

  //initialize prediction array
//  const int n_mt2bins = sr.GetNumberOfMT2Bins();
  const int n_mt2bins = h_LooseNotTight->GetNbinsX();
  Double_t preds[n_mt2bins+2]; //first and last bin are over/underflow
  Double_t predErrors[n_mt2bins+2]; //first and last bin are over/underflow
  for(int i=0; i<n_mt2bins+2; i++){
    preds[i] = 0;
    predErrors[i] = 0;
  }
  
  //mt2bin names
  vector<TString> mt2binsname;
  for(int i=0; i<=n_mt2bins; i++){
//    mt2binsname.push_back(toString(sr.GetMT2Bins()[i]));
    mt2binsname.push_back(toString(h_LooseNotTight->GetBinLowEdge(i+1)));
    cout<<"Added MT2 bin name "<<h_LooseNotTight->GetBinLowEdge(i+1)<<endl;
  }
  
  //loop over mt2bins
  for (Int_t xbin = 0; xbin < n_mt2bins; xbin++) 
    {
      if (verbose) cout<<" --- Looking at MT2 bin "<<xbin<<" starting at "<<mt2binsname[xbin]<<endl;
      //initialize pred_totals
      Float_t pred_total       = 0.0;
      Float_t pred_error_total = 0.0;
  
      //get FR binned hists for this mt2 bin
      TH2D* h_sidebandqcdPrompt = (TH2D*) f_qcd->Get("crgj"+srName+"/h2d_gammaht_gammapt"+bin+mt2binsname[xbin]+"LooseNotTight"); //qcdPrompt (Fragmentation)
      TH2D* h_sidebandgjetPrompt = (TH2D*) f_gjet->Get("crgj"+srName+"/h2d_gammaht_gammapt"+bin+mt2binsname[xbin]+"LooseNotTight"); //gjetPrompt (Prompt contamination)
      TH2D* h_sideband = (TH2D*) f_in->Get("crgj"+srName+"/h2d_gammaht_gammapt"+bin+mt2binsname[xbin]+"LooseNotTight"); //Data
      if (!realDataLocal) h_sideband = (TH2D*) f_in->Get("crgj"+srName+"/h2d_gammaht_gammapt"+bin+mt2binsname[xbin]+"FakeLooseNotTight"); //Need "Fake" folder if running on MC
      
      //if "Poisson", undo sumw2() errors
      if(s.Contains("Poisson")){
	//if(h_sidebandqcdPrompt) h_sidebandqcdPrompt->GetSumw2()->Set(0); //Prompt contamination always come from MC anyway, so weights are fine
	if(h_sideband) h_sideband->GetSumw2()->Set(0);
      }
	
      //skip this mt2bin if there are no fakes
      if(!h_sideband) {cout<<"crgj"<<srName<<"/h2d_gammaht_gammapt"<<bin<<mt2binsname[xbin]<<"LooseNotTight not found"<<endl; continue;}

      // Subtract prompt contamination in sideband (when looking at data)
      if (h_sidebandgjetPrompt && realDataLocal) {
        h_sideband->Add(h_sidebandgjetPrompt, -1);
      }
      // end of prompt subtraction in sideband
      
      //loop over FR bins
      for (Int_t FRbin = 1; FRbin < h_FR->GetSize()+1; FRbin++) 
	{

	  Float_t nFOs      = h_sideband->GetBinContent(FRbin);    // number of denominator not numerator objects for this bin
    if (nFOs<0) nFOs = 0;
	  Float_t nFOsError = h_sideband->GetBinError(FRbin);      // number error on number of of denominator not numerator objects for this bin

	  Float_t FRvalue   = h_FR->GetBinContent(FRbin); // get value of fake rate for this bin
	  Float_t FRerror   = h_FR->GetBinError(FRbin);   // get error on fake rate for this bin	  
	  if (FRvalue>0.9 || FRvalue<0) {
	    cout<<"STRANGE: FRvalue is "<<FRvalue<<". Setting it to 0.5"<<endl;
	    FRvalue = 0.5;
	  }
	  //shift nFOs by fragScale*QCDPrompt in LooseNotTight. (add fragmentation to sideband, unless looking at data)
	  if(h_sidebandqcdPrompt && !realDataLocal) {
	    float fragYield = h_sidebandqcdPrompt->Integral();
	    if (fragYield > 0) {
	      nFOs = nFOs + fragScale*fragYield;
	    }
	  }
	  
	  //--- error propagation shamelessly stolen from frank golfs code---
	  
	  // ok, have all of the inputs, calculate prediction and error for this bin
	  // start with calculating fr/(1-fr) for this bin
	  const Float_t fr = FRvalue / (1 - FRvalue);
	  const Float_t pred = fr * nFOs;
    
	  //    if (verbose && nFOs>0) 
	  cout<<"FR bin "<<FRbin<<" has FR "<<FRvalue<<", nFO "<<nFOs<<" and predicted "<<pred<<endl;
	  
	  // now, need to get errors on these terms, be careful
	  // start with hardest part, error on fr
	  //
	  // start with basic equation: (sigma_f/f)^2 = (sigma_num/num)^2 + (sigma_den/den)^2 - 2 * (sigma_num/num) * (sigma_den/den) * rho_num_den
	  // f is the value fr from above
	  // num is the value FRvalue from above
	  // den is 1 - FRvalue
	  //
	  // sigma_num is the value FRerror from above
	  // sigma_den is also the value FRerror from above (the error on 1 - A is just the error on A)
	  // 
	  // the numerator and denominator are completely anti-correlated, so rho_num_den is -1
	  // 
	  // combining all of this we can simplify the above to be
	  //
	  // (sigma_f/f)^2 = (sigma_num/num)^2 + (sigma_num/(1-num))^2 + 2 * sigma_num^2 * (1/num) * (1/(1-num))
	  // (sigma_f/f)^2 = sigma_num^2 * [1/num + 1/(1-num)]^2
	  // sigma_f = f * sigma_num * (1/(num * (1-num)))
	  // sigma_f = sigma_num * (1 - num)^{-2}
	  //
	  Float_t frError = FRerror * pow((1 - FRvalue), -2);
	  
	  // now that we have the error on fr/(1-fr), the error on the prediction is 
	  // just the sum of this error and the error on FO count added in quadrature
	  // (sigma_pred/pred)^2 = (sigma_fr/fr)^2 + (sigma_fo/fo)^2
	  Float_t pred_error = (nFOs > 0) ? pred * sqrt(pow(frError/fr, 2) + pow(nFOsError/nFOs, 2)) : 0.;
	  
	  // now increment the total values
	  if (pred > 0){
	    pred_total += pred;
	    pred_error_total += pow(pred_error, 2);	    
	  }

	}//exit FR bin loop
  
      //fill the pred array with new pred and errors
      preds[xbin+1]=pred_total;
      predErrors[xbin+1]=sqrt(pred_error_total);
      if (verbose) cout<<" --- Total for this MT2 bin: "<<pred_total<<endl;

      //cleanup
      h_sideband->Delete();
    }//exit mt2bin loop

  //make and fill prediction histogram
  f_out->cd();
  TString directory = "sr"+srName;
  TDirectory* dir = 0;
  dir = (TDirectory*)f_out->Get(directory.Data());
  if (dir == 0) {
    dir = f_out->mkdir(directory.Data());
  } 
  dir->cd();

  TH1F* h_pred = (TH1F*) h_LooseNotTight->Clone();
  h_pred->Reset();
  h_pred->SetName("h_predmt2bins"+s);
  h_pred->SetContent(preds);
  h_pred->SetError(predErrors);
  h_pred->Write();
  //h_pred->Print("all");
  return;
  
}

//this is all we need if the FR is a single number, thus we don't have to loop over 2-dimentional FR histograms
void makePredOneBinFR(TFile* f_out, TFile* f_in, TFile* f_qcd, TFile* f_gjet, TString srName, TH2D* h_FR, const float fragScale = 1, TString s = "", TString plotname = "mt2bins")
{
  
  //sr name
  bool realDataLocal = false;
  if (s.Contains("Data") || s.Contains("data")) realDataLocal = true;
  
  
  if (verbose) cout<<" ------ Looking at SR "<<srName<<", histogram "<<plotname<<". Fake rate histo has "<<h_FR->GetSize()<<" bins"<<endl;
  
  //Single bin or not
  TString bin = "";
  if(s.Contains("FailSieie")) bin = "SingleBin";
  
  //get LooseNotTight hist (this only contains Fakes, there are no Prompts, so no subtraction is needed for MC)
  TH1F* h_LooseNotTight = (TH1F*) f_in->Get("crgj"+srName+"/h_"+plotname+"LooseNotTight");
  
  //check for empty hists
  if(!h_FR){
    cout << "FR hist is null!" << endl;
    return;
  }
  if(!h_LooseNotTight){
    cout << "SR" << srName << " LooseNotTight hist is empty! Skipping signal region..." << endl;
    return;
  }
  

  int mt2bins_tmp = h_LooseNotTight->GetNbinsX();
  if (plotname=="mt2bins") mt2bins_tmp = 1; // use this to make an integrated estimate plot
  const int n_mt2bins = mt2bins_tmp;
  Double_t preds[n_mt2bins+2]; //first and last bin are over/underflow
  Double_t predErrors[n_mt2bins+2]; //first and last bin are over/underflow
  for(int i=0; i<n_mt2bins+2; i++){
    preds[i] = 0;
    predErrors[i] = 0;
  }
  
  //mt2bin names
  vector<TString> mt2binsname;
  for(int i=0; i<=n_mt2bins; i++){
    //    mt2binsname.push_back(toString(sr.GetMT2Bins()[i]));
    mt2binsname.push_back(toString(h_LooseNotTight->GetBinLowEdge(i+1)));
  }
  
  
  TH1F* h_sidebandqcdPrompt = (TH1F*) f_qcd->Get("crgj"+srName+"/h_"+plotname+"LooseNotTight");
  TH1F* h_sidebandgjetPrompt = (TH1F*) f_gjet->Get("crgj"+srName+"/h_"+plotname+"LooseNotTight");
  TH1F* h_sideband = (TH1F*) f_in->Get("crgj"+srName+"/h_"+plotname+"LooseNotTight");
  if (!realDataLocal) h_sideband = (TH1F*) f_in->Get("crgj"+srName+"/h_"+plotname+"FakeLooseNotTight");
  //h_sidebandqcdPrompt->Print("all");
  //h_sidebandgjetPrompt->Print("all");
  //h_sideband->Print("all");
  
  //if "Poisson", undo sumw2() errors
  if(s.Contains("Poisson")){
    //if(h_sidebandqcdPrompt) h_sidebandqcdPrompt->GetSumw2()->Set(0); //Prompt contamination always come from MC anyway, so weights are fine
    if(h_sideband) h_sideband->GetSumw2()->Set(0);
  }
  
  //skip this mt2bin if there are no fakes
  if(!h_sideband) {h_sideband = (TH1F*) h_LooseNotTight->Clone(); h_sideband->Reset();}
  
  // Subtract prompt contamination in sideband (when looking at data)
  if (h_sidebandgjetPrompt && realDataLocal) {
    h_sideband->Add(h_sidebandgjetPrompt, -1);
  }
  
  //loop over mt2bins
  for (Int_t xbin = 0; xbin < n_mt2bins; xbin++)
  {
    if (verbose) cout<<" --- Looking at bin "<<xbin<<" starting at "<<mt2binsname[xbin]<<endl;
    //initialize pred_totals
    Float_t pred_total       = 0.0;
    Float_t pred_error_total = 0.0;

    // end of prompt subtraction in sideband
    
    //loop over FR bins (only one bin in this case)
    for (Int_t FRbin = 4; FRbin < 5; FRbin++)
    {
      
      Float_t nFOs      = h_sideband->GetBinContent(xbin+1);    // number of denominator not numerator objects for this bin
      if (nFOs<0) nFOs = 0;
      Double_t nFOsError = h_sideband->GetBinError(xbin+1);      // number error on number of of denominator not numerator objects for this bin
      
      if (plotname=="mt2bins") {
        nFOs = h_sideband->IntegralAndError(0, -1, nFOsError);
      }
      
      Float_t FRvalue   = h_FR->GetBinContent(FRbin); // get value of fake rate for this bin
      Float_t FRerror   = h_FR->GetBinError(FRbin);   // get error on fake rate for this bin
      
      //shift nFOs by fragScale*QCDPrompt in LooseNotTight. (add fragmentation to sideband, unless looking at data)
      if(h_sidebandqcdPrompt && !realDataLocal) {
        float fragYield = h_sidebandqcdPrompt->GetBinContent(xbin);
        if (plotname=="mt2bins") fragYield = h_sidebandqcdPrompt->Integral();
        if (fragYield > 0) {
          nFOs = nFOs + fragScale*fragYield;
        }
      }
      
      //--- error propagation shamelessly stolen from frank golfs code---
      
      // ok, have all of the inputs, calculate prediction and error for this bin
      // start with calculating fr/(1-fr) for this bin
      const Float_t fr = FRvalue / (1 - FRvalue);
      const Float_t pred = fr * nFOs;
      
      if (verbose ) cout<<"FR bin "<<FRbin<<" has FR "<<FRvalue<<", nFO "<<nFOs<<" and predicted "<<pred<<endl;
      
      // now, need to get errors on these terms, be careful
      // start with hardest part, error on fr
      //
      // start with basic equation: (sigma_f/f)^2 = (sigma_num/num)^2 + (sigma_den/den)^2 - 2 * (sigma_num/num) * (sigma_den/den) * rho_num_den
      // f is the value fr from above
      // num is the value FRvalue from above
      // den is 1 - FRvalue
      //
      // sigma_num is the value FRerror from above
      // sigma_den is also the value FRerror from above (the error on 1 - A is just the error on A)
      //
      // the numerator and denominator are completely anti-correlated, so rho_num_den is -1
      //
      // combining all of this we can simplify the above to be
      //
      // (sigma_f/f)^2 = (sigma_num/num)^2 + (sigma_num/(1-num))^2 + 2 * sigma_num^2 * (1/num) * (1/(1-num))
      // (sigma_f/f)^2 = sigma_num^2 * [1/num + 1/(1-num)]^2
      // sigma_f = f * sigma_num * (1/(num * (1-num)))
      // sigma_f = sigma_num * (1 - num)^{-2}
      //
      Float_t frError = FRerror * pow((1 - FRvalue), -2);
      
      // now that we have the error on fr/(1-fr), the error on the prediction is
      // just the sum of this error and the error on FO count added in quadrature
      // (sigma_pred/pred)^2 = (sigma_fr/fr)^2 + (sigma_fo/fo)^2
      Float_t pred_error = (nFOs > 0) ? pred * sqrt(pow(frError/fr, 2) + pow(nFOsError/nFOs, 2)) : 0.;
      
      // now increment the total values
      if (pred > 0){
        pred_total += pred;
        pred_error_total += pow(pred_error, 2);
      }
      
    }//exit FR bin loop
    
    //fill the pred array with new pred and errors
    preds[xbin+1]=pred_total;
    predErrors[xbin+1]=sqrt(pred_error_total);
    if (verbose) cout<<" --- Total fakes for this bin: "<<pred_total<<endl;

  }//exit mt2bin loop
  
  //cleanup
  //h_sideband->Delete();
  //make and fill prediction histogram
  f_out->cd();
  TString directory = "sr"+srName;
  TDirectory* dir = 0;
  dir = (TDirectory*)f_out->Get(directory.Data());
  if (dir == 0) {
    dir = f_out->mkdir(directory.Data());
  }
  dir->cd();
  
  TH1F* h_pred = (TH1F*) h_LooseNotTight->Clone();
  h_pred->Reset();
  if (plotname=="mt2bins") {
    h_pred->SetName("h_INTpred"+plotname+s);
    h_pred->SetBinContent(1, preds[1]);
    h_pred->SetBinError(1,predErrors[1]);
  }
  else {
    h_pred->SetName("h_pred"+plotname+s);
    h_pred->SetContent(preds);
    h_pred->SetError(predErrors);
  }
  
  h_pred->Write();
  
  return;
  
}


void purityPlotsNew(TFile* f_out, TFile* f_data, TFile* f_gjet, TFile* f_qcd, TFile* f_zinv, TFile* f_zOrig, TString sr, TString FR_type, TString plotname = "mt2bins")

{
  if (verbose) cout<<__LINE__<<" Making plots for region "<<sr<<", FR_type "<<FR_type<<", plotname "<<plotname<<" **************************"<<endl;
  //get hists
  TH1F* h_gjet = (TH1F*) f_gjet->Get("crgj"+sr+"/h_"+plotname);
  TH1F* h_qcd  = (TH1F*) f_qcd->Get("crgj"+sr+"/h_"+plotname);
  TH1F* h_tmp  = (TH1F*) f_data->Get("crgj"+sr+"/h_"+plotname);
  if(!h_gjet || !h_tmp) return;
  TH1F* h_full  = (TH1F*) f_data->Get("crgj"+sr+"/h_"+plotname)->Clone();
  //check existence
  TH1F* h_qcdFake = (TH1F*) f_qcd->Get("crgj"+sr+"/h_"+plotname+"Fake");
  if (!FR_type.Contains("Data") && h_qcdFake) {
    h_full->Add(h_qcdFake); // need to add MC fakes to our total, if running on MC.
  }
  // now h_full includes purity and f

  TH1F* h_fullLooseNotTight = (TH1F*) f_data->Get("crgj"+sr+"/h_"+plotname+"FakeLooseNotTight");

  if (FR_type.Contains("Data") ) {
    h_fullLooseNotTight = (TH1F*) f_data->Get("crgj"+sr+"/h_"+plotname+"LooseNotTight"); // Can't have "Fake" in the name, if running on data
  }
  
  TString srdir = "sr"+sr;
  TH1F* h_predFakes = (TH1F*) f_out->Get(srdir+"/h_pred"+plotname+FR_type);
//  if (plotname.Contains("mt2bins")) {
//    h_predFakes = (TH1F*) f_out->Get(srdir+"/h_pred"+FR_type);
//  }
  TH1F* h_ratio = (TH1F*) f_zinv->Get(srdir+"/h_"+plotname+"Ratio");
  if (!plotname.Contains("mt2bins")) {
    h_ratio = (TH1F*) f_zinv->Get("h_"+plotname+"Ratio");
  }
  if (!h_ratio) {cout<<"Ratio not found for plot. GIVING UP FOR THIS REGION. "<<plotname<<endl; return; }
  TH1F* h_trueZinv = (TH1F*) f_zinv->Get(srdir+"/h_"+plotname);
  bool foundZinv = true;
  if (!h_trueZinv) {
    cout<<"Forget about Zinv here, could not find the plot. Do the rest."<<endl;
    foundZinv = false;
  }


  if (verbose && h_gjet) cout<<__LINE__<<" f_gjet:crgj"<<sr<<"/h_"<<plotname<<" has integral "<<h_gjet->Integral()<<endl;
  if (verbose && h_gjet) cout<<__LINE__<<" f_full:crgj"<<sr<<"/h_"<<plotname<<" has integral "<<h_full->Integral()<<endl;
  if (verbose && h_fullLooseNotTight) cout<<__LINE__<<" h_fullLooseNotTight has integral "<<h_fullLooseNotTight->Integral()<<endl;
  if (verbose && h_gjet && foundZinv) cout<<__LINE__<<" f_zinv:sr"<<sr<<"/h_"<<plotname<<" has integral "<<h_trueZinv->Integral()<<endl;
  if (verbose && h_gjet) cout<<__LINE__<<" f_zinv:sr"<<sr<<" ratio is "<<endl; h_ratio->GetBinContent(1);
  
  f_out->cd();
  TString directory = "sr"+sr;
  TDirectory* dir = 0;
  dir = (TDirectory*)f_out->Get(directory.Data());
  if (dir == 0) {
    dir = f_out->mkdir(directory.Data());
  }
  dir->cd();
  
  //do True purity: gjet+fragQCD / gjet+fragQCD+fakeQCD
  TH1F* h_numTrue = (TH1F*) h_gjet->Clone("h_numTrue"); h_numTrue->SetName("h_numTrue");
  TH1F* h_denTrue = (TH1F*) h_gjet->Clone("h_denTrue"); h_denTrue->SetName("h_denTrue");
  TH1F* h_purityTrue = (TH1F*) h_gjet->Clone("h_"+plotname+"purityTrue"); h_purityTrue->SetName("h_"+plotname+"purityTrue");
  TH1F* h_purityTrueInt = new TH1F("h_"+plotname+"purityTrueInt"+FR_type,"h_"+plotname+"purityTrueInt"+FR_type, 1, 0, 1);
  TH1F* h_numTrueInt = (TH1F*)h_purityTrueInt->Clone("h_"+plotname+"numTrueInt");
  TH1F* h_denTrueInt = (TH1F*)h_purityTrueInt->Clone("h_"+plotname+"denTrueInt");
  if(h_qcd) h_numTrue->Add(h_qcd);
  if(h_qcd) h_denTrue->Add(h_qcd);
  if(h_qcdFake) h_denTrue->Add(h_qcdFake);
  h_purityTrue->Divide(h_numTrue,h_denTrue,1,1,"B");

  double errnum, errden;
  h_numTrueInt->SetBinContent(1, h_numTrue->IntegralAndError(0, -1, errnum) );
  h_numTrueInt->SetBinError(1, errnum );
  h_denTrueInt->SetBinContent(1, h_denTrue->IntegralAndError(0, -1, errden) );
  h_denTrueInt->SetBinError(1, errden );
  h_purityTrueInt->Divide(h_numTrueInt,h_denTrueInt,1,1,"B");

  
  //do FR purity: full - predFakes / full
  TH1F* h_numFR = (TH1F*) h_full->Clone("h_numFR"); h_numFR->SetName("h_numFR");
  TH1F* h_denFR = (TH1F*) h_full->Clone("h_denFR"); h_denFR->SetName("h_mt2bins"); // this is our "DATA", later picked up by cardMaker!!
  TH1F* h_purityFR = (TH1F*) h_full->Clone("h_"+plotname+"purity"+FR_type); h_purityFR->SetName("h_"+plotname+"purity"+FR_type);
  if (FR_type.Contains("Poisson")) {
    h_numFR->GetSumw2()->Set(0); h_numFR->Sumw2(); // reset sumw2
    h_denFR->GetSumw2()->Set(0); h_denFR->Sumw2(); // reset sumw2
  }
  if(h_predFakes) h_numFR->Add(h_predFakes, -1.);
  h_purityFR->Divide(h_numFR,h_denFR,1,1,"B");
  cout<<"CHECKING PURITY FOR "<<h_purityFR->GetName()<<endl;
  if (h_predFakes) h_predFakes->Print("all");
  else cout<<"h_predFakes not found"<<endl;
  h_numFR->Print("all");
  h_denFR->Print("all");
  h_purityFR->Print("all");
//  // add systematic uncertainty to purity if estimated from Sieie sideband
//  for (int ibin = 1; ibin <= h_purityFR->GetNbinsX(); ibin++) {
//    float fivepercent = 0.05*h_purityFR->GetBinContent(ibin);
//    float err = h_purityFR->GetBinError(ibin);OA
//    h_purityFR->SetBinError(ibin, TMath::Sqrt(err*err + fivepercent*fivepercent) );
//  }
  
  TH1F* h_purityInt = new TH1F("h_"+plotname+"purityInt"+FR_type,"h_"+plotname+"purityInt"+FR_type, 1, 0, 1);
  TH1F* h_numInt = (TH1F*)h_purityInt->Clone("h_"+plotname+"numInt"+FR_type);
  TH1F* h_denInt = (TH1F*)h_purityInt->Clone("h_"+plotname+"denInt"+FR_type);
  h_numInt->SetBinContent(1, h_numFR->IntegralAndError(0, -1, errnum) );
  h_numInt->SetBinError(1, errnum );
  h_denInt->SetBinContent(1, h_denFR->IntegralAndError(0, -1, errden) );
  h_denInt->SetBinError(1, errden );
  h_purityInt->Divide(h_numInt,h_denInt,1,1,"B");
//  for (int ibin = 1; ibin <= h_purityInt->GetNbinsX(); ibin++) {
//    float fivepercent = 0.05*h_purityInt->GetBinContent(ibin);
//    float err = h_purityInt->GetBinError(ibin);
//    h_purityInt->SetBinError(ibin, TMath::Sqrt(err*err + fivepercent*fivepercent) );
//  }
  
  // estimate of photon yield = (full - predFakes)*f
  TH1F* h_estimate = (TH1F*) h_numFR->Clone("h_"+plotname+"photonestimate"); h_estimate->SetName("h_"+plotname+"photonestimate"+FR_type);
  h_estimate->Scale(0.92);
  // add 8% error for f, 5% for p
  for (int ibin = 1; ibin <= h_estimate->GetNbinsX(); ibin++) {
    float eightpercent = 0.08*h_estimate->GetBinContent(ibin);
    float fivepercent = 0.05*h_estimate->GetBinContent(ibin);
    float err = h_estimate->GetBinError(ibin);
    h_estimate->SetBinError(ibin, TMath::Sqrt(err*err + eightpercent*eightpercent + fivepercent*fivepercent) );
  }
  
  dir->cd();
  
  //make zinv pred, predZ = estimate * R(Z/Gamma)
  TH1F* h_predZ = (TH1F*) h_estimate->Clone("h_"+plotname+"predZ"+FR_type);
  h_predZ->SetName("h_"+plotname+"predZ"+FR_type);
  if (h_ratio) h_predZ->Multiply(h_predZ,h_ratio,1,1,"B");
  h_predZ->Scale(0.95); // correction based on double-ratio
  
  // tricky, let's add some R(Z/Gamma) uncertainties, which are the only ones missing at this point
  // most of this code is taken from cardMaker.C
  // for integrated estimate, will need a 20% shape uncertainty on all bins except first, do this later.
  TH1F* h_predZwithUncInt = (TH1F*) h_predZ->Clone("h_"+plotname+"predZ"+FR_type+"withRuncInt");
  TH1F* h_predZwithUncBin = (TH1F*) h_predZ->Clone("h_"+plotname+"predZ"+FR_type+"withRuncBin");
  h_predZwithUncBin->Print("all");
  if (plotname.Contains("mt2bins") ) {
    TH1D* h_ht_LOW     = (TH1D*) f_data->Get(srdir+"/h_ht_LOW");
    TH1D* h_nbjets_LOW = (TH1D*) f_data->Get(srdir+"/h_nbjets_LOW");
    TH1D* h_njets_LOW  = (TH1D*) f_data->Get(srdir+"/h_njets_LOW");
    if (h_nbjets_LOW && h_njets_LOW ) {
      int ht_LOW = h_ht_LOW ? h_ht_LOW->GetBinContent(1) : 200;
      int nbjets_LOW = h_nbjets_LOW->GetBinContent(1);
      int njets_LOW = h_njets_LOW->GetBinContent(1);
      TH1D* h_zllgamma_nj  = (TH1D*) f_zinv->Get("h_njbinsRatio");
      TH1D* h_zllgamma_nb  = (TH1D*) f_zinv->Get("h_nbjbinsRatio");
      TH1D* h_zllgamma_ht  = (TH1D*) f_zinv->Get("h_htbinsRatio");
      TH1D* h_zllgamma_mt2 = (TH1D*) f_zinv->Get("h_mt2binsRatio");
      if (h_zllgamma_nj == 0 || h_zllgamma_nb == 0 || h_zllgamma_ht == 0 || h_zllgamma_mt2 == 0) {
        cout<<"Trying fourNuisancesPerBinZGratio, but could not find inclusive Zll/Gamma ratio plots for nuisance parameters"<<endl;
      }
      else {
        int bin_nj   = h_zllgamma_nj ->FindBin(njets_LOW + 0.5);
        int bin_nb   = h_zllgamma_nb ->FindBin(nbjets_LOW + 0.5);
        int bin_ht   = h_zllgamma_ht ->FindBin(ht_LOW + 1);
        //if (srdir.Contains("J")) {bin_nj = 1; bin_nb = 1; bin_ht = 1;}
        // (set to 100% if the ratio doesn't exist)
        float zllgamma_nj  = h_zllgamma_nj ->GetBinContent( bin_nj  ) > 0 ? h_zllgamma_nj ->GetBinError( bin_nj  ) / h_zllgamma_nj ->GetBinContent( bin_nj  ) : 1.;
        float zllgamma_nb  = h_zllgamma_nb ->GetBinContent( bin_nb  ) > 0 ? h_zllgamma_nb ->GetBinError( bin_nb  ) / h_zllgamma_nb ->GetBinContent( bin_nb  ) : 1.;
        float zllgamma_ht  = h_zllgamma_ht ->GetBinContent( bin_ht  ) > 0 ? h_zllgamma_ht ->GetBinError( bin_ht  ) / h_zllgamma_ht ->GetBinContent( bin_ht  ) : 1.;
        float uncInt = TMath::Sqrt(zllgamma_nj*zllgamma_nj + zllgamma_nb*zllgamma_nb + zllgamma_ht*zllgamma_ht);
        if (uncInt > 1) uncInt = 1;
        for (int ibin = 1; ibin <= h_predZwithUncInt->GetNbinsX(); ibin++) {
          int mt2_LOW = h_predZwithUncInt->GetBinLowEdge(ibin);
          int bin_mt2  = h_zllgamma_mt2->FindBin(mt2_LOW + 1);
          if (bin_mt2==0) bin_mt2 = 1;
          float zllgamma_mt2 = h_zllgamma_mt2->GetBinContent( bin_mt2 ) > 0 ? h_zllgamma_mt2->GetBinError( bin_mt2 ) / h_zllgamma_mt2->GetBinContent( bin_mt2 ) : 1.;
          cout<<"Uncertainties are: "<<zllgamma_nj<<" "<<zllgamma_nb<<" "<<zllgamma_ht<<" "<<zllgamma_mt2<<". mt2LOW "<<mt2_LOW<<", bin_mt2 "<<bin_mt2<<endl;
          float uncBin = TMath::Sqrt( uncInt*uncInt + zllgamma_mt2*zllgamma_mt2 );
          if (uncBin > 1) uncBin = 1;
          float errInt = h_predZwithUncInt->GetBinError(ibin);
          float errBin = h_predZwithUncBin->GetBinError(ibin);
          float RzgErrorInt = uncInt*h_predZwithUncInt->GetBinContent(ibin);
          float RzgErrorBin = uncBin*h_predZwithUncBin->GetBinContent(ibin);
          h_predZwithUncInt->SetBinError(ibin, TMath::Sqrt(errInt*errInt + RzgErrorInt*RzgErrorInt) );
          h_predZwithUncBin->SetBinError(ibin, TMath::Sqrt(errBin*errBin + RzgErrorBin*RzgErrorBin) );
          if (h_predZwithUncInt->GetBinError(ibin) > h_predZwithUncInt->GetBinContent(ibin)) h_predZwithUncInt->SetBinError(ibin, h_predZwithUncInt->GetBinContent(ibin));
          if (h_predZwithUncBin->GetBinError(ibin) > h_predZwithUncBin->GetBinContent(ibin)) h_predZwithUncBin->SetBinError(ibin, h_predZwithUncBin->GetBinContent(ibin));
        }
      
      }
    }
    else {cout<<"Could not find h_ht_LOW histogram in "<<srdir<<". Will not make h_"<<plotname<<"predZwithRunc"<<endl;}
    
  }// end of R(Z/Gamma) uncertainties
  h_predZwithUncBin->Print("all");

  // add fine-binned MT2 histograms for macro plots
  if (TString(h_predZwithUncInt->GetName()).Contains("mt2binspredZFailSieieDatawithRuncInt")){
    TH1F* h_trueZinv = (TH1F*) f_zOrig->Get(srdir+"/h_mt2");
    if (!h_trueZinv) { cout<<"could not find "<<srdir<<"/h_mt2. Exiting... "<<endl; return; }
    if (h_trueZinv->Integral(0, -1) == 0) { cout<<"integral of "<<srdir<<"/h_mt2 is 0. Exiting... "<<endl; return; }
    TH1F* h_trueZinvScaled = (TH1F*) h_trueZinv->Clone("h_mt2");
    double interr;
    float newNorm = h_predZwithUncInt->IntegralAndError(0, -1, interr);
    h_trueZinvScaled->Scale( newNorm / h_trueZinv->Integral(0, -1) );
    for (int ibin = 1; ibin <= h_trueZinvScaled->GetNbinsX(); ibin++) {
      float olderr = h_trueZinvScaled->GetBinError(ibin);
      float newerr = interr / newNorm * h_trueZinvScaled->GetBinContent(ibin);
      h_trueZinvScaled->SetBinError(ibin, sqrt(olderr*olderr + newerr*newerr));
    }
    
    h_trueZinvScaled->Write();
  }

  
  //write hists to output file
  h_purityFR->Write();
  h_purityInt->Write();
  h_estimate->Write();
  h_predZ->Write();
  h_predZwithUncInt->Write();
  h_predZwithUncBin->Write();
  if (!FR_type.Contains("Data") && !FR_type.Contains("FailSieie") ) {
    h_purityTrue->Write();
    h_purityTrueInt->Write();
  }
  if (FR_type.Contains("Data") ) {
    h_denFR->Write();
  }
  
  if (verbose) {
    cout<<__LINE__<<"  h_purityTrue              " <<      h_purityTrue		    ->GetName()<<" "<<  h_purityTrue		 ->GetBinContent(1)<<endl;
    cout<<__LINE__<<"  h_purityFR		 " << 	   h_purityFR		    ->GetName()<<" "<<  h_purityFR		 ->GetBinContent(1)<<endl;
    if (h_predFakes) cout<<__LINE__<<"  h_predFakes  		 " << 	   h_predFakes		    ->GetName()<<" "<<  h_predFakes		 ->Integral()<<endl;
    cout<<__LINE__<<"  h_estimate  " << 	   h_estimate ->GetName()<<" "<<  h_estimate ->Integral()<<endl;
    cout<<__LINE__<<"  h_denFR		 " << 	   h_denFR		    ->GetName()<<" "<<  h_denFR		 ->Integral()<<endl;
    cout<<__LINE__<<"  h_predZ                   " << 	   h_predZ                  ->GetName()<<" "<<  h_predZ                  ->Integral()<<endl;
  }
  
  //write raw numbers to output file
  ofstream purityLog;
  purityLog.open("purity.log", ios::app);
  purityLog << "---------------"<<FR_type<<"---------------" << endl;
  purityLog << "Signal Region " << sr << endl;
  for (int i = 1; i<= h_gjet->GetNbinsX(); i++){
    purityLog << h_gjet->GetBinLowEdge(i) << " < MT2 < " << h_gjet->GetBinLowEdge(i+1) << endl;
    if(h_qcdFake) purityLog << "Ngamma (tight): " << h_qcdFake->GetBinContent(i) << endl;
    if(h_fullLooseNotTight) purityLog << "Ngamma (loose not tight): " << h_fullLooseNotTight->GetBinContent(i) << endl;
    if(h_purityFR) purityLog << "Purity: " << h_purityFR->GetBinContent(i) << " +/- " << h_purityFR->GetBinError(i) << endl;
    purityLog << endl;
  }
  purityLog << "------------------------------" << endl;
  purityLog.close();
  
  f_out->cd();
  return;
}


void purity(string input_dir = "/home/users/gzevi/MT2/MT2Analysis/MT2looper/output/V00-00-11skim/", string dataname = "data")
{
  
  //load signal regions
  vector<SR> SRVec =  getSignalRegionsJamboree();
  vector<SR> SRVec2 =  getSignalRegionsMonojet();

  //open files
  // get input files -- default to faking data with same MC file
  TString datanamestring(dataname);
  if (datanamestring.Contains("Data") || datanamestring.Contains("data")) realData = true;
  TFile* f_data = new TFile(Form("%s/%s.root",input_dir.c_str(),dataname.c_str())); //data or qcd+gjets file
  TFile* f_gq = new TFile(Form("%s/qcdplusgjet.root",input_dir.c_str())); //qcd+gjets file
  TFile* f_g = new TFile(Form("%s/gjets_dr0p05_ht.root",input_dir.c_str())); //gjet file
  TFile* f_q = new TFile(Form("%s/qcd_ht.root",input_dir.c_str())); //qcd file
  TFile* f_z = new TFile(Form("%s/zinvFromGJ.root",input_dir.c_str())); //zinv pred from ZinvMaker.C, contains ratio
  TFile* f_zOrig = new TFile(Form("%s/zinv_ht.root",input_dir.c_str())); //zinv file out of the box
  if(f_g->IsZombie() || f_q->IsZombie() || f_gq->IsZombie() || f_data->IsZombie() || f_z->IsZombie()) {
    std::cerr << "Input file does not exist" << std::endl;
    return;
  }

  cout << "Making Fake-Rate Histograms..." << endl;
  
  //get hists for FR calc
  TH2D* h_qcdTight = (TH2D*) f_q->Get("crgjbase/h2d_gammaht_gammaptFake");
  TH2D* h_qcdLoose = (TH2D*) f_q->Get("crgjbase/h2d_gammaht_gammaptFakeLoose");
  if (!h_qcdTight || !h_qcdLoose) cout<<"Could not find FR histograms in QCD MC"<<endl;
  h_qcdTight->SetName("h_qcdTight");
  h_qcdLoose->SetName("h_qcdLoose");
  h_qcdTight->ClearUnderflowAndOverflow();
  h_qcdLoose->ClearUnderflowAndOverflow();
  
  //get hists for FR calc, Sieie Sideband
  TH2D* h_qcdTightFailSieie = (TH2D*) f_q->Get("crgjbase/h2d_gammaht_gammaptSingleBinFakeSieieSB");
  TH2D* h_qcdLooseFailSieie = (TH2D*) f_q->Get("crgjbase/h2d_gammaht_gammaptSingleBinFakeLooseSieieSB");
  if (!h_qcdTightFailSieie || !h_qcdLooseFailSieie) cout<<"Could not find SieieSB FR histograms in QCD MC"<<endl;
  h_qcdTightFailSieie->SetName("h_qcdTightFailSieie");
  h_qcdLooseFailSieie->SetName("h_qcdLooseFailSieie");
  h_qcdTightFailSieie->ClearUnderflowAndOverflow();
  h_qcdLooseFailSieie->ClearUnderflowAndOverflow();
  
  //get hists for FR calc, Sieie Sideband (Data)
  TH2D* h_qcdTightFailSieieData = (TH2D*) f_data->Get("crgjbase/h2d_gammaht_gammaptSingleBinSieieSB");
  TH2D* h_qcdLooseFailSieieData = (TH2D*) f_data->Get("crgjbase/h2d_gammaht_gammaptSingleBinLooseSieieSB");
  if (!realData) {
    h_qcdTightFailSieieData = (TH2D*) f_data->Get("crgjbase/h2d_gammaht_gammaptSingleBinFakeSieieSB");
    h_qcdLooseFailSieieData = (TH2D*) f_data->Get("crgjbase/h2d_gammaht_gammaptSingleBinFakeLooseSieieSB");
  }
  if (!h_qcdTightFailSieieData || !h_qcdTightFailSieieData) cout<<"Could not find SieieSB FR histograms in (pseudo)data"<<endl;

  h_qcdTightFailSieieData->SetName("h_qcdTightFailSieieData");
  h_qcdLooseFailSieieData->SetName("h_qcdLooseFailSieieData");
  h_qcdTightFailSieieData->ClearUnderflowAndOverflow();
  h_qcdLooseFailSieieData->ClearUnderflowAndOverflow();  

  //instantiate output file here
  //TFile* f_out = new TFile("$CMSSW_BASE/../scripts/purity.root","RECREATE");
  TFile* f_out = new TFile(Form("%s/purity.root",input_dir.c_str()),"RECREATE");
  //FR histograms
  TH2D* h_FR = (TH2D*) h_qcdTight->Clone();
  h_FR->Reset();
  TH2D* h_FRFailSieie = (TH2D*) h_qcdTightFailSieie->Clone();
  h_FRFailSieie->Reset();
  TH2D* h_FRFailSieieData = (TH2D*) h_qcdTightFailSieieData->Clone();
  h_FRFailSieieData->Reset();
  
  //regular FR
  h_FR->Divide(h_qcdTight,h_qcdLoose,1,1,"B");  
  h_FR->SetName("h_FR");
  h_FR->SetTitle(h_qcdTight->GetTitle());
  h_FR->Write();

  //FR in Sieie SB
  h_FRFailSieie->Divide(h_qcdTightFailSieie,h_qcdLooseFailSieie,1,1,"B");  
  h_FRFailSieie->SetName("h_FRFailSieie");
  h_FRFailSieie->SetTitle(h_qcdTightFailSieie->GetTitle());
  h_FRFailSieie->Write();
  
  //FR in Sieie SB (Data)
  h_FRFailSieieData->Divide(h_qcdTightFailSieieData,h_qcdLooseFailSieieData,1,1,"B");
  h_FRFailSieieData->SetName("h_FRFailSieieData");
  h_FRFailSieieData->SetTitle(h_qcdTightFailSieieData->GetTitle());
  h_FRFailSieieData->Write();

  //make the prediction hists
  cout << "Making predicted yield histograms..." << endl;
  for(int i = 0; i< (int) SRVec.size(); i++){
    TString srName = SRVec[i].GetName();
    makePred(f_out, f_gq, f_q, f_g, srName, h_FR, 0, ""); //FR using passSieie, LooseNotTight Fakes + 0 qcdPrompt
    //makePred(f_out, f_data, f_q, f_g, SRVec[i], h_FR, 0.5, "plus50"); //FR using passSieie, LooseNotTight Fakes + +50% qcdPrompt
    //makePred(f_out, f_data, f_q, f_g, SRVec[i], h_FR, -0.5, "minus50"); //FR using passSieie, LooseNotTight Fakes + -50% qcdPrompt
    makePred(f_out, f_gq, f_q, f_g, srName, h_FRFailSieie, 0, "FailSieie"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt
    //makePred(f_out, f_data, f_q, f_g, SRVec[i], h_FRFailSieie, 0, "FailSieiePoisson"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt, Poisson errors
    makePred(f_out, f_data, f_q, f_g, srName, h_FRFailSieieData, 0, "FailSieieData"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt, Data
    //makePred(f_out, f_data, f_q, f_g, SRVec[i], h_FR, 0, "Poisson"); //FR using passSieie, LooseNotTight Fakes + 0 qcdPrompt, Poisson errors
  }
  for(int i = 0; i< (int) SRVec2.size(); i++){
    TString srName = SRVec2[i].GetName();
    makePred(f_out, f_gq, f_q, f_g, srName, h_FR, 0, ""); //FR using passSieie, LooseNotTight Fakes + 0 qcdPrompt
    makePred(f_out, f_gq, f_q, f_g, srName, h_FRFailSieie, 0, "FailSieie"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt
    makePred(f_out, f_data, f_q, f_g, srName, h_FRFailSieieData, 0, "FailSieieData"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt, Data
  }
  TString srName = "base";
  makePred(f_out, f_gq, f_q, f_g, srName, h_FR, 0, ""); //FR using passSieie, LooseNotTight Fakes + 0 qcdPrompt
  makePred(f_out, f_gq, f_q, f_g, srName, h_FRFailSieie, 0, "FailSieie"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt
  makePred(f_out, f_data, f_q, f_g, srName, h_FRFailSieieData, 0, "FailSieieData"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt, Data
  makePredOneBinFR(f_out, f_data, f_q, f_g, srName, h_FRFailSieieData, 0, "FailSieieData", "htbins"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt, Data
  makePredOneBinFR(f_out, f_data, f_q, f_g, srName, h_FRFailSieieData, 0, "FailSieieData", "htbins2"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt, Data
  makePredOneBinFR(f_out, f_data, f_q, f_g, srName, h_FRFailSieieData, 0, "FailSieieData", "njbins"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt, Data
  makePredOneBinFR(f_out, f_data, f_q, f_g, srName, h_FRFailSieieData, 0, "FailSieieData", "nbjbins"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt, Data
  makePredOneBinFR(f_out, f_data, f_q, f_g, srName+"Incl", h_FRFailSieieData, 0, "FailSieieData", "htbins"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt, Data
  makePredOneBinFR(f_out, f_data, f_q, f_g, srName+"Incl", h_FRFailSieieData, 0, "FailSieieData", "htbins2"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt, Data
  makePredOneBinFR(f_out, f_data, f_q, f_g, srName+"Incl", h_FRFailSieieData, 0, "FailSieieData", "njbins"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt, Data
  makePredOneBinFR(f_out, f_data, f_q, f_g, srName+"Incl", h_FRFailSieieData, 0, "FailSieieData", "nbjbins"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt, Data
  makePredOneBinFR(f_out, f_data, f_q, f_g, srName+"Incl", h_FRFailSieieData, 0, "FailSieieData", "bosonptbins"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt, Data
  makePredOneBinFR(f_out, f_gq, f_q, f_g, srName, h_FRFailSieie, 0, "FailSieie", "htbins"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt, Data
  makePredOneBinFR(f_out, f_gq, f_q, f_g, srName, h_FRFailSieie, 0, "FailSieie", "htbins2"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt, Data
  makePredOneBinFR(f_out, f_gq, f_q, f_g, srName, h_FRFailSieie, 0, "FailSieie", "njbins"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt, Data
  makePredOneBinFR(f_out, f_gq, f_q, f_g, srName, h_FRFailSieie, 0, "FailSieie", "nbjbins"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt, Data
  makePredOneBinFR(f_out, f_gq, f_q, f_g, srName+"Incl", h_FRFailSieie, 0, "FailSieie", "htbins"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt, Data
  makePredOneBinFR(f_out, f_gq, f_q, f_g, srName+"Incl", h_FRFailSieie, 0, "FailSieie", "htbins2"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt, Data
  makePredOneBinFR(f_out, f_gq, f_q, f_g, srName+"Incl", h_FRFailSieie, 0, "FailSieie", "njbins"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt, Data
  makePredOneBinFR(f_out, f_gq, f_q, f_g, srName+"Incl", h_FRFailSieie, 0, "FailSieie", "nbjbins"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt, Data
  makePredOneBinFR(f_out, f_gq, f_q, f_g, srName+"Incl", h_FRFailSieie, 0, "FailSieie", "bosonptbins"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt, Data
  vector<TString> additionalRegions;
  additionalRegions.push_back("baseJ");
  additionalRegions.push_back("baseVL");
  additionalRegions.push_back("baseL");
  additionalRegions.push_back("baseM");
  additionalRegions.push_back("baseH");
  additionalRegions.push_back("baseUH");
  additionalRegions.push_back("baseIncl");
  for(int i = 0; i< (int) additionalRegions.size(); i++){
   srName = additionalRegions.at(i);
    makePred(f_out, f_gq, f_q, f_g, srName, h_FR, 0, ""); //FR using passSieie, LooseNotTight Fakes + 0 qcdPrompt
    makePred(f_out, f_gq, f_q, f_g, srName, h_FRFailSieie, 0, "FailSieie"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt
    makePred(f_out, f_data, f_q, f_g, srName, h_FRFailSieieData, 0, "FailSieieData"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt, Data
  }

  //make purity plots
  //first delete log file in case it already exists
  if( remove( "purity.log" ) != 0 )
    perror( "Error deleting purity.log" );
  else
    puts( "Deleting old log file..." );
  cout << "Making purity histograms..." << endl;
  for(int i = 0; i< (int) SRVec.size(); i++){
    TString srName = SRVec[i].GetName();
//    purityPlots(f_out, f_g, f_q, f_z, SRVec[i].GetName());
    purityPlotsNew(f_out, f_gq, f_g, f_q, f_z, f_zOrig, srName, "");
    purityPlotsNew(f_out, f_data, f_g, f_q, f_z, f_zOrig, srName, "FailSieieData"); // This needs to be done last (it overwrites previous histograms)
  }
  for(int i = 0; i< (int) SRVec2.size(); i++){
    TString srName = SRVec2[i].GetName();
    //    purityPlots(f_out, f_g, f_q, f_z, SRVec[i].GetName());
    purityPlotsNew(f_out, f_gq, f_g, f_q, f_z, f_zOrig, srName, "");
    purityPlotsNew(f_out, f_data, f_g, f_q, f_z, f_zOrig, srName, "FailSieieData"); // This needs to be done last (it overwrites previous histograms)
  }
  additionalRegions.clear();
  additionalRegions.push_back("base");
  additionalRegions.push_back("baseJ");
  additionalRegions.push_back("baseVL");
  additionalRegions.push_back("baseL");
  additionalRegions.push_back("baseM");
  additionalRegions.push_back("baseH");
  additionalRegions.push_back("baseUH");
  additionalRegions.push_back("baseIncl");
  for(int i = 0; i< (int) additionalRegions.size(); i++){
    srName = additionalRegions.at(i);
    purityPlotsNew(f_out, f_gq, f_g, f_q, f_z, f_zOrig, srName, "");
    purityPlotsNew(f_out, f_gq, f_g, f_q, f_z, f_zOrig, srName, "FailSieie"); // This needs to be done last (it overwrites previous histograms)
    purityPlotsNew(f_out, f_data, f_g, f_q, f_z, f_zOrig, srName, "FailSieieData"); // This needs to be done last (it overwrites previous histograms)
  }
  vector<TString> additionalPlots;
  additionalPlots.push_back("htbins");
  additionalPlots.push_back("htbins2");
  additionalPlots.push_back("njbins");
  additionalPlots.push_back("nbjbins");
  //  additionalPlots.push_back("bosonptbins");
  for(int i = 0; i< (int) additionalPlots.size(); i++){
    srName = "base";
    TString plot = additionalPlots.at(i);
    purityPlotsNew(f_out, f_gq, f_g, f_q, f_z, f_zOrig, srName, "", plot);
    purityPlotsNew(f_out, f_gq, f_g, f_q, f_z, f_zOrig, srName, "FailSieie", plot); // This needs to be done last (it overwrites previous histograms)
    purityPlotsNew(f_out, f_data, f_g, f_q, f_z, f_zOrig, srName, "FailSieieData", plot); // This needs to be done last (it overwrites previous histograms)
    purityPlotsNew(f_out, f_gq, f_g, f_q, f_z, f_zOrig, srName+"Incl", "", plot);
    purityPlotsNew(f_out, f_gq, f_g, f_q, f_z, f_zOrig, srName+"Incl", "FailSieie", plot); // This needs to be done last (it overwrites previous histograms)
    purityPlotsNew(f_out, f_data, f_g, f_q, f_z, f_zOrig, srName+"Incl", "FailSieieData", plot); // This needs to be done last (it overwrites previous histograms)
  }
  purityPlotsNew(f_out, f_gq, f_g, f_q, f_z, f_zOrig, "baseIncl", "", "bosonptbins"); // This needs to be done last (it overwrites previous histograms) 
  purityPlotsNew(f_out, f_data, f_g, f_q, f_z, f_zOrig, "baseIncl", "FailSieieData", "bosonptbins"); // This needs to be done last (it overwrites previous histograms)   



  //save and write
  cout << "Saving and closing..." << endl;
  //f_out->Write();
  f_out->Close();
  
  return ;
}

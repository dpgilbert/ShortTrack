//script to make purity.root file
//which contains purity histograms
//calculated from fake-rate predictions

#include <string>
#include <iostream>
#include <algorithm>
#include <map>
#include <sstream>

#include "TFile.h"
#include "TStyle.h"
#include "TH1D.h"
#include "TH2.h"
#include "TCanvas.h"
#include "TString.h"
#include "TLegend.h"
#include "TKey.h"

#include "../MT2CORE/mt2tree.h"
#include "../MT2CORE/sigSelections.h"
#include "../MT2CORE/SR.h"

using namespace std;
using namespace mt2;

//converts float to string
std::string toString(float in){
  std::stringstream ss;
  ss << in;
  return ss.str();
}

//returns an empty hist with same binning
inline TH1D* sameBin(TH1D* h_in)
{
  TH1D* h_out = (TH1D*) h_in->Clone();
  h_out->Reset();
  return h_out;
}


//takes a FR histogram and signal region and calculates the predicted number of fakes
//in the signal region 'sr' , adding fragScale*qcdPrompt to LooseNotTight
void makePred(TFile* f_out, TFile* f_in, TFile* f_qcd, SR sr, TH2D* h_FR, const float fragScale = 1, TString s = "")
{
  
  //sr name
  TString srName = sr.GetName();

  //get LooseNotTight hist
  TH1D* h_FakeLooseNotTight = (TH1D*) f_in->Get("crgj"+srName+"/h_mt2binsFakeLooseNotTight");
  
  //check for empty hists
  if(!h_FR){
    cout << "FR hist is null!" << endl;
    return;
  }
  if(!h_FakeLooseNotTight){
    //cout << "SR" << srName << " FakeLooseNotTight hist is null! Skipping signal region..." << endl;
    return;
  }

  //initialize prediction array
  const int n_mt2bins = sr.GetNumberOfMT2Bins();
  Double_t preds[n_mt2bins+2]; //first and last bin are over/underflow
  Double_t predErrors[n_mt2bins+2]; //first and last bin are over/underflow
  for(int i=0; i<n_mt2bins+2; i++){
    preds[i] = 0;
    predErrors[i] = 0;
  }

  //mt2bin names
  vector<TString> mt2binsname;
  for(int i=0; i<n_mt2bins; i++){
    mt2binsname.push_back(toString(sr.GetMT2Bins()[i]));
  }
  
  //loop over mt2bins
  for (Int_t xbin = 0; xbin < n_mt2bins; xbin++) 
    {
      //initialize pred_totals
      Float_t pred_total       = 0.0;
      Float_t pred_error_total = 0.0;
  
      //get FR binned hists for this mt2 bin
      TH2D* h_sidebandqcdPrompt = (TH2D*) f_qcd->Get("crgj"+srName+"/h2d_gammaht_gammapt"+mt2binsname[xbin]+"LooseNotTight"); //qcdPrompt
      TH2D* h_sideband = (TH2D*) f_qcd->Get("crgj"+srName+"/h2d_gammaht_gammapt"+mt2binsname[xbin]+"FakeLooseNotTight"); //qcdFake

      //if "Poisson", undo sumw2() errors
      if(s.Contains("Poisson")){
	if(h_sidebandqcdPrompt) h_sidebandqcdPrompt->GetSumw2()->Set(0);
	if(h_sideband) h_sideband->GetSumw2()->Set(0);
      }
	
      //skip this mt2bin if there are no fakes
      if(!h_sideband) continue;

      //loop over FR bins
      for (Int_t FRbin = 1; FRbin < h_FR->GetSize()+1; FRbin++) 
	{

	  Float_t nFOs      = h_sideband->GetBinContent(FRbin);    // number of denominator not numerator objects for this bin
	  Float_t nFOsError = h_sideband->GetBinError(FRbin);      // number error on number of of denominator not numerator objects for this bin

	  Float_t FRvalue   = h_FR->GetBinContent(FRbin); // get value of fake rate for this bin
	  Float_t FRerror   = h_FR->GetBinError(FRbin);   // get error on fake rate for this bin	  

	  //shift nFOs by fragScale*QCDPrompt in LooseNotTight
	  if(h_sidebandqcdPrompt) {
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
      preds[xbin]=pred_total;
      predErrors[xbin]=sqrt(pred_error_total);

      //cleanup
      h_sideband->Delete();
    }//exit mt2bin loop

  //make and fill prediction histogram
  f_out->cd();

  TH1D* h_pred = (TH1D*) h_FakeLooseNotTight->Clone();
  h_pred->Reset();
  h_pred->SetName("h_pred"+srName+s);
  h_pred->SetContent(preds);
  h_pred->SetError(predErrors);
  h_pred->Write();

  return;
  
}

void purityPlots(TFile* f_out, TFile* f_gjet, TFile* f_qcd, TString sr)
{
  //get hists
  TH1D* h_gjet = (TH1D*) f_gjet->Get("crgj"+sr+"/h_mt2bins");
  TH1D* h_qcd = (TH1D*) f_qcd->Get("crgj"+sr+"/h_mt2bins");
  TH1D* h_qcdFake = (TH1D*) f_qcd->Get("crgj"+sr+"/h_mt2binsFake");
  TH1D* h_predFR = (TH1D*) f_out->Get("h_pred"+sr);
  TH1D* h_predSieieSB = (TH1D*) f_out->Get("h_pred"+sr+"FailSieie");
  TH1D* h_predFragPlus50 = (TH1D*) f_out->Get("h_pred"+sr+"plus50");
  TH1D* h_predFragMinus50 = (TH1D*) f_out->Get("h_pred"+sr+"minus50");

  //check existence
  if(!h_gjet) return;
  bool doTrue = false;
  bool doFR = false;
  bool doSieieSB = false;
  bool doFragPlus50 = false;
  bool doFragMinus50 = false;
  if(h_qcdFake) doTrue = true;
  if(h_predFR) doFR = true;
  if(h_predSieieSB) doSieieSB = true;
  if(h_predFragPlus50) doFragPlus50 = true;
  if(h_predFragMinus50) doFragMinus50 = true;

  //initialize numerator, den, and purity hists
  TH1D* h_num = sameBin(h_gjet);
  TH1D* h_denTrue = sameBin(h_gjet);
  TH1D* h_denFR = sameBin(h_gjet);
  TH1D* h_denSieieSB = sameBin(h_gjet);
  TH1D* h_denFragPlus50 = sameBin(h_gjet);
  TH1D* h_denFragMinus50 = sameBin(h_gjet);
  TH1D* h_purityTrue = sameBin(h_gjet);
  TH1D* h_purityFR = sameBin(h_gjet);
  TH1D* h_puritySieieSB = sameBin(h_gjet);
  TH1D* h_purityFragPlus50 = sameBin(h_gjet);
  TH1D* h_purityFragMinus50 = sameBin(h_gjet);

  //fill numerator
  if(h_qcd) h_num->Add(h_gjet, h_qcd);
  else h_num->Add(h_gjet);

  //initialize denominators
  if (h_qcd){ //if there are qcdPrompt, add to gjetPrompt
    h_denTrue->Add(h_gjet, h_qcd);
    h_denFR->Add(h_gjet, h_qcd);
    h_denSieieSB->Add(h_gjet, h_qcd);
    h_denFragPlus50->Add(h_gjet, h_qcd);
    h_denFragMinus50->Add(h_gjet, h_qcd);
  }
  else { //otherwise numerator is just gjet prompt
    h_denTrue->Add(h_gjet);
    h_denFR->Add(h_gjet);
    h_denSieieSB->Add(h_gjet);
    h_denFragPlus50->Add(h_gjet);
    h_denFragMinus50->Add(h_gjet);
  }

  //do True purity
  if(doTrue){
    h_denTrue->Add(h_qcdFake);
    h_purityTrue->Divide(h_num,h_denTrue,1,1,"B");
    h_purityTrue->SetName("h_purity"+sr+"True");
  }

  //do FR purity
  if(doFR){
    h_denFR->Add(h_predFR);
    h_purityFR->Divide(h_num,h_denFR,1,1,"B");
    h_purityFR->SetName("h_purity"+sr+"FR");
  }

  //do SieieSB purity
  if(doSieieSB){
    h_denSieieSB->Add(h_predSieieSB);
    h_puritySieieSB->Divide(h_num,h_denSieieSB,1,1,"B");
    h_puritySieieSB->SetName("h_purity"+sr+"SieieSB");
  }

  //do FragPlus50 purity
  if(doFragPlus50){
    h_denFragPlus50->Add(h_predFragPlus50);
    h_purityFragPlus50->Divide(h_num,h_denFragPlus50,1,1,"B");
    h_purityFragPlus50->SetName("h_purity"+sr+"FragPlus50");
  }

  //do FragMinus50 purity
  if(doFragMinus50){
    h_denFragMinus50->Add(h_predFragMinus50);
    h_purityFragMinus50->Divide(h_num,h_denFragMinus50,1,1,"B");
    h_purityFragMinus50->SetName("h_purity"+sr+"FragMinus50");
  }
   
  f_out->cd();

  //write hists to output file
  if(doTrue) h_purityTrue->Write();
  if(doFR) h_purityFR->Write();
  if(doSieieSB) h_puritySieieSB->Write();
  if(doFragPlus50) h_purityFragPlus50->Write();
  if(doFragMinus50) h_purityFragMinus50->Write();
  
  return;
}

void purity()
{
  
  //load signal regions
  vector<SR> SRVec =  getSignalRegionsZurich_jetpt40();
  
  //open files
  TFile* f_g = new TFile("$CMSSW_BASE/../MT2looper/output/V00-00-12_skim_trig_nj2_ht450_met30_mt2gt200_Zinv/gjet_ht.root"); //gjet file
  TFile* f_q = new TFile("$CMSSW_BASE/../MT2looper/output/V00-00-12_skim_trig_nj2_ht450_met30_mt2gt200_Zinv/qcd_pt.root"); //qcd file
  TFile* f_QCDpluGJET = new TFile("$CMSSW_BASE/../MT2looper/output/V00-00-12_skim_trig_nj2_ht450_met30_mt2gt200_Zinv/qcdplugjet.root"); //qcd+gjets file

  cout << "Making Fake-Rate Histograms..." << endl;
  
  //get hists for FR calc
  TH2D* h_qcdTight = (TH2D*) f_q->Get("crgjbase/h2d_gammaht_gammaptFake");
  TH2D* h_qcdLoose = (TH2D*) f_q->Get("crgjbase/h2d_gammaht_gammaptFakeLoose");
  h_qcdTight->SetName("h_qcdTight");
  h_qcdLoose->SetName("h_qcdLoose");
  
  //get hists for FR calc, Sieie Sideband
  TH2D* h_qcdTightFailSieie = (TH2D*) f_q->Get("crgjbase/h2d_gammaht_gammaptFakeSieieSB");
  TH2D* h_qcdLooseFailSieie = (TH2D*) f_q->Get("crgjbase/h2d_gammaht_gammaptFakeLooseSieieSB");
  h_qcdTightFailSieie->SetName("h_qcdTightFailSieie");
  h_qcdLooseFailSieie->SetName("h_qcdLooseFailSieie");
    
  //instantiate output file here
  TFile* f_out = new TFile("$CMSSW_BASE/../scripts/purity.root","RECREATE");

  //FR histograms
  TH2D* h_FR = (TH2D*) h_qcdTight->Clone();
  h_FR->Reset();
  TH2D* h_FRFailSieie = (TH2D*) h_qcdTightFailSieie->Clone();
  h_FRFailSieie->Reset();
  
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

  //make the prediction hists
  cout << "Making predicted yield histograms..." << endl;
  for(int i = 0; i< (int) SRVec.size(); i++){
    makePred(f_out, f_QCDpluGJET, f_q, SRVec[i], h_FR, 0, ""); //FR using passSieie, LooseNotTight Fakes + 0 qcdPrompt
    makePred(f_out, f_QCDpluGJET, f_q, SRVec[i], h_FR, 0.5, "plus50"); //FR using passSieie, LooseNotTight Fakes + +50% qcdPrompt
    makePred(f_out, f_QCDpluGJET, f_q, SRVec[i], h_FR, -0.5, "minus50"); //FR using passSieie, LooseNotTight Fakes + -50% qcdPrompt 
    makePred(f_out, f_QCDpluGJET, f_q, SRVec[i], h_FRFailSieie, 0, "FailSieie"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt
    makePred(f_out, f_QCDpluGJET, f_q, SRVec[i], h_FRFailSieie, 0, "FailSieiePoisson"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt, Poisson errors
  }

  //make purity plots
  cout << "Making purity histograms..." << endl;
  for(int i = 0; i< (int) SRVec.size(); i++){
    purityPlots(f_out, f_g, f_q, SRVec[i].GetName());
  }

  //save and write
  cout << "Saving and closing..." << endl;
  //f_out->Write();
  f_out->Close();
  
  return ;
}

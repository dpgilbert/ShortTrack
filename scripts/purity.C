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

#include "../MT2CORE/mt2tree.h"
#include "../MT2CORE/sigSelections.h"
#include "../MT2CORE/SR.h"
#include "../MT2CORE/sigSelections.cc"
#include "../MT2CORE/SR.cc"

using namespace std;
using namespace mt2;

bool verbose = false;
bool realData = false;
bool firstTimeMC = true;
bool firstTimeData = true;

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
      if(!h_sideband) continue;

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
    
    if (verbose && nFOs>0) cout<<"FR bin "<<FRbin<<" has FR "<<FRvalue<<", nFO "<<nFOs<<" and predicted "<<pred<<endl;
	  
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
  h_pred->SetName("h_pred"+s);
  h_pred->SetContent(preds);
  h_pred->SetError(predErrors);
  h_pred->Write();
  
  return;
  
}

void purityPlots(TFile* f_out, TFile* f_gjet, TFile* f_qcd, TFile* f_zinv, TString sr)
{
  if (verbose) cout<<__LINE__<<" Making plots for region "<<sr<<endl;
  //get hists
  TH1F* h_gjet = (TH1F*) f_gjet->Get("crgj"+sr+"/h_mt2bins");
  TH1F* h_qcd = (TH1F*) f_qcd->Get("crgj"+sr+"/h_mt2bins");
  TH1F* h_qcdFake = (TH1F*) f_qcd->Get("crgj"+sr+"/h_mt2binsFake");
  TH1F* h_qcdFakeLooseNotTight = (TH1F*) f_qcd->Get("crgj"+sr+"/h_mt2binsFakeLooseNotTight");
  TH1F* h_predFR = (TH1F*) f_out->Get("sr"+sr+"/h_pred");
  TH1F* h_predSieieSB = (TH1F*) f_out->Get("sr"+sr+"/h_pred"+"FailSieie");
  TH1F* h_predFragPlus50 = (TH1F*) f_out->Get("sr"+sr+"/h_pred"+"plus50");
  TH1F* h_predFragMinus50 = (TH1F*) f_out->Get("sr"+sr+"/h_pred"+"minus50");
  TH1F* h_predFRpoisson = (TH1F*) f_out->Get("sr"+sr+"/h_pred"+"Poisson");
  TH1F* h_predSieieSBpoisson = (TH1F*) f_out->Get("sr"+sr+"/h_pred"+"FailSieiePoisson");

  TH1F* h_ratio = (TH1F*) f_zinv->Get("sr"+sr+"/h_mt2binsRatio");

  //check existence
  if(!h_gjet) return;
  if (verbose) cout<<__LINE__<<" f_gjet:crgj"<<sr<<"/h_mt2bins has integral "<<h_gjet->Integral()<<endl;
  bool doTrue = false;
  bool doFR = false;
  bool doSieieSB = false;
  bool doFragPlus50 = false;
  bool doFragMinus50 = false;
  bool doFRpoisson = false;
  bool doSieieSBpoisson = false;
  if(h_qcdFake) doTrue = true;
  if(h_predFR) doFR = true;
  if(h_predSieieSB) doSieieSB = true;
  if(h_predFragPlus50) doFragPlus50 = true;
  if(h_predFragMinus50) doFragMinus50 = true;
  if(h_predFRpoisson) doFRpoisson = true;
  if(h_predSieieSBpoisson) doSieieSBpoisson = true;
  //cout<<doTrue <<" "<<doFR <<" "<<doSieieSB <<" "<<doFragPlus50 <<" "<<doFragMinus50 <<" "<<doFRpoisson <<" "<<doSieieSBpoisson<<endl;

  f_out->cd();
  TString directory = "sr"+sr;
  TDirectory* dir = 0;
  dir = (TDirectory*)f_out->Get(directory.Data());
  if (dir == 0) {
    dir = f_out->mkdir(directory.Data());
  } 
  dir->cd();

  //initialize numerator, den, and purity hists
  TH1F* h_num = sameBin(h_gjet);
  TH1F* h_denTrue = sameBin(h_gjet);
  TH1F* h_denFR = sameBin(h_gjet);
  TH1F* h_denFRpoisson = sameBin(h_gjet);
  TH1F* h_denSieieSB = sameBin(h_gjet);
  TH1F* h_denSieieSBpoisson = sameBin(h_gjet);
  TH1F* h_denFragPlus50 = sameBin(h_gjet);
  TH1F* h_denFragMinus50 = sameBin(h_gjet);
  TH1F* h_purityTrue = sameBin(h_gjet);
  TH1F* h_purityFR = sameBin(h_gjet);
  TH1F* h_purityFRpoisson = sameBin(h_gjet);
  TH1F* h_puritySieieSB = sameBin(h_gjet);
  TH1F* h_estimateSieieSB = sameBin(h_gjet);
  TH1F* h_estimateSieieSBpoisson = sameBin(h_gjet);
  TH1F* h_puritySieieSBpoisson = sameBin(h_gjet);
  TH1F* h_purityFragPlus50 = sameBin(h_gjet);
  TH1F* h_purityFragMinus50 = sameBin(h_gjet);

  //fill numerator
  if(h_qcd) h_num->Add(h_gjet, h_qcd);
  else h_num->Add(h_gjet);

  //initialize denominators
  if (h_qcd){ //if there are qcdPrompt, add to gjetPrompt
    h_denTrue->Add(h_gjet, h_qcd);
    h_denFR->Add(h_gjet, h_qcd);
    h_denFRpoisson->Add(h_gjet, h_qcd);
    h_denSieieSB->Add(h_gjet, h_qcd);
    h_denSieieSBpoisson->Add(h_gjet, h_qcd);
    h_denFragPlus50->Add(h_gjet, h_qcd);
    h_denFragMinus50->Add(h_gjet, h_qcd);
  }
  else { //otherwise numerator is just gjet prompt
    h_denTrue->Add(h_gjet);
    h_denFR->Add(h_gjet);
    h_denFRpoisson->Add(h_gjet);
    h_denSieieSB->Add(h_gjet);
    h_denSieieSBpoisson->Add(h_gjet);
    h_denFragPlus50->Add(h_gjet);
    h_denFragMinus50->Add(h_gjet);
  }

  //do True purity
  if(doTrue) h_denTrue->Add(h_qcdFake);
  h_purityTrue->Divide(h_num,h_denTrue,1,1,"B");
  h_purityTrue->SetName("h_purityTrue");
  h_denTrue->SetName("h_mt2bins");

  //do FR purity
  if(doFR) h_denFR->Add(h_predFR);
  h_purityFR->Divide(h_num,h_denFR,1,1,"B");
  h_purityFR->SetName("h_purityFR");

  //do FR purity with poisson errors on yields
  TH1F* h_numP =  (TH1F*) h_num->Clone("numPoisson");
  h_numP->GetSumw2()->Set(0); h_numP->Sumw2(); // reset sumw2
  h_denFRpoisson->GetSumw2()->Set(0); h_denFRpoisson->Sumw2(); // reset sumw2
  if(doFRpoisson) h_denFRpoisson->Add(h_predFRpoisson);
  h_purityFRpoisson->Divide(h_numP,h_denFRpoisson,1,1,"B");
  h_purityFRpoisson->SetName("h_purityFRpoisson");


  //do SieieSB purity
  if(doSieieSB) h_denSieieSB->Add(h_predSieieSB);
  h_puritySieieSB->Divide(h_num,h_denSieieSB,1,1,"B");
  h_puritySieieSB->SetName("h_puritySieieSB");
  // want to also save: (DenTrue (total of all tight events) - prediction) *0.92, which is our actual gjet prediction
  h_estimateSieieSB->Add(h_denTrue);
  if(doSieieSB) h_estimateSieieSB->Add(h_predSieieSB, -1);
  h_estimateSieieSB->Scale(0.92);
  h_estimateSieieSB->SetName("h_estimateSieieSB");
  //h_estimateSieieSB->Print("all");
  //h_gjet->Print("all");
  
  //TEfficiency * pEff = new TEfficiency((const TH1*) h_num, (const TH1*) h_denSieieSB);
  
  // Dump everything!
  //if (sr == "1M") {
  //  cout<<"h_purity"<<sr<<"SieieSB"<<endl;
  //  TEfficiency * pEff = new TEfficiency(*h_num,*h_denSieieSB);
  //  pEff->SetStatisticOption(TEfficiency::EStatOption::kBBayesian); //kBayesian
  //  h_gjet->Print("all");
  //  h_num->Print("all");
  //  if (doTrue) h_qcdFake->Print("all");
  //  if(doSieieSB) h_predSieieSB->Print("all");
  //  h_denSieieSB->Print("all");
  //  h_puritySieieSB->Print("all");
  //  for ( int i = 1; i <= h_puritySieieSB->GetNbinsX(); i++)
  //	cout<<"Bin "<<i<<" has eff "<<pEff->GetEfficiency(i)<<" +"<<pEff->GetEfficiencyErrorUp(i)<<"/-"<<pEff->GetEfficiencyErrorLow(i)<<endl;;
  //}
  
  //do SieieSB purity with poisson errors on yields
  TH1F* h_numP2 = (TH1F*) h_num->Clone("numPoisson2");
  h_numP2->GetSumw2()->Set(0); h_numP2->Sumw2(); // reset sumw2
  h_denSieieSBpoisson->GetSumw2()->Set(0); h_denSieieSBpoisson->Sumw2(); // reset sumw2
  if(doSieieSBpoisson) h_denSieieSBpoisson->Add(h_predSieieSBpoisson);
  h_puritySieieSBpoisson->Divide(h_numP2,h_denSieieSBpoisson,1,1,"B");
  h_puritySieieSBpoisson->SetName("h_puritySieieSBpoisson");
  h_estimateSieieSBpoisson->Add(h_denTrue);
  h_estimateSieieSBpoisson->GetSumw2()->Set(0); h_estimateSieieSBpoisson->Sumw2();
  if(doSieieSBpoisson) h_estimateSieieSBpoisson->Add(h_predSieieSBpoisson, -1);
  h_estimateSieieSBpoisson->Scale(0.92);
  h_estimateSieieSBpoisson->SetName("h_estimateSieieSBpoisson");
  // Dump everything!
  //if (sr == "1M") {
  //  cout<<"h_purity"<<sr<<"SieieSBpoisson"<<endl;
  //  TEfficiency * pEff = new TEfficiency(*h_numP2,*h_denSieieSBpoisson);
  //  pEff->SetStatisticOption(TEfficiency::EStatOption::kBBayesian); //kBayesian
  //  h_numP2->Print("all");
  //  if(doSieieSBpoisson) h_predSieieSBpoisson->Print("all");
  //  h_denSieieSBpoisson->Print("all");
  //  h_puritySieieSBpoisson->Print("all");
  //  for ( int i = 1; i <= h_puritySieieSB->GetNbinsX(); i++)
  //	cout<<"Bin "<<i<<" has eff "<<pEff->GetEfficiency(i)<<" +"<<pEff->GetEfficiencyErrorUp(i)<<"/-"<<pEff->GetEfficiencyErrorLow(i)<<endl;;
  //}
  
  
  //do FragPlus50 purity
  if(doFragPlus50) h_denFragPlus50->Add(h_predFragPlus50);
  h_purityFragPlus50->Divide(h_num,h_denFragPlus50,1,1,"B");
  h_purityFragPlus50->SetName("h_purityFragPlus50");

  //do FragMinus50 purity
  if(doFragMinus50) h_denFragMinus50->Add(h_predFragMinus50);
  h_purityFragMinus50->Divide(h_num,h_denFragMinus50,1,1,"B");
  h_purityFragMinus50->SetName("h_purityFragMinus50");
   
  //f_out->cd();
  dir->cd();

  //make zinv pred, predZ = N * purity * ratio
  //note frag fraction f is included in purity
  TH1F* h_predZ = (TH1F*) h_num->Clone();
  h_predZ->SetName("h_predZ");
  h_predZ->Multiply(h_predZ,h_puritySieieSB,1,1,"B");
  h_predZ->Multiply(h_predZ,h_ratio,1,1,"B");

  //add systematic error to zinv pred
  for (int bin = 0; bin <= h_predZ->GetNbinsX(); bin++){
    double statErr = h_predZ->GetBinError(bin);
    double fragErr = 0.05*h_predZ->GetBinContent(bin);
    double corrErr = 0.2*h_predZ-> GetBinContent(bin); 
    h_predZ->SetBinError(bin,pow(pow(statErr,2)+pow(fragErr,2)+pow(statErr,2),0.5));
  }
    
  //write hists to output file
  h_purityTrue->Write();
  h_purityFR->Write();
  h_puritySieieSB->Write();
  h_estimateSieieSB->Write();
  h_purityFragPlus50->Write();
  h_purityFragMinus50->Write();
  h_purityFRpoisson->Write();
  h_puritySieieSBpoisson->Write();
  h_estimateSieieSBpoisson->Write();
  h_denTrue->Write();
  h_predZ->Write();

  if (verbose) {
    cout<<__LINE__<<"  h_purityTrue              " <<      h_purityTrue		    ->GetName()<<" "<<  h_purityTrue		 ->Integral()<<endl;   
    cout<<__LINE__<<"  h_purityFR		 " << 	   h_purityFR		    ->GetName()<<" "<<  h_purityFR		 ->Integral()<<endl;   
    cout<<__LINE__<<"  h_puritySieieSB		 " <<      h_puritySieieSB	    ->GetName()<<" "<<  h_puritySieieSB	    	 ->Integral()<<endl;
    cout<<__LINE__<<"  h_estimateSieieSB	 " << 	   h_estimateSieieSB	    ->GetName()<<" "<<  h_estimateSieieSB	 ->Integral()<<endl;   
    cout<<__LINE__<<"  h_purityFragPlus50	 " << 	   h_purityFragPlus50	    ->GetName()<<" "<<  h_purityFragPlus50	 ->Integral()<<endl;   
    cout<<__LINE__<<"  h_purityFragMinus50	 " << 	   h_purityFragMinus50	    ->GetName()<<" "<<  h_purityFragMinus50	 ->Integral()<<endl;   
    cout<<__LINE__<<"  h_purityFRpoisson	 " << 	   h_purityFRpoisson	    ->GetName()<<" "<<  h_purityFRpoisson	 ->Integral()<<endl;   
    cout<<__LINE__<<"  h_puritySieieSBpoisson	 " <<      h_puritySieieSBpoisson   ->GetName()<<" "<<  h_puritySieieSBpoisson   ->Integral()<<endl;
    cout<<__LINE__<<"  h_estimateSieieSBpoisson  " << 	   h_estimateSieieSBpoisson ->GetName()<<" "<<  h_estimateSieieSBpoisson ->Integral()<<endl;
    cout<<__LINE__<<"  h_denTrue		 " << 	   h_denTrue		    ->GetName()<<" "<<  h_denTrue		 ->Integral()<<endl;   
    cout<<__LINE__<<"  h_predZ                   " << 	   h_predZ                  ->GetName()<<" "<<  h_predZ                  ->Integral()<<endl;
  }



  //write raw numbers to output file
  ofstream purityLog;
  purityLog.open("purity.log", ios::app);
  purityLog << "------------------------------" << endl;
  purityLog << "Signal Region " << sr << endl;
  for (int i = 1; i<= h_gjet->GetNbinsX(); i++){
    purityLog << h_gjet->GetBinLowEdge(i) << " < MT2 < " << h_gjet->GetBinLowEdge(i+1) << endl;
    if(h_qcdFake) purityLog << "Ngamma (tight): " << h_qcdFake->GetBinContent(i) << endl;
    if(h_qcdFakeLooseNotTight) purityLog << "Ngamma (loose not tight): " << h_qcdFakeLooseNotTight->GetBinContent(i) << endl;
    if(doFR) purityLog << "Purity: " << h_purityFR->GetBinContent(i) << " +/- " << h_purityFR->GetBinError(i) << endl;
    purityLog << endl;
  }
  purityLog << "------------------------------" << endl;
  purityLog.close();
  
  f_out->cd();
  return;
}


void purityPlotsNew(TFile* f_out, TFile* f_data, TFile* f_gjet, TFile* f_qcd, TFile* f_zinv, TString sr, TString FR_type)

{
  if (verbose) cout<<__LINE__<<" Making plots for region "<<sr<<endl;
  //get hists
  TH1F* h_gjet = (TH1F*) f_gjet->Get("crgj"+sr+"/h_mt2bins");
  TH1F* h_qcd  = (TH1F*) f_qcd->Get("crgj"+sr+"/h_mt2bins");
  TH1F* h_full  = (TH1F*) f_data->Get("crgj"+sr+"/h_mt2bins");
  //check existence
  if(!h_full || !h_gjet) return;
  TH1F* h_qcdFake = (TH1F*) f_qcd->Get("crgj"+sr+"/h_mt2binsFake");
  if (!FR_type.Contains("Data") && h_qcdFake) {
    h_full->Add(h_qcdFake); // need to add MC fakes to our total, if running on MC.
  }
  TH1F* h_fullLooseNotTight = (TH1F*) f_data->Get("crgj"+sr+"/h_mt2binsFakeLooseNotTight");
  if (FR_type.Contains("Data") && h_fullLooseNotTight) {
    h_fullLooseNotTight = (TH1F*) f_data->Get("crgj"+sr+"/h_mt2binsLooseNotTight"); // Can't have "Fake" in the name, if running on data
  }
  TH1F* h_predFR = (TH1F*) f_out->Get("sr"+sr+"/h_pred"+FR_type);
  TH1F* h_ratio = (TH1F*) f_zinv->Get("sr"+sr+"/h_mt2binsRatio");

  if (verbose && h_gjet) cout<<__LINE__<<" f_gjet:crgj"<<sr<<"/h_mt2bins has integral "<<h_gjet->Integral()<<endl;
  if (verbose && h_gjet) cout<<__LINE__<<" f_data:crgj"<<sr<<"/h_mt2bins has integral "<<h_full->Integral()<<endl;
  
  f_out->cd();
  TString directory = "sr"+sr;
  TDirectory* dir = 0;
  dir = (TDirectory*)f_out->Get(directory.Data());
  if (dir == 0) {
    dir = f_out->mkdir(directory.Data());
  }
  dir->cd();
  
  //do True purity
  TH1F* h_numTrue = (TH1F*) h_gjet->Clone("h_numTrue"); h_numTrue->SetName("h_numTrue");
  TH1F* h_denTrue = (TH1F*) h_gjet->Clone("h_denTrue"); h_denTrue->SetName("h_denTrue");
  TH1F* h_purityTrue = (TH1F*) h_gjet->Clone("h_purityTrue"); h_purityTrue->SetName("h_purityTrue");
  if(h_qcd) h_numTrue->Add(h_qcd);
  if(h_qcd) h_denTrue->Add(h_qcd);
  if(h_qcdFake) h_numTrue->Add(h_qcdFake, -1.);
  h_purityTrue->Divide(h_numTrue,h_denTrue,1,1,"B");

  
  //do FR purity
  TH1F* h_numFR = (TH1F*) h_full->Clone("h_numFR"); h_numFR->SetName("h_numFR");
  TH1F* h_denFR = (TH1F*) h_full->Clone("h_denFR"); h_denFR->SetName("h_mt2bins"); // this is our "DATA", later picked up by cardMaker!!
  TH1F* h_purityFR = (TH1F*) h_full->Clone("h_purityFR"); h_purityFR->SetName("h_purity"+FR_type);
  if (FR_type.Contains("Poisson")) {
    h_numFR->GetSumw2()->Set(0); h_numFR->Sumw2(); // reset sumw2
    h_denFR->GetSumw2()->Set(0); h_denFR->Sumw2(); // reset sumw2
  }
  if(h_predFR) h_numFR->Add(h_predFR, -1.);
  h_purityFR->Divide(h_numFR,h_denFR,1,1,"B");
  // estimate = (total - fakes)*f
  TH1F* h_estimate = (TH1F*) h_numFR->Clone("h_estimate"); h_estimate->SetName("h_estimate"+FR_type);
  h_estimate->Scale(0.9);
  
  
  dir->cd();
  
  //make zinv pred, predZ = N * purity * ratio
  //note frag fraction f is included in purity
  TH1F* h_predZ = (TH1F*) h_full->Clone("h_predZ");
  h_predZ->SetName("h_predZ"+FR_type);
  if (h_purityFR) h_predZ->Multiply(h_predZ,h_purityFR,1,1,"B");
  if (h_ratio) h_predZ->Multiply(h_predZ,h_ratio,1,1,"B");
  
  //add systematic error to zinv pred
  for (int bin = 0; bin <= h_predZ->GetNbinsX(); bin++){
    double statErr = h_predZ->GetBinError(bin);
    double fragErr = 0.05*h_predZ->GetBinContent(bin); // 5% for purity/frag
//    double corrErr = 0.2*h_predZ-> GetBinContent(bin); // 20% for R(Z/G)
//    h_predZ->SetBinError(bin,pow(pow(statErr,2)+pow(fragErr,2)+pow(corrErr,2),0.5));
    h_predZ->SetBinError(bin,pow(pow(statErr,2)+pow(fragErr,2),0.5));
  }
  
  //write hists to output file
  h_purityFR->Write();
  h_estimate->Write();
  h_predZ->Write();
  if (!FR_type.Contains("Data") ) {
    h_purityTrue->Write();
    firstTimeMC = false;
  }
  if (FR_type.Contains("Data") ) {
    h_denFR->Write();
    firstTimeData = false;
  }
  
  if (verbose) {
    cout<<__LINE__<<"  h_purityTrue              " <<      h_purityTrue		    ->GetName()<<" "<<  h_purityTrue		 ->Integral()<<endl;
    cout<<__LINE__<<"  h_purityFR		 " << 	   h_purityFR		    ->GetName()<<" "<<  h_purityFR		 ->Integral()<<endl;
    if (h_predFR) cout<<__LINE__<<"  h_predFR  		 " << 	   h_predFR		    ->GetName()<<" "<<  h_predFR		 ->Integral()<<endl;
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
  vector<SR> SRVec =  getSignalRegionsZurich_jetpt30();

  //open files
  // get input files -- default to faking data with same MC file
  TString datanamestring(dataname);
  if (datanamestring.Contains("Data") || datanamestring.Contains("data")) realData = true;
  TFile* f_data = new TFile(Form("%s/%s.root",input_dir.c_str(),dataname.c_str())); //data or qcd+gjets file
  TFile* f_gq = new TFile(Form("%s/qcdplusgjet.root",input_dir.c_str())); //qcd+gjets file
  TFile* f_g = new TFile(Form("%s/gjet_ht.root",input_dir.c_str())); //gjet file
  TFile* f_q = new TFile(Form("%s/qcd_ht.root",input_dir.c_str())); //qcd file
  TFile* f_z = new TFile(Form("%s/zinvFromGJ.root",input_dir.c_str())); //zinv pred from ZinvMaker.C, contains ratio
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
  
  
  //get hists for FR calc, Sieie Sideband
  TH2D* h_qcdTightFailSieie = (TH2D*) f_q->Get("crgjbase/h2d_gammaht_gammaptSingleBinFakeSieieSB");
  TH2D* h_qcdLooseFailSieie = (TH2D*) f_q->Get("crgjbase/h2d_gammaht_gammaptSingleBinFakeLooseSieieSB");
  if (!h_qcdTightFailSieie || !h_qcdLooseFailSieie) cout<<"Could not find SieieSB FR histograms in QCD MC"<<endl;
  h_qcdTightFailSieie->SetName("h_qcdTightFailSieie");
  h_qcdLooseFailSieie->SetName("h_qcdLooseFailSieie");
  
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
  for(int i = 0; i< (int) SRVec.size()+1; i++){
    TString srName = "base";
    if (i < (int) SRVec.size()) srName = SRVec[i].GetName();
    makePred(f_out, f_gq, f_q, f_g, srName, h_FR, 0, ""); //FR using passSieie, LooseNotTight Fakes + 0 qcdPrompt
    //makePred(f_out, f_data, f_q, f_g, SRVec[i], h_FR, 0.5, "plus50"); //FR using passSieie, LooseNotTight Fakes + +50% qcdPrompt
    //makePred(f_out, f_data, f_q, f_g, SRVec[i], h_FR, -0.5, "minus50"); //FR using passSieie, LooseNotTight Fakes + -50% qcdPrompt
    makePred(f_out, f_gq, f_q, f_g, srName, h_FRFailSieie, 0, "FailSieie"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt
    //makePred(f_out, f_data, f_q, f_g, SRVec[i], h_FRFailSieie, 0, "FailSieiePoisson"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt, Poisson errors
    makePred(f_out, f_data, f_q, f_g, srName, h_FRFailSieieData, 0, "FailSieieData"); //FR using !passSieie, LooseNotTight Fakes + 0 qcdPrompt, Data
    //makePred(f_out, f_data, f_q, f_g, SRVec[i], h_FR, 0, "Poisson"); //FR using passSieie, LooseNotTight Fakes + 0 qcdPrompt, Poisson errors
  }


  //make purity plots
  //first delete log file in case it already exists
  if( remove( "purity.log" ) != 0 )
    perror( "Error deleting purity.log" );
  else
    puts( "Deleting old log file..." );
  cout << "Making purity histograms..." << endl;
  for(int i = 0; i< (int) SRVec.size()+1; i++){
    TString srName = "base";
    if (i < (int) SRVec.size()) srName = SRVec[i].GetName();
//    purityPlots(f_out, f_g, f_q, f_z, SRVec[i].GetName());
    purityPlotsNew(f_out, f_gq, f_g, f_q, f_z, srName, "");
    purityPlotsNew(f_out, f_data, f_g, f_q, f_z, srName, "FailSieieData"); // This needs to be done last (it overwrites previous histograms)
  }

  //save and write
  cout << "Saving and closing..." << endl;
  //f_out->Write();
  f_out->Close();
  
  return ;
}

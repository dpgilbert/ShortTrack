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
bool doHybridSimple = false; // hybrid estimate: uses CR MT2 binning until the (MC) integral is less than the threshold below
bool doHybridInclusiveTemplate = true; // take kMT2 from inclusive templates
float hybrid_nevent_threshold = 50.;
float rSFOF = 1.12;
float rSFOFerr = 0.15;
bool verbose = true;

int findLastMT2Hardcoded(TString srname){ 

  if (srname == "1VL")      return 400;
  else if (srname == "2VL") return 400; 
  else if (srname == "3VL") return 400;
  else if (srname =="12VL") return 300;
  else if (srname =="13VL") return 300;
  else if (srname =="14VL") return 300;
  else if (srname =="15VL") return 400;

  else if (srname == "1L") return 400;
  else if (srname == "2L") return 400;
  else if (srname == "3L") return 400;
  else if (srname == "4L") return 400;
  else if (srname == "5L") return 400;
  else if (srname == "6L") return 400;
  else if (srname == "7L") return 200;
  else if (srname == "8L") return 200;
  else if (srname == "9L") return 200;
  else if (srname =="10L") return 500;
  else if (srname =="11L") return 200;

  else if (srname == "1M") return 600;
  else if (srname == "2M") return 600;
  else if (srname == "3M") return 600;
  else if (srname == "4M") return 400;
  else if (srname == "5M") return 400;
  else if (srname == "6M") return 400;
  else if (srname == "7M") return 200;
  else if (srname == "8M") return 200;
  else if (srname == "9M") return 200;
  else if (srname =="10M") return 600;
  else if (srname =="11M") return 200;

  else if (srname == "1H") return 400;
  else if (srname == "2H") return 400;
  else if (srname == "3H") return 400;
  else if (srname == "4H") return 400;
  else if (srname == "5H") return 400;
  else if (srname == "6H") return 400;
  else if (srname == "7H") return 200;
  else if (srname == "8H") return 200;
  else if (srname == "9H") return 200;
  else if (srname =="10H") return 400;
  else if (srname =="11H") return 200;

  else if (srname == "1UH") return 400;
  else if (srname == "2UH") return 400;
  else if (srname == "3UH") return 400;
  else if (srname == "4UH") return 400;
  else if (srname == "5UH") return 400;
  else if (srname == "6UH") return 400;
  else if (srname == "7UH") return 400;
  else if (srname == "8UH") return 400;
  else if (srname == "9UH") return 400;
  else if (srname =="10UH") return 400;
  else if (srname =="11UH") return 400;

  else if (srname == "20") return 500;
  else if (srname == "21") return 600;
  else if (srname == "22") return 500;
  else if (srname == "23") return 400;
  else if (srname == "24") return 500;
  else if (srname == "25") return 400;
  else if (srname == "26") return 600;
  else if (srname == "27") return 400;
  else if (srname == "28") return 500;
  else if (srname == "29") return 400;
  else if (srname == "30") return 500;
  else if (srname == "31") return 400;

  else return -1;

}


int purityRandNorm(TH1D* h_template, TString name , TFile * fData, TFile * fZinv, TFile * fDY, int & lastmt2val_hybrid) {

  //cout<<"purityRandNorm for template "<<name<<endl;
  //h_template->Print("all");
  int lastbin_hybrid = 0;
  lastmt2val_hybrid = 200;
  TString name_emu = name + "emu";
  TString name_zinv = name;
  name_zinv.ReplaceAll("crdy", "sr");
  TH1D* hEMU   = (TH1D*) fData->Get(name_emu);    
  TH1D* hDY    = (TH1D*) fDY->Get(name);    
  TH1D* hZinv  = (TH1D*) fZinv->Get(name_zinv); 
  if (h_template == 0) {
    cout<<"ZinvMaker::purityAndRatio  :  could not find input template"<<endl;
    return lastbin_hybrid;
  }
  if (hDY == 0 || hZinv == 0) {
    cout<<"ZinvMaker::purityAndRatio  :  could not find DY or Zinv MC histogram"<<endl;
    return lastbin_hybrid;
  }
  if (hEMU) h_template->Add(hEMU, -1*rSFOF);

  // find the last bin
  for ( int ibin=1; ibin <= hDY->GetNbinsX(); ++ibin ) {
    float integratedYield = 0;
    integratedYield = hDY->Integral(ibin,-1);
    if (integratedYield < hybrid_nevent_threshold) {
      if (ibin == 1) lastbin_hybrid = 1;
      else {
	lastbin_hybrid = ibin-1;
	lastmt2val_hybrid = hDY->GetBinLowEdge(ibin);
      }
      break;
    }
  }

  // multiply R(Znn/Zll)
  TH1D* ratio =   (TH1D*) hZinv->Clone("ratio");
  ratio->Divide(hDY);

  h_template->Multiply(ratio);

  // Get the integrals to normalize the Zinv tails
  // and the uncertainties on the CR yield (dominated by data stats in the last N bins)
  double integratedYieldErrZinv = 0;
  float integratedYieldZinv = hZinv->IntegralAndError(lastbin_hybrid, -1., integratedYieldErrZinv);
  float relativeErrorZinv = integratedYieldErrZinv/integratedYieldZinv;
  double integratedYieldErr = 0;
  float integratedYield = h_template->IntegralAndError(lastbin_hybrid,-1,integratedYieldErr); 
  float relativeError = integratedYieldErr/integratedYield;

  // Hybridize the template: last N bins have a common stat uncertainty, and they follow the Zinv MC shape
  for ( int ibin=1; ibin <= hZinv->GetNbinsX(); ++ibin ) {

    if (ibin < lastbin_hybrid) continue;

    float kMT2 = hZinv->GetBinContent(ibin) / integratedYieldZinv;
    float err = sqrt( relativeError*relativeError + relativeErrorZinv*relativeErrorZinv);
    h_template->SetBinContent(ibin, integratedYield * kMT2);
    h_template->SetBinError(ibin, integratedYield * kMT2 * err );

  }

  // Normalize it: we just need a shape after all
  h_template->Scale(1./h_template->Integral());

  //h_template->Print("all");


  return lastbin_hybrid;
}

// New function: takes as input a blank histogram with the right binning, then turns it to a template.
// We used to first make a global hybrid template, then rebin it to match the toporegion
// Instead here we first rebin the golbal template to match the toporegion, and only afterwards we hybridize it
int makeHybridTemplate(TString srname, TH1D* & h_template, TString name , TFile * fData, TFile * fZinv, TFile * fDY, int & lastmt2val_hybrid) {

  //cout<<"purityRandNorm for template "<<name<<endl;
  //h_template->Print("all");
  int lastbin_hybrid = 1;
  lastmt2val_hybrid = 200;
  TString name_emu = name + "emu";
  TString name_zinv = name;
  name_zinv.ReplaceAll("crdy", "sr");
  TH1D* hSF   = (TH1D*) fData->Get(name);    
  TH1D* hOF   = (TH1D*) fData->Get(name_emu);    
  TH1D* hDY    = (TH1D*) fDY->Get(name);    
  TH1D* hZinv  = (TH1D*) fZinv->Get(name_zinv); 

  if (h_template == 0) {
    cout<<"ZinvMaker::makeHybridTemplate  :  could not finde input template"<<endl;
    h_template = 0; // Make sure template isn't used
    return lastbin_hybrid;
  }
  if (hDY == 0 || hZinv == 0) {
    cout<<"ZinvMaker::makeHybridTemplate  :  could not find DY or Zinv MC histogram "<<name<<endl;
    h_template = 0;  // Make sure template isn't used
    return lastbin_hybrid;
  }

  // Rebin all the histograms relevant to the template
  double *TopoRegionBins = h_template->GetXaxis()->GetXbins()->fArray;
  int nTopoRegionBins    = h_template->GetXaxis()->GetNbins();
  TH1D* h_RebinnedTemplate = (TH1D*) hSF->Rebin(nTopoRegionBins, "h_RebinnedTemplate", TopoRegionBins);
  TH1D* hOF_Rebin = 0;
  if (hOF) hOF_Rebin = (TH1D*) hOF->Rebin(nTopoRegionBins, "hOF_Rebin", TopoRegionBins);
  TH1D* hDY_Rebin = (TH1D*) hDY->Rebin(nTopoRegionBins, "hDY_Rebin", TopoRegionBins);
  TH1D* hZinv_Rebin = (TH1D*) hZinv->Rebin(nTopoRegionBins, "hZinv_Rebin", TopoRegionBins);
  if (hOF) h_RebinnedTemplate->Add(hOF_Rebin, -1*rSFOF);



  // find the last bin (hardcoded)
  // for every topological region, just hardcode lastbin_hybrid and lastmt2val_hybrid
  lastmt2val_hybrid = findLastMT2Hardcoded(srname);
  if (lastmt2val_hybrid>0) lastbin_hybrid = hDY_Rebin->GetXaxis()->FindBin(lastmt2val_hybrid+1);

  // find the last bin (flexible)
  if (lastmt2val_hybrid == -1) {
    cout<<"Could not find hardcoded last bin for this region. Counting "<<hybrid_nevent_threshold<<" MC events"<<endl;
    for ( int ibin = hDY_Rebin->GetNbinsX()+1; ibin >= 1; --ibin ) {
      //cout<<hDY_Rebin->Integral(ibin,-1)<<endl;
      if (hDY_Rebin->Integral(ibin,-1) < hybrid_nevent_threshold) continue;
      lastbin_hybrid = ibin;
      lastmt2val_hybrid = hDY_Rebin->GetBinLowEdge(ibin);
    break;
    }
  }

  if (verbose) cout<<lastbin_hybrid<<" "<<lastmt2val_hybrid<<endl;
  //if (verbose) hDY->Print("all");
  //if (verbose) hDY_Rebin->Print("all");
  //if (verbose) hZinv_Rebin->Print("all");
  //if (verbose) h_RebinnedTemplate->Print("all");

  // Get the integrals to normalize the Zinv tails
  // and the uncertainties on the CR yield (dominated by data stats in the last N bins)
  double integratedYieldErrZinv = 0;
  float integratedYieldZinv = hZinv_Rebin->IntegralAndError(lastbin_hybrid, -1., integratedYieldErrZinv);
  float relativeErrorZinv = integratedYieldErrZinv/integratedYieldZinv;
  double integratedYieldErrDY = 0;
  float integratedYieldDY = hDY_Rebin->IntegralAndError(lastbin_hybrid, -1., integratedYieldErrDY);
  float relativeErrorDY = integratedYieldErrDY/integratedYieldDY;
  double integratedYieldErr = 0;
  float integratedYield = h_RebinnedTemplate->IntegralAndError(lastbin_hybrid,-1,integratedYieldErr); 
  float relativeError = integratedYieldErr/integratedYield;

  // Hybridize the template: last N bins have a common stat uncertainty, and they follow the Zinv MC shape
  for ( int ibin=1; ibin <= hZinv_Rebin->GetNbinsX(); ++ibin ) {

    if (ibin < lastbin_hybrid) {
      // (SF-OF)*Zinv/DY
      float cont =  h_RebinnedTemplate->GetBinContent(ibin) * hZinv_Rebin->GetBinContent(ibin) / hDY_Rebin->GetBinContent(ibin);
      float err2 =  pow( h_RebinnedTemplate->GetBinError(ibin) / h_RebinnedTemplate->GetBinContent(ibin),2 )  
	+ pow( hZinv_Rebin->GetBinError(ibin) / hZinv_Rebin->GetBinContent(ibin),2)  
	+ pow( hDY_Rebin->GetBinError(ibin)   / hDY_Rebin->GetBinContent(ibin), 2 );
      
      h_RebinnedTemplate->SetBinContent(ibin, cont);
      h_RebinnedTemplate->SetBinError(ibin, sqrt(err2)*cont);

    }
    else {
      float cont = integratedYield * integratedYieldZinv / integratedYieldDY;
      float err2 = pow( relativeError, 2 ) + pow( relativeErrorZinv, 2 ) + pow( relativeErrorDY, 2 );
      float kMT2 = hZinv_Rebin->GetBinContent(ibin) / integratedYieldZinv;
      h_RebinnedTemplate->SetBinContent(ibin, cont * kMT2);
      h_RebinnedTemplate->SetBinError(ibin, cont*sqrt(err2) * kMT2 );

    }

  }

  // Normalize it: we just need a shape after all
  h_RebinnedTemplate->Scale(1./h_RebinnedTemplate->Integral());

  //h_template->Print("all");

  for ( int ibin=1; ibin <= hZinv_Rebin->GetNbinsX(); ++ibin ) {
    h_template->SetBinContent(ibin, h_RebinnedTemplate->GetBinContent(ibin));
    h_template->SetBinError(ibin, h_RebinnedTemplate->GetBinError(ibin));
  }

  return lastbin_hybrid;
}

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
        //cout<<nGamma<<" "<<nGammaErr<<" "<<errOld<<" "<<errNew<<endl;
        ratioInt->SetBinError(ibin, sqrt( errOld*errOld + errNew*errNew ) );
      }
      else {
        ratioInt->SetBinContent(ibin, 0);
        ratioInt->SetBinError(ibin, 0);
      }
    }
    //ratioInt->Print("all");

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

//  // Obtain inclusive templates
//  int lastmt2_VL23J=0, lastmt2_VL2J=0, lastmt2_VL4J=0;
//  int lastmt2_L23J=0,  lastmt2_L26J=0, lastmt2_L46J=0, lastmt2_L7J=0;
//  int lastmt2_M23J=0,  lastmt2_M26J=0, lastmt2_M46J=0, lastmt2_M7J=0;
//  int lastmt2_H23J=0,  lastmt2_H26J=0, lastmt2_H46J=0, lastmt2_H7J=0;
//  int lastmt2_UH=0;
//  TH1D* h_TemplateVL23J      = (TH1D*) fData->Get("crdybaseVL/h_mt2bins23J"); 
//  int lastbin_VL23J = purityRandNorm(h_TemplateVL23J, "crdybaseVL/h_mt2bins23J", fData, fZinv, fDY, lastmt2_VL23J);
//  TH1D* h_TemplateVL2J       = (TH1D*) fData->Get("crdybaseVL/h_mt2bins"); 
//  int lastbin_VL2J  = purityRandNorm(h_TemplateVL2J, "crdybaseVL/h_mt2bins", fData, fZinv, fDY, lastmt2_VL2J);
//  TH1D* h_TemplateVL4J       = (TH1D*) fData->Get("crdybaseVL/h_mt2bins4J");  
//  int lastbin_VL4J  = purityRandNorm(h_TemplateVL4J, "crdybaseVL/h_mt2bins4J", fData, fZinv, fDY, lastmt2_VL4J);  
//  TH1D* h_TemplateL23J       = (TH1D*) fData->Get("crdybaseL/h_mt2bins23J");
//  int lastbin_L23J  = purityRandNorm(h_TemplateL23J, "crdybaseL/h_mt2bins23J", fData, fZinv, fDY, lastmt2_L23J);    
//  TH1D* h_TemplateL26J       = (TH1D*) fData->Get("crdybaseL/h_mt2bins26J");
//  int lastbin_L26J  = purityRandNorm(h_TemplateL26J, "crdybaseL/h_mt2bins26J", fData, fZinv, fDY, lastmt2_L26J);    
//  TH1D* h_TemplateL46J       = (TH1D*) fData->Get("crdybaseL/h_mt2bins46J");   
//  int lastbin_L46J  = purityRandNorm(h_TemplateL46J, "crdybaseL/h_mt2bins46J", fData, fZinv, fDY, lastmt2_L46J); 
//  TH1D* h_TemplateL7J        = (TH1D*) fData->Get("crdybaseL/h_mt2bins7J");    
//  int lastbin_L7J   = purityRandNorm(h_TemplateL7J, "crdybaseL/h_mt2bins7J", fData, fZinv, fDY, lastmt2_L7J);
//  TH1D* h_TemplateM23J       = (TH1D*) fData->Get("crdybaseM/h_mt2bins23J");    
//  int lastbin_M23J  = purityRandNorm(h_TemplateM23J, "crdybaseM/h_mt2bins23J", fData, fZinv, fDY, lastmt2_M23J);
//  TH1D* h_TemplateM26J       = (TH1D*) fData->Get("crdybaseM/h_mt2bins26J");    
//  int lastbin_M26J  = purityRandNorm(h_TemplateM26J, "crdybaseM/h_mt2bins26J", fData, fZinv, fDY, lastmt2_M26J);
//  TH1D* h_TemplateM46J       = (TH1D*) fData->Get("crdybaseM/h_mt2bins46J");    
//  int lastbin_M46J  = purityRandNorm(h_TemplateM46J, "crdybaseM/h_mt2bins46J", fData, fZinv, fDY, lastmt2_M46J);
//  TH1D* h_TemplateM7J        = (TH1D*) fData->Get("crdybaseM/h_mt2bins7J");    
//  int lastbin_M7J   = purityRandNorm(h_TemplateM7J, "crdybaseM/h_mt2bins7J", fData, fZinv, fDY, lastmt2_M7J);
//  TH1D* h_TemplateH23J       = (TH1D*) fData->Get("crdybaseH/h_mt2bins23J");    
//  int lastbin_H23J  = purityRandNorm(h_TemplateH23J, "crdybaseH/h_mt2bins23J", fData, fZinv, fDY, lastmt2_H23J);
//  TH1D* h_TemplateH26J       = (TH1D*) fData->Get("crdybaseH/h_mt2bins26J");    
//  int lastbin_H26J  = purityRandNorm(h_TemplateH26J, "crdybaseH/h_mt2bins26J", fData, fZinv, fDY, lastmt2_H26J);
//  TH1D* h_TemplateH46J       = (TH1D*) fData->Get("crdybaseH/h_mt2bins46J");    
//  int lastbin_H46J  = purityRandNorm(h_TemplateH46J, "crdybaseH/h_mt2bins46J", fData, fZinv, fDY, lastmt2_H46J);
//  TH1D* h_TemplateH7J        = (TH1D*) fData->Get("crdybaseH/h_mt2bins7J");    
//  int lastbin_H7J   = purityRandNorm(h_TemplateH7J, "crdybaseH/h_mt2bins7J", fData, fZinv, fDY, lastmt2_H7J);
//  TH1D* h_TemplateUH        = (TH1D*) fData->Get("crdybaseUH/h_mt2bins");    
//  int lastbin_UH    = purityRandNorm(h_TemplateUH, "crdybaseUH/h_mt2bins", fData, fZinv, fDY, lastmt2_UH);


  for ( unsigned int idir = 0; idir < ndirs; ++idir ) {
    
    TString srname = dirs.at(idir);
    TString directory = "sr"+dirs.at(idir);
    TString directoryDY = "crdy"+dirs.at(idir);

    TString fullhistname = directory + "/h_mt2bins";
    TString fullhistnameDY = directoryDY + "/h_mt2bins";
    TString fullhistnameEM = directoryDY + "/h_mt2binsemu";

    TH1D* hData = (TH1D*) fData->Get(fullhistnameDY);    
    TH1D* hDY   = (TH1D*) fDY->Get(fullhistnameDY);
    TH1D* hDataEM   = (TH1D*) fData->Get(fullhistnameEM);    
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
    int lastmt2val_hybrid = 200;
    int ht_LOW = 0, ht_HI = 0, njets_LOW = 0, njets_HI = 0, nbjets_LOW = 0, nbjets_HI = 0;
    TH1D* h_MT2Template = (TH1D*) hZinv->Clone("h_MT2Template");  
    TString inclusiveTemplateName = "";
  
    if (doHybridInclusiveTemplate) {
      //  Inclusive template: use inclusive template corresponding to this region
      
      //Get variable boundaries for signal region.
      TH1D* h_ht_LOW = (TH1D*) fZinv->Get(directory+"/h_ht_LOW");
      TH1D* h_ht_HI  = (TH1D*) fZinv->Get(directory+"/h_ht_HI");
      if (h_ht_LOW) ht_LOW = h_ht_LOW->GetBinContent(1);
      if (h_ht_HI)  ht_HI = h_ht_HI->GetBinContent(1);
      TH1D* h_njets_LOW = (TH1D*) fZinv->Get(directory+"/h_njets_LOW");
      TH1D* h_njets_HI  = (TH1D*) fZinv->Get(directory+"/h_njets_HI");
      if (h_njets_LOW) njets_LOW = h_njets_LOW->GetBinContent(1);
      if (h_njets_HI)  njets_HI = h_njets_HI->GetBinContent(1);
      TH1D* h_nbjets_LOW = (TH1D*) fZinv->Get(directory+"/h_nbjets_LOW");
      TH1D* h_nbjets_HI  = (TH1D*) fZinv->Get(directory+"/h_nbjets_HI");
      if (h_nbjets_LOW) nbjets_LOW = h_nbjets_LOW->GetBinContent(1);
      if (h_nbjets_HI)  nbjets_HI = h_nbjets_HI->GetBinContent(1);
      
      //Determine which inclusive template to use. If none works, this reverts to HybridSimple, taking template from its own TopoRegion 
      // Start from the Aggregate Regions (hardcoded, since they can partially overlap with the standard regions)
      if (srname == "20") inclusiveTemplateName = "crdy20/h_mt2bins";  // self (2j, HT1000)
      else if (srname == "21") inclusiveTemplateName = "crdy21/h_mt2bins"; // self (2j, HT1500)
      else if (srname == "22") inclusiveTemplateName = "crdy22/h_mt2bins"; // self (4j, HT1000)
      else if (srname == "23") inclusiveTemplateName = "crdy21/h_mt2bins"; // from 21 
      else if (srname == "24") inclusiveTemplateName = "crdy24/h_mt2bins"; // self (7J, HT1000)
      else if (srname == "25") inclusiveTemplateName = "crdy21/h_mt2bins"; // from 21
      else if (srname == "26") inclusiveTemplateName = "crdy20/h_mt2bins"; // from 20
      else if (srname == "27") inclusiveTemplateName = "crdy21/h_mt2bins"; // from 21
      else if (srname == "28") inclusiveTemplateName = "crdy20/h_mt2bins3J"; // need a 3J histogram within SR20
      //      else if (srname == "28") inclusiveTemplateName = "crdy20/h_mt2bins"; // test
      else if (srname == "29") inclusiveTemplateName = "crdy21/h_mt2bins"; // from 21
      else if (srname == "30") inclusiveTemplateName = "crdy24/h_mt2bins"; // from 24
      else if (srname == "31") inclusiveTemplateName = "crdy21/h_mt2bins"; // from 21
      // Now the standard regions
      else if (ht_LOW == 250) {
	if (njets_LOW == 2 && nbjets_LOW==3) inclusiveTemplateName = "crdybaseVL/h_mt2bins3J"; 
	else if (njets_LOW == 2 && njets_HI==4) inclusiveTemplateName = "crdybaseVL/h_mt2bins23J"; 
	else if (njets_LOW == 2)  inclusiveTemplateName = "crdybaseVL/h_mt2bins"; 
	else if (njets_LOW == 4)  inclusiveTemplateName = "crdybaseVL/h_mt2bins4J"; 
      }
      else if (ht_LOW == 450) {
	if (njets_LOW == 2 && nbjets_LOW==3) inclusiveTemplateName = "crdybaseL/h_mt2bins36J"; 
	else if (njets_LOW == 2 && njets_HI==7)  inclusiveTemplateName = "crdybaseL/h_mt2bins26J"; 
	else if (njets_LOW == 2)  inclusiveTemplateName = "crdybaseL/h_mt2bins23J"; 
	else if (njets_LOW == 4)  inclusiveTemplateName = "crdybaseL/h_mt2bins46J"; 
	else if (njets_LOW == 7)  inclusiveTemplateName = "crdybaseL/h_mt2bins7J"; 
      }
      else if (ht_LOW == 575) {
	if (njets_LOW == 2 && nbjets_LOW==3) inclusiveTemplateName = "crdybaseM/h_mt2bins36J"; 
	else if (njets_LOW == 2 && njets_HI==7) inclusiveTemplateName = "crdybaseM/h_mt2bins26J"; 
	else if (njets_LOW == 2)  inclusiveTemplateName = "crdybaseM/h_mt2bins23J"; 
	else if (njets_LOW == 4)  inclusiveTemplateName = "crdybaseM/h_mt2bins46J"; 
	else if (njets_LOW == 7)  inclusiveTemplateName = "crdybaseM/h_mt2bins7J"; 
      }
      else if (ht_LOW == 1000) {
	if (njets_LOW == 2 && nbjets_LOW==3) inclusiveTemplateName = "crdybaseH/h_mt2bins36J"; 
	else if (njets_LOW == 2 && njets_HI==7)  inclusiveTemplateName = "crdybaseH/h_mt2bins26J"; 
	else if (njets_LOW == 2)  inclusiveTemplateName = "crdybaseH/h_mt2bins23J";
	else if (njets_LOW == 4)  inclusiveTemplateName = "crdybaseH/h_mt2bins46J";
	else if (njets_LOW == 7)  inclusiveTemplateName = "crdybaseH/h_mt2bins7J";
      }
      else if (ht_LOW == 1500) inclusiveTemplateName = "crdybaseUH/h_mt2bins";

      if (inclusiveTemplateName == "") {
	cout<< "Can't find template for region: HT "<<ht_LOW<<"-"<<ht_HI<<" and NJ "<<njets_LOW<<"-"<<njets_HI<<endl;
	lastbin_hybrid = 1;
	h_MT2Template = 0;
	cout<<"h_MT2Template is 0, using hybrid within this region (no external templates)"<<endl;
      }
      else {
	cout<< "Using inclusive template "<<inclusiveTemplateName<<" for region: HT "<<ht_LOW<<"-"<<ht_HI<<" and NJ "<<njets_LOW<<"-"<<njets_HI<<endl; 
	// Get the template
	lastbin_hybrid = makeHybridTemplate(srname, h_MT2Template, inclusiveTemplateName , fData, fZinv, fDY, lastmt2val_hybrid);
	cout<<"lastbin_hybrid "<<lastbin_hybrid<<" and lastmt2val_hybrid "<<lastmt2val_hybrid<<endl;
	if (h_MT2Template!=0) h_MT2Template->Print("all");
	else cout<<"h_MT2Template is 0, using hybrid within this region (no external templates)"<<endl;
      }

   


    }

  


    // If there is no template for this region, go back to the standard hybrid
    if (doHybridSimple || (doHybridInclusiveTemplate && h_MT2Template == 0)) {
      // hybrid method: use nominal MC CR yield histogram to determine how many MT2 bins to use
      //  by default: use all MT2 bins integrated (no bin-by-bin).
      //  choose the last bin to try to have at least hybrid_nevent_threshold integrated events

      // Calculate last bin on local histogram
      for ( int ibin=1; ibin <= hDY->GetNbinsX(); ++ibin ) {
	float top = 0, integratedYield = 0;
	//if (hDataEM) top = hDataEM->Integral(ibin,-1)*rSFOF;
	integratedYield = hDY->Integral(ibin,-1) - top;
	if (integratedYield < hybrid_nevent_threshold) {
	  if (ibin == 1) lastbin_hybrid = 1;
	  else {
	    lastbin_hybrid = ibin-1;
	    lastmt2val_hybrid = hDY->GetBinLowEdge(ibin);
	  }
	  break;
	}
      }
      cout<<"lastbin_hybrid for doHybridSimple: "<<lastbin_hybrid<<endl;
    }
    
    

    TH1D* ratio = (TH1D*) hZinv->Clone("ratio");
    ratio->Divide(hDY);
    float ratioValue = hZinv->Integral() / hDY->Integral();

    TH1D* CRyield = (TH1D*) hData->Clone("h_mt2binsCRyield");

    TH1D* purityMC = (TH1D*) hDY->Clone("h_mt2binsPurityMC");
    if (hTop) purityMC->Add(hTop, -1);
    purityMC->Divide(hDY);

    TH1D* purityData = (TH1D*) hData->Clone("h_mt2binsPurityData");
    if (hDataEM) purityData->Add(hDataEM, -1*rSFOF); 
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
    
    if (  doHybridSimple || (doHybridInclusiveTemplate && h_MT2Template==0) ) { 
      // purity needs to describe the integrated purity of the CR
      // ratio needs to be modified so that the last N bins include kMT2
      // CRyield needs to be modified so that the last N bins have the same yield (which is the integral over those N bins)
      if (verbose) cout<<" Implementing simple hybrid "<<endl;

      for ( int ibin=1; ibin <= hZinv->GetNbinsX(); ++ibin ) {

	if (ibin < lastbin_hybrid) continue;
	
	double integratedYieldErr = 0;
	float integratedYield = CRyield->IntegralAndError(lastbin_hybrid,-1,integratedYieldErr);
	CRyieldCard->SetBinContent(ibin, integratedYield);
	CRyieldCard->SetBinError(ibin, integratedYieldErr);
	
	float integratedDen = integratedYield;
	float EM = 0;
	if (hDataEM) EM =  hDataEM->Integral(lastbin_hybrid, -1) * rSFOF;
	float integratedNum = integratedDen - EM;
	if (integratedNum < 0) integratedNum = 0;
	float integratedPurity = integratedNum/integratedDen;
	float integratedPurityErr = sqrt(integratedPurity*(1-integratedPurity)/integratedDen);// sqrt(e(1-e)/N)   
	purityCard->SetBinContent(ibin, integratedPurity);
	purityCard->SetBinError(ibin, integratedPurityErr);

	float integratedZinv = 1;
	float kMT2 = 0;
	integratedZinv = hZinv->Integral(lastbin_hybrid, -1);
	kMT2 = hZinv->GetBinContent(ibin) / integratedZinv;
	ratioCard->SetBinContent(ibin, ratioCard->GetBinContent(ibin) * kMT2);
	ratioCard->SetBinError(ibin, ratioCard->GetBinError(ibin) * kMT2 ); // just rescale the error by the same amount

      }
    } 
    else if (doHybridInclusiveTemplate && h_MT2Template!=0) {
      // For Inclusive template: 
      // CRyield: this is flat, just the integral
      // purity: also flat
      // ratio: this contains the normalized template scaled by the Zinv/DY ratio for this control region
      // Template should be rebinned to match this region. lastbin_hybrid will have to be updated to refer to the new binning
      
      if (verbose) cout<<" Implementing hybrid with template "<<endl;

//      // MT2template needs to be rebinned for this topological region Rebin
//      double *TopoRegionBins = hZinv->GetXaxis()->GetXbins()->fArray;
//      int nTopoRegionBins    = hZinv->GetXaxis()->GetNbins();
//      h_MT2Template->Print("all");
//      TH1D* h_MT2TemplateRebin = (TH1D*) h_MT2Template->Rebin(nTopoRegionBins, "h_MT2TemplateRebin", TopoRegionBins);
//      int newlastbin = h_MT2TemplateRebin->FindBin(lastmt2val_hybrid-1);
//      if (newlastbin == 0) newlastbin = 1; // 1 is the lowest
//      if (verbose) cout<<"Rebinning moved lastbin_hybrid from "<<lastbin_hybrid<<" to "<<newlastbin<<". lastmt2val_hybrid="<<lastmt2val_hybrid<<endl;
//      lastbin_hybrid = newlastbin;
//      if (verbose) h_MT2TemplateRebin->Print("all");
      if (verbose) cout<<"Ratio for this control region is "<<ratioValue<<endl;

      double integratedYieldErr = 0;
      float integratedYield = CRyield->IntegralAndError(0,-1,integratedYieldErr);
      
      float integratedDen = integratedYield;
      float EM = 0;
      if (hDataEM) EM =  hDataEM->Integral(0, -1)*rSFOF;
      float integratedNum = integratedDen - EM;
      if (integratedNum < 0) integratedNum = 0;
      float integratedPurity = integratedNum/integratedDen;
      float integratedPurityErr = sqrt(integratedPurity*(1-integratedPurity)/integratedDen);// sqrt(e(1-e)/N)
      if (verbose) cout<<"Found SF="<<integratedYield<<" and OF="<<EM<<", so purity is "<<integratedPurity<<endl;

      for ( int ibin=1; ibin <= hZinv->GetNbinsX(); ++ibin ) {
	CRyieldCard->SetBinContent(ibin, integratedYield);
	CRyieldCard->SetBinError(ibin,   integratedYieldErr);
	ratioCard->SetBinContent(ibin, ratioValue * h_MT2Template->GetBinContent(ibin));
	ratioCard->SetBinError(ibin,   ratioValue * h_MT2Template->GetBinError(ibin));
	purityCard->SetBinContent(ibin, integratedPurity);
	purityCard->SetBinError(ibin,   integratedPurityErr);
      }

    }

    TH1D* hybridEstimate  = (TH1D*) CRyieldCard->Clone("hybridEstimate");
    hybridEstimate->Multiply(purityCard);
    hybridEstimate->Multiply(ratioCard);
    
    
    TH1D* h_lastbinHybrid = new TH1D("h_lastbinHybrid",";last bin",1,0,1);
    h_lastbinHybrid->SetBinContent(1,lastbin_hybrid);
    TH1D* h_lastmt2Hybrid = new TH1D("h_lastmt2Hybrid",";last M_{T2} value",1,0,1);
    h_lastmt2Hybrid->SetBinContent(1,lastmt2val_hybrid);

    TH1D* CRyieldEM = 0;
    if (hDataEM) CRyieldEM = (TH1D*) hDataEM->Clone("h_mt2binsCRyieldEM");


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
    h_lastmt2Hybrid->Write();
    if (hDataEM) CRyieldEM->Write();


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

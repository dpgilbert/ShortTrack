#include <cmath>
#include <iostream>
#include <sstream>

#ifndef __CINT__
#include "RooGlobalFunc.h"
#endif
#include "RooRealVar.h"
#include "RooDataSet.h"
#include "RooDataHist.h"
#include "RooHistPdf.h"
#include "RooAddPdf.h"
#include "RooPlot.h"

#include "TCanvas.h"
#include "TAxis.h"
#include "TH2D.h"
#include "TH1D.h"
#include "TRandom3.h"
#include "TEfficiency.h"
#include "TGraphAsymmErrors.h"

#include "../MT2CORE/mt2tree.h"
#include "../MT2CORE/sigSelections.h"
#include "../MT2CORE/SR.h"
#include "../MT2CORE/sigSelections.cc"
#include "../MT2CORE/SR.cc"

using namespace RooFit;
using namespace std;
using namespace mt2;

struct Purity {

  float purity;
  float purityErrUp;
  float purityErrDown;

};

int fitPurity();
void fitPuritySR( TFile* f_gjet, TFile*  f_bkg, TFile* f_data, TFile*  f_out, SR sr, TH1D* h_templPrompt, TH1D* h_templFake );
void fitSinglePurity( const std::string& outputdir, const std::string& srName, Purity& loose, Purity& tight, RooRealVar* x, RooDataSet* data, TH1D* h1_templPrompt, TH1D* h1_templFake );
void checkBoundaries( Purity& p );
TH1D* sameBin(TH1D* h_in);
std::string toString(float in);


//float lumi = 4.; // fb-1

int fitPurity(){
//int main( int argc, char* argv[] ) {

  std::string input_dir = "/home/users/mderdzinski/summer2015/MT2Analysis/MT2looper/output/";

  // std::string mc_or_data = "DataRC";
  // if( argc>1 ) {
  //   mc_or_data = std::string(argv[1]);
  // }

  // if( mc_or_data=="data" ) mc_or_data="DataRC";
  // if( mc_or_data=="dataRC" ) mc_or_data="DataRC";
  // if( mc_or_data=="mc" ) mc_or_data="MC";

  // if( argc>2 ) {
  //   input_dir = std::string(argv[2]);
  // }

  // // std::string regionsSet = "zurich";
  // // if( argc>2 ) {
  // //   regionsSet = std::string(argv[2]);
  // // }

  TH1::AddDirectory(kFALSE);

  //load signal regions
  vector<SR> SRVec =  getSignalRegionsZurich_jetpt40();
  
  //open files
  TFile* f_g = new TFile(Form("%s/gjet_ht.root",input_dir.c_str())); //gjet file
  TFile* f_QCDpluGJET = new TFile(Form("%s/qcdplusgjet.root",input_dir.c_str())); //qcd+gjets file
  TFile* f_data = new TFile(Form("%s/gjet_ht.root",input_dir.c_str())); //data file, FIXME: replace with real data eventually...
  // TFile* f_z = new TFile(Form("%s/zinvFromGJ.root",input_dir.c_str())); //zinv pred from ZinvMaker.C, contains ratio
  if(f_g->IsZombie() || f_QCDpluGJET->IsZombie() || f_data->IsZombie()) {
    std::cerr << "Input file does not exist" << std::endl;
    return 1;
  }
  
  //instantiate output file here
  TFile* f_out = new TFile(Form("%s/fitPurity.root",input_dir.c_str()),"RECREATE");

  //get template histograms
  TH1D* h_templPrompt = (TH1D*) f_g->Get("crgjbase/h_chisoLoose");
  TH1D* h_templFake = (TH1D*) f_QCDpluGJET->Get("crgjbase/h_chisoFakeLooseSieieSB");

  //write templates
  h_templPrompt->Write();
  h_templFake->Write();
  
  //loop over SRs
  for(int i = 0; i< (int) SRVec.size(); i++){ fitPuritySR( f_g, f_QCDpluGJET, f_data, f_out, SRVec[i], h_templPrompt, h_templFake ); }
  
  f_out->Close();
  
  return 0;

}


void fitPuritySR( TFile* f_gjet, TFile*  f_bkg, TFile* f_data, TFile*  f_out, SR sr, TH1D* h_templPrompt, TH1D* h_templFake ) {

  //sr name
  TString srName = sr.GetName();
  
  //get number of mt2bins for this SR
  const int n_mt2bins = sr.GetNumberOfMT2Bins();

  //get names of mt2bins
  vector<TString> mt2binsname;
  for(int i=0; i<=n_mt2bins; i++){ mt2binsname.push_back(toString(sr.GetMT2Bins()[i])); }
    
  //rooRealVar for fit
  RooRealVar* x = new RooRealVar( "x", "", 0., 50.);
  
  //make new hist to save purity for this SR
  TH1D* h_all = (TH1D*) f_data->Get("crgj"+srName+"/h_mt2bins");

  if (!h_all){ cout << "Missing h_mt2bins for region " << srName << ", skipping..." << endl; return;}
  
  TH1D* h_purityLoose          = sameBin(h_all);
  TH1D* h_purityLoose_systUp   = sameBin(h_all);
  TH1D* h_purityLoose_systDown = sameBin(h_all);
  TH1D* h_purityTight          = sameBin(h_all);
  TH1D* h_purityTight_systUp   = sameBin(h_all);
  TH1D* h_purityTight_systDown = sameBin(h_all);
  
  h_purityLoose          ->SetName("h_purityLoose");
  h_purityLoose_systUp   ->SetName("h_purityLoose_systUp");
  h_purityLoose_systDown ->SetName("h_purityLoose_systDown");
  h_purityTight          ->SetName("h_purityTight");
  h_purityTight_systUp   ->SetName("h_purityTight_systUp");
  h_purityTight_systDown ->SetName("h_purityTight_systDown");
  
  //output directory
  //const std::string outputdir = Form("rooFit/crgj%s", srName.Data());
  const std::string outputdir = "rooFit";

  // //get template histograms, integrated over mt2
  // TH1D* h_templPrompt = (TH1D*) f_gjet->Get("crgj"+srName+"/h_chisoLoose");
  // TH1D* h_templFake = (TH1D*) f_bkg->Get("crgj"+srName+"/h_chisoFakeLooseSieieSB");

  //loop over mt2bins, calculate the purity for that bin
  for( int i=0; i<n_mt2bins; ++i ) {
    
    //get template histograms per bin
    // TH1D* h_templPrompt = (TH1D*) f_gjet->Get("crgj"+srName+"/h_chiso_mt2bin"+mt2binsname[i]+"Loose");
    // TH1D* h_templFake = (TH1D*) f_bkg->Get("crgj"+srName+"/h_chiso_mt2bin"+mt2binsname[i]+"FakeLooseSieieSB");

    //data to fit to
    RooDataSet* rds_data = (RooDataSet*) f_data->Get("crgj"+srName+"/rds_chIso_"+mt2binsname[i]+"Loose");

    if (!h_templPrompt || !h_templFake || !rds_data ){ cout << "Missing hists for region " << srName << mt2binsname[i] << ", skipping..." << endl; continue;}

    Purity loose, tight;
    fitSinglePurity( outputdir, srName.Data(), loose, tight, x, rds_data, h_templPrompt, h_templFake );

    int ibin = i+1;
    
    h_purityLoose->SetBinContent( ibin, loose.purity );
    h_purityLoose_systUp->SetBinContent( ibin, loose.purity + loose.purityErrUp );
    h_purityLoose_systDown->SetBinContent( ibin, loose.purity - loose.purityErrDown );

    h_purityTight->SetBinContent( ibin, tight.purity );
    h_purityTight_systUp->SetBinContent( ibin, tight.purity + tight.purityErrUp );
    h_purityTight_systDown->SetBinContent( ibin, tight.purity - tight.purityErrDown );

  }

  //make and fill prediction histogram
  f_out->cd();
  TString directory = "sr"+srName;
  TDirectory* dir = 0;
  dir = (TDirectory*)f_out->Get(directory.Data());
  if (dir == 0) {
    dir = f_out->mkdir(directory.Data());
  } 
  dir->cd();

  //save and write purity to file
  h_purityLoose          ->Write();
  h_purityLoose_systUp   ->Write();
  h_purityLoose_systDown ->Write();
  h_purityTight          ->Write();
  h_purityTight_systUp   ->Write();
  h_purityTight_systDown ->Write();
  
  return;

}



void fitSinglePurity( const std::string& outputdir, const std::string& srName, Purity& loose, Purity& tight, RooRealVar* x, RooDataSet* data, TH1D* h1_templPrompt, TH1D* h1_templFake ) {


  float dataIntegral = data->sumEntries();
  float thresh = 2.5;
  float data_pass = data->sumEntries(Form("x<%f", thresh));

  if( dataIntegral == 0. ) {
    loose.purity=-1;
    loose.purityErrUp=0.;
    loose.purityErrDown=0.;
    tight.purity=-1;
    tight.purityErrUp=0.;
    tight.purityErrDown=0.;
    return;
  }



  RooDataHist templPrompt("templPrompt", "", *x, h1_templPrompt);
  RooDataHist templFake  ("templFake"  , "", *x, h1_templFake  );

  RooHistPdf pdfPrompt("pdfPrompt", "", *x, templPrompt );
  RooHistPdf pdfFake  ("pdfFake"  , "", *x, templFake   );

  RooRealVar sigFrac("promptFrac","fraction of prompt",0.9,0.,1.) ;
  RooAddPdf  model("model","", RooArgList(pdfPrompt,pdfFake), sigFrac) ;

  int nBins = h1_templPrompt->GetNbinsX();
  float xMin = h1_templPrompt->GetXaxis()->GetXmin();
  float xMax = h1_templPrompt->GetXaxis()->GetXmax();

  float xMaxFit = 0.9999*xMax;
  x->setRange( "fittingRange", 0., xMaxFit );
  model.fitTo(*data, SumW2Error(kTRUE), Minos(kTRUE), Range("fittingRange")); 

  loose.purity = sigFrac.getVal();
  loose.purityErrUp = sigFrac.getErrorHi();
  loose.purityErrDown = -sigFrac.getErrorLo();

  int cutBin = h1_templPrompt->FindBin(thresh) - 1;

  float sigEff = h1_templPrompt->Integral(1, cutBin)/h1_templPrompt->Integral(1,nBins);
  tight.purity        = sigEff * loose.purity        * dataIntegral / data_pass;
  tight.purityErrUp   = sigEff * loose.purityErrUp   * dataIntegral / data_pass;
  tight.purityErrDown = sigEff * loose.purityErrDown * dataIntegral / data_pass;

  //float sigPassCut = sigFrac.getVal()*sigEff;
  //float bgEff = h1_templFake->Integral(1, cutBin)/h1_templFake->Integral(1,nBins);
  //float bgPassCut = (1.-sigFrac.getVal())*bgEff;
  //tight.purity = sigPassCut / (sigPassCut+bgPassCut);
  //tight.purityErrUp = loose.purityErrUp; // is it ok to assign the same error also to the tight purity?
  //tight.purityErrDown = loose.purityErrDown;
  ////float factor = tight.purity/loose.purity;
  ////tight.purityErrUp = loose.purityErrUp*factor;
  ////tight.purityErrDown = loose.purityErrDown*factor;

  checkBoundaries( loose );
  checkBoundaries( tight );

  RooPlot* xframe = x->frame();
  data->plotOn(xframe, Binning(nBins, xMin, xMax));
  model.plotOn(xframe);

  // Overlay the background component of model with a dashed line
  model.plotOn(xframe,Components(pdfFake),LineStyle(kDashed)) ;

  TCanvas* c1 = new TCanvas("c1", "", 600, 600);
  gPad->SetLeftMargin(0.15);
  TH2D* h2_axes = new TH2D("axes", "", 10, 0., xMaxFit, 10, 0., xframe->GetMaximum()*1.1 );
  h2_axes->Draw();
  xframe->GetYaxis()->SetTitleOffset(1.4); 
  xframe->Draw("same");

  c1->SaveAs(Form("%s/singleFits/purityFit_%s_%s.eps", outputdir.c_str(), srName.c_str(), data->GetName()));
  c1->SaveAs(Form("%s/singleFits/purityFit_%s_%s.png", outputdir.c_str(), srName.c_str(), data->GetName()));
  c1->SaveAs(Form("%s/singleFits/purityFit_%s_%s.pdf", outputdir.c_str(), srName.c_str(), data->GetName()));

  delete c1;
  delete xframe;

  return;

}



void checkBoundaries( Purity& p ) {

  if( p.purity > 1. ) p.purity = 1.;
  if( p.purity < 0. ) p.purity = 0.;
  if( p.purity - p.purityErrDown < 0. ) p.purityErrDown = p.purity;
  if( p.purity + p.purityErrUp   > 1. ) p.purityErrUp   = 1. - p.purity;

}

//returns an empty hist with same binning
inline TH1D* sameBin(TH1D* h_in)
{
  TH1D* h_out = (TH1D*) h_in->Clone();
  h_out->Reset();
  return h_out;
}

//converts float to string
std::string toString(float in){
  std::stringstream ss;
  ss << in;
  return ss.str();
}

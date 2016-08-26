#include <iostream>
#include <utility>
#include <vector>
#include <string>
#include <cmath>
#include <algorithm>

#include "TROOT.h"
#include "TLatex.h"
#include "TString.h"
#include "TObjArray.h"
#include "TH2.h"
#include "THStack.h"
#include "TLegend.h"
#include "TCanvas.h"
#include "TFile.h"
#include "TDirectory.h"
#include "TH1.h"
#include "TH3.h"
#include "TPaveText.h"
#include "TStyle.h"
#include "TKey.h"


//__________________________________________________
void sigContamMaker(TString dir, TString sample) {

  TFile* fin = new TFile(dir+"/"+sample+".root","READ");
  TFile* flostlep = new TFile(dir+"/lostlepFromCRs.root","READ");
  TFile* fout = new TFile(dir+"/"+sample+"_sigcontam.root","RECREATE");
  
  TIter dir_it(fin->GetListOfKeys());
  TKey* dir_k;
  while ((dir_k = (TKey *)dir_it())) {
    if (TString(dir_k->GetClassName()) != "TDirectoryFile") continue;
    TString dir_name = dir_k->GetTitle();
    if(dir_name == "") continue;
    if(!dir_name.Contains("sr")) continue;
    if(dir_name.Contains("base")) continue;
    TDirectory* din = (TDirectory*) dir_k->ReadObj();
    if(din == 0) continue;
    //std::cout << "dir_name: " << dir_name << std::endl;
    TDirectory* dout = fout->mkdir(dir_name);

    // copy only essential histograms for signal yields and systematics
    TIter hist_it(din->GetListOfKeys());
    TKey* hist_k;
    while ((hist_k = (TKey *)hist_it())) {
      TString hist_name = (hist_k->GetName());
      // 3d signal hists
      if (TString(hist_k->GetClassName()) == "TH3D") {
	TH3D* h = (TH3D*)hist_k->ReadObj();
	dout->WriteObject(h,hist_name);
      }
      // 1d yield hist
      else if (hist_name == "h_mt2bins" && TString(hist_k->GetClassName()) == "TH1D") {
	TH1D* h = (TH1D*)hist_k->ReadObj();
	dout->WriteObject(h,hist_name);
      }
    }// loop on hists

    // get 1 lepton cr yields for signal, save hist
    TString dir_name_crsl = TString(dir_name).ReplaceAll("sr","crsl");
    TH3D* h_mt2bins_sigscan_crsl = (TH3D*) fin->Get(dir_name_crsl+"/h_mt2bins_sigscan");
    if (h_mt2bins_sigscan_crsl) {
      h_mt2bins_sigscan_crsl->SetName("h_mt2bins_sigscan_crsl");
      dout->WriteObject(h_mt2bins_sigscan_crsl,"h_mt2bins_sigscan_crsl");
    }
    TH3D* h_mt2bins_sigscan_crsl_genmet = (TH3D*) fin->Get(dir_name_crsl+"/h_mt2bins_sigscan_genmet");
    if (h_mt2bins_sigscan_crsl_genmet) {
      h_mt2bins_sigscan_crsl_genmet->SetName("h_mt2bins_sigscan_crsl_genmet");
      dout->WriteObject(h_mt2bins_sigscan_crsl_genmet,"h_mt2bins_sigscan_crsl_genmet");
    }
    TH1D* h_mt2bins_crsl = (TH1D*) fin->Get(dir_name_crsl+"/h_mt2bins");
    if (h_mt2bins_crsl) {
      h_mt2bins_crsl->SetName("h_mt2bins_crsl");
      dout->WriteObject(h_mt2bins_crsl,"h_mt2bins_crsl");
    }
    TH1D* h_mt2bins_crsl_genmet = (TH1D*) fin->Get(dir_name_crsl+"/h_mt2bins_genmet");
    if (h_mt2bins_crsl_genmet) {
      h_mt2bins_crsl_genmet->SetName("h_mt2bins_crsl_genmet");
      dout->WriteObject(h_mt2bins_crsl_genmet,"h_mt2bins_crsl_genmet");
    }

    // get transfer factor from lostlep file, save hist
    TH1D* h_mt2bins_alpha = (TH1D*) flostlep->Get(dir_name+"/h_mt2binsAlpha");
    if (h_mt2bins_alpha) {
      h_mt2bins_alpha->SetName("h_mt2bins_alpha");
      dout->WriteObject(h_mt2bins_alpha,"h_mt2bins_alpha");
    }
    
    // also save bin boundary hists for CR
    TH1D* h_ht_LOW = (TH1D*) fin->Get(dir_name+"/h_ht_LOW");
    TH1D* h_ht_HI = (TH1D*) fin->Get(dir_name+"/h_ht_HI");
    TH1D* h_nbjets_LOW = (TH1D*) fin->Get(dir_name+"/h_nbjets_LOW");
    TH1D* h_nbjets_HI = (TH1D*) fin->Get(dir_name+"/h_nbjets_HI");
    TH1D* h_njets_LOW = (TH1D*) fin->Get(dir_name+"/h_njets_LOW");
    TH1D* h_njets_HI = (TH1D*) fin->Get(dir_name+"/h_njets_HI");

    dout->cd();
    h_ht_LOW->Write();
    h_ht_HI->Write();
    h_nbjets_LOW->Write();
    h_nbjets_HI->Write();
    h_njets_LOW->Write();
    h_njets_HI->Write();
      
  }// loop on dirs

  fin->Close();
  flostlep->Close();
  fout->Close();

  delete fin;
  delete flostlep;
  delete fout;
  
  return;
}

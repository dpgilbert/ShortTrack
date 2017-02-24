#include "applyWeights.h"

#include <cstdlib>

//_________________________________________________________
// return dummy SF and uncertainties for leptons
//  could eventually read in a histogram or text file to get values
weightStruct getLepSF(float pt, float, int pdgId) {

  weightStruct weights;

  if (abs(pdgId) == 11) {
    if (pt < 20.) { 
      weights.cent = 0.95;
      weights.up = 1.00;
      weights.dn = 0.90;
    }
    else {
      weights.cent = 0.99;
      weights.up = 1.01;
      weights.dn = 0.97;
    }
  }
  else if (abs(pdgId) == 13) {
    if (pt < 20.) { 
      weights.cent = 0.95;
      weights.up = 1.00;
      weights.dn = 0.90;
    }
    else {
      weights.cent = 0.99;
      weights.up = 1.01;
      weights.dn = 0.97;
    }
  }

  return weights;
}

//_________________________________________________________
bool setElSFfile(TString filenameIDISO, TString filenameTRK, bool useTight){
  TFile * f1 = new TFile(filenameIDISO);
  TFile * f2 = new TFile(filenameTRK);
  if (!f1->IsOpen()) std::cout<<"applyWeights::setElSFfile: ERROR: Could not find scale factor file "<<filenameIDISO<<std::endl;
  if (!f2->IsOpen()) std::cout<<"applyWeights::setElSFfile: ERROR: Could not find track scale factor file "<<filenameTRK<<std::endl;
  TH2D* h_id = 0;
  if (useTight) h_id = (TH2D*) f1->Get("GsfElectronToCutBasedSpring15T");
  else h_id = (TH2D*) f1->Get("GsfElectronToCutBasedSpring15V");
  TH2D* h_iso = (TH2D*) f1->Get("MVAVLooseElectronToMini");
  TH2D* h_trk = (TH2D*) f2->Get("EGamma_SF2D");
  if (!h_id || !h_iso || !h_trk) std::cout<<"applyWeights::setElSFfile: ERROR: Could not find scale factor histogram"<<std::endl;
  h_elSF = (TH2D*) h_id->Clone("h_elSF");
  h_elSF->SetDirectory(0);
  h_elSF->Multiply(h_iso);
  //h_elSF->Print("all");
  h_elSF_trk = (TH2D*) h_trk->Clone("h_elSF_trk");
  h_elSF_trk->SetDirectory(0);
  return true;
}

//_________________________________________________________
bool setMuSFfile(TString filenameID, TString filenameISO, TString filenameIP, TString filenameTRK){
  TFile * f1 = new TFile(filenameID);
  TFile * f2 = new TFile(filenameISO);
  TFile * f3 = new TFile(filenameIP);
  TFile * f4 = new TFile(filenameTRK);
  if (!f1->IsOpen()) { std::cout<<"applyWeights::setMuSFfile: ERROR: Could not find ID scale factor file "<<filenameID<<std::endl; return 0;}
  if (!f2->IsOpen()) { std::cout<<"applyWeights::setMuSFfile: ERROR: Could not find ISO scale factor file "<<filenameISO<<std::endl; return 0;}
  if (!f3->IsOpen()) { std::cout<<"applyWeights::setMuSFfile: ERROR: Could not find IP scale factor file "<<filenameIP<<std::endl; return 0;}
  if (!f4->IsOpen()) { std::cout<<"applyWeights::setMuSFfile: ERROR: Could not find TRK scale factor file "<<filenameTRK<<std::endl; return 0;}
  TH2D* h_id = (TH2D*) f1->Get("SF");
  TH2D* h_iso = (TH2D*) f2->Get("SF");
  TH2D* h_ip = (TH2D*) f3->Get("SF");
  TH1D* h_trk_ptlt10 = (TH1D*) f4->Get("ratio_eff_eta3_tk0_dr030e030_corr");
  TH1D* h_trk_ptgt10 = (TH1D*) f4->Get("ratio_eff_eta3_dr030e030_corr");
  if (!h_id || !h_iso || !h_ip || !h_trk_ptlt10 || !h_trk_ptgt10) { std::cout<<"applyWeights::setMuSFfile: ERROR: Could not find scale factor histogram"<<std::endl; return 0;}
  h_muSF = (TH2D*) h_id->Clone("h_muSF");
  h_muSF->SetDirectory(0);
  h_muSF->Multiply(h_iso);
  h_muSF->Multiply(h_ip);
  //h_muSF->Print("all");
  h_muSF_trk_ptlt10 = (TH1D*) h_trk_ptlt10->Clone("h_muSF_trk_ptlt10");
  h_muSF_trk_ptlt10->SetDirectory(0);
  h_muSF_trk_ptgt10 = (TH1D*) h_trk_ptgt10->Clone("h_muSF_trk_ptgt10");
  h_muSF_trk_ptgt10->SetDirectory(0);
  return true;
}

//_________________________________________________________
bool setMuSFfileNoTrk(TString filenameID, TString filenameISO, TString filenameIP){
  TFile * f1 = new TFile(filenameID);
  TFile * f2 = new TFile(filenameISO);
  TFile * f3 = new TFile(filenameIP);
  if (!f1->IsOpen()) { std::cout<<"applyWeights::setMuSFfileNoTrk: ERROR: Could not find ID scale factor file "<<filenameID<<std::endl; return 0;}
  if (!f2->IsOpen()) { std::cout<<"applyWeights::setMuSFfileNoTrk: ERROR: Could not find ISO scale factor file "<<filenameISO<<std::endl; return 0;}
  if (!f3->IsOpen()) { std::cout<<"applyWeights::setMuSFfileNoTrk: ERROR: Could not find IP scale factor file "<<filenameIP<<std::endl; return 0;}
  TH2D* h_id = (TH2D*) f1->Get("SF");
  TH2D* h_iso = (TH2D*) f2->Get("SF");
  TH2D* h_ip = (TH2D*) f3->Get("SF");
  if (!h_id || !h_iso || !h_ip) { std::cout<<"applyWeights::setMuSFfileNoTrk: ERROR: Could not find scale factor histogram"<<std::endl; return 0;}
  h_muSF = (TH2D*) h_id->Clone("h_muSF");
  h_muSF->SetDirectory(0);
  h_muSF->Multiply(h_iso);
  h_muSF->Multiply(h_ip);
  //h_muSF->Print("all");
  return true;
}

//_________________________________________________________
weightStruct getLepSFFromFile(float pt, float eta, int pdgId) {

  weightStruct weights;

  //  if(!h_elSF || !h_muSF || !h_elSF_trk || !h_muSF_trk_ptgt10 || !h_muSF_trk_ptlt10) {
  if(!h_elSF || !h_muSF || !h_elSF_trk) {
    std::cout << "applyWeights::getLepSFFromFile: ERROR: missing input hists" << std::endl
	      << "h_elSF: " << h_elSF << ", h_elSF_trk: " << h_elSF_trk << ", h_muSF: " << h_muSF << std::endl;
    return weights;
  }

  float pt_cutoff = std::max(10.1,std::min(100.,double(pt)));
  float eta_cutoff = std::max(-2.39,std::min(2.39,double(eta)));

  if (abs(pdgId) == 11) {
    int binx = h_elSF->GetXaxis()->FindBin(pt_cutoff);
    int biny = h_elSF->GetYaxis()->FindBin(fabs(eta_cutoff));
    float central = h_elSF->GetBinContent(binx,biny);
    float err  = h_elSF->GetBinError(binx,biny);
    // get also trk sf
    int binx_trk = h_elSF_trk->GetXaxis()->FindBin(eta_cutoff);
    int biny_trk = 1; // hardcoding for now - only one bin in pt (hist starts at 20)
    central *= h_elSF_trk->GetBinContent(binx_trk,biny_trk);
    float trk_err = h_elSF_trk->GetBinError(binx_trk,biny_trk);
    if (pt_cutoff < 20. || pt_cutoff > 80.) err = sqrt(err*err + trk_err*trk_err + 0.01*0.01); // extra 1% error for pt < 20 OR pt > 80
    else err = sqrt(err*err + trk_err*trk_err); // otherwise just use histogram error
    weights.cent = central;
    weights.up = central+err;
    weights.dn = central-err;
    if (central > 1.3 || central < 0.3) 
      std::cout<<"STRANGE: Electron with pT/eta of "<<pt_cutoff<<"/"<<eta_cutoff<<". SF is "<<weights.cent<<" pm "<<err<<std::endl;
  }
  else if (abs(pdgId) == 13) {
    int binx = h_muSF->GetXaxis()->FindBin(pt_cutoff);
    int biny = h_muSF->GetYaxis()->FindBin(fabs(eta_cutoff));
    float central = h_muSF->GetBinContent(binx,biny);
    // get also trk sf, if present
    if (h_muSF_trk_ptgt10 && h_muSF_trk_ptlt10) {
      if (pt > 10) central *= h_muSF_trk_ptgt10->GetBinContent(h_muSF_trk_ptgt10->FindBin(eta_cutoff));
      else central *= h_muSF_trk_ptlt10->GetBinContent(h_muSF_trk_ptlt10->FindBin(eta_cutoff));
    }
    float err  = 0.03; // suggested 3% uncert
    weights.cent = central;
    weights.up = central+err;
    weights.dn = central-err;
    if (central > 1.2 || central < 0.8) 
      std::cout<<"STRANGE: Muon with pT/eta of "<<pt_cutoff<<"/"<<eta_cutoff<<". SF is "<<weights.cent<<" pm "<<err<<std::endl;
  }

  return weights;
}


//_________________________________________________________
bool setElSFfile_fastsim(TString filename){
  TFile * f = new TFile(filename);
  if (!f->IsOpen()) std::cout<<"applyWeights::setElSFfile_fastsim: ERROR: Could not find scale factor file "<<filename<<std::endl;
  TH2D* h_eff = (TH2D*) f->Get("histo2D");
  if (!h_eff) std::cout<<"applyWeights::setElSFfile_fastsim: ERROR: Could not find scale factor histogram"<<std::endl;
  h_elSF_fastsim = (TH2D*) h_eff->Clone("h_elSF_fastsim");
  h_elSF_fastsim->SetDirectory(0);
  //h_elSF_fastsim->Print("all");
  return true;
}

//_________________________________________________________
bool setMuSFfile_fastsim(TString filenameID, TString filenameISO, TString filenameIP){
  TFile * f1 = new TFile(filenameID);
  TFile * f2 = new TFile(filenameISO);
  TFile * f3 = new TFile(filenameIP);
  if (!f1->IsOpen()) { std::cout<<"applyWeights::setMuSFfile: ERROR: Could not find ID scale factor file "<<filenameID<<std::endl; return 0;}
  if (!f2->IsOpen()) { std::cout<<"applyWeights::setMuSFfile: ERROR: Could not find ISO scale factor file "<<filenameISO<<std::endl; return 0;}
  if (!f3->IsOpen()) { std::cout<<"applyWeights::setMuSFfile: ERROR: Could not find IP scale factor file "<<filenameIP<<std::endl; return 0;}
  TH2D* h_id = (TH2D*) f1->Get("histo2D");
  TH2D* h_iso = (TH2D*) f2->Get("histo2D");
  TH2D* h_ip = (TH2D*) f3->Get("histo2D");
  if (!h_id || !h_iso || !h_ip) { std::cout<<"applyWeights::setMuSFfile_fastsim: ERROR: Could not find scale factor histogram"<<std::endl; return 0;}
  h_muSF_fastsim = (TH2D*) h_id->Clone("h_muSF_fastsim");
  h_muSF_fastsim->SetDirectory(0);
  h_muSF_fastsim->Multiply(h_iso);
  h_muSF_fastsim->Multiply(h_ip);
  //h_muSF_fastsim->Print("all");
  return true;
}

//_________________________________________________________
weightStruct getLepSFFromFile_fastsim(float pt, float eta, int pdgId) {

  weightStruct weights;

  if(!h_elSF_fastsim || !h_muSF_fastsim) {
    std::cout << "applyWeights::getLepSFFromFile_fastsim: ERROR: missing input hists" << std::endl;
    return weights;
  }

  float pt_cutoff = std::max(10.1,std::min(100.,double(pt)));
  float eta_cutoff = std::max(-2.39,std::min(2.39,double(eta)));

  if (abs(pdgId) == 11) {
    int binx = h_elSF_fastsim->GetXaxis()->FindBin(pt_cutoff);
    int biny = h_elSF_fastsim->GetYaxis()->FindBin(fabs(eta_cutoff));
    float central = h_elSF_fastsim->GetBinContent(binx,biny);
    float err  = 0.02; // 2% for all pt
    weights.cent = central;
    weights.up = central+err;
    weights.dn = central-err;
    if (central > 1.3 || central < 0.7) 
      std::cout<<"STRANGE: Electron with pT/eta of "<<pt_cutoff<<"/"<<eta_cutoff<<". SF is "<<weights.cent<<" pm "<<err<<std::endl;
  }
  else if (abs(pdgId) == 13) {
    int binx = h_muSF_fastsim->GetXaxis()->FindBin(pt_cutoff);
    int biny = h_muSF_fastsim->GetYaxis()->FindBin(fabs(eta_cutoff));
    float central = h_muSF_fastsim->GetBinContent(binx,biny);
    float err  = 0.02; // 2% for all pt
    weights.cent = central;
    weights.up = central+err;
    weights.dn = central-err;
    if (central > 1.3 || central < 0.7) 
      std::cout<<"STRANGE: Muon with pT/eta of "<<pt_cutoff<<"/"<<eta_cutoff<<". SF is "<<weights.cent<<" pm "<<err<<std::endl;
  }

  return weights;
}

//_________________________________________________________
bool setVetoEffFile_fullsim(TString filename){
  TFile * f = new TFile(filename);
  if (!f->IsOpen()) std::cout<<"applyWeights::setVetoEffFile_fullsim: ERROR: Could not find vetoeff file "<<filename<<std::endl;
  TH2D* h_eff_el = (TH2D*) f->Get("h_ele_comb_eff");
  if (!h_eff_el) std::cout<<"applyWeights::setVetoEffFile_fullsim: ERROR: Could not find vetoeff histogram"<<std::endl;
  h_elVetoEff_fullsim = (TH2D*) h_eff_el->Clone("h_elVetoEff_fullsim");
  h_elVetoEff_fullsim->SetDirectory(0);
  //h_elVetoEff_fullsim->Print("all");

  TH2D* h_eff_mu = (TH2D*) f->Get("h_mu_comb_eff");
  if (!h_eff_mu) { std::cout<<"applyWeights::setMuVetoEffFile_fullsim: ERROR: Could not find vetoeff histogram"<<std::endl; return 0;}
  h_muVetoEff_fullsim = (TH2D*) h_eff_mu->Clone("h_muVetoEff_fullsim");
  h_muVetoEff_fullsim->SetDirectory(0);
  //h_muVetoEff_fullsim->Print("all");

  f->Close();
  delete f;
  return true;
}

//_________________________________________________________
float getLepVetoEffFromFile_fullsim(float pt, float eta, int pdgId) {

  if(!h_elVetoEff_fullsim || !h_muVetoEff_fullsim) {
    std::cout << "applyWeights::getLepVetoEffFromFile_fullsim: ERROR: missing input hists" << std::endl;
    return 0.;
  }

  float pt_cutoff = std::max(5.1,std::min(100.,double(pt)));

  if (abs(pdgId) == 11) {
    int binx = h_elVetoEff_fullsim->GetXaxis()->FindBin(pt_cutoff);
    int biny = h_elVetoEff_fullsim->GetYaxis()->FindBin(fabs(eta));
    return h_elVetoEff_fullsim->GetBinContent(binx,biny);
  }
  else if (abs(pdgId) == 13) {
    int binx = h_muVetoEff_fullsim->GetXaxis()->FindBin(pt_cutoff);
    int biny = h_muVetoEff_fullsim->GetYaxis()->FindBin(fabs(eta));
    return h_muVetoEff_fullsim->GetBinContent(binx,biny);
  }

  return 0.;
}

//_________________________________________________________
bool setVetoEffFile_fastsim(TString filename){
  TFile * f = new TFile(filename);
  if (!f->IsOpen()) std::cout<<"applyWeights::setVetoEffFile_fastsim: ERROR: Could not find vetoeff file "<<filename<<std::endl;
  TH2D* h_eff_el = (TH2D*) f->Get("h_ele_comb_eff");
  if (!h_eff_el) std::cout<<"applyWeights::setVetoEffFile_fastsim: ERROR: Could not find vetoeff histogram"<<std::endl;
  h_elVetoEff_fastsim = (TH2D*) h_eff_el->Clone("h_elVetoEff_fastsim");
  h_elVetoEff_fastsim->SetDirectory(0);
  //h_elVetoEff_fastsim->Print("all");

  TH2D* h_eff_mu = (TH2D*) f->Get("h_mu_comb_eff");
  if (!h_eff_mu) { std::cout<<"applyWeights::setMuVetoEffFile_fastsim: ERROR: Could not find vetoeff histogram"<<std::endl; return 0;}
  h_muVetoEff_fastsim = (TH2D*) h_eff_mu->Clone("h_muVetoEff_fastsim");
  h_muVetoEff_fastsim->SetDirectory(0);
  //h_muVetoEff_fastsim->Print("all");

  f->Close();
  delete f;
  return true;
}

//_________________________________________________________
float getLepVetoEffFromFile_fastsim(float pt, float eta, int pdgId) {

  if(!h_elVetoEff_fastsim || !h_muVetoEff_fastsim) {
    std::cout << "applyWeights::getLepVetoEffFromFile_fastsim: ERROR: missing input hists" << std::endl;
    return 0.;
  }

  float pt_cutoff = std::max(5.1,std::min(100.,double(pt)));

  if (abs(pdgId) == 11) {
    int binx = h_elVetoEff_fastsim->GetXaxis()->FindBin(pt_cutoff);
    int biny = h_elVetoEff_fastsim->GetYaxis()->FindBin(fabs(eta));
    return h_elVetoEff_fastsim->GetBinContent(binx,biny);
  }
  else if (abs(pdgId) == 13) {
    int binx = h_muVetoEff_fastsim->GetXaxis()->FindBin(pt_cutoff);
    int biny = h_muVetoEff_fastsim->GetYaxis()->FindBin(fabs(eta));
    return h_muVetoEff_fastsim->GetBinContent(binx,biny);
  }

  return 0.;
}


//_________________________________________________________
weightStruct getBtagSF(float pt, float, int pdgId) {

  weightStruct weights;

  // put in some dummy values for b,c quarks, based on 8 TeV
  if (abs(pdgId) == 5) {
    // dummy values for b
    weights.cent = 0.95;
    if (pt < 30 || pt > 250) {
      weights.up = 1.00;
      weights.dn = 0.90;
    }
    else if (pt > 30 && pt < 250) {
      weights.up = 0.97;
      weights.dn = 0.93;
    }
  }
  else if (abs(pdgId) == 4) {
    // dummy values for c
    weights.cent = 0.95;
    if (pt < 30 || pt > 250) {
      weights.up = 1.05;
      weights.dn = 0.85;
    }
    else if (pt > 30 && pt < 250) {
      weights.up = 0.99;
      weights.dn = 0.91;
    }
  }
  else {
    // dummy values for light
    weights.cent = 1.05;
    weights.up = 1.15;
    weights.dn = 0.95;
  }

  return weights;

}


//_________________________________________________________
float getBtagEff(float pt, float, int pdgId) {

  // dummy efficiencies for each flavor based on 8 TeV
  float eff;

  if (abs(pdgId) == 5) {
    eff = 0.7;
  }
  else if (abs(pdgId) == 4) {
    eff = 0.17;
  }
  else {
    eff = 0.015;
  }

  return eff;

}


float getPhotonTriggerWeight(float eta, float pt){

  float SF;

  if ( fabs(eta)<1.479 ) {
    if (pt > 150 && pt < 160) SF = 0.02913004;
    else if (pt < 170) SF = 0.2979024;
    else if (pt < 180) SF = 0.9168265;
    else if (pt < 200) SF = 0.9346871;
    else if (pt < 250) SF = 0.9498014;
    else if (pt < 300) SF = 0.9503671;
    else if (pt < 350) SF = 0.9266015;
    else if (pt < 400) SF = 0.9366893;
    else if (pt < 450) SF = 0.9451516;
    else if (pt < 500) SF = 0.9467028;
    else if (pt < 600) SF = 0.9480487;
    else if (pt < 700) SF = 0.9501481;
    else if (pt < 800) SF = 0.9634304;
    else SF = 0.9398705;
  }else{
    if (pt > 150 && pt < 160) SF = 0.02942861;
    else if (pt < 170) SF = 0.1139842;
    else if (pt < 180) SF = 0.6114576;
    else if (pt < 200) SF = 0.8858802;
    else if (pt < 250) SF = 0.9609917;
    else if (pt < 300) SF = 0.9371325;
    else if (pt < 350) SF = 0.9273918;
    else if (pt < 400) SF = 0.9442088;
    else if (pt < 450) SF = 0.9526445;
    else if (pt < 500) SF = 0.9331003;
    else if (pt < 600) SF = 0.9604496;
    else if (pt < 700) SF = 0.9525379;
    else if (pt < 800) SF = 0.9785377;
    else SF = 0.9763883;
  }

  return SF;
}

float getDileptonTriggerWeight(float pt1, int pdgId1, float pt2, int pdgId2, int unc) {

  float SF;
  // based on https://indico.cern.ch/event/592150/attachments/1380789/2099040/dileptrigeff_olivito_301116.pdf
  if      (abs(pdgId1) == 13 && abs(pdgId2) == 13 ) {
    SF = (unc == 0) ? 0.97 : (unc == -1 ? 0.94 : 1.00);
  }
  else if (abs(pdgId1) == 11 && abs(pdgId2) == 11 ) {
    if ( pt1 < 180 && pt2 < 35) SF = (unc == 0) ? 0.91 : (unc == -1 ? 0.84 : 1.00);
    else SF = (unc == 0) ? 1.00 : (unc == -1 ? 0.97 : 1.00);
  }
  else {
    if ( pt1 < 180) SF = (unc == 0) ? 0.92 : (unc == -1 ? 0.89 : 0.95);
    else SF = (unc == 0) ? 0.97 : (unc == -1 ? 0.92 : 1.00);
  }

  return SF;

}

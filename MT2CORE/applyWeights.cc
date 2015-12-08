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
bool setElSFfile(TString filename){
  TFile * f = new TFile(filename);
  if (!f->IsOpen()) std::cout<<"applyWeights::setElSFfile: ERROR: Could not find scale factor file "<<filename<<std::endl;
  TH2D* h_id = (TH2D*) f->Get("CutBasedVeto");
  TH2D* h_iso = (TH2D*) f->Get("MiniIso0p1_vs_AbsEta");
  if (!h_id || !h_iso) std::cout<<"applyWeights::setElSFfile: ERROR: Could not find scale factor histogram"<<std::endl;
  h_elSF = (TH2D*) h_id->Clone("h_elSF");
  h_elSF->SetDirectory(0);
  h_elSF->Multiply(h_iso);
  //h_elSF->Print("all");
  return true;
}

//_________________________________________________________
bool setMuSFfile(TString filenameID, TString filenameISO){
  TFile * f1 = new TFile(filenameID);
  TFile * f2 = new TFile(filenameISO);
  if (!f1->IsOpen()) { std::cout<<"applyWeights::setElSFfile: ERROR: Could not find ID scale factor file "<<filenameID<<std::endl; return 0;}
  if (!f2->IsOpen()) { std::cout<<"applyWeights::setElSFfile: ERROR: Could not find ISO scale factor file "<<filenameISO<<std::endl; return 0;}
  TH2D* h_id = (TH2D*) f1->Get("pt_abseta_PLOT_pair_probeMultiplicity_bin0_&_tag_combRelIsoPF04dBeta_bin0_&_tag_pt_bin0_&_tag_IsoMu20_pass");
  TH2D* h_iso = (TH2D*) f2->Get("pt_abseta_PLOT_pair_probeMultiplicity_bin0_&_tag_combRelIsoPF04dBeta_bin0_&_tag_pt_bin0_&_PF_pass_&_tag_IsoMu20_pass");
  if (!h_id || !h_iso) { std::cout<<"applyWeights::setMuSFfile: ERROR: Could not find scale factor histogram"<<std::endl; return 0;}
  h_muSF = (TH2D*) h_id->Clone("h_muSF");
  h_muSF->SetDirectory(0);
  h_muSF->Multiply(h_iso);
  //h_muSF->Print("all");
  return true;
}

//_________________________________________________________
weightStruct getLepSFFromFile(float pt, float eta, int pdgId) {

  weightStruct weights;

  if(!h_elSF || !h_muSF) {
    std::cout << "applyWeights::getLepSFFromFile: ERROR: missing input hists" << std::endl;
    return weights;
  }

  float pt_cutoff = std::max(10.1,std::min(100.,double(pt)));

  if (abs(pdgId) == 11) {
    int binx = h_elSF->GetXaxis()->FindBin(pt_cutoff);
    int biny = h_elSF->GetYaxis()->FindBin(fabs(eta));
    float central = h_elSF->GetBinContent(binx,biny);
    float err  = h_elSF->GetBinError(binx,biny);
    weights.cent = central;
    weights.up = central+err;
    weights.dn = central-err;
    if (central > 1.2 || central < 0.8) 
      std::cout<<"STRANGE: Electron with pT/eta of "<<pt_cutoff<<"/"<<eta<<". SF is "<<weights.cent<<" pm "<<err<<std::endl;
  }
  else if (abs(pdgId) == 13) {
    int binx = h_muSF->GetXaxis()->FindBin(pt_cutoff);
    int biny = h_muSF->GetYaxis()->FindBin(fabs(eta));
    float central = h_muSF->GetBinContent(binx,biny);
    float err  = 0.014; // adding in quadrature 1% unc. on ID and 1% unc. on ISO
    weights.cent = central;
    weights.up = central+err;
    weights.dn = central-err;
    if (central > 1.2 || central < 0.8) 
      std::cout<<"STRANGE: Muon with pT/eta of "<<pt_cutoff<<"/"<<eta<<". SF is "<<weights.cent<<" pm "<<err<<std::endl;
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
bool setMuSFfile_fastsim(TString filename){
  TFile * f = new TFile(filename);
  if (!f->IsOpen()) { std::cout<<"applyWeights::setMuSFfile_fastsim: ERROR: Could not find scale factor file "<<filename<<std::endl; return 0;}
  TH2D* h_eff = (TH2D*) f->Get("histo2D");
  if (!h_eff) { std::cout<<"applyWeights::setMuSFfile_fastsim: ERROR: Could not find scale factor histogram"<<std::endl; return 0;}
  h_muSF_fastsim = (TH2D*) h_eff->Clone("h_muSF_fastsim");
  h_muSF_fastsim->SetDirectory(0);
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

  if (abs(pdgId) == 11) {
    int binx = h_elSF_fastsim->GetXaxis()->FindBin(pt_cutoff);
    int biny = h_elSF_fastsim->GetYaxis()->FindBin(fabs(eta));
    float central = h_elSF_fastsim->GetBinContent(binx,biny);
    float err  = 0.05; // 5% for all pt
    weights.cent = central;
    weights.up = central+err;
    weights.dn = central-err;
    if (central > 1.3 || central < 0.7) 
      std::cout<<"STRANGE: Electron with pT/eta of "<<pt_cutoff<<"/"<<eta<<". SF is "<<weights.cent<<" pm "<<err<<std::endl;
  }
  else if (abs(pdgId) == 13) {
    int binx = h_muSF_fastsim->GetXaxis()->FindBin(pt_cutoff);
    int biny = h_muSF_fastsim->GetYaxis()->FindBin(fabs(eta));
    float central = h_muSF_fastsim->GetBinContent(binx,biny);
    float err  = 0.01; // 1% for pt > 20
    if (pt_cutoff < 20.) err = 0.03; // 3% for pt < 20
    weights.cent = central;
    weights.up = central+err;
    weights.dn = central-err;
    if (central > 1.3 || central < 0.7) 
      std::cout<<"STRANGE: Muon with pT/eta of "<<pt_cutoff<<"/"<<eta<<". SF is "<<weights.cent<<" pm "<<err<<std::endl;
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

#ifndef APPLYWEIGHTS_H
#define APPLYWEIGHTS_H
#include <iostream>
#include "TString.h"
#include "TFile.h"
#include "TH1D.h"
#include "TH2.h"

struct weightStruct {
  float cent; // central
  float up;
  float dn; // down
};

weightStruct getLepSF(float pt, float eta, int pdgId);
weightStruct getBtagSF(float pt, float eta, int pdgId);
float getBtagEff(float pt, float eta, int pdgId);


TH2D* h_elSF = 0;
TH2D* h_muSF = 0;
bool setElSFfile(TString filename);
bool setMuSFfile(TString filenameID, TString filenameISO);
weightStruct getLepSFFromFile(float pt, float eta, int pdgId);


#endif

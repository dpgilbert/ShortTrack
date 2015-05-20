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

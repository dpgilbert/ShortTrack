#ifndef APPLYWEIGHTS_H
#define APPLYWEIGHTS_H

struct weightStruct {
  float cent; // central
  float up;
  float dn; // down
};

weightStruct getLepSF(float pt, float eta, int pdgId);
weightStruct getBtagSF(float pt, float eta, int pdgId);
float getBtagEff(float pt, float eta, int pdgId);

#endif

#ifndef SELECTIONS_H
#define SELECTIONS_H
#include "TString.h"

bool isLoosePFJet(unsigned int pfJetIdx);
bool isMediumPFJet(unsigned int pfJetIdx);
bool isTightPFJet(unsigned int pfJetIdx);
bool isVetoElectron(unsigned int);
bool isLooseElectron(unsigned int);
bool isMediumElectron(unsigned int);
bool isTightElectron(unsigned int);
bool isLooseMuon(unsigned int);
bool isTightMuon(unsigned int);
int tightChargeEle(unsigned int);
int tightChargeMuon(unsigned int);
float muRelIso03(unsigned int);
float muRelIso04(unsigned int);
float eleRelIso03(unsigned int);
int eleTightID(unsigned int);
int muTightID(unsigned int);
TString triggerName(TString);
bool passHLTTriggerPattern(const char*);


#endif

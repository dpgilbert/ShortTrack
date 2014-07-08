#ifndef SELECTIONS_H
#define SELECTIONS_H

bool passesLoosePFJetID(unsigned int pfJetIdx);
bool isVetoElectron(unsigned int);
bool isLooseElectron(unsigned int);
bool isMediumElectron(unsigned int);
bool isTightElectron(unsigned int);
bool isLooseMuon(unsigned int);
bool threeChargeAgree(unsigned int);
float muRelIso03(unsigned int);
float muRelIso04(unsigned int);

#endif

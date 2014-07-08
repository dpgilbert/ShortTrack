#include "selections.h"
#include "CMS2.h"


bool passesLoosePFJetID(unsigned int pfJetIdx){

    float pfjet_chf_  = cms2.pfjets_chargedHadronE()[pfJetIdx] / cms2.pfjets_p4()[pfJetIdx].energy();
    float pfjet_nhf_  = cms2.pfjets_neutralHadronE()[pfJetIdx] / cms2.pfjets_p4()[pfJetIdx].energy();
    float pfjet_cef_  = cms2.pfjets_chargedEmE()[pfJetIdx] / cms2.pfjets_p4()[pfJetIdx].energy();
    float pfjet_nef_  = cms2.pfjets_neutralEmE()[pfJetIdx] / cms2.pfjets_p4()[pfJetIdx].energy();
    int   pfjet_cm_   = cms2.pfjets_chargedMultiplicity()[pfJetIdx];

    if (cms2.pfjets_pfcandIndicies().size() < 2)
        return false;
    if (pfjet_nef_ >= 0.99)
        return false;
    if (pfjet_nhf_ >= 0.99)
        return false;
    if (pfjet_cm_ < 1)
        return false;
    if (pfjet_chf_ < 1e-6)
        return false;
    if (pfjet_cef_ >= 0.99)
        return false;

    return true;
}

//2012 Electron IDs
//https://twiki.cern.ch/twiki/bin/viewauth/CMS/EgammaCutBasedIdentification#Electron_ID_Working_Points
bool isVetoElectron(unsigned int elIdx){

  if(fabs(cms2.els_etaSC().at(elIdx)) <= 1.479){
    if(fabs(cms2.els_dEtaIn().at(elIdx)) >= 0.007) return false; 
    if(fabs(cms2.els_dPhiIn().at(elIdx)) >= 0.8) return false; 
    if(cms2.els_sigmaIEtaIEta().at(elIdx) >= 0.01) return false; 
    if(cms2.els_hOverE().at(elIdx) >= 0.15) return false; 
    if(fabs(cms2.els_dxyPV().at(elIdx)) >= 0.04) return false; //is this wrt the correct PV?
    if(fabs(cms2.els_dzPV().at(elIdx)) >= 0.2) return false; //is this wrt the correct PV?
    // if(reliso >= 0.15) return false; //still need to implement this
    return true;

  } else if((fabs(cms2.els_etaSC().at(elIdx)) > 1.479) && (fabs(cms2.els_etaSC().at(elIdx)) < 2.5)){
    if(fabs(cms2.els_dEtaIn().at(elIdx)) >= 0.01) return false; 
    if(fabs(cms2.els_dPhiIn().at(elIdx)) >= 0.7) return false; 
    if(cms2.els_sigmaIEtaIEta().at(elIdx) >= 0.03) return false; 
    if(fabs(cms2.els_dxyPV().at(elIdx)) >= 0.04) return false; //is this wrt the correct PV?
    if(fabs(cms2.els_dzPV().at(elIdx)) >= 0.2) return false; //is this wrt the correct PV?
    // if(reliso >= 0.15) return false; //still need to implement this
    return true;

  } else return false;

}

bool isLooseElectron(unsigned int elIdx){

  if(fabs(cms2.els_etaSC().at(elIdx)) <= 1.479){
    if(fabs(cms2.els_dEtaIn().at(elIdx)) >= 0.007) return false; 
    if(fabs(cms2.els_dPhiIn().at(elIdx)) >= 0.15) return false; 
    if(cms2.els_sigmaIEtaIEta().at(elIdx) >= 0.01) return false; 
    if(cms2.els_hOverE().at(elIdx) >= 0.12) return false; 
    if(fabs(cms2.els_dxyPV().at(elIdx)) >= 0.02) return false; //is this wrt the correct PV?
    if(fabs(cms2.els_dzPV().at(elIdx)) >= 0.2) return false; //is this wrt the correct PV?
    if( fabs( (1.0/cms2.els_ecalEnergy().at(elIdx)) - (cms2.els_eOverPIn().at(elIdx)/cms2.els_ecalEnergy().at(elIdx)) ) >= 0.05) return false; // |1/E - 1/p|
    // if(reliso >= 0.15) return false; //still need to implement this
    //Conversion rejection: vertex fit probability 1e-6 //need to add
    //Conversion rejection: missing hits 1 //need to add
    return true;

  } else if((fabs(cms2.els_etaSC().at(elIdx)) > 1.479) && (fabs(cms2.els_etaSC().at(elIdx)) < 2.5)){
    if(fabs(cms2.els_dEtaIn().at(elIdx)) >= 0.009) return false; 
    if(fabs(cms2.els_dPhiIn().at(elIdx)) >= 0.10) return false; 
    if(cms2.els_sigmaIEtaIEta().at(elIdx) >= 0.03) return false; 
    if(cms2.els_hOverE().at(elIdx) >= 0.10) return false; 
    if(fabs(cms2.els_dxyPV().at(elIdx)) >= 0.02) return false; //is this wrt the correct PV?
    if(fabs(cms2.els_dzPV().at(elIdx)) >= 0.2) return false; //is this wrt the correct PV?
    if( fabs( (1.0/cms2.els_ecalEnergy().at(elIdx)) - (cms2.els_eOverPIn().at(elIdx)/cms2.els_ecalEnergy().at(elIdx)) ) >= 0.05) return false; // |1/E - 1/p|
    // if(reliso >= 0.15) return false; //still need to implement this
    //Conversion rejection: vertex fit probability 1e-6 //need to add
    //Conversion rejection: missing hits 1 //need to add
    return true;

  } else return false;

}

bool isMediumElectron(unsigned int elIdx){

  if(fabs(cms2.els_etaSC().at(elIdx)) <= 1.479){
    if(fabs(cms2.els_dEtaIn().at(elIdx)) >= 0.004) return false; 
    if(fabs(cms2.els_dPhiIn().at(elIdx)) >= 0.06) return false; 
    if(cms2.els_sigmaIEtaIEta().at(elIdx) >= 0.01) return false; 
    if(cms2.els_hOverE().at(elIdx) >= 0.12) return false; 
    if(fabs(cms2.els_dxyPV().at(elIdx)) >= 0.02) return false; //is this wrt the correct PV?
    if(fabs(cms2.els_dzPV().at(elIdx)) >= 0.1) return false; //is this wrt the correct PV?
    if( fabs( (1.0/cms2.els_ecalEnergy().at(elIdx)) - (cms2.els_eOverPIn().at(elIdx)/cms2.els_ecalEnergy().at(elIdx)) ) >= 0.05) return false; // |1/E - 1/p|
    // if(reliso >= 0.15) return false; //still need to implement this
    //Conversion rejection: vertex fit probability 1e-6 //need to add
    //Conversion rejection: missing hits 1 //need to add
    return true;

  } else if((fabs(cms2.els_etaSC().at(elIdx)) > 1.479) && (fabs(cms2.els_etaSC().at(elIdx)) < 2.5)){
    if(fabs(cms2.els_dEtaIn().at(elIdx)) >= 0.007) return false; 
    if(fabs(cms2.els_dPhiIn().at(elIdx)) >= 0.03) return false; 
    if(cms2.els_sigmaIEtaIEta().at(elIdx) >= 0.03) return false; 
    if(cms2.els_hOverE().at(elIdx) >= 0.10) return false; 
    if(fabs(cms2.els_dxyPV().at(elIdx)) >= 0.02) return false; //is this wrt the correct PV?
    if(fabs(cms2.els_dzPV().at(elIdx)) >= 0.1) return false; //is this wrt the correct PV?
    if( fabs( (1.0/cms2.els_ecalEnergy().at(elIdx)) - (cms2.els_eOverPIn().at(elIdx)/cms2.els_ecalEnergy().at(elIdx)) ) >= 0.05) return false; // |1/E - 1/p|
    // if(reliso >= 0.15) return false; //still need to implement this
    //Conversion rejection: vertex fit probability 1e-6 //need to add
    //Conversion rejection: missing hits 1 //need to add
    return true;

  } else return false;

}

bool isTightElectron(unsigned int elIdx){

  if(fabs(cms2.els_etaSC().at(elIdx)) <= 1.479){
    if(fabs(cms2.els_dEtaIn().at(elIdx)) >= 0.004) return false; 
    if(fabs(cms2.els_dPhiIn().at(elIdx)) >= 0.03) return false; 
    if(cms2.els_sigmaIEtaIEta().at(elIdx) >= 0.01) return false; 
    if(cms2.els_hOverE().at(elIdx) >= 0.12) return false; 
    if(fabs(cms2.els_dxyPV().at(elIdx)) >= 0.02) return false; //is this wrt the correct PV?
    if(fabs(cms2.els_dzPV().at(elIdx)) >= 0.1) return false; //is this wrt the correct PV?
    if( fabs( (1.0/cms2.els_ecalEnergy().at(elIdx)) - (cms2.els_eOverPIn().at(elIdx)/cms2.els_ecalEnergy().at(elIdx)) ) >= 0.05) return false; // |1/E - 1/p|
    // if(reliso >= 0.10) return false; //still need to implement this
    //Conversion rejection: vertex fit probability 1e-6 //need to add
    //Conversion rejection: missing hits 0 //need to add
    return true;

  } else if((fabs(cms2.els_etaSC().at(elIdx)) > 1.479) && (fabs(cms2.els_etaSC().at(elIdx)) < 2.5)){
    if(fabs(cms2.els_dEtaIn().at(elIdx)) >= 0.005) return false; 
    if(fabs(cms2.els_dPhiIn().at(elIdx)) >= 0.02) return false; 
    if(cms2.els_sigmaIEtaIEta().at(elIdx) >= 0.03) return false; 
    if(cms2.els_hOverE().at(elIdx) >= 0.10) return false; 
    if(fabs(cms2.els_dxyPV().at(elIdx)) >= 0.02) return false; //is this wrt the correct PV?
    if(fabs(cms2.els_dzPV().at(elIdx)) >= 0.1) return false; //is this wrt the correct PV?
    if( fabs( (1.0/cms2.els_ecalEnergy().at(elIdx)) - (cms2.els_eOverPIn().at(elIdx)/cms2.els_ecalEnergy().at(elIdx)) ) >= 0.05) return false; // |1/E - 1/p|
    // if(reliso >= 0.10) return false; //still need to implement this
    //Conversion rejection: vertex fit probability 1e-6 //need to add
    //Conversion rejection: missing hits 0 //need to add
    return true;

  } else return false;

}


bool isLooseMuon(unsigned int muIdx){

  if(!cms2.mus_pid_PFMuon().at(muIdx)) return false; 
   
  bool isGlobal  = true;
  bool isTracker = true;
  if (((cms2.mus_type().at(muIdx)) & (1<<1)) == 0) isGlobal  = false;
  if (((cms2.mus_type().at(muIdx)) & (1<<2)) == 0) isTracker = false;

  if(!(isGlobal || isTracker)) return false;
  
  return true;

}


bool threeChargeAgree(unsigned int elIdx){

  if(cms2.els_charge().at(elIdx) != cms2.els_trk_charge().at(elIdx)) return false;
  if(cms2.els_charge().at(elIdx) != cms2.els_sccharge().at(elIdx))   return false;
  return true;

}

float muRelIso03(unsigned int muIdx){

  float chiso     = cms2.mus_isoR03_pf_ChargedHadronPt().at(muIdx);
  float nhiso     = cms2.mus_isoR03_pf_NeutralHadronEt().at(muIdx);
  float emiso     = cms2.mus_isoR03_pf_PhotonEt().at(muIdx);
  float deltaBeta = cms2.mus_isoR03_pf_PUPt().at(muIdx);
  float absiso = chiso + std::max(0.0, nhiso + emiso - 0.5 * deltaBeta);
  return absiso/(cms2.mus_p4().at(muIdx).pt());

}

float muRelIso04(unsigned int muIdx){

  float chiso     = cms2.mus_isoR04_pf_ChargedHadronPt().at(muIdx);
  float nhiso     = cms2.mus_isoR04_pf_NeutralHadronEt().at(muIdx);
  float emiso     = cms2.mus_isoR04_pf_PhotonEt().at(muIdx);
  float deltaBeta = cms2.mus_isoR04_pf_PUPt().at(muIdx);
  float absiso = chiso + std::max(0.0, nhiso + emiso - 0.5 * deltaBeta);
  return absiso/(cms2.mus_p4().at(muIdx).pt());

}

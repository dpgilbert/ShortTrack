#include "selections.h"
#include "CMS2.h"

using namespace tas;

bool isLoosePFJet(unsigned int pfJetIdx){

    float pfjet_chf_  = cms2.pfjets_chargedHadronE()[pfJetIdx] / cms2.pfjets_p4()[pfJetIdx].energy();
    float pfjet_nhf_  = cms2.pfjets_neutralHadronE()[pfJetIdx] / cms2.pfjets_p4()[pfJetIdx].energy();
    float pfjet_cef_  = cms2.pfjets_chargedEmE()[pfJetIdx] / cms2.pfjets_p4()[pfJetIdx].energy();
    float pfjet_nef_  = cms2.pfjets_neutralEmE()[pfJetIdx] / cms2.pfjets_p4()[pfJetIdx].energy();
    int   pfjet_cm_   = cms2.pfjets_chargedMultiplicity()[pfJetIdx];
    float pfjet_eta   = fabs(cms2.pfjets_p4()[pfJetIdx].eta());

    if (cms2.pfjets_pfcandIndicies().size() < 2)
        return false;
    if (pfjet_nef_ >= 0.99)
        return false;
    if (pfjet_nhf_ >= 0.99)
        return false;

    if (pfjet_eta < 2.4){
      if (pfjet_cm_ < 1)
          return false;
      if (pfjet_chf_ < 1e-6)
          return false;
      if (pfjet_cef_ >= 0.99)
          return false;
    }

    return true;
}

bool isMediumPFJet(unsigned int pfJetIdx){


    float pfjet_nhf_  = cms2.pfjets_neutralHadronE()[pfJetIdx] / cms2.pfjets_p4()[pfJetIdx].energy();
    float pfjet_nef_  = cms2.pfjets_neutralEmE()[pfJetIdx] / cms2.pfjets_p4()[pfJetIdx].energy();

    if (pfjet_nef_ >= 0.95)
        return false;
    if (pfjet_nhf_ >= 0.95)
        return false;

    if (!isLoosePFJet(pfJetIdx)) return false;

    return true;
}

bool isTightPFJet(unsigned int pfJetIdx){


    float pfjet_nhf_  = cms2.pfjets_neutralHadronE()[pfJetIdx] / cms2.pfjets_p4()[pfJetIdx].energy();
    float pfjet_nef_  = cms2.pfjets_neutralEmE()[pfJetIdx] / cms2.pfjets_p4()[pfJetIdx].energy();

    if (pfjet_nef_ >= 0.90)
        return false;
    if (pfjet_nhf_ >= 0.90)
        return false;

    if (!isLoosePFJet(pfJetIdx)) return false;

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
    if(eleRelIso03(elIdx) >= 0.15) return false; 
    return true;

  } else if((fabs(cms2.els_etaSC().at(elIdx)) > 1.479) && (fabs(cms2.els_etaSC().at(elIdx)) < 2.5)){
    if(fabs(cms2.els_dEtaIn().at(elIdx)) >= 0.01) return false; 
    if(fabs(cms2.els_dPhiIn().at(elIdx)) >= 0.7) return false; 
    if(cms2.els_sigmaIEtaIEta().at(elIdx) >= 0.03) return false; 
    if(fabs(cms2.els_dxyPV().at(elIdx)) >= 0.04) return false; //is this wrt the correct PV?
    if(fabs(cms2.els_dzPV().at(elIdx)) >= 0.2) return false; //is this wrt the correct PV?
    if( eleRelIso03(elIdx) >= 0.15) return false; 
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
    if( eleRelIso03(elIdx) >= 0.15) return false; 
    if( cms2.els_conv_vtx_flag().at(elIdx)) return false;
    if( cms2.els_exp_innerlayers().at(elIdx) > 1) return false;
    return true;

  } else if((fabs(cms2.els_etaSC().at(elIdx)) > 1.479) && (fabs(cms2.els_etaSC().at(elIdx)) < 2.5)){
    if(fabs(cms2.els_dEtaIn().at(elIdx)) >= 0.009) return false; 
    if(fabs(cms2.els_dPhiIn().at(elIdx)) >= 0.10) return false; 
    if(cms2.els_sigmaIEtaIEta().at(elIdx) >= 0.03) return false; 
    if(cms2.els_hOverE().at(elIdx) >= 0.10) return false; 
    if(fabs(cms2.els_dxyPV().at(elIdx)) >= 0.02) return false; //is this wrt the correct PV?
    if(fabs(cms2.els_dzPV().at(elIdx)) >= 0.2) return false; //is this wrt the correct PV?
    if( fabs( (1.0/cms2.els_ecalEnergy().at(elIdx)) - (cms2.els_eOverPIn().at(elIdx)/cms2.els_ecalEnergy().at(elIdx)) ) >= 0.05) return false; // |1/E - 1/p|
    if( eleRelIso03(elIdx) >= 0.15 || (eleRelIso03(elIdx) >= 0.1 && cms2.els_p4().at(elIdx).pt() < 20) ) return false; 
    if( cms2.els_conv_vtx_flag().at(elIdx)) return false;
    if( cms2.els_exp_innerlayers().at(elIdx) > 1) return false;
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
    if( eleRelIso03(elIdx) >= 0.15) return false; 
    if( cms2.els_conv_vtx_flag().at(elIdx)) return false;
    if( cms2.els_exp_innerlayers().at(elIdx) > 1) return false;
    return true;

  } else if((fabs(cms2.els_etaSC().at(elIdx)) > 1.479) && (fabs(cms2.els_etaSC().at(elIdx)) < 2.5)){
    if(fabs(cms2.els_dEtaIn().at(elIdx)) >= 0.007) return false; 
    if(fabs(cms2.els_dPhiIn().at(elIdx)) >= 0.03) return false; 
    if(cms2.els_sigmaIEtaIEta().at(elIdx) >= 0.03) return false; 
    if(cms2.els_hOverE().at(elIdx) >= 0.10) return false; 
    if(fabs(cms2.els_dxyPV().at(elIdx)) >= 0.02) return false; //is this wrt the correct PV?
    if(fabs(cms2.els_dzPV().at(elIdx)) >= 0.1) return false; //is this wrt the correct PV?
    if( fabs( (1.0/cms2.els_ecalEnergy().at(elIdx)) - (cms2.els_eOverPIn().at(elIdx)/cms2.els_ecalEnergy().at(elIdx)) ) >= 0.05) return false; // |1/E - 1/p|
    if( eleRelIso03(elIdx) >= 0.15 || (eleRelIso03(elIdx) >= 0.1 && cms2.els_p4().at(elIdx).pt() < 20) ) return false; 
    if( cms2.els_conv_vtx_flag().at(elIdx)) return false;
    if( cms2.els_exp_innerlayers().at(elIdx) > 1) return false;
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
    if( eleRelIso03(elIdx) >= 0.1) return false; 
    if( cms2.els_conv_vtx_flag().at(elIdx)) return false;
    if( cms2.els_exp_innerlayers().at(elIdx) > 0) return false;
    return true;

  } else if((fabs(cms2.els_etaSC().at(elIdx)) > 1.479) && (fabs(cms2.els_etaSC().at(elIdx)) < 2.5)){
    if(fabs(cms2.els_dEtaIn().at(elIdx)) >= 0.005) return false; 
    if(fabs(cms2.els_dPhiIn().at(elIdx)) >= 0.02) return false; 
    if(cms2.els_sigmaIEtaIEta().at(elIdx) >= 0.03) return false; 
    if(cms2.els_hOverE().at(elIdx) >= 0.10) return false; 
    if(fabs(cms2.els_dxyPV().at(elIdx)) >= 0.02) return false; //is this wrt the correct PV?
    if(fabs(cms2.els_dzPV().at(elIdx)) >= 0.1) return false; //is this wrt the correct PV?
    if( fabs( (1.0/cms2.els_ecalEnergy().at(elIdx)) - (cms2.els_eOverPIn().at(elIdx)/cms2.els_ecalEnergy().at(elIdx)) ) >= 0.05) return false; // |1/E - 1/p|
    if( eleRelIso03(elIdx) >= 0.1 || (eleRelIso03(elIdx) >= 0.07 && cms2.els_p4().at(elIdx).pt() < 20) ) return false; 
    if( cms2.els_conv_vtx_flag().at(elIdx)) return false;
    if( cms2.els_exp_innerlayers().at(elIdx) > 0) return false;
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
  // The following cuts are not in the official definition, but they are used by ETH to suppress b->mu
  if (fabs(cms2.mus_dxyPV().at(muIdx)) > 0.5)                                    return false;
  if (fabs(cms2.mus_dzPV().at(muIdx)) > 1.0)                                     return false;
  return true;

}

bool isTightMuon(unsigned int muIdx){
  if (!isLooseMuon(muIdx))                                                 return false;
  bool isGlobal  = true;
  if (((cms2.mus_type().at(muIdx)) & (1<<1)) == 0) isGlobal  = false;
  if (!isGlobal)                                                           return false;
  if (cms2.mus_gfit_chi2().at(muIdx)/cms2.mus_gfit_ndof().at(muIdx) >= 10) return false; 
  if (cms2.mus_gfit_validSTAHits().at(muIdx) == 0)                         return false; 
  if (cms2.mus_numberOfMatchedStations().at(muIdx) < 2)                    return false;
  if (fabs(cms2.mus_dxyPV().at(muIdx)) > 0.2)                              return false;
  if (fabs(cms2.mus_dzPV().at(muIdx)) > 0.5)                               return false;
  if (cms2.mus_validPixelHits().at(muIdx) == 0)                            return false;
  if (cms2.mus_nlayers().at(muIdx) < 6)                                    return false;
  return true;

}

int tightChargeEle(unsigned int elIdx){
  if (cms2.els_isGsfCtfScPixChargeConsistent().at(elIdx))                         return 2;
  else if (cms2.els_trk_charge().at(elIdx) == cms2.els_sccharge().at(elIdx))      return 1;
  else                                                                            return 0;
}

int tightChargeMuon(unsigned int muIdx){
  if ( cms2.mus_ptErr().at(muIdx) / cms2.mus_p4().at(muIdx).pt() < 0.2 )          return 2;
  else                                                                            return 0;
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
float eleRelIso03(unsigned int elIdx){

  float chiso     = cms2.els_pfChargedHadronIso().at(elIdx);
  float nhiso     = cms2.els_pfNeutralHadronIso().at(elIdx);
  float emiso     = cms2.els_pfPhotonIso().at(elIdx);
  float deltaBeta = cms2.els_pfPUIso().at(elIdx);
  float absiso = chiso + std::max(0.0, nhiso + emiso - 0.5 * deltaBeta);
  return absiso/(cms2.els_p4().at(elIdx).pt());
}
int eleTightID(unsigned int elIdx){
  if (isTightElectron(elIdx))  return 3;
  if (isMediumElectron(elIdx)) return 2;
  if (isLooseElectron(elIdx))  return 1;
  if (isVetoElectron(elIdx))   return 0;
  return -1;
}
int muTightID(unsigned int muIdx){
  if (isTightMuon(muIdx))  return 1;
  if (isLooseMuon(muIdx))   return 0;
  return -1;
}
//-------------------------------------------------------
// get exact trigger name corresponding to given pattern
//-------------------------------------------------------
TString triggerName(TString triggerPattern){

  bool    foundTrigger  = false;
  TString exact_hltname = "";

  for( unsigned int itrig = 0 ; itrig < hlt_trigNames().size() ; ++itrig ){
    if( TString( hlt_trigNames().at(itrig) ).Contains( triggerPattern ) ){
      foundTrigger  = true;
      exact_hltname = hlt_trigNames().at(itrig);
      break;
    }
  }

  if( !foundTrigger) return "TRIGGER_NOT_FOUND";

  return exact_hltname;

}
//---------------------------------------------
// Check if trigger passes
//---------------------------------------------
bool passHLTTriggerPattern(const char* arg){

  TString HLTTriggerPattern(arg);
  TString HLTTrigger = triggerName( HLTTriggerPattern );

  if( HLTTrigger.Contains("TRIGGER_NOT_FOUND")){
    return false;
  }
  return passHLTTrigger( HLTTrigger );
}

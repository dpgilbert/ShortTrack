#include "Math/VectorUtil.h"
#include "IsoTrackVeto.h"
#include "CMS2.h"

float TrackIso(int thisPf){

  double coneR=0.3;
  float absIso = 0.0;
  
  for (int ipf = 0; ipf < (int)cms2.pfcands_p4().size(); ipf++) {

    if( ipf == thisPf ) continue; // skip this PFCandidate
    if(cms2.pfcands_charge().at(ipf) == 0 ) continue; // skip neutrals                                                                                                                          
    double dr=ROOT::Math::VectorUtil::DeltaR( cms2.pfcands_p4().at(ipf) , cms2.pfcands_p4().at(thisPf) );
    if( dr > coneR ) continue; // skip pfcands outside the cone                                     
    if( cms2.pfcands_p4().at(ipf).pt()>=0.0 && fabs(cms2.pfcands_dz().at(ipf)) <= 0.1) absIso += cms2.pfcands_p4().at(ipf).pt();

  }

  return absIso;

}

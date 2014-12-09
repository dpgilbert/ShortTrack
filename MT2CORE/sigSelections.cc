#include "sigSelections.h"

namespace mt2 {

    //_________________________________________
    // passes basic event selection, including cleaning cuts
    bool PassesEventSelection
    (
        const SignalRegionVersion::value_type& version,
        const int nvtx
    )
    {

      if (version == SignalRegionVersion::sel2012) {
	if (nvtx == 0) return false;
	// need to add: "cleaning cuts against instrumental effects" (section 7.3 of 2012 MT2 AN)
      }

      else if (version == SignalRegionVersion::sel2015LowLumi) {
	if (nvtx == 0) return false;
	// need to add: "cleaning cuts against instrumental effects" (section 7.3 of 2012 MT2 AN)
      }

      return true;
    }

    //_________________________________________
    // passes signal region
    bool PassesSignalRegion
    (
        const SignalRegionVersion::value_type& version,
        const float mt2,
        const float met,
        const float ht,
        const int njets,
        const int nbtag,
        const float deltaPhiMin,
        const float diffMetMht,
        const float minMTBMet,
        const int nlep,
        const float j1pt,
        const float j2pt,
        const SignalRegionJets::value_type& sr_jets,
        const SignalRegionHtMet::value_type& sr_htmet
    )
    {
      if (!PassesSignalRegionBase(version, mt2, met, ht, njets, deltaPhiMin, diffMetMht, j1pt, j2pt)) return false;
      if (nlep > 0) return false;
      if ((sr_jets != SignalRegionJets::nocut) && (PassesSignalRegionJets(version, njets, nbtag, minMTBMet, mt2) != sr_jets)) return false;
      if ((sr_htmet != SignalRegionHtMet::nocut) && (PassesSignalRegionHtMet(version, met, ht) != sr_htmet)) return false;
      return true;
    }

    //_________________________________________
    // passes signal region, no lepton veto applied
    bool PassesSignalRegionNoLepVeto
    (
        const SignalRegionVersion::value_type& version,
        const float mt2,
        const float met,
        const float ht,
        const int njets,
        const int nbtag,
        const float deltaPhiMin,
        const float diffMetMht,
        const float minMTBMet,
        const float j1pt,
        const float j2pt,
        const SignalRegionJets::value_type& sr_jets,
        const SignalRegionHtMet::value_type& sr_htmet
    )
    {
      if (!PassesSignalRegionBase(version, mt2, met, ht, njets, deltaPhiMin, diffMetMht, j1pt, j2pt)) return false;
      if ((sr_jets != SignalRegionJets::nocut) && (PassesSignalRegionJets(version, njets, nbtag, minMTBMet, mt2) != sr_jets)) return false;
      if ((sr_htmet != SignalRegionHtMet::nocut) && (PassesSignalRegionHtMet(version, met, ht) != sr_htmet)) return false;
      return true;
    }

    //_________________________________________
    // passes signal region: baseline cuts
    bool PassesSignalRegionBase
    (
        const SignalRegionVersion::value_type& version,
        const float mt2,
        const float met,
        const float ht,
        const int njets,
        const float deltaPhiMin,
        const float diffMetMht,
        const float minMTBMet,
        const float j1pt,
        const float j2pt
    )
    {

      if (version == SignalRegionVersion::sel2012) {
	bool ht450 = ht > 450. && met > 200.;
	bool ht750 = ht > 750. && met > 30.;     
	bool dijet100 = njets>=2 && j1pt > 100. && j2pt > 100.;
	bool cleaning = deltaPhiMin > 0.3 && diffMetMht < 70.; 
	bool baseline = (cleaning && dijet100 && (ht450 || ht750));
	return baseline;
      }
      else if (version == SignalRegionVersion::sel2015LowLumi) {
	bool mt2_200 = mt2 > 200.;
	bool ht450 = ht > 450. && met > 200.;
	bool ht575 = ht > 575. && met > 30.;     
	bool dijet100 = njets>=2 && j1pt > 100. && j2pt > 100.;
	bool cleaning = deltaPhiMin > 0.3 && diffMetMht/met < 0.5; 
	bool baseline = (cleaning && dijet100 && (ht450 || ht575) && mt2_200);
	return baseline;
      }

      // shouldn't get here
      return false;
    }

    //_________________________________________
    // passes signal region: jet counting cuts
    SignalRegionJets::value_type PassesSignalRegionJets
    (
        const SignalRegionVersion::value_type& version,
        const int njets,
        const int nbtag,
        const float minMTBMet,
        const float mt2
    )
    {

      if (version == SignalRegionVersion::sel2012) {
	if (njets==2 && nbtag==0) return SignalRegionJets::sr1; //2j, 0b
	else if (njets==2 && (nbtag==1 || nbtag==2)) return SignalRegionJets::sr2;//2j, 1-2 b
	else if (njets>2 && njets<6 && nbtag==0 ) return SignalRegionJets::sr3;//3-5j, 0b
	else if (njets>2 && njets<6 && nbtag==1 ) return SignalRegionJets::sr4;//3-5j, 1b
	else if (njets>2 && njets<6 && nbtag==2 ) return SignalRegionJets::sr5;//3-5j, 2b
	else if (njets>=6 && nbtag==0 ) return SignalRegionJets::sr6;//6j, 0b
	else if (njets>=6 && nbtag==1 ) return SignalRegionJets::sr7;//6j, 1b
	else if (njets>=6 && nbtag==2 ) return SignalRegionJets::sr8;//6j, 2b
	else if (njets>=3 && nbtag>=3 ) return SignalRegionJets::sr9;//3j, 3b                    

      }

      else if (version == SignalRegionVersion::sel2015LowLumi) {
	if ((njets==2 || njets==3) && nbtag==0) return SignalRegionJets::sr1; //2-3j, 0b
	else if ((njets==2 || njets==3) && nbtag==1) return SignalRegionJets::sr2;//2-3j, 1b
	else if ((njets==2 || njets==3) && nbtag==2) { //2-3j, 2b
	  if (minMTBMet < 200. && mt2 < 400.) return SignalRegionJets::sr3;
	  else return SignalRegionJets::sr4;
	}
	else if (njets>=4 && nbtag==0 ) return SignalRegionJets::sr5;//4+j, 0b
	else if (njets>=4 && nbtag==1 ) return SignalRegionJets::sr6;//4+j, 1b
	else if (njets>=4 && nbtag==2 ) {//4+j, 2b
	  if (minMTBMet < 200. && mt2 < 400.) return SignalRegionJets::sr7;
	  else return SignalRegionJets::sr8;
	}
	else if (njets>=2 && nbtag>=3 ) {//2+j, 3+b
	  if (minMTBMet < 200. && mt2 < 400.) return SignalRegionJets::sr9;
	  else return SignalRegionJets::sr10;
	}
      }

      // doesn't pass any of the regions above.. shouldn't happen
      return SignalRegionJets::nocut;

    }

    //_________________________________________
    // passes signal region: HT and MET cuts
    SignalRegionHtMet::value_type PassesSignalRegionHtMet
    (
        const SignalRegionVersion::value_type& version,
        const float met,
        const float ht
    )
    {
      if (version == SignalRegionVersion::sel2012) {
	if (met < 30. || ht < 450.) return SignalRegionHtMet::nocut;
	if (ht > 1200.) return SignalRegionHtMet::h_ht;
	else if (ht > 750.) return SignalRegionHtMet::m_ht;
	else if (ht > 450. && met > 200.) return SignalRegionHtMet::l_ht;
      }
      else if (version == SignalRegionVersion::sel2015LowLumi) {
	if (met < 30. || ht < 450.) return SignalRegionHtMet::nocut;
	if (ht > 1000.) return SignalRegionHtMet::h_ht;
	else if (ht > 575.) return SignalRegionHtMet::m_ht;
	else if (ht > 450. && met > 200.) return SignalRegionHtMet::l_ht;
      }

      // doesn't pass any of the regions above.. shouldn't happen
      return SignalRegionHtMet::nocut;
    }



} // namespace mt2

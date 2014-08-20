#include "sigSelections.h"

namespace mt2 {

    //_________________________________________
    // passes basic event selection, including cleaning cuts
    bool PassesEventSelection
    (
        const int nvtx
    )
    {
      if (nvtx == 0) return false;
      // need to add: "cleaning cuts against instrumental effects" (section 7.3 of 2012 MT2 AN)
      return true;
    }

    //_________________________________________
    // passes signal region
    bool PassesSignalRegion
    (
        const float met,
        const float ht,
        const int njets,
        const int nbtag,
        const float deltaPhiMin,
        const float diffMetMht,
        const int nlep,
        const float j1pt,
        const float j2pt,
        const SignalRegionJets::value_type& sr_jets,
        const SignalRegionHtMet::value_type& sr_htmet
    )
    {
      if (!PassesSignalRegionBase(met, ht, njets, deltaPhiMin, diffMetMht, nlep, j1pt, j2pt)) return false;
      if ((sr_jets != SignalRegionJets::nocut) && (PassesSignalRegionJets(njets, nbtag) != sr_jets)) return false;
      if ((sr_htmet != SignalRegionHtMet::nocut) && (PassesSignalRegionHtMet(met, ht) != sr_htmet)) return false;
      return true;
    }

    //_________________________________________
    // passes signal region: baseline cuts
    bool PassesSignalRegionBase
    (
        const float met,
        const float ht,
        const int njets,
        const float deltaPhiMin,
        const float diffMetMht,
        const int nlep,
        const float j1pt,
        const float j2pt
    )
    {

      bool ht450 = ht > 450;
      bool met30 = met > 30;     
      bool dijet100 = njets>=2 && j1pt > 100. && j2pt > 100.;
      bool cleaning = deltaPhiMin > 0.3 && diffMetMht < 70.; 
      bool baseline = (cleaning && dijet100 && nlep==0 && ht450 && met30);

      return baseline;
    }

    //_________________________________________
    // passes signal region: jet counting cuts
    SignalRegionJets::value_type PassesSignalRegionJets
    (
        const int njets,
        const int nbtag
    )
    {
      if (njets==2 && nbtag==0) return SignalRegionJets::sr1; //2j, 0b
      else if (njets==2 && (nbtag==1 || nbtag==2)) return SignalRegionJets::sr2;//2j, 1-2 b
      else if (njets>2 && njets<6 && nbtag==0 ) return SignalRegionJets::sr3;//3-5j, 0b
      else if (njets>2 && njets<6 && nbtag==1 ) return SignalRegionJets::sr4;//3-5j, 1b
      else if (njets>2 && njets<6 && nbtag==2 ) return SignalRegionJets::sr5;//3-5j, 2b
      else if (njets>=6 && nbtag==0 ) return SignalRegionJets::sr6;//6j, 0b
      else if (njets>=6 && nbtag==1 ) return SignalRegionJets::sr7;//6j, 1b
      else if (njets>=6 && nbtag==2 ) return SignalRegionJets::sr8;//6j, 2b
      else if (njets>=3 && nbtag>=3 ) return SignalRegionJets::sr9;//3j, 3b                    

      return SignalRegionJets::nocut;
    }

    //_________________________________________
    // passes signal region: HT and MET cuts
    SignalRegionHtMet::value_type PassesSignalRegionHtMet
    (
        const float met,
        const float ht
    )
    {
      if (met < 30. || ht < 450.) return SignalRegionHtMet::nocut;
      if (ht > 1200.) return SignalRegionHtMet::h_ht;
      else if (ht > 750.) return SignalRegionHtMet::m_ht;
      else if (ht > 450. && met > 200.) return SignalRegionHtMet::l_ht;

      return SignalRegionHtMet::nocut;
    }



} // namespace mt2

#include "SignalRegion.h"

namespace mt2 {

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
      switch(sr_htmet)
	{
	case SignalRegionHtMet::nocut:	 
	  break;
	case SignalRegionHtMet::h_ht:
	  if (met  < 30.0   ) return false;
	  if (ht   < 1200.0 ) return false;
	  break;
	case SignalRegionHtMet::m_ht:
	  if (met  < 30.0   ) return false;
	  if (ht   > 1200.0 ) return false;
	  if (ht   >  750.0 ) return false;
	  break;
	case SignalRegionHtMet::l_ht:
	  if (met  < 200.0  ) return false;
	  if (ht   <  450.0 ) return false;
	  if (ht   >  750.0 ) return false;
	  break;
	default:
	  return false;
	}
     
      bool dijet100 = njets>=2 && j1pt > 100. && j2pt > 100.;
      bool cleaning = deltaPhiMin > 0.3 && diffMetMht < 70.; 
      // need to add: "cleaning cuts against instrumental effects" (section 7.3)
      bool baseline = (cleaning && dijet100 && nlep==0);

      switch (sr_jets)
	{
	case SignalRegionJets::nocut : return true;
	case SignalRegionJets::sr1 : return (baseline && njets==2 && nbtag==0  ); //2j, 0b
	case SignalRegionJets::sr2 : return (baseline && njets==2 && (nbtag==1 || nbtag==2));//2j, 1-2 b
	case SignalRegionJets::sr3 : return (baseline && njets>2 && njets<6 && nbtag==0 );//3-5j, 0b
	case SignalRegionJets::sr4 : return (baseline && njets>2 && njets<6 && nbtag==1 );//3-5j, 1b
	case SignalRegionJets::sr5 : return (baseline && njets>2 && njets<6 && nbtag==2 );//3-5j, 2b
	case SignalRegionJets::sr6 : return (baseline && njets>=6 && nbtag==0 );//6j, 0b
	case SignalRegionJets::sr7 : return (baseline && njets>=6 && nbtag==1 );//6j, 1b
	case SignalRegionJets::sr8 : return (baseline && njets>=6 && nbtag==2 );//6j, 2b
	case SignalRegionJets::sr9 : return (baseline && njets>=3 && nbtag>=3 );//3j, 3b                    
	case SignalRegionJets::static_size: {throw std::domain_error("ERROR: mt2::PassesSignalRegion(): argument out of bounds!");}
	};
      return false;
    }


} // namespace mt2

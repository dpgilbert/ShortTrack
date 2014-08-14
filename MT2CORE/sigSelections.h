#ifndef SIGNALREGIONS_H
#define SIGNALREGIONS_H

#include <string>
#include "TTree.h"

namespace mt2
{
    struct SignalRegionJets
    {
        enum value_type
        {
            // inclusive
	    nocut= 0,   //inclusive
	    sr1  = 1,   //2j, 0b    
	    sr2  = 2,   //2j, 1-2 b 
	    sr3  = 3,   //3-5j, 0b  
	    sr4  = 4,   //3-5j, 1b  
	    sr5  = 5,   //3-5j, 2b  
	    sr6  = 6,   //6j, 0b    
	    sr7  = 7,   //6j, 1b    
	    sr8  = 8,   //6j, 2b    
	    sr9  = 9,   //3j, 3b    
            // keep track of the size
            static_size
        };
    };

    struct SignalRegionHtMet
    {
        enum value_type
        {
            nocut = 0, 
            h_ht = 20, // ht > 1200
            m_ht = 40, // 750 < ht < 1200
            l_ht = 60, // 450 < ht < 750  && met > 200
            static_size
        };
    };

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
        const SignalRegionJets::value_type& signal_region = SignalRegionJets::nocut,
        const SignalRegionHtMet::value_type& signal_region_type = SignalRegionHtMet::nocut
    );

} // namespace ss

#endif // SIGNALREGIONS_H

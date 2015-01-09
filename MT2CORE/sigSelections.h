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
            // meanings of these will change depending on the selection version
	    nocut= 0,   //inclusive
	    sr1  = 1,   
	    sr2  = 2,   
	    sr3  = 3,   
	    sr4  = 4,   
	    sr5  = 5,   
	    sr6  = 6,   
	    sr7  = 7,   
	    sr8  = 8,   
	    sr9  = 9,   
	    sr10 = 10,   
	    crw  = 11,   
	    crtt = 12,   
            // keep track of the size
            static_size
        };
    };

    struct SignalRegionHtMet
    {
        enum value_type
        {
            // meanings of these will change depending on the selection version
            nocut = 0, 
            l_ht = 20, 
            m_ht = 40, 
            h_ht = 60, 
            static_size
        };
    };

    struct SignalRegionVersion
    {
        enum value_type
        {
            sel2012 = 0, 
            sel2015LowLumi = 1, 
            static_size
        };
    };

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
        const SignalRegionJets::value_type& signal_region = SignalRegionJets::nocut,
        const SignalRegionHtMet::value_type& signal_region_type = SignalRegionHtMet::nocut
    );

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
        const SignalRegionJets::value_type& signal_region = SignalRegionJets::nocut,
        const SignalRegionHtMet::value_type& signal_region_type = SignalRegionHtMet::nocut
    );

    // event level and cleaning cuts
    bool PassesEventSelection
    (
        const SignalRegionVersion::value_type& version,
        const int nvtx
    );

    bool PassesSignalRegionBase
    (
        const SignalRegionVersion::value_type& version,
        const float mt2,
        const float met,
        const float ht,
        const int njets,
        const float deltaPhiMin,
        const float diffMetMht,
        const float j1pt,
        const float j2pt
    );

    SignalRegionJets::value_type PassesSignalRegionJets
    (
        const SignalRegionVersion::value_type& version,
        const int njets,
        const int nbtag,
        const float minMTBMet = 0.,
        const float mt2 = 0.
    );

    SignalRegionHtMet::value_type PassesSignalRegionHtMet
    (
        const SignalRegionVersion::value_type& version,
        const float met,
        const float ht
    );



} // namespace mt2

#endif // SIGNALREGIONS_H

#ifndef SIGNALREGIONS_H
#define SIGNALREGIONS_H

#include <string>
#include "TTree.h"
#include "SR.h"

namespace mt2
{

std::vector<SR> getSignalRegions2012();
std::vector<SR> getSignalRegions2015LowLumi();
std::vector<SR> getSignalRegions2015ExtendedNJets();
std::vector<SR> getSignalRegions2015ExtendedNJets_UltraHighHT();
std::vector<SR> getSignalRegions2015SevenJets_UltraHighHT();
std::vector<SR> getSignalRegionsZurich();
std::vector<SR> getSignalRegionsZurich_jetpt75();

} // namespace mt2

#endif // SIGNALREGIONS_H

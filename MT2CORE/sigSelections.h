#ifndef SIGNALREGIONS_H
#define SIGNALREGIONS_H

#include <string>
#include "TTree.h"
#include "SR.h"

namespace mt2
{

std::vector<SR> getSignalRegions2012();
std::vector<SR> getSignalRegions2015LowLumi();

} // namespace mt2

#endif // SIGNALREGIONS_H

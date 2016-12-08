#ifndef SIGNALREGIONSRS_H
#define SIGNALREGIONSRS_H

#include <string>
#include "TTree.h"
#include "SR_RebalSmear.h"

namespace mt2
{

std::vector<SRRS> getSignalRegionsMonojet_RS();
std::vector<SRRS> getSignalRegionsICHEP_RS();

} // namespace mt2

#endif // SIGNALREGIONSRS_H

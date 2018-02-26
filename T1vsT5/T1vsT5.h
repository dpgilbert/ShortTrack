#ifndef T1VST5_H
#define T1VST5_H

#include <iostream>
#include <vector>
#include <map>
#include <cmath>

#include "TH1.h"
#include "TH2.h"

#include "../CORE/Tools/utils.h"
#include "../CORE/Tools/goodrun.h"
#include "../CORE/Tools/dorky/dorky.h"
#include "../CORE/Tools/badEventFilter.h"

#include "../MT2CORE/mt2tree.h"
#include "../MT2CORE/sigSelections.h"
#include "../MT2CORE/SR.h"

#include "Math/LorentzVector.h"

typedef ROOT::Math::LorentzVector<ROOT::Math::PxPyPzE4D<float> > LorentzVector;

class T1vsT5 {
 public:
  T1vsT5();
  ~T1vsT5();

  void loop (const TChain* chain, const char* sample, const char* outdir, const int maxEvt);
};

#endif

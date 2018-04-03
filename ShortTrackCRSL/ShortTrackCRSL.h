#ifndef SHORTTRACKCRSL_H
#define SHORTTRACKCRSL_H

// C++
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include <stdlib.h>
#include <vector>

// ROOT
#include "TMath.h"
#include "TLorentzVector.h"
#include "TH1.h"
#include "TH2.h"
#include "TChain.h"

#include "../CORE/Tools/utils.h"

#include "../MT2CORE/mt2tree.h"
#include "../MT2CORE/sttree.h"

class ShortTrackCRSL {
 public:
  ShortTrackCRSL();
  ~ShortTrackCRSL();
  int loop(char* indir, char* sample, int selection);
};

#endif

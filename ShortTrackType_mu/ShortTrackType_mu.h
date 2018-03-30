#ifndef SHORTTRACKTYPEMU_H
#define SHORTTRACKTYPEMU_H

// C++
#include <iostream>
#include <cmath>
#include <sstream>

// ROOT
#include "TMath.h"
#include "TLorentzVector.h"
#include "TH1.h"
#include "TH2.h"
#include "TChain.h"

#include "../MT2CORE/mt2tree.h"
#include "../MT2CORE/sttree.h"

class ShortTrackType_mu {
 public:
  ShortTrackType_mu();
  ~ShortTrackType_mu();
  int loop(char* indir, char* sample, char* unskimmed_dir, int selection, int region);
};

#endif

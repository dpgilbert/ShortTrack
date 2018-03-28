#ifndef SHORTTRACKTYPE_H
#define SHORTTRACKTYPE_H

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

class ShortTrackType {
 public:
  ShortTrackType();
  ~ShortTrackType();
  int loop(char* indir, char* sample, char* outdir, char* unskimmed_dir, int selection);
};

#endif

#ifndef SHORTTRACKTYPE_H
#define SHORTTRACKTYPE_H

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

class ShortTrackType {
 public:
  ShortTrackType();
  ~ShortTrackType();
  std::string StringSelection(int selection, int region, bool isWjetsIncl);
  int loop(char* indir, char* sample, char* unskimmed_dir, int selection, int region);
};

#endif

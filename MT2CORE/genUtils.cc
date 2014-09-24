#include "genUtils.h"
#include "CMS2.h"

using namespace tas;

//________________________________________________________________
int getSourceId(int genpIdx){

  // look for earliest mother that is: top, W, Z, photon, or H
  //   terminate search if top or H is found
  //   continue if W, Z, or photon is found, to see if there's a top or H
  //   can also add SUSY particles to logic, not yet done

  int currentIdx = genpIdx;
  int motherIdx = cms2.genps_idx_simplemother().at(currentIdx);
  int motherId = abs(cms2.genps_id_simplemother().at(currentIdx));
  int sourceId = -1;
  if (useSourceId(motherId)) sourceId = motherId;

  while ( (currentIdx >= 0) && (motherIdx >= 0) && !stopSourceId(sourceId) && !stopMotherId(motherId) ) {
    currentIdx = motherIdx;
    motherIdx = cms2.genps_idx_simplemother().at(currentIdx);
    motherId = abs(cms2.genps_id_simplemother().at(currentIdx));
    if (useSourceId(motherId)) sourceId = motherId;
  }

  return sourceId;

}

//________________________________________________________________
// list of sourceIds to terminate search on
//   can add susy particles here
bool stopSourceId(int sourceId){
  int id = abs(sourceId);
  if (id == 6 || id == 25) return true;
  return false;
}

//________________________________________________________________
// list of sourceIds to allow
//   top, photon, Z, W, H
//   can add susy particles here
bool useSourceId(int motherId){
  int id = abs(motherId);
  if (id == 6 || (id >= 22 && id <= 25)) return true;
  return false;
}

//________________________________________________________________
// list of mother PdgIds to terminate search on
//   quarks, gluons, hadrons
bool stopMotherId(int motherId){
  int id = abs(motherId);
  if (id <= 5 || id == 21 || (id > 100 && id < 1000000)) return true;
  return false;
}

#include <string>
#include "TChain.h"
#include "TString.h"

void doAll(std::string input_dir = "/nfs-6/userdata/mt2/V00-00-03", std::string sample = "ttall_msdecays", std::string output_dir = ".") {

  //  gSystem->Load("libMiniFWLite.so");
  gSystem->Load("libMiniBabyIsoTrackCORE.so");
  gSystem->Load("libMiniBabyIsoTrack.so");

  TChain *ch = new TChain("mt2"); 
 
  ch->Add(Form("%s/%s*.root",input_dir.c_str(),sample.c_str()));

  MiniBabyIsoTrackLooper *looper = new MiniBabyIsoTrackLooper();
  looper->loop(ch, output_dir + "/" + sample + "_mini.root"); 
}

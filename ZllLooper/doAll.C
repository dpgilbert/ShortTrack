#include <string>
#include "TChain.h"
#include "TString.h"

void doAll(std::string input_dir = "/nfs-6/userdata/mt2/V00-00-05", std::string sample = "dyjetsll_ht", std::string output_dir = ".") {

  //  gSystem->Load("libMiniFWLite.so");
  gSystem->Load("libZllLooperCORE.so");
  gSystem->Load("libZllLooper.so");

  TChain *ch = new TChain("mt2"); 
 
  ch->Add(Form("%s/%s*.root",input_dir.c_str(),sample.c_str()));

  ZllLooper *looper = new ZllLooper();
  looper->loop(ch, output_dir + "/" + sample + ".root"); 
}

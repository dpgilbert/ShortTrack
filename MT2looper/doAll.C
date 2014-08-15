#include <string>
#include "TChain.h"
#include "TString.h"

void doAll(std::string input_dir = "/nfs-6/userdata/mt2/V00-00-02", std::string sample = "ttall_msdecays", std::string output_dir = ".") {

  //  gSystem->Load("libMiniFWLite.so");
  gSystem->Load("libMT2LooperCORE.so");
  gSystem->Load("libMT2Looper.so");

  TChain *ch = new TChain("treeProducerSusyFullHad"); 
 
  ch->Add(Form("%s/%s*.root",input_dir.c_str(),sample.c_str()));

  MT2Looper *looper = new MT2Looper();
  looper->loop(ch, output_dir + "/" + sample + ".root"); 
}

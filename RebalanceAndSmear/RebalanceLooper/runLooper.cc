#include <string>
#include "TChain.h"
#include "TString.h"

#include "RebalanceLooper.h"

int main(int argc, char **argv) {

  if (argc < 4) {
   std::cout << "USAGE: runLooper <input_dir> <sample> <output_dir>" << std::endl;
   return 1;
  }

  std::string input_dir(argv[1]); 
  std::string sample(argv[2]); 
  std::string output_dir(argv[3]);

  TChain *ch = new TChain("mt2"); 
 
  TString infile = Form("%s/%s*.root",input_dir.c_str(),sample.c_str());
  ch->Add(infile);
  // ch->Add("input_mt2_baby.root");
  if (ch->GetEntries() == 0) {
    std::cout << "ERROR: no entries in chain. filename was: " << infile << std::endl;
    return 2;
  }

  RebalanceLooper *looper = new RebalanceLooper();
  looper->loop(ch, output_dir + "/" + sample + ".root"); 
  // looper->loop(ch, "output.root");
  return 0;
}

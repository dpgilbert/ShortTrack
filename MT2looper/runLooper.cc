#include <string>
#include "TChain.h"
#include "TString.h"

#include "MT2Looper.h"

int main(int argc, char **argv) {

  if (argc < 4) {
    std::cout << "USAGE: runLooper <input_dir> <sample> <output_dir>" << std::endl;
    return 1;
  }

  std::string input_dir(argv[1]); 
  std::string sample(argv[2]); 
  std::string output_dir(argv[3]);

  TChain *ch = new TChain("mt2"); 
 
  ch->Add(Form("%s/%s*.root",input_dir.c_str(),sample.c_str()));

  MT2Looper *looper = new MT2Looper();
  looper->loop(ch, output_dir + "/" + sample + ".root"); 
}

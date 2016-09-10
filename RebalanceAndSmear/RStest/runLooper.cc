#include <string>
#include "TChain.h"
#include "TString.h"

#include "RStest_mt2tree.h"

int main(int argc, char **argv) {

  if (argc < 4) {
   std::cout << "USAGE: runLooper <template file> <input_dir> <rebal_file> <sample> <output_dir>" << std::endl;
   return 1;
  }

  std::string template_file(argv[1]); 
  std::string input_dir(argv[2]); 
  std::string rebal_file(argv[3]);
  std::string sample(argv[4]); 
  std::string output_dir(argv[5]);

  TChain *ch = new TChain("mt2"); 
 
  TString infile = Form("%s/%s*.root",input_dir.c_str(),sample.c_str());
  ch->Add(infile);
  // ch->Add("input_mt2_baby.root");
  if (ch->GetEntries() == 0) {
    std::cout << "ERROR: no entries in chain. filename was: " << infile << std::endl;
    return 2;
  }

  TFile rebalFile(rebal_file.c_str());
  TTree *tree = (TTree*)rebalFile.Get("rebalance");
  JRTrebalTree *rebalTree = new JRTrebalTree();
  rebalTree->Init(tree);

  JRTreader *reader = new JRTreader();
  reader->UseRawHistograms(false);
  reader->Init((char*)template_file.c_str());

  RStest *looper = new RStest();
  looper->loop(ch, *rebalTree, *reader, output_dir + "/" + sample + ".root"); 
  // looper->loop(ch, "output.root");
  return 0;
}

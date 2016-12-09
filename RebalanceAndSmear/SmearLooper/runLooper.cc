#include <string>
#include "TChain.h"
#include "TString.h"

#include "SmearLooper.h"

int main(int argc, char **argv) {

  //standard
  if (argc < 4) {
    std::cout << "USAGE: runLooper <input_dir> <sample> <output_dir> <options>" << std::endl;
    return 1;
  }

  std::string input_dir(argv[1]);
  std::string sample(argv[2]);
  std::string output_dir(argv[3]);

  int bflag = 0;
  int cflag = 0;
  int mflag = 0;
  int rflag = 0;
  int tflag = 0;
  int wflag = 0;
  int max_events = -1;
  float core_scale = 1.;
  float tail_scale = 1.;
  float mean_shift = 0.;
  
  int c;
  while ((c = getopt(argc, argv, "bc:m:n:rt:w")) != -1) {
    switch (c) {
    case 'b':
      bflag = 1;
      break;
    case 'c':
      cflag = 1;
      core_scale = atof(optarg);
      break;
    case 'm':
      mflag = 1;
      mean_shift = atof(optarg);
      break;
    case 'n':
      max_events = atoi(optarg);
      break;
    case 'r':
      rflag = 1;
      break;
    case 't':
      tflag = 1;
      tail_scale = atof(optarg);
      break;
    case 'w':
      wflag = 1;            
    case '?':
      if (isprint(optopt))
        fprintf(stderr, "Unknown option `-%c'.\n", optopt);
      else
        fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
      return 1;
    default:
      abort ();
    }
  }

  printf ("running on %d events with bflag = %d, rflag = %d, wflag = %d, and core_scale = %1.2f, tail_scale = %1.2f, mean_shift = %1.2f\n",
          max_events, bflag, rflag, wflag, core_scale, tail_scale, mean_shift);
  
    
  TChain *ch = new TChain("mt2");
  
  TString infile = Form("%s/%s*.root",input_dir.c_str(),sample.c_str());
  // TString infile = Form("%s/%s.root",input_dir.c_str(),sample.c_str());
  ch->Add(infile);
  if (ch->GetEntries() == 0) {
    std::cout << "ERROR: no entries in chain. filename was: " << infile << std::endl;
    return 2;
  }
  
  SmearLooper *looper = new SmearLooper();
  if (bflag) looper->MakeSmearBaby();
  if (cflag) looper->SetCoreScale(core_scale);
  if (mflag) looper->SetMeanShift(mean_shift);
  if (rflag) looper->DoRebalanceAndSmear();
  if (tflag) looper->SetTailScale(tail_scale);
  if (wflag) looper->ApplyWeights();
  looper->loop(ch, output_dir + "/" + sample + ".root", max_events);
  return 0;

  // //RS batch submit
  // TChain *ch = new TChain("mt2"); 
 
  // ch->Add("input_mt2_baby.root");
  // if (ch->GetEntries() == 0) {
  //   return 2;
  // }

  // SmearLooper *looper = new SmearLooper();
  // looper->loop(ch, "output.root");
  // return 0;

}

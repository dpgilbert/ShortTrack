#include <string>
#include "TChain.h"
#include "TString.h"
#include <vector>
#include <sstream>
#include "SmearLooper.h"


std::vector<std::string> split(const std::string &s, char delim)
{
  std::stringstream ss(s);
  std::string item;
  std::vector<std::string> tokens;
  while (std::getline(ss, item, delim)) {
    tokens.push_back(item);
  }
  return tokens;
}

int main(int argc, char **argv) {

  //standard
  if (argc < 4) {
    std::cout << "USAGE: runLooper <input_dir> <sample> <output_dir> <options>" << std::endl;
    return 1;
  }
  
  std::string input_dir(argv[1]);
  std::string sample(argv[2]);
  std::string output_dir(argv[3]);

  std::vector<std::string> samples = split(sample, ',');
  
  int bflag = 0;
  int cflag = 0;
  int hflag = 0;
  int jflag = 1;
  int lflag = 0;
  int mflag = 0;
  int rflag = 0;
  int tflag = 0;
  int wflag = 0;
  int max_events = -1;
  float core_scale = 1.;
  float tail_scale = 1.;
  float mean_shift = 0.;
  int cut_level = 1;

  int c;
  while ((c = getopt(argc, argv, "bc:hjl:m:n:rt:w")) != -1) {
    switch (c) {
    case 'b':
      bflag = 1;
      break;
    case 'c':
      cflag = 1;
      core_scale = atof(optarg);
      break;
    case 'h':
      hflag = 1;
      break;
    case 'j':
      jflag = 0;
      break;
    case 'm':
      mflag = 1;
      mean_shift = atof(optarg);
      break;
    case 'l':
      lflag = 1;
      cut_level = atoi(optarg);
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

  printf ("running on %d events with bflag = %d, rflag = %d, wflag = %d, hflag = %d, jflag = %d and core_scale = %1.2f, tail_scale = %1.2f, mean_shift = %1.2f\n",
          max_events, bflag, rflag, wflag, hflag, jflag, core_scale, tail_scale, mean_shift);
  
    
  TChain *ch = new TChain("mt2");
  
  // TString infile = Form("%s/%s*.root",input_dir.c_str(),sample.c_str());
  for (unsigned int idx = 0; idx < samples.size(); idx++) {
    TString infile = Form("%s/%s.root",input_dir.c_str(),samples.at(idx).c_str());
    ch->Add(infile);
    if (ch->GetEntries() == 0) {
      std::cout << "ERROR: no entries in chain. filename was: " << infile << std::endl;
      return 2;
    }
  }
  
  SmearLooper *looper = new SmearLooper();
  if (bflag) looper->MakeSmearBaby();
  if (cflag) looper->SetCoreScale(core_scale);
  if (hflag) looper->UseRawHists();
  if (!jflag) looper->UseBjetResponse(jflag);
  if (mflag) looper->SetMeanShift(mean_shift);
  if (rflag) looper->DoRebalanceAndSmear();
  if (tflag) looper->SetTailScale(tail_scale);
  if (wflag) looper->ApplyWeights();
  if (lflag) looper->SetCutLevel(cut_level);
  
  looper->loop(ch, output_dir + "/" + samples.at(0) + ".root", max_events);
  return 0;
}

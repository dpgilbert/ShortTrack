#include "TH1.h"
#include "TFile.h"
#include "TList.h"
#include "TString.h"
#include "TCollection.h"
#include "TKey.h"

#include <iostream>

void rescaleBoundaryHists(std::string infile, int numSamples){

  TFile* f = new TFile(infile.c_str(), "UPDATE");
  TDirectory* dir = 0;

  TIter dir_it(f->GetListOfKeys());
  TKey* dir_k;
  while ((dir_k = (TKey *)dir_it())) {
    if (TString(dir_k->GetClassName()) != "TDirectoryFile") continue;
    std::string dir_name = std::string(dir_k->GetTitle());
    if(dir_name == "") continue;
    dir = (TDirectory*)dir_k->ReadObj();
    if(dir == 0) continue;
    TIter hist_it(dir->GetListOfKeys(), kIterBackward);
    TKey* hist_k;
    while ((hist_k = (TKey *)hist_it())) {
      std::string hist_name = (hist_k->GetTitle());
      if (hist_name.find("_HI") != std::string::npos || hist_name.find("_LOW") != std::string::npos || hist_name.find("h_n_mt2bins") != std::string::npos) {
        TH1* h = (TH1*)hist_k->ReadObj();
        h->Scale(1.0/numSamples);
        dir->cd();
        h->Write("",TObject::kOverwrite);
      }
    }
  }

  delete dir;

  gDirectory->GetList()->Delete();
  
  f->Write("",TObject::kOverwrite);
  f->Close();
  delete f;

}

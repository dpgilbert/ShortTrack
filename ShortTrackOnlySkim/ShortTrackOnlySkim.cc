// A looper to skim MCs for low Dphi events

#include <iostream>
#include <string>

#include "../CORE/Tools/utils.h"

#include "TFile.h"
#include "TTree.h"

using namespace std;

int main (int argc, char ** argv)
{
  if (argc < 4)
    {
      cout << "Usage: ./ShortTrackOnlySkim.exe <input_directory> <sample> <output_directory>" << endl;
      return 0;
    }

  TFile f( Form("%s/%s.root", argv[1], argv[2]) );
  TTree *tree_mt2 = (TTree*)f.Get("mt2st");
  TTree *tree_st = (TTree*)f.Get("st");
  tree_mt2->AddFriend(tree_st);
  
  // run on unskimmed babies to apply lower mt2 cut and no MET cut

  const char* clean = "met_miniaodPt / met_caloPt < 5.0";
  const char* adhoc = "nJet200MuFrac50DphiMet == 0";  
  const char* nstCut = "nshorttracks > 0";
  
  const char * selection_string = Form("%s && %s && %s", clean, adhoc, nstCut);
  
  TString output_name= Form("%s/%s_incl_skim.root", argv[3], argv[2]);
  TFile * outfile = TFile::Open( output_name , "RECREATE" );

  TTree * skimmed = tree_mt2->CopyTree ( selection_string );

  outfile->cd();
  if (skimmed->GetEntries() > 0) {
    cout << skimmed->GetEntries() << " events found" << endl;
    skimmed->Write();
  } else cout << "No events with short tracks" << endl;
  outfile->Close();

}

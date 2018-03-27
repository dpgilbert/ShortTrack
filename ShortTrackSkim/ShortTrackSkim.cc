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
      cout << "Usage: ./ShortTrackSkim.exe <input_directory> <sample> <output_directory>" << endl;
      return 0;
    }

  TFile f( Form("%s/%s.root", argv[1], argv[2]) );
  TTree *tree_mt2 = (TTree*)f.Get("mt2st");
  TTree *tree_st = (TTree*)f.Get("st");
  tree_mt2->AddFriend(tree_st);
  
  // run on unskimmed babies to apply lower mt2 cut and no MET cut

  const char* htmetCut = "(met_pt > 250 || (ht > 1000 && met_pt > 30))";
  const char* mt2Cut = "mt2 > 200";
  const char* njCut = "nJet30 > 1";
  const char* diffMetMhtCut = "diffMetMht / met_pt < 0.5";
  const char* clean = "met_miniaodPt / met_caloPt < 5.0";
  const char* adhoc = "nJet200MuFrac50DphiMet == 0";  
  const char* nstCut = "nshorttracks > 0";
  const char* jptCut = "jet1_pt > 30 && jet2_pt > 30";
  
  const char * selection_string = Form("%s && %s && %s && %s && %s && %s && %s && %s", htmetCut, mt2Cut, njCut, diffMetMhtCut, clean, adhoc, nstCut, jptCut);
  
  TString output_name_sr = Form("%s/%s_sr_skim.root", argv[3], argv[2]);
  TFile * outfile_sr = TFile::Open( output_name_sr , "RECREATE" );
  TString output_name_crsl = Form("%s/%s_crsl_skim.root", argv[3], argv[2]);
  TFile * outfile_crsl = TFile::Open( output_name_crsl , "RECREATE" );
  TString output_name_crqcd = Form("%s/%s_crqcd_skim.root", argv[3], argv[2]);
  TFile * outfile_crqcd = TFile::Open( output_name_crqcd , "RECREATE" );
  TString output_name_crdy = Form("%s/%s_crdy_skim.root", argv[3], argv[2]);
  TFile * outfile_crdy = TFile::Open( output_name_crdy , "RECREATE" );

  TTree * skimmed = tree_mt2->CopyTree ( selection_string );

  outfile_sr->cd();
  TTree * skimmed_sr = skimmed->CopyTree( "deltaPhiMin > 0.3 && nMuons10 + nElectrons10 + nPFLep5LowMT + nPFHad10LowMT == 0" );
  if (skimmed_sr->GetEntries() > 0) {
    cout << skimmed_sr->GetEntries() << " events found in SR" << endl;
    skimmed_sr->Write();
  } else cout << "No events in SR" << endl;
  outfile_sr->Close();

  outfile_crsl->cd();
  TTree * skimmed_crsl = skimmed->CopyTree( "deltaPhiMin > 0.3 && nLepLowMT == 1" );
  if (skimmed_crsl->GetEntries() > 0) {
    cout << skimmed_crsl->GetEntries() << " events found in CRSL" << endl;
    skimmed_crsl->Write();
  } else cout << "No events in CRSL" << endl;
  outfile_crsl->Close();

  outfile_crqcd->cd();
  TTree * skimmed_crqcd = skimmed->CopyTree( "deltaPhiMin < 0.3 && nMuons10 + nElectrons10 + nPFLep5LowMT + nPFHad10LowMT == 0 && nJet200MuFrac50DphiMet == 0 && met_miniaodPt / met_caloPt < 5.0" );
  if (skimmed_crqcd->GetEntries() > 0) {
    cout << skimmed_crqcd->GetEntries() << " events found in CRQCD" << endl;
    skimmed_crqcd->Write();
  } else cout << "No events in CRQCD" << endl;
  outfile_crqcd->Close();

  outfile_crdy->cd();
  TTree * skimmed_crdy = tree_mt2->CopyTree( "nlep == 2 && zll_deltaPhiMin > 0.3 && zll_diffMetMht / zll_met_pt < 0.5 && nJet30 > 1 && zll_mt2 > 200 && (zll_met_pt > 250 || (zll_ht > 1000 && zll_met_pt > 30)) && jet1_pt > 30 && jet2_pt > 30 && nshorttracks > 0" );
  if (skimmed_crdy->GetEntries() > 0) {
    cout << skimmed_crdy->GetEntries() << " events found in CRDY" << endl;
    skimmed_crdy->Write();
  } else cout << "No events in CRDY" << endl;
  outfile_crdy->Close();
}

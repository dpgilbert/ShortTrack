#include "T1vsT5.h"

using namespace std;
using namespace mt2;
using namespace duplicate_removal;

void T1vsT5::loop (const TChain * chain, const char* sample, const char* outdir, const int maxEvt) {

  TH1::SetDefaultSumw2(true);

  const float mt2bins[4] = {200, 300, 400, 1500};

  TH1F h_2to3("h_2to3","M_{T2}, 2-3 Jets",3,mt2bins);
  TH1F h_4to6("h_4to6","M_{T2}, 4-6 Jets",3,mt2bins);
  TH1F h_7plus("h_7plus","M_{T2}, 7+ Jets",3,mt2bins);

  TH1F h_VL("h_VL","M_{T2}, 250 < H_{T} < 450 GeV",3,mt2bins);
  TH1F h_L("h_L","M_{T2}, 450 < H_{T} < 575 GeV",3,mt2bins);
  TH1F h_M("h_M","M_{T2}, 575 < H_{T} < 1000 GeV",3,mt2bins);
  TH1F h_H("h_H","M_{T2}, 1000 < H_{T} < 1500 GeV",3,mt2bins);
  TH1F h_UH("h_UH","M_{T2}, 1500 < H_{T}",3,mt2bins);

  TH1F h_VL_2to3("h_VL_2to3","M_{T2}, VL 2-3J",3,mt2bins);
  TH1F h_VL_4to6("h_VL_4to6","M_{T2}, VL 4-6J",3,mt2bins);
  TH1F h_VL_7plus("h_VL_7plus","M_{T2}, VL 7+J",3,mt2bins);
  TH1F h_L_2to3("h_L_2to3","M_{T2}, L 2-3J",3,mt2bins);
  TH1F h_L_4to6("h_L_4to6","M_{T2}, L 4-6J",3,mt2bins);
  TH1F h_L_7plus("h_L_7plus","M_{T2}, L 7+J",3,mt2bins);
  TH1F h_M_2to3("h_M_2to3","M_{T2}, M 2-3J",3,mt2bins);
  TH1F h_M_4to6("h_M_4to6","M_{T2}, M 4-6J",3,mt2bins);
  TH1F h_M_7plus("h_M_7plus","M_{T2}, M 7+J",3,mt2bins);
  TH1F h_H_2to3("h_H_2to3","M_{T2}, H 2-3J",3,mt2bins);
  TH1F h_H_4to6("h_H_4to6","M_{T2}, H 4-6J",3,mt2bins);
  TH1F h_H_7plus("h_H_7plus","M_{T2}, H 7+J",3,mt2bins);
  TH1F h_UH_2to3("h_UH_2to3","M_{T2}, UH 2-3J",3,mt2bins);
  TH1F h_UH_4to6("h_UH_4to6","M_{T2}, UH 4-6J",3,mt2bins);
  TH1F h_UH_7plus("h_UH_7plus","M_{T2}, UH 7+J",3,mt2bins);

  TTree * tree = (TTree*)chain->Clone("mt2");
  mt2tree mt2_tree;
  mt2_tree.Init(tree);

  int available = tree->GetEntries();
  int max_event = maxEvt < 0 ? available : min(maxEvt, available);
  float w_ = 35.9 * 1000 * (0.0027613) / available;
  
  cout << "Running on " << max_event << " events." << endl;
  cout << "Tree contains " << available << " events." << endl;

  for (int event = 0; event < max_event; event++) {
    if ( (event+1) % 100000 == 0) cout << "Processing event " << event+1 << ", " << (100. * event) / max_event << "% complete" << endl;

    mt2_tree.GetEntry(event);

    const int run_ = mt2_tree.run;
    const int evt_ = mt2_tree.evt;
    const int lumi_ = mt2_tree.lumi;

    if (mt2_tree.nVert == 0) continue;
    
    // Filters
    if (!mt2_tree.Flag_globalSuperTightHalo2016Filter) continue; 
    if (!mt2_tree.Flag_badMuonFilterV2) continue;
    if (!mt2_tree.Flag_eeBadScFilter) continue; 

    if (!mt2_tree.Flag_goodVertices) continue;
    if (!mt2_tree.Flag_HBHENoiseFilter) continue;
    if (!mt2_tree.Flag_HBHENoiseIsoFilter) continue;
    if (!mt2_tree.Flag_EcalDeadCellTriggerPrimitiveFilter) continue;
    if (!mt2_tree.Flag_badChargedHadronFilterV2) continue; 

    const float ht_ = mt2_tree.ht;
    const float met_ = mt2_tree.met_pt;

    if (isinf(ht_) || isnan(ht_) || isinf(met_) || isnan(met_)) continue;

    const int njet_ = mt2_tree.nJet30;
    if (njet_ < 2) continue;

    if (mt2_tree.jet_pt[0] > 13000.) continue;

    const int nlep_ = mt2_tree.nMuons10 + mt2_tree.nElectrons10 + mt2_tree.nPFLep5LowMT + mt2_tree.nPFHad10LowMT;
    const float deltaPhiMin_ = mt2_tree.deltaPhiMin;
    const float diffMetMhtOverMet_ = mt2_tree.diffMetMht / met_;
    const float mt2_ = mt2_tree.mt2;

    if (mt2_ < 200) continue;
    if (nlep_ > 0) continue;
    if (deltaPhiMin_ > 0.3) continue;
    if (diffMetMhtOverMet_ > 0.5) continue;
    if (met_ < 30 || ht_ < 250 || (ht_ < 1200 && met_ < 250)) continue;
    if ((mt2_tree.met_miniaodPt > 200.) && (mt2_tree.met_miniaodPt / mt2_tree.met_caloPt > 5.0)) continue;
    if (mt2_tree.nJet200MuFrac50DphiMet > 0) continue;;

    const float j1pt_ = mt2_tree.jet_pt[0];
    const float j2pt_ = mt2_tree.jet_pt[1];
    const int nb_ = mt2_tree.nBJet20;

    if (njet_ >= 7) {
      h_7plus.Fill(mt2_,w_);
      if (ht_ < 450) {
	h_VL_7plus.Fill(mt2_,w_);
      } else if (ht_ < 575) {
	h_L_7plus.Fill(mt2_,w_);
      } else if (ht_ < 1000) {
	h_M_7plus.Fill(mt2_,w_);
      } else if (ht_ < 1500) {
	h_H_7plus.Fill(mt2_,w_);
      } else {
	h_UH_7plus.Fill(mt2_,w_);
      }
    }
    else if (njet_ >= 4) {
      h_4to6.Fill(mt2_,w_);
      if (ht_ < 450) {
	h_VL_4to6.Fill(mt2_,w_);
      } else if (ht_ < 575) {
	h_L_4to6.Fill(mt2_,w_);
      } else if (ht_ < 1000) {
	h_M_4to6.Fill(mt2_,w_);
      } else if (ht_ < 1500) {
	h_H_4to6.Fill(mt2_,w_);
      } else {
	h_UH_4to6.Fill(mt2_,w_);
      }
    }
    else if (njet_ >= 2) {
      h_2to3.Fill(mt2_,w_);
      if (ht_ < 450) {
	h_VL_2to3.Fill(mt2_,w_);
      } else if (ht_ < 575) {
	h_L_2to3.Fill(mt2_,w_);
      } else if (ht_ < 1000) {
	h_M_2to3.Fill(mt2_,w_);
      } else if (ht_ < 1500) {
	h_H_2to3.Fill(mt2_,w_);
      } else {
	h_UH_2to3.Fill(mt2_,w_);
      }
    }

    if (ht_ < 450) {
      h_VL.Fill(mt2_,w_);
    } else if (ht_ < 575) {
      h_L.Fill(mt2_,w_);
    } else if (ht_ < 1000) {
      h_M.Fill(mt2_,w_);
    } else if (ht_ < 1500) {
      h_H.Fill(mt2_,w_);
    } else {
      h_UH.Fill(mt2_,w_);
    }

  } // Event Loop
  
  cout << "Finished event loop." << endl;

  TString outname = Form("%s/%s.root",outdir,sample);

  TFile outfile(outname, "RECREATE");

  h_2to3.Write();
  h_4to6.Write();
  h_7plus.Write();

  h_VL.Write();
  h_L.Write();
  h_M.Write();
  h_H.Write();
  h_UH.Write();

  h_VL_2to3.Write();
  h_L_2to3.Write();
  h_M_2to3.Write();
  h_H_2to3.Write();
  h_UH_2to3.Write();
  h_VL_4to6.Write();
  h_L_4to6.Write();
  h_M_4to6.Write();
  h_H_4to6.Write();
  h_UH_4to6.Write();
  h_VL_7plus.Write();
  h_L_7plus.Write();
  h_M_7plus.Write();
  h_H_7plus.Write();
  h_UH_7plus.Write();

  outfile.Close();

} // loop function


int main (int argc, char ** argv) {
  int maxEvt = -1;
  if (argc > 4) maxEvt = atoi(argv[4]);

  TChain* ch = new TChain("mt2");
  
  TString infile_name = Form("%s/%s*.root", argv[1], argv[2]);
  ch->Add(infile_name);

  T1vsT5 * l = new T1vsT5();
  l->loop(ch, argv[2], argv[3], maxEvt);
  return 0;
}

T1vsT5::T1vsT5() {}

T1vsT5::~T1vsT5() {};

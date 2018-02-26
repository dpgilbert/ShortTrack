#ifndef sttree_h
#define sttree_h

#include "TROOT.h"
#include "TChain.h"
#include "TFile.h"

#include "mt2tree.h"

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class sttree : public mt2tree {

public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

   // Declaration of leaf types

   // Short tracks
   Int_t           ntracks;
   Int_t           nshorttracks;
   Int_t           nshorttracks_short;
   Int_t           nshorttracks_trkshort;
   Int_t           nshorttracks_trklong;   

   Int_t           track_isshort[50];
   Int_t           track_ispixelonly[50];
   Int_t           track_istrkshort[50];
   Int_t           track_istrklong[50];
   Float_t         track_pt[50];
   Float_t         track_ptErr[50];
   Float_t         track_eta[50];
   Float_t         track_phi[50];
   Float_t         track_nChi2[50];
   Float_t         track_ipSigXY[50];

   TBranch         *b_ntracks;
   TBranch         *b_nshorttracks;
   TBranch         *b_nshorttracks_trkshort;
   TBranch         *b_nshorttracks_trklong;
   TBranch         *b_track_ispixelonly;
   TBranch         *b_track_istrkshort;
   TBranch         *b_track_istrklong;
   TBranch         *b_track_pt;
   TBranch         *b_track_ptErr;
   TBranch         *b_track_eta;
   TBranch         *b_track_phi;
   TBranch         *b_track_nChi2;
   TBranch         *b_track_ipSigXY;

   sttree(TTree *tree=0);
   virtual ~sttree();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init_ST(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef sttree_cxx
sttree::sttree(TTree *tree) : fChain(0) 
{
   Init_ST(tree);
}

sttree::~sttree()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t sttree::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t sttree::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void sttree::Init_ST(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("ntracks", &ntracks, &b_ntracks);
   fChain->SetBranchAddress("nshorttracks", &nshorttracks, &b_nshorttracks);
   fChain->SetBranchAddress("nshorttracks_short", &nshorttracks_trkshort, &b_nshorttracks_trkshort);
   fChain->SetBranchAddress("nshorttracks_long", &nshorttracks_trklong, &b_nshorttracks_trklong);
   fChain->SetBranchAddress("track_ispixelonly", &track_ispixelonly, &b_track_ispixelonly);
   fChain->SetBranchAddress("track_istrkshort", &track_istrkshort, &b_track_istrkshort);
   fChain->SetBranchAddress("track_istrklong", &track_istrklong, &b_track_istrklong);
   fChain->SetBranchAddress("track_pt", &track_pt, &b_track_pt);
   fChain->SetBranchAddress("track_ptErr", &track_ptErr, &b_track_ptErr);
   fChain->SetBranchAddress("track_eta", &track_eta, &b_track_eta);
   fChain->SetBranchAddress("track_phi", &track_phi, &b_track_phi);
   fChain->SetBranchAddress("track_phi", &track_phi, &b_track_phi);
   fChain->SetBranchAddress("track_nChi2", &track_nChi2, &b_track_nChi2);
   fChain->SetBranchAddress("track_ipSigXY", &track_ipSigXY, &b_track_ipSigXY);

   Notify();
}

Bool_t sttree::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void sttree::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t sttree::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef sttree_cxx

//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Thu Feb 22 17:38:52 2018 by ROOT version 6.10/09
// from TTree tree/tree
// found on file: score_deepReco_ttbb_0.root
//////////////////////////////////////////////////////////

#ifndef idxToNtuple_h
#define idxToNtuple_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

#include <iostream>
#include <string>
// Header file for the classes stored in the TTree if any.

class idxToNtuple {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Float_t         MLScore;
   Int_t           nevt;
   Int_t           njets;
   Int_t           nbjets_m;
   Int_t           EventCategory;
   Int_t           genMatch;
   Int_t           jet0Idx;
   Int_t           jet1Idx;
   Int_t           jet2Idx;
   Int_t           jet3Idx;
   Float_t         lepPt;
   Float_t         missinget;
   Float_t         whMass;
   Float_t         leptMass;
   Float_t         hadtMass;

   // List of branches
   TBranch        *b_MLScore;   //!
   TBranch        *b_nevt;   //!
   TBranch        *b_njets;   //!
   TBranch        *b_nbjets_m;   //!
   TBranch        *b_EventCategory;   //!
   TBranch        *b_genMatch;   //!
   TBranch        *b_jet0Idx;   //!
   TBranch        *b_jet1Idx;   //!
   TBranch        *b_jet2Idx;   //!
   TBranch        *b_jet3Idx;   //!
   TBranch        *b_lepPt;   //!
   TBranch        *b_missinget;   //!
   TBranch        *b_whMass;   //!
   TBranch        *b_leptMass;   //!
   TBranch        *b_hadtMass;   //!

   idxToNtuple(TTree *tree=0);
   virtual ~idxToNtuple();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop(const std::string channel);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);

};

#endif

#ifdef idxToNtuple_cxx
idxToNtuple::idxToNtuple(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("score_deepReco_ttbb_0.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("score_deepReco_ttbb_0.root");
      }
      f->GetObject("tree",tree);

   }
   Init(tree);
}

idxToNtuple::~idxToNtuple()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t idxToNtuple::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t idxToNtuple::LoadTree(Long64_t entry)
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

void idxToNtuple::Init(TTree *tree)
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

   fChain->SetBranchAddress("MLScore", &MLScore, &b_MLScore);
   fChain->SetBranchAddress("nevt", &nevt, &b_nevt);
   fChain->SetBranchAddress("njets", &njets, &b_njets);
   fChain->SetBranchAddress("nbjets_m", &nbjets_m, &b_nbjets_m);
   fChain->SetBranchAddress("EventCategory", &EventCategory, &b_EventCategory);
   fChain->SetBranchAddress("genMatch", &genMatch, &b_genMatch);
   fChain->SetBranchAddress("jet0Idx", &jet0Idx, &b_jet0Idx);
   fChain->SetBranchAddress("jet1Idx", &jet1Idx, &b_jet1Idx);
   fChain->SetBranchAddress("jet2Idx", &jet2Idx, &b_jet2Idx);
   fChain->SetBranchAddress("jet3Idx", &jet3Idx, &b_jet3Idx);
   fChain->SetBranchAddress("lepPt", &lepPt, &b_lepPt);
   fChain->SetBranchAddress("missinget", &missinget, &b_missinget);
   fChain->SetBranchAddress("whMass", &whMass, &b_whMass);
   fChain->SetBranchAddress("leptMass", &leptMass, &b_leptMass);
   fChain->SetBranchAddress("hadtMass", &hadtMass, &b_hadtMass);
   Notify();
}

Bool_t idxToNtuple::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void idxToNtuple::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t idxToNtuple::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef idxToNtuple_cxx

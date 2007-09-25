//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Aug  7 10:01:22 2006 by ROOT version 5.12/00
// from TTree T1/GeometryTest Tree
// found on file: matbdg_tree_TOB.root
//////////////////////////////////////////////////////////

#ifndef Loopers_h
#define Loopers_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TString.h>
#include <TGaxis.h>

#include <TTree.h>
#include <iostream>
#include <fstream>

using namespace std;

// histograms
// Target
TH1F* hist_loops;
TH1F* hist_energy_init;
TH1F* hist_energy_end;
TH1F* hist_density;
TH2F* hist_density_vs_loops;
TH1F* hist_pT_init;
TH1F* hist_pT_end;
TH1F* hist_energyLossPerTurn;
TH1F* hist_trackLength;
TH1F* hist_trackLengthPerTurn;
TH1F* hist_lastInteraction;
TH1F* hist_bx;
//

// logfile
ofstream theLogFile;
//

const double pi = 3.14159265;
const double c  = 299792458; // m/s;
const double bx = 25; // ns

class Loopers {
public :
  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  Int_t           fCurrent; //!current Tree number in a TChain
  
  // Declaration of leave types
  Float_t         ParticleEta;
  Float_t         ParticleMass;
  Int_t           Nsteps;
  Double_t        InitialX[10000];   //[Nsteps]
  Double_t        InitialY[10000];   //[Nsteps]
  Double_t        InitialZ[10000];   //[Nsteps]
  Double_t        FinalX[10000];   //[Nsteps]
  Double_t        FinalY[10000];   //[Nsteps]
  Double_t        FinalZ[10000];   //[Nsteps]
  Float_t         InitialE[10000];   //[Nsteps]
  Float_t         FinalE[10000];   //[Nsteps]
  Int_t           ParticleStepID[10000];   //[Nsteps]
  Int_t           ParticleStepPreInteraction[10000];   //[Nsteps]
  Int_t           ParticleStepPostInteraction[10000];   //[Nsteps]
  Float_t         MaterialDensity[10000];   //[Nsteps]
  Float_t         ParticleStepInitialPt[10000];   //[Nsteps]
  Float_t         ParticleStepFinalPt[10000];   //[Nsteps]
  
  // List of branches
  TBranch        *b_ParticleEta;   //!
  TBranch        *b_ParticleMass;   //!
  TBranch        *b_Nsteps;   //!
  TBranch        *b_InitialX;   //!
  TBranch        *b_InitialY;   //!
  TBranch        *b_InitialZ;   //!
  TBranch        *b_FinalX;   //!
  TBranch        *b_FinalY;   //!
  TBranch        *b_FinalZ;   //!
  TBranch        *b_FinalE;   //!
  TBranch        *b_InitialE;   //!
  TBranch        *b_ParticleStepID;   //!
  TBranch        *b_ParticleStepPreInteraction;   //!
  TBranch        *b_ParticleStepPostInteraction;   //!
  TBranch        *b_MaterialDensity;   //!
  TBranch        *b_ParticleStepInitialPt;   //!
  TBranch        *b_ParticleStepFinalPt;   //!
  
  Loopers(TString fileName);
  virtual ~Loopers();
  virtual Int_t    Cut(Long64_t entry);
  virtual Int_t    GetEntry(Long64_t entry);
  virtual Long64_t LoadTree(Long64_t entry);
  virtual void     Init(TTree *tree);
  virtual void     Loop();
  virtual Bool_t   Notify();
  virtual void     Show(Long64_t entry = -1);

  
private:
  //
  virtual void helpfulCommands();
  //
  // directory to store images
  TString theDirName;
  // decay
  Bool_t isDecay;
  Int_t actualParticleID;
};

#endif

#ifdef Loopers_cxx
Loopers::Loopers(TString fileName)
{
  // images directory
  theDirName = "Images";
  
  // files
  cout << "*** Open file... " << endl;
  cout << fileName << endl;
  cout << "***" << endl;
  cout << " Output Directory... " << endl;
  cout << theDirName << endl;
  cout << "***" << endl;
  //
  theLogFile.open("Loopers.log");
  //
  
  // open root files
  TFile* theDetectorFile = new TFile(fileName);
  //
  // get tree
  TTree* theTree = (TTree*)theDetectorFile->Get("T1");
  //
  Init(theTree);
  Book();
  //
  helpfulCommands();
  //
  isDecay = false;
  actualParticleID = 0;
}

Loopers::~Loopers()
{
  if (!fChain) return;
  delete fChain->GetCurrentFile();
}

Int_t Loopers::GetEntry(Long64_t entry)
{
  // Read contents of entry.
  if (!fChain) return 0;
  return fChain->GetEntry(entry);
}
Long64_t Loopers::LoadTree(Long64_t entry)
{
  // Set the environment to read one entry
  if (!fChain) return -5;
  Long64_t centry = fChain->LoadTree(entry);
  if (centry < 0) return centry;
  if (fChain->IsA() != TChain::Class()) return centry;
  TChain *chain = (TChain*)fChain;
  if (chain->GetTreeNumber() != fCurrent) {
    fCurrent = chain->GetTreeNumber();
    Notify();
  }
  return centry;
}

void Loopers::Init(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normaly not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).
  
  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fCurrent = -1;
  fChain->SetMakeClass(1);
  
  fChain->SetBranchAddress("Particle Eta", &ParticleEta, &b_ParticleEta);
  fChain->SetBranchAddress("Particle Mass", &ParticleMass, &b_ParticleMass);
  fChain->SetBranchAddress("Nsteps", &Nsteps, &b_Nsteps);
  fChain->SetBranchAddress("Initial X", InitialX, &b_InitialX);
  fChain->SetBranchAddress("Initial Y", InitialY, &b_InitialY);
  fChain->SetBranchAddress("Initial Z", InitialZ, &b_InitialZ);
  fChain->SetBranchAddress("Final X", FinalX, &b_FinalX);
  fChain->SetBranchAddress("Final Y", FinalY, &b_FinalY);
  fChain->SetBranchAddress("Final Z", FinalZ, &b_FinalZ);
  fChain->SetBranchAddress("Particle Step Initial Energy", InitialE, &b_InitialE);
  fChain->SetBranchAddress("Particle Step Final Energy", FinalE, &b_FinalE);
  fChain->SetBranchAddress("Particle Step ID", ParticleStepID, &b_ParticleStepID);
  fChain->SetBranchAddress("Particle Step Pre Interaction", ParticleStepPreInteraction, &b_ParticleStepPreInteraction);
  fChain->SetBranchAddress("Particle Step Post Interaction", ParticleStepPostInteraction, &b_ParticleStepPostInteraction);
  fChain->SetBranchAddress("Material Density", MaterialDensity, &b_MaterialDensity);
  fChain->SetBranchAddress("Particle Step Initial Pt", ParticleStepInitialPt, &b_ParticleStepInitialPt);
  fChain->SetBranchAddress("Particle Step Final Pt", ParticleStepFinalPt, &b_ParticleStepFinalPt);
  Notify();
}

Bool_t Loopers::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normaly not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.
  
  return kTRUE;
}

void Loopers::Show(Long64_t entry)
{
  // Print contents of entry.
  // If entry is not specified, print current entry
  if (!fChain) return;
  fChain->Show(entry);
}
Int_t Loopers::Cut(Long64_t entry)
{
  // This function may be called from Loop.
  // returns  1 if entry is accepted.
  // returns -1 otherwise.
  return 1;
}

void Loopers::Book(){
  hist_loops = new TH1F("hist_loops",
			"Number of Turns before stopping;Turns [2#pi];Events/bin",
			80,0,20);
  hist_energy_init = new TH1F("hist_energy_init",
			      "Energy at starting point;Energy [MeV];Events/bin",
			      100,0.0,1000.0);
  hist_energy_end = new TH1F("hist_energy_end",
			     "Energy at stopping point;Energy [MeV];Events/bin",
			     100,0.0,1000.0);
  hist_density = new TH1F("hist_density",
			  "Average Density;#bar{#rho} [g/cm^{3}];Events/bin",
			  30,0.0,0.3);
  hist_density_vs_loops = new TH2F("hist_density_vs_loops",
				   "Average Density vs Number of Turns;Turns [2#pi];#bar{#rho} [g/cm^{3}];Events/bin",
				   80,0,20,30,0.0,0.3);
  hist_pT_init = new TH1F("hist_pT_init",
			  "Transverse Momentum at starting point;p_{T} [MeV/c];Events/bin",
			  100,0.0,1000.0);
  hist_pT_end = new TH1F("hist_pT_end",
			 "Transverse Momentum at stopping point;p_{T} [MeV/c];Events/bin",
			 100,0.0,1000.0);
  hist_energyLossPerTurn = new TH1F("hist_energyLossPerTurn",
				    "Energy Loss per Turn;Energy [MeV];Events/bin",
				    50,0.0,500.0);
  hist_trackLength = new TH1F("hist_trackLength",
			      "Track Length;Length [mm];Events/bin",
			      100,0.0,100000.0);
  hist_trackLengthPerTurn = new TH1F("hist_trackLengthPerTurn",
				     "Track Length per Turn;Track Length [mm];Events/bin",
				     50,0.0,50000.0);
  hist_lastInteraction = new TH1F("hist_lastInteraction",
				  "Last Geant4 Process;Process Type;Events/bin",
				  11,-0.5,10.5);
  hist_bx = new TH1F("hist_bx",
		     "Bunch Crossing [25 ns];Bunch Crossing [25 ns];Events/bin",
		     21,-0.25,10.25);
}

void Loopers::MakePlots(TString suffix);

#endif // #ifdef Loopers_cxx

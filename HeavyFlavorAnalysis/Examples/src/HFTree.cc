#include <iostream>

#include "HeavyFlavorAnalysis/Examples/interface/HFTree.h"


#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TH1.h>

#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAna01Event.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaTrack.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TGenCand.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaVertex.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TAnaMuon.hh"
#include "AnalysisDataFormats/HeavyFlavorObjects/rootio/TTrgObj.hh"

// -- Yikes!
TAna01Event  *gHFEvent;
TFile        *gHFFile;

using namespace::std;

// ----------------------------------------------------------------------
HFTree::HFTree(const edm::ParameterSet& iConfig) :
  fRequireCand(iConfig.getUntrackedParameter<bool>("requireCand", true)), 
  fFileName(iConfig.getUntrackedParameter<string>("fileName", string("hfa.root"))), 
  fTreeName(iConfig.getUntrackedParameter<string>("treeName", string("T1"))), 
  fVerbose(iConfig.getUntrackedParameter<int>("verbose", 1)) {
  using namespace std;
  cout << "----------------------------------------------------------------------" << endl;
  cout << "--- HFTree constructor" << endl;
  cout << "---  verbose:                         " << fVerbose << endl;
  cout << "---  fileName:                        " << fFileName << endl; 
  cout << "---  treeName:                        " << fTreeName << endl; 
  cout << "---  requireCand:                     " << (fRequireCand?"true":"false") << endl; 
  cout << "----------------------------------------------------------------------" << endl;
  fFile = TFile::Open(fFileName.c_str(), "RECREATE");
  fTree = new TTree(fTreeName.c_str(), "CMSSW HF tree");
  fEvent = new TAna01Event(0);
  fTree->Branch("TAna01Event", "TAna01Event", &fEvent, 256000/8, 1);

  TH1D *h1 = new TH1D("h1", "h1", 20, 0., 20.);

  gHFEvent = fEvent;
  gHFFile  = fFile;

}


// ----------------------------------------------------------------------
HFTree::~HFTree() {
  
  // -- Save output
  fFile->cd();
  fTree->Write();
  fFile->Write();
  fFile->Close();
  delete fFile;
}


// ----------------------------------------------------------------------
void HFTree::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup) {
  gHFEvent->fRunNumber   = iEvent.id().run();
  gHFEvent->fEventNumber = iEvent.id().event();
    
  if (fRequireCand){
    if (gHFEvent->nCands() > 0) {
      if (fVerbose > 0) {
	cout << "HFTree> filling tree for run: " << gHFEvent->fRunNumber
	     << " event: "  << gHFEvent->fEventNumber ;

	if (fVerbose > 1) {
	  cout << " Cand: ";
	
	  for (int i = 0; i < gHFEvent->nCands(); ++i) {
	    cout << gHFEvent->getCand(i)->fType << " ";
	  }
	}
	cout << endl;
      }
      fTree->Fill();
    } 
  } else {
    fTree->Fill();
  }

  gHFEvent->Clear();
}

// ------------ method called once each job just before starting event loop  ------------
void  HFTree::beginJob(const edm::EventSetup&) {
}

// ------------ method called once each job just after ending the event loop  ------------
void  HFTree::endJob() {
}

//define this as a plug-in
//DEFINE_FWK_MODULE(HFTree);

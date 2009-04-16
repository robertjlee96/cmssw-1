
// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"


class TFile;
class TTree;
class TAna00Event;

class TrackAssociatorBase;

// ----------------------------------------------------------------------
class HFDumpJets : public edm::EDAnalyzer {
 public:
  explicit HFDumpJets(const edm::ParameterSet&);
  ~HFDumpJets();
  
 private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;

  int           fVerbose; 
  std::string   fJetsLabel;
  std::string   fGenCandidatesLabel;
  edm::InputTag fsourceByRefer;
 
  int nevt;
 

  

};

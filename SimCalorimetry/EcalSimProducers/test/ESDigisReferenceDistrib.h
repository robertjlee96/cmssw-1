#ifndef ESDigisReferenceDistrib_H
#define ESDigisReferenceDistrib_H

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/EcalDigi/interface/ESDataFrame.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"

#include <iostream>
#include <fstream>

#include "TFile.h"
#include "TH1F.h"
#include "TH3F.h"

class ESDigisReferenceDistrib: public edm::EDAnalyzer{
  
 public:
  
  /// Constructor
  ESDigisReferenceDistrib(const edm::ParameterSet& ps);
  
  /// Destructor
  ~ESDigisReferenceDistrib();
  
 protected:
  
  /// Analyze
  void analyze(const edm::Event& e, const edm::EventSetup& c);
  
  // BeginJob
  void beginJob(const edm::EventSetup& c);
  
  // EndJob
  void endJob(void);
  
 private:
  
  bool verbose_;
  
  std::string outputRootFile_;
  std::string outputTxtFile_;
  
  edm::InputTag ESdigiCollection_;
  
  TH3F* meESDigi3D_;  
  TH1F* meESDigiADC_[3];
};

#endif

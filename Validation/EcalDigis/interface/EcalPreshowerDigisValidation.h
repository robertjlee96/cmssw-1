#ifndef EcalPreshowerDigisValidation_H
#define EcalPreshowerDigisValidation_H

/*
 * \file EcalPreshowerDigisValidation.h
 *
 * $Date: 2008/02/18 14:20:28 $
 * $Revision: 1.6 $
 * \author F. Cossutti
 *
*/

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/EcalDigi/interface/ESDataFrame.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include "DQMServices/Core/interface/MonitorElement.h"

class EcalPreshowerDigisValidation: public edm::EDAnalyzer{

    typedef std::map<uint32_t,float,std::less<uint32_t> >  MapType;

public:

/// Constructor
EcalPreshowerDigisValidation(const edm::ParameterSet& ps);

/// Destructor
~EcalPreshowerDigisValidation();

protected:

/// Analyze
void analyze(const edm::Event& e, const edm::EventSetup& c);

// BeginJob
void beginJob(const edm::EventSetup& c);

// EndJob
void endJob(void);

private:

 bool verbose_;
 
 DQMStore* dbe_;
 
 std::string outputFile_;

 edm::InputTag ESdigiCollection_;

 MonitorElement* meESDigiMultiplicity_;
 
 MonitorElement* meESDigiADC_[3];

};

#endif

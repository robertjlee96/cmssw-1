#ifndef DQMOffline_Tau_TauDQMSimpleFileSaver_h
#define DQMOffline_Tau_TauDQMSimpleFileSaver_h

/** \class TauDQMSimpleFileSaver
 *  
 *  Class to write all monitor elements registered in DQMStore into ROOT file
 *  (without any naming restrictions imposed by "regular" DQMFileSaver)
 *
 *  $Date: 2012/05/10 17:36:29 $
 *  $Revision: 1.1 $
 *  \author Christian Veelken, UC Davis
 */

// framework & common header files
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <string>

class TauDQMSimpleFileSaver : public edm::EDAnalyzer
{
 public:
  explicit TauDQMSimpleFileSaver(const edm::ParameterSet&);
  virtual ~TauDQMSimpleFileSaver();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();  

private:
  std::string outputFileName_;
  int cfgError_;
};

#endif



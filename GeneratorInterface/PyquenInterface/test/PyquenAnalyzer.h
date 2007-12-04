// $Id: PyquenAnalyzer.h,v 1.2 2007/10/05 15:08:48 loizides Exp $

#ifndef PyquenAnalyzer_H
#define PyquenAnalyzer_H

#include "FWCore/Framework/interface/EDAnalyzer.h"

// forward declarations
class TFile;
class TH1D;


class PyquenAnalyzer : public edm::EDAnalyzer
{ //analyzer module to analyze pythia events
 public:
  explicit PyquenAnalyzer(const edm::ParameterSet& );
  virtual ~PyquenAnalyzer() {} 

  virtual void analyze(const edm::Event&, const edm::EventSetup& );
  virtual void beginJob(const edm::EventSetup& );
  virtual void endJob();

 private:
 
  TH1D*        phdNdEta;           // histogram for dN/deta
  TH1D*        phdNdY;             // histogram for dN/dy
  TH1D*        phdNdPt;            // histogram for dN/dpt
  TH1D*        phdNdPhi;           // histogram for dN/dphi
};

#endif


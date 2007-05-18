#ifndef RecoBTau_GenericMVAJetTagComputer_h
#define RecoBTau_GenericMVAJetTagComputer_h

#include <string>
#include <memory>

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DataFormats/BTauReco/interface/BaseTagInfo.h"
#include "RecoBTau/JetTagComputer/interface/JetTagComputer.h"
#include "RecoBTau/JetTagComputer/interface/GenericMVAComputer.h"

class GenericMVAJetTagComputer : public JetTagComputer
{
 public:
   GenericMVAJetTagComputer(const edm::ParameterSet & parameters) :
	m_calibrationLabel(parameters.getParameter<std::string>("calibrationRecord")) {}
   virtual ~GenericMVAJetTagComputer() {}

   virtual void setEventSetup(const edm::EventSetup &es) const;

   virtual float discriminator(const reco::BaseTagInfo &baseTag) const 
    {
      return m_mvaComputer->eval(baseTag.taggingVariables());
    }

 private:
   std::string m_calibrationLabel;
   mutable std::auto_ptr<GenericMVAComputer> m_mvaComputer;
  // edm::EventSetup * m_eventSetup;
};

#endif

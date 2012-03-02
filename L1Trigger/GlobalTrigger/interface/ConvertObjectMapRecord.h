#ifndef L1Trigger_GlobalTrigger_ConvertObjectMapRecord_h
#define L1Trigger_GlobalTrigger_ConvertObjectMapRecord_h

/**
 * \class ConvertObjectMapRecord
 * 
 * 
 * Description: Reads in the L1GlobalTriggerObjectMapRecord
 *              and copies the information it contains into
 *              a L1GlobalTriggerObjectMaps object and also
 *              puts the names it contains into the ParameterSet
 *              registry.  
 *
 * Implementation:
 *    <TODO: enter implementation details>
 *   
 * \author: Vasile Mihai Ghete - HEPHY Vienna
 * \author: W. David Dagenhart
 * 
 * $Date$
 * $Revision$
 *
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Utilities/interface/InputTag.h"

class ConvertObjectMapRecord : public edm::EDProducer {

public:
    explicit ConvertObjectMapRecord(const edm::ParameterSet& pset);
    ~ConvertObjectMapRecord();

    virtual void produce(edm::Event& event, const edm::EventSetup& es);

private:
    edm::InputTag m_l1GtObjectMapTag;
};

#endif

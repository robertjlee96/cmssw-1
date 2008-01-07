#ifndef GlobalTrigger_L1GtJetCountsCondition_h
#define GlobalTrigger_L1GtJetCountsCondition_h

/**
 * \class L1GtJetCountsCondition
 * 
 * 
 * Description: evaluation of a CondJetCounts condition.
 * 
 * Implementation:
 *    <TODO: enter implementation details>
 *   
 * \author: Vasile Mihai Ghete   - HEPHY Vienna 
 * 
 * $Date$
 * $Revision$
 *
 */

// system include files
#include <iosfwd>
#include <string>

// user include files
//   base classes
#include "L1Trigger/GlobalTrigger/interface/L1GtConditionEvaluation.h"

#include "FWCore/Framework/interface/EventSetup.h"

// forward declarations
class L1GtCondition;
class L1GtJetCountsTemplate;

class L1GlobalTriggerPSB;

// class declaration
class L1GtJetCountsCondition : public L1GtConditionEvaluation
{

public:

    /// constructors
    ///     default
    L1GtJetCountsCondition();

    ///     from base template condition (from event setup usually)
    L1GtJetCountsCondition(L1GtCondition*, const L1GlobalTriggerPSB*,
        const edm::EventSetup& evSetup);

    // copy constructor
    L1GtJetCountsCondition(const L1GtJetCountsCondition&);

    // destructor
    virtual ~L1GtJetCountsCondition();

    // assign operator
    L1GtJetCountsCondition& operator=(const L1GtJetCountsCondition&);

public:

    /// the core function to check if the condition matches
    virtual const bool evaluateCondition() const;

    /// print condition
    virtual void print(std::ostream& myCout) const;

public:

    ///   get / set the pointer to a L1GtCondition
    inline const L1GtJetCountsTemplate* gtJetCountsTemplate() const {
        return m_gtJetCountsTemplate;
    }

    void setGtJetCountsTemplate(const L1GtJetCountsTemplate*);

    ///   get / set the pointer to PSB
    inline const L1GlobalTriggerPSB* gtPSB() const {
        return m_gtPSB;
    }

    void setGtPSB(const L1GlobalTriggerPSB*);

private:

    /// copy function for copy constructor and operator=
    void copy(const L1GtJetCountsCondition& cp);

private:

    /// pointer to a L1GtJetCountsTemplate
    const L1GtJetCountsTemplate* m_gtJetCountsTemplate;

    /// pointer to PSB, to be able to get the trigger objects
    const L1GlobalTriggerPSB* m_gtPSB;

    /// maximum number of jet counts
    unsigned int m_numberL1JetCounts;
    
};

#endif

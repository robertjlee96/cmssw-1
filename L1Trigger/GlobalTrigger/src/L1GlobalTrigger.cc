/**
 * \class L1GlobalTrigger
 * 
 * 
 * Description: see header file.  
 *
 * Implementation:
 *    <TODO: enter implementation details>
 *   
 * \author: Vasile Mihai Ghete - HEPHY Vienna
 * 
 * $Date$
 * $Revision$
 *
 */

// this class header
#include "L1Trigger/GlobalTrigger/interface/L1GlobalTrigger.h"

// system include files
#include <memory>
#include <iostream>
#include <iomanip>
#include <bitset>

#include <boost/cstdint.hpp>

// user include files
//#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutSetupFwd.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerEvmReadoutRecord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMapRecord.h"

#include "DataFormats/L1GlobalTrigger/interface/L1GlobalTriggerObjectMap.h"

#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTReadoutCollection.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTCand.h"
#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuGMTExtendedCand.h"

#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEmCand.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctJetCand.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctEtSums.h"
#include "DataFormats/L1GlobalCaloTrigger/interface/L1GctJetCounts.h"

#include "L1Trigger/GlobalTrigger/interface/L1GlobalTriggerSetup.h"
#include "L1Trigger/GlobalTrigger/interface/L1GlobalTriggerConfig.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "CondFormats/L1TObjects/interface/L1GtParameters.h"
#include "CondFormats/DataRecord/interface/L1GtParametersRcd.h"

#include "CondFormats/L1TObjects/interface/L1GtFwd.h"
#include "CondFormats/L1TObjects/interface/L1GtBoard.h"
#include "CondFormats/L1TObjects/interface/L1GtBoardMaps.h"
#include "CondFormats/DataRecord/interface/L1GtBoardMapsRcd.h"

#include "L1Trigger/GlobalTrigger/interface/L1GlobalTriggerPSB.h"
#include "L1Trigger/GlobalTrigger/interface/L1GlobalTriggerGTL.h"
#include "L1Trigger/GlobalTrigger/interface/L1GlobalTriggerFDL.h"

#include "DataFormats/L1GlobalTrigger/interface/L1GtfeWord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1GtfeExtWord.h"
#include "DataFormats/L1GlobalTrigger/interface/L1TcsWord.h"

#include "DataFormats/Common/interface/RefProd.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/MessageLogger/interface/MessageDrop.h"



// constructors

L1GlobalTrigger::L1GlobalTrigger(const edm::ParameterSet& parSet)
{

    LogDebug ("Trace") << "Entering L1GlobalTriger constructor";

    // input tag for muon collection from GMT
    m_muGmtInputTag = parSet.getUntrackedParameter<edm::InputTag>(
                          "GmtInputTag", edm::InputTag("L1GmtEmulDigis"));

    LogDebug("L1GlobalTrigger")
    << "\nInput tag for muon collection from GMT: "
    << m_muGmtInputTag.label() << " \n"
    << std::endl;

    // input tag for calorimeter collection from GCT
    m_caloGctInputTag = parSet.getUntrackedParameter<edm::InputTag>(
                            "GctInputTag", edm::InputTag("L1GctEmulDigis"));

    LogDebug("L1GlobalTrigger")
    << "\nInput tag for calorimeter collection from GCT: "
    << m_caloGctInputTag.label() << " \n"
    << std::endl;

    // set L1 GT configuration parameters
    if(!m_gtSetup) {
        m_gtSetup = new L1GlobalTriggerSetup(*this, parSet);
    }

    // register products
    produces<L1GlobalTriggerReadoutRecord>();
    produces<L1GlobalTriggerEvmReadoutRecord>();
    produces<L1GlobalTriggerObjectMapRecord>();


    // create new PSBs
    LogDebug("L1GlobalTrigger") << "\n Creating GT PSBs" << std::endl;
    m_gtPSB = new L1GlobalTriggerPSB(*this);

    // create new GTL
    LogDebug("L1GlobalTrigger") << "\n Creating GT GTL" << std::endl;
    m_gtGTL = new L1GlobalTriggerGTL(*this);

    // create new FDL
    LogDebug("L1GlobalTrigger") << "\n Creating GT FDL" << std::endl;
    m_gtFDL = new L1GlobalTriggerFDL(*this);



}

// destructor
L1GlobalTrigger::~L1GlobalTrigger()
{

    if(m_gtSetup)
        delete m_gtSetup;
    m_gtSetup = 0;

    delete m_gtPSB;
    delete m_gtGTL;
    delete m_gtFDL;
}

// member functions

// method called to produce the data
void L1GlobalTrigger::produce(edm::Event& iEvent, const edm::EventSetup& evSetup)
{

    // process event iEvent
    
    // get from EventSetup
    //  parameter record
    edm::ESHandle< L1GtParameters > l1GtPar;
    evSetup.get< L1GtParametersRcd >().get( l1GtPar );
    //  board maps
    edm::ESHandle< L1GtBoardMaps > l1GtBM;
    evSetup.get< L1GtBoardMapsRcd >().get( l1GtBM );

    //    total number of Bx's in the event
    int totalBxInEvent = l1GtPar->gtTotalBxInEvent();
    
    int minBxInEvent = (totalBxInEvent + 1)/2 - totalBxInEvent;
    int maxBxInEvent = (totalBxInEvent + 1)/2 - 1;

    LogDebug("L1GlobalTrigger")
    << "\nTotal number of bunch crosses to put in the GT readout record: "
    << totalBxInEvent << " = " << "["
    << minBxInEvent << ", " << maxBxInEvent << "] BX\n"
    << std::endl;

    // get from EventSetup: active boards
    boost::uint16_t activeBoards = l1GtPar->gtActiveBoards();
    
    LogDebug("L1GlobalTrigger")
    << "\n  Active boards in L1 GT (hex format) = "
    << std::hex << std::setw(sizeof(activeBoards)*2) << std::setfill('0')
    << activeBoards
    << std::dec << std::setfill(' ')
    << std::endl;

    

    // * produce the L1GlobalTriggerReadoutRecord
    LogDebug("L1GlobalTrigger")
    << "\nL1GlobalTrigger : producing L1GlobalTriggerReadoutRecord\n"
    << std::endl;

    std::auto_ptr<L1GlobalTriggerReadoutRecord> gtReadoutRecord(
        new L1GlobalTriggerReadoutRecord(totalBxInEvent) );


    // * produce the L1GlobalTriggerEvmReadoutRecord
    LogDebug("L1GlobalTrigger")
    << "\nL1GlobalTrigger : producing L1GlobalTriggerEvmReadoutRecord\n"
    << std::endl;

    std::auto_ptr<L1GlobalTriggerEvmReadoutRecord> gtEvmReadoutRecord(
        new L1GlobalTriggerEvmReadoutRecord(totalBxInEvent) );

    // * produce the L1GlobalTriggerObjectMapRecord
    LogDebug("L1GlobalTrigger")
    << "\nL1GlobalTrigger : producing L1GlobalTriggerObjectMapRecord\n"
    << std::endl;

    std::auto_ptr<L1GlobalTriggerObjectMapRecord> gtObjectMapRecord(
        new L1GlobalTriggerObjectMapRecord() );



    // * create L1GtfeExtWord

    L1GtfeExtWord gtfeExtWordValue;
    
    int iBoard = 0;    
    L1GtBoard gtfeBoard = L1GtBoard(GTFE, iBoard);    
    gtfeExtWordValue.setBoardId( l1GtBM->boardId(gtfeBoard) );
    
    // cast int to boost::uint16_t (there are normally 3 or 5 BxInEvent)
    gtfeExtWordValue.setRecordLength(static_cast<boost::uint16_t>(totalBxInEvent));

    // set the list of active boards
    gtfeExtWordValue.setActiveBoards(activeBoards);
    
    // set the TOTAL_TRIGNR as read from iEvent
    // TODO check again - PTC stuff
    
    gtfeExtWordValue.setTotalTriggerNr(static_cast<boost::uint32_t>(iEvent.id().event()));

    // ** fill L1GtfeWord in GT DAQ record

    L1GtfeWord& gtfeWordValue = dynamic_cast<L1GtfeExtWord&>(gtfeExtWordValue);
    gtReadoutRecord->setGtfeWord(gtfeWordValue);

    // ** fill L1GtfeExtWord in GT EVM record
    gtEvmReadoutRecord->setGtfeWord(gtfeExtWordValue);

    LogDebug("L1GlobalTrigger")
    << "\n  GTFE board " << gtReadoutRecord->gtfeWord().boardId() << "\n"
    << "    GTFE word: total number of bx in DAQ record = "
    << gtReadoutRecord->gtfeWord().recordLength()
    << "    GTFE word: total number of bx in EVM record = "
    << gtEvmReadoutRecord->gtfeWord().recordLength()
    << std::endl;

    // * create L1TcsWord

    L1TcsWord tcsWordValue;

    iBoard = 0;    
    L1GtBoard tcsBoard = L1GtBoard(TCS, iBoard);    
    tcsWordValue.setBoardId( l1GtBM->boardId(tcsBoard) ); 

    boost::uint16_t trigType = 0x5; // 0101 simulated event
    tcsWordValue.setTriggerType(trigType);

    // set the Event_Nr as read from iEvent
    tcsWordValue.setEventNr(static_cast<boost::uint32_t>(iEvent.id().event()));

    // ** fill L1TcsWord in the EVM record

    gtEvmReadoutRecord->setTcsWord(tcsWordValue);
    LogDebug("L1GlobalTrigger")
    << "\n  TCS word: trigger type = "
    << std::bitset<4>(gtEvmReadoutRecord->tcsWord().triggerType())
    << std::endl;

    // loop over bx in event
    for (int iBxInEvent = minBxInEvent; iBxInEvent <= maxBxInEvent;
            ++iBxInEvent) {

        // * receive GCT data via PSBs
        if ( m_gtPSB ) {
            LogDebug("L1GlobalTrigger")
            << "\nL1GlobalTrigger : running PSB for bx = " << iBxInEvent << "\n"
            << std::endl;
            m_gtPSB->receiveData(iEvent, m_caloGctInputTag, iBxInEvent, evSetup);
        }

        // * receive GMT data via GTL
        if ( m_gtGTL ) {
            LogDebug("L1GlobalTrigger")
            << "\nL1GlobalTrigger : receiving GMT data for bx = " << iBxInEvent << "\n"
            << std::endl;
            m_gtGTL->receiveData(iEvent, m_muGmtInputTag, iBxInEvent, evSetup);
        }

        // * run GTL
        if ( m_gtGTL ) {
            LogDebug("L1GlobalTrigger")
            << "\nL1GlobalTrigger : running GTL for bx = " << iBxInEvent << "\n"
            << std::endl;

            m_gtGTL->run(iBxInEvent);

            LogDebug("L1GlobalTrigger")
            << "\n AlgorithmOR\n" << m_gtGTL->getAlgorithmOR() << "\n"
            << std::endl;

            if (iBxInEvent == 0) { // TODO map only for BX = 0?

                const std::vector<L1GlobalTriggerObjectMap>* objMapVec = m_gtGTL->objectMap();

                gtObjectMapRecord->setGtObjectMap(*objMapVec);
                delete objMapVec;

            }

        }

        // * run FDL
        if ( m_gtFDL ) {
            LogDebug("L1GlobalTrigger")
            << "\nL1GlobalTrigger : running FDL for bx = " << iBxInEvent << "\n"
            << std::endl;

            m_gtFDL->run(iBxInEvent, evSetup);

            std::ostringstream myCoutStream;
            if ( edm::isDebugEnabled() ) {
                m_gtFDL->gtFdlWord()->printGtDecisionWord(myCoutStream);
            }
            LogDebug("L1GlobalTrigger")
            << "\n FDL decision word\n" << myCoutStream.str() << "\n"
            << std::endl;

            gtReadoutRecord->setGtFdlWord( *(*m_gtFDL).gtFdlWord(), iBxInEvent );
            gtEvmReadoutRecord->setGtFdlWord( *(*m_gtFDL).gtFdlWord(), iBxInEvent );

        }

        // reset
        m_gtPSB->reset();
        m_gtGTL->reset();
        m_gtFDL->reset();

        LogDebug("L1GlobalTrigger") << "\n Reset PSB, GTL, FDL\n" << std::endl;

    }


    std::ostringstream myCoutStream;

    if ( edm::isDebugEnabled() ) {
        gtReadoutRecord->printGtDecision(myCoutStream);

        LogDebug("L1GlobalTrigger")
        << myCoutStream.str()
        << std::endl;
        myCoutStream.str("");
        myCoutStream.clear();
    }


    // print result for every bx in event
    if ( edm::isDebugEnabled() ) {
        for (int iBxInEvent = minBxInEvent; iBxInEvent <= maxBxInEvent;
                ++iBxInEvent) {

            gtReadoutRecord->printGtDecision(myCoutStream, iBxInEvent);
            LogDebug("L1GlobalTrigger")
            << myCoutStream.str()
            << std::endl;
            myCoutStream.str("");
            myCoutStream.clear();

            gtReadoutRecord->printTechnicalTrigger(myCoutStream, iBxInEvent);
            LogDebug("L1GlobalTrigger")
            << myCoutStream.str()
            << std::endl;
            myCoutStream.str("");
            myCoutStream.clear();

        }
    }

    if ( m_gtSetup->gtConfig()->getInputMask()[1] ) {

        LogDebug("L1GlobalTrigger")
        << "\n**** Global Muon input disabled! \n  inputMask[1] = "
        << m_gtSetup->gtConfig()->getInputMask()[1]
        << "\n  No persistent reference for L1MuGMTReadoutCollection."
        << "\n**** \n"
        << std::endl;
    } else {

        // ** set muons in L1GlobalTriggerReadoutRecord

        LogDebug("L1GlobalTrigger")
        << "\n**** "
        << "\n  Persistent reference for L1MuGMTReadoutCollection with input tag: "
        << m_muGmtInputTag.label()
        << "\n**** \n"
        << std::endl;

        // get L1MuGMTReadoutCollection reference and set it in GT record

        edm::Handle<L1MuGMTReadoutCollection> gmtRcHandle;
        iEvent.getByLabel(m_muGmtInputTag.label(), gmtRcHandle);

        gtReadoutRecord->setMuCollectionRefProd(gmtRcHandle);

    }

    // test muon part in L1GlobalTriggerReadoutRecord

    if ( edm::isDebugEnabled() && !( m_gtSetup->gtConfig()->getInputMask()[1] ) ) {

        LogTrace("L1GlobalTrigger")
        << "\n Test muon collection in the GT readout record"
        << std::endl;

        // get reference to muon collection
        const edm::RefProd<L1MuGMTReadoutCollection>
        muCollRefProd = gtReadoutRecord->muCollectionRefProd();

        if (muCollRefProd.isNull()) {
            LogTrace("L1GlobalTrigger")
            << "Null reference for L1MuGMTReadoutCollection"
            << std::endl;

        } else {
            LogTrace("L1GlobalTrigger")
            << "RefProd address = " << &muCollRefProd
            << std::endl;

            // test all three variants to get muon index 0 in BXInEvent = 0
            unsigned int indexCand = 0;
            int bxInEvent = 0;

            // test first if the record has the required number of candidates
            if ((*muCollRefProd).getRecord(bxInEvent).getGMTCands().size() > indexCand) {

                LogTrace("L1GlobalTrigger")
                << "Three variants to get muon index 0 in BXInEvent = 0"
                << "\n via RefProd, muonCand(indexCand, bxInEvent), muonCand(indexCand)"
                << std::endl;

                L1MuGMTExtendedCand mu00 = (*muCollRefProd).getRecord(bxInEvent).getGMTCands()[indexCand];
                mu00.print();

                L1MuGMTExtendedCand mu00A = gtReadoutRecord->muonCand(indexCand, bxInEvent);
                mu00A.print();

                L1MuGMTExtendedCand mu00B = gtReadoutRecord->muonCand(indexCand);
                mu00B.print();

            }

            // test methods to get GMT records
            std::vector<L1MuGMTReadoutRecord> muRecords = (*muCollRefProd).getRecords();
            LogTrace("L1GlobalTrigger")
            << "\nNumber of records in the GMT RefProd readout collection = "
            << muRecords.size()
            << std::endl;

            for (std::vector<L1MuGMTReadoutRecord>::const_iterator itMu = muRecords.begin();
                    itMu < muRecords.end(); ++itMu) {

                std::vector<L1MuGMTExtendedCand>::const_iterator gmt_iter;
                std::vector<L1MuGMTExtendedCand> exc = itMu->getGMTCands();
                for(gmt_iter = exc.begin(); gmt_iter != exc.end(); gmt_iter++) {
                    (*gmt_iter).print();
                }

            }

            // test GMT record for BxInEvent = 0  (default argument)
            std::vector<L1MuGMTExtendedCand> muRecord0 = gtReadoutRecord->muonCands();
            LogTrace("L1GlobalTrigger")
            << "\nRecord for BxInEvent = 0 using default argument"
            << std::endl;

            for(std::vector<L1MuGMTExtendedCand>::const_iterator gmt_iter = muRecord0.begin();
                    gmt_iter != muRecord0.end(); gmt_iter++) {
                (*gmt_iter).print();
            }

            // test GMT record for BxInEvent = 1
            std::vector<L1MuGMTExtendedCand> muRecord1 = gtReadoutRecord->muonCands(1);
            LogTrace("L1GlobalTrigger")
            << "\nRecord for BxInEvent = 1 using BxInEvent argument"
            << std::endl;

            for(std::vector<L1MuGMTExtendedCand>::const_iterator gmt_iter = muRecord1.begin();
                    gmt_iter != muRecord1.end(); gmt_iter++) {
                (*gmt_iter).print();
            }
        }

    }

    if ( edm::isDebugEnabled() ) {

        const std::vector<L1GlobalTriggerObjectMap>&
        objMapVec = gtObjectMapRecord->gtObjectMap();

        for (std::vector<L1GlobalTriggerObjectMap>::const_iterator it = objMapVec.begin();
                it != objMapVec.end(); ++it) {
            (*it).print(myCoutStream);
        }

        //const CombinationsInCond*
        //comb = gtObjectMapRecord->getCombinationsInCond("Algo_ComplexSyntax", "Mu_60");

        //if (comb != 0) {
        //    myCoutStream << "\n  Number of combinations passing (Algo_ComplexSyntax, Mu_60): "
        //    << comb->size();
        //}


        //bool result = gtObjectMapRecord->getConditionResult("Algo_ComplexSyntax", "Mu_60");

        //myCoutStream << "\n  Result for condition Mu_60 in Algo_ComplexSyntax: "
        //<< result;

        LogDebug("L1GlobalTrigger")
        << "Test gtObjectMapRecord in L1GlobalTrigger \n\n" << myCoutStream.str() << "\n\n"
        << std::endl;
        myCoutStream.str("");
        myCoutStream.clear();

    }

    // **
    iEvent.put( gtReadoutRecord );
    iEvent.put( gtEvmReadoutRecord );
    iEvent.put( gtObjectMapRecord );

}

// static data members

L1GlobalTriggerSetup* L1GlobalTrigger::m_gtSetup = 0;

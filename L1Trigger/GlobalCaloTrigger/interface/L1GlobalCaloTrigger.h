#ifndef L1GLOBALCALOTRIGGER_H_
#define L1GLOBALCALOTRIGGER_H_

#include "L1Trigger/GlobalCaloTrigger/interface/L1GctEmCand.h"
#include "L1Trigger/GlobalCaloTrigger/interface/L1GctJetCand.h"

#include <vector>



/**
  * Represents the GCT system
  * This is the main point of access for the user
  * 
  * author: Jim Brooke
  * date: 20/2/2006
  * 
  **/ 

class L1GctSourceCard;
class L1GctJetLeafCard;
class L1GctEmLeafCard;

class L1GctWheelJetFpga;
class L1GctWheelEnergyFpga;
class L1GctJetFinalStage;
class L1GctGlobalEnergyAlgos;
class L1GctElectronFinalSort;


class L1GlobalCaloTrigger {
public:
	/// construct the GCT
	L1GlobalCaloTrigger();
	///
	/// dismantle the GCT
	~L1GlobalCaloTrigger();
	///
	/// load files into Source Cards
	void openSourceCardFiles(std::string fileBase);
	///
	/// process an event
	void process();
	///
	/// iso electron outputs to GT
	std::vector<L1GctEmCand> getIsoElectrons();
	/// 
	/// non-iso electron outputs to GT
	std::vector<L1GctEmCand> getNonIsoElectrons();
	///
	/// central jet outputs to GT
	std::vector<L1GctJetCand> getCentralJets();
	///
	/// forward jet outputs to GT
	std::vector<L1GctJetCand> getForwardJets();
	///
	/// tau jet outputs to GT
	std::vector<L1GctJetCand> getTauJets();
	///
	/// Etmiss output to GT
	unsigned getEtMiss();
	///
	/// Etmiss phi output to GT
	unsigned getEtMissPhi();
	///
	/// Total Et output to GT
	unsigned getEtSum();
	///
	/// Total hadronic Et output to GT
	unsigned getEtHad();
	///
	/// get the Source cards
	std::vector<L1GctSourceCard*> getSourceCards() { return theSourceCards; }
	///
	/// get the Jet Leaf cards
	std::vector<L1GctJetLeafCard*> getJetLeafCards() { return theJetLeafCards; }
	///
	/// get the Jet Leaf cards
	std::vector<L1GctEmLeafCard*> getEmLeafCards() { return theEmLeafCards; }
	///
	/// print setup info
	void print();
	
private:
	///
	/// instantiate the hardware & algo objects
	void build();
	///
	/// wire up the hardware obejcts
	void setup();

private:
	///
	/// pointers to the Source Cards
	std::vector<L1GctSourceCard*> theSourceCards;
	///
	/// pointers to the Jet Leaf cards
	std::vector<L1GctJetLeafCard*> theJetLeafCards;
	///
	/// pointers to the EM Leaf cards
	std::vector<L1GctEmLeafCard*> theEmLeafCards;
	///
	/// Wheel Card Jet Fpgas	
	std::vector<L1GctWheelJetFpga*> theWheelJetFpgas;		
	///
	/// Wheel Card Energy Fpgas
	std::vector<L1GctWheelEnergyFpga*> theWheelEnergyFpgas;
	///
	/// central barrel jet find & final sort
	L1GctJetFinalStage* theJetFinalStage;			
	///
	/// energy final stage algos
	L1GctGlobalEnergyAlgos* theEnergyFinalStage;	
	///
	/// electron final stage sorters
	L1GctElectronFinalSort* theIsoEmFinalStage;
	L1GctElectronFinalSort* theNonIsoEmFinalStage;
	
};

#endif /*L1GLOBALCALOTRIGGER_H_*/

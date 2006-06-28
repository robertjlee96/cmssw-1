#include "L1Trigger/GlobalCaloTrigger/interface/L1GctElectronSorter.h"
#include "L1Trigger/GlobalCaloTrigger/interface/L1GctSourceCard.h"

#include "FWCore/Utilities/interface/Exception.h"  

#include <iostream>

using namespace std;


L1GctElectronSorter::L1GctElectronSorter(int nInputs, bool iso, std::vector<L1GctSourceCard*> sourceCards):
  m_id(nInputs),
  m_emCandType(iso),
  m_theSCs(nInputs),
  m_inputCands(nInputs*4),
  m_outputCands(4)
{
  if(m_theSCs.size()!=sourceCards.size()){
    throw cms::Exception("L1GctSetupError")
      <<"L1GctElectronSorter::L1GctELectronSorter() : The number of Source Cards passed in the constructor doesn't correspond to the no of inputs given in the nInput variable";
  }
  for(unsigned i=0;i!=sourceCards.size();i++){
    if(sourceCards[i]!=0){
      m_theSCs[i] = sourceCards[i];
    }else{
      throw cms::Exception("L1GctSetupError")
	<<"L1GctElectronSorter::L1GctElectronSorter() : Pointer to Source Card #"<<i<<" is zero";
    }  
  }
}

L1GctElectronSorter::L1GctElectronSorter(int nInputs, bool iso):
  m_id(nInputs),
  m_emCandType(iso),
  m_theSCs(0),
  m_inputCands(nInputs),
  m_outputCands(4)
{}  

L1GctElectronSorter::~L1GctElectronSorter()
{
}

// clear buffers
void L1GctElectronSorter::reset() {
  m_inputCands.clear();
  m_inputCands.resize(m_id*4);
  m_outputCands.clear();
  m_outputCands.resize(4);
}

// get the input data
void L1GctElectronSorter::fetchInput() {
  // loop over Source Cards - using integers not the vector size because the vector might be the wrong size
  for (unsigned int i=0; i<m_theSCs.size(); i++) {
    // loop over 4 candidates per Source Card
    for (unsigned int j=0; j<4; j++) {
      // get EM candidates, depending on type
      if (m_emCandType) {
	setInputEmCand((j+i*4),m_theSCs[i]->getIsoElectrons()[j]);
      }else {
	setInputEmCand((j+i*4),m_theSCs[i]->getNonIsoElectrons()[j]);
      }
    }
  }
}

//Process sorts the electron candidates after rank and stores the highest four (in the outputCands vector)
void L1GctElectronSorter::process() {


//Convert from caloEmCand to gctEmCand and make temporary copy of data
  std::vector<L1GctEmCand> data = this->convert(m_inputCands);
 
//Then sort it
    sort(data.begin(),data.end(),rank_gt());

//Copy data to output buffer
    for(int i = 0; i<4; i++){
      m_outputCands[i] = data[i];
    }

}

void L1GctElectronSorter::setInputEmCand(int i, L1CaloEmCand cand){
  m_inputCands[i] = cand;
}

std::ostream& operator<<(std::ostream& s, const L1GctElectronSorter& ems) {
  s << "===L1GctElectronSorter===" << std::endl;
  s << "Algo type = " << ems.m_emCandType << std::endl;
  s << "No of Source Cards = " << ems.m_theSCs.size() << std::endl;
  for (unsigned i=0; i<ems.m_theSCs.size(); i++) {
    s << "SourceCard* " << i << " = " << ems.m_theSCs[i]<<std::endl;
  }
  s << "No of Electron Input Candidates = " << ems.m_inputCands.size()<< std::endl;
  s << "No of Electron Output Candidates = " << ems.m_outputCands.size()<< std::endl;
  return s;
}

std::vector<L1GctEmCand> L1GctElectronSorter::convert(std::vector<L1CaloEmCand> cand){
  std::vector<L1GctEmCand> gctCand(cand.size());
  for(unsigned int i = 0;i!=cand.size();i++){
    unsigned int rank = cand[i].rank();
    int phi = 0;
    int eta = 0;
    bool iso = cand[i].isolated();
    L1GctEmCand gctTemp(rank,phi,eta,iso);
    gctCand[i] = gctTemp;
  }
  return gctCand;
}


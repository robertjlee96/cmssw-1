#ifndef HCALDBPRODUCER_HCALDBRECORD_H
#define HCALDBPRODUCER_HCALDBRECORD_H
// -*- C++ -*-
//
// Package:     HcalDbProducer
// Class  :     HcalDbRecord
// 
/**\class HcalDbRecord HcalDbRecord.h CalibFormats/HcalDbProducer/interface/HcalDbRecord.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      
// Created:     Tue Aug  9 19:10:36 CDT 2005
// $Id: HcalDbRecord.h,v 1.9 2009/05/19 16:06:06 rofierzy Exp $
//
#include "boost/mpl/vector.hpp"
#include "FWCore/Framework/interface/DependentRecordImplementation.h"
// #include "FWCore/Framework/interface/EventSetupRecordImplementation.h"

#include "CondFormats/DataRecord/interface/HcalAllRcds.h"

// class HcalDbRecord : public edm::eventsetup::EventSetupRecordImplementation<HcalDbRecord> {};

class HcalDbRecord : public edm::eventsetup::DependentRecordImplementation <HcalDbRecord,  
  boost::mpl::vector<HcalPedestalsRcd, HcalPedestalWidthsRcd, HcalGainsRcd, HcalGainWidthsRcd, 
  HcalQIEDataRcd, HcalChannelQualityRcd, HcalZSThresholdsRcd, HcalRespCorrsRcd, 
  HcalL1TriggerObjectsRcd, HcalElectronicsMapRcd, HcalTimeCorrsRcd, HcalLUTCorrsRcd, HcalPFCorrsRcd > > {}; 

#endif /* HCALDBPRODUCER_HCALDBRECORD_H */


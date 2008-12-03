/*
 * \file EELedTask.cc
 *
 * $Date: 2008/12/03 14:44:53 $
 * $Revision: 1.45 $
 * \author G. Della Ricca
 *
*/

#include <iostream>
#include <fstream>
#include <vector>

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DQMServices/Core/interface/MonitorElement.h"

#include "DQMServices/Core/interface/DQMStore.h"

#include "DataFormats/EcalRawData/interface/EcalRawDataCollections.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"
#include "DataFormats/EcalDigi/interface/EEDataFrame.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalUncalibratedRecHit.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

#include <DQM/EcalCommon/interface/Numbers.h>

#include <DQM/EcalEndcapMonitorTasks/interface/EELedTask.h>

using namespace cms;
using namespace edm;
using namespace std;

EELedTask::EELedTask(const ParameterSet& ps){

  init_ = false;

  dqmStore_ = Service<DQMStore>().operator->();

  prefixME_ = ps.getUntrackedParameter<string>("prefixME", "");

  enableCleanup_ = ps.getUntrackedParameter<bool>("enableCleanup", false);

  mergeRuns_ = ps.getUntrackedParameter<bool>("mergeRuns", false);

  EcalRawDataCollection_ = ps.getParameter<edm::InputTag>("EcalRawDataCollection");
  EEDigiCollection_ = ps.getParameter<edm::InputTag>("EEDigiCollection");
  EcalPnDiodeDigiCollection_ = ps.getParameter<edm::InputTag>("EcalPnDiodeDigiCollection");
  EcalUncalibratedRecHitCollection_ = ps.getParameter<edm::InputTag>("EcalUncalibratedRecHitCollection");

  for (int i = 0; i < 18; i++) {
    meShapeMapL1A_[i] = 0;
    meAmplMapL1A_[i] = 0;
    meTimeMapL1A_[i] = 0;
    meAmplPNMapL1A_[i] = 0;
    meShapeMapL1B_[i] = 0;
    meAmplMapL1B_[i] = 0;
    meTimeMapL1B_[i] = 0;
    meAmplPNMapL1B_[i] = 0;
    mePnAmplMapG01L1_[i] = 0;
    mePnPedMapG01L1_[i] = 0;
    mePnAmplMapG16L1_[i] = 0;
    mePnPedMapG16L1_[i] = 0;

    meShapeMapL2A_[i] = 0;
    meAmplMapL2A_[i] = 0;
    meTimeMapL2A_[i] = 0;
    meAmplPNMapL2A_[i] = 0;
    meShapeMapL2B_[i] = 0;
    meAmplMapL2B_[i] = 0;
    meTimeMapL2B_[i] = 0;
    meAmplPNMapL2B_[i] = 0;
    mePnAmplMapG01L2_[i] = 0;
    mePnPedMapG01L2_[i] = 0;
    mePnAmplMapG16L2_[i] = 0;
    mePnPedMapG16L2_[i] = 0;
  }

}

EELedTask::~EELedTask(){

}

void EELedTask::beginJob(const EventSetup& c){

  ievt_ = 0;

  if ( dqmStore_ ) {
    dqmStore_->setCurrentFolder(prefixME_ + "/EELedTask");
    dqmStore_->rmdir(prefixME_ + "/EELedTask");
  }

  Numbers::initGeometry(c, false);

}

void EELedTask::beginRun(const Run& r, const EventSetup& c) {

  if ( ! mergeRuns_ ) this->reset();

}

void EELedTask::endRun(const Run& r, const EventSetup& c) {

  for (int i = 0; i < 18; i++) {
    if ( meShapeMapL1A_[i] )  meShapeMapL1A_[i]->Reset();
    if ( meAmplMapL1A_[i] ) meAmplMapL1A_[i]->Reset();
    if ( meTimeMapL1A_[i] ) meTimeMapL1A_[i]->Reset();
    if ( meAmplPNMapL1A_[i] ) meAmplPNMapL1A_[i]->Reset();

    if ( meShapeMapL1B_[i] )  meShapeMapL1B_[i]->Reset();
    if ( meAmplMapL1B_[i] ) meAmplMapL1B_[i]->Reset();
    if ( meTimeMapL1B_[i] ) meTimeMapL1B_[i]->Reset();
    if ( meAmplPNMapL1B_[i] ) meAmplPNMapL1B_[i]->Reset();

    if ( meShapeMapL2A_[i] )  meShapeMapL2A_[i]->Reset();
    if ( meAmplMapL2A_[i] ) meAmplMapL2A_[i]->Reset();
    if ( meTimeMapL2A_[i] ) meTimeMapL2A_[i]->Reset();
    if ( meAmplPNMapL2A_[i] ) meAmplPNMapL2A_[i]->Reset();

    if ( meShapeMapL2B_[i] )  meShapeMapL2B_[i]->Reset();
    if ( meAmplMapL2B_[i] ) meAmplMapL2B_[i]->Reset();
    if ( meTimeMapL2B_[i] ) meTimeMapL2B_[i]->Reset();
    if ( meAmplPNMapL2B_[i] ) meAmplPNMapL2B_[i]->Reset();

    if ( mePnAmplMapG01L1_[i] ) mePnAmplMapG01L1_[i]->Reset();
    if ( mePnPedMapG01L1_[i] ) mePnPedMapG01L1_[i]->Reset();

    if ( mePnAmplMapG16L1_[i] ) mePnAmplMapG16L1_[i]->Reset();
    if ( mePnPedMapG16L1_[i] ) mePnPedMapG16L1_[i]->Reset();

    if ( mePnAmplMapG01L2_[i] ) mePnAmplMapG01L2_[i]->Reset();
    if ( mePnPedMapG01L2_[i] ) mePnPedMapG01L2_[i]->Reset();

    if ( mePnAmplMapG16L2_[i] ) mePnAmplMapG16L2_[i]->Reset();
    if ( mePnPedMapG16L2_[i] ) mePnPedMapG16L2_[i]->Reset();
  }

}

void EELedTask::reset(void) {

}

void EELedTask::setup(void){

  init_ = true;

  char histo[200];

  if ( dqmStore_ ) {
    dqmStore_->setCurrentFolder(prefixME_ + "/EELedTask");

    dqmStore_->setCurrentFolder(prefixME_ + "/EELedTask/Led1");
    for (int i = 0; i < 18; i++) {
      sprintf(histo, "EELDT shape %s L1A", Numbers::sEE(i+1).c_str());
      meShapeMapL1A_[i] = dqmStore_->bookProfile2D(histo, histo, 850, 0., 850., 10, 0., 10., 4096, 0., 4096., "s");
      meShapeMapL1A_[i]->setAxisTitle("channel", 1);
      meShapeMapL1A_[i]->setAxisTitle("sample", 2);
      meShapeMapL1A_[i]->setAxisTitle("amplitude", 3);
      dqmStore_->tag(meShapeMapL1A_[i], i+1);
      sprintf(histo, "EELDT amplitude %s L1A", Numbers::sEE(i+1).c_str());
      meAmplMapL1A_[i] = dqmStore_->bookProfile2D(histo, histo, 50, Numbers::ix0EE(i+1)+0., Numbers::ix0EE(i+1)+50., 50, Numbers::iy0EE(i+1)+0., Numbers::iy0EE(i+1)+50., 4096, 0., 4096.*12., "s");
      meAmplMapL1A_[i]->setAxisTitle("jx", 1);
      meAmplMapL1A_[i]->setAxisTitle("jy", 2);
      dqmStore_->tag(meAmplMapL1A_[i], i+1);
      sprintf(histo, "EELDT timing %s L1A", Numbers::sEE(i+1).c_str());
      meTimeMapL1A_[i] = dqmStore_->bookProfile2D(histo, histo, 50, Numbers::ix0EE(i+1)+0., Numbers::ix0EE(i+1)+50., 50, Numbers::iy0EE(i+1)+0., Numbers::iy0EE(i+1)+50., 250, 0., 10., "s");
      meTimeMapL1A_[i]->setAxisTitle("jx", 1);
      meTimeMapL1A_[i]->setAxisTitle("jy", 2);
      dqmStore_->tag(meTimeMapL1A_[i], i+1);
      sprintf(histo, "EELDT amplitude over PN %s L1A", Numbers::sEE(i+1).c_str());
      meAmplPNMapL1A_[i] = dqmStore_->bookProfile2D(histo, histo, 50, Numbers::ix0EE(i+1)+0., Numbers::ix0EE(i+1)+50., 50, Numbers::iy0EE(i+1)+0., Numbers::iy0EE(i+1)+50., 4096, 0., 4096.*12., "s");
      meAmplPNMapL1A_[i]->setAxisTitle("jx", 1);
      meAmplPNMapL1A_[i]->setAxisTitle("jy", 2);
      dqmStore_->tag(meAmplPNMapL1A_[i], i+1);

      sprintf(histo, "EELDT shape %s L1B", Numbers::sEE(i+1).c_str());
      meShapeMapL1B_[i] = dqmStore_->bookProfile2D(histo, histo, 850, 0., 850., 10, 0., 10., 4096, 0., 4096., "s");
      meShapeMapL1B_[i]->setAxisTitle("channel", 1);
      meShapeMapL1B_[i]->setAxisTitle("sample", 2);
      meShapeMapL1B_[i]->setAxisTitle("amplitude", 3);
      dqmStore_->tag(meShapeMapL1B_[i], i+1);
      sprintf(histo, "EELDT amplitude %s L1B", Numbers::sEE(i+1).c_str());
      meAmplMapL1B_[i] = dqmStore_->bookProfile2D(histo, histo, 50, Numbers::ix0EE(i+1)+0., Numbers::ix0EE(i+1)+50., 50, Numbers::iy0EE(i+1)+0., Numbers::iy0EE(i+1)+50., 4096, 0., 4096.*12., "s");
      meAmplMapL1B_[i]->setAxisTitle("jx", 1);
      meAmplMapL1B_[i]->setAxisTitle("jy", 2);
      dqmStore_->tag(meAmplMapL1B_[i], i+1);
      sprintf(histo, "EELDT timing %s L1B", Numbers::sEE(i+1).c_str());
      meTimeMapL1B_[i] = dqmStore_->bookProfile2D(histo, histo, 50, Numbers::ix0EE(i+1)+0., Numbers::ix0EE(i+1)+50., 50, Numbers::iy0EE(i+1)+0., Numbers::iy0EE(i+1)+50., 250, 0., 10., "s");
      meTimeMapL1B_[i]->setAxisTitle("jx", 1);
      meTimeMapL1B_[i]->setAxisTitle("jy", 2);
      dqmStore_->tag(meTimeMapL1B_[i], i+1);
      sprintf(histo, "EELDT amplitude over PN %s L1B", Numbers::sEE(i+1).c_str());
      meAmplPNMapL1B_[i] = dqmStore_->bookProfile2D(histo, histo, 50, Numbers::ix0EE(i+1)+0., Numbers::ix0EE(i+1)+50., 50, Numbers::iy0EE(i+1)+0., Numbers::iy0EE(i+1)+50., 4096, 0., 4096.*12., "s");
      meAmplPNMapL1B_[i]->setAxisTitle("jx", 1);
      meAmplPNMapL1B_[i]->setAxisTitle("jy", 2);
      dqmStore_->tag(meAmplPNMapL1B_[i], i+1);
    }

    dqmStore_->setCurrentFolder(prefixME_ + "/EELedTask/Led2");
    for (int i = 0; i < 18; i++) {
      sprintf(histo, "EELDT shape %s L2A", Numbers::sEE(i+1).c_str());
      meShapeMapL2A_[i] = dqmStore_->bookProfile2D(histo, histo, 850, 0., 850., 10, 0., 10., 4096, 0., 4096., "s");
      meShapeMapL2A_[i]->setAxisTitle("channel", 1);
      meShapeMapL2A_[i]->setAxisTitle("sample", 2);
      meShapeMapL2A_[i]->setAxisTitle("amplitude", 3);
      dqmStore_->tag(meShapeMapL2A_[i], i+1);
      sprintf(histo, "EELDT amplitude %s L2A", Numbers::sEE(i+1).c_str());
      meAmplMapL2A_[i] = dqmStore_->bookProfile2D(histo, histo, 50, Numbers::ix0EE(i+1)+0., Numbers::ix0EE(i+1)+50., 50, Numbers::iy0EE(i+1)+0., Numbers::iy0EE(i+1)+50., 4096, 0., 4096.*12., "s");
      meAmplMapL2A_[i]->setAxisTitle("jx", 1);
      meAmplMapL2A_[i]->setAxisTitle("jy", 2);
      dqmStore_->tag(meAmplMapL2A_[i], i+1);
      sprintf(histo, "EELDT timing %s L2A", Numbers::sEE(i+1).c_str());
      meTimeMapL2A_[i] = dqmStore_->bookProfile2D(histo, histo, 50, Numbers::ix0EE(i+1)+0., Numbers::ix0EE(i+1)+50., 50, Numbers::iy0EE(i+1)+0., Numbers::iy0EE(i+1)+50., 250, 0., 10., "s");
      meTimeMapL2A_[i]->setAxisTitle("jx", 1);
      meTimeMapL2A_[i]->setAxisTitle("jy", 2);
      dqmStore_->tag(meTimeMapL2A_[i], i+1);
      sprintf(histo, "EELDT amplitude over PN %s L2A", Numbers::sEE(i+1).c_str());
      meAmplPNMapL2A_[i] = dqmStore_->bookProfile2D(histo, histo, 50, Numbers::ix0EE(i+1)+0., Numbers::ix0EE(i+1)+50., 50, Numbers::iy0EE(i+1)+0., Numbers::iy0EE(i+1)+50., 4096, 0., 4096.*12., "s");
      meAmplPNMapL2A_[i]->setAxisTitle("jx", 1);
      meAmplPNMapL2A_[i]->setAxisTitle("jy", 2);
      dqmStore_->tag(meAmplPNMapL2A_[i], i+1);

      sprintf(histo, "EELDT shape %s L2B", Numbers::sEE(i+1).c_str());
      meShapeMapL2B_[i] = dqmStore_->bookProfile2D(histo, histo, 850, 0., 850., 10, 0., 10., 4096, 0., 4096., "s");
      meShapeMapL2B_[i]->setAxisTitle("channel", 1);
      meShapeMapL2B_[i]->setAxisTitle("sample", 2);
      meShapeMapL2B_[i]->setAxisTitle("amplitude", 3);
      dqmStore_->tag(meShapeMapL2B_[i], i+1);
      sprintf(histo, "EELDT amplitude %s L2B", Numbers::sEE(i+1).c_str());
      meAmplMapL2B_[i] = dqmStore_->bookProfile2D(histo, histo, 50, Numbers::ix0EE(i+1)+0., Numbers::ix0EE(i+1)+50., 50, Numbers::iy0EE(i+1)+0., Numbers::iy0EE(i+1)+50., 4096, 0., 4096.*12., "s");
      meAmplMapL2B_[i]->setAxisTitle("jx", 1);
      meAmplMapL2B_[i]->setAxisTitle("jy", 2);
      dqmStore_->tag(meAmplMapL2B_[i], i+1);
      sprintf(histo, "EELDT timing %s L2B", Numbers::sEE(i+1).c_str());
      meTimeMapL2B_[i] = dqmStore_->bookProfile2D(histo, histo, 50, Numbers::ix0EE(i+1)+0., Numbers::ix0EE(i+1)+50., 50, Numbers::iy0EE(i+1)+0., Numbers::iy0EE(i+1)+50., 250, 0., 10., "s");
      meTimeMapL2B_[i]->setAxisTitle("jx", 1);
      meTimeMapL2B_[i]->setAxisTitle("jy", 2);
      dqmStore_->tag(meTimeMapL2B_[i], i+1);
      sprintf(histo, "EELDT amplitude over PN %s L2B", Numbers::sEE(i+1).c_str());
      meAmplPNMapL2B_[i] = dqmStore_->bookProfile2D(histo, histo, 50, Numbers::ix0EE(i+1)+0., Numbers::ix0EE(i+1)+50., 50, Numbers::iy0EE(i+1)+0., Numbers::iy0EE(i+1)+50., 4096, 0., 4096.*12., "s");
      meAmplPNMapL2B_[i]->setAxisTitle("jx", 1);
      meAmplPNMapL2B_[i]->setAxisTitle("jy", 2);
      dqmStore_->tag(meAmplPNMapL2B_[i], i+1);
    }

    dqmStore_->setCurrentFolder(prefixME_ + "/EELedTask/Led1/PN");

    dqmStore_->setCurrentFolder(prefixME_ + "/EELedTask/Led1/PN/Gain01");
    for (int i = 0; i < 18; i++) {
      sprintf(histo, "EEPDT PNs amplitude %s G01 L1", Numbers::sEE(i+1).c_str());
      mePnAmplMapG01L1_[i] = dqmStore_->bookProfile(histo, histo, 10, 0., 10., 4096, 0., 4096., "s");
      mePnAmplMapG01L1_[i]->setAxisTitle("channel", 1);
      mePnAmplMapG01L1_[i]->setAxisTitle("amplitude", 2);
      dqmStore_->tag(mePnAmplMapG01L1_[i], i+1);
      sprintf(histo, "EEPDT PNs pedestal %s G01 L1", Numbers::sEE(i+1).c_str());
      mePnPedMapG01L1_[i] = dqmStore_->bookProfile(histo, histo, 10, 0., 10., 4096, 0., 4096., "s");
      mePnPedMapG01L1_[i]->setAxisTitle("channel", 1);
      mePnPedMapG01L1_[i]->setAxisTitle("pedestal", 2);
      dqmStore_->tag(mePnPedMapG01L1_[i], i+1);
    }

    dqmStore_->setCurrentFolder(prefixME_ + "/EELedTask/Led1/PN/Gain16");
    for (int i = 0; i < 18; i++) {
      sprintf(histo, "EEPDT PNs amplitude %s G16 L1", Numbers::sEE(i+1).c_str());
      mePnAmplMapG16L1_[i] = dqmStore_->bookProfile(histo, histo, 10, 0., 10., 4096, 0., 4096., "s");
      mePnAmplMapG16L1_[i]->setAxisTitle("channel", 1);
      mePnAmplMapG16L1_[i]->setAxisTitle("amplitude", 2);
      dqmStore_->tag(mePnAmplMapG16L1_[i], i+1);
      sprintf(histo, "EEPDT PNs pedestal %s G16 L1", Numbers::sEE(i+1).c_str());
      mePnPedMapG16L1_[i] = dqmStore_->bookProfile(histo, histo, 10, 0., 10., 4096, 0., 4096., "s");
      mePnPedMapG16L1_[i]->setAxisTitle("channel", 1);
      mePnPedMapG16L1_[i]->setAxisTitle("pedestal", 2); 
      dqmStore_->tag(mePnPedMapG16L1_[i], i+1);
    }

    dqmStore_->setCurrentFolder(prefixME_ + "/EELedTask/Led2/PN");

    dqmStore_->setCurrentFolder(prefixME_ + "/EELedTask/Led2/PN/Gain01");
    for (int i = 0; i < 18; i++) {
      sprintf(histo, "EEPDT PNs amplitude %s G01 L2", Numbers::sEE(i+1).c_str());
      mePnAmplMapG01L2_[i] = dqmStore_->bookProfile(histo, histo, 10, 0., 10., 4096, 0., 4096., "s");
      mePnAmplMapG01L2_[i]->setAxisTitle("amplitude", 2);
      mePnAmplMapG01L2_[i]->setAxisTitle("channel", 1);
      dqmStore_->tag(mePnAmplMapG01L2_[i], i+1);
      sprintf(histo, "EEPDT PNs pedestal %s G01 L2", Numbers::sEE(i+1).c_str());
      mePnPedMapG01L2_[i] = dqmStore_->bookProfile(histo, histo, 10, 0., 10., 4096, 0., 4096., "s");
      mePnPedMapG01L2_[i]->setAxisTitle("channel", 1);
      mePnPedMapG01L2_[i]->setAxisTitle("pedestal", 2);
      dqmStore_->tag(mePnPedMapG01L2_[i], i+1);
    }

    dqmStore_->setCurrentFolder(prefixME_ + "/EELedTask/Led2/PN/Gain16");
    for (int i = 0; i < 18; i++) {
      sprintf(histo, "EEPDT PNs amplitude %s G16 L2", Numbers::sEE(i+1).c_str());
      mePnAmplMapG16L2_[i] = dqmStore_->bookProfile(histo, histo, 10, 0., 10., 4096, 0., 4096., "s");
      mePnAmplMapG16L2_[i]->setAxisTitle("channel", 1);
      mePnAmplMapG16L2_[i]->setAxisTitle("amplitude", 2);
      dqmStore_->tag(mePnAmplMapG16L2_[i], i+1);
      sprintf(histo, "EEPDT PNs pedestal %s G16 L2", Numbers::sEE(i+1).c_str());
      mePnPedMapG16L2_[i] = dqmStore_->bookProfile(histo, histo, 10, 0., 10., 4096, 0., 4096., "s");
      mePnPedMapG16L2_[i]->setAxisTitle("channel", 1);
      mePnPedMapG16L2_[i]->setAxisTitle("pedestal", 2); 
      dqmStore_->tag(mePnPedMapG16L2_[i], i+1);
    }

  }

}

void EELedTask::cleanup(void){

  if ( ! init_ ) return;

  if ( dqmStore_ ) {
    dqmStore_->setCurrentFolder(prefixME_ + "/EELedTask");

    dqmStore_->setCurrentFolder(prefixME_ + "/EELedTask/Led1");
    for (int i = 0; i < 18; i++) {
      if ( meShapeMapL1A_[i] )  dqmStore_->removeElement( meShapeMapL1A_[i]->getName() );
      meShapeMapL1A_[i] = 0;
      if ( meAmplMapL1A_[i] ) dqmStore_->removeElement( meAmplMapL1A_[i]->getName() );
      meAmplMapL1A_[i] = 0;
      if ( meTimeMapL1A_[i] ) dqmStore_->removeElement( meTimeMapL1A_[i]->getName() );
      meTimeMapL1A_[i] = 0;
      if ( meAmplPNMapL1A_[i] ) dqmStore_->removeElement( meAmplPNMapL1A_[i]->getName() );
      meAmplPNMapL1A_[i] = 0;

      if ( meShapeMapL1B_[i] )  dqmStore_->removeElement( meShapeMapL1B_[i]->getName() );
      meShapeMapL1B_[i] = 0;
      if ( meAmplMapL1B_[i] ) dqmStore_->removeElement( meAmplMapL1B_[i]->getName() );
      meAmplMapL1B_[i] = 0;
      if ( meTimeMapL1B_[i] ) dqmStore_->removeElement( meTimeMapL1B_[i]->getName() );
      meTimeMapL1B_[i] = 0;
      if ( meAmplPNMapL1B_[i] ) dqmStore_->removeElement( meAmplPNMapL1B_[i]->getName() );
      meAmplPNMapL1B_[i] = 0;
    }

    dqmStore_->setCurrentFolder(prefixME_ + "/EELedTask/Led2");
    for (int i = 0; i < 18; i++) {
      if ( meShapeMapL2A_[i] )  dqmStore_->removeElement( meShapeMapL2A_[i]->getName() );
      meShapeMapL2A_[i] = 0;
      if ( meAmplMapL2A_[i] ) dqmStore_->removeElement( meAmplMapL2A_[i]->getName() );
      meAmplMapL2A_[i] = 0;
      if ( meTimeMapL2A_[i] ) dqmStore_->removeElement( meTimeMapL2A_[i]->getName() );
      meTimeMapL2A_[i] = 0;
      if ( meAmplPNMapL2A_[i] ) dqmStore_->removeElement( meAmplPNMapL2A_[i]->getName() );
      meAmplPNMapL2A_[i] = 0;

      if ( meShapeMapL2B_[i] )  dqmStore_->removeElement( meShapeMapL2B_[i]->getName() );
      meShapeMapL2B_[i] = 0;
      if ( meAmplMapL2B_[i] ) dqmStore_->removeElement( meAmplMapL2B_[i]->getName() );
      meAmplMapL2B_[i] = 0;
      if ( meTimeMapL2B_[i] ) dqmStore_->removeElement( meTimeMapL2B_[i]->getName() );
      meTimeMapL2B_[i] = 0;
      if ( meAmplPNMapL2B_[i] ) dqmStore_->removeElement( meAmplPNMapL2B_[i]->getName() );
      meAmplPNMapL2B_[i] = 0;
    }

    dqmStore_->setCurrentFolder(prefixME_ + "/EELedTask/Led1/PN");

    dqmStore_->setCurrentFolder(prefixME_ + "/EELedTask/Led1/PN/Gain01");
    for (int i = 0; i < 18; i++) {
      if ( mePnAmplMapG01L1_[i] ) dqmStore_->removeElement( mePnAmplMapG01L1_[i]->getName() );
      mePnAmplMapG01L1_[i] = 0;
      if ( mePnPedMapG01L1_[i] ) dqmStore_->removeElement( mePnPedMapG01L1_[i]->getName() );
      mePnPedMapG01L1_[i] = 0;
    }

    dqmStore_->setCurrentFolder(prefixME_ + "/EELedTask/Led1/PN/Gain16");
    for (int i = 0; i < 18; i++) {
      if ( mePnAmplMapG16L1_[i] ) dqmStore_->removeElement( mePnAmplMapG16L1_[i]->getName() );
      mePnAmplMapG16L1_[i] = 0;
      if ( mePnPedMapG16L1_[i] ) dqmStore_->removeElement( mePnPedMapG16L1_[i]->getName() );
      mePnPedMapG16L1_[i] = 0;
    }

    dqmStore_->setCurrentFolder(prefixME_ + "/EELedTask/Led2/PN");

    dqmStore_->setCurrentFolder(prefixME_ + "/EELedTask/Led2/PN/Gain01");
    for (int i = 0; i < 18; i++) {
      if ( mePnAmplMapG01L2_[i] ) dqmStore_->removeElement( mePnAmplMapG01L2_[i]->getName() );
      mePnAmplMapG01L2_[i] = 0;
      if ( mePnPedMapG01L2_[i] ) dqmStore_->removeElement( mePnPedMapG01L2_[i]->getName() );
      mePnPedMapG01L2_[i] = 0;
    }

    dqmStore_->setCurrentFolder(prefixME_ + "/EELedTask/Led2/PN/Gain16");
    for (int i = 0; i < 18; i++) {
      if ( mePnAmplMapG16L2_[i] ) dqmStore_->removeElement( mePnAmplMapG16L2_[i]->getName() );
      mePnAmplMapG16L2_[i] = 0;
      if ( mePnPedMapG16L2_[i] ) dqmStore_->removeElement( mePnPedMapG16L2_[i]->getName() );
      mePnPedMapG16L2_[i] = 0;
    }

  }

  init_ = false;

}

void EELedTask::endJob(void){

  LogInfo("EELedTask") << "analyzed " << ievt_ << " events";

  if ( enableCleanup_ ) this->cleanup();

}

void EELedTask::analyze(const Event& e, const EventSetup& c){

  bool enable = false;
  int runType[18] = { -1 };
  int rtHalf[18] = { -1 };
  int waveLength[18] = { -1 };

  Handle<EcalRawDataCollection> dcchs;

  if ( e.getByLabel(EcalRawDataCollection_, dcchs) ) {

    for ( EcalRawDataCollection::const_iterator dcchItr = dcchs->begin(); dcchItr != dcchs->end(); ++dcchItr ) {

      if ( Numbers::subDet( *dcchItr ) != EcalEndcap ) continue;

      int ism = Numbers::iSM( *dcchItr, EcalEndcap );

      runType[ism] = runType[ism];
      rtHalf[ism] = dcchItr->getRtHalf();
      waveLength[ism] = dcchItr->getEventSettings().wavelength;

      if ( dcchItr->getRunType() == EcalDCCHeaderBlock::LED_STD ||
           dcchItr->getRunType() == EcalDCCHeaderBlock::LED_GAP ) enable = true;

    }

  } else {

    LogWarning("EELedTask") << EcalRawDataCollection_ << " not available";

  }

  if ( ! enable ) return;

  if ( ! init_ ) this->setup();

  ievt_++;

  Handle<EEDigiCollection> digis;

  if ( e.getByLabel(EEDigiCollection_, digis) ) {

    int need = digis->size();
    LogDebug("EELedTask") << "event " << ievt_ << " digi collection size " << need;

    for ( EEDigiCollection::const_iterator digiItr = digis->begin(); digiItr != digis->end(); ++digiItr ) {

      EEDetId id = digiItr->id();

      int ix = id.ix();
      int iy = id.iy();

      int ism = Numbers::iSM( id );

      if ( ! ( runType[ism] == EcalDCCHeaderBlock::LED_STD ||
               runType[ism] == EcalDCCHeaderBlock::LED_GAP ) ) continue;

      if ( runType[ism] == EcalDCCHeaderBlock::LED_GAP &&
           rtHalf[ism] != Numbers::RtHalf(id) ) continue;

      LogDebug("EELedTask") << " det id = " << id;
      LogDebug("EELedTask") << " sm, ix, iy " << ism << " " << ix << " " << iy;

      int ic = Numbers::icEE(ism, ix, iy);

      EEDataFrame dataframe = (*digiItr);

      for (int i = 0; i < 10; i++) {

        int adc = dataframe.sample(i).adc();
        float gain = 1.;

        MonitorElement* meShapeMap = 0;

        if ( dataframe.sample(i).gainId() == 1 ) gain = 1./12.;
        if ( dataframe.sample(i).gainId() == 2 ) gain = 1./ 6.;
        if ( dataframe.sample(i).gainId() == 3 ) gain = 1./ 1.;

        if ( Numbers::RtHalf(id) == 0 ) {

          if ( waveLength[ism] == 0 ) meShapeMap = meShapeMapL1A_[ism-1];
          if ( waveLength[ism] == 1 ) meShapeMap = meShapeMapL2A_[ism-1];

        } else if ( Numbers::RtHalf(id) == 1 ) {

          if ( waveLength[ism] == 0 ) meShapeMap = meShapeMapL1B_[ism-1];
          if ( waveLength[ism] == 1 ) meShapeMap = meShapeMapL2B_[ism-1];

        } else {

          LogWarning("EELedTask") << " RtHalf = " << Numbers::RtHalf(id);

        }

//        float xval = float(adc) * gain;
        float xval = float(adc);

        if ( meShapeMap ) meShapeMap->Fill(ic - 0.5, i + 0.5, xval);

      }

    }

  } else {

    LogWarning("EELedTask") << EEDigiCollection_ << " not available";

  }

  float adcA[18];
  float adcB[18];

  for ( int i = 0; i < 18; i++ ) {
    adcA[i] = 0.;
    adcB[i] = 0.;
  }

  Handle<EcalPnDiodeDigiCollection> pns;

  if ( e.getByLabel(EcalPnDiodeDigiCollection_, pns) ) {

    int nep = pns->size();
    LogDebug("EELedTask") << "event " << ievt_ << " pns collection size " << nep;

    for ( EcalPnDiodeDigiCollection::const_iterator pnItr = pns->begin(); pnItr != pns->end(); ++pnItr ) {

      if ( Numbers::subDet( pnItr->id() ) != EcalEndcap ) continue;

      int ism = Numbers::iSM( pnItr->id() );

      int num = pnItr->id().iPnId();

      if ( ! ( runType[ism] == EcalDCCHeaderBlock::LED_STD ||
               runType[ism] == EcalDCCHeaderBlock::LED_GAP ) ) continue;

      LogDebug("EELedTask") << " det id = " << pnItr->id();
      LogDebug("EELedTask") << " sm, num " << ism << " " << num;

      float xvalped = 0.;

      for (int i = 0; i < 4; i++) {

        int adc = pnItr->sample(i).adc();

        MonitorElement* mePNPed = 0;

        if ( pnItr->sample(i).gainId() == 0 ) {
          if ( waveLength[ism] == 0 ) mePNPed = mePnPedMapG01L1_[ism-1];
          if ( waveLength[ism] == 1 ) mePNPed = mePnPedMapG01L2_[ism-1];
        }
        if ( pnItr->sample(i).gainId() == 1 ) {
          if ( waveLength[ism] == 0 ) mePNPed = mePnPedMapG16L1_[ism-1];
          if ( waveLength[ism] == 1 ) mePNPed = mePnPedMapG16L2_[ism-1];
        }

        float xval = float(adc);

        if ( mePNPed ) mePNPed->Fill(num - 0.5, xval);

        xvalped = xvalped + xval;

      }

      xvalped = xvalped / 4;

      float xvalmax = 0.;

      MonitorElement* mePN = 0;

      for (int i = 0; i < 50; i++) {

        int adc = pnItr->sample(i).adc();

        float xval = float(adc);

        if ( xval >= xvalmax ) xvalmax = xval;

      }

      xvalmax = xvalmax - xvalped;

      if ( pnItr->sample(0).gainId() == 0 ) {
        if ( waveLength[ism] == 0 ) mePN = mePnAmplMapG01L1_[ism-1];
        if ( waveLength[ism] == 1 ) mePN = mePnAmplMapG01L2_[ism-1];
      }
      if ( pnItr->sample(0).gainId() == 1 ) {
        if ( waveLength[ism] == 0 ) mePN = mePnAmplMapG16L1_[ism-1];
        if ( waveLength[ism] == 1 ) mePN = mePnAmplMapG16L2_[ism-1];
      }

      if ( mePN ) mePN->Fill(num - 0.5, xvalmax);

      if ( num == 1 ) adcA[ism-1] = xvalmax;
      if ( num == 6 ) adcB[ism-1] = xvalmax;

    }

  } else {

    LogWarning("EELedTask") << EcalPnDiodeDigiCollection_ << " not available";

  }

  Handle<EcalUncalibratedRecHitCollection> hits;

  if ( e.getByLabel(EcalUncalibratedRecHitCollection_, hits) ) {

    int neh = hits->size();
    LogDebug("EELedTask") << "event " << ievt_ << " hits collection size " << neh;

    for ( EcalUncalibratedRecHitCollection::const_iterator hitItr = hits->begin(); hitItr != hits->end(); ++hitItr ) {

      EEDetId id = hitItr->id();

      int ix = id.ix();
      int iy = id.iy();

      int ism = Numbers::iSM( id );

      if ( ism >= 1 && ism <= 9 ) ix = 101 - ix;

      float xix = ix - 0.5;
      float xiy = iy - 0.5;

      if ( ! ( runType[ism] == EcalDCCHeaderBlock::LED_STD ||
               runType[ism] == EcalDCCHeaderBlock::LED_GAP ) ) continue;

      if ( runType[ism] == EcalDCCHeaderBlock::LED_GAP &&
           rtHalf[ism] != Numbers::RtHalf(id) ) continue;

      LogDebug("EELedTask") << " det id = " << id;
      LogDebug("EELedTask") << " sm, ix, iy " << ism << " " << ix << " " << iy;

      MonitorElement* meAmplMap = 0;
      MonitorElement* meTimeMap = 0;
      MonitorElement* meAmplPNMap = 0;

      if ( Numbers::RtHalf(id) == 0 ) {

        if ( waveLength[ism] == 0 ) {
          meAmplMap = meAmplMapL1A_[ism-1];
          meTimeMap = meTimeMapL1A_[ism-1];
          meAmplPNMap = meAmplPNMapL1A_[ism-1];
        }
        if ( waveLength[ism] == 1 ) {
          meAmplMap = meAmplMapL2A_[ism-1];
          meTimeMap = meTimeMapL2A_[ism-1];
          meAmplPNMap = meAmplPNMapL2A_[ism-1];
        }

      } else if ( Numbers::RtHalf(id) == 1 ) { 

        if ( waveLength[ism] == 0 ) {
          meAmplMap = meAmplMapL1B_[ism-1];
          meTimeMap = meTimeMapL1B_[ism-1];
          meAmplPNMap = meAmplPNMapL1B_[ism-1];
        }
        if ( waveLength[ism] == 1 ) {
          meAmplMap = meAmplMapL2B_[ism-1];
          meTimeMap = meTimeMapL2B_[ism-1];
          meAmplPNMap = meAmplPNMapL2B_[ism-1];
        }

      } else {

        LogWarning("EELedTask") << " RtHalf = " << Numbers::RtHalf(id);

      }

      float xval = hitItr->amplitude();
      if ( xval <= 0. ) xval = 0.0;
      float yval = hitItr->jitter() + 6.0;
      if ( yval <= 0. ) yval = 0.0;
      float zval = hitItr->pedestal();
      if ( zval <= 0. ) zval = 0.0;

      LogDebug("EELedTask") << " hit amplitude " << xval;
      LogDebug("EELedTask") << " hit jitter " << yval;
      LogDebug("EELedTask") << " hit pedestal " << zval;

      if ( meAmplMap ) meAmplMap->Fill(xix, xiy, xval);

      if ( xval > 16. ) {
        if ( meTimeMap ) meTimeMap->Fill(xix, xiy, yval);
      }

      float wval = 0.;

      if ( Numbers::RtHalf(id) == 0 ) {

        if ( adcA[ism-1] != 0. ) wval = xval / adcA[ism-1];

      } else if ( Numbers::RtHalf(id) == 1 ) {

        if ( adcB[ism-1] != 0. ) wval = xval / adcB[ism-1];

      } else {

        LogWarning("EELedTask") << " RtHalf = " << Numbers::RtHalf(id);

      }

      LogDebug("EELedTask") << " hit amplitude over PN " << wval;

      if ( meAmplPNMap ) meAmplPNMap->Fill(xix, xiy, wval);

    }

  } else {

    LogWarning("EELedTask") << EcalUncalibratedRecHitCollection_ << " not available";

  }

}


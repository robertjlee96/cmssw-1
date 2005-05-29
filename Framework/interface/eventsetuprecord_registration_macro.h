#ifndef EVENTSETUP_EVENTSETUPRECORD_REGISTRATION_MACRO_H
#define EVENTSETUP_EVENTSETUPRECORD_REGISTRATION_MACRO_H
// -*- C++ -*-
//
// Package:     CoreFramework
// Class  :     eventsetuprecord_registration_macro
// 
/**\class eventsetuprecord_registration_macro eventsetuprecord_registration_macro.h Core/CoreFramework/interface/eventsetuprecord_registration_macro.h

 Description: CPP macro used to register a new EventSetupRecord into the system

 Usage:
    Special code is needed to 'register' a new class that inherits from EventSetupRecord with they system.  The
macro EVENTSETUP_RECORD_REG is used to create that code.

     Example: You have a new record defined called 'DummyRecord'.  Then to register that record with the system
     you place the line

     EVENTSETUP_RECORD_REG(DummyRecord);

     at 'file scope' (i.e., not within a namespace, class or function) in the .cc file for DummyRecord
*/
//
// Author:      Chris Jones
// Created:     Wed Apr  6 14:33:32 EDT 2005
// $Id$
//

// system include files

// user include files
#include "FWCore/CoreFramework/interface/HCTypeTagTemplate.icc"
#include "FWCore/CoreFramework/interface/HCMethods.icc"
#include "FWCore/CoreFramework/interface/eventSetupGetImplementation.icc"

#include "FWCore/CoreFramework/interface/EventSetupRecordProviderFactoryTemplate.h"

#define EVENTSETUP_RECORD_NAME2(_a_, _b_) EVENTSETUP_RECORD_NAME2_HIDDEN(_a_,_b_)
#define EVENTSETUP_RECORD_NAME2_HIDDEN(_a_,_b_) _a_ ## _b_

#define EVENTSETUP_RECORD_REG(_recordclassname_) \
template<> const char* \
edm::eventsetup::heterocontainer::HCTypeTagTemplate<_recordclassname_, edm::eventsetup::EventSetupRecordKey>::className() {return # _recordclassname_; }\
static edm::eventsetup::EventSetupRecordProviderFactoryTemplate<_recordclassname_> EVENTSETUP_RECORD_NAME2(s_factory,__LINE__);\
template void edm::eventsetup::eventSetupGetImplementation<_recordclassname_>(edm::EventSetup const&, _recordclassname_ const*&); \
template  edm::eventsetup::EventSetupRecordKey edm::eventsetup::heterocontainer::makeKey<_recordclassname_, edm::eventsetup::EventSetupRecordKey>() ;\
template class edm::eventsetup::heterocontainer::HCTypeTagTemplate<_recordclassname_, edm::eventsetup::EventSetupRecordKey>;


#endif /* EVENTSETUP_EVENTSETUPRECORD_REGISTRATION_MACRO_H */

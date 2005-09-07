#ifndef ServiceRegistry_Service_h
#define ServiceRegistry_Service_h
// -*- C++ -*-
//
// Package:     ServiceRegistry
// Class  :     Service
// 
/**\class Service Service.h FWCore/ServiceRegistry/interface/Service.h

 Description: Smart pointer used to give easy access to Service's

 Usage:
    

*/
//
// Original Author:  Chris Jones
//         Created:  Wed Sep  7 15:17:17 EDT 2005
// $Id$
//

// system include files

// user include files
#include "FWCore/ServiceRegistry/interface/ServiceRegistry.h"

// forward declarations

namespace edm {
   template<class T>
   class Service
{
   
   public:
   Service() {}
   //virtual ~Service();
   
   // ---------- const member functions ---------------------
   T* operator->() const {
      return &(ServiceRegistry::instance().template get<T>());
   }

   T& operator*() const {
      return ServiceRegistry::instance().template get<T>();
   }
   // ---------- static member functions --------------------
   
   // ---------- member functions ---------------------------
   
   private:
};

}

#endif

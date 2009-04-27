#ifndef Fireworks_Core_FWEDProductRepresentationChecker_h
#define Fireworks_Core_FWEDProductRepresentationChecker_h
// -*- C++ -*-
//
// Package:     Core
// Class  :     FWEDProductRepresentationChecker
//
/**\class FWEDProductRepresentationChecker FWEDProductRepresentationChecker.h Fireworks/Core/interface/FWEDProductRepresentationChecker.h

   Description: <one line class summary>

   Usage:
    <usage>

 */
//
// Original Author:  Chris Jones
//         Created:  Tue Nov 11 15:20:05 EST 2008
// $Id: FWEDProductRepresentationChecker.h,v 1.2 2009/01/23 21:35:41 amraktad Exp $
//

// system include files
#include <string>

// user include files
#include "Fireworks/Core/interface/FWRepresentationCheckerBase.h"

// forward declarations

class FWEDProductRepresentationChecker : public FWRepresentationCheckerBase {

public:
   FWEDProductRepresentationChecker(const std::string& iTypeidName,
                                    const std::string& iPurpose);

   // ---------- const member functions ---------------------
   virtual FWRepresentationInfo infoFor(const std::string& iTypeName) const;

   // ---------- static member functions --------------------

   // ---------- member functions ---------------------------

private:
   FWEDProductRepresentationChecker(const FWEDProductRepresentationChecker&); // stop default

   const FWEDProductRepresentationChecker& operator=(const FWEDProductRepresentationChecker&); // stop default

   // ---------- member data --------------------------------
   const std::string m_typeidName;
};


#endif

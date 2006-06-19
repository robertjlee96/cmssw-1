#ifndef LOCATIONDEF_H
#define LOCATIONDEF_H

#include <string>
#include <stdexcept>

#include "OnlineDB/EcalCondDB/interface/IDef.h"

/**
 *   Def for Location information
 */
class LocationDef : public IDef {
  public:
  friend class EcalCondDBInterface;
  
  LocationDef();
  virtual ~LocationDef();

  // Methods for user data
  std::string getLocation() const;
  void setLocation(std::string loc);

  // Methods from IUniqueDBObject
  int fetchID() throw(std::runtime_error);
  void setByID(int id) throw(std::runtime_error);

  inline bool operator==(const LocationDef& l) const { return m_loc == l.m_loc; }
  inline bool operator!=(const LocationDef& l) const { return m_loc != l.m_loc; }

 protected:
  // User data for this def
  std::string m_loc;

  void fetchAllDefs( std::vector<LocationDef>* fillVec) throw(std::runtime_error);
};

#endif

#ifndef DTStatusFlag_H
#define DTStatusFlag_H
/** \class DTStatusFlag
 *
 *  Description:
 *       Class to hold drift tubes status
 *             ( cell by cell noise and masks )
 *
 *  $Date: 2006/06/12 13:45:12 $
 *  $Revision: 1.2 $
 *  \author Paolo Ronchese INFN Padova
 *
 */

//----------------------
// Base Class Headers --
//----------------------


//------------------------------------
// Collaborating Class Declarations --
//------------------------------------
#include "DataFormats/MuonDetId/interface/DTWireId.h"

//---------------
// C++ Headers --
//---------------
#include <string>
#include <map>

//              ---------------------
//              -- Class Interface --
//              ---------------------

class DTStatusFlagId {

 public:

  DTStatusFlagId();
  ~DTStatusFlagId();

  int   wheelId;
  int stationId;
  int  sectorId;
  int      slId;
  int   layerId;
  int    cellId;

};


class DTStatusFlagData {

 public:

  DTStatusFlagData();
  ~DTStatusFlagData();

  bool noiseFlag;
  bool    feMask;
  bool   tdcMask;
  bool  trigMask;
  bool  deadFlag;
  bool  nohvFlag;

};


class DTStatusFlagCompare {
 public:
  bool operator()( const DTStatusFlagId& idl,
                   const DTStatusFlagId& idr ) const;
};


class DTStatusFlag {

 public:

  /** Constructor
   */
  DTStatusFlag();
  DTStatusFlag( const std::string& version );

  /** Destructor
   */
  ~DTStatusFlag();

  /** Operations
   */
  /// get content
  int cellStatus( int   wheelId,
                  int stationId,
                  int  sectorId,
                  int      slId,
                  int   layerId,
                  int    cellId,
                  bool& noiseFlag,
                  bool&    feMask,
                  bool&   tdcMask,
                  bool&  trigMask,
                  bool&  deadFlag,
                  bool&  nohvFlag ) const;
  int cellStatus( const DTWireId& id,
                  bool& noiseFlag,
                  bool&    feMask,
                  bool&   tdcMask,
                  bool&  trigMask,
                  bool&  deadFlag,
                  bool&  nohvFlag ) const;

  /// access version
  const
  std::string& version() const;
  std::string& version();

  /// reset content
  void clear();

  int setCellStatus( int   wheelId,
                     int stationId,
                     int  sectorId,
                     int      slId,
                     int   layerId,
                     int    cellId,
                     bool noiseFlag,
                     bool    feMask,
                     bool   tdcMask,
                     bool  trigMask,
                     bool  deadFlag,
                     bool  nohvFlag );
  int setCellStatus( const DTWireId& id,
                     bool noiseFlag,
                     bool    feMask,
                     bool   tdcMask,
                     bool  trigMask,
                     bool  deadFlag,
                     bool  nohvFlag );

  int setCellNoise( int   wheelId,
                    int stationId,
                    int  sectorId,
                    int      slId,
                    int   layerId,
                    int    cellId,
                    bool flag );
  int setCellNoise( const DTWireId& id,
                    bool flag );

  int setCellFEMask( int   wheelId,
                     int stationId,
                     int  sectorId,
                     int      slId,
                     int   layerId,
                     int    cellId,
                     bool mask );
  int setCellFEMask( const DTWireId& id,
                     bool mask );

  int setCellTDCMask( int   wheelId,
                      int stationId,
                      int  sectorId,
                      int      slId,
                      int   layerId,
                      int    cellId,
                      bool mask );
  int setCellTDCMask( const DTWireId& id,
                      bool mask );

  int setCellTrigMask( int   wheelId,
                       int stationId,
                       int  sectorId,
                       int      slId,
                       int   layerId,
                       int    cellId,
                       bool mask );
  int setCellTrigMask( const DTWireId& id,
                       bool mask );

  int setCellDead( int   wheelId,
                   int stationId,
                   int  sectorId,
                   int      slId,
                   int   layerId,
                   int    cellId,
                   bool flag );
  int setCellDead( const DTWireId& id,
                   bool flag );

  int setCellNoHV( int   wheelId,
                   int stationId,
                   int  sectorId,
                   int      slId,
                   int   layerId,
                   int    cellId,
                   bool flag );
  int setCellNoHV( const DTWireId& id,
                   bool flag );

  /// Access methods to data
  typedef std::map<DTStatusFlagId,
                   DTStatusFlagData,
                   DTStatusFlagCompare>::const_iterator const_iterator;
  const_iterator begin() const;
  const_iterator end() const;

 private:

  std::string dataVersion;

  std::map<DTStatusFlagId,DTStatusFlagData,DTStatusFlagCompare> cellData;

};


#endif // DTStatusFlag_H


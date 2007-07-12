#ifndef CalibTracker_SiStripConnectivity_SiStripRegionCabling_H
#define CalibTracker_SiStripConnectivity_SiStripRegionCabling_H
#define _USE_MATH_DEFINES

#include <boost/cstdint.hpp>

//CondFormats
#include "CondFormats/SiStripObjects/interface/FedChannelConnection.h"

//DataFormats
#include "DataFormats/SiStripDetId/interface/SiStripDetId.h"
#include "DataFormats/SiStripCommon/interface/SiStripRefGetter.h"
#include "DataFormats/Common/interface/Handle.h"

//stl
#include <vector>
#include <map>
#include <cmath>

/**
   Author: pwing
   Package: CalibFormats/SiStripObjects
   Class: SiStripRegionCabling
   Description: Gives a regional view of the silicon strip tracker cabling. 
   Cabling is divided into (eta,phi) "regions". A "region" within a given 
   sub-detector is called a "wedge". A layer within a given wedge is called 
   an "element".
*/

class SiStripRegionCabling {

 public:

  static const uint32_t MAXLAYERS = 10; //Maximum layers of a sub-detector
  static const uint32_t MAXSUBDETS = 4; //Maximum number of sub-detectors
  enum SubDet {TIB = 0, TOB = 1, TID = 2, TEC = 3, UNKNOWN = 4}; 

  typedef std::map< uint32_t, std::vector<FedChannelConnection> > ElementCabling;
  typedef std::vector< ElementCabling > WedgeCabling;
  typedef std::vector< WedgeCabling > RegionCabling;
  typedef std::vector< RegionCabling > Cabling;
  typedef std::pair<double,double> Position;
  typedef std::pair<uint32_t,uint32_t> PositionIndex;
  typedef uint32_t Region;
  typedef uint32_t Layer;
  typedef uint32_t ElementIndex;

  ~SiStripRegionCabling() {;}

  SiStripRegionCabling(const uint32_t,const uint32_t, const double);

  /** Set and get methods for cabling. */

  inline void setRegionCabling(const Cabling&);

  inline const Cabling& getRegionCabling() const;

  inline const uint32_t etadivisions() const;

  inline const uint32_t phidivisions() const;

  /** Utility methods for interchanging between region, region-indices and 
      eta/phi-position. */

  inline const std::pair<double,double> regionDimensions() const;

  inline const Position position(Region) const;

  inline const Position position(PositionIndex) const;

  inline const PositionIndex positionIndex(Region) const;

  const PositionIndex positionIndex(Position) const;

  const Region region(Position) const;
  
  inline const Region region(PositionIndex) const;

  /** Utility methods for interchanging between region-subdet-layer and the 
      corresponding element index. */

  inline static const ElementIndex elementIndex(const Region, const SubDet, const Layer);

  inline const ElementIndex elementIndex(const Position, const SubDet, const Layer) const;

  inline static const Layer layer(const ElementIndex);
  
  inline static const SubDet subdet(const ElementIndex);
  
  inline static const Region region(const ElementIndex);
 
  /** Utility methods for extracting det-id information */

  static const SubDet subdetFromDetId(uint32_t);

  static const uint32_t layerFromDetId(uint32_t);

  /** Utility methods for updating a SiStripRefGetter<T> container with elements of interest  */
  
  template <class T>
    void updateSiStripRefGetter(edm::SiStripRefGetter<T>& refgetter, 
				const edm::Handle< edm::SiStripLazyGetter<T> >& lazygetter, 
				const ElementIndex index) const;
  
  template <class T>
    void updateSiStripRefGetter(edm::SiStripRefGetter<T>& refgetter, 
				const edm::Handle< edm::SiStripLazyGetter<T> >& lazygetter,
				const Position position, 
				const double deltaeta, 
				const double deltaphi, 
				const SubDet subdet, 
				const Layer layer) const;
  
  template <class T>
    void updateSiStripRefGetter(edm::SiStripRefGetter<T>& refgetter, 
				const edm::Handle< edm::SiStripLazyGetter<T> >& lazygetter,
				const Position position, 
				const double dR,
				const SubDet subdet,
				const Layer layer) const;
  
 private:
 
 SiStripRegionCabling() {;}
 
 /** Number of regions in eta,phi */
 uint32_t etadivisions_;
 uint32_t phidivisions_;
 
 /** Tracker extent in eta */
 double etamax_;
 
 /** Cabling */
 Cabling regioncabling_;
}; 

inline void SiStripRegionCabling::setRegionCabling(const Cabling& regioncabling) {
  regioncabling_ = regioncabling;
}

inline const SiStripRegionCabling::Cabling& SiStripRegionCabling::getRegionCabling() const {
  return regioncabling_;
}

inline const uint32_t SiStripRegionCabling::etadivisions() const {
  return etadivisions_;
}

inline const uint32_t SiStripRegionCabling::phidivisions() const {
  return phidivisions_;
}

inline const std::pair<double,double> SiStripRegionCabling::regionDimensions() const {
  return std::pair<double,double>((2.*etamax_)/etadivisions_,2.*M_PI/phidivisions_);
}

inline const SiStripRegionCabling::Position SiStripRegionCabling::position(Region region) const {
  PositionIndex index = positionIndex(region); 
  return position(index);
}

inline const SiStripRegionCabling::Position SiStripRegionCabling::position(PositionIndex index) const {
  return Position(regionDimensions().first*index.first + regionDimensions().first/2.,
		  regionDimensions().second*index.second + regionDimensions().second/2.);
}

inline const SiStripRegionCabling::PositionIndex SiStripRegionCabling::positionIndex(Region region) const {
  return PositionIndex(region/phidivisions_,region%phidivisions_);
}

inline const SiStripRegionCabling::Region SiStripRegionCabling::region(PositionIndex index) const {
  return index.first*phidivisions_ + index.second;
}
  
inline const SiStripRegionCabling::ElementIndex SiStripRegionCabling::elementIndex(const Region region, const SubDet subdet, const Layer layer) {
  return region*MAXSUBDETS*MAXLAYERS + subdet*MAXLAYERS + layer;
}

inline const SiStripRegionCabling::ElementIndex SiStripRegionCabling::elementIndex(const Position position, const SubDet subdet, const Layer layer) const {
  return elementIndex(region(position),subdet,layer);
}

inline const SiStripRegionCabling::Layer SiStripRegionCabling::layer(const ElementIndex index) {
  return index%MAXLAYERS;
}
  
inline const SiStripRegionCabling::SubDet SiStripRegionCabling::subdet(const ElementIndex index) {
  return static_cast<SiStripRegionCabling::SubDet>((index/MAXLAYERS)%MAXSUBDETS);
}
  
inline const SiStripRegionCabling::Region SiStripRegionCabling::region(const ElementIndex index) {
  return index/(MAXSUBDETS*MAXLAYERS);
}

template <class T>
void SiStripRegionCabling::updateSiStripRefGetter(edm::SiStripRefGetter<T>& refgetter, const edm::Handle< edm::SiStripLazyGetter<T> >& lazygetter, const ElementIndex index) const {
  refgetter.push_back(lazygetter,index);
}

template <class T>
void SiStripRegionCabling::updateSiStripRefGetter(edm::SiStripRefGetter<T>& refgetter, const edm::Handle< edm::SiStripLazyGetter<T> >& lazygetter, const SiStripRegionCabling::Position position, const double deltaeta, const double deltaphi, const SubDet subdet, const Layer layer) const {
  
  //Calculate region of interest boundary
  PositionIndex index = positionIndex(position);
  uint32_t deta = deltaeta/regionDimensions().first;
  uint32_t dphi = deltaphi/regionDimensions().second;
  
  //Loop eta index
  for (uint32_t ieta = 0; ieta < 2*deta + 1; ieta++) {
    int etatemp = index.first - deta + ieta;
    if ((etatemp < 0) || (etatemp >= (int)etadivisions_)) continue;
    
    //Loop phi index
    for (uint32_t iphi = 0; iphi < 2*dphi + 1; iphi++) {
      int phitemp = index.second - dphi + iphi;
      if (phitemp >= (int)phidivisions_) phitemp -= phidivisions_;
      else if (phitemp < 0) phitemp += phidivisions_;
      
      //Update SiStripRefGetter<T>
      updateSiStripRefGetter<T>(refgetter,lazygetter,elementIndex(Position(etatemp,phitemp),subdet,layer));
    }
  }
}

template <class T>
void SiStripRegionCabling::updateSiStripRefGetter(edm::SiStripRefGetter<T>& refgetter, const edm::Handle< edm::SiStripLazyGetter<T> >& lazygetter, const Position position, const double dR, const SubDet subdet, const Layer layer) const {
  SiStripRegionCabling::updateSiStripRefGetter<T>(refgetter,lazygetter,position, 1./sqrt(2)*dR*dR,1./sqrt(2)*dR*dR,subdet,layer);
}


#endif

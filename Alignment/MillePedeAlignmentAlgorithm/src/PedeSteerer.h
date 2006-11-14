#ifndef PEDESTEERER_H
#define PEDESTEERER_H

/**
 * \class PedeSteerer
 *
 * provides steering for pede according to configuration
 *
 * \author    : Gero Flucke
 * date       : October 2006
 * $Date: 2006/11/07 10:43:23 $
 * $Revision: 1.2 $
 * (last update by $Author: flucke $)
 */

#include <fstream>
#include <vector>
#include <map> 

class Alignable;
class AlignableTracker;
class AlignmentParameterStore;
namespace edm {
  class ParameterSet;
}

/***************************************
****************************************/
class PedeSteerer
{
 public:
  PedeSteerer(AlignableTracker *tracker, AlignmentParameterStore *store,
	      const edm::ParameterSet &config);
  /** non-virtual destructor: do not inherit from this class */
  ~PedeSteerer();
    
  /// uniqueId of Alignable, 0 if alignable not known
  /// between this ID and the next there is enough 'space' to add parameter
  /// numbers 0...nPar-1 to make unique IDs for the labels of active parameters
  unsigned int alignableLabel(Alignable *alignable) const;
  unsigned int parameterLabel(unsigned int aliLabel, unsigned int parNum) const;
  
  /// parameter number, 0 <= .. < theMaxNumParam, belonging to unique parameter label
  unsigned int paramNumFromLabel(unsigned int paramLabel) const;
  /// alignable label from parameter label (works also for alignable label...)
  unsigned int alignableLabelFromLabel(unsigned int label) const;
  /// alignable from alignable or parameter label
  Alignable* alignableFromLabel(unsigned int label) const;

 private:
  typedef std::map <Alignable*, unsigned int> AlignableToIdMap;
  typedef std::pair<Alignable*, unsigned int> AlignableToIdPair;

  typedef std::map <unsigned int, Alignable*> IdToAlignableMap;
  typedef std::pair<unsigned int, Alignable*> IdToAlignablePair;

  unsigned int buildMap(Alignable *highestLevelAli);
  unsigned int buildReverseMap();
  unsigned int fixParameters(AlignmentParameterStore *store, AlignableTracker *alignableTracker,
			     const edm::ParameterSet &config);

  std::ofstream     mySteerFile; // text file
  AlignableToIdMap  myAlignableToIdMap; /// providing unique ID for alignable with space for params
  IdToAlignableMap  myIdToAlignableMap; /// reverse map

  static const unsigned int theMaxNumParam;
  static const unsigned int theMinLabel;
};

#endif

//
#ifndef Bsc_BscSD_h
#define Bsc_BscSD_h
//

#include "SimG4Core/Notification/interface/Observer.h"
#include "SimG4Core/SensitiveDetector/interface/SensitiveTkDetector.h"

#include "SimG4Core/Notification/interface/BeginOfEvent.h"
#include "SimG4Core/Notification/interface/EndOfEvent.h"

// last
//#include "SimG4Core/Application/interface/SimTrackManager.h"
//#include "SimG4CMS/Calo/interface/CaloSD.h"


//#include "SimG4Core/Notification/interface/TrackWithHistory.h"
//#include "SimG4Core/Notification/interface/TrackContainer.h"

#include "SimG4CMS/Forward/interface/BscG4Hit.h"
#include "SimG4CMS/Forward/interface/BscG4HitCollection.h"
#include "SimG4CMS/Forward/interface/BscNumberingScheme.h"

  
#include "G4Step.hh"
#include "G4StepPoint.hh"
#include "G4Track.hh"
#include "G4VPhysicalVolume.hh"

//#include <CLHEP/Vector/ThreeVector.h>
//#include <iostream>
//#include <fstream>
//#include <vector>
//#include <map>
#include <string>
 


class TrackingSlaveSD;
//AZ:
class BscSD;

class TrackInformation;
class SimTrackManager;
class TrackingSlaveSD;
class UpdatablePSimHit;
class G4ProcessTypeEnumerator;
class G4TrackToParticleID;


//-------------------------------------------------------------------

class BscSD : public SensitiveTkDetector,
		public Observer<const BeginOfEvent*>,
		public Observer<const EndOfEvent*> {

public:
  
  BscSD(std::string, const DDCompactView &, SensitiveDetectorCatalog &, 
  	  edm::ParameterSet const &, const SimTrackManager* );


  virtual ~BscSD();
  
  virtual bool ProcessHits(G4Step *,G4TouchableHistory *);
  virtual uint32_t  setDetUnitId(G4Step*);

  virtual void Initialize(G4HCofThisEvent * HCE);
  virtual void EndOfEvent(G4HCofThisEvent * eventHC);
  virtual void clear();
  virtual void DrawAll();
  virtual void PrintAll();

  virtual double getEnergyDeposit(G4Step* step);
  //protected:
  //    Collection       hits_;
    void fillHits(edm::PSimHitContainer&, std::string use);
  
  std::vector<std::string> getNames();
  
 private:
  void           update(const BeginOfEvent *);
  void           update(const ::EndOfEvent *);
  virtual void   clearHits();
  
  //void SetNumberingScheme(BscNumberingScheme* scheme);
  
  
  
  //  int eventno;
 private:
  
  G4ThreeVector SetToLocal(G4ThreeVector global);
  G4ThreeVector SetToLocalExit(G4ThreeVector globalPoint);
  void          GetStepInfo(G4Step* aStep);
  G4bool        HitExists();
  void          CreateNewHit();
  void          UpdateHit();
  void          StoreHit(BscG4Hit*);
  void          ResetForNewPrimary();
  void          Summarize();
  
  
 private:
  
  //AZ:
  TrackingSlaveSD* slave;
  BscNumberingScheme * numberingScheme;
  
  G4ThreeVector entrancePoint, exitPoint;
  G4ThreeVector theEntryPoint ;
  G4ThreeVector theExitPoint  ;
  
  float                incidentEnergy;
  G4int                primID  ; 
  
  //  G4String             name;
  std::string             name;
  G4int                    hcID;
  BscG4HitCollection*       theHC; 
  const SimTrackManager*      theManager;
 
  G4int                    tsID; 
  BscG4Hit*               currentHit;
  G4Track*                 theTrack;
  G4VPhysicalVolume*         currentPV;
  // unsigned int         unitID, previousUnitID;
  uint32_t             unitID, previousUnitID;
  G4int                primaryID, tSliceID;  
  G4double             tSlice;
  
  G4StepPoint*         preStepPoint; 
  G4StepPoint*         postStepPoint; 
  float                edeposit;
  
  G4ThreeVector        hitPoint;
  //  G4ThreeVector    Position;
  G4ThreeVector        hitPointExit;
  G4ThreeVector        hitPointLocal;
  G4ThreeVector        hitPointLocalExit;
  float Pabs;
  float Tof;
  float Eloss;	
  short ParticleType; 
  
  float ThetaAtEntry;
  float PhiAtEntry;
  
  int ParentId;
  float Vx,Vy,Vz;
  float X,Y,Z;
  
  
  //
  // Hist
  //
  int eventno;
  
 protected:
  
  float                edepositEM, edepositHAD;
};

#endif // BscSD_h





#include <iostream>
#include <cmath>
#include <string>

#include "GeneratorInterface/ReggeGribovPartonMCInterface/interface/ReggeGribovPartonMCHadronizer.h"
#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"

#include "CLHEP/Random/RandomEngine.h"
#include "CLHEP/Random/RandFlat.h"

#include <HepMC/GenCrossSection.h>
#include <HepMC/GenEvent.h>
#include <HepMC/GenVertex.h>
#include <HepMC/GenParticle.h>
#include <HepMC/HeavyIon.h>
#include <HepMC/PdfInfo.h>
#include <HepMC/Units.h>

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
using namespace edm;
using namespace std;
using namespace gen;

#include "GeneratorInterface/ReggeGribovPartonMCInterface/interface/IO_EPOS.h"
#include "GeneratorInterface/ReggeGribovPartonMCInterface/interface/EPOS_Wrapper.h"

EPOS::IO_EPOS conv;

extern "C"
{
  float gen::rangen_()
  {
    float a = float(gFlatDistribution_->fire());
    return a;
  }

  double gen::drangen_(int *idummy)
  {
    double a = gFlatDistribution_->fire();
    return a;
  }
}

ReggeGribovPartonMCHadronizer::ReggeGribovPartonMCHadronizer(const ParameterSet &pset) :
  BaseHadronizer(pset),
  pset_(pset),
  m_BeamMomentum(pset.getParameter<double>("beammomentum")),
  m_TargetMomentum(pset.getParameter<double>("targetmomentum")),
  m_BeamID(pset.getParameter<int>("beamid")),
  m_TargetID(pset.getParameter<int>("targetid")),
  m_HEModel(pset.getParameter<int>("model")),
  m_bMin(pset.getParameter<double>("bmin")),
  m_bMax(pset.getParameter<double>("bmax")),
  m_ParamFileName(pset.getUntrackedParameter<string>("paramFileName")),
  m_NEvent(0),
  m_ImpactParameter(0.)
{
  Service<RandomNumberGenerator> rng;
  if ( ! rng.isAvailable())
    {
      throw cms::Exception("Configuration")
        << "ReggeGribovPartonMCHadronizer requires the RandomNumberGeneratorService\n"
        "which is not present in the configuration file.  You must add the service\n"
        "in the configuration file or remove the modules that require it.";
    }

  gFlatDistribution_.reset(new CLHEP::RandFlat(rng->getEngine(), 0., 1.));

  int  nevet       = 1; //needed for CS
  int  noTables    = 0; //don't calculate tables
  int  LHEoutput   = 0; //no lhe
  int  dummySeed   = 123;
  char dummyName[] = "dummy";
  crmc_set_f_(nevet,dummySeed,m_BeamMomentum,m_TargetMomentum,m_BeamID,
              m_TargetID,m_HEModel,noTables,LHEoutput,dummyName,
              m_ParamFileName.fullPath().c_str());

  //additionally initialise tables
  initializeTablePaths();

  //change impact paramter
  nucl2_.bminim = float(m_bMin);
  nucl2_.bmaxim = float(m_bMax);

  //use set parameters to init models
  crmc_init_f_();
}


//_____________________________________________________________________
ReggeGribovPartonMCHadronizer::~ReggeGribovPartonMCHadronizer()
{
  // destructor
  gFlatDistribution_.reset();
}

//_____________________________________________________________________
bool ReggeGribovPartonMCHadronizer::generatePartonsAndHadronize()
{
  int iout=0,ievent=0;
  crmc_f_(iout,ievent,m_NParticles,m_ImpactParameter,
         m_PartID[0],m_PartPx[0],m_PartPy[0],m_PartPz[0],
          m_PartEnergy[0],m_PartMass[0],m_PartStatus[0]);
  LogDebug("ReggeGribovPartonMCInterface") << "event generated" << endl;

  HepMC::GenEvent* evt = new HepMC::GenEvent();

  evt->set_event_number(m_NEvent++);
  int sig_id = -1;
  switch (int(c2evt_.typevt)) // if negative typevt mini plasma was created by event (except -4)
    {
    case  0: break; //unknown for qgsjetII
    case  1: sig_id = 101; break;
    case -1: sig_id = 101; break;
    case  2: sig_id = 105; break;
    case -2: sig_id = 105; break;
    case  3: sig_id = 102; break;
    case -3: sig_id = 102; break;
    case  4: sig_id = 103; break;
    case -4: sig_id = 104; break;
    default: LogDebug("ReggeGribovPartonMCInterface") << "Signal ID not recognised for setting HEPEVT" << endl;
    }
  evt->set_signal_process_id(sig_id); //an integer ID uniquely specifying the signal process (i.e. MSUB in Pythia)

#ifdef HEPMC_HAS_CROSS_SECTION
  // set cross section information for this event
  HepMC::GenCrossSection theCrossSection;
  theCrossSection.set_cross_section(double(hadr5_.sigineaa)*1e9); //required in pB
  evt->set_cross_section(theCrossSection);
#endif

  //create event structure;
/*
  vector<HepMC::GenParticle*> vecGenParticles;
  vecGenParticles.resize(m_NParticles);

  for(int i = 0; i < m_NParticles; i++)
    {
      //add particle. do not delete. not stored as a copy
      HepMC::GenParticle* p = new HepMC::GenParticle(HepMC::FourVector(m_PartPx[i],
                                                                       m_PartPy[i],
                                                                       m_PartPz[i],
                                                                       m_PartEnergy[i]),
                                                     m_PartID[i],
                                                     m_PartStatus[i]);
      vecGenParticles[i] = p;
    }


  //number of beam particles
  HepMC::FourVector primaryPos(hepcom_.vhep[0][0],hepcom_.vhep[1][0],hepcom_.vhep[2][0],0);
  for(int i = 0; i < m_NParticles; i++)
    {
      HepMC::GenParticle* p = vecGenParticles[i];
      HepMC::FourVector pos(hepcom_.vhep[0][i]-primaryPos.x(),hepcom_.vhep[1][i]-primaryPos.y(),hepcom_.vhep[2][i]-primaryPos.z(),0);
      int firstMother = hepcom_.jmohep[1][i];
      int secondMother = hepcom_.jmohep[2][i];

      p->print();
      cout << firstMother << " " << secondMother << " -> " << i << endl;
      if (firstMother == -1)
        firstMother = 0;
      if (secondMother == -1)
        secondMother = 0; //heavy ion fix
      if (firstMother < 0 || secondMother < 0 || firstMother > m_NParticles || secondMother > m_NParticles)
        {
          LogError("ReggeGribovPartonMCInterface") << "First mother: " << firstMother << " second mother: " << secondMother << " (max: " << m_NParticles << "). Exiting ..." << endl;
          exit(1);
        }
      //Check if production vertex exists
      HepMC::GenVertex* vertex = p->production_vertex();
      if (!vertex && vecGenParticles[firstMother]->end_vertex ())
        {
          vertex = vecGenParticles[firstMother]->end_vertex ();
          vertex->add_particle_out(p);
          if (vertex != vecGenParticles[secondMother]->end_vertex ())
            vertex->add_particle_in(vecGenParticles[secondMother]);
          cout << "mother 1 vtx " << firstMother << " " << vecGenParticles[firstMother]->production_vertex () << " " << secondMother << " " << vecGenParticles[secondMother]->production_vertex () << endl;
        }
      if (!vertex && vecGenParticles[secondMother]->end_vertex ())
        {
          vertex = vecGenParticles[secondMother]->end_vertex ();
          vertex->add_particle_out(p);
          if (vertex != vecGenParticles[firstMother]->end_vertex ())
            vertex->add_particle_in(vecGenParticles[firstMother]);
          cout << "mother 2 vtx " << secondMother << " " << vecGenParticles[secondMother]->production_vertex () << endl;
        }

      //no vertex found
      if(!vertex)
        {          cout << "mother new vtx" << endl;

          vertex = new HepMC::GenVertex(pos);
          if(!vertex)
            LogError("ReggeGribovPartonMCInterface") << "Could not create new vertex" << endl;

          if(p->status() == 4)
            vertex->add_particle_in(p);
          else
            vertex->add_particle_out(p);
          evt->add_vertex(vertex);
          if(0 &&p->status() == 4)
            evt->set_signal_process_vertex(vertex); // beam particle. set signal vertex
        }
    }

  if (m_TargetID + m_BeamID > 2) //other than pp
    {
      HepMC::HeavyIon ion(cevt_.kohevt,                // Number of hard scatterings
                          cevt_.npjevt,                // Number of projectile participants
                          cevt_.ntgevt,                // Number of target participants
                          cevt_.kolevt,                // Number of NN (nucleon-nucleon) collisions
                          cevt_.npnevt + cevt_.ntnevt, // Number of spectator neutrons
                          cevt_.nppevt + cevt_.ntpevt, // Number of spectator protons
                          -1,                          // Number of N-Nwounded collisions
                          -1,                          // Number of Nwounded-N collisons
                          -1,                          // Number of Nwounded-Nwounded collisions
                          cevt_.bimevt,                // Impact Parameter(fm) of collision
                          cevt_.phievt,                // Azimuthal angle of event plane
                          c2evt_.fglevt,               // eccentricity of participating nucleons
                          hadr5_.sigine*1e9);        // nucleon-nucleon inelastic (in pB)
      evt->set_heavy_ion(ion);
    }
  LogDebug("ReggeGribovPartonMCInterface") << "HepEvt and vertex constructed" << endl;
  evt->print();
  event().reset(evt);*/
  event().reset(conv.read_next_event());
  //EPOS::EPOS_Wrapper::print_hepcom();

  return true;
}

//_____________________________________________________________________
bool ReggeGribovPartonMCHadronizer::hadronize()
{
   return false;
}

bool ReggeGribovPartonMCHadronizer::decay()
{
   return true;
}

bool ReggeGribovPartonMCHadronizer::residualDecay()
{
   return true;
}

void ReggeGribovPartonMCHadronizer::finalizeEvent(){
    return;
}

void ReggeGribovPartonMCHadronizer::statistics(){
    return;
}

const char* ReggeGribovPartonMCHadronizer::classname() const
{
   return "gen::ReggeGribovPartonMCHadronizer";
}

bool ReggeGribovPartonMCHadronizer::declareStableParticles ( const std::vector<int> )
{
 return true;
 }

bool ReggeGribovPartonMCHadronizer::initializeForInternalPartons()
{
 return true;
 }

bool ReggeGribovPartonMCHadronizer::initializeTablePaths()
{
  //epos
  string path_fnii(FileInPath("GeneratorInterface/ReggeGribovPartonMCInterface/data/epos.initl").fullPath());
  string path_fnie(FileInPath("GeneratorInterface/ReggeGribovPartonMCInterface/data/epos.iniev").fullPath());
  string path_fnrj(FileInPath("GeneratorInterface/ReggeGribovPartonMCInterface/data/epos.inirj").fullPath());
  string path_fncs(FileInPath("GeneratorInterface/ReggeGribovPartonMCInterface/data/epos.inics").fullPath());

  if (path_fnii.length() >= 500) LogError("ReggeGribovPartonMCInterface") << "table path too long" << endl;
  else nfname_.nfnii = path_fnii.length();
  if (path_fnie.length() >= 500) LogError("ReggeGribovPartonMCInterface") << "table path too long" << endl;
  else nfname_.nfnie = path_fnie.length();
  if (path_fnrj.length() >= 500) LogError("ReggeGribovPartonMCInterface") << "table path too long" << endl;
  else nfname_.nfnrj = path_fnrj.length();
  if (path_fncs.length() >= 500) LogError("ReggeGribovPartonMCInterface") << "table path too long" << endl;
  else nfname_.nfncs = path_fncs.length();

  strcpy(fname_.fnii, path_fnii.c_str());
  strcpy(fname_.fnie, path_fnie.c_str());
  strcpy(fname_.fnrj, path_fnrj.c_str());
  strcpy(fname_.fncs, path_fncs.c_str());

  //qgsjet
  string path_fndat(FileInPath("GeneratorInterface/ReggeGribovPartonMCInterface/data/qgsjet.dat").fullPath());
  string path_fnncs(FileInPath("GeneratorInterface/ReggeGribovPartonMCInterface/data/qgsjet.ncs").fullPath());

  if (path_fndat.length() >= 500) LogError("ReggeGribovPartonMCInterface") << "table path too long" << endl;
  else qgsnfname_.nfndat = path_fndat.length();
  if (path_fnncs.length() >= 500) LogError("ReggeGribovPartonMCInterface") << "table path too long" << endl;
  else qgsnfname_.nfnncs = path_fnncs.length();

  strcpy(qgsfname_.fndat, path_fndat.c_str());
  strcpy(qgsfname_.fnncs, path_fnncs.c_str());

  qgsfname_.ifdat=1; //option to overwrite the normal path
  qgsfname_.ifncs=2;

  //qgsjetII
  string path_fniidat(FileInPath("GeneratorInterface/ReggeGribovPartonMCInterface/data/qgsdat-II-04.lzma").fullPath());
  string path_fniincs(FileInPath("GeneratorInterface/ReggeGribovPartonMCInterface/data/sectnu-II-04").fullPath());

  if (path_fniidat.length() >= 500) LogError("ReggeGribovPartonMCInterface") << "table path too long" << endl;
  else qgsiinfname_.nfniidat = path_fniidat.length();
  if (path_fniincs.length() >= 500) LogError("ReggeGribovPartonMCInterface") << "table path too long" << endl;
  else qgsiinfname_.nfniincs = path_fniincs.length();

  strcpy(qgsiifname_.fniidat, path_fniidat.c_str());
  strcpy(qgsiifname_.fniincs, path_fniincs.c_str());

  qgsiifname_.ifiidat=1; //option to overwrite the normal path
  qgsiifname_.ifiincs=2;

 return true;
}

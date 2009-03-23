#include "GeneratorInterface/PomwigInterface/interface/PomwigHadronizer.h"

#include <cstring>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <map>
#include <set>

#include <boost/shared_ptr.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

#include <HepMC/GenEvent.h>
#include <HepMC/GenParticle.h>
#include <HepMC/GenVertex.h>
#include <HepMC/PdfInfo.h>
#include <HepMC/HerwigWrapper6_4.h>
#include <HepMC/HEPEVT_Wrapper.h>
#include <HepMC/IO_HERWIG.h>

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"

#include "SimDataFormats/GeneratorProducts/interface/LesHouches.h"
#include "SimDataFormats/GeneratorProducts/interface/LHECommonBlocks.h"

#include "GeneratorInterface/Core/interface/ParameterCollector.h"
#include "GeneratorInterface/Core/interface/BaseHadronizer.h"

#include "GeneratorInterface/LHEInterface/interface/LHEEvent.h"

#include "GeneratorInterface/Herwig6Interface/interface/Herwig6Instance.h"
#include "GeneratorInterface/Herwig6Interface/interface/herwig.h"

namespace gen
{
extern "C" {
	void hwuidt_(int *iopt, int *ipdg, int *iwig, char nwig[8]);
}

// helpers
namespace {
	static inline bool call_hwmsct()
	{
		int result;
		hwmsct(&result);
		return result;
	}

	static int pdgToHerwig(int ipdg, char *nwig)
	{
		int iopt = 1;
		int iwig = 0;
		hwuidt_(&iopt, &ipdg, &iwig, nwig);
		return ipdg ? iwig : 0;
	}

	static bool markStable(int pdgId)
	{
		char nwig[9] = "        ";
		if (!pdgToHerwig(pdgId, nwig))
			return false;
		hwusta(nwig, 1);
		return true;
	}
}

#define qcd_1994 qcd_1994_
extern "C" {
    void qcd_1994(double&,double&,double*,int&);
}
// For H1 2006 fits
#define qcd_2006 qcd_2006_
extern "C" {
    void qcd_2006(double&,double&,int&,double*,double*,double*,double*,double*);
}

extern "C" {
	void hwwarn_(const char *method, int *id);
	void setherwpdf_(void);
	void mysetpdfpath_(const char *path);
}

PomwigHadronizer::PomwigHadronizer(const edm::ParameterSet &params) :
	needClear(false),
	parameters(params.getParameter<edm::ParameterSet>("HerwigParameters")),
	herwigVerbosity(params.getUntrackedParameter<int>("herwigVerbosity", 0)),
	hepmcVerbosity(params.getUntrackedParameter<int>("hepmcVerbosity", 0)),
	maxEventsToPrint(params.getUntrackedParameter<int>("maxEventsToPrint", 0)),
	printCards(params.getUntrackedParameter<bool>("printCards", false)),
	comEnergy(params.getParameter<double>("comEnergy")),
        survivalProbability(params.getParameter<double>("survivalProbability")),
        diffTopology(params.getParameter<int>("diffTopology")),
        h1fit(params.getParameter<int>("h1fit")),
	useJimmy(params.getParameter<bool>("useJimmy")),
	doMPInteraction(params.getParameter<bool>("doMPInteraction")),
	numTrials(params.getUntrackedParameter<int>("numTrialsMPI", 100))
{
	runInfo().setExternalXSecLO(
		params.getUntrackedParameter<double>("crossSection", -1.0));
	runInfo().setFilterEfficiency(
		params.getUntrackedParameter<double>("filterEfficiency", -1.0));
}

PomwigHadronizer::~PomwigHadronizer()
{
	clear();
}

void PomwigHadronizer::clear()
{
	if (!needClear)
		return;

	// teminate elementary process
	call(hwefin);
	if (useJimmy)
		call(jmefin);

	needClear = false;
}

bool PomwigHadronizer::initializeForExternalPartons()
{
   return false;
}

bool PomwigHadronizer::initializeForInternalPartons()
{
   clear();

   std::ostringstream header_str;

   header_str << "----------------------------------------------\n";
   header_str << "Initializing PomwigHadronizer\n";
   header_str << "----------------------------------------------\n";

   // Call hwudat to set up HERWIG block data
   hwudat();
  
   // Setting basic parameters ...
   hwproc.PBEAM1 = comEnergy/2.;
   hwproc.PBEAM2 = comEnergy/2.;
   // Choose beam particles for POMWIG depending on topology
   switch (diffTopology){
        case 0: //DPE
                hwbmch.PART1[0]  = 'E';
                hwbmch.PART1[1]  = '-';
                hwbmch.PART2[0]  = 'E';
                hwbmch.PART2[1]  = '-';
                break;
        case 1: //SD survive PART1
                hwbmch.PART1[0]  = 'E';
                hwbmch.PART1[1]  = '-';
                hwbmch.PART2[0]  = 'P';
                hwbmch.PART2[1]  = ' ';
                break;
        case 2: //SD survive PART2
                hwbmch.PART1[0]  = 'P';
                hwbmch.PART1[1]  = ' ';
                hwbmch.PART2[0]  = 'E';
                hwbmch.PART2[1]  = '-';
                break;
        case 3: //Non diffractive
                hwbmch.PART1[0]  = 'P';
                hwbmch.PART1[1]  = ' ';
                hwbmch.PART2[0]  = 'P';
                hwbmch.PART2[1]  = ' ';
                break;
        default:
                throw edm::Exception(edm::errors::Configuration,"PomwigError")
          <<" Invalid Diff. Topology. Must be DPE(diffTopology = 0), SD particle 1 (diffTopology = 1), SD particle 2 (diffTopology = 2) and Non diffractive (diffTopology = 3)";
                break;
   }
   for(int i=2;i<8;++i){
    hwbmch.PART1[i]  = ' ';
    hwbmch.PART2[i]  = ' ';}

   // initialize other common blocks ...
   call(hwigin);

   hwevnt.MAXER = 100000000;    // O(inf)
   hwpram.LWSUD = 0;            // don't write Sudakov form factors
   hwdspn.LWDEC = 0;            // don't write three/four body decays
                                // (no fort.77 and fort.88 ...)a

   std::memset(hwprch.AUTPDF, ' ', sizeof hwprch.AUTPDF);
   for(unsigned int i = 0; i < 2; i++) {
        hwpram.MODPDF[i] = -111;
        std::memcpy(hwprch.AUTPDF[i], "HWLHAPDF", 8);
   }

   hwevnt.MAXPR = maxEventsToPrint;
   hwpram.IPRINT = herwigVerbosity;

   if (printCards) {
                header_str << "\n";
                header_str << "------------------------------------\n";
                header_str << "Reading HERWIG parameters\n";
                header_str << "------------------------------------\n";
    
   }
   for(gen::ParameterCollector::const_iterator line = parameters.begin();
                                               line != parameters.end(); ++line) {
      if (printCards)
          header_str << "   " << *line << "\n";
      if (!give(*line))
          throw edm::Exception(edm::errors::Configuration)
                << "Herwig 6 did not accept the following: \""
                << *line << "\"." << std::endl;
   }

   if (printCards) header_str << "\n";

   needClear = true;
 
   call(hwuinc);

   hwusta("PI0     ",1);
 
   // Initialize H1 pomeron/reggeon
   if(diffTopology != 3){
        int nstru = hwpram.NSTRU;
        int ifit = h1fit;
        if(nstru == 9){
                if((ifit <= 0)||(ifit >= 7)){
                        throw edm::Exception(edm::errors::Configuration,"PomwigError")
                        <<" Attempted to set non existant H1 1997 fit index. Has to be 1...6";
                }
                header_str << "   H1 1997 pomeron pdf's" << "\n";
                header_str << "   IFIT = "<< ifit << "\n";
                double xp = 0.1;
                double Q2 = 75.0;
                double xpq[13];
                qcd_1994(xp,Q2,xpq,ifit);
        } else if(nstru == 10){
                if((ifit <= 0)||(ifit >= 7)){
                        throw edm::Exception(edm::errors::Configuration,"PomwigError")
                        <<" Attempted to set non existant H1 1997 fit index. Has to be 1...6";
                }
                header_str << "   H1 1997 reggeon pdf's" << "\n";
                header_str << "   IFIT = "<< ifit << "\n";
                double xp = 0.1;
                double Q2 = 75.0;
                double xpq[13];
                qcd_1994(xp,Q2,xpq,ifit);

        } else if(nstru == 12){
                /*if(ifit != 1){
                        throw edm::Exception(edm::errors::Configuration,"PomwigError")
                        <<" Attempted to set non existant H1 2006 A fit index. Only IFIT=1";
                }*/
                ifit = 1;
                header_str << "   H1 2006 A pomeron pdf's" << "\n";
                header_str << "   IFIT = "<< ifit << "\n";
                double xp = 0.1;
                double Q2 = 75.0;
                double xpq[13];
                double f2[2];
                double fl[2];
                double c2[2];
                double cl[2];
                qcd_2006(xp,Q2,ifit,xpq,f2,fl,c2,cl);
        } else if(nstru == 13){
                /*if(ifit != 1){
                        throw edm::Exception(edm::errors::Configuration,"PomwigError")
                        <<" Attempted to set non existant H1 2006 A fit index. Only IFIT=1";
                }*/
                ifit = 1;
                header_str << "   H1 2006 A reggeon pdf's" << "\n";
                header_str << "   IFIT = "<< ifit << "\n";
                double xp = 0.1;
                double Q2 = 75.0;
                double xpq[13];
                double f2[2];
                double fl[2];
                double c2[2];
                double cl[2];
                qcd_2006(xp,Q2,ifit,xpq,f2,fl,c2,cl);
        } else if(nstru == 14){
                /*if(ifit != 2){
                        throw edm::Exception(edm::errors::Configuration,"PomwigError")
                        <<" Attempted to set non existant H1 2006 B fit index. Only IFIT=2";
                }*/
                ifit = 2;
                header_str << "   H1 2006 B pomeron pdf's" << "\n";
                header_str << "   IFIT = "<< ifit << "\n";
                double xp = 0.1;
                double Q2 = 75.0;
                double xpq[13];
                double f2[2];
                double fl[2];
                double c2[2];
                double cl[2];
                qcd_2006(xp,Q2,ifit,xpq,f2,fl,c2,cl);
        } else if(nstru == 15){
                /*if(ifit != 2){
                        throw edm::Exception(edm::errors::Configuration,"PomwigError")
                        <<" Attempted to set non existant H1 2006 B fit index. Only IFIT=2";
                }*/
                ifit = 2;
                header_str << "   H1 2006 B reggeon pdf's" << "\n";
                header_str << "   IFIT = "<< ifit << "\n";
                double xp = 0.1;
                double Q2 = 75.0;
                double xpq[13];
                double f2[2];
                double fl[2];
                double c2[2];
                double cl[2];
                qcd_2006(xp,Q2,ifit,xpq,f2,fl,c2,cl);
        } else{
                throw edm::Exception(edm::errors::Configuration,"PomwigError")
                <<" Only running Pomeron H1 1997 (NSTRU=9), H1 2006 fit A (NSTRU=12) and H1 2006 fit B (NSTRU=14) or Reggeon H1 1997 (NSTRU=10), H1 2006 fit A (NSTRU=13) and H1 2006 fit B (NSTRU=15)";
        }
   }

   call(hweini);

   edm::LogInfo("") << header_str.str();

   return true;
}

bool PomwigHadronizer::declareStableParticles(const std::vector<int> &pdgIds)
{
	for(std::vector<int>::const_iterator iter = pdgIds.begin();
	    iter != pdgIds.end(); ++iter)
		if (!markStable(*iter))
			return false;
	return true;
}

void PomwigHadronizer::statistics()
{
	double RNWGT = 1. / hwevnt.NWGTS;
	double AVWGT = hwevnt.WGTSUM * RNWGT;

	double xsec = 1.0e3 * AVWGT;
        xsec = survivalProbability*xsec;

	runInfo().setInternalXSec(xsec);
}

bool PomwigHadronizer::hadronize()
{
   return false;
}

bool PomwigHadronizer::generatePartonsAndHadronize()
{
	// hard process generation, parton shower, hadron formation

	InstanceWrapper wrapper(this);	// safe guard

	event().reset();

	int counter = 0;
	while(counter++ < numTrials) {
		// call herwig routines to create HEPEVT

		hwuine();	// initialize event

		if (callWithTimeout(10, hwepro)) { // process event and PS
			// We hung for more than 10 seconds
			int error = 199;
			hwwarn_("HWHGUP", &error);
		}

		hwbgen();	// parton cascades

		// call jimmy ... only if event is not killed yet by HERWIG
		if (useJimmy && doMPInteraction && !hwevnt.IERROR &&
		    call_hwmsct())
				continue;

		hwdhob();	// heavy quark decays
		hwcfor();	// cluster formation
		hwcdec();	// cluster decays

		// if event was not killed by HERWIG, break out of retry loop
		if (!hwevnt.IERROR)
			break;

		hwufne();	// finalize event
	}

	if (counter >= numTrials) {
		edm::LogWarning("Generator|PomwigHadronizer")
			<< "JIMMY could not produce MI in "
			<< numTrials << " trials." << std::endl
			<< "Event will be skipped to prevent"
			<< " from deadlock." << std::endl;

		return false;
	}

	return true;
}

void PomwigHadronizer::finalizeEvent()
{
	lhef::LHEEvent::fixHepMCEventTimeOrdering(event().get());

	event()->set_signal_process_id(hwproc.IPROC);
}

bool PomwigHadronizer::decay()
{
	// hadron decays

	InstanceWrapper wrapper(this);	// safe guard

	hwdhad();	// unstable particle decays
	hwdhvy();	// heavy flavour decays
	hwmevt();	// soft underlying event

	hwufne();	// finalize event

	if (hwevnt.IERROR)
		return false;

	event().reset(new HepMC::GenEvent);
	if (!conv.fill_next_event(event().get()))
		throw cms::Exception("PomwigError")
			<< "HepMC Conversion problems in event." << std::endl;

	return true;
}

bool PomwigHadronizer::residualDecay()
{
	return true;
}

}//namespace gen

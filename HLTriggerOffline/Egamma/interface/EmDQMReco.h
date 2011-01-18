#ifndef HLTriggerOffline_Egamma_EmDQMReco_H
#define HLTriggerOffline_Egamma_EmDQMReco_H


// Base Class Headers
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HLTReco/interface/TriggerEventWithRefs.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "DataFormats/Common/interface/RefToBase.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include <vector>
#include "TDirectory.h"
#include "HepMC/GenParticle.h"
#include "CommonTools/Utils/interface/PtComparator.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"

#include <boost/ptr_container/ptr_vector.hpp>

class EmDQMReco : public edm::EDAnalyzer{

  //----------------------------------------

  /** a class managing a set of MonitorElements for quantities of a fourvector
   *  we want to histogram.
   */
  class FourVectorMonitorElements
  {

  public:
    /** @param histogramNameTemplate should be a format string (like used in printf(..)
     *  or boost::format(..) for the histogram NAME where the first %s is replaced with et,eta or phi.
     *
     *  @param histogramTitleTemplate should be a format string (see histogramNameTemplate)
     *   for the histogram TITLE where the first %s is replaced with et,eta or phi.
     */
    FourVectorMonitorElements(EmDQMReco *_parent,
        const std::string &histogramNameTemplate,
        const std::string &histogramTitleTemplate);

    void fill(const math::XYZTLorentzVector &momentum);

  private:
    /** for accessing the histogramming parameters */
    EmDQMReco *parent;

    /** DQM objects (histograms) for the variables to be plotted */
    MonitorElement* etMonitorElement;
    MonitorElement* etaMonitorElement;
    MonitorElement* phiMonitorElement;

  };
  //----------------------------------------

public:


  /// Constructor
  explicit EmDQMReco(const edm::ParameterSet& pset);

  /// Destructor
  ~EmDQMReco();

  // Operations

  void analyze(const edm::Event & event, const edm::EventSetup&);
  void beginJob();
  void endJob();
  void beginRun( const edm::Run&, const edm::EventSetup& );

private:
  // Input from cfg file

  /** the HLT collections to be looked at */
  std::vector<edm::InputTag> theHLTCollectionLabels;

  unsigned int numOfHLTCollectionLabels;  // Will be size of above vector

  bool useHumanReadableHistTitles;
  std::vector<std::string> theHLTCollectionHumanNames; // Human-readable names for the collections
  edm::InputTag theL1Seed;
  std::vector<int> theHLTOutputTypes;
  std::vector<bool> plotiso;
  std::vector<std::vector<edm::InputTag> > isoNames; // there has to be a better solution
  std::vector<std::pair<double,double> > plotBounds;
  std::string theHltName;
  HLTConfigProvider hltConfig_;
  bool isHltConfigInitialized_;

  ////////////////////////////////////////////////////////////
  //          Read from configuration file                  //
  ////////////////////////////////////////////////////////////
  // parameters for generator study
  unsigned int reqNum;
  int   pdgGen;
  double recoEtaAcc;
  double recoEtAcc;
  // plotting paramters
  double plotEtaMax;
  double plotPtMin ;
  double plotPtMax ;
  double plotPhiMax;

  /** number of bins to use for ALL plots (?) */
  unsigned int plotBins ;

  // preselction cuts
  edm::InputTag recocutCollection_;
  unsigned int recocut_;

  /** events which fire this trigger are filled into {et,eta,phi}recomonpath
   */
  std::string triggerNameRecoMonPath;

  /** process name for the trigger results for events to be filled
   *  into {et,eta,phi}recomonpath
   */
  std::string processNameRecoMonPath;

  /** input tag for the reconstructed electron collection
   *  (with respect to which the HLT efficiencies are calculated ?)
   */
  edm::InputTag recoElectronsInputTag;

  ////////////////////////////////////////////////////////////
  //          Create Histograms                             //
  ////////////////////////////////////////////////////////////
  /** \label Et, eta and phi distributions (RECO) for the different
   *  HLT modules to be looked at. */
  /** @{ */
// std::vector<MonitorElement*> etahist;
//  std::vector<MonitorElement*> ethist;
//  std::vector<MonitorElement*> phiHist;

  boost::ptr_vector<FourVectorMonitorElements> standardHist;

//  std::vector<MonitorElement*> etahistmatchreco;
//  std::vector<MonitorElement*> ethistmatchreco;
//  std::vector<MonitorElement*> phiHistMatchReco;
  boost::ptr_vector<FourVectorMonitorElements> histMatchReco;

//  std::vector<MonitorElement*> etahistmatchrecomonpath;
//  std::vector<MonitorElement*> ethistmatchrecomonpath;
//  std::vector<MonitorElement*> phiHistMatchRecoMonPath;
  boost::ptr_vector<FourVectorMonitorElements> histMatchRecoMonPath;


//  std::vector<MonitorElement*> histEtOfHltObjMatchToReco;
//  std::vector<MonitorElement*> histEtaOfHltObjMatchToReco;
//  std::vector<MonitorElement*> histPhiOfHltObjMatchToReco;
  boost::ptr_vector<FourVectorMonitorElements> histHltObjMatchToReco;

  /** @} */

  /** \label Isolation distributions */
  /** @{ */
  std::vector<MonitorElement*> etahistiso;
  std::vector<MonitorElement*> ethistiso;
  std::vector<MonitorElement*> phiHistIso;

  std::vector<MonitorElement*> etahistisomatchreco;
  std::vector<MonitorElement*> ethistisomatchreco;
  std::vector<MonitorElement*> phiHistIsoMatchReco;

  std::vector<MonitorElement*> histEtIsoOfHltObjMatchToReco;
  std::vector<MonitorElement*> histEtaIsoOfHltObjMatchToReco;
  std::vector<MonitorElement*> histPhiIsoOfHltObjMatchToReco;
  /** @} */

  /** Plots of efficiency per step (note that these are NOT
   *  filled with four vector quantities but rather event counts) */
  MonitorElement* totalreco;
  MonitorElement* totalmatchreco;

  /** \name reco histograms */
  /** @{ */
//  MonitorElement* etreco;
//  MonitorElement* etareco;
//  MonitorElement* phiReco;
  boost::scoped_ptr<FourVectorMonitorElements> histReco;

  //  MonitorElement* etrecomonpath;
  //  MonitorElement* etarecomonpath;
  //  MonitorElement* phiRecoMonPath;
  boost::scoped_ptr<FourVectorMonitorElements> histRecoMonpath;

  //  MonitorElement* etahistmonpath;
  //  MonitorElement* ethistmonpath;
  //  MonitorElement* phiHistMonPath;
  boost::scoped_ptr<FourVectorMonitorElements> histMonpath;
  /** @} */

  int eventnum;
  // int prescale;

  // interface to DQM framework
  DQMStore * dbe;
  std::string dirname_;

  template <class T> void fillHistos(edm::Handle<trigger::TriggerEventWithRefs>&,const edm::Event& ,unsigned int, std::vector<reco::Particle>&, bool, bool);
  GreaterByPt<reco::Particle> pTComparator_;
  GreaterByPt<reco::GsfElectron> pTRecoComparator_;


  //----------------------------------------
};
#endif

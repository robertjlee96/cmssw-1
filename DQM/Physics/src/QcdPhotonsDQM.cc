/*
 *  See header file for a description of this class.
 *
 *  $Date: 2012/01/11 13:53:29 $
 *  $Revision: 1.29 $
 *  \author Michael B. Anderson, University of Wisconsin Madison
 */

#include "DQM/Physics/src/QcdPhotonsDQM.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Common/interface/TriggerNames.h"

#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

// Physics Objects
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"
#include "DataFormats/JetReco/interface/Jet.h"

// Vertex
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"

// For removing ECAL Spikes
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterLazyTools.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgo.h"
#include "RecoLocalCalo/EcalRecAlgos/interface/EcalSeverityLevelAlgoRcd.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "CondFormats/EcalObjects/interface/EcalCondObjectContainer.h"

//geometry
//#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
//#include "Geometry/Records/interface/IdealGeometryRecord.h"
//#include "Geometry/Records/interface/CaloGeometryRecord.h"
//#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
//#include "Geometry/CaloGeometry/interface/CaloCellGeometry.h"
//#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
//#include "Geometry/EcalAlgo/interface/EcalPreshowerGeometry.h"

// Math stuff
#include "DataFormats/Math/interface/deltaR.h"
#include "DataFormats/Math/interface/deltaPhi.h"

#include <vector>

#include <string>
#include <cmath>
using namespace std;
using namespace edm;
using namespace reco;


QcdPhotonsDQM::QcdPhotonsDQM(const ParameterSet& parameters) {
  // Get parameters from configuration file
  theTriggerPathToPass_        = parameters.getParameter<string>("triggerPathToPass");
  thePlotTheseTriggersToo_     = parameters.getParameter<vector<string> >("plotTheseTriggersToo");
  trigTag_                    = parameters.getUntrackedParameter<edm::InputTag>("trigTag");
  thePhotonCollectionLabel_    = parameters.getParameter<InputTag>("photonCollection");
  theJetCollectionLabel_       = parameters.getParameter<InputTag>("jetCollection");
  theVertexCollectionLabel_    = parameters.getParameter<InputTag>("vertexCollection");
  theMinJetPt_             = parameters.getParameter<double>("minJetPt");
  theMinPhotonEt_              = parameters.getParameter<double>("minPhotonEt");
  theRequirePhotonFound_       = parameters.getParameter<bool>("requirePhotonFound");
  thePlotPhotonMaxEt_          = parameters.getParameter<double>("plotPhotonMaxEt");
  thePlotPhotonMaxEta_         = parameters.getParameter<double>("plotPhotonMaxEta");
  thePlotJetMaxEta_            = parameters.getParameter<double>("plotJetMaxEta");
  // just to initialize
  isValidHltConfig_ = false;

  // coverity says...
  h_deltaEt_photon_jet = 0;
  h_deltaPhi_jet_jet2 = 0;
  h_deltaPhi_photon_jet = 0;
  h_deltaPhi_photon_jet2 = 0;
  h_deltaR_jet_jet2 = 0;
  h_deltaR_photon_jet2 = 0;
  h_jet2_eta = 0;
  h_jet2_pt = 0;
  h_jet2_ptOverPhotonEt = 0;
  h_jet_count = 0;
  h_jet_eta = 0;
  h_jet_pt = 0;
  h_photon_count_bar = 0;
  h_photon_count_end = 0;
  h_photon_et = 0;
  h_photon_et_beforeCuts = 0;
  h_photon_et_jetco = 0;
  h_photon_et_jetcs = 0;
  h_photon_et_jetfo = 0;
  h_photon_et_jetfs = 0;
  h_photon_et_ratio_co_cs = 0;
  h_photon_et_ratio_co_fo = 0;
  h_photon_et_ratio_co_fs = 0;
  h_photon_et_ratio_cs_fo = 0;
  h_photon_et_ratio_cs_fs = 0;
  h_photon_et_ratio_fo_fs = 0;
  h_photon_eta = 0;
  h_triggers_passed = 0;

  theDbe = Service<DQMStore>().operator->();
  
}

QcdPhotonsDQM::~QcdPhotonsDQM() { 
}


void QcdPhotonsDQM::beginJob() {
 
  logTraceName = "QcdPhotonAnalyzer";

  LogTrace(logTraceName)<<"Parameters initialization";
 
  theDbe->setCurrentFolder("Physics/QcdPhotons");  // Use folder with name of PAG

  std::stringstream aStringStream;
  std::string aString;
  aStringStream << theMinJetPt_;
  aString = aStringStream.str();

  // Monitor of triggers passed
  int numOfTriggersToMonitor = thePlotTheseTriggersToo_.size();
  h_triggers_passed = theDbe->book1D("triggers_passed", "Events passing these trigger paths", numOfTriggersToMonitor, 0, numOfTriggersToMonitor);
  for (int i=0; i<numOfTriggersToMonitor; i++) {
    h_triggers_passed->setBinLabel(i+1,thePlotTheseTriggersToo_[i]);
  }

  // Keep the number of plots and number of bins to a minimum!
  h_photon_et_beforeCuts           = theDbe->book1D("photon_et_beforeCuts",       "#gamma with highest E_{T};E_{T}(#gamma) (GeV)", 20, 0., thePlotPhotonMaxEt_);
  h_photon_et           = theDbe->book1D("photon_et",       "#gamma with highest E_{T};E_{T}(#gamma) (GeV)", 20, 0., thePlotPhotonMaxEt_);
  h_photon_eta          = theDbe->book1D("photon_eta",      "#gamma with highest E_{T};#eta(#gamma)", 40, -thePlotPhotonMaxEta_, thePlotPhotonMaxEta_);
  h_photon_count_bar    = theDbe->book1D("photon_count_bar","Number of #gamma's passing selection (Barrel);Number of #gamma's", 8, -0.5, 7.5);
  h_photon_count_end    = theDbe->book1D("photon_count_end","Number of #gamma's passing selection (Endcap);Number of #gamma's", 8, -0.5, 7.5);

  h_jet_pt             = theDbe->book1D("jet_pt",   "Jet with highest p_{T} (from "+theJetCollectionLabel_.label()+");p_{T}(1^{st} jet) (GeV)",    20, 0., thePlotPhotonMaxEt_);
  h_jet_eta             = theDbe->book1D("jet_eta", "Jet with highest p_{T} (from "+theJetCollectionLabel_.label()+");#eta(1^{st} jet)", 20, -thePlotJetMaxEta_, thePlotJetMaxEta_);
  h_deltaPhi_photon_jet = theDbe->book1D("deltaPhi_photon_jet", "#Delta#phi between Highest E_{T} #gamma and jet;#Delta#phi(#gamma,1^{st} jet)", 20, 0, 3.1415926);
  h_deltaPhi_jet_jet2   = theDbe->book1D("deltaPhi_jet_jet2", "#Delta#phi between Highest E_{T} jet and 2^{nd} jet;#Delta#phi(1^{st} jet,2^{nd} jet)", 20, 0, 3.1415926);
  h_deltaEt_photon_jet  = theDbe->book1D("deltaEt_photon_jet",  "(E_{T}(#gamma)-p_{T}(jet))/E_{T}(#gamma) when #Delta#phi(#gamma,1^{st} jet) > 2.8;#DeltaE_{T}(#gamma,1^{st} jet)/E_{T}(#gamma)", 20, -1.0, 1.0);
  h_jet_count           = theDbe->book1D("jet_count", "Number of "+theJetCollectionLabel_.label()+" (p_{T} > "+aString+" GeV);Number of Jets", 8, -0.5, 7.5);
  h_jet2_pt             = theDbe->book1D("jet2_pt",   "Jet with 2^{nd} highest p_{T} (from "+theJetCollectionLabel_.label()+");p_{T}(2^{nd} jet) (GeV)",    20, 0., thePlotPhotonMaxEt_);
  h_jet2_eta            = theDbe->book1D("jet2_eta", "Jet with 2^{nd} highest p_{T} (from "+theJetCollectionLabel_.label()+");#eta(2^{nd} jet)", 20, -thePlotJetMaxEta_, thePlotJetMaxEta_);
  h_jet2_ptOverPhotonEt = theDbe->book1D("jet2_ptOverPhotonEt", "p_{T}(2^{nd} highest jet) / E_{T}(#gamma);p_{T}(2^{nd} Jet)/E_{T}(#gamma)", 20, 0.0, 4.0);
  h_deltaPhi_photon_jet2 = theDbe->book1D("deltaPhi_photon_jet2","#Delta#phi between Highest E_{T} #gamma and 2^{nd} highest jet;#Delta#phi(#gamma,2^{nd} jet)", 20, 0, 3.1415926);
  h_deltaR_jet_jet2      = theDbe->book1D("deltaR_jet_jet2", "#DeltaR between Highest Jet and 2^{nd} Highest;#DeltaR(1^{st} jet,2^{nd} jet)", 30, 0, 6.0);
  h_deltaR_photon_jet2   = theDbe->book1D("deltaR_photon_jet2", "#DeltaR between Highest E_{T} #gamma and 2^{nd} jet;#DeltaR(#gamma, 2^{nd} jet)", 30, 0, 6.0);

  // Photon Et for different jet configurations
  Float_t bins_et[] = {15,20,30,50,80};
  int num_bins_et = 4;
  h_photon_et_jetcs = theDbe->book1D("photon_et_jetcs", "#gamma with highest E_{T} (#eta(jet)<1.45, #eta(#gamma)#eta(jet)>0);E_{T}(#gamma) (GeV)", num_bins_et, bins_et);
  h_photon_et_jetco = theDbe->book1D("photon_et_jetco", "#gamma with highest E_{T} (#eta(jet)<1.45, #eta(#gamma)#eta(jet)<0);E_{T}(#gamma) (GeV)", num_bins_et, bins_et);
  h_photon_et_jetfs = theDbe->book1D("photon_et_jetfs", "#gamma with highest E_{T} (1.55<#eta(jet)<2.5, #eta(#gamma)#eta(jet)>0);E_{T}(#gamma) (GeV)", num_bins_et, bins_et);
  h_photon_et_jetfo = theDbe->book1D("photon_et_jetfo", "#gamma with highest E_{T} (1.55<#eta(jet)<2.5, #eta(#gamma)#eta(jet)<0);E_{T}(#gamma) (GeV)", num_bins_et, bins_et);
  h_photon_et_jetcs->getTH1F()->Sumw2();
  h_photon_et_jetco->getTH1F()->Sumw2();
  h_photon_et_jetfs->getTH1F()->Sumw2();
  h_photon_et_jetfo->getTH1F()->Sumw2();
  // Ratio of the above Photon Et distributions
  h_photon_et_ratio_co_cs = theDbe->book1D("photon_et_ratio_00_co_cs", "D(|#eta(jet)|<1.45, #eta(jet)*#eta(#gamma)<0) / D(|#eta(jet)|<1.45, #eta(jet)*#eta(#gamma)>0);E_{T}(#gamma) (GeV); ratio", num_bins_et, bins_et);
  h_photon_et_ratio_fo_fs = theDbe->book1D("photon_et_ratio_01_fo_fs", "D(1.55<|#eta(jet)|<2.6, #eta(jet)*#eta(#gamma)<0) / D(1.55<|#eta(jet)|<2.6, #eta(jet)*#eta(#gamma)>0);E_{T}(#gamma) (GeV); ratio", num_bins_et, bins_et);
  h_photon_et_ratio_cs_fs = theDbe->book1D("photon_et_ratio_02_cs_fs", "D(|#eta(jet)|<1.45, #eta(jet)*#eta(#gamma)>0) / D(1.55<|#eta(jet)|<2.6, #eta(jet)*#eta(#gamma)>0);E_{T}(#gamma) (GeV); ratio", num_bins_et, bins_et);
  h_photon_et_ratio_co_fs = theDbe->book1D("photon_et_ratio_03_co_fs", "D(|#eta(jet)|<1.45, #eta(jet)*#eta(#gamma)<0) / D(1.55<|#eta(jet)|<2.6, #eta(jet)*#eta(#gamma)>0);E_{T}(#gamma) (GeV); ratio", num_bins_et, bins_et);
  h_photon_et_ratio_cs_fo = theDbe->book1D("photon_et_ratio_04_cs_fo", "D(|#eta(jet)|<1.45, #eta(jet)*#eta(#gamma)>0) / D(1.55<|#eta(jet)|<2.6, #eta(jet)*#eta(#gamma)<0);E_{T}(#gamma) (GeV); ratio", num_bins_et, bins_et);
  h_photon_et_ratio_co_fo = theDbe->book1D("photon_et_ratio_05_co_fo", "D(|#eta(jet)|<1.45, #eta(jet)*#eta(#gamma)<0) / D(1.55<|#eta(jet)|<2.6, #eta(jet)*#eta(#gamma)<0);E_{T}(#gamma) (GeV); ratio", num_bins_et, bins_et);
  h_photon_et_ratio_co_cs->getTH1F()->Sumw2();
  h_photon_et_ratio_fo_fs->getTH1F()->Sumw2();
  h_photon_et_ratio_cs_fs->getTH1F()->Sumw2();
  h_photon_et_ratio_co_fs->getTH1F()->Sumw2();
  h_photon_et_ratio_cs_fo->getTH1F()->Sumw2();
  h_photon_et_ratio_co_fo->getTH1F()->Sumw2();
}


///
///
///
void QcdPhotonsDQM::beginRun( const edm::Run &iRun, const edm::EventSetup &iSet ) {

  // passed as parameter to HLTConfigProvider::init(), not yet used
  bool isConfigChanged = false;

  // isValidHltConfig_ could be used to short-circuit analyze() in case of problems
  isValidHltConfig_ = hltConfigProvider_.init( iRun, iSet, "HLT", isConfigChanged );

  num_events_in_run = 0;
}


void QcdPhotonsDQM::analyze(const Event& iEvent, const EventSetup& iSetup) {
  num_events_in_run++;

  //// short-circuit if hlt problems
  //if( ! isValidHltConfig_ ) return;
  
  LogTrace(logTraceName)<<"Analysis of event # ";

  ////////////////////////////////////////////////////////////////////
  // Did event pass HLT paths?
  Handle<TriggerResults> HLTresults;
  iEvent.getByLabel(trigTag_, HLTresults); 
  if (!HLTresults.isValid()) {
    //LogWarning("") << ">>> TRIGGER collection does not exist !!!";
    return;
  }
  const edm::TriggerNames & trigNames = iEvent.triggerNames(*HLTresults);

  bool passed_HLT=false;

  // See if event passed trigger paths
  //  increment that bin in the trigger plot
  for (unsigned int i=0; i<thePlotTheseTriggersToo_.size(); i++) {
    passed_HLT = false;
    for (unsigned int ti=0; (ti<trigNames.size()) && !passed_HLT; ++ti) {
      size_t pos = trigNames.triggerName(ti).find(thePlotTheseTriggersToo_[i]);
      if (pos==0) passed_HLT = HLTresults->accept(ti);
    }
    if (passed_HLT) h_triggers_passed->Fill(i);
  }


   // grab photons
  Handle<PhotonCollection> photonCollection;
  iEvent.getByLabel(thePhotonCollectionLabel_, photonCollection);

  // If photon collection is empty, exit
  if (!photonCollection.isValid()) return;


  // Quit if the event did not pass the HLT path we care about
  passed_HLT = false;
  {
    //bool found=false;
  for (unsigned int ti=0; ti<trigNames.size(); ++ti) {
    size_t pos = trigNames.triggerName(ti).find(theTriggerPathToPass_);
    if (pos==0) {
      passed_HLT = HLTresults->accept(ti);
      //found=true;
      break;
    }
  }

  // Assumption: reco photons are ordered by Et
  for (PhotonCollection::const_iterator recoPhoton = photonCollection->begin(); recoPhoton!=photonCollection->end(); recoPhoton++){
    // stop looping over photons once we get to too low Et
    if ( recoPhoton->et() < theMinPhotonEt_ ) break;

    h_photon_et_beforeCuts->Fill(recoPhoton->et());
    break; // leading photon only
  }

  if (!passed_HLT) {
    return;
  }
  }
  
  ////////////////////////////////////////////////////////////////////

  //std::cout << "\tpassed main trigger (" << theTriggerPathToPass_ << ")" << std::endl;

  ////////////////////////////////////////////////////////////////////
  // Does event have valid vertex?
  // Get the primary event vertex
  Handle<VertexCollection> vertexHandle;
  iEvent.getByLabel(theVertexCollectionLabel_, vertexHandle);
  VertexCollection vertexCollection = *(vertexHandle.product());
  //double vtx_ndof = -1.0;
  //double vtx_z    = 0.0;
  //bool   vtx_isFake = true;
  //if (vertexCollection.size()>0) {
  //  vtx_ndof = vertexCollection.begin()->ndof();
  //  vtx_z    = vertexCollection.begin()->z();
  //  vtx_isFake = false;
  //}
  //if (vtx_isFake || fabs(vtx_z)>15 || vtx_ndof<4) return;
  
  int nvvertex = 0;
  for (unsigned int i=0; i<vertexCollection.size(); ++i) {
    if (vertexCollection[i].isValid()) nvvertex++;
  }
  if (nvvertex==0) return;

  ////////////////////////////////////////////////////////////////////

  //std::cout << "\tpassed vertex selection" << std::endl;


  ////////////////////////////////////////////////////////////////////
  // Did the event pass certain L1 Technical Trigger bits?
  // It's probably beam halo
  //  TODO: ADD code
  ////////////////////////////////////////////////////////////////////


  // For finding spikes
  Handle<EcalRecHitCollection> EBReducedRecHits;
  iEvent.getByLabel("reducedEcalRecHitsEB", EBReducedRecHits);
  Handle<EcalRecHitCollection> EEReducedRecHits;
  iEvent.getByLabel("reducedEcalRecHitsEE", EEReducedRecHits); 
  EcalClusterLazyTools lazyTool(iEvent, iSetup, InputTag("reducedEcalRecHitsEB"), InputTag("reducedEcalRecHitsEE") );
  

  // Find the highest et "decent" photon
  float photon_et  = -9.0;
  float photon_eta = -9.0;
  float photon_phi = -9.0;
  bool  photon_passPhotonID = false;
  bool  found_lead_pho = false;
  int   photon_count_bar = 0;
  int   photon_count_end = 0;
  // Assumption: reco photons are ordered by Et
  for (PhotonCollection::const_iterator recoPhoton = photonCollection->begin(); recoPhoton!=photonCollection->end(); recoPhoton++){

    // stop looping over photons once we get to too low Et
    if ( recoPhoton->et() < theMinPhotonEt_ ) break;

    //  Ignore ECAL Spikes
    const reco::CaloClusterPtr  seed = recoPhoton->superCluster()->seed();
    DetId id = lazyTool.getMaximum(*seed).first; // Cluster shape variables
    //    float time  = -999., outOfTimeChi2 = -999., chi2 = -999.;  // UNUSED
    int   flags=-1, severity = -1; 
    const EcalRecHitCollection & rechits = ( recoPhoton->isEB() ? *EBReducedRecHits : *EEReducedRecHits); 
    EcalRecHitCollection::const_iterator it = rechits.find( id );
    if( it != rechits.end() ) {
      //      time = it->time(); // UNUSED
      //      outOfTimeChi2 = it->outOfTimeChi2(); // UNUSED
      //      chi2 = it->chi2(); // UNUSED
      flags = it->recoFlag();

      edm::ESHandle<EcalSeverityLevelAlgo> sevlv;
      iSetup.get<EcalSeverityLevelAlgoRcd>().get(sevlv);
      severity = sevlv->severityLevel( id, rechits);
    }
    bool isNotSpike = ((recoPhoton->isEB() && (severity!=3 && severity!=4 ) && (flags != 2) ) || recoPhoton->isEE());
    if (!isNotSpike) continue;  // move on to next photon
    // END of determining ECAL Spikes

    bool pho_current_passPhotonID = false;
    bool pho_current_isEB = recoPhoton->isEB();
    bool pho_current_isEE = recoPhoton->isEE();

    if ( pho_current_isEB && (recoPhoton->sigmaIetaIeta() < 0.01 || recoPhoton->hadronicOverEm() < 0.05) ) {
      // Photon object in barrel passes photon ID
      pho_current_passPhotonID = true;
      photon_count_bar++;
    } else if ( pho_current_isEE && (recoPhoton->hadronicOverEm() < 0.05) ) {
      // Photon object in endcap passes photon ID
      pho_current_passPhotonID = true;
      photon_count_end++;
    }

    if (!found_lead_pho) {
      found_lead_pho = true;
      photon_passPhotonID = pho_current_passPhotonID;
      photon_et  = recoPhoton->et();
      photon_eta = recoPhoton->eta();
      photon_phi = recoPhoton->phi();
    }
  }
  
  // If user requires a photon to be found, but none is, return.
  //   theRequirePhotonFound should pretty much always be set to 'True'
  //    except when running on qcd monte carlo just to see the jets.
  if ( theRequirePhotonFound_ && (!photon_passPhotonID || photon_et<theMinPhotonEt_) ) return;


  ////////////////////////////////////////////////////////////////////
  // Find the highest et jet
  Handle<View<Jet> > jetCollection;
  iEvent.getByLabel (theJetCollectionLabel_,jetCollection);
  if (!jetCollection.isValid()) return;

  float jet_pt    = -8.0;
  float jet_eta   = -8.0;
  float jet_phi   = -8.0;
  int   jet_count = 0;
  float jet2_pt   = -9.0;
  float jet2_eta  = -9.0;
  float jet2_phi  = -9.0;
  // Assumption: jets are ordered by Et
  for (unsigned int i_jet = 0; i_jet < jetCollection->size(); i_jet++) {
    const Jet* jet = & jetCollection->at(i_jet);

    float jet_current_pt = jet->pt();

    // don't care about jets that overlap with the lead photon
    if ( deltaR(jet->eta(), jet->phi(), photon_eta, photon_phi) < 0.5 ) continue;
    // stop looping over jets once we get to too low Et
    if (jet_current_pt < theMinJetPt_) break;

    jet_count++;
    if (jet_current_pt > jet_pt) {
      jet2_pt  = jet_pt;  // 2nd highest jet get's et from current highest
      jet2_eta = jet_eta;
      jet2_phi = jet_phi;
      jet_pt   = jet_current_pt; // current highest jet gets et from the new highest
      jet_eta  = jet->eta();
      jet_phi  = jet->phi();
    } else if (jet_current_pt > jet2_pt) {
      jet2_pt  = jet_current_pt;
      jet2_eta = jet->eta();
      jet2_phi = jet->phi();
    }
  }
  ////////////////////////////////////////////////////////////////////


  ////////////////////////////////////////////////////////////////////
  // Fill histograms if a jet found
  // NOTE: if a photon was required to be found, but wasn't
  //        we wouldn't have made it to this point in the code
  if ( jet_pt > 0.0 ) {

    // Photon Plots
    h_photon_et       ->Fill( photon_et  );
    h_photon_eta      ->Fill( photon_eta );
    h_photon_count_bar->Fill( photon_count_bar );
    h_photon_count_end->Fill( photon_count_end );

    // Photon Et hists for different orientations to the jet
    if ( fabs(photon_eta)<1.45 && photon_passPhotonID ) {  // Lead photon is in barrel
      if (fabs(jet_eta)<1.45){                          //   jet is in barrel
	if (photon_eta*jet_eta>0) {
	  h_photon_et_jetcs->Fill(photon_et);
	} else {
	  h_photon_et_jetco->Fill(photon_et);
	}
      } else if (jet_eta>1.55 && jet_eta<2.5) {         // jet is in endcap
	if (photon_eta*jet_eta>0) {
	  h_photon_et_jetfs->Fill(photon_et);
	} else {
	  h_photon_et_jetfo->Fill(photon_et);
	}
      }
    } // END of Lead Photon is in Barrel

    // Jet Plots
    h_jet_pt       ->Fill( jet_pt     );
    h_jet_eta      ->Fill( jet_eta    );
    h_jet_count    ->Fill( jet_count  );
    h_deltaPhi_photon_jet   ->Fill( abs(deltaPhi(photon_phi, jet_phi)) );
    if ( abs(deltaPhi(photon_phi,jet_phi))>2.8 ) h_deltaEt_photon_jet->Fill( (photon_et-jet_pt)/photon_et );

    // 2nd Highest Jet Plots
    if ( jet2_pt  > 0.0 ) {
      h_jet2_pt             ->Fill( jet2_pt  );
      h_jet2_eta            ->Fill( jet2_eta );
      h_jet2_ptOverPhotonEt ->Fill( jet2_pt/photon_et );
      h_deltaPhi_photon_jet2->Fill( abs(deltaPhi(photon_phi, jet2_phi)) );
      h_deltaPhi_jet_jet2   ->Fill( abs(deltaPhi(   jet_phi, jet2_phi)) );
      h_deltaR_jet_jet2     ->Fill( deltaR(   jet_eta,    jet_phi, jet2_eta, jet2_phi) );
      h_deltaR_photon_jet2  ->Fill( deltaR(photon_eta, photon_phi, jet2_eta, jet2_phi) );
    }
  } 
  // End of Filling histograms
  ////////////////////////////////////////////////////////////////////
}


void QcdPhotonsDQM::endJob(void) {}

void QcdPhotonsDQM::endRun(const edm::Run& run, const edm::EventSetup& es) {
  if (num_events_in_run>0) { 
    h_triggers_passed->getTH1F()->Scale(1.0/num_events_in_run);
  }
  h_photon_et_ratio_co_cs->getTH1F()->Divide( h_photon_et_jetco->getTH1F(), h_photon_et_jetcs->getTH1F() );
  h_photon_et_ratio_fo_fs->getTH1F()->Divide( h_photon_et_jetfo->getTH1F(), h_photon_et_jetfs->getTH1F() );
  h_photon_et_ratio_cs_fs->getTH1F()->Divide( h_photon_et_jetcs->getTH1F(), h_photon_et_jetfs->getTH1F() );
  h_photon_et_ratio_co_fs->getTH1F()->Divide( h_photon_et_jetco->getTH1F(), h_photon_et_jetfs->getTH1F() );
  h_photon_et_ratio_cs_fo->getTH1F()->Divide( h_photon_et_jetcs->getTH1F(), h_photon_et_jetfo->getTH1F() );
  h_photon_et_ratio_co_fo->getTH1F()->Divide( h_photon_et_jetco->getTH1F(), h_photon_et_jetfo->getTH1F() );
}


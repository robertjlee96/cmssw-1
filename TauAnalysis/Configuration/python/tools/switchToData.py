import FWCore.ParameterSet.Config as cms
import copy

from RecoJets.Configuration.RecoJets_cff import *

from RecoJets.Configuration.GenJetParticles_cff import *
from RecoJets.Configuration.RecoGenJets_cff import *

from PhysicsTools.PatAlgos.tools.coreTools import *

from TauAnalysis.RecoTools.postPatProduction_cff import produceGenObjects

from TauAnalysis.Configuration.tools.analysisSequenceTools import removeAnalyzer

def switchToData(process):

    # remove MC matching from standard PAT sequences
    removeMCMatching(process, ["All"])

    # remove modules from pre-PAT production running on genParticles
    process.producePrePat.remove(ak5CaloJets)
    process.producePrePat.remove(ak5CaloJetsPUCorr)

    process.producePrePat.remove(genParticlesForJets)
    process.producePrePat.remove(ak5GenJets)

    # remove modules from post-PAT production running on genParticles
    process.producePostPat.remove(produceGenObjects)

    # remove modules from producePatTupleZtoElecTauSpecific sequence which run on genParticles
    if hasattr(process, "allDiTauPairs"): 
        process.allDiTauPairs.srcGenParticles = cms.InputTag('')
    if hasattr(process, "allElecTauPairs"):
        process.allElecTauPairs.srcGenParticles = cms.InputTag('')
    if hasattr(process, "allElecTauPairsLooseElectronIsolation"):
        process.allElecTauPairsLooseElectronIsolation.srcGenParticles = cms.InputTag('')
    if hasattr(process, "allElecMuPairs"):
        process.allElecMuPairs.srcGenParticles = cms.InputTag('')
    if hasattr(process, "allElecMuPairsLooseElectronIsolation"): 
        process.allElecMuPairsLooseElectronIsolation.srcGenParticles = cms.InputTag('')
    diTauPairCollectionNames = [
        "allDiTauPairs",
        "selectedDiTauPairs1stTauEta21Individual", "selectedDiTauPairs1stTauEta21Cumulative",
        "selectedDiTauPairs1stTauEta21Individual", "selectedDiTauPairs1stTauEta21Cumulative",
        "selectedDiTauPairs1stTauPt20Individual", "selectedDiTauPairs1stTauPt20Cumulative",
        "selectedDiTauPairs1stTauLeadTrkIndividual", "selectedDiTauPairs1stTauLeadTrkCumulative",
        "selectedDiTauPairs1stTauLeadTrkPtIndividual", "selectedDiTauPairs1stTauLeadTrkPtCumulative",
        "selectedDiTauPairs1stTauTaNCdiscrIndividual", "selectedDiTauPairs1stTauTaNCdiscrCumulative",
        "selectedDiTauPairs1stTauTrkIsoIndividual", "selectedDiTauPairs1stTauTrkIsoCumulative",
        "selectedDiTauPairs1stTauEcalIsoIndividual", "selectedDiTauPairs1stTauEcalIsoCumulative",
        "selectedDiTauPairs1stTauProngIndividual", "selectedDiTauPairs1stTauProngCumulative",
        "selectedDiTauPairs1stTauChargeIndividual", "selectedDiTauPairs1stTauChargeCumulative",
        "selectedDiTauPairs1stTauMuonVetoIndividual", "selectedDiTauPairs1stTauMuonVetoCumulative",
        "selectedDiTauPairs1stTauElectronVetoIndividual", "selectedDiTauPairs1stTauElectronVetoCumulative",
        "selectedDiTauPairs2ndTauEta21Individual", "selectedDiTauPairs2ndTauEta21Cumulative",
        "selectedDiTauPairs2ndTauPt20Individual", "selectedDiTauPairs2ndTauPt20Cumulative",
        "selectedDiTauPairs2ndTauLeadTrkIndividual", "selectedDiTauPairs2ndTauLeadTrkCumulative",
        "selectedDiTauPairs2ndTauLeadTrkPtIndividual", "selectedDiTauPairs2ndTauLeadTrkPtCumulative",
        "selectedDiTauPairs2ndTauLeadTrkPtLooseIndividual", "selectedDiTauPairs2ndTauLeadTrkPtLooseCumulative",
        "selectedDiTauPairs2ndTauTaNCdiscrIndividual", "selectedDiTauPairs2ndTauTaNCdiscrCumulative",
        "selectedDiTauPairs2ndTauTaNCdiscrLooseIndividual", "selectedDiTauPairs2ndTauTaNCdiscrLooseCumulative",
        "selectedDiTauPairs2ndTauTrkIsoIndividual", "selectedDiTauPairs2ndTauTrkIsoCumulative",
        "selectedDiTauPairs2ndTauTrkIsoLooseIndividual", "selectedDiTauPairs2ndTauTrkIsoLooseCumulative",
        "selectedDiTauPairs2ndTauEcalIsoIndividual", "selectedDiTauPairs2ndTauEcalIsoCumulative",
        "selectedDiTauPairs2ndTauEcalIsoLooseIndividual", "selectedDiTauPairs2ndTauEcalIsoLooseCumulative",
        "selectedDiTauPairs2ndTauProngIndividual", "selectedDiTauPairs2ndTauProngCumulative",
        "selectedDiTauPairs2ndTauProngLooseIndividual", "selectedDiTauPairs2ndTauProngLooseCumulative",
        "selectedDiTauPairs2ndTauChargeIndividual", "selectedDiTauPairs2ndTauChargeCumulative",
        "selectedDiTauPairs2ndTauChargeLooseIndividual", "selectedDiTauPairs2ndTauChargeLooseCumulative",
        "selectedDiTauPairs2ndTauMuonVetoIndividual", "selectedDiTauPairs2ndTauMuonVetoCumulative",
        "selectedDiTauPairs2ndTauMuonVetoLooseIndividual", "selectedDiTauPairs2ndTauMuonVetoLooseCumulative",
        "selectedDiTauPairs2ndTauElectronVetoIndividual", "selectedDiTauPairs2ndTauElectronVetoCumulative",
        "selectedDiTauPairs2ndTauElectronVetoLooseIndividual", "selectedDiTauPairs2ndTauElectronVetoLooseCumulative" ]
    for diTauPairCollectionName in diTauPairCollectionNames:
        if hasattr(process, diTauPairCollectionName):
            diTauPairCollection = getattr(process, diTauPairCollectionName)
            diTauPairCollection.srcGenParticles = cms.InputTag('')
    if hasattr(process, "allMuTauPairs"):
        process.allMuTauPairs.srcGenParticles = cms.InputTag('')
    if hasattr(process, "allMuTauPairsLooseMuonIsolation"): 
        process.allMuTauPairsLooseMuonIsolation.srcGenParticles = cms.InputTag('')
    if hasattr(process, "patPFMETs"):
        process.patPFMETs.addGenMET = cms.bool(False)

    # remove modules from the Z --> e + tau-jet analysis sequence which run on GEN collections
    if hasattr(process, "analyzeZtoElecTauEvents"):
        process.analyzeZtoElecTauEvents.analyzers.remove(process.genPhaseSpaceEventInfoHistManager)
        removeAnalyzer(process.analyzeZtoElecTauEvents.analysisSequence,"genPhaseSpaceEventInfoHistManager")
        process.analyzeZtoElecTauEvents.eventDumps[0].doGenInfo = cms.bool(False)
        process.analyzeZtoElecTauEvents.eventDumps[0].genParticleSource = cms.InputTag('')
        process.electronHistManager.genParticleSource = cms.InputTag('')
        process.tauHistManager.genParticleSource = cms.InputTag('')
        process.jetHistManager.genParticleSource = cms.InputTag('')
        process.elecTauPairZeeHypotheses.genLeptonsFromZsSource = cms.InputTag('')
        process.elecTauPairZeeHypothesesLooseElectronIsolation.genLeptonsFromZsSource = cms.InputTag('')
        #process.selectedDiTauPairs1stTauChargeCumulative.srcGenParticles = cms.InputTag('')    
        #process.selectedDiTauPairs1stTauChargeIndividual.srcGenParticles = cms.InputTag('')

    # remove modules from the W --> tau-jet nu analysis sequence which run on GEN collections
    if hasattr(process, "analyzeWtoTauNuEvents"):
        process.analyzeWtoTauNuEvents.eventDumps[0].doGenInfo = cms.bool(False)
        process.analyzeWtoTauNuEvents.eventDumps[0].genParticleSource = cms.InputTag('')
        process.electronHistManager.genParticleSource = cms.InputTag('')
        process.tauHistManager.genParticleSource = cms.InputTag('')
        process.jetHistManager.genParticleSource = cms.InputTag('')

    # remove modules from the Z --> tau-jet + tau-jet analysis sequence which run on GEN collections
    if hasattr(process, "analyzeZtoDiTauEvents"):
        module = getattr(process, "analyzeZtoDiTauEvents")

        # Remove genPhaseSpace info
        if process.genPhaseSpaceEventInfoHistManager in module.analyzers:
            module.analyzers.remove(process.genPhaseSpaceEventInfoHistManager)

        # Remove genParticles
        for analyzer in module.analyzers:
            if hasattr(analyzer , 'genParticleSource'):
                analyzer.genParticleSource = cms.InputTag('')
                #del analyzer.genParticleSource

            if hasattr(analyzer, 'histManagers'):
                for histManager in analyzer.histManagers:
                    if hasattr(histManager , 'genParticleSource'):
                        histManager.genParticleSource = cms.InputTag('')
                        #del histManager.genParticleSource

        removeAnalyzer(module.analysisSequence,"genPhaseSpaceEventInfoHistManager")
        module.eventDumps[0].doGenInfo = cms.bool(False)
        module.eventDumps[0].genParticleSource = cms.InputTag('')
        module.eventDumps[0].genJetSource = cms.InputTag('')
        module.eventDumps[0].genTauJetSource = cms.InputTag('')
        module.eventDumps[0].genEventInfoSource = cms.InputTag('')
        
        process.tauHistManager1.genParticleSource = cms.InputTag('')
        process.tauHistManager2.genParticleSource = cms.InputTag('')
        process.jetHistManager.genParticleSource = cms.InputTag('')

    # remove modules from the A/H --> mu + tau-jet analysis sequence which run on GEN collections
    for module_name in ['analyzeAHtoMuTauEvents_woBtag', 'analyzeAHtoMuTauEvents_wBtag']:
        if hasattr(process, "muTauPairZmumuHypothesesForAHtoMuTau"):
            process.muTauPairZmumuHypothesesForAHtoMuTau.genLeptonsFromZsSource = cms.InputTag('')
        if hasattr(process, "muTauPairZmumuHypothesesForAHtoMuTauLooseMuonIsolation"):
            process.muTauPairZmumuHypothesesForAHtoMuTauLooseMuonIsolation.genLeptonsFromZsSource = cms.InputTag('')
        if hasattr(process, "muTauPairZmumuHypotheses"):  
            process.muTauPairZmumuHypotheses.genLeptonsFromZsSource = cms.InputTag('')
        if hasattr(process, "muTauPairZmumuHypothesesLooseMuonIsolation"):  
            process.muTauPairZmumuHypothesesLooseMuonIsolation.genLeptonsFromZsSource = cms.InputTag('')

        if hasattr(process, module_name):
            module = getattr(process, module_name)

            # Remove genPhaseSpace info
            if process.genPhaseSpaceEventInfoHistManager in module.analyzers:
                module.analyzers.remove(process.genPhaseSpaceEventInfoHistManager)

            # Remove genParticles
            for analyzer in module.analyzers:
                if hasattr(analyzer , 'genParticleSource'):
                    analyzer.genParticleSource = cms.InputTag('')
                    #del analyzer.genParticleSource

                if hasattr(analyzer, 'histManagers'):
                    for histManager in analyzer.histManagers:
                        if hasattr(histManager , 'genParticleSource'):
                            histManager.genParticleSource = cms.InputTag('')
                            #del histManager.genParticleSource

            removeAnalyzer(module.analysisSequence,"genPhaseSpaceEventInfoHistManager")
            #module.eventDumps[0].doGenInfo = cms.bool(False)
            #module.eventDumps[0].genParticleSource = cms.InputTag('')
            
            process.muonHistManager.genParticleSource = cms.InputTag('')
            process.tauHistManager.genParticleSource = cms.InputTag('')
            process.jetHistManager.genParticleSource = cms.InputTag('')


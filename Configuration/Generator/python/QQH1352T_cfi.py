# The following comments couldn't be translated into the new config version:

# This is a vector of ParameterSet names to be read, in this order

# Higgs decays

import FWCore.ParameterSet.Config as cms

source = cms.Source("PythiaSource",
    pythiaPylistVerbosity = cms.untracked.int32(1),
    UseTauola = cms.untracked.bool(True),
    # put here the efficiency of your filter (1. if no filter)
    filterEfficiency = cms.untracked.double(1.0),
    UseTauolaPolarization = cms.untracked.bool(True),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    GeneratorParameters = cms.PSet(
        parameterSets = cms.vstring('generator'),
        generator = cms.vstring('TAUOLA = 0 0 0   ! TAUOLA ')
    ),
    #untracked bool UseTauola = false
    #untracked bool UseTauolaPolarization = false
    # put here the cross section of your process (in pb)
    crossSection = cms.untracked.double(0.388),
    maxEventsToPrint = cms.untracked.int32(3),
    PythiaParameters = cms.PSet(
        pythiaUESettings = cms.vstring('MSTJ(11)=3     ! Choice of the fragmentation function', 'MSTJ(22)=2     ! Decay those unstable particles', 'PARJ(71)=10 .  ! for which ctau  10 mm', 'MSTP(2)=1      ! which order running alphaS', 'MSTP(33)=0     ! no K factors in hard cross sections', 'MSTP(51)=7     ! structure function chosen', 'MSTP(81)=1     ! multiple parton interactions 1 is Pythia default', 'MSTP(82)=4     ! Defines the multi-parton model', 'MSTU(21)=1     ! Check on possible errors during program execution', 'PARP(82)=1.9409   ! pt cutoff for multiparton interactions', 'PARP(89)=1960. ! sqrts for which PARP82 is set', 'PARP(83)=0.5   ! Multiple interactions: matter distrbn parameter', 'PARP(84)=0.4   ! Multiple interactions: matter distribution parameter', 'PARP(90)=0.16  ! Multiple interactions: rescaling power', 'PARP(67)=2.5    ! amount of initial-state radiation', 'PARP(85)=1.0  ! gluon prod. mechanism in MI', 'PARP(86)=1.0  ! gluon prod. mechanism in MI', 'PARP(62)=1.25   ! ', 'PARP(64)=0.2    ! ', 'MSTP(91)=1     !', 'PARP(91)=2.1   ! kt distribution', 'PARP(93)=15.0  ! '),
        parameterSets = cms.vstring('pythiaUESettings', 'processParameters'),
        processParameters = cms.vstring('PMAS(25,1)=135.0        !mass of Higgs', 'MSEL=0                  !user selection for process', 'MSUB(123)=1             !ZZ fusion to H', 'MSUB(124)=1             !WW fusion to H', 'MDME(210,1)=0           !Higgs decay into dd', 'MDME(211,1)=0           !Higgs decay into uu', 'MDME(212,1)=0           !Higgs decay into ss', 'MDME(213,1)=0           !Higgs decay into cc', 'MDME(214,1)=0           !Higgs decay into bb', 'MDME(215,1)=0           !Higgs decay into tt', 'MDME(216,1)=0           !Higgs decay into', 'MDME(217,1)=0           !Higgs decay into Higgs decay', 'MDME(218,1)=0           !Higgs decay into e nu e', 'MDME(219,1)=0           !Higgs decay into mu nu mu', 'MDME(220,1)=1           !Higgs decay into tau tau', 'MDME(221,1)=0           !Higgs decay into Higgs decay', 'MDME(222,1)=0           !Higgs decay into g g', 'MDME(223,1)=0           !Higgs decay into gam gam', 'MDME(224,1)=0           !Higgs decay into gam Z', 'MDME(225,1)=0           !Higgs decay into Z Z', 'MDME(226,1)=0           !Higgs decay into W W')
    )
)




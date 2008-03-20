import FWCore.ParameterSet.Config as cms

source = cms.Source("PythiaSource",
    pythiaPylistVerbosity = cms.untracked.int32(0),
    filterEfficiency = cms.untracked.double(0.00013),
    pythiaHepMCVerbosity = cms.untracked.bool(False),
    crossSection = cms.untracked.double(54710000000.0),
    maxEventsToPrint = cms.untracked.int32(0),
    PythiaParameters = cms.PSet(
        pythiaUESettings = cms.vstring('MSTJ(11)=3     ! Choice of the fragmentation function', 'MSTJ(22)=2     ! Decay those unstable particles', 'PARJ(71)=10 .  ! for which ctau  10 mm', 'MSTP(2)=1      ! which order running alphaS', 'MSTP(33)=0     ! no K factors in hard cross sections', 'MSTP(51)=7     ! structure function chosen', 'MSTP(81)=1     ! multiple parton interactions 1 is Pythia default', 'MSTP(82)=4     ! Defines the multi-parton model', 'MSTU(21)=1     ! Check on possible errors during program execution', 'PARP(82)=1.9409   ! pt cutoff for multiparton interactions', 'PARP(89)=1960. ! sqrts for which PARP82 is set', 'PARP(83)=0.5   ! Multiple interactions: matter distrbn parameter', 'PARP(84)=0.4   ! Multiple interactions: matter distribution parameter', 'PARP(90)=0.16  ! Multiple interactions: rescaling power', 'PARP(67)=2.5    ! amount of initial-state radiation', 'PARP(85)=1.0  ! gluon prod. mechanism in MI', 'PARP(86)=1.0  ! gluon prod. mechanism in MI', 'PARP(62)=1.25   ! ', 'PARP(64)=0.2    ! ', 'MSTP(91)=1     !', 'PARP(91)=2.1   ! kt distribution', 'PARP(93)=15.0  ! '),
        parameterSets = cms.vstring('pythiaUESettings', 'processParameters'),
        processParameters = cms.vstring('MSEL        = 1        ! user defined subprocess', 'MSTJ(26)    = 0        ! Mixing off', 'MDME(863,1) = 3 ! 0.020000    nu_e     e+      D-   ', 'MDME(864,1) = 3 ! 0.055000    nu_e     e+      D*-  ', 'MDME(865,1) = 3 ! 0.005000    nu_e     e+      D_1- ', 'MDME(866,1) = 3 ! 0.005000    nu_e     e+      D*_0-', 'MDME(867,1) = 3 ! 0.008000    nu_e     e+      D*_1-', 'MDME(868,1) = 3 ! 0.012000    nu_e     e+      D*_2-', 'MDME(869,1) = 3 ! 0.020000    nu_mu    mu+     D-   ', 'MDME(870,1) = 3 ! 0.055000    nu_mu    mu+     D*-  ', 'MDME(871,1) = 3 ! 0.005000    nu_mu    mu+     D_1- ', 'MDME(872,1) = 3 ! 0.005000    nu_mu    mu+     D*_0-', 'MDME(873,1) = 3 ! 0.008000    nu_mu    mu+     D*_1-', 'MDME(874,1) = 3 ! 0.012000    nu_mu    mu+     D*_2-', 'MDME(875,1) = 3 ! 0.010000    nu_tau   tau+    D-   ', 'MDME(876,1) = 3 ! 0.030000    nu_tau   tau+    D*-  ', 'MDME(877,1) = 3 ! 0.003500    D-       pi+          ', 'MDME(878,1) = 3 ! 0.011000    D-       rho+         ', 'MDME(879,1) = 3 ! 0.005500    D-       a_1+         ', 'MDME(880,1) = 3 ! 0.004200    D*-      pi+          ', 'MDME(881,1) = 3 ! 0.009000    D*-      rho+         ', 'MDME(882,1) = 3 ! 0.018000    D*-      a_1+         ', 'MDME(883,1) = 3 ! 0.015000    D-       D_s+         ', 'MDME(884,1) = 3 ! 0.018500    D-       D*_s+        ', 'MDME(885,1) = 3 ! 0.013500    D*-      D_s+         ', 'MDME(886,1) = 3 ! 0.025000    D*-      D*_s+        ', 'MDME(887,1) = 3 ! 0.000400    eta_c    K0           ', 'MDME(888,1) = 3 ! 0.000700    eta_c    K*0          ', 'MDME(889,1) = 2 ! 0.000800    J/psi    K0           ', 'MDME(890,1) = 2 ! 0.001400    J/psi    K*0          ', 'MDME(891,1) = 2 ! 0.001900    chi_1c   K0           ', 'MDME(892,1) = 2 ! 0.002500    chi_1c   K*0          ', 'MDME(893,1) = 3 ! 0.429100    u     dbar    cbar   d', 'MDME(894,1) = 3 ! 0.080000    u     cbar    dbar   d', 'MDME(895,1) = 2 ! 0.070000    c     sbar    cbar   d', 'MDME(896,1) = 2 ! 0.020000    c     cbar    sbar   d', 'MDME(897,1) = 3 ! 0.015000    u     dbar    ubar   d', 'MDME(898,1) = 3 ! 0.005000    c     sbar    ubar   d', 'MDME(908,1) = 3 ! 0.020000    nu_e     e+    Dbar0   ', 'MDME(909,1) = 3 ! 0.055000    nu_e     e+    D*bar0  ', 'MDME(910,1) = 3 ! 0.005000    nu_e     e+    D_1bar0 ', 'MDME(911,1) = 3 ! 0.005000    nu_e     e+    D*_0bar0', 'MDME(912,1) = 3 ! 0.008000    nu_e     e+    D*_1bar0', 'MDME(913,1) = 3 ! 0.012000    nu_e     e+    D*_2bar0', 'MDME(914,1) = 3 ! 0.020000    nu_mu    mu+   Dbar0   ', 'MDME(915,1) = 3 ! 0.055000    nu_mu    mu+   D*bar0  ', 'MDME(916,1) = 3 ! 0.005000    nu_mu    mu+   D_1bar0 ', 'MDME(917,1) = 3 ! 0.005000    nu_mu    mu+   D*_0bar0', 'MDME(918,1) = 3 ! 0.008000    nu_mu    mu+   D*_1bar0', 'MDME(919,1) = 3 ! 0.012000    nu_mu    mu+   D*_2bar0', 'MDME(920,1) = 3 ! 0.010000    nu_tau   tau+  Dbar0   ', 'MDME(921,1) = 3 ! 0.030000    nu_tau   tau+  D*bar0  ', 'MDME(922,1) = 3 ! 0.003500    Dbar0      pi+         ', 'MDME(923,1) = 3 ! 0.011000    Dbar0      rho+        ', 'MDME(924,1) = 3 ! 0.005500    Dbar0      a_1+        ', 'MDME(925,1) = 3 ! 0.004200    D*bar0     pi+         ', 'MDME(926,1) = 3 ! 0.009000    D*bar0     rho+        ', 'MDME(927,1) = 3 ! 0.018000    D*bar0     a_1+        ', 'MDME(928,1) = 3 ! 0.015000    Dbar0      D_s+        ', 'MDME(929,1) = 3 ! 0.018500    Dbar0      D*_s+       ', 'MDME(930,1) = 3 ! 0.013500    D*bar0     D_s+        ', 'MDME(931,1) = 3 ! 0.025000    D*bar0     D*_s+       ', 'MDME(932,1) = 3 ! 0.000400    eta_c      K+          ', 'MDME(933,1) = 3 ! 0.000700    eta_c      K*+         ', 'MDME(934,1) = 2 ! 0.000800    J/psi      K+          ', 'MDME(935,1) = 2 ! 0.001400    J/psi      K*+         ', 'MDME(936,1) = 2 ! 0.001900    chi_1c     K+          ', 'MDME(937,1) = 2 ! 0.002500    chi_1c     K*+         ', 'MDME(938,1) = 3 ! 0.429100    u     dbar    cbar    u', 'MDME(939,1) = 3 ! 0.080000    u     cbar    dbar    u', 'MDME(940,1) = 2 ! 0.070000    c     sbar    cbar    u', 'MDME(941,1) = 2 ! 0.020000    c     cbar    sbar    u', 'MDME(942,1) = 3 ! 0.015000    u     dbar    ubar    u', 'MDME(943,1) = 3 ! 0.005000    c     sbar    ubar    u', 'MDME(953,1) = 3 ! 0.020000    nu_e     e+      D_s-  ', 'MDME(954,1) = 3 ! 0.055000    nu_e     e+      D*_s- ', 'MDME(955,1) = 3 ! 0.005000    nu_e     e+      D_1s- ', 'MDME(956,1) = 3 ! 0.005000    nu_e     e+      D*_0s-', 'MDME(957,1) = 3 ! 0.008000    nu_e     e+      D*_1s-', 'MDME(958,1) = 3 ! 0.012000    nu_e     e+      D*_2s-', 'MDME(959,1) = 3 ! 0.020000    nu_mu    mu+     D_s-  ', 'MDME(960,1) = 3 ! 0.055000    nu_mu    mu+     D*_s- ', 'MDME(961,1) = 3 ! 0.005000    nu_mu    mu+     D_1s- ', 'MDME(962,1) = 3 ! 0.005000    nu_mu    mu+     D*_0s-', 'MDME(963,1) = 3 ! 0.008000    nu_mu    mu+     D*_1s-', 'MDME(964,1) = 3 ! 0.012000    nu_mu    mu+     D*_2s-', 'MDME(965,1) = 3 ! 0.010000    nu_tau   tau+    D_s-  ', 'MDME(966,1) = 3 ! 0.030000    nu_tau   tau+    D*_s- ', 'MDME(967,1) = 3 ! 0.003500    D_s-        pi+        ', 'MDME(968,1) = 3 ! 0.011000    D_s-        rho+       ', 'MDME(969,1) = 3 ! 0.005500    D_s-        a_1+       ', 'MDME(970,1) = 3 ! 0.004200    D*_s-       pi+        ', 'MDME(971,1) = 3 ! 0.009000    D*_s-       rho+       ', 'MDME(972,1) = 3 ! 0.018000    D*_s-       a_1+       ', 'MDME(973,1) = 3 ! 0.015000    D_s-        D_s+       ', 'MDME(974,1) = 3 ! 0.018500    D_s-        D*_s+      ', 'MDME(975,1) = 3 ! 0.013500    D*_s-       D_s+       ', 'MDME(976,1) = 3 ! 0.025000    D*_s-       D*_s+      ', 'MDME(977,1) = 3 ! 0.000200    eta_c       eta        ', 'MDME(978,1) = 3 ! 0.000200    eta_c       eta_       ', 'MDME(979,1) = 3 ! 0.000700    eta_c       phi        ', 'MDME(980,1) = 2 ! 0.000400    J/psi       eta        ', 'MDME(981,1) = 2 ! 0.000400    J/psi       eta_       ', 'MDME(982,1) = 2 ! 0.001400    J/psi       phi        ', 'MDME(983,1) = 2 ! 0.001000    chi_1c      eta        ', 'MDME(984,1) = 2 ! 0.000900    chi_1c      eta_       ', 'MDME(985,1) = 2 ! 0.002500    chi_1c      phi        ', 'MDME(986,1) = 3 ! 0.429100    u     dbar    cbar    s', 'MDME(987,1) = 3 ! 0.080000    u     cbar    dba     s', 'MDME(988,1) = 2 ! 0.070000    c     sbar    cbar    s', 'MDME(989,1) = 2 ! 0.020000    c     cbar    sbar    s', 'MDME(990,1) = 3 ! 0.015000    u     dbar    ubar    s', 'MDME(991,1) = 3 ! 0.005000    c     sbar    ubar    s', 'MDME(997,1) = 3 ! 0.047000    nu_tau      tau+       ', 'MDME(998,1) = 3 ! 0.122000    c           sbar       ', 'MDME(999,1) = 3 ! 0.006000    c           dbar       ', 'MDME(1000,1) = 3 ! 0.012000    nu_e      e+     eta_c', 'MDME(1001,1) = 2 ! 0.035000    nu_e      e+     J/psi', 'MDME(1002,1) = 3 ! 0.012000    nu_mu     mu+    eta_c', 'MDME(1003,1) = 2 ! 0.035000    nu_mu     mu+    J/psi', 'MDME(1004,1) = 3 ! 0.003000    nu_tau    tau+   eta_c', 'MDME(1005,1) = 2 ! 0.007000    nu_tau    tau+   J/psi', 'MDME(1006,1) = 3 ! 0.150000    u    dbar   cbar     c', 'MDME(1007,1) = 2 ! 0.037000    u    cbar   dbar     c', 'MDME(1008,1) = 2 ! 0.008000    u    sbar   cbar     c', 'MDME(1009,1) = 2 ! 0.002000    u    cbar   sbar     c', 'MDME(1010,1) = 2 ! 0.050000    c    sbar   cbar     c', 'MDME(1011,1) = 2 ! 0.015000    c    cbar   sbar     c', 'MDME(1012,1) = 2 ! 0.003000    c    dbar   cbar     c', 'MDME(1013,1) = 2 ! 0.001000    c    cbar   dbar     c', 'MDME(1014,1) = 1 ! 0.014000    e+      nu_e     B_s0 ', 'MDME(1015,1) = 1 ! 0.042000    e+      nu_e     B*_s0', 'MDME(1016,1) = 1 ! 0.014000    mu+     nu_mu    B_s0 ', 'MDME(1017,1) = 1 ! 0.042000    mu+     nu_mu    B*_s0', 'MDME(1018,1) = 3 ! 0.240000    dbar    u    s    bbar', 'MDME(1019,1) = 3 ! 0.065000    dbar    s    u    bbar', 'MDME(1020,1) = 3 ! 0.012000    sbar    u    s    bbar', 'MDME(1021,1) = 3 ! 0.003000    sbar    s    u    bbar', 'MDME(1022,1) = 1 ! 0.001000    e+      nu_e      B0  ', 'MDME(1023,1) = 1 ! 0.002000    e+      nu_e      B*0 ', 'MDME(1024,1) = 1 ! 0.001000    mu+     nu_mu     B0  ', 'MDME(1025,1) = 1 ! 0.002000    mu+     nu_mu     B*0 ', 'MDME(1026,1) = 3 ! 0.014000    dbar    u    d    bbar', 'MDME(1027,1) = 3 ! 0.003000    dbar    d    u    bbar', 'MDME(1219,1) = 3 ! 0.105000    nu_ebar     e-     Lambda_c+', 'MDME(1220,1) = 3 ! 0.105000    nu_mubar    mu-    Lambda_c+', 'MDME(1221,1) = 3 ! 0.040000    nu_taubar   tau-   Lambda_c+', 'MDME(1222,1) = 3 ! 0.007700    Lambda_c+       pi-         ', 'MDME(1223,1) = 3 ! 0.020000    Lambda_c+       rho-        ', 'MDME(1224,1) = 3 ! 0.023500    Lambda_c+       a_1-        ', 'MDME(1225,1) = 3 ! 0.028500    Lambda_c+       D_s-        ', 'MDME(1226,1) = 3 ! 0.043500    Lambda_c+       D*_s-       ', 'MDME(1227,1) = 3 ! 0.001100    eta_c           Lambda0     ', 'MDME(1228,1) = 2 ! 0.002200    J/psi           Lambda0     ', 'MDME(1229,1) = 2  ! 0.004400    chi_1c          Lambda0     ', 'MDME(1230,1) = 3 ! 0.429100    ubar    d     c    ud_0     ', 'MDME(1231,1) = 3 ! 0.080000    ubar    c     d    ud_0     ', 'MDME(1232,1) = 2 ! 0.070000    cbar    s     c    ud_0     ', 'MDME(1233,1) = 2 ! 0.020000    cbar    c     s    ud_0     ', 'MDME(1234,1) = 3 ! 0.015000    ubar    d     u    ud_0     ', 'MDME(1235,1) = 3 ! 0.005000    cbar    s     u    ud_0     ', 'BRAT(891) = 0.000519 ! B0->Chi_1c 0.001900*0.273000', 'BRAT(892) = 0.000683 ! B0->Chi_1c 0.002500*0.273000', 'BRAT(936) = 0.000519 ! B+->Chi_1c 0.001900*0.273000', 'BRAT(936) = 0.000683 ! B+->Chi_1c 0.002500*0.273000', 'BRAT(983) = 0.000273 ! Bs->Chi_1c 0.001000*0.273000', 'BRAT(984) = 0.000246 ! Bs->Chi_1c 0.000900*0.273000', 'BRAT(985) = 0.000683 ! Bs->Chi_1c 0.002500*0.273000', 'BRAT(1229)= 0.001201 ! Lambda_b0->Chi_1c+lambda0 0.004400*0.273000', 'MDME(1555,1) = 1 ! 0.273000    J/psi           gamma ', 'MDME(1556,1) = 0 ! 0.727000    rndmflav        rndmflavbar ', 'MDME(858,1) = 0  ! 0.060200    e-    e+', 'MDME(859,1) = 1  ! 0.060100    mu-  mu+', 'MDME(860,1) = 0  ! 0.879700    rndmflav        rndmflavbar')
    )
)




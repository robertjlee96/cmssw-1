// modified by Emmanuele, to remove TTHits

#include "SimDataFormats/SLHC/interface/StackedTrackerTypes.h"

#include "SLHCUpgradeSimulations/L1Trigger/interface/LocalStubBuilder.h"
typedef LocalStubBuilder<cmsUpgrades::Ref_PSimHit_> LocalStubBuilder_PSimHit_;
DEFINE_FWK_MODULE(LocalStubBuilder_PSimHit_);
typedef LocalStubBuilder<cmsUpgrades::Ref_PixelDigi_> LocalStubBuilder_PixelDigi_;
DEFINE_FWK_MODULE(LocalStubBuilder_PixelDigi_);
// typedef LocalStubBuilder<cmsUpgrades::Ref_TTHit_> LocalStubBuilder_TTHit_;
// DEFINE_FWK_MODULE(LocalStubBuilder_TTHit_);

#include "SLHCUpgradeSimulations/L1Trigger/interface/GlobalStubBuilder.h"
typedef GlobalStubBuilder<cmsUpgrades::Ref_PSimHit_> GlobalStubBuilder_PSimHit_;
DEFINE_FWK_MODULE(GlobalStubBuilder_PSimHit_);
typedef GlobalStubBuilder<cmsUpgrades::Ref_PixelDigi_> GlobalStubBuilder_PixelDigi_;
DEFINE_FWK_MODULE(GlobalStubBuilder_PixelDigi_);
// typedef GlobalStubBuilder<cmsUpgrades::Ref_TTHit_> GlobalStubBuilder_TTHit_;
// DEFINE_FWK_MODULE(GlobalStubBuilder_TTHit_);

#include "SLHCUpgradeSimulations/L1Trigger/interface/TrackletBuilder.h"
typedef TrackletBuilder<cmsUpgrades::Ref_PSimHit_> TrackletBuilder_PSimHit_;
DEFINE_FWK_MODULE(TrackletBuilder_PSimHit_);
typedef TrackletBuilder<cmsUpgrades::Ref_PixelDigi_> TrackletBuilder_PixelDigi_;
DEFINE_FWK_MODULE(TrackletBuilder_PixelDigi_);
// typedef TrackletBuilder<cmsUpgrades::Ref_TTHit_> TrackletBuilder_TTHit_;
// DEFINE_FWK_MODULE(TrackletBuilder_TTHit_);

#include "SLHCUpgradeSimulations/L1Trigger/interface/TrackletChainBuilder.h"
typedef TrackletChainBuilder<cmsUpgrades::Ref_PSimHit_> TrackletChainBuilder_PSimHit_;
DEFINE_FWK_MODULE(TrackletChainBuilder_PSimHit_);
typedef TrackletChainBuilder<cmsUpgrades::Ref_PixelDigi_> TrackletChainBuilder_PixelDigi_;
DEFINE_FWK_MODULE(TrackletChainBuilder_PixelDigi_);
// typedef TrackletChainBuilder<cmsUpgrades::Ref_TTHit_> TrackletChainBuilder_TTHit_;
// DEFINE_FWK_MODULE(TrackletChainBuilder_TTHit_);




#include "SLHCUpgradeSimulations/L1Trigger/interface/L1TrackBuilder_LB.h"
typedef L1TrackBuilderLB<cmsUpgrades::Ref_PSimHit_> L1TrackBuilderLB_PSimHit_;
DEFINE_FWK_MODULE(L1TrackBuilderLB_PSimHit_);
typedef L1TrackBuilderLB<cmsUpgrades::Ref_PixelDigi_> L1TrackBuilderLB_PixelDigi_;
DEFINE_FWK_MODULE(L1TrackBuilderLB_PixelDigi_);




/* - The Hit Matching Algorithms - */


#include "SLHCUpgradeSimulations/L1Trigger/interface/HitMatchingAlgorithm_a.h"
typedef ES_HitMatchingAlgorithm_a<cmsUpgrades::Ref_PSimHit_> HitMatchingAlgorithm_a_PSimHit_;
DEFINE_FWK_EVENTSETUP_MODULE(HitMatchingAlgorithm_a_PSimHit_);
typedef ES_HitMatchingAlgorithm_a<cmsUpgrades::Ref_PixelDigi_> HitMatchingAlgorithm_a_PixelDigi_;
DEFINE_FWK_EVENTSETUP_MODULE(HitMatchingAlgorithm_a_PixelDigi_);
// typedef ES_HitMatchingAlgorithm_a<cmsUpgrades::Ref_TTHit_> HitMatchingAlgorithm_a_TTHit_;
// DEFINE_FWK_EVENTSETUP_MODULE(HitMatchingAlgorithm_a_TTHit_);

#include "SLHCUpgradeSimulations/L1Trigger/interface/HitMatchingAlgorithm_thresholds.h"
typedef ES_HitMatchingAlgorithm_thresholds<cmsUpgrades::Ref_PixelDigi_> HitMatchingAlgorithm_thresholds_PixelDigi_;
DEFINE_FWK_EVENTSETUP_MODULE(HitMatchingAlgorithm_thresholds_PixelDigi_);
// typedef ES_HitMatchingAlgorithm_thresholds<cmsUpgrades::Ref_TTHit_> HitMatchingAlgorithm_thresholds_TTHit_;
// DEFINE_FWK_EVENTSETUP_MODULE(HitMatchingAlgorithm_thresholds_TTHit_);

#include "SLHCUpgradeSimulations/L1Trigger/interface/HitMatchingAlgorithm_window.h"
typedef ES_HitMatchingAlgorithm_window<cmsUpgrades::Ref_PixelDigi_> HitMatchingAlgorithm_window_PixelDigi_;
DEFINE_FWK_EVENTSETUP_MODULE(HitMatchingAlgorithm_window_PixelDigi_);
// typedef ES_HitMatchingAlgorithm_window<cmsUpgrades::Ref_TTHit_> HitMatchingAlgorithm_window_TTHit_;
// DEFINE_FWK_EVENTSETUP_MODULE(HitMatchingAlgorithm_window_TTHit_);

#include "SLHCUpgradeSimulations/L1Trigger/interface/HitMatchingAlgorithm_globalgeometry.h"
typedef ES_HitMatchingAlgorithm_globalgeometry<cmsUpgrades::Ref_PSimHit_> HitMatchingAlgorithm_globalgeometry_PSimHit_;
DEFINE_FWK_EVENTSETUP_MODULE(HitMatchingAlgorithm_globalgeometry_PSimHit_);
typedef ES_HitMatchingAlgorithm_globalgeometry<cmsUpgrades::Ref_PixelDigi_> HitMatchingAlgorithm_globalgeometry_PixelDigi_;
DEFINE_FWK_EVENTSETUP_MODULE(HitMatchingAlgorithm_globalgeometry_PixelDigi_);
// typedef ES_HitMatchingAlgorithm_globalgeometry<cmsUpgrades::Ref_TTHit_> HitMatchingAlgorithm_globalgeometry_TTHit_;
// DEFINE_FWK_EVENTSETUP_MODULE(HitMatchingAlgorithm_globalgeometry_TTHit_);

#include "SLHCUpgradeSimulations/L1Trigger/interface/HitMatchingAlgorithm_pixelray.h"
typedef ES_HitMatchingAlgorithm_pixelray<cmsUpgrades::Ref_PixelDigi_> HitMatchingAlgorithm_pixelray_PixelDigi_;
DEFINE_FWK_EVENTSETUP_MODULE(HitMatchingAlgorithm_pixelray_PixelDigi_);
// typedef ES_HitMatchingAlgorithm_pixelray<cmsUpgrades::Ref_TTHit_> HitMatchingAlgorithm_pixelray_TTHit_;
// DEFINE_FWK_EVENTSETUP_MODULE(HitMatchingAlgorithm_pixelray_TTHit_);


/* - The Clustering Algorithms - */


#include "SLHCUpgradeSimulations/L1Trigger/interface/ClusteringAlgorithm_a.h"
typedef ES_ClusteringAlgorithm_a<cmsUpgrades::Ref_PSimHit_> ClusteringAlgorithm_a_PSimHit_;
DEFINE_FWK_EVENTSETUP_MODULE(ClusteringAlgorithm_a_PSimHit_);
typedef ES_ClusteringAlgorithm_a<cmsUpgrades::Ref_PixelDigi_> ClusteringAlgorithm_a_PixelDigi_;
DEFINE_FWK_EVENTSETUP_MODULE(ClusteringAlgorithm_a_PixelDigi_);
// typedef ES_ClusteringAlgorithm_a<cmsUpgrades::Ref_TTHit_> ClusteringAlgorithm_a_TTHit_;
// DEFINE_FWK_EVENTSETUP_MODULE(ClusteringAlgorithm_a_TTHit_);


#include "SLHCUpgradeSimulations/L1Trigger/interface/ClusteringAlgorithm_broadside.h"
typedef ES_ClusteringAlgorithm_broadside<cmsUpgrades::Ref_PSimHit_> ClusteringAlgorithm_broadside_PSimHit_;
DEFINE_FWK_EVENTSETUP_MODULE(ClusteringAlgorithm_broadside_PSimHit_);
typedef ES_ClusteringAlgorithm_broadside<cmsUpgrades::Ref_PixelDigi_> ClusteringAlgorithm_broadside_PixelDigi_;
DEFINE_FWK_EVENTSETUP_MODULE(ClusteringAlgorithm_broadside_PixelDigi_);
// typedef ES_ClusteringAlgorithm_broadside<cmsUpgrades::Ref_TTHit_> ClusteringAlgorithm_broadside_TTHit_;
// DEFINE_FWK_EVENTSETUP_MODULE(ClusteringAlgorithm_broadside_TTHit_);

#include "SLHCUpgradeSimulations/L1Trigger/interface/ClusteringAlgorithm_2d.h"
typedef ES_ClusteringAlgorithm_2d<cmsUpgrades::Ref_PSimHit_> ClusteringAlgorithm_2d_PSimHit_;
DEFINE_FWK_EVENTSETUP_MODULE(ClusteringAlgorithm_2d_PSimHit_);
typedef ES_ClusteringAlgorithm_2d<cmsUpgrades::Ref_PixelDigi_> ClusteringAlgorithm_2d_PixelDigi_;
DEFINE_FWK_EVENTSETUP_MODULE(ClusteringAlgorithm_2d_PixelDigi_);
// typedef ES_ClusteringAlgorithm_2d<cmsUpgrades::Ref_TTHit_> ClusteringAlgorithm_2d_TTHit_;
// DEFINE_FWK_EVENTSETUP_MODULE(ClusteringAlgorithm_2d_TTHit_);


#include "SLHCUpgradeSimulations/L1Trigger/interface/ClusteringAlgorithm_neighbor.h"
typedef ES_ClusteringAlgorithm_neighbor<cmsUpgrades::Ref_PixelDigi_> ClusteringAlgorithm_neighbor_PixelDigi_;
DEFINE_FWK_EVENTSETUP_MODULE(ClusteringAlgorithm_neighbor_PixelDigi_);
// typedef ES_ClusteringAlgorithm_neighbor<cmsUpgrades::Ref_TTHit_> ClusteringAlgorithm_neighbor_TTHit_;
// DEFINE_FWK_EVENTSETUP_MODULE(ClusteringAlgorithm_neighbor_TTHit_);

/* - L1 CaloTrigger - */
//#include "SLHCUpgradeSimulations/L1Trigger/interface/L1CaloTriggerSetupProducer.h"
//DEFINE_FWK_EVENTSETUP_MODULE(L1CaloTriggerSetupProducer);




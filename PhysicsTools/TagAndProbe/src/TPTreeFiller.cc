#include "PhysicsTools/TagAndProbe/interface/TPTreeFiller.h"

tnp::TPTreeFiller::TPTreeFiller(const edm::ParameterSet config) :
    tnp::BaseTreeFiller("fitter_tree",config) 
{
    // Add extra branch for the mass
    tree_->Branch("mass",   &mass_,   "mass/F");

    // set up MC if needed
    if (config.getParameter<bool>("isMC")) {
        tree_->Branch("mcTrue", &mcTrue_, "mcTrue/B");
    }
}

tnp::TPTreeFiller::~TPTreeFiller() {}

void tnp::TPTreeFiller::init(const edm::Event &iEvent) const {
    tnp::BaseTreeFiller::init(iEvent);
}

void tnp::TPTreeFiller::fill(const reco::CandidateBaseRef &probe, double mass, bool mcTrue) const {
    mass_ = mass;
    mcTrue_ = mcTrue;
    tnp::BaseTreeFiller::fill(probe);
}

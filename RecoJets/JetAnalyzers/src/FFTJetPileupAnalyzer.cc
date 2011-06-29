// -*- C++ -*-
//
// Package:    FFTJetPileupAnalyzer
// Class:      FFTJetPileupAnalyzer
// 
/**\class FFTJetPileupAnalyzer FFTJetPileupAnalyzer.cc RecoJets/JetAnalyzers/src/FFTJetPileupAnalyzer.cc

 Description: collects the info produced by FFTJetPileupProcessor and FFTJetPileupEstimator

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Igor Volobouev
//         Created:  Thu Apr 21 15:52:11 CDT 2011
// $Id: FFTJetPileupAnalyzer.cc,v 1.2 2011/04/30 21:49:19 igv Exp $
//
//

#include <cassert>
#include <sstream>
#include <numeric>

#include "TNtuple.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Histograms/interface/MEtoEDMFormat.h"
#include "DataFormats/JetReco/interface/FFTJetPileupSummary.h"

#include "SimDataFormats/PileupSummaryInfo/interface/PileupSummaryInfo.h"

#include "CommonTools/UtilAlgos/interface/TFileService.h"

#include "RecoJets/FFTJetAlgorithms/interface/DiscretizedEnergyFlow.h"

#define init_param(type, varname) varname (ps.getParameter< type >( #varname ))

//
// class declaration
//
class FFTJetPileupAnalyzer : public edm::EDAnalyzer
{
public:
    explicit FFTJetPileupAnalyzer(const edm::ParameterSet&);
    ~FFTJetPileupAnalyzer();

private:
    FFTJetPileupAnalyzer();
    FFTJetPileupAnalyzer(const FFTJetPileupAnalyzer&);
    FFTJetPileupAnalyzer& operator=(const FFTJetPileupAnalyzer&);

    // The following method should take all necessary info from
    // PileupSummaryInfo and fill out the ntuple
    void analyzePileup(const std::vector<PileupSummaryInfo>& pInfo);

    virtual void beginJob() ;
    virtual void analyze(const edm::Event&, const edm::EventSetup&);
    virtual void endJob() ;

    edm::InputTag histoLabel;
    edm::InputTag summaryLabel;
    edm::InputTag fastJetRhoLabel;
    edm::InputTag fastJetSigmaLabel;
    edm::InputTag gridLabel;
    std::string pileupLabel;
    std::string ntupleName;
    std::string ntupleTitle;
    bool collectHistos;
    bool collectSummaries;
    bool collectFastJetRho;
    bool collectPileup;
    bool collectGrids;
    bool verbosePileupInfo;

    std::vector<float> ntupleData;
    TNtuple* nt;
    unsigned long counter;
    int totalNpu;
};

//
// constructors and destructor
//
FFTJetPileupAnalyzer::FFTJetPileupAnalyzer(const edm::ParameterSet& ps)
    : init_param(edm::InputTag, histoLabel),
      init_param(edm::InputTag, summaryLabel),
      init_param(edm::InputTag, fastJetRhoLabel),
      init_param(edm::InputTag, fastJetSigmaLabel),
      init_param(edm::InputTag, gridLabel),
      init_param(std::string, pileupLabel),
      init_param(std::string, ntupleName),
      init_param(std::string, ntupleTitle),
      init_param(bool, collectHistos),
      init_param(bool, collectSummaries),
      init_param(bool, collectFastJetRho),
      init_param(bool, collectPileup),
      init_param(bool, collectGrids),
      init_param(bool, verbosePileupInfo),
      nt(0),
      counter(0),
      totalNpu(-1)
{
}


FFTJetPileupAnalyzer::~FFTJetPileupAnalyzer()
{
}


//
// member functions
//
void FFTJetPileupAnalyzer::analyzePileup(
    const std::vector<PileupSummaryInfo>& info)
{
    const unsigned nBx = info.size();
    ntupleData.push_back(static_cast<float>(nBx));

    double sumpt_Lo = 0.0, sumpt_Hi = 0.0;

    if (verbosePileupInfo)
        std::cout << "\n**** Pileup info begin" << std::endl;

    for (unsigned ibx = 0; ibx < nBx; ++ibx)
    {
        const PileupSummaryInfo& puInfo(info[ibx]);

        const int bx = puInfo.getBunchCrossing();
        const int npu = puInfo.getPU_NumInteractions();
        const std::vector<float>& lopt(puInfo.getPU_sumpT_lowpT());
        const std::vector<float>& hipt(puInfo.getPU_sumpT_highpT());
        const double losum = std::accumulate(lopt.begin(), lopt.end(), 0.0);
        const double hisum = std::accumulate(hipt.begin(), hipt.end(), 0.0);

        // Note that the following only works for in-time pileup
        if (ibx == 0)
        {
            totalNpu = npu;
            sumpt_Lo = losum;
            sumpt_Hi = hisum;
        }

        if (verbosePileupInfo)
            std::cout << "ibx " << ibx << " bx " << bx
                      << " npu " << npu << " losum " << losum
                      << " hisum " << hisum
                      << std::endl;
    }

    if (verbosePileupInfo)
        std::cout << "**** Pileup info end\n" << std::endl;

    ntupleData.push_back(totalNpu);
    ntupleData.push_back(sumpt_Lo);
    ntupleData.push_back(sumpt_Hi);
}


// ------------ method called once each job just before starting event loop
void FFTJetPileupAnalyzer::beginJob()
{
    std::string vars = "cnt:run:event:nbx:npu:sumptLowCut:sumptHiCut";
    if (collectSummaries)
        vars += ":estimate:pileup:uncert:uncertCode";
    if (collectFastJetRho)
        vars += ":fjrho:fjsigma";
    edm::Service<TFileService> fs;
    nt = fs->make<TNtuple>(ntupleName.c_str(), ntupleTitle.c_str(),
                           vars.c_str());
    ntupleData.reserve(nt->GetNvar());
}


// ------------ method called to for each event  ------------
void FFTJetPileupAnalyzer::analyze(const edm::Event& iEvent,
                                   const edm::EventSetup& iSetup)
{
    ntupleData.clear();
    ntupleData.push_back(counter);
    totalNpu = -1;

    const long runnumber = iEvent.id().run();
    const long eventnumber = iEvent.id().event();
    ntupleData.push_back(runnumber);
    ntupleData.push_back(eventnumber);

    // Get pileup information
    bool gotPileup = false;
    if (collectPileup)
    {
        edm::Handle<std::vector<PileupSummaryInfo> > puInfo;
        if (iEvent.getByLabel(pileupLabel, puInfo))
        {
            analyzePileup(*puInfo);
            gotPileup = true;
        }
    }
    if (!gotPileup)
    {
        ntupleData.push_back(-1);
        ntupleData.push_back(-1);
        ntupleData.push_back(0.f);
        ntupleData.push_back(0.f);
    }

    if (collectHistos)
    {
        edm::Handle<TH2D> input;
        iEvent.getByLabel(histoLabel, input);

        edm::Service<TFileService> fs;
        TH2D* copy = new TH2D(*input);

        std::ostringstream os;
        os << copy->GetName() << '_' << counter << '_'
           << totalNpu << '_' << runnumber << '_' << eventnumber;
        const std::string& newname(os.str());
        copy->SetNameTitle(newname.c_str(), newname.c_str());

        copy->SetDirectory(fs->getBareDirectory());
    }

    if (collectSummaries)
    {
        edm::Handle<reco::FFTJetPileupSummary> summary;
        iEvent.getByLabel(summaryLabel, summary);

        ntupleData.push_back(summary->uncalibratedQuantile());
        ntupleData.push_back(summary->pileupRho());
        ntupleData.push_back(summary->pileupRhoUncertainty());
        ntupleData.push_back(summary->uncertaintyCode());
    }

    if (collectFastJetRho)
    {
        edm::Handle<double> fjrho, fjsigma;
        iEvent.getByLabel(fastJetRhoLabel, fjrho);
        iEvent.getByLabel(fastJetSigmaLabel, fjsigma);

        ntupleData.push_back(*fjrho);
        ntupleData.push_back(*fjsigma);
    }

    if (collectGrids)
    {
        edm::Handle<fftjetcms::DiscretizedEnergyFlow> input;
        iEvent.getByLabel(gridLabel, input);

        // Make sure the input grid is reasonable
        const double* data = input->data();
        assert(data);
        assert(input->phiBin0Edge() == 0.0);
        const unsigned nEta = input->nEtaBins();
        const unsigned nPhi = input->nPhiBins();

        // Generate a name for the output histogram
        std::ostringstream os;
        os << "FFTJetGrid_" << counter << '_'
           << totalNpu << '_' << runnumber << '_' << eventnumber;
        const std::string& newname(os.str());

        // Make a histogram and copy the grid data into it
        edm::Service<TFileService> fs;
        TH2F* h = fs->make<TH2F>(newname.c_str(), newname.c_str(),
                                 nEta, input->etaMin(), input->etaMax(),
                                 nPhi, 0.0, 2.0*M_PI);
        for (unsigned ieta=0; ieta<nEta; ++ieta)
            for (unsigned iphi=0; iphi<nPhi; ++iphi)
                h->SetBinContent(ieta+1U, iphi+1U, data[ieta*nPhi + iphi]);
    }

    assert(ntupleData.size() == static_cast<unsigned>(nt->GetNvar()));
    nt->Fill(&ntupleData[0]);

    ++counter;
}


// ------------ method called once each job just after ending the event loop
void FFTJetPileupAnalyzer::endJob()
{
}


//define this as a plug-in
DEFINE_FWK_MODULE(FFTJetPileupAnalyzer);

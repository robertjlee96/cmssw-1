#include <memory>

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/TrackReco/interface/Track.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"

#include <iostream>
#include <string>

#include <TH1.h>
#include <TH2.h>
#include <TROOT.h>
#include <TFile.h>
#include <TCanvas.h>

using namespace edm;
using namespace std;

class TrackValidator : public edm::EDAnalyzer {
 public:
  TrackValidator(const edm::ParameterSet& pset)
    : sim(pset.getParameter<string>("sim")),
      label(pset.getParameter<string>("label")),
      out(pset.getParameter<string>("out")),
      min(pset.getParameter<double>("min")),
      max(pset.getParameter<double>("max")),
      nint(pset.getParameter<int>("nint"))
  {}

  ~TrackValidator(){}

  void beginJob( const EventSetup & ) {
    double step=(max-min)/nint;
    ostringstream title,name;
    etaintervals.push_back(0);
    for (double d=min;d<max;d=d+step) {
      etaintervals.push_back(d+step);
      tot.push_back(0);
      name.str("");
      title.str("");
      name <<"missingtracks_vs_eta["<<d<<","<<d+step<<"]";
      title <<"(Missing Tracks)/(Total Tracks) "<< d << "<#eta<"<<d+step;
      missingtracks.push_back(new TH1F(name.str().c_str(),title.str().c_str(), 5, -2, 2 ));
      name.str("");
      title.str("");
      name <<"pt["<<d<<","<<d+step<<"]";
      title <<"p_{t} residue "<< d << "<#eta<"<<d+step;
      ptdistrib.push_back(new TH1F(name.str().c_str(),title.str().c_str(), 200, -2, 2 ));
      name.str("");
      title.str("");
      name <<"eta["<<d<<","<<d+step<<"]";
      title <<"eta residue "<< d << "<#eta<"<<d+step;
      etadistrib.push_back(new TH1F(name.str().c_str(),title.str().c_str(), 200, -0.2, 0.2 ));
    }

    h_ptSIM     = new TH1F("ptSIM", "generated p_{t}", 550, 0, 110 );
    h_etaSIM    = new TH1F("etaSIM", "generated pseudorapidity", 500, 0, 5 );
    h_tracksSIM = new TH1F("tracksSIM","number of simluated tracks",10,0,10);

    h_pt     = new TH1F("pt", "p_{t} residue", 200, -2, 2 );
    h_eta    = new TH1F("eta", "pseudorapidity residue", 200, -0.2, 0.2 );
    h_tracks = new TH1F("tracks","number of reconstructed tracks",10,0,10);
    h_nchi2  = new TH1F("nchi2", "normalized chi2", 200, 0, 20 );
    h_hits   = new TH1F("hits", "number of hits per track", 30, 0, 30 );
    h_effic  = new TH1F("effic","efficiency vs #eta",nint,&etaintervals[0]);
    h_ptrmsh = new TH1F("PtRMS","PtRMS vs #eta",nint,&etaintervals[0]);
    h_deltaeta= new TH1F("etaRMS","etaRMS vs #eta",nint,&etaintervals[0]);

    chi2_vs_nhits= new TH2F("chi2_vs_nhits","chi2 vs nhits",25,0,25,100,0,10);
    chi2_vs_eta  = new TH2F("chi2_vs_eta","chi2 vs eta",nint,min,max,100,0,10);
    nhits_vs_eta = new TH2F("nhits_vs_eta","nhits vs eta",nint,min,max,25,0,25);
    ptres_vs_eta = new TH2F("ptres_vs_eta","ptresidue vs eta",nint,min,max,200,-2,2);
    etares_vs_eta = new TH2F("etares_vs_eta","etaresidue vs eta",nint,min,max,200,-0.1,0.1);
  }

  virtual void analyze(const edm::Event& event, const edm::EventSetup& setup){
    //
    //get collections from the event
    //
    edm::Handle<SimTrackContainer> simTrackCollection;
    event.getByLabel(sim, simTrackCollection);
    const SimTrackContainer simTC = *(simTrackCollection.product());

    edm::Handle<reco::TrackCollection> trackCollection;
    event.getByLabel(label, trackCollection);
    const reco::TrackCollection tC = *(trackCollection.product());

    //
    //fill simulation histograms
    //
    h_tracksSIM->Fill(simTC.size());
    for (SimTrackContainer::const_iterator simTrack=simTC.begin(); simTrack!=simTC.end(); simTrack++){
      h_ptSIM->Fill(simTrack->momentum().perp());
      h_etaSIM->Fill(simTrack->momentum().pseudoRapidity());
//       if (simTC.size()>1){
// 	cout << "MULTITRACK EVENT" << endl;
// 	cout << "simTrack->momentum().perp()" << simTrack->momentum().perp() << endl;
// 	cout << "simTrack->momentum().pseudoRapidity()" << simTrack->momentum().pseudoRapidity() << endl;
//       }

      int i=0;
      for (vector<TH1F*>::iterator h=missingtracks.begin(); h!=missingtracks.end(); h++){
	if (abs(simTrack->momentum().pseudoRapidity())>etaintervals[i]&&
	    abs(simTrack->momentum().pseudoRapidity())<etaintervals[i+1]) {
	  (*h)->Fill(simTC.size()-tC.size());
	  tot[i]++;
	}
	i++;
      }
    }

    //
    //fill ctf histograms
    //
    h_tracks->Fill(tC.size());
    for (reco::TrackCollection::const_iterator track=tC.begin(); track!=tC.end(); track++){
      
      //nchi2 and hits global distributions
      h_nchi2->Fill(track->normalizedChi2());
      //      h_hitsCTF->Fill(track->numberOfValidHits());//found());
      h_hits->Fill(track->found());
      //       chi2_vs_nhits->Fill(track->numberOfValidHits(),track->normalizedChi2());
      chi2_vs_nhits->Fill(track->found(),track->normalizedChi2());
      chi2_vs_eta->Fill(track->eta(),track->normalizedChi2());
      //       nhits_vs_eta->Fill(track->eta(),track->numberOfValidHits());
      nhits_vs_eta->Fill(track->eta(),track->found());

      //pt and eta residue
      double ptres =1000;
      double etares=1000;
      for (SimTrackContainer::const_iterator simTrack=simTC.begin(); simTrack!=simTC.end(); simTrack++){
	double tmp=track->pt()-simTrack->momentum().perp();
	double tmp2=track->eta()-simTrack->momentum().pseudoRapidity();
	if (abs(tmp)<abs(ptres)) {
	  ptres=tmp; 
	  etares=tmp2;
	}
      }
      h_pt->Fill(ptres);
      h_eta->Fill(etares);
      ptres_vs_eta->Fill(track->eta(),ptres);
      etares_vs_eta->Fill(track->eta(),etares);

      //pt residue distribution per eta interval
      int i=0;
      for (vector<TH1F*>::iterator h=ptdistrib.begin(); h!=ptdistrib.end(); h++){
	for (SimTrackContainer::const_iterator simTrack=simTC.begin(); simTrack!=simTC.end(); simTrack++){
	  ptres=1000;
	  if (abs(simTrack->momentum().pseudoRapidity())>etaintervals[i]&&
	      abs(simTrack->momentum().pseudoRapidity())<etaintervals[i+1]) {
	    double tmp=track->pt()-simTrack->momentum().perp();
	    if (abs(tmp)<abs(ptres)) ptres=tmp;
	  }
	}
	(*h)->Fill(ptres);
	i++;
      }
      //eta residue distribution per eta interval
      i=0;
      for (vector<TH1F*>::iterator h=etadistrib.begin(); h!=etadistrib.end(); h++){
	for (SimTrackContainer::const_iterator simTrack=simTC.begin(); simTrack!=simTC.end(); simTrack++){
	  etares=1000; 
	  ptres =1000;
	  if (abs(simTrack->momentum().pseudoRapidity())>etaintervals[i]&&
	      abs(simTrack->momentum().pseudoRapidity())<etaintervals[i+1]) {
	    double tmp=track->pt()-simTrack->momentum().perp();
	    double tmp2=track->eta()-simTrack->momentum().pseudoRapidity();
	    if (abs(tmp)<abs(ptres)) etares=tmp2;
	  }
	}
	(*h)->Fill(etares);
	i++;
      }
    }


  }



  void endJob() {
    TFile hFile( out.c_str(), "UPDATE" );

    TDirectory * p = hFile.mkdir(label.c_str());

    //write simulation histos
    TDirectory * simD = p->mkdir("simulation");
    simD->cd();
    h_ptSIM->Write();
    h_etaSIM->Write();
    h_tracksSIM->Write();

    //fill efficiency plot versus eta and write missingtracks histos
    TDirectory * misD = p->mkdir("missingtracks");
    misD->cd();
    int i=0;
    for (vector<TH1F*>::iterator h=missingtracks.begin(); h!=missingtracks.end(); h++){
      if (tot[i]!=0) (*h)->Scale(1.0/(float)tot[i]);
      (*h)->Write();
      h_effic->Fill(etaintervals[i+1]-0.00001 ,(*h)->GetBinContent((*h)->FindBin(0)));
      i++;
    }

    //fill pt rms plot versus eta and write pt residue distribution per eta interval histo
    TDirectory * ptD = p->mkdir("ptdistribution");
    ptD->cd();
    i=0;
    for (vector<TH1F*>::iterator h=ptdistrib.begin(); h!=ptdistrib.end(); h++){
      (*h)->Write();
      h_ptrmsh->Fill(etaintervals[i+1]-0.00001 ,(*h)->GetRMS());
      i++;
    }

    //fill eta rms plot versus eta and write eta residue distribution per eta interval histo
    TDirectory * etaD = p->mkdir("etadistribution");
    etaD->cd();
    i=0;
    i=0;
    for (vector<TH1F*>::iterator h=etadistrib.begin(); h!=etadistrib.end(); h++){
      (*h)->Write();
      h_deltaeta->Fill(etaintervals[i+1]-0.00001 ,(*h)->GetRMS());
      i++;
    }

    //write the other histos
    p->cd();
    h_pt->Write();
    h_eta->Write();
    h_tracks->Write();
    h_nchi2->Write();
    h_hits->Write();
    h_effic->Write();
    h_ptrmsh->Write();
    h_deltaeta->Write();
    chi2_vs_nhits->Write();
    chi2_vs_eta->Write();
    nhits_vs_eta->Write();
    ptres_vs_eta->Write();
    etares_vs_eta->Write();

    //     gROOT->SetBatch();
    //     gROOT->SetStyle("Plain");
    //     TCanvas c;
    //     h_ptCTF.GetXaxis()->SetTitle( "p_{t} (GeV/c^{2})" );
    //     h_ptCTF.SetFillColor( kRed );
    //     h_ptCTF.SetLineWidth( 2 );
    //     h_ptCTF.Draw();
    //     c.SaveAs( "ptCTF.jpg" );
    
    hFile.Close();
  }

private:
  string sim,label,out;
  double  min,max;
  int nint;
  TH1F *h_ptSIM, *h_etaSIM, *h_tracksSIM;
  TH1F *h_pt, *h_eta, *h_tracks, *h_nchi2, *h_hits, *h_effic, *h_ptrmsh, *h_deltaeta;
  TH2F *chi2_vs_nhits, *chi2_vs_eta, *nhits_vs_eta, *ptres_vs_eta, *etares_vs_eta;
  vector<double> etaintervals;
  vector<int> tot;
  vector<TH1F*> missingtracks;
  vector<TH1F*> ptdistrib;
  vector<TH1F*> etadistrib;
 

};

DEFINE_SEAL_MODULE();
DEFINE_ANOTHER_FWK_MODULE(TrackValidator);


#ifndef _ChargedHadronSpectra_PlotRecTracks_h_
#define _ChargedHadronSpectra_PlotRecTracks_h_

#include <fstream>
#include <vector>

namespace edm { class Event; class EventSetup; }
class TrackingRecHit;
class TrackerGeometry;
class TrackerHitAssociator;

class PlotRecTracks
{
  public:
    explicit PlotRecTracks(const edm::EventSetup& es_,
                           std::string trackProducer_, std::ofstream& file_);
    ~PlotRecTracks();
    void printRecTracks(const edm::Event& ev);

  private:
    std::string getPixelInfo(const TrackingRecHit* recHit,
                             const std::ostringstream& o,
                             const std::ostringstream& d);
    std::string getStripInfo(const TrackingRecHit* recHit,
                             const std::ostringstream& o,
                             const std::ostringstream& d);
 
    const edm::EventSetup& es;
    std::string trackProducer;
    std::ofstream& file;
    const TrackerGeometry* theTracker;
    TrackerHitAssociator * theHitAssociator;
};

#endif

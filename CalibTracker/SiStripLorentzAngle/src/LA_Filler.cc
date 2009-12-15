#include "CalibTracker/SiStripLorentzAngle/interface/LA_Filler_Fitter.h"
#include "CalibTracker/SiStripCommon/interface/TTREE_FOREACH_ENTRY.hh"

#include <cmath>
#include <boost/lexical_cast.hpp>

void LA_Filler_Fitter::
fill(TTree* tree, Book& book) const {
  TTREE_FOREACH_ENTRY(tree) {
    TFE_MAX(maxEvents_);
    TFE_PRINTSTATUS;
    std::vector<unsigned> PLEAF( tsostrackmulti , tree );
    std::vector<unsigned> PLEAF( clusterdetid , tree );
    std::vector<unsigned> PLEAF( clusterwidth , tree );
    std::vector<float>    PLEAF( clustervariance , tree );
    std::vector<float>    PLEAF( tsosdriftx , tree );
    std::vector<float>    PLEAF( tsosdriftz , tree );
    std::vector<float>    PLEAF( tsoslocaltheta , tree );
    std::vector<float>    PLEAF( tsoslocalphi , tree );
    std::vector<float>    PLEAF( tsosglobalZofunitlocalY , tree );
    std::vector<float> BdotY; if(ensembleBins_) { std::vector<float> PLEAF( tsosBdotY , tree ); swap(BdotY, tsosBdotY); }
    std::vector<float> localy; if(localYbin_) { std::vector<float> PLEAF( tsoslocaly , tree ); swap(localy, tsoslocaly);}
    std::vector<unsigned> seedstrip; if(stripsPerBin_) { std::vector<unsigned> PLEAF( clusterseedstrip , tree ); swap(seedstrip, clusterseedstrip);}

    for(unsigned i=0; i< clusterdetid.size() ; i++) {

      const SiStripDetId detid(clusterdetid[i]);
      if( tsostrackmulti[i] != 1 || ( detid.subDetector()!=SiStripDetId::TIB && 
				      detid.subDetector()!=SiStripDetId::TOB)    )  continue;

      const int sign = tsosglobalZofunitlocalY[i] < 0 ? -1 : 1;

      fill_one_cluster( book, 
			detid, 
			clusterwidth[i], 
			clustervariance[i],
			sign * tsosdriftx[i] / tsosdriftz[i],
			sign * tan(tsoslocaltheta[i]) * cos(tsoslocalphi[i]),
			TFE_index ,
			ensembleBins_ ? fabs(BdotY[i]) : 0,
			localYbin_ ? localy[i] : 0,
			stripsPerBin_ ? seedstrip[i] % 128 : 0);
    }
  }
}

void LA_Filler_Fitter::
fill_one_cluster( Book& book, 
		  const SiStripDetId detid, 
		  const unsigned width, 
		  const float variance, 
		  const float tthetaL, 
		  const float tthetaT, 
		  const Long64_t TFE_index, 
		  const float BdotY, 
		  const float localy, 
		  const unsigned apvstrip) const 
{
  poly<std::string> gran = granularity(detid, tthetaL, TFE_index, localy, apvstrip);
  
  book.fill( width, gran+"_width", 10,0,10);
  book.fill( tthetaL,                   gran+"_reconstruction", 360,-1.0,1.0 );
  book.fill( tthetaT-tthetaL,           gran+ allAndOne(width), 360,-1.0,1.0 );
  book.fill( tthetaT-tthetaL, variance, gran+  varWidth(width), 360,-1.0,1.0 );
  if(methods_ & WIDTH) book.fill( tthetaT, width, gran+method(WIDTH), 81,-0.6,0.6 );
  if(ensembleBins_==0) book.fill( BdotY, gran+"_field", 101,1,5);
}

poly<std::string> LA_Filler_Fitter::
allAndOne(const unsigned width) const 
{ poly<std::string> a1("_all"); if(width==1) a1*="_w1"; return a1;}

poly<std::string> LA_Filler_Fitter::
varWidth(const unsigned width) const { 
  poly<std::string> vw; vw++; 
  if(width==2 && methods_ & (AVGV2|RMSV2) ) vw*=method(AVGV2,0);
  if(width==3 && methods_ & (AVGV3|RMSV3) ) vw*=method(AVGV3,0);
  return vw;
}

poly<std::string> LA_Filler_Fitter::
granularity(const SiStripDetId detid, const float tthetaL, const Long64_t TFE_index, const float localy, const unsigned apvstrip) const {
  poly<std::string> gran;
  gran += subdetLabel(detid);
  if(byLayer_)  gran *= layerLabel(detid);
  if(byModule_) gran *= moduleLabel(detid);
  if(localYbin_) gran += (localy < 0 ? "_yM":"_yP") + boost::lexical_cast<std::string>(abs((int)(localy/localYbin_+(localy<0?-1:0))));
  if(stripsPerBin_) gran += "_strip"+boost::lexical_cast<std::string>((unsigned)((0.5+((apvstrip/64)?(127-apvstrip):apvstrip)/stripsPerBin_)*stripsPerBin_) );
  if(ensembleBins_) {
    gran+= "_ensembleBin"+boost::lexical_cast<std::string>((int)(ensembleBins_*(tthetaL-ensembleLow_)/(ensembleUp_-ensembleLow_)));
    gran+= "";
    if(ensembleSize_) gran*= "_sample"+boost::lexical_cast<std::string>(TFE_index % ensembleSize_);
  }
  return gran;
}

std::string LA_Filler_Fitter::
subdetLabel(const SiStripDetId detid) { return detid.subDetector()==SiStripDetId::TOB? "TOB" : "TIB";}
std::string LA_Filler_Fitter::
moduleLabel(const SiStripDetId detid) { return subdetLabel(detid) + "_module"+boost::lexical_cast<std::string>(detid());}
std::string LA_Filler_Fitter::
layerLabel(const SiStripDetId detid) {
  unsigned layer = detid.subDetector() == SiStripDetId::TOB ? TOBDetId(detid()).layer() : TIBDetId(detid()).layer();
  return subdetLabel(detid)+"_layer"+boost::lexical_cast<std::string>(layer)+(detid.stereo()?"s":"a");
}

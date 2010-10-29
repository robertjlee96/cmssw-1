#ifndef __Cent_Bin_h__
#define __Cent_Bin_h__

#include <TNamed.h>
#include <TFile.h>
#include <vector>
#include <map>

class CBin : public TObject {
 public:
   CBin(){;}
   ~CBin(){;}

   float bin_edge;
   float n_part_mean;
   float n_part_var;
   float n_coll_mean;
   float n_coll_var;
   float n_hard_mean;
   float n_hard_var;
   float b_mean;
   float b_var;
   ClassDef(CBin,1)
};

class CentralityBins : public TNamed {
   
 public:
   typedef std::map<int, const CentralityBins*> RunMap;

   CentralityBins(){;}
   CentralityBins(const char* name, const char* title, int nbins) : TNamed(name,title) {
      table_.reserve(nbins);
      for(int j = 0; j < nbins; ++j){
	 CBin b;
	 table_.push_back(b); 
      }
   }
      ~CentralityBins() {;}
      int getBin(double value) const;
      int getNbins() const {return table_.size();}
      float lowEdge(double value) const { return lowEdgeOfBin(getBin(value));}
      float lowEdgeOfBin(int bin) const { return table_[bin].bin_edge;}
      float NpartMean(double value) const { return NpartMeanOfBin(getBin(value));}
      float NpartMeanOfBin(int bin) const { return table_[bin].n_part_mean;}
      float NpartSigma(double value) const { return NpartSigmaOfBin(getBin(value));}
      float NpartSigmaOfBin(int bin) const { return table_[bin].n_part_var;}
      float NcollMean(double value) const { return NcollMeanOfBin(getBin(value));}
      float NcollMeanOfBin(int bin) const { return table_[bin].n_coll_mean;}
      float NcollSigma(double value) const { return NcollSigmaOfBin(getBin(value));}
      float NcollSigmaOfBin(int bin) const { return table_[bin].n_coll_var;}
      float NhardMean(double value) const { return NhardMeanOfBin(getBin(value));}
      float NhardMeanOfBin(int bin) const { return table_[bin].n_hard_mean;}
      float NhardSigma(double value) const { return NhardSigmaOfBin(getBin(value));}
      float NhardSigmaOfBin(int bin) const { return table_[bin].n_hard_var;}
      float bMean(double value) const { return bMeanOfBin(getBin(value));}
      float bMeanOfBin(int bin) const { return table_[bin].b_mean;}
      float bSigma(double value) const { return bSigmaOfBin(getBin(value));}
      float bSigmaOfBin(int bin) const { return table_[bin].b_var;}

      // private:
      std::vector<CBin> table_;
      ClassDef(CentralityBins,1)
};

CentralityBins::RunMap getCentralityFromFile(TDirectoryFile*, const char* dir, const char* tag, int firstRun = 0, int lastRun = 10);
CentralityBins::RunMap getCentralityFromFile(TDirectoryFile*, const char* tag, int firstRun = 0, int lastRun = 10);





#endif

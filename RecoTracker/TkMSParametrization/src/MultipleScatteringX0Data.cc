#include "RecoTracker/TkMSParametrization/interface/MultipleScatteringX0Data.h"

#include "FWCore/ParameterSet/interface/FileInPath.h"

#include "TH2.h"
#include "TFile.h"
#include "TKey.h"

#include <iostream>
#include <string>

using namespace std;


MultipleScatteringX0Data::MultipleScatteringX0Data()
  : theFile(0), theData(0)
{
  string filename;
  try {
    filename = fileName(); 
    theFile = new TFile(filename.c_str(),"READ");
  } 
  catch (...) {
    cout <<" MultipleScatteringX0Data, no input file found"<<endl;
  }
  if (theFile) {
    theData = dynamic_cast<TH2F*> (theFile->GetKey("h100")->ReadObj());
  }
  if (!theData)  {
    cout << " ** MultipleScatteringX0Data ** file: "
         << filename 
         <<" <-- no data found!!!"<<endl;
  }
}

MultipleScatteringX0Data::~MultipleScatteringX0Data()
{
  if(theFile) {
    theFile->Close();
    delete theFile;
  }
  if (theData){
    delete theData;
  }
}

string MultipleScatteringX0Data::fileName()
{
  string defName="RecoTracker/TkMSParametrization/data/MultipleScatteringX0Data.root";
  edm::FileInPath f(defName);
  return f.fullPath();
} 

int MultipleScatteringX0Data::nBinsEta() const
{
  if (theData) return theData->GetNbinsX();
  else return 0;
}

float MultipleScatteringX0Data::minEta() const
{
  if (theData) return theData->GetXaxis()->GetXmin();
  else return 0;
}

float MultipleScatteringX0Data::maxEta() const
{
  if (theData) return theData->GetXaxis()->GetXmax();
  else return 0;
}

float MultipleScatteringX0Data::sumX0atEta(float eta, float r) const
{
  if(!theData) return 0.;
  eta = fabs(eta);

  int ieta = theData->GetXaxis()->FindBin(eta);
  int irad = theData->GetYaxis()->FindBin(r);

  if (irad < theData->GetNbinsY()) { 
    return theData->GetBinContent(ieta,irad);
  } 
  else {
    float sumX0 = 0;
    for(int ir = theData->GetNbinsY(); ir > 0; ir--) {
      float val = theData->GetBinContent(ieta, ir);
      if (val > sumX0) sumX0 = val;
    }
    return sumX0;
  }
}

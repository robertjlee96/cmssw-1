#ifndef PValidationFormats_h
#define PValidationFormats_h

///////////////////////////////////////////////////////////////////////////////
// PGlobalSimHit
///////////////////////////////////////////////////////////////////////////////
#ifndef PGlobalSimHit_h
#define PGlobalSimHit_h

/** \class PGlobalSimHit
 *  
 *  DataFormat class to hold the information for the Global Hit Validation
 *
 *  $Date: 2007/04/30 19:41:24 $
 *  $Revision: 1.2 $
 *  \author M. Strang SUNY-Buffalo
 */

#include <vector>
#include <memory>

class PGlobalSimHit
{

 public:

  PGlobalSimHit(): nRawGenPart(0), nG4Vtx(0), nG4Trk(0),  
    nECalHits(0), nPreShHits(0), nHCalHits(0), nPxlFwdHits(0),
    nPxlBrlHits(0), nSiFwdHits(0), nSiBrlHits(0),
    nMuonDtHits(0), nMuonCscHits(0), nMuonRpcFwdHits(0),
    nMuonRpcBrlHits(0) {}
  virtual ~PGlobalSimHit(){}

  struct Vtx
  {
    Vtx(): x(0), y(0), z(0) {}
    float x;
    float y;
    float z;
  };

  struct Trk
  {
    Trk() : pt(0), e(0) {}
    float pt;
    float e;
  };

  struct CalHit
  {
    CalHit() : e(0), tof(0), phi(0), eta(0) {}
    float e;
    float tof;
    float phi;
    float eta;
  };

  struct FwdHit
  {
    FwdHit() : tof(0), z(0), phi(0), eta(0) {}
    float tof;
    float z;
    float phi;
    float eta;
  };

  struct BrlHit
  {
    BrlHit() : tof(0), r(0), phi(0), eta(0) {}
    float tof;
    float r;
    float phi;
    float eta;
  };

  typedef std::vector<Vtx> VtxVector;
  typedef std::vector<Trk> TrkVector;
  typedef std::vector<CalHit> CalVector;
  typedef std::vector<FwdHit> FwdVector;
  typedef std::vector<BrlHit> BrlVector;

  // put functions
  void putRawGenPart(int n);
  void putG4Vtx(std::vector<float> x, std::vector<float> y, 
		 std::vector<float> z);
  void putG4Trk(std::vector<float> pt, std::vector<float> e);
  void putECalHits(std::vector<float> e, std::vector<float> tof,
		    std::vector<float> phi, std::vector<float> eta);
  void putPreShHits(std::vector<float> e, std::vector<float> tof,
		     std::vector<float> phi, std::vector<float> eta);
  void putHCalHits(std::vector<float> e, std::vector<float> tof,
		    std::vector<float> phi, std::vector<float> eta);
  void putPxlFwdHits(std::vector<float> tof, std::vector<float> z,
		       std::vector<float> phi, std::vector<float> eta);
  void putPxlBrlHits(std::vector<float> tof, std::vector<float> r,
		      std::vector<float> phi, std::vector<float> eta);
  void putSiFwdHits(std::vector<float> tof, std::vector<float> z,
		      std::vector<float> phi, std::vector<float> eta);
  void putSiBrlHits(std::vector<float> tof, std::vector<float> r,
		     std::vector<float> phi, std::vector<float> eta);
  void putMuonCscHits(std::vector<float> tof, std::vector<float> z,
		       std::vector<float> phi, std::vector<float> eta);
  void putMuonDtHits(std::vector<float> tof, std::vector<float> r,
		      std::vector<float> phi, std::vector<float> eta);
  void putMuonRpcFwdHits(std::vector<float> tof, std::vector<float> z,
			   std::vector<float> phi, std::vector<float> eta);
  void putMuonRpcBrlHits(std::vector<float> tof, std::vector<float> r,
			  std::vector<float> phi, std::vector<float> eta);  


  // get functions
  int getnRawGenPart() {return nRawGenPart;}
  int getnG4Vtx() {return nG4Vtx;}
  VtxVector getG4Vtx() {return G4Vtx;}
  int getnG4Trk() {return nG4Trk;}
  TrkVector getG4Trk() {return G4Trk;}
  int getnECalHits() {return nECalHits;}
  CalVector getECalHits() {return ECalHits;}
  int getnPreShHits() {return nPreShHits;}
  CalVector getPreShHits() {return PreShHits;}
  int getnHCalHits() {return nHCalHits;}
  CalVector getHCalHits() {return HCalHits;}
  int getnPxlFwdHits() {return nPxlFwdHits;}
  FwdVector getPxlFwdHits() {return PxlFwdHits;}
  int getnPxlBrlHits() {return nPxlBrlHits;}
  BrlVector getPxlBrlHits() {return PxlBrlHits;}
  int getnSiFwdHits() {return nSiFwdHits;}
  FwdVector getSiFwdHits() {return SiFwdHits;}
  int getnSiBrlHits() {return nSiBrlHits;}
  BrlVector getSiBrlHits() {return SiBrlHits;}  
  int getnMuonDtHits() {return nMuonDtHits;}
  BrlVector getMuonDtHits() {return MuonDtHits;}
  int getnMuonCscHits() {return nMuonCscHits;}
  FwdVector getMuonCscHits() {return MuonCscHits;}
  int getnMuonRpcFwdHits() {return nMuonRpcFwdHits;}
  FwdVector getMuonRpcFwdHits() {return MuonRpcFwdHits;}
  int getnMuonRpcBrlHits() {return nMuonRpcBrlHits;}
  BrlVector getMuonRpcBrlHits() {return MuonRpcBrlHits;}  

 private:

  // G4MC info
  int nRawGenPart;
  int nG4Vtx;
  VtxVector G4Vtx; 
  int nG4Trk; 
  TrkVector G4Trk; 

  // ECal info
  int nECalHits;
  CalVector ECalHits; 
  int nPreShHits;
  CalVector PreShHits; 

  // HCal info
  int nHCalHits;
  CalVector HCalHits;

  // Tracker info
  int nPxlFwdHits;
  FwdVector PxlFwdHits; 
  int nPxlBrlHits;
  BrlVector PxlBrlHits;
  int nSiFwdHits;
  FwdVector SiFwdHits; 
  int nSiBrlHits;
  BrlVector SiBrlHits;  

  // Muon info
  int nMuonDtHits;
  BrlVector MuonDtHits;
  int nMuonCscHits;
  FwdVector MuonCscHits;
  int nMuonRpcFwdHits;
  FwdVector MuonRpcFwdHits;  
  int nMuonRpcBrlHits;
  BrlVector MuonRpcBrlHits;

}; // end class declaration

#endif // endif PGlobalHit_h

///////////////////////////////////////////////////////////////////////////////
// PGlobalDigi
///////////////////////////////////////////////////////////////////////////////

#ifndef PGlobalDigi_h
#define PGlobalDigi_h

class PGlobalDigi
{
 public:

  PGlobalDigi(): nEBCalDigis(0), nEECalDigis(0), nESCalDigis(0),
    nHBCalDigis(0), nHECalDigis(0), nHOCalDigis(0), nHFCalDigis(0),
    nTIBL1Digis(0), nTIBL2Digis(0), nTIBL3Digis(0), nTIBL4Digis(0),
    nTOBL1Digis(0), nTOBL2Digis(0), nTOBL3Digis(0), nTOBL4Digis(0),
    nTIDW1Digis(0), nTIDW2Digis(0), nTIDW3Digis(0),
    nTECW1Digis(0), nTECW2Digis(0), nTECW3Digis(0), nTECW4Digis(0), 
    nTECW5Digis(0), nTECW6Digis(0), nTECW7Digis(0), nTECW8Digis(0),
    nBRL1Digis(0), nBRL2Digis(0), nBRL3Digis(0), 
    nFWD1pDigis(0), nFWD1nDigis(0), nFWD2pDigis(0), nFWD2nDigis(0),
    nMB1Digis(0), nMB2Digis(0), nMB3Digis(0), nMB4Digis(0),
    nCSCstripDigis(0), nCSCwireDigis(0) {}
  virtual ~PGlobalDigi(){}

  ////////////
  // ECal Info
  ////////////
  struct ECalDigi
  {
    ECalDigi(): maxPos(0), AEE(0), SHE(0) {}
    int maxPos;
    double AEE; //maximum analog equivalent energy
    float SHE; //simhit energy sum
  };
  typedef std::vector<ECalDigi> ECalDigiVector;
  struct ESCalDigi
  {
    ESCalDigi(): ADC0(0), ADC1(0), ADC2(0), SHE(0) {}
    float ADC0, ADC1, ADC2; //ADC counts
    float SHE; //sum simhit energy    
  };
  typedef std::vector<ESCalDigi> ESCalDigiVector;
  //put functions
  void putEBCalDigis(std::vector<int> maxpos,
		     std::vector<double> aee, std::vector<float> she);
  void putEECalDigis(std::vector<int> maxpos,
		     std::vector<double> aee, std::vector<float> she);
  void putESCalDigis(std::vector<float> adc0, std::vector<float> adc1,
		     std::vector<float> adc2, std::vector<float> she);
  //get functions
  int getnEBCalDigis() {return nEBCalDigis;}  
  int getnEECalDigis() {return nEECalDigis;}
  int getnESCalDigis() {return nESCalDigis;}  
  ECalDigiVector getEBCalDigis() {return EBCalDigis;}  
  ECalDigiVector getEECalDigis() {return EECalDigis;}
  ESCalDigiVector getESCalDigis() {return ESCalDigis;}  

  ////////////
  // HCal Info
  ////////////
  struct HCalDigi
  {
    HCalDigi(): AEE(0), SHE(0) {}
    float AEE; //sum analog equivalent energy in fC
    float SHE; //simhit energy sum
  };
  typedef std::vector<HCalDigi> HCalDigiVector;
  //put functions
  void putHBCalDigis(std::vector<float> aee, std::vector<float> she);
  void putHECalDigis(std::vector<float> aee, std::vector<float> she);
  void putHOCalDigis(std::vector<float> aee, std::vector<float> she);
  void putHFCalDigis(std::vector<float> aee, std::vector<float> she);
  //get functions
  int getnHBCalDigis() {return nHBCalDigis;}  
  int getnHECalDigis() {return nHECalDigis;}  
  int getnHOCalDigis() {return nHOCalDigis;}  
  int getnHFCalDigis() {return nHFCalDigis;}  
  HCalDigiVector getHBCalDigis() {return HBCalDigis;}  
  HCalDigiVector getHECalDigis() {return HECalDigis;}  
  HCalDigiVector getHOCalDigis() {return HOCalDigis;}  
  HCalDigiVector getHFCalDigis() {return HFCalDigis;}  

  ////////////////////////
  // Silicon Tracker info
  ///////////////////////

  ///////////////
  // SiStrip info
  ///////////////
  struct SiStripDigi
  {
    SiStripDigi(): ADC(0), STRIP(0) {}
    float ADC; //adc value
    int STRIP; //strip number
  };
  typedef std::vector<SiStripDigi> SiStripDigiVector;
  //put functions
  void putTIBL1Digis(std::vector<float> adc, std::vector<int> strip);
  void putTIBL2Digis(std::vector<float> adc, std::vector<int> strip);
  void putTIBL3Digis(std::vector<float> adc, std::vector<int> strip);
  void putTIBL4Digis(std::vector<float> adc, std::vector<int> strip);
  void putTOBL1Digis(std::vector<float> adc, std::vector<int> strip);
  void putTOBL2Digis(std::vector<float> adc, std::vector<int> strip);
  void putTOBL3Digis(std::vector<float> adc, std::vector<int> strip);
  void putTOBL4Digis(std::vector<float> adc, std::vector<int> strip);
  void putTIDW1Digis(std::vector<float> adc, std::vector<int> strip);
  void putTIDW2Digis(std::vector<float> adc, std::vector<int> strip);
  void putTIDW3Digis(std::vector<float> adc, std::vector<int> strip);
  void putTECW1Digis(std::vector<float> adc, std::vector<int> strip);
  void putTECW2Digis(std::vector<float> adc, std::vector<int> strip);
  void putTECW3Digis(std::vector<float> adc, std::vector<int> strip);
  void putTECW4Digis(std::vector<float> adc, std::vector<int> strip);
  void putTECW5Digis(std::vector<float> adc, std::vector<int> strip);
  void putTECW6Digis(std::vector<float> adc, std::vector<int> strip);
  void putTECW7Digis(std::vector<float> adc, std::vector<int> strip);
  void putTECW8Digis(std::vector<float> adc, std::vector<int> strip);
  //get functions
  int getnTIBL1Digis() {return nTIBL1Digis;}  
  int getnTIBL2Digis() {return nTIBL2Digis;}  
  int getnTIBL3Digis() {return nTIBL3Digis;}  
  int getnTIBL4Digis() {return nTIBL4Digis;}  
  int getnTOBL1Digis() {return nTOBL1Digis;}  
  int getnTOBL2Digis() {return nTOBL2Digis;}  
  int getnTOBL3Digis() {return nTOBL3Digis;}  
  int getnTOBL4Digis() {return nTOBL4Digis;}
  int getnTIDW1Digis() {return nTIDW1Digis;}
  int getnTIDW2Digis() {return nTIDW2Digis;}
  int getnTIDW3Digis() {return nTIDW3Digis;} 
  int getnTECW1Digis() {return nTECW1Digis;}
  int getnTECW2Digis() {return nTECW2Digis;}
  int getnTECW3Digis() {return nTECW3Digis;}
  int getnTECW4Digis() {return nTECW4Digis;}
  int getnTECW5Digis() {return nTECW5Digis;}
  int getnTECW6Digis() {return nTECW6Digis;}
  int getnTECW7Digis() {return nTECW7Digis;}
  int getnTECW8Digis() {return nTECW8Digis;} 
  SiStripDigiVector getTIBL1Digis() {return TIBL1Digis;}  
  SiStripDigiVector getTIBL2Digis() {return TIBL2Digis;}  
  SiStripDigiVector getTIBL3Digis() {return TIBL3Digis;}  
  SiStripDigiVector getTIBL4Digis() {return TIBL4Digis;}
  SiStripDigiVector getTOBL1Digis() {return TOBL1Digis;}  
  SiStripDigiVector getTOBL2Digis() {return TOBL2Digis;}  
  SiStripDigiVector getTOBL3Digis() {return TOBL3Digis;}  
  SiStripDigiVector getTOBL4Digis() {return TOBL4Digis;}   
  SiStripDigiVector getTIDW1Digis() {return TIDW1Digis;}
  SiStripDigiVector getTIDW2Digis() {return TIDW2Digis;}
  SiStripDigiVector getTIDW3Digis() {return TIDW3Digis;} 
  SiStripDigiVector getTECW1Digis() {return TECW1Digis;}
  SiStripDigiVector getTECW2Digis() {return TECW2Digis;}
  SiStripDigiVector getTECW3Digis() {return TECW3Digis;}
  SiStripDigiVector getTECW4Digis() {return TECW4Digis;}
  SiStripDigiVector getTECW5Digis() {return TECW5Digis;}
  SiStripDigiVector getTECW6Digis() {return TECW6Digis;}
  SiStripDigiVector getTECW7Digis() {return TECW7Digis;}
  SiStripDigiVector getTECW8Digis() {return TECW8Digis;}

  ///////////////
  // SiPixel info
  ///////////////
  struct SiPixelDigi
  {
    SiPixelDigi(): ADC(0), ROW(0), COLUMN(0) {}
    float ADC; //adc value
    int ROW; //row number
    int COLUMN; //column number
  };
  typedef std::vector<SiPixelDigi> SiPixelDigiVector;
  //put functions
  void putBRL1Digis(std::vector<float> adc, std::vector<int> row,
		    std::vector<int> column);
  void putBRL2Digis(std::vector<float> adc, std::vector<int> row,
		    std::vector<int> column);
  void putBRL3Digis(std::vector<float> adc, std::vector<int> row,
		    std::vector<int> column);
  void putFWD1pDigis(std::vector<float> adc, std::vector<int> row,
		    std::vector<int> column);
  void putFWD1nDigis(std::vector<float> adc, std::vector<int> row,
		    std::vector<int> column);
  void putFWD2pDigis(std::vector<float> adc, std::vector<int> row,
		    std::vector<int> column);
  void putFWD2nDigis(std::vector<float> adc, std::vector<int> row,
		    std::vector<int> column);
  //get functions
  int getnBRL1Digis() {return nBRL1Digis;}  
  int getnBRL2Digis() {return nBRL2Digis;}  
  int getnBRL3Digis() {return nBRL3Digis;}
  int getnFWD1pDigis() {return nFWD1pDigis;}  
  int getnFWD1nDigis() {return nFWD1nDigis;}    
  int getnFWD2pDigis() {return nFWD2pDigis;}  
  int getnFWD2nDigis() {return nFWD2nDigis;}  
  SiPixelDigiVector getBRL1Digis() {return BRL1Digis;}  
  SiPixelDigiVector getBRL2Digis() {return BRL2Digis;}  
  SiPixelDigiVector getBRL3Digis() {return BRL3Digis;}  
  SiPixelDigiVector getFWD1pDigis() {return FWD1pDigis;}
  SiPixelDigiVector getFWD1nDigis() {return FWD1nDigis;} 
  SiPixelDigiVector getFWD2pDigis() {return FWD2pDigis;}
  SiPixelDigiVector getFWD2nDigis() {return FWD2nDigis;} 

  ////////////
  // Muon info
  ////////////

  //////////
  // DT Info
  ////////// 
  struct DTDigi
  {
    DTDigi(): SLAYER(0), TIME(0), LAYER(0) {}
    int SLAYER; //superlayer number
    float TIME; //time of hit
    int LAYER; //layer number
  };
  typedef std::vector<DTDigi> DTDigiVector;
  //put functions
  void putMB1Digis(std::vector<int> slayer, std::vector<float> time, 
		   std::vector<int> layer);
  void putMB2Digis(std::vector<int> slayer, std::vector<float> time, 
		   std::vector<int> layer);
  void putMB3Digis(std::vector<int> slayer, std::vector<float> time, 
		   std::vector<int> layer);
  void putMB4Digis(std::vector<int> slayer, std::vector<float> time, 
		   std::vector<int> layer);
  //get functions
  int getnMB1Digis() {return nMB1Digis;}  
  int getnMB2Digis() {return nMB2Digis;}  
  int getnMB3Digis() {return nMB3Digis;}  
  int getnMB4Digis() {return nMB4Digis;}  
  DTDigiVector getMB1Digis() {return MB1Digis;}  
  DTDigiVector getMB2Digis() {return MB2Digis;}  
  DTDigiVector getMB3Digis() {return MB3Digis;}  
  DTDigiVector getMB4Digis() {return MB4Digis;}  

  /////////////////
  // CSC Strip info
  /////////////////
  struct CSCstripDigi
  {
    CSCstripDigi(): ADC(0) {}
    float ADC; //ped subtracted amplitude
  };
  typedef std::vector<CSCstripDigi> CSCstripDigiVector;
  //put functions
  void putCSCstripDigis(std::vector<float> adc);
  //get functions
  int getnCSCstripDigis() {return nCSCstripDigis;}  
  CSCstripDigiVector getCSCstripDigis() {return CSCstripDigis;}  

  /////////////////
  // CSC Wire info
  /////////////////
  struct CSCwireDigi
  {
    CSCwireDigi(): TIME(0) {}
    float TIME; //time
  };
  typedef std::vector<CSCwireDigi> CSCwireDigiVector;
  //put functions
  void putCSCwireDigis(std::vector<float> time);
  //get functions
  int getnCSCwireDigis() {return nCSCwireDigis;}  
  CSCwireDigiVector getCSCwireDigis() {return CSCwireDigis;} 

 private:

  ////////////
  // ECal info
  ////////////
  int nEBCalDigis;
  ECalDigiVector EBCalDigis;
  int nEECalDigis;
  ECalDigiVector EECalDigis;
  int nESCalDigis;
  ESCalDigiVector ESCalDigis;

  ////////////
  // HCal info
  ////////////
  int nHBCalDigis;
  HCalDigiVector HBCalDigis;
  int nHECalDigis;
  HCalDigiVector HECalDigis;
  int nHOCalDigis;
  HCalDigiVector HOCalDigis;
  int nHFCalDigis;
  HCalDigiVector HFCalDigis;

  ////////////////////////
  // Silicon Tracker info
  ///////////////////////

  //////////////
  //SiStrip info
  //////////////
  int nTIBL1Digis;  
  SiStripDigiVector TIBL1Digis;
  int nTIBL2Digis;  
  SiStripDigiVector TIBL2Digis;
  int nTIBL3Digis; 
  SiStripDigiVector TIBL3Digis;
  int nTIBL4Digis;  
  SiStripDigiVector TIBL4Digis;
  int nTOBL1Digis;
  SiStripDigiVector TOBL1Digis;
  int nTOBL2Digis;  
  SiStripDigiVector TOBL2Digis;
  int nTOBL3Digis;  
  SiStripDigiVector TOBL3Digis;
  int nTOBL4Digis; 
  SiStripDigiVector TOBL4Digis;
  int nTIDW1Digis;   
  SiStripDigiVector TIDW1Digis;
  int nTIDW2Digis;
  SiStripDigiVector TIDW2Digis;
  int nTIDW3Digis;
  SiStripDigiVector TIDW3Digis; 
  int nTECW1Digis;
  SiStripDigiVector TECW1Digis;
  int nTECW2Digis;
  SiStripDigiVector TECW2Digis;
  int nTECW3Digis;
  SiStripDigiVector TECW3Digis;
  int nTECW4Digis;
  SiStripDigiVector TECW4Digis;
  int nTECW5Digis;
  SiStripDigiVector TECW5Digis;
  int nTECW6Digis;
  SiStripDigiVector TECW6Digis;
  int nTECW7Digis;
  SiStripDigiVector TECW7Digis;
  int nTECW8Digis;
  SiStripDigiVector TECW8Digis;

  //////////////
  //SiPixel info
  //////////////
  int nBRL1Digis;
  SiPixelDigiVector BRL1Digis;
  int nBRL2Digis;  
  SiPixelDigiVector BRL2Digis; 
  int nBRL3Digis; 
  SiPixelDigiVector BRL3Digis; 
  int nFWD1pDigis; 
  SiPixelDigiVector FWD1pDigis;
  int nFWD1nDigis;
  SiPixelDigiVector FWD1nDigis; 
  int nFWD2pDigis;
  SiPixelDigiVector FWD2pDigis;
  int nFWD2nDigis;
  SiPixelDigiVector FWD2nDigis; 

  ////////////
  // Muon info
  ////////////

  //////////
  // DT Info
  ////////// 
  int nMB1Digis;
  DTDigiVector MB1Digis; 
  int nMB2Digis;
  DTDigiVector MB2Digis; 
  int nMB3Digis;
  DTDigiVector MB3Digis; 
  int nMB4Digis; 
  DTDigiVector MB4Digis; 

  /////////////////
  // CSC Strip info
  ////////////////
  int nCSCstripDigis;
  CSCstripDigiVector CSCstripDigis;

  /////////////////
  // CSC Wire info
  ////////////////
  int nCSCwireDigis;
  CSCwireDigiVector CSCwireDigis;
 
}; // end class declaration

#endif //PGlobalDigiHit_h

///////////////////////////////////////////////////////////////////////////////
// PGlobalRecHit
///////////////////////////////////////////////////////////////////////////////

#ifndef PGlobalRecHit_h
#define PGlobalRecHit_h

class PGlobalRecHit
{
 public:

  PGlobalRecHit(): nEBCalRecHits(0), nEECalRecHits(0), nESCalRecHits(0),
    nHBCalRecHits(0), nHECalRecHits(0), nHOCalRecHits(0), nHFCalRecHits(0),
    nTIBL1RecHits(0), nTIBL2RecHits(0), nTIBL3RecHits(0), nTIBL4RecHits(0),
    nTOBL1RecHits(0), nTOBL2RecHits(0), nTOBL3RecHits(0), nTOBL4RecHits(0),
    nTIDW1RecHits(0), nTIDW2RecHits(0), nTIDW3RecHits(0),
    nTECW1RecHits(0), nTECW2RecHits(0), nTECW3RecHits(0), nTECW4RecHits(0), 
    nTECW5RecHits(0), nTECW6RecHits(0), nTECW7RecHits(0), nTECW8RecHits(0),
    nBRL1RecHits(0), nBRL2RecHits(0), nBRL3RecHits(0), 
    nFWD1pRecHits(0), nFWD1nRecHits(0), nFWD2pRecHits(0), nFWD2nRecHits(0),
    nDTRecHits(0), nCSCRecHits(0), nRPCRecHits(0) {}
  virtual ~PGlobalRecHit(){}

  ////////////
  // ECal Info
  ////////////
  struct ECalRecHit
  {
    ECalRecHit(): RE(0), SHE(0) {}
    float RE; //reconstructed energy
    float SHE; //simhit energy
  };
  typedef std::vector<ECalRecHit> ECalRecHitVector;
  //put functions
  void putEBCalRecHits(std::vector<float> re, std::vector<float> she);
  void putEECalRecHits(std::vector<float> re, std::vector<float> she);
  void putESCalRecHits(std::vector<float> re, std::vector<float> she);
  //get functions
  int getnEBCalRecHits() {return nEBCalRecHits;}  
  int getnEECalRecHits() {return nEECalRecHits;}
  int getnESCalRecHits() {return nESCalRecHits;}  
  ECalRecHitVector getEBCalRecHits() {return EBCalRecHits;}  
  ECalRecHitVector getEECalRecHits() {return EECalRecHits;}
  ECalRecHitVector getESCalRecHits() {return ESCalRecHits;}  

  ////////////
  // HCal Info
  ////////////
  struct HCalRecHit
  {
    HCalRecHit(): REC(0), R(0), SHE(0) {}
    float REC; // reconstructed energy
    float R;   // distance in cone 
    float SHE; // simhit energy
  };
  typedef std::vector<HCalRecHit> HCalRecHitVector;
  //put functions
  void putHBCalRecHits(std::vector<float> rec, std::vector<float> r, 
		       std::vector<float> she);
  void putHECalRecHits(std::vector<float> rec, std::vector<float> r, 
		       std::vector<float> she);
  void putHOCalRecHits(std::vector<float> rec, std::vector<float> r, 
		       std::vector<float> she);
  void putHFCalRecHits(std::vector<float> rec, std::vector<float> r, 
		       std::vector<float> she);
  //get functions
  int getnHBCalRecHits() {return nHBCalRecHits;}  
  int getnHECalRecHits() {return nHECalRecHits;}  
  int getnHOCalRecHits() {return nHOCalRecHits;}  
  int getnHFCalRecHits() {return nHFCalRecHits;}  
  HCalRecHitVector getHBCalRecHits() {return HBCalRecHits;}  
  HCalRecHitVector getHECalRecHits() {return HECalRecHits;}  
  HCalRecHitVector getHOCalRecHits() {return HOCalRecHits;}  
  HCalRecHitVector getHFCalRecHits() {return HFCalRecHits;}  

  ////////////////////////
  // Silicon Tracker info
  ///////////////////////

  ///////////////
  // SiStrip info
  ///////////////
  struct SiStripRecHit
  {
    SiStripRecHit(): RX(0), RY(0), SX(0), SY(0) {}
    float RX; //reconstructed x
    float RY; //reconstructed y
    float SX; //simulated x
    float SY; //simulated y
  };
  typedef std::vector<SiStripRecHit> SiStripRecHitVector;
  //put functions
  void putTIBL1RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTIBL2RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTIBL3RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTIBL4RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTOBL1RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTOBL2RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTOBL3RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTOBL4RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTIDW1RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTIDW2RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTIDW3RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTECW1RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTECW2RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTECW3RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTECW4RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTECW5RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTECW6RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTECW7RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putTECW8RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  //get functions
  int getnTIBL1RecHits() {return nTIBL1RecHits;}  
  int getnTIBL2RecHits() {return nTIBL2RecHits;}  
  int getnTIBL3RecHits() {return nTIBL3RecHits;}  
  int getnTIBL4RecHits() {return nTIBL4RecHits;}  
  int getnTOBL1RecHits() {return nTOBL1RecHits;}  
  int getnTOBL2RecHits() {return nTOBL2RecHits;}  
  int getnTOBL3RecHits() {return nTOBL3RecHits;}  
  int getnTOBL4RecHits() {return nTOBL4RecHits;}
  int getnTIDW1RecHits() {return nTIDW1RecHits;}
  int getnTIDW2RecHits() {return nTIDW2RecHits;}
  int getnTIDW3RecHits() {return nTIDW3RecHits;} 
  int getnTECW1RecHits() {return nTECW1RecHits;}
  int getnTECW2RecHits() {return nTECW2RecHits;}
  int getnTECW3RecHits() {return nTECW3RecHits;}
  int getnTECW4RecHits() {return nTECW4RecHits;}
  int getnTECW5RecHits() {return nTECW5RecHits;}
  int getnTECW6RecHits() {return nTECW6RecHits;}
  int getnTECW7RecHits() {return nTECW7RecHits;}
  int getnTECW8RecHits() {return nTECW8RecHits;} 
  SiStripRecHitVector getTIBL1RecHits() {return TIBL1RecHits;}  
  SiStripRecHitVector getTIBL2RecHits() {return TIBL2RecHits;}  
  SiStripRecHitVector getTIBL3RecHits() {return TIBL3RecHits;}  
  SiStripRecHitVector getTIBL4RecHits() {return TIBL4RecHits;}
  SiStripRecHitVector getTOBL1RecHits() {return TOBL1RecHits;}  
  SiStripRecHitVector getTOBL2RecHits() {return TOBL2RecHits;}  
  SiStripRecHitVector getTOBL3RecHits() {return TOBL3RecHits;}  
  SiStripRecHitVector getTOBL4RecHits() {return TOBL4RecHits;}   
  SiStripRecHitVector getTIDW1RecHits() {return TIDW1RecHits;}
  SiStripRecHitVector getTIDW2RecHits() {return TIDW2RecHits;}
  SiStripRecHitVector getTIDW3RecHits() {return TIDW3RecHits;} 
  SiStripRecHitVector getTECW1RecHits() {return TECW1RecHits;}
  SiStripRecHitVector getTECW2RecHits() {return TECW2RecHits;}
  SiStripRecHitVector getTECW3RecHits() {return TECW3RecHits;}
  SiStripRecHitVector getTECW4RecHits() {return TECW4RecHits;}
  SiStripRecHitVector getTECW5RecHits() {return TECW5RecHits;}
  SiStripRecHitVector getTECW6RecHits() {return TECW6RecHits;}
  SiStripRecHitVector getTECW7RecHits() {return TECW7RecHits;}
  SiStripRecHitVector getTECW8RecHits() {return TECW8RecHits;}

  ///////////////
  // SiPixel info
  ///////////////
  struct SiPixelRecHit
  {
    SiPixelRecHit(): RX(0), RY(0), SX(0), SY(0) {}
    float RX; //reconstructed x
    float RY; //reconstructed y
    float SX; //simulated x
    float SY; //simulated y
  };
  typedef std::vector<SiPixelRecHit> SiPixelRecHitVector;
  //put functions
  void putBRL1RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putBRL2RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putBRL3RecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putFWD1pRecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putFWD1nRecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putFWD2pRecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  void putFWD2nRecHits(std::vector<float> rx, std::vector<float> ry,
		       std::vector<float> sx, std::vector<float> sy);
  //get functions
  int getnBRL1RecHits() {return nBRL1RecHits;}  
  int getnBRL2RecHits() {return nBRL2RecHits;}  
  int getnBRL3RecHits() {return nBRL3RecHits;}
  int getnFWD1pRecHits() {return nFWD1pRecHits;}  
  int getnFWD1nRecHits() {return nFWD1nRecHits;}    
  int getnFWD2pRecHits() {return nFWD2pRecHits;}  
  int getnFWD2nRecHits() {return nFWD2nRecHits;}  
  SiPixelRecHitVector getBRL1RecHits() {return BRL1RecHits;}  
  SiPixelRecHitVector getBRL2RecHits() {return BRL2RecHits;}  
  SiPixelRecHitVector getBRL3RecHits() {return BRL3RecHits;}  
  SiPixelRecHitVector getFWD1pRecHits() {return FWD1pRecHits;}
  SiPixelRecHitVector getFWD1nRecHits() {return FWD1nRecHits;} 
  SiPixelRecHitVector getFWD2pRecHits() {return FWD2pRecHits;}
  SiPixelRecHitVector getFWD2nRecHits() {return FWD2nRecHits;} 

  ////////////
  // Muon info
  ////////////

  //////////
  // DT Info
  ////////// 
  struct DTRecHit
  {
    DTRecHit(): RHD(0), SHD(0) {}
    float RHD; //distance of rechit from wire
    float SHD; //distance of simhit from wire
  };
  typedef std::vector<DTRecHit> DTRecHitVector;
  //put functions
  void putDTRecHits(std::vector<float> rhd, std::vector<float> shd);
  //get functions
  int getnDTRecHits() {return nDTRecHits;}  
  DTRecHitVector getDTRecHits() {return DTRecHits;}  

  /////////////////
  // CSC info
  /////////////////
  struct CSCRecHit
  {
    CSCRecHit(): RHPHI(0), RHPERP(0), SHPHI(0) {}
    float RHPHI; //reconstructed hit phi
    float RHPERP; //reconstructed hit perp
    float SHPHI; //simulated hit phi
  };
  typedef std::vector<CSCRecHit> CSCRecHitVector;
  //put functions
  void putCSCRecHits(std::vector<float> rhphi, std::vector<float> rhperp, 
		     std::vector<float> shphi);
  //get functions
  int getnCSCRecHits() {return nCSCRecHits;}  
  CSCRecHitVector getCSCRecHits() {return CSCRecHits;}  

  /////////////////
  // RPC info
  /////////////////
  struct RPCRecHit
  {
    RPCRecHit(): RHX(0), SHX(0) {}
    float RHX; //reconstructed hit x
    float SHX; //simulated hit x
  };
  typedef std::vector<RPCRecHit> RPCRecHitVector;
  //put functions
  void putRPCRecHits(std::vector<float> rhx, std::vector<float> shx);
  //get functions
  int getnRPCRecHits() {return nRPCRecHits;}  
  RPCRecHitVector getRPCRecHits() {return RPCRecHits;} 

 private:

  ////////////
  // ECal info
  ////////////
  int nEBCalRecHits;
  ECalRecHitVector EBCalRecHits;
  int nEECalRecHits;
  ECalRecHitVector EECalRecHits;
  int nESCalRecHits;
  ECalRecHitVector ESCalRecHits;

  ////////////
  // HCal info
  ////////////
  int nHBCalRecHits;
  HCalRecHitVector HBCalRecHits;
  int nHECalRecHits;
  HCalRecHitVector HECalRecHits;
  int nHOCalRecHits;
  HCalRecHitVector HOCalRecHits;
  int nHFCalRecHits;
  HCalRecHitVector HFCalRecHits;

  ////////////////////////
  // Silicon Tracker info
  ///////////////////////

  //////////////
  //SiStrip info
  //////////////
  int nTIBL1RecHits;  
  SiStripRecHitVector TIBL1RecHits;
  int nTIBL2RecHits;  
  SiStripRecHitVector TIBL2RecHits;
  int nTIBL3RecHits; 
  SiStripRecHitVector TIBL3RecHits;
  int nTIBL4RecHits;  
  SiStripRecHitVector TIBL4RecHits;
  int nTOBL1RecHits;
  SiStripRecHitVector TOBL1RecHits;
  int nTOBL2RecHits;  
  SiStripRecHitVector TOBL2RecHits;
  int nTOBL3RecHits;  
  SiStripRecHitVector TOBL3RecHits;
  int nTOBL4RecHits; 
  SiStripRecHitVector TOBL4RecHits;
  int nTIDW1RecHits;   
  SiStripRecHitVector TIDW1RecHits;
  int nTIDW2RecHits;
  SiStripRecHitVector TIDW2RecHits;
  int nTIDW3RecHits;
  SiStripRecHitVector TIDW3RecHits; 
  int nTECW1RecHits;
  SiStripRecHitVector TECW1RecHits;
  int nTECW2RecHits;
  SiStripRecHitVector TECW2RecHits;
  int nTECW3RecHits;
  SiStripRecHitVector TECW3RecHits;
  int nTECW4RecHits;
  SiStripRecHitVector TECW4RecHits;
  int nTECW5RecHits;
  SiStripRecHitVector TECW5RecHits;
  int nTECW6RecHits;
  SiStripRecHitVector TECW6RecHits;
  int nTECW7RecHits;
  SiStripRecHitVector TECW7RecHits;
  int nTECW8RecHits;
  SiStripRecHitVector TECW8RecHits;

  //////////////
  //SiPixel info
  //////////////
  int nBRL1RecHits;
  SiPixelRecHitVector BRL1RecHits;
  int nBRL2RecHits;  
  SiPixelRecHitVector BRL2RecHits; 
  int nBRL3RecHits; 
  SiPixelRecHitVector BRL3RecHits; 
  int nFWD1pRecHits; 
  SiPixelRecHitVector FWD1pRecHits;
  int nFWD1nRecHits;
  SiPixelRecHitVector FWD1nRecHits; 
  int nFWD2pRecHits;
  SiPixelRecHitVector FWD2pRecHits;
  int nFWD2nRecHits;
  SiPixelRecHitVector FWD2nRecHits; 

  ////////////
  // Muon info
  ////////////

  //////////
  // DT Info
  ////////// 
  int nDTRecHits;
  DTRecHitVector DTRecHits; 

  /////////////////
  // CSC info
  ////////////////
  int nCSCRecHits;
  CSCRecHitVector CSCRecHits;

  /////////////////
  // RPC info
  ////////////////
  int nRPCRecHits;
  RPCRecHitVector RPCRecHits;
 
}; // end class declaration

#endif //PGlobalRecHitHit_h

///////////////////////////////////////////////////////////////////////////////
// PEcalValidInfo
///////////////////////////////////////////////////////////////////////////////

#ifndef  PEcalValidInfo_H
#define  PEcalValidInfo_H

/*----------------------------------------------------------
Class Description:
      The Class, PEcalValidInfo, includes all the quantities 
    needed to validate for the Simulation of Eletromagnetic 
    Calorimetor. 
       The Objects of this class will be save into regular 
    Root file vis EDProducer.

Author: X.HUANG ( huangxt@fnal.gov )
Date:  Dec, 2005

---------------------------------------------------------*/

#include <string>
#include <vector>
#include <CLHEP/Vector/LorentzVector.h>

class EcalTestAnalysis; 

class PEcalValidInfo 
{
   friend  class   EcalTestAnalysis;
   friend  class   PreshowerTestAnalysis;
   friend  class   SimHitSingleTest;
   friend  class   EcalSimHitsValidProducer;
   typedef  std::vector<float>   FloatVector;

public:
   PEcalValidInfo()
  :ee1(0.0),ee4(0.0),ee9(0.0),ee16(0.0),ee25(0.0),
   eb1(0.0),eb4(0.0),eb9(0.0),eb16(0.0),eb25(0.0),
   totalEInEE(0.0), totalEInEB(0.0), totalEInES(0.0),
   totalHits(0), nHitsInEE(0),nHitsInEB(0),nHitsInES(0),nHitsIn1ES(0),nHitsIn2ES(0) 
{

 }


   ~PEcalValidInfo() {} 

   // Get functions.
   float  ee1x1() const { return ee1; }
   float  ee2x2() const { return ee4; }
   float  ee3x3() const { return ee9; }
   float  ee4x4() const { return ee16;}
   float  ee5x5() const { return ee25;}

   float  eb1x1() const { return eb1; }
   float  eb2x2() const { return eb4; }
   float  eb3x3() const { return eb9; }
   float  eb4x4() const { return eb16;}
   float  eb5x5() const { return eb25;}

   float  eInEE()  const { return totalEInEE; }
   float  eInEB()  const { return totalEInEB; }
   float  eInES()  const { return totalEInES; }

   float  eInEEzp()  const { return totalEInEEzp; }
   float  eInEEzm()  const { return totalEInEEzm; }

   float  eInESzp()  const { return totalEInESzp; }
   float  eInESzm()  const { return totalEInESzm; }

   int    hitsInEcal() const { return totalHits; }
   int    hitsInEE()   const { return nHitsInEE; }
   int    hitsInEB()   const { return nHitsInEB; }
   int    hitsInES()   const { return nHitsInES; }
   int    hitsIn1ES()  const { return nHitsIn1ES;}
   int    hitsIn2ES()  const { return nHitsIn2ES;}
  
   int    hitsIn1ESzp()  const { return nHitsIn1ESzp;}
   int    hitsIn1ESzm()  const { return nHitsIn1ESzm;}
   int    hitsIn2ESzp()  const { return nHitsIn2ESzp;}
   int    hitsIn2ESzm()  const { return nHitsIn2ESzm;}       

   int    crystalInEB()   const { return nCrystalInEB;}
   int    crystalInEEzp() const { return nCrystalInEEzp; }
   int    crystalInEEzm() const { return nCrystalInEEzm; }

   FloatVector  bX0() const { return eBX0; }
   FloatVector  eX0() const { return eEX0; }


   FloatVector  eIn1ES() const { return eOf1ES; }
   FloatVector  eIn2ES() const { return eOf2ES; }
   FloatVector  zOfInES()  const { return zOfES;  }

   FloatVector  eIn1ESzp() const { return eOf1ESzp; }
   FloatVector  eIn1ESzm() const { return eOf1ESzm; }
 
   FloatVector  eIn2ESzp() const { return eOf2ESzp; }
   FloatVector  eIn2ESzm() const { return eOf2ESzm; }

   FloatVector  phiOfEEHits() const { return phiOfEECaloG4Hit; }
   FloatVector  etaOfEEHits() const { return etaOfEECaloG4Hit; }
   FloatVector  tOfEEHits()   const { return tOfEECaloG4Hit;   }
   FloatVector  eOfEEHits()   const { return eOfEECaloG4Hit;   }
   FloatVector  eOfEEPlusHits()    const { return eOfEEPlusCaloG4Hit;   }
   FloatVector  eOfEEMinusHits()   const { return eOfEEMinusCaloG4Hit;   }


   FloatVector  phiOfEBHits() const { return phiOfEBCaloG4Hit; }
   FloatVector  etaOfEBHits() const { return etaOfEBCaloG4Hit; }
   FloatVector  tOfEBHits()   const { return tOfEBCaloG4Hit;   }
   FloatVector  eOfEBHits()   const { return eOfEBCaloG4Hit;   }

   FloatVector  phiOfiESHits() const { return phiOfESCaloG4Hit; }
   FloatVector  etaOfESHits() const { return etaOfESCaloG4Hit; }
   FloatVector  tOfESHits()   const { return tOfESCaloG4Hit;   }
   FloatVector  eOfESHits()   const { return eOfESCaloG4Hit;   }

   HepLorentzVector  momentum() const { return theMomentum; }
   HepLorentzVector  vertex() const  { return theVertex; }
   
   int pId()  const { return thePID; }   

private:
 
   float  ee1;       //Energy deposition in cluser1x1
   float  ee4;       //Energy deposition in cluser2x2
   float  ee9;       //Energy deposition in cluser3x3
   float  ee16;      //Energy deposition in cluser4x4
   float  ee25;      //Energy deposition in cluser5x5

   float  eb1;       //Energy deposition in cluser1x1
   float  eb4;       //Energy deposition in cluser2x2
   float  eb9;       //Energy deposition in cluser3x3
   float  eb16;      //Energy deposition in cluser4x4
   float  eb25;      //Energy deposition in cluser5x5


 
   float  totalEInEE;       //The Total Energy deposited in EE;
   float  totalEInEB;       //The Total Energy deposited in EB;
   float  totalEInES;       //The Total Energy deposited in ES;
 
   float  totalEInEEzp;
   float  totalEInEEzm;
   float  totalEInESzp;
   float  totalEInESzm;



   int totalHits;          //Total number of Hits.
   int nHitsInEE;          //Total number of Hits in EE.
   int nHitsInEB;          //Total number of Hits in EB.
   int nHitsInES;          //Total number of Hits in ES.
   int nHitsIn1ES;         //Total number of Hits in 1st Layer of ES;
   int nHitsIn2ES;         //Total number of Hits in 2nd Layer of ES;

   int nHitsIn1ESzp;
   int nHitsIn1ESzm;
   int nHitsIn2ESzp;
   int nHitsIn2ESzm;       

   int nCrystalInEB;
   int nCrystalInEEzp;
   int nCrystalInEEzm;


   FloatVector eBX0;       // longitudinal Energy deposition In EB.
   FloatVector eEX0;       // longitudinal Energy deposition In EE.

   FloatVector  eOf1ES;    // Energy deposition of Hits in 1st layer of ES;
   FloatVector  eOf2ES;    // Energy deposition of Hits in 2nd layer of ES;              
   FloatVector  zOfES;


   FloatVector  eOf1ESzp;
   FloatVector  eOf1ESzm;
   FloatVector  eOf2ESzp;
   FloatVector  eOf2ESzm;

   FloatVector  phiOfEECaloG4Hit;    // Phi of Hits.
   FloatVector  etaOfEECaloG4Hit;    // Eta of Hits.
   FloatVector  tOfEECaloG4Hit;      // Tof of Hits.
   FloatVector  eOfEECaloG4Hit;      // Energy depostion of Hits.
   FloatVector  eOfEEPlusCaloG4Hit;       // Energy depostion of Hits.
   FloatVector  eOfEEMinusCaloG4Hit;      // Energy depostion of Hits.

   FloatVector  phiOfESCaloG4Hit;    // Phi of Hits.
   FloatVector  etaOfESCaloG4Hit;    // Eta of Hits.
   FloatVector  tOfESCaloG4Hit;      // Tof of Hits.
   FloatVector  eOfESCaloG4Hit;      // Energy depostion of Hits.

   FloatVector  phiOfEBCaloG4Hit;    // Phi of Hits.
   FloatVector  etaOfEBCaloG4Hit;    // Eta of Hits.
   FloatVector  tOfEBCaloG4Hit;      // Tof of Hits.
   FloatVector  eOfEBCaloG4Hit;      // Energy depostion of Hits.



   int thePID;                      // add more ??
   HepLorentzVector theMomentum;  
   HepLorentzVector theVertex;
};


#endif // endif PECal

///////////////////////////////////////////////////////////////////////////////
// PHcalValidInfoJets
///////////////////////////////////////////////////////////////////////////////

#ifndef  PHcalValidInfoJets_H
#define  PHcalValidInfoJets_H

#include <string>
#include <vector>
#include <memory>

class SimG4HcalValidation;

class PHcalValidInfoJets {

  friend class SimG4HcalValidation;

public:
       
  PHcalValidInfoJets(): nJetHit(0), nJet(0), ecalJet(0.), hcalJet(0.),
			hoJet(0.), etotJet(0.), detaJet(0.), dphiJet(0.),
			drJet(0.), dijetM(0.) {}
  virtual ~PHcalValidInfoJets() {}

  // acceess

  std::vector<float> jethite() const {return jetHite;}
  std::vector<float> jethitr() const {return jetHitr;}
  std::vector<float> jethitt() const {return jetHitt;}
  int                njethit() const {return nJetHit;}

  std::vector<float> jete()    const {return jetE;}
  std::vector<float> jeteta()  const {return jetEta;}
  std::vector<float> jetphi()  const {return jetPhi;}
  int                njet()    const {return nJet;} 

  float              ecaljet() const {return ecalJet;}
  float              hcaljet() const {return hcalJet;}
  float                hojet() const {return   hoJet;}
  float              etotjet() const {return etotJet;}

  float              detajet() const {return detaJet;}
  float              dphijet() const {return dphiJet;}
  float                drjet() const {return   drJet;}
  float               dijetm() const {return  dijetM;}

  // fill
  void fillTProfileJet      (double e, double r, double t);
  void fillEcollectJet      (double ee, double he, double hoe, double etot);
  void fillEtaPhiProfileJet (double eta0, double phi0, double eta,
                             double phi, double dist);
  void fillJets             (std::vector<double> enj, std::vector<double> etaj,
			     std::vector<double> phij);
  void fillDiJets           (double mass);

private:

  int                 nJetHit, nJet;
  float               ecalJet, hcalJet, hoJet, etotJet;
  float               detaJet, dphiJet, drJet, dijetM;
  std::vector<float>  jetHite;
  std::vector<float>  jetHitr;
  std::vector<float>  jetHitt;
  std::vector<float>  jetE;
  std::vector<float>  jetEta;
  std::vector<float>  jetPhi;

};

#endif

///////////////////////////////////////////////////////////////////////////////
// PHcalValidInfoLayer
///////////////////////////////////////////////////////////////////////////////

#ifndef  PHcalValidInfoLayer_H
#define  PHcalValidInfoLayer_H

#include <string>
#include <vector>
#include <memory>

class SimG4HcalValidation;

class PHcalValidInfoLayer {

  friend class SimG4HcalValidation;

public:
       
  PHcalValidInfoLayer(): hitN(0), eHO(0.0),eHBHE(0.0),eEBEE(0.0),elongHF(0.0),
			 eshortHF(0.0), eEcalHF(0.0), eHcalHF(0.0) {}
  virtual ~PHcalValidInfoLayer() {}

  // access
  int                    nHit()   const {return hitN;}

  float                   eho()   const {return eHO;}    
  float                 ehbhe()   const {return eHBHE;}    
  float                 eebee()   const {return eEBEE;}    
  float               elonghf()   const {return elongHF;}    
  float              eshorthf()   const {return eshortHF;}    
  float               eecalhf()   const {return eEcalHF;}    
  float               ehcalhf()   const {return eHcalHF;}    

  std::vector<float>   elayer()   const {return eLayer;}
  std::vector<float>   edepth()   const {return eDepth;}

  std::vector<float>   etaHit()   const {return hitEta;} 
  std::vector<float>   phiHit()   const {return hitPhi;} 
  std::vector<float>     eHit()   const {return hitE;} 
  std::vector<float>     tHit()   const {return hitTime;} 
  std::vector<float> layerHit()   const {return hitLayer;} 
  std::vector<float>    idHit()   const {return hitId;} 

  // filling
  void fillLayers (double el[], double ed[], double ho, double hbhe,
		   double ebee);
  void fillHF     (double fibl, double fibs, double enec, double enhc);
  void fillHits   (int Nhits, int lay, int unitID, double eta, double phi, 
		   double ehit, double t); 
  //  void clear();


private:

  int                hitN;
  float              eHO, eHBHE, eEBEE;
  float              elongHF, eshortHF, eEcalHF, eHcalHF;
  std::vector<float> eLayer;
  std::vector<float> eDepth;
  // SimHits parameters
  std::vector<float> hitLayer; // float for int
  std::vector<float> hitId;    // float for int
  std::vector<float> hitEta;
  std::vector<float> hitPhi;
  std::vector<float> hitE;
  std::vector<float> hitTime;

};

#endif

///////////////////////////////////////////////////////////////////////////////
// PHcalValidInfoNxN
///////////////////////////////////////////////////////////////////////////////

#ifndef  PHcalValidInfoNxN_H
#define  PHcalValidInfoNxN_H

#include <string>
#include <vector>
#include <memory>

class SimG4HcalValidation;


class PHcalValidInfoNxN {

  friend class SimG4HcalValidation;

public:
       
  PHcalValidInfoNxN(): nNxN(0), ecalNxNr(0), hcalNxNr(0.), hoNxNr(0.), 
    etotNxNr(0.), ecalNxN(0.), hcalNxN(0.), hoNxN(0.), etotNxN(0.) {}
  virtual ~PHcalValidInfoNxN() {}

  // access
  std::vector<float> idnxn() const {return idNxN;}
  std::vector<float>  enxn() const {return  eNxN;}
  std::vector<float>  tnxn() const {return  tNxN;}
  int                 nnxn() const {return  nNxN;}
  
  float           ecalnxnr() const {return ecalNxNr;}
  float           hcalnxnr() const {return hcalNxNr;}
  float             honxnr() const {return   hoNxNr;}
  float           etotnxnr() const {return etotNxNr;}

  float           ecalnxn () const {return ecalNxN ;}
  float           hcalnxn () const {return hcalNxN ;}
  float             honxn () const {return   hoNxN ;}
  float           etotnxn () const {return etotNxN ;}
  

  // fill
  void fillHvsE        (double ee, double he, double hoe, double etot);
  void fillEcollectNxN (double een, double hen, double hoen, double etotn);
  void fillTProfileNxN (double e, int i, double t);

private:

  int                nNxN;
  float              ecalNxNr, hcalNxNr, hoNxNr, etotNxNr;
  float              ecalNxN,  hcalNxN,  hoNxN,  etotNxN;
  std::vector<float> idNxN; // float for int
  std::vector<float> eNxN;
  std::vector<float> tNxN;

};

#endif

///////////////////////////////////////////////////////////////////////////////
// PMuonSimHit
///////////////////////////////////////////////////////////////////////////////

#ifndef PMuonSimHit_h
#define PMuonSimHit_h

#include <vector>
#include <memory>

/// Class PMuonSimHit defines structure of simulated hits data in CSC,DT,RPC
/// for validation. It also includes vertex and track info.

class PMuonSimHit
{
 public:

  PMuonSimHit(): nRawGenPart(0), nG4Vtx(0), nG4Trk(0), 
                 nCSCHits(0), nDTHits(0), nRPCHits(0) {}
  virtual ~PMuonSimHit(){}

  struct Vtx
  {
    Vtx(): x(0), y(0), z(0) {}
    float x;
    float y;
    float z;
  };

  struct Trk
  {
    Trk() : pt(0), e(0), eta(0), phi(0) {}
    float pt;
    float e;
    float eta;
    float phi;
  };


  struct CSC
  {
    CSC() :
         _cscId(0), 
         _detUnitId(0),   _trackId(0),     _processType(0), 
         _particleType(0),_pabs(0),
         _globposz(0),    _globposphi(0),  _globposeta(0), 
	 _locposx(0),     _locposy(0),     _locposz(0), 
	 _locdirx(0),     _locdiry(0),     _locdirz(0), 
         _locdirtheta(0), _locdirphi(0),
	 _exitpointx(0),  _exitpointy(0),  _exitpointz(0),
	 _entrypointx(0), _entrypointy(0), _entrypointz(0), 
         _enloss(0),      _tof(0) {}
 
    int   _cscId;
    unsigned int _detUnitId;
    float _trackId;
    float _processType;
    float _particleType;
    float _pabs;
    float _globposz;
    float _globposphi;
    float _globposeta;
    float _locposx;
    float _locposy;
    float _locposz;
    float _locdirx;
    float _locdiry;
    float _locdirz;
    float _locdirtheta;
    float _locdirphi;
    float _exitpointx;
    float _exitpointy;
    float _exitpointz;
    float _entrypointx;
    float _entrypointy;
    float _entrypointz;
    float _enloss;
    float _tof;
  };

  struct DT
  {
    DT() : 
         _detUnitId(0),   _trackId(0),     _processType(0), 
         _particleType(0),_pabs(0), 
         _globposz(0),    _globposphi(0),  _globposeta(0),
	 _locposx(0),     _locposy(0),     _locposz(0), 
	 _locdirx(0),     _locdiry(0),     _locdirz(0), 
         _locdirtheta(0), _locdirphi(0),
	 _exitpointx(0),  _exitpointy(0),  _exitpointz(0),
	 _entrypointx(0), _entrypointy(0), _entrypointz(0), 
         _enloss(0),      _tof(0) {}

    unsigned int _detUnitId;
    float _trackId;
    float _processType;
    float _particleType;
    float _pabs;
    float _globposz;
    float _globposphi;
    float _globposeta;
    float _locposx;
    float _locposy;
    float _locposz;
    float _locdirx;
    float _locdiry;
    float _locdirz;
    float _locdirtheta;
    float _locdirphi;
    float _exitpointx;
    float _exitpointy;
    float _exitpointz;
    float _entrypointx;
    float _entrypointy;
    float _entrypointz;
    float _enloss;
    float _tof;
  };

  struct RPC
  {
    RPC() : 
         _detUnitId(0),   _trackId(0),     _processType(0), 
         _particleType(0),_pabs(0), 
         _globposz(0),    _globposphi(0),  _globposeta(0),
	 _locposx(0),     _locposy(0),     _locposz(0), 
	 _locdirx(0),     _locdiry(0),     _locdirz(0), 
         _locdirtheta(0), _locdirphi(0),
	 _exitpointx(0),  _exitpointy(0),  _exitpointz(0),
	 _entrypointx(0), _entrypointy(0), _entrypointz(0), 
         _enloss(0),      _tof(0) {}

    unsigned int _detUnitId;
    float _trackId;
    float _processType;
    float _particleType;
    float _pabs;
    float _globposz;
    float _globposphi;
    float _globposeta;
    float _locposx;
    float _locposy;
    float _locposz;
    float _locdirx;
    float _locdiry;
    float _locdirz;
    float _locdirtheta;
    float _locdirphi;
    float _exitpointx;
    float _exitpointy;
    float _exitpointz;
    float _entrypointx;
    float _entrypointy;
    float _entrypointz;
    float _enloss;
    float _tof;
  };

  typedef std::vector<Vtx> VtxVector;
  typedef std::vector<Trk> TrkVector;

  typedef std::vector<CSC> CSCVector;
  typedef std::vector<DT>  DTVector;
  typedef std::vector<RPC>  RPCVector;

  /// put functions

  void putRawGenPart(int n);

  void putG4Vtx(std::vector<float> x,   std::vector<float> y,
                std::vector<float> z);
  void putG4Trk(std::vector<float> pt,  std::vector<float> e,
                std::vector<float> eta, std::vector<float> phi);  

  void putCSCHits(
               std::vector<int>  _cscId,
               std::vector<unsigned int> _detUnitId,
	       std::vector<float> _trackId , 
               std::vector<float> _processType,
	       std::vector<float> _particleType, 
               std::vector<float> _pabs,
	       std::vector<float> _globposz, 
               std::vector<float> _globposphi, 
               std::vector<float> _globposeta,
	       std::vector<float> _locposx, 
               std::vector<float> _locposy, 
               std::vector<float> _locposz,
	       std::vector<float> _locdirx, 
               std::vector<float> _locdiry, 
               std::vector<float> _locdirz,
	       std::vector<float> _locdirtheta, 
               std::vector<float> _locdirphi, 
	       std::vector<float> _exitpointx, 
               std::vector<float> _exitpointy, 
               std::vector<float> _exitpointz,
	       std::vector<float> _entrypointx, 
               std::vector<float> _entrypointy, 
               std::vector<float> _entrypointz,
	       std::vector<float> _enloss, 
               std::vector<float> _tof);   

  void putDTHits(
               std::vector<unsigned int> _detUnitId,
	       std::vector<float> _trackId , 
               std::vector<float> _processType,
	       std::vector<float> _particleType, 
               std::vector<float> _pabs,
	       std::vector<float> _globposz, 
               std::vector<float> _globposphi, 
               std::vector<float> _globposeta,
	       std::vector<float> _locposx, 
               std::vector<float> _locposy, 
               std::vector<float> _locposz,
	       std::vector<float> _locdirx, 
               std::vector<float> _locdiry, 
               std::vector<float> _locdirz,
	       std::vector<float> _locdirtheta, 
               std::vector<float> _locdirphi, 
	       std::vector<float> _exitpointx, 
               std::vector<float> _exitpointy, 
               std::vector<float> _exitpointz,
	       std::vector<float> _entrypointx, 
               std::vector<float> _entrypointy, 
               std::vector<float> _entrypointz,
	       std::vector<float> _enloss, 
               std::vector<float> _tof); 

  void putRPCHits(
               std::vector<unsigned int> _detUnitId,
	       std::vector<float> _trackId , 
               std::vector<float> _processType,
	       std::vector<float> _particleType, 
               std::vector<float> _pabs,
	       std::vector<float> _globposz, 
               std::vector<float> _globposphi, 
               std::vector<float> _globposeta,
	       std::vector<float> _locposx, 
               std::vector<float> _locposy, 
               std::vector<float> _locposz,
	       std::vector<float> _locdirx, 
               std::vector<float> _locdiry, 
               std::vector<float> _locdirz,
	       std::vector<float> _locdirtheta, 
               std::vector<float> _locdirphi, 
	       std::vector<float> _exitpointx, 
               std::vector<float> _exitpointy, 
               std::vector<float> _exitpointz,
	       std::vector<float> _entrypointx, 
               std::vector<float> _entrypointy, 
               std::vector<float> _entrypointz,
	       std::vector<float> _enloss, 
               std::vector<float> _tof); 

  /// get functions

  int getnRawGenPart() {return nRawGenPart;}
  int getnG4Vtx() {return nG4Vtx;}
  int getnG4Trk() {return nG4Trk;}

  VtxVector getG4Vtx() {return G4Vtx;}
  TrkVector getG4Trk() {return G4Trk;}

  int getnCSCHits() {return nCSCHits;}
  CSCVector getCSCHits() {return CSCHits;}

  int getnDTHits() {return nDTHits;}
  DTVector getDTHits() {return DTHits;}

  int getnRPCHits() {return nRPCHits;}
  RPCVector getRPCHits() {return RPCHits;}

 
private:

  /// G4MC info

  int nRawGenPart;
  int nG4Vtx;
  VtxVector G4Vtx; 
  int nG4Trk; 
  TrkVector G4Trk;
 
  /// Hit info

  int nCSCHits;
  CSCVector CSCHits; 

  int nDTHits;
  DTVector DTHits; 

  int nRPCHits;
  RPCVector RPCHits; 

};

#endif

///////////////////////////////////////////////////////////////////////////////
// PTrackerSimHit
///////////////////////////////////////////////////////////////////////////////

#ifndef PTrackerSimHit_h
#define PTrackerSimHit_h

#include <vector>
#include <memory>

class PTrackerSimHit
{

 public:

  PTrackerSimHit(): nRawGenPart(0), nG4Vtx(0), nG4Trk(0), nHits(0) {}
  virtual ~PTrackerSimHit(){}

  struct Vtx
  {
    Vtx(): x(0), y(0), z(0) {}
    float x;
    float y;
    float z;
  };

  struct Trk
  {
    Trk() : pt(0), e(0), eta(0), phi(0) {}
    float pt;
    float e;
    float eta;
    float phi;
  };


  struct Hit
  {
    Hit() : _sysID(0), _detUnitId(0), _trackId(0), _processType(0), 
            _particleType(0), _pabs(0), 
	    _lpx(0), _lpy(0), _lpz(0), 
	    _ldx(0), _ldy(0), _ldz(0), _ldtheta(0), _ldphi(0),
	    _exx(0), _exy(0), _exz(0),
	    _enx(0), _eny(0), _enz(0), _eloss(0), _tof(0) {}
    int   _sysID; 
    float _detUnitId;
    float _trackId;
    float _processType;
    float _particleType;
    float _pabs;
    float _lpx;
    float _lpy;
    float _lpz;
    float _ldx;
    float _ldy;
    float _ldz;
    float _ldtheta;
    float _ldphi;
    float _exx;
    float _exy;
    float _exz;
    float _enx;
    float _eny;
    float _enz;
    float _eloss;
    float _tof;
  };


  typedef std::vector<Vtx> VtxVector;
  typedef std::vector<Trk> TrkVector;
  typedef std::vector<Hit> HitVector;

  // put functions
  void putRawGenPart(int n);
  void putG4Vtx(std::vector<float> x, std::vector<float> y, std::vector<float> z);
  void putG4Trk(std::vector<float> pt, std::vector<float> e, std::vector<float> eta, std::vector<float> phi);  
  void putHits(std::vector<int> _sysID, std::vector<float> _detUnitId,
	       std::vector<float>_trackId , std::vector<float>_processType,
	       std::vector<float>_particleType, std::vector<float> _pabs,
	       std::vector<float>_lpx, std::vector<float>_lpy, std::vector<float>_lpz,
	       std::vector<float>_ldx, std::vector<float>_ldy, std::vector<float>_ldz,
	       std::vector<float>_ldtheta, std::vector<float>_ldphi, 
	       std::vector<float>_exx, std::vector<float>_exy, std::vector<float>_exz,
	       std::vector<float>_enx, std::vector<float>_eny, std::vector<float>_enz,
	       std::vector<float>_eloss, std::vector<float>_tof);   

  // get functions
  int getnRawGenPart() {return nRawGenPart;}
  int getnG4Vtx() {return nG4Vtx;}
  VtxVector getG4Vtx() {return G4Vtx;}
  int getnG4Trk() {return nG4Trk;}
  TrkVector getG4Trk() {return G4Trk;}
  int getnHits() {return nHits;}
  HitVector getHits() {return Hits;}

 private:

  // G4MC info
  int nRawGenPart;
  int nG4Vtx;
  VtxVector G4Vtx; 
  int nG4Trk; 
  TrkVector G4Trk; 
  // Tracker info
  int nHits;
  HitVector Hits; 


}; // end class declaration

#endif

#endif // endif PValidationFormats_h

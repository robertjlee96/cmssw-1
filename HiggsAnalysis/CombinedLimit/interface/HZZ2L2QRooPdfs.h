#ifndef HZZ2L2QROOPDFS
#define HZZ2L2QROOPDFS

#include "RooAbsPdf.h"
#include "RooRealProxy.h"
#include "RooAbsReal.h"

class RooCB : public RooAbsPdf {
 public:
  RooCB();
  RooCB(const char *name, const char *title,
        RooAbsReal& _x,
        RooAbsReal& _mean,
        RooAbsReal& _width,
        RooAbsReal& _alpha,
        RooAbsReal& _n,
        RooAbsReal& _theta
	);
  RooCB(const RooCB& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooCB(*this,newname); }
  inline virtual ~RooCB() { }

 protected:

  RooRealProxy x ;
  RooRealProxy mean;
  RooRealProxy width;
  RooRealProxy alpha;
  RooRealProxy n;
  RooRealProxy theta;

  Double_t evaluate() const ;

 private:

  ClassDef(RooCB,1)
    };

 
class RooDoubleCB : public RooAbsPdf {
public:
  RooDoubleCB();
  RooDoubleCB(const char *name, const char *title,
	      RooAbsReal& _x,
	      RooAbsReal& _mean,
	      RooAbsReal& _width,
	      RooAbsReal& _alpha1,
	      RooAbsReal& _n1,
	      RooAbsReal& _alpha2,
	      RooAbsReal& _n2
	   );
  RooDoubleCB(const RooDoubleCB& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooDoubleCB(*this,newname); }
  inline virtual ~RooDoubleCB() { }

protected:

  RooRealProxy x ;
  RooRealProxy mean;
  RooRealProxy width;
  RooRealProxy alpha1;
  RooRealProxy n1;
  RooRealProxy alpha2;
  RooRealProxy n2;
  
  Double_t evaluate() const ;

private:

  ClassDef(RooDoubleCB,1)
};
 
class RooFermi : public RooAbsPdf {
public:
  RooFermi();
  RooFermi(const char *name, const char *title,
 	    RooAbsReal& _x,
            RooAbsReal& _cutOff,
	   RooAbsReal& _beta
	   );
  RooFermi(const RooFermi& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooFermi(*this,newname); }
  inline virtual ~RooFermi() { }

protected:

  RooRealProxy x ;
  RooRealProxy cutOff ;
  RooRealProxy beta ;
  
  Double_t evaluate() const ;

private:

  ClassDef(RooFermi,1) 
};
  
class RooRelBW : public RooAbsPdf {
public:
  RooRelBW();
  RooRelBW(const char *name, const char *title,
	   RooAbsReal& _x,
	   RooAbsReal& _mean,
	   RooAbsReal& _width,
	   RooAbsReal& _n
	   );
  RooRelBW(const RooRelBW& other, const char* name=0) ;
  virtual TObject* clone(const char* newname) const { return new RooRelBW(*this,newname); }
  inline virtual ~RooRelBW() { }

protected:

  RooRealProxy x ;
  RooRealProxy mean ;
  RooRealProxy width ;
  RooRealProxy n ;
  
  Double_t evaluate() const ;

private:

  ClassDef(RooRelBW,1)
};
 
#endif

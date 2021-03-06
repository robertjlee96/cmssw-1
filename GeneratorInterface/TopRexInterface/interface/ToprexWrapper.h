#ifndef TOPREXWRAPPER_H
#define TOPREXWRAPPER_H
//////////////////////////////////////////////////////////////////////////
// Sergey.Slabospitsky@cern.ch, July 2006
// Wrapper for FORTRAN version of TopReX

// Initialization routine
                                                                               
//--------------------------------------------------------------------------
// TopReX routine declaration

#define toprex toprex_ 
    extern "C" {
      void toprex(const char*,const char*,const char*, double*,int,int,int);
    }

// define methods to hide the subtle syntax necessary to call fortran from C++
void call_toprex( const char* frame, const char* beam, const char* target,
		  double win );

// 
#define txgive txgive_ 
    extern "C" {
        void txgive(const char*, int);

      }

// define methods to hide the subtle syntax necessary to call fortran from C++
//id call_txgive( const char* chin) 
// txgive( chin,strlen(chin) ); }


//--------------------------------------------------------------------------
// TopReX Common Block Declarations
extern "C" {
    extern struct {
        int ipar[200];
        double rpar[200];
    } txpar_;
}
#define txpar txpar_

#endif

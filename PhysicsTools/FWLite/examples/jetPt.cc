// Standard includes
#include <iostream>
#include <string>

// CMS includes
#include "DataFormats/FWLite/interface/Handle.h"
#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/JetReco/interface/CaloJet.h"

#include "PhysicsTools/FWLite/interface/FWLiteCont.h"
#include "PhysicsTools/FWLite/interface/OptionUtils.h"  // (optutl::)
#include "PhysicsTools/FWLite/interface/dout.h"
#include "PhysicsTools/FWLite/interface/dumpSTL.icc"

// root includes
#include "TFile.h"
#include "TROOT.h"
#include "TSystem.h"
#include "TH1.h"
#include "TH2.h"
#include "TString.h"

using namespace std;


//////////////////////////
// Forward Declarations //
//////////////////////////

// This subroutine, written by you (below), uses the command line
// arguments and creates an output tag (if any).  This subroutine must
// exist.
void outputNameTagFunc (string &tag);

// Book all histograms to be filled this job.  If wanted, you can skip
// this subroutine and book all histograms in the main subroutine.
void bookHistograms (FWLiteCont &event);


///////////////////////////
// ///////////////////// //
// // Main Subroutine // //
// ///////////////////// //
///////////////////////////

int main (int argc, char* argv[]) 
{
   ////////////////////////////////
   // ////////////////////////// //
   // // Command Line Options // //
   // ////////////////////////// //
   ////////////////////////////////

   // Tell people what this analysis code does.
   optutl::setUsageString ("Plots Jet Pt");

   // Setup default options
   optutl::setupDefaultOptions();

   //////////////////////////////////////////////////////
   // Add any command line options you would like here //
   //////////////////////////////////////////////////////
   // optutl::addOption ("sampleName",   optutl::kString, 
   //                    "Sample name (e.g., top, Wqq, etc.)");   

   // Parse the command line arguments
   optutl::parseArguments (argc, argv);

   //////////////////////////////////
   // //////////////////////////// //
   // // Create Event Container // //
   // //////////////////////////// //
   //////////////////////////////////

   // This object 'event' is used both to get all information from the
   // event as well as to store histograms, etc.
   FWLiteCont event (&outputNameTagFunc);

   ////////////////////////////////////////
   // ////////////////////////////////// //
   // //         Begin Run            // //
   // // (e.g., book histograms, etc) // //
   // ////////////////////////////////// //
   ////////////////////////////////////////

   // Setup a style
   gROOT->SetStyle ("Plain");

   // Book those histograms!
   bookHistograms (event);

   //////////////////////
   // //////////////// //
   // // Event Loop // //
   // //////////////// //
   //////////////////////

   for (event.toBegin(); ! event.atEnd(); ++event) 
   {
      dout << endl;
      //////////////////////////////////
      // Take What We Need From Event //
      //////////////////////////////////
      fwlite::Handle< vector< reco::CaloJet > > jetCollection;
      jetCollection.getByLabel (event, "sisCone5CaloJets");
      assert ( jetCollection.isValid() );
						
      // Loop over the jets
      const vector< reco::CaloJet >::const_iterator kJetEnd = jetCollection->end();
      for (vector< reco::CaloJet >::const_iterator jetIter = jetCollection->begin();
           kJetEnd != jetIter; 
           ++jetIter) 
      {         
         dout << endl;
         event.hist("jetPt")->Fill (jetIter->pt());
         dout << endl;
      } // for jetIter
      dout << endl;
   } // for event

   dout << endl;
      
   ////////////////////////
   // ////////////////// //
   // // Clean Up Job // //
   // ////////////////// //
   ////////////////////////

   // Histograms will be automatically written to the root file
   // specificed by command line options.

   // All done!  Bye bye.
   return 0;
}


//////////////  //////////////////////////////////  //////////////
//////////////  // //////////////////////////// //  //////////////
//////////////  // // Supporting Subroutines // //  //////////////
//////////////  // //////////////////////////// //  //////////////
//////////////  //////////////////////////////////  //////////////


void outputNameTagFunc (string &tag)
{
   // If you do not want to give you output filename any "tag" based
   // on the command line options, simply do nothing here.  This
   // function is designed to be called by FWLiteCont constructor.

   // if ( optutl::boolValue ("someCondition") )
   // { 
   //    tag += "_someCond";
   // }
}


void bookHistograms (FWLiteCont &event)
{
   event.add( new TH1F( "jetPt", "jetPt", 1000, 0, 1000) );
}
					


#include "PtrelTemplateMaker.h"

#include "TClass.h"
#include "TDirectory.h"
#include "TKey.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TObjString.h"

#include "PtrelUtils.h"

ClassImp(PtrelTemplateMaker)


char const * PtrelTemplateMaker::directory = "/Histograms/MCtruth";


void PtrelTemplateMaker::make(
    char const * inputfile,
    char const * outputfile
) const
{
    // Information
    Info(__FUNCTION__, "Reading %s input file", inputfile);

    // Opening a input file
    CreateSafely(TFile, input, TFile::Open(inputfile, "READ"))

    // Opening a output file
    CreateSafely(TFile, output, TFile::Open(outputfile, "RECREATE"))

    // Make templates
    CallSafely( makeTemplates(input, output) )

    // Make effciencies
    CallSafely( makeEfficiencies(input, output, TPRegexp("n_pT_b"), TPRegexp("ntag_pT_b_[A-Z]*$")) )
    CallSafely( makeEfficiencies(input, output, TPRegexp("n_eta_b"), TPRegexp("ntag_eta_b_[A-Z]*$")) )
    CallSafely( makeEfficiencies(input, output, TPRegexp("p_pT_b"), TPRegexp("ptag_pT_b_[A-Z]*$")) )
    CallSafely( makeEfficiencies(input, output, TPRegexp("p_eta_b"), TPRegexp("ptag_eta_b_[A-Z]*$")) )
    // Make mistag for counting
    CallSafely( makeEfficiencies(input, output, TPRegexp("n_pT_cl"), TPRegexp("p_pT_cl")) )
    CallSafely( makeEfficiencies(input, output, TPRegexp("n_eta_cl"), TPRegexp("p_eta_cl")) )

    // Closing the files
    input->Close();
    output->Close();
}


TH2* PtrelTemplateMaker::processTH2(TObject * object) const
{
    // Cast the object pointer into 2D histogram
    TH2 * histogram2D = (TH2*) object;

    // Check for ptrel rebinning
    if ( rebin_[Dependency::ptrel] != 1 )
    {
        Info(__FUNCTION__, "Rebinning ptrel a factor %d in %s", rebin_[Dependency::ptrel], histogram2D->GetName());
        histogram2D->RebinY( rebin_[Dependency::ptrel] );
    }

    // Check the histogram dependency and rebin
    for (Int_t j = 1; j < Dependency::Dimension; ++j)
        if ( TString(histogram2D->GetName()).Contains(Dependency::Name[j]) && rebin_[j] != 1)
        {
            Info(__FUNCTION__, "Rebinning %s a factor %d in %s", Dependency::Name[j], rebin_[j], histogram2D->GetName());
            histogram2D->RebinX( rebin_[j] );
        }

    return histogram2D;
}


bool PtrelTemplateMaker::makeEfficiencies (
    TFile * input,
    TFile * output,
    TPRegexp patternD,
    TPRegexp patternN
) const
{
    char name[256];

	  // Return value
	  bool status = false;
	
    // Information
    Info(__FUNCTION__, "Starting making efficiencies");

    // Creating sub directory for templates
    if (!output->FindKey("mctruth")) output->mkdir("mctruth");

    // Move to the directory with 2d histograms
    input->cd(PtrelTemplateMaker::directory);

    // Loop over all denominator keys in this directory
    TIter nextkeyD( gDirectory->GetListOfKeys() );

    // Iterator for the denominators
    TKey * keyD;

    while (( keyD = (TKey*)nextkeyD() ))
    {
        // Select only 2D histograms
        TObject * objectD = keyD->ReadObj();
        if ( objectD->IsA()->InheritsFrom( "TH2" ) )
            // Select those histogram that match the pattern
            if ( TString(objectD->GetName()).Contains(patternD) )
            {
	        // Updating status
                status = true;  
							
                // Information
                Info(__FUNCTION__, "Selecting as denominator %s", objectD->GetName());

                // Cast the object pointer into 2D histogram
                TH2D * denominator2D = (TH2D*) processTH2(objectD);

                // Denominator histogram
                sprintf(name, "denominator_%s", denominator2D->GetName());
                TH1D * denominator1D = denominator2D->ProjectionX(name, -1, -1, "e");

                // Loop over all numerator keys in this directory
                TIter nextkeyN( gDirectory->GetListOfKeys() );

                // Iterator for the numerator
                TKey * keyN;

                while (( keyN = (TKey*)nextkeyN() ))
                {
                    // Select only 2D histograms
                    TObject * objectN = keyN->ReadObj();
                    if ( objectN->IsA()->InheritsFrom( "TH2" ) )
                        // Select those histogram that match the pattern
                        if ( TString(objectN->GetName()).Contains(patternN) )
                        {
                            // Information
                            Info(__FUNCTION__, "Selecting as numerator %s", objectN->GetName());

                            // Cast the object pointer into 2D histogram
                            TH2D * numerator2D = (TH2D*) processTH2(objectN);

                            // Numerator histogram
                            sprintf(name, "numerator_%s", denominator2D->GetName());
                            TH1D * numerator1D = numerator2D->ProjectionX(name, -1, -1, "e");

                            // MCTruth efficiencies histogram
                            sprintf(name, "mctruth_%s_%s", denominator2D->GetName(), numerator2D->GetName());
                            Info(__FUNCTION__, "Calculating efficiency %s", name);
                            TH1D * mctruth = (TH1D*) numerator1D->Clone();
                            mctruth->SetName(name);
                            mctruth->Divide(numerator1D, denominator1D, 1., 1., "e");
                            efficiencyHistogramSetup(mctruth);

                            // Save the efficiency histogram
                            output->cd("mctruth");
                            mctruth->Write();
                        }
                };
            }
    };

		if (!status) Error(__FUNCTION__, "Non matching histograms were found");
			
    return status;
}


bool PtrelTemplateMaker::makeTemplates(
    TFile * input,
    TFile * output
) const
{
    // Check function form exist
    for (std::size_t i = 0; i < (std::size_t)Flavor::Dimension; ++i)
        if (functions_[i].GetExpFormula().IsNull())
        {
            Error(__FUNCTION__, "Some or all function forms are not set");
            return false;
        }

    char name[256];

    // Information
    Info(__FUNCTION__, "Starting making templates");

    // Creating sub directory for templates
    output->cd();
    if (!output->FindKey("functions")) output->mkdir("functions");
    if (!output->FindKey("templates")) output->mkdir("templates");

    // Move to the directory with 2d histograms
    input->cd(PtrelTemplateMaker::directory);

    // Loop over all keys in this directory
    TIter nextkey( gDirectory->GetListOfKeys() );

    TKey * key;

    while (( key = (TKey*)nextkey() ))
    {
        TObject * object = key->ReadObj();
        if ( object->IsA()->InheritsFrom( "TH2" ) )
        {
            // Look over the flavor histograms
            for (Int_t i = 0; i < Flavor::Dimension; ++i)
                if ( TString(object->GetName()).Contains(Flavor::Name[i]) )
                {
                    // Cast the object pointer into 2D histogram
                    TH2D * histogram2D = (TH2D*) processTH2(object);

                    // Get the number of bins
                    Int_t nbins = histogram2D->GetNbinsX();

                    Double_t * lastParameters = 0;

                    for (Int_t j = 1; j <= nbins; ++j)
                    {
                        // Set histogram name by the formula = template_x_bin
                        sprintf(name, "template_%s_%d", histogram2D->GetName(), j);
                        // Project histogram
                        TH1D * histogram1D = histogram2D->ProjectionY(name, j, j, "e");
                        
                        // Setup histogram
                        ptrelHistogramSetup(histogram1D);

                        // Clone a new function
                        TF1 * function = (TF1*) functions_[i].Clone();

                        // Set function name and title by the formula = function_x_bin
                        sprintf(name, "function_%s_%d", histogram2D->GetName(), j);
                        function->SetName(name);
                        function->SetTitle(name);

                        if (j != 1) function->SetParameters(lastParameters);

                        // Fit the histogram
                        histogram1D->Fit(function, "Q", "", function->GetXmin(), function->GetXmax());

                        // Information
                        Info(__FUNCTION__, "Fitting %s chi2/ndf = (%f/%d)", histogram1D->GetName(), function->GetChisquare(), function->GetNDF());

                        // Get the parameter of the last optiomization
                        lastParameters = function->GetParameters();

                        // Saving the function and histograms
                        output->cd("functions");
                        function->Write();
                        output->cd("templates");
                        histogram1D->Write();
                    }
                }
        }
    };

    // Save rebinning information
    char factor[256];
    for (Int_t i = 0; i < Dependency::Dimension; ++i)
        if (rebin_[i] != 1)
        {
            output->cd();
            if (!output->FindKey("parameters")) output->mkdir("parameters");
            output->cd("parameters");
            Info(__FUNCTION__, "Saving rebinning factor %d for %s", rebin_[i], Dependency::Name[i]);
            sprintf(factor, "%d", rebin_[i]);
            TObjString(factor).Write(Dependency::Name[i]);
        }

    return true;
}

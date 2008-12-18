#include "FWCore/Utilities/interface/EDMException.h"
#include "PhysicsTools/Utilities/interface/Parameter.h"
#include "PhysicsTools/Utilities/interface/Gaussian.h"
#include "PhysicsTools/Utilities/interface/Numerical.h"
#include "PhysicsTools/Utilities/interface/Exponential.h"
#include "PhysicsTools/Utilities/interface/Polynomial.h"
#include "PhysicsTools/Utilities/interface/Constant.h"
#include "PhysicsTools/Utilities/interface/Operations.h"
#include "PhysicsTools/Utilities/interface/MultiHistoChiSquare.h"
#include "PhysicsTools/Utilities/interface/RootMinuit.h"
#include "PhysicsTools/Utilities/interface/RootMinuitCommands.h"
#include "PhysicsTools/Utilities/interface/FunctClone.h"
#include "PhysicsTools/Utilities/interface/rootPlot.h"
#include "PhysicsTools/Utilities/interface/Expression.h"
#include "PhysicsTools/Utilities/interface/HistoPdf.h"
#include "TROOT.h"
#include "TH1.h"
#include "TFile.h"
#include <boost/program_options.hpp>
using namespace boost;
namespace po = boost::program_options;

#include <iostream>
#include <algorithm> 
#include <exception>
#include <iterator>
#include <string>
#include <vector>
using namespace std;

// A helper function to simplify the main part.
template<class T>
ostream& operator<<(ostream& os, const vector<T>& v) {
  copy(v.begin(), v.end(), ostream_iterator<T>(cout, " ")); 
  return os;
}

//A function that sets istogram contents to 0 
//if they are too small
void fix(TH1* histo) {
  for(int i = 1; i <= histo->GetNbinsX(); ++i) {
    if(histo->GetBinContent(i) < 0.1) {
      histo->SetBinContent(i, 0.0);
      histo->SetBinError(i, 0.0);
    }
  }
}

struct sig_tag;
struct bkg_tag;

int main(int ac, char *av[]) {
  gROOT->SetStyle("Plain");
  try {
    typedef funct::FunctExpression Expr;
    typedef fit::MultiHistoChiSquare<Expr, Expr, Expr, Expr, Expr> ChiSquared;

    double fMin, fMax;
    string ext;
    po::options_description desc("Allowed options");
    desc.add_options()
      ("help,h", "produce help message")
      ("input-file,i", po::value< vector<string> >(), "input file")
      ("min,m", po::value<double>(&fMin)->default_value(60), "minimum value for fit range")
      ("max,M", po::value<double>(&fMax)->default_value(120), "maximum value for fit range")
      ("plot-format,p", po::value<string>(&ext)->default_value("ps"), 
       "output plot format")
      ;
    
    po::positional_options_description p;
    p.add("input-file", -1);
    
    po::variables_map vm;
    po::store(po::command_line_parser(ac, av).
	    options(desc).positional(p).run(), vm);
    po::notify(vm);
    
    if (vm.count("help")) {
      cout << "Usage: options_description [options]\n";
      cout << desc;
      return 0;
      }
    
    fit::RootMinuitCommands<ChiSquared> commands("csa08HLTFit.txt");

    const unsigned int rebinMuMuNoIso = 2, rebinMuMu = 1, rebinMuMu1HLT = 1, rebinMuMu2HLT = 1, rebinMuTk = 2, rebinMuSa = 8;
    // assume that the bin size is 1 GeV!!!
    funct::Constant rebinMuMuNoIsoConst(rebinMuMuNoIso),rebinMuMuConst(rebinMuMu),rebinMuMu1HLTConst(rebinMuMu1HLT),rebinMuMu2HLTConst(rebinMuMu2HLT), rebinMuTkConst(rebinMuTk), rebinMuSaConst(rebinMuSa);

    if (vm.count("input-file")) {
      cout << "Input files are: " 
	   << vm["input-file"].as< vector<string> >() << "\n";
      vector<string> v_file = vm["input-file"].as< vector<string> >();
      for(vector<string>::const_iterator it = v_file.begin(); 
	  it != v_file.end(); ++it) {
	TFile * root_file = new TFile(it->c_str(),"read");

	TH1D * histoZMuMuNoIso = (TH1D*) root_file->Get("nonIsolatedZToMuMuPlots/zMass");
	histoZMuMuNoIso->Rebin(rebinMuMuNoIso);
	fix(histoZMuMuNoIso);

	TH1D * histoZMuMu = (TH1D*) root_file->Get("goodZToMuMuPlots/zMass");
	histoZMuMu->Rebin(rebinMuMu);
	fix(histoZMuMu);

	TH1D * histoZMuMu1HLT = (TH1D*) root_file->Get("goodZToMuMu1HLTPlots/zMass");
	histoZMuMu1HLT->Rebin(rebinMuMu1HLT);
	fix(histoZMuMu1HLT);

	TH1D * histoZMuMu2HLT = (TH1D*) root_file->Get("goodZToMuMu2HLTPlots/zMass");
	histoZMuMu2HLT->Rebin(rebinMuMu2HLT);
	fix(histoZMuMu2HLT);

	TH1D * histoZMuTk = (TH1D*) root_file->Get("goodZToMuMuOneTrackPlots/zMass");
	histoZMuTk->Rebin(rebinMuTk);
	fix(histoZMuTk);

	TH1D * histoZMuSa = (TH1D*) root_file->Get("goodZToMuMuOneStandAloneMuonPlots/zMass");
	histoZMuSa->Rebin(rebinMuSa);
	fix(histoZMuSa);

	cout << ">>> histogram loaded\n";
	string f_string = *it;
	replace(f_string.begin(), f_string.end(), '.', '_');
	replace(f_string.begin(), f_string.end(), '/', '_');
	string plot_string = f_string + "." + ext;
	cout << ">>> Input files loaded\n";
	
	const char * kYieldZMuMu = "YieldZMuMu";
	const char * kEfficiencyTk = "EfficiencyTk";
	const char * kEfficiencySa = "EfficiencySa";
	const char * kEfficiencyIso = "EfficiencyIso";
	const char * kEfficiencyHLT = "EfficiencyHLT"; 
	const char * kYieldBkgZMuTk = "YieldBkgZMuTk"; 
	const char * kYieldBkgZMuSa = "YieldBkgZMuSa"; 
	const char * kYieldBkgZMuMuNotIso = "YieldBkgZMuMuNotIso";
	const char * kAlpha = "Alpha";
	const char * kBeta = "Beta";
	const char * kLambda = "Lambda";
	const char * kA0 = "A0"; 
	const char * kA1 = "A1"; 
	const char * kA2 = "A2"; 
	const char * kB0 = "B0"; 
	const char * kB1 = "B1"; 
	const char * kB2 = "B2"; 
	const char * kMeanZMuSa = "MeanZMuSa";
	const char * kSigmaZMuSa = "SigmaZMuSa";
	
	funct::Parameter yieldZMuMu(kYieldZMuMu, commands.par(kYieldZMuMu));
	funct::Parameter effTk(kEfficiencyTk, commands.par(kEfficiencyTk)); 
	funct::Parameter effSa(kEfficiencySa, commands.par(kEfficiencySa)); 
	funct::Parameter effIso(kEfficiencyIso, commands.par(kEfficiencyIso)); 
	funct::Parameter effHLT(kEfficiencyHLT, commands.par(kEfficiencyHLT)); 
	funct::Parameter yieldBkgZMuTk(kYieldBkgZMuTk, commands.par(kYieldBkgZMuTk));
	funct::Parameter yieldBkgZMuSa(kYieldBkgZMuSa, commands.par(kYieldBkgZMuSa));
	funct::Parameter yieldBkgZMuMuNotIso(kYieldBkgZMuMuNotIso, commands.par(kYieldBkgZMuMuNotIso));
	funct::Parameter meanZMuSa(kMeanZMuSa, commands.par(kMeanZMuSa));
	funct::Parameter sigmaZMuSa(kSigmaZMuSa, commands.par(kSigmaZMuSa)); 
	funct::Parameter alpha(kAlpha, commands.par(kAlpha));
	funct::Parameter beta(kBeta, commands.par(kBeta));
	funct::Parameter lambda(kLambda, commands.par(kLambda));
	funct::Parameter a0(kA0, commands.par(kA0));
	funct::Parameter a1(kA1, commands.par(kA1));
	funct::Parameter a2(kA2, commands.par(kA2));
	funct::Parameter b0(kB0, commands.par(kB0));
	funct::Parameter b1(kB1, commands.par(kB1));
	funct::Parameter b2(kB2, commands.par(kB2));
	funct::Constant cFMin(fMin), cFMax(fMax);

	// count ZMuMu Yield
	double nZMuMu = 0, nZMuMu1HLT = 0, nZMuMu2HLT = 0;
	{
	  unsigned int nBins = histoZMuMu->GetNbinsX();
	  double xMin = histoZMuMu->GetXaxis()->GetXmin();
	  double xMax = histoZMuMu->GetXaxis()->GetXmax();
	  double deltaX =(xMax - xMin) / nBins;
	  for(size_t i = 0; i < nBins; ++i) { 
	    double x = xMin + (i +.5) * deltaX;
	    if(x > fMin && x < fMax){
	      nZMuMu += histoZMuMu->GetBinContent(i+1);
	      nZMuMu1HLT += histoZMuMu1HLT->GetBinContent(i+1);
	      nZMuMu2HLT += histoZMuMu2HLT->GetBinContent(i+1);
	    }
	  }
	}
	// aggiungi 1HLT 2HLT
	cout << ">>> count of ZMuMu yield in the range [" << fMin << ", " << fMax << "]: " << nZMuMu << endl;
	cout << ">>> count of ZMuMu (1HLT) yield in the range [" << fMin << ", " << fMax << "]: "  << nZMuMu1HLT << endl;
	cout << ">>> count of ZMuMu (2HLT) yield in the range [" << fMin << ", " << fMax << "]: "  << nZMuMu2HLT << endl;
	funct::RootHistoPdf zPdfMuMuNonIso(*histoZMuMu, fMin, fMax);//imposto le pdf a quella si ZMuMu
	funct::RootHistoPdf zPdfMuTk = zPdfMuMuNonIso;
	funct::RootHistoPdf zPdfMuMu1HLT = zPdfMuMuNonIso;
	funct::RootHistoPdf zPdfMuMu2HLT = zPdfMuMuNonIso;
	zPdfMuMuNonIso.rebin(rebinMuMuNoIso);
	zPdfMuTk.rebin(rebinMuTk);
	zPdfMuMu1HLT.rebin(rebinMuMu1HLT);
	zPdfMuMu2HLT.rebin(rebinMuMu2HLT);

	funct::Numerical<2> _2;
	funct::Numerical<1> _1;

	//Efficiency term
	Expr zMuMuEff1HLTTerm = _2 * (effTk ^ _2) *  (effSa ^ _2) * (effIso ^ _2) * effHLT * ( _1 - effHLT); 
	Expr zMuMuEff2HLTTerm = (effTk ^ _2) *  (effSa ^ _2) * (effIso ^ _2) * (effHLT ^ _2) ; 
	Expr zMuMuNoIsoEffTerm = (effTk ^ _2) * (effSa ^ _2) * (_1 - (effIso ^ _2)) * (_1 - (_1 - effHLT) ^ _2);
	Expr zMuTkEffTerm = _2 * (effTk ^ _2) * effSa * (_1 - effSa) * (effIso ^ _2) * effHLT;
	Expr zMuSaEffTerm = _2 * (effSa ^ _2) * effTk * (_1 - effTk) * (effIso ^ _2) * effHLT;


	Expr zMuMu1HLT = rebinMuMu1HLTConst * zMuMuEff1HLTTerm * yieldZMuMu;
	Expr zMuMu2HLT = rebinMuMu2HLTConst * zMuMuEff2HLTTerm * yieldZMuMu;

	Expr zMuTkBkg = yieldBkgZMuTk * funct::Exponential(lambda) * funct::Polynomial<2>(a0, a1, a2);
	Expr zMuTkBkgScaled = rebinMuTkConst * zMuTkBkg;
	Expr zMuTk = rebinMuTkConst * (zMuTkEffTerm * yieldZMuMu * zPdfMuTk + zMuTkBkg);

	Expr zMuMuNoIsoBkg = yieldBkgZMuMuNotIso * funct::Exponential(alpha) * funct::Polynomial<2>(b0, b1, b2);
	Expr zMuMuNoIsoBkgScaled = rebinMuMuNoIsoConst * zMuMuNoIsoBkg;
	Expr zMuMuNoIso = rebinMuMuNoIsoConst * (zMuMuNoIsoEffTerm * yieldZMuMu * zPdfMuMuNonIso + zMuMuNoIsoBkg);

	Expr zMuSa = rebinMuSaConst * (zMuSaEffTerm * yieldZMuMu * funct::Gaussian(meanZMuSa, sigmaZMuSa) 
				       + (yieldBkgZMuSa * funct::Exponential(beta)));

	TH1D histoZCount1HLT("histoZCount1HLT", "", 1, fMin, fMax);
	histoZCount1HLT.Fill(100, nZMuMu1HLT);
	TH1D histoZCount2HLT("histoZCount2HLT", "", 1, fMin, fMax);
	histoZCount2HLT.Fill(100, nZMuMu2HLT);
				       
	ChiSquared chi2(zMuMu1HLT, & histoZCount1HLT,
			zMuMu2HLT, & histoZCount2HLT,
			zMuTk, histoZMuTk, 
			zMuSa, histoZMuSa, 
			zMuMuNoIso,histoZMuMuNoIso,
			fMin, fMax);
	cout << "N. bins: " << chi2.numberOfBins() << endl;
	fit::RootMinuit<ChiSquared> minuit(chi2, true);
	commands.add(minuit, yieldZMuMu);
	commands.add(minuit, effTk);
	commands.add(minuit, effSa);
	commands.add(minuit, effIso);
	commands.add(minuit, effHLT);
	commands.add(minuit, yieldBkgZMuTk);
	commands.add(minuit, yieldBkgZMuSa);
	commands.add(minuit, yieldBkgZMuMuNotIso);
	commands.add(minuit, meanZMuSa);
	commands.add(minuit, sigmaZMuSa);
	commands.add(minuit, lambda);
	commands.add(minuit, alpha);
	commands.add(minuit, beta);
	commands.add(minuit, a0);
	commands.add(minuit, a1);
	commands.add(minuit, a2);
	commands.add(minuit, b0);
	commands.add(minuit, b1);
	commands.add(minuit, b2);
	commands.run(minuit);
	const unsigned int nPar = 19;//WARNIG: this must be updated manually for now
	ROOT::Math::SMatrix<double, nPar, nPar, ROOT::Math::MatRepSym<double, nPar> > err;
	minuit.getErrorMatrix(err);
	std::cout << "error matrix:" << std::endl;
	for(size_t i = 0; i < nPar; ++i) {
	  for(size_t j = 0; j < nPar; ++j) {
	    std::cout << err(i, j) << "\t";
	  }
	  std::cout << std::endl;
	} 
	minuit.printFitResults();
	//Plot
	double s;
	s = 0;
	for(int i = 1; i <= histoZMuMuNoIso->GetNbinsX(); ++i)
	  s += histoZMuMuNoIso->GetBinContent(i);
	histoZMuMuNoIso->SetEntries(s);
	s = 0;
	for(int i = 1; i <= histoZMuMu->GetNbinsX(); ++i)
	  s += histoZMuMu->GetBinContent(i);
	histoZMuMu->SetEntries(s);
	s = 0;
	for(int i = 1; i <= histoZMuMu1HLT->GetNbinsX(); ++i)
	  s += histoZMuMu1HLT->GetBinContent(i);
	histoZMuMu1HLT->SetEntries(s);
	s = 0;
	for(int i = 1; i <= histoZMuMu2HLT->GetNbinsX(); ++i)
	  s += histoZMuMu2HLT->GetBinContent(i);
	histoZMuMu2HLT->SetEntries(s);
	s = 0;
	for(int i = 1; i <= histoZMuTk->GetNbinsX(); ++i)
	  s += histoZMuTk->GetBinContent(i);
	histoZMuTk->SetEntries(s);
	s = 0;
	for(int i = 1; i <= histoZMuSa->GetNbinsX(); ++i)
	  s += histoZMuSa->GetBinContent(i);
	histoZMuSa->SetEntries(s);

	string ZMuMu1HLTPlot = "ZMuMu1HLTFit_" + plot_string;
	root::plot<Expr>(ZMuMu1HLTPlot.c_str(), *histoZMuMu1HLT, zMuMu1HLT, fMin, fMax, 
			  effTk, effSa, effIso, effHLT, yieldZMuMu, 
			  kRed, 2, kDashed, 100, 
			  "Z -> #mu #mu mass", "#mu #mu invariant mass (GeV/c^{2})", 
			  "Events");

	string ZMuMu2HLTPlot = "ZMuMu2HLTFit_" + plot_string;
	root::plot<Expr>(ZMuMu2HLTPlot.c_str(), *histoZMuMu2HLT, zMuMu2HLT, fMin, fMax, 
			  effTk, effSa, effIso, effHLT, yieldZMuMu, 
			  kRed, 2, kDashed, 100, 
			  "Z -> #mu #mu mass", "#mu #mu invariant mass (GeV/c^{2})", 
			  "Events");

	
	string ZMuMuNoIsoPlot = "ZMuMuNoIsoFit_" + plot_string;
	root::plot<Expr>(ZMuMuNoIsoPlot.c_str(), *histoZMuMuNoIso, zMuMuNoIso, fMin, fMax, 
			 effTk, effSa, effIso, effHLT, yieldZMuMu,
			 kRed, 2, kDashed, 100, 
			 "Z -> #mu #mu Not Iso mass", "#mu #mu invariant mass (GeV/c^{2})", 
			 "Events");	
	
	string ZMuTkPlot = "ZMuTkFit_X_" + plot_string;
	root::plot<Expr>(ZMuTkPlot.c_str(), *histoZMuTk, zMuTk, fMin, fMax,
			 effTk, effSa, effIso, effHLT, yieldZMuMu,
			 yieldBkgZMuTk, lambda, a0, a1, a2,
			 kRed, 2, kDashed, 100,
                         "Z -> #mu + (unmatched) track mass", "#mu #mu invariant mass (GeV/c^{2})",
                         "Events");
	ZMuTkPlot = "ZMuTkFit_" + plot_string;
	TF1 funZMuTk = root::tf1_t<sig_tag, Expr>("ZMuTkFunction", zMuTk, fMin, fMax, 
						  effTk, effSa, effIso, effHLT, yieldZMuMu, 
						  yieldBkgZMuTk, lambda, a0, a1, a2);
	funZMuTk.SetLineColor(kRed);
	funZMuTk.SetLineWidth(2);
	funZMuTk.SetLineStyle(kDashed);
	funZMuTk.SetNpx(10000);
	TF1 funZMuTkBkg = root::tf1_t<bkg_tag, Expr>("ZMuTkBack", zMuTkBkgScaled, fMin, fMax, 
						     yieldBkgZMuTk, lambda, a0, a1, a2);
	funZMuTkBkg.SetLineColor(kGreen);
	funZMuTkBkg.SetLineWidth(2);
	funZMuTkBkg.SetLineStyle(kDashed);
	funZMuTkBkg.SetNpx(10000);
	histoZMuTk->SetTitle("Z -> #mu + (unmatched) track mass");
	histoZMuTk->SetXTitle("#mu + (unmatched) track invariant mass (GeV/c^{2})");
	histoZMuTk->SetYTitle("Events");
	TCanvas *canvas = new TCanvas("canvas");
	histoZMuTk->Draw("e");
	funZMuTkBkg.Draw("same");
	funZMuTk.Draw("same");
	canvas->SaveAs(ZMuTkPlot.c_str());
	canvas->SetLogy();
	string logZMuTkPlot = "log_" + ZMuTkPlot;
	canvas->SaveAs(logZMuTkPlot.c_str());
	string ZMuSaPlot = "ZMuSaFit_" + plot_string;
	root::plot<Expr>(ZMuSaPlot.c_str(), *histoZMuSa, zMuSa, fMin, fMax, 
			 effSa, effTk, effIso,
			 yieldZMuMu, meanZMuSa, sigmaZMuSa, yieldBkgZMuSa, 
			 kRed, 2, kDashed, 10000, 
			 "Z -> #mu + (unmatched) standalone mass", 
			 "#mu + (unmatched) standalone invariant mass (GeV/c^{2})", 
			 "Events");
      }
    }
  }
  catch(exception& e) {
    cerr << "error: " << e.what() << "\n";
    return 1;
  }
  catch(...) {
    cerr << "Exception of unknown type!\n";
  }

  return 0;
}




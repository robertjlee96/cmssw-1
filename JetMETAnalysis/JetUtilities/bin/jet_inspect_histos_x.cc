////////////////////////////////////////////////////////////////////////////////
//
// jet_inspect_histos_x
// --------------------
//
//            07/19/2009 Philipp Schieferdecker <philipp.schieferdecker@cern.ch>
////////////////////////////////////////////////////////////////////////////////


#include "JetMETAnalysis/JetUtilities/interface/CommandLine.h"
#include "JetMETAnalysis/JetUtilities/interface/ObjectLoader.h"
#include "JetMETAnalysis/JetUtilities/interface/RootStyle.h"

#include <TApplication.h>
#include <TStyle.h>
#include <TFile.h>
#include <TCanvas.h>
#include <TLatex.h>
#include <TH1F.h>
#include <TF1.h>
#include <TText.h>
#include <TLine.h>
#include <TLegend.h>

#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>


using namespace std;


////////////////////////////////////////////////////////////////////////////////
// declare local functions
////////////////////////////////////////////////////////////////////////////////
void set_xaxis_range(TH1* h1,TH1* h2=0,TH1* h3=0,TH1* h4=0);
void get_xaxis_range(TH1* h,int& binmin,int& binmax);
void set_yaxis_range(TH1* h1,float ymin=-1.,float ymax=-1.);
void set_draw_attributes(TH1* h,
			 unsigned index,
			 bool fill,
			 const vector<int>& colors,
			 const vector<int>& fillstyles);
void draw_stats(TH1* h,double xoffset,Color_t color,Color_t fitColor);
void draw_range(const ObjectLoader<TH1F>& hl,
		const vector<unsigned int>& indices,
		bool  addFixedVars=true);
void draw_line_mean(TH1* h);
void draw_line_median(TH1* h);
void draw_line_peak(TH1* h);
void draw_zline(TH1* h1);
void draw_line_legend(bool mean,bool median,bool peak);

void draw_residual(TPad* pad,TH1* hist,TF1* func,
		   TPad*& hpad, TPad*& rpad,
		   int errMode=-1,//-1:no residuals//0:chi2perbin,3:relative
		   bool firsthisto=true,
		   const string input="",
		   const string alg="",
		   const string variable="");


////////////////////////////////////////////////////////////////////////////////
// main
////////////////////////////////////////////////////////////////////////////////

//______________________________________________________________________________
int main(int argc,char** argv)
{
  CommandLine cl;
  if (!cl.parse(argc,argv)) return 0;

  vector<string> inputs     = cl.getVector<string>("inputs");
  vector<string> algs       = cl.getVector<string>("algs",          "kt4calo");
  vector<string> variables  = cl.getVector<string>("variables","RelRsp:RefPt");
  int            npercanvas = cl.getValue<int>    ("npercanvas",            0);
  float          ymin       = cl.getValue<float>  ("ymin",                 -1);
  float          ymax       = cl.getValue<float>  ("ymax",                 -1);
  bool           norm       = cl.getValue<bool>   ("norm",              false);
  bool           mean       = cl.getValue<bool>   ("mean",              false);
  bool           median     = cl.getValue<bool>   ("median",            false);
  bool           peak       = cl.getValue<bool>   ("peak",              false);
  bool           logx       = cl.getValue<bool>   ("logx",              false);
  bool           logy       = cl.getValue<bool>   ("logy",              false);
  bool           fill       = cl.getValue<bool>   ("fill",               true);
  bool           fullfit    = cl.getValue<int>    ("fullfit",            true);
  vector<int>    colors     = cl.getVector<int>   ("colors",               "");
  vector<int>    fillstyles = cl.getVector<int>   ("fillstyles",           "");
  string         prefix     = cl.getValue<string> ("prefix",               "");
  string         suffix     = cl.getValue<string> ("suffix",               "");
  string         opath      = cl.getValue<string> ("opath",                "");
  vector<string> formats    = cl.getVector<string>("formats",              "");
  bool           batch      = cl.getValue<bool>   ("batch",             false);
  int            residual   = cl.getValue<int>    ("residual",              -1);


  if (!cl.check()) return 0;
  cl.print();

  if (prefix.empty()) prefix=algs[0];
  
  if (batch&&formats.size()==0) formats.push_back("pdf");

  if (!fillstyles.empty() && (colors.size()!=fillstyles.size())) {
    cout<<"Error: #Fillstyles has no corresponding colors!"<<endl;return 0;
  }
  
  argc = (batch) ? 2 : 1; if (batch) argv[1] = (char*)"-b";
  TApplication* app=new TApplication("jet_inspect_histos",&argc,argv);
  
  set_root_style();
    
  gStyle->SetOptStat(0);
  gStyle->SetOptFit(0);
  gStyle->SetOptTitle(0);//hh 11.04.2010
  
  vector<TCanvas*> c; int nx(1),ny(1);
  
  /// LOOP OVER FILES
  for (unsigned int ifile=0;ifile<inputs.size();ifile++) {

    string input(inputs[ifile]);
    TFile* file=new TFile(input.c_str(),"READ");
    if (!file->IsOpen()) {cout<<"Can't open "<<file->GetName()<<endl;return 0;}
    
    /// LOOP OVER ALGORITHMS
    for (unsigned int ialg=0;ialg<algs.size();ialg++) {

      string alg = algs[ialg];
      TDirectory* dir =(TDirectory*)file->Get(alg.c_str());
      if (0==dir) { cout<<"No dir "<<algs[ialg]<<" found"<<endl; return 0; }

      /// LOOP OVER VARIABLES
      for (unsigned int ivar=0;ivar<variables.size();ivar++) {
	
	string variable=variables[ivar];

	ObjectLoader<TH1F> hl;
	hl.load_objects(dir,variable);
	
	bool put_range = (npercanvas!=0||
			  (hl.nvariables()>0&&hl.nobjects(hl.nvariables()-1)==1));
	
	if (ifile==0&&ialg==0&&ivar==0) {
	  if (npercanvas==0)
	    npercanvas= (hl.nvariables()==0)? 1 : hl.nobjects(hl.nvariables()-1);
	  nx=(int)std::sqrt((float)npercanvas);
	  ny=nx;
	  if (nx*ny<npercanvas) nx++;
	  if (nx*ny<npercanvas) ny++;
	}
	
	hl.begin_loop();
	
	vector<unsigned int> indices; TH1F* h(0); unsigned int ihisto(0);
	unsigned int icolor(0);
	while ((h=hl.next_object(indices))) {

	  if (norm) {
	    TF1* f = h->GetFunction("fit");
	    if (0!=f) f->SetParameter(0,f->GetParameter(0)/h->Integral());
	    h->Sumw2();
	    h->Scale(1./h->Integral());
	  }
	  

	  if (ifile==0&&ialg==0&&ivar==0&&
	      (c.size()==0||ihisto%npercanvas==0)) {
	    stringstream sscname;sscname<<prefix<<"_"<<hl.quantity();
	    for (unsigned int i=0;i<hl.nvariables();i++) {
	      if (variables.size()>1&&hl.nobjects(i)==1) continue;
	      sscname<<"_"<<hl.variable(i);
	      if (i<hl.nvariables()-1||put_range)
		sscname<<hl.minimum(i,indices[i])<<"to"
		       <<hl.maximum(i,indices[i]);
	    }
	    if (!suffix.empty()) sscname<<"_"<<suffix;

	    c.push_back(new TCanvas(sscname.str().c_str(),
				    sscname.str().c_str(),
				    1000,1000));
	    c.back()->Divide(nx,ny,1e-04,1e-04);
	  }

	  int icnv = ihisto/npercanvas;
	  int ipad = ihisto%npercanvas+1;
	  c[icnv]->cd(ipad);
	  
	  if (ifile==0&&ialg==0&&ivar==0) {
	    icolor=0;
	    if (logx&&(h->GetEntries()>0)) gPad->SetLogx();
	    if (logy&&(h->GetEntries()>0)) gPad->SetLogy();
	    gPad->SetLeftMargin(0.13);
	    gPad->SetRightMargin(0.05);
	    gPad->SetTopMargin(0.12);
	    gPad->SetBottomMargin(0.15);

	    if (ymin!=-1 || ymax!=-1) set_yaxis_range(h,ymin,ymax);
	    if (colors.empty()) h->SetLineColor(kBlack);
	    else h->SetLineColor(colors[icolor]);
	    set_draw_attributes(h,icolor,fill,colors,fillstyles);

	    h->SetMaximum(1.5*h->GetMaximum());
	    if (logy&&(h->GetEntries()>0)) h->SetMaximum(10.*h->GetMaximum());

	    TPad* hpad(0); TPad* rpad(0); //pointer to new histo/resi pads
	    draw_residual((TPad*)gPad,h,h->GetFunction("fit"),hpad,rpad,
			  residual,true,input,alg,h->GetName()); 

	    TH1F* r1(0);
	    if (0!=rpad) r1 = (TH1F*) rpad->GetListOfPrimitives()->First();
	    set_xaxis_range(h,0,r1);
	    if (0!=rpad) rpad->cd();draw_zline(r1);hpad->cd();
    
	    if (0==hpad) h->Draw("EH");
	    else hpad->cd();

	    if (colors.empty()) draw_stats(h,0.65,kBlack,kBlack);
	    else draw_stats(h,0.65,colors[icolor],colors[icolor]);
	    draw_range(hl,indices,(variables.size()==1));
	    draw_line_legend(mean,median,peak);
	  }
	  else {
	    icolor=ifile+ialg+ivar;
	    if (colors.empty()) h->SetLineColor(kBlue);
	    else if (icolor>colors.size()-1) {
	      cout<<"WARNING: #Histo Vs specified colors mismatch!"<<endl;
	      h->SetLineColor(kBlue);
	    }
	    else h->SetLineColor(colors[icolor]);
	    set_draw_attributes(h,icolor,fill,colors,fillstyles);

	    TPad* hpad(0); TPad* rpad(0);
	    draw_residual((TPad*)gPad,h,h->GetFunction("fit"),hpad,rpad,
			  residual,false,input,alg,h->GetName()); 
    
	    if (0==hpad) h->Draw("EHSAME");
	    else hpad->cd();

	    if (colors.empty() || (icolor>colors.size()-1)) 
	      draw_stats(h,0.15,kBlue,kBlue);
	    else draw_stats(h,0.15,colors[icolor],colors[icolor]);

	    //TH1F* hdraw = (TH1F*)gPad->GetListOfPrimitives()->First();

	    TH1F* r1(0); TH1F* r2(0);
	    TH1F* h1 = (TH1F*) hpad->GetListOfPrimitives()->First();
	    if (0!=rpad) r1 = (TH1F*) rpad->GetListOfPrimitives()->First();
	    if (0!=rpad) r2 = (TH1F*) rpad->GetListOfPrimitives()->Last();
	    set_xaxis_range(h1,h,r1,r2);
	    if (0!=rpad) rpad->cd();draw_zline(r1);hpad->cd();


	    if (ymin!=-1 || ymax!=-1) set_yaxis_range(h1,ymin,ymax);
	    if (h->GetMaximum()>h1->GetMaximum())
	      h1->SetMaximum(1.2*h->GetMaximum());
	  }
	  if (mean)   draw_line_mean(h);
	  if (median) draw_line_median(h);
	  if (peak)   draw_line_peak(h);

	  TF1* f = h->GetFunction("fit");
	  if (fullfit&&(0!=f)){
	    TF1* ff = (TF1*)f->Clone("ff");
	    ff->SetRange(h->GetXaxis()->GetXmin(),h->GetXaxis()->GetXmax());
	    ff->SetLineColor(f->GetLineColor());
	    ff->SetLineStyle(kDashed);
	    ff->SetLineWidth(2);
	    ff->Draw("SAME");
	  }
	  
	  ihisto++;
	  
	} // histos
      } // variables
    } // algorithms
  } // inputs
  
  for (unsigned int ic=0;ic<c.size();ic++) {
    string output = c[ic]->GetName();
    if (!opath.empty()) output = opath + "/" + output;
    for (unsigned int ifmt=0;ifmt<formats.size();ifmt++) {
      c[ic]->Print((output+"."+formats[ifmt]).c_str());
    }
  }
  
  if (!batch) app->Run();
  
  return 0;
}


////////////////////////////////////////////////////////////////////////////////
// implement local functions
////////////////////////////////////////////////////////////////////////////////



//______________________________________________________________________________
void draw_residual(TPad* pad,TH1* hist,TF1* func,
		   TPad*& hpad,TPad*& rpad,
		   int errMode,
		   bool firstHisto,
		   const string input,
		   const string alg,
		   const string variable)
{
  if (errMode<0) return;
  else if (errMode>3) {
    cout<<"ERROR: draw_residual() invalid error mode"<<endl;return;
  }
  if (0==pad || 0==hist) return;


  //divide this pad
  if (firstHisto) pad->Divide(1,2,0.01,0.0);

  // make default settings
  if (firstHisto) pad->GetPad( 1 )->SetFillColor( 0 );
  if (firstHisto) pad->GetPad( 2 )->SetFillColor( 0 );
  if (firstHisto) pad->GetPad( 1 )->SetPad( 0.0,0.25,1.0, 1.0 );
  if (firstHisto) pad->GetPad( 2 )->SetPad( 0.0, 0.0,1.0,0.25 );

  if (firstHisto) pad->cd(1);
  if (firstHisto && pad->GetLogx()) gPad->SetLogx();
  if (firstHisto && pad->GetLogy()) gPad->SetLogy();
  if (firstHisto) gPad->SetTopMargin(0.1);
  if (firstHisto) gPad->SetLeftMargin(0.13);
  if (firstHisto) gPad->SetRightMargin(0.05);

  if (firstHisto) pad->cd(2);
  if (firstHisto && pad->GetLogx())gPad->SetLogx();
  if (firstHisto) gPad->SetBottomMargin(0.375);
  if (firstHisto) gPad->SetLeftMargin(0.13);
  if (firstHisto) gPad->SetRightMargin(0.05);

  // define residual histo
  
  stringstream ssresname;
  ssresname<<"rsh_"<<input<<"_"<<alg<<"_"<<variable;
  TH1F *resiHist = new TH1F(ssresname.str().c_str(),
			    "Residual Histogram ",
			    hist->GetNbinsX(),
			    hist->GetXaxis()->GetXmin(), 
			    hist->GetXaxis()->GetXmax());

  resiHist->Sumw2();
  resiHist->SetLineWidth(1);
  resiHist->SetXTitle(hist->GetXaxis()->GetTitle());
  resiHist->GetYaxis()->CenterTitle(1);
  resiHist->GetYaxis()->SetTitleSize( 0.13 );
  resiHist->GetYaxis()->SetTitleOffset( 0.34 );
  resiHist->GetYaxis()->SetLabelSize( 0.13 );
  resiHist->GetYaxis()->SetNdivisions( 505 );
  resiHist->GetXaxis()->SetTitleSize( 0.16 );
  resiHist->GetXaxis()->SetLabelSize( 0.16 );
  resiHist->GetXaxis()->SetTitleOffset( 1 );
  resiHist->GetXaxis()->SetLabelOffset( 0.006 );
  resiHist->GetXaxis()->SetNdivisions( 505 );
  resiHist->GetXaxis()->SetTickLength( resiHist->GetXaxis()->GetTickLength() * 3.0 );

  resiHist->SetFillStyle(hist->GetFillStyle());
  resiHist->SetFillColor(hist->GetLineColor());
  resiHist->SetMarkerColor(hist->GetLineColor());
  if (3==errMode) resiHist->SetLineColor(hist->GetLineColor());
  else resiHist->SetLineColor(kBlack);

  if ( errMode == 0 )
    resiHist->SetYTitle( "#frac{(data - fit)}{#sqrt{data}}" );
  else if ( errMode == 1 )
    resiHist->SetYTitle( "#frac{(data - fit)}{#sqrt{fit}}" );
  else if (errMode == 2)
    resiHist->SetYTitle( "#frac{(data - fit)}{binerror}" );
  else 
    resiHist->SetYTitle( "#frac{(data-fit)}{data}" );
  


  int    NDF   = 0;
  int    chi2  = 0;
  double nEvts = 0;
  int    nBins = 0;

  double evtThresh = 0.0;
  if( errMode != 1 )
    {
      evtThresh = 0.01 * hist->GetMaximum();
    }
  double evtErr, theory, lowEdge, highEdge, binChi2;

  double binWidth = hist->GetXaxis()->GetBinWidth( 1 );

  int firstBin = 1;
  int lastBin = hist->GetNbinsX();

  int binStarted = firstBin;

  double funcMin, funcMax;
  if (0!=func) func->GetRange( funcMin, funcMax );
  else {
    funcMin=hist->GetXaxis()->GetXmin();
    funcMax=hist->GetXaxis()->GetXmax();
  }

  // Go through all bins excluding unterflow and overlow bins
  for( int i = firstBin; i <= lastBin; i++ ) {
    
    // Are we outside the definition ranges of the function?
    if( hist->GetXaxis()->GetBinUpEdge( i ) < funcMin || 
	hist->GetXaxis()->GetBinLowEdge( i ) > funcMax ) {
      resiHist->SetBinContent( i, 0.0 );
      binStarted = i;
      continue;
    }
    
    if (0==func) continue;
    
    nEvts += double( hist->GetBinContent( i ) );
      
    if ( ( nEvts <= evtThresh ) && ( i != hist->GetNbinsX() ) )
      continue;
      
    lowEdge  = hist->GetXaxis()->GetBinLowEdge( binStarted );
    highEdge = hist->GetXaxis()->GetBinUpEdge( i );
    theory = func->Integral( lowEdge, highEdge ) / binWidth;
      
    if ( errMode == 0 )
      evtErr = sqrt( nEvts );
    else if ( errMode == 1 )
      evtErr = sqrt( theory );
    else
      evtErr = hist->GetBinError( i );
    
    if(!(evtErr == 0)) {
      binChi2 = ( nEvts - theory ) / evtErr;
      binChi2 *= binChi2;
      
      chi2 += binChi2;
	
      for( int jTmp = binStarted; jTmp <= i; jTmp++ ) {
	if ( 3!=errMode) resiHist->SetBinContent( jTmp, ( nEvts - theory ) / evtErr );
	else {
	  resiHist->SetBinContent( jTmp, (nEvts-theory)/nEvts );
	  resiHist->SetBinError( jTmp, theory/nEvts/nEvts*evtErr);
	}
      }
	
      binStarted = i + 1;
      nEvts = 0;
      nBins++;
    }
  }
  if (0!=func) {

    // Why ?
    nBins -= 1;
    
    int npar = func->GetNpar();
    
    double parmin, parmax;
    int fixed = 0;
    
    for( int i = 0; i < npar; i++ ) {
      func->GetParLimits( i, parmin, parmax );
      if ( parmin >= parmax ) fixed++;
    }
    
    NDF = nBins - npar + fixed;
    
    float hmax = std::max(abs(resiHist->GetMaximum()),abs(resiHist->GetMinimum()));
    hmax *=1.2;
    if (hmax>=.5) hmax = .49999;
    resiHist->SetMaximum(1.*hmax);
    resiHist->SetMinimum(-1.*hmax);
    
  }
      
  pad->cd(2);
  rpad = (TPad*)gPad;
      
  if (firstHisto)
    resiHist->Draw("E1");
  else {
    TH1F* rhist = (TH1F*)gPad->GetListOfPrimitives()->First();
    float hmax  = std::max(abs(resiHist->GetMaximum()),abs(resiHist->GetMinimum()));
    hmax *=1.2;
    if (hmax>=.5) hmax = .49999;
    if (hmax>abs(rhist->GetMaximum())) {
      resiHist->SetMaximum(hmax);resiHist->SetMinimum(-1.*hmax);
	  rhist->SetMaximum(resiHist->GetMaximum());
	  rhist->SetMinimum(resiHist->GetMinimum());
    }
    resiHist->Draw("E1SAME");
  }
  pad->cd(1);
  hpad = (TPad*)gPad;
  if (firstHisto) hist->Draw("EH");
  else hist->Draw("EHSAME");

}




//______________________________________________________________________________
void set_xaxis_range(TH1* h1,TH1* h2,TH1* h3,TH1* h4)
{
  if (h1->GetEntries()==0) return;
  int binmin,binmax;
  get_xaxis_range(h1,binmin,binmax);
  if (0!=h2) {
    int binmin2,binmax2;
    get_xaxis_range(h2,binmin2,binmax2);
    binmin = std::min(binmin,binmin2);
    binmax = std::max(binmax,binmax2);
  }
  h1->GetXaxis()->SetRange(binmin,binmax);
  if (0!=h2) h2->GetXaxis()->SetRange(binmin,binmax);
  if (0!=h3) h3->GetXaxis()->SetRange(binmin,binmax);
  if (0!=h4) h4->GetXaxis()->SetRange(binmin,binmax);
}

//______________________________________________________________________________
void set_yaxis_range(TH1* h1,float ymin,float ymax)
{
  if (h1->GetEntries()==0) return;
  if (ymin!=-1) h1->SetMinimum(ymin);
  if (ymax!=-1) h1->SetMaximum(ymax);
}

//______________________________________________________________________________
void get_xaxis_range(TH1* h,int& binmin,int &binmax)
{
  binmin=-1; binmax=-1;
  for (int i=1;i<h->GetNbinsX();i++) {
    double bc = h->GetBinContent(i);
    if (bc>0) {
      if (binmin==-1) binmin=i;
      binmax=i;
    }
  }
}


//______________________________________________________________________________
void set_draw_attributes(TH1* h,unsigned index,bool fill,
			 const vector<int>& colors,
			 const vector<int>& fillstyles)
{
  if (fill) {
    Style_t fillstyle = (fillstyles.empty() || (index>fillstyles.size()-1)) ? 
      (3001+index) : fillstyles[index];
    h->SetFillColor(h->GetLineColor());
    h->SetFillStyle(fillstyle);
  }
  TF1* fitfnc = h->GetFunction("fit");
  if (0!=fitfnc) {
    fitfnc->SetLineWidth(2);
    fitfnc->SetLineColor(h->GetLineColor());
  }
}


//______________________________________________________________________________
void draw_stats(TH1* h,double xoffset,Color_t color,Color_t fitColor)
{
  TF1* fitfnc = h->GetFunction("fit");
  stringstream ssentries;
  ssentries<<setw(6) <<setiosflags(ios::left)<<"N:"
	   <<setw(10)<<resetiosflags(ios::left)<<setprecision(4)<<h->GetEntries();
  
  stringstream ssmean;
  ssmean<<setw(6)<<setiosflags(ios::left)<<"Mean:"
	<<setw(9)<<resetiosflags(ios::left)<<setprecision(4)<<h->GetMean();

  stringstream ssrms;
  ssrms<<setw(6)<<setiosflags(ios::left)<<"RMS:"
       <<setw(9)<<resetiosflags(ios::left)<<setprecision(4)<<h->GetRMS();

  stringstream sspeak;
  if (0!=fitfnc)
    sspeak<<setw(6)<<setiosflags(ios::left)<<"Peak: "
	  <<setw(10)<<resetiosflags(ios::left)<<setprecision(4)
	  <<fitfnc->GetParameter(1);

  stringstream sssgma;
  if (0!=fitfnc)
    sssgma<<setw(6)<<setiosflags(ios::left)<<"Sigma:"
	  <<setw(9)<<resetiosflags(ios::left)<<setprecision(4)
	  <<fitfnc->GetParameter(2);
  
  TLatex stats;
  stats.SetNDC(true);
  stats.SetTextAlign(12);
  stats.SetTextSize(0.045);
  stats.SetTextColor(color);
  stats.SetTextFont(42);
  
  stats.DrawLatex(xoffset,0.84,ssentries.str().c_str());
  stats.DrawLatex(xoffset,0.805,ssmean.str().c_str());
  stats.DrawLatex(xoffset,0.77,ssrms.str().c_str());
  if (0!=fitfnc) {
    stats.SetTextColor(fitColor);
    stats.DrawLatex(xoffset,0.735,sspeak.str().c_str());
    stats.DrawLatex(xoffset,0.70,sssgma.str().c_str());
  }
}


//______________________________________________________________________________
void draw_range(const ObjectLoader<TH1F>& hl,
		const vector<unsigned int>& indices,
		bool  addFixedVars)
{
  TLatex range;
  range.SetNDC(true);
  range.SetTextAlign(13);
  range.SetTextSize(0.055);
  range.SetTextFont(42);

  if (hl.nvariables()>2) range.SetTextSize(0.055-(hl.nvariables()*.0055));

  string varnameEta = "#eta";
  for (unsigned int i=0;i<hl.nvariables();i++)
    if (hl.variable(i)=="JetEta"&&hl.minimum(i,0)>=0) varnameEta="|#eta|";
  
  string varnameY = "y";
  for (unsigned int i=0;i<hl.nvariables();i++)
    if (hl.variable(i)=="JetY"&&hl.minimum(i,0)>=0) varnameY="|y|";
  
  stringstream ssrange;

  for (unsigned int i=0;i<hl.nvariables();i++) {
    
    if (hl.nobjects(i)==1&&!addFixedVars) continue;
    
    string varname = hl.variable(i);
    string unit    = "";
    double varmin  = hl.minimum(i,indices[i]);
    double varmax  = hl.maximum(i,indices[i]);
    bool   threshold(false);
    
    if (varname=="RefPt")    { varname = "p_{T}^{REF}"; unit = " GeV"; }
    if (varname=="JetPt")    { varname = "p_{T}";       unit = " GeV"; }
    if (varname=="JetEta")   { varname = varnameEta;    unit =     ""; }
    if (varname=="JetY")     { varname = varnameY;      unit =     ""; }
    if (varname=="JetPhi")   { varname = "#varphi";     unit =     ""; }
    if (varname=="PtRel")    { varname = "p_{T}^{rel}", unit = " GeV"; }
    if (varname=="RelLepPt") { varname = "p_{T}^{l}/p_{T}^{jet}",unit = ""; }
    if (varname=="ThreshPt") { varname = "p_{T}^{3rd}", unit = " GeV"; 
      threshold = true; }

    if (threshold) ssrange<<varname<<" < "<<varmax<<unit<<"    ";
    else ssrange<<varmin<<" < "<<varname<<" < "<<varmax<<unit<<"    ";
  }
  
  range.DrawLatex(0.13,0.96,ssrange.str().c_str());
}


//______________________________________________________________________________
void draw_zline(TH1* h1)
{
  if (0==h1) return;
  float xmin = h1->GetBinLowEdge(h1->GetXaxis()->GetFirst());
  float xmax = h1->GetBinLowEdge(h1->GetXaxis()->GetLast()+1);

  TLine* zline = new TLine(xmin,0,xmax,0);
  zline->SetLineStyle(kDashed);
  zline->SetLineWidth(1);
  zline->Draw("SAME");
}

//______________________________________________________________________________
void draw_line_mean(TH1* h)
{
  double mean = h->GetMean();
  int binMean;
  for (binMean=1;binMean<h->GetNbinsX();binMean++) {
    float binLowEdge = h->GetBinLowEdge(binMean);
    float binHighEdge = binLowEdge + h->GetBinWidth(binMean);
    if (mean>=binLowEdge&&mean<binHighEdge) break;
  }
  double xmin = mean;
  double xmax = mean;
  double ymin = h->GetMinimum();
  double ymax = h->GetBinContent(binMean);
  TLine* lineMean = new TLine(xmin,ymin,xmax,ymax);
  lineMean->SetLineColor(h->GetLineColor());
  lineMean->SetLineStyle(2);
  lineMean->SetLineWidth(3);
  lineMean->Draw("SAME");
}


//______________________________________________________________________________
void draw_line_median(TH1* h)
{
  double median;
  double prb(0.5);
  h->GetQuantiles(1,&median,&prb);
  int binMedian;
  for (binMedian=1;binMedian<h->GetNbinsX();binMedian++) {
    double binLowEdge = h->GetBinLowEdge(binMedian);
    double binHighEdge = binLowEdge + h->GetBinWidth(binMedian);
    if (median>=binLowEdge&&median<binHighEdge) break;
  }
  double xmin = median;
  double xmax = median;
  double ymin = h->GetMinimum();
  double ymax = h->GetBinContent(binMedian);
  TLine* lineMedian = new TLine(xmin,ymin,xmax,ymax);
  lineMedian->SetLineColor(h->GetLineColor());
  lineMedian->SetLineStyle(3);
  lineMedian->SetLineWidth(3);
  lineMedian->Draw("SAME");
}


//______________________________________________________________________________
void draw_line_peak(TH1* h)
{
  TF1* fitFnc = h->GetFunction("fit"); if (0==fitFnc) return;
  double peak = fitFnc->GetParameter(1);
  double xmin = peak;
  double xmax = peak;
  double ymin = h->GetMinimum();
  double ymax = fitFnc->Eval(peak);
  TLine* linePeak = new TLine(xmin,ymin,xmax,ymax);
  linePeak->SetLineColor(h->GetLineColor());
  linePeak->SetLineStyle(4);
  linePeak->SetLineWidth(3);
  linePeak->Draw("SAME");
}


//______________________________________________________________________________
void draw_line_legend(bool mean,bool median,bool peak)
{
  if (!mean&&!median&&!peak) return;
  TLegend* leg = new TLegend(0.75,0.5,0.95,0.5-(mean+median+peak)*0.055);
  leg->SetLineColor(10);
  leg->SetFillColor(10);
  leg->SetBorderSize(0);
  if (mean) {
    TLine* lMean = new TLine();
    lMean->SetLineWidth(3);
    lMean->SetLineStyle(2);
    leg->AddEntry(lMean,"mean","l");
  }
  if (median) {
    TLine* lMedian = new TLine();
    lMedian->SetLineWidth(3);
    lMedian->SetLineStyle(3);
    leg->AddEntry(lMedian,"median","l");
  }
  if (peak) {
    TLine* lPeak = new TLine();
    lPeak->SetLineWidth(3);
    lPeak->SetLineStyle(4);
    leg->AddEntry(lPeak,"peak","l");
  }
  leg->Draw();
}


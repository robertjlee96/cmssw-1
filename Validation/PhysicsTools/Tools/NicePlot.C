
class Style : public TH1F {
};

Style *s1;
Style *s2;
Style *s3;

Style *sg1;
Style *sback;

void InitNicePlot() {
  s1 = new Style(); 

  s1->SetLineWidth(2);   
  s1->SetLineColor(1);
//  s1->SetLineStyle(1);   

  s2 = new Style(); 

  s2->SetLineWidth(2);   
  s2->SetLineColor(3);   
//  s2->SetLineStyle(2);

  s3 = new Style();
  s3->SetLineColor(4);
  s3->SetLineWidth(1);
//  s3->SetLineStyle(4);

  sg1 = new Style();

  sg1->SetMarkerColor(4);
  sg1->SetLineColor(4);
  sg1->SetLineWidth(2);  
  sg1->SetMarkerStyle(21);

  sback =  new Style();
  sback->SetFillStyle(1001);  
  sback->SetFillColor(5);  
//    sback->SetFillColor(3);
  
}


void FormatHisto( TH1* h, const Style* s ) {
  //  h->SetStats(0);
  h->GetYaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetTitleOffset(1.2);
  h->GetXaxis()->SetTitleSize(0.06);
  h->GetYaxis()->SetLabelSize(0.045);
  h->GetXaxis()->SetLabelSize(0.045);

  h->SetLineWidth( s->GetLineWidth() );
  h->SetLineColor( s->GetLineColor() );
  h->SetFillStyle( s->GetFillStyle() );
  h->SetFillColor( s->GetFillColor() );
}

void FormatPad( TPad* pad, bool grid = true) {
  
  
  pad->SetGridx(grid);
  pad->SetGridy(grid);
  
  pad->SetBottomMargin(0.14);
  pad->SetLeftMargin(0.15);
  pad->SetRightMargin(0.05);
  pad->Modified();
  pad->Update();
}


void SavePlot(const char* name, const char* dir) {
  string eps = dir;
  eps += "/";
  eps += name;
  eps += ".eps";
  gPad->SaveAs( eps.c_str() );

  string png = dir;
  png += "/";
  png += name;
  png += ".png";
  gPad->SaveAs( png.c_str() );

}

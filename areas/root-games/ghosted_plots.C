#include "TCanvas.h"
#include "TH2.h"
#include "THStack.h"
#include "TRandom.h"


// before running this, make sure that you've typed
//   .L jet-plots.C+
// then run this with 
//   .x ghosted_plots.C

// aborted attempt...

void ghosted_plots () {

  // display the various 2-d drawing options
  gROOT->Reset();
  gStyle->SetOptStat(0);
  gStyle->SetPalette(1);
  gStyle->SetCanvasColor(1);
  gStyle->SetFrameFillColor(0);

  // set up canvas
  TCanvas * lego = new TCanvas("lego","lego options",400,50,800,600);
  Int_t cancolor = 0;
  lego->SetFillColor(cancolor);
  lego->SetTheta(40.549);
  lego->SetPhi(110.101);

  ////vector<double> col 
  int ngrey = 3;
  for (int ir = 0; ir < ngrey; ir++) {
    for (int ig = 0; ig < ngrey; ig++) {
      for (int ib = 0; ib < ngrey; ib++) {
        int icol = 7+ir + ngrey *ig + ngrey*ngrey * ib;
        color=(TColor*)(gROOT->GetListOfColors()->At(icol));
        color->SetRGB(1-ir*1.0/ngrey,1-ig*1.0/ngrey,1-ib*1.0/ngrey);
      }
    }
  }

  TPaveLabel pl;
  Float_t x1=0.63, y1=0.875, x2=0.95, y2=0.925;
   

  // create the jet histograms
  //lego->Divide(2,2);
  //lego->cd(1);
  gPad->SetTheta(40.549);
  gPad->SetPhi(110.101);
//  JetHist * jets = new JetHist("50GeV.res");
  JetHist * jets = new JetHist("ghosted-siscone.dat");
  //JetHist * jets = new JetHist("ghosted-siscone-pair1.0.dat");
  //JetHist * jets = new JetHist("ghosted-kt-pair0.9.dat");
  //JetHist * jets = new JetHist("ghosted-kt-ev0.dat");
  //JetHist * jets = new JetHist("ghosted-siscone-f0.60.dat");
  jets->stack.Draw("lego1");
  pl.DrawPaveLabel(x1,y1,x2,y2,"SISCone, R=1, f=0.60","brNDC");

  //JetHist * jets = new JetHist("ghosted-kt-ev1.dat");
  //JetHist * jets = new JetHist("ghosted-not.dat");
  //jets->stack.Draw("lego1");
  //pl.DrawPaveLabel(x1,y1,x2,y2,"kt, R=1","brNDC");


  //JetHist * jets = new JetHist("50GeV+9minbias.res");

  return;
  ///
  lego->Update();
}


#include "TCanvas.h"
#include "TH2.h"
#include "THStack.h"
#include "TRandom.h"
#include<vector>

void surfaces () {
   // display the various 2-d drawing options
   gROOT->Reset();
   gStyle->SetOptStat(0);
   gStyle->SetPalette(1);
   gStyle->SetCanvasColor(33);
   gStyle->SetFrameFillColor(18);
   
   int nbins = 30;

   //TF2 *f2 = new TF2("f2","xygaus + xygaus(5) + xylandau(10)",-4,4,-4,4);
   //Double_t params[] = {130,-1.4,1.8,1.5,1, 150,2,0.5,-2,0.5, 3600,-2,0.7,-3,0.3};
   //f2->SetParameters(params);
   TH2F * h2 = new TH2F("h2","xygaus + xygaus(5) + xylandau(10)",nbins,-4,4,nbins,-4,4);
   h2->SetFillColor(kRed);

   // do a second one
   TH2F * h3 = new TH2F("h2","(xygaus + xygaus(5) + xylandau(10))",nbins,-4,4,nbins,-4,4);
   h3->SetFillColor(kBlue);

   // do a second one
   TH2F * h4 = new TH2F("h2","xygaus + xygaus(5) + xylandau(10)",nbins,-4,4,nbins,-4,4);
   h4->SetFillColor(kYellow);

   // do another one
   TH2F * h5 = new TH2F("h2","(xygaus + xygaus(5) + xylandau(10))",nbins,-4,4,nbins,-4,4);
   h5->SetFillColor(kGreen);

   //h2->FillRandom("f2",40000);
   //h3->FillRandom("f2",10000);
   //h4->Fill(1,2,-1);
   //h5->FillRandom("f2",10000);

   h2->Fill(1,2,10);
   h3->Fill(3,1,10);
   h3->Fill(2,1,10);
   h5->Fill(2,0,10);
   h5->Fill(1,2,10);

   TPaveLabel pl;
   
   //basic 2-d options
   Float_t x1=0.67, y1=0.875, x2=0.85, y2=0.95;
   Int_t cancolor = 17;
//   TCanvas c2h("c2h","2-d options",10,10,800,600);
//   c2h.Divide(2,2);
//   c2h.SetFillColor(cancolor);
//   c2h.cd(1);
//   h2->Draw();       pl.DrawPaveLabel(x1,y1,x2,y2,"SCAT","brNDC");
//   c2h.cd(2);
//   h2->Draw("box");  pl.DrawPaveLabel(x1,y1,x2,y2,"BOX","brNDC");
//   c2h.cd(3);
//   h2->Draw("arr");  pl.DrawPaveLabel(x1,y1,x2,y2,"ARR","brNDC");
//   c2h.cd(4);
//   h2->Draw("colz"); pl.DrawPaveLabel(x1,y1,x2,y2,"COLZ","brNDC");
//   c2h.Update();
//   // see the canvas 
//
//   
//   //text option
//   TCanvas ctext("ctext","text option",50,50,800,600);
//   gPad->SetGrid();
//   ctext.SetFillColor(cancolor);
//   ctext->SetGrid();
//   h2->Draw("text"); pl.DrawPaveLabel(x1,y1,x2,y2,"TEXT","brNDC");
//   ctext.Update();
//   // see the canvas 
//
//
//   
//   //contour options
//   TCanvas cont("contours","contours",100,100,800,600);
//   cont.Divide(2,2);
//   gPad->SetGrid();
//   cont.SetFillColor(cancolor);
//   cont.cd(1);
//   h2->Draw("contz"); pl.DrawPaveLabel(x1,y1,x2,y2,"CONTZ","brNDC");
//   cont.cd(2);
//   gPad->SetGrid();
//   h2->Draw("cont1"); pl.DrawPaveLabel(x1,y1,x2,y2,"CONT1","brNDC");
//   cont.cd(3);
//   gPad->SetGrid();
//   h2->Draw("cont2"); pl.DrawPaveLabel(x1,y1,x2,y2,"CONT2","brNDC");
//   cont.cd(4);
//   gPad->SetGrid();
//   h2->Draw("cont3"); pl.DrawPaveLabel(x1,y1,x2,y2,"CONT3","brNDC");
//   cont.Update();
//   // see the canvas 
//   
   //lego options
   TCanvas * lego = new TCanvas("lego","lego options",480,50,800,600);
   //lego->Divide(2,2);
   lego->SetFillColor(cancolor);
   //lego->cd(1);
   //
   //
   //h2->Draw("lego1");     pl.DrawPaveLabel(x1,y1,x2,y2,"LEGO","brNDC");
   //lego->cd(2);

   // try a stack...
   THStack * hs = new THStack("hs","test of stack");
   hs->Add(h4);
   hs->Add(h3);
   hs->Add(h2);
   hs->Add(h5);
   hs->Draw("lego1");

   //lego->cd(3);
   //hs->Draw("lego1 cyl");

   lego->Update();

   std::vector<int> f(5);

   //for (Int_t i=1;i<100;i+=5) {
   //   h->SetFillColor((i+4)/5);
   //   h->GetXaxis()->SetRange(i,i+4);
   //   h->DrawCopy("same");
   //}
   //h3->GetXaxis()->SetRange(-1,1);
   //
   //
   //h2->DrawCopy("lego1");    pl.DrawPaveLabel(x1,y1,x2,y2,"LEGO1","brNDC");
   //h3->DrawCopy("same lego1");    pl.DrawPaveLabel(x1,y1,x2,y2,"LEGO1","brNDC");



   //lego.cd(3);
   //gPad->SetTheta(61); gPad->SetPhi(-82);
   //h2->Draw("lego2 cyl"); pl.DrawPaveLabel(x1,y1,x2+0.05,y2,"SURFCYL","brNDC");
   //lego.cd(4);
   //gPad->SetTheta(21); gPad->SetPhi(-90);
   //h2->Draw("legocyl"); pl.DrawPaveLabel(x1,y1,x2+0.05,y2,"SURF1CYL","brNDC");
   //lego.Update();
   // see the canvas 

   
//   //surface options
//   TCanvas surf("surfopt","surface options",200,200,800,600);
//   surf.Divide(2,2);
//   surf.SetFillColor(cancolor);
//   surf.cd(1);
//   h2->Draw("surf1");   pl.DrawPaveLabel(x1,y1,x2,y2,"SURF1","brNDC");
//   surf.cd(2);
//   h2->Draw("surf2z");  pl.DrawPaveLabel(x1,y1,x2,y2,"SURF2Z","brNDC");
//   surf.cd(3);
//   h2->Draw("surf3");   pl.DrawPaveLabel(x1,y1,x2,y2,"SURF3","brNDC");
//   surf.cd(4);
//   h2->Draw("surf4");   pl.DrawPaveLabel(x1,y1,x2,y2,"SURF4","brNDC");
//   surf.Update();
//   // see the canvas 
//
}

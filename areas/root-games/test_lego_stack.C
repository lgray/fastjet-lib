// example program associated with bug report regarding colour
// of tops of stacks
void test_lego_stack () {
  // display the various 2-d drawing options
  gROOT->Reset();

  // set up canvas
  TCanvas * lego = new TCanvas("lego","lego options",400,50,800,600);

  int nbins = 12;
  TH2F * h1 = new TH2F("h1","h1",nbins,0,6,nbins,-4,4);
  TH2F * h2 = new TH2F("h2","h1",nbins,0,6,nbins,-4,4);
  TH2F * h3 = new TH2F("h3","h1",nbins,0,6,nbins,-4,4);
  TH2F * h4 = new TH2F("h4","h1",nbins,0,6,nbins,-4,4);
  TH2F * h5 = new TH2F("h5","h1",nbins,0,6,nbins,-4,4);
  h1->SetFillColor(1); // grey
  h2->SetFillColor(2); // red 
  h3->SetFillColor(3); // green
  h4->SetFillColor(4); // blue
  h5->SetFillColor(5); // yellow

  // with just the following lines, in the stack, the top of the
  // non-zero bins of h3 and h4 are painted in grey (lowest stack
  // element), whereas they should have been painted in their own colors
  // 
  h2->Fill(2,0,10);
  h3->Fill(3,0,10); 
  h4->Fill(4,0,10);
  h5->Fill(5,0,10);

  // By uncommenting the following lines, then in the non-zero bins of
  // h3 (h4) one also gives an infinitesimal filling to h2 (h2 and
  // h3). This causes the tops of h3 and h4 to be filled in correctly
  //----------------------------------------
  //h2->Fill(3,0,0.01);
  //h2->Fill(4,0,0.01); h3->Fill(4,0,0.01);
  //-------------------------------------------

  //lego options
  THStack * hs = new THStack("hs","test of stack");
  hs->Add(h1);
  hs->Add(h2); 
  hs->Add(h3);
  hs->Add(h4);
  hs->Add(h5);

  hs->Draw("lego1");
  
  lego->Update();

}


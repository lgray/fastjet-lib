
// before ".x"ing this file remember to load (.L) file2hist.C
{
  gROOT->Reset();
  TCanvas * c1 = new TCanvas("c1");
  c1->Divide(2,2);

  c1->cd(1);
  TH1D * h1 = new TH1D("h1","just ghosts",80,0.0,8.0);
  file2hist(h1, "../b");
  h1->SetFillColor(16);
  h1->Draw("");

  c1->cd(2);
  TH1D * h2 = new TH1D("h2","one hard particle in jet",80,0.0,8.0);
  file2hist(h2, "../d");
  h2->SetFillColor(18);
  h2->Draw("");

  c1->cd(3);
  TH1D * h3 = new TH1D("h3","Two hard particles in jet",80,0.0,8.0);
  file2hist(h3, "../e");
  h3->SetFillColor(38);
  h3->Draw("");


}

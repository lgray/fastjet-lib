#include<vector>
#include<fstream>
#include<iostream>
#include<sstream>
#include<cstdlib>

#include "TCanvas.h"
#include "TH2.h"
#include "THStack.h"


using namespace std;


class JetHist {
private:
  vector<TH2F *> _jets;
  TH2F * _background;
public:
  JetHist(const string & filename);
  ~JetHist();
  THStack stack;
  TH2F * jet(int i) {return i>= 0 ? _jets[i] : _background;}
};

// get jet "histograms" from filename which is expected to be made of repeated
// blocks as follows:
//       jet# eta phi pt ...
//        ipart eta phi pt
//        ipart eta phi pt
//        ...
//       #END
JetHist::JetHist (const string & filename) {
  ifstream file(filename.c_str());
  string line;
  double etamax=6;
  double phimax = 2*3.14159265;
  int    nbins=30;

  // construct a histogram for the background to the jets
  ostringstream bname;
  bname << filename <<"-background";
  _background = new TH2F(bname.str().c_str(),bname.str().c_str(),
			 nbins,0.0,phimax,2*nbins,-etamax,etamax);
  _background->SetFillColor(kWhite);
  // these were supposed to have labelled the axes, but it doesn't work.
  _background->GetXaxis()->SetTitle("#phi");
  _background->GetYaxis()->SetTitle("#eta");
  _background->GetZaxis()->SetTitle("p_{#perp}");
  stack.Add(_background);

  while (getline(file,line)) {
    if (line.substr(0,1) != " ") {continue;} // all interesting lines start with space?
    ostringstream name;
    name << filename<<"-jet-"<< _jets.size();
    TH2F * hist = new TH2F(name.str().c_str(),name.str().c_str(),
			   nbins,0.0,phimax,2*nbins,-etamax,etamax);
    int    i;
    double eta, phi, pt;
    cout << filename <<": jet "<<_jets.size()<<endl;
    bool have_line = true;
    while (have_line || getline(file,line)) {
      have_line = false;
      if (line.substr(0,4) == "#END") {break;}
      istringstream sline(line);
      sline >> i >> eta >> phi >> pt;
      cout << i << " "<<eta<<" "<<phi<<" "<<pt<<endl;
      hist->Fill(phi,eta,pt); // fill at phi,eta with weight pt
      
      // workaround for bug in stacks: fill all lower elements of the stack
      // with a fake amount -- this, miraculously will lead to correct coloring
      // of the top of the stack!
      //for (unsigned int j = 0; j < _jets.size(); j++) {
      //  _jets[j]->Fill(phi,eta,1e-7); 
      //}
    }
    // give it a colour (whatever that means...)
    //hist->SetFillColor(_jets.size());
    int njet = _jets.size();
    //hist->SetFillColor(njet+2);
    hist->SetFillColor(njet%50+2); // %50 seems tomake to diff to many-jet case
    //if (njet == 0) hist->SetFillColor(kRed);
    //else if (njet == 1) hist->SetFillColor(kBlue);
    //else  hist->SetFillColor(kGreen);

    // add it to the list of jets (so we can delete it later...)
    _jets.push_back(hist);

    // put it onto the stack
    stack.Add(hist);
    //if (njet == 2) break;
    //break;
  }
}

// clean up --------------
JetHist::~JetHist () {
  for (unsigned int i = 0; i < _jets.size(); i++) {
    delete _jets[i];
  }
  delete _background;
}



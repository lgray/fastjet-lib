#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceActiveArea.hh"
#include "ClusterSequencePassiveArea.hh"
#include<iostream>
#include<sstream>
#include<fstream>
#include<valarray>
#include<vector>
#include <cstdlib>
#include<cstddef> // for size_t
#include "CmdLine.hh"
#include<cmath>
#include "SimpleHist.hh"

namespace fj = fastjet;
using namespace std;

inline double pow2(const double x) {return x*x;};

//----------------------------------------------------------------------
/// A program that allows one to determine the area of a simple 2-parton
/// system as a function of the separation of the partons
int main (int argc, char ** argv) {

  CmdLine cmdline(argc,argv);
  fj::Strategy  strategy  = fj::Strategy(cmdline.int_val("-strategy",
				     cmdline.int_val("-clever", fj::Best)));
  double ktR   = cmdline.double_val("-r",1.0);
  double ghost_area = cmdline.double_val("-ghost_area",cmdline.double_val("-cell_area",0.01));
  double ghost_etamax = cmdline.double_val("-ghost_etamax",6.0);
  double grid_scatter = cmdline.double_val("-grid_scatter",0.0001);
  double kt_scatter   = cmdline.double_val("-kt_scatter",0.1);
  int    n            = cmdline.int_val("-n",20);
  int    repeat = 1;
  double anchor_pt = cmdline.present("-anchor") ? 100.0 : 0.0;

  fj::JetFinder jet_finder = cmdline.present("-cam") ? 
                                fj::cambridge_algorithm : fj::kt_algorithm;

  // create the definitions for our jet finder and areas spec...
  fj::JetDefinition jet_def(jet_finder, ktR, strategy);
  fj::ActiveAreaSpec active_area_spec(ghost_etamax, repeat, 
				      ghost_area, grid_scatter, kt_scatter);

  // the histogram...
  SimpleHist areahist(-0.000001,9.0/fj::pi,150);

  cout << "# " << cmdline.command_line() << endl;
  cout << "# strategy     = " << jet_def.strategy()<<endl;
  cout << "# anchor_pt    = " << anchor_pt    << endl;
  cout << "# ktR          = " << ktR          << endl;
  cout << "# ghost_etamax = " << ghost_etamax << endl;
  cout << "# ghost_area   = " << ghost_area   << endl;
  cout << "# nev          = " << n            << endl;
  cout << "# jet finder   = " << jet_finder   << endl;

  int njets = 0;
  double average_area = 0.0, average_ar2 = 0.0;
  for (int i = 0; i<n; i++) {
    vector<fj::PseudoJet> input_jets(0);
    input_jets.push_back(fj::PseudoJet(anchor_pt,0.0,0.0,anchor_pt));
    fj::ClusterSequenceActiveAreaExplicitGhosts clust(input_jets, jet_def, 
						      active_area_spec);

    vector<fj::PseudoJet> output_jets(clust.inclusive_jets());
    for (unsigned j = 0; j < output_jets.size(); j++) {
      // only take jets that are reasonably close to center
      if (abs(output_jets[j].rap()) < ghost_etamax - ktR &&
	  output_jets[j].perp2() > pow2(anchor_pt)*0.999999) {
	double normarea = clust.area(output_jets[j])/fj::pi*pow2(ktR);
	average_area += normarea; 
	average_ar2  += pow2(normarea);
	areahist.add_entry(normarea);
	njets++;
      }
    }
  }

  average_area /= njets;
  average_ar2  /= njets;
  average_ar2 = sqrt((average_ar2-pow2(average_area))/njets);
  cout << "# average area = " << average_area << " +- " << average_ar2 << endl;
  double rescale = 1.0 / (areahist.binsize() * njets);
  for (unsigned i = 0; i < areahist.size(); i++) {
    cout << areahist.binmid(i) << " " << areahist[i]*rescale 
	 << " " << sqrt(areahist[i])*rescale << endl;
  }
}

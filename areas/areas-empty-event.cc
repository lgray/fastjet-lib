#include "FjPseudoJet.hh"
#include "FjClusterSequence.hh"
#include "FjClusterSequenceWithArea.hh"
#include<iostream>
#include<sstream>
#include<valarray>
#include<vector>
#include <cstdlib>
#include<cstddef> // for size_t
#include "CmdLine.hh"

using namespace std;

inline double pow2(const double x) {return x*x;};
//----------------------------------------------------------------------
/// a program to test and time the kt algorithm as implemented in fastjet
int main (int argc, char ** argv) {

  CmdLine cmdline(argc,argv);
  int  repeat  = cmdline.int_val("-repeat",1);
  double ktR   = cmdline.double_val("-r",1.0);
  double cell_area = cmdline.double_val("-cell_area",0.01);
  double ghost_etamax = cmdline.double_val("-ghost_etamax",6.0);
  double grid_scatter = cmdline.double_val("-grid_scatter",0.00001);
  double kt_scatter   = cmdline.double_val("-kt_scatter",0.1);

  double average_area = 0.0, average_area2 = 0.0;
  int    njets = 0;

  for (int irep = 0; irep < repeat; irep++) {
    vector<FjPseudoJet> empty_input;
    FjClusterSequenceWithArea clust_seq(empty_input,cell_area,ghost_etamax,
					grid_scatter, kt_scatter,
                                        ktR);
    
    vector<FjPseudoJet> jets = clust_seq.inclusive_jets();
    for (size_t j = 0; j < jets.size(); j++) {
      //if (abs(jets[j].rap()) < ghost_etamax) {
      if (abs(jets[j].rap()) < ghost_etamax-2*ktR) {
	double area = clust_seq.area(jets[j]);
	average_area  += area;
	average_area2 += area*area;
	njets++;
      }
    }
  }

  average_area  /= njets;
  average_area2 /= njets;
  average_area2 = sqrt((average_area2 - average_area*average_area)/njets);
  cout << "njets = " << njets << " from "<<repeat<<" runs\n";
  cout << "average area " << average_area << " +- " << average_area2 << " (std-dev: " <<average_area2*sqrt(njets)<<")\n";

}

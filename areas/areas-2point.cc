#include "FjPseudoJet.hh"
#include "FjClusterSequence.hh"
#include "FjClusterSequenceWithMeanArea.hh"
#include<iostream>
#include<sstream>
#include<fstream>
#include<valarray>
#include<vector>
#include <cstdlib>
#include<cstddef> // for size_t
#include "CmdLine.hh"
#include<cmath>

using namespace std;

//----------------------------------------------------------------------
/// A program that allows one to determine the area of a simple 2-parton
/// system as a function of the separation of the partons
int main (int argc, char ** argv) {

  CmdLine cmdline(argc,argv);
  FjStrategy  strategy  = FjStrategy(cmdline.int_val("-strategy",
				     cmdline.int_val("-clever", Best)));
  int  repeat  = cmdline.int_val("-repeat",1);
  double ktR   = cmdline.double_val("-r",1.0);
  double cell_area = cmdline.double_val("-cell_area",0.01);
  double ghost_etamax = cmdline.double_val("-ghost_etamax",6.0);
  double grid_scatter = cmdline.double_val("-grid_scatter",0.00001);
  double kt_scatter   = cmdline.double_val("-kt_scatter",0.1);
  int    n            = cmdline.int_val("-n",20);
  double dr_max       = cmdline.double_val("-r",2.1);

  if (cmdline.present("-cam")) {FjClusterSequence::set_jet_finder(cambridge_algorithm);}

  if (!cmdline.all_options_used()) {cerr << 
      "Error: some options unsupported"<<endl; 
    exit(-1);}

  // document what is happening...
  cout << "# " << cmdline.command_line() << endl;
  cout << "# col1 = dr; cols2-5=jet0:{eta,area,area_err,area_stddev}"<<endl;
  cout << "#            cols6-9=jet1:{eta,area,area_err,area_stddev}"<<endl;
  
  double pt0 = 10000.0, pt1 = 1.0;
  for (int i = 0; i<=n; i++) {
    double dr = (i * dr_max) / n;
    double pz = sinh(dr*0.5);
    double E  = cosh(dr*0.5);
    // constrct jets at eta = +- dr
    vector<FjPseudoJet> input_jets(min(i+1,2));
    input_jets[0] = FjPseudoJet(0.0,pt0,+pt0*pz,pt0*E);
    if (i >= 1) input_jets[1] = FjPseudoJet(0.0,pt1,-pt1*pz,pt1*E);

    FjClusterSequenceWithMeanArea clust(input_jets,
					cell_area,ghost_etamax,
					grid_scatter, kt_scatter, repeat,
					ktR,strategy);

    vector<FjPseudoJet> output_jets = sorted_by_pt(clust.inclusive_jets());
    printf ("%7.3f %7.3f %7.3f %7.3f %7.3f", dr, output_jets[0].rap(), 
	    clust.area(output_jets[0]), clust.area_err(output_jets[0]),
	    clust.area_err(output_jets[0]) * sqrt(1.0*repeat));
    if (output_jets.size() > 1) {
      printf (" %7.3f %7.3f %7.3f %7.3f", output_jets[1].rap(), 
	    clust.area(output_jets[1]), clust.area_err(output_jets[1]),
	    clust.area_err(output_jets[1]) * sqrt(1.0*repeat));
    } else {printf(" 0 0 0 0");}
    printf ("\n");
  }

}

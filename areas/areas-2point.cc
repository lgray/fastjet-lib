#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceActiveArea.hh"
//#include "ClusterSequencePassiveArea.hh"
#include<iostream>
#include<sstream>
#include<fstream>
#include<valarray>
#include<vector>
#include <cstdlib>
#include<cstddef> // for size_t
#include "CmdLine.hh"
#include<cmath>

// get the plugins
#include "SISConePlugin.hh"
#include "CDFMidPointPlugin.hh"

namespace fj = fastjet;
using namespace std;

//----------------------------------------------------------------------
/// A program that allows one to determine the area of a simple 2-parton
/// system as a function of the separation of the partons
int main (int argc, char ** argv) {

  CmdLine cmdline(argc,argv);
  fj::Strategy  strategy  = fj::Strategy(cmdline.int_val("-strategy",
				     cmdline.int_val("-clever", fj::Best)));
  int  repeat  = cmdline.int_val("-repeat",1);
  double ktR   = cmdline.double_val("-r",1.0);
  double ghost_area = cmdline.double_val("-ghost_area",cmdline.double_val("-cell_area",0.01));
  double ghost_etamax = cmdline.double_val("-ghost_etamax",6.0);
  double grid_scatter = cmdline.double_val("-grid_scatter",1.);
  double kt_scatter   = cmdline.double_val("-kt_scatter",0.1);
  int    n            = cmdline.int_val("-n",20);
  double dr_max       = cmdline.double_val("-dr_max",2.1);
  double dr_min       = cmdline.double_val("-dr_min",0.0);


  fj::RecombinationScheme  rec_scheme  = cmdline.present("-pt_scheme") ? fj::BIpt_scheme :
              							       fj::E_scheme;

  // create the definitions for our jet finder...
  fj::JetDefinition jet_def;
  if (cmdline.present("-cam")) {
    jet_def = fj::JetDefinition(fj::cambridge_algorithm, ktR, rec_scheme, strategy);}
  else if (cmdline.present("-kt")) {
    jet_def = fj::JetDefinition(fj::kt_algorithm, ktR, rec_scheme, strategy);}
  else if (cmdline.present("-midpoint")) {
    double overlap = cmdline.value("-f",0.5);
    double seed    = cmdline.value("-seed",0.0);
    jet_def = fj::JetDefinition(new fj::CDFMidPointPlugin(ktR,overlap,seed));}
  else if (cmdline.present("-siscone")) {
    double overlap = cmdline.double_val("-f",0.5);
    int    npass   = cmdline.int_val("-npass",1);
    fj::SISConePlugin * plugin = new fj::SISConePlugin(ktR,overlap,npass);
    if (cmdline.present("-smstop"))plugin->set_split_merge_stopping_scale(1e-50);
    jet_def = fj::JetDefinition(plugin);}
  else {
    cerr << "Must specify one of -kt | -cam | -siscone" << endl;
    exit(-1);
  }


  if (!cmdline.all_options_used()) {cerr << 
      "Error: some options unsupported"<<endl; 
    exit(-1);}

  // create the definitions for our areas spec...
  fj::ActiveAreaSpec active_area_spec(ghost_etamax, repeat, ghost_area, 
                                      grid_scatter, kt_scatter);

  cerr << "strategy is "<<jet_def.strategy()<<endl;

  // document what is happening...
  cout << "# " << cmdline.command_line() << endl;
  cout << "# col1 = dr; cols2-5=jet0:{eta,area,area_err,area_stddev}"<<endl;
  cout << "#            cols6-9=jet1:{eta,area,area_err,area_stddev}"<<endl;
  
  double pt0 = 10000.0, pt1 = 1.0;
  for (int i = 0; i<=n; i++) {
    double dr;
    if ( n == 1 ) { dr = dr_min; i++; } 
    else { dr = dr_min + (i * (dr_max-dr_min)) / n; }
    double pz = sinh(dr*0.5);
    double E  = cosh(dr*0.5);
    // constrct jets at eta = +- dr
    vector<fj::PseudoJet> input_jets(min(i+1,2));
    input_jets[0] = fj::PseudoJet(0.0,pt0,+pt0*pz,pt0*E);
    if (i >= 1) input_jets[1] = fj::PseudoJet(0.0,pt1,-pt1*pz,pt1*E);

    fj::ClusterSequenceActiveArea clust(input_jets, jet_def,
        				active_area_spec);
    //fj::ClusterSequencePassiveArea clust(input_jets, jet_def,
    //    				   1.0);


    vector<fj::PseudoJet> output_jets = sorted_by_pt(clust.inclusive_jets());
    printf ("%7.3f %7.3f %7.3f %7.3f %7.3f", dr, output_jets[0].rap(), 
	    clust.area(output_jets[0]), clust.area_error(output_jets[0]),
	    clust.area_error(output_jets[0]) * sqrt(1.0*repeat));
    if (output_jets.size() > 1) {
      printf (" %7.3f %7.3f %7.3f %7.3f", output_jets[1].rap(), 
	    clust.area(output_jets[1]), clust.area_error(output_jets[1]),
	    clust.area_error(output_jets[1]) * sqrt(1.0*repeat));
    } else {printf(" 0 0 0 0");}
    printf ("\n");
  }

}

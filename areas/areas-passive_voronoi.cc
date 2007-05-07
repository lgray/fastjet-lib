#include "fastjet/internal/BasicRandom.hh"
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/AreaDefinition.hh"
//#include "fastjet/ClusterSequenceAreaBase.hh"
#include "fastjet/ClusterSequenceWithArea.hh"
//#include "fastjet/ClusterSequenceVoronoiArea.hh"
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

namespace fj = fastjet;
using namespace std;

//----------------------------------------------------------------------
/// A program that allows one to compare the results of the PassiveArea
/// and VoronoiArea classes. For a given number of particles, we generate
/// random particles and test jet areas at various effective_Rfact
int main (int argc, char ** argv) {

  CmdLine cmdline(argc,argv);

  // parse command line
  fj::Strategy  strategy  = fj::Strategy(cmdline.int_val("-strategy",
							 cmdline.int_val("-clever", fj::Best)));
  double ktR   = cmdline.double_val("-r",1.0);
  int    nr    = cmdline.int_val("-nr",14);
  double r_max = cmdline.double_val("-dr_max",1.5);
  double r_min = cmdline.double_val("-dr_min",0.1);
  
  int    repeat= cmdline.int_val("-repeat", 1);
  int    N     = cmdline.int_val("-n",10);
  string event = cmdline.string_val("-f", "");

  bool verbose = cmdline.present("-v");

  if (!cmdline.all_options_used()) {cerr << 
      "Error: some options unsupported"<<endl; 
    exit(-1);}

  // create the definitions for our jet finder and areas spec...
  fj::JetDefinition jet_def(fj::kt_algorithm, ktR, strategy);
  vector<fj::PseudoJet> input_jets;

  // document what is happening...
  cout << "# " << cmdline.command_line() << endl;
  cout << "# col1 = dr; col2-3 = jet; col4-5 = area_{pass,voronoi}; col6 = diff"<<endl;
  
  // read particles
  if (event.empty()){
    fj::BasicRandom<double> rnd_gen;
    double rap,phi,kt;
    for (int j=0;j<N;j++){
      phi = 2.0*M_PI*rnd_gen();
      rap = -5.0+10.0*rnd_gen();
      kt  = exp(-6+12*rnd_gen());
      input_jets.push_back(fj::PseudoJet(kt*cos(phi),kt*sin(phi),kt*sinh(rap),kt*cosh(rap)));
    }
  } else {
    FILE *flux=fopen(event.c_str(), "r");
    char fline[512];
    double px,py,pz,E;
    while (fgets(fline, 512, flux)!=NULL){
      if (fline[0]!='#'){ // skip lines beginning with '#'
	if (sscanf(fline, "%le%le%le%le", &px, &py, &pz, &E)==4){
	  input_jets.push_back(fj::PseudoJet(px, py, pz, E));
	} else {
	  cout << "error in reading event file Giving up." << endl;
	  fclose(flux);
	  exit(2);
	}
      }
    }
    fclose(flux);
  }

  for (int i = 0; i<=nr; i++) {
    // effective radius
    double r  = r_min + (i * (r_max-r_min)) / nr;

    // clustering with area computation
    // first declare area definitions
    fj::VoronoiAreaSpec voronoi_spec(r);
    fj::AreaDefinition area_def_voronoi(voronoi_spec);

    // then compute
    fj::ClusterSequencePassiveArea clust_passive(input_jets, jet_def, r);
    fj::ClusterSequenceWithArea clust_voronoi(input_jets, jet_def, area_def_voronoi);

    // output jets with area computation
    vector<fj::PseudoJet> output_jets_passive = sorted_by_pt(clust_passive.inclusive_jets());
    vector<fj::PseudoJet> output_jets_voronoi = sorted_by_pt(clust_voronoi.inclusive_jets());

    double ap, av, da;
    for (int j=0;j<output_jets_voronoi.size();j++){
      //ap = clust_passive.area(output_jets_passive[j]);
      ap = av = clust_voronoi.area(output_jets_voronoi[j]);
      da = ap-av;
      if ((verbose) || (fabs(da)>1e-10*ap)){
	printf ("%7.3f\t%7.3f\t%7.3f\t%lf\t%lf\t%le\n", r, 
		output_jets_voronoi[j].rap(),output_jets_voronoi[j].phi(), 
		ap, av, da);
      }
    }
  }
  

  return 0;
}

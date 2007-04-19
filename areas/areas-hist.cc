#include<iostream>
#include<iomanip>
#include<sstream>
#include<fstream>
#include<valarray>
#include<vector>
#include <cstdlib>
#include<cstddef> // for size_t
#include<cmath>

// fastjet stuff
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceActiveArea.hh"
#include "ClusterSequencePassiveArea.hh"

// get the plugins
#include "SISConePlugin.hh"
#include "CDFMidPointPlugin.hh"

// local things
#include "CmdLine.hh"
#include "SimpleHist.hh"

namespace fj = fastjet;
using namespace std;

inline double pow2(const double x) {return x*x;};

//----------------------------------------------------------------------
/// A program that allows one to determine the distribution of areas
/// in events populated either by ghosts only, or by ghosts and a single
/// hard particle
int main (int argc, char ** argv) {

  CmdLine cmdline(argc,argv);
  fj::Strategy  strategy  = fj::Strategy(cmdline.int_val("-strategy",
				     cmdline.int_val("-clever", fj::Best)));
  fj::RecombinationScheme  rec_scheme  = cmdline.present("-pt_scheme") ? fj::BIpt_scheme :
              		                fj::E_scheme;

  double ktR   = cmdline.double_val("-r",1.0);
  double ghost_area = cmdline.double_val("-ghost_area",cmdline.double_val("-cell_area",0.01));
  double ghost_etamax = cmdline.double_val("-ghost_etamax",6.0);
  double grid_scatter = cmdline.double_val("-grid_scatter",0.0001);
  double kt_scatter   = cmdline.double_val("-kt_scatter",0.1);
  int    n            = cmdline.int_val("-n",20);
  int repeat = cmdline.value("-repeat", 1);
  double precision_limit = cmdline.value("-prec",0.);
  
  double anchor_pt = cmdline.present("-anchor") ? 100.0 : 0.0;
  int    writefreq    = int(cmdline.double_val("-freq",1.0*max(n/10,1000)));

  int nhist      = cmdline.value("-nhist",150);
  double histmax = cmdline.value("-histmax",9.0);

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
    double overlap = cmdline.value("-f",0.5);
    int    npass   = cmdline.value("-npass",1);
    jet_def = fj::JetDefinition(new fj::SISConePlugin(ktR,overlap,npass));}
  else {
    cerr << "Must specify one of -kt | -cam | -siscone" << endl;
    exit(-1);
  }

  string outfile;
  if (cmdline.present("-out")) {
     outfile = cmdline.value<string>("-out");
  } else { 
     outfile = "";
  }
  
  if (!cmdline.all_options_used()) {
    cerr << "ERROR: exiting become some options unrecognized" << endl;
    exit(-1);
  }

  // create the definitions for our jet finder and areas spec...
  fj::ActiveAreaSpec active_area_spec(ghost_etamax, repeat, 
				      ghost_area, grid_scatter, kt_scatter);

  // the histogram...
  SimpleHist areahist(-0.000001,histmax/fj::pi,nhist);


  int njets = 0;
  double average_area = 0.0, average_ar2 = 0.0;
  double relative_error = 1e100;
  for (int i = 0; i<n; i++) {
    vector<fj::PseudoJet> input_jets(0);
    input_jets.push_back(fj::PseudoJet(anchor_pt,0.0,0.0,anchor_pt));
    fj::ClusterSequenceActiveAreaExplicitGhosts clust(input_jets, jet_def, 
						      active_area_spec);
    //cout << "Clustering " << clust.n_particles() << " particles" << endl;
    vector<fj::PseudoJet> output_jets(clust.inclusive_jets());
    for (unsigned j = 0; j < output_jets.size(); j++) {
      // only take jets that are reasonably close to center
      if (abs(output_jets[j].rap()) < ghost_etamax - ktR &&
          output_jets[j].perp2() > pow2(anchor_pt)*0.999999) {
      //if (true) {
	double normarea = clust.area(output_jets[j])/(fj::pi*pow2(ktR));
	average_area += normarea; 
	average_ar2  += pow2(normarea);
	areahist.add_entry(normarea);
	njets++;
        //(*ostr) << setw(12) << output_jets[j].rap() << " " 
        //     << setw(10) << normarea  << " "
        //     << setw(4)  << output_jets[j].user_index() << endl; 
      }
    }
    //vector<fj::PseudoJet> unclust = clust.unclustered_particles();
    //(*ostr) << "Unclustered particles: " << unclust.size() << endl;
    //for (unsigned j = 0; j < unclust.size(); j++) {
    //  (*ostr) << "UNCLUST: " << unclust[j].rap() << " " << unclust[j].phi() << " " << unclust[j].perp() << " " << unclust[j].cluster_hist_index() << endl;


    double av_ar2 = sqrt((average_ar2/njets-pow2(average_area/njets))/njets);
    relative_error = abs(av_ar2/(average_area/njets));
    if ( i+1==n || (i+1) % writefreq == 0 || i+1 == 10 || i+1 == 100 || 
         relative_error < precision_limit ) { 

       // (re-)initialise the output file
       ostream * ostr;
       if ( outfile != "" ) {
         ostr = new ofstream(outfile.c_str());
       } else {
         ostr = & cout;
       }
    
       (*ostr) << "# " << cmdline.command_line() << endl;
       (*ostr) << "# strategy     = " << jet_def.strategy()<<endl;
       (*ostr) << "# anchor_pt    = " << anchor_pt    << endl;
       (*ostr) << "# ktR          = " << ktR          << endl;
       (*ostr) << "# ghost_etamax = " << ghost_etamax << endl;
       (*ostr) << "# ghost_area   = " << ghost_area   << endl;
       (*ostr) << "# nev          = " << n            << endl;
       (*ostr) << "# precision    = " << precision_limit << endl;
       (*ostr) << "# nhist        = " << nhist        << endl;
       (*ostr) << "# histmax      = " << histmax      << endl;
       (*ostr) << "# jet def      = " << jet_def.description() << endl;
       (*ostr) << "# "                                << endl;
       (*ostr) << "# number of events = " << i+1 << endl;
   
       (*ostr) << "# average area = " << average_area/njets << " +- " << av_ar2 << endl;
       (*ostr) << "# <A^2>        = " << average_ar2/njets << endl;
       (*ostr) << "# sqrt(<A^2>-<A>^2)  = " << av_ar2*sqrt(float(njets)) << endl;
       double rescale = 1.0 / (areahist.binsize() * njets);
       for (unsigned i = 0; i < areahist.size(); i++) {
           (*ostr) << areahist.binmid(i) << " " << areahist[i]*rescale 
	   << " " << sqrt(areahist[i])*rescale << endl;
       }
       
       if ( outfile != "" ) { delete ostr; }
    }
    
    if ( relative_error < precision_limit ) {break;}
  } // end loop over events

}


//STARTHEADER
// $Id$
//
// Copyright (c) 2005-2006, Matteo Cacciari and Gavin Salam
//
//----------------------------------------------------------------------
// This file is part of FastJet.
//
//  FastJet is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  The algorithms that underlie FastJet have required considerable
//  development and are described in hep-ph/0512210. If you use
//  FastJet as part of work towards a scientific publication, please
//  include a citation to the FastJet paper.
//
//  FastJet is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with FastJet; if not, write to the Free Software
//  Foundation, Inc.:
//      59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//----------------------------------------------------------------------
//ENDHEADER


//----------------------------------------------------------------------
/// areas.cc: Program to help work out areas of jets
/// 
/// It reads files containing multiple events in the format 
/// p1x p1y p1z E1
/// p2x p2y p2z E2
/// ...
/// #END
/// 
/// An example input file containing 10 events is included as 
/// ../data/Pythia-PtMin1000-LHC-10ev.dat
///
/// Usage:
///   fastjet_timing [-strategy NUMBER] [-repeat nrepeats] [-massive] \
///                  [-combine nevents] [-r Rparameter] [-incl ptmin] [...] \
///                  < data_file
///
/// where the clustering can be repeated to aid timing and multiple
/// events can be combined to get to larger multiplicities. Some options:
///
///   -strategy N   indicate stratgey from the enum fj::Strategy (see
///                 fj::ClusterSequence.hh).
///
///   -combine nev  for combining multiple events from the data file in order
///                 to get to large multiplicities.
///
///   -incl ptmin   output of all inclusive jets with pt > ptmin is obtained
///                 with the -incl option.
///
///   -excld dcut   output of all exclusive jets as obtained in a clustering
///                 with dcut
///
///   -massless     read in only the 3-momenta and deduce energies assuming
///                 that particles are massless
///
///   -write        for writing out detailed clustering sequence (valuable
///                 for testing purposes)
///
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceActiveArea.hh"
#include "ClusterSequencePassiveArea.hh"
#include<iostream>
#include<sstream>
#include<valarray>
#include<vector>
#include <cstdlib>
#include<cstddef> // for size_t
#include "CmdLine.hh"

namespace fj = fastjet;
using namespace std;

inline double pow2(const double x) {return x*x;};

void print_jet(const fj::ClusterSequence & cs, const fj::PseudoJet & jet) {
  vector<fj::PseudoJet> cnst = cs.constituents(jet);
  for (size_t i = 0; i < cnst.size(); i++) {
    printf("%6i %18.5f %18.5f %18.6e\n",i,cnst[i].rap(),cnst[i].phi(),cnst[i].perp());
  }
  printf("#END\n");
};

//----------------------------------------------------------------------
/// a program to test and time the kt algorithm as implemented in fastjet
int main (int argc, char ** argv) {

  CmdLine cmdline(argc,argv);
  // allow the use to specify the fj::Strategy either through the
  // -clever or the -strategy options (both will take numerical
  // values); the latter will override the former.
  fj::Strategy  strategy  = fj::Strategy(cmdline.int_val("-strategy",
				     cmdline.int_val("-clever", fj::Best)));
  double ktR   = cmdline.double_val("-r",1.0);
  double effective_R_fact = cmdline.double_val("-rfact",1.0);
  fj::JetFinder jet_fndr= cmdline.present("-cam")? fj::cambridge_algorithm: fj::kt_algorithm;
  fj::JetDefinition jet_def(jet_fndr, ktR, strategy);

  // set up things to do with how we measure the area
  fj::ActiveAreaSpec area_spec;
  area_spec.set_repeat      (cmdline.int_val("-repeat",1)            );
  area_spec.set_ghost_area   (cmdline.double_val("-ghost_area",cmdline.double_val("-cell_area",0.01))   );
  area_spec.set_ghost_etamax(cmdline.double_val("-ghost_etamax",6.0) );
  area_spec.set_grid_scatter(cmdline.double_val("-grid_scatter",1e-4));
  area_spec.set_kt_scatter  (cmdline.double_val("-kt_scatter",0.1)   );

  //int  repeat  = cmdline.int_val("-repeat",1);
  //double ghost_area = cmdline.double_val("-ghost_area",cmdline.double_val("-cell_area",0.01));
  //double ghost_etamax = cmdline.double_val("-ghost_etamax",6.0);
  //double grid_scatter = cmdline.double_val("-grid_scatter",0.00001);
  //double kt_scatter   = cmdline.double_val("-kt_scatter",0.1);

  int  combine = cmdline.int_val("-combine",1);
  bool writeout   = cmdline.present("-write");
  bool hydjet  = cmdline.present("-hydjet");
  double inclkt = cmdline.double_val("-incl",-1.0);
  int    excln  = cmdline.int_val   ("-excln",-1);
  double excld  = cmdline.double_val("-excld",-1.0);
  double etamax = cmdline.double_val("-etamax",1.0e310);
  bool   massless = cmdline.present("-massless");
  int    nev     = cmdline.int_val("-nev",1);
  bool   print_jets = cmdline.present("-print_jets");

  if (!cmdline.all_options_used()) {cerr << 
      "Error: some options unsupported"<<endl; 
    exit(-1);}

  cout << "# "<< cmdline.command_line() << endl;

  for (int iev = 0; iev < nev; iev++) {
  vector<fj::PseudoJet> input_particles;
  string line;
  int  ndone = 0;
  while (getline(cin, line)) {
      //cout << line<<endl;
    istringstream linestream(line);
    if (line == "#END") {
      ndone += 1;
      if (ndone == combine) {break;}
    }
    if (line.substr(0,1) == "#") {continue;}
    valarray<double> fourvec(4);
    if (hydjet) {
      // special reading from hydjet.txt event record (though actually
      // this is supposed to be a standard pythia event record, so
      // being able to read from it is perhaps not so bad an idea...)
      int ii, istat,id,m1,m2,d1,d2;
      double mass;
      linestream >> ii>> istat >> id >> m1 >> m2 >> d1 >> d2
		 >> fourvec[0] >> fourvec[1] >> fourvec[2] >> mass;
      // current file contains mass of particle as 4th entry
      if (istat == 1) {
	fourvec[3] = sqrt(+pow2(fourvec[0])+pow2(fourvec[1])
			  +pow2(fourvec[2])+pow2(mass));
      } else {continue;}
    } else {
      if (massless) {
	linestream >> fourvec[0] >> fourvec[1] >> fourvec[2];
	fourvec[3] = sqrt(pow2(fourvec[0])+pow2(fourvec[1])+pow2(fourvec[2]));}
      else {
	linestream >> fourvec[0] >> fourvec[1] >> fourvec[2] >> fourvec[3];
      }
    }
    fj::PseudoJet psjet(fourvec);
    psjet.set_user_index(0);
    if (abs(psjet.rap() < etamax)) {input_particles.push_back(psjet);}
  }

  //srand(2); // moved inside loop
  //double average_area = 0.0;
  //double average_area2 = 0.0;
  valarray<double> average_area; 
  valarray<double> average_area2;

    
  //fj::ClusterSequenceActiveArea clust_seq(input_particles,
  //      				  ghost_area,ghost_etamax,
  //      				  grid_scatter, kt_scatter, repeat,
  //					  ktR,strategy,writeout);

  fj::ClusterSequenceActiveArea clust_seq(input_particles,jet_def,area_spec,writeout);
  //fj::ClusterSequencePassiveArea clust_seq(input_particles,jet_def,effective_R_fact,writeout);

  cerr << "strategy used =  "<< clust_seq.strategy_string()<< endl;
  //cerr << "number of particles = " << clust_seq.n_particles() << endl;

    
  vector<fj::PseudoJet> jets;
  
  // now provide some nice output...
  if (inclkt >= 0.0) {
    jets = clust_seq.inclusive_jets(inclkt);
  }

  if (excln > 0) {
    jets = sorted_by_E(clust_seq.exclusive_jets(excln));
  }

  if (excld > 0.0) {
    jets = sorted_by_pt(clust_seq.exclusive_jets(excld));
  }

  //  double median_pt_per_area = clust_seq.pt_per_unit_area();
  double median_pt_per_area = 0.0;
  printf(" ijet   rap      phi        Pt         area  +-   err   stddev  pt_corr\n");
  for (size_t j = 0; j < jets.size(); j++) {
    double area = clust_seq.area(jets[j]);
    
    printf("%5u %9.5f %8.5f %10.3f %8.3f +- %6.3f %7.3f %10.3f\n",j,jets[j].rap(),
	   jets[j].phi(),jets[j].perp(), area, clust_seq.area_error(jets[j]), clust_seq.area_error(jets[j])*sqrt(1.0*area_spec.repeat()), jets[j].perp() - area*median_pt_per_area);
  }

//  //cout << "median pt_over_area = " << clust_seq.pt_per_unit_area()<<endl;
//  cout << "median pt_over_area = " << clust_seq.pt_per_unit_area(fj::ClusterSequenceActiveArea::median)<<endl;
//  cout << "pt/area: " << clust_seq.pt_per_unit_area(fj::ClusterSequenceActiveArea::pttot_over_areatot)<<endl;
//  cout << "pt/area with cut: " << clust_seq.pt_per_unit_area(fj::ClusterSequenceActiveArea::pttot_over_areatot_cut)<<endl;
//  cout << "average ratio (with cut): "<< clust_seq.pt_per_unit_area(fj::ClusterSequenceActiveArea::mean_ratio_cut)<<endl;
//  
  } // iev
}

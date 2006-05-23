
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
///   -strategy N   indicate stratgey from the enum FjStrategy (see
///                 FjClusterSequence.hh).
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
#include "FjPseudoJet.hh"
#include "FjClusterSequence.hh"
#include "FjClusterSequenceWithMeanArea.hh"
#include<iostream>
#include<sstream>
#include<valarray>
#include<vector>
#include <cstdlib>
#include<cstddef> // for size_t
#include "CmdLine.hh"

using namespace std;

inline double pow2(const double x) {return x*x;};

void print_jet(const FjClusterSequence & cs, const FjPseudoJet & jet) {
  vector<FjPseudoJet> cnst = cs.constituents(jet);
  for (size_t i = 0; i < cnst.size(); i++) {
    printf("%6i %18.5f %18.5f %18.6e\n",i,cnst[i].rap(),cnst[i].phi(),cnst[i].perp());
  }
  printf("#END\n");
};

//----------------------------------------------------------------------
/// a program to test and time the kt algorithm as implemented in fastjet
int main (int argc, char ** argv) {

  CmdLine cmdline(argc,argv);
  // allow the use to specify the FjStrategy either through the
  // -clever or the -strategy options (both will take numerical
  // values); the latter will override the former.
  FjStrategy  strategy  = FjStrategy(cmdline.int_val("-strategy",
				     cmdline.int_val("-clever", Best)));
  int  repeat  = cmdline.int_val("-repeat",1);
  bool writeout   = cmdline.present("-write");
  bool hydjet  = cmdline.present("-hydjet");
  double ktR   = cmdline.double_val("-r",1.0);
  //double inclkt = cmdline.double_val("-incl",-1.0);
  //int    excln  = cmdline.int_val   ("-excln",-1);
  //double excld  = cmdline.double_val("-excld",-1.0);
  double etamax = cmdline.double_val("-etamax",1.0e310);
  bool   massless = cmdline.present("-massless");
  int    nev     = cmdline.int_val("-nev",1);
  bool   nopileup  = cmdline.present("-nopileup"); 
  double cell_area = cmdline.double_val("-cell_area",0.01);
  double ghost_etamax = cmdline.double_val("-ghost_etamax",6.0);
  double grid_scatter = cmdline.double_val("-grid_scatter",0.00001);
  double kt_scatter   = cmdline.double_val("-kt_scatter",0.1);
  bool   print_jets = cmdline.present("-print_jets");

  if (!cmdline.all_options_used()) {cerr << 
      "Error: some options unused"<<endl; 
    exit(-1);}

  for (int iev = 0; iev < nev; iev++) {
  vector<FjPseudoJet> full_event;
  vector<FjPseudoJet> hard_event;
  string line;
  int  nsub  = 0;
  cerr << "Doing event "<< iev<<endl;
  while (getline(cin, line)) {
      //cout << line<<endl;
    istringstream linestream(line);
    if (line == "#END") {break;}
    if (line == "#SUBSTART") {
      // if more sub events follow, make copy of hard one here
      if (nsub == 1) hard_event = full_event;
      nsub += 1;
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
      }
    } else {
      if (massless) {
	linestream >> fourvec[0] >> fourvec[1] >> fourvec[2];
	fourvec[3] = sqrt(pow2(fourvec[0])+pow2(fourvec[1])+pow2(fourvec[2]));}
      else {
	linestream >> fourvec[0] >> fourvec[1] >> fourvec[2] >> fourvec[3];
      }
    }
    FjPseudoJet psjet(fourvec);
    psjet.set_user_index(0);
    if (abs(psjet.rap() < etamax)) {full_event.push_back(psjet);}

  }

  // if we have read in only one event, copy it across here...
  if (nsub == 1) hard_event = full_event;
  // we needed to make sure we read things in 
  if (nopileup)  {full_event = hard_event;}


  //srand(2); // moved inside loop
  //double average_area = 0.0;
  //double average_area2 = 0.0;
  valarray<double> average_area; 
  valarray<double> average_area2;

    
  FjClusterSequenceWithMeanArea full_clust(full_event,
					  cell_area,ghost_etamax,
					  grid_scatter, kt_scatter, repeat,
					  ktR,strategy,writeout);

  FjClusterSequenceWithMeanArea hard_clust(hard_event,
					  cell_area,ghost_etamax,
					  grid_scatter, kt_scatter, repeat,
					  ktR,strategy,writeout);

  vector<FjPseudoJet> hard_jets = sorted_by_pt(hard_clust.inclusive_jets());
  vector<FjPseudoJet> full_jets = sorted_by_pt(full_clust.inclusive_jets());

  if (hard_jets[0].plain_distance(full_jets[0]) > 
      hard_jets[0].plain_distance(full_jets[1])) { 
    swap(full_jets[0],full_jets[1]);}

  double median_pt_per_area = full_clust.pt_per_unit_area();
  double median_pt_per_area_hard = hard_clust.pt_per_unit_area();

  for (int i = 0; i < 2; i++) {
    cout << full_jets[i].perp() - hard_jets[i].perp() <<" "
         << full_jets[i].plain_distance(hard_jets[i]) <<" "
	 << full_jets[i].perp() - hard_jets[i].perp() 
            - median_pt_per_area*full_clust.area(full_jets[i]) <<" "
         << - median_pt_per_area_hard*hard_clust.area(hard_jets[i]) <<
      endl ;
  }

  if (print_jets) {
  printf(" ijet   eta      phi        Pt         area  +-   err   stddev  pt_corr\n");
  for (size_t j = 0; j < full_jets.size(); j++) {
    double area = full_clust.area(full_jets[j]);
    
    printf("%5u %9.5f %8.5f %10.3f %8.3f +- %6.3f %7.3f %10.3f\n",j,full_jets[j].rap(),
	   full_jets[j].phi(),full_jets[j].perp(), area, full_clust.area_err(full_jets[j]), full_clust.area_err(full_jets[j])*sqrt(1.0*repeat), full_jets[j].perp() - area*median_pt_per_area);
  }

  //double dummy = full_clust.pt_per_unit_area(FjClusterSequenceWithMeanArea::play);
  //cout << "median pt_over_area = " << full_clust.pt_per_unit_area()<<endl;
  cerr << "median pt_over_area = " << full_clust.pt_per_unit_area(FjClusterSequenceWithMeanArea::median)<<endl;
  cerr << "old median  = " << full_clust.pt_per_unit_area(FjClusterSequenceWithMeanArea::old_median)<<endl;
  cerr << "pt/area: " << full_clust.pt_per_unit_area(FjClusterSequenceWithMeanArea::pttot_over_areatot)<<endl;
  cerr << "pt/area with cut: " << full_clust.pt_per_unit_area(FjClusterSequenceWithMeanArea::pttot_over_areatot_cut)<<endl;
  cerr << "average ratio (with cut): "<< full_clust.pt_per_unit_area(FjClusterSequenceWithMeanArea::mean_ratio_cut)<<endl;
  cerr << "pt/area with cut (range 3): " << full_clust.pt_per_unit_area(FjClusterSequenceWithMeanArea::pttot_over_areatot_cut,3.0)<<endl;
  cerr << "average ratio (range 3,with cut): "<< full_clust.pt_per_unit_area(FjClusterSequenceWithMeanArea::mean_ratio_cut,3.0)<<endl;
  } // if print_jets

  } // iev
}

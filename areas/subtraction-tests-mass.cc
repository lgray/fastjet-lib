
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
#include<fstream>
#include<valarray>
#include<vector>
#include <cstdlib>
#include<cstddef> // for size_t
#include "CmdLine.hh"
#include "CSHisto.hh"


using namespace std;

inline double pow2(const double x) {return x*x;};

void print_jet(const FjClusterSequence & cs, const FjPseudoJet & jet) {
  vector<FjPseudoJet> cnst = cs.constituents(jet);
  for (size_t i = 0; i < cnst.size(); i++) {
    printf("%6i %18.5f %18.5f %18.6e\n",i,cnst[i].rap(),cnst[i].phi(),cnst[i].perp());
  }
  printf("#END\n");
};


void determine_Zmass_kt(const vector<FjPseudoJet> & event, 
			double cell_area, double ghost_etamax,
			double grid_scatter, double kt_scatter, int repeat,
			double ktR, FjStrategy strategy,
			double & mass, double & corrected_mass);

void read_event(istream &, double, bool, bool,
		vector<FjPseudoJet> &, vector<FjPseudoJet> & );

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
  //bool writeout   = cmdline.present("-write");
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
  double bin_width    = cmdline.double_val("-bin",5.0);
  //bool   print_jets   = cmdline.present("-print_jets");
  string input_file   = cmdline.string_val("-in");
  string output_file  = cmdline.string_val("-out");

  if (!cmdline.all_options_used()) {cerr << 
      "Error: some options unused"<<endl; 
    exit(-1);}

  // input will be from the file named with the "-in" option
  ifstream input(input_file.c_str());

  double max_bin = 400.0; int nbins = int(max_bin/bin_width + 0.5);
  CSHisto inv_mass_hard(00.0, max_bin, nbins);
  CSHisto inv_mass_hcor(00.0, max_bin, nbins);
  CSHisto inv_mass_full(00.0, max_bin, nbins);
  CSHisto inv_mass_fcor(00.0, max_bin, nbins);

  for (int iev = 0; iev < nev; iev++) {
    cerr << "Doing event "<< iev<<endl;
    vector<FjPseudoJet> hard_event, full_event;
    
    // read in the event 
    read_event(input, etamax, hydjet, massless, hard_event, full_event);
      
    // dumb it down if need be...
    if (nopileup)  full_event = hard_event;
    
    // deduce the masses
    double hard_ev_mass, hcor_ev_mass;
    determine_Zmass_kt(hard_event,
		       cell_area,ghost_etamax, grid_scatter, kt_scatter, 
		       repeat, ktR, strategy, hard_ev_mass, hcor_ev_mass);
    double full_ev_mass, fcor_ev_mass;
    determine_Zmass_kt(full_event,
		       cell_area,ghost_etamax, grid_scatter, kt_scatter, 
		       repeat, ktR, strategy, full_ev_mass, fcor_ev_mass);
    
    cout <<"inv mass of two hardest (hard) jets = "<< hard_ev_mass << endl;
    cout <<"inv mass of two hardest (hcor) jets = "<< hcor_ev_mass << endl;
    cout <<"inv mass of two hardest (full) jets = "<< full_ev_mass << endl;
    cout <<"inv mass of two hardest (fcor) jets = "<< fcor_ev_mass << endl;
    
    inv_mass_hard.fill(hard_ev_mass);
    inv_mass_hcor.fill(hcor_ev_mass);
    inv_mass_full.fill(full_ev_mass);
    inv_mass_fcor.fill(fcor_ev_mass);
    
  } // iev
  
  
  // sending output to a file...
  ofstream output(output_file.c_str());
  output << "# " << cmdline.command_line() << endl;
  output << "# bin-centre hard hcor full fcor" <<endl;

  // print out mass histograms.
  for (unsigned i = 0; i < inv_mass_hard.size(); i++) {
    output <<  inv_mass_hard.bin_centre(i) <<" "
	    << inv_mass_hard.bin_weight(i) <<" "
	    << inv_mass_hcor.bin_weight(i) <<" "
	    << inv_mass_full.bin_weight(i) <<" "
	    << inv_mass_fcor.bin_weight(i) << endl;
  }
}




//======================================================================
void determine_Zmass_kt(const vector<FjPseudoJet> & event, 
			double cell_area, double ghost_etamax,
			double grid_scatter, double kt_scatter, int repeat,
			double ktR, FjStrategy strategy,
			double & mass, double & corrected_mass) {

  FjClusterSequenceWithMeanArea clust(event,
				      cell_area,ghost_etamax,
				      grid_scatter, kt_scatter, repeat,
				      ktR,strategy);

  double median_pt_per_area = clust.pt_per_unit_area();
  vector<FjPseudoJet> jets = sorted_by_pt(clust.inclusive_jets());

  vector<FjPseudoJet> corrected_jets(jets.size());
  for (unsigned i = 0; i < jets.size(); i++) {
    double correction_factor = 1 - 
      median_pt_per_area*clust.area(jets[i])/jets[i].perp(); 
    corrected_jets[i] =  max(correction_factor,0.0) * jets[i];
  }
  corrected_jets = sorted_by_pt(corrected_jets);

  mass            = sqrt(abs((jets[0]+jets[1]).m2()));
  corrected_mass = sqrt(abs((corrected_jets[0]+corrected_jets[1]).m2()));

}


//======================================================================
void read_event(istream & input, double etamax, bool hydjet, bool massless,
		vector<FjPseudoJet> & hard_event, 
		vector<FjPseudoJet> & full_event) {
  string line;
  int  nsub  = 0;
  while (getline(input, line)) {
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

}

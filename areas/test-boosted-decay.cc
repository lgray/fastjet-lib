
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
#include "fastjet/ClusterSequenceArea.hh"
#include<iostream>
#include<sstream>
#include<fstream>
#include<valarray>
#include<vector>
#include <cstdlib>
#include<cstddef> // for size_t
#include "CmdLine.hh"
#include "SimpleHist.hh"
#include "jet_def_from_cmdline.hh"
#include "read_event.hh"
#include <cmath>

namespace fj = fastjet;
using namespace fj;
using namespace std;

const double maxrap_for_median = 4.0;

void print_jet(const fj::ClusterSequence & cs, const fj::PseudoJet & jet) {
  vector<fj::PseudoJet> cnst = cs.constituents(jet);
  for (size_t i = 0; i < cnst.size(); i++) {
    printf("%6i %18.5f %18.5f %18.6e\n",i,cnst[i].rap(),cnst[i].phi(),cnst[i].perp());
  }
  printf("#END\n");
};



void determine_Zmass_kt(const vector<fj::PseudoJet> & event, 
                        fj::JetDefinition jet_def,
                        fj::AreaDefinition area_def,
                        bool                       rho_uses_something,
	   	        const fj::JetDefinition  & rho_jet_def,
		        const fj::AreaDefinition & rho_area_def,
			double & mass, double & corrected_mass, 
			double & ext_corrected_mass);

//enum ConeVariant {not_cone, midpoint_050, midpoint_075, searchcone_075};

//void determine_Zmass_cone(const vector<fj::PseudoJet> & event, 
//			  double R, ConeVariant cone_variant,
//			  double & mass, double & corrected_mass);

double Zmass_from_jets(const vector<fj::PseudoJet> & jets);



//----------------------------------------------------------------------
/// a program to test and time the kt algorithm as implemented in fastjet
int main (int argc, char ** argv) {

  CmdLine cmdline(argc,argv);
  // allow the use to specify the fj::Strategy either through the
  // -clever or the -strategy options (both will take numerical
  // values); the latter will override the former.
  fj::Strategy  strategy  = fj::Strategy(cmdline.int_val("-strategy",
				     cmdline.int_val("-clever", fj::Best)));
  int  repeat  = cmdline.int_val("-repeat",1);
  //bool writeout   = cmdline.present("-write");
  bool hydjet  = cmdline.present("-hydjet");
  double etamax = cmdline.double_val("-etamax",1.0e310);
  bool   massless = cmdline.present("-massless");
  int    nev      = int(cmdline.double_val("-nev",1.0));
  bool   nopileup  = cmdline.present("-nopileup"); 

  // properties of the jet algorithm and the area finding
  fj::JetDefinition jet_def = jet_def_from_cmdline(cmdline);
  fj::AreaDefinition area_def = area_def_from_cmdline(cmdline);

  // for (e.g.) cone algorithm, allow one to estimate rho with a more
  // reliable alg.
  bool rho_uses_cam05 = cmdline.present("-rho_uses_cam05");
  bool rho_uses_kt05 = cmdline.present("-rho_uses_kt05");
  bool rho_uses_something = false;
  fj::JetDefinition  rho_jet_def ;
  fj::AreaDefinition rho_area_def;
  if (rho_uses_cam05) {
    rho_jet_def  = fj::JetDefinition (fj::cambridge_algorithm,0.5);
    rho_area_def = fj::AreaDefinition(fj::VoronoiAreaSpec(0.5));
    rho_uses_something = true;
  } else if (rho_uses_kt05) {
    rho_jet_def  = fj::JetDefinition (fj::kt_algorithm,0.5);
    rho_area_def = fj::AreaDefinition(fj::VoronoiAreaSpec(0.9));
    rho_uses_something = true;
  }

  double bin_width    = cmdline.double_val("-bin",2.0);
  double max_bin      = cmdline.double_val("-max",200.0);
  double min_bin      = cmdline.double_val("-min",0.0);
  //bool   print_jets   = cmdline.present("-print_jets");
  string input_file   = cmdline.string_val("-in");
  string output_file  = cmdline.string_val("-out");
  string ev_output_file  = cmdline.string_val("-evout",output_file+".ev");

  int    writefreq    = int(cmdline.double_val("-freq",1.0*max(nev/10,1000)));
  string rerun_string = cmdline.string_val("-rerun","");
  cerr <<"writefreq is "<<writefreq<<endl;

  if (!cmdline.all_options_used()) {cerr << 
      "Error: some options unsupported"<<endl; 
    exit(-1);}


  // input will be from the file named with the "-in" option
  ifstream input(input_file.c_str());
  ofstream  * ev_output = NULL;
  if (ev_output_file != "") {
    ev_output = new ofstream(ev_output_file.c_str());
    *ev_output << "# " << cmdline.command_line() << endl;
    *ev_output << "# " << jet_def.description() << endl;
  }

  int nbins = int((max_bin-min_bin)/bin_width + 0.5);
  SimpleHist mass_dist(min_bin, max_bin, nbins);
  SimpleHist rtktdist_dist(min_bin, max_bin, nbins);
  SimpleHist massB_dist(min_bin, max_bin, nbins);
  SimpleHist rtktdistB_dist(min_bin, max_bin, nbins);


  for (int iev = 0; iev < nev; iev++) {
    cerr << "Doing event "<< iev<<endl;
    vector<fj::PseudoJet> hard_event, full_event;
    
    // read in the event 
    read_event_common(input, etamax, hydjet, massless, hard_event, full_event);

    // dumb it down if need be...
    if (nopileup)  full_event = hard_event;

    // keep it simple for now...
    ClusterSequence cs(full_event, jet_def);
    vector<PseudoJet> jets = sorted_by_pt(cs.inclusive_jets());
    for (unsigned j = 0; j < min(2U,jets.size()); j++) {
      PseudoJet & jet = jets[j];
      double mass = jet.m();
      double sqrt_ktdist = 0;
      PseudoJet parent1, parent2;
      if (cs.has_parents(jet,parent1,parent2)) {
        sqrt_ktdist = sqrt(parent1.kt_distance(parent2));
      }
      
      // now try recursive subjet analysis
      double massB = 0.0;
      double rtktB = 0.0;
      PseudoJet this_jet = jet;
      while (cs.has_parents(this_jet,parent1,parent2)) {
        if (parent1.m2() < parent2.m2()) swap(parent1, parent2);
        //cout << "   --> " << this_jet.m() << " " << parent1.m() << endl;
        if (parent1.m() < 0.66667*this_jet.m()) {
          break;
        } else {
          this_jet = parent1;
        }
      }
      massB = this_jet.m();
      if (cs.has_parents(this_jet,parent1,parent2)) {
        rtktB = sqrt(parent1.kt_distance(parent2));
      }
      

      // limit the amount of information that is output 
      if (iev < 100) {
        // provide user with some info (maybe get rid of this at some point?)
        cout << mass << " " << sqrt_ktdist << " " 
             << massB << " " << rtktB << endl;
      }
      if (ev_output != NULL) {
        *ev_output << mass << " " << sqrt_ktdist << " " 
                   << massB << " " << rtktB << endl;
      }

      mass_dist.add_entry(mass);
      rtktdist_dist.add_entry(sqrt_ktdist);
      massB_dist.add_entry(massB);
      rtktdistB_dist.add_entry(rtktB);
    }
      
    
    // write intermediate and final results...
    if ( iev+1==nev || (iev+1) % writefreq == 0) {
      // sending output to a file...
      ofstream output(output_file.c_str());
      if (rerun_string != "") {
	output << "# Rerun with:\n";
	output << "# "<<rerun_string<<endl;
      }
      output << "# " << cmdline.command_line() << endl;
      output << "# jet_def  = " <<  jet_def.description() << endl;
      output << "# area_def = " << area_def.description() << endl;
      if (rho_uses_something) {
        output << "# rho_jet_def  = " <<  rho_jet_def.description() << endl;
        output << "# rho_area_def = " << rho_area_def.description() << endl;
      }
      output << "# maxrap_for_median = " << maxrap_for_median << endl;
      output << "# nev = " <<iev+1 <<endl;
      output << "# bin-centre mass_hist sqrt_ktdist_hist" <<endl;
      
      double norm = (iev+1) * bin_width;

      // print out mass histograms.
      for (unsigned i = 0; i < mass_dist.size(); i++) {
	output << mass_dist.binmid(i)          << " "
	       << mass_dist[i]/norm     << " "
	       << rtktdist_dist[i]/norm << " "
	       << massB_dist[i]/norm     << " "
	       << rtktdistB_dist[i]/norm << endl;
      }
    }
    
  } // iev
  
  
}


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
#include "FlavourHolder.hh"

// for getting cone algorithm from CDF
#include "MidPointAlgorithm.hh"
#include "PhysicsTower.hh"
#include "Cluster.hh"


using namespace std;

inline double pow2(const double x) {return x*x;};




//void determine_Zmass_kt(const vector<FjPseudoJet> & event, 
//			double cell_area, double ghost_etamax,
//			double grid_scatter, double kt_scatter, int repeat,
//			double ktR, FjStrategy strategy,
//			double & mass, double & corrected_mass, 
//			double & ext_corrected_mass);

enum ConeVariant {not_cone, midpoint_050, midpoint_075, searchcone_075};

//void determine_Zmass_cone(const vector<FjPseudoJet> & event, 
//			  double R, ConeVariant cone_variant,
//			  double & mass, double & corrected_mass);
//

void look_at_event(vector<FjPseudoJet> & event,
		   const FjJetDefinition  & jet_def,
		   const FjActiveAreaSpec & area_spec);

double Zmass_from_jets(const vector<FjPseudoJet> & jets);

void read_event(istream &, double, bool, bool,
		vector<FjPseudoJet> &, vector<FjPseudoJet> & );


//----------------------------------------------------------------------
/// a program to test and time the kt algorithm as implemented in fastjet
int main (int argc, char ** argv) {

  CmdLine cmdline(argc,argv);
  // how we treat the event that is read in
  string input_file = cmdline.string_val("-in");
  bool   hydjet     = cmdline.present("-hydjet");
  double etamax     = cmdline.double_val("-etamax",1.0e310);
  bool   massless   = cmdline.present("-massless");
  int    nev        = int(cmdline.double_val("-nev",1.0));
  bool   nopileup   = cmdline.present("-nopileup"); 

  // properties of the jet algorithm
  // allow the use to specify the FjStrategy either through the
  // -clever or the -strategy options (both will take numerical
  // values); the latter will override the former.
  FjStrategy  strategy  = FjStrategy(cmdline.int_val("-strategy",
				     cmdline.int_val("-clever", Best)));
  double ktR   = cmdline.double_val("-r",1.0);
  FjJetFinder jet_fndr= cmdline.present("-cam")? cambridge_algorithm: kt_algorithm;
  FjJetDefinition jet_def(jet_fndr, ktR, strategy);

  // allow for a cone???
  ConeVariant cone_variant = not_cone;
  if (cmdline.present("-searchcone")) {
    cone_variant = searchcone_075; }
  else if (cmdline.present("-cone")) {
    cone_variant = midpoint_050; }
  else if (cmdline.present("-cone075")) {
    cone_variant = midpoint_075; }
  bool   cone         = cone_variant != not_cone;

  // set up things to do with how we measure the area
  FjActiveAreaSpec area_spec;
  area_spec.set_repeat      (cmdline.int_val("-repeat",1)            );
  area_spec.set_cell_area   (cmdline.double_val("-cell_area",0.01)   );
  area_spec.set_ghost_etamax(cmdline.double_val("-ghost_etamax",6.0) );
  area_spec.set_grid_scatter(cmdline.double_val("-grid_scatter",1e-5));
  area_spec.set_kt_scatter  (cmdline.double_val("-kt_scatter",0.1)   );

  // how we process and output things
  double bin_width    = cmdline.double_val("-bin",5.0);
  double max_bin      = cmdline.double_val("-max",400.0);
  string output_file  = cmdline.string_val("-out");
  int    writefreq    = int(cmdline.double_val("-freq",1.0*max(nev/10,1000)));
  string rerun_string = cmdline.string_val("-rerun","");
  cerr <<"writefreq is "<<writefreq<<endl;

  // sanity check on command-line structure
  if (!cmdline.all_options_used()) {cerr << 
      "Error: some options unsupported"<<endl; 
    exit(-1);}

  // input will be from the file named with the "-in" option
  ifstream input(input_file.c_str());

  //int nbins = int(max_bin/bin_width + 0.5);
  //CSHisto inv_mass_hard(00.0, max_bin, nbins);
  //CSHisto inv_mass_hcor(00.0, max_bin, nbins);
  //CSHisto inv_mass_full(00.0, max_bin, nbins);
  //CSHisto inv_mass_fcor(00.0, max_bin, nbins);
  //
  //// histograms using the "extended" area subtraction...
  //CSHisto inv_mass_hecr(00.0, max_bin, nbins);
  //CSHisto inv_mass_fecr(00.0, max_bin, nbins);


  for (int iev = 0; iev < nev; iev++) {
    cerr << "Doing event "<< iev<<endl;
    vector<FjPseudoJet> hard_event, full_event;
    
    // read in the event 
    read_event(input, etamax, hydjet, massless, hard_event, full_event);
      
    // dumb it down if need be...
    if (nopileup)  full_event = hard_event;
  
    look_at_event(hard_event, jet_def, area_spec);
    
  } // iev
  
  
}



//======================================================================
double Zmass_from_jets(const vector<FjPseudoJet> & jets) {
  vector<FjPseudoJet> sorted_jets = sorted_by_pt(jets);
  double mass = sqrt(abs((sorted_jets[0]+sorted_jets[1]).m2()));
  return mass;
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
    int particleID=0, particleCharge=0; // default vals if not set by prog
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
	linestream >> particleID >> particleCharge;
      }
    }
    FjPseudoJet psjet(fourvec);
    psjet.set_user_index(particleID);
    if (abs(psjet.rap() < etamax)) {full_event.push_back(psjet);}

  }

  // if we have read in only one event, copy it across here...
  if (nsub == 1) hard_event = full_event;

  // if there was nothing in the event 
  if (nsub == 0) {
    cerr << "Error: read empty event\n";
    exit(-1);
  }
}

// this will often be useful...
typedef vector<FjPseudoJet>::const_iterator FJPJ_iter;


//-------------------------------------------------------------
/// Return a string that says how many b and b-bar there are in 
/// the jet (b = b, B = bbar)
string b_string(const FjClusterSequence & cs, const FjPseudoJet & jet) {

  string res;
  vector<FjPseudoJet> cnst = cs.constituents(jet);
  for (FJPJ_iter particle = cnst.begin(); particle != cnst.end(); particle++) {
    int nb = particle -> user_index();
    if      (nb > 0) {res += "b";}
    else if (nb < 0) {res += "B";}
  }
  return res;
}


//-------------------------------------------------------------
/// routine for ("visually") looking at a ttbar event
void look_at_event(vector<FjPseudoJet> & event,
		   const FjJetDefinition  & jet_def,
		   const FjActiveAreaSpec & area_spec) {
  
  vector<FjPseudoJet> hadronic_event;
  vector<FjPseudoJet> leptonic_event;

  // we will separate out the muon and any neutrinos from the other
  // particles (using the user index which has been set to the particle
  // idhep value); we'll treat electrons and taus as if they're hadronic
  // since we are generating semi-leptonic ttbar events where the lepton
  // is a muon.
  for (FJPJ_iter particle = event.begin(); particle != event.end(); particle++){

    FlavourHolder flav(particle->user_index());

    if (flav.is_neutrino() || flav.is_muon()) {
      leptonic_event.push_back(*particle);
    } else {
      FjPseudoJet hadron = *particle;
      hadron.set_user_index(flav[5]); // set hadron user index to number of b-quarks
      hadronic_event.push_back(hadron);
    }
  }

  FjClusterSequenceWithMeanArea clust_seq(hadronic_event, jet_def, area_spec);

  // print general header...
  printf(" rap      phi        Pt         area  +-   err      pt_corr  flavour\n");

  // print leptonic part of the event 
  for (FJPJ_iter lepton = leptonic_event.begin(); 
                                   lepton != leptonic_event.end(); lepton++){
    printf("%9.5f %8.5f %10.3f %8.3f +- %6.3f %10.3f %7d\n",
	   lepton->rap(), lepton->phi(), lepton->perp(), 0.0,0.0,lepton->perp(),
	   lepton->user_index());
  }

  cout << " "<<endl;

  // print jetty part of the event (only jets with pt > 5 GeV)
  vector<FjPseudoJet> jets = clust_seq.inclusive_jets(5.0);
  double median_pt_over_area = clust_seq.pt_per_unit_area(
				  FjClusterSequenceWithMeanArea::median);

  for (FJPJ_iter jet = jets.begin(); jet != jets.end(); jet++){
    printf("%9.5f %8.5f %10.3f %8.3f +- %6.3f %10.3f %7s\n",
	   jet->rap(), jet->phi(), jet->perp(), 
	   clust_seq.area(*jet),clust_seq.area_err(*jet),
	   jet->perp()-median_pt_over_area*clust_seq.area(*jet),
	   b_string(clust_seq, *jet).c_str()
	   );
  }
}

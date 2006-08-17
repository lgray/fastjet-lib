
//STARTHEADER
// $Id: areas-test-mean-class.cc 265 2006-08-11 21:40:31Z salam $
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
// fastjet subtraction example program. 
//
// Compile it with: make fastjet_subtraction
// run it with    : ./fastjet_subtraction < data/single-event.dat
//
//----------------------------------------------------------------------
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequenceActiveArea.hh"
#include<iostream> // needed for io
#include<sstream>  // needed for internal io
#include<vector> 

using namespace std;

// a declaration of a function that pretty prints a list of jets
void print_jets (const fastjet::ClusterSequenceActiveArea &, 
                 const vector<fastjet::PseudoJet> &);

/// an example program showing how to use fastjet
int main (int argc, char ** argv) {
  
  vector<fastjet::PseudoJet> input_particles;
  
  // read in input particles
  double px, py , pz, E;
  while (cin >> px >> py >> pz >> E) {
    // create a fastjet::PseudoJet with these components and put it onto
    // back of the input_particles vector
    input_particles.push_back(fastjet::PseudoJet(px,py,pz,E)); 
  }
  
  // create an object that represents your choice of jet finder and 
  // the associated parameters
  double Rparam = 1.0;
  fastjet::Strategy strategy = fastjet::Best;
  fastjet::JetDefinition jet_def(fastjet::kt_algorithm, Rparam, strategy);

  // create an object that specifies how we to define the (active) area
  double ghost_etamax = 6.0;
  int    active_area_repeats = 3;
  double ghost_area    = 0.01;
  fastjet::ActiveAreaSpec area_spec(ghost_etamax, active_area_repeats, 
                                    ghost_area);

  // run the jet clustering with the above jet definition
  fastjet::ClusterSequenceActiveArea clust_seq(input_particles, 
                                               jet_def, area_spec);

  // tell the user what was done
  cout << "Strategy adopted by FastJet was "<<
       clust_seq.strategy_string()<<endl<<endl;

  // extract the inclusive jets with pt > 5 GeV, sorted by pt
  double ptmin = 5.0;
  vector<fastjet::PseudoJet> inclusive_jets = clust_seq.inclusive_jets(ptmin);

  // print them out
  cout << "Printing inclusive jets with pt > "<< ptmin<<" GeV\n";
  cout << "---------------------------------------\n";
  print_jets(clust_seq, inclusive_jets);
  cout << endl;


}


//----------------------------------------------------------------------
/// a function that pretty prints a list of jets
void print_jets (const fastjet::ClusterSequenceActiveArea & clust_seq, 
		 const vector<fastjet::PseudoJet> & unsorted_jets ) {

  // sort jets into increasing pt
  vector<fastjet::PseudoJet> jets = sorted_by_pt(unsorted_jets);  

  // get median pt per unit area
  // NB pt_per_unit_area only exists in ClusterSequenceActiveArea, and
  // not in the base class ClusterSequenceWithArea
  double median_pt_per_area = clust_seq.pt_per_unit_area();

  printf(" ijet   rap      phi        Pt         area     Pt corr\n");
  for (size_t j = 0; j < jets.size(); j++) {

    double area     = clust_seq.area(jets[j]);
    double pt_corr  = jets[j].perp() - area*median_pt_per_area;

    printf("%5u %9.5f %8.5f %10.3f %8.3f %10.3f\n",j,jets[j].rap(),
	   jets[j].phi(),jets[j].perp(), area, pt_corr);
  }

  cout << endl;
  cout << "median pt_over_area = " << median_pt_per_area << endl << endl;


};








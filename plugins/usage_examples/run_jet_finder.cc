//STARTHEADER
// $Id: ClusterSequence.cc 370 2006-11-28 16:25:44Z salam $
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

#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include<iostream> // needed for io
#include<sstream>  // needed for internal io
#include<vector> 

using namespace std;

// a declaration of a function that pretty prints a list of jets
void print_jets (const fastjet::ClusterSequence &, 
                 const vector<fastjet::PseudoJet> &);



/// subroutine that runs fastjet (on input particles from cin) using
/// an arbitrary jet definition supplied as an argument
void run_jet_finder (const vector<fastjet::PseudoJet> & input_particles,
                     const fastjet::JetDefinition & jet_def) {
  
  // run the jet clustering with the above jet definition
  fastjet::ClusterSequence clust_seq(input_particles, jet_def);

  // tell the user what was done
  cout << "Ran " << jet_def.description() << endl;

  // extract the inclusive jets with pt > 5 GeV
  double ptmin = 5.0;
  vector<fastjet::PseudoJet> inclusive_jets = clust_seq.inclusive_jets(ptmin);

  // print them out
  cout << "Printing inclusive jets with pt > "<< ptmin<<" GeV\n";
  cout << "---------------------------------------\n";
  print_jets(clust_seq, inclusive_jets);
  cout << endl;


}


//----------------------------------------------------------------------
/// a function that reads particles from the supplied istream
void read_input_particles(istream & input, 
                          vector<fastjet::PseudoJet> & input_particles){

  // read in input particles
  double px, py , pz, E;
  while (input >> px >> py >> pz >> E) {
    // create a fastjet::PseudoJet with these components and put it onto
    // back of the input_particles vector
    input_particles.push_back(fastjet::PseudoJet(px,py,pz,E)); 
  }
  
}

//----------------------------------------------------------------------
/// a function that pretty prints a list of jets
void print_jets (const fastjet::ClusterSequence & clust_seq, 
		 const vector<fastjet::PseudoJet> & jets) {

  // sort jets into increasing pt
  vector<fastjet::PseudoJet> sorted_jets = sorted_by_pt(jets);  

  // label the columns
  printf("%5s %15s %15s %15s %15s\n","jet #", "rapidity", 
	 "phi", "pt", "n constituents");
  
  // print out the details for each jet
  for (unsigned int i = 0; i < sorted_jets.size(); i++) {
    int n_constituents = clust_seq.constituents(sorted_jets[i]).size();
    printf("%5u %15.8f %15.8f %15.8f %8u\n",
	   i, sorted_jets[i].rap(), sorted_jets[i].phi(),
	   sorted_jets[i].perp(), n_constituents);
  }

};

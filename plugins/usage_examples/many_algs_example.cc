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
// example program to illustrate the use of multiple jet-definitions,
// some of which are plugin based.
//
// Compile it with: make many_algs_example
// run it with    : ./many_algs_example < data/single-event.dat
//----------------------------------------------------------------------

#include "run_jet_finder.hh"
#include "CDFMidPointPlugin.hh"
#include "PxConePlugin.hh"
#include<vector>
#include<iostream>
#include "fastjet/PseudoJet.hh"

using namespace std;

int main(int argc, char** argv) {

  // read input particles
  vector<fastjet::PseudoJet> input_particles;
  read_input_particles(cin, input_particles);
  
  // we will have four jet definitions, and the first two will be
  // plugins
  vector<fastjet::JetDefinition> jet_defs(4);
  vector<fastjet::JetDefinition::Plugin *> plugins(2);

  // common parameters
  double jet_radius = 0.7;
  double overlap_threshold = 0.5;


  // set up a pxcone jet definition
  double min_jet_energy = 5.0;
  bool   E_scheme_jets = false;
  plugins[0] = new fastjet::PxConePlugin (jet_radius, min_jet_energy, 
                                         overlap_threshold, E_scheme_jets);
  jet_defs[0] = fastjet::JetDefinition(plugins[0]);



  // set up a CDF midpoint jet definition
  double seed_threshold = 1.0;
  double cone_area_fraction = 1.0;
  int    max_pair_size = 2;
  int    max_iterations = 100;
  plugins[1] = new fastjet::CDFMidPointPlugin (seed_threshold, jet_radius, 
                                          cone_area_fraction, max_pair_size,
                                          max_iterations, overlap_threshold);
  jet_defs[1] = fastjet::JetDefinition(plugins[1]);
  

  // set up kt and cam/aachen definitions
  jet_defs[2] = fastjet::JetDefinition(fastjet::kt_algorithm, jet_radius);
  jet_defs[3] = fastjet::JetDefinition(fastjet::cambridge_algorithm, 
                                       jet_radius);


  // call the example jet-finding routine with each of jet definitions
  for (vector<fastjet::JetDefinition>::const_iterator jd_it = jet_defs.begin();
       jd_it != jet_defs.end(); jd_it++) {
    run_jet_finder(input_particles, *jd_it);
  }

  // clean up plugin memory.
  delete plugins[0];
  delete plugins[1];
}

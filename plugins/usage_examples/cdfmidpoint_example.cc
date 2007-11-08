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
// example program to illustrate the use of the cdf midpoint plugin
//
// Compile it with: make cdfmidpoint_example
// run it with    : ./cdfmidpoint_example < data/single-event.dat
//
//----------------------------------------------------------------------

#include "run_jet_finder.hh"

// get info on how fastjet was configured
#include "fastjet/config.h"

// make sure we have what is needed
#ifdef ENABLE_PLUGIN_CDFCONES
#  include "fastjet/CDFMidPointPlugin.hh"
#else
#  error "CDFMidPointPlugin not configured according to config.h"
#endif

#include<vector>
#include<iostream>
using namespace std;

int main(int argc, char** argv) {

  
  // define a plugin pointer
  fastjet::JetDefinition::Plugin * plugin;

  // allocate a new plugin
  double seed_threshold = 1.0;
  double cone_radius = 0.7;
  double cone_area_fraction = 1.0;
  int    max_pair_size = 2;
  int    max_iterations = 100;
  double overlap_threshold = 0.5;
  plugin = new fastjet::CDFMidPointPlugin (seed_threshold, cone_radius, 
                                           cone_area_fraction, max_pair_size,
                                           max_iterations, overlap_threshold);

  // create a jet-definition based on the plugin
  fastjet::JetDefinition jet_def(plugin);

  // call the example routine that runs with arbitrary jet definitions
  vector<fastjet::PseudoJet> particles;
  read_input_particles(cin, particles);
  run_jet_finder(particles, jet_def);

  // clean up plugin memory.
  delete plugin;
}

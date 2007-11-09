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

#include "fastjet/config.h"
#include "run_jet_finder.hh"

// get info on how fastjet was configured
#include "fastjet/config.h"

#ifdef ENABLE_PLUGIN_SISCONE
#  include "fastjet/SISConePlugin.hh"
#endif
#ifdef ENABLE_PLUGIN_CDFCONES
#  include "fastjet/CDFMidPointPlugin.hh"
#  include "fastjet/CDFJetCluPlugin.hh"
#endif
#ifdef ENABLE_PLUGIN_PXCONE
#  include "fastjet/PxConePlugin.hh"
#endif

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
  vector<fastjet::JetDefinition> jet_defs(6);
  vector<fastjet::JetDefinition::Plugin *> plugins(3);

  // common parameters
  double jet_radius = 0.7;
  //double jet_radius = 1.0;
  double overlap_threshold = 0.5;

  vector<fastjet::JetDefinition>::const_iterator 
    jet_def_begin = jet_defs.begin();

  // set up a pxcone jet definition (if wanted -- requires f77, and you
  // should compile the pxcone plugin (not there by default))
#ifdef ENABLE_PLUGIN_PXCONE
  double min_jet_energy = 5.0;
  bool   E_scheme_jets = false;
  plugins[0] = new fastjet::PxConePlugin (jet_radius, min_jet_energy, 
                                         overlap_threshold, E_scheme_jets);
  jet_defs[0] = fastjet::JetDefinition(plugins[0]);
#else
  plugins[0] = NULL;
  //jet_def_begin++; // skip first jet def
#endif // ENABLE_PLUGIN_PXCONE



  // set up a CDF midpoint jet definition
#ifdef ENABLE_PLUGIN_CDFCONES
  double seed_threshold = 1.0;
  double cone_area_fraction = 1.0;
  int    max_pair_size = 2;
  int    max_iterations = 100;
  plugins[1] = new fastjet::CDFMidPointPlugin (seed_threshold, jet_radius, 
                                          cone_area_fraction, max_pair_size,
                                          max_iterations, overlap_threshold);
  jet_defs[1] = fastjet::JetDefinition(plugins[1]);
#else
  plugins[1] = NULL;
#endif

  // set up a siscone jet definition
#ifdef ENABLE_PLUGIN_SISCONE
  int npass = 0;               // do infinite number of passes
  double protojet_ptmin = 0.0; // use all protojets
  plugins[2] = new fastjet::SISConePlugin (jet_radius, overlap_threshold, 
                                           npass, protojet_ptmin);
  jet_defs[2] = fastjet::JetDefinition(plugins[2]);
#else
  plugins[2] = NULL;
#endif

  // set up kt and cam/aachen definitions
  jet_defs[3] = fastjet::JetDefinition(fastjet::kt_algorithm, jet_radius);
  jet_defs[4] = fastjet::JetDefinition(fastjet::cambridge_algorithm, 
                                       jet_radius);
  jet_defs[5] = fastjet::JetDefinition(fastjet::antikt_algorithm, 
                                       jet_radius);

  // call the example jet-finding routine with each of jet definitions
  int index=0;
  for (vector<fastjet::JetDefinition>::const_iterator jd_it = jet_def_begin;
       jd_it != jet_defs.end(); jd_it++) {
    // only run if the plugin is available
    if ((index>=3) || (plugins[index] != NULL))
      run_jet_finder(input_particles, *jd_it);
    index++;
  }

  // clean up plugin memory.
  if (plugins[0] != NULL) delete plugins[0]; // pxcone may not be defined
  if (plugins[1] != NULL) delete plugins[1]; // cdfcones may not be defined
  if (plugins[2] != NULL) delete plugins[2]; // siscone may not be defined
}

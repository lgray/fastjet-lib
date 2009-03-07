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
#ifdef ENABLE_PLUGIN_D0RUNIICONE
#  include "fastjet/D0RunIIConePlugin.hh"
#endif
#ifdef ENABLE_PLUGIN_TRACKJET
#include "fastjet/TrackJetPlugin.hh"
#endif
#ifdef ENABLE_PLUGIN_ATLASCONE
#include "fastjet/ATLASConePlugin.hh"
#endif
#ifdef ENABLE_PLUGIN_EECAMBRIDGE
#include "fastjet/EECambridgePlugin.hh"
#endif
#ifdef ENABLE_PLUGIN_JADE
#include "fastjet/JadePlugin.hh"
#endif
// end of the plugin list (don't modify this line)

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
  vector<fastjet::JetDefinition> jet_defs;
  vector<fastjet::JetDefinition::Plugin *> plugins;

  // common parameters
  double jet_radius = 0.7;
  //double jet_radius = 1.0;
  double overlap_threshold = 0.5;

  // set up a pxcone jet definition (if wanted -- requires f77, and you
  // should compile the pxcone plugin (not there by default))
#ifdef ENABLE_PLUGIN_PXCONE
  double min_jet_energy = 5.0;
  bool   E_scheme_jets = false;
  plugins.push_back( new fastjet::PxConePlugin (jet_radius, min_jet_energy, 
                                        overlap_threshold, E_scheme_jets));
  jet_defs.push_back(fastjet::JetDefinition(plugins.back()));
#endif // ENABLE_PLUGIN_PXCONE



  // set up a CDF midpoint jet definition
#ifdef ENABLE_PLUGIN_CDFCONES
  double seed_threshold = 1.0;
  double cone_area_fraction = 1.0;
  int    max_pair_size = 2;
  int    max_iterations = 100;
  plugins.push_back(new fastjet::CDFMidPointPlugin(seed_threshold, jet_radius, 
                                          cone_area_fraction, max_pair_size,
                                          max_iterations, overlap_threshold));
  jet_defs.push_back(fastjet::JetDefinition(plugins.back()));
#endif

  // set up a siscone jet definition
#ifdef ENABLE_PLUGIN_SISCONE
  int npass = 0;               // do infinite number of passes
  double protojet_ptmin = 0.0; // use all protojets
  plugins.push_back(new fastjet::SISConePlugin (jet_radius, overlap_threshold, 
                                                npass, protojet_ptmin));
  jet_defs.push_back(fastjet::JetDefinition(plugins.back()));
#endif

  // set up a d0runiicone jet definition
#ifdef ENABLE_PLUGIN_D0RUNIICONE
  double min_jet_Et = 6.0; // earlier D0 analyses used 8 GeV
  plugins.push_back(new fastjet::D0RunIIConePlugin (jet_radius, min_jet_Et, 
                                              overlap_threshold));
  jet_defs.push_back(fastjet::JetDefinition(plugins.back()));
#endif // ENABLE_PLUGIN_D0RUNIICONE

  // set up a trackjet
#ifdef ENABLE_PLUGIN_TRACKJET
  plugins.push_back(new fastjet::TrackJetPlugin(jet_radius));
  jet_defs.push_back(fastjet::JetDefinition(plugins.back()));
#endif // ENABLE_PLUGIN_TRACKJET
  // set up a atlascone
#ifdef ENABLE_PLUGIN_ATLASCONE
  plugins.push_back(new fastjet::ATLASConePlugin(jet_radius));
  jet_defs.push_back(fastjet::JetDefinition(plugins.back()));
#endif // ENABLE_PLUGIN_ATLASCONE
  // set up a eecambridge
#ifdef ENABLE_PLUGIN_EECAMBRIDGE
  double ycut = 0.08;
  plugins.push_back(new fastjet::EECambridgePlugin(ycut));
  jet_defs.push_back(fastjet::JetDefinition(plugins.back()));
#endif // ENABLE_PLUGIN_EECAMBRIDGE
  // set up a jade
#ifdef ENABLE_PLUGIN_JADE
  plugins.push_back(new fastjet::JadePlugin());
  jet_defs.push_back(fastjet::JetDefinition(plugins.back()));
#endif // ENABLE_PLUGIN_JADE
  // end of the plugins instantiation (don't modify this line)

  // set up kt and cam/aachen definitions
  jet_defs.push_back(fastjet::JetDefinition(fastjet::kt_algorithm, jet_radius));
  jet_defs.push_back(fastjet::JetDefinition(fastjet::cambridge_algorithm, 
                                            jet_radius));
  jet_defs.push_back(fastjet::JetDefinition(fastjet::antikt_algorithm, 
                                            jet_radius));

  // call the example jet-finding routine with each of jet definitions
  for (vector<fastjet::JetDefinition>::const_iterator jd_it = jet_defs.begin();
       jd_it != jet_defs.end(); jd_it++) {
    run_jet_finder(input_particles, *jd_it);
  }

  // clean up plugin memory.
  for (vector<fastjet::JetDefinition>::const_iterator jd_it = jet_defs.begin();
       jd_it != jet_defs.end(); jd_it++) {
    if (jd_it->plugin() != NULL) delete jd_it->plugin();
  }
}

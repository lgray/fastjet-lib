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
// example program to illustrate the use of the pxcone plugin
//
// Compile it with: make pxcone_example
// run it with    : ./pxcone_example < data/single-event.dat
//
// When compiling it make sure that you've modified ../../Makefile
// appropriately to get the relevant fortran libraries
//
//----------------------------------------------------------------------

#include "run_jet_finder.hh"
#include "PxConePlugin.hh"

int main(int argc, char** argv) {

  
  // define a plugin point
  fastjet::JetDefinition::Plugin * plugin;

  // allocate a new plugin
  double cone_radius = 0.7;
  double min_jet_energy = 5.0;
  double overlap_threshold = 0.5;
  bool   E_scheme_jets = false;
  plugin = new fastjet::PxConePlugin (cone_radius, min_jet_energy, 
                                      overlap_threshold, E_scheme_jets);

  // create a jet-definition based on the plugin
  fastjet::JetDefinition jet_def(plugin);

  // call the example routine that runs with arbitrary jet definitions
  run_jet_finder(jet_def);

  // clean up plugin memory.
  delete plugin;
}

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

#ifndef __FASTJET_JETDEFINITION_HH__
#define __FASTJET_JETDEFINITION_HH__

#include<cassert>
#include "fastjet/internal/numconsts.hh"
#include<string>

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

//======================================================================
/// the various options for the algorithmic strategy to adopt in
/// clustering events with kt and cambridge style algorithms.
enum Strategy {
  /// experimental ...
  N2MinHeapTiled   = -4, 
  /// fastest from about 50..10^4
  N2Tiled     = -3, 
  /// legacy
  N2PoorTiled = -2, 
  /// fastest below 50
  N2Plain     = -1, 
  /// worse even than the usual N^3 algorithms
  N3Dumb      =  0, 
  /// automatic selection of the best (based on N)
  Best        =  1, 
  /// best of the NlnN variants -- best overall for N>10^4
  NlnN        =  2, 
  /// legacy N ln N using 3pi coverage of cylinder
  NlnN3pi     =  3, 
  /// legacy N ln N using 4pi coverage of cylinder
  NlnN4pi     =  4,
  /// Chan's closest pair method (in a variant with 4pi coverage),
  /// for use exclusively with the Cambridge algorithm
  NlnNCam4pi   = 14,
  NlnNCam2pi2R = 13,
  NlnNCam      = 12, // 2piMultD
  /// the plugin has been used...
  plugin_strategy = 999
};


//======================================================================
/// the various families of jet-clustering algorithm
enum JetFinder {
  /// the longitudinally invariant kt algorithm
  kt_algorithm=0,
  /// the longitudinally invariant variant of the cambridge algorithm
  /// (aka Aachen algoithm).
  cambridge_algorithm=1,
  /// any plugin algorithm supplied by the user
  plugin_algorithm = 99
};


//======================================================================
/// class that is intended to hold a full definition of the jet
/// clusterer
class JetDefinition {
  
public:

  /// forward declaration of a class that allows the user to introduce
  /// their own plugin 
  class Plugin;

  /// constructor that to fully specify a jet-definition (together
  /// with information about how algorithically to run it). 
  ///
  /// [at some point might recombination schemes be added here?]
  JetDefinition(JetFinder jet_finder = kt_algorithm, 
		  double R = 1.0, 
		  Strategy strategy = Best) :
    _jet_finder(jet_finder), _Rparam(R), _strategy(strategy) {
    // the largest sensible value for R
    assert(_Rparam <= 0.5*pi);
    assert(_jet_finder != plugin_algorithm &&
	   _strategy   != plugin_strategy);
    _plugin = 0;
  };

  /// constructor based on a pointer to a user's plugin; the object
  /// pointed to must remain valid for the whole duration of existence
  /// of the JetDefinition and any related ClusterSequences
  JetDefinition(const Plugin * plugin) {
    _plugin = plugin;
    _strategy = plugin_strategy;
    _Rparam = -1.0;
    _jet_finder = plugin_algorithm;
  };

  /// return a pointer to the plugin 
  const Plugin * plugin() const {return _plugin;};

  // return information about the definition...
  JetFinder jet_finder  () const {return _jet_finder  ;}; 
  double      R           () const {return _Rparam      ;};
  Strategy  strategy    () const {return _strategy    ;};

  /// return a textual description of the current jet definition 
  std::string description() const;


private:
  JetFinder _jet_finder;
  double      _Rparam    ;
  Strategy  _strategy  ;

  const Plugin * _plugin;
};


// forward declaration, needed in order to specify interface for the
// plugin.
class ClusterSequence;


//======================================================================
/// a class that allows a user to introduce their own "plugin" jet
/// finder
class JetDefinition::Plugin{
public:
  /// return a textual description of the jet-definition implemented
  /// in this plugin
  virtual std::string description() const = 0;

  /// given a ClusterSequence that has been filled up with initial
  /// particles, the following function should fill up the rest of the
  /// ClusterSequence, using the following member functions of
  /// ClusterSequence:
  ///   - plugin_do_ij_recombination()
  ///   - plugin_do_iB_recombination()
  virtual void run_clustering(ClusterSequence &) const = 0;

  /// a destructor to be overloaded if need be...
  virtual ~Plugin() {};
};

FASTJET_END_NAMESPACE

#endif // __FASTJET_JETDEFINITION_HH__

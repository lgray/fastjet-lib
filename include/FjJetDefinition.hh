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

#ifndef __FJJETDEFINITION_H_
#define __FJJETDEFINITION_H_


//======================================================================
/// the various options for the algorithmic strategy to adopt in
/// clustering events with kt and cambridge style algorithms.
enum FjStrategy {
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
  NlnN4pi     =  4 
};


//======================================================================
/// the various families of jet-clustering algorithm
enum FjJetFinder {
  /// the longitudinally invariant kt algorithm
  kt_algorithm=0,
  /// the longitudinally invariant variant of the cambridge algorithm
  /// (aka Aachen algoithm).
  cambridge_algorithm=1
};


//======================================================================
/// class that is intended to hold a full definition of the jet
/// clusterer
class FjJetDefinition {
  
  
private:
  FjJetFinder _jet_finder;
  double      _Rparam    ;
  FjStrategy  _strategy  ;

public:

  /// constructor that to fully specify a jet-definition (together
  /// with information about how algorithically to run it). 
  ///
  /// [at some point might recombination schemes be added here?]
  FjJetDefinition(FjJetFinder jet_finder = kt_algorithm, 
		  double R = 1.0, 
		  FjStrategy strategy = Best) :
    _jet_finder(jet_finder), _Rparam(R), _strategy(strategy) {
};


  // return information about the definition...
  FjJetFinder jet_finder  () const {return _jet_finder  ;}; 
  double      R           () const {return _Rparam      ;};
  FjStrategy  strategy    () const {return _strategy    ;};

};

#endif // __FJJETDEFINITION_H_

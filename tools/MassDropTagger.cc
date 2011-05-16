//STARTHEADER
// $Id$
//
// Copyright (c) 2005-2011, Matteo Cacciari, Gavin Salam and Gregory Soyez
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

#include <fastjet/tools/MassDropTagger.hh>
#include <sstream>

FASTJET_BEGIN_NAMESPACE

using namespace std;

//----------------------------------------------------------------------
// class MassDropTagger
// Class that helps perform 2-pronged boosted ftagging using
// the "mass-drop" technique
//
// <FULL DESCRIPTION TO BE ADDED>
//
// Options
// 
// The constructor has the following arguments:
//  - The first argument is the jet definition to be used to
//    recluster the constituents of the jet to be filtered (in the
//    rest frame of the tagged jet).
//  - The second argument is the cut on tau_2 [0.08 by default]
//
// Input conditions
// 
//  - the original jet must have constituents
//
// Output/interface
// 
//  - a copy of the original jet is kept
//  - the 2 subjets are kept as pieces if some substructure is found,
//    otherwise a single 0-momentum piece
//  - the 'mu' and 'y' values corresponding to the unclustering step
//    that passed the tagger's cuts

// description of the tagger
string MassDropTagger::description() const{ 
  ostringstream oss;
  oss << "MassDropTagger with mu=" << _mu << " and ycut=" << _ycut;
  return oss.str();
}

// the tagging itself
//  - jet   the PseudoJet to tag
PseudoJet MassDropTagger::apply(const PseudoJet & jet) const{
  PseudoJet j = jet;
  PseudoJet j1, j2;
  bool had_parents;

  while ((had_parents = j.has_parents(j1,j2))) {
    // make parent1 the more massive jet
    if (j1.m() < j2.m()) std::swap(j1,j2);

    // if we pass the conditions on the mass drop and its degree of
    // asymmetry (z/(1-z) \sim kt_dist/m^2 > rtycut), then we've found
    // something interesting, so exit the loop
    if ( (j1.m() < _mu*j.m()) && (j1.kt_distance(j2) > _ycut*j.m2()) )
      break;
    else
      j = j1;
  }
    
  if (!had_parents){
    // no Higgs found, return a merged jet with a single piece of mom 0
    return join<MassDropStructure>(PseudoJet(0.0,0.0,0.0,0.0));
  }

  // apply the filter
  PseudoJet result = join<MassDropStructure>(j1,j2);
  result.extra_properties<MassDropTagger>()._mu = (j.m()!=0.0) ? j1.m()/j.m() : 0.0;
  result.extra_properties<MassDropTagger>()._y  = (j1.m2()!=0.0) ? j1.kt_distance(j2)/j.m2() : 0.0;
  return result;
}

FASTJET_END_NAMESPACE


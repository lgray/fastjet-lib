//FJSTARTHEADER
// $Id$
//
// Copyright (c) 2005-2014, Matteo Cacciari, Gavin P. Salam and Gregory Soyez
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
//  development. They are described in the original FastJet paper,
//  hep-ph/0512210 and in the manual, arXiv:1111.6097. If you use
//  FastJet as part of work towards a scientific publication, please
//  quote the version you use and include a citation to the manual and
//  optionally also to hep-ph/0512210.
//
//  FastJet is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with FastJet. If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------
//FJENDHEADER

#include "fastjet/tools/Filter.hh"
#include "fastjet/tools/Recluster.hh"
#include <fastjet/ClusterSequenceActiveAreaExplicitGhosts.hh>
#include <cassert>
#include <algorithm>
#include <sstream>
#include <typeinfo>

using namespace std;


FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

//----------------------------------------------------------------------
// Filter class implementation
//----------------------------------------------------------------------

// class description
string Filter::description() const {
  if (!_initialised){
    return "uninitialised Filter";
  }

  ostringstream ostr;
  ostr << "Filter with subjet_def = ";
  if (_Rfiltfunc) {
    ostr << "Cambridge/Aachen algorithm with dynamic Rfilt"
         << " (recomb. scheme deduced from jet, or E-scheme if not unique)";
  } else if (_Rfilt > 0) {
    ostr << "Cambridge/Aachen algorithm with Rfilt = " 
         << _Rfilt 
         << " (recomb. scheme deduced from jet, or E-scheme if not unique)";
  } else {
    ostr << _subjet_def.description();
  }
  ostr<< ", selection " << _selector.description();
  if (_subtractor) {
    ostr << ", subtractor: " << _subtractor->description();
  } else if (_rho != 0) {
    ostr << ", subtracting with rho = " << _rho;
  }
  return ostr.str();
}


// core functions
//----------------------------------------------------------------------

// return a vector of subjets, which are the ones that would be kept
// by the filtering
PseudoJet Filter::result(const PseudoJet &jet) const {
  if (!_initialised){
    //Q: do we throw or do we return an empty PJ?
    throw Error("uninitialised Filter");
  }

  // start by getting the list of subjets (including a list of sanity
  // checks)
  // NB: subjets is empty to begin with (see the comment for
  //     _set_filtered_elements_cafilt)
  vector<PseudoJet> subjets; 
  JetDefinition subjet_def;
  _set_filtered_elements(jet, subjets);

  // now build the vector of kept and rejected subjets
  vector<PseudoJet> kept, rejected;
  // Note that in the following line we make a copy of the _selector
  // to avoid issues with needing a mutable _selector
  Selector selector_copy = _selector;
  if (selector_copy.takes_reference()) selector_copy.set_reference(jet);
  selector_copy.sift(subjets, kept, rejected);

  // gather the info under the form of a PseudoJet
  return _finalise(jet, kept, rejected);
}


// sets filtered_elements to be all the subjets on which filtering will work
void Filter::_set_filtered_elements(const PseudoJet & jet,
                                    vector<PseudoJet> & filtered_elements) const {
  // recluster the jet
  PseudoJet reclustered_jet = ((_Rfilt>=0) || (_Rfiltfunc))
    ? Recluster(cambridge_algorithm, (_Rfiltfunc) ? (*_Rfiltfunc)(jet) : _Rfilt, false)(jet)
    : Recluster(_subjet_def, false)(jet);

  // order the filtered elements in pt
  filtered_elements = sorted_by_pt(reclustered_jet.pieces());
}

// gather the information about what is kept and rejected under the
// form of a PseudoJet with a special ClusterSequenceInfo
PseudoJet Filter::_finalise(const PseudoJet & /*jet*/, 
                            vector<PseudoJet> & kept, 
                            vector<PseudoJet> & rejected) const {
  assert(kept.size()+rejected.size()>0);
  // figure out which recombiner to use
  const JetDefinition::Recombiner &rec = (kept.size()>0)
    ? *(kept[0].associated_cs()->jet_def().recombiner())
    : *(rejected[0].associated_cs()->jet_def().recombiner());

  // create an appropriate structure and transfer the info to it
  PseudoJet filtered_jet = join<StructureType>(kept, rec);
  StructureType *fs = (StructureType*) filtered_jet.structure_non_const_ptr();
  fs->_rejected = rejected;
  
  return filtered_jet;
}


FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

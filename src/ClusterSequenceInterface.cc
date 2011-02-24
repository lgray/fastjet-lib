//STARTHEADER
// $Id$
//
// Copyright (c) 2005-2010, Matteo Cacciari, Gavin Salam and Gregory Soyez
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

#include "fastjet/ClusterSequenceInterface.hh"
#include "fastjet/Error.hh"
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceAreaBase.hh"
#include <iostream>

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

using namespace std;


//----------------------------------------------------------------------
// Direct access to the associated ClusterSequence object.
//----------------------------------------------------------------------

// check whether this PseudoJet has an associated parent
// ClusterSequence
bool ClusterSequenceInterface::has_associated_cluster_sequence() const{
  return (_associated_cs != NULL);
}

// get a (const) pointer to the associated ClusterSequence (NULL if
// inexistent)
const ClusterSequence* ClusterSequenceInterface::associated_cluster_sequence() const{
  if (! has_associated_cluster_sequence()) return NULL;

  return _associated_cs;
}


// If there is a valid cluster sequence associated with this jet,
// returns a pointer to it; otherwise throws an Error.
//
// Open question: should these errors be upgraded to classes of their
// own so that they can be caught? [Maybe, but later]
const ClusterSequence * ClusterSequenceInterface::validated_cs() const {
  if (!_associated_cs) 
    throw Error("you requested information about the internal structure of a jet, but it is not associated with a ClusterSequence or its associated ClusterSequence has gone out of scope.");
  return _associated_cs;
}


//----------------------------------------------------------------------
// Methods for access to information about jet structure
//----------------------------------------------------------------------

// check if it has been recombined with another PseudoJet in which
// case, return its partner through the argument. Otherwise,
// 'partner' is set to 0.
//
// false is also returned if this PseudoJet has no associated
// ClusterSequence
bool ClusterSequenceInterface::has_partner(const PseudoJet &reference, PseudoJet &partner) const{
  return validated_cs()->has_partner(reference, partner);
}

// check if it has been recombined with another PseudoJet in which
// case, return its child through the argument. Otherwise, 'child'
// is set to 0.
// 
// false is also returned if this PseudoJet has no associated
// ClusterSequence, with the child set to 0
bool ClusterSequenceInterface::has_child(const PseudoJet &reference, PseudoJet &child) const{
  return validated_cs()->has_child(reference, child);
}

// check if it is the product of a recombination, in which case
// return the 2 parents through the 'parent1' and 'parent2'
// arguments. Otherwise, set these to 0.
//
// false is also returned if this PseudoJet has no parent
// ClusterSequence
bool ClusterSequenceInterface::has_parents(const PseudoJet &reference, PseudoJet &parent1, PseudoJet &parent2) const{
  return validated_cs()->has_parents(reference, parent1, parent2);
}

// check if the current PseudoJet contains the one passed as
// argument
//
// false is also returned if this PseudoJet has no associated
// ClusterSequence.
bool ClusterSequenceInterface::contains(const PseudoJet &reference, const PseudoJet &constituent) const{
  if ((!has_associated_cluster_sequence()) || (!constituent.has_associated_cluster_sequence()))
    throw Error("you requested information about the internal structure of a jet, but it is not associated with a ClusterSequence or its associated ClusterSequence has gone out of scope."); 

  if (associated_cluster_sequence() != constituent.associated_cluster_sequence()) return false;

  return validated_cs()->object_in_jet(constituent, reference);
}

// check if the current PseudoJet is contained the one passed as
// argument
//
// false is also returned if this PseudoJet has no associated
// ClusterSequence
bool ClusterSequenceInterface::is_inside(const PseudoJet &reference, const PseudoJet &jet) const{
  if ((!has_associated_cluster_sequence()) || (!jet.has_associated_cluster_sequence()))
    throw Error("you requested information about the internal structure of a jet, but it is not associated with a ClusterSequence or its associated ClusterSequence has gone out of scope."); 

  if (associated_cluster_sequence() != jet.associated_cluster_sequence()) return false;

  return validated_cs()->object_in_jet(reference, jet);
}


// return true if the interface supports constituents. 
//
// an Error is thrown if this PseudoJet has no currently valid
// associated ClusterSequence
bool ClusterSequenceInterface::has_constituents() const{
  if (!has_associated_cluster_sequence())
    throw Error("you requested information about the internal structure of a jet, but it is not associated with a ClusterSequence or its associated ClusterSequence has gone out of scope."); 

  return true;
}


// retrieve the constituents. An empty vector is returned if there is
// no associated ClusterSequence
vector<PseudoJet> ClusterSequenceInterface::constituents(const PseudoJet &reference) const{
  return validated_cs()->constituents(reference);
}

// return true if the interface supports exclusive_subjets. 
//
// an Error is thrown if this PseudoJet has no currently valid
// associated ClusterSequence
bool ClusterSequenceInterface::has_exclusive_subjets() const{
  if (!has_associated_cluster_sequence())
    throw Error("you requested information about the internal structure of a jet, but it is not associated with a ClusterSequence or its associated ClusterSequence has gone out of scope."); 

  return true;
}

// return a vector of all subjets of the current jet (in the sense
// of the exclusive algorithm) that would be obtained when running
// the algorithm with the given dcut. 
//
// Time taken is O(m ln m), where m is the number of subjets that
// are found. If m gets to be of order of the total number of
// constituents in the jet, this could be substantially slower than
// just getting that list of constituents.
//
// an Error is thrown if this PseudoJet has no currently valid
// associated ClusterSequence
std::vector<PseudoJet> ClusterSequenceInterface::exclusive_subjets (const PseudoJet &reference, const double & dcut) const {
  return validated_cs()->exclusive_subjets(reference, dcut);
}

// return the size of exclusive_subjets(...); still n ln n with same
// coefficient, but marginally more efficient than manually taking
// exclusive_subjets.size()
//
// an Error is thrown if this PseudoJet has no currently valid
// associated ClusterSequence
int ClusterSequenceInterface::n_exclusive_subjets(const PseudoJet &reference, const double & dcut) const {
  return validated_cs()->n_exclusive_subjets(reference, dcut);
}

// return the list of subjets obtained by unclustering the supplied
// jet down to n subjets (or all constituents if there are fewer
// than n).
//
// requires n ln n time
//
// an Error is thrown if this PseudoJet has no currently valid
// associated ClusterSequence
std::vector<PseudoJet> ClusterSequenceInterface::exclusive_subjets (const PseudoJet &reference, int nsub) const {
  return validated_cs()->exclusive_subjets(reference, nsub);
}

// return the dij that was present in the merging nsub+1 -> nsub 
// subjets inside this jet.
//
// an Error is thrown if this PseudoJet has no currently valid
// associated ClusterSequence
double ClusterSequenceInterface::exclusive_subdmerge(const PseudoJet &reference, int nsub) const {
  return validated_cs()->exclusive_subdmerge(reference, nsub);
}

// return the maximum dij that occurred in the whole event at the
// stage that the nsub+1 -> nsub merge of subjets occurred inside 
// this jet.
//
// an Error is thrown if this PseudoJet has no currently valid
// associated ClusterSequence
double ClusterSequenceInterface::exclusive_subdmerge_max(const PseudoJet &reference, int nsub) const {
  return validated_cs()->exclusive_subdmerge_max(reference, nsub);
}


//----------------------------------------------------------------------
// the following ones require a computation of the area in the
// associated ClusterSequence (See ClusterSequenceAreaBase for details)
//----------------------------------------------------------------------

// if possible, return a valid ClusterSequenceAreaBase pointer; otherwise
// throw an error
const ClusterSequenceAreaBase * ClusterSequenceInterface::validated_csab() const {
  const ClusterSequenceAreaBase *csab = dynamic_cast<const ClusterSequenceAreaBase*>(validated_cs());
  if (csab == NULL) throw Error("you requested jet-area related information, but the PseudoJet does not have associated area information.");
  return csab;
}


// check if it has a defined area
bool ClusterSequenceInterface::has_area() const{
  if (! has_associated_cluster_sequence()) return false;
  return (dynamic_cast<const ClusterSequenceAreaBase*>(_associated_cs) != NULL);
}

// return the jet (scalar) area.
// throw an Error if there is no support for area in the associated CS
double ClusterSequenceInterface::area(const PseudoJet &reference) const{
  return validated_csab()->area(reference);
}

// return the error (uncertainty) associated with the determination
// of the area of this jet.
// throws an Error if there is no support for area in the associated CS
double ClusterSequenceInterface::area_error(const PseudoJet &reference) const{
  return validated_csab()->area_error(reference);
}

// return the jet 4-vector area
// throws an Error if there is no support for area in the associated CS
PseudoJet ClusterSequenceInterface::area_4vector(const PseudoJet &reference) const{
  return validated_csab()->area_4vector(reference);
}

// true if this jet is made exclusively of ghosts
// throws an Error if there is no support for area in the associated CS
bool ClusterSequenceInterface::is_pure_ghost(const PseudoJet &reference) const{
  return validated_csab()->is_pure_ghost(reference);
}



FASTJET_END_NAMESPACE

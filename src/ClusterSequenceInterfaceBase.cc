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


#include "fastjet/ClusterSequenceInterfaceBase.hh"
#include "fastjet/Error.hh"
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceAreaBase.hh"

using namespace std;

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

// ClusterSequenceInterfaceBase implementation
//
// Contains any information related to the clustering that should be
// directly accessible to PseudoJet.
//
// By default, this class implements basic access to the
// ClusterSequence related to a PseudoJet (like its constituents or
// its area). But it can be overloaded in order e.g. to give access
// to the jet substructure.
//
// Note that it accesses the underlying ClusterSequence through a
// ClusterSequenceWrapper object so it can check when the former goes
// out of scope.
//


//-------------------------------------------------------------
// Direct access to the associated ClusterSequence object.
//
// Get access to the associated ClusterSequence (if any)
//-------------------------------------------------------------

// get a (const) pointer to the parent ClusterSequence (NULL if
// inexistent)
const ClusterSequence* ClusterSequenceInterfaceBase::associated_cluster_sequence() const{
  throw Error("This ClusterSequenceInterface does not have an associated cluster sequence");
}
  
// if the jet has a valid associated cluster sequence then return a
// pointer to it; otherwise throw an error
const ClusterSequence * ClusterSequenceInterfaceBase::validated_cs() const{
  throw Error("This ClusterSequenceInterface does not have a validated cluster sequence");
}

// if the jet has valid area information then return a pointer to
// the associated ClusterSequenceAreaBase object; otherwise throw an error
const ClusterSequenceAreaBase * ClusterSequenceInterfaceBase::validated_csab() const{
  throw Error("This ClusterSequenceInterface does not have a validated cluster sequence with area");
}


//-------------------------------------------------------------
// Methods for access to information about jet structure
//
// These allow access to jet constituents, and other jet
// subtructure information. They only work if the jet is associated
// with a ClusterSequence.
//-------------------------------------------------------------

// check if it has been recombined with another PseudoJet in which
// case, return its partner through the argument. Otherwise,
// 'partner' is set to 0.
//
// an Error is thrown if this PseudoJet has no currently valid
// associated ClusterSequence
bool ClusterSequenceInterfaceBase::has_partner(const PseudoJet &reference, PseudoJet &partner) const{
  throw Error("This ClusterSequenceInterface has no implementation for has_partner");
}

// check if it has been recombined with another PseudoJet in which
// case, return its child through the argument. Otherwise, 'child'
// is set to 0.
// 
// an Error is thrown if this PseudoJet has no currently valid
// associated ClusterSequence
bool ClusterSequenceInterfaceBase::has_child(const PseudoJet &reference, PseudoJet &child) const{
  throw Error("This ClusterSequenceInterface has no implementation for has_child");
}

// check if it is the product of a recombination, in which case
// return the 2 parents through the 'parent1' and 'parent2'
// arguments. Otherwise, set these to 0.
//
// an Error is thrown if this PseudoJet has no currently valid
// associated ClusterSequence
bool ClusterSequenceInterfaceBase::has_parents(const PseudoJet &reference, PseudoJet &parent1, PseudoJet &parent2) const{
  throw Error("This ClusterSequenceInterface has no implementation for has_parents");
}

// check if the current PseudoJet contains the one passed as
// argument.
//
// an Error is thrown if this PseudoJet has no currently valid
// associated ClusterSequence
bool ClusterSequenceInterfaceBase::contains(const PseudoJet &reference, const PseudoJet &constituent) const{
  throw Error("This ClusterSequenceInterface has no implementation for contains");
}

// check if the current PseudoJet is contained the one passed as
// argument.
//
// an Error is thrown if this PseudoJet has no currently valid
// associated ClusterSequence
bool ClusterSequenceInterfaceBase::is_inside(const PseudoJet &reference, const PseudoJet &jet) const{
  throw Error("This ClusterSequenceInterface has no implementation for is_inside");
}

// retrieve the constituents. 
//
// an Error is thrown if this PseudoJet has no currently valid
// associated ClusterSequence
vector<PseudoJet> ClusterSequenceInterfaceBase::constituents(const PseudoJet &reference) const{
  throw Error("This ClusterSequenceInterface has no implementation for constituents");
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
vector<PseudoJet> ClusterSequenceInterfaceBase::exclusive_subjets (const PseudoJet &reference, const double & dcut) const{
  throw Error("This ClusterSequenceInterface has no implementation for exclusive_subjets");
}

// return the size of exclusive_subjets(...); still n ln n with same
// coefficient, but marginally more efficient than manually taking
// exclusive_subjets.size()
//
// an Error is thrown if this PseudoJet has no currently valid
// associated ClusterSequence
int ClusterSequenceInterfaceBase::n_exclusive_subjets(const PseudoJet &reference, const double & dcut) const{
  throw Error("This ClusterSequenceInterface has no implementation for n_exclusive_subjets");
}

// return the list of subjets obtained by unclustering the supplied
// jet down to n subjets (or all constituents if there are fewer
// than n).
//
// requires n ln n time
//
// an Error is thrown if this PseudoJet has no currently valid
// associated ClusterSequence
vector<PseudoJet> ClusterSequenceInterfaceBase::exclusive_subjets (const PseudoJet &reference, int nsub) const{
  throw Error("This ClusterSequenceInterface has no implementation for exclusive_subjets");
}

// return the dij that was present in the merging nsub+1 -> nsub 
// subjets inside this jet.
//
// an Error is thrown if this PseudoJet has no currently valid
// associated ClusterSequence
double ClusterSequenceInterfaceBase::exclusive_subdmerge(const PseudoJet &reference, int nsub) const{
  throw Error("This ClusterSequenceInterface has no implementation for exclusive_submerge");
}

// return the maximum dij that occurred in the whole event at the
// stage that the nsub+1 -> nsub merge of subjets occurred inside 
// this jet.
//
// an Error is thrown if this PseudoJet has no currently valid
// associated ClusterSequence
double ClusterSequenceInterfaceBase::exclusive_subdmerge_max(const PseudoJet &reference, int nsub) const{
  throw Error("This ClusterSequenceInterface has no implementation for exclusive_submerge_max");
}


// retrieve the pieces building the jet. 
// By default, a jet is made of a single piece: itself
//------------------------------------------------------------------
std::vector<PseudoJet> ClusterSequenceInterfaceBase::pieces(const PseudoJet &reference) const{
  return vector<PseudoJet>(1, reference);
}

// the following ones require a computation of the area in the
// parent ClusterSequence (See ClusterSequenceAreaBase for details)
//------------------------------------------------------------------

// return the jet (scalar) area.
// throws an Error if there is no support for area in the parent CS
double ClusterSequenceInterfaceBase::area(const PseudoJet &reference) const{
  throw Error("This ClusterSequenceInterface has no implementation for area");
}

// return the error (uncertainty) associated with the determination
// of the area of this jet.
// throws an Error if there is no support for area in the parent CS
double ClusterSequenceInterfaceBase::area_error(const PseudoJet &reference) const{
  throw Error("This ClusterSequenceInterface has no implementation for area_error");
}

// return the jet 4-vector area.
// throws an Error if there is no support for area in the parent CS
PseudoJet ClusterSequenceInterfaceBase::area_4vector(const PseudoJet &reference) const{
  throw Error("This ClusterSequenceInterface has no implementation for area_4vector");
}

// true if this jet is made exclusively of ghosts.
// throws an Error if there is no support for area in the parent CS
bool ClusterSequenceInterfaceBase::is_pure_ghost(const PseudoJet &reference) const{
  throw Error("This ClusterSequenceInterface has no implementation for is_pure_ghost");
}

FASTJET_END_NAMESPACE

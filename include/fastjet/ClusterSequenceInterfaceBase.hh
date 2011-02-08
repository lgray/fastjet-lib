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


#ifndef __FASTJET_CLUSTER_SEQUENCE_INTERFACE_BASE_HH__
#define __FASTJET_CLUSTER_SEQUENCE_INTERFACE_BASE_HH__

#include "fastjet/internal/base.hh"

#include <vector>

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

class PseudoJet;
class ClusterSequence;
class ClusterSequenceAreaBase;

/// @ingroup extra_info
/// \class ClusterSequenceInterfaceBase
///
/// Contains any information related to the clustering that should be
/// directly accessible to PseudoJet.
///
/// By default, this class implements basic access to the
/// ClusterSequence related to a PseudoJet (like its constituents or
/// its area). But it can be overloaded in order e.g. to give access
/// to the jet substructure.
///
/// Note that the pointer to the underlying ClusterSequence has to be
/// set to NULL (by the ClusterSequence itself) when it goes out of
/// scope.
///
class ClusterSequenceInterfaceBase{
public:
  /// default ctor
  ClusterSequenceInterfaceBase(){};

  /// default (virtual) dtor
  virtual ~ClusterSequenceInterfaceBase(){};

  //-------------------------------------------------------------
  /// @name Direct access to the associated ClusterSequence object.
  ///
  /// Get access to the associated ClusterSequence (if any)
  //\{
  //-------------------------------------------------------------
  /// returns true if there is a valid associated ClusterSequence
  virtual bool has_associated_cluster_sequence() const { return false;};

  /// get a (const) pointer to the parent ClusterSequence (NULL if
  /// inexistent)
  virtual const ClusterSequence* associated_cluster_sequence() const;
  
  /// if the jet has a valid associated cluster sequence then return a
  /// pointer to it; otherwise throw an error
  virtual const ClusterSequence * validated_cs() const;

  /// if the jet has valid area information then return a pointer to
  /// the associated ClusterSequenceAreaBase object; otherwise throw an error
  virtual const ClusterSequenceAreaBase * validated_csab() const;

  //\}

  //-------------------------------------------------------------
  /// @name Methods for access to information about jet structure
  ///
  /// These allow access to jet constituents, and other jet
  /// subtructure information. They only work if the jet is associated
  /// with a ClusterSequence.
  //-------------------------------------------------------------
  //\{

  /// check if it has been recombined with another PseudoJet in which
  /// case, return its partner through the argument. Otherwise,
  /// 'partner' is set to 0.
  ///
  /// an Error is thrown if this PseudoJet has no currently valid
  /// associated ClusterSequence
  virtual bool has_partner(const PseudoJet &reference, PseudoJet &partner) const;

  /// check if it has been recombined with another PseudoJet in which
  /// case, return its child through the argument. Otherwise, 'child'
  /// is set to 0.
  /// 
  /// an Error is thrown if this PseudoJet has no currently valid
  /// associated ClusterSequence
  virtual bool has_child(const PseudoJet &reference, PseudoJet &child) const;

  /// check if it is the product of a recombination, in which case
  /// return the 2 parents through the 'parent1' and 'parent2'
  /// arguments. Otherwise, set these to 0.
  ///
  /// an Error is thrown if this PseudoJet has no currently valid
  /// associated ClusterSequence
  virtual bool has_parents(const PseudoJet &reference, PseudoJet &parent1, PseudoJet &parent2) const;

  /// check if the current PseudoJet contains the one passed as
  /// argument.
  ///
  /// an Error is thrown if this PseudoJet has no currently valid
  /// associated ClusterSequence
  virtual bool contains(const PseudoJet &reference, const PseudoJet &constituent) const;

  /// check if the current PseudoJet is contained the one passed as
  /// argument.
  ///
  /// an Error is thrown if this PseudoJet has no currently valid
  /// associated ClusterSequence
  virtual bool is_inside(const PseudoJet &reference, const PseudoJet &jet) const;

  /// retrieve the constituents. 
  ///
  /// an Error is thrown if this PseudoJet has no currently valid
  /// associated ClusterSequence
  virtual std::vector<PseudoJet> constituents(const PseudoJet &reference) const;

  /// return a vector of all subjets of the current jet (in the sense
  /// of the exclusive algorithm) that would be obtained when running
  /// the algorithm with the given dcut. 
  ///
  /// Time taken is O(m ln m), where m is the number of subjets that
  /// are found. If m gets to be of order of the total number of
  /// constituents in the jet, this could be substantially slower than
  /// just getting that list of constituents.
  ///
  /// an Error is thrown if this PseudoJet has no currently valid
  /// associated ClusterSequence
  virtual std::vector<PseudoJet> exclusive_subjets (const PseudoJet &reference, const double & dcut) const;

  /// return the size of exclusive_subjets(...); still n ln n with same
  /// coefficient, but marginally more efficient than manually taking
  /// exclusive_subjets.size()
  ///
  /// an Error is thrown if this PseudoJet has no currently valid
  /// associated ClusterSequence
  virtual int n_exclusive_subjets(const PseudoJet &reference, const double & dcut) const;

  /// return the list of subjets obtained by unclustering the supplied
  /// jet down to n subjets (or all constituents if there are fewer
  /// than n).
  ///
  /// requires n ln n time
  ///
  /// an Error is thrown if this PseudoJet has no currently valid
  /// associated ClusterSequence
  virtual std::vector<PseudoJet> exclusive_subjets (const PseudoJet &reference, int nsub) const;

  /// return the dij that was present in the merging nsub+1 -> nsub 
  /// subjets inside this jet.
  ///
  /// an Error is thrown if this PseudoJet has no currently valid
  /// associated ClusterSequence
  virtual double exclusive_subdmerge(const PseudoJet &reference, int nsub) const;

  /// return the maximum dij that occurred in the whole event at the
  /// stage that the nsub+1 -> nsub merge of subjets occurred inside 
  /// this jet.
  ///
  /// an Error is thrown if this PseudoJet has no currently valid
  /// associated ClusterSequence
  virtual double exclusive_subdmerge_max(const PseudoJet &reference, int nsub) const;


  // the following ones require a computation of the area in the
  // parent ClusterSequence (See ClusterSequenceAreaBase for details)
  //------------------------------------------------------------------

  /// check if it has a defined area
  virtual bool has_area() const {return false;};

  /// return the jet (scalar) area.
  /// throws an Error if there is no support for area in the parent CS
  virtual double area(const PseudoJet &reference) const;

  /// return the error (uncertainty) associated with the determination
  /// of the area of this jet.
  /// throws an Error if there is no support for area in the parent CS
  virtual double area_error(const PseudoJet &reference) const;

  /// return the jet 4-vector area.
  /// throws an Error if there is no support for area in the parent CS
  virtual PseudoJet area_4vector(const PseudoJet &reference) const;

  /// true if this jet is made exclusively of ghosts.
  /// throws an Error if there is no support for area in the parent CS
  virtual bool is_pure_ghost(const PseudoJet &reference) const;

  //\} --- end of jet structure -------------------------------------
};

FASTJET_END_NAMESPACE

#endif  //  __FASTJET_CLUSTER_SEQUENCE_INTERFACE_BASE_HH__

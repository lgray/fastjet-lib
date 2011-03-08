#ifndef __FASTJET_TOOLS_FILTER_HH__
#define __FASTJET_TOOLS_FILTER_HH__

//STARTHEADER
// $Id$
//
// Copyright (c) 2009-2010, Matteo Cacciari, Gavin Salam and Gregory Soyez
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

#include <fastjet/ClusterSequence.hh>
#include <fastjet/Selector.hh>
#include <fastjet/MergedJetInterface.hh>    // to derive the FilteredJet interface from MergedJetInterface
#include "Transformer.hh" // to derive Filter from Transformer
#include <iostream>
#include <string>

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

// fwd declarations
class Filter;
class FilteredJetInterface;

//----------------------------------------------------------------------
/// \class Filter
/// class that helps perform filtering on jets, and optionally
/// subtraction (if rho > 0).
///
/// Description
/// -----------
/// Though the original version was applied on Cambridge/Aachen jets,
/// this one takes any jet (that has constituents) and reclusters it
/// with a given algorithm. Only subjets passing a given condition are
/// kept, the others are thrown.
///
/// Options
/// -------
/// The constructor has the following arguments:
///  - The first argument is the jet definition to be used to
///    recluster the constituents of the jet to be filtered.
///  - The second argument is a Selector specifying the condition for
///    a subjet to be kept.
///  - As an optional 3rd argument, one can pass a value of rho (the
///    estimated background per unit area) in which case, every subjet
///    is subtracted before the selection condition is applied.
///
/// Input conditions
/// ----------------
///  - the original jet must have constituents
///  - if rho>0, the jet must be the result of a Clustering with
///    active area with explicit ghosts support or a merging of
///    such pieces
///
/// Output/interface
/// ----------------
///  - 
///  - a copy of the original jet is kept
///  - 
///
/// Credits
/// -------
/// Filtering with just "nfilt" was originally proposed for
/// boosted-object reconstruction in arXiv:0802.2470, and for normal
/// kinematic reconstruction (e.g. dijet mass peaks) in
/// arXiv:0810.1304.
///
/// Filtering with just "ptkeep" was proposed in arXiv:0912.1342 under
/// the name "trimming", though there "ptkeep" is expressed as a fraction
/// of a hard scale in the problem rather than in absolute terms.
///
/// The selection of the jet definition used for the sub-clustering
/// goes as follows:
///  - if the filter is created using a given jet definition, that one is used
///  - if the filter is created specifying only Rfilt, the jet
///    definition associated with the first jet is used
class Filter : public Transformer{
public:
  /// trivial ctor
  /// Note: this is just for derived classes
  ///       a Filter initialised through this constructor will not work!
  Filter(){};

  /// define a filter that decomposes a jet into subjets using a
  /// generic JetDefinition and then keeps only a subset of these
  /// subjets according to a Selector. Optionally, each subjet may be
  /// internally bakground-subtracted prior to selection.
  ///
  ///  \param subjet_def   the jet definition applied to obtain the subjets
  ///  \param selector     the Selector applied to compute the kept subjets
  ///  \param rho          if non-zero, backgruond-subtract each subjet befor selection
  ///
  /// Note: internal subtraction only applies on jets that are
  /// obtained with a cluster sequence with area support and explicit
  /// ghosts
  Filter(JetDefinition subjet_def, Selector selector, double rho = 0.0) : 
    _subjet_def(subjet_def), _selector(selector), _rho(rho) {}

  /// default dtor
  virtual ~Filter(){};

  /// runs the filtering and sets kept and rejected to be the jets of interest
  /// (with non-zero rho, they will have been subtracted).
  ///
  /// \param jet    the jet that gets filtered
  /// \return the filtered jet
  virtual PseudoJet operator()(const PseudoJet & jet) const;

  /// action of the transformer on each jet from the vector 
  /// this has to be repeated because it shares the same name as the operator()(PseudoJet)
  virtual std::vector<PseudoJet> operator()(const std::vector<PseudoJet> & originals) const{
    return Transformer::operator()(originals);
  }

  /// class description
  std::string description() const;

  typedef FilteredJetInterface InterfaceType;

protected:
  /// sets filtered_elements to be all the subjets on which filtering will work
  /// [NB: this routine is work in progress as part of a transition to a Filter
  ///      that also works on jet collections]
  void _set_filtered_elements(const PseudoJet & jet,
			      std::vector<PseudoJet> & filtered_elements,
			      ClusterSequence * &internal_cs) const;
  
  /// gather the information about what is kept and rejected under the
  /// form of a PseudoJet with a special ClusterSequenceInfo
  PseudoJet _finalise(const PseudoJet & jet, 
		      std::vector<PseudoJet> & kept, 
		      std::vector<PseudoJet> & rejected, 
		      ClusterSequence * &internal_cs) const;

  /// check if the jet is obtained from C/A or a superposition of C/A pieces
  bool _recursively_check_ca(const PseudoJet & jet) const;

  /// set the filtered elements in the simple case of C/A+C/A
  void _set_filtered_elements_cafilt(
    const PseudoJet & jet,
    std::vector<PseudoJet> & filtered_elements,
    double Rfilt) const;

  /// set the filtered elements in the generic re-clustering case (wo subtraction)
  ClusterSequence* _set_filtered_elements_generic_unsubtracted(
    const PseudoJet & jet, 
    std::vector<PseudoJet> & filtered_elements) const;

  /// set the filtered elements in the generic re-clustering case (with subtraction)
  ClusterSequence* _set_filtered_elements_generic_subtracted(
    const PseudoJet & jet, 
    std::vector<PseudoJet> & filtered_elements) const;

  JetDefinition _subjet_def; ///< the jet definition to use to extract the subjets
  Selector _selector;        ///< the subjet selection criterium
  double _rho;               ///< the background density (used for subtraction when possible)
};



//----------------------------------------------------------------------
/// \class FilteredJetInterface
/// Class to contain interface information for a filtered jet.
class FilteredJetInterface : public MergedJetInterface {
public:
  /// constructor from an original ClusterSequenceInfo
  /// We just share the original ClusterSequenceWrapper and initialise
  /// the rest
  FilteredJetInterface(){}

  /// virtual dtor to allow further overloading  
  virtual ~FilteredJetInterface(){}

  /// description
  virtual std::string description() const { return "Filtered PseudoJet"; }

  //------------------------------------------------------------------
  /// @name The filter-specific information
  //------------------------------------------------------------------

  /// returns the original jet (the first of the original jets
  /// if you filtered a collection of jets)
  const PseudoJet & original() const {return _original_jet;}

  /// returns the subjets that were not kept during the filtering procedure
  /// (subtracted if the filter requests it, and valid in the original cs)
  const std::vector<PseudoJet> & rejected() const {return _rejected;}

  friend class Filter;  // allow the filter to change the protected/private members

protected:
  PseudoJet _original_jet;
  std::vector<PseudoJet> _rejected;

  SharedPtr<ClusterSequence> _internal_cs;  //< for a generic filter (we use a shared-ptr to avoid worrying about memory management)
};


FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

#endif   // __FASTJET_TOOLS_FILTER_HH__

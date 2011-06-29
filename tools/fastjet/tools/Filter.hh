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

#ifndef __FASTJET_TOOLS_FILTER_HH__
#define __FASTJET_TOOLS_FILTER_HH__

#include <fastjet/ClusterSequence.hh>
#include <fastjet/Selector.hh>
#include <fastjet/CompositeJetStructure.hh> // to derive the FilterStructure from CompositeJetStructure
#include <fastjet/tools/Transformer.hh>     // to derive Filter from Transformer
#include <iostream>
#include <string>

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

// fwd declarations
class Filter;
class FilterStructure;

//----------------------------------------------------------------------
/// @ingroup tools
/// \class Filter
/// Class that helps perform filtering/trimming on jets, and optionally
/// subtraction (if rho > 0).
///
/// Though the original version was applied on Cambridge/Aachen jets,
/// this one takes any jet (that has constituents) and reclusters it
/// with a given algorithm. A user-provided Selector is applied to
/// decide which of the subjets are kept to produce the filtered jet
/// (others are discarded).
///
///
/// \section desc Options
/// 
/// The constructor has the following arguments:
///  - The first argument is the jet definition to be used to
///    recluster the constituents of the jet to be filtered.
///  - The second argument is a Selector specifying the condition for
///    a subjet to be kept. If the selector takes a reference, the jet
///    being filtered is used.
///  - As an optional 3rd argument, one can pass a value of rho (the
///    estimated background per unit area) in which case, every subjet
///    is subtracted before the selection condition is applied.
///
///
/// \section input Input conditions
/// 
///  - the original jet must have constituents
///  - if rho>0, the jet must be the result of a Clustering with
///    active area with explicit ghosts support or a merging of
///    such pieces
///
/// \section output Output/structure
/// 
///  - a copy of the original jet is kept
///  - kept pieces are stored under the form of a "CompositeJet"
///  - rejected pieces are also stored in the structure
///
/// \section usage Usage Examples
/// 
/// Filtering as proposed in arXiv:0802.2470 for boosted object
/// reconstruction (and used also in arXiv:0810.1304 for dijet
/// reconstructions) involves two parameters, the filtering radius,
/// Rfilt, and the number of subjets you wish to keep, nfilt. To get a
/// filter of this kind define
///
///    Filter filter(JetDefinition(cambridge_algorithm,Rfilt),
///                  SelectorNHardest(nfilt));
///
/// You apply it as follows
///
///    PseudoJet filtered_jet = filter(jet);
///
/// To get trimming defined with respect to a jet's pt,
/// arXiv:0912.1342, you need an Rtrim to define subjets and a
/// pt_fraction_min to decide which subjets to keep:
///
///    Filter trimmer(JetDefinition(cambridge_algorithm,Rfilt),
///                   SelectorPtFractionMin(pt_fraction_min));
///
/// You then apply it as before
///
///    PseudoJet trimmed_jet = trimmer(jet);
///
/// You can then find out which pieces were filtered or trimmed jet is
/// made of by calling
/// 
///    trimmed_jet.pieces()
///
/// Trimming defined with respect to an event's effective mass can
/// be carried out with a SelectorPtMin(...) selector.
///
/// More sophisticated filters/trimmers can easily be obtained by
/// combining Selectors.
///
/// [MORE INFO, E.G. ON PIECES REJECTED, SHOULD FOLLOW]
///
///
/// \section impl Implementation
/// 
/// If the jet was defined with the cambridge/aachen algorithm (or is
/// made of pieces each of which comes from the C/A alg) and the
/// filtering definition is C/A, then the filter does not rerun the
/// C/A algorithm on the constituents, but instead makes use of the
/// existent C/A cluster sequence in the original jet.
///
/// See also \subpage Example12  for a usage example.
class Filter : public Transformer{
public:
  /// trivial ctor
  /// Note: this is just for derived classes
  ///       a Filter initialised through this constructor will not work!
  Filter() : _Rfiltfunc(0){};

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
    _subjet_def(subjet_def), _Rfiltfunc(0), _Rfilt(-1), _selector(selector), _rho(rho) {}

  /// Same as the full constructor (see above) but just specifying the radius
  /// By default, Cambridge-Aachen is used
  /// If the jet (or all its pieces) is obtained with a non-default
  /// recombiner, that one will be used
  ///  \param Rfilt   the filtering radius
  Filter(double Rfilt, Selector selector, double rho = 0.0) : 
    _Rfiltfunc(0), _Rfilt(Rfilt), _selector(selector), _rho(rho) { 
    if (_Rfilt<0)
      throw Error("Attempt to create a Filter with a negative filtering radius");
  }

  /// Same as the full constructor (see above) but just specifying a
  /// filtering radius that will depend on the jet being filtered
  /// As for the previous case, Cambridge-Aachen is used
  /// If the jet (or all its pieces) is obtained with a non-default
  /// recombiner, that one will be used
  ///  \param Rfilt_func   the filtering radius function of a PseudoJet
  Filter(FunctionOfPseudoJet<double> *Rfilt_func, Selector selector, double rho = 0.0) : 
    _Rfiltfunc(Rfilt_func), _Rfilt(-1), _selector(selector), _rho(rho) {}

  /// default dtor
  virtual ~Filter(){};

  /// runs the filtering and sets kept and rejected to be the jets of interest
  /// (with non-zero rho, they will have been subtracted).
  ///
  /// \param jet    the jet that gets filtered
  /// \return the filtered jet
  virtual PseudoJet result(const PseudoJet & jet) const;

  /// class description
  virtual std::string description() const;

  typedef FilterStructure StructureType;

protected:
  /// sets filtered_elements to be all the subjets on which filtering will work
  /// [NB: this routine is work in progress as part of a transition to a Filter
  ///      that also works on jet collections]
  void _set_filtered_elements(const PseudoJet & jet,
			      std::vector<PseudoJet> & filtered_elements) const;
  
  /// set the filtered elements in the simple case of C/A+C/A
  void _set_filtered_elements_cafilt(const PseudoJet & jet,
				     std::vector<PseudoJet> & filtered_elements,
				     double Rfilt) const;

  /// set the filtered elements in the generic re-clustering case
  void _set_filtered_elements_generic(const PseudoJet & jet, 
				      std::vector<PseudoJet> & filtered_elements) const;

  /// gather the information about what is kept and rejected under the
  /// form of a PseudoJet with a special ClusterSequenceInfo
  PseudoJet _finalise(const PseudoJet & jet, 
		      std::vector<PseudoJet> & kept, 
		      std::vector<PseudoJet> & rejected) const;

  // a series of checks
  //--------------------------------------------------------------------
  /// get the pieces down to the fundamental pieces
  bool _get_all_pieces(const PseudoJet &jet, std::vector<PseudoJet> &all_pieces) const;

  /// get the common recombiner to all pieces (NULL if none)
  const JetDefinition::Recombiner* _get_common_recombiner() const;

  /// check if one can apply the simplified trick for C/A subjets
  bool _check_ca() const;

  /// check if the jet (or all its pieces) have explicit ghosts
  /// (assuming the jet has area support
  ///
  /// Note that if the jet has an associated cluster sequence that is no
  /// longer valid, an error will be thrown
  bool _check_explicit_ghosts() const;

  mutable JetDefinition _subjet_def; 
                               ///< the jet definition to use to extract the subjets
  FunctionOfPseudoJet<double> *_Rfiltfunc; 
                               ///< a dynamic filtering radius function of the jet being filtered
  double _Rfilt;               ///< a constant specifying the subjet radius (with C/A)
  mutable Selector _selector;  ///< the subjet selection criterium
  double _rho;                 ///< the background density (used for subtraction when possible)

  // internal useful variables
  mutable std::vector<PseudoJet> all_pieces;
};



//----------------------------------------------------------------------
/// @ingroup tools
/// \class FilterStructure
/// Class to contain structure information for a filtered jet.
class FilterStructure : public CompositeJetStructure {
public:
  /// constructor from an original ClusterSequenceInfo
  /// We just share the original ClusterSequenceWrapper and initialise
  /// the rest
  FilterStructure(const std::vector<PseudoJet> & pieces, 
		  const JetDefinition::Recombiner *rec = 0)
    : CompositeJetStructure(pieces, rec){}

  /// virtual dtor to allow further overloading  
  virtual ~FilterStructure(){}

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
};


FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

#endif   // __FASTJET_TOOLS_FILTER_HH__

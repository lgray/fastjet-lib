#ifndef __FASTJET_TOOLS_RECLUSTER_HH__
#define __FASTJET_TOOLS_RECLUSTER_HH__

// $Id$
//
// Copyright (c) 2014, Matteo Cacciari, Gavin P. Salam and Gregory Soyez
//
//----------------------------------------------------------------------
// This file is part of FastJet
//
// It is free software; you can redistribute it and/or modify it under
// the terms of the GNU General Public License as published by the
// Free Software Foundation; either version 2 of the License, or (at
// your option) any later version.
//
// It is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
// or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public
// License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this code. If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------

#include <fastjet/JetDefinition.hh>
#include <fastjet/FunctionOfPseudoJet.hh>   // to derive Recluster from FOfPJ<PJ>
#include <iostream>
#include <string>

// TODO:
//
//  - maintain Voronoi areas? Requires CSAB:has_voronoi_area()    {->UNASSIGNED}
//  - make sure the description of the class is OK



FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

//----------------------------------------------------------------------
/// \class Recluster
/// Recluster a jet's constituents with a new jet definition.
///
/// The result of the reclustering is returned as a single PseudoJet
/// with a CompositeJet structure. The pieces of that PseudoJet will
/// be the individual subjets
///
/// When constructed from a JetDefinition, that definition will be
/// used to obtain the subjets.  When constructed from a JetAlgorithm
/// and parameters (0 parameters for e+e-, just R or R and an extra
/// parameter for others) the recombination scheme will be taken as
/// the same one used to initially cluster the original jet.
///
/// The result of the reclustering depends on its usage. There are two
/// typical use-cases: either we recluster one fat jet into subjets,
/// OR, we recluster the jet with a different jet alg. When Recluster
/// is created from a full jet definition. The last parameter of the
/// constructors below dicatate that behaviour: if "single" is true
/// (the default), a single jet, issued from a regular clustering is
/// returned (if there are more than one, the hardest is taken);
/// otherwise (single==false), the result will be a composite jet with
/// each subjet as pieces. [Note that since the structure of the
/// resulting PseudoJet depends on its usage, this class inherits from
/// FunctionOfPseudoJet<PseudoJet (including a description) rather
/// than being a full-fledged Transformer]
///
class Recluster : public FunctionOfPseudoJet<PseudoJet> {
public:
  /// the various options for the output of Recluster
  enum Keep{
    /// keep only the hardest subjet and return a "standard" jet with
    /// an associated ClusterSequence [this will be the default]
    keep_only_hardest,
    /// keep all the subjets. result() will join them into a composite
    /// jet
    keep_all
  };

  /// dummy ctor (uses an undefined JetDefinition)
  Recluster() : _subjet_def(), _acquire_recombiner(true),
                _keep(keep_only_hardest), _cambridge_optimisation_enabled(true){}

  /// define a recluster that decomposes a jet into subjets using a
  /// generic JetDefinition
  ///
  ///  \param subjet_def    the jet definition applied to obtain the subjets
  ///  \param acquire_recombiner
  ///                       when true, the reclustering will guess the
  ///                       recombiner from the input jet instead of
  ///                       the one in subjet_def. An error is thrown
  ///                       if none is found
  ///  \param keep_in       Recluster::keep_only_hardest: the result is
  ///                       the hardest subjet after reclustering,
  ///                       returned as a "standard" jet.
  ///                       Recluster::keep_all: the result is a
  ///                       composite jet with subjets as pieces.
  Recluster(const JetDefinition & subjet_def, 
            bool acquire_recombiner_in = true, 
            Keep keep_in = keep_only_hardest)
    : _subjet_def(subjet_def), _acquire_recombiner(acquire_recombiner_in), 
      _keep(keep_in), _cambridge_optimisation_enabled(true) {}

  /// define a recluster that decomposes a jet into subjets using a
  /// JetAlgorithm and its parameters
  ///
  ///  \param subjet_alg    the jet algorithm applied to obtain the subjets
  ///  \param subjet_radius the jet radius if required
  ///  \param keep_in       Recluster::keep_only_hardest: the result is
  ///                       the hardest subjet after reclustering,
  ///                       returned as a "standard" jet.
  ///                       Recluster::keep_all: the result is a
  ///                       composite jet with subjets as pieces.
  /// 
  /// This ctor will always acquire the recombiner from the jet being
  /// reclustered (it will throw if none can be found). For the
  /// version where the radius is not provided (the second below),
  /// max_allowable_R will be used if the algorithm requires a jet
  /// radius. If you wish to use Recluster with an algorithm that
  /// requires an extra parameter (like the genkt algorithm), please
  /// specify the jet definition fully using the constructor above.
  Recluster(JetAlgorithm subjet_alg, double subjet_radius, Keep keep_in = keep_only_hardest);
  Recluster(JetAlgorithm subjet_alg, Keep keep_in = keep_only_hardest);

  /// default dtor
  virtual ~Recluster(){}

  //----------------------------------------------------------------------
  // tweaking the behaviour

  /// do not try to optimise reclustering with Cambridge/Aachen algorithms
  void disable_cambridge_optimisation(bool disabled=true){ _cambridge_optimisation_enabled = !disabled;}
  void disable_cambridge_optimization(bool disabled=true){ _cambridge_optimisation_enabled = !disabled;}

  //----------------------------------------------------------------------
  // retrieving info about the behaviour

  /// class description
  virtual std::string description() const;

  /// returns true if this reclusterer is set to acquire the
  /// recombiner from the input jets
  bool acquire_recombiner() const{ return _acquire_recombiner;}

  /// returns the current "keep" mode i.e. whether only the hardest
  /// subjet is returned or all of them (see Keep above)
  Keep keep() const{ return _keep;}


  //----------------------------------------------------------------------
  // core action of ths class

  /// runs the reclustering and sets kept and rejected to be the jets of interest
  /// (with non-zero rho, they will have been subtracted).
  ///
  /// \param jet    the jet that gets reclustered
  /// \return the reclustered jet
  virtual PseudoJet result(const PseudoJet & jet) const;

  /// a lower-level method that does the actual work of reclustering
  /// the input jet. The resulting subjets are stored in output_jets
  /// and the jet definition that has been used is stored in
  /// output_jet_def
  ///
  /// \param input_jet       the (input) jet that one wants to recluster
  /// \param output_jets     subjets resulting from the new clustering
  /// \param output_jet_def  the jet def that has been used to obtain output_jets
  ///
  /// returns true if the ca soptimisation has been used (this means
  /// that generate_output_jet will watch out for non-explicit-ghost
  /// areas that might be leftover)
  bool get_new_jets_and_def(const PseudoJet & input_jet, 
                            std::vector<PseudoJet> & output_jets, 
                            JetDefinition & output_jet_def) const;

  /// given a set of subjets and a jet definition used, create the
  /// resulting PseudoJet
  PseudoJet generate_output_jet(std::vector<PseudoJet> & subjets, 
                                JetDefinition & jet_def_used,
                                bool ca_optimisation_used) const;


private:
  /// set the reclustered elements in the simple case of C/A+C/A
  void _recluster_ca(const std::vector<PseudoJet> & all_pieces,
                     std::vector<PseudoJet> & subjets,
                     double Rfilt) const;

  /// set the reclustered elements in the generic re-clustering case
  void _recluster_generic(const PseudoJet & jet, 
                          std::vector<PseudoJet> & subjets,
                          const JetDefinition & subjet_def,
                          bool do_areas) const;
  
  // a series of checks
  //--------------------------------------------------------------------
  /// get the pieces down to the fundamental pieces
  bool _get_all_pieces(const PseudoJet &jet, std::vector<PseudoJet> &all_pieces) const;

  /// associate the proper recombiner taken from the underlying pieces
  /// (an error is thrown if the pieces do no share a common
  /// recombiner)
  void _acquire_recombiner_from_pieces(const std::vector<PseudoJet> &all_pieces, 
                                       JetDefinition &subjet_def) const;

  /// check if one can apply the simplified trick for C/A subjets
  bool _check_ca(const std::vector<PseudoJet> &all_pieces, 
                 const JetDefinition &subjet_def) const;

  /// check if the jet (or all its pieces) have explicit ghosts
  /// (assuming the jet has area support
  ///
  /// Note that if the jet has an associated cluster sequence that is no
  /// longer valid, an error will be thrown
  bool _check_explicit_ghosts(const std::vector<PseudoJet> &all_pieces) const;

  JetDefinition _subjet_def;   ///< the jet definition to use to extract the subjets
  bool _acquire_recombiner;    ///< get the recombiner from the input
                               ///< jet rather than from subjet_def
  Keep _keep;                  ///< dicates what subjets are kept and
                               ///< what is returned (see Keep above)

  bool _cambridge_optimisation_enabled; ///<enable the checks to
                                        ///< perform optimisation when
                                        ///< C/A reclustering is asked

  static LimitedWarning   _explicit_ghost_warning;
};

FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

#endif   // __FASTJET_TOOLS_RECLUSTER_HH__

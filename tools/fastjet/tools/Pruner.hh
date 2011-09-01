#ifndef __FASTJET_TOOLS_PRUNER_HH__
#define __FASTJET_TOOLS_PRUNER_HH__

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

#include "fastjet/ClusterSequence.hh"
#include "fastjet/tools/Transformer.hh"
#include <iostream>
#include <string>

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

// fwd declarations
class Pruner;
class PruningRecombiner;
class PruningPlugin;

//----------------------------------------------------------------------
/// @ingroup tools_generic
/// \class Pruner
/// transformer that prunes a jet
///
/// This transformer prunes a jet according to the ideas presented in 
/// arXiv:0903.5081 (S.D. Ellis, C.K. Vermilion and J.R. Walsh). 
/// 
/// Briefly, the jet's constituents are reclustered using a
/// user-specified jet definition. During the clustering, objects i
/// and j are only recombined if one of the following two criteria is
/// satisfied:
///  - the geometric distance between i and j is smaller than 'Rcut'
///  - the transverse momenta of i and j are at least 'zcut' p_t(i+j)
/// If both these criteria fail, the hardest of i and j is kept and
/// the softest rejected. The provider 'recombiner' is used when the
/// test is passed.
///
/// The jet definition passed to the constructor specifies which
/// algorithm and recombination scheme have to be used to recluster
/// the jet constituents. The radius of that internal clustering is
/// internally overwritten to a large value to ensure that a single
/// jet is found by the internal clustering (as a precaution, an error
/// is thrown if this is not the case)
///
/// 
//----------------------------------------------------------------------
class Pruner : public Transformer{
public:
  /// ctor
  ///  \param jet_def the jet definition for the internal clustering
  ///  \param zcut    pt-fraction cut in the pruning
  ///  \param Rcut    angular distance cut in the pruning
  Pruner(const JetDefinition &jet_def, double zcut, double Rcut)
    : _jet_def(jet_def), _zcut(zcut), _Rcut(Rcut) {}

  /// action on a single jet
  virtual PseudoJet result(const PseudoJet &jet) const;

  /// transformer description
  virtual std::string description() const;

  /// the result has the structure of a jet in the internal
  /// ClusterSequence
  typedef StructureType ClusterSequenceStructure;

private:
  JetDefinition _jet_def; ///< the internal jet definition (only the 
                          ///< algorithm and the recombiner< are used)
  double _zcut;		  ///< the pt-fraction cut
  double _Rcut;           ///< the angular distance cut
};

//----------------------------------------------------------------------
/// \if internal_doc
/// @ingroup internal
/// \class PruningRecombiner
/// recombines the objects that are not vetoed by pruning
///
/// This recombiner only recombine objects (i and j) that pass one of
/// the following two criteria:
///
///  - the geometric distance between i and j is smaller than 'Rcut'
///  - the transverse momenta of i and j are at least 'zcut' p_t(i+j)
///
/// If both these criteria fail, the hardest of i and j is kept and
/// the softest rejected. The provider 'recombiner' is used when the
/// test is passed.
///
/// Note that this in not meant for standalone use [in particular
/// because it could lead to memory issues due to the rejected indices
/// stored internally].
///
/// \endif
class PruningRecombiner : public JetDefinition::Recombiner{
public:
  /// ctor
  ///  \param zcut   transverse momentum fraction cut
  ///  \param Rcut   separation cut
  ///  \param recomb pointer to a recombiner to use to cluster pairs
  ///  \param kept   pointer to a vector that will remember what is
  ///                kept and rejected (it MUST be large enough and
  ///                initialised to true)
  PruningRecombiner(double zcut, double Rcut, 
		    const JetDefinition::Recombiner *recombiner)
    : _zcut2(zcut*zcut), _Rcut2(Rcut*Rcut), 
      _recombiner(recombiner){}

  /// decide whether to recombine things or not
  virtual void recombine(const PseudoJet &pa, 
			 const PseudoJet &pb,
			 PseudoJet &pab) const;

  virtual std::string description() const;

  const std::vector<unsigned int> & rejected() const{ return _rejected;}

private:
  double _zcut2;  ///< transverse momentum fraction cut 
  double _Rcut2;  ///< separation cut
  const JetDefinition::Recombiner *_recombiner; ///< the underlying recombiner to use
  mutable std::vector<unsigned int> _rejected;  ///< list of rejected history indices
};


//----------------------------------------------------------------------
/// \if internal_doc
/// @ingroup internal
/// \class PruningPlugin
/// FastJet internal plugin that clusters the particles using the
/// PruningRecombiner. 
///
/// See PruningRecombiner for a description of what pruning does.
///
/// Note that this is an internal FastJet class used by the Pruner
/// transformer and not meant to be used as a standalone clustering
/// tool.
///
/// \endif
//----------------------------------------------------------------------
class PruningPlugin : public JetDefinition::Plugin{
public:
  /// ctor
  ///  \param jet_def the jet definition to be used for the 
  ///                 internal clustering
  ///  \param zcut    transverse momentum fraction cut
  ///  \param Rcut    separation cut
  PruningPlugin(const JetDefinition &jet_def, double zcut, double Rcut)
    : _jet_def(jet_def), _zcut(zcut), _Rcut(Rcut){}

  /// the actual clustering work for the plugin
  virtual void run_clustering(ClusterSequence &input_cs) const;

  /// description of the plugin
  virtual std::string description() const;

  /// returns the radius
  virtual double R() const {return _jet_def.R();}

private:
  /// recursively marks history element i and its parents as rejected
  void _recursively_mark_as_rejected(const unsigned int i,
       const std::vector<ClusterSequence::history_element> & hist, 
       std::vector<bool> & kept) const;

  JetDefinition _jet_def; ///< the internal jet definition
  double _zcut;  ///< transverse momentum fraction cut 
  double _Rcut;  ///< separation cut
};



FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

#endif   // __FASTJET_TOOLS_PRUNER_HH__

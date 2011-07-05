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

#ifndef __FASTJET_MASS_DROP_TAGGER_HH__
#define __FASTJET_MASS_DROP_TAGGER_HH__

#include <fastjet/tools/Transformer.hh>
#include <fastjet/internal/LimitedWarning.hh>
#include <fastjet/CompositeJetStructure.hh>

FASTJET_BEGIN_NAMESPACE

class MassDropTagger;
class MassDropStructure;

//----------------------------------------------------------------------
/// @ingroup tools
/// \class MassDropTagger
/// Class that helps perform 2-pronged boosted tagging using
/// the "mass-drop" technique
///
/// This implements the boosted Higgs tagger introduced by Jonathan
/// Butterworth, Adam Davison, Mathieu Rubin and Gavin Salam in
/// arXiv:0802.2470.
///
/// The tagger proceeds as follows:
///
///  0. start from a jet obtained from with the Cambridge/Aachen
///     algorithm
///
///  1. undo the last step of the clustering step j -> j1 + j2 (label
///     them such as j1 is the most massive).
///  
///  2. if there is a mass drop, i.e. m_j1/m_j < mu_cut, and the
///     splitting is sufficiently symmetric, \f${/rm
///     min}(p_{tj1}^2,p_{tj2}^2)\Delta R_{j1,j2}^2 > y_{/rm cut}
///     m_j^2\f$, keep j as the result of the tagger (with j1 and j2
///     its 2 subjets)
///
///  3. otherwise, redefine j to be equal to j1 and return to step 1.
///
/// Note that in the original proposal, j1 and j2 were both required
/// to be b-tagged and a filter (with Rfilt=min(0.3,Rbb/2) and
/// n_filt=3) was applied to j to obtain the final "Higgs candidate".
/// This filtering technique can always be done by defining a
/// FunctionOfPseudoJet to compute dynamically Rfilt and then use the
/// Filter
///   RfiltDyn rfilt;
///   Filter final_filter(&rfilt, SelectorNHardest(3));
/// See the filter example to see explicitly how this can be done.
/// 
///
/// \section desc Options
/// 
/// The constructor has the following arguments:
///  - The first argument is minimal mass drop required (mu_cut) [0.67
///    by default]
///  - The second argument is asymmetry cut (y_cut) [0.09 by default]
///
/// \section input Input conditions
/// 
///  - one must be able to succesively "uncluster" the original jet
///    using "has_parents"
///
/// \section output Output/structure
/// 
///  - the 2 subjets are kept as pieces if some substructure is found,
///    otherwise a single 0-momentum piece is returned
///  - the 'mu' and 'y' values corresponding to the unclustering step
///    that passed the tagger's cuts
///
class MassDropTagger : public Transformer{
public:
  /// default ctor
  MassDropTagger(const double mu=0.67, const double ycut=0.09) : _mu(mu), _ycut(ycut){};

  /// description of the tagger
  virtual std::string description() const;

  /// the tagging itself
  ///  \param jet   the PseudoJet to tag
  virtual PseudoJet result(const PseudoJet & jet) const;

  /// the type of the associated structure
  typedef MassDropStructure StructureType;

protected:
  double _mu, _ycut;
  static LimitedWarning _warnings_nonca;
};


//------------------------------------------------------------------------
/// @ingroup tools
/// \class MassDropStructure
/// the structure returned by the MassDropTagger transformer.
///
/// See the MassDropTagger class description for the details of what
/// is inside this structure
///
class MassDropStructure : public CompositeJetStructure{
public:
  /// ctor with initialisation
  ///  \param pieces  the pieces of the created jet
  ///  \param rec     the recombiner from the underlying cluster sequence
  MassDropStructure(const std::vector<PseudoJet> & pieces, 
		    const JetDefinition::Recombiner *recombiner = 0) :
    CompositeJetStructure(pieces, recombiner), _mu(0.0), _y(0.0){}

  /// the mass-drop ratio, pieces[0].m()/jet.m(), for the splitting
  /// that triggered the mass-drop condition
  inline double mu() const{return _mu;}

  /// The value of y = (squared kt distance) / (squared mass) for the
  /// splitting that triggered the mass-drop condition
  inline double y() const {return _y;}

protected:
  double _mu;  ///< the value of the mass-drop parameter
  double _y;   ///< the value of the asymmetry parameter

  // allow the tagger to set these
  friend class MassDropTagger;
};



FASTJET_END_NAMESPACE

#endif  //  __FASTJET_MASS_DROP_TAGGER_HH__


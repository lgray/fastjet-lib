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
#include <fastjet/CompositeJetStructure.hh>

FASTJET_BEGIN_NAMESPACE

class MassDropTagger;
class MassDropStructure;

//----------------------------------------------------------------------
/// @ingroup tools
/// \class MassDropTagger
/// Class that helps perform 2-pronged boosted ftagging using
/// the "mass-drop" technique
///
/// <FULL DESCRIPTION TO BE ADDED>
///
/// \section desc Options
/// 
/// The constructor has the following arguments:
///  - The first argument is the jet definition to be used to
///    recluster the constituents of the jet to be filtered (in the
///    rest frame of the tagged jet).
///  - The second argument is the cut on tau_2 [0.08 by default]
///
/// \section input Input conditions
/// 
///  - the original jet must have constituents
///
/// \section output Output/interface
/// 
///  - a copy of the original jet is kept
///  - the 2 subjets are kept as pieces if some substructure is found,
///    otherwise a single 0-momentum piece
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
  virtual PseudoJet apply(const PseudoJet & jet) const;

  /// the type of the associated structure
  typedef MassDropStructure StructureType;

protected:
  double _mu, _ycut;
};


//------------------------------------------------------------------------
/// \class MassDropStructure
/// the structure returned by the MassDropTagger transformer.
///
/// See the MassDropTagger class description for the details of what
/// is inside this structure
///
class MassDropStructure : public CompositeJetStructure{
public:
  /// ctor with pieces initialisation
  MassDropStructure(std::vector<PseudoJet> pieces) :
    CompositeJetStructure(pieces), _mu(0.0), _y(0.0){}

  /// access to the associated N-subjettiness
  inline double mu() const{return _mu;}

  /// access to the associated angle with the boosted axis
  inline double y() const {return _y;}

protected:
  mutable double _mu;  ///< the value of the mass-drop parameter
  mutable double _y;   ///< the value of the asymmetry parameter

  // allow the tagger to set these
  friend class MassDropTagger;
};



FASTJET_END_NAMESPACE

#endif  //  __FASTJET_MASS_DROP_TAGGER_HH__


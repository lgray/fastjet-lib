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

#ifndef __FASTJET_NSUBJETTINESS_TAGGER_HH__
#define __FASTJET_NSUBJETTINESS_TAGGER_HH__

#include <fastjet/PseudoJet.hh>
#include <fastjet/JetDefinition.hh>
#include <fastjet/CompositeJetStructure.hh>
#include <fastjet/tools/Transformer.hh>

FASTJET_BEGIN_NAMESPACE

class NSubjettinessTagger;
class NSubjettinessStructure;

//----------------------------------------------------------------------
/// @ingroup tools
/// \class NSubjettinessTagger
/// Class that helps perform 2-pronged boosted tagging using
/// N-subjettiness
///
/// This is the implementation of the N-Subjettiness tagger introduced
/// by Ji-Hun Kim in arXiv:1011.1493.
///
/// To tag a fat jet, we proceed as follows:
///
///  - boost its constituents into the rest frame of the jet
///
///  - recluster them using another jet definition (the original
///    choice was SISCone in spherical coordinates with R=0.6 and
///    f=0.75.
///
///  - keep the 2 most energetic subjets (\f$q_{1,2}\f$) and compute
///    the 2-subjettiness
///    \f[
///      \tau_2^j = \frac{2}{m_{\rm jet}^2}\,
///                 \sum_{k\in {\rm jet}} {\rm min}(q_1.p_k,q_2.p_k)
///    \f]
///    where the sum runs over the constituents of the jet. 
///
///  - require \f$\tau_2^j < \tau_2^{\rm cut}\f$ [0.08 by default]
///
///  - impose that (in the rest frame of the fat jet), the angles
///    between the 2 most energetic subjets and the boost axis are
///    both large enough: \f$\cos(\theta_s)<c_\theta^{\rm cut}\f$ 
///    [0.8 by default]
///
/// Note that in the original version, the jets are reconstructed
/// using SISCone with R=0.8 and f=0.75. Also, b-tagging was imposed
/// on the 2 subjets found in the tagging procedure.
///
/// \section desc Options
/// 
/// The constructor has the following arguments:
///  - The first argument is the jet definition to be used to
///    recluster the constituents of the jet to be filtered (in the
///    rest frame of the tagged jet).
///  - The second argument is the cut on tau_2 [0.08 by default]
///  - The 3rd argument is the cut on cos(theta_s) [0.8 by default]
///  - If the 4th argument is true, 2 exclusive rest-frame jets will
///    be considered in place of the 2 most energetic inclusive jets
///
/// \section input Input conditions
/// 
///  - the original jet must have constituents
///
/// \section output Output/interface
/// 
///  - the 2 subjets are kept as pieces if some substructure is found,
///    otherwise a single 0-momentum piece
///  - the tau2 and maximal cos(theta_s) values computed during the
///    tagging
///
class NSubjettinessTagger : public Transformer{
public:
  /// ctor with arguments (see the class description above)
  NSubjettinessTagger(const JetDefinition subjet_def, 
		      const double tau2cut=0.08, 
		      const double costhetascut=0.8,
		      const bool use_exclusive = false)
    : _subjet_def(subjet_def), _t2cut(tau2cut), _costscut(costhetascut),
      _use_exclusive(use_exclusive){};

  /// tagger description
  virtual std::string description() const;

  /// action on a single jet
  virtual PseudoJet apply(const PseudoJet & jet) const;

  /// the type of Structure returned
  typedef NSubjettinessStructure StructureType;

protected:
  JetDefinition _subjet_def;
  double _t2cut, _costscut;
  bool _use_exclusive;
};


//------------------------------------------------------------------------
/// \class NSubjettinessStructure
/// the structure returned by the NSubjettinessTagger transformer.
///
/// See the NSubjettinessTagger class description for the details of
/// what is inside this structure
///
class NSubjettinessStructure : public CompositeJetStructure{
public:
  /// ctor with pieces initialisation
  NSubjettinessStructure(std::vector<PseudoJet> pieces) :
    CompositeJetStructure(pieces), _tau2(0.0), _costhetas(1.0){}

  /// access to the associated N-subjettiness
  inline double tau2() const{return _tau2;}

  /// access to the associated angle with the boosted axis
  inline double costhetas() const {return _costhetas;}

protected:
  mutable double _tau2;      ///< the value of the N-subjettiness
  mutable double _costhetas; ///< the minimal angle between the dijets
			     ///< and the boost axis

  // allow the tagger to set these
  friend class NSubjettinessTagger;
};

FASTJET_END_NAMESPACE
#endif  //  __FASTJET_NSUBJETTINESS_TAGGER_HH__


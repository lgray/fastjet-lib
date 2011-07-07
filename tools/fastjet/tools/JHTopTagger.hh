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

#ifndef __FASTJET_JH_TOP_TAGGER_HH__
#define __FASTJET_JH_TOP_TAGGER_HH__

#include <fastjet/tools/Transformer.hh>
#include <fastjet/CompositeJetStructure.hh>
#include <fastjet/internal/LimitedWarning.hh>

FASTJET_BEGIN_NAMESPACE

class JHTopTagger;
class JHTopStructure;

//----------------------------------------------------------------------
/// @ingroup tools
/// \class JHTopTagger
/// Class that helps perform boosted top tagging using the "Johns
/// Hopkins" method from arXiv:0806.0848 (Kaplan, Rehermann, Schwartz
/// and Tweedie)
///
/// <FULL DESCRIPTION TO BE ADDED>
///
/// Implementation details: 
///   - find the 3-pronged structure
///   - apply kinematic constraints if needed
///
///
/// \section desc Options
/// 
/// The constructor has the following arguments:
///  - The first argument is the minimal mass drop requested
///  - The second argument is the maximal mass of the subjets
///
/// \section input Input conditions
/// 
///  - the original jet must have an associated (and valid)
///    ClusterSequence
///
/// \section output Output/interface
/// 
///  - 
///
class JHTopTagger : public Transformer{
public:
  /// default ctor
  /// The parameters are the following:
  ///  \param delta_p          fractional pt cut imposed on the subjets
  ///                          (computed as a fraction of the original jet)
  ///  \param delta_r          minimal distance between 2 subjets
  //                           (computed as |y1-y2|+|phi1-phi2|)
  ///  \param cos_theta_W_max  the maximal value for the polarisation 
  ///                          angle of the W
  ///  \param mW               the W mass
  ///
  /// The default values of all these parameters are taken from
  /// arXiv:0806:0848
  JHTopTagger(const double delta_p=0.10, const double delta_r=0.19, 
	      double cos_theta_W_max=0.7, double mW=81.0)
    : _delta_p(delta_p), _delta_r(delta_r),
      _cos_theta_W_max(cos_theta_W_max), _mW(mW){};

  /// description of the tagger
  virtual std::string description() const;

  /// the tagging itself
  ///  \param jet   the PseudoJet to tag
  virtual PseudoJet result(const PseudoJet & jet) const;

  /// the type of the associated structure
  typedef JHTopStructure StructureType;

protected:
  /// runs the Johns Hopkins decomposition procedure
  std::vector<PseudoJet> _split_once(const PseudoJet & startjet) const;

  /// compute the W helicity angle
  double _cos_theta_W(const PseudoJet & result) const;

  mutable const PseudoJet * _jet;    ///< the jet being processed
  double _delta_p, _delta_r, _cos_theta_W_max, _mW;
  static LimitedWarning _warnings_nonca;
};


//------------------------------------------------------------------------
/// \class JHTopStructure
/// the structure returned by the JHTopTagger transformer.
///
/// See the JHTopTagger class description for the details of what
/// is inside this structure
///
class JHTopStructure : public CompositeJetStructure{
public:
  /// ctor with pieces initialisation
  JHTopStructure(std::vector<PseudoJet> pieces,
		 const JetDefinition::Recombiner *recombiner = 0) :
    CompositeJetStructure(pieces, recombiner), _cos_theta_w(0.0){}

  /// direct access to the W subjet
  inline PseudoJet W() const{ 
    return _W;
  }

  /// direct access to the first W subjet (the hardest)
  inline PseudoJet W1() const{
    assert(_pieces.size()>0);
    return _pieces[0];
  }

  /// direct access to the second W subjet
  inline PseudoJet W2() const{
    assert(_pieces.size()>1);
    return _pieces[1];
  }

  /// direct access to the non-W subjets
  /// it will have 1 or 2 pieces depending on whether the tagger has
  /// found 3 or 4 pieces
  inline PseudoJet non_W() const{ 
    return _non_W;
  }

  /// access to the associated N-subjettiness
  inline double cos_theta_W() const {return _cos_theta_w;}

protected:
  double _cos_theta_w;   ///< the W helicity angle
  PseudoJet _W;          ///< the tagged W
  PseudoJet _non_W;      ///< the remaining pieces

  // allow the tagger to set these
  friend class JHTopTagger;
};



FASTJET_END_NAMESPACE

#endif  //  __FASTJET_JADE_DISTANCE_TAGGER_HH__


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
class JHTopTaggerStructure;

//----------------------------------------------------------------------
/// @ingroup tools_taggers
/// \class JHTopTagger
/// Class that helps perform boosted top tagging using the "Johns
/// Hopkins" method from arXiv:0806.0848 (Kaplan, Rehermann, Schwartz
/// and Tweedie)
///
///The tagger proceeds as follows:
///  - start from a jet J obtained with the Cambridge/Aachen algorithm
///  - undo the last iteration j -> j_1,j_2 (with pt_1>pt_2) until the
///    two subjets satisfy pt_1 > delta_p pt_J (with pt_J the pt of
///    the original jet) and |y_1 - y_2| + |phi_1 - phi_2| > delta_r.
///  - if one of these criteria is not satisfied, carry on the
///    procedure with j_1 (discarding j_2)
///  - for each of the subjets found, repeat the procedure. If some
///    new substructure is found, keep these 2 new subjets, otherwise
///    keep the original subjet (found during the first iteration)
///  - at this stage, one has at most 4 subjets. If one has less than
///    3, the tagger has failed.
///  - reconstruct the W from the 2 subjets with a mass closest to the
///    W mass
///  - impose that the W helicity angle be less than a threshold
///    cos_theta_W_max.
///
///
/// \section desc Options
/// 
/// The constructor has the following arguments:
///  - delta_p: the fractional pt cut imposed on the subjets (computed
///             as a fraction of the original jet) [0.10 by default]
///  - delta_r: the minimal distance between 2 subjets (computed as
///             |y1-y2|+|phi1-phi2|) [0.19 by default]
///  - cos_theta_W_max: the maximal value for the polarisation angle
///                     of the W [0.7 by default]
///  - mW: the W mass [80.4 by default]
///
/// \section input Input conditions
/// 
///  - the original jet must have an associated (and valid)
///    ClusterSequence
///  - if the original jet has not been obtained with the
///    Cambridge/Aachen algorithm, a warning will be issued
///
/// \section output Output/interface
///
/// A  JHTopTagger can be used as follows:
///
/// \code
///  JHTopTagger top_tagger(...);
///  PseudoJet top_candidate = top_tagger(jet);
///  if (top_candidate != 0) {
///    double top_mass = top_candidate.m();
///    double W_mass   = top_candidate.structure_of<JHTopTagger>().W().m();
///    // then place your cuts on top and W mass
///  }
/// \endcode
///
/// Other information that is available from the structure_of<JHTopTagger>() 
/// call is
///
/// - PseudoJet W1(): the harder of the two prongs of the W
/// - PseudoJet W2(): the softer of the two prongs of the W
/// - PseudoJet non_W(): the non-W subjet(s) of the top candidate (i.e. the b)
/// - double cos_theta_W(): the W helicity angle
///
/// The top_candidate also has the following pieces:
///
/// - top_candidate.pieces()[0]: W1
/// - top_candidate.pieces()[1]: W2
/// - top_candidate.pieces()[2]: the harder of the non-W subjets
/// - top_candidate.pieces()[3]: the softer of the non-W subjets (may not exist)
///
/// The W itself has two pieces (W1, W2).
///
/// See also \subpage Example13  for a usage example.
///
class JHTopTagger : public Transformer{
public:
  /// default ctor
  /// The parameters are the following:
  ///  \param delta_p          fractional pt cut imposed on the subjets
  ///                          (computed as a fraction of the original jet)
  ///  \param delta_r          minimal distance between 2 subjets
  ///                          (computed as |y1-y2|+|phi1-phi2|)
  ///  \param cos_theta_W_max  the maximal value for the polarisation 
  ///                          angle of the W
  ///  \param mW               the W mass
  ///
  /// The default values of all these parameters are taken from
  /// arXiv:0806:0848
  JHTopTagger(const double delta_p=0.10, const double delta_r=0.19, 
	      double cos_theta_W_max=0.7, double mW=80.4)
    : _delta_p(delta_p), _delta_r(delta_r),
      _cos_theta_W_max(cos_theta_W_max), _mW(mW){};

  /// description of the tagger
  virtual std::string description() const;

  /// returns the tagged PseudoJet if successful, 0 otherwise
  ///  \param jet   the PseudoJet to tag
  virtual PseudoJet result(const PseudoJet & jet) const;

  /// the type of the associated structure
  typedef JHTopTaggerStructure StructureType;

protected:
  /// runs the Johns Hopkins decomposition procedure
  std::vector<PseudoJet> _split_once(const PseudoJet & jet_to_split,
				     const PseudoJet & reference_jet) const;

  /// computes the W helicity angle
  double _cos_theta_W(const PseudoJet & result) const;

  double _delta_p, _delta_r, _cos_theta_W_max, _mW;
  static LimitedWarning _warnings_nonca;
};


//------------------------------------------------------------------------
/// @ingroup tools_taggers
/// \class JHTopTaggerStructure
/// the structure returned by the JHTopTagger transformer.
///
/// See the JHTopTagger class description for the details of what
/// is inside this structure
///
class JHTopTaggerStructure : public CompositeJetStructure{
public:
  /// ctor with pieces initialisation
  JHTopTaggerStructure(std::vector<PseudoJet> pieces,
		 const JetDefinition::Recombiner *recombiner = 0) :
    CompositeJetStructure(pieces, recombiner), _cos_theta_w(0.0){}

  /// returns the W subjet
  inline const PseudoJet & W() const{ 
    return _W;
  }

  /// returns the first W subjet (the hardest)
  inline const PseudoJet & W1() const{
    assert(_pieces.size()>0);
    return _pieces[0];
  }

  /// returns the second W subjet
  inline const PseudoJet & W2() const{
    assert(_pieces.size()>1);
    return _pieces[1];
  }

  /// returns the non-W subjet
  /// It will have 1 or 2 pieces depending on whether the tagger has
  /// found 3 or 4 pieces
  inline const PseudoJet & non_W() const{ 
    return _non_W;
  }

  /// returns the W helicity angle
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


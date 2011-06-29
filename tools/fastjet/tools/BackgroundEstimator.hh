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

#ifndef __FASTJET_BACKGROUND_ESTIMATOR_HH__
#define __FASTJET_BACKGROUND_ESTIMATOR_HH__

#include <fastjet/ClusterSequenceAreaBase.hh>
#include <fastjet/FunctionOfPseudoJet.hh>
#include <fastjet/Selector.hh>
#include <iostream>

FASTJET_BEGIN_NAMESPACE     // defined in fastjet/internal/base.hh



//----------------------------------------------------------------------
/// @ingroup tools
/// A background rescaling that is a simple polynomial in y
class BackgroundRescalingYPolynomial : public FunctionOfPseudoJet<double> {
public:
  /// construct a background rescaling polynomial of the form
  /// a0 + a1*y + a2*y^2 + a3*y^3 + a4*y^4
  ///
  /// The following values give a reasonable reproduction of the
  /// Pythia8 tune 4C background shape for pp collisions at
  /// sqrt(s)=7TeV:
  ///
  /// - a0 =  1.157
  /// - a1 =  0
  /// - a2 = -0.0266
  /// - a3 =  0
  /// - a4 =  0.000048
  ///
  BackgroundRescalingYPolynomial(double a0=1, 
				 double a1=0, 
				 double a2=0, 
				 double a3=0, 
				 double a4=0) : _a0(a0), _a1(a1), _a2(a2), _a3(a3), _a4(a4) {}

  /// return the rescaling factor associated with this jet
  virtual double result(const PseudoJet & jet) const;
private:
  double _a0, _a1, _a2, _a3, _a4;
};


//----------------------------------------------------------------------
/// @ingroup tools
/// Class that implements pt/area_4vector.perp() for background estimation
class BackgroundJetPtDensity : public FunctionOfPseudoJet<double> {
public:
  virtual double result(const PseudoJet & jet) const {
    return jet.perp() / jet.area_4vector().perp();
  }
  virtual std::string description() const {return "BackgroundJetPtDensity";}
};


//----------------------------------------------------------------------
/// @ingroup tools
/// Class that implements (scalar pt sum of jet)/(scalar area of jet)
/// for background estimation. Optionally it can return a quantity
/// based on the sum of pt^n, e.g. for use in subtracting
/// fragementation function moments.
class BackgroundJetScalarPtDensity : public FunctionOfPseudoJet<double> {
public:
  /// Default constructor provides background estimation with scalar pt sum
  BackgroundJetScalarPtDensity() : _pt_power(1) {}

  /// Constructor to provide background estimation based on 
  /// \f$ sum_{i\in jet} p_{ti}^{n} \f$
  BackgroundJetScalarPtDensity(double n) : _pt_power(n) {}

  virtual double result(const PseudoJet & jet) const;

  virtual std::string description() const {return "BackgroundScalarJetPtDensity";}

private:
  double _pt_power;
};

//----------------------------------------------------------------------
/// @ingroup tools
/// Class that implements
/// \f$  \frac{1}{A} \sum_{i \in jet} (\sqrt{p_{ti}^2+m^2} - p_{ti}) \f$
/// for background estimation.
///
/// This is useful for correcting jet masses in cases where the event
/// involves massive particles.
class BackgroundJetPtMDensity : public FunctionOfPseudoJet<double> {
public:
  virtual double result(const PseudoJet & jet) const {
    std::vector<PseudoJet> constituents = jet.constituents();
    double scalar_ptm = 0;
    for (unsigned i = 0; i < constituents.size(); i++) {
      scalar_ptm += constituents[i].mperp() - constituents[i].perp();
    }
    return scalar_ptm / jet.area();
  }

  virtual std::string description() const {return "BackgroundPtMDensity";}
};




/// @ingroup tools
/// \class BackgroundEstimator
/// Class to estimate the density of the background per unit area
///
/// For a given event, this class calculated the median of the
/// distribution of pt/Area for all jets in the event that pass some
/// selection criterion.
///
/// Events are passed either in the form of a ClusterSequenceArea (in
/// which case the jets used as those returned by "inclusive_jets()")
/// or directly as a set of jets.
///
/// The selection criterion is typically a geometrical one (e.g. all
/// jets with |y|<2) sometimes supplemented with some kinematical
/// restriction (e.g. exclusion of the two hardest jets).
///
/// Beware: 
///   by default, to correctly handle partially empty events, the
///   class attempts to calculate an "empty area", based
///   (schematically) on
///
///          range.total_area() - sum_{jets_in_range} jets.area()
///  
///   For ranges with small areas, this can be innacurate (particularly 
///   relevant in dense events where empty_area should be zero and ends
///   up not being zero).
///
///   This calculation of empty area can be avoided if you supply a
///   ClusterSequenceArea class with explicit ghosts
///   (ActiveAreaExplicitGhosts). This is _recommended_!
///
class BackgroundEstimator {
public:
  /// @name constructors and destructors
  //\{
  //----------------------------------------------------------------
  /// ctor from a ClusterSequenceAreaBase with area
  ///
  /// \param csa         the ClusterSequenceArea to use
  /// \param rho_range   a Selector that specifies the range over 
  ///                    which jets will be considered 
  ///
  /// Pre-conditions: 
  ///  - one should be able to estimate the "empty area" (i.e. the area
  ///    not occupied by jets). This is feasible if at least one of the following
  ///    conditions is satisfied:
  ///     ( i) the ClusterSequence has explicit ghosts
  ///     (ii) the range has a computable area.
  ///  - the jet algorithm must be suited for median computation
  ///    (otherwise a warning will be issues)
  ///
  /// Note that selectors with e.g. hardest-jets exclusion do not have
  /// a well-defined area. For this reasons, it is STRONGLY advised to
  /// use an area with explicit ghosts.
  BackgroundEstimator(const ClusterSequenceAreaBase &csa, const Selector &rho_range);
  
  /// ctor from a list of jets
  ///
  /// \param jets        the jets to use
  /// \param rho_range   the range over which jets will be considered
  ///
  /// Pre-conditions:
  ///  - all the jets must come from the same cluster sequence
  ///  - As for the above ctor, one needs to be able to estimate the
  ///    empty area. The conditions that the ClusterSequence must have
  ///    explicit ghosts is checked on the ClusterSequence shared by
  ///    the jets.
  ///  - As for the above ctor, the jet alg must be adequate
  BackgroundEstimator(const std::vector<PseudoJet> &jets, const Selector &rho_range);
  
  /// Constructor that just sets the rho range. The CSA or jets
  /// actually used for estimating the background must be passed
  /// later via set_cluster_sequence(...) or set_jets(...)
  ///
  /// \param rho_range   the range over which jets will be considered
  ///
  BackgroundEstimator(const Selector &rho_range) : _rho_range(rho_range) {
    reset();
  }

  /// default ctor. In order to use the BG estimator you then need to
  /// call set_selector(...) and also set_cluster_sequence(...) or
  /// set_jets(...)
  BackgroundEstimator()  {reset();}
  

  /// default dtor
  ~BackgroundEstimator();

  //\}

  /// @ name  retrieving fundamental information
  //\{
  //----------------------------------------------------------------

  /// get rho, the median background density oer unit area
  double rho() const {
    if (_rho_range.takes_reference())
      throw Error("The background estimation is obtained from a selector that takes a reference jet. rho(PseudoJet) should be used in that case");
    _recompute_if_needed();
    return _rho;
  }

  /// get sigma, the background fluctuations per unit area
  double sigma() const {
    if (_rho_range.takes_reference())
      throw Error("The background estimation is obtained from a selector that takes a reference jet. rho(PseudoJet) should be used in that case");
    _recompute_if_needed();
    return _sigma;
  }

  /// get rho, the median background density per unit area, locally at
  /// the position of a given jet.
  ///
  /// If the Selector associated with the range takes a reference jet
  /// (i.e. is relocatable), then for subsequent operations the
  /// Selector has that jet set as its reference.
  double rho(const PseudoJet & jet) {
    _recompute_if_needed(jet);
    double our_rho = _rho;
    if (_rescaling_class != 0) { 
      our_rho *= (*_rescaling_class)(jet);
    }
    return our_rho;
  }

  /// get sigma, the background fluctuations per unit area,
  /// locally at the position of a given jet.
  ///
  /// If the Selector associated with the range takes a reference jet
  /// (i.e. is relocatable), then for subsequent operations the
  /// Selector has that jet set as its reference.
  double sigma(const PseudoJet &jet) {
    _recompute_if_needed(jet);
    double our_sigma = _sigma;
    if (_rescaling_class != 0) { 
      our_sigma *= (*_rescaling_class)(jet);
    }
    return our_sigma;
  }

  //\}
  
  /// @ name  retrieving additional useful information
  //\{
  //----------------------------------------------------------------
  /// Returns the mean area of the jets used to actually compute the
  /// background properties in the last call of rho() or sigma()
  double mean_area() const{
    _recompute_if_needed();
    return _mean_area;
  }
  
  /// returns the number of jets used to actually compute the
  /// background properties in the last call of rho() or sigma()
  unsigned int n_jets_used() const{
    _recompute_if_needed();
    return _n_jets_used;
  }

  /// Returns the estimate of the area (within the range defined by
  /// the selector) that is not occupied by jets. The value is that
  /// for the last call of rho() or sigma()
  ///
  /// The answer is defined to be zero if the area calculation
  /// involved explicit ghosts; if the area calculation was an active
  /// area, then use is made of the active area's internal list of
  /// pure ghost jets (taking those that pass the selector); otherwise
  /// it is based on the difference between the selector's total area
  /// and the area of the jets that pass the selector.
  ///
  /// The result here is just the cached result of the corresponding
  /// call to the ClusterSequenceAreaBase function.
  double empty_area() const{
    _recompute_if_needed();
    return _empty_area;
  }

  /// Returns the number of empty jets used when computing the
  /// background properties. The value is that for the last call of
  /// rho() or sigma().
  ///
  /// If the area has explicit ghosts the result is zero; for active
  /// areas it is the number of internal pure ghost jets that pass the
  /// selector; otherwise it is deduced from the empty area, divided by 
  /// \f$ 0.55 \pi R^2 \f$ (the average pure-ghost-jet area).
  ///
  /// The result here is just the cached result of the corresponding
  /// call to the ClusterSequenceAreaBase function.
  double n_empty_jets() const{
    _recompute_if_needed();
    return _n_empty_jets;
  }

  //}

  /// @name configuring behaviour
  //\{
  //----------------------------------------------------------------

  /// (re)set the cluster sequence (with area support) to be used by
  /// future calls to rho() etc. 
  ///
  /// \param csa  the cluster sequence area
  ///
  /// Pre-conditions: 
  ///  - one should be able to estimate the "empty area" (i.e. the area
  ///    not occupied by jets). This is feasible if at least one of the following
  ///    conditions is satisfied:
  ///     ( i) the ClusterSequence has explicit ghosts
  ///     (ii) the range has a computable area.
  ///  - the jet algorithm must be suited for median computation
  ///    (otherwise a warning will be issues)
  ///
  /// Note that selectors with e.g. hardest-jets exclusion do not have
  /// a well-defined area. For this reasons, it is STRONGLY advised to
  /// use an area with explicit ghosts.
  void set_cluster_sequence(const ClusterSequenceAreaBase & csa);

  /// (re)set the jets (which must have area support) to be used by future
  /// calls to rho() etc.; for the conditions that must be satisfied
  /// by the jets, see the Constructor that takes jets.
  void set_jets(const std::vector<PseudoJet> &jets);

  /// (re)set the selector to be used for future calls to rho() etc.
  void set_selector(const Selector & rho_range_selector) {
    _rho_range = rho_range_selector;
    _uptodate = false;
  }


  /// Resets the class to its default state, including the choice to
  /// use 4-vector areas.
  ///
  void reset();

  /// By default when calculating pt/Area for a jet, it is the
  /// transverse component of the 4-vector area that is used in the ratiof \f$p_t/A\f$. 
  /// Calling this function with a "false" argument causes the scalar area to
  /// be used instead. 
  ///
  /// While the difference between the two choices is usually small,
  /// for high-precision work it is usually the 4-vector area that is
  /// to be preferred.
  ///
  ///  \param use_it             whether one uses the 4-vector area or not (true by default)
  void set_use_area_4vector(bool use_it = true){
    _use_area_4vector = use_it;
    _uptodate = false;
  }  

  /// check if the estimator uses the 4-vector area or the scalar area
  bool use_area_4vector() const{ return _use_area_4vector;}

  /// The FastJet v2.X sigma calculation had a small spurious offset
  /// in the limit of a small number of jets. This is fixed by default
  /// in versions 3 upwards. The old behaviour can be obtained with a
  /// call to this function.
  void set_provide_fj2_sigma(bool provide_fj2_sigma = true) {
    _provide_fj2_sigma = provide_fj2_sigma;
    _uptodate = false;
  }

  /// Set a pointer to a class that calculates the quantity whose
  /// median will be calculated; if the pointer is null then pt/area
  /// is used (as occurs also if this function is not called).
  void set_jet_density_class(const FunctionOfPseudoJet<double> * jet_density_class) {
    _jet_density_class = jet_density_class;
    _uptodate = false;
  }

  /// return the pointer to the jet density class
  const FunctionOfPseudoJet<double> *  jet_density_class() {
    return _jet_density_class;
  }

  /// Set a pointer to a class that calculates the rescaling factor as
  /// a function of the jet (position). Note that the rescaling factor
  /// is used both in the determination of the "global" rho (the pt/A
  /// of each jet is divided by this factor) and when asking for a
  /// local rho (the result is multiplied by this factor).
  ///
  /// The BackgroundRescalingYPolynomial class can be used to get a
  /// rescaling that depends just on rapidity.
  void set_rescaling_class(const FunctionOfPseudoJet<double> * rescaling_class) {
    _rescaling_class = rescaling_class;
    _uptodate = false;
  }

//   /// Set the class that calculates the rescaling factor as a function
//   /// of the jet (position). Usage as for set_rescaling_class, but it just
//   /// take a copy of the rescaling class, which must derive from BackgroundRescalingBase
//   template<class T> void set_rescaling_class(const T & rescaling_class) {
//     BackgroundRescalingBase * rescaling_class_copy = new T(rescaling_class);
//     _rescaling_class_sharedptr.reset(rescaling_class_copy);
//     _rescaling_class = rescaling_class_copy;
//     _uptodate = false;
//   }
// 
//   /// include a template for pointers that are derived from BackgroundRescalingBase
//   template<class T> void set_rescaling_class(T * rescaling_class) {
//     const BackgroundRescalingBase * brb = dynamic_cast<const BackgroundRescalingBase *>(rescaling_class);
//     if (brb != 0) {
//       set_rescaling_class(brb);
//       return;
//     } else {
//       throw Error("set_rescaling called with a pointer that does not dynamic cast down to const BackgroundRescalingBase");
//     }
//   }
// 
//   void set_rescaling_class(SharedPtr<BackgroundRescalingBase> & rescaling_class) {
//     _rescaling_class_sharedptr = rescaling_class;
//     _rescaling_class = _rescaling_class_sharedptr.get();
//   }

  /// return the pointer to the jet density class
  const FunctionOfPseudoJet<double> *  rescaling_class() {
    return _rescaling_class;
  }

  //\}

protected:

  /// given a quantity in a vector (e.g. pt_over_area) and knowledge
  /// about the number of empty jets, calculate the median and
  /// stand_dev_if_gaussian (roughly from the 16th percentile)
  ///
  /// If do_fj2_calculation is set to true then this performs FastJet
  /// 2.X estimation of the standard deviation, which has a spurious
  /// offset in the limit of a small number of jets.
  void _median_and_stddev(const std::vector<double> & quantity_vector, 
			  double n_empty_jets, 
			  double & median, 
			  double & stand_dev_if_gaussian,
			  bool do_fj2_calculation = false
			  ) const;


private:

  /// do the actual job
  void _compute() const;
   
  /// check if the properties need to be recomputed 
  /// and do so if needed
  void _recompute_if_needed() const {
    if (!_uptodate) _compute();
    _uptodate = true;
  }

  /// for estimation using a selector that takes a reference jet
  /// (i.e. a selector that can be relocated) this function allows one
  /// to set its position.
  ///
  /// Note that this HAS to be called before any attempt to compute
  /// the background properties. The call is, however, performed
  /// automatically by the functions rho(jet) and sigma(jet).
  void _recompute_if_needed(const PseudoJet &jet);

  /// check that the underlying structure is still alive
  /// throw an error otherwise
  void _check_csa_alive() const;

  /// check that the algorithm used for the clustering is adapted for
  /// background estimation (i.e. either kt or C/A)
  /// Issue a warning otherwise
  void _check_jet_alg_good_for_median() const;
  
  // the information needed to do the computation
  Selector _rho_range;                      ///< range to compute the background in
  std::vector<PseudoJet> _included_jets;    ///< jets to be used
  bool _use_area_4vector;
  bool _provide_fj2_sigma;
  PseudoJet _current_reference;

  const FunctionOfPseudoJet<double> * _jet_density_class;
  const FunctionOfPseudoJet<double> * _rescaling_class;
  //SharedPtr<BackgroundRescalingBase> _rescaling_class_sharedptr;
  
  // the actual results of the computation
  mutable double _rho;		        ///< background estimated density per unit area
  mutable double _sigma;	        ///< background estimated fluctuations
  mutable double _mean_area;	        ///< mean area of the jets used to estimate the background
  mutable unsigned int _n_jets_used;    ///< number of jets used to estimate the background
  mutable double _n_empty_jets;         ///< number of empty (pure-ghost) jets
  mutable double _empty_area;           ///< the empty (pure-ghost/unclustered) area!

  // internal variables
  SharedPtr<PseudoJetStructureBase> _csi;   ///< allows to check if _csa is still valid
  mutable bool _uptodate;                   ///< true when the background computation is up-to-date

  /// handle warning messages
  static LimitedWarning _warnings;
  static LimitedWarning _warnings_zero_area;
  static LimitedWarning _warnings_empty_area;
  //static LimitedWarning _warnings_relocation;
};



FASTJET_END_NAMESPACE

#endif  // __BACKGROUND_ESTIMATOR_HH__


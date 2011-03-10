#ifndef __FASTJET_BACKGROUND_ESTIMATOR_HH__
#define __FASTJET_BACKGROUND_ESTIMATOR_HH__

#include <fastjet/ClusterSequenceAreaBase.hh>
#include <fastjet/Selector.hh>
#include <iostream>

FASTJET_BEGIN_NAMESPACE     // defined in fastjet/internal/base.hh


/// @ingroup tools
/// \class BackgroundEstimator
/// Class to estimate the density of the background per unit area
///
/// The default behaviour of this class is to compute the global
/// properties of the background as it is done in ClusterSequenceArea.
/// The list of jets included in the computation of the median
/// background is the one that passed a selection test (this could at
/// the same time select jets in a given "range" and jets satisfying
/// various kinematic cuts)
///
/// Default behaviour:
///   by default the list of included jets is the inclusive jets from
///   the given ClusterSequence; the list of explicitly excluded jets 
///   is empty; we use 4-vector area
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
class BackgroundEstimator{
public:
  /// @name constructors and destructors
  //\{
  //----------------------------------------------------------------
  /// ctor from a ClusterSequence with area
  ///
  /// \param csa         the ClusterSequenceArea to use
  /// \param rho_range   the range over which jets will be considered
  ///
  /// Pre-conditions: 
  ///  - one should be able to estimate the "empty area" (i.e. the area
  ///    not occupied by jets). This is feasible is one of the following
  ///    conditions is satisfied:
  ///     ( i) the ClusterSequence has explicit ghosts
  ///     (ii) the range has a computable area.
  ///  - the jet algorithm must be suited for median computation
  ///    (otherwise a warning will be issues)
  /// Note that selectors with e.g. hardest-jets exclusion do not have a
  /// well-defined area
  /// For these reasons, it is STRONGLY advised to use explicit ghosts
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
  
  /// default dtor
  ~BackgroundEstimator();

  //\}

  /// @ name  retrieving fundamental information
  //\{
  //----------------------------------------------------------------

  /// get rho, the median background density oer unit area
  double rho() {
    _recompute_if_needed();
    return _rho;
  }

  /// get sigma, the background fluctuations per unit area
  double sigma() {
    _recompute_if_needed();
    return _sigma;
  }

  /// get rho, the median background density oer unit area,
  /// locally at the position of a given jet.
  ///
  /// This requires a relocatable range
  double rho(const PseudoJet jet) {
    set_reference(jet);
    return rho();
  }

  /// get sigma, the background fluctuations per unit area,
  /// locally at the position of a given jet.
  ///
  /// This requires a relocatable range
  double sigma(const PseudoJet &jet) {
    set_reference(jet);
    return sigma();
  }

  //\}
  
  /// @ name  retrieving additional useful information
  //\{
  //----------------------------------------------------------------
  /// get the median area of the jets used to actually compute the background properties
  double mean_area(){
    _require_uptodate();
    return _mean_area;
  }
  
  /// get the number of jets used to actually compute the background properties
  unsigned int n_jets_used(){
    _require_uptodate();
    return _n_jets_used;
  }

  /// get the number of empty jets used when computing the background properties;
  /// (it is deduced from the empty area with an assumption about the average
  /// area of jets)
  double n_empty_jets(){
    _require_uptodate();
    return _n_empty_jets;
  }

  /// returns the estimate of the area (within Range) that is not occupied
  /// by the jets (excluded jets are removed from this count)
  double empty_area(){
    _require_uptodate();
    return _empty_area;
  }

  //}

  /// @name configuring behaviour
  //\{
  //----------------------------------------------------------------

  /// for estimation using a relocatable selector (i.e. local range)
  /// this allows to set its position. Note that this HAS to be called
  /// before any attempt to compute the background properties
  BackgroundEstimator & set_reference(const PseudoJet &jet);

  /// reset to default values
  /// set the list of included jets to the inclusive jets and clear the excluded ones
  void reset();

  /// specify if one uses the scalar or 4-vector area
  ///  \param use_it             whether one uses the 4-vector area or not (true by default)
  void set_use_area_4vector(bool use_it = true){
    _use_area_4vector = use_it;
    _uptodate = false;
  }  
  //\}
  
private:
  /// do the actual job
  void _compute();
  
  /// check if the properties need to be recomputed 
  /// and do so if needed
  void _require_uptodate(){
    if (!_uptodate){
      throw Error("The requested information can only be obtained once the background has actually been computed");
    }
  }

  /// check if the properties need to be recomputed 
  /// and do so if needed
  void _recompute_if_needed(){
    if (!_uptodate)
      _compute();
    _uptodate = true;
  }

  /// check that the underlying structure is still alive
  /// throw an error otherwise
  void _check_csa_alive();

  /// check that the algorithm used for the clustering is adapted for
  /// background estimation (i.e. either kt or C/A)
  /// Issue a warning otherwise
  void _check_jet_alg_good_for_median();
  
  // the information needed to do the computation
  Selector _rho_range;                      ///< range to compute the background in
  std::vector<PseudoJet> _included_jets;    ///< jets to be used
  std::vector<PseudoJet> _selected_jets;    ///< jets used in practice
  bool _use_area_4vector;
  
  // the actual results of the computation
  double _rho;		                    ///< background estimated density per unit area
  double _sigma;		            ///< background estimated fluctuations
  double _mean_area;		            ///< mean area of the jets used to estimate the background
  unsigned int _n_jets_used;                ///< number of jets used to estimate the background
  double _n_empty_jets;                     ///< number of empty (pure-ghost) jets
  double _empty_area;                       ///< the empty (pure-ghost/unclustered) area!

  // internal variables
  SharedPtr<PseudoJetStructureBase> _csi;   ///< allows to check if _csa is still valid
  bool _uptodate;                           ///< true when the background computation is up-to-date

  /// handle warning messages
  static LimitedWarning _warnings;
  static LimitedWarning _warnings_zero_area;
  static LimitedWarning _warnings_relocation;
};

FASTJET_END_NAMESPACE

#endif  // __BACKGROUND_ESTIMATOR_HH__


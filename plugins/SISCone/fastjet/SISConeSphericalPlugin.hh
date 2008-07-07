#ifndef __SISCONESPHERICALPLUGIN_HH__
#define __SISCONESPHERICALPLUGIN_HH__

#include "fastjet/JetDefinition.hh"
#include "fastjet/ClusterSequence.hh" // needed for the extras we define
#include <vector>
#include <memory>
#include <cmath>

// put a forward declaration to the Csiscone class to avoid having to
// include the siscone headers here
namespace siscone_spherical {
  class CSphsiscone;
}

// questionable whether this should be in fastjet namespace or not...
FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

// another forward declaration to reduce includes
class PseudoJet;

//----------------------------------------------------------------------
//
/// SISConePlugin is a plugin for fastjet (v2.1 upwards) that provides
/// an interface to the seedless infrared safe cone jet finder by
/// Gregory Soyez and Gavin Salam.
///
/// This is the version of SISCone using spherical coordinates
///
/// SISCone uses geometrical techniques to exhaustively consider all
/// possible distinct cones. It then finds out which ones are stable
/// and sends the result to the Tevatron Run-II type split-merge
/// procedure for overlapping cones.
///
/// Four parameters govern the "physics" of the algorithm:
///
///  - the cone_radius (this should be self-explanatory!)
///
///  - the overlap_threshold is the parameter which dictates how much
///    two jets must overlap (E_overlap/min(E1,E2)) if they are to be 
///    merged
///
///  - Not all particles are in stable cones in the first round of
///    searching for stable cones; one can therefore optionally have the
///    the jet finder carry out additional passes of searching for
///    stable cones among particles that were in no stable cone in
///    previous passes --- the maximum number of passes carried out is
///    n_pass_max. If this is zero then additional passes are carried
///    out until no new stable cones are found.
///
///  - Protojet Emin: protojets that are below this Emin
///    (default = 0) are discarded before each iteration of the
///    split-merge loop.
///
/// One parameter governs some internal algorithmic shortcuts: 
///
/// - if "caching" is turned on then the last event clustered by
///   siscone is stored -- if the current event is identical and the
///   cone_radius and n_pass_mass are identical, then the only part of
///   the clustering that needs to be rerun is the split-merge part,
///   leading to significant speed gains; there is a small (O(N) storage
///   and speed) penalty for caching, so it should be kept off
///   (default) if only a single overlap_threshold is used.
///
/// The final jets can be accessed by requestion the
/// inclusive_jets(...) from the ClusterSequence object. Note that
/// these PseudoJets have their user_index() set to the index of the
/// pass in which they were found (first pass = 0). NB: This does not
/// currently work for jets that consist of a single particle.
///
/// For further information on the details of the algorithm see the
/// SISCone paper; for documentation about the implementation, see the
/// siscone/doc/html/index.html file.
//
class SISConeSphericalPlugin : public JetDefinition::Plugin {
public:

  /// enum for the different split-merge scale choices;
  /// Note that order _must_ be the same as in siscone
  enum SplitMergeScale {SM_pt,     ///< transverse momentum (E-scheme), IR unsafe
                        SM_Et,     ///< transverse energy (E-scheme), not long. boost invariant
                                   ///< original run-II choice [may not be implemented]
                        SM_mt,     ///< transverse mass (E-scheme), IR safe except
                                   ///< in decays of two identical narrow heavy particles
                        SM_pttilde, ///< pt-scheme pt = \sum_{i in jet} |p_{ti}|, should
                                   ///< be IR safe in all cases
                        SM_E ///< energy
  };


  /// Main constructor for the SISCone Plugin class.  
  ///
  /// Note: wrt version prior to 2.4 this constructor differs in that a 
  /// the default value has been removed for overlap_threshold. The
  /// former has been removed because the old default of 0.5 was found
  /// to be unsuitable in high-noise environments; so the user should
  /// now explicitly think about the value for this -- we recommend
  /// 0.75.
  ///
  SISConeSphericalPlugin (double cone_radius,
			  double overlap_threshold,
			  int    n_pass_max = 0,
			  double protojet_Emin = 0.0, 
			  bool   caching = false,
			  SplitMergeScale  split_merge_scale = SM_E,
			  double split_merge_stopping_scale = 0.0) :
    _cone_radius           (cone_radius       ),
    _overlap_threshold     (overlap_threshold ),
    _n_pass_max            (n_pass_max ), 
    _protojet_Emin         (protojet_Emin),
    _caching               (caching),             
    _split_merge_scale     (split_merge_scale),
    _split_merge_stopping_scale (split_merge_stopping_scale),
    _ghost_sep_scale       (0.0),
    _use_E_weighted_splitting  (false) {}



  /// copy constructor
  SISConeSphericalPlugin (const SISConeSphericalPlugin & plugin) {
    *this = plugin;
  }

  /// the cone radius
  double cone_radius        () const {return _cone_radius        ;}

  /// Fraction of overlap energy in a jet above which jets are merged
  /// and below which jets are split.
  double overlap_threshold  () const {return _overlap_threshold  ;}

  /// the maximum number of passes of stable-cone searching (<=0 is same
  /// as infinity).
  int n_pass_max  () const {return _n_pass_max  ;}

  /// minimum energy for a protojet to be considered in the split-merge step
  /// of the algorithm
  double protojet_Emin  () const {return _protojet_Emin  ;}

  /// return the scale to be passed to SISCone as the protojet_Emin
  /// -- if we have a ghost separation scale that is above the
  /// protojet_ptmin, then the ghost_separation_scale becomes the
  /// relevant one to use here
  double protojet_or_ghost_Emin  () const {return std::max(_protojet_Emin,
                                                           _ghost_sep_scale);}

  /// indicates scale used in split-merge
  SplitMergeScale split_merge_scale() const {return _split_merge_scale;}
  /// sets scale used in split-merge
  void set_split_merge_scale(SplitMergeScale sms) {_split_merge_scale = sms;}

  /// set the "split_merge_stopping_scale": if the scale variable for
  /// all protojets is below this, then stop the split-merge procedure
  /// and keep only those jets found so far. This is useful in
  /// determination of areas of hard jets because it can be used to
  /// avoid running the split-merging on the pure ghost-part of the
  /// event.
  void set_split_merge_stopping_scale(double scale) {
    _split_merge_stopping_scale = scale;}

  /// return the value of the split_merge_stopping_scale (see
  /// set_split_merge_stopping_scale(...) for description)
  double split_merge_stopping_scale() {return _split_merge_stopping_scale;}

  /// indicate if te splittings are done using the anti-kt distance
  bool split_merge_use_E_weighted_splitting() const {return _use_E_weighted_splitting;}
  void set_split_merge_use_E_weighted_splitting(bool val) {
    _use_E_weighted_splitting = val;}

  /// indicates whether caching is turned on or not.
  bool caching() const {return _caching ;}

  // the things that are required by base class
  virtual std::string description () const;
  virtual void run_clustering(ClusterSequence &) const;
  /// the plugin mechanism's standard way of accessing the jet radius
  virtual double R() const {return cone_radius();}

  /// return true since there is specific support for the measurement
  /// of passive areas, in the sense that areas determined from all
  /// particles below the ghost separation scale will be a passive
  /// area. 
  virtual bool supports_ghosted_passive_areas() const {return true;}
  
  /// set the ghost separation scale for passive area determinations
  /// _just_ in the next run (strictly speaking that makes the routine
  /// a non const, so related internal info must be stored as a mutable)
  virtual void set_ghost_separation_scale(double scale) const {
    _ghost_sep_scale = scale;
  }

  virtual double ghost_separation_scale() const {return _ghost_sep_scale;}

private:
  double _cone_radius, _overlap_threshold;
  int    _n_pass_max;
  double _protojet_Emin;
  bool   _caching;//, _split_merge_on_transverse_mass;
  SplitMergeScale _split_merge_scale;
  double _split_merge_stopping_scale;

  mutable double _ghost_sep_scale;
  bool _use_E_weighted_splitting;

  // variables for caching the results and the input
  static std::auto_ptr<SISConeSphericalPlugin        > stored_plugin;
  static std::auto_ptr<std::vector<PseudoJet>        > stored_particles;
  static std::auto_ptr<siscone_spherical::CSphsiscone> stored_siscone;

};



//======================================================================
/// Class that provides extra information about a SISCone clustering
class SISConeSphericalExtras : public ClusterSequence::Extras {
public:
  /// returns a reference to the vector of stable cones (aka protocones)
  const std::vector<PseudoJet> & stable_cones() const {return _protocones;}

  /// an old name for getting the vector of stable cones (aka protocones)
  const std::vector<PseudoJet> & protocones() const {return _protocones;}


  /// access to the siscone jet def plugin (more convenient than
  /// getting it from the original jet definition, because here it's
  /// directly of the right type (rather than the base type)
  const SISConeSphericalPlugin * jet_def_plugin() const {return _jet_def_plugin;}

  /// return a brief summary of the contents of the extras object
  /// (specifically, the number of protocones.
  std::string description() const;

  /// return the smallest difference in squared distance encountered
  /// during splitting between a particle and two overlapping
  /// protojets.
  inline double most_ambiguous_split() const {return _most_ambiguous_split;}

private:
  std::vector<PseudoJet> _protocones;
  const SISConeSphericalPlugin * _jet_def_plugin;
  double                _most_ambiguous_split;
  // let us be written to by SISConeSphericalPlugin
  friend class SISConeSphericalPlugin;
};


FASTJET_END_NAMESPACE        // defined in fastjet/internal/base.hh

#endif // __SISCONEPLUGIN_HH__


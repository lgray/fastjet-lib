#ifndef __SISCONEPLUGIN_HH__
#define __SISCONEPLUGIN_HH__

#include "fastjet/JetDefinition.hh"
#include "fastjet/ClusterSequence.hh" // needed for the extras we define
#include <vector>
#include <memory>

// put a forward declaration to the Csiscone class to avoid having to
// include the siscone headers here
namespace siscone {
  class Csiscone;
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
/// As of 2006-12-26, this plugin is beta, as is the SISCone code
/// itself.
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
///    two jets must overlap (pt_overlap/min(pt1,pt2)) if they are to be 
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
///  - Protojet ptmin: protojets that are below this ptmin
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
class SISConePlugin : public JetDefinition::Plugin {
public:

  /// Constructor for the SISCone Plugin class
  SISConePlugin (double cone_radius,
                 double overlap_threshold = 0.5,
                 int    n_pass_max = 1,
                 double protojet_ptmin = 0.0, 
                 bool   caching = false,
                 bool   split_merge_on_transverse_mass = true) :
    _cone_radius           (cone_radius       ),
    _overlap_threshold     (overlap_threshold ),
    _n_pass_max            (n_pass_max ), 
    _protojet_ptmin        (protojet_ptmin),
    _caching               (caching),             
    _split_merge_on_transverse_mass (split_merge_on_transverse_mass) {}
  
  /// backwards compatible constructor for the SISCone Plugin class
  /// (avoid using this in future).
  SISConePlugin (double cone_radius,
                 double overlap_threshold,
                 int    n_pass_max,
                 bool   caching ) :
    _cone_radius           (cone_radius       ),
    _overlap_threshold     (overlap_threshold ),
    _n_pass_max            (n_pass_max ), 
    _protojet_ptmin        (0.0),
    _caching               (caching),
    _split_merge_on_transverse_mass(true)     {}

  /// copy constructor
  SISConePlugin (const SISConePlugin & plugin) {
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

  /// minimum pt for a protojet to be considered in the split-merge step
  /// of the algorithm
  double protojet_ptmin  () const {return _protojet_ptmin  ;}

  /// indicates whether the split-merge orders on transverse mass or not.
  bool split_merge_on_transverse_mass() const {return _split_merge_on_transverse_mass ;}
  void set_split_merge_on_transverse_mass(bool val) {
    _split_merge_on_transverse_mass = val;}

  /// indicates whether caching is turned on or not.
  bool caching() const {return _caching ;}

  // the things that are required by base class
  virtual std::string description () const;
  virtual void run_clustering(ClusterSequence &) const;

private:
  double _cone_radius, _overlap_threshold;
  int    _n_pass_max;
  double _protojet_ptmin;
  bool   _caching, _split_merge_on_transverse_mass;

  // variables for caching the results and the input
  static std::auto_ptr<SISConePlugin          > stored_plugin;
  static std::auto_ptr<std::vector<PseudoJet> > stored_particles;
  static std::auto_ptr<siscone::Csiscone      > stored_siscone;

};


//======================================================================
/// Class that provides extra information about a SISCone clustering
class SISConeExtras : public ClusterSequence::Extras {
public:
  /// returns a reference to the vector of protocones
  const std::vector<PseudoJet> & protocones() const {return _protocones;}

  /// access to the siscone jet def plugin (more convenient than
  /// getting it from the original jet definition, because here it's
  /// directly of the right type (rather than the base type)
  const SISConePlugin * jet_def_plugin() const {return _jet_def_plugin;}

  /// return a brief summary of the contents of the extras object
  /// (specifically, the number of protocones.
  std::string description() const;

  /// return the smallest difference in squared distance encountered
  /// during splitting between a particle and two overlapping
  /// protojets.
  inline double most_ambiguous_split() const {return _most_ambiguous_split;}

private:
  std::vector<PseudoJet> _protocones;
  const SISConePlugin * _jet_def_plugin;
  double                _most_ambiguous_split;
  // let us be written to by SISConePlugin
  friend class SISConePlugin;
};


FASTJET_END_NAMESPACE        // defined in fastjet/internal/base.hh

#endif // __SISCONEPLUGIN_HH__


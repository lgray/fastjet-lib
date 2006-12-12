#ifndef __SCONESPLUGIN_HH__
#define __SCONESPLUGIN_HH__

#include "fastjet/JetDefinition.hh"

// questionable whether this should be in fastjet namespace or not...
FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

//----------------------------------------------------------------------
//
/// SConesPlugin is a plugin for fastjet (v2.1 upwards) that provides
/// an interface to the seedless cone jet finder by Gregory Soyez and
/// Gavin Salam.
///
/// As of 2006-12-12, this plugin is in development, as is the SCones
/// code itself
class SConesPlugin : public JetDefinition::Plugin {
public:
  SConesPlugin (double cone_radius,
                double overlap_threshold = 0.5) :
    _cone_radius           (cone_radius       ),
    _overlap_threshold     (overlap_threshold ) {};

  /// the cone radius
  double cone_radius        () const {return _cone_radius        ;};
  /// Fraction of overlap energy in a jet above which jets are medged
  /// and below which jets are split.
  double overlap_threshold  () const {return _overlap_threshold  ;};

  // the things that are required by base class
  virtual std::string description () const;
  virtual void run_clustering(ClusterSequence &) const;

private:
  double _cone_radius, _overlap_threshold;
};

FASTJET_END_NAMESPACE        // defined in fastjet/internal/base.hh

#endif // __SCONESPLUGIN_HH__


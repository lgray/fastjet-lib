#ifndef __PXCONEPLUGIN_HH__
#define __PXCONEPLUGIN_HH__

#include "fastjet/JetDefinition.hh"

// questionable whether this should be in fastjet namespace or not...

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

/// a plugin for fastjet-v2.1 that provides an interface to the fortran
/// pxcone algorithm
class PxConePlugin : public JetDefinition::Plugin {
public:
  /// a compact constructor
  PxConePlugin (double  cone_radius      , 
		double  min_jet_energy = 5.0  , 
		double  overlap_threshold = 0.5,
                bool    E_scheme_jets = false) : 
    _cone_radius        (cone_radius        ),
    _min_jet_energy     (min_jet_energy     ),
    _overlap_threshold  (overlap_threshold  ),
    _E_scheme_jets      (E_scheme_jets      ) {};


  // some functions to return info about parameters ----------------

  /// the cone radius
  double cone_radius        () const {return _cone_radius        ;};

  /// minimum jet energy (protojets below this are thrown own before
  /// merging/splitting) -- called epslon in pxcone
  double min_jet_energy     () const {return _min_jet_energy     ;};

  /// Maximum fraction of overlap energy in a jet -- called ovlim in pxcone.
  double overlap_threshold  () const {return _overlap_threshold  ;};

  /// if true then the final jets are returned as the E-scheme recombination
  /// of the particle momenta (by default, pxcone returns massless jets with
  /// a mean phi,eta type of recombination); regardless of what is
  /// returned, the internal pxcone jet-finding procedure is
  /// unaffected.
  bool E_scheme_jets()         const {return _E_scheme_jets      ;};

  // the things that are required by base class
  virtual std::string description () const;
  virtual void run_clustering(ClusterSequence &) const;

private:

  double _cone_radius       ;
  double _min_jet_energy    ;
  double _overlap_threshold ;

  bool _E_scheme_jets;
};

FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

#endif // __PXCONEPLUGIN_HH__

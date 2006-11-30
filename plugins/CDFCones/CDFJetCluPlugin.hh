#ifndef __CDFJETCLUPLUGIN_HH__
#define __CDFJETCLUPLUGIN_HH__

#include "fastjet/JetDefinition.hh"
#include "fastjet/PseudoJet.hh"
#include <map>

// questionable whether this should be in fastjet namespace or not...

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

/// a plugin for fastjet-v2.1 that provides an interface to the CDF
/// jetclu algorithm
class CDFJetCluPlugin : public JetDefinition::Plugin {
public:
  /// a compact constructor
  CDFJetCluPlugin (double   cone_radius, 
		   double   overlap_threshold = 0.5, 
		   double   seed_threshold = 1.0) : 
    _seed_threshold    ( seed_threshold    ),    
    _cone_radius       ( cone_radius       ),
    _adjacency_cut     (   2               ),
    _max_iterations    ( 100               ),
    _iratch            (   1               ),
    _overlap_threshold ( overlap_threshold )  {};

  /// a constructor that looks like the one provided by CDF
  CDFJetCluPlugin (
                     double seed_threshold   ,	 
		     double cone_radius      ,
		     int    adjacency_cut    ,
		     int    max_iterations   ,
		     int    iratch           ,
		     double overlap_threshold) :
    _seed_threshold    (seed_threshold    ),    
    _cone_radius       (cone_radius       ),
    _adjacency_cut     (adjacency_cut     ),
    _max_iterations    (max_iterations    ),
    _iratch            (iratch            ),
    _overlap_threshold (overlap_threshold )  {};

  // some functions to return info about parameters
  double seed_threshold    () const {return _seed_threshold    ;};
  double cone_radius       () const {return _cone_radius       ;};
  int    adjacency_cut     () const {return _adjacency_cut     ;};
  int    max_iterations    () const {return _max_iterations    ;};
  int    iratch            () const {return _iratch            ;};
  double overlap_threshold () const {return _overlap_threshold ;};


  // the things that are required by base class
  virtual std::string description () const;
  virtual void run_clustering(ClusterSequence &) const;

                      

private:

  double _seed_threshold   ;
  double _cone_radius      ;
  int    _adjacency_cut    ;
  int    _max_iterations   ;
  int    _iratch           ;
  double _overlap_threshold;

  /// given a jet try inserting its energy into the map -- if that
  /// energy entry already exists, modify the jet infinitesimally so
  /// as ensure that the jet energy is unique
  void _insert_unique (PseudoJet & jet, std::map<double,int> & jetmap) const;

};

FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

#endif // __CDFJETCLUPLUGIN_HH__

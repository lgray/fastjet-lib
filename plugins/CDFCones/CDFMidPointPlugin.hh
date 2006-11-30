#ifndef __CDFMIDPOINTPLUGIN_HH__
#define __CDFMIDPOINTPLUGIN_HH__

#include "fastjet/JetDefinition.hh"

// questionable whether this should be in fastjet namespace or not...

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

/// a plugin for fastjet-v2.1 that provides an interface to the CDF
/// midpoint algorithm
class CDFMidPointPlugin : public JetDefinition::Plugin {
public:
  /// a compact constructor
  CDFMidPointPlugin (double   cone_radius, 
		     double   overlap_threshold = 0.5, 
		     double   seed_threshold = 1.0,	     
		     double   cone_area_fraction = 1.0) : 
    _seed_threshold     (seed_threshold     ),    
    _cone_radius        (cone_radius        ),
    _cone_area_fraction (cone_area_fraction ),
    _max_pair_size      (2                  ),
    _max_iterations     (100                ),
    _overlap_threshold  (overlap_threshold  )  {};

  /// a constructor that looks like the one provided by CDF
  CDFMidPointPlugin (
                     double seed_threshold     ,	 
		     double cone_radius        ,
		     double cone_area_fraction ,
		     int    max_pair_size      ,
		     int    max_iterations     ,
		     double overlap_threshold  ) :
    _seed_threshold     (seed_threshold     ),    
    _cone_radius        (cone_radius        ),
    _cone_area_fraction (cone_area_fraction ),
    _max_pair_size      (max_pair_size      ),
    _max_iterations     (max_iterations     ),
    _overlap_threshold  (overlap_threshold  )  {};

  // some functions to return info about parameters
  double seed_threshold     () const {return _seed_threshold     ;};
  double cone_radius        () const {return _cone_radius        ;};
  double cone_area_fraction () const {return _cone_area_fraction ;};
  int    max_pair_size      () const {return _max_pair_size      ;};
  int    max_iterations     () const {return _max_iterations     ;};
  double overlap_threshold  () const {return _overlap_threshold  ;};


  // the things that are required by base class
  virtual std::string description () const;
  virtual void run_clustering(ClusterSequence &) const;

private:

  double _seed_threshold    ;
  double _cone_radius       ;
  double _cone_area_fraction;
  int    _max_pair_size     ;
  int    _max_iterations    ;
  double _overlap_threshold ;

};

FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

#endif // __CDFMIDPOINTPLUGIN_HH__

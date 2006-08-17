#ifndef __FASTJET_ACTIVEAREASPEC_HH__
#define __FASTJET_ACTIVEAREASPEC_HH__

#include<vector>
#include "fastjet/PseudoJet.hh"

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

//----------------------------------------------------------------------
/// Class that defines the parameters that go into the measurement
/// of active jet areas.
class ActiveAreaSpec {
public:
  /// default constructor
  ActiveAreaSpec(): _ghost_etamax(6.0), _repeat(5), _ghost_area(0.01), 
                    _grid_scatter(1e-4), _kt_scatter(0.1), 
                    _mean_ghost_kt(1e-100),
                    _actual_ghost_area(-1.0) {_initialize();};
  
  /// explicit constructor
  explicit ActiveAreaSpec(double ghost_etamax, 
                          int    repeat = 5,
                          double ghost_area=0.01, 
                          double grid_scatter = 1e-4, 
                          double kt_scatter = 0.1,
                          double mean_ghost_kt = 1e-100): 
    _ghost_etamax(ghost_etamax), 
    _repeat(repeat), 
    _ghost_area(ghost_area), 
    _grid_scatter(grid_scatter),  
    _kt_scatter(kt_scatter), 
    _mean_ghost_kt(mean_ghost_kt),
    _actual_ghost_area(-1.0) {_initialize();};


  /// does the initialization of actual ghost parameters
  void _initialize();

  // for accessing values set by the user
  inline double ghost_etamax() const {return _ghost_etamax;};
  inline double ghost_area   () const {return _ghost_area   ;};
  inline double grid_scatter() const {return _grid_scatter;};
  inline double kt_scatter  () const {return _kt_scatter  ;};
  inline double mean_ghost_kt() const {return _mean_ghost_kt  ;};
  inline int    repeat      () const {return _repeat      ;};

  // for accessing values 
  inline double actual_ghost_area() const {return _actual_ghost_area;};
  inline int    n_ghosts()          const {return _n_ghosts;};

  // when explicitly modifying values, sometimes call the initializer
  inline void set_ghost_area   (double val) {_ghost_area    = val; _initialize();};
  inline void set_ghost_etamax(double val) {_ghost_etamax = val; _initialize();};
  inline void set_grid_scatter(double val) {_grid_scatter   = val; };
  inline void set_kt_scatter  (double val) {_kt_scatter     = val; };
  inline void set_mean_ghost_kt(double val){_mean_ghost_kt  = val; };
  inline void set_repeat      (int    val) {_repeat         = val; };

  /// push the ghost 4-momenta onto the back of the vector of PseudoJets
  void add_ghosts(std::vector<PseudoJet> & ) const;

private:
  
  // quantities that determine nature and distribution of ghosts
  double _ghost_etamax;
  int    _repeat      ;
  double _ghost_area   ;  
  double _grid_scatter;
  double _kt_scatter  ;
  double _mean_ghost_kt;

  // derived quantities
  double _actual_ghost_area, _dphi, _deta;
  int    _n_ghosts, _nphi, _neta;
};

FASTJET_END_NAMESPACE

#endif // __FASTJET_ACTIVEAREASPEC_HH__


#ifndef __FJCLUSTERSEQUENCEWITHAREA__
#define __FJCLUSTERSEQUENCEWITHAREA__ 

#include "FjPseudoJet.hh"
#include "FjClusterSequence.hh"
#include<iostream>
#include<vector>



using namespace std;


//----------------------------------------------------------------------
/// Class that defines the parameters that go into the measurement
/// of active jet areas.
class FjActiveAreaSpec {
public:
  /// default constructor
  FjActiveAreaSpec(): _cell_area(0.01), _ghost_etamax(6.0), 
		      _grid_scatter(1e-5), _kt_scatter(0.1), 
		      _repeat(5), _actual_cell_area(-1.0) {_initialize();};
  
  /// explicit constructor
  FjActiveAreaSpec(double in_cell_area, double etamax_for_area,
        	   double in_grid_scatter = 0.00001, 
        	   double in_kt_scatter = 0.1, int in_repeat = 5): 
    _cell_area(in_cell_area), _ghost_etamax(etamax_for_area),
    _grid_scatter(in_grid_scatter),  _kt_scatter(in_kt_scatter), 
    _repeat(in_repeat), _actual_cell_area(-1.0) {_initialize();};


  /// does the initialization of actual cell parameters
  void _initialize();

  // for accessing values set by the user
  inline double cell_area   () const {return _cell_area   ;};
  inline double ghost_etamax() const {return _ghost_etamax;};
  inline double grid_scatter() const {return _grid_scatter;};
  inline double kt_scatter  () const {return _kt_scatter  ;};
  inline int    repeat      () const {return _repeat      ;};

  // for accessing values 
  inline double actual_cell_area() const {return _actual_cell_area;};
  inline int    n_cells()          const {return _n_cells;};

  // when explicitly modifying values, sometimes call the initializer
  inline void set_cell_area   (double val) {_cell_area    = val; _initialize();};
  inline void set_ghost_etamax(double val) {_ghost_etamax = val; _initialize();};
  inline void set_grid_scatter(double val) {_grid_scatter = val; };
  inline void set_kt_scatter  (double val) {_kt_scatter   = val; };
  inline void set_repeat      (int    val) {_repeat       = val; };

  /// adds the ghost 4-momenta to the vector of FjPseudoJet's
  void add_ghosts(vector<FjPseudoJet> & ) const;

private:
  
  // input quantities
  double _cell_area   ;  
  double _ghost_etamax;
  double _grid_scatter;
  double _kt_scatter  ;
  int    _repeat      ;

  // derived quantities
  double _actual_cell_area, _dphi, _deta;
  int    _n_cells, _nphi, _neta;
};


//======================================================================
/// Class that behaves essentially like FjClusterSequence except
/// that it also provides access to the area of a jet (which
/// will be a random quantity... Figure out what to do about seeds 
/// later...)
class FjClusterSequenceWithArea : public FjClusterSequence {
public:
  /// constructor using a FjActiveAreaSpec to specify how the area is
  /// to be measured
  template<class L> FjClusterSequenceWithArea
         (const std::vector<L> & pseudojets, 
          const FjJetDefinition & jet_def,
	  const FjActiveAreaSpec & area_spec,
	  const bool & writeout_combinations = false) 
	   : FjClusterSequence() {
	   _initialise(pseudojets,jet_def,area_spec,writeout_combinations); };

  /// constructor with a legacy interface
  template<class L> FjClusterSequenceWithArea
         (const std::vector<L> & pseudojets, 
	  double cell_area, double etamax_for_area,
	  double grid_scatter = 0.00001, double kt_scatter = 0.1,
	  const double & R = 1.0,
	  const FjStrategy & strategy = Best,
	  const bool & writeout_combinations = false);

  /// does the actual work of initialisation
  template<class L> void FjClusterSequenceWithArea::_initialise
         (const std::vector<L> & pseudojets, 
          const FjJetDefinition & jet_def,
	  const FjActiveAreaSpec & area_spec,
	  const bool & writeout_combinations); 

  //vector<FjPseudoJet> constituents (const FjPseudoJet & jet) const;

  /// returns the number of hard particles (i.e. those supplied by the user).
  unsigned int n_hard_particles() const;

  /// returns the area of a jet
  double area (const FjPseudoJet & jet) const;

  /// returns a four vector corresponding to the sum (E-scheme) of the
  /// ghost four-vectors composing the jet area, normalised such that
  /// for a small contiguous area the p_t of the extended_area jet is
  /// equal to area of the jet.
  FjPseudoJet extended_area (const FjPseudoJet & jet) const;

  /// true if a jet is made exclusively of ghosts
  bool is_pure_ghost(const FjPseudoJet & jet) const;
  bool is_pure_ghost(int history_index) const;

  /// returns the total area under study
  double total_area () const;
  
private:

  int    _n_cells;
  double _cell_area;
  vector<bool> _is_pure_ghost;
  vector<double> _areas;
  vector<FjPseudoJet> _extended_areas;
  
  unsigned int _initial_hard_n;


  ///// adds the "ghost" momenta, which will be used to estimate the
  ///// jet area
  //void _add_ghosts(double cell_area, double etamax_for_area,
  //		   double grid_scatter, double kt_scatter);

  /// also adds the "ghost" momenta, which will be used to estimate
  /// the jet area
  void _add_ghosts(const FjActiveAreaSpec & area_spec); 

  /// routine to be called after the processing is done so as to
  /// establish summary information on all the jets (areas, whether
  /// pure ghost, etc.)
  void _post_process();

};


//----------------------------------------------------------------------
// initialise from some generic type... Has to be made available
// here in order for the template aspect of it to work...
template<class L> FjClusterSequenceWithArea::FjClusterSequenceWithArea (
			          const std::vector<L> & pseudojets,
                        	  double cell_area, double etamax_for_area,
				  double grid_scatter, double kt_scatter,
				  const double & R,
				  const FjStrategy & strategy,
				  const bool & writeout_combinations) :
  FjClusterSequence() {

  FjActiveAreaSpec area_spec(cell_area,etamax_for_area,grid_scatter,kt_scatter);
  FjJetDefinition jet_def(_default_jet_finder, R, strategy);

  _initialise(pseudojets,jet_def,area_spec,writeout_combinations);

}

//----------------------------------------------------------------------
// initialise from some generic type... Has to be made available
// here in order for the template aspect of it to work...
template<class L> void FjClusterSequenceWithArea::_initialise
         (const std::vector<L> & pseudojets, 
          const FjJetDefinition & jet_def,
	  const FjActiveAreaSpec & area_spec,
	  const bool & writeout_combinations) {
  // don't reserve space yet -- will be done below

  // insert initial jets this way so that any type L that can be
  // converted to a pseudojet will work fine (basically FjPseudoJet
  // and any type that has [] subscript access to the momentum
  // components, such as CLHEP HepLorentzVector).
  for (unsigned int i = 0; i < pseudojets.size(); i++) {
    FjPseudoJet mom(pseudojets[i]);
    mom.set_user_index(0); // for user's particles (user index now lost...)
    _jets.push_back(mom);
    _is_pure_ghost.push_back(false);
  }

  _initial_hard_n = _jets.size();

  _add_ghosts(area_spec);

  if (writeout_combinations) {
    cout << "# Printing particles including ghosts\n";
    for (size_t j = 0; j < _jets.size(); j++) {
      printf("%5u %20.13f %20.13f %20.13e\n",
	       j,_jets[j].rap(),_jets[j].phi(),_jets[j].kt2());
    }
    cout << "# Finished printing particles including ghosts\n";
  }

  // this will ensure that we can still point to jets without
  // difficulties arising!
  _jets.reserve(_jets.size()*2);

  // run the clustering
  _initialise_and_run(jet_def,writeout_combinations);

  // set up all other information
  _post_process();
}


// //----------------------------------------------------------------------
// // initialise from some generic type... Has to be made available
// // here in order for the template aspect of it to work...
// template<class L> FjClusterSequenceWithArea::FjClusterSequenceWithArea (
// 			          const std::vector<L> & pseudojets,
//                         	  double cell_area, double etamax_for_area,
// 				  double grid_scatter, double kt_scatter,
// 				  const double & R,
// 				  const FjStrategy & strategy,
// 				  const bool & writeout_combinations) :
//   FjClusterSequence() {
// 
//   // don't reserve space yet -- will be done below
// 
//   // insert initial jets this way so that any type L that can be
//   // converted to a pseudojet will work fine (basically FjPseudoJet
//   // and any type that has [] subscript access to the momentum
//   // components, such as CLHEP HepLorentzVector).
//   for (unsigned int i = 0; i < pseudojets.size(); i++) {
//     FjPseudoJet mom(pseudojets[i]);
//     mom.set_user_index(0); // for user's particles (user index now lost...)
//     _jets.push_back(mom);
//     _is_pure_ghost.push_back(false);
//   }
// 
//   _initial_hard_n = _jets.size();
// 
//   _add_ghosts(cell_area, etamax_for_area, grid_scatter, kt_scatter);
// 
//   if (writeout_combinations) {
//     cout << "# Printing particles including ghosts\n";
//     for (size_t j = 0; j < _jets.size(); j++) {
//       printf("%5u %20.13f %20.13f %20.13e\n",
// 	       j,_jets[j].rap(),_jets[j].phi(),_jets[j].kt2());
//     }
//     cout << "# Finished printing particles including ghosts\n";
//   }
// 
//   // this will ensure that we can still point to jets without
//   // difficulties arising!
//   _jets.reserve(_jets.size()*2);
// 
//   // run the clustering
//   _initialise_and_run(R,strategy,writeout_combinations);
// 
//   // set up all other information
//   _post_process();
// }

inline unsigned int FjClusterSequenceWithArea::n_hard_particles() const {return _initial_hard_n;}


#endif // __FJCLUSTERSEQUENCEWITHAREA__ 

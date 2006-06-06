
#ifndef __FJCLUSTERSEQUENCEWITHAREA__
#define __FJCLUSTERSEQUENCEWITHAREA__ 

#include "FjPseudoJet.hh"
#include "FjClusterSequence.hh"
#include<iostream>
#include<vector>



using namespace std;



/// Class that behaves essentially like FjClusterSequence except
/// that it also provides access to the area of a jet (which
/// will be a random quantity... Figure out what to do about seeds 
/// later...)
class FjClusterSequenceWithArea : public FjClusterSequence {
public:
  template<class L> FjClusterSequenceWithArea
         (const std::vector<L> & pseudojets, 
	  double cell_area, double etamax_for_area,
	  double grid_scatter = 0.00001, double kt_scatter = 0.1,
	  const double & R = 1.0,
	  const FjStrategy & strategy = Best,
	  const bool & writeout_combinations = false);

  //vector<FjPseudoJet> constituents (const FjPseudoJet & jet) const;

  /// returns the number of hard particles (i.e. those supplied by the user).
  unsigned int n_hard_particles() const;


  /// returns the area of a jet
  double area (const FjPseudoJet & jet) const;

  /// true if a jet is made exclusively of ghosts
  bool is_pure_ghost(const FjPseudoJet & jet) const;
  bool is_pure_ghost(int hist_ix) const;

  /// returns the total area under study
  double total_area () const;
  
private:

  int    _n_cells;
  double _cell_area;
  vector<bool> _is_pure_ghost;
  vector<double> _areas;
  
  unsigned int _initial_hard_n;


  /// adds the "ghost" momenta, which will be used to estimate the
  /// jet area
  void _add_ghosts(double cell_area, double etamax_for_area,
		   double grid_scatter, double kt_scatter);

  /// routine to be called after the processing is done so as to
  /// esetablish summary information on all the jets (areas, whether
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

  _add_ghosts(cell_area, etamax_for_area, grid_scatter, kt_scatter);

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
  _initialise_and_run(R,strategy,writeout_combinations);

  // set up all other information
  _post_process();
}

inline unsigned int FjClusterSequenceWithArea::n_hard_particles() const {return _initial_hard_n;}


#endif // __FJCLUSTERSEQUENCEWITHAREA__ 

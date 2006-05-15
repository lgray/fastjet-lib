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
  double area (const FjPseudoJet & jet) const;
  double total_area () const;
  int n_particles() const {return _n_particles;};

private:

  int    _n_cells;
  double _cell_area;
  int    _n_particles;
  /// add the "ghost" momenta, which will be used to estimate the
  /// jet area
  void _add_ghosts(double cell_area, double etamax_for_area,
		   double grid_scatter, double kt_scatter);
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
  }

  _add_ghosts(cell_area, etamax_for_area, grid_scatter, kt_scatter);
  _n_particles = _jets.size();

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

  _initialise_and_run(R,strategy,writeout_combinations);
}



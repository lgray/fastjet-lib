#include "FjPseudoJet.hh"
#include "FjClusterSequence.hh"
#include<iostream>
#include<cmath>
#include <cstdlib>
#include<cassert>
#include<string>

using namespace std;


void FjClusterSequence::_initialise_and_run (
				  const double & R,
				  const FjStrategy & strategy,
				  const bool & writeout_combinations) {

  _print_banner();
  
  _writeout_combinations = writeout_combinations;
  _Rparam = R;  _R2 = _Rparam*_Rparam; _invR2 = 1.0/_R2;
  _strategy = strategy;

  // it's not too clear exactly what this does yet..
  _fill_initial_history();

  // automatically redefine the strategy according to N if that is
  // what the user requested
  if (_strategy == Best) {
    int N = _jets.size();
#ifndef DROP_CGAL
    if (N > 9000/_Rparam) { // empirical observation of how it scales with R
      _strategy = NlnN; }   // see GPS CCN27-57
    else
#endif  // DROP_CGAL
      if (N > 55*max(0.5,min(1.0,_Rparam))) {// empirical scaling with R
      _strategy = N2Tiled;
    } else {
      _strategy = N2Plain;
    }
  }

    
  // run the code containing the selected strategy
  if (_strategy == NlnN || _strategy == NlnN3pi 
      || _strategy == NlnN4pi ) {
    this->_delaunay_cluster();
  } else if (_strategy ==  N3Dumb ) {
    this->_really_dumb_cluster();
  } else if (_strategy == N2Tiled) {
    this->_faster_tiled_N2_cluster();
  } else if (_strategy == N2PoorTiled) {
    this->_tiled_N2_cluster();
  } else if (_strategy == N2Plain) {
    this->_simple_N2_cluster();
  } else {
    cerr << "Unrecognised value for strategy: "<<_strategy<<"\n";
    assert(false);
  }
}


// this needs to be defined outside the class definition.
bool FjClusterSequence::_first_time = true;
//----------------------------------------------------------------------
// prints a banner on the first call
void FjClusterSequence::_print_banner() {

  if (!_first_time) {return;}
  _first_time = false;
  
  cout << "#---------------------------------------------------------------------\n";
  cout << "#                      FastJet release 1.0   			 \n";
  cout << "#            Written by Matteo Cacciari and Gavin Salam		 \n"; 
  cout << "#            http://www.lpthe.jussieu.fr/~salam/fastjet		 \n"; 
  cout << "#								      	 \n";
  cout << "# Longitudinally invariant Kt Jet clustering, using the fast geometric\n";
  cout << "# algorithms of hep-ph/0512210 (please cite this if you use FastJet)  \n";
#ifndef DROP_CGAL
  cout << "# This package makes use of the CGAL library: http://www.cgal.org/    \n";
#endif  // DROP_CGAL
  cout << "#---------------------------------------------------------------------\n";
}

//----------------------------------------------------------------------
// Return the component corresponding to the specified index.
// taken from CLHEP
string FjClusterSequence::strategy_string ()  const {
  string strategy;
  switch(_strategy) {
  case NlnN:
    strategy = "NlnN"; break;
  case NlnN3pi:
    strategy = "NlnN3pi"; break;
  case NlnN4pi:
    strategy = "NlnN4pi"; break;
  case N2Plain:
    strategy = "N2Plain"; break;
  case N2Tiled:
    strategy = "N2Tiled"; break;
  case N2PoorTiled:
    strategy = "N2PoorTiled"; break;
  case N3Dumb:
    strategy = "N3Dumb"; break;
  default:
    strategy = "Unrecognized";
  }
  return strategy;
}  


//----------------------------------------------------------------------
// return all inclusive jets with pt > ptmin
vector<FjPseudoJet> FjClusterSequence::inclusive_jets (const double & ptmin) const{
  double dcut = ptmin*ptmin;
  int i = _history.size() - 1; // last jet
  vector<FjPseudoJet> jets;
  while (i >= 0) {
    if (_history[i].max_dij_so_far < dcut) {break;}
    if (_history[i].parent2 == BeamJet && _history[i].dij >= dcut) {
      // for beam jets
      int parent1 = _history[i].parent1;
      jets.push_back(_jets[_history[parent1].jetp_index]);}
    i--;
  }
  return jets;
}


//----------------------------------------------------------------------
// return the number of exclusive jets that would have been obtained
// running the algorithm in exclusive mode with the given dcut
int FjClusterSequence::n_exclusive_jets (const double & dcut) const {

  // first locate the point where clustering would have stopped (i.e. the
  // first time max_dij_so_far > dcut)
  int i = _history.size() - 1; // last jet
  while (i >= 0) {
    if (_history[i].max_dij_so_far <= dcut) {break;}
    i--;
  }
  int stop_point = i + 1;
  // relation between stop_point, njets assumes one extra jet disappears
  // at each clustering.
  int njets = 2*_initial_n - stop_point;
  return njets;
}

//----------------------------------------------------------------------
// return all exclusive jets that would have been obtained running
// the algorithm in exclusive mode with the given dcut
vector<FjPseudoJet> FjClusterSequence::exclusive_jets (const double & dcut) const {
  int njets = n_exclusive_jets(dcut);
  return exclusive_jets(njets);
}


//----------------------------------------------------------------------
// return the jets obtained by clustering the event to n jets.
vector<FjPseudoJet> FjClusterSequence::exclusive_jets (const int & njets) const {

  // make sure the user does not ask for more than jets than there
  // were particles in the first place.
  assert (njets <= _initial_n);

  // calculate the point where we have to stop the clustering.
  // relation between stop_point, njets assumes one extra jet disappears
  // at each clustering.
  int stop_point = 2*_initial_n - njets;

  // some sanity checking to make sure that e+e- does not give us
  // surprises (should we ever implement e+e-)...
  if (2*_initial_n != static_cast<int>(_history.size())) {
    cerr << "2*_initial_n != _history.size() -- this endangers internal assumptions!\n";
    assert(false);
  }

  // now go forwards and reconstitute the jets that we have --
  // basically for any history element, see if the parent jets to
  // which it refers were created before the stopping point -- if they
  // were then add them to the list, otherwise they are subsequent
  // recombinations of the jets that we are looking for.
  vector<FjPseudoJet> jets;
  for (unsigned int i = stop_point; i < _history.size(); i++) {
    int parent1 = _history[i].parent1;
    if (parent1 < stop_point) {
      jets.push_back(_jets[_history[parent1].jetp_index]);
    }
    int parent2 = _history[i].parent2;
    if (parent2 < stop_point && parent2 > 0) {
      jets.push_back(_jets[_history[parent2].jetp_index]);
    }
    
  }
  return jets;

  // sanity check...
  if (static_cast<int>(jets.size()) != njets) {
    cerr << "FjClusterSequence::exclusive_jets: size of returned vector ("
	 <<jets.size()<<") does not coincide with requested number of jets ("
	 <<njets<<")\n";
  }
}

//----------------------------------------------------------------------
/// return the dmin corresponding to the recombination that went from
/// n+1 to n jets
double FjClusterSequence::exclusive_dmerge (const int & njets) const {
  assert(njets > 0);
  if (njets >= _initial_n) {return 0.0;}
  return _history[2*_initial_n-njets-1].dij;
}


//----------------------------------------------------------------------
/// return the maximum of the dmin encountered during all recombinations 
/// up to the one that led to an n-jet final state; identical to
/// exclusive_dmerge, except in cases where the dmin do not increase
/// monotonically.
double FjClusterSequence::exclusive_dmerge_max (const int & njets) const {
  assert(njets > 0);
  if (njets >= _initial_n) {return 0.0;}
  return _history[2*_initial_n-njets-1].max_dij_so_far;
}


//----------------------------------------------------------------------
// return a vector of the particles that make up a jet
vector<FjPseudoJet> FjClusterSequence::constituents (const FjPseudoJet & jet) const {
  vector<FjPseudoJet> subjets;
  add_constituents(jet, subjets);
  return subjets;
}


//----------------------------------------------------------------------
// recursive routine that adds on constituents of jet to the subjet_vector
void FjClusterSequence::add_constituents (
           const FjPseudoJet & jet, vector<FjPseudoJet> & subjet_vector) const {
  // find out position in cluster history
  int i = jet.cluster_hist_index();
  int parent1 = _history[i].parent1;
  int parent2 = _history[i].parent2;

  if (parent1 == InexistentParent) {
    // It is an original particle (labelled by its parent having value
    // InexistentParent), therefore add it on to the subjet vector
    subjet_vector.push_back(jet);
    return;
  } 

  // add parent 1
  add_constituents(_jets[_history[parent1].jetp_index], subjet_vector);

  // see if parent2 is a real jet; if it is then add its constituents
  if (parent2 != BeamJet) {
    add_constituents(_jets[_history[parent2].jetp_index], subjet_vector);
  }
}


//----------------------------------------------------------------------
// initialise the history in a standard way
void FjClusterSequence::_fill_initial_history () {
  for (int i = 0; i < static_cast<int>(_jets.size()) ; i++) {
    history_element element;
    element.parent1 = InexistentParent;
    element.parent2 = InexistentParent;
    element.child   = Invalid;
    element.jetp_index = i;
    element.dij     = 0.0;
    element.max_dij_so_far = 0.0;

    _history.push_back(element);
    
    // get cross-referencing right from FjPseudoJets
    _jets[i].set_cluster_hist_index(i);
  }
  _initial_n = _jets.size();
}

//----------------------------------------------------------------------
// initialise the history in a standard way
void FjClusterSequence::_add_step_to_history (
	       const int & step_number, const int & parent1, 
	       const int & parent2, const int & jetp_index,
	       const double & dij) {

  history_element element;
  element.parent1 = parent1;
  element.parent2 = parent2;
  element.jetp_index = jetp_index;
  element.child = Invalid;
  element.dij   = dij;
  element.max_dij_so_far = max(dij,_history[_history.size()-1].max_dij_so_far);
  _history.push_back(element);

  int local_step = _history.size()-1;
  assert(local_step == step_number);

  assert(parent1 >= 0);
  _history[parent1].child = local_step;
  if (parent2 >= 0) {_history[parent2].child = local_step;}

  // get cross-referencing right from FjPseudoJets
  if (jetp_index != Invalid) {
    assert(jetp_index >= 0);
    //cout << _jets.size() <<" "<<jetp_index<<"\n";
    _jets[jetp_index].set_cluster_hist_index(local_step);
  }

  if (_writeout_combinations) {
    cout << local_step << ": " 
	 << parent1 << " with " << parent2
	 << "; y = "<< dij<<endl;
  }

}


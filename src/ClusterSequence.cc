//STARTHEADER
// $Id$
//
// Copyright (c) 2005-2006, Matteo Cacciari and Gavin Salam
//
//----------------------------------------------------------------------
// This file is part of FastJet.
//
//  FastJet is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  The algorithms that underlie FastJet have required considerable
//  development and are described in hep-ph/0512210. If you use
//  FastJet as part of work towards a scientific publication, please
//  include a citation to the FastJet paper.
//
//  FastJet is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with FastJet; if not, write to the Free Software
//  Foundation, Inc.:
//      59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//----------------------------------------------------------------------
//ENDHEADER

#include "fastjet/Error.hh"
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include<iostream>
#include<sstream>
#include<cmath>
#include<cstdlib>
#include<cassert>
#include<string>

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

using namespace std;

//// initialised static member has to go in the .cc code
JetFinder ClusterSequence::_default_jet_finder = kt_algorithm;
//


//----------------------------------------------------------------------
void ClusterSequence::_initialise_and_run (
				  const double & R,
				  const Strategy & strategy,
				  const bool & writeout_combinations) {

  JetDefinition jet_def(_default_jet_finder, R, strategy);
  _initialise_and_run(jet_def, writeout_combinations);
}


//----------------------------------------------------------------------
void ClusterSequence::_initialise_and_run (
				  const JetDefinition & jet_def,
				  const bool & writeout_combinations) {

  // transfer all relevant info into internal variables
  _decant_options(jet_def, writeout_combinations);

  // set up the history entries for the initial particles (those
  // currently in _jets)
  _fill_initial_history();

//  // automatically redefine the strategy according to N if that is
//  // what the user requested
//  if (_strategy == Best) {
//    int N = _jets.size();
//#ifndef DROP_CGAL
//    if (N > 14500/_Rparam) { // empirical observation of how it scales with R
//      _strategy = NlnN; }    // see GPS CCN27-57 (Numbers have changed since 
//    else                     // introducing N2MinHeapTiled; scaling is approx.)
//#endif  // DROP_CGAL
//      if (N > 450) {
//      _strategy = N2MinHeapTiled;
//    }
//    else if (N > 55*max(0.5,min(1.0,_Rparam))) {// empirical scaling with R
//      _strategy = N2Tiled;
//    } else {
//      _strategy = N2Plain;
//    }
//  }


  // automatically redefine the strategy according to N if that is
  // what the user requested -- transition points (and especially
  // their R-dependence) are based on empirical observations for a
  // R=0.4, 0.7 and 1.0, running on toth (3.4GHz, Pentium IV D [dual
  // core] with 2MB of cache).
  if (_strategy == Best) {
    int N = _jets.size();
    if (N > 6200/pow(_Rparam,2.0) 
	&& jet_def.jet_finder() == cambridge_algorithm) {
      _strategy = NlnNCam;}
    else
#ifndef DROP_CGAL
    if (N > 16000/pow(_Rparam,1.15)) {
      _strategy = NlnN; }   
    else                    
#endif  // DROP_CGAL
      if (N > 450) {
      _strategy = N2MinHeapTiled;
    }
    else if (N > 55*max(0.5,min(1.0,_Rparam))) {// empirical scaling with R
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
  } else if (_strategy == N2MinHeapTiled) {
    this->_minheap_faster_tiled_N2_cluster();
  } else if (_strategy == NlnNCam4pi) {
    this->_CP2DChan_cluster();
  } else if (_strategy == NlnNCam2pi2R) {
    this->_CP2DChan_cluster_2pi2R();
  } else if (_strategy == NlnNCam) {
    this->_CP2DChan_cluster_2piMultD();
  } else {
    ostringstream err;
    err << "Unrecognised value for strategy: "<<_strategy;
    throw Error(err.str());
    //assert(false);
  }
}


// this needs to be defined outside the class definition.
bool ClusterSequence::_first_time = true;
//----------------------------------------------------------------------
// prints a banner on the first call
void ClusterSequence::_print_banner() {

  if (!_first_time) {return;}
  _first_time = false;
  
  cout << "#---------------------------------------------------------------------\n";
  cout << "#                        FastJet release 2.0    			 \n";
  cout << "#            Written by Matteo Cacciari and Gavin Salam		 \n"; 
  cout << "#            http://www.lpthe.jussieu.fr/~salam/fastjet		 \n"; 
  cout << "#								      	 \n";
  cout << "# Longitudinally invariant Kt Jet clustering, using the fast geometric\n";
  cout << "# algorithms of hep-ph/0512210 (please cite this if you use FastJet)  \n";
#ifndef DROP_CGAL
  cout << "# This package makes use of the CGAL library: http://www.cgal.org/    \n";
#endif  // DROP_CGAL
  cout << "# This package incorporates T.Chan's closest-pair algorithm, Proc. 13th     \n";
  cout << "# ACM-SIAM Symposium on Discrete Algorithms, pp.472-473, 2002\n";
  cout << "#---------------------------------------------------------------------\n";
}

//----------------------------------------------------------------------
// transfer all relevant info into internal variables
void ClusterSequence::_decant_options(const JetDefinition & jet_def,
                                      const bool & writeout_combinations) {

  // let the user know what's going on
  _print_banner();

  // make a local copy of the jet definition (for future use?)
  _jet_def = jet_def;
  
  _writeout_combinations = writeout_combinations;
  _jet_finder = jet_def.jet_finder();
  _Rparam = jet_def.R();  _R2 = _Rparam*_Rparam; _invR2 = 1.0/_R2;
  _strategy = jet_def.strategy();
}


//----------------------------------------------------------------------
// initialise the history in a standard way
void ClusterSequence::_fill_initial_history () {

  if (_jets.size() == 0) {throw Error("Cannot run jet-finder on empty event");}

  // reserve sufficient space for everything
  _jets.reserve(_jets.size()*2);
  _history.reserve(_jets.size()*2);

  for (int i = 0; i < static_cast<int>(_jets.size()) ; i++) {
    history_element element;
    element.parent1 = InexistentParent;
    element.parent2 = InexistentParent;
    element.child   = Invalid;
    element.jetp_index = i;
    element.dij     = 0.0;
    element.max_dij_so_far = 0.0;

    _history.push_back(element);
    
    // get cross-referencing right from PseudoJets
    _jets[i].set_cluster_hist_index(i);
  }
  _initial_n = _jets.size();
}


//----------------------------------------------------------------------
// Return the component corresponding to the specified index.
// taken from CLHEP
string ClusterSequence::strategy_string ()  const {
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
  case N2MinHeapTiled:
    strategy = "N2MinHeapTiled"; break;
  case N2PoorTiled:
    strategy = "N2PoorTiled"; break;
  case N3Dumb:
    strategy = "N3Dumb"; break;
  case NlnNCam4pi:
    strategy = "NlnNCam4pi"; break;
  case NlnNCam2pi2R:
    strategy = "NlnNCam2pi2R"; break;
  case NlnNCam:
    strategy = "NlnNCam"; break; // 2piMultD
  default:
    strategy = "Unrecognized";
  }
  return strategy;
}  


//----------------------------------------------------------------------
// return all inclusive jets with pt > ptmin
vector<PseudoJet> ClusterSequence::inclusive_jets (const double & ptmin) const{
  double dcut = ptmin*ptmin;
  int i = _history.size() - 1; // last jet
  vector<PseudoJet> jets;
  if (_jet_finder == kt_algorithm) {
    while (i >= 0) {
      // with our specific definition of dij and diB (i.e. R appears only in 
      // dij), then dij==diB is the same as the jet.perp2() and we can exploit
      // this in selecting the jets...
      if (_history[i].max_dij_so_far < dcut) {break;}
      if (_history[i].parent2 == BeamJet && _history[i].dij >= dcut) {
	// for beam jets
	int parent1 = _history[i].parent1;
	jets.push_back(_jets[_history[parent1].jetp_index]);}
      i--;
    }
  } else if (_jet_finder == cambridge_algorithm) {
    while (i >= 0) {
      // inclusive jets are all at end of clustering sequence in the
      // Cambridge algorithm -- so if we find a non-exclusive jet, then
      // we can exit
      if (_history[i].parent2 != BeamJet) {break;}
      int parent1 = _history[i].parent1;
      const PseudoJet & jet = _jets[_history[parent1].jetp_index];
      if (jet.perp2() >= dcut) {jets.push_back(jet);}
      i--;
    }
  } else {throw Error("Unrecognized jet algorithm");}
  return jets;
}


//----------------------------------------------------------------------
// return the number of exclusive jets that would have been obtained
// running the algorithm in exclusive mode with the given dcut
int ClusterSequence::n_exclusive_jets (const double & dcut) const {

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
vector<PseudoJet> ClusterSequence::exclusive_jets (const double & dcut) const {
  int njets = n_exclusive_jets(dcut);
  return exclusive_jets(njets);
}


//----------------------------------------------------------------------
// return the jets obtained by clustering the event to n jets.
vector<PseudoJet> ClusterSequence::exclusive_jets (const int & njets) const {

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
    ostringstream err;
    err << "2*_initial_n != _history.size() -- this endangers internal assumptions!\n";
    throw Error(err.str());
    //assert(false);
  }

  // now go forwards and reconstitute the jets that we have --
  // basically for any history element, see if the parent jets to
  // which it refers were created before the stopping point -- if they
  // were then add them to the list, otherwise they are subsequent
  // recombinations of the jets that we are looking for.
  vector<PseudoJet> jets;
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

  // sanity check...
  if (static_cast<int>(jets.size()) != njets) {
    ostringstream err;
    err << "ClusterSequence::exclusive_jets: size of returned vector ("
	 <<jets.size()<<") does not coincide with requested number of jets ("
	 <<njets<<")";
    throw Error(err.str());
  }

  return jets;
}

//----------------------------------------------------------------------
/// return the dmin corresponding to the recombination that went from
/// n+1 to n jets
double ClusterSequence::exclusive_dmerge (const int & njets) const {
  assert(njets > 0);
  if (njets >= _initial_n) {return 0.0;}
  return _history[2*_initial_n-njets-1].dij;
}


//----------------------------------------------------------------------
/// return the maximum of the dmin encountered during all recombinations 
/// up to the one that led to an n-jet final state; identical to
/// exclusive_dmerge, except in cases where the dmin do not increase
/// monotonically.
double ClusterSequence::exclusive_dmerge_max (const int & njets) const {
  assert(njets > 0);
  if (njets >= _initial_n) {return 0.0;}
  return _history[2*_initial_n-njets-1].max_dij_so_far;
}


//----------------------------------------------------------------------
// return a vector of the particles that make up a jet
vector<PseudoJet> ClusterSequence::constituents (const PseudoJet & jet) const {
  vector<PseudoJet> subjets;
  add_constituents(jet, subjets);
  return subjets;
}


//----------------------------------------------------------------------
// recursive routine that adds on constituents of jet to the subjet_vector
void ClusterSequence::add_constituents (
           const PseudoJet & jet, vector<PseudoJet> & subjet_vector) const {
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
void ClusterSequence::_add_step_to_history (
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

  // get cross-referencing right from PseudoJets
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




//======================================================================
// Return an order in which to read the history such that _history[order[i]] 
// will always correspond to the same set of consituent particles if 
// two branching histories are equivalent in terms of the particles
// contained in any given pseudojet.
vector<int> ClusterSequence::unique_history_order() const {

  // first construct an array that will tell us the lowest constituent
  // of a given jet -- this will always be one of the original
  // particles, whose order is well defined and so will help us to
  // follow the tree in a unique manner.
  valarray<int> lowest_constituent(_history.size());
  int hist_n = _history.size();
  lowest_constituent = hist_n; // give it a large number
  for (int i = 0; i < hist_n; i++) {
    // sets things up for the initial partons
    lowest_constituent[i] = min(lowest_constituent[i],i); 
    // propagates them through to the children of this parton
    if (_history[i].child > 0) lowest_constituent[_history[i].child] 
      = min(lowest_constituent[_history[i].child],lowest_constituent[i]);
  }

  // establish an array for what we have and have not extracted so far
  valarray<bool> extracted(_history.size()); extracted = false;
  vector<int> unique_tree;
  unique_tree.reserve(_history.size());

  // now work our way through the tree
  for (unsigned i = 0; i < n_particles(); i++) {
    if (!extracted[i]) {
      unique_tree.push_back(i);
      extracted[i] = true;
      _extract_tree_children(i, extracted, lowest_constituent, unique_tree);
    }
  }

  return unique_tree;
}

//======================================================================
// helper for unique_history_order
void ClusterSequence::_extract_tree_children(
       int position, 
       valarray<bool> & extracted, 
       const valarray<int> & lowest_constituent,
       vector<int> & unique_tree) const {
  if (!extracted[position]) {
    // that means we may have unidentified parents around, so go and
    // collect them (extracted[position]) will then be made true)
    _extract_tree_parents(position,extracted,lowest_constituent,unique_tree);
  } 
  
  // now look after the children...
  int child = _history[position].child;
  if (child  >= 0) _extract_tree_children(child,extracted,lowest_constituent,unique_tree);
}

//======================================================================
// helper for unique_history_order
void ClusterSequence::_extract_tree_parents(
       int position, 
       valarray<bool> & extracted, 
       const valarray<int> & lowest_constituent,
       vector<int> & unique_tree) const {

  if (!extracted[position]) {
    int parent1 = _history[position].parent1;
    int parent2 = _history[position].parent2;
    // where relevant order parents so that we will first treat the
    // one containing the smaller "lowest_constituent"
    if (parent1 >= 0 && parent2 >= 0) {
      if (lowest_constituent[parent1] > lowest_constituent[parent2]) 
	swap(parent1, parent2);
    }
    // then actually run through the parents to extract the constituents...
    if (parent1 >= 0 && !extracted[parent1]) 
      _extract_tree_parents(parent1,extracted,lowest_constituent,unique_tree);
    if (parent2 >= 0 && !extracted[parent2]) 
      _extract_tree_parents(parent2,extracted,lowest_constituent,unique_tree);
    // finally declare this position to be accounted for and push it
    // onto our list.
    unique_tree.push_back(position);
    extracted[position] = true;
  }
}


//======================================================================
/// carries out the bookkeeping associated with the step of recombining
/// jet_i and jet_j (assuming a distance dij) and returns the index
/// of the recombined jet, newjet_k.
void ClusterSequence::_do_ij_recombination_step(
                               const int & jet_i, const int & jet_j, 
			       const double & dij, 
			       int & newjet_k) {

  // create the new jet
  _jets.push_back(_jets[jet_i] + _jets[jet_j]);

  // get its index
  newjet_k = _jets.size()-1;

  // get history index
  int newstep_k = _history.size();
  // and provide jet with the info
  _jets[newjet_k].set_cluster_hist_index(newstep_k);

  // finally sort out the history 
  int hist_i = _jets[jet_i].cluster_hist_index();
  int hist_j = _jets[jet_j].cluster_hist_index();

  _add_step_to_history(newstep_k, min(hist_i, hist_j), max(hist_i,hist_j),
		       newjet_k, dij);

}


//======================================================================
/// carries out the bookkeeping associated with the step of recombining
/// jet_i with the beam
void ClusterSequence::_do_iB_recombination_step(
				  const int & jet_i, const double & diB) {
  // get history index
  int newstep_k = _history.size();

  // recombine the jet with the beam
  _add_step_to_history(newstep_k,_jets[jet_i].cluster_hist_index(),BeamJet,
		       Invalid, diB);

}

FASTJET_END_NAMESPACE


#include "fastjet/ClusterSequenceActiveAreaExplicitGhosts.hh"


using namespace std;

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

// save some typing
typedef ClusterSequenceActiveAreaExplicitGhosts ClustSeqActAreaEG;



//----------------------------------------------------------------------
///
void ClustSeqActAreaEG::_add_ghosts (
			 const ActiveAreaSpec & area_spec) {

  // add the ghosts to the jets
  area_spec.add_ghosts(_jets);

  // now add labelling...
  for (unsigned i = _initial_hard_n; i < _jets.size(); i++) {
    //_jets[i].set_user_index(1);
    _is_pure_ghost.push_back(true);
  }

  // and record some info from the area_spec
  _ghost_area = area_spec.actual_ghost_area();
  _n_ghosts   = area_spec.n_ghosts();
}


//----------------------------------------------------------------------
// return the area of a jet
double ClustSeqActAreaEG::area (const PseudoJet & jet) const {
  return _areas[jet.cluster_hist_index()];
}


//----------------------------------------------------------------------
// return the total area
double ClustSeqActAreaEG::total_area () const {
  return _n_ghosts * _ghost_area;
}


//----------------------------------------------------------------------
// return the extended area of a jet
PseudoJet ClustSeqActAreaEG::area_4vector (const PseudoJet & jet) const {
  return _area_4vectors[jet.cluster_hist_index()];
}

//----------------------------------------------------------------------
bool ClustSeqActAreaEG::is_pure_ghost(const PseudoJet & jet) const 
{
  return _is_pure_ghost[jet.cluster_hist_index()];
}

//----------------------------------------------------------------------
bool ClustSeqActAreaEG::is_pure_ghost(int hist_ix) const 
{
  return _is_pure_ghost[hist_ix];
}


//======================================================================
// sort out the areas
void ClustSeqActAreaEG::_post_process() {

  // sort out sizes
  _areas.resize(_history.size());
  _area_4vectors.resize(_history.size());
  _is_pure_ghost.resize(_history.size());
  
  // First set up areas for the initial particles (ghost=_ghost_area,
  // real particles = 0); recall that _initial_n here is the number of
  // particles including ghosts
  for (int i = 0; i < _initial_n; i++) {
    if (_is_pure_ghost[i]) {
      _areas[i] = _ghost_area;
      // normalise pt to be _ghost_area (NB we make use of fact that
      // for initial particles, jet and clust_hist index are the same).
      _area_4vectors[i] = (_ghost_area/_jets[i].perp()) * _jets[i];
    } else {
      _areas[i] = 0;
      _area_4vectors[i] = PseudoJet(0.0,0.0,0.0,0.0);
    }
  }
  
  // next follow the branching through and set up the areas 
  // and ghost-nature at each step of the clustering (rather than
  // each jet).
  for (unsigned i = _initial_n; i < _history.size(); i++) {
    if (_history[i].parent2 == BeamJet) {
      _is_pure_ghost[i]  = _is_pure_ghost[_history[i].parent1];
      _areas[i]          = _areas[_history[i].parent1];
      _area_4vectors[i] = _area_4vectors[_history[i].parent1];
    } else {
      _is_pure_ghost[i]  = _is_pure_ghost[_history[i].parent1] && 
	                   _is_pure_ghost[_history[i].parent2]   ;
      _areas[i]          = _areas[_history[i].parent1] + 
	                   _areas[_history[i].parent2]  ;
      _area_4vectors[i] = _area_4vectors[_history[i].parent1] + 
	                   _area_4vectors[_history[i].parent2]  ;
    }

  }
  
}

FASTJET_END_NAMESPACE


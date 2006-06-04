#include "FjClusterSequenceWithArea.hh"

void FjClusterSequenceWithArea::_add_ghosts (double cell_area, 
	     double etamax_for_area, double grid_scatter, double kt_scatter) {
  // add on area-measuring dummy particles
  double deta = sqrt(cell_area);
  double dphi = deta;
  int nphi = int(ceil(twopi/dphi)); dphi = twopi/nphi;
  int neta = int(ceil(etamax_for_area/deta)); deta = etamax_for_area / neta;
  _cell_area = dphi * deta;
  _n_cells   = (2*neta+1)*nphi;

  // add momenta for cells
  for (int ieta = -neta; ieta <= neta; ieta++) {
    for (int iphi = 0; iphi < nphi; iphi++) {
      // include random offsets for all quantities
      double phi = (iphi+0.5) * dphi + rand()*grid_scatter/RAND_MAX;
      double eta = ieta * deta + rand()*grid_scatter/RAND_MAX;
      double kt = 1.0e-100*(1+rand()*kt_scatter/RAND_MAX);
      //double kt = 0.0000001*(1+rand()*10.0/RAND_MAX);
     
      double pminus = kt*exp(-eta);
      double pplus  = kt*exp(+eta);
      double px = kt*sin(phi);
      double py = kt*cos(phi);
      //cout << kt<<" "<<eta<<" "<<phi<<"\n";
      //if (phi>=twopi || phi < 0.0) cout << "Hey: "<< phi-twopi<<"\n";
      FjPseudoJet mom(px,py,0.5*(pplus-pminus),0.5*(pplus+pminus));
      mom.set_user_index(1);  // for ghost particles (user index now lost...)
      _jets.push_back(mom);
      _is_pure_ghost.push_back(true);
    }
  }
}

// return the area of a jet
double FjClusterSequenceWithArea::area (const FjPseudoJet & jet) const {
  return _areas[jet.cluster_hist_index()];
}

// return the total area
double FjClusterSequenceWithArea::total_area () const {
  return _n_cells * _cell_area;
}


bool FjClusterSequenceWithArea::is_pure_ghost(const FjPseudoJet & jet) const 
{
  return _is_pure_ghost[jet.cluster_hist_index()];
}

bool FjClusterSequenceWithArea::is_pure_ghost(int hist_ix) const 
{
  return _is_pure_ghost[hist_ix];
}


//======================================================================
// sort out the areas
void FjClusterSequenceWithArea::_post_process() {

  // sort out sizes
  _areas.resize(_history.size());
  _is_pure_ghost.resize(_history.size());
  
  // First set up areas for the initial particles (ghost=_cell_area,
  // real particles = 0); recall that _initial_n here is the number of
  // particles including ghosts
  for (int i = 0; i < _initial_n; i++) {
    _areas[i] = _is_pure_ghost[i] ? _cell_area : 0.0;
  }
  
  // next follow the branching through and set up the areas 
  // and ghost-nature at each step of the clustering (rather than
  // each jet).
  for (unsigned i = _initial_n; i < _history.size(); i++) {
    if (_history[i].parent2 == BeamJet) {
      _is_pure_ghost[i] = _is_pure_ghost[_history[i].parent1];
      _areas[i]         = _areas[_history[i].parent1];
    } else {
      _is_pure_ghost[i] = _is_pure_ghost[_history[i].parent1] && 
	                  _is_pure_ghost[_history[i].parent2]   ;
      _areas[i]         = _areas[_history[i].parent1] + 
	                  _areas[_history[i].parent2]  ;
    }

  }
  
}

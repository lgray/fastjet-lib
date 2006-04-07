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
      double kt = 0.000000001*(1+rand()*kt_scatter/RAND_MAX);
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
    }
  }
}

// return the area of a jet
double FjClusterSequenceWithArea::area (const FjPseudoJet & jet) const {
  
  vector<FjPseudoJet> jets = constituents(jet);
  int count = 0;
  for (unsigned int i = 0; i< jets.size(); i++) {
    if (jets[i].user_index() == 1) count++;
  }
  return count * _cell_area;
}

double FjClusterSequenceWithArea::total_area () const {
  return _n_cells * _cell_area;
}

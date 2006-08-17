#include "fastjet/ActiveAreaSpec.hh"

using namespace std;

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

//======================================================================
/// sets the detailed parameters for the ghosts (which may not be quite
/// the same as those requested -- this is in order for things to fit
/// in nicely into 2pi etc...
void ActiveAreaSpec::_initialize() {
  // add on area-measuring dummy particles
  _deta = sqrt(_ghost_area);
  _dphi = _deta;
  _nphi = int(ceil(twopi/_dphi)); _dphi = twopi/_nphi;
  _neta = int(ceil(_ghost_etamax/_deta)); _deta = _ghost_etamax / _neta;
  _actual_ghost_area = _dphi * _deta;
  _n_ghosts   = (2*_neta+1)*_nphi;
}

//----------------------------------------------------------------------
/// adds the ghost 4-momenta to the vector of PseudoJet's
void ActiveAreaSpec::add_ghosts(vector<PseudoJet> & event) const {
  // add momenta for ghosts
  for (int ieta = -_neta; ieta <= _neta; ieta++) {
    for (int iphi = 0; iphi < _nphi; iphi++) {
      // include random offsets for all quantities
      double phi = (iphi+0.5) * _dphi*(1 + rand()*_grid_scatter/RAND_MAX);
      double eta = ieta * _deta*(1 + rand()*_grid_scatter/RAND_MAX);
      double kt = _mean_ghost_kt*(1+rand()*_kt_scatter/RAND_MAX);
     
      double pminus = kt*exp(-eta);
      double pplus  = kt*exp(+eta);
      double px = kt*sin(phi);
      double py = kt*cos(phi);
      //cout << kt<<" "<<eta<<" "<<phi<<"\n";
      //if (phi>=twopi || phi < 0.0) cout << "Hey: "<< phi-twopi<<"\n";
      PseudoJet mom(px,py,0.5*(pplus-pminus),0.5*(pplus+pminus));
      //mom.set_user_index(1);  // for ghost particles (user index now lost...)
      event.push_back(mom);
      //_is_pure_ghost.push_back(true);
    }
  }
}

FASTJET_END_NAMESPACE


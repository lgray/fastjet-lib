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
      double phi = (iphi+0.5) * _dphi + _dphi*rand()*_grid_scatter/RAND_MAX;
      double eta = ieta * _deta + _deta*rand()*_grid_scatter/RAND_MAX;
      //double phi = (iphi+0.5) * _dphi* + rand()*_grid_scatter/RAND_MAX;
      //double eta = ieta * _deta + rand()*_grid_scatter/RAND_MAX;
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


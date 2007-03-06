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
#include<iostream>
#include<sstream>

using namespace std;

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

BasicRandom<double> ActiveAreaSpec::_random_generator;

//======================================================================
/// sets the detailed parameters for the ghosts (which may not be quite
/// the same as those requested -- this is in order for things to fit
/// in nicely into 2pi etc...
void ActiveAreaSpec::_initialize() {
  // add on area-measuring dummy particles
  _drap = sqrt(_ghost_area);
  _dphi = _drap;
  _nphi = int(ceil(twopi/_dphi)); _dphi = twopi/_nphi;
  _nrap = int(ceil(_ghost_maxrap/_drap)); _drap = _ghost_maxrap / _nrap;
  _actual_ghost_area = _dphi * _drap;
  _n_ghosts   = (2*_nrap+1)*_nphi;

  //_random_generator.info(cerr);
}

//----------------------------------------------------------------------
/// adds the ghost 4-momenta to the vector of PseudoJet's
void ActiveAreaSpec::add_ghosts(vector<PseudoJet> & event) const {
  // add momenta for ghosts
  for (int irap = -_nrap; irap <= _nrap; irap++) {
    for (int iphi = 0; iphi < _nphi; iphi++) {
      // // include random offsets for all quantities
      // double phi = (iphi+0.5) * _dphi + _dphi*rand()*_grid_scatter/RAND_MAX;
      // double rap = irap * _drap + _drap*rand()*_grid_scatter/RAND_MAX;
      // //double phi = (iphi+0.5) * _dphi* + rand()*_grid_scatter/RAND_MAX;
      // //double rap = irap * _drap + rand()*_grid_scatter/RAND_MAX;
      // double kt = _mean_ghost_kt*(1+rand()*_kt_scatter/RAND_MAX);
     
      // include random offsets for all quantities
      double phi = (iphi+0.5) * _dphi + _dphi*_our_rand()*_grid_scatter;
      double rap = irap * _drap + _drap*_our_rand()*_grid_scatter;
      //double phi = (iphi+0.5) * _dphi* + _our_rand()*_grid_scatter;
      //double rap = irap * _drap + _our_rand()*_grid_scatter;
      double kt = _mean_ghost_kt*(1+_our_rand()*_kt_scatter);



      double pminus = kt*exp(-rap);
      double pplus  = kt*exp(+rap);
      double px = kt*sin(phi);
      double py = kt*cos(phi);
      //cout << kt<<" "<<rap<<" "<<phi<<"\n";
      //if (phi>=twopi || phi < 0.0) cout << "Hey: "<< phi-twopi<<"\n";
      PseudoJet mom(px,py,0.5*(pplus-pminus),0.5*(pplus+pminus));
      //mom.set_user_index(1);  // for ghost particles (user index now lost...)
      event.push_back(mom);
      //_is_pure_ghost.push_back(true);
    }
  }
}

string ActiveAreaSpec::description() {

  ostringstream ostr;
  ostr << "Active area specification with ghosts of area " << actual_ghost_area() 
       << " (had requested " << ghost_area() << ")"
       << ", placed up to y = " << ghost_maxrap() 
       << ", scattered rel. to perfect grid by " << grid_scatter() 
       << ", mean_ghost_kt = " << mean_ghost_kt()
       << ", rel kt_scatter =  " << kt_scatter()
       << ", n repetitions of ghost distributions =  " << repeat();
  return ostr.str();
}

FASTJET_END_NAMESPACE


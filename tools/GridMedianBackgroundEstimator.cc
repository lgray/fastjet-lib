//STARTHEADER
// $Id$
//
// Copyright (c) 2005-2011, Matteo Cacciari, Gavin Salam and Gregory Soyez
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


#include "fastjet/tools/GridMedianBackgroundEstimator.hh"
using namespace std;

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

//----------------------------------------------------------------------
// setting a new event
//----------------------------------------------------------------------
// tell the background estimator that it has a new event, composed
// of the specified particles.
void GridMedianBackgroundEstimator::set_particles(const vector<PseudoJet> & particles) {
  fill(_scalar_pt.begin(), _scalar_pt.end(), 0.0);
  for (unsigned i = 0; i < particles.size(); i++) {
    int j = igrid(particles[i]);
    if (j >= 0){
      if (_rescaling_class == 0)
	_scalar_pt[j] += particles[i].perp();
      else
	_scalar_pt[j] += particles[i].perp()/(*_rescaling_class)(particles[i]);
    }
  }
  sort(_scalar_pt.begin(), _scalar_pt.end());

  _has_particles = true;
}


//----------------------------------------------------------------------
// retrieving fundamental information
//----------------------------------------------------------------------
// get rho, the median background density per unit area
double GridMedianBackgroundEstimator::rho() const {
  return _percentile(_scalar_pt, 0.5) / _cell_area;
}


//----------------------------------------------------------------------
// get sigma, the background fluctuations per unit area; must be
// multipled by sqrt(area) to get fluctuations for a region of a
// given area.
double GridMedianBackgroundEstimator::sigma() const{
  // watch out: by definition, our sigma is the standard deviation of
  // the pt density multiplied by the square root of the cell area
  return (_percentile(_scalar_pt, 0.5) -
	  _percentile(_scalar_pt, (1.0-0.6827)/2.0)
	  )/sqrt(_cell_area);
}

//----------------------------------------------------------------------
// get rho, the background density per unit area, locally at the
// position of a given jet. Note that this is not const, because a
// user may then wish to query other aspects of the background that
// could depend on the position of the jet last used for a rho(jet)
// determination.
double GridMedianBackgroundEstimator::rho(const PseudoJet & jet)  {
  //_warning_rho_of_jet.warn("rho(jet) not yet implemented; currently just returns global rho");
  double rescaling = (_rescaling_class == 0) ? 1.0 : (*_rescaling_class)(jet);
  return rescaling*rho();
}


//----------------------------------------------------------------------
// get sigma, the background fluctuations per unit area, locally at
// the position of a given jet. As for rho(jet), it is non-const.
double GridMedianBackgroundEstimator::sigma(const PseudoJet & jet){
  //_warning_rho_of_jet.warn("rho(jet) not yet implemented; currently just returns global rho");
  double rescaling = (_rescaling_class == 0) ? 1.0 : (*_rescaling_class)(jet);
  return rescaling*sigma();
}



//----------------------------------------------------------------------
// configuring the behaviour
//----------------------------------------------------------------------
// Set a pointer to a class that calculates the rescaling factor as
// a function of the jet (position). Note that the rescaling factor
// is used both in the determination of the "global" rho (the pt/A
// of each jet is divided by this factor) and when asking for a
// local rho (the result is multiplied by this factor).
//
// The BackgroundRescalingYPolynomial class can be used to get a
// rescaling that depends just on rapidity.
//
// Note that this has to be called BEFORE any attempt to do an
// actual computation
void GridMedianBackgroundEstimator::set_rescaling_class(const FunctionOfPseudoJet<double> * rescaling_class) {
  // The rescaling is taken into account when particles are set. So
  // you need to call set_particles again if you set the rescaling
  // class. We thus warn if there are already some available
  // particles
  if (_has_particles)
    _warning_rescaling.warn("GridMedianBackgroundEstimator::set_rescaling_class(): trying to set the rescaling class when there are already particles that have been set is dangerous: the rescaling will not affect the already existing particles resulting in mis-estimation of rho. You need to call set_particles() again before proceeding with any background estimation.");
  
  BackgroundEstimatorBase::set_rescaling_class(rescaling_class);
}


//----------------------------------------------------------------------
// protected material
//----------------------------------------------------------------------
// configure the grid
void GridMedianBackgroundEstimator::setup_grid() {
  // this grid-definition code is becoming repetitive -- it should
  // probably be moved somewhere central...
  double ny_double = (_ymax-_ymin) / _requested_grid_spacing;
  _ny = int(ny_double+0.5);
  _dy = (_ymax-_ymin) / _ny;
  
  _nphi = int (twopi / _requested_grid_spacing + 0.5);
  _dphi = twopi / _nphi;

  // some sanity checking (could throw a fastjet::Error)
  assert(_ny >= 1 && _nphi >= 1);

  _ntotal = _nphi * _ny;
  _scalar_pt.resize(_ntotal);
  _cell_area = _dy * _dphi;
}


//----------------------------------------------------------------------
// retrieve the grid cell index for a given PseudoJet
int GridMedianBackgroundEstimator::igrid(const PseudoJet & p) const {
  // directly taking int does not work for values between -1 and 0
  // so use floor instead
  // double iy_double = (p.rap() - _ymin) / _dy;
  // if (iy_double < 0.0) return -1;
  // int iy = int(iy_double);
  // if (iy >= _ny) return -1;

  // writing it as below gives a huge speed gain (factor two!). Even
  // though answers are identical and the routine here is not the
  // speed-critical step. It's not at all clear why.
  int iy = int(floor( (p.rap() - _ymin) / _dy ));
  if (iy < 0 || iy >= _ny) return -1;

  int iphi = int( p.phi()/_dphi );
  assert(iphi >= 0 && iphi <= _nphi);
  if (iphi == _nphi) iphi = 0; // just in case of rounding errors

  int igrid_res = iy*_nphi + iphi;
  assert (igrid_res >= 0 && igrid_res < _ny*_nphi);
  return igrid_res;
}


FASTJET_END_NAMESPACE        // defined in fastjet/internal/base.hh

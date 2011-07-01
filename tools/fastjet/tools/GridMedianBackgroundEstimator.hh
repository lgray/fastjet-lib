#ifndef __GRID_MEDIAN_BACKGROUND_ESTIMATOR_HH__
#define __GRID_MEDIAN_BACKGROUND_ESTIMATOR_HH__

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


#include "fastjet/tools/BackgroundEstimatorBase.hh"

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

/// \class GridMedianBackgroundEstimator
/// 
/// Background Estimator based on the median pt/area of a set of grid
/// cells. 
///
/// DOCUMENTATION STILL INCOMPLETE
class GridMedianBackgroundEstimator : public BackgroundEstimatorBase {
public:
  GridMedianBackgroundEstimator(double requested_grid_spacing, double ymax) :
  _ymin(-ymax), _ymax(ymax), 
  _requested_grid_spacing(requested_grid_spacing) {setup_grid();}
  
  void set_particles(const std::vector<PseudoJet> & particles);

  double rho() const;
  double rho(const PseudoJet & jet);


private:
  void setup_grid();
  double percentile(double perc) const;

  int igrid(const PseudoJet & p) const;

  double _ymin, _ymax, _dy, _dphi, _requested_grid_spacing, _cell_area;
  int _ny, _nphi, _ntotal;

  std::vector<double> _scalar_pt;

  LimitedWarning _warning_rho_of_jet;

};

FASTJET_END_NAMESPACE        // defined in fastjet/internal/base.hh

#endif // __GRID_MEDIAN_BACKGROUND_ESTIMATOR_HH__

#ifndef __FASTJET_BACKGROUND_ESTIMATOR_BASE_HH__
#define __FASTJET_BACKGROUND_ESTIMATOR_BASE_HH__

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

#include <fastjet/ClusterSequenceAreaBase.hh>
#include <fastjet/FunctionOfPseudoJet.hh>
#include <fastjet/Selector.hh>
#include <iostream>

FASTJET_BEGIN_NAMESPACE     // defined in fastjet/internal/base.hh




/// @ingroup tools
/// \class BackgroundEstimatorBase
///
/// Abstract base class that provides the basic interface for classes
/// that estimate levels of background radiation in hadrion and
/// heavy-ion collider events.
///
///
class BackgroundEstimatorBase {
public:

  /// @ name  retrieving fundamental information
  //\{
  //----------------------------------------------------------------

  /// get rho, the background density per unit area
  virtual double rho() const = 0;

  /// get sigma, the background fluctuations per unit area; must be
  /// multipled by sqrt(area) to get fluctuations for a region of a
  /// given area.
  virtual double sigma() const { 
    throw Error("sigma() not supported for this Background Estimator");
  }

  /// get rho, the background density per unit area, locally at the
  /// position of a given jet. Note that this is not const, because a
  /// user may then wish to query other aspects of the background that
  /// could depend on the position of the jet last used for a rho(jet)
  /// determination.
  virtual double rho(const PseudoJet & jet) = 0;

  /// get sigma, the background fluctuations per unit area, locally at
  /// the position of a given jet. As for rho(jet), it is non-const.
  virtual double sigma(const PseudoJet & jet) { 
    throw Error("sigma(jet) not supported for this Background Estimator");
  }

  /// returns true if this background estimator has support for
  /// determination of sigma
  virtual bool has_sigma() {return false;}
  //\}
  

  /// @name setting a new event
  //\{
  //----------------------------------------------------------------

  /// tell the background estimator that it has a new event, composed
  /// of the specified particles.
  virtual void set_particles(const std::vector<PseudoJet> & particles) = 0;

  //\}

};



FASTJET_END_NAMESPACE

#endif  // __BACKGROUND_ESTIMATOR_BASE_HH__


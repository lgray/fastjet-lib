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

#include "fastjet/tools/Subtractor.hh"

FASTJET_BEGIN_NAMESPACE     // defined in fastjet/internal/base.hh

PseudoJet Subtractor::operator()(const PseudoJet & jet) const {
  if (!jet.has_area()){
    throw Error("Trying to subtract a jet without area support");
  }
  
  double rho = _bge->rho(jet);
  PseudoJet subtracted_jet = jet;
  PseudoJet area4vect = jet.area_4vector();
  // sanity check
  if (rho*area4vect.perp() < jet.perp() ) { 
    // this subtraction should retain the jet's structural
    // information
    subtracted_jet -= rho*area4vect;
  } else { 
    // this sets the jet's momentum to zero while
    // maintaining all of the jet's structural information
    subtracted_jet *= 0;
  }
  return subtracted_jet;
}

FASTJET_END_NAMESPACE

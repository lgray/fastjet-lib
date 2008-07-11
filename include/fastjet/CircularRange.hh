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


#ifndef __FASTJET_CIRCULARRANGE_HH__
#define __FASTJET_CIRCULARRANGE_HH__

#include "fastjet/RangeDefinition.hh"


class CircularRange : public fastjet::RangeDefinition {
public:
  /// constructor
  CircularRange() {}
  
  /// initialise CircularRange with a jet
  CircularRange(const fastjet::PseudoJet & jet, double distance) {
                _distance = distance;
		_rapjet = jet.rap();
		_phijet = jet.phi();
		_total_area = fastjet::pi*_distance*_distance;  }

  /// initialise CircularRange with a (rap,phi) point
  CircularRange(double rap, double phi, double distance) {
                _distance = distance;
		_rapjet = rap;
		_phijet = phi;
		_total_area = fastjet::pi*_distance*_distance;  }
  
  /// destructor
  virtual ~CircularRange() {}
  
  /// return description of range
  virtual inline std::string description() const {
    std::ostringstream ostr;
    ostr << "Range: within distance "<< _distance << " of given jet or point." ;
    return ostr.str(); }
  
  /// return bool according to whether (rap,phi) is in range
  virtual inline bool is_in_range(double rap, double phi) const {
     double pi = fastjet::pi;
     double deltaphi = _phijet - phi;
     if ( deltaphi > pi) { deltaphi -= 2.*pi; }
     else if ( deltaphi < -pi) { deltaphi += 2.*pi; }
     bool inrange = ( (rap-_rapjet)*(rap-_rapjet) +
                deltaphi*deltaphi <= _distance*_distance );
     return inrange; }

  /// return the minimal and maximal rapidity of this range
  virtual inline void get_rap_limits(double & rapmin, double & rapmax) const {
     rapmin = _rapjet - _distance;
     rapmax = _rapjet + _distance; }

private:
  double _distance,_rapjet,_phijet;

};

#endif // __FASTJET_CIRCULARRANGE_HH__

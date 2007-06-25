//STARTHEADER
// $Id:  $
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

#ifndef __FASTJET_RANGEDEFINITION_HH__
#define __FASTJET_RANGEDEFINITION_HH__

#include "fastjet/PseudoJet.hh"
#include<sstream>
#include<iostream>
#include<string>

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

//----------------------------------------------------------------------
//
/// class for holding a range definition specification, given by limits
/// on rapidity and azimuth.
///
class RangeDefinition {
public:
  /// default constructor
  RangeDefinition() {}

  /// constructor for a range definition given by |y|<rapmax
  RangeDefinition(double rapmax) {
                     assert ( rapmax > 0.0 );
                     _rapmax = rapmax;
		     _rapmin = -rapmax;
		     _phimin = 0.0;
		     _phimax = twopi;
		     _total_area = 2.0*rapmax*twopi; }
  
  /// destructor does nothing
  virtual ~RangeDefinition() {}
     
  /// constructor for a range definition given by 
  /// rapmin <= y <= rapmax, phimin <= phi <= phimax
  RangeDefinition(double rapmin, double rapmax, 
                  double phimin = 0.0, double phimax = twopi) {
                     assert ( rapmin < rapmax);
                     assert ( phimin < phimax);
                     assert ( phimin >= 0.0 );
                     _rapmax = rapmax;
		     _rapmin = rapmin;
		     _phimin = phimin;
		     _phimax = phimax;
		     _total_area = (_rapmax - _rapmin)*(_phimax - _phimin); }


  /// return bool according to whether the jet is within the given range
  inline bool is_in_range(const PseudoJet & jet) const {
    double rap = jet.rap();
    double phi = jet.phi();
    return is_in_range(rap,phi);
  }
  
  /// return bool according to whether a (rap,phi) point is in range
  virtual inline bool is_in_range(double rap, double phi) const {
    return  ( rap >= _rapmin && 
              rap <= _rapmax &&
              phi >= _phimin &&
              phi <= _phimax );
  }
  
  /// area of the range region
  virtual inline double area() const { return _total_area; }
  
  /// textual description of range
  virtual inline std::string description() const {
    std::ostringstream ostr;
    ostr << "Range: " << _rapmin << " <= y <= "   << _rapmax << ", "
                      << _phimin << " <= phi <= " << _phimax ;
    return ostr.str();
}

protected:
  double _total_area;

  /// calculate, and set in _total_area, the area with a numerical test
  /// takes a reasonable time with rapmax = 10, npoints = 100
  void _numerical_total_area(double rapmax, int npoints) ;

private:
  double _rapmin,_rapmax,_phimin,_phimax;

};

FASTJET_END_NAMESPACE        // defined in fastjet/internal/base.hh

#endif // __FASTJET_RANGEDEFINITION_HH__

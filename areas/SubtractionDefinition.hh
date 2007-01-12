//STARTHEADER
// $Id: SubtractionDefinition.hh 375 2006-11-30 10:33:24Z salam $
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

#ifndef __FASTJET_SUBTRACTIONDEFINITION_HH__
#define __FASTJET_SUBTRACTIONDEFINITION_HH__

#include "fastjet/internal/base.hh"

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

//======================================================================
/// the different possible backgroud shapes
enum BackgroundShape {
   /// flat, for high luminosity pp collisions
   Flat = 0,
   /// parabolic, for heavy ion collisions
   Parabolic = 1
};


//======================================================================
/// class that is intended to hold a full definition of the jet
/// subtraction procedure
class SubtractionDefinition {

public:

  /// constructor to specify the subtraction procedure
  SubtractionDefinition(BackgroundShape background_shape,
                        ClusterSequenceActiveArea::mean_pt_strategies mean_pt_type=median, 
                        int sub_iterations = 1,
                        BackgroundShape first_subtraction = Flat,
                        double initial_R = 0.1) :
      _background_shape(background_shape),
      _mean_pt_type(mean_pt_type),
      _sub_iterations(sub_iterations),
      _first_subtraction(first_subtraction),
      _initial_R(initial_R){};     


  // return information about the definition...
  BackgroundShape background_shape() const {return _background_shape;};
  BackgroundShape first_subtraction() const {return _first_subtraction;};
  ClusterSequenceActiveArea::mean_pt_strategies mean_pt_type() const
          {return _mean_pt_type;};
  double initial_R() const {return _initial_R;};
  int sub_iterations() const {return _sub_iterations;};
  
  
private:

  BackgroundShape _background_shape, _first_subtraction;
  ClusterSequenceActiveArea::mean_pt_strategies _mean_pt_type;  
  double _initial_R;
  int _sub_iterations;
    
}; // end class SubtractionDefinition


FASTJET_END_NAMESPACE

#endif // __FASTJET_JETDEFINITION_HH__

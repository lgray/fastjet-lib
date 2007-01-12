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

#include <sstream>
#include <string>
#include "fastjet/ClusterSequenceActiveArea.hh"
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
                        int sub_iterations = 1,
                        BackgroundShape pre_subtraction = Flat,
                        double initial_R = 0.1) :
      _background_shape(background_shape),
      _sub_iterations(sub_iterations),
      _pre_subtraction_shape(pre_subtraction),
      _initial_R(initial_R){ _mean_pt_strategy = ClusterSequenceActiveArea::median;};     


  /// return information about the definition...
  BackgroundShape background_shape() const {return _background_shape;};
  BackgroundShape pre_subtraction() const {return _pre_subtraction_shape;};
  ClusterSequenceActiveArea::mean_pt_strategies mean_pt_strategy() const
          {return _mean_pt_strategy;};
  double initial_R() const {return _initial_R;};
  int sub_iterations() const {return _sub_iterations;};
  
  /// set mean_pt_strategy
  inline void set_mean_pt_strategy(ClusterSequenceActiveArea::mean_pt_strategies
                         mean_pt_strategy) { _mean_pt_strategy = mean_pt_strategy;};

  /// set boolean flag for 4-vector subtraction
  inline void set_four_vector_subtraction(bool four_vector_subtraction)
          {_four_vector_subtraction = four_vector_subtraction;};

  /// return a textual description of the current subtraction definition 
  std::string description() const {
    ostringstream name;
    name << "Subtraction definition uses " << _background_shape_name()
         << " background shape with " << sub_iterations() << " pass(es).";
    if (background_shape() == Flat) { 
       name << " Flat background level is determined with strategy " << mean_pt_strategy();
    }
    if (sub_iterations() > 1 ) {
       name << " Inital radius = " << initial_R() << ". First subtraction uses " <<
       _background_shape_name() << " background shape." << endl;
    } 
    return name.str();
  };

  
private:

  BackgroundShape _background_shape, _pre_subtraction_shape;
  ClusterSequenceActiveArea::mean_pt_strategies _mean_pt_strategy;  
  double _initial_R;
  int _sub_iterations;
  bool _four_vector_subtraction;
  
  std::string _background_shape_name() const {
     return (background_shape() == Flat) ? "flat" :"parabolic";
  };
    
}; // end class SubtractionDefinition


FASTJET_END_NAMESPACE

#endif // __FASTJET_JETDEFINITION_HH__

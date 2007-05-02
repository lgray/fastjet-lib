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

#ifndef __FASTJET_CLUSTERSEQUENCEWITHAREA_HH__
#define __FASTJET_CLUSTERSEQUENCEWITHAREA_HH__

#include "fastjet/ClusterSequence.hh"

FASTJET_BEGIN_NAMESPACE

/// base class that sets interface for extensions of ClusterSequence
/// that provide information about the area of each jet; 
///
/// the virtual functions here all return 0, since no area determination
/// is implemented.
class ClusterSequenceWithArea : public ClusterSequence {
public:
  
  /// a constructor which just carries out the construction of the
  /// parent class
  template<class L> ClusterSequenceWithArea
         (const std::vector<L> & pseudojets, 
	  const JetDefinition & jet_def,
	  const bool & writeout_combinations = false) :
           ClusterSequence(pseudojets, jet_def, writeout_combinations) {}


  /// default constructor
  ClusterSequenceWithArea() {}


  /// destructor
  virtual ~ClusterSequenceWithArea() {}


  /// return the area associated with the given jet; this base class
  /// returns 0.
  virtual double area       (const PseudoJet & jet) const {return 0.0;}

  /// return the error (uncertainty) associated with the determination
  /// of the area of this jet; this base class returns 0.
  virtual double area_error (const PseudoJet & jet) const {return 0.0;}

  /// return the total area, up to |y|<maxrap, that is free of jets
  virtual double empty_area(double maxrap) const;

  /// return something similar to the number of pure ghost jets
  /// in the given rapidity range in an active area case.
  /// For the local implementation we return empty_area/(0.55 pi R^2),
  /// based on measured properties of ghost jets with kt and cam. Note
  /// that the number returned is a double.
  virtual double n_empty_jets(double maxrap) const {
    double R = jet_def().R();
    return empty_area(maxrap)/(0.55*pi*R*R);
  }

  /// the median of (pt/area) for jets contained within |y|<maxrap, 
  /// making use also of the info on n_empty_jets
  virtual double median_pt_per_unit_area(double maxrap) const;

  /// the median of (pt/area_4vector) for jets contained within
  /// making use also of the info on n_empty_jets
  virtual double median_pt_per_unit_area_4vector(double maxrap) const;
  
  /// the function that does the work for median_pt_per_unit_area and 
  /// median_pt_per_unit_area_4vector: 
  /// - something_is_area_4vect = false -> use plain area
  /// - something_is_area_4vect = true  -> use 4-vector area
  virtual double median_pt_per_unit_something(
                     double maxrap, bool use_area_4vector) const;


  /// return a PseudoJet whose 4-vector is defined by the following integral
  ///
  ///       \int drap d\phi PseudoJet("rap,phi,pt=one") *
  ///                           * Theta("rap,phi inside jet boundary")
  ///
  /// where PseudoJet("rap,phi,pt=one") is a 4-vector with the given
  /// rapidity (rap), azimuth (phi) and pt=1, while Theta("rap,phi
  /// inside jet boundary") is a function that is 1 when rap,phi
  /// define a direction inside the jet boundary and 0 otherwise.
  ///
  /// This base class returns a null 4-vector.
  virtual PseudoJet area_4vector(const PseudoJet & jet) const {
    return PseudoJet(0.0,0.0,0.0,0.0);}

};



FASTJET_END_NAMESPACE

#endif // __FASTJET_CLUSTERSEQUENCEWITHAREA_HH__

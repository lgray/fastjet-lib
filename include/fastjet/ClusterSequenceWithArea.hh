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

#ifndef __FASTJET_CLUSTERSEQUENCEWITHAREA_HH__
#define __FASTJET_CLUSTERSEQUENCEWITHAREA_HH__

#include "fastjet/ClusterSequenceAreaBase.hh"
#include "fastjet/ClusterSequenceActiveArea.hh"
#include "fastjet/ClusterSequenceActiveAreaExplicitGhosts.hh"
#include "fastjet/ClusterSequenceVoronoiArea.hh"
#include "fastjet/AreaDefinition.hh"

FASTJET_BEGIN_NAMESPACE

class ClusterSequenceWithArea : public ClusterSequenceAreaBase {
public:
  template<class L> ClusterSequenceWithArea
         (const std::vector<L> & pseudojets, 
	  const JetDefinition & jet_def,
	  const AreaDefinition & area_def_in);

  /// return a reference to the area definition
  const AreaDefinition & area_def() const {return _area_def;}


  /// return the area associated with the given jet
  virtual double area       (const PseudoJet & jet) const {
    return _area_base->area(jet);}

  /// return the error (uncertainty) associated with the determination
  /// of the area of this jet
  virtual double area_error (const PseudoJet & jet) const {
    return _area_base->area_error(jet);}

  /// return the 4-vector area
  virtual PseudoJet area_4vector(const PseudoJet & jet) const {
    return _area_base->area_4vector(jet);}

  /// return the total area, up to |y|<maxrap, that is free of jets
  virtual double empty_area(double maxrap) const {
    return _area_base->empty_area(maxrap);}

  /// return something similar to the number of pure ghost jets
  /// in the given rapidity range in an active area case.
  /// For the local implementation we return empty_area/(0.55 pi R^2),
  /// based on measured properties of ghost jets with kt and cam. Note
  /// that the number returned is a double.
  virtual double n_empty_jets(double maxrap) const {
    return _area_base->n_empty_jets(maxrap);
  }


private:
  std::auto_ptr<ClusterSequenceAreaBase> _area_base;
  AreaDefinition _area_def;
};

//----------------------------------------------------------------------
template<class L> ClusterSequenceWithArea::ClusterSequenceWithArea
(const std::vector<L> & pseudojets, 
 const JetDefinition  & jet_def,
 const AreaDefinition & area_def_in) : _area_def(area_def_in) {
  
  ClusterSequenceAreaBase * _area_base_ptr;
  switch(_area_def.area_type()) {
  case AreaDefinition::active_area:
    _area_base_ptr = new ClusterSequenceActiveArea(pseudojets, 
                                                   jet_def, 
                                                   _area_def.active_spec());
    break;
  case AreaDefinition::active_area_explicit_ghosts:
    _area_base_ptr = new ClusterSequenceActiveAreaExplicitGhosts(pseudojets, 
                                                   jet_def, 
                                                   _area_def.active_spec());
    break;
  case AreaDefinition::voronoi_area:
    _area_base_ptr = new ClusterSequenceVoronoiArea(pseudojets, 
                                                   jet_def, 
                                                   _area_def.voronoi_spec());
    break;
  default:
    std::cerr << "Error: unrecognized area_type in ClusterSequenceWithArea:" 
              << _area_def.area_type() << std::endl;
    exit(-1);
  }
  // now copy across the information from the area base class
  _area_base = std::auto_ptr<ClusterSequenceAreaBase>(_area_base_ptr);
  transfer_from_sequence(*_area_base);
}

FASTJET_END_NAMESPACE

#endif // __FASTJET_CLUSTERSEQUENCEWITHAREA_HH__



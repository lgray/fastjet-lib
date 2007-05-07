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


#ifndef __FASTJET_AREADEFINITION_HH__
#define __FASTJET_AREADEFINITION_HH__

#include "fastjet/ActiveAreaSpec.hh"

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

//----------------------------------------------------------------------
//
/// class for holding a "Voronoi area" specification; an area will be
/// assigned to each particle, which is the area of the intersection
/// of the particle's Voronoi cell with a circle of radius
/// R*effective_Rfact.
///
class VoronoiAreaSpec {
public:

  VoronoiAreaSpec(double effective_Rfact = 1.0) : 
    _effective_Rfact(effective_Rfact) {};
  
  double effective_Rfact() const {return _effective_Rfact;}

  std::string description() const;

private:
  double _effective_Rfact;
};


//----------------------------------------------------------------------
/// class that holds a generic area definition
///
class AreaDefinition {
public:

  /// constructor for an area definition based on an active area
  /// specification, together with an option to get explicit ghosts
  AreaDefinition(const ActiveAreaSpec & spec, bool explicit_ghosts = false);

  /// constructor for an area definition based on a voronoi area specification
  AreaDefinition(const VoronoiAreaSpec & );

  /// return a description of the current area definition
  std::string description() const;

  /// the different types of area that are supported
  enum AreaType {active_area=0, active_area_explicit_ghosts = 1, 
                 voronoi_area=2};
  
  /// return info about the type of area being used by this defn
  AreaType area_type() const {return _area_type;}

  /// return a reference to the active area spec
  const ActiveAreaSpec  & active_spec()  const {return _active_spec;}

  /// return a reference to the voronoi area spec
  const VoronoiAreaSpec & voronoi_spec() const {return _voronoi_spec;}
  


private:

  AreaType        _area_type;
  ActiveAreaSpec  _active_spec;
  VoronoiAreaSpec _voronoi_spec;
};

FASTJET_END_NAMESPACE        // defined in fastjet/internal/base.hh


#endif // __FASTJET_AREADEFINITION_HH__

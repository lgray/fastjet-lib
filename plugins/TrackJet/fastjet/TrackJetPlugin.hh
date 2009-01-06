//STARTHEADER
// $Id: TrackJetPlugin.hh 1240 2008-06-28 18:43:47Z soyez $
//
// Copyright (c) 2007-2008, Matteo Cacciari, Gavin Salam and Gregory Soyez
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

#ifndef __TRACKJETPLUGIN_HH__
#define __TRACKJETPLUGIN_HH__

#include "fastjet/JetDefinition.hh"
#include <list>
#include <memory>
#include <cmath>

// questionable whether this should be in fastjet namespace or not...
FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

// another forward declaration to reduce includes
class PseudoJet;

//----------------------------------------------------------------------
//
class TrackJetPlugin : public JetDefinition::Plugin {
public:
  /// Main constructor for the TrackJet Plugin class.  
  ///
  /// The argument is an initialised list of jet algorithms
  /// \param _radius  the distance at which point a particle is no longer
  ///                 recombied into the jet
  /// \param jet_recombination_scheme  the recombination scheme used to 
  ///                                  sum the 4-vecors inside the jet
  /// \param track_recombination_scheme  the recombination scheme used to 
  ///                                    sum the 4-vecors when accumulating
  ///                                    track into a the jet
  /// Both recombiners are defaulted to pt_scheme recomb as for the Rivet
  /// implementation.
  TrackJetPlugin (double _radius, 
		  RecombinationScheme jet_recombination_scheme=pt_scheme, 
		  RecombinationScheme track_recombination_scheme=pt_scheme){
    radius  = _radius;
    radius2 = radius*radius;
    jet_recombiner = JetDefinition::DefaultRecombiner(jet_recombination_scheme);
    track_recombiner = JetDefinition::DefaultRecombiner(track_recombination_scheme);
  }

  /// copy constructor
  TrackJetPlugin (const TrackJetPlugin & plugin) {
    *this = plugin;
  }

  // the things that are required by base class
  virtual std::string description () const;
  virtual void run_clustering(ClusterSequence &) const;

  /// the plugin mechanism's standard way of accessing the jet radius
  /// here we return the R of the last alg in the list
  virtual double R() const {return radius;}

private:
  double radius, radius2;

  JetDefinition::DefaultRecombiner jet_recombiner;
  JetDefinition::DefaultRecombiner track_recombiner;
};

FASTJET_END_NAMESPACE        // defined in fastjet/internal/base.hh

#endif // __TRACKJETPLUGIN_HH__


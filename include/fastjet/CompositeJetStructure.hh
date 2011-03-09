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


#ifndef __FASTJET_COMPOSITEJET_STRUCTURE_HH__
#define __FASTJET_COMPOSITEJET_STRUCTURE_HH__

#include <fastjet/PseudoJet.hh>
#include <fastjet/PseudoJetStructureBase.hh>

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

/// @ingroup tools
/// \class CompositeJetStructure
/// The structure for a jet made of pieces
///
/// This stores the vector of the pieces that make the jet and provide
/// the methods to access them
class CompositeJetStructure : public PseudoJetStructureBase{
public:
  // basic class info
  //------------------------------------------------------------------------------
  /// default ctor
  CompositeJetStructure(){};

  /// ctor with initialisation
  CompositeJetStructure(const std::vector<PseudoJet> & initial_pieces) : _pieces(initial_pieces){};

  /// default dtor
  virtual ~CompositeJetStructure(){};

  /// description
  virtual std::string description() const;

  // things reimplemented from the base structure
  //------------------------------------------------------------------------------
  /// true if the jet has constituents (i.e. all pieces do)
  virtual bool has_constituents() const;

  /// return the constituents (i.e. the union of the constituents of each piece)
  /// 
  /// If any of the pieces has no constituent, an error is thrown
  virtual std::vector<PseudoJet> constituents(const PseudoJet &jet) const;

  /// true if it has pieces (always the case)
  virtual bool has_pieces() const {return true;};

  /// returns the pieces
  virtual std::vector<PseudoJet> pieces(const PseudoJet &jet) const;

protected:
  std::vector<PseudoJet> _pieces;  ///< the pieces building the jet
};



FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

#endif // __FASTJET_MERGEDJET_STRUCTURE_HH__

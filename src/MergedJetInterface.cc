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

#include <fastjet/MergedJetInterface.hh>

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

using namespace std;


//-------------------------------------------------------------------------------
// \class MergedJetInterface
// The interface for qa jet made of pieces
//
// This stores the vector of the pieces that make the jet and provide
// the methods to access them
// -------------------------------------------------------------------------------

// description
std::string MergedJetInterface::description() const{ 
  string str = "Merged PseudoJet";
  return str; 
}



// things reimplemented from the base interface
//------------------------------------------------------------------------------
bool MergedJetInterface::has_constituents() const{
  for (vector<PseudoJet>::const_iterator pit=_pieces.begin(); pit!=_pieces.end(); pit++)
    if (!pit->has_constituents()) return false;

  return true;
}

std::vector<PseudoJet> MergedJetInterface::constituents(const PseudoJet &jet) const{
  // the following code automatically throws an Error if any of teh
  // pieces has no constituents
  vector<PseudoJet> all_constituents = _pieces[0].constituents();
  for (unsigned i = 1; i < _pieces.size(); i++) {
    vector<PseudoJet> constits = _pieces[i].constituents();
    copy(constits.begin(), constits.end(), back_inserter(all_constituents));
  }
 
  return all_constituents;
}

std::vector<PseudoJet> MergedJetInterface::pieces(const PseudoJet &jet) const{
  return _pieces;
}


FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

//STARTHEADER
// $Id$
//
// Copyright (c) 2006 Matteo Cacciari and Gavin Salam
//
//----------------------------------------------------------------------
// This file is part of a simple command-line handling environment
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

#ifndef __FlavourHolder__
#define __FlavourHolder__

#include<vector>

//----------------------------------------------------------------------
/// Class that holds the flavour of an object in a form that
/// makes it easy to access the various quark components...
///
/// NB: particle numbering is given at http://www-cpd.fnal.gov/psm/stdhep/
///
class FlavourHolder {

public:
  /// initialise the FlavourHolder from a standard idhep value
  FlavourHolder(int idhep);

  /// return the amount of a given flavour (1..6)
  const int operator[] (int) const;

  // establish whether in specific class...
  bool is_charged_lepton() const;
  bool is_neutrino() const;
  bool is_lepton() const;
  bool is_muon() const;
  
  /// return the idhep where known
  const int idhep() const;
  
private:
  
  std::vector<int> _flav_content;
  int              _idhep;
};


//-----  inline material ------------------------------------------------
inline const int FlavourHolder::operator[] (int iflv) const {
  return _flav_content[iflv];
}

/// leptons are number 11..16
inline bool  FlavourHolder::is_lepton() const {
  return (abs(_idhep) >= 11 && abs(_idhep) <=16); 
}

/// neutrinos are leptons with even numbers
inline bool  FlavourHolder::is_neutrino() const {
  return is_lepton() && _idhep%2 == 0;}

/// charged leptons are leptons with odd numbers
inline bool  FlavourHolder::is_charged_lepton() const {
  return is_lepton() && _idhep%2 != 0;}

/// muons are +-13
inline bool  FlavourHolder::is_muon() const {return abs(_idhep) == 13;}
  
inline const int FlavourHolder::idhep() const {return _idhep;} 


#endif // __FlavourHolder__

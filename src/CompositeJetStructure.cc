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

#include <fastjet/CompositeJetStructure.hh>

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

using namespace std;

//-------------------------------------------------------------------------------
// helper functions to build a jet made of pieces
//-------------------------------------------------------------------------------

// build a "CompositeJet" from the vector of its pieces
//
// In this case, E-scheme recombination is assumed to compute the
// total momentum
PseudoJet join(const vector<PseudoJet> & pieces, JetDefinition::Recombiner * recombiner){
  // compute the total momentum
  //--------------------------------------------------
  PseudoJet result;  // automatically initialised to 0
  for (unsigned int i=0; i<pieces.size(); i++){
    if (recombiner)
      recombiner->plus_equal(result, pieces[i]);
    else
      result += pieces[i];
  }

  // attach a CompositeJetStructure to the result
  //--------------------------------------------------
  CompositeJetStructure *cj_struct = new CompositeJetStructure(pieces, recombiner);

  result.set_structure_shared_ptr(SharedPtr<PseudoJetStructureBase>(cj_struct));

  return result;
}

// build a "CompositeJet" from a single PseudoJet
PseudoJet join(const PseudoJet & j1, 
	       JetDefinition::Recombiner * recombiner){
  return join(vector<PseudoJet>(1,j1), recombiner);
}

// build a "CompositeJet" from two PseudoJet
PseudoJet join(const PseudoJet & j1, const PseudoJet & j2, 
	       JetDefinition::Recombiner * recombiner){
  vector<PseudoJet> pieces;
  pieces.push_back(j1);
  pieces.push_back(j2);
  return join(pieces, recombiner);
}

// build a "CompositeJet" from 3 PseudoJet
PseudoJet join(const PseudoJet & j1, const PseudoJet & j2, const PseudoJet & j3, 
	       JetDefinition::Recombiner * recombiner){
  vector<PseudoJet> pieces;
  pieces.push_back(j1);
  pieces.push_back(j2);
  pieces.push_back(j3);
  return join(pieces, recombiner);
}

// build a "CompositeJet" from 4 PseudoJet
PseudoJet join(const PseudoJet & j1, const PseudoJet & j2, const PseudoJet & j3, const PseudoJet & j4,
	       JetDefinition::Recombiner * recombiner){
  vector<PseudoJet> pieces;
  pieces.push_back(j1);
  pieces.push_back(j2);
  pieces.push_back(j3);
  pieces.push_back(j4);
  return join(pieces, recombiner);
}


//-------------------------------------------------------------------------------
// \class CompositeJetStructure
// The structure for a jet made of pieces
//
// This stores the vector of the pieces that make the jet and provide
// the methods to access them
// -------------------------------------------------------------------------------

CompositeJetStructure::CompositeJetStructure(const std::vector<PseudoJet> & initial_pieces, 
					     JetDefinition::Recombiner * recombiner)
  : _pieces(initial_pieces){
  // deal with area support (cache the area if needed)
  //--------------------------------------------------
  // check if all the pieces have area, in which case store it
  bool has_area = true;
  for (vector<PseudoJet>::const_iterator pit=_pieces.begin(); pit!=_pieces.end(); pit++){
    if (!pit->has_area()){
      has_area = false;
      continue;
    }
  }

  _area             = 0.0;
  _area_error       = 0.0;
  
  if (has_area){
    _area_4vector_ptr = new PseudoJet();
    _area             = 0.0;
    _area_error       = 0.0;
    for (unsigned int i=0; i<_pieces.size(); i++){
      const PseudoJet & p = _pieces[i];
      _area       += p.area();
      _area_error += p.area_error();
      if (recombiner)
	recombiner->plus_equal(*_area_4vector_ptr, p.area_4vector());
      else
	*_area_4vector_ptr += p.area_4vector();
    } 
  } else {
    _area_4vector_ptr = 0;
  }

}


// description
std::string CompositeJetStructure::description() const{ 
  string str = "Composite PseudoJet";
  return str; 
}



// things reimplemented from the base structure
//------------------------------------------------------------------------------
bool CompositeJetStructure::has_constituents() const{
  for (vector<PseudoJet>::const_iterator pit=_pieces.begin(); pit!=_pieces.end(); pit++)
    if (!pit->has_constituents()) return false;

  return true;
}

std::vector<PseudoJet> CompositeJetStructure::constituents(const PseudoJet &jet) const{
  // the following code automatically throws an Error if any of the
  // pieces has no constituents
  vector<PseudoJet> all_constituents = _pieces[0].constituents();
  for (unsigned i = 1; i < _pieces.size(); i++) {
    vector<PseudoJet> constits = _pieces[i].constituents();
    copy(constits.begin(), constits.end(), back_inserter(all_constituents));
  }
 
  return all_constituents;
}

std::vector<PseudoJet> CompositeJetStructure::pieces(const PseudoJet &jet) const{
  return _pieces;
}


// area-related material

// check if it has a well-defined area
bool CompositeJetStructure::has_area() const{
  return (_area_4vector_ptr != 0);
}

// return the jet (scalar) area.
double CompositeJetStructure::area(const PseudoJet &reference) const{
  return _area;
}

// return the error (uncertainty) associated with the determination
// of the area of this jet.
// 
// Be conservative: return the sum of the errors
double CompositeJetStructure::area_error(const PseudoJet &reference) const{
  return _area_error;
}

// return the jet 4-vector area.
PseudoJet CompositeJetStructure::area_4vector(const PseudoJet &reference) const{
  return *_area_4vector_ptr; // one is supposed to call has_area before!
}



FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

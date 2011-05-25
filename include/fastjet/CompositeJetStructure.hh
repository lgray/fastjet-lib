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

// to have access to the recombiner we need to include the JetDefinition header
#include <fastjet/JetDefinition.hh>

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
  CompositeJetStructure() : _area_4vector_ptr(0), _area(0.0), _area_error(0.0){};

  /// ctor with initialisation
  CompositeJetStructure(const std::vector<PseudoJet> & initial_pieces, 
			const JetDefinition::Recombiner * recombiner = 0);

  /// default dtor
  virtual ~CompositeJetStructure(){
    if (_area_4vector_ptr) delete _area_4vector_ptr;
  };

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

  // area-related material

  /// check if it has a well-defined area
  virtual bool has_area() const;

  /// return the jet (scalar) area.
  virtual double area(const PseudoJet &reference) const;

  /// return the error (uncertainty) associated with the determination
  /// of the area of this jet.
  ///
  /// Be conservative: return the sum of the errors
  virtual double area_error(const PseudoJet &reference) const;

  /// return the jet 4-vector area.
  virtual PseudoJet area_4vector(const PseudoJet &reference) const;


  // allow to modify the area information
  // (for use in join())
  //------------------------------------------------------------------------------
  void set_area_information(PseudoJet *area_4vector_ptr, double area, double area_error=0.0){
    _area_4vector_ptr = area_4vector_ptr;
    _area = area;
    _area_error = area_error;
  }


protected:
  std::vector<PseudoJet> _pieces;  ///< the pieces building the jet
  PseudoJet * _area_4vector_ptr;   ///< pointer to the 4-vector jet area
  double _area, _area_error;       ///< the jet area and associated error
};



// helpers to "join" jets and produce a structure derived from
// CompositeJetStructure
//------------------------------------------------------------------------

/// build a "CompositeJet" from the vector of its pieces with an
/// extended structure of type T derived from CompositeJetStructure
template<typename T> PseudoJet join(const std::vector<PseudoJet> & pieces){
  PseudoJet result(0.0,0.0,0.0,0.0);
  for (unsigned int i=0; i<pieces.size(); i++){
    const PseudoJet it = pieces[i];
    result += it;
  }

  T *cj_struct = new T(pieces);
  result.set_structure_shared_ptr(SharedPtr<PseudoJetStructureBase>(cj_struct));

  return result;
}


/// build a "CompositeJet" from a single PseudoJet with an extended
/// structure of type T derived from CompositeJetStructure
template<typename T> PseudoJet join(const PseudoJet & j1){
  return join<T>(std::vector<PseudoJet>(1,j1));
}

/// build a "CompositeJet" from two PseudoJet with an extended
/// structure of type T derived from CompositeJetStructure
template<typename T> PseudoJet join(const PseudoJet & j1, const PseudoJet & j2){
  std::vector<PseudoJet> pieces;
  pieces.push_back(j1);
  pieces.push_back(j2);
  return join<T>(pieces);
}

/// build a "CompositeJet" from 3 PseudoJet with an extended structure
/// of type T derived from CompositeJetStructure
template<typename T> PseudoJet join(const PseudoJet & j1, const PseudoJet & j2, 
				    const PseudoJet & j3){
  std::vector<PseudoJet> pieces;
  pieces.push_back(j1);
  pieces.push_back(j2);
  pieces.push_back(j3);
  return join<T>(pieces);
}

/// build a "CompositeJet" from 4 PseudoJet with an extended structure
/// of type T derived from CompositeJetStructure
template<typename T> PseudoJet join(const PseudoJet & j1, const PseudoJet & j2, 
				    const PseudoJet & j3, const PseudoJet & j4){
  std::vector<PseudoJet> pieces;
  pieces.push_back(j1);
  pieces.push_back(j2);
  pieces.push_back(j3);
  pieces.push_back(j4);
  return join<T>(pieces);
}


FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

#endif // __FASTJET_MERGEDJET_STRUCTURE_HH__

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

#include "fastjet/tools/Transformer.hh"

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

using namespace std;

//-------------------------------------------------------------------------------
// class Transformer
// Base (abstract) class for a jet transformer
//
// The idea of a transformer is to take a list of jets, apply some
// transformation on them and return the list of modified jets This
// base class sets the fundamental requirements for all the
// transformers.
// 
// Similarly, to gain access to the information relative to the
// transformation, a "transformed" PseudoJet will have a
// corresponding PseudoJetInterface. Any transformer thus have to
// providwe (at least) 2 classes:
//  - the transformer itself (derived from Transformer)
//  - the associated property class (derived from TransformerInterface see below)
//-------------------------------------------------------------------------------

// description of the transformer
std::string Transformer::description() const{
  return "dummy transformer";
}

// action of the transformer on a single jet
PseudoJet Transformer::apply(const PseudoJet & original) const{
  // default behaviour?
  // options include
  //  - simply copy the original
  //  - keep the original with an empty PseudoJetInterfaceBase
  //  - throw an error
  // we go for the 2nd one mostly to illustrate the process but also
  // because it would follow a more standard behaviour of a
  // Transformer leading to an object with a TransformerInterface

  PseudoJet result = original;

  // the following sets the "structure"
  PseudoJetStructureBase *struct_ptr = new PseudoJetStructureBase();
  result.set_structure_shared_ptr(SharedPtr<PseudoJetStructureBase>(struct_ptr));

  return result;
}

// // action of the transformer on each jet from the vector
// vector<PseudoJet> Transformer::operator()(const std::vector<PseudoJet> & originals) const{
//   vector<PseudoJet> result;
//   for (vector<PseudoJet>::const_iterator it=originals.begin(); it!=originals.end(); it++)
//     result.push_back((*this)(*it));
//   return result;
// }


FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

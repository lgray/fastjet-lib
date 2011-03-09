#ifndef __FASTJET_TRANSFORMER_HH__
#define __FASTJET_TRANSFORMER_HH__

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

#include <fastjet/SharedPtr.hh>
#include <fastjet/PseudoJet.hh>
#include <fastjet/PseudoJetStructureBase.hh>

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

// forward declarations of what we will have down here
class Transformer;

/// @ingroup tools
/// \class Transformer
/// Base (abstract) class for a jet transformer
///
/// The idea of a transformer is that applied to a jet, it somehow
/// modifies its momentum and/or contents.. If applied to a vector of
/// jets, the Transformer is applied to each one individually.
///
/// This class here is a base class that provides a basic template on
/// which actual Transformers may be built (one example is a filter).
///
/// Any new transformer must implement the operator(). 
///
/// In addition many transformers will want to associated extra
/// information on the resulting jet's substructure, by setting a
/// shared pointer to some class derived from PseudoJetInterfaceBase.
/// It is the user's responsability to implement this and also set
/// up a typedef so that DerivedTransformer::InterfaceType is the
/// corresponding Interface type.
///
/// [.......comments still under preparation......]

/// transformation on them and return the list of modified jets This
/// base class sets the fundamental requirements for all the
/// transformers.
/// 
/// Similarly, to gain access to the information relative to the
/// transformation, a "transformed" PseudoJet will have a
/// corresponding PropertyInterface. Any transformer thus must
/// provide (at least) 2 classes:
///  - the transformer itself (derived from Transformer)
///  - the associated property class (derived from TransformerInterface see below)
class Transformer{
public:
  /// default ctor
  Transformer(){}

  /// default dtor
  virtual ~Transformer(){}

  /// description of the transformer
  virtual std::string description() const;

  /// information about the associated structure type
  typedef PseudoJetStructureBase StructureType;

  /// action of the transformer on a single jet
  virtual PseudoJet operator()(const PseudoJet & original) const;

  /// action of the transformer on each jet from the vector
  virtual std::vector<PseudoJet> operator()(const std::vector<PseudoJet> & originals) const;
};

FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

#endif // __FASTJET_TRANSFORMER_HH__

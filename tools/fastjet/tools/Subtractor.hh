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

#ifndef __FASTJET_TOOLS_SUBTRACTOR_HH__
#define __FASTJET_TOOLS_SUBTRACTOR_HH__

#include <fastjet/tools/Transformer.hh> // to derive Subtractor from Transformer
#include <fastjet/tools/BackgroundEstimator.hh> // used as a ctor argument

FASTJET_BEGIN_NAMESPACE     // defined in fastjet/internal/base.hh


//----------------------------------------------------------------------
/// @ingroup tools
/// \class Subtractor
/// Class that helps performing jet background subtraction
///
/// This class is nothing but a transformer that makes use of the
/// BackgroundEstimator introduced above to subtract the background 
/// from the jet.
///
/// \section desc Options
/// 
/// The constructor takes as an argument a BackgroundEstimator.
///
/// \section input Input conditions
/// 
///  - the original jet must have area support (4-vector)
///
/// \section output Output/interface
/// 
///  The structure of the jet is not modified.
///
class Subtractor : public Transformer{
public:
  /// define a subtractor based on a BackgroundEstimator
  Subtractor(BackgroundEstimator * bge) : 
    _bge(bge) {}

  /// default dtor
  virtual ~Subtractor(){};

  /// returns a jet that's subtracted
  ///
  /// \param jet    the jet that is to be subtracted
  /// \return       the subtracted jet
  virtual PseudoJet operator()(const PseudoJet & jet) const;

  /// action of the transformer on each jet from the vector. This has
  /// to be repeated because the
  /// Subtractor::operator(vector<PseudoJet>) shares the same name as
  /// the operator()(PseudoJet)
  virtual std::vector<PseudoJet> operator()(const std::vector<PseudoJet> & originals) const{
    return Transformer::operator()(originals);
  }

  /// class description
  std::string description() const {
    return "Subtractor";
  }

protected:
  /// the tool used to estimate the background
  /// if has to be mutable in case its underlying selector takes a reference jet
  mutable BackgroundEstimator * _bge;
};

FASTJET_END_NAMESPACE

#endif  // __FASTJET_TOOLS_SUBTRACTOR_HH__


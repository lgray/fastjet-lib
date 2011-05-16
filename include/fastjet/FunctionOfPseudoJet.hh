#ifndef __FASTJET_FUNCTION_OF_PSEUDOJET_HH__
#define __FASTJET_FUNCTION_OF_PSEUDOJET_HH__

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

#include <fastjet/PseudoJet.hh>

FASTJET_BEGIN_NAMESPACE

/// \class FunctionOfPseudoJet
/// a generic function of any type of a PseudoJet
///
/// This class implements a generic funstion returning an object of a
/// given (template) type as a function of a PseudoJet
///
/// Constraints: TOut had to be default-constructible.
template<typename TOut>
class FunctionOfPseudoJet{
public:
  /// default ctor
  FunctionOfPseudoJet(){}

  /// default dtor (virtual to make it polymorphic)
  virtual ~FunctionOfPseudoJet(){}

  /// returns a description of the function (an empty string by
  /// default)
  virtual std::string description() const{ return "";}

  /// the action of the function
  /// this _has_ to be overloaded in derived classes
  ///  \param pj   the PseudoJet input to the function
  virtual TOut apply(const PseudoJet &pj) const = 0;

  /// apply the function using the "traditional" () operator.
  /// By default, this just calls the apply(...) method above.
  ///  \param pj   the PseudoJet input to the function
  virtual TOut operator()(const PseudoJet &pj) const { return apply(pj);}

  /// apply the function on a vector of PseudoJet, returning a vector
  /// of the results.
  /// This just calls apply on every PseudoJet in the vector.
  ///  \param pjs  the vector of PseudoJet inputs to the function
  virtual std::vector<TOut> operator()(const std::vector<PseudoJet> &pjs) const {
    std::vector<TOut> res(pjs.size());
    for (unsigned int i=0; i<pjs.size(); i++)
      res[i] = apply(pjs[i]);
    return res;
  }
};

FASTJET_END_NAMESPACE

#endif  // __FASTJET_FUNCTION_OF_PSEUDOJET_HH__

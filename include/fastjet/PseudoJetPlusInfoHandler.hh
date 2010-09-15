//STARTHEADER
// $Id$
//
// Copyright (c) 2005-2010, Matteo Cacciari, Gavin Salam and Gregory Soyez
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

#ifndef __PSEUDOJET_PLUS_INFO_HANDLER_HH__
#define __PSEUDOJET_PLUS_INFO_HANDLER_HH__

#include "fastjet/internal/numconsts.hh"
#include "fastjet/ClusterSequenceWrapper.hh"

FASTJET_BEGIN_NAMESPACE

//-------------------------------------------------------------------
// since C++ does not support partial specialisation of template
// member functions, we shall need an extra class that actually does
// the job (partial spec is allowed in the case of a class)
//-------------------------------------------------------------------

// fwd declaration of PJ
class PseudoJet;


// \class PseudoJetPlusInfoHandler
// the default handler that does not support extra info
//
// This template construct depends on a type and a boolean value.  we
// shall implement a generic definition that is supposed to hold for a
// type T that just has T[0--3] and, below, specialise it to the case
// where T inherits from PseudoJet, thus giving access to more
// information
template<typename T, bool b>
class PseudoJetPlusInfoHandler{
public:
  // ctor
  PseudoJetPlusInfoHandler(const T &t){ _t = &t;};

  // conversion to PJ*
  PseudoJet * operator()(){
    return NULL;
  }

protected:
  // a pointer to the actual data (we can use a pointer as it's always
  // going to be a temporary object
  const T* _t; 
};


// specialisation for the case where T is derived from a PseudoJet
// the default handler that does not support extra info
template<typename T>
class PseudoJetPlusInfoHandler<T, true>{
public:
  // ctor
  PseudoJetPlusInfoHandler(const T &t){ _t = &t;};

  // conversion to PJ*
  PseudoJet * operator()(){
    return _t;
  }
  
protected:
  // a pointer to the actual data (we can use a pointer as it's always
  // going to be a temporary object
  const T* _t;
};


FASTJET_END_NAMESPACE

#endif  // __PSEUDOJET_PLUS_INFO_HANDLER_HH__

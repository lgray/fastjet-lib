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


#ifndef __FASTJET_CLUSTER_SEQUENCE_WRAPPER_HH__
#define __FASTJET_CLUSTER_SEQUENCE_WRAPPER_HH__

#include "fastjet/internal/base.hh"

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

class ClusterSequence;

// forward declaration of the ClusterSequenceWrapper
/// \if internal_doc
/// @ingroup internal
/// \class ClusterSequenceWrapper
///
/// A wrapper class that hold a pointer to a ClusterSequence object
/// It has ClusterSequence as a friend class so that only
/// ClusterSequence can change its availability status
/// \endif
class ClusterSequenceWrapper{
public:
  ClusterSequenceWrapper() : _cs(NULL){};
  ClusterSequenceWrapper(ClusterSequence *cs) : _cs(cs){};

  const ClusterSequence * cs() const { return _cs;}
  bool is_alive() const { return (_cs != NULL);}

  friend class ClusterSequence;

private:
  ClusterSequence * _cs;
};

FASTJET_END_NAMESPACE

#endif  //  __FASTJET_CLUSTER_SEQUENCE_WRAPPER_HH__

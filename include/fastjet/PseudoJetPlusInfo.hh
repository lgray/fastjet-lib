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

#ifndef __PSEUDOJET_PLUS_INFO_HH__
#define __PSEUDOJET_PLUS_INFO_HH__

#include "fastjet/internal/numconsts.hh"
#include "fastjet/PseudoJet.hh"

FASTJET_BEGIN_NAMESPACE

//-----------------------------------------------
/// \ingroup extra_info
/// \class PseudoJetPlusInfo
/// a templated extension of PseudoJet that carries extra information
///
/// a class that carries the PseudoJet together with some extra info
/// though you could live with the PseudoJet::_extra_info which is
/// already in PseudoJet, this class is mostly a helper that deals
/// with the association of a generic extra info (derived
/// PsseudoJet::ExtraInfo) to a PseudoJet
///
/// WARNING: the template parameter T (i.e. your specific extra info
/// type) has to be derived from PseudoJet::ExtraInfo (this is mostly
/// related to the use of dynamic_casts that we have adopted as a
/// safe-keeper against misuse of pointers)
//-----------------------------------------------
template<typename TExtraInfo>
class PseudoJetPlusInfo : public PseudoJet{
public:
  /// ctor with initialisation
  /// \param pj   the underlying PseudoJet
  /// \param ei   the extra information
  PseudoJetPlusInfo(const PseudoJet &pj, const TExtraInfo &ei) : PseudoJet(pj){
    const ExtraInfo * extra_info_ptr = extra_info();
    if (!extra_info_ptr) {
      extra_info_shared().reset(new TExtraInfo(ei));
    } else if (dynamic_cast<const TExtraInfo*>(extra_info_ptr)) {
      // then this is already a valid PseudoJetPlusInfo
      // don't do anything
    } else {
      // we already have an info but of wrong type!
      throw ("invalid extra_info for initialising PseudoJetPlusInfo");
    }
  }
};


FASTJET_END_NAMESPACE

#endif  // __PSEUDOJET_PLUS_INFO_HH__

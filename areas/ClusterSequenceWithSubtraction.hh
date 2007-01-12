//STARTHEADER
// $Id: ClusterSequenceWithSubtraction.hh 342 2006-10-19 21:29:36Z salam $
//
// Copyright (c) 2005-2006, Matteo Cacciari and Gavin Salam
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

#ifndef __FASTJET_CLUSTERSEQUENCEWITHSUBTRACTION_HH__
#define __FASTJET_CLUSTERSEQUENCEWITHSUBTRACTION_HH__

#include<iostream>
#include<vector>
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequenceActiveArea.hh"
#include "SubtractionDefinition.hh"

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

using namespace std;


/// class that implements the subtraction of a parabolic or flat
/// background, giving access to both the unsubtracted and the
/// subtracted jets
class ClusterSequenceWithSubtraction : public ClusterSequenceActiveArea {

public:

  /// constructor based on JetDefinition and ActiveAreaSpec
  template<class L> ClusterSequenceWithSubtraction
         (const std::vector<L> & pseudojets, 
	  const JetDefinition & jet_def,
	  const ActiveAreaSpec & area_spec,
	  const SubtractionDefinition & sub_def) ;



  /// function returning the jet WITHOUT subtraction
  PseudoJet unsubtracted(const PseudoJet &) const;

};  
  
FASTJET_END_NAMESPACE

#endif // __FASTJET_CLUSTERSEQUENCEWITHSUBTRACTION_HH__

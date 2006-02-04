//STARTHEADER
// $Id$
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


#include "FjPseudoJet.hh"
#include "FjClusterSequence.hh"
#include<iostream>
#include<cmath>
#include <cstdlib>
#include<cassert>


using namespace std;


//----------------------------------------------------------------------
/// Run the clustering in a very slow variant of the N^3 algorithm. 
///
/// The only thing this routine has going for it is that memory usage
/// is O(N)!
void FjClusterSequence::_really_dumb_cluster () {

  // the array that will be overwritten here will be one
  // of pointers to jets.
  vector<FjPseudoJet *> jetsp(_jets.size());
  vector<int>         indices(_jets.size());

  for (size_t i = 0; i<_jets.size(); i++) {
    jetsp[i] = & _jets[i];
    indices[i] = i;
  }

  for (int n = jetsp.size(); n > 0; n--) {
    int ii, jj;
    // find smallest beam distance
    double ymin = jetsp[0]->beam_distance();
    ii = 0; jj = -2;
    for (int i = 0; i < n; i++) {
      if (jetsp[i]->beam_distance() < ymin) {
	ymin = jetsp[i]->beam_distance(); ii = i; jj = -2;}
    }

    // find smallest distance between pair of jetsp
    for (int i = 0; i < n-1; i++) {
      for (int j = i+1; j < n; j++) {
	double y = jetsp[i]->kt_distance(*jetsp[j])*_invR2;
	if (y < ymin) {ymin = y; ii = i; jj = j;}
      }
    }

    // output recombination sequence
    // old "ktclus" way of labelling
    //cout <<n<< " "<< ii+1 << " with " << jj+1 << "; y = "<< ymin<<endl;
    // new delaunay way of labelling
    int jjindex_or_beam, iiindex;
    if (jj < 0) {jjindex_or_beam = BeamJet; iiindex = indices[ii];} 
    else {
      jjindex_or_beam = max(indices[ii],indices[jj]);
      iiindex =         min(indices[ii],indices[jj]);
    }

    // now recombine
    int newn = 2*jetsp.size() - n;
    if (jj >= 0) {
      // combine pair
      _jets.push_back(*jetsp[ii] + *jetsp[jj]);
      jetsp[ii] = &_jets[_jets.size()-1];
      // have jj point to jet that was pointed at by n-1 
      // (since original jj is no longer current, so put n-1 into jj)
      jetsp[jj] = jetsp[n-1];

      indices[ii] = newn;
      indices[jj] = indices[n-1];
      _add_step_to_history(newn,iiindex,
			      jjindex_or_beam,_jets.size()-1,ymin);
    } else {
      // combine ii with beam
      // put last jet (pointer) in place of ii (which has disappeared)
      jetsp[ii] = jetsp[n-1];
      indices[ii] = indices[n-1];
      _add_step_to_history(newn,iiindex,jjindex_or_beam,Invalid, ymin);
    }
  }

}

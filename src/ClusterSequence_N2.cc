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


// The plain N^2 part of the ClusterSequence class -- separated out
// from the rest of the class implementation so as to speed up
// compilation of this particular part while it is under test.

#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include<iostream>

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh


using namespace std;

//----------------------------------------------------------------------
/// Order(N^2) clustering 
///
/// Works for any class BJ that satisfies certain minimal 
/// requirements (which are ...?)
///
/// - need to have _bj_set_jetinfo
/// - need to have _bj_dist
/// - should contain members kt2 (=energy^2), NN, NN_dist, _jets_index

template<class BJ> void ClusterSequence::_simple_N2_cluster() {
  int n = _jets.size();
  BJ * briefjets = new BJ[n];
  BJ * jetA = briefjets, * jetB;
  
  // initialise the basic jet info 
  for (int i = 0; i< n; i++) {
    _bj_set_jetinfo(jetA, i);
    jetA++; // move on to next entry of briefjets
  }
  BJ * tail = jetA; // a semaphore for the end of briefjets
  BJ * head = briefjets; // a nicer way of naming start

  // now initialise the NN distances: jetA will run from 1..n-1; and
  // jetB from 0..jetA-1
  for (jetA = head + 1; jetA != tail; jetA++) {
    // set NN info for jetA based on jets running from head..jetA-1,
    // checking in the process whether jetA itself is an undiscovered
    // NN of one of those jets.
    _bj_set_NN_crosscheck(jetA, head, jetA);
  }


  // now create the diJ (where J is i's NN) table -- remember that 
  // we differ from standard normalisation here by a factor of R2
  double * diJ = new double[n];
  jetA = head;
  for (int i = 0; i < n; i++) {
    diJ[i] = _bj_diJ(jetA);
    jetA++; // have jetA follow i
  }

  // now run the recombination loop
  int history_location = n-1;
  while (tail != head) {

    // find the minimum of the diJ on this round
    double diJ_min = diJ[0];
    int diJ_min_jet = 0;
    for (int i = 1; i < n; i++) {
      if (diJ[i] < diJ_min) {diJ_min_jet = i; diJ_min  = diJ[i];}
    }
    
    // do the recombination between A and B
    history_location++;
    jetA = & briefjets[diJ_min_jet];
    jetB = jetA->NN;
    // put the normalisation back in
    diJ_min *= _invR2; 
    if (jetB != NULL) {
      // jet-jet recombination
      // If necessary relabel A & B to ensure jetB < jetA, that way if
      // the larger of them == newtail then that ends up being jetA and 
      // the new jet that is added as jetB is inserted in a position that
      // has a future!
      if (jetA < jetB) {swap(jetA,jetB);}

      int nn; // new jet index
      _do_ij_recombination_step(jetA->_jets_index, jetB->_jets_index, diJ_min, nn);

      //OBS // get the two history indices
      //OBS int hist_a = _jets[jetA->_jets_index].cluster_hist_index();
      //OBS int hist_b = _jets[jetB->_jets_index].cluster_hist_index();
      //OBS // create the recombined jet
      //OBS _jets.push_back(_jets[jetA->_jets_index] + _jets[jetB->_jets_index]);
      //OBS int nn = _jets.size() - 1;
      //OBS _jets[nn].set_cluster_hist_index(history_location);
      //OBS // update history
      //OBS _add_step_to_history(history_location, 
      //OBS   		   min(hist_a,hist_b),max(hist_a,hist_b),
      //OBS 			   nn, diJ_min);
      // what was jetB will now become the new jet
      _bj_set_jetinfo(jetB, nn);
    } else {
      // jet-beam recombination
      _do_iB_recombination_step(jetA->_jets_index, diJ_min);
      //OBS // get the hist_index
      //OBS int hist_a = _jets[jetA->_jets_index].cluster_hist_index();
      //OBS _add_step_to_history(history_location,hist_a,BeamJet,Invalid,diJ_min); 
    }

    // now update our nearest neighbour info and diJ table
    // first reduce size of table
    tail--; n--;
    // Copy last jet contents and diJ info into position of jetA
    *jetA = *tail;
    diJ[jetA - head] = diJ[tail-head];

    // Initialise jetB's NN distance as well as updating it for 
    // other particles.
    // NB: by having different loops for jetB == or != NULL we could
    //     perhaps save a few percent (usually avoid one if inside loop),
    //     but will not do it for now because on laptop fluctuations are
    //     too large to reliably measure a few percent difference...
    for (BJ * jetI = head; jetI != tail; jetI++) {
      // see if jetI had jetA or jetB as a NN -- if so recalculate the NN
      if (jetI->NN == jetA || jetI->NN == jetB) {
	_bj_set_NN_nocross(jetI, head, tail);
	diJ[jetI-head] = _bj_diJ(jetI); // update diJ 
      } 
      // check whether new jetB is closer than jetI's current NN and
      // if need be update things
      if (jetB != NULL) {
	double dist = _bj_dist(jetI,jetB);
	if (dist < jetI->NN_dist) {
	  if (jetI != jetB) {
	    jetI->NN_dist = dist;
	    jetI->NN = jetB;
	    diJ[jetI-head] = _bj_diJ(jetI); // update diJ...
	  }
	}
	if (dist < jetB->NN_dist) {
	  if (jetI != jetB) {
	    jetB->NN_dist = dist;
	    jetB->NN      = jetI;}
	}
      }
      // if jetI's NN is the new tail then relabel it so that it becomes jetA
      if (jetI->NN == tail) {jetI->NN = jetA;}
    }

    
    if (jetB != NULL) {diJ[jetB-head] = _bj_diJ(jetB);}


  }
  

  // final cleaning up;
  delete[] diJ;
  delete[] briefjets;
}

//*************************************************************************
//
//                             THINGS FOR E+E-
//
//*************************************************************************


//----------------------------------------------------------------------
template<> inline void ClusterSequence::_bj_set_jetinfo(
                           EEBriefJet * const jetA, const int _jets_index) const {

  double E = _jets[_jets_index].E();
  jetA->kt2  = E*E; // replaces energy; might one day become more general
  double norm = _jets[_jets_index].modp2();
  if (norm > 0) {
    norm = 1.0/sqrt(norm);
    jetA->nx = norm * _jets[_jets_index].px();
    jetA->ny = norm * _jets[_jets_index].py();
    jetA->nz = norm * _jets[_jets_index].pz();
  } else {
    jetA->nx = 0.0;
    jetA->ny = 0.0;
    jetA->nz = 1.0;
  }
  jetA->_jets_index = _jets_index;
  // initialise NN info as well
  jetA->NN_dist = _R2;
  jetA->NN      = NULL;
}

//----------------------------------------------------------------------
// returns the angular distance between the two jets
template<> double ClusterSequence::_bj_dist(
                const EEBriefJet * const jeta, 
                const EEBriefJet * const jetb) const {
  double dist = 1.0 
    - jeta->nx*jetb->nx
    - jeta->ny*jetb->ny
    - jeta->nz*jetb->nz;
  dist *= 2; // distance is _2_*min(Ei^2,Ej^2)*(1-cos theta)
  //cout << "Dist = " << dist << ": " 
  //     << jeta->nx << " "
  //     << jeta->ny << " "
  //     << jeta->nz << " "
  //     <<endl;
  return dist;
}


//----------------------------------------------------------------------
/// Force instantiation of desired versions of _simple_N2_cluster
///
/// This is not very elegant...
void ClusterSequence::_dummy_N2_cluster_instantiation() {
  _simple_N2_cluster<BriefJet>();
  _simple_N2_cluster<EEBriefJet>();
}

FASTJET_END_NAMESPACE


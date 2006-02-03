// The plain N^2 part of the FjClusterSequence class -- separated out
// from the rest of the class implementation so as to speed up
// compilation of this particular part while it is under test.

#include "FjPseudoJet.hh"
#include "FjClusterSequence.hh"
#include<iostream>


using namespace std;

//----------------------------------------------------------------------
/// Order(N^2) clustering 
///
void FjClusterSequence::_simple_N2_cluster() {
  int n = _jets.size();
  BriefJet * briefjets = new BriefJet[n];
  BriefJet * jetA = briefjets, * jetB;
  
  // initialise the basic jet info 
  for (int i = 0; i< n; i++) {
    _bj_set_jetinfo(jetA, i);
    jetA++; // move on to next entry of briefjets
  }
  BriefJet * tail = jetA; // a semaphore for the end of briefjets
  BriefJet * head = briefjets; // a nicer way of naming start

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

      // get the two history indices
      int hist_a = _jets[jetA->_jets_index].cluster_hist_index();
      int hist_b = _jets[jetB->_jets_index].cluster_hist_index();
      // create the recombined jet
      _jets.push_back(_jets[jetA->_jets_index] + _jets[jetB->_jets_index]);
      int nn = _jets.size() - 1;
      _jets[nn].set_cluster_hist_index(history_location);
      // update history
      _add_step_to_history(history_location, 
			   min(hist_a,hist_b),max(hist_a,hist_b),
			   nn, diJ_min);
      // what was jetB will now become the new jet
      _bj_set_jetinfo(jetB, nn);
    } else {
      // jet-beam recombination
      // get the hist_index
      int hist_a = _jets[jetA->_jets_index].cluster_hist_index();
      _add_step_to_history(history_location,hist_a,BeamJet,Invalid,diJ_min); 
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
    for (BriefJet * jetI = head; jetI != tail; jetI++) {
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

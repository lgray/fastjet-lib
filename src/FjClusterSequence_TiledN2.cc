// The plain N^2 part of the FjClusterSequence class -- separated out
// from the rest of the class implementation so as to speed up
// compilation of this particular part while it is under test.

#include "FjPseudoJet.hh"
#include "FjClusterSequence.hh"
#include<iostream>
#include<cmath>

using namespace std;


//----------------------------------------------------------------------
void FjClusterSequence::_bj_remove_from_tiles(TiledJet * const jet) {
  Tile * tile = & _tiles[jet->tile_index];

  if (jet->previous == NULL) {
    // we are at head of the tile, so reset it.
    // If this was the only jet on the tile then tile->head will now be NULL
    tile->head = jet->next;
  } else {
    // adjust link from previous jet in this tile
    jet->previous->next = jet->next;
  }
  if (jet->next != NULL) {
    // adjust backwards-link from next jet in this tile
    jet->next->previous = jet->previous;
  }
}

//----------------------------------------------------------------------
/// Set up the tiles:
///  - decide the range in eta
///  - allocate the tiles
///  - set up the cross-referencing info between tiles
///
/// The neighbourhood of a tile is set up as follows
///
/// 	      LRR
///           LXR
///           LLR
///
/// such that tiles is an array containing XLLLLRRRR with pointers
///                                         |   \ RH_tiles
///                                         \ surrounding_tiles
///
/// with appropriate precautions when close to the edge of the tiled
/// region.
///
void FjClusterSequence::_initialise_tiles() {

  // first decide tile sizes
  _tile_size_eta = _Rparam;
  _n_tiles_phi   = int(floor(twopi/_Rparam));
  _tile_size_phi = twopi / _n_tiles_phi; // >= _Rparam and fits in 2pi

  // always include zero rapidity in the tiling region
  _tiles_eta_min = 0.0;
  _tiles_eta_max = 0.0;
  // but go no further than following
  const double maxrap = 7.0;

  // and find out how much further one should go
  for(unsigned int i = 0; i < _jets.size(); i++) {
    double eta = _jets[i].rap();
    // first check if eta is in range -- to avoid taking into account
    // very spurious rapidities due to particles with near-zero kt.
    if (abs(eta) < maxrap) {
      if (eta < _tiles_eta_min) {_tiles_eta_min = eta;}
      if (eta > _tiles_eta_max) {_tiles_eta_max = eta;}
    }
  }

  // now adjust the values
  _tiles_ieta_min = int(floor(_tiles_eta_min/_tile_size_eta));
  _tiles_ieta_max = int(floor( _tiles_eta_max/_tile_size_eta));
  _tiles_eta_min = _tiles_ieta_min * _tile_size_eta;
  _tiles_eta_max = _tiles_ieta_max * _tile_size_eta;

  // allocate the tiles
  _tiles.resize((_tiles_ieta_max-_tiles_ieta_min+1)*_n_tiles_phi);

  // now set up the cross-referencing between tiles
  for (int ieta = _tiles_ieta_min; ieta <= _tiles_ieta_max; ieta++) {
    for (int iphi = 0; iphi < _n_tiles_phi; iphi++) {
      Tile * tile = & _tiles[(ieta-_tiles_ieta_min)*_n_tiles_phi+iphi];
      // no jets in this tile yet
      tile->head = NULL; // first element of tiles points to itself
      tile->begin_tiles[0] =  tile;
      Tile ** pptile = & (tile->begin_tiles[0]);
      pptile++;
      //
      // set up L's in column to the left of X
      tile->surrounding_tiles = pptile;
      if (ieta > _tiles_ieta_min) {
	// run idphi from 5 to 7 instead of -1..1 so as to get around
	// problem that (-1)%n = -1 rather than n-1 as we would like...
	for (int idphi = 5; idphi <=7; idphi++) {
	  *pptile = & _tiles[(ieta-1-_tiles_ieta_min)*_n_tiles_phi+
			     (iphi+idphi) % _n_tiles_phi];
	  pptile++;
	}	
      }
      // now set up last L (below X)
      *pptile = & _tiles[(ieta-_tiles_ieta_min)*_n_tiles_phi+
			 (iphi+5) % _n_tiles_phi];
      pptile++;
      // set up first R (above X)
      tile->RH_tiles = pptile;
      *pptile = & _tiles[(ieta-_tiles_ieta_min)*_n_tiles_phi+
			 (iphi+1) % _n_tiles_phi];
      pptile++;
      // set up remaining R's, to the right of X
      if (ieta < _tiles_ieta_max) {
	for (int idphi = 5; idphi <=7; idphi++) {
	  *pptile = & _tiles[(ieta+1-_tiles_ieta_min)*_n_tiles_phi+
			     (iphi+idphi) % _n_tiles_phi];
	  pptile++;
	}	
      }
      // now put semaphore for end tile
      tile->end_tiles = pptile;
      // finally make sure tiles are untagged
      tile->tagged = false;
    }
  }

}


//----------------------------------------------------------------------
/// return the tile index corresponding to the given eta,phi point
int FjClusterSequence::_tile_index(const double & eta, const double & phi) const {
  int ieta, iphi;
  if      (eta <= _tiles_eta_min) {ieta = 0;}
  else if (eta >= _tiles_eta_max) {ieta = _tiles_ieta_max-_tiles_ieta_min;}
  else {
    //ieta = int(floor((eta - _tiles_eta_min) / _tile_size_eta));
    ieta = int(((eta - _tiles_eta_min) / _tile_size_eta));
    // following needed in case of rare but nasty rounding errors
    if (ieta > _tiles_ieta_max-_tiles_ieta_min) {
      ieta = _tiles_ieta_max-_tiles_ieta_min;} 
  }
  // allow for some extent of being beyond range in calculation of phi
  // as well
  //iphi = (int(floor(phi/_tile_size_phi)) + _n_tiles_phi) % _n_tiles_phi;
  // with just int and no floor, things run faster but beware
  iphi = int((phi+twopi)/_tile_size_phi) % _n_tiles_phi;
  return (iphi + ieta * _n_tiles_phi);
}


//----------------------------------------------------------------------
// overloaded version which additionally sets up information regarding the
// tiling
inline void FjClusterSequence::_bj_set_jetinfo( TiledJet * const jet,
					      const int _jets_index) {
  // first call the generic setup
  _bj_set_jetinfo<>(jet, _jets_index);

  // Then do the setup specific to the tiled case.

  // Find out which tile it belonds to
  jet->tile_index = _tile_index(jet->eta, jet->phi);

  // Insert it into the tile's linked list of jets
  Tile * tile = &_tiles[jet->tile_index];
  jet->previous   = NULL;
  jet->next       = tile->head;
  if (jet->next != NULL) {jet->next->previous = jet;}
  tile->head      = jet;
}


//----------------------------------------------------------------------
/// output the contents of the tiles
void FjClusterSequence::_print_tiles(TiledJet * briefjets ) const {
  for (vector<Tile>::const_iterator tile = _tiles.begin(); 
       tile < _tiles.end(); tile++) {
    cout << "Tile " << tile - _tiles.begin()<<" = ";
    vector<int> list;
    for (TiledJet * jetI = tile->head; jetI != NULL; jetI = jetI->next) {
      list.push_back(jetI-briefjets);
      //cout <<" "<<jetI-briefjets;
    }
    sort(list.begin(),list.end());
    for (unsigned int i = 0; i < list.size(); i++) {cout <<" "<<list[i];}
    cout <<"\n";
  }
}


//----------------------------------------------------------------------
/// Add to the vector tile_union the tiles that are in the neighbourhood
/// of the specified tile_index, including itself -- start adding
/// from position n_near_tiles-1, and increase n_near_tiles as
/// you go along (could have done it more C++ like with vector with reserved
/// space, but fear is that it would have been slower, e.g. checking
/// for end of vector at each stage to decide whether to resize it)
void FjClusterSequence::_add_neighbours_to_tile_union(const int tile_index, 
	       vector<int> & tile_union, int & n_near_tiles) const {
  for (Tile * const * near_tile = _tiles[tile_index].begin_tiles; 
       near_tile != _tiles[tile_index].end_tiles; near_tile++){
    // get the tile number
    tile_union[n_near_tiles] = *near_tile - & _tiles[0];
    n_near_tiles++;
  }
}


//----------------------------------------------------------------------
/// Like _add_neighbours_to_tile_union, but only adds neighbours if 
/// their "tagged" status is false; when a neighbour is added its
/// tagged status is set to true.
inline void FjClusterSequence::_add_untagged_neighbours_to_tile_union(
               const int tile_index, 
	       vector<int> & tile_union, int & n_near_tiles)  {
  for (Tile ** near_tile = _tiles[tile_index].begin_tiles; 
       near_tile != _tiles[tile_index].end_tiles; near_tile++){
    if (! (*near_tile)->tagged) {
      (*near_tile)->tagged = true;
      // get the tile number
      tile_union[n_near_tiles] = *near_tile - & _tiles[0];
      n_near_tiles++;
    }
  }
}


//----------------------------------------------------------------------
/// run a tiled clustering
void FjClusterSequence::_tiled_N2_cluster() {

  _initialise_tiles();

  int n = _jets.size();
  TiledJet * briefjets = new TiledJet[n];
  TiledJet * jetA = briefjets, * jetB;
  TiledJet oldB;
  

  // will be used quite deep inside loops, but declare it here so that
  // memory (de)allocation gets done only once
  vector<int> tile_union(3*n_tile_neighbours);
  
  // initialise the basic jet info 
  for (int i = 0; i< n; i++) {
    _bj_set_jetinfo(jetA, i);
    //cout << i<<": "<<jetA->tile_index<<"\n";
    jetA++; // move on to next entry of briefjets
  }
  TiledJet * tail = jetA; // a semaphore for the end of briefjets
  TiledJet * head = briefjets; // a nicer way of naming start

  // set up the initial nearest neighbour information
  vector<Tile>::const_iterator tile;
  for (tile = _tiles.begin(); tile != _tiles.end(); tile++) {
    // first do it on this tile
    for (jetA = tile->head; jetA != NULL; jetA = jetA->next) {
      for (jetB = tile->head; jetB != jetA; jetB = jetB->next) {
	double dist = _bj_dist(jetA,jetB);
	if (dist < jetA->NN_dist) {jetA->NN_dist = dist; jetA->NN = jetB;}
	if (dist < jetB->NN_dist) {jetB->NN_dist = dist; jetB->NN = jetA;}
      }
    }
    // then do it for RH tiles
    for (Tile ** RTile = tile->RH_tiles; RTile != tile->end_tiles; RTile++) {
      for (jetA = tile->head; jetA != NULL; jetA = jetA->next) {
	for (jetB = (*RTile)->head; jetB != NULL; jetB = jetB->next) {
	  double dist = _bj_dist(jetA,jetB);
	  if (dist < jetA->NN_dist) {jetA->NN_dist = dist; jetA->NN = jetB;}
	  if (dist < jetB->NN_dist) {jetB->NN_dist = dist; jetB->NN = jetA;}
	}
      }
    }
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

    //if (n == 19) {cout << "Hello "<<jetA-head<<" "<<jetB-head<<"\n";}

    //cout <<" WILL RECOMBINE "<< jetA-briefjets<<" "<<jetB-briefjets<<"\n";

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
      //cout <<n-1<<" "<<jetA-head<<" "<<jetB-head<<"; ";
      _add_step_to_history(history_location, 
			   min(hist_a,hist_b),max(hist_a,hist_b),
			   nn, diJ_min);
      // what was jetB will now become the new jet
      _bj_remove_from_tiles(jetA);
      oldB = * jetB;  // take a copy because we will need it...
      _bj_remove_from_tiles(jetB);
      _bj_set_jetinfo(jetB, nn); // also registers the jet in the tiling
    } else {
      // jet-beam recombination
      // get the hist_index
      int hist_a = _jets[jetA->_jets_index].cluster_hist_index();
      //cout <<n-1<<" "<<jetA-head<<" "<<-1<<"; ";
      _add_step_to_history(history_location,hist_a,BeamJet,Invalid,diJ_min); 
      _bj_remove_from_tiles(jetA);
    }

    // first establish the set of tiles over which we are going to 
    // have to run searches for updated and new nearest-neighbours
    int n_near_tiles = 0;
    _add_neighbours_to_tile_union(jetA->tile_index, tile_union, n_near_tiles);
    if (jetB != NULL) {
      bool sort_it = false;
      if (jetB->tile_index != jetA->tile_index) {
	sort_it = true;
	_add_neighbours_to_tile_union(jetB->tile_index,tile_union,n_near_tiles);
      }
      if (oldB.tile_index != jetA->tile_index && 
	  oldB.tile_index != jetB->tile_index) {
	sort_it = true;
	_add_neighbours_to_tile_union(oldB.tile_index,tile_union,n_near_tiles);
      }

      if (sort_it) {
	// sort the tiles before then compressing the list
	sort(tile_union.begin(), tile_union.begin()+n_near_tiles);
	// and now condense the list
	int nnn = 1;
	for (int i = 1; i < n_near_tiles; i++) {
	  if (tile_union[i] != tile_union[nnn-1]) {
	    tile_union[nnn] = tile_union[i]; 
	    nnn++;
	  }
	}
	n_near_tiles = nnn;
      }
    }

    // now update our nearest neighbour info and diJ table
    // first reduce size of table
    tail--; n--;
    if (jetA == tail) {
      // there is nothing to be done
    } else {
      // Copy last jet contents and diJ info into position of jetA
      *jetA = *tail;
      diJ[jetA - head] = diJ[tail-head];
      // IN the tiling fix pointers to tail and turn them into
      // pointers to jetA (from predecessors, successors and the tile
      // head if need be)
      if (jetA->previous == NULL) {
	_tiles[jetA->tile_index].head = jetA;
      } else {
	jetA->previous->next = jetA;
      }
      if (jetA->next != NULL) {jetA->next->previous = jetA;}
    }

    // Initialise jetB's NN distance as well as updating it for 
    // other particles.
    for (int itile = 0; itile < n_near_tiles; itile++) {
      Tile * tile = &_tiles[tile_union[itile]];
      for (TiledJet * jetI = tile->head; jetI != NULL; jetI = jetI->next) {
	// see if jetI had jetA or jetB as a NN -- if so recalculate the NN
	if (jetI->NN == jetA || (jetI->NN == jetB && jetB != NULL)) {
	  jetI->NN_dist = _R2;
	  jetI->NN      = NULL;
	  // now go over tiles that are neighbours of I (include own tile)
	  for (Tile ** near_tile  = tile->begin_tiles; 
	               near_tile != tile->end_tiles; near_tile++) {
	    // and then over the contents of that tile
	    for (TiledJet * jetJ  = (*near_tile)->head; 
                            jetJ != NULL; jetJ = jetJ->next) {
	      double dist = _bj_dist(jetI,jetJ);
	      if (dist < jetI->NN_dist && jetJ != jetI) {
		jetI->NN_dist = dist; jetI->NN = jetJ;
	      }
	    }
	  }
	  diJ[jetI-head] = _bj_diJ(jetI); // update diJ 
	}
	// check whether new jetB is closer than jetI's current NN and
	// if need to update things
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
      }
    }


    if (jetB != NULL) {diJ[jetB-head] = _bj_diJ(jetB);}
    //cout << n<<" "<<briefjets[95].NN-briefjets<<" "<<briefjets[95].NN_dist <<"\n";

    // remember to update pointers to tail
    for (Tile ** near_tile = _tiles[tail->tile_index].begin_tiles; 
	         near_tile!= _tiles[tail->tile_index].end_tiles; near_tile++){
      // and then the contents of that tile
      for (TiledJet * jetJ = (*near_tile)->head; 
	             jetJ != NULL; jetJ = jetJ->next) {
	if (jetJ->NN == tail) {jetJ->NN = jetA;}
      }
    }

    //for (int i = 0; i < n; i++) {
    //  if (briefjets[i].NN-briefjets >= n && briefjets[i].NN != NULL) {cout <<"YOU MUST BE CRAZY for n ="<<n<<", i = "<<i<<", NN = "<<briefjets[i].NN-briefjets<<"\n";}
    //}


    if (jetB != NULL) {diJ[jetB-head] = _bj_diJ(jetB);}
    //cout << briefjets[95].NN-briefjets<<" "<<briefjets[95].NN_dist <<"\n";

  }

  // final cleaning up;
  delete[] diJ;
  delete[] briefjets;
}


//----------------------------------------------------------------------
/// run a tiled clustering
void FjClusterSequence::_faster_tiled_N2_cluster() {

  _initialise_tiles();

  int n = _jets.size();
  TiledJet * briefjets = new TiledJet[n];
  TiledJet * jetA = briefjets, * jetB;
  TiledJet oldB;
  

  // will be used quite deep inside loops, but declare it here so that
  // memory (de)allocation gets done only once
  vector<int> tile_union(3*n_tile_neighbours);
  
  // initialise the basic jet info 
  for (int i = 0; i< n; i++) {
    _bj_set_jetinfo(jetA, i);
    //cout << i<<": "<<jetA->tile_index<<"\n";
    jetA++; // move on to next entry of briefjets
  }
  TiledJet * head = briefjets; // a nicer way of naming start

  // set up the initial nearest neighbour information
  vector<Tile>::const_iterator tile;
  for (tile = _tiles.begin(); tile != _tiles.end(); tile++) {
    // first do it on this tile
    for (jetA = tile->head; jetA != NULL; jetA = jetA->next) {
      for (jetB = tile->head; jetB != jetA; jetB = jetB->next) {
	double dist = _bj_dist(jetA,jetB);
	if (dist < jetA->NN_dist) {jetA->NN_dist = dist; jetA->NN = jetB;}
	if (dist < jetB->NN_dist) {jetB->NN_dist = dist; jetB->NN = jetA;}
      }
    }
    // then do it for RH tiles
    for (Tile ** RTile = tile->RH_tiles; RTile != tile->end_tiles; RTile++) {
      for (jetA = tile->head; jetA != NULL; jetA = jetA->next) {
	for (jetB = (*RTile)->head; jetB != NULL; jetB = jetB->next) {
	  double dist = _bj_dist(jetA,jetB);
	  if (dist < jetA->NN_dist) {jetA->NN_dist = dist; jetA->NN = jetB;}
	  if (dist < jetB->NN_dist) {jetB->NN_dist = dist; jetB->NN = jetA;}
	}
      }
    }
  }
  
  // now create the diJ (where J is i's NN) table -- remember that 
  // we differ from standard normalisation here by a factor of R2
  struct diJ_plus_link {
    double diJ;
    TiledJet * jet;
  };
  diJ_plus_link * diJ = new diJ_plus_link[n];
  jetA = head;
  for (int i = 0; i < n; i++) {
    diJ[i].diJ = _bj_diJ(jetA);
    diJ[i].jet = jetA;
    jetA->diJ_posn = i;
    jetA++; // have jetA follow i
  }

  // now run the recombination loop
  int history_location = n-1;
  while (n > 0) {

    // find the minimum of the diJ on this round
    double diJ_min = diJ[0].diJ;
    diJ_plus_link * best, *stop; // pointers a bit faster than indices
    best = diJ;
    stop = diJ+n;
    for (diJ_plus_link * here = diJ+1; here != stop; here++) {
      if (here->diJ < diJ_min) {best = here; diJ_min  = here->diJ;}
    }

    // do the recombination between A and B
    history_location++;
    jetA = best->jet;
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
      //cout <<n-1<<" "<<jetA-head<<" "<<jetB-head<<"; ";
      _add_step_to_history(history_location, 
			   min(hist_a,hist_b),max(hist_a,hist_b),
			   nn, diJ_min);
      // what was jetB will now become the new jet
      _bj_remove_from_tiles(jetA);
      oldB = * jetB;  // take a copy because we will need it...
      _bj_remove_from_tiles(jetB);
      _bj_set_jetinfo(jetB, nn); // also registers the jet in the tiling
    } else {
      // jet-beam recombination
      // get the hist_index
      int hist_a = _jets[jetA->_jets_index].cluster_hist_index();
      //cout <<n-1<<" "<<jetA-head<<" "<<-1<<"; ";
      _add_step_to_history(history_location,hist_a,BeamJet,Invalid,diJ_min); 
      _bj_remove_from_tiles(jetA);
    }

    // first establish the set of tiles over which we are going to
    // have to run searches for updated and new nearest-neighbours --
    // basically a combination of vicinity of the tiles of the two old
    // and one new jet.
    int n_near_tiles = 0;
    _add_untagged_neighbours_to_tile_union(jetA->tile_index, 
					   tile_union, n_near_tiles);
    if (jetB != NULL) {
      if (jetB->tile_index != jetA->tile_index) {
	_add_untagged_neighbours_to_tile_union(jetB->tile_index,
					       tile_union,n_near_tiles);
      }
      if (oldB.tile_index != jetA->tile_index && 
	  oldB.tile_index != jetB->tile_index) {
	_add_untagged_neighbours_to_tile_union(oldB.tile_index,
					       tile_union,n_near_tiles);
      }
    }

    // now update our nearest neighbour info and diJ table
    // first reduce size of table
    n--;
    // then compactify the diJ by taking the last of the diJ and copying
    // it to the position occupied by the diJ for jetA
    diJ[n].jet->diJ_posn = jetA->diJ_posn;
    diJ[jetA->diJ_posn] = diJ[n];

    // Initialise jetB's NN distance as well as updating it for 
    // other particles.
    for (int itile = 0; itile < n_near_tiles; itile++) {
      Tile * tile = &_tiles[tile_union[itile]];
      tile->tagged = false; // reset tag 
      for (TiledJet * jetI = tile->head; jetI != NULL; jetI = jetI->next) {
	// see if jetI had jetA or jetB as a NN -- if so recalculate the NN
	if (jetI->NN == jetA || (jetI->NN == jetB && jetB != NULL)) {
	  jetI->NN_dist = _R2;
	  jetI->NN      = NULL;
	  // now go over tiles that are neighbours of I (include own tile)
	  for (Tile ** near_tile  = tile->begin_tiles; 
	               near_tile != tile->end_tiles; near_tile++) {
	    // and then over the contents of that tile
	    for (TiledJet * jetJ  = (*near_tile)->head; 
                            jetJ != NULL; jetJ = jetJ->next) {
	      double dist = _bj_dist(jetI,jetJ);
	      if (dist < jetI->NN_dist && jetJ != jetI) {
		jetI->NN_dist = dist; jetI->NN = jetJ;
	      }
	    }
	  }
	  diJ[jetI->diJ_posn].diJ = _bj_diJ(jetI); // update diJ 
	}
	// check whether new jetB is closer than jetI's current NN and
	// if need to update things
	if (jetB != NULL) {
	  double dist = _bj_dist(jetI,jetB);
	  if (dist < jetI->NN_dist) {
	    if (jetI != jetB) {
	      jetI->NN_dist = dist;
	      jetI->NN = jetB;
	      diJ[jetI->diJ_posn].diJ = _bj_diJ(jetI); // update diJ...
	    }
	  }
	  if (dist < jetB->NN_dist) {
	    if (jetI != jetB) {
	      jetB->NN_dist = dist;
	      jetB->NN      = jetI;}
	  }
	}
      }
    }

    if (jetB != NULL) {diJ[jetB->diJ_posn].diJ = _bj_diJ(jetB);}

  }

  // final cleaning up;
  delete[] diJ;
  delete[] briefjets;
}


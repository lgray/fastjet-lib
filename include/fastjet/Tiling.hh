#ifndef __FASTJET_TILING_HH__
#define __FASTJET_TILING_HH__

//STARTHEADER
// $Id: ClusterSequence.hh 2867 2012-03-31 09:17:15Z salam $
//
// Copyright (c) 2005-2011, Matteo Cacciari, Gavin P. Salam and Gregory Soyez
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
//  along with FastJet. If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------
//ENDHEADER

//#include "fastjet/PseudoJet.hh"
#include "fastjet/internal/MinHeap.hh"
#include "fastjet/ClusterSequence.hh"

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

/// structure analogous to BriefJet, but with the extra information
/// needed for dealing with tiles
class TiledJet {
public:
  double     eta, phi, kt2, NN_dist;
  TiledJet * NN, *previous, * next; 
  int        _jets_index, tile_index;
  bool _minheap_update_needed;

  // indicate whether jets need to have their minheap entries
  // updated).
  inline void label_minheap_update_needed() {_minheap_update_needed = true;}
  inline void label_minheap_update_done()   {_minheap_update_needed = false;}
  inline bool minheap_update_needed() const {return _minheap_update_needed;}
};

const int n_tile_neighbours = 9;

struct Tile {
  /// pointers to neighbouring tiles, including self
  Tile *   begin_tiles[n_tile_neighbours]; 
  /// neighbouring tiles, excluding self
  Tile **  surrounding_tiles; 
  /// half of neighbouring tiles, no self
  Tile **  RH_tiles;  
  /// just beyond end of tiles
  Tile **  end_tiles; 
  /// start of list of BriefJets contained in this tile
  TiledJet * head;    
  /// sometimes useful to be able to tag a tile
  bool     tagged;    
  /// for all particles in the tile, this stores the largest of the
  /// (squared) nearest-neighbour distances.
  double max_NN_dist;
  double eta_centre, phi_centre;
};

//----------------------------------------------------------------------
class Tiling {
public:
  Tiling(const ClusterSequence & cs);

  void get_next_clustering(int & jetA_index, int & jetB_index, double & dij);
  

protected:
  const ClusterSequence & _cs;
  const std::vector<PseudoJet> & _jets;
  std::vector<Tile> _tiles;


  double _Rparam;
  double _tiles_eta_min, _tiles_eta_max;
  double _tile_size_eta, _tile_size_phi;
  double _tile_half_size_eta, _tile_half_size_phi;
  int    _n_tiles_phi,_tiles_ieta_min,_tiles_ieta_max;

  std::vector<TiledJet *> _jets_for_minheap;
  
  MinHeap _minheap;

  void _initialise_tiles();

  // reasonably robust return of tile index given ieta and iphi, in particular
  // it works even if iphi is negative
  inline int _tile_index (int ieta, int iphi) const {
    // note that (-1)%n = -1 so that we have to add _n_tiles_phi
    // before performing modulo operation
    return (ieta-_tiles_ieta_min)*_n_tiles_phi
                  + (iphi+_n_tiles_phi) % _n_tiles_phi;
  }

};


FASTJET_END_NAMESPACE

#endif // __FASTJET_TILING_HH__

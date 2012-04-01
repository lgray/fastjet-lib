//STARTHEADER
// $Id: ClusterSequence.hh 2863 2012-03-30 16:16:14Z salam $
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

#include "fastjet/Tiling.hh"
using namespace std;

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

Tiling::Tiling(const ClusterSequence & cs) :
  _cs(cs), _jets(cs.jets()), _minheap(_jets.size()) {
  _Rparam = cs.jet_def().R();
  _initialise_tiles();
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
void Tiling::_initialise_tiles() {

  // first decide tile sizes (with a lower bound to avoid huge memory use with
  // very small R)
  double default_size = max(0.1,_Rparam);
  _tile_size_eta = default_size;
  // it makes no sense to go below 3 tiles in phi -- 3 tiles is
  // sufficient to make sure all pair-wise combinations up to pi in
  // phi are possible
  _n_tiles_phi   = max(3,int(floor(twopi/default_size)));
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

  _tile_half_size_eta = _tile_size_eta * 0.5;
  _tile_half_size_phi = _tile_size_phi * 0.5;

  // allocate the tiles
  _tiles.resize((_tiles_ieta_max-_tiles_ieta_min+1)*_n_tiles_phi);

  // now set up the cross-referencing between tiles
  for (int ieta = _tiles_ieta_min; ieta <= _tiles_ieta_max; ieta++) {
    for (int iphi = 0; iphi < _n_tiles_phi; iphi++) {
      Tile * tile = & _tiles[_tile_index(ieta,iphi)];
      // no jets in this tile yet
      tile->head = NULL; // first element of tiles points to itself
      tile->begin_tiles[0] =  tile;
      Tile ** pptile = & (tile->begin_tiles[0]);
      pptile++;
      //
      // set up L's in column to the left of X
      tile->surrounding_tiles = pptile;
      if (ieta > _tiles_ieta_min) {
	// with the itile subroutine, we can safely run tiles from
	// idphi=-1 to idphi=+1, because it takes care of
	// negative and positive boundaries
	for (int idphi = -1; idphi <=+1; idphi++) {
	  *pptile = & _tiles[_tile_index(ieta-1,iphi+idphi)];
	  pptile++;
	}	
      }
      // now set up last L (below X)
      *pptile = & _tiles[_tile_index(ieta,iphi-1)];
      pptile++;
      // set up first R (above X)
      tile->RH_tiles = pptile;
      *pptile = & _tiles[_tile_index(ieta,iphi+1)];
      pptile++;
      // set up remaining R's, to the right of X
      if (ieta < _tiles_ieta_max) {
	for (int idphi = -1; idphi <= +1; idphi++) {
	  *pptile = & _tiles[_tile_index(ieta+1,iphi+idphi)];
	  pptile++;
	}	
      }
      // now put semaphore for end tile
      tile->end_tiles = pptile;
      // finally make sure tiles are untagged
      tile->tagged = false;
      // and ensure max distance is sensibly initialised
      tile->max_NN_dist = 0;
      // and also position of centre of tile
      tile->eta_centre = (ieta+0.5)*_tile_size_eta;
      tile->phi_centre = (iphi+0.5)*_tile_size_phi;
    }
  }

}


FASTJET_END_NAMESPACE

#ifndef __FASTJET_RECTANGULARGRID_HH__
#define __FASTJET_RECTANGULARGRID_HH__

//FJSTARTHEADER
// $Id$
//
// Copyright (c) 2005-2014, Matteo Cacciari, Gavin P. Salam and Gregory Soyez
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
//  development. They are described in the original FastJet paper,
//  hep-ph/0512210 and in the manual, arXiv:1111.6097. If you use
//  FastJet as part of work towards a scientific publication, please
//  quote the version you use and include a citation to the manual and
//  optionally also to hep-ph/0512210.
//
//  FastJet is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with FastJet. If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------
//FJENDHEADER

#include "fastjet/PseudoJet.hh"

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh


//----------------------------------------------------------------------
/// Class to indicate generic structure of tilings
class TilingBase {
public:
  /// returns the index of the tile in which p is located, or -1 if p
  /// is outside the tiling region
  virtual int index(const PseudoJet & p) const = 0;

  /// returns the total number of tiles in the tiling; valid tile
  /// indices run from 0 ... n_tiles()-1;
  virtual int n_tiles() const = 0;

  /// returns the number of tiles that are "good"; i.e. there is scope
  /// for having tiles that, for whatever reason, should be ignored;
  /// there are situations in which having "non-good" tiles may be the
  /// simplest mechanism to obtain a tiling with holes in it
  virtual int n_good_tiles() const {return n_tiles();}

  /// returns whether a give tile is good
  virtual bool is_good(int itile) const {return true;}

  /// returns true if all tiles have the same area
  virtual bool all_tiles_equal_area() const {return true;}

  /// returns the area of tile itile. Here with a default
  /// implementation to return mean_tile_area(), consistent with the
  /// fact that all_tiles_equal_area() returns true.
  virtual double tile_area(int /* itile */) const {return mean_tile_area();}

  /// returns the mean area of the tiles.
  virtual double mean_tile_area() const = 0;

  /// returns a string to describe the tiling
  virtual std::string description() const = 0;

};

//----------------------------------------------------------------------
/// Class that holds a generic rectangular tiling
class RectangularGrid : public TilingBase {
public:
  /// ctor with simple initialisation
  ///  \param rapmax     the maximal absolute rapidity extent of the grid
  ///  \param cell_size  the grid spacing (equivalently, cell size)
  RectangularGrid(double rapmax, double cell_size) :
      _ymax(rapmax), _ymin(-rapmax), 
      _requested_drap(cell_size), _requested_dphi(cell_size) {
    _setup_grid();
  }

  /// ctor with more control over initialisation
  ///  \param rapmin     the minimum rapidity extent of the grid
  ///  \param rapmax     the maximum rapidity extent of the grid
  ///  \param drap       the grid spacing in rapidity
  ///  \param dphi       the grid spacing in azimuth
  RectangularGrid(double rapmin, double rapmax, double drap, double dphi) 
    : _ymax(rapmax), _ymin(rapmin), 
      _requested_drap(drap), _requested_dphi(dphi) {
    _setup_grid();
  }

  /// dummy ctor (will give an unusable grid)
  RectangularGrid()
    : _ymax(-1.0), _ymin(1.0), _requested_drap(-1.0), _requested_dphi(-1.0) {
    _ntotal = 0;
  }

  // this was being kept inline, but it seems to make little
  // difference whether it is or not (at least on Gavin's mac)
  virtual int index(const PseudoJet & p) const;
  //  inline virtual int index(const PseudoJet & p) const;
//  {
//     // the code below has seem some degree of optimization: don't change
//     // it without testing the speed again
//     int iy = int(floor( (p.rap() - _ymin) * _inverse_dy ));
//     if (iy < 0 || iy >= _ny) return -1;
//     int iphi = int( p.phi() * _inverse_dphi );
//     if (iphi == _nphi) iphi = 0; // just in case of rounding errors
//     return iy*_nphi + iphi;
//   }


  virtual int n_tiles() const {return _ntotal;}

  /// returns the area of tile itile.
  virtual double tile_area(int itile) const {return mean_tile_area();}

  /// returns the mean area of tiles.
  virtual double mean_tile_area() const {return _dphi*_dy;};

  virtual std::string description() const;
  
private:
  void _setup_grid();
  
  // information about the requested grid
  double _ymax, _ymin;  ///< maximal and minimal rapidity coverage of the grid
  double _requested_drap; ///< requested rapidity spacing
  double _requested_dphi; ///< requested phi spacing

  // information about the actual grid
  double _dy, _dphi, _cell_area, _inverse_dy, _inverse_dphi;
  int _ny, _nphi, _ntotal;
};

FASTJET_END_NAMESPACE        // defined in fastjet/internal/base.hh

#endif // __FASTJET_RECTANGULARGRID_HH__

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


#ifndef __PSEUDOJET_H_
#define __PSEUDOJET_H_

#include<valarray>
#include<vector>
#include "numconsts.hh"

//using namespace std;

const double MaxRap = 1e5;

/// Class to contain pseudojets, including minimal information of use to
/// to jet-clustering routines.
class FjPseudoJet {
 public:
  FjPseudoJet() {};
  /// construct a pseudojet from explicit components
  FjPseudoJet(const double px, const double py, const double pz, const double E);
  /// constructor from any object that has px,py,pz,E = some_four_vector[0--3],
  template <class L> FjPseudoJet(const L & some_four_vector) ;

  // first "const double &" says that result is a reference to the
  // stored value and that we will not change that stored value.
  //
  // secont "const" says that "this" will not be modified by these
  // functions.
  inline const double & E()   const {return _E;};
  inline const double & e()   const {return _E;}; // like CLHEP
  inline const double & px()  const {return _px;};
  inline const double & py()  const {return _py;};
  inline const double & pz()  const {return _pz;};
  inline const double & phi() const {return _phi;}; // note 0--2pi
  inline const double & rap() const {return _rap;};
  inline const double & rapidity() const {return _rap;}; // like CLHEP
  inline const double & kt2() const {return _kt2;};
  inline const double & perp2() const {return _kt2;};  // like CLHEP
  inline double  perp() const {return sqrt(_kt2);};    // like CLHEP
  double operator () (int i) const ; // returns vector components
  inline double operator [] (int i) const { return (*this)(i); }; // this too

  // taken from CLHEP
  enum { X=0, Y=1, Z=2, T=3, NUM_COORDINATES=4, SIZE=NUM_COORDINATES };


  /// return the cluster_hist_index, intended to be used by clustering
  /// routines.
  inline const int & cluster_hist_index() const {return _cluster_hist_index;};
  /// set the cluster_hist_index, intended to be used by clustering routines.
  inline void set_cluster_hist_index(const int index) {_cluster_hist_index = index;};

  /// return the user_index, intended to allow the user to "add" information
  inline const int & user_index() const {return _user_index;};
  /// set the user_index, intended to allow the user to "add" information
  inline void set_user_index(const int index) {_user_index = index;};

  /// return a valarray containing the four-momentum (components 0-2
  /// are 3-mom, component 3 is energy).
  std::valarray<double> four_mom() const;

  /// returns kt distance between this jet and another
  double kt_distance(const FjPseudoJet & other) const;

  /// returns kt distance between this jet and another
  double plain_distance(const FjPseudoJet & other) const;

  // this seemed to compile except if it was used
  friend inline double 
    kt_distance(const FjPseudoJet & jet1, const FjPseudoJet & jet2) { 
                                        return jet1.kt_distance(jet2);};

  /// returns distance between this jet and the beam
  inline const double & beam_distance() const {return _kt2;};

  // maybe not necessary for it to be friend?
  // [but without it does not work...]
  friend FjPseudoJet operator+(const FjPseudoJet &, const FjPseudoJet &);

 private: 
  // NB: following order must be kept for things to behave sensibly...
  double _px,_py,_pz,_E;
  double _phi, _rap, _kt2; 
  int    _cluster_hist_index, _user_index;
  void _finish_init();
  //vertex_type * vertex0, vertex1;
};



//----------------------------------------------------------------------
// Routines to do with providing sorted arrays of vectors.

/// return a vector of jets sorted into decreasing transverse momentum
std::vector<FjPseudoJet> sorted_by_pt(const std::vector<FjPseudoJet> & jets);

/// return a vector of jets sorted into increasing rapidity
std::vector<FjPseudoJet> sorted_by_rapidity(const std::vector<FjPseudoJet> & jets);

/// return a vector of jets sorted into decreasing energy
std::vector<FjPseudoJet> sorted_by_E(const std::vector<FjPseudoJet> & jets);

//----------------------------------------------------------------------
// some code to help sorting

/// sort the indices so that values[indices[0->n-1]] is sorted
/// into increasing order 
void sort_indices(std::vector<int> & indices, 
		  const std::vector<double> & values);

/// given a vector of values with a one-to-one correspondence with the
/// vector of objects, sort objects into an order such that the
/// associated values would be in increasing order (but don't actually
/// touch the values vector in the process).
template<class T> std::vector<T> objects_sorted_by_values(const std::vector<T> & objects, 
					      const std::vector<double> & values);

/// a class that helps us carry out indexed sorting.
class IndexedSortHelper {
public:
  inline IndexedSortHelper (const std::vector<double> * reference_values) {
    _ref_values = reference_values;
  };
  inline int operator() (const int & i1, const int & i2) const {
    return  (*_ref_values)[i1] < (*_ref_values)[i2];
  };
private:
  const std::vector<double> * _ref_values;
};


//----------------------------------------------------------------------
/// constructor from any object that has px,py,pz,E = some_four_vector[0--3],
// NB: do not know if it really needs to be inline, but when it wasn't
//     linking failed with g++ (who knows what was wrong...)
template <class L> inline  FjPseudoJet::FjPseudoJet(const L & some_four_vector) {

  _px = some_four_vector[0];
  _py = some_four_vector[1];
  _pz = some_four_vector[2];
  _E  = some_four_vector[3];
  this->_finish_init();
};


////// fun and games...
////template<class L> class FJVector : public L {
//////  /** Default Constructor: create jet with no constituents */
//////  FjVector<L>();
////
////};
////

#endif // __PSEUDOJET_H_

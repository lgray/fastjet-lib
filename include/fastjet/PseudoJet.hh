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


// NB: to do at some point
//     - add += and *= operators

#ifndef __FASTJET_PSEUDOJET_HH__
#define __FASTJET_PSEUDOJET_HH__

#include<valarray>
#include<vector>
#include<cassert>
#include "fastjet/internal/numconsts.hh"

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

//using namespace std;

/// Used to protect against parton-level events where pt can be zero
/// for some partons, giving rapidity=infinity. KtJet fails in those cases.
const double MaxRap = 1e5;

/// Class to contain pseudojets, including minimal information of use to
/// to jet-clustering routines.
class PseudoJet {
 public:
  PseudoJet() {};
  /// construct a pseudojet from explicit components
  PseudoJet(const double px, const double py, const double pz, const double E);
  /// constructor from any object that has px,py,pz,E = some_four_vector[0--3],
  template <class L> PseudoJet(const L & some_four_vector) ;

  // first "const double &" says that result is a reference to the
  // stored value and that we will not change that stored value.
  //
  // second "const" says that "this" will not be modified by these
  // functions.
  inline double E()   const {return _E;};
  inline double e()   const {return _E;}; // like CLHEP
  inline double px()  const {return _px;};
  inline double py()  const {return _py;};
  inline double pz()  const {return _pz;};

  /// returns phi (in the range 0..2pi)
  inline const double phi() const {return phi_02pi();};

  /// returns phi in the range -pi..pi
  inline const double phi_std()  const {
    return _phi > pi ? _phi-twopi : _phi;};

  /// returns phi in the range 0..2pi
  inline const double phi_02pi() const {return _phi;};

  /// returns the rapidity or some large value when the rapidity
  /// is infinite
  inline double rap() const {return _rap;};

  /// the same as rap()
  inline double rapidity() const {return _rap;}; // like CLHEP

  /// returns the squared transverse momentum
  inline double kt2() const {return _kt2;};
  /// returns the squared transverse momentum
  inline double perp2() const {return _kt2;};  // like CLHEP
  /// returns the scalar transverse momentum
  inline double  perp() const {return sqrt(_kt2);};    // like CLHEP
  /// returns the squared invariant mass // like CLHEP
  inline double  m2() const {return (_E+_pz)*(_E-_pz)-_kt2;};    
  /// returns the invariant mass 
  /// (If m2() is negative then -sqrt(-m2()) is returned, as in CLHEP)
  inline double  m() const;    
  /// returns component i, where X==0, Y==1, Z==2, E==3
  double operator () (int i) const ; 
  /// returns component i, where X==0, Y==1, Z==2, E==3
  inline double operator [] (int i) const { return (*this)(i); }; // this too

  // taken from CLHEP
  enum { X=0, Y=1, Z=2, T=3, NUM_COORDINATES=4, SIZE=NUM_COORDINATES };


  /// return the cluster_hist_index, intended to be used by clustering
  /// routines.
  inline const int & cluster_hist_index() const {return _cluster_hist_index;};
  /// set the cluster_hist_index, intended to be used by clustering routines.
  inline void set_cluster_hist_index(const int index) {_cluster_hist_index = index;};

  /// alternative name for cluster_hist_index() [perhaps more meaningful]
  inline const int cluster_sequence_history_index() const {
    return cluster_hist_index();};
  /// alternative name for set_cluster_hist_index(...) [perhaps more
  /// meaningful]
  inline void set_cluster_sequence_history_index(const int index) {
    set_cluster_hist_index(index);};


  /// return the user_index, intended to allow the user to "add" information
  inline const int & user_index() const {return _user_index;};
  /// set the user_index, intended to allow the user to "add" information
  inline void set_user_index(const int index) {_user_index = index;};

  /// return a valarray containing the four-momentum (components 0-2
  /// are 3-mom, component 3 is energy).
  std::valarray<double> four_mom() const;

  /// returns kt distance (R=1) between this jet and another
  double kt_distance(const PseudoJet & other) const;

  /// returns squared cylinder (rap-phi) distance between this jet and another
  double plain_distance(const PseudoJet & other) const;
  /// returns squared cylinder (rap-phi) distance between this jet and
  /// another
  inline double squared_distance(const PseudoJet & other) const {
    return plain_distance(other);};

  //// this seemed to compile except if it was used
  //friend inline double 
  //  kt_distance(const PseudoJet & jet1, const PseudoJet & jet2) { 
  //                                      return jet1.kt_distance(jet2);};

  /// returns distance between this jet and the beam
  inline double beam_distance() const {return _kt2;};


  void operator*=(double);
  void operator/=(double);
  void operator+=(const PseudoJet &);
  void operator-=(const PseudoJet &);

 private: 
  // NB: following order must be kept for things to behave sensibly...
  double _px,_py,_pz,_E;
  double _phi, _rap, _kt2; 
  int    _cluster_hist_index, _user_index;
  void _finish_init();
  //vertex_type * vertex0, vertex1;
};


//----------------------------------------------------------------------
// routines for basic binary operations

PseudoJet operator+(const PseudoJet &, const PseudoJet &);
PseudoJet operator-(const PseudoJet &, const PseudoJet &);
PseudoJet operator*(double, const PseudoJet &);
PseudoJet operator*(const PseudoJet &, double);
PseudoJet operator/(const PseudoJet &, double);

/// returns true if the momenta of the two input jets are identical
bool have_same_momentum(const PseudoJet &, const PseudoJet &);

//----------------------------------------------------------------------
// Routines to do with providing sorted arrays of vectors.

/// return a vector of jets sorted into decreasing transverse momentum
std::vector<PseudoJet> sorted_by_pt(const std::vector<PseudoJet> & jets);

/// return a vector of jets sorted into increasing rapidity
std::vector<PseudoJet> sorted_by_rapidity(const std::vector<PseudoJet> & jets);

/// return a vector of jets sorted into decreasing energy
std::vector<PseudoJet> sorted_by_E(const std::vector<PseudoJet> & jets);

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
template <class L> inline  PseudoJet::PseudoJet(const L & some_four_vector) {

  _px = some_four_vector[0];
  _py = some_four_vector[1];
  _pz = some_four_vector[2];
  _E  = some_four_vector[3];
  this->_finish_init();
  // some default values for these two indices
  set_cluster_hist_index(-1);
  set_user_index(-1);
}


////// fun and games...
////template<class L> class FJVector : public L {
//////  /** Default Constructor: create jet with no constituents */
//////  Vector<L>();
////
////};
////

// taken literally from CLHEP
inline double PseudoJet::m() const {
  double mm = m2();
  return mm < 0.0 ? -std::sqrt(-mm) : std::sqrt(mm);
}


FASTJET_END_NAMESPACE

#endif // __FASTJET_PSEUDOJET_HH__

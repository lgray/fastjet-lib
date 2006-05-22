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
#include<valarray>
#include<iostream>
#include<cmath>

using namespace std;


//----------------------------------------------------------------------
// another constructor...
FjPseudoJet::FjPseudoJet(const double px, const double py, const double pz, const double E) {
  
  _E  = E ;
  _px = px;
  _py = py;
  _pz = pz;

  this->_finish_init();
};


//----------------------------------------------------------------------
/// do standard end of initialisation
void FjPseudoJet::_finish_init () {
  _kt2 = this->px()*this->px() + this->py()*this->py();
  if (_kt2 == 0.0) {
    _phi = 0.0; } 
  else {
    _phi = atan2(this->py(),this->px());
  }
  if (_phi < 0.0) {_phi += twopi;}
  if (_phi >= twopi) {_phi -= twopi;} // can happen is phi=-|eps<1e-15|?
  if (this->E() != abs(this->pz())) {
    _rap = 0.5*log((this->E() + this->pz())/(this->E() - this->pz()));
      } else {
    // Overlapping points can give problems. Let's lift the degeneracy
    // in case of multiple 0-pT points (can be found at parton-level)
    double MaxRapHere = MaxRap + abs(this->pz());
    if (this->pz() >= 0.0) {_rap = MaxRapHere;} else {_rap = -MaxRapHere;}
  }
}


//----------------------------------------------------------------------
// return a valarray four-momentum
valarray<double> FjPseudoJet::four_mom() const {
  valarray<double> mom(4);
  mom[0] = _px;
  mom[1] = _py;
  mom[2] = _pz;
  mom[3] = _E ;
  return mom;
}

//----------------------------------------------------------------------
// Return the component corresponding to the specified index.
// taken from CLHEP
double FjPseudoJet::operator () (int i) const {
  switch(i) {
  case X:
    return px();
  case Y:
    return py();
  case Z:
    return pz();
  case T:
    return e();
  default:
    std::cerr << "FjPseudoJet subscripting: bad index (" << i << ")"
		 << std::endl;
  }
  return 0.;
}  

//----------------------------------------------------------------------
// return "sum" of two pseudojets
FjPseudoJet operator+ (const FjPseudoJet & jet1, const FjPseudoJet & jet2) {
  return FjPseudoJet(jet1.four_mom()+jet2.four_mom());
} 

//----------------------------------------------------------------------
// return kt-distance between this jet and another one
double FjPseudoJet::kt_distance(const FjPseudoJet & other) const {
  //double distance = min(this->kt2(), other.kt2());
  double distance = min(_kt2, other._kt2);
  double dphi = abs(_phi - other._phi);
  if (dphi > pi) {dphi = twopi - dphi;}
  double drap = _rap - other._rap;
  distance = distance * (dphi*dphi + drap*drap);
  return distance;
}


//----------------------------------------------------------------------
// return squared cylinder (eta-phi) distance between this jet and another one
double FjPseudoJet::plain_distance(const FjPseudoJet & other) const {
  double dphi = abs(_phi - other._phi);
  if (dphi > pi) {dphi = twopi - dphi;}
  double drap = _rap - other._rap;
  return (dphi*dphi + drap*drap);
}

//----------------------------------------------------------------------
// sort the indices so that values[indices[0..n-1]] is sorted
// into increasing order 
void sort_indices(vector<int> & indices, 
			 const vector<double> & values) {
  IndexedSortHelper index_sort_helper(&values);
  sort(indices.begin(), indices.end(), index_sort_helper);
}

//----------------------------------------------------------------------
/// given a vector of values with a one-to-one correspondence with the
/// vector of objects, sort objects into an order such that the
/// associated values would be in increasing order
template<class T> vector<T>  objects_sorted_by_values(
                       const vector<T> & objects, 
		       const vector<double> & values) {

  assert(objects.size() == values.size());

  // get a vector of indices
  vector<int> indices(values.size());
  for (size_t i = 0; i < indices.size(); i++) {indices[i] = i;}
  
  // sort the indices
  sort_indices(indices, values);
  
  // copy the objects 
  vector<T> objects_sorted(objects.size());
  
  // place the objects in the correct order
  for (size_t i = 0; i < indices.size(); i++) {
    objects_sorted[i] = objects[indices[i]];
  }

  return objects_sorted;
}

//----------------------------------------------------------------------
/// return a vector of jets sorted into decreasing kt2
vector<FjPseudoJet> sorted_by_pt(const vector<FjPseudoJet> & jets) {
  vector<double> minus_kt2(jets.size());
  for (size_t i = 0; i < jets.size(); i++) {minus_kt2[i] = -jets[i].kt2();}
  return objects_sorted_by_values(jets, minus_kt2);
}

//----------------------------------------------------------------------
/// return a vector of jets sorted into increasing rapidity
vector<FjPseudoJet> sorted_by_rapidity(const vector<FjPseudoJet> & jets) {
  vector<double> rapidities(jets.size());
  for (size_t i = 0; i < jets.size(); i++) {rapidities[i] = jets[i].rap();}
  return objects_sorted_by_values(jets, rapidities);
}

//----------------------------------------------------------------------
/// return a vector of jets sorted into decreasing energy
vector<FjPseudoJet> sorted_by_E(const vector<FjPseudoJet> & jets) {
  vector<double> energies(jets.size());
  for (size_t i = 0; i < jets.size(); i++) {energies[i] = -jets[i].E();}
  return objects_sorted_by_values(jets, energies);
}


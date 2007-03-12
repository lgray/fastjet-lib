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


#include "fastjet/Error.hh"
#include "fastjet/PseudoJet.hh"
#include<valarray>
#include<iostream>
#include<sstream>
#include<cmath>

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

using namespace std;


//----------------------------------------------------------------------
// another constructor...
PseudoJet::PseudoJet(const double px, const double py, const double pz, const double E) {
  
  _E  = E ;
  _px = px;
  _py = py;
  _pz = pz;

  this->_finish_init();

  // some default values for these two indices
  set_cluster_hist_index(-1);
  set_user_index(-1);

}


//----------------------------------------------------------------------
/// do standard end of initialisation
void PseudoJet::_finish_init () {
  _kt2 = this->px()*this->px() + this->py()*this->py();
  if (_kt2 == 0.0) {
    _phi = 0.0; } 
  else {
    _phi = atan2(this->py(),this->px());
  }
  if (_phi < 0.0) {_phi += twopi;}
  if (_phi >= twopi) {_phi -= twopi;} // can happen if phi=-|eps<1e-15|?
  if (this->E() == abs(this->pz()) && _kt2 == 0) {
    // Point has infinite rapidity -- convert that into a very large
    // number, but in such a way that different 0-pt momenta will have
    // different rapidities (so as to lift the degeneracy between
    // them) [this can be relevant at parton-level]
    double MaxRapHere = MaxRap + abs(this->pz());
    if (this->pz() >= 0.0) {_rap = MaxRapHere;} else {_rap = -MaxRapHere;}
  } else {
    // get the rapidity in a way that's modestly insensitive to roundoff
    // error when things pz,E are large (actually the best we can do without
    // explicit knowledge of mass)
    double effective_m2 = max(0.0,m2()); // force non tachyonic mass
    double E_plus_pz    = _E + abs(_pz); // the safer of p+, p-
    // p+/p- = (p+ p-) / (p-)^2 = (kt^2+m^2)/(p-)^2
    _rap = 0.5*log((_kt2 + effective_m2)/(E_plus_pz*E_plus_pz));
    if (_pz > 0) {_rap = - _rap;}
  }

  //// original determination 
  //if (this->E() != abs(this->pz())) {
  //  _rap = 0.5*log((this->E() + this->pz())/(this->E() - this->pz()));
  //    } else {
  //  // Overlapping points can give problems. Let's lift the degeneracy
  //  // in case of multiple 0-pT points (can be found at parton-level)
  //  double MaxRapHere = MaxRap + abs(this->pz());
  //  if (this->pz() >= 0.0) {_rap = MaxRapHere;} else {_rap = -MaxRapHere;}
  //}
}


//----------------------------------------------------------------------
// return a valarray four-momentum
valarray<double> PseudoJet::four_mom() const {
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
double PseudoJet::operator () (int i) const {
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
    ostringstream err;
    err << "PseudoJet subscripting: bad index (" << i << ")";
    throw Error(err.str());
  }
  return 0.;
}  

//----------------------------------------------------------------------
// return "sum" of two pseudojets
PseudoJet operator+ (const PseudoJet & jet1, const PseudoJet & jet2) {
  //return PseudoJet(jet1.four_mom()+jet2.four_mom());
  return PseudoJet(jet1.px()+jet2.px(),
		   jet1.py()+jet2.py(),
		   jet1.pz()+jet2.pz(),
		   jet1.E() +jet2.E()  );
} 

//----------------------------------------------------------------------
// return difference of two pseudojets
PseudoJet operator- (const PseudoJet & jet1, const PseudoJet & jet2) {
  //return PseudoJet(jet1.four_mom()-jet2.four_mom());
  return PseudoJet(jet1.px()-jet2.px(),
		   jet1.py()-jet2.py(),
		   jet1.pz()-jet2.pz(),
		   jet1.E() -jet2.E()  );
} 

//----------------------------------------------------------------------
// return the product, coeff * jet
PseudoJet operator* (double coeff, const PseudoJet & jet) {
  //return PseudoJet(coeff*jet.four_mom());
  // the following code is hopefully more efficient
  PseudoJet coeff_times_jet(jet);
  coeff_times_jet *= coeff;
  return coeff_times_jet;
} 

//----------------------------------------------------------------------
// return the product, coeff * jet
PseudoJet operator* (const PseudoJet & jet, double coeff) {
  return coeff*jet;
} 

//----------------------------------------------------------------------
// return the ratio, jet / coeff
PseudoJet operator/ (const PseudoJet & jet, double coeff) {
  return (1.0/coeff)*jet;
} 

//----------------------------------------------------------------------
/// multiply the jet's momentum by the coefficient
void PseudoJet::operator*=(double coeff) {
  _px *= coeff;
  _py *= coeff;
  _pz *= coeff;
  _E  *= coeff;
  _kt2*= coeff*coeff;
  // phi and rap are unchanged
}

//----------------------------------------------------------------------
/// divide the jet's momentum by the coefficient
void PseudoJet::operator/=(double coeff) {
  (*this) *= 1.0/coeff;
}


//----------------------------------------------------------------------
/// add the other jet's momentum to this jet
void PseudoJet::operator+=(const PseudoJet & other_jet) {
  _px += other_jet._px;
  _py += other_jet._py;
  _pz += other_jet._pz;
  _E  += other_jet._E ;
  _finish_init(); // we need to recalculate phi,rap,kt2
}


//----------------------------------------------------------------------
/// subtract the other jet's momentum from this jet
void PseudoJet::operator-=(const PseudoJet & other_jet) {
  _px -= other_jet._px;
  _py -= other_jet._py;
  _pz -= other_jet._pz;
  _E  -= other_jet._E ;
  _finish_init(); // we need to recalculate phi,rap,kt2
}

//----------------------------------------------------------------------
/// transform this jet (given in the rest frame of prest) into a jet
/// in the lab frame;
//
// NB: code adapted from that in herwig f77 (checked how it worked
// long ago)
PseudoJet & PseudoJet::boost(const PseudoJet & prest) {
  
  if (prest.px() == 0.0 && prest.py() == 0.0 && prest.pz() == 0.0) 
    return *this;

  double m = prest.m();
  assert(m != 0);

  double pf4  = (  px()*prest.px() + py()*prest.py()
                 + pz()*prest.pz() + E()*prest.E() )/m;
  double fn   = (pf4 + E()) / (prest.E() + m);
  _px +=  fn*prest.px();
  _py +=  fn*prest.py();
  _pz +=  fn*prest.pz();
  _E = pf4;

  _finish_init(); // we need to recalculate phi,rap,kt2
  return *this;
}


//----------------------------------------------------------------------
/// transform this jet (given in the rest frame of prest) into a jet
/// in the lab frame;
//
// NB: code adapted from that in herwig f77 (checked how it worked
// long ago)
PseudoJet & PseudoJet::unboost(const PseudoJet & prest) {
  
  if (prest.px() == 0.0 && prest.py() == 0.0 && prest.pz() == 0.0) 
    return *this;

  double m = prest.m();
  assert(m != 0);

  double pf4  = ( -px()*prest.px() - py()*prest.py()
                 - pz()*prest.pz() + E()*prest.E() )/m;
  double fn   = (pf4 + E()) / (prest.E() + m);
  _px -=  fn*prest.px();
  _py -=  fn*prest.py();
  _pz -=  fn*prest.pz();
  _E = pf4;

  _finish_init(); // we need to recalculate phi,rap,kt2
  return *this;
}


//----------------------------------------------------------------------
/// returns true if the momenta of the two input jets are identical
bool have_same_momentum(const PseudoJet & jeta, const PseudoJet & jetb) {
  return jeta.px() == jetb.px()
    &&   jeta.py() == jetb.py()
    &&   jeta.pz() == jetb.pz()
    &&   jeta.E()  == jetb.E();
}


//----------------------------------------------------------------------
// return kt-distance between this jet and another one
double PseudoJet::kt_distance(const PseudoJet & other) const {
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
double PseudoJet::plain_distance(const PseudoJet & other) const {
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
vector<PseudoJet> sorted_by_pt(const vector<PseudoJet> & jets) {
  vector<double> minus_kt2(jets.size());
  for (size_t i = 0; i < jets.size(); i++) {minus_kt2[i] = -jets[i].kt2();}
  return objects_sorted_by_values(jets, minus_kt2);
}

//----------------------------------------------------------------------
/// return a vector of jets sorted into increasing rapidity
vector<PseudoJet> sorted_by_rapidity(const vector<PseudoJet> & jets) {
  vector<double> rapidities(jets.size());
  for (size_t i = 0; i < jets.size(); i++) {rapidities[i] = jets[i].rap();}
  return objects_sorted_by_values(jets, rapidities);
}

//----------------------------------------------------------------------
/// return a vector of jets sorted into decreasing energy
vector<PseudoJet> sorted_by_E(const vector<PseudoJet> & jets) {
  vector<double> energies(jets.size());
  for (size_t i = 0; i < jets.size(); i++) {energies[i] = -jets[i].E();}
  return objects_sorted_by_values(jets, energies);
}


FASTJET_END_NAMESPACE


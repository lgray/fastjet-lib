//STARTHEADER
// $Id$
//
// Copyright (c) 2005-2011, Matteo Cacciari, Gavin Salam and Gregory Soyez
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

#include <fastjet/tools/CASubJetTagger.hh>
#include <fastjet/ClusterSequence.hh>

#include <algorithm>
#include <cmath>
#include <sstream>

using namespace std;

FASTJET_BEGIN_NAMESPACE


// the tagger's description
//----------------------------------------------------------------------
string CASubJetTagger::description() const{
  ostringstream oss;
  oss << "CASubJetTagger with z_threshold=" << _z_threshold << " and scale choice ";
  switch (_scale_choice) {
  case kt2_distance:         oss << "kt2_distance";         break;
  case jade_distance:        oss << "jade_distance";        break;
  case jade2_distance:       oss << "jade2_distance";       break;
  case plain_distance:       oss << "plain_distance";       break;
  case mass_drop_distance:   oss << "mass_drop_distance";   break;
  case dot_product_distance: oss << "dot_product_distance"; break;
  default:
    throw fastjet::Error("unrecognized scale choice");
  }

  return oss.str();
}

// run the tagger on the given cs/jet
//----------------------------------------------------------------------
PseudoJet CASubJetTagger::result(const fastjet::PseudoJet & jet) const{
  // make sure that the jet results from a Cambridge/Aachen clustering
  if (jet.validated_cs()->jet_def().jet_algorithm() != cambridge_algorithm)
    throw Error("CASubJetTagger can only be applied on jets from a Cambridge/Aachen clustering");

  // copy the jet for future internal usage
  _original_jet = jet;

  // recurse in the jet to find the max distance
  JetAux aux;
  aux.jet          = jet;
  aux.aux_distance = 0.0;
  aux.delta_r      = 0.0;
  aux.z            = 1.0;
  _recurse_through_jet(jet, aux);

  // create the result and its associated structure
  PseudoJet result = aux.jet;
  
  CASubJetStructure * s = new CASubJetStructure(result);
  s->_scale_choice = _scale_choice;
  s->_distance     = aux.aux_distance;
  s->_absolute_z   = _absolute_z_cut;
  s->_z            = aux.z;

  // the tagger is considered to have failed if we've reached the
  // initial constituents (or the distance cut) without finding a
  // structure
  PseudoJet d1, d2;
  if ((!result.has_parents(d1, d2)) || (aux.delta_r*aux.delta_r < _dr2_min))
    result = PseudoJet();
  
  result.set_structure_shared_ptr(SharedPtr<PseudoJetStructureBase>(s));
  return result;
}


///----------------------------------------------------------------------
/// work through the jet, establishing a distance at each branching
inline void CASubJetTagger::_recurse_through_jet(const fastjet::PseudoJet & jet, JetAux &aux) const {

  fastjet::PseudoJet parent1, parent2;
  if (! jet.has_parents(parent1, parent2)) return;

  /// make sure the objects are not _too_ close together
  if (parent1.squared_distance(parent2) < _dr2_min) return;

  // distance
  double dist=0.0;
  switch (_scale_choice) {
  case kt2_distance:
    // a standard (LI) kt distance
    dist = parent1.kt_distance(parent2);
    break;
  case jade_distance:
    // something a bit like a mass: pti ptj Delta R_ij^2
    dist = parent1.perp()*parent2.perp()*parent1.squared_distance(parent2);
    break;
  case jade2_distance:
    // something a bit like a mass*deltaR^2: pti ptj Delta R_ij^4
    dist = parent1.perp()*parent2.perp()*pow(parent1.squared_distance(parent2),2);
    break;
  case plain_distance:
    // Delta R_ij^2
    dist = parent1.squared_distance(parent2);
    break;
  case mass_drop_distance:
    // Delta R_ij^2
    dist = jet.m() - std::max(parent1.m(),parent2.m());
    break;
  case dot_product_distance:
    // parent1 . parent2 
    // ( = jet.m2() - parent1.m2() - parent2.m() in a 
    // 4-vector recombination scheme) 
    dist = dot_product(parent1, parent2);
    break;
  default:
    throw fastjet::Error("unrecognized scale choice");
  }

  // check the z cut
  bool zcut1 = true;
  bool zcut2 = true;
  double z2 = 0.0;

  // not very efficient -- sort out later
  if (parent1.perp2() < parent2.perp2()) std::swap(parent1,parent2);

  if (_absolute_z_cut) {
    z2    = parent2.perp() / _original_jet.perp();
    zcut1 = parent1.perp() / _original_jet.perp() >= _z_threshold;
  } else {
    z2    = parent2.perp()/(parent1.perp()+parent2.perp());
  }
  zcut2 = z2 >= _z_threshold;

  if (zcut1 && zcut2){
    if (dist > aux.aux_distance){
      aux.jet          = jet;
      aux.aux_distance = dist;
      aux.delta_r      = sqrt(parent1.squared_distance(parent2));
      aux.z            = z2; // the softest
    }
  }    

  if (zcut1) _recurse_through_jet(parent1,aux);
  if (zcut2) _recurse_through_jet(parent2,aux);
}

FASTJET_END_NAMESPACE

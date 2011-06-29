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

#include "fastjet/tools/Filter.hh"
#include <fastjet/ClusterSequenceActiveAreaExplicitGhosts.hh>
#include <cassert>
#include <algorithm>
#include <sstream>

using namespace std;


FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

//----------------------------------------------------------------------
// Filter class implementation
//----------------------------------------------------------------------


// class description
string Filter::description() const {
  ostringstream ostr;
  ostr << "Filter with subjet_def = ";
  if (_Rfiltfunc)
    ostr << "Cambridge/Aachen algorithm with dynamic Rfilt";
  else
    ostr << _subjet_def.description();
  ostr<< ", and selection " << _selector.description();
  return ostr.str();
}


// return a vector of subjets, which are the ones that would be kept
// by the filtering
PseudoJet Filter::result(const PseudoJet &jet) const {
  // start by getting the list of subjets (including a list of sanity
  // checks)
  // NB: subjets is empty to begin with (see the comment for
  //     _set_filtered_elements_cafilt)
  vector<PseudoJet> subjets; 
  _set_filtered_elements(jet, subjets);

  // now build the vector of kept and rejected subjets
  vector<PseudoJet> kept, rejected;
  // Note that the following line is the one requiring that _selector
  // be declared as mutable
  if (_selector.takes_reference()) _selector.set_reference(jet);
  _selector.sift(subjets, kept, rejected);

  // gather the info under the form of a PseudoJet
  return _finalise(jet, kept, rejected);
}


// sets filtered_elements to be all the subjets on which filtering will work
void Filter::_set_filtered_elements(const PseudoJet & jet,
				    vector<PseudoJet> & filtered_elements) const {
  // sanity checks
  //-------------------------------------------------------------------
  // make sure that the jet has constituents
  if (! jet.has_constituents())
    throw Error("Filter can only be applied on jets having constituents");
  
  // if rho!=0, make sure we have a CS that supports area and has
  // explicit ghosts 
  // watch out: that will fail for a CompositeJet!!
  // TODO: add support for composite jets
  if (_rho != 0.0){
    if (!jet.has_area())   
      throw Error("Attempt to filter and subtract (non-zero rho) without area info for the original jet");

    // if (!jet.has_associated_cluster_sequence())
    //   throw Error("Attempt to filter and subtract (non-zero rho) without a cluster sequence associated with the jet");
    // 
    // // note that the validated_csab() used in the next line will
    // // automatically throw an error if there is no valis CSAB so we
    // // just have to check for the explicit ghosts
    // if (!jet.validated_csab()->has_explicit_ghosts())
    //   throw Error("Attempt to filter and subtract (non-zero rho) without explicit ghosts");

    if (!_recursively_check_explicit_ghosts(jet))
      throw Error("Attempt to filter and subtract (non-zero rho) without explicit ghosts");
  }

  // if we're dealing with a dynamic determination of the filtering
  // radius, do it now
  if (_Rfiltfunc)
    _subjet_def = JetDefinition(cambridge_algorithm, (*_Rfiltfunc)(jet));

  // get the jet definition to be use and whether we can apply our
  // simplified C/A+C/A filter
  //
  // we apply C/A clustering iff
  //  - the request subjet_def is C/A
  //  - the jet is either directly coming from C/A or if it is a
  //    superposition of C/A jets
  //  - the pieces agree with the recombination scheme of subjet_def
  //------------------------------------------------------------------
  bool simple_cafilt = _check_ca(jet);
 
  // extract the subjets
  //-------------------------------------------------------------------
  if (simple_cafilt){
    // first make sure that 'filtered_elemetns' is empty
    filtered_elements.clear();
    _set_filtered_elements_cafilt(jet, filtered_elements, _subjet_def.R());
  } else {
   _set_filtered_elements_generic(jet, filtered_elements);
  }

  // order the filtered elements in pt
  filtered_elements = sorted_by_pt(filtered_elements);
}


// gather the information about what is kept and rejected under the
// form of a PseudoJet with a special ClusterSequenceInfo
PseudoJet Filter::_finalise(const PseudoJet & jet, 
			    vector<PseudoJet> & kept, 
			    vector<PseudoJet> & rejected) const {
  // figure out which recombiner to use
  const JetDefinition::Recombiner &rec = *(_subjet_def.recombiner());

  // create an appropriate structure and transfer the info to it
  PseudoJet filtered_jet = join<StructureType>(kept, rec);
  StructureType *fs = (StructureType*) filtered_jet.structure_non_const_ptr();
  fs->_original_jet = jet;
  fs->_rejected = rejected;
  
  return filtered_jet;
}


// check if one can apply the simplification for C/A subjets
bool Filter::_check_ca(const PseudoJet & jet) const{
  if (_subjet_def.jet_algorithm() != cambridge_algorithm) return false;

  vector<PseudoJet> all_pieces;
  if (!(_recursively_check_ca(jet, all_pieces))) return false;
  if (! all_pieces.size()) return 0; // just in case one passes a CompositeJet with 0 pieces!

  // for now we know that all the pieces come from a C/A clustering
  // (hence have an associated cluster sequence)
  //
  // We'll enforce that they all come from the same ClusterSequence
  // (otherwise there may be interferences and we'd better recluster
  // the whole set of constituents)
  //
  // Note that we're sure there's at least one piece
  const ClusterSequence * cs_ref = all_pieces[0].associated_cluster_sequence();
  for (unsigned int i=1; i<all_pieces.size(); i++)
    if (all_pieces[i].associated_cluster_sequence() != cs_ref) return false;

  // vector<PseudoJet>::iterator pit = all_pieces.begin(); // there's at least 1
  // const ClusterSequence * cs_ref = pit->associated_cluster_sequence();
  // while (++pit != all_pieces.end())
  //   if (pit->associated_cluster_sequence() != cs_ref) return false;

  // we also have to make sure that the filtering radius is not larger
  // than any of the inter-pieces distance
  double Rfilt2 = _subjet_def.R();
  Rfilt2 *= Rfilt2;
  for (unsigned int i=0; i<all_pieces.size()-1; i++){
    for (unsigned int j=i+1; j<all_pieces.size(); j++){
      if (all_pieces[i].squared_distance(all_pieces[j]) <  Rfilt2) return false;
    }
  }

  return true;
}
	

// check if the jet is obtained from C/A or a superposition of C/A pieces
//
// Note that if the jet has an associated cluster sequence that is no
// longer valid, an error will be thrown
bool Filter::_recursively_check_ca(const PseudoJet & jet, vector<PseudoJet> &cumulative_pieces) const{
  if (jet.has_associated_cluster_sequence()){
    cumulative_pieces.push_back(jet);
    return jet.validated_cs()->jet_def().jet_algorithm() == cambridge_algorithm;
  }

  if (jet.has_pieces()){
    const vector<PseudoJet> pieces = jet.pieces();
    for (vector<PseudoJet>::const_iterator it=pieces.begin(); it!=pieces.end(); it++)
      if (!_recursively_check_ca(*it, cumulative_pieces)) return false;
    return true;
  }

  return false;
}


// check if the jet (or all its pieces) have explicit ghosts
// (assuming the jet has area support
//
// Note that if the jet has an associated cluster sequence that is no
// longer valid, an error will be thrown
bool Filter::_recursively_check_explicit_ghosts(const PseudoJet & jet) const{
  if (jet.has_associated_cluster_sequence())
    return jet.validated_csab()->has_explicit_ghosts();

  if (jet.has_pieces()){
    const vector<PseudoJet> pieces = jet.pieces();
    for (vector<PseudoJet>::const_iterator it=pieces.begin(); it!=pieces.end(); it++)
      if (!_recursively_check_explicit_ghosts(*it)) return false;
    return true;
  }

  return false;
}

// set the filtered elements in the simple case of C/A+C/A
//
// WATCH OUT: this could be recursively called, so filtered elements
//            of 'jet' are APPENDED to 'filtered_elements'
void Filter::_set_filtered_elements_cafilt(const PseudoJet & jet, 
					   vector<PseudoJet> & filtered_elements, 
					   double Rfilt) const{
  // we know that the jet is either a C/A jet or a superposition of
  // such pieces
  if (jet.has_associated_cluster_sequence()){
    // just extract the exclusive subjets of 'jet'
    const ClusterSequence *cs = jet.associated_cluster_sequence(); 
    vector<PseudoJet> local_fe;

    double dcut = Rfilt / cs->jet_def().R();
    if (dcut>=1.0){
      local_fe.push_back(jet);
    } else {
      dcut *= dcut;
      local_fe = jet.exclusive_subjets(dcut);
    }

    // subtract the jets if needed
    // Note that this one would work on pieces!!
    //-----------------------------------------------------------------
    if (_rho != 0.0){
      const ClusterSequenceAreaBase * csab = jet.validated_csab();
      for (unsigned int i=0;i<local_fe.size();i++)
	local_fe[i] = csab->subtracted_jet(local_fe[i], _rho);
    }

    copy(local_fe.begin(), local_fe.end(), back_inserter(filtered_elements));
    return;
  }

  // just recurse into the pieces
  const vector<PseudoJet> & pieces = jet.pieces();
  for (vector<PseudoJet>::const_iterator it = pieces.begin(); 
       it!=pieces.end(); it++)
    _set_filtered_elements_cafilt(*it, filtered_elements, Rfilt);
}


// set the filtered elements in the generic re-clustering case (wo
// subtraction)
void Filter::_set_filtered_elements_generic(const PseudoJet & jet, 
					    vector<PseudoJet> & filtered_elements) const{
  // create a new, internal, ClusterSequence from the jet constituents
  // get the subjets directly from there
  //
  // If the jet has area support then we separate the ghosts from the
  // "regular" particles so the subjets will also haev area
  // support. Note that we do this regardless of whether rho is zero
  // or not.
  // ---------------------------------------------------------------
  if (jet.has_area()){
    vector<PseudoJet> all_constituents = jet.constituents();
    vector<PseudoJet> regular_constituents, ghosts;  

    for (vector<PseudoJet>::iterator it = all_constituents.begin(); 
	 it != all_constituents.end(); it++){
      if (it->is_pure_ghost())
	ghosts.push_back(*it);
      else
	regular_constituents.push_back(*it);
    }

    // figure the ghost area from the 1st ghost (if none, any value
    // would probably do as the area will be 0 and subtraction will have
    // no effect!)
    double ghost_area = (ghosts.size()) ? ghosts[0].area() : 0.01;
    ClusterSequenceActiveAreaExplicitGhosts * csa
      = new ClusterSequenceActiveAreaExplicitGhosts(regular_constituents, 
						    _subjet_def, 
						    ghosts, ghost_area);

    // get the subjets: we use the subtracted or unsubtracted ones
    // depending on rho
    if (_rho != 0)
      filtered_elements = csa->subtracted_jets(_rho);
    else 
      filtered_elements = csa->inclusive_jets();
    
    // allow the cs to be deleted when it's no longer used
    csa->delete_self_when_unused();
  } else {
    ClusterSequence * cs = new ClusterSequence(jet.constituents(), _subjet_def);
    filtered_elements = cs->inclusive_jets();
    // allow the cs to be deleted when it's no longer used
    cs->delete_self_when_unused();
  }
}

//----------------------------------------------------------------------
// FilterInterface implementation 
//----------------------------------------------------------------------


FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

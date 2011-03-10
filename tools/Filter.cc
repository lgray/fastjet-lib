//STARTHEADER
// $Id: Filter.cc 1737 2010-07-08 16:19:56Z soyez $
//
// Copyright (c) 2009-2010, Matteo Cacciari, Gavin Salam and Gregory Soyez
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
  ostr << _subjet_def.description();
  ostr<< ", and selection " << _selector.description();
  return ostr.str();
}


// return a vector of subjets, which are the ones that would be kept by the filtering
PseudoJet Filter::operator()(const PseudoJet &jet) const {
  ClusterSequence *internal_cs;

  // start by getting the list of subjets (including a sanity check
  // that all the jets in the argument vector share the same
  // underlying ClusterSequence)
  vector<PseudoJet> subjets; // NB: empty to begin with (see the comment for _set_filtered_elements_cafilt)
  _set_filtered_elements(jet, subjets, internal_cs);

  // decide what to keep and what to reject
  // we first make a copy of the pointers then apply the selector
  vector<const PseudoJet*> subjet_pointers;
  for (unsigned int i=0;i<subjets.size(); i++)
    subjet_pointers.push_back(&(subjets[i]));

  Selector selector_copy = _selector;
  if (selector_copy.takes_reference()) selector_copy.set_reference(jet);
  selector_copy.nullify_non_selected(subjet_pointers);

  // now build the vector of kept and rejected subjets
  vector<PseudoJet> kept, rejected;
  for (unsigned int i=0;i<subjets.size(); i++){
    if (subjet_pointers[i]==0)
      rejected.push_back(subjets[i]);
    else
      kept.push_back(subjets[i]);
  }

  // gather the info under the form of a PseudoJet
  return _finalise(jet, kept, rejected, internal_cs);
}


// sets filtered_elements to be all the subjets on which filtering will work
void Filter::_set_filtered_elements(const PseudoJet & jet,
				    vector<PseudoJet> & filtered_elements,
				    ClusterSequence * &internal_cs) const {
  // sanity checks
  //-------------------------------------------------------------------
  // make sure that the jet has constituents
  if (! jet.has_constituents())
    throw Error("Filter can only be applied on jets having constituents");
  
  // if rho!=0, make sure we have a CS that supports area and has explicit ghosts
  // watch out: that will fail for a MergedJet!!x
  if (_rho != 0.0){
    if (!jet.has_area())   
      throw Error("Attempt to filter and subtract (non-zero rho) without area info for the original jet");

    if (!jet.has_associated_cluster_sequence())
      throw Error("Attempt to filter and subtract (non-zero rho) without a cluster sequence associated with the jet");

    // note theat the validated_csab() used in the next line will
    // automatically throw an error if there is no valis CSAB so we
    // just have to check for the explicit ghosts
    if (!jet.validated_csab()->has_explicit_ghosts())
      throw Error("Attempt to filter and subtract (non-zero rho) without explicit ghosts");
  }


  // get the jet definition to be use and whether we can apply our simplified C/A+C/A filter
  //
  // we apply C/A clustering iff
  //  - the request subjet_def is C/A
  //  - the jet is either directly coming from C/A or if it is a superposition of C/A jets
  //  - the pieces agree with the recombination scheme of subjet_def
  //-------------------------------------------------------------------
  bool simple_cafilt = (_subjet_def.jet_algorithm() == cambridge_algorithm) && (_recursively_check_ca(jet));
 
  // extract the subjets
  //-------------------------------------------------------------------
  if (simple_cafilt){
    _set_filtered_elements_cafilt(jet, filtered_elements, _subjet_def.R());
    internal_cs = NULL;
  } else {
    if (_rho != 0.0){
      internal_cs = _set_filtered_elements_generic_subtracted(jet, filtered_elements);
    } else {
      internal_cs = _set_filtered_elements_generic_unsubtracted(jet, filtered_elements);
    }
  }

  // order the filtered elements in pt
  filtered_elements = sorted_by_pt(filtered_elements);
}


// gather the information about what is kept and rejected under the
// form of a PseudoJet with a special ClusterSequenceInfo
PseudoJet Filter::_finalise(const PseudoJet & jet, vector<PseudoJet> & kept, vector<PseudoJet> & rejected, ClusterSequence * &internal_cs) const {
  PseudoJet filtered_jet(0.0,0.0,0.0,0.0);

  // create an appropriate structure and transfer the info to it
  FilteredJetStructure *fi = new FilteredJetStructure();

  fi->_original_jet = jet;
  fi->_pieces = kept;   // in the base interface
  fi->_rejected = rejected;

  if (internal_cs)
    fi->_internal_cs.reset(internal_cs);

  // to sum the kept pieces, extract the recombiner used for the sub-clustering
  const JetDefinition::Recombiner &rec = *(_subjet_def.recombiner());
  for (unsigned i = 0; i < kept.size(); i++)
    rec.plus_equal(filtered_jet, kept[i]);

  // make sure the filtered jet has the same index (cluster and user)
  // (i.e. "looks like") the original jet
  // what about extra info? 
  // TODO: do we want this: in principle, this could interfere with teh recombiner??
  filtered_jet.set_cluster_hist_index(jet.cluster_hist_index());
  filtered_jet.set_user_index(jet.user_index());

  // finally attach the clustering info to the PJ
  filtered_jet.set_structure_shared_ptr(SharedPtr<PseudoJetStructureBase>(fi));

  return filtered_jet;
}


// check if the jet is obtained from C/A or a superposition of C/A pieces
bool Filter::_recursively_check_ca(const PseudoJet & jet) const{
  if (jet.has_associated_cluster_sequence()) 
    return jet.associated_cluster_sequence()->jet_def().jet_algorithm() == cambridge_algorithm;

  if (jet.has_pieces()){
    const vector<PseudoJet> pieces = jet.pieces();
    for (vector<PseudoJet>::const_iterator it=pieces.begin(); it!=pieces.end(); it++)
      if (!_recursively_check_ca(*it)) return false;
    return true;
  }

  return false;
}



// set the filtered elements in the simple case of C/A+C/A
//
// WATCH OUT: this could be recursively called, so filtered elements
// of 'jet' are APPENDED to 'filtered_elements'
void Filter::_set_filtered_elements_cafilt(const PseudoJet & jet, vector<PseudoJet> & filtered_elements, double Rfilt) const{
  // we know that the jet is either a C/A jet or a superposition of such pieces
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
  for (vector<PseudoJet>::const_iterator it = pieces.begin(); it!=pieces.end(); it++)
    _set_filtered_elements_cafilt(*it, filtered_elements, Rfilt);
}


// set the filtered elements in the generic re-clustering case (wo subtraction)
ClusterSequence * Filter::_set_filtered_elements_generic_unsubtracted(const PseudoJet & jet, 
								      vector<PseudoJet> & filtered_elements) const{
  // create a new, internal, ClusterSequence from the jet constituents
  // get the subjets directly from there
  //---------------------------------------------------------------
  ClusterSequence * cs = new ClusterSequence(jet.constituents(), _subjet_def);
  filtered_elements = cs->inclusive_jets();

  return cs;
}

// set the filtered elements in the generic re-clustering case (with subtraction)
ClusterSequence * Filter::_set_filtered_elements_generic_subtracted(const PseudoJet & jet, 
								    vector<PseudoJet> & filtered_elements) const{
  // create a new, internal, ClusterSequence from jet constituents
  // 
  // the difference is that we need to separate the ghosts to get a
  // reliable area computation
  // ---------------------------------------------------------------
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
    = new ClusterSequenceActiveAreaExplicitGhosts(regular_constituents, _subjet_def, 
						  ghosts, ghost_area);
      
  // get the subjets
  filtered_elements = csa->subtracted_jets(_rho);

  return csa;
}



//----------------------------------------------------------------------
// FilterInterface implementation 
//----------------------------------------------------------------------


FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

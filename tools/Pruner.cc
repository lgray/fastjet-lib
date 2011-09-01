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

#include <fastjet/ClusterSequence.hh>
#include "fastjet/tools/Pruner.hh"
#include <cassert>
#include <algorithm>
#include <sstream>

using namespace std;


FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh


//----------------------------------------------------------------------
// class Pruner
// transformer that prunes a jet
//
// long description TBA
//----------------------------------------------------------------------
// action on a single jet
PseudoJet Pruner::result(const PseudoJet &jet) const{
  // at the moment we only deal with jets that have an associated
  // cluster sequence and we do not support area
  if (!jet.has_associated_cluster_sequence()){
    throw Error("Pruner: at the moment Pruner only handles jets with an associated ClusterSequence");
  }

  // build the pruning plugin
  PruningPlugin * pruning_plugin = 
    new PruningPlugin(JetDefinition(_jet_def.jet_algorithm(), 999.99, _jet_def.recombiner()),
		      _zcut, _Rcut);

  // now recluster the constituents of the jet with that plugin
  JetDefinition internal_jet_def(pruning_plugin);
  ClusterSequence * cs = new ClusterSequence(jet.constituents(), internal_jet_def);
  vector<PseudoJet> jets = cs->inclusive_jets();
  if (jets.size()!=1){
    ostringstream oss;
    oss << "Pruner: found " << jets.size() << " jets during re-clustering. Aborting (to be improved!)";
    throw Error(oss.str());
  }
  
  PseudoJet result = jets[0];
  
  // make sure things remain persistent when leaving
  internal_jet_def.delete_plugin_when_unused();
  cs->delete_self_when_unused();

  return result;  
}

//----------------------------------------------------------------------
// transformer description
std::string Pruner::description() const{
  ostringstream oss;
  oss << "Pruner with jet definition " << _jet_def.description()
      << ", Rcut=" << _Rcut
      << "and zcut=" << _zcut;
  return oss.str();
}




//----------------------------------------------------------------------
// class PruningRecombiner
// recombiner thet objects that are not vetoed by pruning
//
// This recombiner only recombine objects (i and j) that pass one of
// the following two criteria:
//
//  - the geometric distance between i and j is smaller than 'Rcut'
//  - the transverse momenta of i and j are at least 'zcut' p_t(i+j)
//
// If both these criteria fail, the hardest jet is kept and the
// softest rejected
//----------------------------------------------------------------------

//----------------------------------------------------------------------
// decide whether to recombine things or not
void PruningRecombiner::recombine(const PseudoJet &pa, 
				  const PseudoJet &pb,
				  PseudoJet &pab) const{
  PseudoJet p;
  _recombiner->recombine(pa, pb, p);

  // if the 2 particles are close enough, do the recombination
  if (pa.squared_distance(pb)<=_Rcut2){
    pab=p; return;
  }

  // check which is the softest
  double pt2a = pa.perp2();
  double pt2b = pb.perp2();

  if (pt2a < pt2b){
    if (pt2a<_zcut2*p.perp2()){
      pab = pb; _rejected.push_back(pa.cluster_hist_index());
    } else {
      pab = p;
    }
  } else {
    if (pt2b<_zcut2*p.perp2()) {
      pab = pa; _rejected.push_back(pb.cluster_hist_index());
    } else {
      pab = p;
    }
  }
}

string PruningRecombiner::description() const{
  ostringstream oss;
  oss << "Pruning recombiner with Rcut=" << sqrt(_Rcut2)
      << ", zcut=" << sqrt(_zcut2)
      << " and an underlying recombiner " << _recombiner->description();
  return oss.str();
}




//----------------------------------------------------------------------
// class PruningPlugin
// create a cluster sequence based on the pruning
//----------------------------------------------------------------------
void PruningPlugin::run_clustering(ClusterSequence &input_cs) const{
  // declare a pruning recombiner
  PruningRecombiner pruning_recombiner(_zcut, _Rcut, _jet_def.recombiner());
  JetDefinition jet_def = _jet_def;
  jet_def.set_recombiner(&pruning_recombiner);

  // cluster the particles using that recombiner
  ClusterSequence internal_cs(input_cs.jets(), jet_def);
  const vector<ClusterSequence::history_element> & internal_hist = internal_cs.history();

  // we could just browse the history of the internal CS and deide to
  // do only the recombinations when none of the two objects being
  // recombined are in the "rejected" list of the recombiner (*). That
  // would have the side effect that ff 2 particles are recombined and
  // the result is later vetoed, an orphaned part of the CS will be
  // floating around. We will proceed differently and discard the
  // clustering of 2 particles if any of their child is vetoed later
  // on in the clustering.
  //
  // this is achieved by building a vector, initially filled with
  // "true", of the history elements to be kept. For each of the
  // elements rejected by the pruning recombiner, set that element
  // _and its recursive parents_ to false.
  //
  // (*) note that the number in that list appear in the same order
  //     than in the history, so the search is trivial
  vector<bool> kept(internal_hist.size(), true);

  const vector<unsigned int> &pr_rej = pruning_recombiner.rejected();
  for (vector<unsigned int>::const_reverse_iterator rit=pr_rej.rbegin();
       rit!=pr_rej.rend(); rit++){
    if (kept[*rit]) _recursively_mark_as_rejected(*rit, internal_hist, kept);
  }

  // now reconstruct the final CS
  //
  // We map the internal CS to the input one (watch out: this is done
  // using history indices)
  vector<unsigned int> internal2input(internal_hist.size());
  for (unsigned int i=0; i<input_cs.jets().size(); i++)
    internal2input[i] = i;

  for (unsigned int i=input_cs.jets().size(); i<internal_hist.size(); i++){
    const ClusterSequence::history_element &he = internal_hist[i];

    // deal with recombinations with the beam
    if (he.parent2 == ClusterSequence::BeamJet){
      int internal_jetp_index = internal_hist[he.parent1].jetp_index;
      int internal_hist_index = internal_cs.jets()[internal_jetp_index].cluster_hist_index();

      // a safekeeper in case everything in that jet is rejected
      if (!kept[internal_hist_index]) continue; 

      int input_jetp_index = input_cs.history()[internal2input[internal_hist_index]].jetp_index;

      // cout << "Beam recomb for internal " << internal_hist_index
      // 	   << " (input jet index=" << input_jetp_index << endl;

      input_cs.plugin_record_iB_recombination(input_jetp_index, he.dij);
      continue;
    }

    // now, deal with two-body recombinations
    if (!kept[he.parent1]){ // 1 is rejected, we keep only 2
      internal2input[i]=internal2input[he.parent2];
      // cout << "rejecting internal " << he.parent1
      // 	   << ", mapping internal " << i 
      // 	   << " to internal " << he.parent2
      // 	   << " i.e. " << internal2input[i] << endl;
    } else if (!kept[he.parent2]){ // 2 is rejected, we keep only 1
      internal2input[i]=internal2input[he.parent1];
      // cout << "rejecting internal " << he.parent2 
      // 	   << ", mapping internal " << i 
      // 	   << " to internal " << he.parent1
      // 	   << " i.e. " << internal2input[i] << endl;
    } else { // do the recombination
      int new_index;
      input_cs.plugin_record_ij_recombination(input_cs.history()[internal2input[he.parent1]].jetp_index,
					      input_cs.history()[internal2input[he.parent2]].jetp_index,
					      he.dij, internal_cs.jets()[he.jetp_index], new_index);
      internal2input[i]=input_cs.jets()[new_index].cluster_hist_index();
      // cout << "merging " << internal2input[he.parent1] << " (int: " << he.parent1 << ")"
      // 	   << " and "    << internal2input[he.parent2] << " (int: " << he.parent2 << ")"
      // 	   << " into "   << internal2input[i] << " (int: " << i << ")" << endl;
    }
  }
}

string PruningPlugin::description() const{
  ostringstream oss;
  oss << "Pruning plugin with jet definition " << _jet_def.description()
      << ", Rcut=" << _Rcut
      << "and zcut=" << _zcut;
  return oss.str();
}

void PruningPlugin::_recursively_mark_as_rejected(const unsigned int i,
      const vector<ClusterSequence::history_element> & hist, 
      vector<bool> & kept) const{
  //cout << "rejecting " << i << endl;
  kept[i]=false;
  if (hist[i].parent1 != ClusterSequence::InexistentParent){
    _recursively_mark_as_rejected(hist[i].parent1, hist, kept);
    _recursively_mark_as_rejected(hist[i].parent2, hist, kept);
  }      
}


FASTJET_END_NAMESPACE

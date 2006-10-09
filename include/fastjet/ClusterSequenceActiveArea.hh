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

#ifndef __FASTJET_CLUSTERSEQUENCEACTIVEAREA_HH__
#define __FASTJET_CLUSTERSEQUENCEACTIVEAREA_HH__


#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequenceWithArea.hh"
#include "fastjet/ClusterSequenceActiveAreaExplicitGhosts.hh"
#include<iostream>
#include<vector>

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

using namespace std;

/// Class that behaves essentially like ClusterSequence except
/// that it also provides access to the area of a jet (which
/// will be a random quantity... Figure out what to do about seeds 
/// later...)
class ClusterSequenceActiveArea : public ClusterSequenceWithArea {
public:

  /// constructor based on JetDefinition and ActiveAreaSpec
  template<class L> ClusterSequenceActiveArea
         (const std::vector<L> & pseudojets, 
	  const JetDefinition & jet_def,
	  const ActiveAreaSpec & area_spec,
	  const bool & writeout_combinations = false) ;

  virtual double area (const PseudoJet & jet) const {
                             return _average_area[jet.cluster_hist_index()];};
  virtual double area_error (const PseudoJet & jet) const {
                             return _average_area2[jet.cluster_hist_index()];};

  virtual PseudoJet area_4vector (const PseudoJet & jet) const {
                    return _average_area_4vector[jet.cluster_hist_index()];};

  /// enum providing a variety of tentative strategies for estimating
  /// the background (e.g. non-jet) activity in a highly populated event; the
  /// one that has been most extensively tested is median.
  enum mean_pt_strategies{median=0, non_ghost_median, pttot_over_areatot, 
			  pttot_over_areatot_cut, mean_ratio_cut, play};

  /// return the transverse momentum per unit area according to one
  /// of the above strategies; for some strategies (those with "cut"
  /// in their name) the parameter "range" allows one to exclude a
  /// subset of the jets for the background estimation, those that
  /// have pt/area > median(pt/area)*range.
  double pt_per_unit_area(mean_pt_strategies strat=median, 
                          double range=2.0 ) const;

  /// fits a form pt_per_unit_area(y) = a + b*y^2 in the range
  /// abs(y)<raprange (for negative raprange, it defaults to
  /// _etalim_for_area).
  void parabolic_pt_per_unit_area(double & a,double & b, double raprange=-1.0,
				  double exclude_above=-1.0) const;



private:

  /// does the initialisation and running specific to the active
  /// areas class
  void _initialise_and_run_AA (const JetDefinition & jet_def,
                               const ActiveAreaSpec & area_spec,
                               const bool & writeout_combinations = false);


  valarray<double> _average_area, _average_area2;
  valarray<PseudoJet> _average_area_4vector;
  double           _non_jet_area, _non_jet_area2, _non_jet_number;

  double _etamax_for_area; // max eta where we put ghosts
  double _etalim_for_area; // max eta where we trust jet areas

  /// transfer the history (and jet-momenta) from clust_seq to our
  /// own internal structure while removing ghosts
  void _transfer_ghost_free_history(
           const ClusterSequenceActiveAreaExplicitGhosts & clust_seq);


  /// transfer areas from the ClusterSequenceActiveAreaExplicitGhosts
  /// object into our internal area bookkeeping...
  void _transfer_areas(const vector<int> & unique_hist_order, 
                       const ClusterSequenceActiveAreaExplicitGhosts & );

  /// routine for extracting the tree in an order that will be independent
  /// of any degeneracies in the recombination sequence that don't
  /// affect the composition of the final jets
  void _extract_tree(vector<int> &) const;
  /// do the part of the extraction associated with pos, working
  /// through its children and their parents
  void _extract_tree_children(int pos, valarray<bool> &, const valarray<int> &, vector<int> &) const;
  /// do the part of the extraction associated with the parents of pos.
  void _extract_tree_parents (int pos, valarray<bool> &, const valarray<int> &,  vector<int> &) const;

  /// since we are playing nasty games with seeds, we should warn
  /// the user a few times
  //static int _n_seed_warnings;
  //const static int _max_seed_warnings = 10;

};


template<class L> ClusterSequenceActiveArea::ClusterSequenceActiveArea 
(const std::vector<L> & pseudojets, 
 const JetDefinition & jet_def,
 const ActiveAreaSpec & area_spec,
 const bool & writeout_combinations) {

  // transfer the initial jets (type L) into our own array
  _transfer_input_jets(pseudojets);

  // run the clustering for active areas
  _initialise_and_run_AA(jet_def, area_spec, writeout_combinations);

}


  
FASTJET_END_NAMESPACE

#endif // __FASTJET_CLUSTERSEQUENCEACTIVEAREA_HH__

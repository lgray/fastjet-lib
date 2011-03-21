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

// design questions?
//
//  - keep the option to define things with just a Selector and a list of particles
//    HOWTO:
//     . ClusterSequenceArea:
//        o impose an area for the Selector
//        o if not relocatable, take ghosts maxrap from there
//          otherwise, get it from the maxrap of the particles
//          warn in the doc that one should set a decent maxrap
//        o use kt, R=0.6
//        o active_area_explicit_ghosts
//        o have functions that allow to control the maxrap, the ghost area, the alg, R
//
//     . have a SharedPtr<ClusterSequenceArea> initialised only in that case
//       remove the const CSA & _csa;
//     . access the cs using 
//         dynamic_cast<ClusterSequenceArea*>(_csw()->cs());
//       
//
//  - use a default Selector: SelectorStrip(2.0) * !SelectorNHardest(2)

#include "fastjet/tools/BackgroundEstimator.hh"
#include <fastjet/ClusterSequenceAreaBase.hh>
#include <fastjet/ClusterSequenceStructure.hh>
#include <iostream>

FASTJET_BEGIN_NAMESPACE     // defined in fastjet/internal/base.hh

using namespace std;


/// allow for warnings
LimitedWarning BackgroundEstimator::_warnings;
LimitedWarning BackgroundEstimator::_warnings_zero_area;


//---------------------------------------------------------------------
// class BackgroundEstimator
// Class to estimate the density of the background per unit area
//---------------------------------------------------------------------

// default ctor
//  - csa        the ClusterSequenceArea to use
//  - rho_range  the range over which jets will be considered
BackgroundEstimator::BackgroundEstimator(const ClusterSequenceAreaBase &csa, const Selector &rho_range)
  : _rho_range(rho_range){

  _csi = csa.structure_shared_ptr();

  // sanity checks
  //---------------
  //  (i) check the alg is appropriate
  _check_jet_alg_good_for_median();

  //  (ii) check that, if there are no explicit ghosts, the selector has an area
  if ((!csa.has_explicit_ghosts()) && (!_rho_range.has_area())){
    throw Error("BackgroundEstimator: either an area with explicit ghosts (recommended) or a Selector with finite area is needed (to allow for the computation of the empty area)");
  }

  // get the initial list of jets
  _included_jets = csa.inclusive_jets();

  // initialise things properly
  reset();
}


// ctor from a list of jets
//  - jets        the set of jets to use for the computation
//  - rho_range   the range over which jets will be considered
BackgroundEstimator::BackgroundEstimator(const vector<PseudoJet> &jets, const Selector &rho_range)
  : _rho_range(rho_range){

  if (! jets.size())
    throw Error("BackgroundEstimator::BackgroundEstimator: At least one jet is needed to compute the background properties");

  // sanity checks
  //---------------
  //  (o) check that there is an underlying CS shared by all the jets
  if (! (jets[0].has_associated_cluster_sequence()) && (jets[0].has_area()))
    throw Error("BackgroundEstimator::BackgroundEstimator: the jets used to estimate the background properties must be associated with a valid ClusterSequenceAreaBase");

  _csi = jets[0].structure_shared_ptr();
  ClusterSequenceStructure * csi = dynamic_cast<ClusterSequenceStructure*>(_csi());
  const ClusterSequenceAreaBase * csab = csi->validated_csab();

  for (unsigned int i=1;i<jets.size(); i++){
    if (! jets[i].has_associated_cluster_sequence()) // area automatic if the next test succeeds
      throw Error("BackgroundEstimator::BackgroundEstimator: the jets used to estimate the background properties must be associated with a valid ClusterSequenceAreaBase");

    if (jets[i].structure_shared_ptr().get() != _csi.get())
      throw Error("BackgroundEstimator::BackgroundEstimator: all the jets used to estimate the background properties must share the same ClusterSequence");
  }

  //  (i) check the alg is appropriate
  _check_jet_alg_good_for_median();

  //  (ii) check that, if there are no explicit ghosts, the selector has an area
  if ((!csab->has_explicit_ghosts()) && (!_rho_range.has_area())){
    throw Error("BackgroundEstimator: either an area with explicit ghosts (recommended) or a Selector with finite area is needed (to allow for the computation of the empty area)");
  }


  // get the initial list of jets
  _included_jets = jets;

  // initialise things properly
  reset();
}


// default dtor
BackgroundEstimator::~BackgroundEstimator(){

}


// for estimation using a relocatable selector (i.e. local range)
// this allows to set its position. Note that this HAS to be called
// before any attempt to compute the background properties
BackgroundEstimator & BackgroundEstimator::set_reference(const PseudoJet &jet){
  // if the range is norrelocatable, do nothing
  if (_rho_range.takes_reference()){
    // relocate the range and make sure things get recomputed the next
    // time one tries to get some information
    _rho_range.set_reference(jet);
    _uptodate=false;
  }

  return *this;
}

// reset to default values
// 
// set the variou options to their default values
void BackgroundEstimator::reset(){
  // set the remaining default parameters
  set_use_area_4vector();  // true by default

  // reset the computed values
  _rho = _sigma = 0.0;
  _n_jets_used = _n_empty_jets = 0;
  _empty_area = _mean_area = 0.0;

  _uptodate = false;
}


// do the actual job
void BackgroundEstimator::_compute(){
  // check if the clustersequence is still valid
  _check_csa_alive();

  // fill the vector of pt/area with the jets 
  //  - in included_jets
  //  - not in excluded_jets
  //  - in the range
  vector<double> pt_over_areas;
  double total_area  = 0.0;
  
  _n_jets_used = 0;

  // apply the selector to the included jets
  _selected_jets = _rho_range(_included_jets);

  // compute the pt/area for the selected jets
  for (unsigned i = 0; i < _selected_jets.size(); i++) {
    const PseudoJet & current_jet = _selected_jets[i];

    double this_area = (_use_area_4vector) ? current_jet.area_4vector().perp() : current_jet.area(); 

    if (this_area>0){
      pt_over_areas.push_back(current_jet.perp()/this_area);
      total_area  += this_area;
      _n_jets_used++;
    } else {
      _warnings_zero_area.warn("BackgroundEstimator::get_median_rho_and_sigma(...): discarded jet with zero area. Zero-area jets may be due to (i) too large a ghost area (ii) a jet being outside the ghost range (iii) the computation not being done using an appropriate algorithm (kt;C/A).");
    }
      
  }
  
  // there is nothing inside our region, so answer will always be zero
  if (pt_over_areas.size() == 0) {
    _rho        = 0.0;
    _sigma      = 0.0;
    _mean_area  = 0.0;
    return;
  }

  // get median (pt/area) [this is the "old" median definition. It considers
  // only the "real" jets in calculating the median, i.e. excluding the
  // only-ghost ones; it will be supplemented with more info below]
  sort(pt_over_areas.begin(), pt_over_areas.end());

  // determine the number of empty jets
  _empty_area = 0.0;
  _n_empty_jets = 0.0;
  const ClusterSequenceAreaBase * csab = (dynamic_cast<ClusterSequenceStructure*>(_csi()))->validated_csab();

  if (csab->has_explicit_ghosts()) {
    _empty_area = 0.0;
    _n_empty_jets = 0;
  } else {
    // note that we are sure that the selector has an area
    // And we also need to use the scalar area
    if (_use_area_4vector){
      _empty_area = _rho_range.area();
      for (unsigned i = 0; i < _selected_jets.size(); i++) _empty_area -= _selected_jets[i].area();
    } else {
      _empty_area = _rho_range.area() - total_area;
    }
    //if (_empty_area<0) _empty_area = 0;
    double Rused = csab->jet_def().R();
    _n_empty_jets = _empty_area / (0.55*pi*Rused*Rused);
  }

  double total_njets = _n_jets_used + _n_empty_jets;
  total_area  += _empty_area;

  // now get the median & error, accounting for empty jets
  // define the fractions of distribution at median, median-1sigma
  double posn[2] = {0.5, (1.0-0.6827)/2.0};
  double res[2];

  for (int i = 0; i < 2; i++) {
    double nj_median_pos = (total_njets-1)*posn[i] - _n_empty_jets;
    double nj_median_ratio;
    if (nj_median_pos >= 0 && pt_over_areas.size() > 1) {
      int int_nj_median = int(nj_median_pos);
      nj_median_ratio =
        pt_over_areas[int_nj_median] * (int_nj_median+1-nj_median_pos)
        + pt_over_areas[int_nj_median+1] * (nj_median_pos - int_nj_median);
    } else {
      nj_median_ratio = 0.0;
    }
    res[i] = nj_median_ratio;
  }

  // store the results
  double error  = res[0] - res[1];
  _rho        = res[0];
  _mean_area  = total_area / total_njets;
  _sigma      = error * sqrt(_mean_area);

  // record that the computation has been performed  
  _uptodate = true;
}


// check that the underlying structure is still alive
// throw an error otherwise
void BackgroundEstimator::_check_csa_alive(){
  if (! dynamic_cast<ClusterSequenceStructure*>(_csi())->has_associated_cluster_sequence())
    throw Error("BackgroundEstimator: modifications are no longer possible as the underlying ClusterSequence has gone out of scope");
}


// check that the algorithm used for the clustering is adapted for
// background estimation (i.e. either kt or C/A)
// Issue a warning otherwise
void BackgroundEstimator::_check_jet_alg_good_for_median(){
  const ClusterSequence * cs = dynamic_cast<ClusterSequenceStructure*>(_csi())->validated_cs();

  if (cs->jet_def().jet_algorithm() != kt_algorithm
      && cs->jet_def().jet_algorithm() != cambridge_algorithm
      && cs->jet_def().jet_algorithm() != cambridge_for_passive_algorithm) {
    _warnings.warn("BackgroundEstimator: jet_def being used may not be suitable for estimating diffuse backgrounds (good alternatives are kt, cam)");
  }
}

FASTJET_END_NAMESPACE



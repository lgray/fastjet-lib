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

double BackgroundJetScalarPtDensity::result(const PseudoJet & jet) const {
  std::vector<PseudoJet> constituents = jet.constituents();
  double scalar_pt = 0;
  for (unsigned i = 0; i < constituents.size(); i++) {
    scalar_pt += pow(constituents[i].perp(), _pt_power);
  }
  return scalar_pt / jet.area();
}


//----------------------------------------------------------------------
double BackgroundRescalingYPolynomial::result(const PseudoJet & jet) const {
  double y = jet.rap();
  double y2 = y*y;
  double rescaling = _a0 + _a1*y + _a2*y2 + _a3*y2*y + _a4*y2*y2;
  return rescaling;
}

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

  // initialise things properly
  reset();

  // tell the BGE about the cluster sequence
  set_cluster_sequence(csa);
}



//----------------------------------------------------------------------
// ctor from a list of jets
//  - jets        the set of jets to use for the computation
//  - rho_range   the range over which jets will be considered
BackgroundEstimator::BackgroundEstimator(const vector<PseudoJet> &jets, const Selector &rho_range)
  : _rho_range(rho_range){

  // initialise things properly
  reset();
}


// default dtor
BackgroundEstimator::~BackgroundEstimator(){

}


//----------------------------------------------------------------------
void BackgroundEstimator::set_cluster_sequence(const ClusterSequenceAreaBase & csa) {
  _csi = csa.structure_shared_ptr();

  // sanity checks
  //---------------
  //  (i) check the alg is appropriate
  _check_jet_alg_good_for_median();

  //  (ii) check that, if there are no explicit ghosts, the selector has a finite area
  if ((!csa.has_explicit_ghosts()) && (!_rho_range.has_finite_area())){
    throw Error("BackgroundEstimator: either an area with explicit ghosts (recommended) or a Selector with finite area is needed (to allow for the computation of the empty area)");
  }

  // get the initial list of jets
  _included_jets = csa.inclusive_jets();

  _uptodate = false;
}


//----------------------------------------------------------------------
void BackgroundEstimator::set_jets(const vector<PseudoJet> &jets) {
  
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
      throw Error("BackgroundEstimator::set_jets(...): the jets used to estimate the background properties must be associated with a valid ClusterSequenceAreaBase");

    if (jets[i].structure_shared_ptr().get() != _csi.get())
      throw Error("BackgroundEstimator::set_jets(...): all the jets used to estimate the background properties must share the same ClusterSequence");
  }

  //  (i) check the alg is appropriate
  _check_jet_alg_good_for_median();

  //  (ii) check that, if there are no explicit ghosts, the selector has a finite area
  if ((!csab->has_explicit_ghosts()) && (!_rho_range.has_finite_area())){
    throw Error("BackgroundEstimator: either an area with explicit ghosts (recommended) or a Selector with finite area is needed (to allow for the computation of the empty area)");
  }


  // get the initial list of jets
  _included_jets = jets;

  // ensure recalculation of quantities that need it
  _uptodate = false;
}


//----------------------------------------------------------------------
// for estimation using a relocatable selector (i.e. local range)
// this allows to set its position. Note that this HAS to be called
// before any attempt to compute the background properties
void BackgroundEstimator::_recompute_if_needed(const PseudoJet &jet){
  // if the range is relocatable, handles its relocation
  if (_rho_range.takes_reference()){
    // check that the reference is not the same as the previous one
    // (would avoid an unnecessary recomputation)
    if (jet == _current_reference) return;

    // relocate the range and make sure things get recomputed the next
    // time one tries to get some information
    _rho_range.set_reference(jet);
    _uptodate=false;
  }

  _recompute_if_needed();
}

// reset to default values
// 
// set the variou options to their default values
void BackgroundEstimator::reset(){
  // set the remaining default parameters
  set_use_area_4vector();  // true by default
  set_provide_fj2_sigma(false);

  // reset the computed values
  _rho = _sigma = 0.0;
  _n_jets_used = _n_empty_jets = 0;
  _empty_area = _mean_area = 0.0;

  _jet_density_class = 0; // null pointer
  _rescaling_class = 0;   // null pointer

  _uptodate = false;
}


// do the actual job
void BackgroundEstimator::_compute() const {
  // check if the clustersequence is still valid
  _check_csa_alive();

  // fill the vector of pt/area (or the quantity from the jet density class) 
  //  - in the range
  vector<double> vector_for_median;
  double total_area  = 0.0;
  _n_jets_used = 0;

  // apply the selector to the included jets
  vector<PseudoJet> selected_jets = _rho_range(_included_jets);

  // compute the pt/area for the selected jets
  for (unsigned i = 0; i < selected_jets.size(); i++) {
    const PseudoJet & current_jet = selected_jets[i];

    double this_area = (_use_area_4vector) ? current_jet.area_4vector().perp() : current_jet.area(); 

    if (this_area>0){
      double median_input;
      if (_jet_density_class == 0) {
	median_input = current_jet.perp()/this_area;
      } else {
	median_input = (*_jet_density_class)(current_jet);
      }
      if (_rescaling_class != 0) {
	median_input /= (*_rescaling_class)(current_jet);
      }
      vector_for_median.push_back(median_input);
      total_area  += this_area;
      _n_jets_used++;
    } else {
      _warnings_zero_area.warn("BackgroundEstimator::_compute(...): discarded jet with zero area. Zero-area jets may be due to (i) too large a ghost area (ii) a jet being outside the ghost range (iii) the computation not being done using an appropriate algorithm (kt;C/A).");
    }
      
  }
  
  // there is nothing inside our region, so answer will always be zero
  if (vector_for_median.size() == 0) {
    _rho        = 0.0;
    _sigma      = 0.0;
    _mean_area  = 0.0;
    return;
  }

  // determine the number of empty jets
  const ClusterSequenceAreaBase * csab = (dynamic_cast<ClusterSequenceStructure*>(_csi()))->validated_csab();
  if (csab->has_explicit_ghosts()) {
    _empty_area = 0.0;
    _n_empty_jets = 0;
  } else {
    _empty_area = csab->empty_area(_rho_range);
    _n_empty_jets = csab->n_empty_jets(_rho_range);
  }

  double total_njets = _n_jets_used + _n_empty_jets;
  total_area  += _empty_area;

  double stand_dev;
  _median_and_stddev(vector_for_median, _n_empty_jets, _rho, stand_dev, 
		     _provide_fj2_sigma);

  // process and store the results (_rho was already stored above)
  _mean_area  = total_area / total_njets;
  _sigma      = stand_dev * sqrt(_mean_area);

  // record that the computation has been performed  
  _uptodate = true;
}

//----------------------------------------------------------------------
void BackgroundEstimator::_median_and_stddev(const vector<double> & quantity_vector, 
					     double n_empty_jets, 
					     double & median, 
					     double & stand_dev_if_gaussian,
					     bool do_fj2_calculation) const {

  // this check is redundant (the code below behaves sensibly even
  // with a zero size), but serves as a reminder of what happens if
  // the quantity vector is zero-sized
  if (quantity_vector.size() == 0) {
    median = 0;
    stand_dev_if_gaussian = 0;
    return;
  }

  vector<double> sorted_quantity_vector = quantity_vector;
  sort(sorted_quantity_vector.begin(), sorted_quantity_vector.end());

  // now get the median & error, accounting for empty jets
  // define the fractions of distribution at median, median-1sigma
  double posn[2] = {0.5, (1.0-0.6827)/2.0};
  double res[2];

  int n_jets_used = sorted_quantity_vector.size();
  double total_njets = n_jets_used + _n_empty_jets;

  for (int i = 0; i < 2; i++) {
    double nj_median_pos;
    if (do_fj2_calculation) {
      nj_median_pos = (total_njets-1)*posn[i] - n_empty_jets;
    } else {
      nj_median_pos = (total_njets)*posn[i] - n_empty_jets - 0.5;
    }

    double nj_median_ratio;
    if (nj_median_pos >= 0 && sorted_quantity_vector.size() > 1) {
      int int_nj_median = int(nj_median_pos);
      nj_median_ratio =
	sorted_quantity_vector[int_nj_median] * (int_nj_median+1-nj_median_pos)
	+ sorted_quantity_vector[int_nj_median+1] * (nj_median_pos - int_nj_median);
    } else if (nj_median_pos > -0.5 && sorted_quantity_vector.size() >= 1 && !do_fj2_calculation) {
      // in the LHS of this "bin", just keep a constant value (we could have
      // interpolated to zero, but this might misbehave in cases where all jets
      // are active, because it would go to zero too fast)
      nj_median_ratio = sorted_quantity_vector[0];
    } else {
      nj_median_ratio = 0.0;
    }
    res[i] = nj_median_ratio;
  }
  
  median = res[0];
  stand_dev_if_gaussian = res[0] - res[1];
}


// check that the underlying structure is still alive;
// throw an error otherwise
void BackgroundEstimator::_check_csa_alive() const{
  ClusterSequenceStructure* csa = dynamic_cast<ClusterSequenceStructure*>(_csi());
  if (csa == 0) {
    throw Error("BackgroundEstimator: there is no cluster sequence associated with the BackgroundEstimator");
  }
  if (! dynamic_cast<ClusterSequenceStructure*>(_csi())->has_associated_cluster_sequence())
    throw Error("BackgroundEstimator: modifications are no longer possible as the underlying ClusterSequence has gone out of scope");
}


// check that the algorithm used for the clustering is suitable for
// background estimation (i.e. either kt or C/A).
// Issue a warning otherwise
void BackgroundEstimator::_check_jet_alg_good_for_median() const{
  const ClusterSequence * cs = dynamic_cast<ClusterSequenceStructure*>(_csi())->validated_cs();

  if (cs->jet_def().jet_algorithm() != kt_algorithm
      && cs->jet_def().jet_algorithm() != cambridge_algorithm
      && cs->jet_def().jet_algorithm() != cambridge_for_passive_algorithm) {
    _warnings.warn("BackgroundEstimator: jet_def being used may not be suitable for estimating diffuse backgrounds (good alternatives are kt, cam)");
  }
}




FASTJET_END_NAMESPACE



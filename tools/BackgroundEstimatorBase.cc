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


#include "fastjet/tools/BackgroundEstimatorBase.hh"

using namespace std;

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

LimitedWarning BackgroundEstimatorBase::_warnings_empty_area;

//----------------------------------------------------------------------
// given a quantity in a vector (e.g. pt_over_area) and knowledge
// about the number of empty jets, calculate the median and
// stand_dev_if_gaussian (roughly from the 16th percentile)
//
// If do_fj2_calculation is set to true then this performs FastJet
// 2.X estimation of the standard deviation, which has a spurious
// offset in the limit of a small number of jets.
void BackgroundEstimatorBase::_median_and_stddev(const vector<double> & quantity_vector, 
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
  double total_njets = n_jets_used + n_empty_jets;

  if (n_empty_jets < -n_jets_used/4.0)
    _warnings_empty_area.warn("BackgroundEstimatorBase::_median_and_stddev(...): the estimated empty area is suspiciously large and may lead to an over-estimation of rho. This may be due to (i) a rare statistical fluctuation or (ii) too small a range used to estimate the background properties.");

  for (int i = 0; i < 2; i++) {
    res[i] = _percentile(sorted_quantity_vector,
			 (do_fj2_calculation) ? ((total_njets-1)*posn[i]+0.5)/total_njets
                                              : posn[i],
                         n_empty_jets);
  }
  
  median = res[0];
  stand_dev_if_gaussian = res[0] - res[1];
}


//----------------------------------------------------------------------
// computes a percentile of a given _sorted_ vector
//  - sorted_quantity_vector   the vector contains the data sample
//  - perc                     the percentile to compute
//  - nempty                   an additional number of 0's
//                             (considered at the beginning of 
//                             the quantity vector)
double BackgroundEstimatorBase::_percentile(const vector<double> &sorted_quantity_vector, const double perc, const unsigned int nempty) const {
  assert(perc >= 0.0 && perc <= 1.0);
  double percentile_position = sorted_quantity_vector.size()*perc - nempty - 0.5;

  if (percentile_position >= 0 && sorted_quantity_vector.size() > 1) {
    int int_percentile_pos = int(percentile_position);
    double result =
	sorted_quantity_vector[int_percentile_pos] * (int_percentile_pos+1-percentile_position)
	+ sorted_quantity_vector[int_percentile_pos+1] * (percentile_position - int_percentile_pos);
    return result;

  } else if (percentile_position > -0.5 && sorted_quantity_vector.size() >= 1) {
    return sorted_quantity_vector[0];

  } else {
    return 0.0;
  }

}


FASTJET_END_NAMESPACE        // defined in fastjet/internal/base.hh

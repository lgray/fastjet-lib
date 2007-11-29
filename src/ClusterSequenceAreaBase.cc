
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




#include "fastjet/ClusterSequenceAreaBase.hh"

FASTJET_BEGIN_NAMESPACE

using namespace std;


/// allow for warnings
LimitedWarning ClusterSequenceAreaBase::_warnings;

//----------------------------------------------------------------------
/// return the total area, within range, that is free of jets.
/// 
/// Calculate this as (range area) - \sum_{i in range} A_i
///
double ClusterSequenceAreaBase::empty_area(const RangeDefinition & range) const {
  double empty = range.area();
  vector<PseudoJet> incl_jets(inclusive_jets(0.0));
  for (unsigned i = 0; i < incl_jets.size(); i++) {
    if (range.is_in_range(incl_jets[i])) empty -= area(incl_jets[i]);
  }
  return empty;
}

double ClusterSequenceAreaBase::median_pt_per_unit_area(const RangeDefinition & range) const {
  return median_pt_per_unit_something(range,false);
}

double ClusterSequenceAreaBase::median_pt_per_unit_area_4vector(const RangeDefinition & range) const {
  return median_pt_per_unit_something(range,true);
}


//----------------------------------------------------------------------
/// the median of (pt/area) for jets contained within range, counting
/// the empty area as if it were made up of a collection of empty
/// jets each of area (0.55 * pi R^2).
double ClusterSequenceAreaBase::median_pt_per_unit_something(
                const RangeDefinition & range, bool use_area_4vector) const {

  double median, sigma, mean_area;
  get_median_rho_and_sigma(range, use_area_4vector, median, sigma, mean_area);
  return median;

}


//----------------------------------------------------------------------
/// fits a form pt_per_unit_area(y) = a + b*y^2 for jets in range. 
/// exclude_above allows one to exclude large values of pt/area from fit. 
/// use_area_4vector = true uses the 4vector areas.
void ClusterSequenceAreaBase::parabolic_pt_per_unit_area(
       double & a, double & b, const RangeDefinition & range, 
       double exclude_above, bool use_area_4vector) const {
  
  int n=0;
  int n_excluded = 0;
  double mean_f=0, mean_x2=0, mean_x4=0, mean_fx2=0; 

  vector<PseudoJet> incl_jets = inclusive_jets();

  for (unsigned i = 0; i < incl_jets.size(); i++) {
    if (range.is_in_range(incl_jets[i])) {
      double this_area;
      if ( use_area_4vector ) {
          this_area = area_4vector(incl_jets[i]).perp();     
      } else {
          this_area = area(incl_jets[i]);
      }
      double f = incl_jets[i].perp()/this_area;
      if (exclude_above <= 0.0 || f < exclude_above) {
	double x = incl_jets[i].rap(); double x2 = x*x;
	mean_f   += f;
	mean_x2  += x2;
	mean_x4  += x2*x2;
	mean_fx2 += f*x2;
	n++;
      } else {
	n_excluded++;
      }
    }
  }

  if (n <= 1) {
    // meaningful results require at least two jets inside the
    // area -- mind you if there are empty jets we should be in 
    // any case doing something special...
    a = 0.0;
    b = 0.0;
  } else {
    mean_f   /= n;
    mean_x2  /= n;
    mean_x4  /= n;
    mean_fx2 /= n;
    
    b = (mean_f*mean_x2 - mean_fx2)/(mean_x2*mean_x2 - mean_x4);
    a = mean_f - b*mean_x2;
  }
  //cerr << "n_excluded = "<< n_excluded << endl;
}




void ClusterSequenceAreaBase::get_median_rho_and_sigma(
            const RangeDefinition & range, bool use_area_4vector,
            double & median, double & sigma, double & mean_area) const {

  _check_jet_alg_good_for_median();

  vector<double> pt_over_areas;
  vector<PseudoJet> incl_jets = inclusive_jets();
  double total_area  = 0.0;
  double total_njets = 0;

  for (unsigned i = 0; i < incl_jets.size(); i++) {
    if (range.is_in_range(incl_jets[i])) {
      double this_area;
      if (use_area_4vector) {
          this_area = area_4vector(incl_jets[i]).perp();
      } else {
          this_area = area(incl_jets[i]);
      }
      pt_over_areas.push_back(incl_jets[i].perp()/this_area);
      total_area  += this_area;
      total_njets += 1.0;
    }
  }

  // there is nothing inside our region, so answer will always be zero
  if (pt_over_areas.size() == 0) {
    median = 0.0;
    sigma  = 0.0;
    mean_area = 0.0;
    return;
  }
  
  // get median (pt/area) [this is the "old" median definition. It considers
  // only the "real" jets in calculating the median, i.e. excluding the
  // only-ghost ones]
  sort(pt_over_areas.begin(), pt_over_areas.end());

  // now get the median & error, accounting for empty jets
  // define the fractions of distribution at median, median-1sigma
  double posn[2] = {0.5, (1.0-0.6827)/2.0};
  double res[2];
  
  double n_empty = n_empty_jets(range);
  total_njets += n_empty;
  total_area  += empty_area(range);

  for (int i = 0; i < 2; i++) {
    double nj_median_pos = 
      (pt_over_areas.size()-1 + n_empty)*posn[i] - n_empty;
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
  median = res[0];
  double error  = res[0] - res[1];
  mean_area = total_area / total_njets;
  sigma  = error * sqrt(mean_area);
}


/// return a vector of all subtracted jets, using area_4vector, given rho.
/// Only inclusive_jets above ptmin are subtracted and returned.
/// the ordering is the same as that of sorted_by_pt(cs.inclusive_jets()),
/// i.e. not necessarily ordered in pt once subtracted
vector<PseudoJet> ClusterSequenceAreaBase::subtracted_jets(const double rho,
                                                           const double ptmin) 
                                                           const {
  vector<PseudoJet> sub_jets;
  vector<PseudoJet> jets = sorted_by_pt(inclusive_jets(ptmin));
  for (unsigned i=0; i<jets.size(); i++) {
     PseudoJet sub_jet = subtracted_jet(jets[i],rho);
     sub_jets.push_back(sub_jet);
  }
  return sub_jets;
}

/// return a vector of subtracted jets, using area_4vector.
/// Only inclusive_jets above ptmin are subtracted and returned.
/// the ordering is the same as that of sorted_by_pt(cs.inclusive_jets()),
/// i.e. not necessarily ordered in pt once subtracted
vector<PseudoJet> ClusterSequenceAreaBase::subtracted_jets(
                                                 const RangeDefinition & range, 
						 const double ptmin)
						 const {
  double rho = median_pt_per_unit_area_4vector(range);
  return subtracted_jets(rho,ptmin);
}


/// return a subtracted jet, using area_4vector, given rho
PseudoJet ClusterSequenceAreaBase::subtracted_jet(const PseudoJet & jet,
                                                  const double rho) const {
  PseudoJet area4vect = area_4vector(jet);
  PseudoJet sub_jet;
  // sanity check
  if (rho*area4vect.perp() < jet.perp() ) { 
    sub_jet = jet - rho*area4vect;
  } else { sub_jet = PseudoJet(0.0,0.0,0.0,0.0); }
  return sub_jet;
}


/// return a subtracted jet, using area_4vector;  note that this is
/// potentially inefficient if repeatedly used for many different
/// jets, because rho will be recalculated each time around.
PseudoJet ClusterSequenceAreaBase::subtracted_jet(const PseudoJet & jet,
                                       const RangeDefinition & range) const {
  double rho = median_pt_per_unit_area_4vector(range);
  PseudoJet sub_jet = subtracted_jet(jet, rho);
  return sub_jet;
}


/// return the subtracted pt, given rho
double ClusterSequenceAreaBase::subtracted_pt(const PseudoJet & jet,
                                              const double rho,
                                              bool use_area_4vector) const {
  if ( use_area_4vector ) { 
     PseudoJet sub_jet = subtracted_jet(jet,rho);
     return sub_jet.perp();
  } else {
     return jet.perp() - rho*area(jet);
  }
}  


/// return the subtracted pt; note that this is
/// potentially inefficient if repeatedly used for many different
/// jets, because rho will be recalculated each time around.
double ClusterSequenceAreaBase::subtracted_pt(const PseudoJet & jet,
                                              const RangeDefinition & range,
                                              bool use_area_4vector) const {
  if ( use_area_4vector ) { 
     PseudoJet sub_jet = subtracted_jet(jet,range);
     return sub_jet.perp();
  } else {
     double rho = median_pt_per_unit_area(range);
     return subtracted_pt(jet,rho,false);
  }
}  


/// check the jet algorithm is suitable (and if not issue a warning)
void ClusterSequenceAreaBase::_check_jet_alg_good_for_median() const {
  if (jet_def().jet_algorithm() != kt_algorithm
      && jet_def().jet_algorithm() != cambridge_algorithm
      && jet_def().jet_algorithm() !=  cambridge_for_passive_algorithm) {
    _warnings.warn("ClusterSequenceAreaBase: jet_def being used may not be suitable for estimating diffuse backgrounds (good options are kt, cam)");
  }
}



FASTJET_END_NAMESPACE

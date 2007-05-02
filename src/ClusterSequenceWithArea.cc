#include "fastjet/ClusterSequenceWithArea.hh"

FASTJET_BEGIN_NAMESPACE

using namespace std;

//----------------------------------------------------------------------
/// return the total area, up to |y|<maxrap, that is free of jets.
/// 
/// Calculate this as 2pi*2*maxrap - \sum_{|y_i|<maxrap} A_i
///
double ClusterSequenceWithArea::empty_area(double maxrap) const {
  double empty = twopi * 2*maxrap;
  vector<PseudoJet> incl_jets(inclusive_jets(0.0));
  for (unsigned i = 0; i < incl_jets.size(); i++) {
    if (abs(incl_jets[i].rap()) < maxrap) empty -= area(incl_jets[i]);
  }
  return empty;
}

double ClusterSequenceWithArea::median_pt_per_unit_area(double maxrap) const {
  return median_pt_per_unit_something(maxrap,false);
}

double ClusterSequenceWithArea::median_pt_per_unit_area_4vector(double maxrap) const {
  return median_pt_per_unit_something(maxrap,true);
}


//----------------------------------------------------------------------
/// the median of (pt/area) for jets contained within |y|<maxrap, counting
/// the empty area as if it were made up of a collection of empty
/// jets each of area (0.55 * pi R^2).
double ClusterSequenceWithArea::median_pt_per_unit_something(
                double maxrap, bool use_area_4vector) const {

  vector<double> pt_over_areas;
  vector<PseudoJet> incl_jets = inclusive_jets();
  for (unsigned i = 0; i < incl_jets.size(); i++) {
    if (abs(incl_jets[i].rap()) < maxrap) {
      double this_area;
      if (use_area_4vector) {
          this_area = area_4vector(incl_jets[i]).perp();
      } else {
          this_area = area(incl_jets[i]);
      }
      pt_over_areas.push_back(incl_jets[i].perp()/this_area);
    }
  }

  // there is nothing inside our region, so answer will always be zero
  if (pt_over_areas.size() == 0) {return 0.0;}
  
  // get median (pt/area) [this is the "old" median definition. It considers
  // only the "real" jets in calculating the median, i.e. excluding the
  // only-ghost ones]
  sort(pt_over_areas.begin(), pt_over_areas.end());

  // now get the median, accounting for empty jets
  double nj_median_pos = (pt_over_areas.size()-1 - n_empty_jets(maxrap))/2.0;
  double nj_median_ratio;
  if (nj_median_pos >= 0 && pt_over_areas.size() > 1) {
    int int_nj_median = int(nj_median_pos);
    nj_median_ratio = 
      pt_over_areas[int_nj_median] * (int_nj_median+1-nj_median_pos)
      + pt_over_areas[int_nj_median+1] * (nj_median_pos - int_nj_median);
  } else {
    nj_median_ratio = 0.0;
  }

  return nj_median_ratio;
}


FASTJET_END_NAMESPACE

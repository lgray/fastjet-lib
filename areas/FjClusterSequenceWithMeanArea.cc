#include "FjPseudoJet.hh"
#include "FjClusterSequence.hh"
#include "FjClusterSequenceWithMeanArea.hh"
#include "FjClusterSequenceWithArea.hh"
#include<iostream>
#include<vector>


using namespace std;




//----------------------------------------------------------------------
double FjClusterSequenceWithMeanArea::pt_per_unit_area(
		       mean_pt_strategies strat, double range) const {
  
  vector<FjPseudoJet> incl_jets = inclusive_jets();
  vector<double> pt_over_areas;

  for (unsigned i = 0; i < incl_jets.size(); i++) {
    if (abs(incl_jets[i].rap()) < _etalim_for_area) {
      double this_area = area(incl_jets[i]);
      pt_over_areas.push_back(incl_jets[i].perp()/this_area);
    }
  }
  
  // get median (pt/area) [this is the "old" median definition]
  sort(pt_over_areas.begin(), pt_over_areas.end());
  double old_median_ratio = pt_over_areas[pt_over_areas.size()/2];

  // new median definition that takes into account non-jet area, 
  // and for fractional median position interpolates between the
  // corresponding entries in the pt_over_areas array
  double nj_median_pos = (pt_over_areas.size()-1 - _non_jet_number)/2.0;
  double nj_median_ratio;
  if (nj_median_pos >= 0 && pt_over_areas.size() > 1) {
    int int_nj_median = int(nj_median_pos);
    nj_median_ratio = 
      pt_over_areas[int_nj_median] * (int_nj_median+1-nj_median_pos)
      + pt_over_areas[int_nj_median+1] * (nj_median_pos - int_nj_median);
  } else {
    nj_median_ratio = 0.0;
  }


  // get various forms of mean (pt/area)
  double pt_sum = 0.0, pt_sum_with_cut = 0.0;
  double area_sum = _non_jet_area, area_sum_with_cut = _non_jet_area;
  double ratio_sum = 0.0; 
  double ratio_n = _non_jet_number;
  for (unsigned i = 0; i < incl_jets.size(); i++) {
    if (abs(incl_jets[i].rap()) < _etalim_for_area) {
      double this_area = area(incl_jets[i]);
      pt_sum   += incl_jets[i].perp();
      area_sum += this_area;
      double ratio = incl_jets[i].perp()/this_area;
      if (ratio < range*nj_median_ratio) {
	pt_sum_with_cut   += incl_jets[i].perp();
	area_sum_with_cut += this_area;
	ratio_sum += ratio; ratio_n++;
      }
    }
  }
  
  if (strat == play) {
    double trunc_sum = 0, trunc_sumsqr = 0;
    vector<double> means(pt_over_areas.size()), sd(pt_over_areas.size());
    for (unsigned i = 0; i < pt_over_areas.size() ; i++ ) {
      double ratio = pt_over_areas[i];
      trunc_sum += ratio;
      trunc_sumsqr += ratio*ratio;
      means[i] = trunc_sum / (i+1);
      sd[i]    = sqrt(abs(means[i]*means[i]  - trunc_sumsqr/(i+1)));
      cerr << "i, means, sd: " <<i<<", "<< means[i] <<", "<<sd[i]<<", "<<
	sd[i]/sqrt(i+1.0)<<endl;
    }
    cout << "-----------------------------------"<<endl;
    for (unsigned i = 0; i <= pt_over_areas.size()/2 ; i++ ) {
      cout << "Median "<< i <<" = " << pt_over_areas[i]<<endl;
    }
    cout << "Number of non-jets: "<<_non_jet_number<<endl;
    cout << "Area of non-jets: "<<_non_jet_area<<endl;
    cout << "Default median position: " << (pt_over_areas.size()-1)/2.0<<endl;
    cout << "NJ median position: " << nj_median_pos <<endl;
    cout << "NJ median value: " << nj_median_ratio <<endl;
    return 0.0;
  }

  switch(strat) {
  case median:
    return nj_median_ratio;
  case old_median:
    return old_median_ratio; 
  case pttot_over_areatot:
    return pt_sum / area_sum;
  case pttot_over_areatot_cut:
    return pt_sum_with_cut / area_sum_with_cut;
  case mean_ratio_cut:
    return ratio_sum/ratio_n;
  default:
    return nj_median_ratio;
  }

}

#include "FjPseudoJet.hh"
#include "FjClusterSequence.hh"
#include "FjClusterSequenceWithMeanArea.hh"
#include "FjClusterSequenceWithArea.hh"
#include<iostream>
#include<vector>


using namespace std;


int FjClusterSequenceWithMeanArea::_n_seed_warnings = 0;
const int _max_seed_warnings = 10;

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


//----------------------------------------------------------------------
// fit a parabola to pt/area as a function of rapidity, using the
// formulae of CCN28-36 (which actually fits f = a+b*x^2)
void FjClusterSequenceWithMeanArea::parabolic_pt_per_unit_area(
                     double & a, double & b, double raprange) {
  
  double this_raprange;
  if (raprange <= 0) {this_raprange = _etalim_for_area;}
  else {this_raprange = raprange;}

  int n=0;
  double mean_f=0, mean_x2=0, mean_x4=0, mean_fx2=0; 


  vector<FjPseudoJet> incl_jets = inclusive_jets();

  for (unsigned i = 0; i < incl_jets.size(); i++) {
    if (abs(incl_jets[i].rap()) < this_raprange) {
      double this_area = area(incl_jets[i]);
      double f = incl_jets[i].perp()/this_area;
      double x = incl_jets[i].rap(); double x2 = x*x;
      mean_f   += f;
      mean_x2  += x2;
      mean_x4  += x2*x2;
      mean_fx2 += f*x2;
      n++;
    }
  }

  mean_f   /= n;
  mean_x2  /= n;
  mean_x4  /= n;
  mean_fx2 /= n;

  b = (mean_f*mean_x2 - mean_fx2)/(mean_x2*mean_x2 - mean_x4);
  a = mean_f - b*mean_x2;
}


//----------------------------------------------------------------------
void FjClusterSequenceWithMeanArea::_transfer_areas(
	    const vector<int> & unique_hist_order,
    	    const FjClusterSequenceWithArea & clust_seq  ) {

  const vector<history_element> & cs_history  = clust_seq.history();
  const vector<FjPseudoJet>     & cs_jets     = clust_seq.jets();
  vector<int>    cs_unique_hist_order = clust_seq.unique_history_order();

  const double tolerance = 1e-13; // to decide when two jets are the same

  int j = -1;
  int hist_index = -1;
  
  valarray<double> our_areas(_history.size());
  our_areas = 0.0;

  valarray<FjPseudoJet> our_ext_areas(_history.size());
  our_ext_areas = FjPseudoJet(0.0,0.0,0.0,0.0);

  for (unsigned i = 0; i < cs_history.size(); i++) {
    // only consider composite particles
    unsigned cs_hist_index = cs_unique_hist_order[i];
    if (cs_hist_index < clust_seq.n_particles()) continue;
    const history_element & cs_hist = cs_history[cs_unique_hist_order[i]];
    int parent1 = cs_hist.parent1;
    int parent2 = cs_hist.parent2;

    if (parent2 == BeamJet) {
      // need to look at parent to get the actual jet
      const FjPseudoJet & jet = 
  	  cs_jets[cs_history[parent1].jetp_index];
      double area = clust_seq.area(jet);
      FjPseudoJet ext_area = clust_seq.extended_area(jet);

      if (clust_seq.is_pure_ghost(parent1)) {
	if (abs(jet.rap()) < _etalim_for_area) {
	  _non_jet_area  += area;
	  _non_jet_area2 += area*area;
	  _non_jet_number += 1;
	}
      } else {

	// get next "combined-particle" index in our own history
	// making sure we don't go beyond it's bounds (if we do
	// then we're in big trouble anyway...)
	while (++j < static_cast<int>(_history.size())) {
	  hist_index = unique_hist_order[j];
	  if (hist_index >= _initial_n) break;}

	// sanity check 
	const FjPseudoJet & refjet = 
	  _jets[_history[_history[hist_index].parent1].jetp_index];
	//if (jet.perp2() != refjet.perp2()) {
	if (abs(jet.perp2()-refjet.perp2()) > 
	            tolerance*max(jet.perp2(),refjet.perp2())) {
	  cerr << jet.perp() << " " << refjet.perp() << " "<< jet.perp() - refjet.perp() << endl;
	  throw FjError("Could not match clustering sequence for an inclusive jet when reconstructing areas"); }

	// set the area at this clustering stage
	our_areas[hist_index]  = area; 
	our_ext_areas[hist_index]  = ext_area; 

	// update the parent as well -- that way its area is the area
	// immediately before clustering (i.e. resolve an ambiguity in
	// the Cambridge case and ensure in the kt case that the original
	// particles get a correct area)
	our_areas[_history[hist_index].parent1] = area;
	our_ext_areas[_history[hist_index].parent1] = ext_area;
	
      }
    }
    else if (!clust_seq.is_pure_ghost(parent1) && 
	     !clust_seq.is_pure_ghost(parent2)) {

      // get next "combined-particle" index in our own history
      while (++j < static_cast<int>(_history.size())) {
	hist_index = unique_hist_order[j];
	if (hist_index >= _initial_n) break;}
      
      const FjPseudoJet & jet = cs_jets[cs_hist.jetp_index];
      const FjPseudoJet & refjet = _jets[_history[hist_index].jetp_index];

      // run sanity check 
      if (abs(jet.perp2()-refjet.perp2()) > 
	  tolerance*max(jet.perp2(),refjet.perp2())) {
	  cerr << jet.perp() << " " << refjet.perp() << " "<< jet.perp() - refjet.perp() << endl;
	  throw FjError("Could not match clustering sequence for an exclusive jet when reconstructing areas"); }

      // update area and our local index (maybe redundant since later
      // the descendants will reupdate it?)
      double area  = clust_seq.area(jet);
      our_areas[hist_index]  += area; 

      FjPseudoJet ext_area = clust_seq.extended_area(jet);
      our_ext_areas[hist_index] = our_ext_areas[hist_index] + ext_area; 

      // now update areas of parents (so that they becomes areas
      // immediately before clustering occurred). This is of use
      // because it allows us to set the areas of the original hard
      // particles in the kt algorithm; for the Cambridge case it
      // means a jet's area will be the area just before it clusters
      // with another hard jet.
      const FjPseudoJet & jet1 = cs_jets[cs_history[parent1].jetp_index];
      int our_parent1 = _history[hist_index].parent1;
      our_areas[our_parent1] = clust_seq.area(jet1);
      our_ext_areas[our_parent1] = clust_seq.extended_area(jet1);

      const FjPseudoJet & jet2 = cs_jets[cs_history[parent2].jetp_index];
      int our_parent2 = _history[hist_index].parent2;
      our_areas[our_parent2] = clust_seq.area(jet2);
      our_ext_areas[our_parent2] = clust_seq.extended_area(jet2);
    }

  }

  _average_area  += our_areas; 
  _average_area2 += our_areas*our_areas; 

  // poverty of operators forces us to write things this way...
  _average_ext_area = _average_ext_area + our_ext_areas;
  
}



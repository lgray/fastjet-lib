#include "FjPseudoJet.hh"
#include "FjClusterSequence.hh"
#include<iostream>
#include<vector>

#include "FjClusterSequenceWithArea.hh"

using namespace std;

/// Class that behaves essentially like FjClusterSequence except
/// that it also provides access to the area of a jet (which
/// will be a random quantity... Figure out what to do about seeds 
/// later...)
class FjClusterSequenceWithMeanArea : public FjClusterSequence {
public:
  template<class L> FjClusterSequenceWithMeanArea
         (const std::vector<L> & pseudojets, 
	  double cell_area, double etamax_for_area,
	  double grid_scatter = 0.00001, double kt_scatter = 0.1,
	  int    area_nrepeat = 5,
	  const double & R = 1.0,
	  const FjStrategy & strategy = Best,
	  const bool & writeout_combinations = false);


private:

  valarray<double> _average_area, _average_area2;
  valarray<int>    _incl_ix_of_clust_ix;
  double           _non_jet_area, _non_jet_area2;

  int _incl_ix_of_jet(const FjPseudoJet & jet) const {
    int ix = _incl_ix_of_clust_ix[jet.cluster_hist_index()];
    if (ix == Invalid) {throw "Asked for area of non-inclusive jet";}
    return ix;
  }

  double _etamax_for_area; // max eta where we put ghosts
  double _etalim_for_area; // max eta where we trust jet areas


public : 
  double area (const FjPseudoJet & jet) const {
                             return _average_area[_incl_ix_of_jet(jet)];};
  double area_err (const FjPseudoJet & jet) const {
                             return _average_area2[_incl_ix_of_jet(jet)];};

  /// return the transverse momentum per unit area excluding 
  /// jets that have pt/area > median(pt/area)*range.
  /// NB: this will be wrong for events that are not "dense" because
  ///     of a large number of jets that will have zero pt.
  enum mean_pt_strategies{median=0, pttot_over_areatot, pttot_over_areatot_cut, mean_ratio_cut};

  double pt_per_unit_area(mean_pt_strategies strat=median, double range=2.0 ) const;
};


/// horrible, really horrible initialiser that throws away all
/// information on non-inclusive jets (there's bound to be a better
/// way...)
template<class L> 
   FjClusterSequenceWithMeanArea::FjClusterSequenceWithMeanArea (
	        const std::vector<L> & pseudojets,
              	double cell_area, double etamax_for_area,
		double grid_scatter, double kt_scatter, int area_nrepeat,
		const double & R,
		const FjStrategy & strategy,
		const bool & writeout_combinations) :
     FjClusterSequence(pseudojets, R, strategy, writeout_combinations) 
{
  
  // for future reference...
  _etamax_for_area = etamax_for_area;
  _etalim_for_area = _etamax_for_area - _Rparam;
  
  // arrange to have a mapping between the cluster_hist_index and the
  // index of the inclusive jets...
  _incl_ix_of_clust_ix.resize(_history.size());
  _incl_ix_of_clust_ix = Invalid;
  vector<FjPseudoJet> incl_jets = inclusive_jets();
  for (unsigned int i=0; i < incl_jets.size(); i++) {
    _incl_ix_of_clust_ix[incl_jets[i].cluster_hist_index()] = i;
  }
  
  // initialize our local area information
  _average_area.resize(incl_jets.size());  _average_area  = 0.0;
  _average_area2.resize(incl_jets.size()); _average_area2 = 0.0;
  _non_jet_area = 0.0; _non_jet_area2 = 0.0;
     
  // run the clustering multiple times so as to get areas of all the
  // inclusive jets (one day this should be changed so as to get
  // area of ALL jets.
  cerr << "***** * WATCH OUT ******; I am resetting the random seed\n";
  for (int irepeat = 0; irepeat < area_nrepeat; irepeat++) {
    // WARNING: setting seed manually at each turn of loop (because
    // we suspect that CGAL plays with it)
    srand(irepeat+2);

    FjClusterSequenceWithArea clust_seq(pseudojets,cell_area,etamax_for_area,
					grid_scatter, kt_scatter,
					R,strategy);
       
    vector<FjPseudoJet> incl_jets4area = clust_seq.inclusive_jets();
    for (unsigned int i=0; i < incl_jets4area.size(); i++) {
      double area = clust_seq.area(incl_jets4area[i]);
      if (i < incl_jets.size()) {
	_average_area[i]  += area;
	_average_area2[i] += area*area;
      } else if (abs(incl_jets4area[i].rap()) < _etalim_for_area) {
	_non_jet_area  += area;
	_non_jet_area2 += area*area;
      }
    }
    //cerr << "non-jet area sum was " << _non_jet_area << endl;
  }
  
  _average_area  /= area_nrepeat;
  _average_area2 /= area_nrepeat;
  _average_area2 = sqrt(abs(_average_area2 - _average_area*_average_area)/
			 area_nrepeat);

  _non_jet_area  /= area_nrepeat;
  _non_jet_area2 /= area_nrepeat;
  _non_jet_area2  = sqrt(abs(_non_jet_area2 - _non_jet_area*_non_jet_area)/
			 area_nrepeat);

  cerr << "Non-jet area = " << _non_jet_area << " +- " << _non_jet_area2<<endl;

}




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
  
  // get median (pt/area)
  sort(pt_over_areas.begin(), pt_over_areas.end());
  double median_ratio = pt_over_areas[pt_over_areas.size()/2];

  // get various forms of mean (pt/area)
  double pt_sum = 0.0, pt_sum_with_cut = 0.0;
  double area_sum = 0.0, area_sum_with_cut = 0.0;
  double ratio_sum = 0.0; 
  int ratio_n = 0;
  for (unsigned i = 0; i < incl_jets.size(); i++) {
    if (abs(incl_jets[i].rap()) < _etalim_for_area) {
      double this_area = area(incl_jets[i]);
      pt_sum   += incl_jets[i].perp();
      area_sum += this_area;
      double ratio = incl_jets[i].perp()/this_area;
      if (ratio < range*median_ratio) {
	pt_sum_with_cut   += incl_jets[i].perp();
	area_sum_with_cut += this_area;
	ratio_sum += ratio; ratio_n++;
      }
    }
  }
  
  switch(strat) {
  case median:
    return median_ratio; 
  case pttot_over_areatot:
    return pt_sum / area_sum;
  case pttot_over_areatot_cut:
    return pt_sum_with_cut / area_sum_with_cut;
  case mean_ratio_cut:
    return ratio_sum/ratio_n;
  default:
    return median_ratio;
  }

}

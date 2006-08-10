#ifndef __FJCLUSTERSEQUENCEWITHMEANAREA__
#define __FJCLUSTERSEQUENCEWITHMEANAREA__


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

  /// constructor based on FjJetDefinition and FjActiveAreaSpec
  template<class L> FjClusterSequenceWithMeanArea
         (const std::vector<L> & pseudojets, 
	  const FjJetDefinition & jet_def,
	  const FjActiveAreaSpec & area_spec,
	  const bool & writeout_combinations = false) :
     FjClusterSequence(pseudojets, jet_def, writeout_combinations) {
	   _initialize(pseudojets, jet_def, area_spec, writeout_combinations);};

  /// legacy constructor
  template<class L> FjClusterSequenceWithMeanArea
         (const std::vector<L> & pseudojets, 
	  double cell_area, double etamax_for_area,
	  double grid_scatter = 0.00001, double kt_scatter = 0.1,
	  int    area_nrepeat = 5,
	  const double & R = 1.0,
	  const FjStrategy & strategy = Best,
	  const bool & writeout_combinations = false);

  double area (const FjPseudoJet & jet) const {
                             return _average_area[jet.cluster_hist_index()];};
  double area_err (const FjPseudoJet & jet) const {
                             return _average_area2[jet.cluster_hist_index()];};

  FjPseudoJet extended_area (const FjPseudoJet & jet) const {
                    return _average_ext_area[jet.cluster_hist_index()];};

  /// return the transverse momentum per unit area excluding 
  /// jets that have pt/area > median(pt/area)*range.
  /// NB: this will be wrong for events that are not "dense" because
  ///     of a large number of jets that will have zero pt.
  enum mean_pt_strategies{median=0, old_median, pttot_over_areatot, 
			  pttot_over_areatot_cut, mean_ratio_cut, play};

  double pt_per_unit_area(mean_pt_strategies strat=median, double range=2.0 ) const;

  /// fits a form pt_per_unit_area(y) = a + b*y^2 in the range
  /// abs(y)<raprange (for negative raprange, it defaults to
  /// _etalim_for_area).
  void parabolic_pt_per_unit_area(double & a,double & b, double raprange=-1.0,
				  double exclude_above=-1.0);



private:

  /// does the actual initialisation work 
  template<class L> void _initialize
         (const std::vector<L> & pseudojets, 
	  const FjJetDefinition & jet_def,
	  const FjActiveAreaSpec & area_spec,
	  const bool & writeout_combinations = false);


  valarray<double> _average_area, _average_area2;
  valarray<FjPseudoJet> _average_ext_area;
  double           _non_jet_area, _non_jet_area2, _non_jet_number;

  double _etamax_for_area; // max eta where we put ghosts
  double _etalim_for_area; // max eta where we trust jet areas

  /// transfer areas from the FjClusterSequenceWithArea object into
  /// our internal area bookkeeping...
  void _transfer_areas(const vector<int> &, const FjClusterSequenceWithArea & );

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
  static int _n_seed_warnings;
  const static int _max_seed_warnings = 10;

};



/// initializer that should create information about areas of all
/// jets; a PseudoJet's area is defined as that just before it gets
/// clustered with something else (or written off as an inclusive jet).
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
  
  FjJetDefinition jet_def(_default_jet_finder, R, strategy);
  FjActiveAreaSpec area_spec(cell_area,etamax_for_area,grid_scatter,kt_scatter,
			     area_nrepeat);
  _initialize(pseudojets, jet_def, area_spec, writeout_combinations) ;
}



template<class L> 
   void FjClusterSequenceWithMeanArea::_initialize (
		const std::vector<L> & pseudojets, 
		const FjJetDefinition & jet_def,
		const FjActiveAreaSpec & area_spec,
		const bool & writeout_combinations) 
  {
  // code for testing the unique tree
  vector<int> unique_tree;
  unique_tree = unique_history_order();

  // for future reference...
  _etamax_for_area = area_spec.ghost_etamax();
  _etalim_for_area = _etamax_for_area - _Rparam;
  
  
  // initialize our local area information
  _average_area.resize(_history.size());  _average_area  = 0.0;
  _average_area2.resize(_history.size()); _average_area2 = 0.0;
  _average_ext_area.resize(_history.size()); 
  _average_ext_area = FjPseudoJet(0.0,0.0,0.0,0.0);
  _non_jet_area = 0.0; _non_jet_area2 = 0.0; _non_jet_number=0.0;
     
  if (_n_seed_warnings < _max_seed_warnings) {
    cerr << "***** * WATCH OUT ******; I am resetting the random seed\n";
    _n_seed_warnings += 1;
    if (_n_seed_warnings == _max_seed_warnings) cerr << "[last time this warning is output]\n";
  }


  // run the clustering multiple times so as to get areas of all the jets
  for (int irepeat = 0; irepeat < area_spec.repeat(); irepeat++) {
    // WARNING: setting seed manually at each turn of loop (because
    // we suspect that CGAL plays with it)
    srand(irepeat+2);

    FjClusterSequenceWithArea clust_seq(pseudojets, jet_def, area_spec);

    // transfer areas from clust_seq into our object
    _transfer_areas(unique_tree, clust_seq);
    //cerr << "non-jet area sum was " << _non_jet_area << endl;
  }
  
  _average_area  /= area_spec.repeat();
  _average_area2 /= area_spec.repeat();
  _average_area2 = sqrt(abs(_average_area2 - _average_area*_average_area)/
			 area_spec.repeat());

  _non_jet_area  /= area_spec.repeat();
  _non_jet_area2 /= area_spec.repeat();
  _non_jet_area2  = sqrt(abs(_non_jet_area2 - _non_jet_area*_non_jet_area)/
			 area_spec.repeat());
  _non_jet_number /= area_spec.repeat();

  // following bizarre way of writing things is related to 
  // poverty of operations on FjPseudoJet objects (as well as some confusion
  // in one or two places)
  for (unsigned i = 0; i < _average_ext_area.size(); i++) {
    _average_ext_area[i] = (1.0/area_spec.repeat()) * _average_ext_area[i];
  }
  //cerr << "Non-jet area = " << _non_jet_area << " +- " << _non_jet_area2<<endl;


}


#endif // __FJCLUSTERSEQUENCEWITHMEANAREA__

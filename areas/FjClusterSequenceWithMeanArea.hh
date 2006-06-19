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


public : 
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
  enum mean_pt_strategies{median=0, old_median, pttot_over_areatot, pttot_over_areatot_cut, mean_ratio_cut, play};

  double pt_per_unit_area(mean_pt_strategies strat=median, double range=2.0 ) const;
};




/// initialiser that should create information about areas of all
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
  
  // code for testing the unique tree
  vector<int> unique_tree;
  unique_tree = unique_history_order();

//  cout << "Printing unique-tree form of history for "<<_initial_n<<" "<<_history.size()<<" particles \n";
//  for (unsigned i = 0; i < unique_tree.size(); i++) {
//    const history_element & hist_element = _history[unique_tree[i]];
//    cout << i <<" "<<unique_tree[i]<<" "<<hist_element.dij<<
//     " "<<hist_element.parent1<<" "<<hist_element.parent2<<
//      endl;
//    //cout << i <<" "<<hist_element.dij<<
//    //  " "<<hist_element.parent1<<" "<<hist_element.parent2<<
//    //  endl;
//  }

  // for future reference...
  _etamax_for_area = etamax_for_area;
  _etalim_for_area = _etamax_for_area - _Rparam;
  
  
  // initialize our local area information
  _average_area.resize(_history.size());  _average_area  = 0.0;
  _average_area2.resize(_history.size()); _average_area2 = 0.0;
  _average_ext_area.resize(_history.size()); 
  _average_ext_area = FjPseudoJet(0.0,0.0,0.0,0.0);
  _non_jet_area = 0.0; _non_jet_area2 = 0.0; _non_jet_number=0.0;
     
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

    // transfer areas from clust_seq into our object
    _transfer_areas(unique_tree, clust_seq);
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
  _non_jet_number /= area_nrepeat;

  // following bizarre way of writing things is related to 
  // poverty of operations on FjPseudoJet objects (as well as some confusion
  // in one or two places)
  for (unsigned i = 0; i < _average_ext_area.size(); i++) {
    _average_ext_area[i] = (1.0/area_nrepeat) * _average_ext_area[i];
  }
  //cerr << "Non-jet area = " << _non_jet_area << " +- " << _non_jet_area2<<endl;


}


#endif // __FJCLUSTERSEQUENCEWITHMEANAREA__

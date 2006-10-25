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


//----------------------------------------------------------------------
// here's where we put the main page for fastjet (as explained in the
// Doxygen faq)
//......................................................................
/*! \mainpage FastJet code documentation
 *
 * These pages provide automatically generated documentation for the 
 * FastJet package.
 * 
 * For further information and normal documentation, see the main <a
 * href="http://www.lpthe.jussieu.fr/~salam/fastjet">FastJet</a> page.
 */
//----------------------------------------------------------------------

#ifndef __FASTJET_CLUSTERSEQUENCE_HH__
#define __FASTJET_CLUSTERSEQUENCE_HH__

#include<vector>
#include<map>
#include "fastjet/internal/DynamicNearestNeighbours.hh"
#include "fastjet/PseudoJet.hh"
#include<cassert>
#include<iostream>
#include<string>
#include<cmath> // needed to get double std::abs(double)
#include "fastjet/Error.hh"
#include "fastjet/JetDefinition.hh"

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh


/// deals with clustering
class ClusterSequence {


 public: 

  /// default constructor
  ClusterSequence () {};

  /// create a clustersequence starting from the supplied set
  /// of pseudojets and clustering them with the long-invariant
  /// kt algorithm (E-scheme recombination) with the supplied
  /// value for R.
  ///
  /// If strategy=DumbN3 a very stupid N^3 algorithm is used for the
  /// clustering; otherwise strategy = NlnN* uses cylinders algorithms
  /// with some number of pi coverage. If writeout_combinations=true a
  /// summary of the recombination sequence is written out
  template<class L> ClusterSequence (const std::vector<L> & pseudojets, 
		   const double & R = 1.0,
		   const Strategy & strategy = Best,
		   const bool & writeout_combinations = false);


  /// create a clustersequence starting from the supplied set
  /// of pseudojets and clustering them with jet definition specified
  /// by jet_def (which also specifies the clustering strategy)
  template<class L> ClusterSequence (
			          const std::vector<L> & pseudojets,
				  const JetDefinition & jet_def,
				  const bool & writeout_combinations = false);


  // NB: in the routines that follow, for extracting lists of jets, a
  //     list structure might be more efficient, if sometimes a little
  //     more awkward to use (at least for old fortran hands).

  /// return a vector of all jets (in the sense of the inclusive
  /// algorithm) with pt >= ptmin. Time taken should be of the order
  /// of the number of jets returned.
  std::vector<PseudoJet> inclusive_jets (const double & ptmin = 0.0) const;

  /// return the number of jets (in the sense of the exclusive
  /// algorithm) that would be obtained when running the algorithm
  /// with the given dcut.
  int n_exclusive_jets (const double & dcut) const;

  /// return a vector of all jets (in the sense of the exclusive
  /// algorithm) that would be obtained when running the algorithm
  /// with the given dcut.
  std::vector<PseudoJet> exclusive_jets (const double & dcut) const;

  /// return a vector of all jets when the event is clustered (in the
  /// exclusive sense) to exactly njets.
  std::vector<PseudoJet> exclusive_jets (const int & njets) const;

  /// return the dmin corresponding to the recombination that went from
  /// n+1 to n jets (sometimes known as d_{n n+1}).
  double exclusive_dmerge (const int & njets) const;

  /// return the maximum of the dmin encountered during all recombinations 
  /// up to the one that led to an n-jet final state; identical to
  /// exclusive_dmerge, except in cases where the dmin do not increase
  /// monotonically.
  double exclusive_dmerge_max (const int & njets) const;

  /// return a vector of the particles that make up jet
  std::vector<PseudoJet> constituents (const PseudoJet & jet) const;
  /// add on to subjet_vector the subjets of jet.
  void add_constituents (const PseudoJet & jet, 
			 std::vector<PseudoJet> & subjet_vector) const;

  /// return the enum value of the strategy used to cluster the event
  inline Strategy strategy_used () const {return _strategy;};
  std::string strategy_string () const;


  /// returns the scale associated with a jet as required for this
  /// clustering algorithm (kt^2 for the kt-algorithm, 1 for the 
  /// Cambridge algorithm). [May become virtual at some point]
  double jet_scale_for_algorithm(const PseudoJet & jet) const;

  //----- next follow functions designed specifically for plugins, which
  //      may only be called when plugin_activated() returns true

  /// record the fact that there has been a recombination between
  /// jets()[jet_i] and jets()[jet_k], with the specified dij, and
  /// return the index (newjet_k) allocated to the new jet, whose
  /// momentum is assumed to be the 4-vector sum of that of jet_i and
  /// jet_j
  void plugin_record_ij_recombination(int jet_i, int jet_j, double dij, 
				      int & newjet_k) {
    assert(plugin_activated());
    _do_ij_recombination_step(jet_i, jet_j, dij, newjet_k);
  };

  /// as for the simpler variant of plugin_record_ij_recombination,
  /// except that the new jet is attributed the momentum and
  /// user_index of newjet
  void plugin_record_ij_recombination(int jet_i, int jet_j, double dij, 
				      const PseudoJet & newjet, 
				      int & newjet_k);

  /// record the fact that there has been a recombination between
  /// jets()[jet_i] and the beam, with the specified diB; when looking
  /// for inclusive jets, any iB recombination will returned to the user 
  /// as a jet.
  void plugin_record_iB_recombination(int jet_i, double diB) {
    assert(plugin_activated());
    _do_iB_recombination_step(jet_i, diB);
  };

  /// returns true when the plugin is allowed to run the show.
  inline bool plugin_activated() const {return _plugin_activated;};

public:
  /// set the default (static) jet finder across all current and future
  /// ClusterSequence objects -- deprecated.
  static void set_jet_finder (JetFinder jet_finder) {_default_jet_finder = jet_finder;};


  /// a single element in the clustering history (see vector _history
  /// below).
  struct history_element{
    int parent1; /// index in _history where first parent of this jet
                 /// was created (InexistentParent if this jet is an
                 /// original particle)

    int parent2; /// index in _history where second parent of this jet
                 /// was created (InexistentParent if this jet is an
                 /// original particle); BeamJet if this history entry
                 /// just labels the fact that the jet has recombined
                 /// with the beam)

    int child;   /// index in _history where the current jet is
		 /// recombined with another jet to form its child. It
		 /// is Invalid if this jet does not further
		 /// recombine.

    int jetp_index; /// index in the _jets vector where we will find the
                 /// PseudoJet object corresponding to this jet
                 /// (i.e. the jet created at this entry of the
                 /// history). NB: if this element of the history
                 /// corresponds to a beam recombination, then
                 /// jetp_index=Invalid.

    double dij;  /// the distance corresponding to the recombination
		 /// at this stage of the clustering.

    double max_dij_so_far; /// the largest recombination distance seen
			   /// so far in the clustering history.
  };

  enum JetType {Invalid=-3, InexistentParent = -2, BeamJet = -1};

  /// allow the user to access the jets in this raw manner (needed
  /// because we don't seem to be able to access protected elements of
  /// the class for an object that is not "this" (at least in case where
  /// "this" is of a slightly different kind from the object, both
  /// derived from ClusterSequence).
  const std::vector<PseudoJet> & jets()    const;

  /// allow the user to access the history in this raw manner (see
  /// above for motivation).
  const std::vector<history_element> & history() const;

  /// returns the number of particles that were provided to the
  /// clustering algorithm (helps the user find their way around the
  /// history and jets objects if they weren't paying attention
  /// beforehand).
  unsigned int n_particles() const;

  /// returns a vector of size n_particles() which indicates, for 
  /// each of the initial particles (in the order in which they were
  /// supplied), which of the supplied jets it belongs to; if it does
  /// not belong to any of the supplied jets, the index is set to -1;
  std::vector<int> particle_jet_indices(const std::vector<PseudoJet> &) const;

  /// routine that returns a an order in which to read the history
  /// such that clusterings that lead to identical jet compositions
  /// but different histories (because of degeneracies in the
  /// clustering order) will have matching constituents for each
  /// matching entry in the unique_history_order.
  ///
  /// The order has the property that an entry's parents will always
  /// appear prior to that entry itself. 
  ///
  /// Roughly speaking the order is such that we first provide all
  /// steps that lead to the final jet containing particle 1; then we
  /// have the steps that lead to reconstruction of the jet containing
  /// the next-lowest-numbered unclustered particle, etc...
  /// [see GPS CCN28-12 for more info -- of course a full explanation
  /// here would be better...]
  std::vector<int> unique_history_order() const;


protected:
  static JetFinder _default_jet_finder;
  JetDefinition _jet_def;

  /// transfer the vector<L> of input jets into our own vector<PseudoJet>
  /// _jets (with some reserved space for future growth).
  template<class L> void _transfer_input_jets(
                                     const std::vector<L> & pseudojets);

  /// This is the routine that will do all the initialisation and
  /// then run the clustering (may be called by various constructors).
  /// It assumes _jets contains the momenta to be clustered.
  void _initialise_and_run (const JetDefinition & jet_def,
			    const bool & writeout_combinations);

  /// This is an alternative routine for initialising and running the
  /// clustering, provided for legacy purposes. The jet finder is that
  /// specified in the static member _default_jet_finder.
  void _initialise_and_run (const double & R,
			    const Strategy & strategy,
			    const bool & writeout_combinations);

  /// fills in the various member variables with "decanted" options from
  /// the jet_definition and writeout_combinations variables
  void _decant_options(const JetDefinition & jet_def,
                       const bool & writeout_combinations);

  /// fill out the history (and jet cross refs) related to the initial
  /// set of jets (assumed already to have been "transferred"),
  /// without any clustering
  void _fill_initial_history();

  /// carry out the recombination between the jets numbered jet_i and
  /// jet_j, at distance scale dij; return the index newjet_k of the
  /// result of the recombination of i and j.
  void _do_ij_recombination_step(const int & jet_i, const int & jet_j, 
				 const double & dij, int & newjet_k);

  /// carry out an recombination step in which _jets[jet_i] merges with
  /// the beam, 
  void _do_iB_recombination_step(const int & jet_i, const double & diB);


  /// This contains the physical PseudoJets; for each PseudoJet one
  /// can find the corresponding position in the _history by looking
  /// at _jets[i].cluster_hist_index().
  std::vector<PseudoJet> _jets;


  /// this vector will contain the branching history; for each stage,
  /// _history[i].jetp_index indicates where to look in the _jets
  /// vector to get the physical PseudoJet.
  std::vector<history_element> _history;

  bool _writeout_combinations;
  int  _initial_n;
  double _Rparam, _R2, _invR2;
  Strategy    _strategy;
  JetFinder   _jet_finder;

 private:

  bool _plugin_activated;

  void _really_dumb_cluster ();
  void _delaunay_cluster ();
  void _simple_N2_cluster ();
  void _tiled_N2_cluster ();
  void _faster_tiled_N2_cluster ();

  //
  void _minheap_faster_tiled_N2_cluster();

  // things needed specifically for Cambridge with Chan's 2D closest
  // pairs method
  void _CP2DChan_cluster();
  void _CP2DChan_cluster_2pi2R ();
  void _CP2DChan_cluster_2piMultD ();
  void _CP2DChan_limited_cluster(double D);
  void _do_Cambridge_inclusive_jets();

  void _add_step_to_history(const int & step_number, const int & parent1, 
			       const int & parent2, const int & jetp_index,
			       const double & dij);

  /// internal routine associated with the construction of the unique
  /// history order (following children in the tree)
  void _extract_tree_children(int pos, std::valarray<bool> &, 
		const std::valarray<int> &, std::vector<int> &) const;

  /// internal routine associated with the construction of the unique
  /// history order (following parents in the tree)
  void _extract_tree_parents (int pos, std::valarray<bool> &, 
                const std::valarray<int> &,  std::vector<int> &) const;


  // these will be useful shorthands in the Voronoi-based code
  typedef std::pair<int,int> TwoVertices;
  typedef std::pair<double,TwoVertices> DijEntry;
  typedef std::multimap<double,TwoVertices> DistMap;

  /// currently used only in the Voronoi based code
  void _add_ktdistance_to_map(const int & ii, 
			      DistMap & DijMap,
  			      const DynamicNearestNeighbours * DNN);

  /// for making sure the user knows what it is they're running...
  void _print_banner();
  /// will be set by default to be true for the first run
  static bool _first_time;


  //----------------------------------------------------------------------
  /// the fundamental structure which contains the minimal info about
  /// a jet, as needed for our plain N^2 algorithm -- the idea is to
  /// put all info that will be accessed N^2 times into an array of
  /// BriefJets...
  struct BriefJet {
    double     eta, phi, kt2, NN_dist;
    BriefJet * NN;
    int        _jets_index;
  };
  /// structure analogous to BriefJet, but with the extra information
  /// needed for dealing with tiles
  class TiledJet {
  public:
    double     eta, phi, kt2, NN_dist;
    TiledJet * NN, *previous, * next; 
    int        _jets_index, tile_index, diJ_posn;
    // routines that are useful in the minheap version of tiled
    // clustering ("misuse" the otherwise unused diJ_posn, so as
    // to indicate whether jets need to have their minheap entries
    // updated).
    inline void label_minheap_update_needed() {diJ_posn = 1;};
    inline void label_minheap_update_done()   {diJ_posn = 0;};
    inline bool minheap_update_needed() const {return diJ_posn==1;};
  };

  //-- some of the functions that follow are templates and will work
  //as well for briefjet and tiled jets

  /// set the kinematic and labelling info for jeta so that it corresponds
  /// to _jets[_jets_index]
  template <class J> void _bj_set_jetinfo( J * const jet, 
						 const int _jets_index) const;

  /// "remove" this jet, which implies updating links of neighbours and
  /// perhaps modifying the tile structure
  void _bj_remove_from_tiles( TiledJet * const jet) const;

  /// return the distance between two BriefJet objects
  template <class J> double _bj_dist(const J * const jeta, 
			const J * const jetb) const;

  // return the diJ (multiplied by _R2) for this jet assuming its NN
  // info is correct
  template <class J> double _bj_diJ(const J * const jeta) const;

  /// for testing purposes only: if in the range head--tail-1 there is a
  /// a jet which corresponds to hist_index in the history, then
  /// return a pointer to that jet; otherwise return tail.
  template <class J> inline J * _bj_of_hindex(
                          const int hist_index, 
			  J * const head, J * const tail) 
    const {
    J * res;
    for(res = head; res<tail; res++) {
      if (_jets[res->_jets_index].cluster_hist_index() == hist_index) {break;}
    }
    return res;
  };


  //-- remaining functions are different in various cases, so we
  //   will use templates but are not sure if they're useful...

  /// updates (only towards smaller distances) the NN for jeta without checking
  /// whether in the process jeta itself might be a new NN of one of
  /// the jets being scanned -- span the range head to tail-1 with
  /// assumption that jeta is not contained in that range
  template <class J> void _bj_set_NN_nocross(J * const jeta, 
            J * const head, const J * const tail) const;

  /// reset the NN for jeta and DO check whether in the process jeta
  /// itself might be a new NN of one of the jets being scanned --
  /// span the range head to tail-1 with assumption that jeta is not
  /// contained in that range
  template <class J> void _bj_set_NN_crosscheck(J * const jeta, 
            J * const head, const J * const tail) const;
  


  /// number of neighbours that a tile will have (rectangular geometry
  /// gives 9 neighbours).
  static const int n_tile_neighbours = 9;
  //----------------------------------------------------------------------
  /// The fundamental structures to be used for the tiled N^2 algorithm
  /// (see CCN27-44 for some discussion of pattern of tiling)
  struct Tile {
    /// pointers to neighbouring tiles, including self
    Tile *   begin_tiles[n_tile_neighbours]; 
    /// neighbouring tiles, excluding self
    Tile **  surrounding_tiles; 
    /// half of neighbouring tiles, no self
    Tile **  RH_tiles;  
    /// just beyond end of tiles
    Tile **  end_tiles; 
    /// start of list of BriefJets contained in this tile
    TiledJet * head;    
    /// sometimes useful to be able to tag a tile
    bool     tagged;    
  };
  std::vector<Tile> _tiles;
  double _tiles_eta_min, _tiles_eta_max;
  double _tile_size_eta, _tile_size_phi;
  int    _n_tiles_phi,_tiles_ieta_min,_tiles_ieta_max;

  // reasonably robust return of tile index given ieta and iphi, in particular
  // it works even if iphi is negative
  inline int _tile_index (int ieta, int iphi) const {
    // note that (-1)%n = -1 so that we have to add _n_tiles_phi
    // before performing modulo operation
    return (ieta-_tiles_ieta_min)*_n_tiles_phi
                  + (iphi+_n_tiles_phi) % _n_tiles_phi;
  }

  // routines for tiled case, including some overloads of the plain
  // BriefJet cases
  int  _tile_index(const double & eta, const double & phi) const;
  void _tj_set_jetinfo ( TiledJet * const jet, const int _jets_index);
  void  _bj_remove_from_tiles(TiledJet * const jet);
  void _initialise_tiles();
  void _print_tiles(TiledJet * briefjets ) const;
  void _add_neighbours_to_tile_union(const int tile_index, 
		 std::vector<int> & tile_union, int & n_near_tiles) const;
  void _add_untagged_neighbours_to_tile_union(const int tile_index, 
		 std::vector<int> & tile_union, int & n_near_tiles);


};



//**********************************************************************
//**************    START   OF   INLINE   MATERIAL    ******************
//**********************************************************************


//----------------------------------------------------------------------
// Transfer the initial jets into our internal structure
template<class L> void ClusterSequence::_transfer_input_jets(
                                       const std::vector<L> & pseudojets) {

  // this will ensure that we can point to jets without difficulties
  // arising.
  _jets.reserve(pseudojets.size()*2);

  // insert initial jets this way so that any type L that can be
  // converted to a pseudojet will work fine (basically PseudoJet
  // and any type that has [] subscript access to the momentum
  // components, such as CLHEP HepLorentzVector).
  for (unsigned int i = 0; i < pseudojets.size(); i++) {
    _jets.push_back(pseudojets[i]);}
  
}

//----------------------------------------------------------------------
// initialise from some generic type... Has to be made available
// here in order for it the template aspect of it to work...
template<class L> ClusterSequence::ClusterSequence (
			          const std::vector<L> & pseudojets,
				  const double & R,
				  const Strategy & strategy,
				  const bool & writeout_combinations) {

  // transfer the initial jets (type L) into our own array
  _transfer_input_jets(pseudojets);

  // run the clustering
  _initialise_and_run(R,strategy,writeout_combinations);
}


//----------------------------------------------------------------------
/// constructor of a jet-clustering sequence from a vector of
/// four-momenta, with the jet definition specified by jet_def
template<class L> ClusterSequence::ClusterSequence (
			          const std::vector<L> & pseudojets,
				  const JetDefinition & jet_def,
				  const bool & writeout_combinations) {

  // transfer the initial jets (type L) into our own array
  _transfer_input_jets(pseudojets);

  // run the clustering
  _initialise_and_run(jet_def,writeout_combinations);
}


inline const std::vector<PseudoJet> & ClusterSequence::jets () const {
  return _jets;
}

inline const std::vector<ClusterSequence::history_element> & ClusterSequence::history () const {
  return _history;
}

inline unsigned int ClusterSequence::n_particles() const {return _initial_n;}



inline double ClusterSequence::jet_scale_for_algorithm(
				  const PseudoJet & jet) const {
  if (_jet_finder == kt_algorithm)             {return jet.kt2();}
  else if (_jet_finder == cambridge_algorithm) {return 1.0;}
  else {throw Error("Unrecognised jet finder");}
}


//----------------------------------------------------------------------
template <class J> inline void ClusterSequence::_bj_set_jetinfo(
                            J * const jetA, const int _jets_index) const {
    jetA->eta  = _jets[_jets_index].rap();
    jetA->phi  = _jets[_jets_index].phi_02pi();
    jetA->kt2  = jet_scale_for_algorithm(_jets[_jets_index]);
    jetA->_jets_index = _jets_index;
    // initialise NN info as well
    jetA->NN_dist = _R2;
    jetA->NN      = NULL;
}




//----------------------------------------------------------------------
template <class J> inline double ClusterSequence::_bj_dist(
                const J * const jetA, const J * const jetB) const {
  double dphi = std::abs(jetA->phi - jetB->phi);
  double deta = (jetA->eta - jetB->eta);
  if (dphi > pi) {dphi = twopi - dphi;}
  return dphi*dphi + deta*deta;
}

//----------------------------------------------------------------------
template <class J> inline double ClusterSequence::_bj_diJ(const J * const jet) const {
  double kt2 = jet->kt2;
  if (jet->NN != NULL) {if (jet->NN->kt2 < kt2) {kt2 = jet->NN->kt2;}}
  return jet->NN_dist * kt2;
}


//----------------------------------------------------------------------
// set the NN for jet without checking whether in the process you might
// have discovered a new nearest neighbour for another jet
template <class J> inline void ClusterSequence::_bj_set_NN_nocross(
                 J * const jet, J * const head, const J * const tail) const {
  double NN_dist = _R2;
  J * NN  = NULL;
  if (head < jet) {
    for (J * jetB = head; jetB != jet; jetB++) {
      double dist = _bj_dist(jet,jetB);
      if (dist < NN_dist) {
	NN_dist = dist;
	NN = jetB;
      }
    }
  }
  if (tail > jet) {
    for (J * jetB = jet+1; jetB != tail; jetB++) {
      double dist = _bj_dist(jet,jetB);
      if (dist < NN_dist) {
	NN_dist = dist;
	NN = jetB;
      }
    }
  }
  jet->NN = NN;
  jet->NN_dist = NN_dist;
}


//----------------------------------------------------------------------
template <class J> inline void ClusterSequence::_bj_set_NN_crosscheck(J * const jet, 
		    J * const head, const J * const tail) const {
  double NN_dist = _R2;
  J * NN  = NULL;
  for (J * jetB = head; jetB != tail; jetB++) {
    double dist = _bj_dist(jet,jetB);
    if (dist < NN_dist) {
      NN_dist = dist;
      NN = jetB;
    }
    if (dist < jetB->NN_dist) {
      jetB->NN_dist = dist;
      jetB->NN = jet;
    }
  }
  jet->NN = NN;
  jet->NN_dist = NN_dist;
}




FASTJET_END_NAMESPACE

#endif // __FASTJET_CLUSTERSEQUENCE_HH__

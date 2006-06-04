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
 * See also the main <a
 * href="http://www.lpthe.jussieu.fr/~salam/fastjet">FastJet</a> page.
 */
//----------------------------------------------------------------------

#ifndef __CLUSTERSEQUENCE_H_
#define __CLUSTERSEQUENCE_H_

#include<vector>
#include<map>
#include "DynamicNearestNeighbours.hh"
#include "FjPseudoJet.hh"
#include<cassert>
#include<iostream>
#include<string>
#include<cmath> // needed to get double std::abs(double)
#include "FjError.hh"

/// the various options for the algorithmic strategy to adopt in
/// clustering the event.
enum FjStrategy {
  /// fastest from about 50..10^4
  N2Tiled     = -3, 
  /// legacy
  N2PoorTiled = -2, 
  /// fastest below 50
  N2Plain     = -1, 
  /// worse even than the usual N^3 algorithms
  N3Dumb      =  0, 
  /// automatic selection of the best
  Best        =  1, 
  /// best of the NlnN variants -- best overall for N>10^4
  NlnN        =  2, 
  /// legacy N ln N using 3pi coverage of cylinder
  NlnN3pi     =  3, 
  /// legacy N ln N using 4pi coverage of cylinder
  NlnN4pi     =  4 
};

/// deals with clustering
class FjClusterSequence {


 public: 

  /// empty initializer
  FjClusterSequence () {};

  /// create a clustersequence starting from the supplied set
  /// of pseudojets and clustering them with the long-invariant
  /// kt algorithm (E-scheme recombination) with the supplied
  /// value for R.
  ///
  /// If strategy=DumbN3 a very stupid N^3 algorithm is used for the
  /// clustering; otherwise strategy = NlnN* uses cylinders algorithms
  /// with some number of pi coverage. If writeout_combinations=true a
  /// summary of the recombination sequence is written out
  template<class L> FjClusterSequence (const std::vector<L> & pseudojets, 
		   const double & R = 1.0,
		   const FjStrategy & strategy = Best,
		   const bool & writeout_combinations = false);

  // NB: in the routines that follow, for extracting lists of jets, a
  //     list structure might be more efficient, if sometimes a little
  //     more awkward to use (at least for old fortran hands).

  /// return a vector of all jets (in the sense of the inclusive
  /// algorithm) with pt >= ptmin. Time taken should be of the order
  /// of the number of jets returned.
  std::vector<FjPseudoJet> inclusive_jets (const double & ptmin = 0.0) const;

  /// return the number of jets (in the sense of the exclusive
  /// algorithm) that would be obtained when running the algorithm
  /// with the given dcut.
  int n_exclusive_jets (const double & dcut) const;

  /// return a vector of all jets (in the sense of the exclusive
  /// algorithm) that would be obtained when running the algorithm
  /// with the given dcut.
  std::vector<FjPseudoJet> exclusive_jets (const double & dcut) const;

  /// return a vector of all jets when the event is clustered (in the
  /// exclusive sense) to exactly njets.
  std::vector<FjPseudoJet> exclusive_jets (const int & njets) const;

  /// return the dmin corresponding to the recombination that went from
  /// n+1 to n jets (sometimes known as d_{n n+1}).
  double exclusive_dmerge (const int & njets) const;

  /// return the maximum of the dmin encountered during all recombinations 
  /// up to the one that led to an n-jet final state; identical to
  /// exclusive_dmerge, except in cases where the dmin do not increase
  /// monotonically.
  double exclusive_dmerge_max (const int & njets) const;

  /// return a vector of the particles that make up jet
  std::vector<FjPseudoJet> constituents (const FjPseudoJet & jet) const;
  /// add on to subjet_vector the subjets of jet.
  void add_constituents (const FjPseudoJet & jet, 
			 std::vector<FjPseudoJet> & subjet_vector) const;

  /// return the enum value of the strategy used to cluster the event
  inline FjStrategy strategy_used () const {return _strategy;};
  std::string strategy_string () const;


  /// returns the scale associated with a jet as required for this
  /// clustering algorithm (kt^2 for the kt-algorithm, 1 for the 
  /// Cambridge algorithm). [May become virtual at some point]
  double jet_scale_for_algorithm(const FjPseudoJet & jet) const;

  /// things related to choice of algorithm
  enum FjJetFinder { kt_algorithm = 0, cambridge_algorithm = 1};

private:
  static FjJetFinder _jet_finder;

public:
  static void set_jet_finder (FjJetFinder jet_finder) {_jet_finder = jet_finder;};


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
                 /// FjPseudoJet object corresponding to this jet
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
  /// derived from FjClusterSequence).
  const std::vector<FjPseudoJet> & jets()    const;

  /// allow the user to access the history in this raw manner (see
  /// above for motivation).
  const std::vector<history_element> & history() const;

  /// returns the number of particles that were provided to the
  /// clustering algorithm (helps the user finr their way around the
  /// history and jets objects if they weren't paying attention
  /// beforehand).
  unsigned int n_particles() const;

 protected:

  /// this is the routine that will do all the initialisation and
  /// then run the clustering (may be called by various constructors).
  void _initialise_and_run (//test// const std::vector<FjPseudoJet> & pseudojets, 
			    const double & R,
			    const FjStrategy & strategy,
			    const bool & writeout_combinations);
  /// This contains the physical FjPseudoJets; for each FjPseudoJet one
  /// can find the corresponding position in the _history by looking
  /// at _jets[i].cluster_hist_index().
  std::vector<FjPseudoJet> _jets;


  /// this vector will contain the branching history; for each stage,
  /// _history[i].jetp_index indicates where to look in the _jets
  /// vector to get the physical FjPseudoJet.
  std::vector<history_element> _history;

  bool _writeout_combinations;
  int  _initial_n;
  double _Rparam, _R2, _invR2;
  FjStrategy    _strategy;


 private:


  void _really_dumb_cluster ();
  void _delaunay_cluster ();
  void _simple_N2_cluster ();
  void _tiled_N2_cluster ();
  void _faster_tiled_N2_cluster ();

  void _fill_initial_history();
  void _add_step_to_history(const int & step_number, const int & parent1, 
			       const int & parent2, const int & jetp_index,
			       const double & dij);

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
  struct TiledJet {
    double     eta, phi, kt2, NN_dist;
    TiledJet * NN, *previous, * next; 
    int        _jets_index, tile_index, diJ_posn;
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


// initialise from some generic type... Has to be made available
// here in order for it the template aspect of it to work...
template<class L> FjClusterSequence::FjClusterSequence (
			          const std::vector<L> & pseudojets,
				  const double & R,
				  const FjStrategy & strategy,
				  const bool & writeout_combinations) {

  // this will ensure that we can point to jets without difficulties
  // arising
  _jets.reserve(pseudojets.size()*2);

  // insert initial jets this way so that any type L that can be
  // converted to a pseudojet will work fine (basically FjPseudoJet
  // and any type that has [] subscript access to the momentum
  // components, such as CLHEP HepLorentzVector).
  for (unsigned int i = 0; i < pseudojets.size(); i++) {
    _jets.push_back(pseudojets[i]);}

  _initialise_and_run(R,strategy,writeout_combinations);
}


inline const std::vector<FjPseudoJet> & FjClusterSequence::jets () const {
  return _jets;
}

inline const std::vector<FjClusterSequence::history_element> & FjClusterSequence::history () const {
  return _history;
}

inline unsigned int FjClusterSequence::n_particles() const {return _initial_n;}



inline double FjClusterSequence::jet_scale_for_algorithm(
				  const FjPseudoJet & jet) const {
  if (_jet_finder == kt_algorithm)             {return jet.kt2();}
  else if (_jet_finder == cambridge_algorithm) {return 1.0;}
  else {throw FjError("Unrecognised jet algorithm");}
}


//----------------------------------------------------------------------
template <class J> inline void FjClusterSequence::_bj_set_jetinfo(
                            J * const jetA, const int _jets_index) const {
    jetA->eta  = _jets[_jets_index].rap();
    jetA->phi  = _jets[_jets_index].phi();
    jetA->kt2  = jet_scale_for_algorithm(_jets[_jets_index]);
    jetA->_jets_index = _jets_index;
    // initialise NN info as well
    jetA->NN_dist = _R2;
    jetA->NN      = NULL;
}




//----------------------------------------------------------------------
template <class J> inline double FjClusterSequence::_bj_dist(
                const J * const jetA, const J * const jetB) const {
  double dphi = std::abs(jetA->phi - jetB->phi);
  double deta = (jetA->eta - jetB->eta);
  if (dphi > pi) {dphi = twopi - dphi;}
  return dphi*dphi + deta*deta;
}

//----------------------------------------------------------------------
template <class J> inline double FjClusterSequence::_bj_diJ(const J * const jet) const {
  double kt2 = jet->kt2;
  if (jet->NN != NULL) {if (jet->NN->kt2 < kt2) {kt2 = jet->NN->kt2;}}
  return jet->NN_dist * kt2;
}


//----------------------------------------------------------------------
// set the NN for jet without checking whether in the process you might
// have discovered a new nearest neighbour for another jet
template <class J> inline void FjClusterSequence::_bj_set_NN_nocross(
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
template <class J> inline void FjClusterSequence::_bj_set_NN_crosscheck(J * const jet, 
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



#endif // __CLUSTERSEQUENCE_H_

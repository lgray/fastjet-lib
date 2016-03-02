#ifndef __FASTJET_NNBASE_HH__
#define __FASTJET_NNBASE_HH__

//FJSTARTHEADER
// $Id$
//
// Copyright (c) 2016, Matteo Cacciari, Gavin P. Salam and Gregory Soyez
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
//  development. They are described in the original FastJet paper,
//  hep-ph/0512210 and in the manual, arXiv:1111.6097. If you use
//  FastJet as part of work towards a scientific publication, please
//  quote the version you use and include a citation to the manual and
//  optionally also to hep-ph/0512210.
//
//  FastJet is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with FastJet. If not, see <http://www.gnu.org/licenses/>.
//----------------------------------------------------------------------
//FJENDHEADER

#include<fastjet/ClusterSequence.hh>


FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

/// @ingroup advanced_usage
/// \class _NoInfo
/// dummy class, used as a default template argument
class _NoInfo {};

/// @ingroup advanced_usage
/// \class NNInfo
/// template that will help initialise a BJ with a PseudoJet and extra information
template<class I> class NNInfo {
public:
  NNInfo()         : _info(NULL) {}
  NNInfo(I * info) : _info(info) {}
  template<class BJ> void init_jet(BJ * briefjet, const fastjet::PseudoJet & jet, int index) { briefjet->init(jet, index, _info);}
private:
  I * _info;
};

/// @ingroup advanced_usage
/// Specialisation of NNInfo for cases where there is no extra info
template<> class NNInfo<_NoInfo>  {
public:
  NNInfo()           {}
  NNInfo(_NoInfo * ) {}
  template<class BJ> void init_jet(BJ * briefjet, const fastjet::PseudoJet & jet, int index) { briefjet->init(jet, index);}
};


//----------------------------------------------------------------------
/// @ingroup advanced_usage
/// \class NBase
/// Help solve closest pair problems with generic interparticle and
/// beam distance.
///
/// Description and derived classes:
///
///   This is an abstract base class for several ways of solving the
///   problem:
///  
///    - NNH        provides an implementation for generic measures
///  
///    - NNPlainN2  provides an implementation for distances satisfying
///                 the FastJet lemma i.e. distances for which the
///                 minimum corresponds to a geometric nearest neighbour
///                 (i.e. the distance can be factorised in a momentum
///                 factor and a geometric piece). This is based on the
///                 fastjet N2Plain clustering strategy
///  
///    - NNTiledN2  is a tiled version of NNPlainN2 (based on the N2Tiled
///                 FastJet clustering strategy). It further requires
///                 that the search for the nearest neighbour of a given
///                 point can be done in its tile or the neighbouring
///                 ones. In practice, this means that the beam distance
///                 is smaller than the distance between a given tile
///                 and all non-neighbouring ones.
///
/// Underlying BJ class:
/// 
///   All derived classes will be templated with a BJ (brief jet)
///   class --- BJ should basically cache the minimal amount of
///   information that is needed to efficiently calculate
///   interparticle distances and particle-beam distances.
///   
///   This class can be used with or without an extra "Information"
///   template, i.e. NN*<BJ> or NN*<BJ,I>. BJ must provide one of the
///   two following init function:
///   
///     void   BJ::init(const PseudoJet & jet);            // initialise with a PseudoJet
///     void   BJ::init(const PseudoJet & jet, I * info);  // initialise with a PseudoJet + info
///   
///   where info might be a pointer to a class that contains, e.g.,
///   information about R, or other parameters of the jet algorithm
///   
///   It must then provide information about the distance that depends
///   on the specific case (see the corresponding classes for details).
///
///   NOTE: IN ALL CASES, THE DISTANC EMUST BE SYMMETRIC!!!
///
/// Workflow:
///
///   In all cases, the usage of NNBase classes works as follows:
///
///   First, from the list of particles, create an NNwhatever<BJ>
///   object of the appropriate type with the appropriate BJ class
///   (and optional extra info).
///
///   Then, cluster using a loop like this (assuming a FastJet plugin)
///     
///     while (njets > 0) {
///       int i, j, k;
///       // get the i and j that minimize the distance
///       double dij = nn.dij_min(i, j);  
///
///       // do the appropriate recombination and update the nn
///       if (j >= 0) {    // interparticle recombination
///         cs.plugin_record_ij_recombination(i, j, dij, k);
///         nn.merge_jets(i, j, cs.jets()[k], k); 
///       } else {         // bbeam recombination
///         double diB = cs.jets()[i].E()*cs.jets()[i].E(); // get new diB
///         cs.plugin_record_iB_recombination(i, diB);
///         nn.remove_jet(i);
///       }
///       njets--;
///     }
///
/// For an example of how the NNH<BJ> class is used, see the Jade (and
/// EECambridge) plugins
template<class I = _NoInfo> class NNBase : public NNInfo<I> {
public:
  /// constructor with an initial set of jets (which will be assigned indices
  /// 0 ... jets.size()-1
  NNBase() {}
  NNBase(I * info) : NNInfo<I>(info) {}

  /// initialisation from a given list of particles
  virtual void start(const std::vector<PseudoJet> & jets) = 0;
  
  /// return the dij_min and indices iA, iB, for the corresponding jets.
  /// If iB < 0 then iA recombines with the beam
  virtual double dij_min(int & iA, int & iB) = 0;

  /// remove the jet pointed to by index iA
  virtual void remove_jet(int iA) = 0;

  /// merge the jets pointed to by indices A and B and replace them with
  /// jet, assigning it an index jet_index.
  virtual void merge_jets(int iA, int iB, const PseudoJet & jet, int jet_index) =  0;
};


FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh


#endif // __FASTJET_NNBASE_HH__

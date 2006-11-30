//STARTHEADER
// $Id: ClusterSequence.cc 370 2006-11-28 16:25:44Z salam $
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

#include "CDFMidPointPlugin.hh"
#include "fastjet/ClusterSequence.hh"
#include <sstream>

// CDF stuff
#include "MidPointAlgorithm.hh"
#include "PhysicsTower.hh"
#include "Cluster.hh"

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

using namespace std;

string CDFMidPointPlugin::description () const {
  ostringstream desc;
  
  desc << "CDF MidPoint jet finder with " 
       << "seed_threshold = "     << seed_threshold     () << ", "
       << "cone_radius = "        << cone_radius        () << ", "
       << "cone_area_fraction = " << cone_area_fraction () << ", " 
       << "max_pair_size = "      << max_pair_size      () << ", "
       << "max_iterations = "     << max_iterations     () << ", "
       << "overlap_threshold  = " << overlap_threshold  () ;

  return desc.str();
}


void CDFMidPointPlugin::run_clustering(ClusterSequence & clust_seq) const {
 
  // create the physics towers needed by the CDF code
  vector<PhysicsTower> towers;
  towers.reserve(clust_seq.jets().size());
  for (unsigned i = 0; i < clust_seq.jets().size(); i++) {
    LorentzVector fourvect(clust_seq.jets()[i].px(),
			   clust_seq.jets()[i].py(),
			   clust_seq.jets()[i].pz(),
			   clust_seq.jets()[i].E());
    PhysicsTower tower(fourvect);
    // misuse one of the indices for tracking, since the MidPoint
    // implementation doesn't seem to make use of these indices
    tower.calTower.iEta = i;
    towers.push_back(tower);
  }

  // prepare the CDF algorithm
  MidPointAlgorithm m(_seed_threshold,_cone_radius,_cone_area_fraction,
		      _max_pair_size,_max_iterations,_overlap_threshold);
    
  // run the CDF algorithm
  std::vector<Cluster> jets;
  m.run(towers,jets);


  // now transfer the jets back into our own structure -- we will
  // mimic the cone code with a sequential recombination sequence in
  // which the jets are built up by adding one particle at a time
  for(vector<Cluster>::const_iterator jetIter = jets.begin(); 
                                      jetIter != jets.end(); jetIter++) {
    const vector<PhysicsTower> & tower_list = jetIter->towerList;
    int jet_k = tower_list[0].calTower.iEta;
  
    int ntow = int(jetIter->towerList.size());
    for (int itow = 1; itow < ntow; itow++) {
      int jet_i = jet_k;
      // retrieve our misappropriated index for the jet
      int jet_j = tower_list[itow].calTower.iEta;
      // do a fake recombination step with dij=0
      double dij = 0.0;
      clust_seq.plugin_record_ij_recombination(jet_i, jet_j, dij, jet_k);
    }
  
    // NB: put a sensible looking d_iB just to be nice...
    double d_iB = clust_seq.jets()[jet_k].perp2();
    clust_seq.plugin_record_iB_recombination(jet_k, d_iB);
  }


  // following code is for testing only
  //cout << endl;
  //for(vector<Cluster>::const_iterator jetIter = jets.begin(); 
  //                                    jetIter != jets.end(); jetIter++) {
  //  cout << jetIter->fourVector.pt() << " " << jetIter->fourVector.y() << endl;
  //}
  //cout << "-----------------------------------------------------\n";
  //vector<PseudoJet> ourjets(clust_seq.inclusive_jets());
  //for (vector<PseudoJet>::const_reverse_iterator ourjet = ourjets.rbegin();
  //     ourjet != ourjets.rend(); ourjet++) {
  //  cout << ourjet->perp() << " " << ourjet->rap() << endl;
  //}
  //cout << endl;
}

FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

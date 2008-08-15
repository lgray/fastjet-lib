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

#include "fastjet/CDFJetCluPlugin.hh"
#include "fastjet/ClusterSequence.hh"
#include <sstream>
#include <cassert>

// CDF stuff
#include "JetCluAlgorithm.hh"
#include "PhysicsTower.hh"
#include "Cluster.hh"

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

using namespace std;

string CDFJetCluPlugin::description () const {
  ostringstream desc;
  
  desc << "CDF JetClu jet algorithm with " 
       << "seed_threshold = "     << seed_threshold    () << ", "
       << "cone_radius = "        << cone_radius       () << ", "
       << "adjacency_cut = "      << adjacency_cut     () << ", " 
       << "max_iterations = "     << max_iterations    () << ", "
       << "iratch = "             << iratch            () << ", "
       << "overlap_threshold = "  << overlap_threshold () ;

  return desc.str();
}


void CDFJetCluPlugin::run_clustering(ClusterSequence & clust_seq) const {
 
  // create the physics towers needed by the CDF code
  vector<PhysicsTower> towers;
  towers.reserve(clust_seq.jets().size());

  // create a map to identify jets (actually just the input particles)...
  //map<double,int> jetmap;

  for (unsigned i = 0; i < clust_seq.jets().size(); i++) {
    PseudoJet particle(clust_seq.jets()[i]);
    //_insert_unique(particle, jetmap);
    LorentzVector fourvect(particle.px(), particle.py(),
			   particle.pz(), particle.E());
    PhysicsTower tower(fourvect);
    // add tracking information for later
    tower.fjindex = i;
    towers.push_back(tower);
  }

  // prepare the CDF algorithm
  JetCluAlgorithm j(seed_threshold(), cone_radius(), adjacency_cut(),
                    max_iterations(), iratch(), overlap_threshold());
    
  // run the CDF algorithm
  std::vector<Cluster> jets;
  j.run(towers,jets);


  // now transfer the jets back into our own structure -- we will
  // mimic the cone code with a sequential recombination sequence in
  // which the jets are built up by adding one particle at a time

  // NB: with g++-4.0, the reverse iterator code gave problems, so switch
  //     to indices instead
  //for(vector<Cluster>::const_reverse_iterator jetIter = jets.rbegin(); 
  //                                    jetIter != jets.rend(); jetIter++) {
  //  const vector<PhysicsTower> & tower_list = jetIter->towerList;
  //  int jet_k = jetmap[tower_list[0].fourVector.E];
  //
  //  int ntow = int(jetIter->towerList.size());

  for(int iCDFjets = jets.size()-1; iCDFjets >= 0; iCDFjets--) {
    const vector<PhysicsTower> & tower_list = jets[iCDFjets].towerList;
    //int jet_k = jetmap[tower_list[0].fourVector.E];
    int jet_k = tower_list[0].fjindex;
  
    int ntow = int(tower_list.size());
    for (int itow = 1; itow < ntow; itow++) {
      int jet_i = jet_k;
      // retrieve our misappropriated index for the jet
      int jet_j;
      jet_j = tower_list[itow].fjindex;
      //int alt_jet_j = jetmap[tower_list[itow].fourVector.E];
      //assert(jet_j == alt_jet_j);
      //cout << jet_j << endl;
      // safety check
      assert (jet_j >= 0 && jet_j < int(towers.size()));
      // do a fake recombination step with dij=0
      double dij = 0.0;
      // JetClu does E-scheme recombination so we can stick with the
      // simple option
      clust_seq.plugin_record_ij_recombination(jet_i, jet_j, dij, jet_k);
      //if (itow != ntow) {
      //  clust_seq.plugin_record_ij_recombination(jet_i, jet_j, dij, jet_k);
      //} else {
      //  clust_seq.plugin_record_ij_recombination(jet_i, jet_j, dij, 
      //       PseudoJet(jetIter->fourVector.px,jetIter->fourVector.py,
      //                 jetIter->fourVector.pz,jetIter->fourVector.E),
      //                                           jet_k);
      //}
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
  //for (vector<PseudoJet>::const_iterator ourjet = ourjets.begin();
  //     ourjet != ourjets.end(); ourjet++) {
  //  cout << ourjet->perp() << " " << ourjet->rap() << endl;
  //}
  //cout << endl;
}


//// following code should now be obsolete since addition of 
//// index to the physics tower in the CDF code
// void CDFJetCluPlugin::_insert_unique(PseudoJet & jet, 
//                                      map<double,int> & jetmap) const {
//   while (jetmap.find(jet.E()) != jetmap.end()) {
//     // deal with cases where something else has the same energy, and
//     // also with situation where that energy is zero.
//     if (jet.E() != 0.0) {
//       jet *= 1.0+1e-12;
//     } else {
//       jet += PseudoJet(0.0,0.0,0.0,1e-300);
//     }
//   }
//   jetmap[jet.E()] = jet.cluster_hist_index();
// }


FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

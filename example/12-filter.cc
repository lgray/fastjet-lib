//----------------------------------------------------------------------
/// \file
/// \page Example12 12 - use of filtering
///
/// fastjet example program, illustration the use of a Filter
///
/// [MORE DETAILS TO FOLLOW......]
///
/// run it with    : ./12-filter < data/single-event.dat
///
/// Source code: 12-filter.cc
//----------------------------------------------------------------------

#include <fastjet/PseudoJet.hh>
#include <fastjet/ClusterSequence.hh>
#include <fastjet/Selector.hh>
#include <iostream>
#include "fastjet/tools/Filter.hh"

#include <cstdio>   // needed for io

using namespace fastjet;
using namespace std;

/// an example program showing how to use fastjet
int main (int argc, char ** argv) {
  // read in input particles
  //----------------------------------------------------------
  vector<PseudoJet> input_particles;
  
  double px, py , pz, E;
  while (cin >> px >> py >> pz >> E) {
    // create a fastjet::PseudoJet with these components and put it onto
    // back of the input_particles vector
    input_particles.push_back(PseudoJet(px,py,pz,E)); 
  }
 
  // get the resulting jets ordered in pt
  //----------------------------------------------------------
  JetDefinition jet_def(cambridge_algorithm, 1.2);
  ClusterSequence clust_seq(input_particles, jet_def);
  vector<fastjet::PseudoJet> inclusive_jets = sorted_by_pt(clust_seq.inclusive_jets(5.0));

  // label the columns
  printf("%5s %15s %15s %15s\n","jet #", "rapidity", "phi", "pt");
 
  // print out the details for each jet
  for (unsigned int i = 0; i < inclusive_jets.size(); i++) {
    printf("%5u %15.8f %15.8f %15.8f\n",
	   i, inclusive_jets[i].rap(), inclusive_jets[i].phi(),
	   inclusive_jets[i].perp());
  }

  // transformer piece
  //----------------------------------------------------------
  vector<PseudoJet> candidates;
  candidates.push_back(inclusive_jets[0]); // yes, it could crash if there is no jet above 5 GeV... lazyness
  candidates.push_back(join(inclusive_jets[0],inclusive_jets[1])); // yes, it could crash if there is no jet above 5 GeV... lazyness

  vector<Filter> filters;
  filters.push_back(Filter(JetDefinition(kt_algorithm, 0.2), SelectorPtMin(2.0)));
  filters.push_back(Filter(JetDefinition(kt_algorithm, 0.2), SelectorNHardest(3)));
  filters.push_back(Filter(JetDefinition(cambridge_algorithm, 0.4), SelectorNHardest(2)));

  for (vector<PseudoJet>::iterator jit=candidates.begin(); jit!=candidates.end(); jit++){
    const PseudoJet & c = *jit;
    cout << "Original jet : " << c.description() << endl;
    cout << "  rap = " << c.rap() << ", phi = " << c.phi() << ", pt = " << c.perp() << endl;

    for (vector<Filter>::iterator it=filters.begin(); it!=filters.end(); it++){
      const Filter & f = *it;
      
      cout << "Applying filter: " << f.description() << endl;
      PseudoJet j = f(c);
      
      cout << "Resulting jet : " << j.description() << endl;
      cout << "  rap = " << j.rap() << ", phi = " << j.phi() << ", pt = " << j.perp() << endl;
      cout << "  #pieces: " << j.pieces().size() << endl;
      
      // note that, alternatively, we could directly access teh
      // relevant structure
      assert(j.has_properties_of<Filter>());
      const FilteredJetStructure * fj_struct = j.extra_properties<Filter>();
      cout << "  #rejected pieces: " << fj_struct->rejected().size() << endl;
    }
    cout << endl;
  }

  return 0;
}

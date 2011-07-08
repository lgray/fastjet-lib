//----------------------------------------------------------------------
/// \file
/// \page Example11 11 - use of filtering
///
/// fastjet example program illustrating the use of the fastjet::Filter class
///
/// To do that, we apply different filter examples on a either the
/// hardest jet of the given event or the composition of the two
/// hardest jets: a filter keeping a fixed number of subjets (as in
/// arXiv:0802.2470), and a "trimmer" i.e. a filter keeping subjets
/// carrying a sufficient fraction of the pt of the jet
/// (arXiv:0912.1342).
///
/// run it with    : ./11-filter < data/single-event.dat
///
/// Source code: 11-filter.cc
//----------------------------------------------------------------------

#include <fastjet/PseudoJet.hh>
#include <fastjet/ClusterSequence.hh>
#include <fastjet/Selector.hh>
#include <iostream>
#include "fastjet/tools/Filter.hh"

#include <cstdio>   // needed for io

using namespace fastjet;
using namespace std;

// a function returning
//   min(Rmax, deltaR_factor * deltaR(j1,j2))
// where j1 and j2 are the 2 subjets of j
// if the jet does not have 2 exactly pieces, Rmax is used.
class DynamicRfilt : public FunctionOfPseudoJet<double>{
public:
  // default ctor 
  DynamicRfilt(double Rmax, double deltaR_factor) : _Rmax(Rmax), _deltaR_factor(deltaR_factor){}

  // action of the function
  double result(const PseudoJet &j) const{
    if (! j.has_pieces()) return _Rmax;

    vector<PseudoJet> pieces = j.pieces();
    if (! pieces.size()==2) return _Rmax;

    double deltaR = pieces[0].delta_R(pieces[1]);
    return min(_Rmax, _deltaR_factor * deltaR);
  }

private:
  double _Rmax, _deltaR_factor;
};

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

  // simple test to avoid that the example below crashes:
  // make sure there is at least 2 jets above our 5 GeV
  if (inclusive_jets.size()<2){
    cout << "Please provide an event with at least 2 jets above 5 GeV" << endl;
    return 1;
  }

  // the sample PseudoJet that we shall filter
  //  - the hardest jet of the event
  //  - the composition of the 2 hardest jets (showing that the Filter
  //    can also be applied on a CompositeJet)
  //----------------------------------------------------------
  vector<PseudoJet> candidates;
  candidates.push_back(inclusive_jets[0]);
  candidates.push_back(join(inclusive_jets[1],inclusive_jets[2]));

  // create a few filters
  //----------------------------------------------------------
  vector<Filter> filters;

  // the Aachen/Cambridge filter with Rfilt=0.3
  filters.push_back(Filter(JetDefinition(cambridge_algorithm, 0.3), SelectorNHardest(3)));

  // the Aachen/Cambridge filter with Rfilt=min(0.3, 0.5*Rbb) as in arXiv:0802.2470
  SharedPtr<DynamicRfilt> dynamic_Rfilt(new DynamicRfilt(0.3, 0.5));
  filters.push_back(Filter(dynamic_Rfilt.get(), SelectorNHardest(3)));

  // Filtering with a pt cut as for trimming (arXiv:0912.1342)
  filters.push_back(Filter(JetDefinition(kt_algorithm, 0.2), SelectorPtFractionMin(0.03)));

  // apply the various filters on the test PseudoJet
  // and show the result
  //----------------------------------------------------------
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
      
      // access properties specific to the Filter
      //
      // We first make sure that the jet indeed has a structure
      // compatible with the result of a Filter (using
      // has_structure_of()), then retrieve the pieces rejected by the
      // filter (using structure_of())
      assert(j.has_structure_of<Filter>());
      const Filter::StructureType & fj_struct = j.structure_of<Filter>();
      cout << "  #rejected pieces: " << fj_struct.rejected().size() << endl;
    }
    cout << endl;
  }

  return 0;
}

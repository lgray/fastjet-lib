
// fastjet stuff
#include "fastjet/ClusterSequence.hh"
#include "SISConePlugin.hh"

// scones stuff
#include "momentum.h"
#include "siscone.h"

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

using namespace std;
using namespace siscone;

string SISConePlugin::description () const {
  ostringstream desc;
  
  desc << "SISCone jet finder with " 
       << "cone_radius = "        << cone_radius        () << ", "
       << "overlap_threshold  = " << overlap_threshold  () << ", "
       << "n_pass_max  = "        << n_pass_max         () ;

  // create a fake scones object so that we can find out more about it
  Csiscone siscone;
  if (siscone.merge_identical_protocones) {
    desc << ", and (IR unsafe) merge_indentical_protocones=true" ;
  }

  return desc.str();
}

void SISConePlugin::run_clustering(ClusterSequence & clust_seq) const {

  int n = clust_seq.jets().size();
  // transfer fastjet initial particles into the siscone type
  vector<Cmomentum> siscone_momenta(n);
  for(int i = 0; i < n; i++) {
    const PseudoJet & p = clust_seq.jets()[i]; // shorthand
    siscone_momenta[i] = Cmomentum(p.px(), p.py(), p.pz(), p.E());
  }

  // run the jet finding
  Csiscone siscone;
  siscone.compute_jets(siscone_momenta, cone_radius(), overlap_threshold(),
                      n_pass_max());

  // extract the jets [in reverse order -- to get nice ordering in pt at end]
  int njet = siscone.jets.size();

  for (int ijet = njet-1; ijet >= 0; ijet--) {
    const Cjet & jet = siscone.jets[ijet]; // shorthand
    
    // Successively merge the particles that make up the cone jet
    // until we have all particles in it.  Start off with the zeroth
    // particle.
    int jet_k = jet.content[0];
    for (unsigned ipart = 1; ipart < jet.content.size(); ipart++) {
      // take the last result of the merge
      int jet_i = jet_k;
      // and the next element of the jet
      int jet_j = jet.content[ipart];
      // and merge them (with a fake dij)
      double dij = 0.0;

      // create the new jet by hand so that we can adjust its user index
      PseudoJet newjet = clust_seq.jets()[jet_i] + clust_seq.jets()[jet_j];

      // set the user index to be the pass in which the jet was discovered
      newjet.set_user_index(jet.pass);
        
      clust_seq.plugin_record_ij_recombination(jet_i, jet_j, dij, newjet, jet_k);
    }
    // we have merged all the jet's particles into a single object, so now
    // "declare" it to be a beam (inclusive) jet.
    // [NB: put a sensible looking d_iB just to be nice...]
    double d_iB = clust_seq.jets()[jet_k].perp2();
    clust_seq.plugin_record_iB_recombination(jet_k, d_iB);
  }
}


// OBSOLETE CODE  
//// temporary, for dealing with warnings...
//if (siscone.n_warnings > 0) {
//  // print the event (very dirty...)
//  cout.precision(14);
//  cout << "Culprit event is:" << endl;
//  for (int i = 0; i < n; i++) {
//    const PseudoJet & p = clust_seq.jets()[i]; // shorthand
//    cout << p.px() << " " << p.py() << " " << p.pz() << " " << p.E() << endl;
//  }
//}


FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

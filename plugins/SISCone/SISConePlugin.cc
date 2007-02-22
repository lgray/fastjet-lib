
// fastjet stuff
#include "fastjet/ClusterSequence.hh"
#include "SISConePlugin.hh"

// scones stuff
#include "momentum.h"
#include "siscone.h"

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

using namespace std;
using namespace siscone;

// the static objects
auto_ptr<SISConePlugin     > SISConePlugin::stored_plugin    ;
auto_ptr<vector<PseudoJet> > SISConePlugin::stored_particles ;
auto_ptr<Csiscone          > SISConePlugin::stored_siscone   ;

string SISConePlugin::description () const {
  ostringstream desc;
  
  const string on = "on";
  const string off = "off";
  const string pt2m2 = "pt^2+m^2";
  const string pt2 = "pt^2 (IR unsafe)";

  desc << "SISCone jet finder with " ;
  desc << "cone_radius = "       << cone_radius        () << ", ";
  desc << "overlap_threshold = " << overlap_threshold  () << ", ";
  desc << "n_pass_max = "        << n_pass_max         () << ", ";
  desc << "protojet_ptmin = "    << protojet_ptmin()      << ", ";
  desc << "split-merge uses = " << (_split_merge_on_transverse_mass ? 
                                   pt2m2 : pt2) << ", ";
  desc << "caching turned "      << (caching() ? on : off);

  // create a fake scones object so that we can find out more about it
  Csiscone siscone;
  if (siscone.merge_identical_protocones) {
    desc << ", and (IR unsafe) merge_indentical_protocones=true" ;
  }

  return desc.str();
}


void SISConePlugin::run_clustering(ClusterSequence & clust_seq) const {


  Csiscone * siscone;
  Csiscone   local_siscone;

  unsigned n = clust_seq.jets().size();

  bool new_siscone = true; // by default we'll be running it

  if (caching()) {

    // Establish if we have a cached run with the same R, npass and
    // particles. If not then do any tidying up / reallocation that's
    // necessary for the next round of caching, otherwise just set
    // relevant pointers so that we can reuse and old run.
    if (stored_siscone.get() != 0) {
      new_siscone = !(stored_plugin->cone_radius()   == cone_radius()
                      && stored_plugin->n_pass_max() == n_pass_max()  
                      && stored_particles->size()    == n);
      if (!new_siscone) {
        for(unsigned i = 0; i < n; i++) {
          // only check momentum because indices will be correctly dealt
          // with anyway when extracting the clustering order.
          new_siscone |= !have_same_momentum(clust_seq.jets()[i], 
                                             (*stored_particles)[i]);
        }
      }
    } 
      
    // allocate the new siscone, etc., if need be
    if (new_siscone) {
      stored_siscone  .reset( new Csiscone                           );
      stored_particles.reset( new vector<PseudoJet>(clust_seq.jets()));
      stored_plugin   .reset( new SISConePlugin(*this)               );
    }

    siscone = stored_siscone.get();
  } else {
    siscone = &local_siscone;
  }

  if (new_siscone) {
    // transfer fastjet initial particles into the siscone type
    vector<Cmomentum> siscone_momenta(n);
    for(unsigned i = 0; i < n; i++) {
      const PseudoJet & p = clust_seq.jets()[i]; // shorthand
      siscone_momenta[i] = Cmomentum(p.px(), p.py(), p.pz(), p.E());
    }
    
    // run the jet finding
    siscone->compute_jets(siscone_momenta, cone_radius(), overlap_threshold(),
                          n_pass_max(), protojet_ptmin(), 
                          split_merge_on_transverse_mass());
  } else {
    // just run the overlap part of the jets.
    siscone->recompute_jets(overlap_threshold(), protojet_ptmin(), 
                            split_merge_on_transverse_mass());
  }

  // extract the jets [in reverse order -- to get nice ordering in pt at end]
  int njet = siscone->jets.size();

  for (int ijet = njet-1; ijet >= 0; ijet--) {
    const Cjet & jet = siscone->jets[ijet]; // shorthand
    
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

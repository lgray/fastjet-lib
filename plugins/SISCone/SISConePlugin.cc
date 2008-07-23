
// fastjet stuff
#include "fastjet/ClusterSequence.hh"
#include "fastjet/SISConePlugin.hh"

//// scones stuff
//#include "siscone/momentum.h"
//#include "siscone/siscone.h"

// other stuff
#include<sstream>

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

using namespace std;
using namespace siscone;

/// shortcut for converting siscone Cmomentum into PseudoJet
template<> PseudoJet::PseudoJet(const siscone::Cmomentum & four_vector) {
  (*this) = PseudoJet(four_vector.px,four_vector.py,four_vector.pz,
                      four_vector.E);
}


string SISConePlugin::description () const {
  ostringstream desc;
  
  const string on = "on";
  const string off = "off";

  string sm_scale_string = "split-merge uses " + 
    split_merge_scale_name(Esplit_merge_scale(split_merge_scale()));

  desc << "SISCone jet algorithm with " ;
  desc << "cone_radius = "       << cone_radius        () << ", ";
  desc << "overlap_threshold = " << overlap_threshold  () << ", ";
  desc << "n_pass_max = "        << n_pass_max         () << ", ";
  desc << "protojet_ptmin = "    << protojet_ptmin()      << ", ";
  desc <<  sm_scale_string                                << ", ";
  desc << "caching turned "      << (caching() ? on : off);
  desc << ", SM stop scale = "     << _split_merge_stopping_scale;

  // add a note to the description if we use the pt-weighted splitting
  if (_use_pt_weighted_splitting){
    desc << ", using pt-weighted splitting";
  }

  // create a fake siscone object so that we can find out more about it
  Csiscone siscone;
  if (siscone.merge_identical_protocones) {
    desc << ", and (IR unsafe) merge_indentical_protocones=true" ;
  }

  desc << ", SISCone code v" << siscone_version();

  return desc.str();
}


void SISConePlugin::set_clustering_parameters(ClusterSequence & clust_seq, Csiscone *siscone) const {
  // when running with ghosts for passive areas, do not put the
  // ghosts into the stable-cone search (not relevant)
  siscone->stable_cone_soft_pt2_cutoff = ghost_separation_scale()
                                         * ghost_separation_scale();
  // set the type of splitting we want (default=std one, true->pt-weighted split)
  siscone->set_pt_weighted_splitting(_use_pt_weighted_splitting);
}

void SISConePlugin::run_siscone_clustering(ClusterSequence & clust_seq, 
					   Csiscone *siscone,
					   vector<Cmomentum> siscone_momenta) const {
  // run the jet finding
  //cout << "plg sms: " << split_merge_scale() << endl;
  siscone->compute_jets(siscone_momenta, cone_radius(), overlap_threshold(),
			n_pass_max(), protojet_or_ghost_ptmin(), 
			Esplit_merge_scale(split_merge_scale()));
}

void SISConePlugin::rerun_siscone_clustering(ClusterSequence & clust_seq, 
					     Csiscone *siscone) const {
  // just run the overlap part of the jets.
  //cout << "plg rcmp sms: " << split_merge_scale() << endl;
  siscone->recompute_jets(overlap_threshold(), protojet_or_ghost_ptmin(), 
			  Esplit_merge_scale(split_merge_scale()));
}

void SISConePlugin::reset_stored_plugin() const{
  stored_plugin.reset( new SISConePlugin(*this));
}

FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh


// fastjet stuff
#include "fastjet/ClusterSequence.hh"
#include "fastjet/SISConeSphericalPlugin.hh"

//// scones stuff
//#include "siscone/spherical/momentum.h"
//#include "siscone/spherical/siscone.h"

// other stuff
#include<sstream>

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

using namespace std;
using namespace siscone_spherical;

/// shortcut for converting siscone CSphmomentum into PseudoJet
template<> PseudoJet::PseudoJet(const siscone_spherical::CSphmomentum & four_vector) {
  (*this) = PseudoJet(four_vector.px,four_vector.py,four_vector.pz,
                      four_vector.E);
}

string SISConeSphericalPlugin::description () const {
  ostringstream desc;
  
  const string on = "on";
  const string off = "off";

  string sm_scale_string = "split-merge uses " + 
    split_merge_scale_name(Esplit_merge_scale(split_merge_scale()));

  desc << "Spherical SISCone jet algorithm with " ;
  desc << "cone_radius = "       << cone_radius        () << ", ";
  desc << "overlap_threshold = " << overlap_threshold  () << ", ";
  desc << "n_pass_max = "        << n_pass_max         () << ", ";
  desc << "protojet_Emin = "     << protojet_Emin()      << ", ";
  desc <<  sm_scale_string                                << ", ";
  desc << "caching turned "      << (caching() ? on : off);
  desc << ", SM stop scale = "     << _split_merge_stopping_scale;

  // add a note to the description if we use the pt-weighted splitting
  if (_use_E_weighted_splitting){
    desc << ", using E-weighted splitting";
  }

  if (_use_jet_def_recombiner){
    desc << ", using jet-definition's own recombiner";
  }

  // create a fake siscone object so that we can find out more about it
  CSphsiscone siscone;
  if (siscone.merge_identical_protocones) {
    desc << ", and (IR unsafe) merge_indentical_protocones=true" ;
  }

  desc << ", SISCone code v" << siscone_version();

  return desc.str();
}


void SISConeSphericalPlugin::set_clustering_parameters(ClusterSequence & clust_seq, CSphsiscone *siscone) const {
  // when running with ghosts for passive areas, do not put the
  // ghosts into the stable-cone search (not relevant)
  siscone->stable_cone_soft_E2_cutoff = ghost_separation_scale()
                                      * ghost_separation_scale();
  // set the type of splitting we want (default=std one, true->pt-weighted split)
  siscone->set_E_weighted_splitting(_use_E_weighted_splitting);
}


void SISConeSphericalPlugin::run_siscone_clustering(ClusterSequence & clust_seq, 
						    CSphsiscone *siscone,
						    vector<CSphmomentum> siscone_momenta) const {
  // run the jet finding
  //cout << "plg sms: " << split_merge_scale() << endl;
  siscone->compute_jets(siscone_momenta, cone_radius(), overlap_threshold(),
			n_pass_max(), protojet_or_ghost_Emin(), 
			Esplit_merge_scale(split_merge_scale()));
}

void SISConeSphericalPlugin::rerun_siscone_clustering(ClusterSequence & clust_seq, 
						      CSphsiscone *siscone) const {
  // just run the overlap part of the jets.
  //cout << "plg rcmp sms: " << split_merge_scale() << endl;
  siscone->recompute_jets(overlap_threshold(), protojet_or_ghost_Emin(), 
			  Esplit_merge_scale(split_merge_scale()));
}

void SISConeSphericalPlugin::reset_stored_plugin() const{
  stored_plugin.reset( new SISConeSphericalPlugin(*this));
}


FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

#ifndef __SISCONEBASEPLUGIN_HH__
#define __SISCONEBASEPLUGIN_HH__

#include "fastjet/JetDefinition.hh"
#include "fastjet/ClusterSequence.hh" 
#include <vector>
#include <memory>
#include <cmath>

#include <sstream>

// questionable whether this should be in fastjet namespace or not...
FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

// another forward declaration to reduce includes
//class PseudoJet;

//----------------------------------------------------------------------
//
/// SISConeBasePlugin is a plugin for fastjet (v2.1 upwards) that
/// provides a base interface to SISCone-type cone jet finder by
/// Gregory Soyez and Gavin Salam.
///
/// This is a purely virtual template class that needs to be overloaded
/// for the specific implementations of SISCone (i.e. regular or
/// spherical as of July 16th 2008).
///
/// any derived plugin MUST overload the following methods:
///   description()
///   set_clustering_parameters()
///   run_siscone_clustering()
///   rerun_siscone_clustering()
///
/// For further details, see the derived plugins or
/// http://projects.hepforge.com/siscone
//
template <typename SISConeMain, typename SISConeMomentum, typename SISConeJet>
class SISConeBasePlugin : public JetDefinition::Plugin {
public:
  /// default ctor
  SISConeBasePlugin (){
  }

  /// copy constructor
  SISConeBasePlugin (const SISConeBasePlugin & plugin) {
    *this = plugin;
  }

  /// the cone radius
  double cone_radius        () const {return _cone_radius        ;}

  /// Fraction of overlap energy in a jet above which jets are merged
  /// and below which jets are split.
  double overlap_threshold  () const {return _overlap_threshold  ;}

  /// the maximum number of passes of stable-cone searching (<=0 is same
  /// as infinity).
  int n_pass_max  () const {return _n_pass_max  ;}

  /// set the "split_merge_stopping_scale": if the scale variable for
  /// all protojets is below this, then stop the split-merge procedure
  /// and keep only those jets found so far. This is useful in
  /// determination of areas of hard jets because it can be used to
  /// avoid running the split-merging on the pure ghost-part of the
  /// event.
  void set_split_merge_stopping_scale(double scale) {
    _split_merge_stopping_scale = scale;}

  /// return the value of the split_merge_stopping_scale (see
  /// set_split_merge_stopping_scale(...) for description)
  double split_merge_stopping_scale() {return _split_merge_stopping_scale;}

  /// indicates whether caching is turned on or not.
  bool caching() const {return _caching ;}

  // the things that are required by base class
  /// plugin description
  virtual std::string description () const {
    return std::string("SISCone Base Plugin: please use a derived plugin instead");
  }

  /// really do the clustering work
  virtual void run_clustering(ClusterSequence &) const;

  /// the plugin mechanism's standard way of accessing the jet radius
  virtual double R() const {return cone_radius();}

  /// return true since there is specific support for the measurement
  /// of passive areas, in the sense that areas determined from all
  /// particles below the ghost separation scale will be a passive
  /// area. 
  virtual bool supports_ghosted_passive_areas() const {
    return true;
  }
  
  /// set the ghost separation scale for passive area determinations
  /// _just_ in the next run (strictly speaking that makes the routine
  /// a non const, so related internal info must be stored as a mutable)
  virtual void set_ghost_separation_scale(double scale) const {
    _ghost_sep_scale = scale;
  }

  virtual double ghost_separation_scale() const {
    return _ghost_sep_scale;
  }

  double _cone_radius, _overlap_threshold;
  int    _n_pass_max;
  bool   _caching;//, _split_merge_on_transverse_mass;
  double _split_merge_stopping_scale;

  mutable double _ghost_sep_scale;


  // the list of internam methods that NEEDS to be updated
  /// set the parameters required before clustering that are specific to each derived plugin
  virtual void set_clustering_parameters(ClusterSequence & clust_seq, SISConeMain *siscone) const =0;

  /// call the clustering itself 
  virtual void run_siscone_clustering(ClusterSequence & clust_seq, SISConeMain *siscone,
				      std::vector<SISConeMomentum> siscone_momenta) const =0;

  /// call the re-clustering itself 
  virtual void rerun_siscone_clustering(ClusterSequence & clust_seq, SISConeMain *siscone) const =0;

  /// call the re-clustering itself 
  virtual void reset_stored_plugin() const =0;

  // variables for caching the results and the input
  static std::auto_ptr<SISConeBasePlugin<SISConeMain, SISConeMomentum, SISConeJet> > stored_plugin;
  static std::auto_ptr<std::vector<PseudoJet> > stored_particles;
  static std::auto_ptr<SISConeMain            > stored_siscone;

};


//======================================================================
/// Class that provides extra information about a SISCone clustering
template <typename Main, typename Momentum, typename Jet>
class SISConeBaseExtras : public ClusterSequence::Extras {
public:
  /// returns a reference to the vector of stable cones (aka protocones)
  const std::vector<PseudoJet> & stable_cones() const {return _protocones;}

  /// an old name for getting the vector of stable cones (aka protocones)
  const std::vector<PseudoJet> & protocones() const {return _protocones;}


  /// access to the siscone jet def plugin (more convenient than
  /// getting it from the original jet definition, because here it's
  /// directly of the right type (rather than the base type)
  const SISConeBasePlugin<Main, Momentum, Jet>* jet_def_plugin() const {return _jet_def_plugin;}

  /// return a brief summary of the contents of the extras object
  /// (specifically, the number of protocones.
  std::string description() const;

  /// return the smallest difference in squared distance encountered
  /// during splitting between a particle and two overlapping
  /// protojets.
  inline double most_ambiguous_split() const {return _most_ambiguous_split;}

private:
  std::vector<PseudoJet> _protocones;
  const SISConeBasePlugin<Main, Momentum, Jet> * _jet_def_plugin;
  double                _most_ambiguous_split;
  // let us be written to by SISConePlugin
friend class SISConeBasePlugin<Main, Momentum, Jet>;
};


/////////////////////////////////////////////
// now comes the implementation            //
/////////////////////////////////////////////
template<typename SIS, typename Mom, typename Jet> 
std::auto_ptr<SISConeBasePlugin<SIS, Mom, Jet> > SISConeBasePlugin<SIS, Mom, Jet>::stored_plugin;

template<typename SIS, typename Mom, typename Jet>
std::auto_ptr<std::vector<PseudoJet> > SISConeBasePlugin<SIS, Mom, Jet>::stored_particles;

template<typename SIS, typename Mom, typename Jet> 
std::auto_ptr<SIS> SISConeBasePlugin<SIS, Mom, Jet>::stored_siscone;


// the base class implementation
template<typename SIS, typename Mom, typename Jet>
void SISConeBasePlugin<SIS, Mom, Jet>::run_clustering(ClusterSequence & clust_seq) const {

  SIS   local_siscone;
  SIS * siscone;
  
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
      stored_siscone  .reset( new SIS );
      stored_particles.reset( new std::vector<PseudoJet>(clust_seq.jets()));
      reset_stored_plugin();
    }

    siscone = stored_siscone.get();
  } else {
    siscone = &local_siscone;
  }

  // make sure stopping scale is set in siscone
  siscone->SM_var2_hardest_cut_off = _split_merge_stopping_scale*_split_merge_stopping_scale;

  // set the specific parameters
  set_clustering_parameters(clust_seq, siscone);

  if (new_siscone) {
    // transfer fastjet initial particles into the siscone type
    std::vector<Mom> siscone_momenta(n);
    for(unsigned i = 0; i < n; i++) {
      const PseudoJet & p = clust_seq.jets()[i]; // shorthand
      siscone_momenta[i] = Mom(p.px(), p.py(), p.pz(), p.E());
    }
    
    // run the jet finding
    run_siscone_clustering(clust_seq, siscone, siscone_momenta);    
  } else {
    // rerun the jet finding
    rerun_siscone_clustering(clust_seq, siscone);    
  }


  // extract the jets [in reverse order -- to get nice ordering in pt at end]
  int njet = siscone->jets.size();

  for (int ijet = njet-1; ijet >= 0; ijet--) {
    const Jet & jet = siscone->jets[ijet]; // shorthand
    
    // Successively merge the particles that make up the cone jet
    // until we have all particles in it.  Start off with the zeroth
    // particle.
    int jet_k = jet.contents[0];
    for (unsigned ipart = 1; ipart < jet.contents.size(); ipart++) {
      // take the last result of the merge
      int jet_i = jet_k;
      // and the next element of the jet
      int jet_j = jet.contents[ipart];
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

  // now copy the list of protocones into an "extras" objects
  SISConeBaseExtras<SIS, Mom, Jet> * extras = new SISConeBaseExtras<SIS, Mom, Jet>;
  for (unsigned ipass = 0; ipass < siscone->protocones_list.size(); ipass++) {
    for (unsigned ipc = 0; ipc < siscone->protocones_list[ipass].size(); ipc++) {
      //double rap = siscone->protocones_list[ipass][ipc].eta;
      //double phi = siscone->protocones_list[ipass][ipc].phi;
      //PseudoJet protocone(cos(phi),sin(phi),sinh(rap),cosh(rap));
      PseudoJet protocone(siscone->protocones_list[ipass][ipc]);
      protocone.set_user_index(ipass);
      extras->_protocones.push_back(protocone);
    }
  }
  extras->_most_ambiguous_split = siscone->most_ambiguous_split;

  // tell it what the jet definition was
  extras->_jet_def_plugin = this;

  // give the extras object to the cluster sequence.
  clust_seq.plugin_associate_extras(std::auto_ptr<ClusterSequence::Extras>(extras));
}


/// 
template<typename SIS, typename Mom, typename Jet>
std::string SISConeBaseExtras<SIS, Mom, Jet>::description() const {
  std::ostringstream ostr;
  ostr << "This SISCone clustering found " << protocones().size() 
       << " stable protocones";
  return ostr.str();
}


FASTJET_END_NAMESPACE        // defined in fastjet/internal/base.hh

#endif // __SISCONEBASEPLUGIN_HH__


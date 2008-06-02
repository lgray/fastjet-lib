#ifndef __NESTEDALGSPLUGIN_HH__
#define __NESTEDALGSPLUGIN_HH__

#include "fastjet/JetDefinition.hh"
#include <list>
#include <memory>
#include <cmath>

// questionable whether this should be in fastjet namespace or not...
FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

// another forward declaration to reduce includes
class PseudoJet;

//----------------------------------------------------------------------
//
/// NestedAglsPlugin is a plugin for fastjet (v2.4 upwards) that, given
/// a list of jet definitions, performs the clustering by feeding the 
/// particles to the first algorithm and then, successively feeding the 
/// output to the next algorithm in the list.
//
class NestedAlgsPlugin : public JetDefinition::Plugin {
public:
  /// Main constructor for the NestedAlgs Plugin class.  
  ///
  /// The argument is an initialised list of jet algorithms
  NestedAlgsPlugin (std::list<JetDefinition> &_algs) :
    algs(_algs){}

  /// copy constructor
  NestedAlgsPlugin (const NestedAlgsPlugin & plugin) {
    *this = plugin;
  }

  // the things that are required by base class
  virtual std::string description () const;
  virtual void run_clustering(ClusterSequence &) const;
  /// the plugin mechanism's standard way of accessing the jet radius
  /// here we return the R of the last alg in the list
  virtual double R() const {return algs.rbegin()->R();}

private:
  std::list<JetDefinition> algs;
};

FASTJET_END_NAMESPACE        // defined in fastjet/internal/base.hh

#endif // __SISCONEPLUGIN_HH__


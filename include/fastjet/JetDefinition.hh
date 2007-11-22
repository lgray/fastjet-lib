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

#ifndef __FASTJET_JETDEFINITION_HH__
#define __FASTJET_JETDEFINITION_HH__

#include<cassert>
#include "fastjet/internal/numconsts.hh"
#include "fastjet/PseudoJet.hh"
#include<string>
#include<memory>

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

/// return a string containing information about the release
//  NB: (implemented in ClusterSequence.cc but defined here because
//  this is a visible location)
std::string fastjet_version_string();

//======================================================================
/// the various options for the algorithmic strategy to adopt in
/// clustering events with kt and cambridge style algorithms.
enum Strategy {
  /// fastest form about 500..10^4
  N2MinHeapTiled   = -4, 
  /// fastest from about 50..500
  N2Tiled     = -3, 
  /// legacy
  N2PoorTiled = -2, 
  /// fastest below 50
  N2Plain     = -1, 
  /// worse even than the usual N^3 algorithms
  N3Dumb      =  0, 
  /// automatic selection of the best (based on N)
  Best        =  1, 
  /// best of the NlnN variants -- best overall for N>10^4
  NlnN        =  2, 
  /// legacy N ln N using 3pi coverage of cylinder
  NlnN3pi     =  3, 
  /// legacy N ln N using 4pi coverage of cylinder
  NlnN4pi     =  4,
  /// Chan's closest pair method (in a variant with 4pi coverage),
  /// for use exclusively with the Cambridge algorithm
  NlnNCam4pi   = 14,
  NlnNCam2pi2R = 13,
  NlnNCam      = 12, // 2piMultD
  /// the plugin has been used...
  plugin_strategy = 999
};


//======================================================================
/// the various families of jet-clustering algorithm
enum JetFinder {
  /// the longitudinally invariant kt algorithm
  kt_algorithm=0,
  /// the longitudinally invariant variant of the cambridge algorithm
  /// (aka Aachen algoithm).
  cambridge_algorithm=1,
  /// like the k_t but with distance measures 
  ///       dij = min(1/kti^2,1/ktj^2) Delta R_{ij}^2 / R^2
  ///       diB = 1/kti^2
  antikt_algorithm=2, 
  /// like the k_t but with distance measures 
  ///       dij = min(kti^{2p},ktj^{2p}) Delta R_{ij}^2 / R^2
  ///       diB = 1/kti^{2p}
  genkt_algorithm=3, 
  /// a version of cambridge with a special distance measure for particles
  /// whose pt is < extra_param()
  cambridge_for_passive_algorithm=11,
  /// a version of genkt with a special distance measure for particles
  /// whose pt is < extra_param() [relevant for passive areas when p<=0]
  genkt_for_passive_algorithm=13, 
  /// any plugin algorithm supplied by the user
  plugin_algorithm = 99
};

/// provide standard Les Houches nomenclature (algorithm is general
/// recipe without the parameters)
typedef JetFinder JetAlgorithm;

/// provide other possible names for the Cambridge/Aachen algorithm?
const JetAlgorithm aachen_algorithm = cambridge_algorithm;
const JetAlgorithm cambridge_aachen_algorithm = cambridge_algorithm;

//======================================================================
/// the various recombination schemes
enum RecombinationScheme {
  /// summing the 4-momenta
  E_scheme=0,
  /// pt weighted recombination of y,phi (and summing of pt's)
  /// with preprocessing to make things massless by rescaling E=|\vec p|
  pt_scheme=1,
  /// pt^2 weighted recombination of y,phi (and summing of pt's)
  /// with preprocessing to make things massless by rescaling E=|\vec p|
  pt2_scheme=2,
  /// pt weighted recombination of y,phi (and summing of pt's)
  /// with preprocessing to make things massless by rescaling |\vec p|->=E
  Et_scheme=3,
  /// pt^2 weighted recombination of y,phi (and summing of pt's)
  /// with preprocessing to make things massless by rescaling |\vec p|->=E
  Et2_scheme=4,
  /// pt weighted recombination of y,phi (and summing of pt's), with 
  /// no preprocessing
  BIpt_scheme=5,
  /// pt^2 weighted recombination of y,phi (and summing of pt's)
  /// no preprocessing
  BIpt2_scheme=6,
  /// for the user's external scheme
  external_scheme = 99
};




// forward declaration, needed in order to specify interface for the
// plugin.
class ClusterSequence;




//======================================================================
/// class that is intended to hold a full definition of the jet
/// clusterer
class JetDefinition {
  
public:

  /// forward declaration of a class that allows the user to introduce
  /// their own plugin 
  class Plugin;

  // forward declaration of a class that will provide the
  // recombination scheme facilities and/or allow a user to
  // extend these facilities
  class Recombiner;

  /// constructor to fully specify a jet-definition (together
  /// with information about how algorithically to run it). 
  ///
  JetDefinition(JetFinder jet_finder, 
                double R, 
                Strategy strategy,
                RecombinationScheme recomb_scheme = E_scheme) :
    _jet_finder(jet_finder), _Rparam(R), _strategy(strategy) {
    // the largest sensible value for R
    assert(_Rparam <= 0.5*pi);
    assert(_jet_finder != plugin_algorithm &&
           _strategy   != plugin_strategy);
    _plugin = NULL;
    set_recombination_scheme(recomb_scheme);
  }
  

  /// constructor with alternative ordering or arguments -- note that
  /// we have not provided a default jet finder, to avoid ambiguous
  /// JetDefinition() constructor.
  JetDefinition(JetFinder jet_finder = kt_algorithm, 
                double R = 1.0, 
                RecombinationScheme recomb_scheme = E_scheme,
                Strategy strategy = Best) {
    *this = JetDefinition(jet_finder, R, strategy, recomb_scheme);
  }


  /// constructor in a form that allows the user to provide a pointer
  /// to an external recombiner class (which must remain valid for the
  /// life of the JetDefinition object).
  JetDefinition(JetFinder jet_finder, 
                double R, 
                const Recombiner * recombiner,
                Strategy strategy = Best) {
    *this = JetDefinition(jet_finder, R, strategy, external_scheme);
    _recombiner = recombiner;
  }

  /// constructor based on a pointer to a user's plugin; the object
  /// pointed to must remain valid for the whole duration of existence
  /// of the JetDefinition and any related ClusterSequences
  JetDefinition(const Plugin * plugin) {
    _plugin = plugin;
    _strategy = plugin_strategy;
    _Rparam = _plugin->R();
    _jet_finder = plugin_algorithm;
    set_recombination_scheme(E_scheme);
  }

  /// set the recombination scheme to the one provided
  void set_recombination_scheme(RecombinationScheme);

  /// set the recombiner class to the one provided
  void set_recombiner(const Recombiner * recomb) {
    _recombiner = recomb;
    _default_recombiner = DefaultRecombiner(external_scheme);
  }

  /// return a pointer to the plugin 
  const Plugin * plugin() const {return _plugin;};

  // return information about the definition...
  JetFinder jet_finder  () const {return _jet_finder  ;}
  JetAlgorithm jet_algorithm  () const {return _jet_finder  ;}
  double    R           () const {return _Rparam      ;}
  // a general purpose extra parameter, whose meaning depends on
  // the algorithm, and may often be unused.
  double    extra_param () const {return _extra_param ;}
  Strategy  strategy    () const {return _strategy    ;}
  RecombinationScheme recombination_scheme() const {
    return _default_recombiner.scheme();}

  /// (re)set the jet finder
  void set_jet_finder(JetFinder njf) {_jet_finder = njf;}
  /// (re)set the general purpose extra parameter
  void set_extra_param(double xtra_param) {_extra_param = xtra_param;}

  /// return a pointer to the currently defined recombiner (it may
  /// be the internal one)
  const Recombiner * recombiner() const {
    return _recombiner == 0 ? & _default_recombiner : _recombiner;}

  /// return a textual description of the current jet definition 
  std::string description() const;


public:
  //======================================================================
  /// An abstract base class that will provide the recombination scheme
  /// facilities and/or allow a user to extend these facilities
  class Recombiner {
  public:
    /// return a textual description of the recombination scheme
    /// implemented here
    virtual std::string description() const = 0;
    
    /// recombine pa and pb and put result into pab
    virtual void recombine(const PseudoJet & pa, const PseudoJet & pb, 
                           PseudoJet & pab) const = 0;

    /// routine called to preprocess each input jet (to make all input
    /// jets compatible with the scheme requirements (e.g. massless).
    virtual void preprocess(PseudoJet & p) const {};
    
    /// a destructor to be replaced if necessary in derived classes...
    virtual ~Recombiner() {};

    /// pa += pb in the given recombination scheme. Not virtual -- the
    /// user should have no reason to want to redefine this!
    inline void plus_equal(PseudoJet & pa, const PseudoJet & pb) const {
      // put result in a temporary location in case the recombiner
      // does something funny (ours doesn't, but who knows about the
      // user's)
      PseudoJet pres; 
      recombine(pa,pb,pres);
      pa = pres;
    }

  };
  
  
  //======================================================================
  /// A class that will provide the recombination scheme facilities and/or
  /// allow a user to extend these facilities
  class DefaultRecombiner : public Recombiner {
  public:
    DefaultRecombiner(RecombinationScheme recomb_scheme = E_scheme) : 
      _recomb_scheme(recomb_scheme) {}
    
    virtual std::string description() const;
    
    /// recombine pa and pb and put result into pab
    virtual void recombine(const PseudoJet & pa, const PseudoJet & pb, 
                           PseudoJet & pab) const;

    virtual void preprocess(PseudoJet & p) const;

    /// return the index of the recombination scheme
    RecombinationScheme scheme() const {return _recomb_scheme;}
    
  private:
    RecombinationScheme _recomb_scheme;
  };


  //======================================================================
  /// a class that allows a user to introduce their own "plugin" jet
  /// finder
  class Plugin{
  public:
    /// return a textual description of the jet-definition implemented
    /// in this plugin
    virtual std::string description() const = 0;
    
    /// given a ClusterSequence that has been filled up with initial
    /// particles, the following function should fill up the rest of the
    /// ClusterSequence, using the following member functions of
    /// ClusterSequence:
    ///   - plugin_do_ij_recombination(...)
    ///   - plugin_do_iB_recombination(...)
    virtual void run_clustering(ClusterSequence &) const = 0;
    
    virtual double R() const = 0;
    
    /// return true if there is specific support for the measurement
    /// of passive areas, in the sense that areas determined from all
    /// particles below the ghost separation scale will be a passive
    /// area. [If you don't understand this, ignore it!]
    virtual bool supports_ghosted_passive_areas() const {return false;}

    /// set the ghost separation scale for passive area determinations
    /// in future runs (strictly speaking that makes the routine
    /// a non const, so related internal info must be stored as a mutable)
    virtual void set_ghost_separation_scale(double scale) const;
    virtual double ghost_separation_scale() const {return 0.0;}

    /// a destructor to be replaced if necessary in derived classes...
    virtual ~Plugin() {};
  };

private:


  JetFinder _jet_finder;
  double    _Rparam;
  double    _extra_param ; ///< parameter whose meaning varies according to context
  Strategy  _strategy  ;

  const Plugin * _plugin;

  // when we use our own recombiner it's useful to point to it here
  // so that we don't have to worry about deleting it etc...
  DefaultRecombiner _default_recombiner;
  const Recombiner * _recombiner;

};






FASTJET_END_NAMESPACE

#endif // __FASTJET_JETDEFINITION_HH__

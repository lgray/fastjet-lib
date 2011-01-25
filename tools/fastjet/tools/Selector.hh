#ifndef __SELECTOR_HH__
#define __SELECTOR_HH__

//STARTHEADER
// $Id$
//
// Copyright (c) 2009-2010, Matteo Cacciari, Gavin Salam and Gregory Soyez
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

#include "fastjet/PseudoJet.hh"
#include "fastjet/GhostedAreaSpec.hh"  // for area support
#include <cassert>
#include <limits>
#include <cmath>

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

/// @ingroup advanced_usage
/// \class SelectorWorker
/// default selector worker is an abstract virtual base class
///
/// The Selector class is only an interface, it is the SelectorWorker
/// that really does the work. To implement various selectors, one
/// thus has to overload this class.
class SelectorWorker {
public:
  //----------------------------------------------------------
  // fundamental info
  //----------------------------------------------------------
  /// default dtor
  virtual ~SelectorWorker() {}

  /// returns a description of the worker
  virtual std::string description() const {return "missing description";}

  /// return a copy of the current object
  virtual SelectorWorker* copy(){ 
    //TODO: do we throw an error or just return NULL?
    throw Error("this worker has nothing to copy");
  }

  //----------------------------------------------------------
  // basic operations for checking what gets selected
  //----------------------------------------------------------

  /// returns true if a given object passes the selection criterium
  /// this has to be overloaded by derived workers
  virtual bool pass(const PseudoJet & jet) const = 0;

  /// For each jet that does not pass the cuts, this routine sets the 
  /// pointer to 0. 
  ///
  /// It does not assume that the PseudoJet* passed as argumetn are not NULL
  virtual void terminator(std::vector<const PseudoJet *> & jets) const {
    for (unsigned i = 0; i < jets.size(); i++) {
      if (jets[i] && !pass(*jets[i])) jets[i] = NULL;
    }
  }

  /// returns true if this can be applied jet by jet
  virtual bool applies_jet_by_jet() const {return true;}


  //----------------------------------------------------------
  // operations for relocation
  //----------------------------------------------------------

  /// returns true is the worker can be relocated
  virtual bool is_relocatable() const { return false;}

  /// performs the relocation
  virtual void relocate(const PseudoJet &centre){
    throw Error("relocate undefined for a non-relocatable selector worker");
  }


  //----------------------------------------------------------
  // operations for area and extent
  //----------------------------------------------------------

  /// returns the rapidity range for which it may return "true"
  virtual void get_rapidity_extent(double & rapmin, double & rapmax) {
    rapmax = std::numeric_limits<double>::max();
    rapmin = -rapmax; 
  }

  /// check if it has a finite area
  virtual bool has_area() const { return false;}

  /// check if it has an analytically computable area
  virtual bool has_computable_area() const { return false;}

  /// if it has a computable area, return it
  virtual double computable_area() const{
    throw Error("this selector has no computable area");
  }
};

//----------------------------------------------------------------------
/// @ingroup advanced_usage
/// \class Selector
/// class to help with jet selections
class Selector{
public:
  Selector() {}
  Selector(SelectorWorker * worker) {_worker.reset(worker);}

  /// dummy virtual dtor
  virtual ~Selector(){}

  /// return true if the jet passes the selection
  bool pass(const PseudoJet & jet) const {
    assert(_worker());
    if (!_worker->applies_jet_by_jet()) {
      throw Error("Cannot apply this selector to an individual jet");
    }
    return _worker->pass(jet);
  }

  /// an operator way of knowing whether a given jet passes the selection or not
  bool operator()(const PseudoJet & jet) const {
    return pass(jet);
  }

  /// returns true if this can be applied jet by jet
  bool applies_jet_by_jet() const {
    assert(_worker());
    return _worker->applies_jet_by_jet();
  }


  /// returns a vector with the jets that pass the selection
  std::vector<PseudoJet> operator()(const std::vector<PseudoJet> & jets) const {
    assert(_worker());
    std::vector<PseudoJet> result;
    if (_worker->applies_jet_by_jet()) {
      //if (false) {
      // for workers that apply jet by jet, this is more efficient
      for (std::vector<PseudoJet>::const_iterator jet = jets.begin(); 
           jet != jets.end(); jet++) {
        if (_worker->pass(*jet)) result.push_back(*jet);
      }
    } else {
      // for workers that can only be applied to entire vectors,
      // go through the following
      std::vector<const PseudoJet *> jetptrs(jets.size());
      for (unsigned i = 0; i < jets.size(); i++) {
        jetptrs[i] = & jets[i];
      }
      _worker->terminator(jetptrs);
      for (unsigned i = 0; i < jetptrs.size(); i++) {
        if (jetptrs[i]) result.push_back(jets[i]);
      }
    }
    return result;
  }

  const SharedPtr<SelectorWorker> & worker() const {return _worker;}

//  /// returns a vector with the jets that pass the selection
//  std::vector<SharedPtr<PseudoJet> > 
//  operator()(const std::vector<SharedPtr<PseudoJet> >& jets) const {
//    assert(_worker());
//    std::vector<SharedPtr<PseudoJet> >result;
//    for (std::vector<SharedPtr<PseudoJet> >::const_iterator jet = jets.begin(); 
//	 jet != jets.end(); jet++) {
//      if (_worker->pass(**jet)) result.push_back(*jet);
//    }
//    return result;
//  }

  /// returns the rapidity range for which it may return "true"
  virtual void get_rapidity_extent(double &rapmin, double &rapmax) const {
    assert(_worker());
    return _worker->get_rapidity_extent(rapmin, rapmax);
  }

  /// return a textual description of the selector
  virtual std::string description() const {
    assert(_worker());
    return _worker->description();
  }

  /// check if it has a finite area
  virtual bool has_area() const{
    assert(_worker());
    return _worker->has_area();
  }

  /// get the area
  ///
  /// The argument passed is the requested cell area . It will be
  /// discarded if the selector has an analytically-computable area
  virtual double area(double cell_area=gas::def_ghost_area) const{
    assert(has_area());  //< make sure area makes sense

    if (_worker->has_computable_area()) return _worker->computable_area();

    // generate a set of "ghosts"
    double rapmin, rapmax;
    get_rapidity_extent(rapmin, rapmax);
    GhostedAreaSpec ghost_spec(rapmin, rapmax, 1, cell_area);
    std::vector<PseudoJet> ghosts;
    ghost_spec.add_ghosts(ghosts);

    // check what passes the selection
    // unsigned int npass= 0;
    // for (std::vector<PseudoJet>::const_iterator jet = ghosts.begin(); jet != ghosts.end(); jet++)
    //   if (_worker->geometric_pass(*jet)) npass++;
    return ghost_spec.ghost_area() * ((*this)(ghosts)).size();
  }

  //----------------------------------------------------
  // non-const operations
  //----------------------------------------------------

protected:
  /// Helper for copying selector workers if needed
  ///
  /// The following is needed if we want to modify a selectors that
  /// shares a worker with another selector. In that case, we need to
  /// get another copy of the worker to avoid interferences
  ///
  /// Note that any non-const operation has to call this to behave
  /// correctly w.r.t shared workers!
  void _copy_worker_if_needed(){
    // do nothing if there's a sinlge user of the worker
    if (_worker.unique()) return;

    // call the worker's copy
    //std::cout << "will make a copy of " << description() << std::endl;
    _worker.reset(_worker->copy());
  }
public:

  /// returns true if this can be applied jet by jet
  bool is_relocatable() const {
    assert(_worker());
    return _worker->is_relocatable();
  }

  /// relocate the selector on a given PseudoJet
  void relocate(const PseudoJet &centre){
    assert(_worker());

    // if the worker is not relocatable, do nothing 
    if (! _worker->is_relocatable()){
      return;
    }
    
    // since this is a non-const operation, make sure we have a
    // correct behaviour with respect to shared workers
    _copy_worker_if_needed();

    _worker->relocate(centre);
  }

private:
  SharedPtr<SelectorWorker> _worker; ///< the underlying worker
};


//----------------------------------------------------------------------
// a list of specific selectors
//----------------------------------------------------------------------


// logical operations
//----------------------------------------------------------------------

/// logical not applied on a selector
///
/// This will keep objects that do not pass the 's' selector
Selector operator!(const Selector & s);

/// logical and between two selectors
///
/// this will keep the objects that are selected by both s1 and s2
/// 
/// watch out: for both s1 and s2, the selection is applied on the
///   original list of objects. For successive applications of two
///   selectors (convolution/multiplication) see the operator *
Selector operator&&(const Selector & s1, const Selector & s2);

/// logical or between two selectors
///
/// this will keep the objects that are selected by s1 or s2
Selector operator ||(const Selector & s1, const Selector & s2);

/// successive application of 2 selectors
///
/// Apply the selector s2, then the selector s1.
///
/// watch out: the operator * acts like an operator product i.e. does
///   not commute. The order of its arguments is therefore important.
///   Whenever they commute (in particluar, when they apply jet by
///   jet), this would have the same effect as the logical &&.
Selector operator*(const Selector & s1, const Selector & s2);


// selection with kinematic cuts
//----------------------------------------------------------------------

Selector SelectorPtMin(double ptmin);                    ///< select objects with pt >= ptmin
Selector SelectorPtMax(double ptmax);                    ///< select objects with pt <= ptmax
Selector SelectorPtRange(double ptmin, double ptmax);    ///< select objects with ptmin <= pt <= ptmax

Selector SelectorEtMin(double Etmin);                    ///< select objects with Et >= Etmin
Selector SelectorEtMax(double Etmax);                    ///< select objects with Et <= Etmax
Selector SelectorEtRange(double Etmin, double Etmax);    ///< select objects with Etmin <= Et <= Etmax

Selector SelectorEMin(double Emin);                      ///< select objects with E >= Emin
Selector SelectorEMax(double Emax);                      ///< select objects with E <= Emax
Selector SelectorERange(double Emin, double Emax);       ///< select objects with Emin <= E <= Emax

Selector SelectorMMin(double Mmin);                      ///< select objects with M >= Mmin
Selector SelectorMMax(double Mmax);                      ///< select objects with M <= Mmax
Selector SelectorMRange(double Mmin, double Mmax);       ///< select objects with Mmin <= M <= Mmax

Selector SelectorRapMin(double rapmin);                  ///< select objects with rap >= rapmin
Selector SelectorRapMax(double rapmax);                  ///< select objects with rap <= rapmax
Selector SelectorRapRange(double rapmin, double rapmax); ///< select objects with rapmin <= rap <= rapmax

Selector SelectorAbsRapMin(double absrapmin);                     ///< select objects with |rap| >= absrapmin
Selector SelectorAbsRapMax(double absrapmax);                     ///< select objects with |rap| <= absrapmax
Selector SelectorAbsRapRange(double absrapmin, double absrapmax); ///< select objects with absrapmin <= |rap| <= absrapmax

Selector SelectorEtaMin(double etamin);                  ///< select objects with eta >= etamin
Selector SelectorEtaMax(double etamax);                  ///< select objects with eta <= etamax
Selector SelectorEtaRange(double etamin, double etamax); ///< select objects with etamin <= eta <= etamax

Selector SelectorAbsEtaMin(double absetamin);                     ///< select objects with |eta| >= absetamin
Selector SelectorAbsEtaMax(double absetamax);                     ///< select objects with |eta| <= absetamax
Selector SelectorAbsEtaRange(double absetamin, double absetamax); ///< select objects with absetamin <= |eta| <= absetamax

Selector SelectorPhiRange(double phimin, double phimax); ///< select objects with phimin <= phi <= phimax

Selector SelectorNHardest(unsigned int n);   ///< select the n hardest objects 


// selection with geometric objects
//----------------------------------------------------------------------

Selector SelectorCircle(const double & radius); ///< select on objets within a distance 'radius' of a variable location
Selector SelectorDoughnut(const double & radius_in, const double & radius_out); ///< select on objets with distance from the centre is between 'radius_in' and 'radius_out' 
Selector SelectorStrip(const double & radius); ///< select on objets within a distance 'radius' of a variable location


FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

#endif // __SELECTOR_HH__


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


#ifndef __FASTJET_PSEUDOJET_HH__
#define __FASTJET_PSEUDOJET_HH__

#include<valarray>
#include<vector>
#include<cassert>
#include<cmath>
#include<iostream>
#include "fastjet/internal/numconsts.hh"
#include "fastjet/internal/IsBase.hh"
#include "fastjet/internal/DerivedPseudoJetHelper.hh"
#include "fastjet/SharedPtr.hh"
#include "fastjet/Error.hh"
#include "fastjet/ClusterSequenceInterfaceBase.hh"

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

//using namespace std;

/// Used to protect against parton-level events where pt can be zero
/// for some partons, giving rapidity=infinity. KtJet fails in those cases.
const double MaxRap = 1e5;

/// default value for phi, meaning it (and rapidity) have yet to be calculated) 
const double pseudojet_invalid_phi = -100.0;

// forward definition
class ClusterSequenceAreaBase;

/// @ingroup basic_classes
/// \class PseudoJet
/// Class to contain pseudojets, including minimal information of use to
/// to jet-clustering routines.
class PseudoJet {

 public:
  //----------------------------------------------------------------------
  /// @name Constructors and destructor
  //\{
  /// default constructor leaves PseudoJet unusable
  PseudoJet() {};
  /// construct a pseudojet from explicit components
  PseudoJet(const double px, const double py, const double pz, const double E);
  /// constructor from any object that has px,py,pz,E = some_four_vector[0--3],
  template <class L> PseudoJet(const L & some_four_vector) ;

  /// default (virtual) destructor
  virtual ~PseudoJet(){};
  //\} ---- end of constructors and destructors --------------------------

  //----------------------------------------------------------------------
  /// @name Kinematic access functions
  //\{
  //----------------------------------------------------------------------
  inline double E()   const {return _E;}
  inline double e()   const {return _E;} // like CLHEP
  inline double px()  const {return _px;}
  inline double py()  const {return _py;}
  inline double pz()  const {return _pz;}

  /// returns phi (in the range 0..2pi)
  inline double phi() const {return phi_02pi();}

  /// returns phi in the range -pi..pi
  inline double phi_std()  const {
    _ensure_valid_rap_phi();
    return _phi > pi ? _phi-twopi : _phi;}

  /// returns phi in the range 0..2pi
  inline double phi_02pi() const {
    _ensure_valid_rap_phi();
    return _phi;
  }

  /// returns the rapidity or some large value when the rapidity
  /// is infinite
  inline double rap() const {
    _ensure_valid_rap_phi();
    return _rap;
  }

  /// the same as rap()
  inline double rapidity() const {return rap();} // like CLHEP

  /// returns the pseudo-rapidity or some large value when the
  /// rapidity is infinite
  double pseudorapidity() const;
  double eta() const {return pseudorapidity();}

  /// returns the squared transverse momentum
  inline double kt2() const {return _kt2;}
  /// returns the squared transverse momentum
  inline double perp2() const {return _kt2;}  // like CLHEP
  /// returns the scalar transverse momentum
  inline double  perp() const {return sqrt(_kt2);}    // like CLHEP
  /// returns the squared invariant mass // like CLHEP
  inline double  m2() const {return (_E+_pz)*(_E-_pz)-_kt2;}    
  /// returns the squared transverse mass = kt^2+m^2
  inline double mperp2() const {return (_E+_pz)*(_E-_pz);}
  /// returns the transverse mass = sqrt(kt^2+m^2)
  inline double mperp() const {return sqrt(std::abs(mperp2()));}
  /// returns the invariant mass 
  /// (If m2() is negative then -sqrt(-m2()) is returned, as in CLHEP)
  inline double  m() const;    
  /// return px^2+py^2+pz^2
  inline double modp2() const {return _kt2+_pz*_pz;}
  /// return the transverse energy
  inline double Et() const {return (_kt2==0) ? 0.0 : _E/sqrt(1.0+_pz*_pz/_kt2);}
  /// return the transverse energy squared
  inline double Et2() const {return (_kt2==0) ? 0.0 : _E*_E/(1.0+_pz*_pz/_kt2);}

  /// returns component i, where X==0, Y==1, Z==2, E==3
  double operator () (int i) const ; 
  /// returns component i, where X==0, Y==1, Z==2, E==3
  inline double operator [] (int i) const { return (*this)(i); }; // this too



  /// returns kt distance (R=1) between this jet and another
  double kt_distance(const PseudoJet & other) const;

  /// returns squared cylinder (rap-phi) distance between this jet and another
  double plain_distance(const PseudoJet & other) const;
  /// returns squared cylinder (rap-phi) distance between this jet and
  /// another
  inline double squared_distance(const PseudoJet & other) const {
    return plain_distance(other);}

  /// returns other.phi() - this.phi(), constrained to be in 
  /// range -pi .. pi
  double delta_phi_to(const PseudoJet & other) const;

  //// this seemed to compile except if it was used
  //friend inline double 
  //  kt_distance(const PseudoJet & jet1, const PseudoJet & jet2) { 
  //                                      return jet1.kt_distance(jet2);}

  /// returns distance between this jet and the beam
  inline double beam_distance() const {return _kt2;}

  /// return a valarray containing the four-momentum (components 0-2
  /// are 3-mom, component 3 is energy).
  std::valarray<double> four_mom() const;

  //\}  ------- end of kinematic access functions

  // taken from CLHEP
  enum { X=0, Y=1, Z=2, T=3, NUM_COORDINATES=4, SIZE=NUM_COORDINATES };


  //----------------------------------------------------------------------
  /// @name Kinematic modification functions
  //\{
  //----------------------------------------------------------------------
  /// transform this jet (given in the rest frame of prest) into a jet
  /// in the lab frame [NOT FULLY TESTED]
  PseudoJet & boost(const PseudoJet & prest);
  /// transform this jet (given in lab) into a jet in the rest
  /// frame of prest  [NOT FULLY TESTED]
  PseudoJet & unboost(const PseudoJet & prest);

  void operator*=(double);
  void operator/=(double);
  void operator+=(const PseudoJet &);
  void operator-=(const PseudoJet &);

  /// reset the 4-momentum according to the supplied components and
  /// put the user and history indices back to their default values
  inline void reset(double px, double py, double pz, double E);
  
  /// reset the PseudoJet to be equal to psjet (including its
  /// indices); NB if the argument is derived from a PseudoJet then
  /// the "reset" used will be the templated version (which does not
  /// know about indices...)
  inline void reset(const PseudoJet & psjet) {
    (*this) = psjet;
  }

  /// reset the 4-momentum according to the supplied generic 4-vector
  /// (accessible via indexing, [0]==px,...[3]==E) and put the user
  /// and history indices back to their default values.
  template <class L> inline void reset(const L & some_four_vector) {
    reset(some_four_vector[0], some_four_vector[1],
          some_four_vector[2], some_four_vector[3]);
  }

  //\} --- end of kin mod functions ------------------------------------

  //----------------------------------------------------------------------
  /// @name User index functions
  ///
  /// To allow the user to set and access an integer index which can
  /// be exploited by the user to associate extra information with a
  /// particle/jet (for example pdg id, or an indication of a
  /// particle's origin within the user's analysis)
  //
  //\{

  /// return the user_index, 
  inline int user_index() const {return _user_index;}
  /// set the user_index, intended to allow the user to add simple
  /// identifying information to a particle/jet
  inline void set_user_index(const int index) {_user_index = index;}

  //\} ----- end of use index functions ---------------------------------

  //----------------------------------------------------------------------
  /// @name Extra information types and functions
  ///
  /// Allows PseudoJet to carry extra info (as an object derived from
  /// ExtraInfo).
  /// See also the PseudoJetPlusInfo<TExtraInfo> class that should
  /// make this easier.
  //\{

  /// @ingroup extra_info
  /// \class ExtraInfo
  /// a base class to hold extra information in PseudoJet
  ///
  /// This is a dummy class to hold extra information. The motivation
  /// behind its existence is a safety procedure: we could symply hold
  /// a generic pointer but this allows for clean destruction when
  /// memory is released and this allows consistency checks at the
  /// level of the end-user by using dynamic_cast instead of a
  /// brute-force cast.
  class ExtraInfo{
  public:
    // dummy ctor
    ExtraInfo(){};

    // dummy virtual dtor
    // makes it polymorphic to allow for dynamic_cast
    virtual ~ExtraInfo(){}; 
  };

  /// error class to be thrown if accessing extra info when it doesn't
  /// exist
  class InexistentExtraInfo : public Error {
  public:
    InexistentExtraInfo();
  };

  /// retrieve a pointer to the extra information
  const ExtraInfo* extra_info() const{
    if (!_extra_info()) return NULL;
    return _extra_info.get();
  }

  /// sets the internal shared pointer to the extra information.
  ///
  /// Note that the PseudoJet will now _own_ the pointer, and delete
  /// the corresponding object when it (and anything copied from it)
  /// goes out of scope. If that behaviour does not fit your needs,
  /// access the shared pointer directly through the
  /// extra_info_shared() function.
  void set_extra_info(ExtraInfo * extra_info_in) {
    _extra_info.reset(extra_info_in);
  }

  /// returns a reference to the dynamic cast conversion of extra_info
  /// to type L.
  ///
  /// throws an InexistentExtraInfo() error if there is no extra info;
  /// throws a std::bad_cast if the conversion doesn't work
  template<class L>
  const L & extra_info_cast() const{
    if (_extra_info.get() == 0) throw InexistentExtraInfo();
    return dynamic_cast<const L &>(* _extra_info.get());
  }

  /// retrieve a shared pointer to the extra information
  SharedPtr<ExtraInfo> & extra_info_shared(){
    return _extra_info;
  }

  /// retrieve a shared pointer to the extra information
  const SharedPtr<ExtraInfo> & extra_info_shared() const{
    return _extra_info;
  }

  // \} --- end of extra info functions ---------------------------------

  //----------------------------------------------------------------------
  /// @name Description
  ///
  /// Since a PseudoJet can have an interface that contains a variety
  /// of information, we provide a description that allows one to check
  /// exactly what kind of PseudoJet we are dealing with
  //
  //\{

  /// return a string describing what kind of PseudoJet we are dealing with 
  std::string description() const;

  //\} ----- end of description functions ---------------------------------

  //-------------------------------------------------------------
  /// @name Access to the associated ClusterSequence object.
  ///
  /// In addition to having kinematic information, jets may contain a
  /// reference to an associated ClusterSequence (this is the case,
  /// for example, if the jet has been returned by a ClusterSequence
  /// member function).
  //\{
  //-------------------------------------------------------------
  /// returns true if this PseudoJet has an associated (and still
  /// valid) ClusterSequence.
  bool has_associated_cluster_sequence() const;

  /// get a (const) pointer to the parent ClusterSequence (NULL if
  /// inexistent)
  const ClusterSequence* associated_cluster_sequence() const;
  //\}

  /// set the associated csw
  void set_associated_csi(const SharedPtr<ClusterSequenceInterfaceBase> &csi){
    _associated_csi = csi;
  }

  /// return a copy of the ClusterSequenceInterface
  const SharedPtr<ClusterSequenceInterfaceBase> & associated_csi() const {
    return _associated_csi;
  }
  
  /// if the jet has a valid associated cluster sequence then return a
  /// pointer to it; otherwise throw an error
  const ClusterSequence * validated_cs() const;

  /// if the jet has a valid associated cluster sequence interface
  /// then return a pointer to it; otherwise throw an error
  const SharedPtr<ClusterSequenceInterfaceBase> validated_csi() const;

  /// if the jet has valid area information then return a pointer to
  /// the associated ClusterSequenceAreaBase object; otherwise throw an error
  const ClusterSequenceAreaBase * validated_csab() const;

  //-------------------------------------------------------------
  /// @name Methods for access to information about jet structure
  ///
  /// These allow access to jet constituents, and other jet
  /// subtructure information. They only work if the jet is associated
  /// with a ClusterSequence.
  //-------------------------------------------------------------
  //\{

  /// check if it has been recombined with another PseudoJet in which
  /// case, return its partner through the argument. Otherwise,
  /// 'partner' is set to 0.
  ///
  /// an Error is thrown if this PseudoJet has no currently valid
  /// associated ClusterSequence
  virtual bool has_partner(PseudoJet &partner) const;

  /// check if it has been recombined with another PseudoJet in which
  /// case, return its child through the argument. Otherwise, 'child'
  /// is set to 0.
  /// 
  /// an Error is thrown if this PseudoJet has no currently valid
  /// associated ClusterSequence
  virtual bool has_child(PseudoJet &child) const;

  /// check if it is the product of a recombination, in which case
  /// return the 2 parents through the 'parent1' and 'parent2'
  /// arguments. Otherwise, set these to 0.
  ///
  /// an Error is thrown if this PseudoJet has no currently valid
  /// associated ClusterSequence
  virtual bool has_parents(PseudoJet &parent1, PseudoJet &parent2) const;

  /// check if the current PseudoJet contains the one passed as
  /// argument.
  ///
  /// an Error is thrown if this PseudoJet has no currently valid
  /// associated ClusterSequence
  virtual bool contains(const PseudoJet &constituent) const;

  /// check if the current PseudoJet is contained the one passed as
  /// argument.
  ///
  /// an Error is thrown if this PseudoJet has no currently valid
  /// associated ClusterSequence
  virtual bool is_inside(const PseudoJet &jet) const;

  /// retrieve the constituents. 
  ///
  /// an Error is thrown if this PseudoJet has no currently valid
  /// associated ClusterSequence
  virtual std::vector<PseudoJet> constituents() const;

  /// return a vector of all subjets of the current jet (in the sense
  /// of the exclusive algorithm) that would be obtained when running
  /// the algorithm with the given dcut. 
  ///
  /// Time taken is O(m ln m), where m is the number of subjets that
  /// are found. If m gets to be of order of the total number of
  /// constituents in the jet, this could be substantially slower than
  /// just getting that list of constituents.
  ///
  /// an Error is thrown if this PseudoJet has no currently valid
  /// associated ClusterSequence
  std::vector<PseudoJet> exclusive_subjets (const double & dcut) const;

  /// return the size of exclusive_subjets(...); still n ln n with same
  /// coefficient, but marginally more efficient than manually taking
  /// exclusive_subjets.size()
  ///
  /// an Error is thrown if this PseudoJet has no currently valid
  /// associated ClusterSequence
  int n_exclusive_subjets(const double & dcut) const;

  /// return the list of subjets obtained by unclustering the supplied
  /// jet down to n subjets (or all constituents if there are fewer
  /// than n).
  ///
  /// requires n ln n time
  ///
  /// an Error is thrown if this PseudoJet has no currently valid
  /// associated ClusterSequence
  std::vector<PseudoJet> exclusive_subjets (int nsub) const;

  /// return the dij that was present in the merging nsub+1 -> nsub 
  /// subjets inside this jet.
  ///
  /// an Error is thrown if this PseudoJet has no currently valid
  /// associated ClusterSequence
  double exclusive_subdmerge(int nsub) const;

  /// return the maximum dij that occurred in the whole event at the
  /// stage that the nsub+1 -> nsub merge of subjets occurred inside 
  /// this jet.
  ///
  /// an Error is thrown if this PseudoJet has no currently valid
  /// associated ClusterSequence
  double exclusive_subdmerge_max(int nsub) const;


  // the following ones require a computation of the area in the
  // parent ClusterSequence (See ClusterSequenceAreaBase for details)
  //------------------------------------------------------------------

  /// check if it has a defined area
  virtual bool has_area() const;

  /// return the jet (scalar) area.
  /// throws an Error if there is no support for area in the parent CS
  virtual double area() const;

  /// return the error (uncertainty) associated with the determination
  /// of the area of this jet.
  /// throws an Error if there is no support for area in the parent CS
  virtual double area_error() const;

  /// return the jet 4-vector area.
  /// throws an Error if there is no support for area in the parent CS
  virtual PseudoJet area_4vector() const;

  /// true if this jet is made exclusively of ghosts.
  /// throws an Error if there is no support for area in the parent CS
  virtual bool is_pure_ghost() const;

  //\} --- end of jet structure -------------------------------------



  //----------------------------------------------------------------------
  /// @name Members mainly intended for internal use
  //----------------------------------------------------------------------
  //\{
  /// return the cluster_hist_index, intended to be used by clustering
  /// routines.
  inline int cluster_hist_index() const {return _cluster_hist_index;}
  /// set the cluster_hist_index, intended to be used by clustering routines.
  inline void set_cluster_hist_index(const int index) {_cluster_hist_index = index;}

  /// alternative name for cluster_hist_index() [perhaps more meaningful]
  inline int cluster_sequence_history_index() const {
    return cluster_hist_index();}
  /// alternative name for set_cluster_hist_index(...) [perhaps more
  /// meaningful]
  inline void set_cluster_sequence_history_index(const int index) {
    set_cluster_hist_index(index);}

  //\} ---- end of internal use functions ---------------------------

 protected:  

  SharedPtr<ClusterSequenceInterfaceBase> _associated_csi;
  SharedPtr<ExtraInfo> _extra_info;


 private: 
  // NB: following order must be kept for things to behave sensibly...
  double _px,_py,_pz,_E;
  mutable double _phi, _rap;
  double _kt2; 
  int    _cluster_hist_index, _user_index;

  /// calculate phi, rap, kt2 based on the 4-momentum components
  void _finish_init();
  /// set the indices to default values
  void _reset_indices();

  /// ensure that the internal values for rapidity and phi 
  /// correspond to 4-momentum structure
  inline void _ensure_valid_rap_phi() const {
    if (_phi == pseudojet_invalid_phi) _set_rap_phi();
  }

  /// set cached rapidity and phi values
  void _set_rap_phi() const;
};


//----------------------------------------------------------------------
// routines for basic binary operations

PseudoJet operator+(const PseudoJet &, const PseudoJet &);
PseudoJet operator-(const PseudoJet &, const PseudoJet &);
PseudoJet operator*(double, const PseudoJet &);
PseudoJet operator*(const PseudoJet &, double);
PseudoJet operator/(const PseudoJet &, double);

inline double dot_product(const PseudoJet & a, const PseudoJet & b) {
  return a.E()*b.E() - a.px()*b.px() - a.py()*b.py() - a.pz()*b.pz();
}

/// returns true if the momenta of the two input jets are identical
bool have_same_momentum(const PseudoJet &, const PseudoJet &);

/// return a pseudojet with the given pt, y, phi and mass
PseudoJet PtYPhiM(double pt, double y, double phi, double m = 0.0);

//----------------------------------------------------------------------
// Routines to do with providing sorted arrays of vectors.

/// return a vector of jets sorted into decreasing transverse momentum
std::vector<PseudoJet> sorted_by_pt(const std::vector<PseudoJet> & jets);

/// return a vector of jets sorted into increasing rapidity
std::vector<PseudoJet> sorted_by_rapidity(const std::vector<PseudoJet> & jets);

/// return a vector of jets sorted into decreasing energy
std::vector<PseudoJet> sorted_by_E(const std::vector<PseudoJet> & jets);

/// return a vector of jets sorted into increasing pz
std::vector<PseudoJet> sorted_by_pz(const std::vector<PseudoJet> & jets);

//----------------------------------------------------------------------
// some code to help sorting

/// sort the indices so that values[indices[0->n-1]] is sorted
/// into increasing order 
void sort_indices(std::vector<int> & indices, 
		  const std::vector<double> & values);

/// given a vector of values with a one-to-one correspondence with the
/// vector of objects, sort objects into an order such that the
/// associated values would be in increasing order (but don't actually
/// touch the values vector in the process).
template<class T> std::vector<T> objects_sorted_by_values(const std::vector<T> & objects, 
					      const std::vector<double> & values);

/// \if internal_doc
/// @ingroup internal
/// \class IndexedSortHelper
/// a class that helps us carry out indexed sorting.
/// \endif
class IndexedSortHelper {
public:
  inline IndexedSortHelper (const std::vector<double> * reference_values) {
    _ref_values = reference_values;
  };
  inline int operator() (const int & i1, const int & i2) const {
    return  (*_ref_values)[i1] < (*_ref_values)[i2];
  };
private:
  const std::vector<double> * _ref_values;
};


//----------------------------------------------------------------------
/// constructor from any object that has px,py,pz,E = some_four_vector[0--3],
// NB: do not know if it really needs to be inline, but when it wasn't
//     linking failed with g++ (who knows what was wrong...)
template <class L> inline  PseudoJet::PseudoJet(const L & some_four_vector) {
  // now check whether L is simply a class that implements
  // some_fuor_vector[0--3] or actually is derived from PseudoJet and
  // has extra information
  DerivedPseudoJetHelper<L, IsBaseAndDerived<PseudoJet,L>::value> dpj_helper(some_four_vector);

  if (dpj_helper() != NULL){
    const PseudoJet *pj = dpj_helper();
    reset(*pj);
  } else {
    reset(some_four_vector);
  }
}


//----------------------------------------------------------------------
inline void PseudoJet::_reset_indices() { 
  set_cluster_hist_index(-1);
  set_user_index(-1);
  _associated_csi.reset();
}

//----------------------------------------------------------------------
/// specialization of the "reset" template for case where something
/// is reset to a pseudojet -- it then takes the user and history
/// indices from the psjet
// template<> inline void PseudoJet::reset<PseudoJet>(const PseudoJet & psjet) {
//   (*this) = psjet;
// }

////// fun and games...
////template<class L> class FJVector : public L {
//////  /** Default Constructor: create jet with no constituents */
//////  Vector<L>();
////
////};
////

// taken literally from CLHEP
inline double PseudoJet::m() const {
  double mm = m2();
  return mm < 0.0 ? -std::sqrt(-mm) : std::sqrt(mm);
}


inline void PseudoJet::reset(double px, double py, double pz, double E) {
  _px = px;
  _py = py;
  _pz = pz;
  _E  = E;
  _finish_init();
  _reset_indices();
}


FASTJET_END_NAMESPACE

#endif // __FASTJET_PSEUDOJET_HH__

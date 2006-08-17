
#ifndef __FASTJET_CLUSTERSEQUENCEWITHAREA_HH__
#define __FASTJET_CLUSTERSEQUENCEWITHAREA_HH__

#include "fastjet/ClusterSequence.hh"

FASTJET_BEGIN_NAMESPACE

/// base class that sets interface for extensions of ClusterSequence
/// that provide information about the area of each jet; 
///
/// the virtual functions here all return 0, since no area determination
/// is implemented.
class ClusterSequenceWithArea : public ClusterSequence {
public:
  
  /// a constructor which just carries out the construction of the
  /// parent class
  template<class L> ClusterSequenceWithArea
         (const std::vector<L> & pseudojets, 
	  const JetDefinition & jet_def,
	  const bool & writeout_combinations = false) :
     ClusterSequence(pseudojets, jet_def, writeout_combinations) {};


  /// default constructor
  ClusterSequenceWithArea() {};


  /// destructor
  virtual ~ClusterSequenceWithArea() {};


  /// return the area associated with the given jet; this base class
  /// returns 0.
  virtual double area       (const PseudoJet & jet) const {return 0.0;}


  /// return the error (uncertainty) associated with the determination
  /// of the area of this jet; this base class returns 0.
  virtual double area_error (const PseudoJet & jet) const {return 0.0;}


  /// return a PseudoJet whose 4-vector is defined by the following integral
  ///
  ///       \int drap d\phi PseudoJet("rap,phi,pt=one") *
  ///                           * Theta("rap,phi inside jet boundary")
  ///
  /// where PseudoJet("rap,phi,pt=one") is a 4-vector with the given
  /// rapdity (rap), azimuth (phi) and pt=1, while Theta("rap,phi
  /// inside jet boundary") is a function that is 1 when rap,phi
  /// define a direction inside the jet boundary and 0 otherwise.
  ///
  /// This base class returns a null 4-vector.
  virtual PseudoJet area_4vector(const PseudoJet & jet) const {
    return PseudoJet(0.0,0.0,0.0,0.0);}

};



FASTJET_END_NAMESPACE

#endif // __FASTJET_CLUSTERSEQUENCEWITHAREA_HH__

#ifndef __FJCLUSTERSEQUENCEWITHPASSIVEAREA__
#define __FJCLUSTERSEQUENCEWITHPASSIVEAREA__

#include "FjPseudoJet.hh"
#include "FjClusterSequence.hh"
#include<memory>
#include<vector>

class FjClusterSequenceWithPassiveArea : public FjClusterSequence {
public:
  template<class L> FjClusterSequenceWithPassiveArea
         (const std::vector<L> & pseudojets, 
	  const FjJetDefinition & jet_def,
	  double effective_Rfact = 1.0,
	  const bool & writeout_combinations = false);
  
  ~FjClusterSequenceWithPassiveArea();

  /// return the area associated with the given jet
  inline double area(const FjPseudoJet & jet) const {
    return _passive_area[jet.cluster_hist_index()];};

  /// return the error of the area associated with the given jet
  /// (0 by definition for a passive area)
  inline double area_err(const FjPseudoJet & jet) const {
    return 0.0;};

  /// passive area calculator -- to be defined in the .cc file (it will do
  /// the true hard work)
  class PassiveAreaCalc; 
  
private:
  

  void _initializePA();

  std::vector<double>   _passive_area;
  //std::auto_ptr<PassiveAreaCalc> _pa_calc;
  PassiveAreaCalc * _pa_calc;
  double _effective_Rfact;
};


//----------------------------------------------------------------------
///
template<class L> FjClusterSequenceWithPassiveArea::FjClusterSequenceWithPassiveArea
(const std::vector<L> & pseudojets, 
 const FjJetDefinition & jet_def,
 double effective_Rfact,
 const bool & writeout_combinations) :
  FjClusterSequence(pseudojets, jet_def, writeout_combinations),
  _effective_Rfact(effective_Rfact) {

  // the jet clustering's already been done, now worry about areas...
  _initializePA();
}


#endif // __FJCLUSTERSEQUENCEWITHPASSIVEAREA__

#ifndef __FASTJET_CLUSTERSEQUENCEPASSIVEAREA_HH__
#define __FASTJET_CLUSTERSEQUENCEPASSIVEAREA_HH__

#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequenceWithArea.hh"
#include<memory>
#include<vector>

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

class ClusterSequencePassiveArea : public ClusterSequenceWithArea {
public:
  template<class L> ClusterSequencePassiveArea
         (const std::vector<L> & pseudojets, 
	  const JetDefinition & jet_def,
	  double effective_Rfact = 1.0,
	  const bool & writeout_combinations = false);
  
  ~ClusterSequencePassiveArea();

  /// return the area associated with the given jet
  virtual inline double area(const PseudoJet & jet) const {
    return _passive_area[jet.cluster_hist_index()];};

  /// return the error of the area associated with the given jet
  /// (0 by definition for a passive area)
  virtual inline double area_error(const PseudoJet & jet) const {
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
template<class L> ClusterSequencePassiveArea::ClusterSequencePassiveArea
(const std::vector<L> & pseudojets, 
 const JetDefinition & jet_def,
 double effective_Rfact,
 const bool & writeout_combinations) :
  //ClusterSequence(pseudojets, jet_def, writeout_combinations),
  _effective_Rfact(effective_Rfact) {

  // transfer the initial jets (type L) into our own array
  _transfer_input_jets(pseudojets);

  // run the clustering
  _initialise_and_run(jet_def,writeout_combinations);

  // the jet clustering's already been done, now worry about areas...
  _initializePA();
}



FASTJET_END_NAMESPACE

#endif // __FASTJET_CLUSTERSEQUENCEPASSIVEAREA_HH__



#ifndef __FASTJET_CLUSTERSEQUENCEVORONOIAREA_HH__
#define __FASTJET_CLUSTERSEQUENCEVORONOIAREA_HH__

#include "fastjet/PseudoJet.hh"
#include "fastjet/AreaDefinition.hh"
#include "fastjet/ClusterSequenceAreaBase.hh"
#include <memory>
#include <vector>

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

/**
 * \class ClusterSequenceVoronoiArea
 * Handle the computation of Voronoi jet area.
 */
class ClusterSequenceVoronoiArea : public ClusterSequenceAreaBase {
public:
  /// template ctor
  /// \param pseudojet              list of jets (template type)
  /// \param jet_def                jet definition
  /// \param effective_Rfact        effective radius
  /// \param writeout_combinations  ??????
  template<class L> ClusterSequenceVoronoiArea
         (const std::vector<L> & pseudojets, 
	  const JetDefinition & jet_def,
	  const VoronoiAreaSpec & spec = VoronoiAreaSpec(),
	  const bool & writeout_combinations = false);
  
  /// default dtor
  ~ClusterSequenceVoronoiArea();

  /// return the area associated with the given jet
  virtual inline double area(const PseudoJet & jet) const {
    return _voronoi_area[jet.cluster_hist_index()];};

  /// return the error of the area associated with the given jet
  /// (0 by definition for a voronoi area)
  /// true for Voronoi area ?????????
  virtual inline double area_error(const PseudoJet & jet) const {
    return 0.0;};

  /// passive area calculator -- to be defined in the .cc file (it will do
  /// the true hard work)
  class VoronoiAreaCalc; 
  

private:  
  /// initialisation of the Voronoi Area
  void _initializeVA();

  std::vector<double> _voronoi_area;  ///< vector containing the result
  VoronoiAreaCalc *_pa_calc;          ///< area calculator
  double _effective_Rfact;            ///< effective radius
};




/// template constructor need to be specified in the header!
//----------------------------------------------------------------------
template<class L> ClusterSequenceVoronoiArea::ClusterSequenceVoronoiArea
(const std::vector<L> &pseudojets, 
 const JetDefinition &jet_def,
 const VoronoiAreaSpec & spec,
 const bool & writeout_combinations) :
  _effective_Rfact(spec.effective_Rfact()) {

  // transfer the initial jets (type L) into our own array
  _transfer_input_jets(pseudojets);

  // run the clustering
  _initialise_and_run(jet_def,writeout_combinations);

  // the jet clustering's already been done, now worry about areas...
  _initializeVA();
}

FASTJET_END_NAMESPACE

#endif // __FASTJET_CLUSTERSEQUENCEVORONOIAREA_HH__

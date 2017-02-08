// -*-c++-*-
%include "std_string.i"
%include "std_vector.i"
// extra configurations

%module fastjet

%{
#include "fastjet/config_auto.h"
#include "fastjet/config.h"
#include "fastjet/internal/base.hh"
#include "fastjet/internal/numconsts.hh"
#include "fastjet/internal/IsBase.hh"
#include "fastjet/internal/deprecated.hh"
#include "fastjet/internal/BasicRandom.hh"
#include "fastjet/SharedPtr.hh"
#include "fastjet/LimitedWarning.hh"
#include "fastjet/Error.hh"
#include "fastjet/PseudoJetStructureBase.hh"
#include "fastjet/PseudoJet.hh"
#include "fastjet/FunctionOfPseudoJet.hh"
#include "fastjet/RangeDefinition.hh"
#include "fastjet/Selector.hh"
#include "fastjet/JetDefinition.hh"
#include "fastjet/CompositeJetStructure.hh"
#include "fastjet/WrappedStructure.hh"
#include "fastjet/ClusterSequenceStructure.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/RectangularGrid.hh"
#include "fastjet/NNBase.hh"
#include "fastjet/NNH.hh"
#include "fastjet/NNFJN2Plain.hh"
#include "fastjet/NNFJN2Tiled.hh"
#include "fastjet/GhostedAreaSpec.hh"
#include "fastjet/AreaDefinition.hh"
#include "fastjet/ClusterSequenceAreaBase.hh"
#include "fastjet/ClusterSequenceActiveAreaExplicitGhosts.hh"
#include "fastjet/ClusterSequenceActiveArea.hh"
#include "fastjet/ClusterSequence1GhostPassiveArea.hh"
#include "fastjet/ClusterSequencePassiveArea.hh"
#include "fastjet/ClusterSequenceVoronoiArea.hh"
#include "fastjet/ClusterSequenceArea.hh"
  %}

%template(vectorPJ) std::vector<fastjet::PseudoJet>;

%include "fastjet/config_auto.h"
%include "fastjet/config.h"
%include "fastjet/internal/base.hh"
%include "fastjet/internal/numconsts.hh"
 //%include "fastjet/internal/IsBase.hh"
%include "fastjet/internal/deprecated.hh"
%include "fastjet/internal/BasicRandom.hh"
%include "fastjet/SharedPtr.hh"
%include "fastjet/LimitedWarning.hh"
%include "fastjet/Error.hh"
%include "fastjet/PseudoJetStructureBase.hh"
%include "fastjet/PseudoJet.hh"
%include "fastjet/FunctionOfPseudoJet.hh"
%include "fastjet/RangeDefinition.hh"
%include "fastjet/Selector.hh"
%include "fastjet/JetDefinition.hh"
%include "fastjet/CompositeJetStructure.hh"
%include "fastjet/ClusterSequenceStructure.hh"
%include "fastjet/ClusterSequence.hh"
%include "fastjet/RectangularGrid.hh"
%include "fastjet/NNBase.hh"
%include "fastjet/NNH.hh"
%include "fastjet/NNFJN2Plain.hh"
%include "fastjet/NNFJN2Tiled.hh"
%include "fastjet/GhostedAreaSpec.hh"
%include "fastjet/AreaDefinition.hh"
%include "fastjet/ClusterSequenceAreaBase.hh"
%include "fastjet/ClusterSequenceActiveAreaExplicitGhosts.hh"
%include "fastjet/ClusterSequenceActiveArea.hh"
%include "fastjet/ClusterSequence1GhostPassiveArea.hh"
%include "fastjet/ClusterSequencePassiveArea.hh"
%include "fastjet/ClusterSequenceVoronoiArea.hh"
%include "fastjet/ClusterSequenceArea.hh"

namespace fastjet {

// the templated ctors must be specialised for PseudoJet
%define FASTJET_TEMPLATED_CTOR_FOR_PSEUDOJET(Class)
%extend Class {
   %template(Class) Class<PseudoJet>;
}
%enddef

FASTJET_TEMPLATED_CTOR_FOR_PSEUDOJET(ClusterSequence)
FASTJET_TEMPLATED_CTOR_FOR_PSEUDOJET(ClusterSequenceActiveAreaExplicitGhosts)
FASTJET_TEMPLATED_CTOR_FOR_PSEUDOJET(ClusterSequenceActiveArea)
FASTJET_TEMPLATED_CTOR_FOR_PSEUDOJET(ClusterSequence1GhostPassiveArea)
FASTJET_TEMPLATED_CTOR_FOR_PSEUDOJET(ClusterSequencePassiveArea)
FASTJET_TEMPLATED_CTOR_FOR_PSEUDOJET(ClusterSequenceVoronoiArea)
FASTJET_TEMPLATED_CTOR_FOR_PSEUDOJET(ClusterSequenceArea)

// These make JetDefinition, Selector and PseudoJet all printable
%extend JetDefinition {
  std::string __str__() {return $self->description();}

  std::vector<PseudoJet> __call__(const std::vector<PseudoJet> & particles) {
    return (*self)(particles);
  }
}

%extend Selector {
  std::string __str__() {return $self->description();}

  // The C++ operators [* && || !] map to [* & | ~] in python
  Selector __mul__   (const Selector & other) {return *($self) *  other;}
  Selector __and__   (const Selector & other) {return *($self) && other;}
  Selector __or__    (const Selector & other) {return *($self) || other;}
  Selector __invert__()                       {return !(*($self));}
 }

%extend PseudoJet {
  //PseudoJet(const PseudoJet & p) {return new fastjet::PseudoJet(p);}

  std::string __str__() {
    const unsigned int len_max=4096;
    char temp[len_max];
    snprintf(temp,len_max, "[%f, %f, %f, %f]",$self->px(), $self->py(), $self->pz(), $self->E());
    return std::string(temp);
  }

  // these C++ operators are not automatically handled by SWIG (would only
  // be handled if there were part of the class)
  PseudoJet __add__ (const PseudoJet & p) {return *($self) + p;}
  PseudoJet __sub__ (const PseudoJet & p) {return *($self) - p;}
  bool      __eq__  (const PseudoJet & p) {return *($self) == p;}
  bool      __ne__  (const PseudoJet & p) {return *($self) != p;}
  PseudoJet __mul__ (double x) {return *($self) * x;}
  PseudoJet __rmul__(double x) {return *($self) * x;}
  PseudoJet __div__ (double x) {return *($self) / x;}
  bool      __eq__  (double x) {return *($self) == x;}
  bool      __ne__  (double x) {return *($self) != x;}
}

%template(FunctionOfPseudoJetDouble) FunctionOfPseudoJet<double>;
%template(FunctionOfPseudoJetPseudoJet) FunctionOfPseudoJet<fastjet::PseudoJet>;
  
}

%{
#include "fastjet/tools/Transformer.hh"
#include "fastjet/tools/Boost.hh"
#include "fastjet/tools/Recluster.hh"
#include "fastjet/tools/Filter.hh"
#include "fastjet/tools/Pruner.hh"
#include "fastjet/tools/CASubJetTagger.hh"
#include "fastjet/tools/MassDropTagger.hh"
#include "fastjet/tools/RestFrameNSubjettinessTagger.hh"
#include "fastjet/tools/TopTaggerBase.hh"
#include "fastjet/tools/JHTopTagger.hh"
#include "fastjet/tools/BackgroundEstimatorBase.hh"
#include "fastjet/tools/JetMedianBackgroundEstimator.hh"
#include "fastjet/tools/GridMedianBackgroundEstimator.hh"
#include "fastjet/tools/Subtractor.hh"
%}


%include "fastjet/tools/Transformer.hh"
%include "fastjet/tools/Boost.hh"
%include "fastjet/tools/Recluster.hh"
%include "fastjet/tools/Filter.hh"
%include "fastjet/tools/Pruner.hh"
%include "fastjet/tools/CASubJetTagger.hh"
%include "fastjet/tools/MassDropTagger.hh"
%include "fastjet/tools/RestFrameNSubjettinessTagger.hh"
%include "fastjet/tools/TopTaggerBase.hh"
%include "fastjet/tools/JHTopTagger.hh"
%include "fastjet/tools/BackgroundEstimatorBase.hh"
%include "fastjet/tools/JetMedianBackgroundEstimator.hh"
%include "fastjet/tools/GridMedianBackgroundEstimator.hh"
%include "fastjet/tools/Subtractor.hh"


namespace fastjet{
// all the tools operator() must be availale
%define FASTJET_OPERATOR_PARENTHESIS_CALLABLE(Class)
%extend Class {
  std::string  __str__() {
    return $self->description();
  }
}
%enddef

FASTJET_OPERATOR_PARENTHESIS_CALLABLE(Boost)
FASTJET_OPERATOR_PARENTHESIS_CALLABLE(Unboost)
FASTJET_OPERATOR_PARENTHESIS_CALLABLE(Recluster)
FASTJET_OPERATOR_PARENTHESIS_CALLABLE(Filter)
FASTJET_OPERATOR_PARENTHESIS_CALLABLE(Pruner)
FASTJET_OPERATOR_PARENTHESIS_CALLABLE(CASubJetTagger)
FASTJET_OPERATOR_PARENTHESIS_CALLABLE(MassDropTagger)
FASTJET_OPERATOR_PARENTHESIS_CALLABLE(RestFrameNSubjettinessTagger)
FASTJET_OPERATOR_PARENTHESIS_CALLABLE(JHTopTagger)
FASTJET_OPERATOR_PARENTHESIS_CALLABLE(Subtractor)
}

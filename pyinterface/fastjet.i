%module fastjet
%{
#include "fastjet/config_auto.h"
#include "fastjet/config.h"
#include "fastjet/internal/base.hh"
#include "fastjet/internal/numconsts.hh"
#include "fastjet/internal/IsBase.hh"
#include "fastjet/internal/deprecated.hh"
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
#include "fastjet/ClusterSequenceStructure.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/NNBase.hh"
#include "fastjet/NNH.hh"
  %}

%include "fastjet/config_auto.h"
%include "fastjet/config.h"
%include "fastjet/internal/base.hh"
%include "fastjet/internal/numconsts.hh"
 //%include "fastjet/internal/IsBase.hh"
%include "fastjet/internal/deprecated.hh"
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
%include "fastjet/NNBase.hh"
%include "fastjet/NNH.hh"

 // extra configurations
%include "std_vector.i"
namespace std{
  %template(vectorPJ) vector<fastjet::PseudoJet>;
}
%extend fastjet::ClusterSequence {
     %template(ClusterSequence) ClusterSequence<PseudoJet>;
}

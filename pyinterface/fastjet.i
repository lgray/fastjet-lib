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

// These make JetDefinition, Selector and PseudoJet all printable
%extend fastjet::JetDefinition {
  char *__str__() {
    const unsigned int len_max=4096;
    static char temp[len_max];
    snprintf(temp,len_max, "Jet Definition: %s",$self->description().c_str());
    return &temp[0];
  }
}

%extend fastjet::Selector {char *__str__() {
    const unsigned int len_max=4096;
    static char temp[len_max];
    snprintf(temp,len_max, "Selector: %s",$self->description().c_str());
    return &temp[0];
  }
}

%extend fastjet::PseudoJet {
  char *__str__() {
    const unsigned int len_max=4096;
    static char temp[len_max];
    snprintf(temp,len_max, "[%f, %f, %f, %f]",$self->px(), $self->py(), $self->pz(), $self->E());
    return &temp[0];
  }
}

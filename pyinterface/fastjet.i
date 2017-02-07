// -*-c++-*-
%include "std_string.i"
%include "std_vector.i"

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
namespace std{
  %template(vectorPJ) vector<fastjet::PseudoJet>;
}

namespace fastjet {

%extend ClusterSequence {
     %template(ClusterSequence) ClusterSequence<PseudoJet>;
}

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

}

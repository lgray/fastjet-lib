#include "fastjet/JetDefinition.hh"
#include "fastjet/Error.hh"
#include<sstream>

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

using namespace std;

string JetDefinition::description() const {
  ostringstream name;
  if (jet_finder() == plugin_algorithm) {
    return plugin()->description();
  } else if (jet_finder() == kt_algorithm) {
    name << "Longitudinally invariant kt algorithm with R = " << R();
  } else if (jet_finder() == cambridge_algorithm) {
    name << "Longitudinally invariant Cambridge/Aachen algorithm with R = " 
	 << R() ;
  } else {
    throw Error("Unrecognized jet_finder");
  }
  return name.str();
}

FASTJET_END_NAMESPACE

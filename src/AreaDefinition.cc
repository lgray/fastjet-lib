#include "fastjet/AreaDefinition.hh"
#include<sstream>
#include<string>

using namespace std;

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

string VoronoiAreaSpec::description() const {
  ostringstream ostr;
  ostr << "Voronoi area with effective_Rfact = " << effective_Rfact() ;
  return ostr.str();
}


//----------------------------------------------------------------------
///  return info about the type of area being used by this defn
string AreaDefinition::description() const {
  ostringstream ostr;

  switch(area_type()) {
  case active_area:
    return active_spec().description();
  case active_area_explicit_ghosts:
    ostr << active_spec().description() << " (explicit ghosts)" ;
    return ostr.str();
  case voronoi_area:
    return voronoi_spec().description();
  default:
    cerr << "Error: unrecognized area_type in AreaDefinition::description():" 
         << area_type() << endl;
    exit(-1);
  }
}

FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

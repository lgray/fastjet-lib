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
    name << " and " << recombiner()->description();
  } else if (jet_finder() == cambridge_algorithm) {
    name << "Longitudinally invariant Cambridge/Aachen algorithm with R = " 
	 << R() ;
    name << " and " << recombiner()->description();
  } else {
    throw Error("Unrecognized jet_finder");
  }
  return name.str();
}


void JetDefinition::set_recombination_scheme(
                               RecombinationScheme recomb_scheme) {
  _default_recombiner = JetDefinition::DefaultRecombiner(recomb_scheme);
  _recombiner = 0;
}


string JetDefinition::DefaultRecombiner::description() const {
  switch(_recomb_scheme) {
  case E_scheme:
    return "E scheme recombination";
  case pt_scheme:
    return "pt scheme recombination";
  case pt2_scheme:
    return "pt2 scheme recombination";
  case Et_scheme:
    return "Et scheme recombination";
  case Et2_scheme:
    return "Et2 scheme recombination";
  case BIpt_scheme:
    return "boost-invariant pt scheme recombination";
  case BIpt2_scheme:
    return "boost-invariant pt2 scheme recombination";
  default:
    ostringstream err;
    err << "DefaultRecombiner: unrecognized recombination scheme " 
        << _recomb_scheme;
    throw Error(err.str());
  }
}


void JetDefinition::DefaultRecombiner::recombine(
           const PseudoJet & pa, const PseudoJet & pb,
           PseudoJet & pab) const {
  
  double weighta, weightb;

  switch(_recomb_scheme) {
  case E_scheme:
    pab = pa + pb; 
    pab.set_user_index(0);
    return;
  // all remaining schemes are massless recombinations and locally
  // we just set weights, while the hard work is done below...
  case pt_scheme:
  case Et_scheme:
  case BIpt_scheme:
    weighta = pa.perp(); 
    weightb = pb.perp();
    break;
  case pt2_scheme:
  case Et2_scheme:
  case BIpt2_scheme:
    weighta = pa.perp2(); 
    weightb = pb.perp2();
    break;
  default:
    ostringstream err;
    err << "DefaultRecombiner: unrecognized recombination scheme " 
        << _recomb_scheme;
    throw Error(err.str());
  }

  double perp_ab = pa.perp() + pb.perp();
  double y_ab    = (weighta * pa.rap() + weightb * pb.rap())/(weighta+weightb);
  
  // take care with periodicity in phi...
  double phi_a = pa.phi(), phi_b = pb.phi();
  if (phi_a - phi_b > pi)  phi_b += twopi;
  if (phi_a - phi_b < -pi) phi_b -= twopi;
  double phi_ab = (weighta * phi_a + weightb * phi_b)/(weighta+weightb);

  pab = PseudoJet(perp_ab*cos(phi_ab),
                  perp_ab*sin(phi_ab),
                  perp_ab*sinh(y_ab),
                  perp_ab*cosh(y_ab));
  pab.set_user_index(0);
}


void JetDefinition::DefaultRecombiner::preprocess(PseudoJet & p) const {
  switch(_recomb_scheme) {
  case E_scheme:
  case BIpt_scheme:
  case BIpt2_scheme:
    break;
  case pt_scheme:
  case pt2_scheme:
    {
      // these schemes (as in the ktjet implementation) need massless
      // initial 4-vectors with essentially E=|p|.
      double newE = sqrt(p.perp2()+p.pz()*p.pz());
      int    user_index = p.user_index();
      p = PseudoJet(p.px(), p.py(), p.pz(), newE);
      p.set_user_index(user_index);
    }
    break;
  case Et_scheme:
  case Et2_scheme:
    {
      // these schemes (as in the ktjet implementation) need massless
      // initial 4-vectors with essentially E=|p|.
      double rescale = p.E()/sqrt(p.perp2()+p.pz()*p.pz());
      int    user_index = p.user_index();
      p = PseudoJet(rescale*p.px(), rescale*p.py(), rescale*p.pz(), p.E());
      p.set_user_index(user_index);
    }
    break;
  default:
    ostringstream err;
    err << "DefaultRecombiner: unrecognized recombination scheme " 
        << _recomb_scheme;
    throw Error(err.str());
  }
}

FASTJET_END_NAMESPACE

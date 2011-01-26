#include "fastjet/ClusterSequeneArea.hh"
#include "fastjet/tools/Selector.hh"
#include "fastjet/tools/BackgroundEstimator"
#include<iostream> // needed for io

using namespace std;
using namespace fastjet;


int main() {

  Selector jet_acceptance = SelectorPtMin(25.0) && SelectorAbsRapMax(2.8);
  Selector background_acceptance = SelectorStripRange(1.0) * (!SelectorNHardest(2));
  JetDefinition jet_def_akt04 = JetDefinition(antikt_algorithm, 0.4);
  JetDefinition jet_def_bkgd  = JetDefinition(kt_algorithm, 0.4);
  AreaDefinition area_def; // can the default work here?

  // read in an event
  vector<PseudoJet> input_particles;
  double px, py , pz, E;
  while (cin >> px >> py >> pz >> E) {
    input_particles.push_back(fastjet::PseudoJet(px,py,pz,E)); 
  }

  if (jets.size() > 2 && jets[0].perp() > 100) {
    double A_J = (jets[0].perp()-jets[1].perp())/(jets[0].perp()+jets[1].perp());
  }
}

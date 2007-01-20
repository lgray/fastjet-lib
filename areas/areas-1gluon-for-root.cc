#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceActiveAreaExplicitGhosts.hh"
#include "SISConePlugin.hh"
#include<iostream>
#include<iomanip>
#include<fstream>
#include<sstream>
#include<valarray>
#include<vector>
#include <cstdlib>
#include<cstddef> // for size_t
#include "CmdLine.hh"

namespace fj = fastjet;
using namespace std;

inline double pow2(const double x) {return x*x;};

// this is how we'll be wanting our output of jets...
ostream & operator<<(ostream & ostr, const fj::PseudoJet & jet) {
  ostr << jet.rap() << " " << jet.phi() << " " << jet.perp();
  return ostr;
}

//----------------------------------------------------------------------
/// a program to test and time the kt algorithm as implemented in fastjet
int main (int argc, char ** argv) {

  CmdLine cmdline(argc,argv);
  double ktR   = cmdline.double_val("-r",1.0);
  double ghost_area = cmdline.double_val("-ghost_area",cmdline.double_val("-cell_area",0.01));
  double ghost_etamax = cmdline.double_val("-ghost_etamax",6.0);
  double grid_scatter = cmdline.double_val("-grid_scatter",0.0001);
  double kt_scatter   = cmdline.double_val("-kt_scatter",0.1);
  double ghost_kt = cmdline.double_val("-ghost_kt",1e-100);
  double partner_phi = cmdline.double_val("-partner",0.0);
  int skip = cmdline.int_val("-skip",0);
  string outfile = cmdline.string_val("-out");

  if (!cmdline.all_options_used()) exit(-1);

  fj::JetDefinition jet_def(fj::kt_algorithm, ktR);
  //fj::JetDefinition jet_def(new fj::SISConePlugin(ktR,0.50,0));
  fj::ActiveAreaSpec active_area_spec(ghost_etamax, 1, ghost_area, 
                                      grid_scatter, kt_scatter, ghost_kt);

  for (int irep = 0; irep <= skip; irep++) {
    vector<fj::PseudoJet> empty_input;
    empty_input.push_back(fj::PseudoJet(-100.0,0.0,0.0,100.0));
    if (partner_phi != 0.0) {
      empty_input.push_back(fj::PseudoJet(-100.0*cos(partner_phi),
                                          -100.0*sin(partner_phi),0.0,100.0));
    }
    fj::ClusterSequenceActiveAreaExplicitGhosts clust_seq(empty_input,jet_def,active_area_spec);
    if (irep < skip) continue;

    ofstream ostr(outfile.c_str());
    ostr << "# " << cmdline.command_line() << endl;
    ostr << "# jet def: " << jet_def.description() << endl;
    vector<fj::PseudoJet> jets = clust_seq.inclusive_jets();
    for (size_t j = 0; j < jets.size(); j++) {
      ostr << j << " " << jets[j] << endl;
      vector<fj::PseudoJet> particles(clust_seq.constituents(jets[j]));
      for (size_t ip = 0; ip < particles.size(); ip++) {
        ostr << " " << ip << " " << particles[ip] << endl;
        cout << setprecision(14) << particles[ip][0] << " "
             << particles[ip][1] << " "
             << particles[ip][2] << " "
             << particles[ip][3] << endl;
      }
      ostr << "#END" <<endl;
    }
  }

}

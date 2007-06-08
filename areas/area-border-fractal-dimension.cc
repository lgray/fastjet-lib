#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceActiveArea.hh"
//#include "ClusterSequencePassiveArea.hh"
#include<iostream>
#include<sstream>
#include<fstream>
#include<valarray>
#include<vector>
#include <cstdlib>
#include<cstddef> // for size_t
#include "CmdLine.hh"
#include "jet_def_from_cmdline.hh"
#include "CSHisto.hh"
#include "AverageAndError.hh"

namespace fj = fastjet;
using namespace fj;
using namespace std;

inline double pow2(const double x) {return x*x;};


struct GhostCoord {
  GhostCoord() {}
  GhostCoord(int i, int j) : irap(i), iphi(j) {}
  int  irap,iphi;
};


//----------------------------------------------------------------------
/// a program to measure the fractal dimension of the boundary of a jet
int main (int argc, char ** argv) {

  CmdLine cmdline(argc,argv);
  fj::JetDefinition jet_def = jet_def_from_cmdline(cmdline);
  fj::AreaDefinition area_def = area_def_from_cmdline(cmdline);
  
  const GhostedAreaSpec & ghost_spec = area_def.ghost_spec();
  int nrap = 2 * ghost_spec.nrap() + 1;
  int nphi = ghost_spec.nphi();
  int nghost = nrap*nphi;
  int ghost_jet_index[nrap][nphi];
  GhostCoord ghost_coord[nghost];

  //cout << "nrap = " << nrap << ", nphi = " << nphi << endl;

  // get a correspondence between ghost indices and their coordinates
  for (int irap = 0; irap < nrap; irap++) {
    for (int iphi = 0; iphi < nphi; iphi++) {
      int idx = iphi + irap*nphi;
      ghost_coord[idx] = GhostCoord(irap, iphi);
      ghost_jet_index[irap][iphi] = -1;
    }
  }
  
  // now generate some ghost particles
  vector<PseudoJet> particles;
  ghost_spec.add_ghosts(particles);
  assert(nghost == int(particles.size()));

  // optionally later add some hard particles to particles

  // now get the jets -- and for each one so some labelling of consituents
  ClusterSequence cs(particles,jet_def);
  vector<PseudoJet> jets = cs.inclusive_jets();
  for (unsigned int ijet = 0; ijet < jets.size(); ijet++) {
    vector<PseudoJet> constituents(cs.constituents(jets[ijet]));
    for (unsigned icst = 0; icst < constituents.size(); icst++) {
      // find out which particle we have and in our 2d-array set the 
      // jet index
      int ipart = constituents[icst].cluster_hist_index();
      ghost_jet_index[ghost_coord[ipart].irap][ghost_coord[ipart].iphi] = ijet;
    }
  }

  // now figure out the border sizes for each jet
  vector<int> jet_nborder(jets.size(),0);
  // leave out high-rap borders
  for (int irap = 1; irap < nrap-1; irap++) {
    for (int iphi = 0; iphi < nphi-1; iphi++) {
      int ijet = ghost_jet_index[irap][iphi];
      if (ijet < 0) cout << "met ghost with no jet: " << irap << " " << iphi << endl;
      bool border = false;
      border |= (ijet != ghost_jet_index[irap-1][iphi]);
      border |= (ijet != ghost_jet_index[irap+1][iphi]);
      border |= (ijet != ghost_jet_index[irap][(iphi+1)%nphi]);
      border |= (ijet != ghost_jet_index[irap][(iphi-1)%nphi]);
      border |= (ijet != ghost_jet_index[irap+1][(iphi+1)%nphi]);
      border |= (ijet != ghost_jet_index[irap+1][(iphi-1)%nphi]);
      border |= (ijet != ghost_jet_index[irap-1][(iphi+1)%nphi]);
      border |= (ijet != ghost_jet_index[irap-1][(iphi-1)%nphi]);
      if (border) jet_nborder[ijet]++;
    }
  }

  // now get average border size
  AverageAndError av;
  for (unsigned ijet = 0; ijet < jets.size(); ijet++) {
    if (abs(jets[ijet].rap()) < ghost_spec.ghost_maxrap() - 1.0) {
      av.add(jet_nborder[ijet]);
    }
  }

  cout << "# ghost area, av boundary count +- error sum-boundary-count" << endl;
  cout << ghost_spec.actual_ghost_area() << " " << av.average() << " +- " << av.error() << " " << av.sum() << endl;
}

/// \file PJtiming.cc
///
/// Standalone program for investigating the impact of FJ3.0 v FJ2.4
/// on timing for creation of vectors of PseudoJets.
/// 
/// Results are for g++ 4.4.5 and were obtained on 12 May 2011, around
/// revision 2093.
///
/// - FJ3.0 (with two shared pointers, no default initialisation of rest):
///   - C++ vector: 140ns / PJ (for vector of size 10^4; 110ns for smaller vector)
///   - C   array:    9ns / PJ
///
/// - FJ2.4
///   - C++ vector:  27ns / PJ (for vector of size 10^4; much less for smaller vector)
///   - C   array:    0ns / PJ
#include "fastjet/PseudoJet.hh"
#include <iostream>
#include "CmdLine.hh"

using namespace fastjet;
using namespace std;

// // a 
// class TestingObject {
//   TestingObject
//   double px, py, pz, E
// };

void make_vector(int sz) {
  vector<PseudoJet> particles(sz);
  particles[sz-1].reset(0,0,0,0); // make sure something happens
}

void make_array(int sz) {
  PseudoJet particles[sz];
  particles[sz-1].reset(0,0,0,0); // make sure something happens
}

int main(int argc, char** argv) {
  CmdLine cmdline(argc,argv);
  int n = int(cmdline.value("-n",100.0));
  int sz = int(cmdline.value("-sz",100.0));
  
  bool array = cmdline.present("-array");

  for (int i = 0; i < n; i++) {
    if (array) {make_array(sz);}
    else       {make_vector(sz);}
  }
}

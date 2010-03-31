//STARTHEADER
// $Id$
//
// Copyright (c) 2005-2007, Matteo Cacciari, Gavin Salam and Gregory Soyez
//
//----------------------------------------------------------------------
// This file is part of FastJet.
//
//  FastJet is free software; you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation; either version 2 of the License, or
//  (at your option) any later version.
//
//  The algorithms that underlie FastJet have required considerable
//  development and are described in hep-ph/0512210. If you use
//  FastJet as part of work towards a scientific publication, please
//  include a citation to the FastJet paper.
//
//  FastJet is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with FastJet; if not, write to the Free Software
//  Foundation, Inc.:
//      59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//----------------------------------------------------------------------
//ENDHEADER

#include <iostream>
#include "fastjet/ClusterSequence.hh"
#include "fastjet/SISConePlugin.hh"

using namespace std;
using namespace fastjet;

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

/// a namespace for the fortran-wrapper which contains commonly-used
/// structures and means to transfer fortran <-> C++
namespace fwrapper {
  vector<PseudoJet> input_particles, jets;
  auto_ptr<JetDefinition::Plugin> plugin;
  JetDefinition jet_def;
  auto_ptr<ClusterSequence> cs;

  /// helper routine to transfer fortran input particles into 
  void transfer_input_particles(const double * p, const int & npart) {
    input_particles.resize(0);
    input_particles.reserve(npart);
    for (int i=0; i<npart; i++) {
      valarray<double> mom(4); // mom[0..3]
      for (int j=0;j<=3; j++) {
         mom[j] = *(p++);
      }
      PseudoJet psjet(mom);
      input_particles.push_back(psjet);    
    }
  }

  /// helper routine to help transfer jets -> f77jets[4*ijet+0..3]
  void transfer_jets(double * f77jets, int & njets) {
    njets = jets.size();
    for (int i=0; i<njets; i++) {
      for (int j=0;j<=3; j++) {
        *f77jets = jets[i][j];
        f77jets++;
      } 
    }
  }

}
FASTJET_END_NAMESPACE

using namespace fastjet::fwrapper;


extern "C" {   

// f77 interface to SISCone (via fastjet), as defined in arXiv:0704.0292
// [see below for the interface to kt, Cam/Aachen & kt]
//
// Corresponds to the following Fortran subroutine
// interface structure:
//
//   SUBROUTINE FASTJETSISCONE(P,NPART,R,F,F77JETS,NJETS)
//   DOUBLE PRECISION P(4,*), R, F, F77JETS(4,*)
//   INTEGER          NPART, NJETS
// 
// where on input
//
//   P        the input particle 4-momenta
//   NPART    the number of input momenta
//   R        the radius parameter
//   F        the overlap threshold
//
// and on output 
//
//   F77JETS  the output jet momenta (whose second dim should be >= NPART)
//            sorted in order of decreasing p_t.
//   NJETS    the number of output jets 
//
void fastjetsiscone_(const double * p, const int & npart,                   
                     const double & R, const double & f,                   
                     double * f77jets, int & njets) {

    // transfer p[4*ipart+0..3] -> input_particles[i]
    transfer_input_particles(p, npart);
    
    // prepare jet def and run fastjet
    plugin.reset(new SISConePlugin(R,f));
    jet_def = plugin.get();
    
    // perform clustering
    cs.reset(new ClusterSequence(input_particles,jet_def));
    // extract jets (pt-ordered)
    jets = sorted_by_pt(cs->inclusive_jets());

    // transfer jets -> f77jets[4*ijet+0..3]
    transfer_jets(f77jets, njets);
}



// f77 interface to the pp generalised-kt (sequential recombination)
// algorithms, as defined in arXiv.org:0802.1189, which includes
// kt, Cambridge/Aachen and anti-kt as special cases.
//
// Corresponds to the following Fortran subroutine
// interface structure:
//
//   SUBROUTINE FASTJETPPSEQREC(P,NPART,R,PALG,F77JETS,NJETS)
//   DOUBLE PRECISION P(4,*), R, PALG, F, F77JETS(4,*)
//   INTEGER          NPART, NJETS
// 
// where on input
//
//   P        the input particle 4-momenta
//   NPART    the number of input momenta
//   R        the radius parameter
//   PALG     the power for the generalised kt alg 
//            (1.0=kt, 0.0=C/A,  -1.0 = anti-kt)
//
// and on output 
//
//   F77JETS  the output jet momenta (whose second dim should be >= NPART)
//            sorted in order of decreasing p_t.
//   NJETS    the number of output jets 
//
// For the values of PALG that correspond to "standard" cases (1.0=kt,
// 0.0=C/A, -1.0 = anti-kt) this routine actually calls the direct
// implementation of those algorithms, whereas for other values of
// PALG it calls the generalised kt implementation.
//
void fastjetppgenkt_(const double * p, const int & npart,                   
                     const double & R, const double & palg,
                     double * f77jets, int & njets) {

    // transfer p[4*ipart+0..3] -> input_particles[i]
    transfer_input_particles(p, npart);
    
    // prepare jet def and run fastjet
    if (palg == 1.0) {
      jet_def = JetDefinition(kt_algorithm, R);
    }  else if (palg == 0.0) {
      jet_def = JetDefinition(cambridge_algorithm, R);
    }  else if (palg == -1.0) {
      jet_def = JetDefinition(antikt_algorithm, R);
    } else {
      jet_def = JetDefinition(genkt_algorithm, R, palg);
    }
    
    // perform clustering
    cs.reset(new ClusterSequence(input_particles,jet_def));
    // extract jets (pt-ordered)
    jets = sorted_by_pt(cs->inclusive_jets());

    // transfer jets -> f77jets[4*ijet+0..3]
    transfer_jets(f77jets, njets);
    
}

/// f77 interface to provide access to the constituents of a jet found
/// in the jet clustering with one of the above routines.
///
/// Given the index ijet of a jet (in the range 1...njets) obtained in
/// the last call to jet clustering, fill the array
/// constituent_indices, with nconstituents entries, with the indices
/// of the constituents that belong to that jet (which will be in the
/// range 1...npart)
void fastjetconstituents_(const int & ijet, 
   	                  int * constituent_indices, int & nconstituents) {
  assert(cs.get() != 0);
  cout << ijet << " " << jets.size() << endl;
  assert(ijet > 0 && ijet <= jets.size());

  vector<PseudoJet> constituents = cs->constituents(jets[ijet-1]);

  nconstituents = constituents.size();
  for (unsigned i = 0; i < nconstituents; i++) {
    constituent_indices[i] = constituents[i].cluster_hist_index()+1;
  }
}

}

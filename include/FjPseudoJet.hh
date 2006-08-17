#ifndef __FJPSEUDOJET_HH__
#define __FJPSEUDOJET_HH__

#include "fastjet/PseudoJet.hh"

/// typedef which provides backwards compatibility for 
/// user programs based on the v1 interface
typedef fastjet::PseudoJet FjPseudoJet;

/// thought not officially "declared" in the docuementation, 
/// this was used in the v1 fastjet_timing.cc example program,
/// so in order to ensure that it still compiles we define it...
const double twopi = fastjet::twopi;

#endif //__FJPSEUDOJET_HH__

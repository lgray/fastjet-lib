#ifndef __FJCLUSTERSEQUENCE_HH__
#define __FJCLUSTERSEQUENCE_HH__

#include "fastjet/ClusterSequence.hh"


/// typedef which provides backwards compatibility for 
/// user programs based on the v1 interface
typedef fastjet::ClusterSequence FjClusterSequence;

/// typedef which provides backwards compatibility for 
/// user programs based on the v1 interface
typedef fastjet::Strategy        FjStrategy;


// below follow redefinitions of all the strategy constants
// to allow a v1 legacy user to access the strategy names
// as before

/// experimental ...
const FjStrategy N2MinHeapTiled   = fastjet::N2MinHeapTiled;
/// fastest from about 50..10^4
const FjStrategy N2Tiled     = fastjet::N2Tiled;
/// legacy
const FjStrategy N2PoorTiled = fastjet::N2PoorTiled;
/// fastest below 50
const FjStrategy N2Plain     = fastjet::N2Plain;
/// worse even than the usual N^3 algorithms
const FjStrategy N3Dumb      = fastjet::N3Dumb;
/// automatic selection of the best (based on N)
const FjStrategy Best        = fastjet::Best;
/// best of the NlnN variants -- best overall for N>10^4
const FjStrategy NlnN        = fastjet::NlnN;
/// legacy N ln N using 3pi coverage of cylinder
const FjStrategy NlnN3pi     = fastjet::NlnN3pi;
/// legacy N ln N using 4pi coverage of cylinder
const FjStrategy NlnN4pi     = fastjet::NlnN4pi;
/// Chan's closest pair method (in a variant with 4pi coverage),
/// for use exclusively with the Cambridge algorithm
const FjStrategy NlnNCam4pi   = fastjet::NlnNCam4pi;
const FjStrategy NlnNCam2pi2R = fastjet::NlnNCam2pi2R;
const FjStrategy NlnNCam2piMultD = fastjet::NlnNCam2piMultD;


#endif //__FJCLUSTERSEQUENCE_HH__

#ifndef _READ_EVENT_HH__
#define _READ_EVENT_HH__

#include "fastjet/PseudoJet.hh"
#include <vector>
#include <iostream>
// common subroutine for reading events, both in pythia format and in
// hydjet format
void read_event_common(std::istream &, double, bool, bool,
                       std::vector<fastjet::PseudoJet> &, 
                       std::vector<fastjet::PseudoJet> & );
#endif // _READ_EVENT_HH__

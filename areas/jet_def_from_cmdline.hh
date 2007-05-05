#include "CmdLine.hh"
#include "fastjet/JetDefinition.hh"

/// return a jet definition based on the command line...
fastjet::JetDefinition jet_def_from_cmdline(const CmdLine & cmdline);

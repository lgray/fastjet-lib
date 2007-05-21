#include "CmdLine.hh"
#include "fastjet/JetDefinition.hh"
#include "fastjet/AreaDefinition.hh"

/// return a jet definition based on the command line...
fastjet::JetDefinition jet_def_from_cmdline(const CmdLine & cmdline);

/// return an area definition based on the command line...
fastjet::AreaDefinition area_def_from_cmdline(const CmdLine & cmdline);

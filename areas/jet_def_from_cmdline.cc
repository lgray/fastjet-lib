#include "jet_def_from_cmdline.hh"

#include "SISConePlugin.hh"
#include "CDFMidPointPlugin.hh"

using namespace fastjet;

/// return a jet definition based on the command line...
JetDefinition jet_def_from_cmdline(const CmdLine & cmdline) {
  double R = cmdline.value("-R",0.7);
  JetDefinition jet_def;
  if (cmdline.present("-kt")) {
    jet_def = JetDefinition(kt_algorithm,R);
  } else if (cmdline.present("-cam")) {
    jet_def = JetDefinition(cambridge_algorithm,R);
  } else if (cmdline.present("-antikt")) {
    jet_def = JetDefinition(antikt_algorithm,R);
  } else {
    double f = cmdline.value("-f",0.5);
    if (cmdline.present("-midpoint")) {
      double seed = cmdline.value("-seed",1);
      jet_def = new CDFMidPointPlugin(R, f, seed);
    } else if (cmdline.present("-siscone")) {
      int npass = 0;
      double protojet_ptmin = cmdline.value("-sisptmin",0.0);
      SISConePlugin * plugin = new SISConePlugin(R, f,npass,protojet_ptmin);
      if (cmdline.present("-smstop")) {
        plugin->set_split_merge_stopping_scale(
                                cmdline.value<double>("-smstop"));
      }
      jet_def = JetDefinition(plugin);
    } else {
      cerr << "Error: one of {-kt|-cam|-antikt|-siscone|-midpoint} must be present" << endl;
      exit(-1);
    }
  }
  return jet_def;
}

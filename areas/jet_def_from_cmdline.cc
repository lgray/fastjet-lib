#include "jet_def_from_cmdline.hh"

#include "SISConePlugin.hh"
#include "CDFMidPointPlugin.hh"

using namespace fastjet;

/// return a jet definition based on the command line...
JetDefinition jet_def_from_cmdline(const CmdLine & cmdline) {
  double R = cmdline.value("-R",0.7);
  R = cmdline.value("-r",R); // allow alternative
  JetDefinition jet_def;

  Strategy  strategy  = Strategy(cmdline.value<int>("-strategy",
				     cmdline.value<int>("-clever", Best)));

  if (cmdline.present("-kt")) {
    jet_def = JetDefinition(kt_algorithm,R,strategy);
  } else if (cmdline.present("-cam")) {
    jet_def = JetDefinition(cambridge_algorithm,R,strategy);
  } else if (cmdline.present("-antikt")) {
    jet_def = JetDefinition(antikt_algorithm,R,strategy);
  } else {
    // CONE ALGORITHMS
    double f = cmdline.value("-f",0.5);
    if (cmdline.present("-midpoint")) {
      // MIDPOINT
      double seed = cmdline.value("-seed",1);
      jet_def = new CDFMidPointPlugin(R, f, seed);
    } else if (cmdline.present("-siscone")) {
      // SISCONE
      int npass = cmdline.value("-npass", 0);
      SISConePlugin::SplitMergeScale sms = SISConePlugin::SM_pttilde;
      if (cmdline.present("-smscale")) {
        string smscale_name = cmdline.value<string>("-smscale");
        if (smscale_name == "pttilde") {sms = SISConePlugin::SM_pttilde;}
        else if (smscale_name == "pt") {sms = SISConePlugin::SM_pt;}
        else if (smscale_name == "Et") {sms = SISConePlugin::SM_Et;}
        else if (smscale_name == "mt") {sms = SISConePlugin::SM_mt;}
        else {
          cerr << "jet_def_from_cmdline: unrecognised smscale" << endl;
          exit (-1);
        }
      }
      double protojet_ptmin = cmdline.value("-sisptmin",0.0);
      bool   caching = false;
      SISConePlugin * plugin = new SISConePlugin(R, 
                                                 f,
                                                 npass,
                                                 protojet_ptmin,
                                                 caching,
                                                 sms);
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

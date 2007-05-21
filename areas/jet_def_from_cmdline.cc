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


//----------------------------------------------------------------------
AreaDefinition area_def_from_cmdline(const CmdLine & cmdline) {

  AreaDefinition area_def;

  int  repeat  = cmdline.int_val("-repeat",1);
  double ghost_area = cmdline.double_val("-ghost_area",cmdline.double_val("-cell_area",0.01));
  double ghost_etamax = cmdline.double_val("-ghost_etamax",6.0);
  double grid_scatter = cmdline.double_val("-grid_scatter",1.0);
  double kt_scatter   = cmdline.double_val("-kt_scatter",0.1);

  if (cmdline.present("-voronoi")) {
    // create the definitions for our jet finder and areas spec...
    //JetDefinition jet_def(kt_algorithm, ktR, strategy);
    double voronoi_rescale = cmdline.value("-voronoi_rescale",1.0);
    VoronoiAreaSpec voronoi_area_spec(voronoi_rescale);
    area_def = AreaDefinition(voronoi_area_spec);
  } else {
    // create the definitions for our jet finder and areas spec...
    //JetDefinition jet_def(kt_algorithm, ktR, strategy);
    GhostedAreaSpec ghosted_area_spec(ghost_etamax, repeat, ghost_area, 
                                          grid_scatter, kt_scatter);
    if (cmdline.present("-passive")) {
      area_def = AreaDefinition(passive_area, ghosted_area_spec);
    } else {
      area_def = AreaDefinition(active_area, ghosted_area_spec);
    }
  }
  return area_def;
}

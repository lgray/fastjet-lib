//STARTHEADER
// $Id: fastjet_timing.cc 293 2006-08-17 19:38:38Z salam $
//
// Copyright (c) 2005-2006, Matteo Cacciari and Gavin Salam
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


//----------------------------------------------------------------------
/// fastjet_timing.cc: Program to help time and test the fastjet package
/// 
/// It reads files containing multiple events in the format 
/// p1x p1y p1z E1
/// p2x p2y p2z E2
/// ...
/// #END
/// 
/// An example input file containing 10 events is included as 
/// data/Pythia-PtMin1000-LHC-10ev.dat
///
/// Usage:
///   fastjet_timing [-strategy NUMBER] [-repeat nrepeats] [-massive] \
///                  [-combine nevents] [-r Rparameter] [-incl ptmin] [...] \
///                  < data_file
///
/// where the clustering can be repeated to aid timing and multiple
/// events can be combined to get to larger multiplicities. Some options:
///
/// Options for reading
/// -------------------
///
///   -nev     n    number of events to run
///
///   -combine n    for combining multiple events from the data file in order
///                 to get a single high-multipicity event to run.
///
///   -massless     read in only the 3-momenta and deduce energies assuming
///                 that particles are massless
///
///   -dense        adds dense ghost coverage
///
///   -repeat n     repeats each event n times
///
/// Output Options
/// --------------
///
///   -incl ptmin   output of all inclusive jets with pt > ptmin is obtained
///                 with the -incl option.
///
///   -excld dcut   output of all exclusive jets as obtained in a clustering
///                 with dcut
///
///   -excly ycut   output of all exclusive jets as obtained in a clustering
///                 with ycut
///
///   -excln n      output of clustering to n exclusive jets
///
///   -ee-print     print things as px,py,pz,E
///
///   -get-all-dij  print out all dij values
///   -get-all-yij  print out all yij values
///
///   -const        show jet constituents (works with excl jets)
///
///   -write        for writing out detailed clustering sequence (valuable
///                 for testing purposes)
///
///   -unique_write writes out the sequence of dij's according to the
///                 "unique_history_order" (useful for verifying consistency
///                 between different clustering strategies).
///
///   -root file    sends output to file that can be read in with the script in
///                 root/ so as to show a lego-plot of the event
///
///   -cones        show extra info about internal steps for SISCone
///
/// Algorithms
/// ----------
///   -kt           switch to the longitudinally invariant kt algorithm
///                 Note: this is the default one.
///
///   -cam          switch to the inclusive Cambridge/Aachen algorithm --
///                 note that the option -excld dcut provides a clustering
///                 up to the dcut which is the minimum squared
///                 distance between any pair of jets.
///
///   -antikt       switch to the anti-kt clustering algorithm
///
///   -genkt        switch to the genkt algorithm
///                 you can provide the parameter of the alg as an argument to 
///                 -genkt (1 by default)
///                 
///   -eekt         switch to the e+e- kt algorithm
///
///   -eegenkt      switch to the genkt algorithm
///                 you can provide the parameter of the alg as an argument to 
///                 -ee_genkt (1 by default)
///                 
/// plugins (don't delete this line)
///
///   -pxcone       switch to the PxCone jet algorithm
/// 
///   -siscone       switch to the SISCone jet algorithm (seedless cones)
///   -sisconespheri switch to the Spherical SISCone jet algorithm (seedless cones)
///
///   -midpoint     switch to CDF's midpoint code
///
///   -jetclu       switch to CDF's jetclu code
///
///   -d0runiicone  switch to D0's run II midpoint cone
///
///   -trackjet     switch to the TrackJet plugin
///
///   -atlascone     switch to the ATLASCone plugin
///
///   -eecambridge     switch to the EECambridge plugin
///
///  end of plugins (don't delete this line)
///
///
/// Options for running algs
/// ------------------------
///
///   -r            sets the radius of the jet algorithm (default = 1.0)
///
///   -overlap | -f sets the overlap fraction in cone algs with split-merge
///
///   -seed         sets the seed threshold
///
///   -strategy N   indicate stratgey from the enum fastjet::Strategy (see
///                 fastjet/JetDefinition.hh).
///


#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/GhostedAreaSpec.hh"
#include<iostream>
#include<sstream>
#include<fstream>
#include<valarray>
#include<vector>
#include <cstdlib>
#include<cstddef> // for size_t
#include "CmdLine.hh"

// get info on how fastjet was configured
#include "fastjet/config.h"

// include the installed plugins (don't delete this line)
#ifdef ENABLE_PLUGIN_SISCONE
#include "fastjet/SISConePlugin.hh"
#include "fastjet/SISConeSphericalPlugin.hh"
#endif
#ifdef ENABLE_PLUGIN_CDFCONES
#include "fastjet/CDFMidPointPlugin.hh"
#include "fastjet/CDFJetCluPlugin.hh"
#endif
#ifdef ENABLE_PLUGIN_PXCONE
#include "fastjet/PxConePlugin.hh"
#endif
#ifdef ENABLE_PLUGIN_D0RUNIICONE
#include "fastjet/D0RunIIConePlugin.hh"
#endif 
#ifdef ENABLE_PLUGIN_TRACKJET
#include "fastjet/TrackJetPlugin.hh"
#endif
#ifdef ENABLE_PLUGIN_ATLASCONE
#include "fastjet/ATLASConePlugin.hh"
#endif
#ifdef ENABLE_PLUGIN_EECAMBRIDGE
#include "fastjet/EECambridgePlugin.hh"
#endif
// end of installed plugins inclusion (don't delete this line)

using namespace std;

// to avoid excessive typing, define an abbreviation for the 
// fastjet namespace
namespace fj = fastjet;

inline double pow2(const double x) {return x*x;}

// pretty print the jets and their subjets
void print_jets_and_sub (fj::ClusterSequence & clust_seq, 
                         const vector<fj::PseudoJet> & jets, double dcut);

/// sort and pretty print jets, with exact behaviour depending on 
/// whether ee_print is true or not
bool ee_print = false;
void print_jets(const vector<fj::PseudoJet> & jets, const fj::ClusterSequence & cs, bool show_const = false);

void is_unavailable(const string & algname) {
  cerr << algname << " requested, but not available for this compilation";
  exit(-1);
}


/// a program to test and time a range of algorithms as implemented or
/// wrapped in fastjet
int main (int argc, char ** argv) {

  CmdLine cmdline(argc,argv);
  // allow the use to specify the fj::Strategy either through the
  // -clever or the -strategy options (both will take numerical
  // values); the latter will override the former.
  fj::Strategy  strategy  = fj::Strategy(cmdline.int_val("-strategy",
                                        cmdline.int_val("-clever", fj::Best)));
  int  repeat  = cmdline.int_val("-repeat",1);
  int  combine = cmdline.int_val("-combine",1);
  bool write   = cmdline.present("-write");
  bool unique_write = cmdline.present("-unique_write");
  bool hydjet  = cmdline.present("-hydjet");
  double ktR   = cmdline.double_val("-r",1.0);
  ktR   = cmdline.double_val("-R",ktR); // allow -r and -R
  double inclkt = cmdline.double_val("-incl",-1.0);
  int    excln  = cmdline.int_val   ("-excln",-1);
  double excld  = cmdline.double_val("-excld",-1.0);
  double excly  = cmdline.double_val("-excly",-1.0);
  ee_print = cmdline.present("-ee-print");
  bool   get_all_dij   = cmdline.present("-get-all-dij");
  bool   get_all_yij   = cmdline.present("-get-all-yij");
  double subdcut = cmdline.double_val("-subdcut",-1.0);
  double etamax = cmdline.double_val("-etamax",1.0e305);
  bool   show_constituents = cmdline.present("-const");
  bool   massless = cmdline.present("-massless");
  int    nev     = cmdline.int_val("-nev",1);
  bool   add_dense_coverage = cmdline.present("-dense");
  double ghost_maxrap = cmdline.value("-ghost-maxrap",5.0);

  bool show_cones = cmdline.present("-cones"); // only works for siscone

  // for cone algorithms
  // allow -f and -overlap
  double overlap_threshold = cmdline.double_val("-overlap",0.5);
  overlap_threshold = cmdline.double_val("-f",overlap_threshold); 
  double seed_threshold = cmdline.double_val("-seed",1.0);

  // for ee algorithms, allow to specify ycut
  double ycut = cmdline.double_val("-ycut",0.08);

  // for printing jets to a file for reading by root
  string rootfile = cmdline.value<string>("-root","");

  // out default scheme is the E_scheme
  fj::RecombinationScheme scheme = fj::E_scheme;

  // The following option causes the Cambridge algo to be used.
  // Note that currently the only output that works sensibly here is
  // "-incl 0"
  fj::JetDefinition jet_def;
  if (cmdline.present("-cam") || cmdline.present("-CA")) {
    jet_def = fj::JetDefinition(fj::cambridge_algorithm, ktR, scheme, strategy);
  } else if (cmdline.present("-antikt")) {
    jet_def = fj::JetDefinition(fj::antikt_algorithm, ktR, scheme, strategy);
  } else if (cmdline.present("-genkt")) {
    double p = cmdline.value<double>("-genkt");
    jet_def = fj::JetDefinition(fj::genkt_algorithm, ktR, p, scheme, strategy);
  } else if (cmdline.present("-eekt")) {
    jet_def = fj::JetDefinition(fj::ee_kt_algorithm);
  } else if (cmdline.present("-eegenkt")) {
    double p = cmdline.value<double>("-eegenkt");
    jet_def = fj::JetDefinition(fj::ee_genkt_algorithm, ktR, p, scheme, strategy);

// checking if one asks to run a plugin (don't delete this line)
  } else if (cmdline.present("-midpoint")) {
#ifdef ENABLE_PLUGIN_CDFCONES
    typedef fj::CDFMidPointPlugin MPPlug; // for brevity
    double cone_area_fraction = 1.0;
    int    max_pair_size = 2;
    int    max_iterations = 100;
    MPPlug::SplitMergeScale sm_scale = MPPlug::SM_pt;
    if (cmdline.present("-sm-pttilde")) sm_scale = MPPlug::SM_pttilde;
    if (cmdline.present("-sm-pt")) sm_scale = MPPlug::SM_pt; // default
    if (cmdline.present("-sm-mt")) sm_scale = MPPlug::SM_mt;
    if (cmdline.present("-sm-Et")) sm_scale = MPPlug::SM_Et;
    jet_def = fj::JetDefinition( new fj::CDFMidPointPlugin (
                                      seed_threshold, ktR, 
                                      cone_area_fraction, max_pair_size,
                                      max_iterations, overlap_threshold,
                                      sm_scale));
#else  // ENABLE_PLUGIN_CDFCONES
    is_unavailable("midpoint");
#endif // ENABLE_PLUGIN_CDFCONES
  } else if (cmdline.present("-pxcone")) {
#ifdef ENABLE_PLUGIN_PXCONE
    double min_jet_energy = 5.0;
    jet_def = fj::JetDefinition( new fj::PxConePlugin (
                                      ktR, min_jet_energy,
                                      overlap_threshold));
#else  // ENABLE_PLUGIN_PXCONE
    is_unavailable("pxcone");
#endif // ENABLE_PLUGIN_PXCONE
  } else if (cmdline.present("-jetclu")) {
#ifdef ENABLE_PLUGIN_CDFCONES
    jet_def = fj::JetDefinition( new fj::CDFJetCluPlugin (
                                      ktR, overlap_threshold, seed_threshold));
#else  // ENABLE_PLUGIN_CDFCONES
    is_unavailable("pxcone");
#endif // ENABLE_PLUGIN_CDFCONES
  } else if (cmdline.present("-siscone") || cmdline.present("-sisconespheri")) {
#ifdef ENABLE_PLUGIN_SISCONE
    typedef fj::SISConePlugin SISPlug; // for brevity
    int npass = cmdline.value("-npass",0);
    if (cmdline.present("-siscone")) {
      double sisptmin = cmdline.value("-sisptmin",0.0);
      SISPlug * plugin = new SISPlug (ktR, overlap_threshold,npass,sisptmin);
      if (cmdline.present("-sm-pt")) plugin->set_split_merge_scale(SISPlug::SM_pt);
      if (cmdline.present("-sm-mt")) plugin->set_split_merge_scale(SISPlug::SM_mt);
      if (cmdline.present("-sm-Et")) plugin->set_split_merge_scale(SISPlug::SM_Et);
      if (cmdline.present("-sm-pttilde")) plugin->set_split_merge_scale(SISPlug::SM_pttilde);
      // cause it to use the jet-definition's own recombiner
      plugin->set_use_jet_def_recombiner(true);
      jet_def = fj::JetDefinition(plugin);
    } else {
      double sisEmin = cmdline.value("-sisEmin",0.0);
      fj::SISConeSphericalPlugin * plugin = 
	new fj::SISConeSphericalPlugin(ktR, overlap_threshold,npass,sisEmin);
      if (cmdline.present("-ghost-sep")) {
	plugin->set_ghost_separation_scale(cmdline.value<double>("-ghost-sep"));
      }
      jet_def = fj::JetDefinition(plugin);
    }
#else  // ENABLE_PLUGIN_SISCONE
    is_unavailable("siscone");
#endif // ENABLE_PLUGIN_SISCONE
  } else if (cmdline.present("-d0runiicone")) {
#ifdef ENABLE_PLUGIN_D0RUNIICONE
    double min_jet_Et = 6.0; // was 8 GeV in earlier work
    jet_def = fj::JetDefinition(new fj::D0RunIIConePlugin(ktR,min_jet_Et));
#else  // ENABLE_PLUGIN_D0RUNIICONE
    is_unavailable("D0RunIICone");
#endif // ENABLE_PLUGIN_D0RUNIICONE
  } else if (cmdline.present("-trackjet")) {
#ifdef ENABLE_PLUGIN_TRACKJET
    jet_def = fj::JetDefinition(new fj::TrackJetPlugin(ktR));
#else  // ENABLE_PLUGIN_TRACKJET
    is_unavailable("TrackJet");
#endif // ENABLE_PLUGIN_TRACKJET
  } else if (cmdline.present("-atlascone")) {
#ifdef ENABLE_PLUGIN_ATLASCONE
    jet_def = fj::JetDefinition(new fj::ATLASConePlugin(ktR));
#else  // ENABLE_PLUGIN_ATLASCONE
    is_unavailable("ATLASCone");
#endif // ENABLE_PLUGIN_ATLASCONE
  } else if (cmdline.present("-eecambridge")) {
#ifdef ENABLE_PLUGIN_EECAMBRIDGE
    jet_def = fj::JetDefinition(new fj::EECambridgePlugin(ycut));
#else  // ENABLE_PLUGIN_EECAMBRIDGE
    is_unavailable("EECambridge");
#endif // ENABLE_PLUGIN_EECAMBRIDGE
// end of checking if one asks to run a plugin (don't delete this line)
  } else {
    cmdline.present("-kt"); // kt is default, but allow user to specify it too [and ignore return value!]
    jet_def = fj::JetDefinition(fj::kt_algorithm, ktR, strategy);
  }



  if (!cmdline.all_options_used()) {cerr << 
      "Error: some options were not recognized"<<endl; 
    exit(-1);}


  for (int iev = 0; iev < nev; iev++) {
  vector<fj::PseudoJet> jets;
  string line;
  int  ndone = 0;
  while (getline(cin, line)) {
      //cout << line<<endl;
    istringstream linestream(line);
    if (line == "#END") {
      ndone += 1;
      if (ndone == combine) {break;}
    }
    if (line.substr(0,1) == "#") {continue;}
    valarray<double> fourvec(4);
    if (hydjet) {
      // special reading from hydjet.txt event record (though actually
      // this is supposed to be a standard pythia event record, so
      // being able to read from it is perhaps not so bad an idea...)
      int ii, istat,id,m1,m2,d1,d2;
      double mass;
      linestream >> ii>> istat >> id >> m1 >> m2 >> d1 >> d2
		 >> fourvec[0] >> fourvec[1] >> fourvec[2] >> mass;
      // current file contains mass of particle as 4th entry
      if (istat == 1) {
	fourvec[3] = sqrt(+pow2(fourvec[0])+pow2(fourvec[1])
			  +pow2(fourvec[2])+pow2(mass));
      }
    } else {
      if (massless) {
	linestream >> fourvec[0] >> fourvec[1] >> fourvec[2];
	fourvec[3] = sqrt(pow2(fourvec[0])+pow2(fourvec[1])+pow2(fourvec[2]));}
      else {
	linestream >> fourvec[0] >> fourvec[1] >> fourvec[2] >> fourvec[3];
      }
    }
    fj::PseudoJet psjet(fourvec);
    if (abs(psjet.rap() < etamax)) {jets.push_back(psjet);}
  }

  // add a fake underlying event which is very soft, uniformly distributed
  // in eta,phi so as to allow one to reconstruct the area that is associated
  // with each jet.
  if (add_dense_coverage) {
    fj::GhostedAreaSpec ghosted_area_spec(ghost_maxrap);
    //fj::GhostedAreaSpec ghosted_area_spec(-2.0,4.0); // asymmetric range
    // for plots, reduce the scatter default of 1, to avoid "holes"
    // in the subsequent calorimeter view
    ghosted_area_spec.set_grid_scatter(0.5); 
    ghosted_area_spec.add_ghosts(jets);
    //----- old code ------------------
    // srand(2);
    // int nphi = 60;
    // int neta = 100;
    // double kt = 1e-1;
    // for (int iphi = 0; iphi<nphi; iphi++) {
    //   for (int ieta = -neta; ieta<neta+1; ieta++) {
    // 	double phi = (iphi+0.5) * (fj::twopi/nphi) + rand()*0.001/RAND_MAX;
    // 	double eta = ieta * (10.0/neta)  + rand()*0.001/RAND_MAX;
    // 	kt = 1e-20*(1+rand()*0.1/RAND_MAX);
    // 	double pminus = kt*exp(-eta);
    // 	double pplus  = kt*exp(+eta);
    // 	double px = kt*sin(phi);
    // 	double py = kt*cos(phi);
    // 	//cout << kt<<" "<<eta<<" "<<phi<<"\n";
    // 	fj::PseudoJet mom(px,py,0.5*(pplus-pminus),0.5*(pplus+pminus));
    // 	jets.push_back(mom);
    //   }
    // }
  }
  
  for (int irepeat = 0; irepeat < repeat ; irepeat++) {
    int nparticles = jets.size();
    try {
    fj::ClusterSequence clust_seq(jets,jet_def,write);
    if (irepeat != 0) {continue;}
    cout << "iev "<<iev<< ": number of particles = "<< nparticles << endl;
    cout << "strategy used =  "<< clust_seq.strategy_string()<< endl;
    cout << "Algorithm: " << jet_def.description() << " (" << fj::fastjet_version_string() << ")" << endl;

    // now provide some nice output...
    if (inclkt >= 0.0) {
      vector<fj::PseudoJet> jets = sorted_by_pt(clust_seq.inclusive_jets(inclkt));
      print_jets(jets, clust_seq, show_constituents);
      if (rootfile != "") {
        ofstream ostr(rootfile.c_str());
        ostr << "# " << cmdline.command_line() << endl;
        ostr << "# output for root" << endl;
        clust_seq.print_jets_for_root(jets,ostr);
      }

    }

    if (excln > 0) {
      cout << "Printing "<<excln<<" exclusive jets\n";
      print_jets(clust_seq.exclusive_jets(excln), clust_seq, show_constituents);
    }

    if (excld > 0.0) {
      cout << "Printing exclusive jets for d = "<<excld<<"\n";
      print_jets(clust_seq.exclusive_jets(excld), clust_seq, show_constituents);
    }

    if (excly > 0.0) {
      cout << "Printing exclusive jets for ycut = "<<excly<<"\n";
      print_jets(clust_seq.exclusive_jets_ycut(excly), clust_seq, show_constituents);
    }

    if (get_all_dij) {
      for (int i = nparticles-1; i >= 0; i--) {
        printf("d for n = %4d -> %4d is %14.5e\n", i+1, i, clust_seq.exclusive_dmerge(i));
      }
    }
    if (get_all_yij) {
      for (int i = nparticles-1; i >= 0; i--) {
        printf("y for n = %4d -> %4d is %14.5e\n", i+1, i, clust_seq.exclusive_ymerge(i));
      }
    }

    // have the option of printing out the subjets (at scale dcut) of
    // each inclusive jet
    if (subdcut >= 0.0) {
      print_jets_and_sub(clust_seq, clust_seq.inclusive_jets(), subdcut);
    }
    
    // useful for testing that recombination sequences are unique
    if (unique_write) {
      vector<int> unique_history = clust_seq.unique_history_order();
      // construct the inverse of the above mapping
      vector<int> inv_unique_history(clust_seq.history().size());
      for (unsigned int i = 0; i < unique_history.size(); i++) {
	inv_unique_history[unique_history[i]] = i;}

      for (unsigned int i = 0; i < unique_history.size(); i++) {
	fj::ClusterSequence::history_element el = 
	  clust_seq.history()[unique_history[i]];
	int uhp1 = el.parent1>=0 ? inv_unique_history[el.parent1] : el.parent1;
	int uhp2 = el.parent2>=0 ? inv_unique_history[el.parent2] : el.parent2;
	printf("%7d u %15.8e %7d u %7d u\n",i,el.dij,uhp1, uhp2);
      }
    }


#ifdef ENABLE_PLUGIN_SISCONE
    // provide some complementary information for SISCone 
    if (show_cones) {
      const fj::SISConeExtras * extras = 
        dynamic_cast<const fj::SISConeExtras *>(clust_seq.extras());
      cout << "most ambiguous split (difference in squared dist) = "
           << extras->most_ambiguous_split() << endl;
      vector<fastjet::PseudoJet> stable_cones(extras->stable_cones()); 
      stable_cones = sorted_by_rapidity(stable_cones);
      for (unsigned int i = 0; i < stable_cones.size(); i++) {
      //if (stable_cones[i].phi() < 5.0 && stable_cones[i].phi() > 4.0) {
        printf("%5u %15.8f %15.8f %15.8f\n",
               i,stable_cones[i].rap(),stable_cones[i].phi(),
               stable_cones[i].perp() );
      //}
      }
      
      // also show passes for jets
      vector<fj::PseudoJet> sisjets = clust_seq.inclusive_jets();
      printf("\n%15s %15s %15s %12s %8s %8s\n","rap","phi","pt","user-index","pass","nconst");
      for (unsigned i = 0; i < sisjets.size(); i++) {
        printf("%15.8f %15.8f %15.8f %12d %8d %8d\n",
               sisjets[i].rap(), sisjets[i].phi(), sisjets[i].perp(), 
	       sisjets[i].user_index(), extras->pass(sisjets[i]),
	       clust_seq.constituents(sisjets[i]).size()
	       );
	
      }
    }
#endif // ENABLE_PLUGIN_SISCONE
  } // try
  catch (fastjet::Error fjerr) {
    cout << "Caught fastjet error, exiting gracefully" << endl;
    exit(0);
  }

  } // irepeat
  } // iev

  // if we've instantiated a plugin, delete it
  if (jet_def.strategy()==fj::plugin_strategy){
    delete jet_def.plugin();
  }
}




//------ HELPER ROUTINES -----------------------------------------------
/// print a single jet
void print_jet (const fj::ClusterSequence & clust_seq, 
                const fj::PseudoJet & jet) {
  int n_constituents = clust_seq.constituents(jet).size();
  printf("%15.8f %15.8f %15.8f %8u\n",
         jet.rap(), jet.phi(), jet.perp(), n_constituents);
}


//----------------------------------------------------------------------
void print_jets(const vector<fj::PseudoJet> & jets_in, const fj::ClusterSequence & cs, bool show_constituents) {
  vector<fj::PseudoJet> jets;
  if (ee_print) {
    jets = sorted_by_E(jets_in);
    for (size_t j = 0; j < jets.size(); j++) {
      printf("%5u %15.8f %15.8f %15.8f %15.8f\n",
	     j,jets[j].px(),jets[j].py(),jets[j].pz(),jets[j].E());
      if (show_constituents) {
	vector<fj::PseudoJet> const_jets = cs.constituents(jets[j]);
	for (size_t k = 0; k < const_jets.size(); k++) {
	  printf("        jet%03u %15.8f %15.8f %15.8f %15.8f\n",j,const_jets[k].px(),
		 const_jets[k].py(),const_jets[k].pz(),const_jets[k].E());
	}
	cout << "\n\n";
    }

    }
  } else {
    jets = sorted_by_pt(jets_in);
    for (size_t j = 0; j < jets.size(); j++) {
      printf("%5u %15.8f %15.8f %15.8f\n",
	     j,jets[j].rap(),jets[j].phi(),jets[j].perp());

      if (show_constituents) {
	vector<fj::PseudoJet> const_jets = cs.constituents(jets[j]);
	for (size_t k = 0; k < const_jets.size(); k++) {
	  printf("        jet%03u %15.8f %15.8f %15.8f\n",j,const_jets[k].rap(),
		 const_jets[k].phi(),sqrt(const_jets[k].kt2()));
	}
	cout << "\n\n";
      }
    }
  }

}


//----- SUBJETS --------------------------------------------------------
/// a function that pretty prints a list of jets and the subjets for each
/// one
void print_jets_and_sub (fj::ClusterSequence & clust_seq, 
                         const vector<fj::PseudoJet> & jets, double dcut) {

  // sort jets into increasing pt
  vector<fj::PseudoJet> sorted_jets = sorted_by_pt(jets);  

  // label the columns
  printf("Printing jets and their subjets with subdcut = %10.5f\n",dcut);
  printf("%5s %15s %15s %15s %15s\n","jet #", "rapidity", 
	 "phi", "pt", "n constituents");

  // have various kinds of subjet finding, to test consistency among them
  enum SubType {internal, newclust_dcut, newclust_R};
  SubType subtype = internal;
  //SubType subtype = newclust_dcut;

  // print out the details for each jet
  for (unsigned int i = 0; i < sorted_jets.size(); i++) {
    // if jet pt^2 < dcut with kt alg, then some methods of
    // getting subjets will return nothing -- so skip the jet
    if (clust_seq.jet_def().jet_algorithm() == fj::kt_algorithm 
        && sorted_jets[i].perp2() < dcut) continue;

    printf("%5u       ",i);
    print_jet(clust_seq, sorted_jets[i]);
    vector<fj::PseudoJet> subjets;
    fj::ClusterSequence * cspoint;
    if (subtype == internal) {
      cspoint = &clust_seq;
      subjets = clust_seq.exclusive_subjets(sorted_jets[i], dcut);
      //subjets = clust_seq.exclusive_subjets(sorted_jets[i], 5);
      //double dd = clust_seq.exclusive_subdmerge_max(sorted_jets[i], 3);
      //subjets = clust_seq.exclusive_subjets(sorted_jets[i], dd*1.0000001);
    } else if (subtype == newclust_dcut) {
      cspoint = new fj::ClusterSequence(clust_seq.constituents(sorted_jets[i]),
                                        clust_seq.jet_def());
      subjets = cspoint->exclusive_jets(dcut);
      //subjets = cspoint->exclusive_jets(int(min(5U,cspoint->n_particles())));
    } else if (subtype == newclust_R) {
      assert(clust_seq.jet_def().jet_algorithm() == fj::cambridge_algorithm);
      fj::JetDefinition subjd(clust_seq.jet_def().jet_algorithm(), 
                              clust_seq.jet_def().R()*sqrt(dcut));
      cspoint = new fj::ClusterSequence(clust_seq.constituents(sorted_jets[i]),
                                        subjd);
      subjets = cspoint->inclusive_jets();
    } else {
      cerr << "unrecognized subtype for subjet finding" << endl;
      exit(-1);
    }

    subjets = sorted_by_pt(subjets);
    for (unsigned int j = 0; j < subjets.size(); j++) {
      printf("    -sub-%02u ",j);
      print_jet(*cspoint, subjets[j]);
    }

    if (cspoint != &clust_seq) delete cspoint;

    //fj::ClusterSequence subseq(clust_seq.constituents(sorted_jets[i]),
    //                          fj::JetDefinition(fj::cambridge_algorithm, 0.4));
    //vector<fj::PseudoJet> subjets = sorted_by_pt(subseq.inclusive_jets());
    //for (unsigned int j = 0; j < subjets.size(); j++) {
    //  printf("    -sub-%02u ",j);
    //  print_jet(subseq, subjets[j]);
    //}
  }

}


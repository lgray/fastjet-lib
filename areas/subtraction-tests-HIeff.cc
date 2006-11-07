
//STARTHEADER
// $Id: subtraction-tests-inclpt.cc 286 2006-08-17 17:49:07Z salam $
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
/// subtraction-tests-HIeff.cc: 
///   Program to help study question of efficiency, purity and accuaracy
///   of jet-finding in HI collisions.
///             
/// 
/// It reads files containing multiple events in the format 
///
///    p1x p1y p1z E1
///    p2x p2y p2z E2
///    #HARDEND
///    p1x p1y p1z E1
///    p2x p2y p2z E2
///    #HARDEND
///    p1x p1y p1z E1
///    p2x p2y p2z E2
///    #HARDEND
///    p1x p1y p1z E1
///    p2x p2y p2z E2
///    ...
///    #END
/// 
/// where each #HARDEND line indicates the end of a hard sub-event
///
/// OBSOLETE instructions:
/// ----------------------
/// Usage:
///   fastjet_timing [-strategy NUMBER] [-repeat nrepeats] [-massive] \
///                  [-combine nevents] [-r Rparameter] [-incl ptmin] [...] \
///                  < data_file
///
/// where the clustering can be repeated to aid timing and multiple
/// events can be combined to get to larger multiplicities. Some options:
///
///   -strategy N   indicate stratgey from the enum fj::Strategy (see
///                 fj::ClusterSequence.hh).
///
///   -combine nev  for combining multiple events from the data file in order
///                 to get to large multiplicities.
///
///   -incl ptmin   output of all inclusive jets with pt > ptmin is obtained
///                 with the -incl option.
///
///   -excld dcut   output of all exclusive jets as obtained in a clustering
///                 with dcut
///
///   -massless     read in only the 3-momenta and deduce energies assuming
///                 that particles are massless
///
///   -write        for writing out detailed clustering sequence (valuable
///                 for testing purposes)
///
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceActiveArea.hh"
#include<iostream>
#include<sstream>
#include<fstream>
#include<valarray>
#include<vector>
#include <cstdlib>
#include<cstddef> // for size_t
#include "CmdLine.hh"
#include "SimpleHist.hh"


// for getting cone algorithm from CDF
#include "MidPointAlgorithm.hh"
#include "PhysicsTower.hh"
#include "Cluster.hh"


namespace fj = fastjet;
using namespace std;

inline double pow2(const double x) {return x*x;};

void print_cnst(const fj::ClusterSequence & cs, const fj::PseudoJet & jet) {
  vector<fj::PseudoJet> cnst = cs.constituents(jet);
  for (size_t i = 0; i < cnst.size(); i++) {
    printf("%6i %18.5f %18.5f %18.6e\n",i,cnst[i].rap(),cnst[i].phi(),cnst[i].perp());
  }
  printf("#END\n");
};

void print_jet(const fj::PseudoJet & jet) {
  printf("%18.5f %18.5f %18.6e\n",jet.rap(),jet.phi(),jet.perp());
};



// enum ConeVariant {not_cone, midpoint_050, midpoint_075, searchcone_075};

// void determine_Zmass_cone(const vector<fj::PseudoJet> & event, 
// 			  double R, ConeVariant cone_variant,
// 			  double & mass, double & corrected_mass);

// double Zmass_from_jets(const vector<fj::PseudoJet> & jets);

void read_event(istream &, double, bool, bool, double,
		vector<vector<fj::PseudoJet> >&, vector<fj::PseudoJet> &,
                string &);

// handy shorthands
typedef vector<fj::PseudoJet> jet_vector;

void reorder_jets(jet_vector & ref_jets, 
		  jet_vector & other_jets, 
                  double max_dist,
                  unsigned int & n);


//----------------------------------------------------------------------
/// a program to test and time the kt algorithm as implemented in fastjet
int main (int argc, char ** argv) {

  CmdLine cmdline(argc,argv);
  // allow the use to specify the fj::Strategy either through the
  // -clever or the -strategy options (both will take numerical
  // values); the latter will override the former.
  fj::Strategy  strategy  = fj::Strategy(cmdline.int_val("-strategy",
				     cmdline.int_val("-clever", fj::Best)));
  int  repeat  = cmdline.int_val("-repeat",1);
  //bool writeout   = cmdline.present("-write");
  bool hydjet  = cmdline.present("-hydjet");
  double ktR   = cmdline.double_val("-r",1.0);
  //double inclkt = cmdline.double_val("-incl",-1.0);
  //int    excln  = cmdline.int_val   ("-excln",-1);
  //double excld  = cmdline.double_val("-excld",-1.0);
  double etamax = cmdline.double_val("-etamax",1.0e310);
  bool   massless = cmdline.present("-massless");
  int    nev      = int(cmdline.double_val("-nev",1.0));
  bool   nopileup  = cmdline.present("-nopileup"); 
  double ghost_area = cmdline.double_val("-ghost_area",cmdline.double_val("-cell_area",0.01));
  double ghost_etamax = cmdline.double_val("-ghost_etamax",6.0);
  double grid_scatter = cmdline.double_val("-grid_scatter",0.0001);
  double kt_scatter   = cmdline.double_val("-kt_scatter",0.1);
  // options related to histogramming
  double pt_bin_width    = cmdline.double_val("-ptbinwidth",  10.0);
  double pt_min_bin      = cmdline.double_val("-ptminbin",  20.0);
  double pt_max_bin      = cmdline.double_val("-ptmaxbin", 100.0);
  // options related to matching
  double max_rapphi_dist = cmdline.double_val("-maxrapphi",ktR);
  // we will study only those jets that are below maxrap
  double maxrap = cmdline.double_val("-maxrap",2.0);
  //bool   print_jets   = cmdline.present("-print_jets");
  string input_file   = cmdline.string_val("-in");
  string output_file  = cmdline.string_val("-out");

  double discard_below_pt = cmdline.double_val("-discard",-1.0);

  //ConeVariant cone_variant = not_cone;
  //if (cmdline.present("-searchcone")) {
  //  cone_variant = searchcone_075; }
  //else if (cmdline.present("-cone")) {
  //  cone_variant = midpoint_050; }
  //else if (cmdline.present("-cone075")) {
  //  cone_variant = midpoint_075; }
  //bool   cone         = cone_variant != not_cone;
  int    writefreq    = int(cmdline.double_val("-freq",1.0*max(nev/10,1000)));
  string rerun_string = cmdline.string_val("-rerun","");
  cerr <<"writefreq is "<<writefreq<<endl;
  fj::JetFinder jet_finder = cmdline.present("-cam") ? 
                                fj::cambridge_algorithm : fj::kt_algorithm;

  if (!cmdline.all_options_used()) {cerr << 
      "Error: some options unsupported"<<endl; 
    exit(-1);}

  // create the definitions for our jet finder and areas spec...
  fj::JetDefinition jet_def(jet_finder, ktR, strategy);
  fj::ActiveAreaSpec active_area_spec(ghost_etamax, repeat, ghost_area, 
                                      grid_scatter, kt_scatter);


  // input will be from the file named with the "-in" option
  ifstream input(input_file.c_str());

  int nptbins = int(0.5+(pt_max_bin-pt_min_bin)/pt_bin_width);
  // number of true jets in each pt range
  SimpleHist pt_true_entries(pt_min_bin,pt_max_bin,nptbins);
  // number of fake reconstructed (i.e. unmatched) jets in each pt range
  SimpleHist pt_fake_entries(pt_min_bin,pt_max_bin,nptbins);
  // number of true jets in each pt range that never got matched with 
  // a reconstructed one.
  SimpleHist pt_lost_entries(pt_min_bin,pt_max_bin,nptbins);
  // spreads in pt and rap-phi distance -- note the nptbins+1 limit 
  // to allow for "outflow" bins
  vector<SimpleHist> pt_offsets(nptbins+1), rapphi_offsets(nptbins+1);

  for(int ipt = 0; ipt <= nptbins; ipt++) {
    pt_offsets[ipt].declare(-35.0,35.0,35);
    rapphi_offsets[ipt].declare(0.0,min(1.0,max_rapphi_dist),25);
  }

  string input_description;

  for (int iev = 0; iev < nev; iev++) {
    if (iev < 100 || iev%100 == 0) cerr << "Doing event "<< iev<<endl;
    vector<fj::PseudoJet> full_event;
    vector<vector<fj::PseudoJet> > hard_events;
    
    // read in the event 
    read_event(input, etamax, hydjet, massless, discard_below_pt, 
               hard_events, full_event, input_description); 
      
    cout << "Event size = " << full_event.size() << endl;

    //-- run the jet finder on the full event ----------------------
    fj::ClusterSequenceActiveArea full_seq(full_event, jet_def, 
                                           active_area_spec);
    vector<fj::PseudoJet> full_jets = full_seq.inclusive_jets();
    double a, b;
    full_seq.parabolic_pt_per_unit_area(a,b);
    vector<fj::PseudoJet> full_corrected_jets;
    typedef vector<fj::PseudoJet>::const_iterator jet_iter;
    // get a vector of corrected jets (only those whose corrected pt
    // > 10 GeV).
    for (jet_iter jet = full_jets.begin(); jet != full_jets.end(); jet++) {
      if (abs(jet->rap()) < active_area_spec.ghost_etamax() - 1.0) {
        double rho_at_rap = a + b*pow2(jet->rap());
        fj::PseudoJet area = full_seq.area_4vector(*jet);
        if (jet->perp() > rho_at_rap*area.perp()) {
          fj::PseudoJet corrected_jet = *jet - rho_at_rap*area;
          if (corrected_jet.perp() > 10.0) {
            full_corrected_jets.push_back(corrected_jet);}
        }
      }
    }


    //-- run the jet finder on individual hard events ----------------------
    vector<fj::PseudoJet> hard_jets;
    for (unsigned ihard = 0; ihard < hard_events.size();  ihard++) {
      fj::ClusterSequence hard_seq(hard_events[ihard], jet_def);
      vector<fj::PseudoJet> event_jets = hard_seq.inclusive_jets(10.0);
      copy(event_jets.begin(), event_jets.end(), back_inserter(hard_jets));
    }

    //-- then put the jets into some sensible common order...
    hard_jets = sorted_by_pt(hard_jets);
    full_corrected_jets = sorted_by_pt(full_corrected_jets);
    
    unsigned  nmatch;
    reorder_jets(hard_jets, full_corrected_jets, max_rapphi_dist, nmatch);

    //// --- print out some info
    //cout << "Matched "<<nmatch<<" jets"<< endl;
    //for (unsigned i = 0; i < nmatch; i++) {
    //  cout << i << " " << hard_jets[i].perp() << " " << full_corrected_jets[i].perp() - hard_jets[i].perp() << " " << hard_jets[i].squared_distance(full_corrected_jets[i]) << endl ; 
    //}
    //cout << " +++++++ " << endl;
    //
    //for (jet_iter jet = hard_jets.begin(); jet != hard_jets.end(); jet++) {
    //  print_jet(*jet);}
    //cout << " ----- " << endl;
    //for (jet_iter jet = full_corrected_jets.begin(); 
    //              jet != full_corrected_jets.end(); jet++) {
    //  print_jet(*jet);}

    //------ now do some proper analysis ----------
    for (unsigned i = 0; i < hard_jets.size(); i++) {
      if (hard_jets[i].perp() < pt_min_bin 
          || abs(hard_jets[i].rap()) > maxrap) {continue;}
      unsigned int ipt = pt_true_entries.bin(hard_jets[i].perp());
      pt_true_entries[ipt]++;
      if (i < nmatch) {
        pt_offsets[ipt].add_entry(full_corrected_jets[i].perp() 
                                                   - hard_jets[i].perp());
        rapphi_offsets[ipt].add_entry(sqrt(hard_jets[i].squared_distance(
                                                   full_corrected_jets[i])));
      } else {
        pt_lost_entries[ipt]++;
      }
    }
    for (unsigned i = nmatch; i < full_corrected_jets.size(); i++) {
      if (full_corrected_jets[i].perp() < pt_min_bin  
          || abs(full_corrected_jets[i].rap()) > maxrap) {continue;}
      pt_fake_entries.add_entry(full_corrected_jets[i].perp());
    }


    // sending output to a file...
    if ( iev+1==nev || (iev+1) % writefreq == 0) {
    ofstream output(output_file.c_str());
    if (rerun_string != "") {
      output << "# Rerun with:\n";
      output << "# "<<rerun_string<<endl;
    }
    output << "# " << cmdline.command_line() << endl;
    output << "# input was taken from ---------------------" << endl;
    output << input_description;
    output << "# ------------------------------------------" << endl;
    output << "# " << jet_def.description() << endl;
    output << "# max rap-phi distance (for matching jets) = " << max_rapphi_dist << endl;
    output << "# maxrap (for studying jets) = " << maxrap << endl;
    output << "# nev = " <<iev+1 <<endl;
    for(unsigned int ipt = 0; ipt < pt_true_entries.outflow_size(); ipt++) {
      double binhi = ipt >= pt_true_entries.size() ? 100000.0 : 
        pt_true_entries.binhi(ipt);
      output << "# index = "<< ipt*2 << " ; "
             << "pt range = " << pt_true_entries.binlo(ipt) 
             << " - " << binhi << " ; "
             << "  true = " << pt_true_entries[ipt]
             << "  fake = " << pt_fake_entries[ipt]
             << "  lost = " << pt_lost_entries[ipt] << endl
             << "# ptoffset distribution" << endl;
      for (unsigned ibin = 0; ibin < pt_offsets[ipt].size(); ibin++) {
        output << pt_offsets[ipt].binlo(ibin) << " " 
               << pt_offsets[ipt].binmid(ibin) << " "
               << pt_offsets[ipt].binhi(ibin) << " "
               << pt_offsets[ipt][ibin]/pt_true_entries[ipt] << endl;
      }
      output << endl << endl;
      output << "# index = "<< ipt*2+1 
             << " rap-phi offset distribution " << endl;
      for (unsigned ibin = 0; ibin < rapphi_offsets[ipt].size(); ibin++) {
        output << rapphi_offsets[ipt].binlo(ibin) << " " 
               << rapphi_offsets[ipt].binmid(ibin) << " "
               << rapphi_offsets[ipt].binhi(ibin) << " "
               << rapphi_offsets[ipt][ibin]/pt_true_entries[ipt] << endl;
      }
      output << endl << endl;
      
    }
    }

  } // iev
  
  

}




//----------------------------------------------------------------------
/// reorder jets so that those in the second list are matched to
/// those in the reference list to which they are closest in rap-phi.
/// 
/// The reference jets are assumed to be provided in some meaningful
/// order (e.g. decreasing pt) -- initially, the first reference jet
/// is paired off, then the second reference one, etc.
///
/// The number of jets that have been paired is returned in "n".
/// 
/// Jets are only paired if they are closer than max_dist apart, and
/// if the pairing corresponds to the closest possible interpair
/// distance for the given reference jet.
///
/// Reference jet and "other" jets that have not been successfully
/// paired are moved to the end of their respective vectors (at
/// positions [n...]).
///
/// The algorithm used scales as N^2.
void reorder_jets(jet_vector & ref_jets, 
		  jet_vector & other_jets, 
                  double max_dist,
                  unsigned int & n) {
  
  // place reasonable upper bound on n (maxmimum number of paired
  // jets)
  int n_ref = ref_jets.size();
  int n_other = other_jets.size();
  n = min(n_ref, n_other);

  double max_dist2 = pow2(max_dist);

  // now search for correspondences
  for (unsigned ir = 0; ir < n; ir++) {
    // first find the closest of the remaining "other" jets
    for (unsigned io = ir+1; io < n; io++) {
      if (ref_jets[ir].squared_distance(other_jets[io]) <
	  ref_jets[ir].squared_distance(other_jets[ir])) {
	swap(other_jets[io],other_jets[ir]);
      }
    }
    // now decide if we accept the choice
    double dist2 = ref_jets[ir].squared_distance(other_jets[ir]);
    bool accept;
    // first make sure we're "close enough"
    accept = dist2 < max_dist2;
    // then make sure no already-paired other jet is closer
    for (unsigned iop = 0; iop < ir ; iop++) {
      accept = accept && ref_jets[ir].squared_distance(other_jets[iop]) >= dist2;
    }
    // if the pairing is not accepted
    if (! accept) {
      // reduce total number of planned pairings 
      n--; 
      // move this reference jet to the end, while shifting everything
      // else up
      fj::PseudoJet tmp = ref_jets[ir];
      for (int irp = ir; irp < n_ref-1; irp++) {
        ref_jets[irp] = ref_jets[irp+1];}
      ref_jets[n_ref-1] = tmp;
      // the number of available reference jets has to be brought down
      n_ref--;
      // the position in the reference jets list needs to be adjusted.
      ir--;
    }
  }
}




//======================================================================
void read_event(istream & input, double etamax, bool hydjet, bool massless,
		double discard_below_pt,
		vector<vector<fj::PseudoJet> > & hard_events, 
		vector<fj::PseudoJet> & full_event, 
                string & input_description) {
  string line;
  int  nsub  = 0;
  vector<fj::PseudoJet> sub_event;

  static bool first_go = true;

  while (getline(input, line)) {
      //cout << line<<endl;
    istringstream linestream(line);
    if (line == "#END") {break;}
    if (line == "#HARDEND") {
      hard_events.push_back(sub_event);
      copy(sub_event.begin(), sub_event.end(), back_inserter(full_event));
      sub_event.resize(0);
      nsub += 1;
    }
    if (line.substr(0,1) == "#") {
      if (first_go) input_description += line + "\n";
      continue;}
    first_go = false;
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
    psjet.set_user_index(0);
    if (abs(psjet.rap() < etamax) && psjet.perp() >= discard_below_pt) {
      sub_event.push_back(psjet);}
  }

  copy(sub_event.begin(), sub_event.end(), back_inserter(full_event));

  //// if we have read in only one event, copy it across here...
  //if (nsub == 1) hard_event = full_event;

  // if there was nothing in the event 
  if (nsub == 0) {
    cerr << "Error: read empty event\n";
    exit(-1);
  }
}


//STARTHEADER
// $Id$
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
/// areas.cc: Program to help work out areas of jets
/// 
/// It reads files containing multiple events in the format 
/// p1x p1y p1z E1
/// p2x p2y p2z E2
/// ...
/// #END
/// 
/// An example input file containing 10 events is included as 
/// ../data/Pythia-PtMin1000-LHC-10ev.dat
///
/// Usage:
///   fastjet_timing [-strategy NUMBER] [-repeat nrepeats] [-massive] \
///                  [-combine nevents] [-r Rparameter] [-incl ptmin] [...] \
///                  < data_file
///
/// where the clustering can be repeated to aid timing and multiple
/// events can be combined to get to larger multiplicities. Some options:
///
///   -strategy N   indicate stratgey from the enum FjStrategy (see
///                 FjClusterSequence.hh).
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
#include "FjPseudoJet.hh"
#include "FjClusterSequence.hh"
#include "FjClusterSequenceWithMeanArea.hh"
#include<iostream>
#include<sstream>
#include<fstream>
#include<valarray>
#include<vector>
#include <cstdlib>
#include<cstddef> // for size_t
#include "CmdLine.hh"
#include "CSHisto.hh"


// for getting cone algorithm from CDF
#include "MidPointAlgorithm.hh"
#include "PhysicsTower.hh"
#include "Cluster.hh"


using namespace std;

inline double pow2(const double x) {return x*x;};

void print_jet(const FjClusterSequence & cs, const FjPseudoJet & jet) {
  vector<FjPseudoJet> cnst = cs.constituents(jet);
  for (size_t i = 0; i < cnst.size(); i++) {
    printf("%6i %18.5f %18.5f %18.6e\n",i,cnst[i].rap(),cnst[i].phi(),cnst[i].perp());
  }
  printf("#END\n");
};


void fill_inclpt_ktalg(const vector<FjPseudoJet> & event, 
		       double cell_area, double ghost_etamax,
		       double grid_scatter, double kt_scatter, int repeat,
		       double ktR, FjStrategy strategy,
		       CSHisto & , CSHisto & , CSHisto &  , CSHisto & 
		       );

// enum ConeVariant {not_cone, midpoint_050, midpoint_075, searchcone_075};

// void determine_Zmass_cone(const vector<FjPseudoJet> & event, 
// 			  double R, ConeVariant cone_variant,
// 			  double & mass, double & corrected_mass);

// double Zmass_from_jets(const vector<FjPseudoJet> & jets);

void read_event(istream &, double, bool, bool, double,
		vector<FjPseudoJet> &, vector<FjPseudoJet> & );


//----------------------------------------------------------------------
/// a program to test and time the kt algorithm as implemented in fastjet
int main (int argc, char ** argv) {

  CmdLine cmdline(argc,argv);
  // allow the use to specify the FjStrategy either through the
  // -clever or the -strategy options (both will take numerical
  // values); the latter will override the former.
  FjStrategy  strategy  = FjStrategy(cmdline.int_val("-strategy",
				     cmdline.int_val("-clever", Best)));
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
  double cell_area = cmdline.double_val("-cell_area",0.01);
  double ghost_etamax = cmdline.double_val("-ghost_etamax",6.0);
  double grid_scatter = cmdline.double_val("-grid_scatter",0.00001);
  double kt_scatter   = cmdline.double_val("-kt_scatter",0.1);
  // options related to histogramming
  double bin_width    = cmdline.double_val("-bin",5.0);
  double min_bin      = cmdline.double_val("-min",-100.0);
  double max_bin      = cmdline.double_val("-max", 400.0);
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
  if (cmdline.present("-cam")) {FjClusterSequence::set_jet_finder(cambridge_algorithm);}

  if (!cmdline.all_options_used()) {cerr << 
      "Error: some options unsupported"<<endl; 
    exit(-1);}

  // input will be from the file named with the "-in" option
  ifstream input(input_file.c_str());

  int nbins = int((max_bin-min_bin)/bin_width + 0.5);
  CSHisto inclpt_hard(min_bin, max_bin, nbins);
  CSHisto inclpt_hcor(min_bin, max_bin, nbins);
  CSHisto inclpt_hcrp(min_bin, max_bin, nbins);
  CSHisto inclpt_hcrp_xcl(min_bin, max_bin, nbins);
  CSHisto inclpt_full(min_bin, max_bin, nbins);
  CSHisto inclpt_fcor(min_bin, max_bin, nbins);
  CSHisto inclpt_fcrp(min_bin, max_bin, nbins);
  CSHisto inclpt_fcrp_xcl(min_bin, max_bin, nbins);


  for (int iev = 0; iev < nev; iev++) {
    if (iev < 100 || iev%100 == 0) cerr << "Doing event "<< iev<<endl;
    vector<FjPseudoJet> hard_event, full_event;
    
    // read in the event 
    read_event(input, etamax, hydjet, massless, discard_below_pt, hard_event, full_event); 
      
    //cout << "Event sizes: "<<hard_event.size()<<" "<<full_event.size()<<endl;

    // dumb it down if need be...
    if (nopileup)  full_event = hard_event;
    
    // fill histograms with inclusive jet cross sections for
    // just the hard event
    fill_inclpt_ktalg(hard_event,
	   cell_area,ghost_etamax, grid_scatter, kt_scatter, 
	   repeat, ktR, strategy, inclpt_hard, inclpt_hcor, 
		      inclpt_hcrp, inclpt_hcrp_xcl);


    // fill histograms with inclusive jet cross sections for
    // the full event
    if (full_event.size() != hard_event.size()) {
      fill_inclpt_ktalg(full_event,
		cell_area,ghost_etamax, grid_scatter, kt_scatter, 
		repeat, ktR, strategy, inclpt_full, inclpt_fcor, 
			inclpt_fcrp, inclpt_fcrp_xcl);
    }

    // limit the amount of information that is output 
    if (iev < 100) {
      //// provide user with some info (maybe get rid of this at some point?)
      //cout <<"inv mass of two hardest (hard) jets = "<< hard_ev_mass << endl;
      //cout <<"inv mass of two hardest (hcor) jets = "<< hcor_ev_mass << endl;
      //cout <<"inv mass of two hardest (hecr) jets = "<< hecr_ev_mass << endl;
      //cout <<"inv mass of two hardest (full) jets = "<< full_ev_mass << endl;
      //cout <<"inv mass of two hardest (fcor) jets = "<< fcor_ev_mass << endl;
      //cout <<"inv mass of two hardest (fecr) jets = "<< fecr_ev_mass << endl;
    }

    
    // write intermediate and final results...
    if ( iev+1==nev || (iev+1) % writefreq == 0) {
      // sending output to a file...
      ofstream output(output_file.c_str());
      if (rerun_string != "") {
	output << "# Rerun with:\n";
	output << "# "<<rerun_string<<endl;
      }
      output << "# " << cmdline.command_line() << endl;
      output << "# nev = " <<iev+1 <<endl;
      output << "# discarded particles with pt < "<<discard_below_pt<<endl;
      output << "# bin-lower-edge(1) bin-centre(2) bin-upper-edge(3) hard(4) hcor(5) hcrp(6) full(7) fcor(8) fcrp(9) hcrp_xcl(10) fcrp_xcl(11)\n";
      
      // print out inclusive-pt histograms.
      for (unsigned i = 0; i < inclpt_hard.size(); i++) {
	output << inclpt_hard.bin_lower_edge(i) <<" "
	       << inclpt_hard.bin_centre(i) <<" "
	       << inclpt_hard.bin_upper_edge(i) <<" "
	       << inclpt_hard.bin_weight(i)/((iev+1)*bin_width) <<" "
	       << inclpt_hcor.bin_weight(i)/((iev+1)*bin_width) <<" "
	       << inclpt_hcrp.bin_weight(i)/((iev+1)*bin_width) <<" "
	       << inclpt_full.bin_weight(i)/((iev+1)*bin_width) <<" "
	       << inclpt_fcor.bin_weight(i)/((iev+1)*bin_width) <<" "
	       << inclpt_fcrp.bin_weight(i)/((iev+1)*bin_width) <<" "
	       << inclpt_hcrp_xcl.bin_weight(i)/((iev+1)*bin_width) <<" "
	       << inclpt_fcrp_xcl.bin_weight(i)/((iev+1)*bin_width) <<endl;
      }
    }
    
  } // iev
  
  
}




//======================================================================
void fill_inclpt_ktalg(const vector<FjPseudoJet> & event, 
			double cell_area, double ghost_etamax,
			double grid_scatter, double kt_scatter, int repeat,
			double ktR, FjStrategy strategy,
			CSHisto & inclpt_uncorrected, 
		        CSHisto & inclpt_corrected,
		        CSHisto & inclpt_corrected_withrap, 
			CSHisto & inclpt_corrected_withrap_xcl) {

  // sometimes (with the -discard flag) it happens that you discard
  // _all_ particles in the event. In such a case don't even think of
  // running fastjet...
  if (event.size() == 0) {return;}

  FjClusterSequenceWithMeanArea clust(event,
				      cell_area,ghost_etamax,
				      grid_scatter, kt_scatter, repeat,
				      ktR,strategy);

  double median_pt_per_area = clust.pt_per_unit_area();
  
  double a,b;
  clust.parabolic_pt_per_unit_area(a,b);
  //cout << "a,b =" << a <<" " <<b<<endl;

  double a_xcl,b_xcl;
  // -1.0 here signifies default rapidity range and we exclude things
  // that have pt/area harder than twice the median_pt_per_area.
  clust.parabolic_pt_per_unit_area(a_xcl,b_xcl, -1.0, 
				   2.0*median_pt_per_area);
  //cout << "a_xcl,b_xcl =" << a_xcl <<" " <<b_xcl<<endl;


  vector<FjPseudoJet> jets = clust.inclusive_jets();

  for (unsigned i = 0; i < jets.size(); i++) {
    if (abs(jets[i].rap()) < ghost_etamax - 1.0) 
      inclpt_uncorrected.fill(jets[i].perp());
  }

  //vector<FjPseudoJet> corrected_jets(jets.size());
  for (unsigned i = 0; i < jets.size(); i++) {
    //double correction_factor = 1 - 
    //  median_pt_per_area*clust.area(jets[i])/jets[i].perp(); 
    //corrected_jets[i] =  max(correction_factor,0.0) * jets[i];
    if (abs(jets[i].rap()) < ghost_etamax - 1.0) {
      double pt   = jets[i].perp();
      double rap  = jets[i].rap();
      double area = clust.area(jets[i]);
      inclpt_corrected.fill(pt- median_pt_per_area*area);
      inclpt_corrected_withrap.fill(pt-area*(a+b*rap*rap));
      inclpt_corrected_withrap_xcl.fill(pt-area*(a_xcl+b_xcl*rap*rap));
    }

  }

  //for (unsigned i = 0; i < corrected_jets.size(); i++) {
  //  if (abs(corrected_jets[i].rap()) < ghost_etamax - 1.0) 
  //    
  //}

}


////======================================================================
//void determine_Zmass_cone(const vector<FjPseudoJet> & event, 
//			double R, ConeVariant cone_variant,
//			double & mass, double & corrected_mass) {
//  
//  // Define MidPoint algorithm.
//  double m_seedThreshold    = 1;
//  double m_coneRadius       = R;
//
//  double m_overlapThreshold;
//  double m_coneAreaFraction;
//  switch(cone_variant) {
//  case(midpoint_050): 
//    m_coneAreaFraction = 1.00;
//    m_overlapThreshold = 0.50; break;
//  case(midpoint_075): 
//    m_coneAreaFraction = 1.00;
//    m_overlapThreshold = 0.75; break;
//  case(searchcone_075):
//    m_coneAreaFraction = 0.25;
//    m_overlapThreshold = 0.75; break;
//  default:
//    cerr << "Unrecognized cone_variant: "<<cone_variant<<endl; 
//    exit(-1);
//  }
//
//  int    m_maxPairSize      = 2;
//  int    m_maxIterations    = 100;
//  MidPointAlgorithm m(m_seedThreshold,m_coneRadius,m_coneAreaFraction,m_maxPairSize,m_maxIterations,m_overlapThreshold);
//
//  // convert our event into a the CDF format
//  vector<PhysicsTower> towers;
//  for (unsigned i = 0; i < event.size(); i++) 
//    towers.push_back(PhysicsTower(LorentzVector(
//		  event[i].px(),event[i].py(),event[i].pz(),event[i].E())));
//  
//  // run the jet algorithm
//  vector<Cluster> m_jets;
//  m.run(towers,m_jets);
//
//  // extract the jets
//  vector<FjPseudoJet> jets;
//  for (unsigned i=0; i < m_jets.size(); i++) 
//    jets.push_back(FjPseudoJet(m_jets[i].fourVector.px,
//			       m_jets[i].fourVector.py,
//			       m_jets[i].fourVector.pz,
//			       m_jets[i].fourVector.E));
// 
//  mass = Zmass_from_jets(jets);
//  corrected_mass = mass;
//}
//
//
////======================================================================
//double Zmass_from_jets(const vector<FjPseudoJet> & jets) {
//  vector<FjPseudoJet> sorted_jets = sorted_by_pt(jets);
//  double mass = sqrt(abs((sorted_jets[0]+sorted_jets[1]).m2()));
//  return mass;
//}

//======================================================================
void read_event(istream & input, double etamax, bool hydjet, bool massless,
		double discard_below_pt,
		vector<FjPseudoJet> & hard_event, 
		vector<FjPseudoJet> & full_event) {
  string line;
  int  nsub  = 0;
  while (getline(input, line)) {
      //cout << line<<endl;
    istringstream linestream(line);
    if (line == "#END") {break;}
    if (line == "#SUBSTART") {
      // if more sub events follow, make copy of hard one here
      if (nsub == 1) hard_event = full_event;
      nsub += 1;
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
    FjPseudoJet psjet(fourvec);
    psjet.set_user_index(0);
    if (abs(psjet.rap() < etamax) && psjet.perp() >= discard_below_pt) {
      full_event.push_back(psjet);}

  }

  // if we have read in only one event, copy it across here...
  if (nsub == 1) hard_event = full_event;

  // if there was nothing in the event 
  if (nsub == 0) {
    cerr << "Error: read empty event\n";
    exit(-1);
  }
}

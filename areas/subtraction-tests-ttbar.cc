
//STARTHEADER
// $Id$
//
// Copyright (c) 2006, Matteo Cacciari and Gavin Salam
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

// what output do we want?
//
// . hard event, full event, with and without subtraction (4)
// . for the kt in inclusive and exclusive formulations 
//   [and cam in inclusive, since exclusive not yet functional]


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
#include<iostream>
#include<sstream>
#include<fstream>
#include<valarray>
#include<vector>
#include <cstdlib>
#include<cstddef> // for size_t
#include "fastjet/ClusterSequenceArea.hh"
#include "CmdLine.hh"
#include "CSHisto.hh"
#include "FlavourHolder.hh"
#include "jet_def_from_cmdline.hh"


namespace fj = fastjet;
using namespace std;

inline double pow2(const double x) {return x*x;};

const double maxrap_for_median = 4.0;
bool rho_from_area_4vector = true;


//void determine_Zmass_kt(const vector<fj::PseudoJet> & event, 
//			double ghost_area, double ghost_etamax,
//			double grid_scatter, double kt_scatter, int repeat,
//			double ktR, fj::Strategy strategy,
//			double & mass, double & corrected_mass, 
//			double & ext_corrected_mass);

//void determine_Zmass_cone(const vector<fj::PseudoJet> & event, 
//			  double R, ConeVariant cone_variant,
//			  double & mass, double & corrected_mass);
//

void look_at_event(const vector<fj::PseudoJet> & event,
		   const fj::JetDefinition  & jet_def,
		   const fj::AreaDefinition & area_def,
                   bool                       rho_uses_cam05,
		   const fj::JetDefinition  & rho_jet_def,
		   const fj::AreaDefinition & rho_area_def,
		   const bool verbose, 
		   double & Wmass_incl, double & tmass_incl,
		   double & Wmass_excl, double & tmass_excl,
		   double & Wmass_incl_ecor, double & tmass_incl_ecor,
		   double & Wmass_excl_ecor, double & tmass_excl_ecor
		   );

double Zmass_from_jets(const vector<fj::PseudoJet> & jets);

void read_event(istream &, double, bool, bool,
		vector<fj::PseudoJet> &, vector<fj::PseudoJet> & );


//----------------------------------------------------------------------
/// a program to test and time the kt algorithm as implemented in fastjet
int main (int argc, char ** argv) {

  CmdLine cmdline(argc,argv);
  // how we treat the event that is read in
  string input_file = cmdline.string_val("-in");
  bool   hydjet     = cmdline.present("-hydjet");
  double etamax     = cmdline.double_val("-etamax",1.0e310);
  bool   massless   = cmdline.present("-massless");
  int    nev        = int(cmdline.double_val("-nev",1.0));
  bool   nopileup   = cmdline.present("-nopileup"); 

  // properties of the jet algorithm and the area finding
  fj::JetDefinition jet_def = jet_def_from_cmdline(cmdline);
  fj::AreaDefinition area_def = area_def_from_cmdline(cmdline);


  // for (e.g.) cone algorithm, allow one to estimate rho with a more
  // reliable alg.
  bool rho_uses_cam05 = cmdline.present("-rho_uses_cam05");
  bool rho_uses_kt05 = cmdline.present("-rho_uses_kt05");
  fj::JetDefinition  rho_jet_def ;
  fj::AreaDefinition rho_area_def;
  if (rho_uses_cam05) {
    rho_jet_def  = fj::JetDefinition (fj::cambridge_algorithm,0.5);
    rho_area_def = fj::AreaDefinition(fj::VoronoiAreaSpec(0.5));
  } else if (rho_uses_kt05) {
    rho_jet_def  = fj::JetDefinition (fj::kt_algorithm,0.5);
    rho_area_def = fj::AreaDefinition(fj::VoronoiAreaSpec(0.9));
  }

  rho_from_area_4vector = ! cmdline.present("-plain_rho");

  // how we process and output things
  bool   verbose      = cmdline.present("-verbose");
  double bin_width    = cmdline.double_val("-bin",5.0);
  double max_bin      = cmdline.double_val("-max",400.0);
  string output_file  = cmdline.string_val("-out");
  int    writefreq    = int(cmdline.double_val("-freq",1.0*max(nev/10,1000)));
  string rerun_string = cmdline.string_val("-rerun","");
  cerr <<"writefreq is "<<writefreq<<endl;

  // sanity check on command-line structure
  if (!cmdline.all_options_used()) {cerr << 
      "Error: some options unsupported"<<endl; 
    exit(-1);}

  // input will be from the file named with the "-in" option
  ifstream input(input_file.c_str());

  int nbins = int(max_bin/bin_width + 0.5);
  CSHisto hard_Wmass_incl     (00.0, max_bin, nbins);
  CSHisto hard_tmass_incl     (00.0, max_bin, nbins);
  CSHisto hard_Wmass_excl     (00.0, max_bin, nbins);
  CSHisto hard_tmass_excl     (00.0, max_bin, nbins);
  CSHisto hard_Wmass_incl_ecor(00.0, max_bin, nbins);
  CSHisto hard_tmass_incl_ecor(00.0, max_bin, nbins);
  CSHisto hard_Wmass_excl_ecor(00.0, max_bin, nbins);
  CSHisto hard_tmass_excl_ecor(00.0, max_bin, nbins);

  CSHisto full_Wmass_incl     (00.0, max_bin, nbins);
  CSHisto full_tmass_incl     (00.0, max_bin, nbins);
  CSHisto full_Wmass_excl     (00.0, max_bin, nbins);
  CSHisto full_tmass_excl     (00.0, max_bin, nbins);
  CSHisto full_Wmass_incl_ecor(00.0, max_bin, nbins);
  CSHisto full_tmass_incl_ecor(00.0, max_bin, nbins);
  CSHisto full_Wmass_excl_ecor(00.0, max_bin, nbins);
  CSHisto full_tmass_excl_ecor(00.0, max_bin, nbins);


  for (int iev = 0; iev < nev; iev++) {
    if (iev < 100 || iev%100 == 0) cerr << "Doing event "<< iev<<endl;
    vector<fj::PseudoJet> hard_event, full_event;
    
    // read in the event 
    read_event(input, etamax, hydjet, massless, hard_event, full_event);
      
    // dumb it down if need be...
    if (nopileup)  full_event = hard_event;
  
    double Wmass_incl, tmass_incl;
    double Wmass_excl, tmass_excl;
    double Wmass_incl_ecor, tmass_incl_ecor;
    double Wmass_excl_ecor, tmass_excl_ecor;


    look_at_event(hard_event, jet_def, area_def, 
                  rho_uses_cam05, rho_jet_def, rho_area_def,
                  verbose, 
		  Wmass_incl, tmass_incl,
		  Wmass_excl, tmass_excl,
		  Wmass_incl_ecor, tmass_incl_ecor,
		  Wmass_excl_ecor, tmass_excl_ecor
		  );

    // fill the histograms
    hard_Wmass_incl     .fill(Wmass_incl     );
    hard_tmass_incl     .fill(tmass_incl	 );    
    hard_Wmass_excl     .fill(Wmass_excl     );    
    hard_tmass_excl     .fill(tmass_excl	 );    
    hard_Wmass_incl_ecor.fill(Wmass_incl_ecor); 
    hard_tmass_incl_ecor.fill(tmass_incl_ecor);
    hard_Wmass_excl_ecor.fill(Wmass_excl_ecor); 
    hard_tmass_excl_ecor.fill(tmass_excl_ecor);

    // only run things again if we truly have an event to run on...
    if (full_event.size() != hard_event.size()) {
      look_at_event(full_event, jet_def, area_def, 
                    rho_uses_cam05, rho_jet_def, rho_area_def,
                    verbose, 
		    Wmass_incl, tmass_incl,
		    Wmass_excl, tmass_excl,
		    Wmass_incl_ecor, tmass_incl_ecor,
		    Wmass_excl_ecor, tmass_excl_ecor
		    );
    }

    // fill the histograms
    full_Wmass_incl     .fill(Wmass_incl     );
    full_tmass_incl     .fill(tmass_incl	 );    
    full_Wmass_excl     .fill(Wmass_excl     );    
    full_tmass_excl     .fill(tmass_excl	 );    
    full_Wmass_incl_ecor.fill(Wmass_incl_ecor); 
    full_tmass_incl_ecor.fill(tmass_incl_ecor);
    full_Wmass_excl_ecor.fill(Wmass_excl_ecor); 
    full_tmass_excl_ecor.fill(tmass_excl_ecor);
  
    // write intermediate and final results...
    if ( iev+1==nev || (iev+1) % writefreq == 0) {
      // sending output to a file...
      ofstream output(output_file.c_str());
      if (rerun_string != "") {
	output << "# Rerun with:\n";
	output << "# "<<rerun_string<<endl;
      }
      output << "# " << cmdline.command_line() << endl;
      output << "# jet_def  = " <<  jet_def.description() << endl;
      output << "# area_def = " << area_def.description() << endl;
      if (rho_uses_cam05) {
        output << "# rho_jet_def  = " <<  rho_jet_def.description() << endl;
        output << "# rho_area_def = " << rho_area_def.description() << endl;
      }
      output << "# maxrap_for_median = " << maxrap_for_median << endl;
      output << "# rho_from_area_4vector = " << rho_from_area_4vector << endl;
      output << "# nev = " <<iev+1 <<endl;
      output << "# bin-lo(1) bin-mid (2) bin-hi(3) ";
      int jj = 3;
      output << "hard_Wmass_incl("     <<++jj<<") "; 
      output << "hard_tmass_incl("     <<++jj<<") ";
      output << "hard_Wmass_excl("     <<++jj<<") ";
      output << "hard_tmass_excl("     <<++jj<<") ";
      output << "hard_Wmass_incl_ecor("<<++jj<<") ";
      output << "hard_tmass_incl_ecor("<<++jj<<") ";
      output << "hard_Wmass_excl_ecor("<<++jj<<") ";
      output << "hard_tmass_excl_ecor("<<++jj<<") "; 
      output << "full_Wmass_incl("     <<++jj<<") "; 
      output << "full_tmass_incl("     <<++jj<<") ";
      output << "full_Wmass_excl("     <<++jj<<") ";
      output << "full_tmass_excl("     <<++jj<<") ";
      output << "full_Wmass_incl_ecor("<<++jj<<") ";
      output << "full_tmass_incl_ecor("<<++jj<<") ";
      output << "full_Wmass_excl_ecor("<<++jj<<") ";
      output << "full_tmass_excl_ecor("<<++jj<<") "; 
      output << endl;


      // print out mass histograms.
      for (unsigned i = 0; i < hard_Wmass_incl.size(); i++) {
	output << hard_Wmass_incl.bin_lower_edge(i) <<" "
	       << hard_Wmass_incl.bin_centre(i) <<" "
	       << hard_Wmass_incl.bin_upper_edge(i) <<" "
	       << hard_Wmass_incl     .bin_weight(i)/((iev+1)*bin_width) <<" "
	       << hard_tmass_incl     .bin_weight(i)/((iev+1)*bin_width) <<" "
	       << hard_Wmass_excl     .bin_weight(i)/((iev+1)*bin_width) <<" "
	       << hard_tmass_excl     .bin_weight(i)/((iev+1)*bin_width) <<" "
	       << hard_Wmass_incl_ecor.bin_weight(i)/((iev+1)*bin_width) <<" "
	       << hard_tmass_incl_ecor.bin_weight(i)/((iev+1)*bin_width) <<" "
	       << hard_Wmass_excl_ecor.bin_weight(i)/((iev+1)*bin_width) <<" "
	       << hard_tmass_excl_ecor.bin_weight(i)/((iev+1)*bin_width) <<" "
	       << full_Wmass_incl     .bin_weight(i)/((iev+1)*bin_width) <<" "
	       << full_tmass_incl     .bin_weight(i)/((iev+1)*bin_width) <<" "
	       << full_Wmass_excl     .bin_weight(i)/((iev+1)*bin_width) <<" "
	       << full_tmass_excl     .bin_weight(i)/((iev+1)*bin_width) <<" "
	       << full_Wmass_incl_ecor.bin_weight(i)/((iev+1)*bin_width) <<" "
	       << full_tmass_incl_ecor.bin_weight(i)/((iev+1)*bin_width) <<" "
	       << full_Wmass_excl_ecor.bin_weight(i)/((iev+1)*bin_width) <<" "
	       << full_tmass_excl_ecor.bin_weight(i)/((iev+1)*bin_width) <<" "
	       << endl;
      }
    }
  }
}



//======================================================================
double Zmass_from_jets(const vector<fj::PseudoJet> & jets) {
  vector<fj::PseudoJet> sorted_jets = sorted_by_pt(jets);
  double mass = sqrt(abs((sorted_jets[0]+sorted_jets[1]).m2()));
  return mass;
}


//======================================================================
void read_event(istream & input, double etamax, bool hydjet, bool massless,
		vector<fj::PseudoJet> & hard_event, 
		vector<fj::PseudoJet> & full_event) {
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
    int particleID=0, particleCharge=0; // default vals if not set by prog
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
	linestream >> particleID >> particleCharge;
      }
    }
    fj::PseudoJet psjet(fourvec);
    psjet.set_user_index(particleID);
    if (abs(psjet.rap() < etamax)) {full_event.push_back(psjet);}

  }

  // if we have read in only one event, copy it across here...
  if (nsub == 1) hard_event = full_event;

  // if there was nothing in the event 
  if (nsub == 0) {
    cerr << "Error: read empty event\n";
    exit(-1);
  }
}

// this will often be useful...
typedef vector<fj::PseudoJet>::iterator FJPJ_iter;
typedef vector<fj::PseudoJet>::const_iterator FJPJ_citer;


//-------------------------------------------------------------
/// Return a string that says how many b and b-bar there are in 
/// the jet (b = b, B = bbar)
string b_string(const fj::ClusterSequence & cs, const fj::PseudoJet & jet) {

  string res;
  vector<fj::PseudoJet> cnst = cs.constituents(jet);
  for (FJPJ_iter particle = cnst.begin(); particle != cnst.end(); particle++) {
    int nb = particle -> user_index();
    if      (nb > 0) {res += "b";}
    else if (nb < 0) {res += "B";}
  }
  return res;
}


//----------------------------------------------------------------------
/// Return true if the jet contains one or more b's
bool b_tag(const fj::ClusterSequence & cs, const fj::PseudoJet & jet) {
  bool res = false;
  vector<fj::PseudoJet> cnst = cs.constituents(jet);
  for (FJPJ_iter particle = cnst.begin(); particle != cnst.end(); particle++) {
    int nb = particle -> user_index();
    res |= (nb != 0);
  }
  return res;
}

//----------------------------------------------------------------------
/// Return net count of the number of b's
int b_count(const fj::ClusterSequence & cs, const fj::PseudoJet & jet) {
  int res = 0;
  vector<fj::PseudoJet> cnst = cs.constituents(jet);
  for (FJPJ_iter particle = cnst.begin(); particle != cnst.end(); particle++) {
    int nb = particle -> user_index();
    res += nb;
  }
  return res;
}



//-------------------------------------------------------------
void separate_event(const vector<fj::PseudoJet> & event, 
		    vector<fj::PseudoJet> & leptonic_event, 
		    fj::ClusterSequenceArea * & clust_seq,
		    const fj::JetDefinition & jet_def,
		    const fj::AreaDefinition & area_def,
                    vector<fj::PseudoJet> & hadronic_event
                    ) {

  // make sure the event is clear
  hadronic_event.resize(0);

  // we will separate out the muon and any neutrinos from the other
  // particles (using the user index which has been set to the particle
  // idhep value); we'll treat electrons and taus as if they're hadronic
  // since we are generating semi-leptonic ttbar events where the lepton
  // is a muon.
  for (FJPJ_citer particle = event.begin(); particle != event.end(); particle++){

    FlavourHolder flav(particle->user_index());

    if (flav.is_neutrino() || flav.is_muon()) {
      leptonic_event.push_back(*particle);
    } else {
      fj::PseudoJet hadron = *particle;
      hadron.set_user_index(flav[5]); // set hadron user index to number of b-quarks
      hadronic_event.push_back(hadron);
    }
  }

  clust_seq = new fj::ClusterSequenceArea(hadronic_event, jet_def, area_def);

}

//----------------------------------------------------------------------
/// takes a cs and an array of jets extracted from it somehow (possibly 
/// modified by subtraction) and runs an analysis to extract the W
/// mass and top mass; NB it assumes that it is the bbar that is to be
/// associated with the hadronically decaying W
void extract_masses(const fj::ClusterSequenceArea & cs, 
		    const vector<fj::PseudoJet> & jets,
		    const bool verbose, 
		    double & Wmass, 
		    double & tmass) {
  
  vector<fj::PseudoJet> nonb_jets, b_jets;

  for (FJPJ_citer jet = jets.begin(); jet != jets.end(); jet++){
    if (verbose) {
      printf("%9.5f %8.5f %10.3f %8.3f +- %6.3f %10.3f %7s ",
	     jet->rap(), jet->phi(), jet->perp(), 
	     cs.area(*jet),cs.area_error(*jet),
	     //jet->perp()-median_pt_over_area* cs.area(*jet),
	     0.0,
	     b_string(cs, *jet).c_str()
	     );
      cout << b_tag(cs, *jet) << " "<< b_count(cs, *jet)<<endl;}
    
    fj::PseudoJet jetcopy = *jet;
    jetcopy.set_user_index(b_count(cs,*jet));
    if (jetcopy.user_index() == 0) {
      nonb_jets.push_back(jetcopy);
    } else {
      b_jets.push_back(jetcopy);}
  }

  tmass = 0.0;
  Wmass = 0.0;

  // we need the jets to be in order for the rest of the analysis to make
  // sense
  b_jets = sorted_by_pt(b_jets);
  nonb_jets = sorted_by_pt(nonb_jets);

  if (nonb_jets.size() >= 2) {
    fj::PseudoJet W = nonb_jets[0]+nonb_jets[1];
    Wmass = sqrt(W.m2());
    for (FJPJ_citer bjet = b_jets.begin(); bjet != b_jets.end(); bjet++){
      if (bjet->user_index() == -1) {tmass = sqrt((W+(*bjet)).m2()); break;}
    }
    if (verbose) cout << "W and top masses: "<<Wmass<<" "<<tmass<<endl;
  } else {
    if (verbose) cout << "insufficient number of jets to calculate mass"<<endl;}

}


//----------------------------------------------------------------------
/// correct the vector of jets based on their extended area and the 
/// pt_per_unit_area
void ext_correct_jets(const fj::ClusterSequenceArea & cs, 
		      vector<fj::PseudoJet> & jets,
		      const double pt_per_unit_area) {

  for (unsigned i = 0; i < jets.size(); i++) {
    fj::PseudoJet area_4vect = pt_per_unit_area * cs.area_4vector(jets[i]);
    if (area_4vect.perp2() >= jets[i].perp2() || 
	area_4vect.E()     >= jets[i].E()) {
      // if the correction is too large, set the jet to zero
      jets[i] *=  0.0 ;
    } else {
      // otherwise do an E-scheme subtraction
      jets[i] -= area_4vect;
    }
  }
}

//-------------------------------------------------------------
/// routine for ("visually") looking at a ttbar event and also
/// for extracting information about it...
void look_at_event(const vector<fj::PseudoJet> & event,
		   const fj::JetDefinition  & jet_def,
		   const fj::AreaDefinition & area_def,
                   bool                       rho_uses_cam05,
		   const fj::JetDefinition  & rho_jet_def,
		   const fj::AreaDefinition & rho_area_def,
		   const bool verbose,
		   double & Wmass_incl, double & tmass_incl,
		   double & Wmass_excl, double & tmass_excl,
		   double & Wmass_incl_ecor, double & tmass_incl_ecor,
		   double & Wmass_excl_ecor, double & tmass_excl_ecor
		   ) {
  
  vector<fj::PseudoJet> leptonic_event;
  vector<fj::PseudoJet> hadronic_event;
  
  fj::ClusterSequenceArea * clust_seq;

  // extract the leptonic and jet parts of the event
  separate_event(event, leptonic_event, clust_seq, jet_def, 
                 area_def, hadronic_event);

  if (verbose) {
    // print general header...
    printf(
      " rap      phi        Pt         area  +-   err      pt_corr  flavour\n");
    // print leptonic part of the event 
    for (FJPJ_iter lepton = leptonic_event.begin(); 
	 lepton != leptonic_event.end(); lepton++){
      printf("%9.5f %8.5f %10.3f %8.3f +- %6.3f %10.3f %7d\n",
	     lepton->rap(), lepton->phi(), lepton->perp(), 0.0,0.0,lepton->perp(),
	     lepton->user_index());
    }
    cout << " "<<endl;
  }

  // use the default or an alternative alg to estimate rho
  double median_pt_over_area;
  if (rho_uses_cam05) {
    fj::ClusterSequenceArea rho_cs(hadronic_event, rho_jet_def, rho_area_def);
    median_pt_over_area = rho_cs.median_pt_per_unit_something(
                                   maxrap_for_median, rho_from_area_4vector);
  } else {
    //median_pt_over_area = clust_seq->median_pt_per_unit_area_4vector(
    //                                                 maxrap_for_median);
    median_pt_over_area = clust_seq->median_pt_per_unit_something(
                                   maxrap_for_median, rho_from_area_4vector);
  }

  vector<fj::PseudoJet> jets;

  // get inclusive jets with a 5 GeV threshold
  jets = clust_seq->inclusive_jets(5.0);
  if (verbose) cout << "inclusive" << endl;
  extract_masses(*clust_seq, jets, verbose, Wmass_incl, tmass_incl);

  // correct the jets and get new masses
  ext_correct_jets(*clust_seq, jets, median_pt_over_area);
  if (verbose) cout << "inclusive corrected" << endl;
  extract_masses(*clust_seq, jets, verbose, Wmass_incl_ecor, tmass_incl_ecor);

  // view the event "exclusively" as consisting of 4 jets
  jets = clust_seq->exclusive_jets(4);
  if (verbose) cout << "exclusive" << endl;
  extract_masses(*clust_seq, jets, verbose, Wmass_excl, tmass_excl);

  // correct the jets and get new masses
  ext_correct_jets(*clust_seq, jets, median_pt_over_area);
  if (verbose) cout << "exclusive corrected" << endl;
  extract_masses(*clust_seq, jets, verbose, Wmass_excl_ecor, tmass_excl_ecor);
  
  delete clust_seq;
}

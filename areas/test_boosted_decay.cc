
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
#include "fastjet/ClusterSequenceArea.hh"
#include<iostream>
#include<sstream>
#include<fstream>
#include<valarray>
#include<vector>
#include <cstdlib>
#include<cstddef> // for size_t
#include "CmdLine.hh"
#include "CSHisto.hh"
#include "jet_def_from_cmdline.hh"


namespace fj = fastjet;
using namespace std;

inline double pow2(const double x) {return x*x;};

const double maxrap_for_median = 4.0;

void print_jet(const fj::ClusterSequence & cs, const fj::PseudoJet & jet) {
  vector<fj::PseudoJet> cnst = cs.constituents(jet);
  for (size_t i = 0; i < cnst.size(); i++) {
    printf("%6i %18.5f %18.5f %18.6e\n",i,cnst[i].rap(),cnst[i].phi(),cnst[i].perp());
  }
  printf("#END\n");
};



void determine_Zmass_kt(const vector<fj::PseudoJet> & event, 
                        fj::JetDefinition jet_def,
                        fj::AreaDefinition area_def,
                        bool                       rho_uses_something,
	   	        const fj::JetDefinition  & rho_jet_def,
		        const fj::AreaDefinition & rho_area_def,
			double & mass, double & corrected_mass, 
			double & ext_corrected_mass);

//enum ConeVariant {not_cone, midpoint_050, midpoint_075, searchcone_075};

//void determine_Zmass_cone(const vector<fj::PseudoJet> & event, 
//			  double R, ConeVariant cone_variant,
//			  double & mass, double & corrected_mass);

double Zmass_from_jets(const vector<fj::PseudoJet> & jets);

void read_event(istream &, double, bool, bool,
		vector<fj::PseudoJet> &, vector<fj::PseudoJet> & );


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
  double etamax = cmdline.double_val("-etamax",1.0e310);
  bool   massless = cmdline.present("-massless");
  int    nev      = int(cmdline.double_val("-nev",1.0));
  bool   nopileup  = cmdline.present("-nopileup"); 

  // properties of the jet algorithm and the area finding
  fj::JetDefinition jet_def = jet_def_from_cmdline(cmdline);
  fj::AreaDefinition area_def = area_def_from_cmdline(cmdline);

  // for (e.g.) cone algorithm, allow one to estimate rho with a more
  // reliable alg.
  bool rho_uses_cam05 = cmdline.present("-rho_uses_cam05");
  bool rho_uses_kt05 = cmdline.present("-rho_uses_kt05");
  bool rho_uses_something = false;
  fj::JetDefinition  rho_jet_def ;
  fj::AreaDefinition rho_area_def;
  if (rho_uses_cam05) {
    rho_jet_def  = fj::JetDefinition (fj::cambridge_algorithm,0.5);
    rho_area_def = fj::AreaDefinition(fj::VoronoiAreaSpec(0.5));
    rho_uses_something = true;
  } else if (rho_uses_kt05) {
    rho_jet_def  = fj::JetDefinition (fj::kt_algorithm,0.5);
    rho_area_def = fj::AreaDefinition(fj::VoronoiAreaSpec(0.9));
    rho_uses_something = true;
  }

  double bin_width    = cmdline.double_val("-bin",5.0);
  double max_bin      = cmdline.double_val("-max",400.0);
  double min_bin      = cmdline.double_val("-min",0.0);
  //bool   print_jets   = cmdline.present("-print_jets");
  string input_file   = cmdline.string_val("-in");
  string output_file  = cmdline.string_val("-out");

  int    writefreq    = int(cmdline.double_val("-freq",1.0*max(nev/10,1000)));
  string rerun_string = cmdline.string_val("-rerun","");
  cerr <<"writefreq is "<<writefreq<<endl;

  if (!cmdline.all_options_used()) {cerr << 
      "Error: some options unsupported"<<endl; 
    exit(-1);}


  // input will be from the file named with the "-in" option
  ifstream input(input_file.c_str());

  int nbins = int((max_bin-min_bin)/bin_width + 0.5);
  CSHisto inv_mass_hard(min_bin, max_bin, nbins);
  CSHisto inv_mass_hcor(min_bin, max_bin, nbins);
  CSHisto inv_mass_full(min_bin, max_bin, nbins);
  CSHisto inv_mass_fcor(min_bin, max_bin, nbins);

  // histograms using the "extended" area subtraction...
  CSHisto inv_mass_hecr(min_bin, max_bin, nbins);
  CSHisto inv_mass_fecr(min_bin, max_bin, nbins);


  for (int iev = 0; iev < nev; iev++) {
    cerr << "Doing event "<< iev<<endl;
    vector<fj::PseudoJet> hard_event, full_event;
    
    // read in the event 
    read_event(input, etamax, hydjet, massless, hard_event, full_event);
      
    // dumb it down if need be...
    if (nopileup)  full_event = hard_event;
    
    // deduce the masses
    double hard_ev_mass, hcor_ev_mass, hecr_ev_mass;
    determine_Zmass_kt(hard_event, jet_def, area_def,
                       rho_uses_something,rho_jet_def,rho_area_def,
                       hard_ev_mass, hcor_ev_mass, hecr_ev_mass);


    double full_ev_mass, fcor_ev_mass, fecr_ev_mass;
    if (full_event.size() != hard_event.size()) {
      determine_Zmass_kt(full_event, jet_def, area_def,
                         rho_uses_something,rho_jet_def,rho_area_def,
                         full_ev_mass, fcor_ev_mass, fecr_ev_mass);
    } else {
      full_ev_mass = hard_ev_mass;
      fcor_ev_mass = hcor_ev_mass;
      fecr_ev_mass = hecr_ev_mass;
    }

    // limit the amount of information that is output 
    if (iev < 100) {
      // provide user with some info (maybe get rid of this at some point?)
      cout <<"inv mass of two hardest (hard) jets = "<< hard_ev_mass << endl;
      cout <<"inv mass of two hardest (hcor) jets = "<< hcor_ev_mass << endl;
      cout <<"inv mass of two hardest (hecr) jets = "<< hecr_ev_mass << endl;
      cout <<"inv mass of two hardest (full) jets = "<< full_ev_mass << endl;
      cout <<"inv mass of two hardest (fcor) jets = "<< fcor_ev_mass << endl;
      cout <<"inv mass of two hardest (fecr) jets = "<< fecr_ev_mass << endl;
    }

    // fill histograms
    inv_mass_hard.fill(hard_ev_mass);
    inv_mass_hcor.fill(hcor_ev_mass);
    inv_mass_full.fill(full_ev_mass);
    inv_mass_fcor.fill(fcor_ev_mass);

    inv_mass_hecr.fill(hecr_ev_mass);
    inv_mass_fecr.fill(fecr_ev_mass);
    
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
      if (rho_uses_something) {
        output << "# rho_jet_def  = " <<  rho_jet_def.description() << endl;
        output << "# rho_area_def = " << rho_area_def.description() << endl;
      }
      output << "# maxrap_for_median = " << maxrap_for_median << endl;
      output << "# nev = " <<iev+1 <<endl;
      output << "# bin-centre hard hcor full fcor hecr fecr" <<endl;
      
      // print out mass histograms.
      for (unsigned i = 0; i < inv_mass_hard.size(); i++) {
	output <<  inv_mass_hard.bin_centre(i) <<" "
	       << inv_mass_hard.bin_weight(i)/((iev+1)*bin_width) <<" "
	       << inv_mass_hcor.bin_weight(i)/((iev+1)*bin_width) <<" "
	       << inv_mass_full.bin_weight(i)/((iev+1)*bin_width) <<" "
	       << inv_mass_fcor.bin_weight(i)/((iev+1)*bin_width) <<" "
	       << inv_mass_hecr.bin_weight(i)/((iev+1)*bin_width) <<" "
	       << inv_mass_fecr.bin_weight(i)/((iev+1)*bin_width) << endl;
      }
    }
    
  } // iev
  
  
}




//======================================================================
void determine_Zmass_kt(const vector<fj::PseudoJet> & event, 
                        fj::JetDefinition jet_def,
                        fj::AreaDefinition area_def,
                        bool                       rho_uses_something,
	   	        const fj::JetDefinition  & rho_jet_def,
		        const fj::AreaDefinition & rho_area_def,
			double & mass, double & corrected_mass,
			double & ext_corrected_mass) {

  fj::ClusterSequenceArea clust(event,jet_def,area_def);

//  double median_pt_per_area = clust.pt_per_unit_area();
  // use the default or an alternative alg to estimate rho
  double median_pt_per_area,median_pt_per_area_4vect;
  if (rho_uses_something) {
    fj::ClusterSequenceArea rho_cs(event, rho_jet_def, rho_area_def);
    median_pt_per_area = rho_cs.median_pt_per_unit_area(maxrap_for_median);
    median_pt_per_area_4vect = rho_cs.median_pt_per_unit_something(
                                   maxrap_for_median, true);
  } else {
    median_pt_per_area = clust.median_pt_per_unit_area(maxrap_for_median);
    median_pt_per_area_4vect = clust.median_pt_per_unit_something(
                                   maxrap_for_median, true);
  }
  
  vector<fj::PseudoJet> jets = clust.inclusive_jets();
  mass = Zmass_from_jets(jets);

  vector<fj::PseudoJet> corrected_jets(jets.size());
  for (unsigned i = 0; i < jets.size(); i++) {
    double correction_factor = 1 - 
      median_pt_per_area*clust.area(jets[i])/jets[i].perp(); 
    corrected_jets[i] =  max(correction_factor,0.0) * jets[i];
  }

  corrected_mass = Zmass_from_jets(corrected_jets);

  // now to the correction with the "extended" area
  for (unsigned i = 0; i < jets.size(); i++) {
    fj::PseudoJet area_4vect = median_pt_per_area_4vect*clust.area_4vector(jets[i]);
    if (area_4vect.perp2() >= jets[i].perp2() || 
	area_4vect.E()     >= jets[i].E()) {
      // if the correction is too large, set the jet to zero
      corrected_jets[i] =  0.0 * jets[i];
    } else {
      // otherwise do an E-scheme subtraction
      double px,py,pz,E;
      px = jets[i].px() - area_4vect.px();
      py = jets[i].py() - area_4vect.py();
      pz = jets[i].pz() - area_4vect.pz();
      E  = jets[i].E()  - area_4vect.E();
      corrected_jets[i] = fj::PseudoJet(px,py,pz,E);
    }
  }

  ext_corrected_mass = Zmass_from_jets(corrected_jets);

}


//======================================================================
// void determine_Zmass_cone(const vector<fj::PseudoJet> & event, 
// 			double R, ConeVariant cone_variant,
// 			double & mass, double & corrected_mass) {
//   
//   // Define MidPoint algorithm.
//   double m_seedThreshold    = 1;
//   double m_coneRadius       = R;
// 
//   double m_overlapThreshold;
//   double m_coneAreaFraction;
//   switch(cone_variant) {
//   case(midpoint_050): 
//     m_coneAreaFraction = 1.00;
//     m_overlapThreshold = 0.50; break;
//   case(midpoint_075): 
//     m_coneAreaFraction = 1.00;
//     m_overlapThreshold = 0.75; break;
//   case(searchcone_075):
//     m_coneAreaFraction = 0.25;
//     m_overlapThreshold = 0.75; break;
//   default:
//     cerr << "Unrecognized cone_variant: "<<cone_variant<<endl; 
//     exit(-1);
//   }
// 
//   int    m_maxPairSize      = 2;
//   int    m_maxIterations    = 100;
//   MidPointAlgorithm m(m_seedThreshold,m_coneRadius,m_coneAreaFraction,m_maxPairSize,m_maxIterations,m_overlapThreshold);
// 
//   // convert our event into a the CDF format
//   vector<PhysicsTower> towers;
//   for (unsigned i = 0; i < event.size(); i++) 
//     towers.push_back(PhysicsTower(LorentzVector(
// 		  event[i].px(),event[i].py(),event[i].pz(),event[i].E())));
//   
//   // run the jet algorithm
//   vector<Cluster> m_jets;
//   m.run(towers,m_jets);
// 
//   // extract the jets
//   vector<fj::PseudoJet> jets;
//   for (unsigned i=0; i < m_jets.size(); i++) 
//     jets.push_back(fj::PseudoJet(m_jets[i].fourVector.px,
// 			       m_jets[i].fourVector.py,
// 			       m_jets[i].fourVector.pz,
// 			       m_jets[i].fourVector.E));
//  
//   mass = Zmass_from_jets(jets);
//   corrected_mass = mass;
// }


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
    // take substrings to avoid problems when there are extra "pollution"
    // characters (e.g. line-feed).
    if (line.substr(0,4) == "#END") {break;}
    if (line.substr(0,9) == "#SUBSTART") {
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
    fj::PseudoJet psjet(fourvec);
    psjet.set_user_index(0);
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

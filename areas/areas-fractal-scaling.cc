#include<iostream>
#include<iomanip>
#include<sstream>
#include<fstream>
#include<valarray>
#include<vector>
#include <cstdlib>
#include<cstddef> // for size_t
#include<cmath>

// fastjet stuff
#include "fastjet/ClusterSequenceArea.hh"

// get the plugins
#include "SISConePlugin.hh"
#include "CDFMidPointPlugin.hh"

// local things
#include "CmdLine.hh"
#include "SimpleHist.hh"
#include "jet_def_from_cmdline.hh"

namespace fj = fastjet;
using namespace std;

inline double pow2(const double x) {return x*x;};


/// micro class to calculate averages and errors
class AverageAndError {
public:

   /// default constructor
   AverageAndError() { _sum = 0.0; _sum2 = 0.0; _n=0;}
   
   /// add one event
   inline void add(double x)  { _sum += x, 
                                _sum2 += x*x;
			        _n += 1;
			      }
 
   /// return sum
   inline double sum() const { return _sum; }

   /// return sum2
   inline double sum2() const { return _sum2; }
   
   /// return number of events
   inline int n() const { return _n; }
   
   /// calculate and return average
   inline double average() const { return (_n > 0) ? _sum/_n : 0. ; }

   /// calculate and return average of squares
   inline double average2() const { return (_n > 0) ? _sum2/_n : 0. ; }

   /// calculate and return error
   inline double error() const { return (_n > 1) ? sqrt((_sum2/_n - _sum*_sum/_n/_n)/(_n-1)) : 0.; }

   /// calculate and return the standard deviation
   inline double sd() const { return (_n > 1) ? sqrt(_sum2/_n - _sum*_sum/_n/_n) : 0.; }
   
private:

double _sum, _sum2;
int _n;

};




//----------------------------------------------------------------------
/// A program that allows one to determine the distribution of areas
/// in events populated either by soft particles only, or by the soft
/// and one hard particle, varying the pt-distance between hard and soft

int main (int argc, char ** argv) {

  
  srand(1);

  CmdLine cmdline(argc,argv);

  fj::RecombinationScheme  rec_scheme  = cmdline.present("-pt_scheme") ? fj::BIpt_scheme :
              							       fj::E_scheme;

 // NB: default value of R is 0.7 !!!
  fj::JetDefinition jet_def = jet_def_from_cmdline(cmdline);
  double ktR = jet_def.R();
  fj::AreaDefinition area_def = area_def_from_cmdline(cmdline);

  double soft_pt = cmdline.double_val("-soft",1.0);
  double randomness = cmdline.double_val("-randomness",0.1);
  int    nsoft       = cmdline.int_val("-nsoft",1000);
  
  int    n       = cmdline.int_val("-n",20);
  int    writefreq    = int(cmdline.double_val("-freq",1.0*max(n/10,1000)));
  
  bool   checkpoint = cmdline.present("-checkpoint");
//  bool   passivearea =  cmdline.present("-passive");
  bool   oneghost2 =  cmdline.present("-oneghost2");
  bool   voronoi2 =  cmdline.present("-voronoi2");
//  bool   area4vector = ! cmdline.present("-plain_area");

  string outfile;
  if (cmdline.present("-out")) {
     outfile = cmdline.value<string>("-out");
  } else { 
     outfile = "";
  }

  if (!cmdline.all_options_used()) {
    cerr << "ERROR: exiting because some options unrecognized" << endl;
    exit(-1);
  }


  // create the definitions for our jet finder and areas spec...
//  fj::GhostedAreaSpec active_area_spec(ghost_etamax, repeat, 
//				      ghost_area, grid_scatter, kt_scatter);
  fj::VoronoiAreaSpec voronoi_area_spec(1.0);
  
  fj::AreaDefinition area_def_2;
//        area_def_2 = fastjet::AreaDefinition(fastjet::passive_area,active_area_spec);
  if ( oneghost2 ) {
        area_def.ghost_spec().set_repeat(1);
        area_def_2 = fastjet::AreaDefinition(fastjet::one_ghost_passive_area,area_def.ghost_spec());
  } else if ( voronoi2 ) {
        area_def_2 = voronoi_area_spec;
  } else { cerr << "Area2 not set" << endl;
           abort();
  }
//        area_def_2 = fastjet::AreaDefinition(active_area_spec);
 
  int  nsoftjets = 0;
  AverageAndError area1,area2,areadiff;
	 
  for (int i = 0; i<n; i++) {
    if ( i < 10 ) { cout << "# event " << i << endl; }
    vector<fj::PseudoJet> input_jets(0);

    double ghost_etamax = area_def.ghost_spec().ghost_etamax();
    double etamin = -ghost_etamax, etamax = -etamin;
    if ( soft_pt > 0 ) {
    // generate the random soft particles
      for(int isoft = 0; isoft < nsoft; isoft++) {
          double phi =  fj::twopi*rand()/RAND_MAX;
          double eta = etamin + (etamax-etamin)*rand()/RAND_MAX;
	  double kt = soft_pt*(1-randomness/2.+rand()*randomness/RAND_MAX);
	  //cout <<iphi << " "<< ieta << " "<< phi<< " "<< eta<<" "<<kt<<endl;
	  double pminus = kt*exp(-eta);
	  double pplus  = kt*exp(+eta);
	  double px = kt*sin(phi);
	  double py = kt*cos(phi);
	  //cout << kt<<" "<<eta<<" "<<phi<<"\n";
	  fj::PseudoJet mom(px,py,0.5*(pplus-pminus),0.5*(pplus+pminus));
	  mom.set_user_index(1);
	  input_jets.push_back(mom);
      } // isoft
    }
    //cout << input_jets.size() << endl;
    
    // do the clustering with one area
//    if (checkpoint) area_def.ghost_spec().checkpoint_random();
    fj::ClusterSequenceArea clust(input_jets,jet_def,area_def);
    // analyse the jets
    vector<fj::PseudoJet> output_jets(sorted_by_pt(clust.inclusive_jets()));

    // do the clustering with a second area
    fj::ClusterSequenceArea clust_2(input_jets,jet_def,area_def_2);
    // analyse the jets
    vector<fj::PseudoJet> output_jets_2(sorted_by_pt(clust_2.inclusive_jets()));
    
    //cout << "# of jets found " << output_jets.size()  << endl;
    for (unsigned j = 0; j < output_jets.size(); j++) {
      // only take jets that are reasonably close to center
      if (abs(output_jets[j].rap()) < ghost_etamax - ktR) {
	double normarea = clust.area(output_jets[j])/(fj::pi*pow2(ktR));
        double normarea_2 = clust_2.area(output_jets_2[j])/(fj::pi*pow2(ktR));

        area1.add(normarea);
        area2.add(normarea_2);
        areadiff.add(abs(normarea-normarea_2));
	nsoftjets++;

	
      } // end selection of central jets
    
    } // j - loop over jets in a single event


  if ( i+1==n || (i+1) % writefreq == 0 || i+1 == 10 || i+1 == 100 ) { 

    // (re-)initialise the output file
    ostream * ostr;
    if ( outfile != "" ) {
        ostr = new ofstream(outfile.c_str());
    } else {
        ostr = & cout;
    }


    (*ostr) << "# " << cmdline.command_line() << endl;
    (*ostr) << "# strategy     = " << jet_def.strategy()<<endl;
    (*ostr) << "# soft_pt      = " << soft_pt    << endl;
    (*ostr) << "# nsoft        = " << nsoft    << endl;
    (*ostr) << "# soft density = " << nsoft/2./etamax/2./fj::pi << endl;
    (*ostr) << "# soft area    = " <<  1./(nsoft/2./etamax/2./fj::pi) << endl;
    (*ostr) << "# lambda       = " << sqrt(1./(nsoft/2./etamax/2./fj::pi)) << endl;
    (*ostr) << "# randomness   = " << randomness   << endl;
    (*ostr) << "# R            = " << ktR          << endl;
//    (*ostr) << "# ghost_etamax = " << ghost_etamax << endl;
//    (*ostr) << "# ghost_area   = " << ghost_area   << endl;
    (*ostr) << "# nev          = " << n            << endl;
    //(*ostr) << "# nhist        = " << nhist        << endl;
    //(*ostr) << "# histmax      = " << histmax      << endl;
    (*ostr) << "# jet def      = " << jet_def.description() << endl;
    (*ostr) << "# area def     = " << area_def.description() << endl;
    (*ostr) << "# area2 def     = " << area_def_2.description() << endl;
    (*ostr) << "# "                                << endl;
    (*ostr) << "# number of events = " << i+1 << endl;
    (*ostr) << "# soft jets = " << nsoftjets << endl;
    (*ostr) << "# average area 1 = " << area1.average() <<  " +- " << area1.error() << endl;
    (*ostr) << "# average area 2 = " << area2.average() <<  " +- " << area2.error() << endl;
    (*ostr) << "# average area diff = " << areadiff.average() << " +- " << areadiff.error() << endl;

  
    (*ostr) << " " << endl;
    if ( outfile != "" ) { delete ostr; }
  
  } // if output
  } // i - loop over events
  
  
}
    
    
    
    
    

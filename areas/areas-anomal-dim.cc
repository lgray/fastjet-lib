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
#include "fastjet/PseudoJet.hh"
#include "fastjet/ClusterSequence.hh"
#include "fastjet/ClusterSequenceActiveArea.hh"
#include "ClusterSequencePassiveArea.hh"

// get the plugins
#include "SISConePlugin.hh"
#include "CDFMidPointPlugin.hh"

// local things
#include "CmdLine.hh"
#include "SimpleHist.hh"

namespace fj = fastjet;
using namespace std;

inline double pow2(const double x) {return x*x;};

//----------------------------------------------------------------------
/// A program that allows one to determine the distribution of areas
/// in events populated either by soft particles only, or by the soft
/// and one hard particle, varying the pt-distance between hard and soft

int main (int argc, char ** argv) {

  
  srand(1);

  CmdLine cmdline(argc,argv);

  double ghost_area = cmdline.double_val("-ghost_area",cmdline.double_val("-cell_area",0.01));
  double ghost_etamax = cmdline.double_val("-ghost_etamax",4.0);
  double grid_scatter = cmdline.double_val("-grid_scatter",0.0001);
  double kt_scatter   = cmdline.double_val("-kt_scatter",0.1);

  fj::Strategy  strategy  = fj::Strategy(cmdline.int_val("-strategy",
				     cmdline.int_val("-clever", fj::Best)));
  double ktR   = cmdline.double_val("-r",1.0);
  fj::JetDefinition jet_def;
  if (cmdline.present("-cam")) {
    jet_def = fj::JetDefinition(fj::cambridge_algorithm, ktR, strategy);}
  else if (cmdline.present("-kt")) {
    jet_def = fj::JetDefinition(fj::kt_algorithm, ktR, strategy);}
  else if (cmdline.present("-midpoint")) {
    double overlap = cmdline.value("-f",0.5);
    double seed    = cmdline.value("-seed",0.0);
    jet_def = fj::JetDefinition(new fj::CDFMidPointPlugin(ktR,overlap,seed));}
  else if (cmdline.present("-siscone")) {
    double overlap = cmdline.double_val("-f",0.5);
    int    npass   = cmdline.int_val("-npass",1);
    jet_def = fj::JetDefinition(new fj::SISConePlugin(ktR,overlap,npass));}
  else {
    cerr << "Must specify one of -kt | -cam | -siscone" << endl;
    exit(-1);
  }

  double soft_pt = cmdline.double_val("-soft",1.0);
  double hard_pt = cmdline.double_val("-hard",10.0);

  int    n       = cmdline.int_val("-n",20);
  int    hist    = cmdline.value("-hist",1);
 
  ostream * ostr;
  if (cmdline.present("-out")) {
    ostr = new ofstream(cmdline.value<string>("-out").c_str());
  } else {
    ostr = & cout;
  }

  if (!cmdline.all_options_used()) {
    cerr << "ERROR: exiting become some options unrecognized" << endl;
    exit(-1);
  }

  // create the definitions for our jet finder and areas spec...
  int repeat = 1;
  fj::ActiveAreaSpec active_area_spec(ghost_etamax, repeat, 
				      ghost_area, grid_scatter, kt_scatter);


  // the histogram...
  SimpleHist softareahist(-0.000001,2.,100);
  SimpleHist hardareahist(-0.000001,2.,100);

  (*ostr) << "# " << cmdline.command_line() << endl;
  (*ostr) << "# strategy     = " << jet_def.strategy()<<endl;
  (*ostr) << "# hard_pt      = " << hard_pt    << endl;
  (*ostr) << "# soft_pt      = " << soft_pt    << endl;
  (*ostr) << "# R            = " << ktR          << endl;
  (*ostr) << "# ghost_etamax = " << ghost_etamax << endl;
  (*ostr) << "# ghost_area   = " << ghost_area   << endl;
  (*ostr) << "# nev          = " << n            << endl;
  //(*ostr) << "# nhist        = " << nhist        << endl;
  //(*ostr) << "# histmax      = " << histmax      << endl;
  (*ostr) << "# jet def      = " << jet_def.description() << endl;



  int nhardjets = 0, nsoftjets = 0;
  double average_area_hard = 0.0, average_ar2_hard = 0.0,
         average_area_soft = 0.0, average_ar2_soft = 0.0;
	 
  for (int i = 0; i<n; i++) {
    if ( i < 10 ) { cout << "# event " << i << endl; }
    vector<fj::PseudoJet> input_jets(0);


    if ( hard_pt > 0 ) {
      // input the hard jet
      fj::PseudoJet hard_particle(hard_pt,0.0,0.0,hard_pt);
//    fj::PseudoJet
//      hard_particle(hard_pt/sqrt(3.),hard_pt/sqrt(3.),hard_pt/sqrt(3.),hard_pt);
      hard_particle.set_user_index(2);
      input_jets.push_back(hard_particle);
      fj::PseudoJet hard_particle2(-hard_pt,0.0,0.0,hard_pt);
//    fj::PseudoJet 
//      hard_particle2(-hard_pt/sqrt(3.),-hard_pt/sqrt(3.),-hard_pt/sqrt(3.),hard_pt);
      hard_particle2.set_user_index(2);
      input_jets.push_back(hard_particle2);
    }
    
    if ( soft_pt > 0 ) {
    // generate the quasi-random soft particles
      //int nphi = 63;
      //int neta = 200;
      //int nphi = 63;
      //int neta = 120;
      int nphi = 20;
      int neta = 40;
      double etamin = -ghost_etamax, etamax = ghost_etamax;
      double randomness = 0.1;
      int nsoft = 10000;
      for(int isoft = 0; isoft < nsoft; isoft++) {
//      for (int iphi = 0; iphi<nphi; iphi++) {
//        for (int ieta = 0; ieta < neta+1; ieta++) {  
//          double phi = (iphi+0.5) * (fj::twopi/nphi) + rand()*randomness/RAND_MAX;
//	  double eta = etamin + ieta * ((etamax-etamin)/neta)  
//	                                   + rand()*randomness/RAND_MAX;
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
//        }
//      }
      } // isoft
    }
    //cout << input_jets.size() << endl;
    
    // do the clustering  
    fj::ClusterSequenceActiveArea clust(input_jets,jet_def,active_area_spec);
     
    
    // analyse the jets
    vector<fj::PseudoJet> output_jets(clust.inclusive_jets());	 
    bool hard = false;
    //cout << "# of jets found " << output_jets.size()  << endl;
    for (unsigned j = 0; j < output_jets.size(); j++) {
    int hard_found = 0;
    //bool hard2 = false;
      // only take jets that are reasonably close to center
      if (abs(output_jets[j].rap()) < ghost_etamax - ktR) {
	double normarea = clust.area(output_jets[j])/(fj::pi*pow2(ktR));
	// loop over constituents, look for hard particle
	//cout << " hard_found " << hard_found << endl;
	if ( hard_found < 2 ) {
	  //cout << "j " << j << endl;
	  vector<fj::PseudoJet> constits = clust.constituents(output_jets[j]);
          //normarea = float(constits.size())/nsoft*fj::twopi*(etamax-etamin)/(fj::pi*pow2(ktR));
	   for (unsigned k = 0; k < constits.size(); k++) {
	     if (constits[k].user_index() == 2) { hard_found += 1;
	                                          hard = true;
//						  cout << "hard pt " << constits[k].perp() << endl; 
//						  cout << "hard rap " << constits[k].rap() << endl; 
//						  cout << "hard phi " << constits[k].phi() << endl; 
						  break; }
	   }
	}
//        if ( abs(output_jets[j].rap()) < 0.5*ktR && abs(output_jets[j].phi() - fj::pi) < 0.5*ktR ) 
//              { hard2 = true; }

//        if ( hard || hard2 ) { cout << hard << " " << hard2 << " " <<  output_jets[j].rap()
//	                            <<  " " << output_jets[j].phi() << endl; }

	if ( hard ) {
	  average_area_hard += normarea; 
	  average_ar2_hard  += pow2(normarea);
	  hardareahist.add_entry(normarea);
	  nhardjets++; 
	  hard = false;
	} else {
	  average_area_soft += normarea; 
	  average_ar2_soft  += pow2(normarea);
	  softareahist.add_entry(normarea);
	  nsoftjets++; 
        }
	
      }
    
    } // j - loop over jets in a single event
    
  } // i - loop over events

  average_area_hard /= nhardjets;
  average_ar2_hard  /= nhardjets;
  average_ar2_hard = sqrt((average_ar2_hard-pow2(average_area_hard))/nhardjets);
  average_area_soft /= nsoftjets;
  average_ar2_soft  /= nsoftjets;
  double save_av2 = average_ar2_soft;
  average_ar2_soft = sqrt((average_ar2_soft-pow2(average_area_soft))/nsoftjets);
  
  (*ostr) << "# av of squares " <<  save_av2 << endl;
  (*ostr) << "# hard jets = " << nhardjets << endl;
  (*ostr) << "# soft jets = " << nsoftjets << endl;
  (*ostr) << "# average area hard = " << average_area_hard << " +- " <<  average_ar2_hard << endl;
  (*ostr) << "# average area soft = " << average_area_soft << " +- " <<  average_ar2_soft << endl;
  (*ostr) << "# correct av. area hard (?) = " <<  save_av2/average_area_soft << endl;

  if ( hist ) { 
    double rescale = 1.0 / (hardareahist.binsize() * nhardjets);
    double softrescale = 1.0 / (softareahist.binsize() * nsoftjets);
    for (unsigned i = 0; i < hardareahist.size(); i++) {
      (*ostr) << hardareahist.binmid(i) 
              << " " << hardareahist[i]*rescale 
	      << " " << sqrt(hardareahist[i])*rescale 
              << " " << softareahist[i]*softrescale 
	      << " " << sqrt(softareahist[i])*softrescale 
	      << endl;
    }
  }
}
    
    
    
    
    

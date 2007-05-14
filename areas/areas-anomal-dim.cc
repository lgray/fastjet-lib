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
//#include "fastjet/PseudoJet.hh"
//#include "fastjet/ClusterSequence.hh"
//#include "fastjet/AreaDefinition.hh"
#include "fastjet/ClusterSequenceArea.hh"
//#include "fastjet/ClusterSequenceActiveArea.hh"
//#include "ClusterSequencePassiveArea.hh"

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
  int repeat = cmdline.value("-repeat", 1);

  fj::Strategy  strategy  = fj::Strategy(cmdline.int_val("-strategy",
				     cmdline.int_val("-clever", fj::Best)));
  fj::RecombinationScheme  rec_scheme  = cmdline.present("-pt_scheme") ? fj::BIpt_scheme :
              							       fj::E_scheme;
  
				     
  double ktR   = cmdline.double_val("-r",1.0);
  fj::JetDefinition jet_def;
  if (cmdline.present("-cam")) {
    jet_def = fj::JetDefinition(fj::cambridge_algorithm, ktR, rec_scheme, strategy);}
  else if (cmdline.present("-kt")) {
    jet_def = fj::JetDefinition(fj::kt_algorithm, ktR, rec_scheme, strategy);}
  else if (cmdline.present("-midpoint")) {
    double overlap = cmdline.value("-f",0.5);
    double seed    = cmdline.value("-seed",0.0);
    jet_def = fj::JetDefinition(new fj::CDFMidPointPlugin(ktR,overlap,seed));}
  else if (cmdline.present("-siscone")) {
    double overlap = cmdline.double_val("-f",0.5);
    int    npass   = cmdline.int_val("-npass",1);
    fj::SISConePlugin * plugin = new fj::SISConePlugin(ktR,overlap,npass);
    if (cmdline.present("-smstop"))plugin->set_split_merge_stopping_scale(1e-50);
    jet_def = fj::JetDefinition(plugin);}
  else {
    cerr << "Must specify one of -kt | -cam | -siscone" << endl;
    exit(-1);
  }

  double soft_pt = cmdline.double_val("-soft",1.0);
  double hard_pt = cmdline.double_val("-hard",10.0);
  double rad_pt  = cmdline.double_val("-rad",hard_pt);
  double randomness = cmdline.double_val("-randomness",0.1);
  int    nsoft       = cmdline.int_val("-nsoft",10000);
  int    nhard       = cmdline.int_val("-nhard",1);
  double ptlim = cmdline.double_val("-ptlim",1.);
  double distlim = cmdline.double_val("-distlim",1e-3);
  double distmax = cmdline.double_val("-distmax",2.);
  
  int    n       = cmdline.int_val("-n",20);
  int    hist    = cmdline.value("-hist",1);
  int    writefreq    = int(cmdline.double_val("-freq",1.0*max(n/10,1000)));
  
  bool   emission = cmdline.present("-emission");
  bool   linear = cmdline.present("-linear");
  bool   checkpoint = cmdline.present("-checkpoint");
  bool   passivearea =  cmdline.present("-passive");

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

  // only one hard particle if we are studying radiation
  if ( emission ) { nhard = 1; };
  
  // distlim cannot be zero if logarithmic random generation is used
  if ( !(linear) && distlim == 0 ) { distlim = 1e-100;}

  // create the definitions for our jet finder and areas spec...
  fj::GhostedAreaSpec active_area_spec(ghost_etamax, repeat, 
				      ghost_area, grid_scatter, kt_scatter);
  fj::VoronoiAreaSpec voronoi_area_spec(1.0);
  
  fj::AreaDefinition area_def;
  if ( ! passivearea ) {
        area_def = fastjet::AreaDefinition(active_area_spec);
  } else {
        area_def = fastjet::AreaDefinition(fastjet::passive_area,active_area_spec);
 //       area_def = voronoi_area_spec;
  }

  // the histograms...
  SimpleHist softareahist(-0.000001,2.,100);
  SimpleHist hardareahist(-0.000001,2.,100);
//   SimpleHist hardptdist(hard_pt*0.2,hard_pt*1.8,80);
//   double rho = nsoft/(fj::twopi*2.*ghost_etamax)*soft_pt;
//   double typical_soft_contrib = rho*fj::pi*ktR*ktR*0.6;
//   SimpleHist ptdist(typical_soft_contrib*0.1 + hard_pt,
//                     typical_soft_contrib*2 + hard_pt,200 );
  SimpleHist hardptdist(0.,2000.,1000);
  SimpleHist ptdist(0.,2000.,1000);
  
  int nhardjets = 0, nsoftjets = 0;
  double average_area_hard = 0.0, average_ar2_hard = 0.0,
         average_area_soft = 0.0, average_ar2_soft = 0.0;
  double average_area_rad = 0.0, average_ar2_rad = 0.0;
  double av_sub_pt = 0.0, av_sub_pt2 = 0.0;
  double av_pt = 0.0, av_pt2 = 0.0;

  double logptlim = log(ptlim);
  double loghardpt = log(rad_pt);
  double logdistlim = log(distlim);
  double logdistmaxR = log(distmax*ktR);
  double alphas = 0.3;
  double ca = 3.0;
  double cf = 4.0/3.0;
  double coeff = 2.*alphas*cf/fj::pi;
//  coeff = 1.;
	 
  for (int i = 0; i<n; i++) {
    if ( i < 10 ) { cout << "# event " << i << endl; }
    vector<fj::PseudoJet> input_jets(0);

    
    // generate radiated particle
    double weight = 0.;
    fj::PseudoJet radiated;
       double emitted_pt,emitted_dist;
    if ( emission ) {
//       double emitted_pt,emitted_dist;
       if (linear) {
          emitted_pt = ptlim + (rad_pt - ptlim)*rand()/RAND_MAX;
          emitted_dist = distlim + (ktR*distmax-distlim)*rand()/RAND_MAX;
          weight = coeff/emitted_pt/emitted_dist;
	  weight *= (rad_pt - ptlim)*(ktR*distmax-distlim); // jacobian
       } else {
          double u = logptlim + (loghardpt-logptlim)*rand()/RAND_MAX;
          emitted_pt = exp(u);
          u = logdistlim + (logdistmaxR-logdistlim)*rand()/RAND_MAX;
          emitted_dist = exp(u);
          weight = coeff;
	  weight *= (loghardpt-logptlim)*(logdistmaxR-logdistlim); // jacobian
       }
       if ( i < 10 ) {cout << "emitted pt, dist " << emitted_pt << " " << emitted_dist << endl;
	              cout << "weight " << weight << endl;}
//        double y_phi = fj::twopi*rand()/RAND_MAX;
//        double emitted_y = emitted_dist*sin(y_phi);
//        double emitted_phi = emitted_dist*cos(y_phi);
       double emitted_y = emitted_dist;
       double emitted_phi = 0.;
       double em_pminus = emitted_pt*exp(-emitted_y);
       double em_pplus  = emitted_pt*exp(+emitted_y);
//        double em_px = emitted_pt*cos(emitted_phi);
//        double em_py = emitted_pt*sin(emitted_phi);
       double em_px = emitted_pt;
       double em_py = 0.;
       fj::PseudoJet mom(em_px,em_py,0.5*(em_pplus-em_pminus),
  	  	 	             0.5*(em_pplus+em_pminus));
       radiated = mom;
       // check radiated momentum
       if ( i < 10 ) {
             cout << "rad pt, y, phi " << radiated.perp() << " "
	                               << radiated.rap()  << " "
	                               << radiated.phi()  << " " << endl;
       }
    }
 


    if ( hard_pt > 0 ) {
      // input the hard jet
      fj::PseudoJet hard_particle(hard_pt,0.0,0.0,hard_pt);
      hard_particle.set_user_index(2);
      input_jets.push_back(hard_particle);
      if ( nhard == 2 ) {
        fj::PseudoJet hard_particle2(-hard_pt,0.0,0.0,hard_pt);
        hard_particle2.set_user_index(2);
        input_jets.push_back(hard_particle2);
      } else if ( nhard > 2 ) { cerr << "nhard must be <= 2" << endl; exit(-1); }
    }
    
    double etamin = -ghost_etamax, etamax = ghost_etamax;
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
    
    // do the clustering WITHOUT the radiated particle
    if (checkpoint) area_def.ghost_spec().checkpoint_random();
    fj::ClusterSequenceArea clust(input_jets,jet_def,area_def);
    // analyse the jets
    vector<fj::PseudoJet> output_jets(sorted_by_pt(clust.inclusive_jets()));
    
    // add radiated particle
    auto_ptr<fj::ClusterSequenceArea> clust_rad;
    vector<fj::PseudoJet> output_jets_rad;	    
    if (emission) {
//       cout << "radiated pt " << radiated.perp() << endl;
       input_jets.push_back(radiated);
       // redo the clustering WITH the radiated particle
       if (checkpoint) area_def.ghost_spec().restore_checkpoint_random();
       clust_rad.reset(new fj::ClusterSequenceArea(input_jets,jet_def,area_def));
       // analyse the jets
       output_jets_rad = sorted_by_pt(clust_rad->inclusive_jets());
    }
         

//    cout << "sizes " << output_jets.size() << " " << output_jets_rad.size() << endl;

    bool hard = false;
    //cout << "# of jets found " << output_jets.size()  << endl;
    for (unsigned j = 0; j < output_jets.size(); j++) {
    int hard_found = 0;
      // only take jets that are reasonably close to center
      if (abs(output_jets[j].rap()) < ghost_etamax - ktR) {
	double normarea = clust.area(output_jets[j])/(fj::pi*pow2(ktR));
	// loop over constituents, look for hard particle
	//cout << " hard_found " << hard_found << endl;
	if ( hard_found < nhard ) {
	  //cout << "j " << j << endl;
	  vector<fj::PseudoJet> constits = clust.constituents(output_jets[j]);
	   for (unsigned k = 0; k < constits.size(); k++) {
	     if (constits[k].user_index() == 2) { hard_found += 1;
	                                          hard = true;
						  break; }
	   }
	}

	if ( hard ) {
	  hard = false;

	  average_area_hard += normarea; 
	  average_ar2_hard  += pow2(normarea);
	  hardareahist.add_entry(normarea);
	  nhardjets++; 
	  
	  if ( emission ) {
	    // look for presence of hard particle in output_jets_rad
	    int ihard;
            for (unsigned m = 0; m < output_jets_rad.size(); m++) {
  	      vector<fj::PseudoJet> constits = clust_rad->constituents(output_jets_rad[m]);
 	      for (unsigned k = 0; k < constits.size(); k++) {
	       if (constits[k].user_index() == 2) { if ( i < 10 ) { cout << "FOUND, m = " << m << endl;}; ihard = m; break;} 
              }
	    }   
	    double normarea_rad = clust_rad->area(output_jets_rad[ihard])/(fj::pi*pow2(ktR));
  	    average_area_rad += weight*(normarea_rad - normarea);
	    average_ar2_rad += pow2(weight*(normarea_rad - normarea));
// debug
//  	    average_area_rad +=	    weight;
//	    cout << "WEIGHT " << weight << endl;
//	    average_ar2_rad +=  pow2(weight);
            if ( i < 10 ) {
  	      cout << "areas " << normarea_rad << " " << normarea << " "
	           << radiated.perp() << " " 
	           << radiated.rap() << " " << weight*(normarea_rad - normarea) << endl;
	    }
	  }
    
          // study pt of hard jet
	  av_pt += output_jets[j].perp();
	  av_pt2 += pow2(output_jets[j].perp());
          
	  // perform subtraction on hard jets
//          double median_pt = clust.pt_per_unit_area(fj::ClusterSequenceActiveArea::median_4vector);
          double median_pt = clust.median_pt_per_unit_area_4vector(ghost_etamax-ktR);
// to use this is wrong unless -pt_scheme is used
//          double median_pt = clust.pt_per_unit_area();
          double hard_area = clust.area(output_jets[j]);
	  fj::PseudoJet areavect = clust.area_4vector(output_jets[j]);
//	  double sub_pt = output_jets[j].perp() - hard_area*median_pt;
	  fj::PseudoJet sub_4vec = output_jets[j] - median_pt*areavect;

	  double sub_pt_4vec = sub_4vec.perp();
	  double sub_pt = sub_pt_4vec;
	  av_sub_pt += sub_pt;
	  av_sub_pt2 += sub_pt*sub_pt;
	  hardptdist.add_entry(sub_pt);
	  ptdist.add_entry(output_jets[j].perp());
	  if ( i < 10 ) { 
  	    cout << "median pt per unit area (median_4vector) " << median_pt << endl;
            cout << " hard_pt, j, jet[j].perp, subtr_pt " << hard_pt << " "  
	         << j << " " << output_jets[j].perp() << " " <<  sub_pt << endl;  
          }
	} else {
	  average_area_soft += normarea; 
	  average_ar2_soft  += pow2(normarea);
	  softareahist.add_entry(normarea);
	  nsoftjets++; 
        }
	
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
    (*ostr) << "# hard_pt      = " << hard_pt    << endl;
    (*ostr) << "# nhard        = " << nhard    << endl;
    (*ostr) << "# soft_pt      = " << soft_pt    << endl;
    (*ostr) << "# nsoft        = " << nsoft    << endl;
    (*ostr) << "# randomness   = " << randomness   << endl;
    (*ostr) << "# R            = " << ktR          << endl;
    (*ostr) << "# ghost_etamax = " << ghost_etamax << endl;
    (*ostr) << "# ghost_area   = " << ghost_area   << endl;
    (*ostr) << "# nev          = " << n            << endl;
    //(*ostr) << "# nhist        = " << nhist        << endl;
    //(*ostr) << "# histmax      = " << histmax      << endl;
    (*ostr) << "# jet def      = " << jet_def.description() << endl;
    (*ostr) << "# w max        = " << rad_pt << endl;
    (*ostr) << "# w cutoff     = " << ptlim << endl;
    (*ostr) << "# dist max     = " << distmax << endl;
    (*ostr) << "# dist cutoff  = " << distlim << endl;
    
    (*ostr) << "# "                                << endl;
    (*ostr) << "# number of events = " << i+1 << endl;
    (*ostr) << "# U.E. energy density = " << float(nsoft)*soft_pt/2/ghost_etamax/fj::twopi << endl;

  
    (*ostr) << "# av of squares " <<  average_ar2_soft/ nsoftjets << endl;
    (*ostr) << "# hard jets = " << nhardjets << endl;
    (*ostr) << "# soft jets = " << nsoftjets << endl;
//    (*ostr) << "# average area hard = " << average_area_hard / nhardjets << 
//     " +- " <<sqrt((average_ar2_hard/nhardjets-pow2(average_area_hard/nhardjets))/nhardjets) << endl;
    (*ostr) << "# average area hard = " << average_area_hard / nhardjets  << 
     " +- "  <<sqrt((average_ar2_hard/nhardjets-pow2(average_area_hard/nhardjets))/nhardjets) << endl;
    (*ostr) << "# average area soft = " << average_area_soft/nsoftjets  << 
     " +- " <<  sqrt((average_ar2_soft/nsoftjets-pow2(average_area_soft/nsoftjets))/nsoftjets) << endl;
    (*ostr) << "# correct av. area hard (?) = " <<  (average_ar2_soft/nsoftjets)/(average_area_soft/nsoftjets) << endl;

    double area_rad = average_area_rad / nhardjets;
    double area_rad_err = sqrt((average_ar2_rad/nhardjets-pow2(average_area_rad/nhardjets))/nhardjets);
    (*ostr) << "# average area rad = " << area_rad  <<  " +- "  << area_rad_err << endl;

    (*ostr) << "# d factor (assuming linearity and intersect in 0) = " << area_rad/coeff*fj::pi/log(rad_pt/ptlim) <<  " +- "  << area_rad_err/coeff*fj::pi/log(rad_pt/ptlim) << endl;

    (*ostr) << "# average pt = " << av_pt/nhardjets << 
     " +- " <<  sqrt((av_pt2/nhardjets-pow2(av_pt/nhardjets))/nhardjets) << endl;
    (*ostr) << "# average subtracted pt = " << av_sub_pt/nhardjets << 
     " +- " <<  sqrt((av_sub_pt2/nhardjets-pow2(av_sub_pt/nhardjets))/nhardjets) << endl;

    if ( hist ) {
      double rescale = 1.0 / (hardareahist.binsize() *  hardareahist.total_weight());
      double softrescale = 1.0 / (softareahist.binsize() * nsoftjets);
      for (unsigned i = 0; i < hardareahist.size(); i++) {
        (*ostr) << hardareahist.binmid(i) 
                << " " << hardareahist[i]*rescale 
  	        << " " << sqrt(hardareahist[i])*rescale 
                << " " << softareahist[i]*softrescale 
	        << " " << sqrt(softareahist[i])*softrescale 
	        << endl;
      }


      (*ostr) << "\n\n" << endl;
      double rescalept = 1.0 / (hardptdist.binsize() * hardptdist.total_weight());
      (*ostr) << "# Subtracted pt" << endl;
      for (unsigned i = 0; i < hardptdist.size(); i++) {
        (*ostr) << hardptdist.binmid(i) 
                << " " << hardptdist[i]*rescalept
  	        << " " << sqrt(hardptdist[i])*rescalept 
	        << endl;
      }


      (*ostr) << "\n\n" << endl;
      rescalept = 1.0 / (ptdist.binsize() * ptdist.total_weight());
      (*ostr) << "# Reconstructed pt" << endl;
      for (unsigned i = 0; i < ptdist.size(); i++) {
        (*ostr) << ptdist.binmid(i) 
                << " " << ptdist[i]*rescalept
  	        << " " << sqrt(ptdist[i])*rescalept 
	        << endl;
      }

    }
  
    (*ostr) << " " << endl;
    if ( outfile != "" ) { delete ostr; }
  
  } // if output
  } // i - loop over events
  
  
}
    
    
    
    
    

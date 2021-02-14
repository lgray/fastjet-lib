/// 
/// Thread safety tests, based on example program provided by 
/// ATLAS via Chris Delitzsch. Requires C++14.
/// 
#include <iostream>
#include <stdlib.h>
#include <fstream>
#include <memory>
#include <iomanip>
#include <thread>
#include "fastjet/ClusterSequence.hh"
#include "fastjet/tools/Filter.hh"
#include "fastjet/tools/Pruner.hh"

using namespace fastjet;
using namespace std;

void groomJets(const Transformer* f, const vector<fastjet::PseudoJet>& ungroomed_jets, uint ig) {
  unsigned int ij=0;
  // loop on jet candidates
  for (const PseudoJet & u : ungroomed_jets){
    // apply groomer f to jet u
    PseudoJet j = (*f)(u);
    cout << "Groomer " << ig << " jet " << ij << " pt: " << setprecision(4) << j.perp() << endl;
    ++ij;
  }
}

int main(int argc, char ** argv) {

#ifndef FASTJET_HAVE_THREAD_SAFETY
  cout << argv[0] << ": FastJet not configured with thread safety, bailing out gracefully" << endl;
  return 0;
#else 
  // Set up the input event once
  //----------------------------------------------------------
  
  // read in input particles
  //----------------------------------------------------------
  vector<fastjet::PseudoJet> input_particles;
  
  double px, py , pz, E;
  ifstream myfile ("../example/data/boosted_top_event.dat");
  while (myfile >> px >> py >> pz >> E) {
    // create a fastjet::PseudoJet with these components and put it onto
    // back of the input_particles vector
    input_particles.push_back(fastjet::PseudoJet(px,py,pz,E)); 
  }
  
  constexpr double R = 1.0;
  fastjet::JetDefinition jet_def(fastjet::antikt_algorithm, R);

  constexpr bool cs_delete_self = true;
  constexpr bool do_parallel_threads = true;

  // Repeatedly process the same event
  for(uint evt=0; evt<100; ++evt) {
    cout << "Start of event " << evt << endl;
    constexpr double ptmin = 5.0;

    // run the jet clustering with the above jet definition
    //----------------------------------------------------------
    // auto clust_seq = new fastjet::ClusterSequence(input_particles, jet_def);
    auto clust_seq = new fastjet::ClusterSequence(input_particles, jet_def);
    // get the resulting jets ordered in pt
    //----------------------------------------------------------
    vector<fastjet::PseudoJet> ungroomed_jets = sorted_by_pt(clust_seq->inclusive_jets(ptmin));
    // Using explicit delete instead of unique_ptr fails
    // -- due to references in PseudoJets?
    unique_ptr<fastjet::ClusterSequence> p_clust_seq;
    if(cs_delete_self) {
      clust_seq->delete_self_when_unused();
    } else {
      p_clust_seq.reset(clust_seq);
    }

    // Generate a few groomers that will process the ungroomed jets
    // Copied from the grooming example, but they don't really need distinct configurations
    vector<unique_ptr<Transformer> > groomers;
    constexpr double Rfilt = 0.3;
    constexpr unsigned int nfilt = 3;
    groomers.emplace_back(std::make_unique<Filter>(JetDefinition(cambridge_algorithm, Rfilt), 
                                                   SelectorNHardest(nfilt) ) );

    constexpr double Rtrim = 0.2;
    constexpr double ptfrac = 0.03;
    groomers.emplace_back(std::make_unique<Filter>(JetDefinition(kt_algorithm, Rtrim), 
                                                   SelectorPtFractionMin(ptfrac) ) );

    constexpr double zcut = 0.1;
    constexpr double rcut_factor = 0.5;
    groomers.emplace_back(std::make_unique<Pruner>(cambridge_algorithm, zcut, rcut_factor));

    // Sanity check, print the input jets
    if(evt==0) {
      for(const PseudoJet & u : ungroomed_jets) {
        cout << "Ungroomed jet pt: " << u.perp() << endl;
      }
    }

    // We spawn a thread to run each of the groomers
    vector<thread> threads;
    uint ig=0;
    for (const unique_ptr<Transformer>& pf : groomers){
      threads.emplace_back(thread(groomJets, pf.get(), ungroomed_jets, ig));
      if(!do_parallel_threads) { threads.back().join(); }; // Run one thread at a time
      ++ig;
    }

    if(do_parallel_threads) {
      // Wait for all the threads to terminate before moving on
      // This makes sure that the ungroomed jets don't go out of scope before the threads complete execution
      for(thread& t : threads) {
        t.join();
      }
    }

  } // event loop

  return 0;
#endif // FASTJET_HAVE_THREAD_SAFETY
}

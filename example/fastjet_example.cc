//----------------------------------------------------------------------
// fastjet example program. 
// Compile it with: make fastjet_example
// run it with    : ./fastjet_example < ../data/single-event.dat
//
// People who are familiar with the ktjet package are encouraged to
// compare this file to the ktjet_example.cc program which does the
// same thing in the ktjet framework.
//----------------------------------------------------------------------
#include "FjPseudoJet.hh"
#include "FjClusterSequence.hh"
#include<iostream> // needed for io
#include<sstream>  // needed for internal io
#include<vector> 

using namespace std;

// a declaration of a function that pretty prints a list of jets
void print_jets (const FjClusterSequence &, const vector<FjPseudoJet> &);

// an example program showing how to use fastjet
int main (int argc, char ** argv) {
  
  vector<FjPseudoJet> input_particles;
  
  // read in input particles
  double px, py , pz, E;
  while (cin >> px >> py >> pz >> E) {
    // create a FjPseudoJet with these components and put it onto
    // back of the input_particles vector
    input_particles.push_back(FjPseudoJet(px,py,pz,E)); 
  }

  // run the jet clustering with option R=1.0 and strategy=Best
  FjClusterSequence clust_seq(input_particles, 0.5, Best);

  // tell the user what was done
  cout << "Strategy adopted by FastJet was "<<
       clust_seq.strategy_string()<<endl<<endl;

  // extract the inclusive jets with pt > 5 GeV, sorted by pt
  double ptmin = 5.0;
  vector<FjPseudoJet> inclusive_jets = clust_seq.inclusive_jets(ptmin);

  // print them out
  cout << "Printing inclusive jets with pt > "<< ptmin<<" GeV\n";
  cout << "---------------------------------------\n";
  print_jets(clust_seq, inclusive_jets);
  cout << endl;

  // extract the exclusive jets with dcut = 25 GeV^2 
  double dcut = 25.0;
  vector<FjPseudoJet> exclusive_jets = clust_seq.exclusive_jets(dcut);

  // print them out
  cout << "Printing exclusive jets with dcut = "<< dcut<<" GeV^2\n";
  cout << "--------------------------------------------\n";
  print_jets(clust_seq, exclusive_jets);


}


//----------------------------------------------------------------------
// a function that pretty prints a list of jets
void print_jets (const FjClusterSequence & clust_seq, 
		 const vector<FjPseudoJet> & jets) {

  // sort jets into increasing pt
  vector<FjPseudoJet> sorted_jets = sorted_by_pt(jets);  

  // label the columns
  printf("%5s %15s %15s %15s %15s\n","jet #", "rapidity", 
	 "phi", "pt", "n constituents");
  
  // print out the details for each jet
  for (unsigned int i = 0; i < sorted_jets.size(); i++) {
    int n_constituents = clust_seq.constituents(sorted_jets[i]).size();
    printf("%5u %15.8f %15.8f %15.8f %8u\n",
	   i, sorted_jets[i].rap(), sorted_jets[i].phi(),
	   sorted_jets[i].perp(), n_constituents);
  }

};

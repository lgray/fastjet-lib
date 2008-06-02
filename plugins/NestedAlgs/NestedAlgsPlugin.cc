// fastjet stuff
#include "fastjet/ClusterSequence.hh"
#include "fastjet/NestedAlgsPlugin.hh"

// other stuff
#include <vector>
#include <sstream>

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

using namespace std;

string NestedAlgsPlugin::description () const {
  ostringstream desc;
  
  desc << "NestedAlgs: successive application of " ;
  unsigned int i=1;
  for (list<JetDefinition>::const_iterator it=algs.begin();it!=algs.end();it++){
    desc << "Alg. " << i++ << " [" << it->description() << "] - ";
  }

  return desc.str();
}

void NestedAlgsPlugin::run_clustering(ClusterSequence & clust_seq) const {
  vector<PseudoJet> momenta;

  // build the initial list of particles
  momenta = clust_seq.jets();
  unsigned int step_n = momenta.size();

  // initialise the conversion table
  vector<unsigned int> conversion_table(2*step_n);
  vector<unsigned int> new_conversion_table;
  for (unsigned int i=0;i<step_n;i++)
    conversion_table[i]=i;

  // Now the steps go as follows:
  // for each alg in the list, 
  //  - do the clustering,
  //  - copy the history into the main one
  //  - update the list of momenta and the index consersion table
  list<JetDefinition>::const_iterator it = algs.begin();

  while (it!=algs.end()){
    // do the clustering
    ClusterSequence step_cs(momenta, *it);

    // clear the momenta as we shall fill them again
    momenta.clear();
    new_conversion_table.clear();

    // retrieve the history
    const vector<ClusterSequence::history_element> & step_history = step_cs.history();

    // copy the history
    // note that we skip the initial steps which are just the 
    // declaration of the particles.
    vector<ClusterSequence::history_element>::const_iterator hist_it = step_history.begin();
    for (unsigned int i=step_n;i!=0;i--)
      hist_it++;
    while (hist_it != step_history.end()){
      // check if it is a recombination with the beam or a simple recombination
      if (hist_it->parent2 == ClusterSequence::BeamJet){
	// save this jet for future clustering
	unsigned int jet_index = step_cs.history()[hist_it->parent1].jetp_index;
	momenta.push_back(step_cs.jets()[jet_index]);
	new_conversion_table.push_back(conversion_table[jet_index]);
      } else {
	// record combination
	unsigned int jet1_index = step_cs.history()[hist_it->parent1].jetp_index;
	unsigned int jet2_index = step_cs.history()[hist_it->parent2].jetp_index;
	PseudoJet newjet = step_cs.jets()[hist_it->jetp_index];
	int jet_k;
	clust_seq.plugin_record_ij_recombination(conversion_table[jet1_index], 
						 conversion_table[jet2_index],
						 hist_it->dij, newjet, jet_k);

	// save info in the conversion table for tracking purposes
	conversion_table[hist_it->jetp_index]=jet_k;
      }

      // go to the next history element
      hist_it++;
    }

    // finalise this step:
    //  - update nr of particles
    //  - update conversion table
    step_n = momenta.size();
    for (int i=0;i<step_n;i++)
      conversion_table[i] = new_conversion_table[i];

    // go to the next alg
    it++;
  }

  // now all the algs have been applied, we can recombine the remaining 
  // particles with the beam to make them jets.
  // the position of the momenta in the main "jets()" vector
  // are encoded in the conversion_table so we know where they are
  double Rlast=R();
  for (int i=0;i<step_n;i++){
    clust_seq.plugin_record_iB_recombination(conversion_table[i], Rlast);
  }
}

FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

#ifndef __TESTGROOMERAREAS_HH__
#define __TESTGROOMERAREAS_HH__

/// \file TestPseudoJet.hh 
/// Provides a series of tests of PseudoJets

#include "TestBase.hh"
#include "fastjet/ClusterSequenceArea.hh"
#include "fastjet/tools/Filter.hh"
#include "fastjet/tools/Pruner.hh"

/// class to test some of the basic features of the CASubJetTagger
class TestGroomerAreas : public TestBase {
  std::string short_name()  const {return "TestGroomerAreas";}

  bool run_test () {
    JetDefinition jet_def(kt_algorithm, 1.0);
    AreaDefinition area_def(active_area_explicit_ghosts);
    ClusterSequenceArea csa(default_event(), jet_def, area_def);
    
    PseudoJet jet = SelectorNHardest(1)(csa.inclusive_jets())[0];

    vector<PseudoJet> pieces = jet.pieces();
    PseudoJet jet2 = join(jet.pieces());
    PseudoJet jet3 = join(join(pieces[0].pieces()),pieces[1]);

    // cout << jet.has_associated_cluster_sequence() << endl;
    // cout << jet2.has_associated_cluster_sequence() << endl;
    // cout << pieces[0].pieces().size() << endl;

    _pass_test = true;

    _correct_area = -1.0;
    Filter filter(0.3,SelectorNHardest(2));
    _pass_test &= check_groomer(jet , filter, "1-piece       jet, filter");
    _pass_test &= check_groomer(jet2, filter, "1+1-piece     jet, filter");
    _pass_test &= check_groomer(jet3, filter, "(1+1)+1-piece jet, filter");
    
    _correct_area = -1.0;
    Pruner pruner(kt_algorithm,0.1, 0.5);
    _pass_test &= check_groomer(jet , pruner, "1-piece       jet, pruner");
    _pass_test &= check_groomer(jet2, pruner, "1+1-piece     jet, pruner");
    _pass_test &= check_groomer(jet3, pruner, "(1+1)+1-piece jet, pruner");
    
    return _pass_test;
  }

  // a test that should work
  bool check_groomer(const PseudoJet & jet, const Transformer & groomer, 
		     const string & testname) {
    PseudoJet groomed_jet = groomer(jet);
    //cout << jet.area() << " " << groomed_jet.area() << endl;
    if (!verify_equal(groomed_jet.has_area(), true, testname+" (has area)")) return false;
    if (_correct_area >= 0.0) {
      return verify_almost_equal(groomed_jet.area(), _correct_area,
				 testname+" (area equality)");
    } else {
      _correct_area = groomed_jet.area();
      return true;
    }
  }
  
  double _correct_area;
};


#endif // __TESTGROOMERAREAS_HH__

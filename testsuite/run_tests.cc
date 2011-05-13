//----------------------------------------------------------------------
/// \file run_tests.cc
///
/// Usage:
///   ./run-tests [-verbose] 
#include "TestBase.hh"
#include "TestPseudoJet.hh"
#include <iomanip>
#include "CmdLine.hh"

int main(int argc, char** argv) {
  CmdLine cmdline(argc,argv);

  bool verbose = cmdline.present("-verbose");

  vector<TestBase *> tests;

  // the list of tests we will perform
  tests.push_back(new TestPtYPhiM());
  tests.push_back(new TestPJAssignment());

  // loop over the tests
  for (unsigned i = 0; i < tests.size(); i++) {
    bool pass = tests[i]->run_test();
    if (pass) {
      cout << setw(4) << i << "  PASS: " << tests[i]->short_name() <<endl;
      if (verbose) tests[i]->print_OK(cout, 15);
    } else {
      cout << setw(4) << i << "  FAIL: " << tests[i]->short_name() <<endl;
      cout << "      (" << tests[i]->description() << ")" << endl;
      tests[i]->print_failures();
    }
    // clean up after the test....
    delete tests[i];
  }
  
}

#include "TestBase.hh"
#include "TestPseudoJet.hh"
#include <iomanip>


int main() {

  vector<TestBase *> tests;

  // the list of tests we will perform
  tests.push_back(new TestPtYPhiM());

  for (unsigned i = 0; i < tests.size(); i++) {
    bool pass = tests[i]->run_test();
    if (pass) {
      cout << setw(4) << i << "  PASS: " << tests[i]->short_name() <<endl;
    } else {
      cout << setw(4) << i << "  FAIL: " << tests[i]->short_name() <<endl;
      tests[i]->print_failures();
    }
  }
  
}

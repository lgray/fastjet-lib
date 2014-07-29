#ifndef __TESTRECOMBINERS_HH__
#define __TESTRECOMBINERS_HH__

#include "TestBase.hh"
#include "fastjet/JetDefinition.hh"

/// class to test some things that happen with Groomers and Recombiners
/// put together
class TestRecombiners : public TestBase {
  std::string short_name()  const {return "TestRecombiners";}

  virtual bool run_test () {
    double R = 1.0;
    JetDefinition jd1(antikt_algorithm, R);

    PseudoJet a = PtYPhiM(100.0, 0, 0,    1.0);
    PseudoJet b = PtYPhiM(50.0,  0, pi/4, 1.0);
    
    jd1.set_recombination_scheme(BIpt_scheme);
    PseudoJet c;
    jd1.recombiner()->recombine(a, b, c);
    verify_almost_equal(c.phi(), pi/12.0, "BIpt_scheme phi");

    return _pass_test;
  }
};
#endif // __TESTRECOMBINERS_HH__

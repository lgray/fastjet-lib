#ifndef __TESTPSEUDOJET_HH__
#define __TESTPSEUDOJET_HH__

/// \file TestPseudoJet.hh 
/// Provides a series of tests of PseudoJets

#include "TestBase.hh"


//----------------------------------------------------------------------
class TestPtYPhiM : public TestBase {
  std::string short_name()  const {return "TestPtYPhiM";}
  std::string description() const {return short_name();}

  bool run_test () {
    for (unsigned i = 0; i < 100; i++) {
      // range of pt values chosen affects choice for tolerance below
      double pt = uniform_random(1.0, 10.0);
      double rap = uniform_random(-5.0, 5.0);
      double phi = uniform_random(0.0, twopi);

      PseudoJet p = PtYPhiM(pt, rap, phi);
      verify_equal(pt , p.perp(),  "pt test (m=0)");
      verify_equal(rap, p.rap() ,  "rap test (m=0)");
      verify_equal(phi, p.phi() ,  "phi test (m=0)");
      verify_equal(0  , p.m()   ,  "m test (m=0)", 1e-5); // lower tolerance

      p += PseudoJet(1e-100,1e-100,1e-100,1e-100);
      verify_equal(pt , p.perp(),  "2nd pt test (m=0)");
      verify_equal(rap, p.rap() ,  "2nd rap test (m=0)");
      verify_equal(phi, p.phi() ,  "2nd phi test (m=0)");
      verify_equal(0  , p.m()   ,  "2nd m test (m=0)", 1e-5); // lower tolerance

      double m = uniform_random(40.0,100.0);
      p = PtYPhiM(pt, rap, phi, m); 
      verify_equal(pt , p.perp(),  "2nd pt test");
      verify_equal(rap, p.rap() ,  "2nd rap test");
      verify_equal(phi, p.phi() ,  "2nd phi test");
      verify_equal(m  , p.m()   ,  "2nd m test", 1e-5); // lower tolerance

      p += PseudoJet(1e-100,1e-100,1e-100,1e-100);
      verify_equal(pt , p.perp(),  "2nd pt test");
      verify_equal(rap, p.rap() ,  "2nd rap test");
      verify_equal(phi, p.phi() ,  "2nd phi test");
      verify_equal(m  , p.m()   ,  "2nd m test", 1e-5); // lower tolerance
    }
    return _pass_test;
  }
};

#endif __TESTPSEUDOJET_HH__

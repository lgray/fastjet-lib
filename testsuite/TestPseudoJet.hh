#ifndef __TESTPSEUDOJET_HH__
#define __TESTPSEUDOJET_HH__

/// \file TestPseudoJet.hh 
/// Provides a series of tests of PseudoJets

#include "TestBase.hh"



//----------------------------------------------------------------------
/// Class to test that assignments via PtYPhiM are being performed
/// correctly
class TestPtYPhiM : public TestBase {
  std::string short_name()  const {return "TestPtYPhiM";}

  bool run_test () {
    for (unsigned i = 0; i < 10; i++) {
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
      verify_equal(0  , p.m()   ,  "2nd m test (m=0)", 1e-10); // lower tolerance

      PseudoJet pp = p;
      pp = pp + PseudoJet(1e-100,1e-100,1e-100,1e-100);
      verify_equal(p,pp, "usual addition");

      double m = uniform_random(1.0,10.0);
      p = PtYPhiM(pt, rap, phi, m); 
      verify_equal(pt , p.perp(),  "pt test");
      verify_equal(rap, p.rap() ,  "rap test");
      verify_equal(phi, p.phi() ,  "phi test");
      verify_equal(m  , p.m()   ,  "m test", 1e-5); // lower tolerance

      p += PseudoJet(1e-100,1e-100,1e-100,1e-100);
      verify_equal(pt , p.perp(),  "2nd pt test");
      verify_equal(rap, p.rap() ,  "2nd rap test");
      verify_equal(phi, p.phi() ,  "2nd phi test");
      verify_equal(m  , p.m()   ,  "2nd m test", 1e-5); // lower tolerance
    }
    return _pass_test;
  }
};


//----------------------------------------------------------------------
class Info : public PseudoJet::UserInfoBase {
public:
  Info() : pdg(21) {}
  int pdg;
};

//----------------------------------------------------------------------
class MyPseudoJet : public PseudoJet {
public:
  MyPseudoJet() {set_user_info(new Info());}
  MyPseudoJet(const PseudoJet & in) {
    reset(in);
    if (dynamic_cast<const Info *>(user_info_ptr()) == 0) {
      set_user_info(new Info());
    }
  }
  
  // NB dangerous if reset(...) has been called...
  int pdg_id() const {return user_info<Info>().pdg;}
};

//----------------------------------------------------------------------
/// Tests of assignments and resets of PseudoJets
class TestPJAssignment : public TestBase {
 std::string short_name()  const {return "TestPJAssignment";}
 std::string description()  const {return "Tests of assignments and resets of PseudoJets";}

  bool run_test () {
    
    PseudoJet a = random_PtYPhiM();

    // check default indices
    verify_equal(a.user_index(),         -1, "default user index");
    verify_equal(a.cluster_hist_index(), -1, "default cluster index");
    verify_equal(a.user_info_ptr(),       0, "default user_info");
    verify_equal(a.structure_ptr(),       0, "default structure");

    // set indices -- we'll check them again later
    a.set_user_index(10);
    a.set_cluster_hist_index(11);

    // check assignments and resets
    PseudoJet b = a;
    verify_equal(a, b, "assignment from PJ");
    b.reset(a);
    verify_equal(a, b, "reset from PJ");

    double p[4];
    p[0] = a.px();
    p[1] = a.py();
    p[2] = a.pz();
    p[3] = a.E();

    // checks assignemnts and resets from 4-vectors
    PseudoJet c(p);
    verify_equal(a, b, "assignment from 4-vector");
    PseudoJet d;
    d.reset(p);
    verify_equal(a, b, "reset from 4-vector");

    MyPseudoJet particle(a);
    verify_equal(particle.pdg_id(), 21 , "default pdg ID");

    // checks that assignments and resets from MyPJ -> PJ -> MyPJ behave sensibly
    b = particle; // remember b is a PseudoJet
    MyPseudoJet particle2(b);
    verify_equal(particle, particle2, "MyPJ -> PJ -> MyPJ (via assignment)");
    MyPseudoJet particle3;
    particle3.reset(b);
    verify_equal(particle, particle3, "MyPJ -> PJ -> MyPJ (via reset)");
    
    // NB these values were set earlier
    verify_equal(b.user_index(),         10, "remembering modified user index");
    verify_equal(b.cluster_hist_index(), 11, "remembering modified cluster index");

    // make sure that reset with simple 4-component vector also resets everything else
    MyPseudoJet particle4;
    particle4.reset(b.px(), b.py(), b.pz(), b.E());

    verify_different(particle.user_info_ptr(), particle4.user_info_ptr(), "user info reset");
    verify_equal(particle4.user_index(),         -1, "default user index on reset from 4-mom");
    verify_equal(particle4.cluster_hist_index(), -1, "default clust index on reset from 4-mom");
    verify_equal(particle4.user_info_ptr(),       0, "default user_info");
    verify_equal(particle4.structure_ptr(),       0, "default structure");

    return _pass_test;
  }
};

#endif // __TESTPSEUDOJET_HH__


FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

class TestBase {
  virtual std::string description() const = 0;
  virtual bool run_test();

  // some utility functions
  bool equality_within_tolerance(...);
};


class TestPassiveArea : public TestBase {
  bool run_test ()
}

class TestPtYPhiM : public TestBase {
  bool run_test () {
    for (unsigned i = 0; i < 100; i++) {
      PseudoJet p = PtYPhiM(.........);
      if (p.rap() != rap) return false;
      if (p.phi() != phi) return false;

      p += PseudoJet(1e-100,1e-100,1e-100,1e-100);
      if (p.rap() != rap) return false;
      if (p.phi() != phi) return false;
    }
  }
}


FASTJET_END_NAMESPACE

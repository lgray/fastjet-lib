#ifndef __TESTBASE_HH__
#define __TESTBASE_HH__

#include "fastjet/PseudoJet.hh"
#include "fastjet/internal/BasicRandom.hh"
#include <iostream>
#include <sstream>

using namespace std;
using namespace fastjet;

//FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

//----------------------------------------------------------------------
/// Base class setting out the basic functionality that any test needs
/// to provide, and also including some helper functions
class TestBase {
public:
  TestBase() : _pass_test(true) {}

  virtual std::string description() const = 0;
  virtual std::string short_name()  const = 0;
  virtual bool run_test() = 0;

  /// a helper function to verify equality to within some specified
  /// tolerance, with the tolerance defined as a relative tolerance
  /// for large numbers (>>1) and an absolute tolerance for small
  /// numbers (<<1).
  bool equal_within_tolerance(double a, double b, double tol = -1.0) const {
    double local_tol = tol >= 0 ? tol : default_tolerance();
    return abs(a-b) < local_tol*(1+max(abs(a),abs(b)));
  }

  /// verifies two things are equal within tolerance; if not it
  /// registers failure in the _pass_test
  void verify_equal(double a, double b, const string & point, double tol = -1.0) {
    if (!equal_within_tolerance(a,b,tol)) {
      _pass_test = false;
      std::ostringstream ostr;
      ostr << point << ": " << a << " != " << b << " (within tol = " << tol << ")";
      _failure_points.push_back(ostr.str());
    }
  }

  /// print a list of all the failures
  void print_failures(std::ostream & ostr = std::cout, unsigned max_print = 5) {
    for (unsigned i=0; i < min(_failure_points.size(), max_print); i++) {
      ostr << "           " << _failure_points[i] << endl;
    }
    if (_failure_points.size() > max_print) {
      ostr << "           ... and " << _failure_points.size() - max_print 
	   << " more failures ... " << endl;
    }
  }

  /// a default tolerance to use when tolerances are not specified
  virtual double default_tolerance() const {
    return 1e-10;
  }


protected:
  BasicRandom<double> random;
  /// return a random number in the range xmin to xmax
  double uniform_random(double xmin, double xmax) {return xmin + (xmax-xmin)*random();}
  
  bool _pass_test;
  std::vector<std::string> _failure_points;
};


//FASTJET_END_NAMESPACE

#endif // __TESTBASE_HH__

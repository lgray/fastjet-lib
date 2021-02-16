#ifndef __TESTTHREADSBASE_HH__
#define __TESTTHREADSBASE_HH__
#include "TestBase.hh"
#include <thread>
#include <sstream>

using namespace std;
using namespace fastjet;



template<class R>
void thread_run_test(R * test, unsigned itest) {
  test->run_test_i(itest);
}

/// a class to help with thread tests;
///
/// It is to be templated with a subclass of ThreadedTestBase, creates
/// two copies of the template class, one which executes a series of
/// tests sequentially, the other which executes them in a set of
/// threads. It then compares the results.
template<class R>
class TestThread : public TestBase {

public:
  TestThread() {}
  /// descriptions are taken from the template class
  std::string description() const {return threaded.description();}
  std::string short_name()  const {return threaded.short_name();}

  bool run_test() {
    
    unsigned n = sequential.n_threads();

    // first do the threaded part
    vector<unique_ptr<thread>> threads;
    for (unsigned i = 0; i < n; i++) {
      threads.emplace_back(make_unique<thread>(thread_run_test<R>, &threaded, i));
    }
    for (unsigned i = 0; i < n; i++) {
      threads[i]->join();
    }

    // then run the sequential test
    for (unsigned i = 0; i < n; i++) {
      thread_run_test<R>(&sequential, i);
    }


    // finally check the results are in agreement
    bool outcome = true;
    for (unsigned i = 0; i < n; i++) {
      ostringstream ostr;
      ostr << short_name() << ": size of result from thread " << i;
      outcome &= verify_equal(sequential.result()[i].size(), 
                              threaded.result()[i].size(), ostr.str());
      for (unsigned j = 0; j < sequential.result()[i].size(); j++) {
        ostringstream ostr2;
        ostr2 << ostr.str() << ", value " << j;
        // last argument (true) tells verify_almost_equal to ignore 
        // structure info in its test
        outcome &= verify_almost_equal(sequential.result()[i][j], 
                      threaded.result()[i][j], ostr2.str(), -1 , true);
      }
    }
    return outcome;
  }

protected:
  R sequential;
  R threaded;
  /// stores the result of running in a thread-safe manner
  vector<vector<R> > _result;
};

//--------------------------------------------------------------------
/// Base class for threaded tests; it provides
///
/// - tools to load events
/// - a function run_test_i(i) to run test i
///   (the TestThread class will run this sequentially on one 
///   copy of the class and in parallel on the other copy)
/// - storage for the outcome of the test (_result)
///
/// The class is templated according to the type of the underlying
/// results, and that type should be supported by the TestThread class
/// (double or PseudoJet) 
template<class S>
class ThreadedTestBase {
public:
  ThreadedTestBase(unsigned n = 0) {_result.resize(n);}
  void set_size(unsigned n) {_result.resize(n);}
  virtual ~ThreadedTestBase() {}

  /// this is the critical part that the user needs to implement
  /// (together with the constructor))
  virtual void run_test_i(unsigned i) = 0;

  /// default short name is the class name (this will take on the
  /// derived class name, albeit in its mangled form)
  virtual std::string short_name()  const {return typeid(*this).name();}
  /// one can include a more detailed description
  virtual std::string description() const {return short_name();}

  /// the number of threads that this is meant to be run across
  unsigned int n_threads() const {return result().size();}

  /// access to the results
  const vector<vector<S> > & result() const {return _result;}

  /// returns the event corresponding to the given filename
  void load_event(const string & filename) {
    ifstream istr(filename.c_str());
    double px, py , pz, E;
    vector<PseudoJet> input_particles;
    while (istr >> px >> py >> pz >> E) {
      // create a fastjet::PseudoJet with these components and put it onto
      // back of the input_particles vector
      input_particles.push_back(fastjet::PseudoJet(px,py,pz,E)); 
      input_particles.back().set_user_index(input_particles.size()-1);
    }
    _events.push_back(input_particles);
  }
  
  //----------------------------------------------------
  void load_default_event() {
    return load_event("../example/data/single-event.dat");
  }

protected:
  vector<vector<S> > _result;
  vector<vector<PseudoJet>> _events;
};


//-------------------------------------------------------------
/// just generates a banner; we can't explicit test the outcome
/// of this, but when running the code we'll look to see
/// how many times the banner comes out...
class ThreadedBanner : public ThreadedTestBase<double> {
public:
  ThreadedBanner() : ThreadedTestBase<double>(8) {
  }

  //std::string short_name() const {return typeid(*this).name();}

  void run_test_i(unsigned i) {
    ClusterSequence::print_banner();
  }
};

//-------------------------------------------------------------
/// since rap and phi have cached calculations, this test
/// checks that if we evaluate them in separate threads 
/// we get the same answers
class ThreadedTestPhiRap : public ThreadedTestBase<double> {
public:

  ThreadedTestPhiRap() : ThreadedTestBase<double>(8) {
    load_default_event();
  }

  void run_test_i(unsigned i) {
    _result[i].reserve(_events[0].size() * 4);
    for (const auto & j: _events[0]) {
      _result[i].push_back(j.phi());
      _result[i].push_back(j.rap());
      _result[i].push_back(j.phi());
      _result[i].push_back(j.rap()); 
    }
  } 
};

//-------------------------------------------------------------
/// Test clutering with multiple algorithms
class ThreadedClustering1Ev : public ThreadedTestBase<PseudoJet> {
public:

  ThreadedClustering1Ev() : _R_values{0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 1.0} {
    set_size(_R_values.size());
    load_default_event();
  }

  void run_test_i(unsigned i) {
    JetDefinition jet_def(antikt_algorithm, _R_values[i]);
    ClusterSequence cs(_events[0], jet_def);
    _result[i] = cs.inclusive_jets();
  } 

protected:
  vector<double> _R_values;
};


#endif // __TESTTHREADSBASE_HH__

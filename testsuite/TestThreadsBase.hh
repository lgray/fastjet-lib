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

/// a class to help with thread tests
template<class R>
class TestThread : public TestBase {

public:
  TestThread() {}
  std::string description() const {return threaded.description();}
  std::string short_name()  const {return threaded.short_name();}

  bool run_test() {

    
    // first run the sequential test
    unsigned n = sequential.n_threads();
    for (unsigned i = 0; i < n; i++) {
      thread_run_test<R>(&sequential, i);
    }

    // now do the threaded part
    vector<unique_ptr<thread>> threads;
    for (unsigned i = 0; i < n; i++) {
      threads.emplace_back(make_unique<thread>(thread_run_test<R>, &threaded, i));
    }
    for (unsigned i = 0; i < n; i++) {
      threads[i]->join();
    }

    // finally check the results are in agreement
    bool outcome = true;
    for (unsigned i = 0; i < n; i++) {
      ostringstream ostr;
      ostr << short_name() << ": size of result from thread " << i;
      outcome &= verify_equal(sequential.result()[i].size(), threaded.result()[i].size(), ostr.str());
      for (unsigned j = 0; j < sequential.result()[i].size(); j++) {
        ostringstream ostr2;
        ostr2 << ostr.str() << ", value " << j;
        outcome &= verify_almost_equal(sequential.result()[i][j], threaded.result()[i][j], ostr2.str());
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

//------------------------------------------------
template<class S>
class ThreadedTestBase {
public:
  ThreadedTestBase(unsigned n) {_result.resize(n);}
  virtual ~ThreadedTestBase() {}

  virtual std::string short_name()  const = 0;
  virtual void run_test_i(unsigned i) = 0;
  virtual std::string description() const {return short_name();}

  unsigned int n_threads() const {return result().size();}
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


/// since rap and phi have cached calculations, this test
/// checks that if we evaluate them in separate threads 
/// we get the same answers
class ThreadedTestPhiRap : public ThreadedTestBase<double> {
public:
  ThreadedTestPhiRap() : ThreadedTestBase<double>(8) {
    load_default_event();
  }

  std::string short_name() const {return "ThreadedTestPhiRap";}

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


#endif // __TESTTHREADSBASE_HH__

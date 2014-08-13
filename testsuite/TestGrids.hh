#ifndef __TESTGRIDS_HH__
#define __TESTGRIDS_HH__

#include "TestBase.hh"
#include "fastjet/tools/GridMedianBackgroundEstimator.hh"


/// class to test some things that happen with Groomers and Grids
/// put together
class TestGrids : public TestBase {
  std::string short_name()  const {return "TestGrids";}

  virtual bool run_test () {
    
    RectangularGrid grid;
    VERIFY_THROWS(GridMedianBackgroundEstimator gmbge(grid), "GMBGE constructor with default (uninitialised) grid throws");

    grid = RectangularGrid(2.0, 5.0, 0.5, twopi/12.0, !SelectorRapRange(3.0,4.0));
    verify_equal(grid.n_tiles(), 72, "total number of grid tiles");
    verify_equal(grid.n_good_tiles(), 48, "number of good grid tiles");
    
    return _pass_test;
  }
};
#endif // __TESTGRIDS_HH__



#include "fastjet/Error.hh"

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

// a dummy tool that is present just to ensure that the library is not
// empty (and avoid error on OS X)
void dummy_tool() {
  throw Error("this dummy routine should not be called");
}


FASTJET_END_NAMESPACE

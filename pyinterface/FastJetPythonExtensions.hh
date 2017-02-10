#ifndef __FASTJET_PYTHONUSERINFO_HH__
#include "fastjet/PseudoJet.hh"
#include "Python.h"

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

class UserInfoPython : public fastjet::PseudoJet::UserInfoBase {
public:
  UserInfoPython(PyObject * pyobj) : _pyobj(pyobj) {
    Py_XINCREF(_pyobj);
  }

  PyObject * get_pyobj() const {return _pyobj;}
  //const PyObject * get_pyobj() const {return _pyobj;}
  
  ~UserInfoPython() {
    Py_XDECREF(_pyobj);
  }
private:
  PyObject * _pyobj;
};

FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

#endif // __FASTJET_PYTHONUSERINFO_HH__

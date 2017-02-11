#ifndef __FASTJET_PYTHONUSERINFO_HH__
#include "fastjet/PseudoJet.hh"
#include "fastjet/Selector.hh"
#include "fastjet/Error.hh"
#include "Python.h"

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

//----------------------------------------------------------------------
/// \class UserInfoPython
/// Internal helper class for making user info usable within python
///
/// This is an internal class that makes possible the calls to
///   pseudojet->set_python_info(object)
///   object = pseudojet->python_info()
/// through the PseudoJet::UserInfoBase interface and for any object
/// class in python
class UserInfoPython : public fastjet::PseudoJet::UserInfoBase {
public:
  UserInfoPython(PyObject * pyobj) : _pyobj(pyobj) {
    Py_XINCREF(_pyobj);
  }

  PyObject * get_pyobj() const {
    // since there's going to be an extra reference to this object
    // one must increase the reference count; it seems that this
    // is _our_ responsibility
    Py_XINCREF(_pyobj);
    return _pyobj;
  }
  //const PyObject * get_pyobj() const {return _pyobj;}
  
  ~UserInfoPython() {
    Py_XDECREF(_pyobj);
  }
private:
  PyObject * _pyobj;
};

//----------------------------------------------------------------------
/// \class SelectorWorkerPython
/// Internal class for making python functions usable as selectors
///
/// This is an internal class that makes possible the calls to
///   selector = Selector(pyton_function)
/// where python_function will take a PseufoJet as argument and return
/// a bool
class SelectorWorkerPython : public SelectorWorker{
public:
  SelectorWorkerPython(PyObject *py_function) : _py_function(py_function){
    Py_XINCREF(_py_function);

    // we directly make sure that the function is callable
    if (!PyCallable_Check(_py_function)){
      PyErr_SetString(PyExc_TypeError, "SelectorWorkerPython::SelectorWorkerPython: the argument should be callable");
      // do we also throw a fastjet error?
    }
  }

  ~SelectorWorkerPython(){
    Py_XDECREF(_py_function);
  }    

  virtual std::string description() const{
    return "Selector based on python function";
  }

  virtual bool pass(const PseudoJet &jet) const{
    // first make a copy of the jet in a PyObject* managed by swig
    PseudoJet jet_copy = jet;  // not sure this is needed
    PyObject *py_jet = 0;
    py_jet = SWIG_NewPointerObj((new fastjet::PseudoJet(static_cast< const fastjet::PseudoJet& >(jet_copy))), SWIGTYPE_p_fastjet__PseudoJet, SWIG_POINTER_OWN |  0 );

    Py_XINCREF(_py_function);
    PyObject * args = Py_BuildValue("(O)", py_jet);
    PyObject *py_result = PyObject_CallObject(_py_function, args);
    Py_XDECREF(_py_function);

    if (py_result == NULL)
      throw Error("SelectorWorkerPython::pass(): call to python function returned a NULL result.");

    // somehow the conversion from bool via SWIG_AsVal_bool is not available
    //bool result;
    //int conversion_result = SWIG_AsVal_bool(py_result, &result);
    //if (!SWIG_IsOK(conversion_result)){
    //  throw Error("SelectorWorkerPython::pass(): the value returned by the python function could not be casted to a bool");
    //}

    if (!PyBool_Check(py_result))
      throw Error("SelectorWorkerPython::pass(): the value returned by the python function could not be cast to a bool");
    int result = PyObject_IsTrue(py_result);
    if (result == -1)
      throw Error("SelectorWorkerPython::pass(): the value returned by the python function could not be cast to a bool");
    Py_XDECREF(py_result);

    return result;
  }
  
private:
  PyObject *_py_function;
};

// effectively create a Selector for python
Selector SelectorPython(PyObject *py_function) {
  return Selector(new SelectorWorkerPython(py_function));
}



FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

#endif // __FASTJET_PYTHONUSERINFO_HH__

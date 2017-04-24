#ifndef __FASTJET_PYTHONUSERINFO_HH__
#include "fastjet/PseudoJet.hh"
#include "fastjet/JetDefinition.hh"
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
/// Internal class for making python classes/functions usable as selectors
///
/// This is an internal class that makes possible the calls to
///   selector = Selector(pyton_function)
/// where python_function will take a PseufoJet as argument and return
/// a bool
class SelectorWorkerPython : public SelectorWorker{
public:
  SelectorWorkerPython(PyObject *py_class_or_function) : _py_class_or_function(py_class_or_function){
    Py_XINCREF(_py_class_or_function);

    // we directly make sure that the function is callable
    if (!PyCallable_Check(_py_class_or_function)){
      PyErr_SetString(PyExc_TypeError,
          "SelectorWorkerPython::SelectorWorkerPython: the argument should be callable");
      // do we also throw a fastjet error?
    }
  }

  ~SelectorWorkerPython(){
    Py_XDECREF(_py_class_or_function);
  }    

  virtual std::string description() const{
    if (PyObject_HasAttrString(_py_class_or_function, "__str__")){
      Py_XINCREF(_py_class_or_function);
      PyObject* result = PyObject_Str(_py_class_or_function);
      const char *str_result = PyString_AsString(result);
      Py_XDECREF(_py_class_or_function);
      return std::string("Selector based on python condition ")+std::string(str_result);
    }
    return "Selector based on python function";
  }

  virtual bool pass(const PseudoJet &jet) const{
    // first make a copy of the jet in a PyObject* managed by swig
    PseudoJet jet_copy = jet;  // not sure this is needed
    PyObject *py_jet = 0;
    py_jet = SWIG_NewPointerObj((new fastjet::PseudoJet(static_cast< const fastjet::PseudoJet& >(jet_copy))), SWIGTYPE_p_fastjet__PseudoJet, SWIG_POINTER_OWN |  0 );

    Py_XINCREF(_py_class_or_function);
    PyObject * args = Py_BuildValue("(O)", py_jet);
    PyObject *py_result = PyObject_CallObject(_py_class_or_function, args);
    Py_XDECREF(_py_class_or_function);

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
    Py_XDECREF(py_result); ///???

    return result;
  }
  
private:
  PyObject *_py_class_or_function;
};

// effectively create a Selector for python
Selector SelectorPython(PyObject *py_function) {
  return Selector(new SelectorWorkerPython(py_function));
}

//----------------------------------------------------------------------
/// \class RecombinerPython
/// Class allowing user-defined Recombiners in python
///
/// If a (python) user implements a (python) class providing the 
///   __str__()
///   PseudoJet recombine(PseudoJet pa, PseudoJe pb)
///   PseudoJet preprocess(PseudoJet pa)
/// methods
///
/// Note that compared to the C++ implementation, the result is
/// returned by the method rather than being passed as a reference.
class RecombinerPython : public JetDefinition::Recombiner{
public:
  /// ctor with the python recombier class as an argument
  RecombinerPython(PyObject *py_class) : _py_class(py_class){
    Py_XINCREF(_py_class);
    
    // here we could add some tests that the class has the required
    // methods
  }
  
  /// dtor
  virtual ~RecombinerPython(){
    Py_XDECREF(_py_class);
  }

  /// return a textual description of the recombiner
  virtual std::string description() const{
    if (! PyObject_HasAttrString(_py_class, "__str__")){
      throw Error("RecombinerPython: the provided class should implement the __str__ method (for description");
    }
    
    Py_XINCREF(_py_class);
    PyObject* result = PyObject_Str(_py_class);
    const char *str_result = PyString_AsString(result);
    Py_XDECREF(_py_class);
    return std::string("User-defined recombiner based on python recombiner ")+std::string(str_result);
  }
  
  /// recombine pa and pb and put result into pab
  virtual void recombine(const PseudoJet & pa, const PseudoJet & pb, 
                         PseudoJet & pab) const{
    // first make a copy of the arguments as PyObject* managed by swig
    PseudoJet pa_copy = pa;  // not sure this is needed
    PyObject *py_pa = 0;
    py_pa = SWIG_NewPointerObj((new fastjet::PseudoJet(static_cast< const fastjet::PseudoJet& >(pa_copy))), SWIGTYPE_p_fastjet__PseudoJet, SWIG_POINTER_OWN |  0 );

    PseudoJet pb_copy = pb;  // not sure this is needed
    PyObject *py_pb = 0;
    py_pb = SWIG_NewPointerObj((new fastjet::PseudoJet(static_cast< const fastjet::PseudoJet& >(pb_copy))), SWIGTYPE_p_fastjet__PseudoJet, SWIG_POINTER_OWN |  0 );

    //PseudoJet pab_copy = pa;  // not sure this is needed
    //PyObject *py_pab = 0;
    //py_pab = SWIG_NewPointerObj((new fastjet::PseudoJet(static_cast< fastjet::PseudoJet& >(pab_copy))), SWIGTYPE_p_fastjet__PseudoJet, SWIG_POINTER_OWN |  0 );

    Py_XINCREF(_py_class);
    PyObject *py_result = PyObject_CallMethod(_py_class, (char *) "recombine",
                                              (char *) "(OO)", py_pa, py_pb);
    Py_XDECREF(_py_class);

    if (py_result == NULL)
      throw Error("RecombinerPython::recombine(): call to python function returned a NULL result.");

    void *pab_void_ptr = 0;
    PseudoJet *pab_ptr = 0;
    int res1 = SWIG_ConvertPtr(py_result, &pab_void_ptr, SWIGTYPE_p_fastjet__PseudoJet, 0 );
    if (!SWIG_IsOK(res1)) {
      throw Error("RecombinerPython::recombine(): cannot reinterpret the last argument as a fastjet::PseudoJet.");
    }
    pab_ptr = reinterpret_cast< fastjet::PseudoJet * >(pab_void_ptr);
    pab = *pab_ptr;
    Py_XDECREF(py_result);  ///needed???
  }

  /// routine called to preprocess each input jet (to make all input
  /// jets compatible with the scheme requirements (e.g. massless).
  virtual void preprocess(PseudoJet & pa) const {
      // first make a copy of the arguments as PyObject* managed by swig
    PseudoJet pa_copy = pa;  // not sure this is needed
    PyObject *py_pa = 0;
    py_pa = SWIG_NewPointerObj((new fastjet::PseudoJet(static_cast< fastjet::PseudoJet& >(pa_copy))), SWIGTYPE_p_fastjet__PseudoJet, SWIG_POINTER_OWN |  0 );

    Py_XINCREF(_py_class);
    PyObject *py_result = PyObject_CallMethod(_py_class, (char *) "preprocess",
                                              (char *) "(O)", py_pa);
    Py_XDECREF(_py_class);

    if (py_result == NULL)
      throw Error("RecombinerPython::preprocess(): call to python function returned a NULL result.");

    void *pa_void_ptr = 0;
    PseudoJet *pa_ptr = 0;
    int res1 = SWIG_ConvertPtr(py_pa, &pa_void_ptr, SWIGTYPE_p_fastjet__PseudoJet, 0);
    if (!SWIG_IsOK(res1)) {
      throw Error("RecombinerPython::preprocess(): cannot reinterpret the last argument as a fastjet::PseudoJet.");
    }
    pa_ptr = reinterpret_cast< fastjet::PseudoJet * >(pa_void_ptr);
    pa = *pa_ptr;
    Py_XDECREF(py_result);  ///needed???
  }

private:
   PyObject *_py_class;  
};



FASTJET_END_NAMESPACE      // defined in fastjet/internal/base.hh

#endif // __FASTJET_PYTHONUSERINFO_HH__

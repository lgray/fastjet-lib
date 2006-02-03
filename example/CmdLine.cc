#include "CmdLine.hh"
#include<string>
#include<sstream>
#include<iostream> // testing
#include<vector>
#include<cstddef> // for size_t
using namespace std;

// initialise the various structures that we shall
// use to access the command-line options;
//
// If an option appears several times, it is its LAST value
// that will be used in searching for option values (opposite of f90)
CmdLine::CmdLine (const int argc, char** argv) {
  __arguments.resize(argc);
  for(int iarg = 0; iarg < argc; iarg++){
    __arguments[iarg] = argv[iarg];
  }
  this->init();
}

/// constructor from a vector of strings, one argument per string
CmdLine::CmdLine (const vector<string> & args) {
  __arguments = args;
  this->init();
}

//----------------------------------------------------------------------
void CmdLine::init (){
  __command_line = "";
  for(size_t iarg = 0; iarg < __arguments.size(); iarg++){
    __command_line += __arguments[iarg];
    __command_line += " ";
  }
  
  // group things into options
  bool next_may_be_val = false;
  string currentopt;
  for(size_t iarg = 1; iarg < __arguments.size(); iarg++){
    // if expecting an option value, then take it (even if
    // it is actually next option...)
    if (next_may_be_val) {__options[currentopt] = iarg;}
    // now see if it might be an option itself
    string arg = __arguments[iarg];
    bool thisisopt = (arg.compare(0,1,"-") == 0);
    if (thisisopt) {
      // set option to a standard undefined value and say that 
      // we expect (possibly) a value on next round
      currentopt = arg;
      __options[currentopt] = -1;
      next_may_be_val = true;}
    else {
      // otherwise throw away the argument for now...
      next_may_be_val = false;
      currentopt = "";
    }
  }
}

// indicates whether an option is present
bool CmdLine::present(const string & opt) {
  return (__options.find(opt) != __options.end());
}

// indicates whether an option is present and has a value associated
bool CmdLine::present_and_set(const string & opt) {
  return (__options.find(opt) != __options.end() && 
	  __options[opt] > 0);
}


// return the string value corresponding to the specified option
string CmdLine::string_val(const string & opt) {
  if (!this->present_and_set(opt)) {
    cerr << "Error: Option "<<opt
	 <<" is needed but is not present_and_set"<<endl;
    exit(-1);
  }
  return __arguments[__options[opt]];
}

// as above, but if opt is not present_and_set, return default
string CmdLine::string_val(const string & opt, const string & defval) {
  if (this->present_and_set(opt)) {return string_val(opt);} 
  else {return defval;}
}

// Return the integer value corresponding to the specified option;
// Not too sure what happens if option is present_and_set but does not
// have string value...
int CmdLine::int_val(const string & opt) {
  int result;
  string optstring = string_val(opt);
  istringstream optstream(optstring);
  optstream >> result;
  if (optstream.fail()) {
    cerr << "Error: could not convert option ("<<opt<<") value ("
	 <<optstring<<") to int"<<endl; 
    exit(-1);}
  return result;
}

// as above, but if opt is not present_and_set, return default
int CmdLine::int_val(const string & opt, const int & defval) {
  if (this->present_and_set(opt)) {return int_val(opt);} 
  else {return defval;}
}


// Return the integer value corresponding to the specified option;
// Not too sure what happens if option is present_and_set but does not
// have string value...
double CmdLine::double_val(const string & opt) {
  double result;
  string optstring = string_val(opt);
  istringstream optstream(optstring);
  optstream >> result;
  if (optstream.fail()) {
    cerr << "Error: could not convert option ("<<opt<<") value ("
	 <<optstring<<") to double"<<endl; 
    exit(-1);}
  return result;
}

// as above, but if opt is not present_and_set, return default
double CmdLine::double_val(const string & opt, const double & defval) {
  if (this->present_and_set(opt)) {return double_val(opt);} 
  else {return defval;}
}


// return the full command line including the command itself
string CmdLine::command_line() {
  return __command_line;
}

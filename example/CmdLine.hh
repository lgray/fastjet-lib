#ifndef __CMDLINE__
#define __CMDLINE__

#include<string>
#include<map>
#include<vector>
using namespace std;

/// Class designed to deal with command-line arguments in a fashion similar
/// to what was done in f90 iolib.
///
/// Note that functionality might be slightly different? 
/// Currently do not implement access to arguments by index
/// though data structure would in principle allow this quite easily.
///
/// GPS 03/01/05
/// [NB: wonder if some of this might be more efficiently written 
/// with templates for different type that can be read in...]
///
/// Other question: dealing with list of options is rather common
/// occurrence -- command-line arguments, but also card files; maybe one
/// could somehow use base/derived classes to share common functionality? 
///
class CmdLine {
  map<string,int> __options;
  vector<string> __arguments;
  //string __progname;
  string __command_line;

 public :
  CmdLine() {};
  /// initialise a CmdLine from a C-style array of command-line arguments
  CmdLine(const int argc, char** argv);
  /// initialise a CmdLine from a C++ vector of arguments 
  CmdLine(const vector<string> & args);

  /// true if the option is present
  bool    present(const string & opt);
  /// true if the option is present and corresponds to a value
  bool    present_and_set(const string & opt);

  /// return the integer value corresponding to the given option
  int     int_val(const string & opt);
  /// return the integer value corresponding to the given option or default if option is absent
  int     int_val(const string & opt, const int & defval);

  /// return the double value corresponding to the given option
  double  double_val(const string & opt);
  /// return the double value corresponding to the given option or default if option is absent
  double  double_val(const string & opt, const double & defval);

  /// return the string value corresponding to the given option
  string  string_val(const string & opt);
  /// return the string value corresponding to the given option or default if option is absent
  string  string_val(const string & opt, const string & defval);

  /// return the full command line
  string  command_line();

 private:
  /// builds the internal structures needed to keep track of arguments and options
  void init();
};

#endif

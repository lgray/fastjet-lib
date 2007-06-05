// 
// Tool intended to get a bunch of histograms from the output of
// subtraction-correlation.cc
//
#include<iostream>
#include<string>
#include<vector>
#include<sstream>
#include"SimpleHist.hh"

using namespace std;

double pow2(double x) {return x*x;}

int main () {
  cout << "# Running subtraction-correl-pull" << endl;
  // the histograms we'll use -- first not making use of errors
  SimpleHist full_hard_diff(-100,100,100);
  SimpleHist full_hard_sub_diff = full_hard_diff;
  SimpleHist full_hard_sub_err  = full_hard_diff;
  // then one to examine "pull" (differences normalised to errors)
  SimpleHist full_hard_sub_diff_norm(-20,20,400);
  

  // read files and input the histograms
  string line;
  vector<double> vals(17);
  while (getline(cin,line)) {
    if (line.substr(0,1) == "#") {
      // preserve comment lines
      cout << line << endl;
    } else {
      // get values form line ([0] skipped to we match gnuplot col numbers)
      istringstream lstr(line);
      for(int i = 1; i <= 16; i++) lstr >> vals[i];
      // and do something with them
      double err = sqrt(pow2(vals[3]) + pow2(vals[9]));
      full_hard_diff.add_entry(vals[7]-vals[1]);
      full_hard_sub_diff.add_entry(vals[8]-vals[2]);
      full_hard_sub_err.add_entry(err);
      full_hard_sub_diff_norm.add_entry((vals[8]-vals[2])/err);
    }
  }

  // output the results
  output(full_hard_diff, full_hard_sub_diff, full_hard_sub_err);
  cout << endl << endl;
  output(full_hard_sub_diff_norm);

}

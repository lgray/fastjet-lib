//======================================================================
// GPS June 2007
//
// routine for testing best selection in boosted jet studies
//
//
#include<iostream>
#include<sstream>
#include<fstream>
#include<string>
#include "CmdLine.hh"
#include "fastjet/ClusterSequence.hh"

using namespace std;

fastjet::ClusterSequence cs;

struct MRtD {
  double m, rtd;
};

//----------------------------------------------------------------------
double pass_frac(const vector<MRtD> & ev, 
                 double m_lo, double m_hi, double rtd_lo) {
  int npass = 0;
  for (unsigned i = 0; i < ev.size(); i++) {
    if (ev[i].m >= m_lo && ev[i].m <= m_hi && ev[i].rtd > rtd_lo) npass++;
  }
  return (1.0*npass)/ev.size();
}

void read(ifstream & ifstr, vector<MRtD> & evs, bool use_parents) {

  string line;
  double res[4];
  while (getline(ifstr, line)) {
    if (line.substr(0,1) == "#") continue;
    istringstream istr(line);
    MRtD mrtd;
    istr >> res[0] >> res[1] >> res[2] >> res[3];
    if (use_parents) {
      mrtd.m = res[2];
      mrtd.rtd = res[3];
    } else {
      mrtd.m = res[0];
      mrtd.rtd = res[1];
    }
    evs.push_back(mrtd);
  }
}

//----------------------------------------------------------------------
int main (int argc, char ** argv) {
  CmdLine cmdline(argc,argv);
  
  double m_min = 60.0;
  double m_max = 110.1;
  double rtd_max = cmdline.value("-rtd_max",100.1);

  vector<MRtD> signal, bkgd;
  ifstream sig_str(cmdline.value<string>("-sig").c_str());
  ifstream bkgd_str(cmdline.value<string>("-bkgd").c_str());
  bool use_parents = cmdline.present("-parents");

  if (!cmdline.all_options_used()) {cerr << 
      "Error: some options unsupported"<<endl; 
    exit(-1);}

  read(sig_str, signal, use_parents);
  read(bkgd_str, bkgd, use_parents);

  cout << "# " << cmdline.command_line() << endl;
  cout << "# nev:" << signal.size() << " " << bkgd.size() << endl;
  cout << "# Format: eff, bkgd_eff, m_lo, m_hi, rtd_lo" << endl;
  for (double m_lo = m_min; m_lo < m_max; m_lo+=2.0) {
    for (double m_hi = m_lo+2.0; m_hi < m_max; m_hi+=2.0) {
      for (double rtd_lo = 0.0; rtd_lo < rtd_max; rtd_lo += 10.0) {
        double eff = pass_frac(signal, m_lo, m_hi, rtd_lo);
        double bkgd_eff = pass_frac(bkgd, m_lo, m_hi, rtd_lo);
        cout << eff << " " << bkgd_eff 
             << " " << m_lo << " " << m_hi << " " << rtd_lo << endl;
      }
    }
  }
  
}

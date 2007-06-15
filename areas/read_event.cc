#include "read_event.hh"
#include<sstream>

using namespace std;
namespace fj = fastjet;

inline double pow2(const double x) {return x*x;};



//======================================================================
// common subroutine for reading events, both in pythia format and in
// hydjet format
//
void read_event_common(
                istream & input, double etamax, bool hydjet, bool massless,
		vector<fj::PseudoJet> & hard_event, 
		vector<fj::PseudoJet> & full_event) {
  string line;
  int  nsub  = 0;
  while (getline(input, line)) {
      //cout << line<<endl;
    istringstream linestream(line);
    // take substrings to avoid problems when there are extra "pollution"
    // characters (e.g. line-feed).
    if (line.substr(0,4) == "#END") {break;}
    if (line.substr(0,9) == "#SUBSTART") {
      // if more sub events follow, make copy of hard one here
      if (nsub == 1) hard_event = full_event;
      nsub += 1;
    }
    if (line.substr(0,1) == "#") {continue;}
    valarray<double> fourvec(4);
    if (hydjet) {
      // special reading from hydjet.txt event record (though actually
      // this is supposed to be a standard pythia event record, so
      // being able to read from it is perhaps not so bad an idea...)
      int ii, istat,id,m1,m2,d1,d2;
      double mass;
      linestream >> ii>> istat >> id >> m1 >> m2 >> d1 >> d2
		 >> fourvec[0] >> fourvec[1] >> fourvec[2] >> mass;
      // current file contains mass of particle as 4th entry
      if (istat == 1) {
	fourvec[3] = sqrt(+pow2(fourvec[0])+pow2(fourvec[1])
			  +pow2(fourvec[2])+pow2(mass));
      }
    } else {
      if (massless) {
	linestream >> fourvec[0] >> fourvec[1] >> fourvec[2];
	fourvec[3] = sqrt(pow2(fourvec[0])+pow2(fourvec[1])+pow2(fourvec[2]));}
      else {
	linestream >> fourvec[0] >> fourvec[1] >> fourvec[2] >> fourvec[3];
      }
    }
    fj::PseudoJet psjet(fourvec);
    psjet.set_user_index(0);
    if (abs(psjet.rap() < etamax)) {full_event.push_back(psjet);}

  }

  // if we have read in only one event, copy it across here...
  if (nsub == 1) hard_event = full_event;

  // if there was nothing in the event 
  if (nsub == 0) {
    cerr << "Error: read empty event\n";
    exit(-1);
  }
}

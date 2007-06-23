#ifndef __FASTJET_RANGEDEFINITION_HH__
#define __FASTJET_RANGEDEFINITION_HH__

#include "fastjet/PseudoJet.hh"
#include<sstream>
#include<string>

FASTJET_BEGIN_NAMESPACE      // defined in fastjet/internal/base.hh

//----------------------------------------------------------------------
//
/// class for holding a range definition specification, given by limits
/// on rapidity and azimuth.
///
class RangeDefinition {
public:
  /// default constructor
  RangeDefinition() {}

  /// constructor for a range definition given by |y|<rapmax
  RangeDefinition(double rapmax) {
                     assert ( rapmax > 0.0 );
                     _rapmax = rapmax;
		     _rapmin = -rapmax;
		     _phimin = 0.0;
		     _phimax = twopi; }
  
  /// destructor does nothing
  virtual ~RangeDefinition() {}
     
  /// constructor for a range definition given by 
  /// rapmin <= y <= rapmax, phimin <= phi <= phimax
  RangeDefinition(double rapmin, double rapmax, 
                  double phimin = 0.0, double phimax = twopi) {
                     assert ( rapmin < rapmax);
                     assert ( phimin < phimax);
                     assert ( phimin >= 0.0 );
                     _rapmax = rapmax;
		     _rapmin = rapmin;
		     _phimin = phimin;
		     _phimax = phimax; }


  /// return bool according to whether the jet is within the given range
  virtual inline bool is_in_range(const PseudoJet & jet) const {
    double rap = jet.rap();
    double phi = jet.phi();
    return ( rap >= _rapmin && 
             rap <= _rapmax &&
             phi >= _phimin &&
             phi <= _phimax);
  }
  
  /// area of the range region
  virtual inline double area() const {
    return (_rapmax - _rapmin)*(_phimax - _phimin);
  }
  
  /// textual description of range
  virtual inline std::string description() const {
    std::ostringstream ostr;
    ostr << "Range: " << _rapmin << " <= y <= "   << _rapmax << ", "
                      << _phimin << " <= phi <= " << _phimax ;
    return ostr.str();
}
  
private:
  double _rapmin,_rapmax,_phimin,_phimax;


};

FASTJET_END_NAMESPACE        // defined in fastjet/internal/base.hh


#endif // __FASTJET_RANGEDEFINITION_HH__

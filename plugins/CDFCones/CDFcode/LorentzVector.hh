#ifndef _LORENTZ_VECTOR_HH_
#define _LORENTZ_VECTOR_HH_

#include <cmath>

class LorentzVector
{
private:
  double _px,_py,_pz,_E;
  mutable double phi_store, rap_store ;
  const static double bad_val = -1e100;

 public:

  LorentzVector(): _px(0), _py(0), _pz(0), _E(0), phi_store(bad_val), rap_store(bad_val) {}
  LorentzVector(double p1, double p2, double p3, double p0): _px(p1), _py(p2), _pz(p3), _E(p0), phi_store(bad_val), rap_store(bad_val)  {}
  LorentzVector(const LorentzVector& p): _px(p._px), _py(p._py), _pz(p._pz), _E(p._E), phi_store(bad_val), rap_store(bad_val)  {}

  double px()   const {return _px;}
  double py()   const {return _py;}
  double pz()   const {return _pz;}
  double E ()   const {return _E;}

  double p()   const {return sqrt(_px*_px + _py*_py + _pz*_pz);}
  double pt()  const {return sqrt(_px*_px + _py*_py);}
  double mt()  const {return sqrt((_E-_pz)*(_E+_pz));}
  //double y()   const {return 0.5*log((_E + _pz)/(_E - _pz));}
  double Et()  const {return _E/p()*pt();}
  double eta() const {return 0.5*log((p() + _pz)/(p() - _pz));}
  double phi() const
  {
    if (phi_store == bad_val) calc_phi_rap();
    return phi_store;
    // double r = atan2(_py,_px);
    // if(r < 0)
    //   r += 2*M_PI;
    // return r;
  }
  double y()   const {
    if (phi_store == bad_val) calc_phi_rap();
    return rap_store;
  }
  void add(LorentzVector v)
  {
    _px += v._px;
    _py += v._py;
    _pz += v._pz;
    _E  += v._E;
    phi_store = bad_val;
  }
  void subtract(LorentzVector v)
  {
    _px -= v._px;
    _py -= v._py;
    _pz -= v._pz;
    _E  -= v._E;
    phi_store = bad_val;
  }
  bool isEqual(LorentzVector v)
  {
    return _px == v._px && _py == v._py && _pz == v._pz && _E == v._E;
  }

  void calc_phi_rap() const {
    phi_store = atan2(_py,_px);
    if(phi_store < 0) phi_store += 2*M_PI;
    rap_store = 0.5*log((_E + _pz)/(_E - _pz));
  }
};

#endif

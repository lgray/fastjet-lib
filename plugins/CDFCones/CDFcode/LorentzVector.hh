#ifndef _LORENTZ_VECTOR_HH_
#define _LORENTZ_VECTOR_HH_

#include <cmath>

class LorentzVector
{
 public:

  double px,py,pz,E;
  mutable double phi_store, rap_store ;
  const static double bad_phi = -10.0;

  LorentzVector(): px(0), py(0), pz(0), E(0), phi_store(bad_phi) {}
  LorentzVector(double p1, double p2, double p3, double p0): px(p1), py(p2), pz(p3), E(p0), phi_store(bad_phi)  {}
  LorentzVector(const LorentzVector& p): px(p.px), py(p.py), pz(p.pz), E(p.E), phi_store(bad_phi)  {}
  double p()   const {return sqrt(px*px + py*py + pz*pz);}
  double pt()  const {return sqrt(px*px + py*py);}
  double mt()  const {return sqrt((E-pz)*(E+pz));}
  //double y()   const {return 0.5*log((E + pz)/(E - pz));}
  double Et()  const {return E/p()*pt();}
  double eta() const {return 0.5*log((p() + pz)/(p() - pz));}
  double phi() const
  {
    if (phi_store == bad_phi) calc_phi_rap();
    return phi_store;
    // double r = atan2(py,px);
    // if(r < 0)
    //   r += 2*M_PI;
    // return r;
  }
  double y()   const {
    if (phi_store == bad_phi) calc_phi_rap();
    return rap_store;
  }
  void add(LorentzVector v)
  {
    px += v.px;
    py += v.py;
    pz += v.pz;
    E  += v.E;
    phi_store = bad_phi;
  }
  void subtract(LorentzVector v)
  {
    px -= v.px;
    py -= v.py;
    pz -= v.pz;
    E  -= v.E;
    phi_store = bad_phi;
  }
  bool isEqual(LorentzVector v)
  {
    return px == v.px && py == v.py && pz == v.pz && E == v.E;
  }

  void calc_phi_rap() const {
    phi_store = atan2(py,px);
    if(phi_store < 0) phi_store += 2*M_PI;
    rap_store = 0.5*log((E + pz)/(E - pz));
  }
};

#endif

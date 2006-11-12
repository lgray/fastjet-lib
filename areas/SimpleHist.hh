#include<valarray>
#include<string>
#include<cmath>

class SimpleHist {
public:
  SimpleHist() {};
  SimpleHist(double minv, double maxv, unsigned int n): 
    _minv(minv), _maxv(maxv), _dv((maxv-minv)/n), _weights(n+1) {
    _weights = 0.0;
  };

  // declare (or redeclare) the histogram
  void declare(double minv, double maxv, unsigned int n) {
    _minv = minv; _maxv = maxv; _dv = (maxv-minv)/n; 
    _weights.resize(n+1);
    _weights = 0.0;
  }

  double min() const {return _minv;};
  double max() const {return _maxv;};
  /// returns the size of the histogram proper
  unsigned int size() const {return _weights.size()-1;};
  /// returns the size of the histogram plus outflow bin
  unsigned int outflow_size() const {return _weights.size();};

  double & operator[](int i) {return _weights[i];};
  const double & operator[](int i) const {return _weights[i];};
  
  /// returns the outflow bin
  double & outflow() {return _weights[size()];};
  const double & outflow() const {return _weights[size()];};

  double binlo (int i) const {return i*_dv + _minv;};
  double binhi (int i) const {return (i+1)*_dv + _minv;};
  double binmid(int i) const {return (i+0.5)*_dv + _minv;};
  double binsize()     const {return _dv;};

  unsigned int bin(double v) const {
    if (v >= _minv && v < _maxv) {
      int i = int((v-_minv)/_dv); 
      if (i >= 0 && i < int(size())) {return unsigned(i);} 
    }
    // otherwise...
    return size();
  }

  /// return the total weight in the histogram (inefficient)...
  double total_weight() const {
    double tot = 0.0;
    for (unsigned i = 0; i < _weights.size(); i++) tot += _weights[i];
    return tot;
  }

  void add_entry(double v, double weight = 1.0) {
    //if (v >= _minv && v < _maxv) {
    //  int i = int((v-_minv)/_dv); 
    //  if (i >= 0 && i < int(_weights.size())) _weights[i] += weight;
    //}
    _weights[bin(v)] += weight;
  };

  // Operations with constants ---------------------------------------
  SimpleHist & operator*=(double fact) {
    for (unsigned i = 0; i < outflow_size(); i++) (*this)[i] *= fact;
    return *this;
  };
  SimpleHist & operator/=(double fact) {
    *this *= 1.0/fact;
    return *this;
  };

  // Operations with another histogram -------------------------------
  SimpleHist & operator*=(const SimpleHist & other) {
    assert(other.outflow_size() == outflow_size());
    for (unsigned i = 0; i < outflow_size(); i++) (*this)[i] *= other[i];
    return *this;
  };

  SimpleHist & operator/=(const SimpleHist & other) {
    assert(other.outflow_size() == outflow_size());
    for (unsigned i = 0; i < outflow_size(); i++) (*this)[i] /= other[i];
    return *this;
  };

  SimpleHist & operator+=(const SimpleHist & other) {
    assert(other.outflow_size() == outflow_size());
    for (unsigned i = 0; i < outflow_size(); i++) (*this)[i] += other[i];
    return *this;
  };

  SimpleHist & operator-=(const SimpleHist & other) {
    assert(other.outflow_size() == outflow_size());
    for (unsigned i = 0; i < outflow_size(); i++) (*this)[i] -= other[i];
    return *this;
  };




private:
  double _minv, _maxv, _dv, _total_weight;
  std::valarray<double> _weights;
  std::string _name;
};



// Binary operations with constants -----------------------------
SimpleHist operator*(const SimpleHist & hist, double fact) {
  SimpleHist result(hist.min(), hist.max(), hist.outflow_size());
  for (unsigned i = 0; i < hist.outflow_size(); i++) result[i] = hist[i] * fact;
  return result;
}

SimpleHist operator/(const SimpleHist & hist, double fact) {
  SimpleHist result(hist.min(), hist.max(), hist.outflow_size());
  for (unsigned i = 0; i < hist.outflow_size(); i++) result[i] = hist[i] / fact;
  return result;
}

inline SimpleHist operator*(double fact, const SimpleHist & hist) {
  return hist*fact;
}

inline SimpleHist operator/(double fact, const SimpleHist & hist) {
  return hist/fact;
}


// Binary operations with other histograms ------------------------
SimpleHist operator*(const SimpleHist & hista, const SimpleHist & histb) {
  assert(hista.outflow_size() == histb.outflow_size());
  SimpleHist result(hista.min(), hista.max(), hista.outflow_size());
  for (unsigned i = 0; i < hista.outflow_size(); i++) result[i] = hista[i] * histb[i];
  return result;
}
SimpleHist operator/(const SimpleHist & hista, const SimpleHist & histb) {
  assert(hista.outflow_size() == histb.outflow_size());
  SimpleHist result(hista.min(), hista.max(), hista.outflow_size());
  for (unsigned i = 0; i < hista.outflow_size(); i++) result[i] = hista[i] / histb[i];
  return result;
}
SimpleHist operator+(const SimpleHist & hista, const SimpleHist & histb) {
  assert(hista.outflow_size() == histb.outflow_size());
  SimpleHist result(hista.min(), hista.max(), hista.outflow_size());
  for (unsigned i = 0; i < hista.outflow_size(); i++) result[i] = hista[i] + histb[i];
  return result;
}
SimpleHist operator-(const SimpleHist & hista, const SimpleHist & histb) {
  assert(hista.outflow_size() == histb.outflow_size());
  SimpleHist result(hista.min(), hista.max(), hista.outflow_size());
  for (unsigned i = 0; i < hista.outflow_size(); i++) result[i] = hista[i] - histb[i];
  return result;
}


// Unary mathematical functions
SimpleHist sqrt(const SimpleHist & hist) {
  SimpleHist result(hist.min(), hist.max(), hist.outflow_size());
  for (unsigned i = 0; i < hist.outflow_size(); i++) result[i] = sqrt(hist[i]);
  return result;
}

// Unary mathematical functions
SimpleHist pow2(const SimpleHist & hist) {
  SimpleHist result(hist.min(), hist.max(), hist.outflow_size());
  for (unsigned i = 0; i < hist.outflow_size(); i++) result[i] = hist[i]*hist[i];
  return result;
}


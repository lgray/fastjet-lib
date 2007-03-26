#include<valarray>
#include<string>
#include<cmath>

class SimpleHist {
public:
  SimpleHist() {};
  SimpleHist(double minv, double maxv, unsigned int n): 
    _minv(minv), _maxv(maxv), _dv((maxv-minv)/n), _weights(n+1),
    _weights2(n+1), _entries(n+1){
    _weights = 0.0;
    _entries = 0;
    _have_total = false;
    _have_total_entries = false;
  };

  // declare (or redeclare) the histogram
  void declare(double minv, double maxv, unsigned int n) {
    _minv = minv; _maxv = maxv; _dv = (maxv-minv)/n; 
    _weights.resize(n+1);
    _weights = 0.0;
    _weights2.resize(n+1);
    _weights2 = 0.0;
    _entries.resize(n+1);
    _entries = 0;
    _have_total = false;
    _have_total_entries = false;
  }

  double min() const {return _minv;};
  double max() const {return _maxv;};
  /// returns the size of the histogram proper
  unsigned int size() const {return _weights.size()-1;};
  /// returns the size of the histogram plus outflow bin
  unsigned int outflow_size() const {return _weights.size();};

  double & operator[](int i) {_have_total = false; return _weights[i];};
  const double & operator[](int i) const {return _weights[i];};
  
  /// returns the outflow bin
  double & outflow() {return _weights[size()];};
  const double & outflow() const {return _weights[size()];};

  /// returns the number of entries in given bin
  int & entries_in_bin(int i) {_have_total_entries = false; return _entries[i];};
  const int & entries_in_bin(int i) const {return _entries[i];};
  
  /// returns the number of entries in the outflow bin
  int & outflow_entries() {return _entries[size()];};
  const int & outflow_entries() const {return _entries[size()];};

  double binlo (int i) const {return i*_dv + _minv;};
  double binhi (int i) const {return (i+1)*_dv + _minv;};
  double binmid(int i) const {return (i+0.5)*_dv + _minv;};
  double binsize()     const {return _dv;};

  /// return average in each bin = _weights[i]/_entries[i]
  const double average (int i) const {
        return (_entries[i] > 0 ) ? _weights[i]/float(_entries[i]) : 0.; };

  /// return error on average in each bin 
  const double error (int i) const {
      return (_entries[i] > 1 ) ?
             sqrt((_weights2[i]/float(_entries[i]) -
             _weights[i]*_weights[i]/float(_entries[i])/float(_entries[i]))
             /float(_entries[i]-1))
	     : 0. ; };
  
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
    if (!_have_total) {
      _total_weight = 0.0;
      for (unsigned i = 0; i < _weights.size(); i++) {
        _total_weight += _weights[i];}
      _have_total = true;
    }
    return _total_weight;
  }

  /// return the total number of entries in the histogram (inefficient)...
  int total_entries() const {
    if (!_have_total_entries) {
      _total_entries = 0;
      for (unsigned i = 0; i < _entries.size(); i++) {
        _total_entries += _entries[i];}
      _have_total_entries = true;
    }
    return _total_entries;
  }

  void add_entry(double v, double weight = 1.0) {
    //if (v >= _minv && v < _maxv) {
    //  int i = int((v-_minv)/_dv); 
    //  if (i >= 0 && i < int(_weights.size())) _weights[i] += weight;
    //}
    _have_total = false;
    _weights[bin(v)] += weight;
    _weights2[bin(v)] += weight*weight;
    _have_total_entries = false;
    _entries[bin(v)]++;
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
  double _minv, _maxv, _dv;
  std::valarray<double> _weights;
  std::valarray<double> _weights2;
  std::valarray<int> _entries;
  std::string _name;
  mutable double _total_weight;
  mutable double _total_weight2;
  mutable int    _total_entries;
  mutable bool   _have_total;
  mutable bool   _have_total_entries;
};



// Binary operations with constants -----------------------------
inline SimpleHist operator*(const SimpleHist & hist, double fact) {
  SimpleHist result(hist.min(), hist.max(), hist.outflow_size());
  for (unsigned i = 0; i < hist.outflow_size(); i++) result[i] = hist[i] * fact;
  return result;
}

inline SimpleHist operator/(const SimpleHist & hist, double fact) {
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
inline SimpleHist operator*(const SimpleHist & hista, const SimpleHist & histb) {
  assert(hista.outflow_size() == histb.outflow_size());
  SimpleHist result(hista.min(), hista.max(), hista.size());
  for (unsigned i = 0; i < hista.outflow_size(); i++) result[i] = hista[i] * histb[i];
  return result;
}
inline SimpleHist operator/(const SimpleHist & hista, const SimpleHist & histb) {
  assert(hista.outflow_size() == histb.outflow_size());
  SimpleHist result(hista.min(), hista.max(), hista.size());
  for (unsigned i = 0; i < hista.outflow_size(); i++) result[i] = hista[i] / histb[i];
  return result;
}
inline SimpleHist operator+(const SimpleHist & hista, const SimpleHist & histb) {
  assert(hista.outflow_size() == histb.outflow_size());
  SimpleHist result(hista.min(), hista.max(), hista.size());
  for (unsigned i = 0; i < hista.outflow_size(); i++) result[i] = hista[i] + histb[i];
  return result;
}
inline SimpleHist operator-(const SimpleHist & hista, const SimpleHist & histb) {
  assert(hista.outflow_size() == histb.outflow_size());
  SimpleHist result(hista.min(), hista.max(), hista.size());
  for (unsigned i = 0; i < hista.outflow_size(); i++) result[i] = hista[i] - histb[i];
  return result;
}


// Unary mathematical functions
inline SimpleHist sqrt(const SimpleHist & hist) {
  SimpleHist result(hist.min(), hist.max(), hist.size());
  for (unsigned i = 0; i < hist.outflow_size(); i++) result[i] = sqrt(hist[i]);
  return result;
}

// Unary mathematical functions
inline SimpleHist pow2(const SimpleHist & hist) {
  SimpleHist result(hist.min(), hist.max(), hist.size());
  for (unsigned i = 0; i < hist.outflow_size(); i++) result[i] = hist[i]*hist[i];
  return result;
}


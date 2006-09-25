#include<valarray>
#include<string>

class SimpleHist {
public:
  SimpleHist(double minv, double maxv, int n): 
    _minv(minv), _maxv(maxv), _dv((maxv-minv)/n), _weights(n) {};

  double & operator[](int i) {return _weights[i];};
  const double & operator[](int i) const {return _weights[i];};

  double binlo (int i) const {return i*_dv + _minv;};
  double binhi (int i) const {return (i+1)*_dv + _minv;};
  double binmid(int i) const {return (i+0.5)*_dv + _minv;};
  double binsize()     const {return _dv;};

  void add_entry(double v, double weight = 1.0) {
    if (v >= _minv && v < _maxv) {
      int i = int((v-_minv)/_dv); 
      if (i >= 0 && i < int(_weights.size())) _weights[i] += weight;
    }
  };

  unsigned int size() const {return _weights.size();};

private:
  double _minv, _maxv, _dv;
  std::valarray<double> _weights;
  std::string _name;
};



/// a very dirty histogramming class

#ifndef __CSHISTO__
#define __CSHISTO__


class CSHisto {
public:
  CSHisto(double min, double max, int nbins);
  void fill(double val);
  double outflow() const {return _outflow;};
  double total_weight() const {return _total_weight;};
  unsigned size() const {return _bins.size();};
  double bin_weight(int n) const {return _bins[n];} ;
  double bin_centre(int n) const {return _min+(n+0.5)*_bin_size;};
private:
  double           _min, _max, _bin_size, _outflow, _total_weight;
  valarray<double> _bins;
};


CSHisto::CSHisto(double min, double max, int nbins) {
  _min = min;
  _max = max;
  _bin_size = (max - min)/nbins;

  _bins.resize(nbins);
  _bins = 0.0;
  _outflow = 0.0;
}


void CSHisto::fill(double val) {
  int posn = int((val-_min) / _bin_size);
  if (posn >= 0 && posn < int(_bins.size())) {
    _bins[posn] += 1;}
  else {_outflow += 1;}
  _total_weight += 1;
}

#endif //__CSHISTO__

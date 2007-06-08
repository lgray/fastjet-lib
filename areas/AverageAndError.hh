#include<cmath>

/// micro class to calculate averages and errors
class AverageAndError {
public:

   /// default constructor
   AverageAndError() { _sum = 0.0; _sum2 = 0.0; _n=0;}
   
   /// add one event
   inline void add(double x)  { _sum += x, 
                                _sum2 += x*x;
			        _n += 1;
			      }
 
   /// return sum
   inline double sum() const { return _sum; }

   /// return sum2
   inline double sum2() const { return _sum2; }
   
   /// return number of events
   inline int n() const { return _n; }
   
   /// calculate and return average
   inline double average() const { return (_n > 0) ? _sum/_n : 0. ; }

   /// calculate and return average of squares
   inline double average2() const { return (_n > 0) ? _sum2/_n : 0. ; }

   /// calculate and return error
   inline double error() const { return (_n > 1) ? std::sqrt((_sum2/_n - _sum*_sum/_n/_n)/(_n-1)) : 0.; }

   /// calculate and return the standard deviation
   inline double sd() const { return (_n > 1) ? std::sqrt(_sum2/_n - _sum*_sum/_n/_n) : 0.; }
   
private:

double _sum, _sum2;
int _n;

};


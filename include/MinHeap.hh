
#ifndef __MINHEAP__HH__
#define __MINHEAP__HH__

#include<vector>
#include<cassert>
#include<memory>
#include<limits>

//======================================================================
/// A class which provides a "heap"-like structure that allows
/// access to a the minimal value of a dynamically changing set of numbers
class MinHeap {
public:
  /// construct a MinHeap from the vector of values, allowing future
  /// expansion to a maximum size max_size;
  MinHeap (const std::vector<double> & values, unsigned int max_size) :
    _heap(max_size) {_initialise(values);};

  /// constructor in which the the maximum size is the size of the values array
  MinHeap (const std::vector<double> & values) :
    _heap(values.size()) {_initialise(values);};
  
  /// return the location of the minimal value on the heap
  inline unsigned int minloc() const {
    return (_heap[0].minloc) - &(_heap[0]);};
  
  /// return the minimal value on the heap
  inline double       minval() const {return _heap[0].minloc->value;};

  inline double operator[](int i) const {return _heap[i].value;};

  /// remove the value at the specified location (i.e. replace it with
  /// the largest possible value).
  void remove(unsigned int loc) {
    update(loc,std::numeric_limits<double>::max());};

  /// update the value at the specified location
  void update(unsigned int, double);

private:

  struct ValueLoc{
    double value;
    ValueLoc * minloc;
  };
      
  std::vector<ValueLoc> _heap;

  void _initialise(const std::vector<double> & values);


};

#endif // __MINHEAP__HH__

#ifndef DROP_CGAL // in case we do not have the code for CGAL
#ifndef __DNN4PICYLINDER__
#define __DNN4PICYLINDER__

#include "DynamicNearestNeighbours.hh"
#include "DnnPlane.hh"
#include "numconsts.hh"
using namespace std;

/// class derived from DynamicNearestNeighbours that provides an
/// implementation for the surface of cylinder (using two copies of
/// DnnPlane, one running from 0--2pi, the other from pi--3pi).
class Dnn4piCylinder : public DynamicNearestNeighbours {
 public:
  /// empty initaliser
  Dnn4piCylinder() {};

  /// Initialiser from a set of points on an Eta-Phi plane, where
  /// eta can have an arbitrary ranges and phi must be in range
  /// 0 <= phi < 2pi
  Dnn4piCylinder(const vector<EtaPhi> &, const bool & verbose = false );

  /// Returns the index of  the nearest neighbour of point labelled
  /// by ii (assumes ii is valid)
  int NearestNeighbourIndex(const int & ii) const ;

  /// Returns the distance to the nearest neighbour of point labelled
  /// by index ii (assumes ii is valid)
  double NearestNeighbourDistance(const int & ii) const ;

  /// Returns true iff the given index corresponds to a point that
  /// exists in the DNN structure (meaning that it has been added, and
  /// not removed in the meantime)
  bool Valid(const int & index) const;

  void RemoveAndAddPoints(const vector<int> & indices_to_remove,
			  const vector<EtaPhi> & points_to_add,
			  vector<int> & indices_added,
			  vector<int> & indices_of_updated_neighbours);

  ~Dnn4piCylinder();

 private:

  bool _verbose;

  // NB: we define POINTERS here because the initialisation gave
  //     us problems (things crashed!), perhaps because in practice
  //     we were making a copy without being careful and defining
  //     a proper copy constructor.
  DnnPlane * _DNN1, * _DNN2;

  /// given a phi value in the 0--2pi range return one 
  /// in the pi--3pi range.
  inline EtaPhi _remap_phi(const EtaPhi & point) {
    double phi = point.second;
    if (phi < pi) { phi += twopi ;}
    return EtaPhi(point.first, phi);};

};


// here follow some inline implementations of the simpler of the
// functions defined above

inline int Dnn4piCylinder::NearestNeighbourIndex(const int & current) const {
  return (_DNN1->NearestNeighbourDistance(current) < 
	  _DNN2->NearestNeighbourDistance(current)) ? 
    _DNN1->NearestNeighbourIndex(current) : 
    _DNN2->NearestNeighbourIndex(current) ; 
};

inline double Dnn4piCylinder::NearestNeighbourDistance(const int & current) const {
  return (_DNN1->NearestNeighbourDistance(current) < 
	  _DNN2->NearestNeighbourDistance(current)) ? 
    _DNN1->NearestNeighbourDistance(current) : 
    _DNN2->NearestNeighbourDistance(current) ; 
};

inline bool Dnn4piCylinder::Valid(const int & index) const {
  return (_DNN1->Valid(index) && _DNN2->Valid(index));
};


inline Dnn4piCylinder::~Dnn4piCylinder() {
  delete _DNN1; 
  delete _DNN2;
}

#endif //  __DNN4PICYLINDER__
#endif //  DROP_CGAL 

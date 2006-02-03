#ifndef __DYNAMICNEARESTNEIGHBOURS__
#define __DYNAMICNEARESTNEIGHBOURS__

#include<vector>
//using namespace std;


/// Shortcut for dealing with eta-phi coordinates.
typedef std::pair<double,double> EtaPhi;

/// type to be used when throwing errors in Dynamic Nearest Neighbours code
struct DNN_Error {
  inline DNN_Error () { ;};
};

///
/// Abstract base class for quick location of nearest neighbours in a set of
/// points, with facilities for adding and removing points from the
/// set after initialisation. Derived classes will be
/// named according to the convention DnnSomeName (e.g. DnnPlane).
///
/// The main purpose of this abstract base class is to define the
/// general interface of a whole set of classes that deal with
/// nearest-neighbour location on different 2-d geometries and with
/// various underlying data structures and algorithms.
///
class DynamicNearestNeighbours {

public:
  /// Dummy initialiser --- does nothing!
  //virtual DynamicNearestNeighbours() {};
   
  /// Initialiser --- sets up the necessary structures to allow efficient
  /// nearest-neighbour finding on the std::vector<EtaPhi> of input points
  //virtual DynamicNearestNeighbours(const std::vector<EtaPhi> &, 
  //				   const bool & verbose = false ) = 0;

  /// Returns the index of the nearest neighbour of point labelled
  /// by ii (assumes ii is valid)
  virtual int NearestNeighbourIndex(const int & ii) const = 0;

  /// Returns the distance to the nearest neighbour of point labelled
  /// by index ii (assumes ii is valid)
  virtual double NearestNeighbourDistance(const int & ii) const = 0;

  /// Returns true iff the given index corresponds to a point that
  /// exists in the DNN structure (meaning that it has been added, and
  /// not removed in the meantime)
  virtual bool Valid(const int & index) const = 0;

  /// remove the points labelled by the std::vector indices_to_remove, and
  /// add the points specified by the std::vector points_to_add
  /// (corresponding indices will be calculated automatically); the
  /// idea behind this routine is that the points to be added will
  /// somehow be close to the one or other of the points being removed
  /// and this can be used by the implementation to provide hints for
  /// inserting the new points in whatever structure it is using.  In a
  /// kt-algorithm the points being added will be a result of a
  /// combination of the points to be removed -- hence the proximity
  /// is (more or less) guaranteed.
  virtual void RemoveAndAddPoints(const std::vector<int> & indices_to_remove,
			  const std::vector<EtaPhi> & points_to_add,
			  std::vector<int> & indices_added,
			  std::vector<int> & indices_of_updated_neighbours) = 0;


  /// Remove the point labelled by index and return the list of
  /// points whose nearest neighbours have changed in the process
  inline void RemovePoint (const int & index,
			   std::vector<int> & indices_of_updated_neighbours) {
    std::vector<int> indices_added;
    std::vector<EtaPhi> points_to_add;
    std::vector<int> indices_to_remove(1);
    indices_to_remove[0] = index;
    RemoveAndAddPoints(indices_to_remove, points_to_add, indices_added,
		       indices_of_updated_neighbours
		       );};


  /// Removes the two points labelled by index1, index2 and adds in the
  /// a point with coordinates newpoint; it returns an index for the new 
  /// point (index 3) and a std::vector of indices of neighbours whose
  /// nearest neighbour has changed (the list includes index3, i.e. the new
  /// point).
  inline void RemoveCombinedAddCombination(
			const int & index1, const int & index2,
			const EtaPhi & newpoint,
			int & index3,
			std::vector<int> & indices_of_updated_neighbours) {
    std::vector<int> indices_added(1);
    std::vector<EtaPhi> points_to_add(1);
    std::vector<int> indices_to_remove(2);
    indices_to_remove[0] = index1;
    indices_to_remove[1] = index2;
    points_to_add[0] = newpoint;
    RemoveAndAddPoints(indices_to_remove, points_to_add, indices_added,
		       indices_of_updated_neighbours
		       );
    index3 = indices_added[0];
  };

  /// destructor -- here it is now implemented
  virtual ~DynamicNearestNeighbours () {}
};
  
#endif // __DYNAMICNEARESTNEIGHBOURS__

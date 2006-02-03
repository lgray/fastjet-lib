#ifndef DROP_CGAL // in case we do not have the code for CGAL
#include <set>
#include "Dnn4piCylinder.hh"
using namespace std;

//----------------------------------------------------------------------
/// initialiser...
Dnn4piCylinder::Dnn4piCylinder(
	const vector<EtaPhi> & input_points, const bool & verbose) {
  
  _verbose = verbose;
  vector<EtaPhi> copied_points(input_points.size());
  for (unsigned int i=0; i < input_points.size(); i++) {
    double phi = input_points[i].second;
    assert(phi >= 0.0 && phi < 2*pi);
    copied_points[i] = _remap_phi(input_points[i]);
  }

  if (_verbose) cout << "============== Preparing _DNN1" << endl;
  _DNN1 = new DnnPlane(input_points, verbose);
  if (_verbose) cout << "============== Preparing _DNN2" << endl;
  _DNN2 = new DnnPlane(copied_points, verbose);
};


//----------------------------------------------------------------------
/// insertion and removal of points
void Dnn4piCylinder::RemoveAndAddPoints(const vector<int> & indices_to_remove,
				const vector<EtaPhi> & points_to_add,
				vector<int> & indices_added,
				vector<int> & indices_of_updated_neighbours) {
  
  vector<int> indices1, indices2;
  
  _DNN1->RemoveAndAddPoints(indices_to_remove,points_to_add,
				      indices_added,indices1);

  // create a vector with the remapped points (pi..3pi)
  vector<EtaPhi> remapped_points(points_to_add.size());
  for (size_t i = 0; i < points_to_add.size(); i++) {
    remapped_points[i] = _remap_phi(points_to_add[i]);
  }
  _DNN2->RemoveAndAddPoints(indices_to_remove, remapped_points, 
				      indices_added,indices2);
  
  // merge the two sequences of updated vertices, avoiding double entries
  // of vertices with the same index
  set<int> index_set;
  unsigned int i;
  for (i=0; i < indices1.size(); i++) {index_set.insert(indices1[i]);}
  for (i=0; i < indices2.size(); i++) {index_set.insert(indices2[i]);}

  indices_of_updated_neighbours.clear();
  for (set<int>::iterator iter = index_set.begin(); 
       iter != index_set.end(); iter++) {
    indices_of_updated_neighbours.push_back(*iter);
  }
}

#endif //  DROP_CGAL 

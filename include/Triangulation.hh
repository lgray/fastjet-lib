#ifndef DROP_CGAL // in case we do not have the code for CGAL
#ifndef __TRIANGULATION__
#define __TRIANGULATION__

// file: examples/Triangulation_2/Voronoi.C
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_hierarchy_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>

/// the basic geometrical kernel that lies at the base of all CGAL
/// operations
struct K : CGAL::Exact_predicates_inexact_constructions_kernel {};

// our extras to help us navigate, find distance, etc.
const int INFINITE_VERTEX=-1;
const int NEW_VERTEX=-2;
const double HUGE_DOUBLE=1e300;

/// A class to provide an "int" with an initial value.
class InitialisedInt {
 private:
  int _val;
 public:
  inline InitialisedInt () {_val=NEW_VERTEX;};
  inline InitialisedInt& operator= (int value) {_val = value; return *this;};
  inline int val() const {return _val;};
};


// We can have triangulations with and without hierarchies -- those with 
// are able to guarantee N ln N time for the construction of a large
// triangulation, whereas those without go as N^{3/2} for points
// sufficiently uniformly distributed in a plane.
//
//#define NOHIERARCHY
#ifdef NOHIERARCHY
typedef CGAL::Triangulation_vertex_base_with_info_2<InitialisedInt,K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Triangulation;
#else
typedef CGAL::Triangulation_vertex_base_with_info_2<InitialisedInt,K> Vbb;
typedef CGAL::Triangulation_hierarchy_vertex_base_2<Vbb> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb,Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K,Tds>  Dt;
typedef CGAL::Triangulation_hierarchy_2<Dt> Triangulation;
#endif

typedef Triangulation::Vertex_handle  Vertex_handle;
typedef Triangulation::Point          Point; /// CGAL Point structure
typedef Triangulation::Vertex_circulator Vertex_circulator;
typedef Triangulation::Face_circulator Face_circulator;
typedef Triangulation::Face_handle Face_handle;


#endif // __TRIANGULATION__
#endif //  DROP_CGAL 

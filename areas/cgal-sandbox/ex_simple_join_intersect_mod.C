//! \file examples/Boolean_set_operations_2/ex_simple_join_intersect.C
// Computing the union and the intersection of two simple polygons.

//#include "bso_rational_nt.h"
//#include <CGAL/Cartesian.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <list>
#include<cassert>

//typedef CGAL::Cartesian<Number_type>               Kernel;
struct Kernel : CGAL::Exact_predicates_inexact_constructions_kernel {};
typedef Kernel::Point_2                            Point_2;
typedef CGAL::Polygon_2<Kernel>                    Polygon_2;
typedef CGAL::Polygon_with_holes_2<Kernel>         Polygon_with_holes_2;
typedef std::list<Polygon_with_holes_2>            Pwh_list_2;

#include "print_utils.h"

using namespace std;

// try our own area...
template<class Kernel, class Container>
double my_area (const CGAL::Polygon_2<Kernel, Container>& P)
{
  typename CGAL::Polygon_2<Kernel, Container>::Vertex_const_iterator  
    v1 = P.vertices_begin(),
    v2 = v1 + 1; 
  double refx = v1->x();
  double refy = v1->y();

  double area = 0.0;
  for (; v2 != P.vertices_end(); v1++, v2++) {
    // take cross product...
    double vx1 = v1->x() - refx; double vy1 = v1->y() - refy;
    double vx2 = v2->x() - refx; double vy2 = v2->y() - refy;
    double darea = 0.5*(vx1*vy2 - vy1*vx2);
    area += darea;
  }
  return area;
}


// attempt at getting areas cleanly...
template<class Kernel, class Container> double pwh_area(
	const CGAL::Polygon_with_holes_2<Kernel, Container>& pwh) {

  // do not really know what to do with an unbounded polygon...
  assert(!pwh.is_unbounded());

  // first get outer area
  double area = pwh.outer_boundary().area();

  // then add hole areas (which should have opposite sign...)
  typename CGAL::Polygon_with_holes_2<Kernel,Container>::
                                             Hole_const_iterator  hit;
  for (hit = pwh.holes_begin(); hit != pwh.holes_end(); hit++) {
    area += hit->area();
  }
  return area;
}


int main ()
{
  // Construct the two input polygons.
  Polygon_2 P;
  P.push_back (Point_2 (0, 0));
  P.push_back (Point_2 (5, 0));
  P.push_back (Point_2 (3.5, 1.5));
  P.push_back (Point_2 (2.5, 0.5));
  P.push_back (Point_2 (1.5, 1.5));
     
  std::cout << "P = "; print_polygon (P);

  Polygon_2 Q;
  Q.push_back (Point_2 (0, 2));
  Q.push_back (Point_2 (1.5, 0.5));
  Q.push_back (Point_2 (2.5, 1.5));
  Q.push_back (Point_2 (3.5, 0.5));
  Q.push_back (Point_2 (5, 2));
     
  std::cout << "Q = "; print_polygon (Q);

  // Compute the union of P and Q.
  Polygon_with_holes_2 unionR;

  if (CGAL::join (P, Q, unionR)) {
    std::cout << "The union: ";
    print_polygon_with_holes (unionR);
    cout << "Outer area is "<< unionR.outer_boundary().area() << endl;
    cout << "My outer area is "<< my_area(unionR.outer_boundary()) << endl;
    cout << "Number of holes = " << unionR.number_of_holes() << endl;

    Polygon_with_holes_2::Hole_const_iterator hit;
    for (hit = unionR.holes_begin(); hit != unionR.holes_end(); hit++) {
      cout << "Hole area is "<< hit->area() << endl;
      cout << "and mine is "<< my_area(*hit) << endl;
    }
    cout << "Union area = "<< pwh_area(unionR)<<endl;

    //std::cout << "Area of union is "<< unionR.area()<<std::endl;
  } else
    std::cout << "P and Q are disjoint and their union is trivial." 
              << std::endl;
  std::cout << std::endl;

  // Compute the intersection of P and Q.
  Pwh_list_2                  intR;
  Pwh_list_2::const_iterator  it;

  CGAL::intersection (P, Q, std::back_inserter(intR));

  std::cout << "The intersection:" << std::endl;
  for (it = intR.begin(); it != intR.end(); ++it) {
    std::cout << "--> ";
    print_polygon_with_holes (*it);
    cout << "area was " << pwh_area(*it) << endl;
  }
  
  return 0;
}

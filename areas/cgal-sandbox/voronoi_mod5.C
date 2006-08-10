// file: examples/Triangulation_2/Voronoi.C

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Gps_circle_segment_traits_2.h>
#include <CGAL/General_polygon_set_2.h>
#include <CGAL/General_polygon_with_holes_2.h>
#include <list>
#include<cassert>

#include <fstream>
#include <ostream>
#include <iterator>
#include <cmath>

//struct Kernel : CGAL::Exact_predicates_inexact_constructions_kernel {};
struct Kernel : CGAL::Exact_predicates_exact_constructions_kernel {};
//typedef CGAL::Filtered_kernel< CGAL::Simple_cartesian<double> > Kernel;
//typedef CGAL::Simple_cartesian<double> Kernel;

typedef CGAL::Delaunay_triangulation_2<Kernel>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Edge_circulator  Edge_circulator;
typedef Triangulation::Vertex_iterator Vertex_iterator;
typedef Triangulation::Vertex_handle   Vertex_handle;
typedef Triangulation::Point          Point;
typedef Kernel::Circle_2                                Circle_2;
typedef Kernel::Vector_2                                Vector_2;

typedef CGAL::Gps_circle_segment_traits_2<Kernel>       Traits_2;
typedef CGAL::General_polygon_set_2<Traits_2>           Polygon_set_2;
typedef Traits_2::Polygon_2                             Polygon_2;
typedef Traits_2::Polygon_with_holes_2                  Polygon_with_holes_2;
typedef Traits_2::Curve_2                               Curve_2;
typedef Traits_2::X_monotone_curve_2                    X_monotone_curve_2;
typedef CGAL::Orientation                         Orientation;

//typedef CGAL::Polygon_2<Kernel>                    Polygon_2;
//typedef CGAL::Polygon_with_holes_2<Kernel>         Polygon_with_holes_2;
typedef std::list<Polygon_with_holes_2>            Pwh_list_2;


const double twopi = 6.283185307179586476925286766559005768394;

using namespace std;

// allows us to have the same code regardless of whether we use
// exact or inexact constructions...
inline double to_double(double x) {return x;}

//----------------------------------------------------------------------
/// Construct a polygon from a circle.
Polygon_2 construct_polygon (const Circle_2& circle)
{
  // Subdivide the circle into two x-monotone arcs.
  Traits_2 traits;
  Curve_2 curve (circle);
  std::list<CGAL::Object>  objects;
  traits.make_x_monotone_2_object() (curve, std::back_inserter(objects));
  CGAL_assertion (objects.size() == 2);

  // Construct the polygon.
  Polygon_2 pgn;
  X_monotone_curve_2 arc;
  std::list<CGAL::Object>::iterator iter;

  for (iter = objects.begin(); iter != objects.end(); ++iter) {
    CGAL::assign (arc, *iter);
    pgn.push_back (arc);
  }

  return pgn;
}


//----------------------------------------------------------------------
/// given a triangulation and a vertex, create the polygon representing
/// the vertex's voronoi cell -- will give assertion failure if the
/// cell is infinite
Polygon_2 create_voronoi_cell(const Triangulation & T, 
			      const Vertex_handle & vertex) {
  Polygon_2 plgn;

  Edge_circulator first_edge = T.incident_edges(vertex);
  Edge_circulator ec = first_edge;
  do {
    // need to watch out for infinite edges... We assume that the user
    // has put some boundary points around things so that the point he/she
    // is interested in will never have infinite edges...
    assert(! T.is_infinite(ec));
    // get the dual of the edge of the triangulation
    CGAL::Object o = T.dual(ec);
    Kernel::Segment_2 s;
    // make sure we're able to create a segment from it...
    assert(CGAL::assign(s,o));
    // add one end of the point to the polygon (we're hoping that
    // direction of edges is always consistent...)
    plgn.push_back(X_monotone_curve_2(s[0],s[1]));
  } while (++ec != first_edge);
  return plgn;
}


//----------------------------------------------------------------------
/// Convert any funny type of point into a normal point, as long as the
/// the funny_point satisfies the following conditions:
///
/// - funny_point.x() and funny_point.y() are defined
/// - the return type can be converted to a double with the CGAL function
///   to_double
///
template<class FP> Point to_point(const FP & funny_point) {
  return Point(to_double(funny_point.x()), to_double(funny_point.y()));
}

//----------------------------------------------------------------------
/// return the theta value of the object, in range -pi..pi. 
///
/// It is a template so that we can use both vectors and points.
template<class Obj> double to_theta(const Obj & obj) {
  return atan2(to_double(obj.y()),to_double(obj.x()));
}

//----------------------------------------------------------------------
/// returns the area of the triangle defined by the origin and the
/// two points. The result is positive when obj2 is oriented
/// counterclockwise wrt obj1.
///
/// It is a template so that we can use both vectors and points.
template<class Obj> double triangle_area(const Obj& obj1, const Obj& obj2) {
  return to_double(0.5*(obj1.x()*obj2.y() - obj1.y()*obj2.x()));
}


//----------------------------------------------------------------------
/// send a gnuplot-readable set of output points corresponding to the
/// polygon to the ostr
void gnuplot_output(ostream & ostr, const Polygon_2 & plgn) {


  for (Traits_2::Curve_const_iterator it = plgn.curves_begin();
       it != plgn.curves_end(); it++) {
    if (it->is_linear()) {
      cout << it->source() << endl << it->target() << endl;
    } else {
      //cerr << "Circular arc" << endl;
      Circle_2 circle = it->supporting_circle();
      Point circle_center = circle.center();
      Orientation orient = it->orientation();
      Point start_point = to_point(it->source());
      Point end_point   = to_point(it->target());
      double start_theta = to_theta(start_point-circle_center);
      double end_theta   = to_theta(end_point-circle_center);
      // things go counterclockwise...?
      if (orient == CGAL::COUNTERCLOCKWISE && start_theta > end_theta) {
	start_theta -= twopi;
      } else if (orient == CGAL::CLOCKWISE && end_theta > start_theta) {
	start_theta += twopi;
      }
      double radius = sqrt(to_double(circle.squared_radius()));
      const int npoint = 20;
      for (int i = 0; i < npoint; i++) {
	double theta = start_theta + i*(end_theta-start_theta)/npoint;
	Point point(circle_center.x()+radius*cos(theta),
		    circle_center.y()+radius*sin(theta));
	cout << point << endl;
      }
    }
  }

}

//----------------------------------------------------------------------
/// returns the area of a (general) Polygon_2
double polygon_area(const Polygon_2 & plgn) {

  double area = 0.0;

  for (Traits_2::Curve_const_iterator it = plgn.curves_begin();
       it != plgn.curves_end(); it++) {
    double lcl_area;
    if (it->is_linear()) {
      //cout << it->source() << " -> " << it->target() << endl;
      lcl_area = triangle_area(to_point(it->source()), to_point(it->target()));
    } else {
      //cerr << "Circular arc" << endl;
      Circle_2 circle = it->supporting_circle();
      Point circle_center = circle.center();
      Orientation orient = it->orientation();
      Point start_point = to_point(it->source());
      Point end_point   = to_point(it->target());
      double start_theta = to_theta(start_point-circle_center);
      double end_theta   = to_theta(end_point-circle_center);
      // things go counterclockwise...?
      if (orient == CGAL::COUNTERCLOCKWISE && start_theta > end_theta) {
	start_theta -= twopi;
      } else if (orient == CGAL::CLOCKWISE && end_theta > start_theta) {
	start_theta += twopi;
      }
      // area of circular pie slice
      lcl_area = 0.5*to_double(circle.squared_radius())*(end_theta-start_theta);
      // replace triangle defined wrt circle center with triangle
      // defined wrt origin
      lcl_area += triangle_area(start_point,end_point) - triangle_area(
		     start_point-circle_center, end_point-circle_center);
    }
    //cerr << "   segment area = " <<lcl_area << endl;
    area += lcl_area;
  }

  return area;
}



//----------------------------------------------------------------------
int main( )
{

  vector<Kernel::Segment_2> segments;

  const int nsep = 333*3;
  for (int isep = 1; isep <= nsep; isep++) {
    double sep = isep*(3.0/nsep);
    //cerr << "sep = " << sep << endl;

    Triangulation T;
    Vertex_handle zeroth_vertex = T.insert(Point(0.0,0.17));
    Vertex_handle first_vertex  = T.insert(Point(sep,0.38));

    ////// boundary points...
    T.insert(Point(-10.0,0.0));
    T.insert(Point(+10.0,0.0));
    T.insert(Point(0.0,-10.0));
    T.insert(Point(0.0,+10.0));

    double area;
    Polygon_2 plgn = create_voronoi_cell(T, first_vertex);
    Polygon_2 circle = construct_polygon(Circle_2(first_vertex->point(),1.0));

    Pwh_list_2 vertex_neighbourhood;
    CGAL::intersection(plgn, circle, std::back_inserter(vertex_neighbourhood)); 
    area = polygon_area(vertex_neighbourhood.begin()->outer_boundary()); 
    Vector_2 dsep = zeroth_vertex->point()-first_vertex->point();
    cout << sqrt(to_double(dsep.squared_length())) << " " << area << endl;

  }
  
  return 0;
}
 

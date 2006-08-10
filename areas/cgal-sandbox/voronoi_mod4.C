// file: examples/Triangulation_2/Voronoi.C

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
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

struct K : CGAL::Exact_predicates_inexact_constructions_kernel {};

typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Edge_circulator  Edge_circulator;
typedef Triangulation::Vertex_iterator Vertex_iterator;
typedef Triangulation::Vertex_handle   Vertex_handle;
typedef Triangulation::Point          Point;
typedef K::Circle_2                                Circle_2;
typedef K::Vector_2                                Vector_2;

typedef CGAL::Gps_circle_segment_traits_2<K>       Traits_2;
typedef CGAL::General_polygon_set_2<Traits_2>           Polygon_set_2;
typedef Traits_2::Polygon_2                             Polygon_2;
typedef Traits_2::Polygon_with_holes_2                  Polygon_with_holes_2;
typedef Traits_2::Curve_2                               Curve_2;
typedef Traits_2::X_monotone_curve_2                    X_monotone_curve_2;
typedef CGAL::Orientation                         Orientation;

//typedef CGAL::Polygon_2<K>                    Polygon_2;
//typedef CGAL::Polygon_with_holes_2<K>         Polygon_with_holes_2;
typedef std::list<Polygon_with_holes_2>            Pwh_list_2;


const double twopi = 6.283185307179586476925286766559005768394;

using namespace std;

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
    K::Segment_2 s;
    // make sure we're able to create a segment from it...
    assert(CGAL::assign(s,o));
    // add one end of the point to the polygon (we're hoping that
    // direction of edges is always consistent...)
    plgn.push_back(X_monotone_curve_2(s[0],s[1]));
  } while (++ec != first_edge);
  return plgn;
}


//----------------------------------------------------------------------
/// send a gnuplot-readable set of output points corresponding to the
/// polygon to the ostr
void gnuplot_output(ostream & ostr, const Polygon_2 & plgn) {

  //ostream_iterator<pair<double,double> > test(cout,"\n");
  //ostream_iterator<double> test(cout,"\n");
  //pair<double, double> pp = make_pair(2.0,2.0);
  //*test = pp;
  //test++;
  
  

  for (Traits_2::Curve_const_iterator it = plgn.curves_begin();
       it != plgn.curves_end(); it++) {
    if (it->is_linear()) {
      cout << it->source() << endl << it->target() << endl;
    } else {
      cerr << "Circular arc" << endl;
      Circle_2 circle = it->supporting_circle();
      Point circle_center = circle.center();
      Orientation orient = it->orientation();
      Point start_point = Point(to_double(it->source().x()),to_double(it->source().y()));
      Point end_point = Point(to_double(it->target().x()),to_double(it->target().y()));
      double start_phi = atan2(start_point.y()-circle_center.y(), 
			       start_point.x()-circle_center.x());
      double end_phi   = atan2(end_point.y()-circle_center.y(), 
			       end_point.x()-circle_center.x());
      // things go counterclockwise...?
      if (orient == CGAL::COUNTERCLOCKWISE && start_phi > end_phi) {
	start_phi -= twopi;
      } else if (orient == CGAL::CLOCKWISE && end_phi > start_phi) {
	start_phi += twopi;
      }
      double radius = sqrt(circle.squared_radius());
      const int npoint = 20;
      for (int i = 0; i < npoint; i++) {
	double phi = start_phi + i*(end_phi-start_phi)/npoint;
	Point point(circle_center.x()+radius*cos(phi),
		    circle_center.y()+radius*sin(phi));
	cout << point << endl;
      }
      //cerr << orient << " " << CGAL::COUNTERCLOCKWISE << endl;
      //cerr << start_phi << " "<<end_phi<<endl;
      //cerr << " " << circle_center <<endl;
      //cerr << " " << start_point <<endl;
      //cerr << " " << end_point <<endl;
    }
    //it->approximate(test, 14);
  }

  //Traits_2::Curve_const_iterator it = plgn.curves_begin();
  //for (; it != plgn.curves_end(); it++) {
  //  //double x = to_double(it->source().x());
  //  cout << it->source() << endl;
  //}
  //// close the polygon...
  //it = plgn.curves_begin();
  //cout << it->source() << endl;
}


//----------------------------------------------------------------------
int main( )
{

  vector<K::Segment_2> segments;

  //std::ifstream in("data/voronoi.cin");
  //std::istream_iterator<Point> begin(in);
  //std::istream_iterator<Point> end;
  Triangulation T;
  //T.insert(begin, end);
  T.insert(Point(0.0,0.0));
  Vertex_handle first_vertex = T.insert(Point(1.0,0.0));
  //T.insert(Point(-1.0,0.0));
  //T.insert(Point(0.0,1.0));

  ////// boundary points...
  T.insert(Point(-10.0,0.0));
  T.insert(Point(+10.0,0.0));
  T.insert(Point(0.0,-10.0));
  T.insert(Point(0.0,+10.0));


  Polygon_2 plgn = create_voronoi_cell(T, first_vertex);
  Polygon_2 circle = construct_polygon(Circle_2(first_vertex->point(),1.0));
  // gnuplot code for viewing the points and the triangulation
  cout << "set size square" << endl;
  cout << "plot '-' w l, '-' w l,'-' w p ps 3" << endl;
  gnuplot_output(cout, plgn);
  cout << endl;
  gnuplot_output(cout, circle);
  cout << "e"<<endl;
  

  Pwh_list_2 vertex_neighbourhood;
  CGAL::intersection(plgn, circle, std::back_inserter(vertex_neighbourhood)); 
  gnuplot_output(cout, vertex_neighbourhood.begin()->outer_boundary());
  cout << 'e' << endl;
  
  for(Vertex_iterator vertex_it = T.vertices_begin(); 
      vertex_it != T.vertices_end(); vertex_it++) {
    cout << vertex_it->point() << endl;
  }
  cout << "e"<<endl;

  //cerr << "Area is " << P.area() << endl; 


  
  return 0;
}
 

// file: examples/Triangulation_2/Voronoi.C

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <list>
#include<cassert>

#include <fstream>
#include <ostream>

struct K : CGAL::Exact_predicates_inexact_constructions_kernel {};

typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Edge_circulator  Edge_circulator;
typedef Triangulation::Vertex_iterator Vertex_iterator;
typedef Triangulation::Vertex_handle   Vertex_handle;
typedef Triangulation::Point          Point;

typedef CGAL::Polygon_2<K>                    Polygon_2;
typedef CGAL::Polygon_with_holes_2<K>         Polygon_with_holes_2;
typedef std::list<Polygon_with_holes_2>            Pwh_list_2;


using namespace std;

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
    plgn.push_back(s[0]);
  } while (++ec != first_edge);
  return plgn;
}


//----------------------------------------------------------------------
/// send a gnuplot-readable set of output points corresponding to the
/// polygon to the ostr
void gnuplot_output(ostream & ostr, const Polygon_2 & plgn) {
  Polygon_2::Vertex_const_iterator v1 = plgn.vertices_begin();
  for (; v1 != plgn.vertices_end(); v1++) {
    cout << v1->x() << " " << v1->y() << endl;
  }
  // close the polygon...
  v1 = plgn.vertices_begin();
  cout << v1->x() << " " << v1->y() << endl;
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
  T.insert(Point(-1.0,0.0));
  T.insert(Point(0.0,1.0));

  ////// boundary points...
  T.insert(Point(-10.0,0.0));
  T.insert(Point(+10.0,0.0));
  T.insert(Point(0.0,-10.0));
  T.insert(Point(0.0,+10.0));


  Polygon_2 plgn = create_voronoi_cell(T, first_vertex);
  // gnuplot code for viewing the points and the triangulation
  cout << "set size square" << endl;
  cout << "plot '-' w l, '-' w p ps 3" << endl;
  gnuplot_output(cout, plgn);
  cout << "e"<<endl;
  

  for(Vertex_iterator vertex_it = T.vertices_begin(); 
      vertex_it != T.vertices_end(); vertex_it++) {
    cout << vertex_it->point() << endl;
  }
  cout << "e"<<endl;

  cerr << "Area is " << P.area() << endl; 


  
  return 0;
}
 

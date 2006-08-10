// file: examples/Triangulation_2/Voronoi.C

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <list>
#include<cassert>

#include <fstream>

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

// given a triangulation and a vertex, create the polygon representing
// the vertex's voronoi cell -- will give assertion failure if the
// cell is infinite
Polygon_2 create_voronoi_cell(const Triangulation & T, 
			 const Vertex_handle & vertex) {
  Polygon_2 plgn;

  Edge_circulator first_edge = T.incident_edges(vertex);
  Edge_circulator ec = first_edge;
  do {
    // need to watch out for infinite edges...
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

  int ns = 0;
  int nr = 0;  
  int nl = 0;
  int nd = 0;
  //Edge_iterator eit =T.edges_begin();
  //for ( ; eit !=T.edges_end(); ++eit) {
  Edge_circulator start = T.incident_edges(first_vertex);
  Edge_circulator eit = start;
  do {
    nd++;
    // need to watch out for infinite edges...
    cerr << "Got here\n";
    if (T.is_infinite(eit)) {continue;}
    CGAL::Object o = T.dual(eit);
    //Edge_circulator o = eit;
    cerr << "Got past here\n";
    K::Segment_2 s;
    K::Ray_2     r;
    K::Line_2    l;
    K::Point_2   p0, p1;
    if (CGAL::assign(s,o)) {++ns;
      //cout << "seg:  " << s[0] <<" "<< s[1] <<endl;
      segments.push_back(s);
    }
    if (CGAL::assign(r,o)) {++nr;
      //cout << "ray:  " << r.source() <<" "<< r.direction().vector() <<endl;
      segments.push_back(K::Segment_2(r.source(), 
				      r.source()+r.direction().vector()*5));
    }
    if (CGAL::assign(l,o)) {++nl;
      //cout << "line: " << l.point(0) <<" "<< l.direction().vector() <<endl;
      segments.push_back(K::Segment_2(l.point(0)-l.direction().vector()*5, 
				      l.point(0)+l.direction().vector()*5));
    }
  } while (++eit != start);

  Polygon_2 P;
  // gnuplot code for viewing the points and the triangulation
  cout << "set size square" << endl;
  cout << "plot '-' w l, '-' w p ps 3" << endl;
  for(vector<K::Segment_2>::const_iterator seg_it = segments.begin(); 
      seg_it != segments.end(); seg_it++) {
    cout <<(*seg_it)[0]<<endl;
    cout <<(*seg_it)[1]<<endl<<endl;
    P.push_back((*seg_it)[0]); // add the point to the polygon
  }
  cout << "e"<<endl;
  

  for(Vertex_iterator vertex_it = T.vertices_begin(); 
      vertex_it != T.vertices_end(); vertex_it++) {
    cout << vertex_it->point() << endl;
  }
  cout << "e"<<endl;

  cerr << "Area is " << P.area() << endl; 


  std::cerr << "The voronoi diagram has " << ns << " finite edges, " 
	    <<  nr << " rays and " << nl << " lines" <<std::endl;
  std::cerr << "The voronoi diagram has " << nd << " elements"  << std::endl;
  
  return 0;
}
 

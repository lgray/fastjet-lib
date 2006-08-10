// file: examples/Triangulation_2/Voronoi.C

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

#include <fstream>

struct K : CGAL::Exact_predicates_inexact_constructions_kernel {};

typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;
typedef Triangulation::Vertex_iterator Vertex_iterator;
typedef Triangulation::Vertex_handle   Vertex_handle;
typedef Triangulation::Point          Point;

using namespace std;

int main( )
{

  vector<K::Segment_2> segments;

  //std::ifstream in("data/voronoi.cin");
  //std::istream_iterator<Point> begin(in);
  //std::istream_iterator<Point> end;
  Triangulation T;
  //T.insert(begin, end);
  T.insert(Point(0.0,0.0));
  T.insert(Point(1.0,0.0));
  T.insert(Point(-1.0,0.0));
  T.insert(Point(0.0,1.0));

  // boundary points...
  T.insert(Point(-10.0,0.0));
  T.insert(Point(+10.0,0.0));
  T.insert(Point(0.0,-10.0));
  T.insert(Point(0.0,+10.0));

  int ns = 0;
  int nr = 0;  
  int nl = 0;
  int nd = 0;
  Edge_iterator eit =T.edges_begin();
  for ( ; eit !=T.edges_end(); ++eit) {
    nd++;
    CGAL::Object o = T.dual(eit);
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
  }

  // gnuplot code for viewing the points and the triangulation
  cout << "set size square" << endl;
  cout << "plot '-' w l, '-' w p ps 3" << endl;
  for(vector<K::Segment_2>::const_iterator seg_it = segments.begin(); 
      seg_it != segments.end(); seg_it++) {
    cout <<(*seg_it)[0]<<endl;
    cout <<(*seg_it)[1]<<endl<<endl;
  }
  cout << "e"<<endl;

  for(Vertex_iterator vertex_it = T.vertices_begin(); 
      vertex_it != T.vertices_end(); vertex_it++) {
    cout << vertex_it->point() << endl;
  }
  cout << "e"<<endl;



  std::cerr << "The voronoi diagram has " << ns << " finite edges, " 
	    <<  nr << " rays and " << nl << " lines" <<std::endl;
  std::cerr << "The voronoi diagram has " << nd << " elements"  << std::endl;
  
  return 0;
}
 

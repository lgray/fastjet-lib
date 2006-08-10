#include "FjClusterSequenceWithPassiveArea.hh"

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/Gps_circle_segment_traits_2.h>
#include <CGAL/General_polygon_set_2.h>
#include <CGAL/General_polygon_with_holes_2.h>
#include <list>
#include<cassert>

#include <ostream>
#include <iterator>
#include <cmath>


using namespace std;




typedef FjClusterSequenceWithPassiveArea::PassiveAreaCalc PAC;


/// class for carrying out a passive area calculation on a set of initial
/// vectors
class FjClusterSequenceWithPassiveArea::PassiveAreaCalc {
public:
  /// constructor that takes a range of a vector together with the
  /// effective radius for the intersection of discs with voronoi
  /// cells
  PassiveAreaCalc(const vector<FjPseudoJet>::const_iterator &,
		  const vector<FjPseudoJet>::const_iterator &,
		  double effective_R);

  /// return the area of the particle associated with the given
  /// index
  double area (int index) const {return _areas[index];};

private:

  // some typedefs for compact referencing of the CGAL components
  // we'll need -- note that they are different from those used
  // in other parts, because of the need for an exact kernel
  struct EEKernel : CGAL::Exact_predicates_exact_constructions_kernel {};
  //typedef CGAL::Filtered_kernel< CGAL::Simple_cartesian<double> > Kernel;
  //typedef CGAL::Simple_cartesian<double> Kernel;
  
  typedef CGAL::Delaunay_triangulation_2<EEKernel>  Triangulation;
  typedef Triangulation::Edge_iterator  Edge_iterator;
  typedef Triangulation::Edge_circulator  Edge_circulator;
  typedef Triangulation::Vertex_iterator Vertex_iterator;
  typedef Triangulation::Vertex_handle   Vertex_handle;
  typedef Triangulation::Point          Point;
  typedef EEKernel::Circle_2                                Circle_2;
  typedef EEKernel::Vector_2                                Vector_2;
  
  typedef CGAL::Gps_circle_segment_traits_2<EEKernel>       Traits_2;
  typedef CGAL::General_polygon_set_2<Traits_2>           Polygon_set_2;
  typedef Traits_2::Polygon_2                             Polygon_2;
  typedef Traits_2::Polygon_with_holes_2                  Polygon_with_holes_2;
  typedef Traits_2::Curve_2                               Curve_2;
  typedef Traits_2::X_monotone_curve_2                    X_monotone_curve_2;
  typedef CGAL::Orientation                         Orientation;
  typedef std::list<Polygon_with_holes_2>            Pwh_list_2;

  Triangulation _triang;
  std::vector<double>        _areas;    // numbered as jets
  std::vector<Vertex_handle> _vertices; // numbered as jets
  double _effective_R, _effective_R_squared;

  /// allows us to have the same code regardless of whether we use
  /// exact or inexact constructions...
  //inline double to_dble(double x) const {return x;}
  Polygon_2 construct_polygon(const Circle_2& circle) const;
  Polygon_2 construct_polygon(const Vertex_handle & vertex) const;
  Polygon_2 construct_masked_polygon(const Vertex_handle & vertex) const;
  template<class FP> Point to_point(const FP & funny_point) const;
  template<class Obj> double to_theta(const Obj & obj) const;
  template<class Obj> double triangle_area(const Obj& obj1, const Obj& obj2) const;
  void gnuplot_output(ostream & ostr, const Polygon_2 & plgn) const;
  double polygon_area(const Polygon_2 & plgn) const;
};



//----------------------------------------------------------------------
/// Construct a polygon from a circle.
PAC::Polygon_2 PAC::construct_polygon (const Circle_2& circle) const
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
/// given a vertex (and our triangulation), create the polygon representing
/// the vertex's voronoi cell -- will give assertion failure if the
/// cell is infinite
PAC::Polygon_2 PAC::construct_polygon(const Vertex_handle & vertex) const {
  Polygon_2 plgn;

  Edge_circulator first_edge = _triang.incident_edges(vertex);
  Edge_circulator ec = first_edge;
  do {
    // need to watch out for infinite edges... We assume that the user
    // has put some boundary points around things so that the point he/she
    // is interested in will never have infinite edges...
    assert(! _triang.is_infinite(ec));
    // get the dual of the edge of the triangulation
    CGAL::Object o = _triang.dual(ec);
    EEKernel::Segment_2 s;
    // make sure we're able to create a segment from it...
    assert(CGAL::assign(s,o));
    // if the two points of the segment are equal don't add this
    // segment to the polygon (it causes a precondition failure)
    if (CGAL::compare(s[0].x(),s[1].x()) == CGAL::EQUAL &&
	CGAL::compare(s[0].y(),s[1].y()) == CGAL::EQUAL) {continue;}
    // add one end of the points to the polygon (we're hoping that
    // direction of edges is always consistent...)
    plgn.push_back(X_monotone_curve_2(s[0],s[1]));
  } while (++ec != first_edge);
  return plgn;
}


//----------------------------------------------------------------------
/// Convert any funny type of point into a normal double point, as long as the
/// the funny_point satisfies the following conditions:
///
/// - funny_point.x() and funny_point.y() are defined
/// - the return type can be converted to a double with the CGAL function
///   to_double
///
template<class FP> PAC::Point PAC::to_point(const FP & funny_point) const {
  return Point(to_double(funny_point.x()), to_double(funny_point.y()));
}


//----------------------------------------------------------------------
/// return the theta value of the object, in range -pi..pi. 
///
/// It is a template so that we can use both vectors and points.
template<class Obj> double PAC::to_theta(const Obj & obj) const {
  return atan2(to_double(obj.y()),to_double(obj.x()));
}


//----------------------------------------------------------------------
/// returns the area of the triangle defined by the origin and the
/// two points. The result is positive when obj2 is oriented
/// counterclockwise wrt obj1.
///
/// It is a template so that we can use both vectors and points.
template<class Obj> double PAC::triangle_area(const Obj& obj1, const Obj& obj2) const {
  return to_double(0.5*(obj1.x()*obj2.y() - obj1.y()*obj2.x()));
}


//----------------------------------------------------------------------
/// send a gnuplot-readable set of output points corresponding to the
/// polygon to the ostr
void PAC::gnuplot_output(ostream & ostr, const Polygon_2 & plgn) const {


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
/// construct the polygon corresponding to the voronoi cell of the supplied
/// vertex, masked with a disc or radius _effective_R centred on the
/// vertex.
///
PAC::Polygon_2 PAC::construct_masked_polygon(const Vertex_handle & vertex) const {

  // get the voronoi-cell polygon
  Polygon_2 plgn = construct_polygon(vertex);

  // recall that circles take an effective radius...
  Polygon_2 circle = construct_polygon(Circle_2(vertex->point(),
						_effective_R_squared));
  
  Pwh_list_2 intersected_object;
  CGAL::intersection(plgn, circle, std::back_inserter(intersected_object)); 

  // make sure there's only one object in the list
  assert(intersected_object.size() == 1);
  // make sure there are no holes
  assert(intersected_object.begin()->number_of_holes() == 0);

  // the actual polygon we're interested is the outer boundary of the
  // first element of the list...
  return intersected_object.begin()->outer_boundary();
}

//----------------------------------------------------------------------
/// returns the area of a (general) Polygon_2
double PAC::polygon_area(const Polygon_2 & plgn) const {

  double area = 0.0;

  for (Traits_2::Curve_const_iterator it = plgn.curves_begin();
       it != plgn.curves_end(); it++) {
    double lcl_area;
    if (it->is_linear()) {
      // this segment is linear -- so we just calculate the area of the
      // triangle composed of the subject and a vertex at the origin
      lcl_area = triangle_area(to_point(it->source()), to_point(it->target()));
    } else {
      // this segment is circular -- so we'll find the circle, work
      // out the range in phi that we need and calculate the area
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
    area += lcl_area;
  }

  return area;
}



//----------------------------------------------------------------------
// the constructor...
PAC::PassiveAreaCalc(const vector<FjPseudoJet>::const_iterator & jet_begin,
		     const vector<FjPseudoJet>::const_iterator & jet_end,
		     double effective_R) {

  assert(effective_R < 0.5*pi);

  _effective_R         = effective_R;
  _effective_R_squared = effective_R*effective_R;

  double minrap = numeric_limits<double>::max();
  double maxrap = -minrap;

  unsigned int n_tot = 0, n_added = 0;

  // loop over jets and create the triangulation, as well as cross-referencing
  // info
  for (vector<FjPseudoJet>::const_iterator jet_it = jet_begin; 
       jet_it != jet_end; jet_it++) {
    if (jet_it->perp2() == 0.0 && jet_it->E() == jet_it->pz()) {
      // ignore jets with infinite rapidity
      _vertices.push_back(Vertex_handle(NULL));
    } else {

      // generate the corresponding point
      double rap = jet_it->rap(), phi = jet_it->phi();
      Point jet_point(rap, phi);

      // insert into the triangulation and make a note of the vertex handle
      _vertices.push_back(_triang.insert(jet_point));

      // insert into the triangulation a copy of the point if it falls
      // within 2*_R_effective of the 0,2pi borders (because we are
      // interested in any voronoi edge within _R_effective of the
      // other border)
      if (phi < 2*_effective_R) {
	_triang.insert(Point(rap,phi+twopi));
      } else if (twopi-phi < 2*_effective_R) {
	_triang.insert(Point(rap,phi-twopi));
      }

      // track the rapidity range
      maxrap = max(maxrap,rap);
      minrap = min(minrap,rap);
      n_added++;
    }
    n_tot++;
  }

  assert(n_added > 0);

  // now add some final points at borders to make sure that all internal
  // voronoi cells are finite -- the choice of a factor 5 here is a little
  // arbitrary (it should be at least sqrt(2)???)
  double range = 5*max(2*twopi, maxrap-minrap); // 5*max(phi-range,rap-range)
  double midrap = 0.5*(minrap+maxrap);
  _triang.insert(Point(midrap,3*pi+range));
  _triang.insert(Point(midrap, -pi-range));
  _triang.insert(Point(minrap-range, pi));
  _triang.insert(Point(maxrap+range, pi));

  // now store the areas
  _areas.resize(n_tot);
  for (unsigned int i = 0; i < _areas.size(); i++) {
    if (_vertices[i] == NULL) {
      _areas[i] = 0;
    } else {
      Polygon_2 masked_cell = construct_masked_polygon(_vertices[i]);
      _areas[i] = polygon_area(masked_cell);
    }
  }
}


//----------------------------------------------------------------------
///
void FjClusterSequenceWithPassiveArea::_initializePA () {
  
  // check we're the Kt algorithm (Cambridge area is not calculable
  // in the same way).
  assert(_jet_def.jet_finder() == kt_algorithm);
  
  // run the PAC on our original particles
  //_pa_calc = auto_ptr<PAC>(new PAC(_jets.begin(), 
  //      			   _jets.begin()+n_particles(),
  //      			   _effective_Rfact*_jet_def.R()
  //      			   ));
  _pa_calc = new PAC(_jets.begin(), 
		     _jets.begin()+n_particles(),
		     _effective_Rfact*_jet_def.R()
		     );

  // transfer the areas to our local structure -- first the initial
  // ones
  _passive_area.reserve(2*n_particles());
  for (unsigned int i = 0; i < n_particles(); i++) {
    _passive_area.push_back(_pa_calc->area(i));
  }
			   
  // then the combined areas that arise from the clustering
  for (unsigned int i = n_particles(); i < _history.size(); i++) {
    double area;
    if (_history[i].parent2 >= 0) {
      area = _passive_area[_history[i].parent1] + 
  	     _passive_area[_history[i].parent2];
    } else {
      area = _passive_area[_history[i].parent1];
    }
    _passive_area.push_back(area);
  }

}

//----------------------------------------------------------------------
FjClusterSequenceWithPassiveArea::~FjClusterSequenceWithPassiveArea() {
  delete _pa_calc;
}

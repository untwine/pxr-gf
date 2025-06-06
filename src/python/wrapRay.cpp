// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#include <pxr/gf/ray.h>
#include <pxr/gf/bbox3d.h>
#include <pxr/gf/line.h>
#include <pxr/gf/lineSeg.h>
#include <pxr/gf/plane.h>
#include <pxr/gf/range3d.h>

#include <pxr/tf/pyUtils.h>
#include <pxr/tf/wrapTypeHelpers.h>

#include <pxr/boost/python/class.hpp>
#include <pxr/boost/python/def.hpp>
#include <pxr/boost/python/copy_const_reference.hpp>
#include <pxr/boost/python/operators.hpp>
#include <pxr/boost/python/return_arg.hpp>
#include <pxr/boost/python/tuple.hpp>

#include <string>

using std::string;

using namespace pxr;

using namespace pxr::boost::python;

namespace {

static void
SetStartPointHelper( GfRay &self, const GfVec3d &startPoint ) {
    self.SetPointAndDirection( startPoint, self.GetDirection() );
}

static void
SetDirectionHelper( GfRay &self, const GfVec3d &direction ) {
    self.SetPointAndDirection( self.GetStartPoint(), direction );
}

static tuple
FindClosestPointHelper( const GfRay &self, const GfVec3d &point ) {
    double rayDist;
    GfVec3d result = self.FindClosestPoint( point, &rayDist );
    return make_tuple( result, rayDist );
}

static tuple
FindClosestPointsHelper1( const GfRay &l1, const GfLine &l2 )
{
    GfVec3d p1(0), p2(0);
    double t1 = 0.0, t2 = 0.0;
    bool result = GfFindClosestPoints( l1, l2, &p1, &p2, &t1, &t2 );
    return make_tuple( result, p1, p2, t1, t2 );
}

static tuple
FindClosestPointsHelper2( const GfRay &l1, const GfLineSeg &l2 )
{
    GfVec3d p1(0), p2(0);
    double t1 = 0.0, t2 = 0.0;
    bool result = GfFindClosestPoints( l1, l2, &p1, &p2, &t1, &t2 );
    return make_tuple( result, p1, p2, t1, t2 );
}

static tuple
IntersectHelper1( const GfRay &self, const GfVec3d &p0,
                  const GfVec3d &p1, const GfVec3d &p2 )
{
    double dist = 0;
    GfVec3d barycentricCoords(0);
    bool frontFacing = false;
    bool result =
        self.Intersect( p0, p1, p2, &dist, &barycentricCoords, &frontFacing );
    return make_tuple( result, dist, barycentricCoords, frontFacing );
}

static tuple
IntersectHelper2( const GfRay &self, const GfPlane &plane )
{
    double dist = 0;
    bool frontFacing = false;
    bool result = self.Intersect( plane, &dist, &frontFacing );
    return make_tuple( result, dist, frontFacing );
}

static tuple
IntersectHelper3( const GfRay &self, const GfRange3d &box )
{
    double enterDist = 0, exitDist = 0;
    bool result = self.Intersect( box, &enterDist, &exitDist );
    return make_tuple( result, enterDist, exitDist );
}

static tuple
IntersectHelper4( const GfRay &self, const GfBBox3d &box )
{
    double enterDist = 0, exitDist = 0;
    bool result = self.Intersect( box, &enterDist, &exitDist );
    return make_tuple( result, enterDist, exitDist );
}

static tuple
IntersectHelper5( const GfRay &self, const GfVec3d& center, double radius )
{
    double enterDist = 0, exitDist = 0;
    bool result = self.Intersect( center, radius, &enterDist, &exitDist );
    return make_tuple( result, enterDist, exitDist );
}

static tuple
IntersectHelper6(const GfRay &self, 
                 const GfVec3d &origin, 
                 const GfVec3d &axis,
                 double radius)
{
    double enter = 0, exit = 0;
    bool result = self.Intersect(origin, axis, radius, &enter, &exit);
    return make_tuple(result, enter, exit);
}

static tuple
IntersectHelper7(const GfRay &self, 
                 const GfVec3d &origin, 
                 const GfVec3d &axis,
                 double radius,
                 double height)
{
    double enter = 0, exit = 0;
    bool result = self.Intersect(origin, axis, radius, height, &enter, &exit);
    return make_tuple(result, enter, exit);
}

static string _Repr(GfRay const &self) {
    return TF_PY_REPR_PREFIX + "Ray(" + TfPyRepr(self.GetStartPoint()) + ", " +
        TfPyRepr(self.GetDirection()) + ")";
}

} // anonymous namespace 

void wrapRay()
{    
    typedef GfRay This;

    def("FindClosestPoints", FindClosestPointsHelper1, 
        "FindClosestPoints( r1, l2 ) -> tuple<intersects=bool, "
        "p1 = GfVec3d, p2 = GfVec3d, t1 = double, t2 = double>\n"
        "\n"
        "r1 : GfRay\n"
        "l2 : GfLine\n"
        "\n"
        "Computes the closest points between a ray and a line,\n"
        "returning a tuple. The first item in the tuple is true if "
        "they intersect. The two points are returned in p1 and p2.\n"
        "The parametric distance of each point on the ray and line is\n"
        "returned in t1 and t2.\n"
        "----------------------------------------------------------------------"
        );
    def("FindClosestPoints", FindClosestPointsHelper2, 
        "FindClosestPoints( r1, s2 ) -> tuple<intersects = bool, "
        "p1 = GfVec3d, p2 = GfVec3d, t1 = double, t2 = double>\n"
        "\n"
        "r1 : GfRay\n"
        "s2 : GfLineSeg\n"
        "\n"
        "Computes the closest points between a ray and a line segment,\n"
        "returning a tuple. The first item in the tuple is true if "
        "they intersect. The two points are returned in p1 and p2.\n"
        "The parametric distance of each point on the ray and line\n"
        "segment is returned in t1 and t2.\n"
        "----------------------------------------------------------------------"
        );

    class_<This>("Ray", "", init<>())
        .def(init< const GfVec3d &, const GfVec3d & >())

        .def(TfTypePythonClass())

        .def("SetPointAndDirection",
             &This::SetPointAndDirection, return_self<>())
        .def("SetEnds", &This::SetEnds, return_self<>())

        .add_property( "startPoint", make_function
                       (&This::GetStartPoint,
                        return_value_policy<copy_const_reference>()),
                       SetStartPointHelper )
        .add_property( "direction", make_function
                       (&This::GetDirection,
                        return_value_policy<copy_const_reference>()),
                       SetDirectionHelper )

        .def("GetPoint", &This::GetPoint )

        .def("FindClosestPoint", FindClosestPointHelper)

        .def("Transform", &This::Transform, return_self<>())
        
        .def("Intersect", IntersectHelper1,
             "Intersect( p0, p1, p2 ) -> tuple<intersects = bool, dist =\n"
             "float, barycentric = GfVec3d, frontFacing = bool>\n"
             "\n"
             "Intersects the ray with the triangle formed by points p0,\n"
             "p1, and p2.  The first item in the tuple is true if the ray\n"
             "intersects the triangle. dist is the the parametric\n"
             "distance to the intersection point, the barycentric\n"
             "coordinates of the intersection point, and the front-facing\n"
             "flag. The barycentric coordinates are defined with respect\n"
             "to the three vertices taken in order.  The front-facing\n"
             "flag is True if the intersection hit the side of the\n"
             "triangle that is formed when the vertices are ordered\n"
             "counter-clockwise (right-hand rule).\n"
             "\n"
             "Barycentric coordinates are defined to sum to 1 and satisfy\n"
             "this relationsip:\n"
             "\n"
             "    intersectionPoint = (barycentricCoords[0] * p0 +\n"
             "                         barycentricCoords[1] * p1 +\n"
             "                         barycentricCoords[2] * p2);\n"
             "----------------------------------------------------------------------"
            )
        .def( "Intersect", IntersectHelper2,
              "Intersect( plane ) -> tuple<intersects = bool, dist = float,\n"
              "frontFacing = bool>\n"
              "\n"
              "Intersects the ray with the Gf.Plane.  The first item in\n"
              "the returned tuple is true if the ray intersects the plane.\n"
              "dist is the parametric distance to the intersection point\n"
              "and frontfacing is true if the intersection is on the side\n"
              "of the plane toward which the plane's normal points.\n"
             "----------------------------------------------------------------------"
            )
        .def( "Intersect", IntersectHelper3,
              "Intersect( range3d ) -> tuple<intersects = bool, enterDist\n"
              "= float, exitDist = float>\n"
              //\n"
              "Intersects the plane with an axis-aligned box in a\n"
              "Gf.Range3d.  intersects is true if the ray intersects it at\n"
              "all within bounds. If there is an intersection then enterDist\n"
              "and exitDist will be the parametric distances to the two\n"
              "intersection points.\n"
              "----------------------------------------------------------------------"
            )
        .def( "Intersect", IntersectHelper4,
              "Intersect( bbox3d ) -> tuple<intersects = bool, enterDist\n"
              "= float, exitDist = float>\n"
              //\n"
              "Intersects the plane with an oriented box in a Gf.BBox3d.\n"
              "intersects is true if the ray intersects it at all within\n"
              "bounds. If there is an intersection then enterDist and\n"
              "exitDist will be the parametric distances to the two\n"
              "intersection points.\n"
              "----------------------------------------------------------------------"
            )
        .def( "Intersect", IntersectHelper5,
              "Intersect( center, radius ) -> tuple<intersects = bool,\n"
              "enterDist = float, exitDist = float>\n"
              "\n"
              "Intersects the plane with an sphere. intersects is true if\n"
              "the ray intersects it at all within the sphere. If there is\n"
              "an intersection then enterDist and exitDist will be the\n"
              "parametric distances to the two intersection points.\n"
              "----------------------------------------------------------------------"
            )
        .def( "Intersect", IntersectHelper6,
              "Intersect( origin, axis, radius ) -> tuple<intersects = bool,\n"
              "enterDist = float, exitDist = float>\n"
              "\n"
              "Intersects the plane with an infinite cylinder. intersects\n"
              "is true if the ray intersects it at all within the\n"
              "sphere. If there is an intersection then enterDist and\n"
              "exitDist will be the parametric distances to the two\n"
              "intersection points.\n"
              "----------------------------------------------------------------------"
            )
        .def( "Intersect", IntersectHelper7,
              "Intersect( origin, axis, radius, height ) -> \n"
              "tuple<intersects = bool, enterDist = float, exitDist = float>\n"
              "\n"
              "Intersects the plane with an cylinder. intersects\n"
              "is true if the ray intersects it at all within the\n"
              "sphere. If there is an intersection then enterDist and\n"
              "exitDist will be the parametric distances to the two\n"
              "intersection points.\n"
              "----------------------------------------------------------------------"
            )

        .def( str(self) )
        .def( self == self )
        .def( self != self )

        .def("__repr__", _Repr)

        ;
    
}

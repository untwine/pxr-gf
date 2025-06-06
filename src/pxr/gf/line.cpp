// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#include "./line.h"
#include "./math.h"
#include "./ostreamHelpers.h"

#include <pxr/tf/type.h>

#include <ostream>

namespace pxr {

// CODE_COVERAGE_OFF_GCOV_BUG
TF_REGISTRY_FUNCTION(pxr::TfType) {
    TfType::Define<GfLine>();
}
// CODE_COVERAGE_ON_GCOV_BUG

GfVec3d
GfLine::FindClosestPoint(const GfVec3d &point, double *t) const
{
    // Compute the vector from the start point to the given point.
    GfVec3d v = point - _p0;

    // Find the length of the projection of this vector onto the line.
    double lt = GfDot(v, _dir);
    
    if (t)
        *t = lt;

    return GetPoint( lt );
}

bool
GfFindClosestPoints( const GfLine &l1, const GfLine &l2,
                     GfVec3d *closest1, GfVec3d *closest2,
                     double *t1, double *t2 )
{
    // Define terms:
    //   p1 = line 1's position
    //   d1 = line 1's direction
    //   p2 = line 2's position
    //   d2 = line 2's direction
    const GfVec3d &p1 = l1._p0; 
    const GfVec3d &d1 = l1._dir;
    const GfVec3d &p2 = l2._p0;
    const GfVec3d &d2 = l2._dir;

    // We want to find points closest1 and closest2 on each line.
    // Their parametric definitions are:
    //   closest1 = p1 + t1 * d1
    //   closest2 = p2 + t2 * d2
    //
    // We know that the line connecting closest1 and closest2 is
    // perpendicular to both the ray and the line segment. So:
    //   d1 . (closest2 - closest1) = 0
    //   d2 . (closest2 - closest1) = 0
    //
    // Substituting gives us:
    //   d1 . [ (p2 + t2 * d2) - (p1 + t1 * d1) ] = 0
    //   d2 . [ (p2 + t2 * d2) - (p1 + t1 * d1) ] = 0
    //
    // Rearranging terms gives us:
    //   t2 * (d1.d2) - t1 * (d1.d1) = d1.p1 - d1.p2
    //   t2 * (d2.d2) - t1 * (d2.d1) = d2.p1 - d2.p2
    //
    // Substitute to simplify:
    //   a = d1.d2
    //   b = d1.d1
    //   c = d1.p1 - d1.p2
    //   d = d2.d2
    //   e = d2.d1 (== a, if you're paying attention)
    //   f = d2.p1 - d2.p2
    double a = GfDot(d1, d2);
    double b = GfDot(d1, d1);
    double c = GfDot(d1, p1) - GfDot(d1, p2);
    double d = GfDot(d2, d2);
    double e = a;
    double f = GfDot(d2, p1) - GfDot(d2, p2);

    // And we end up with:
    //  t2 * a - t1 * b = c
    //  t2 * d - t1 * e = f
    //
    // Solve for t1 and t2:
    //  t1 = (c * d - a * f) / (a * e - b * d)
    //  t2 = (c * e - b * f) / (a * e - b * d)
    //
    // Note the identical denominators...
    double denom = a * e - b * d;

    // Denominator == 0 means the lines are parallel; no intersection.
    if ( GfIsClose( denom, 0, 1e-6 ) )
        return false;

    double lt1 = (c * d - a * f) / denom;
    double lt2 = (c * e - b * f) / denom;

    if ( closest1 )
        *closest1 = l1.GetPoint( lt1 );

    if ( closest2 )
        *closest2 = l2.GetPoint( lt2 );

    if ( t1 )
        *t1 = lt1;

    if ( t2 )
        *t2 = lt2;
    
    return true;
}

std::ostream &
operator<<(std::ostream &out, const GfLine &line)
{
    return out 
        << '(' 
        << "point:" << Gf_OstreamHelperP(line.GetPoint(0.0)) 
        << ' ' 
        << "direction:" << Gf_OstreamHelperP(line.GetDirection()) 
        << ')';
}

}  // namespace pxr

// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#ifndef PXR_GF_LINE_SEG_H
#define PXR_GF_LINE_SEG_H

/// \file gf/lineSeg.h
/// \ingroup group_gf_BasicGeometry

#include "./line.h"
#include "./vec3d.h"
#include "./api.h"

#include <float.h>
#include <iosfwd>

namespace pxr {

/// \class GfLineSeg
/// \ingroup group_gf_BasicGeometry
///
/// Basic type: 3D line segment
///
/// This class represents a three-dimensional line segment in space.
///
class GfLineSeg {

  public:

    /// The default constructor leaves line parameters undefined.
    GfLineSeg() {
    }

    /// Construct a line segment that spans two points.
    GfLineSeg(const GfVec3d &p0, const GfVec3d &p1 ) {
        _length = _line.Set( p0, p1 - p0 );
    }

    /// Return the point on the segment specified by the parameter t. 
    /// p = p0 + t * (p1 - p0)
    GfVec3d GetPoint( double t ) const {return _line.GetPoint( t * _length );}

    /// Return the normalized direction of the line.
    const GfVec3d &GetDirection() const { return _line.GetDirection(); }

    /// Return the length of the line
    double GetLength() const { return _length; }

    /// Returns the point on the line that is closest to \p point. If
    /// \p t is not \c NULL, it will be set to the parametric
    /// distance along the line of the closest point.
    GF_API
    GfVec3d FindClosestPoint(const GfVec3d &point, double *t = NULL) const;

    /// Component-wise equality test. The starting points and directions,
    /// must match exactly for lines to be considered equal.
    bool		operator ==(const GfLineSeg &l) const {
	return (_line == l._line && _length  == l._length);
    }

    /// Component-wise inequality test. The starting points,
    /// and directions must match exactly for lines to be
    /// considered equal.
    bool		operator !=(const GfLineSeg &r) const {
	return ! (*this == r);
    }

  private:
    GF_API
    friend bool GfFindClosestPoints( const GfLine &, const GfLineSeg &,
                                     GfVec3d *, GfVec3d *,
                                     double *, double * );
    GF_API
    friend bool GfFindClosestPoints( const GfLineSeg &, const GfLineSeg &,
                                     GfVec3d *, GfVec3d *,
                                     double *, double * );

    GfLine              _line;
    double              _length;   // distance from p0 to p1
};

/// Computes the closets points on \p line and \p seg.
///
/// The two points are returned in \p p1 and \p p2. The parametric distances
/// of \p p1 and \p p2 along the line and segment are returned in \p t1 and \p
/// t2.
///
/// This returns \c false if the lines were close enough to parallel that no
/// points could be computed; in this case, the other return values are
/// undefined.
GF_API
bool GfFindClosestPoints( const GfLine &line, const GfLineSeg &seg,
                          GfVec3d *p1 = nullptr, GfVec3d *p2 = nullptr,
                          double *t1 = nullptr, double *t2 = nullptr );

/// Computes the closets points on two line segments, \p seg1 and \p seg2. The
/// two points are returned in \p p1 and \p p2. The parametric distances of \p
/// p1 and \p p2 along the segments are returned in \p t1 and \p t2.
///
/// This returns \c false if the lines were close enough to parallel that no
/// points could be computed; in this case, the other return values are
/// undefined.
GF_API
bool GfFindClosestPoints( const GfLineSeg &seg1, const GfLineSeg &seg2,
                          GfVec3d *p1 = nullptr, GfVec3d *p2 = nullptr,
                          double *t1 = nullptr, double *t2 = nullptr );

/// Output a GfLineSeg.
/// \ingroup group_gf_DebuggingOutput
GF_API std::ostream &operator<<(std::ostream&, const GfLineSeg&);

}  // namespace pxr

#endif // PXR_GF_LINE_SEG_H

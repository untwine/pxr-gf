// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

////////////////////////////////////////////////////////////////////////
// This file is generated by a script.  Do not edit directly.  Edit the
// vec.template.h file to make changes.

#ifndef PXR_GF_VEC3H_H
#define PXR_GF_VEC3H_H

/// \file gf/vec3h.h
/// \ingroup group_gf_LinearAlgebra

#include <pxr/tf/diagnostic.h>
#include "./api.h"
#include "./limits.h"
#include "./traits.h"
#include "./math.h"
#include "./half.h"
#include <pxr/tf/hash.h>

#include <cstddef>
#include <cmath>

#include <iosfwd>

namespace pxr {

class GfVec3h;

template <>
struct GfIsGfVec<class GfVec3h> { static const bool value = true; };

/// \class GfVec3h
/// \ingroup group_gf_LinearAlgebra
///
/// Basic type for a vector of 3 GfHalf components.
///
/// Represents a vector of 3 components of type \c GfHalf.
/// It is intended to be fast and simple.
///
class GfVec3h
{
public:
    /// Scalar element type and dimension.
    typedef GfHalf ScalarType;
    static const size_t dimension = 3;

    /// Default constructor does no initialization.
    GfVec3h() = default;

    /// Initialize all elements to a single value.
    constexpr explicit GfVec3h(GfHalf value)
        : _data{ value, value, value }
    {
    }

    /// Initialize all elements with explicit arguments.
    constexpr GfVec3h(GfHalf s0, GfHalf s1, GfHalf s2)
        : _data{ s0, s1, s2 }
    {
    }

    /// Construct with pointer to values.
    template <class Scl>
    constexpr explicit GfVec3h(Scl const *p)
        : _data{ p[0], p[1], p[2] }
    {
    }

    /// Construct from GfVec3d.
    explicit GfVec3h(class GfVec3d const &other);

    /// Construct from GfVec3f.
    explicit GfVec3h(class GfVec3f const &other);

    /// Implicitly convert from GfVec3i.
    GfVec3h(class GfVec3i const &other);
 
    /// Create a unit vector along the X-axis.
    static GfVec3h XAxis() {
        GfVec3h result(0);
        result[0] = 1;
        return result;
    }
    /// Create a unit vector along the Y-axis.
    static GfVec3h YAxis() {
        GfVec3h result(0);
        result[1] = 1;
        return result;
    }
    /// Create a unit vector along the Z-axis.
    static GfVec3h ZAxis() {
        GfVec3h result(0);
        result[2] = 1;
        return result;
    }

    /// Create a unit vector along the i-th axis, zero-based.  Return the zero
    /// vector if \p i is greater than or equal to 3.
    static GfVec3h Axis(size_t i) {
        GfVec3h result(0);
        if (i < 3)
            result[i] = 1;
        return result;
    }

    /// Set all elements with passed arguments.
    GfVec3h &Set(GfHalf s0, GfHalf s1, GfHalf s2) {
        _data[0] = s0;
        _data[1] = s1;
        _data[2] = s2;
        return *this;
    }

    /// Set all elements with a pointer to data.
    GfVec3h &Set(GfHalf const *a) {
        return Set(a[0], a[1], a[2]);
    }

    /// Direct data access.
    GfHalf const *data() const { return _data; }
    GfHalf *data() { return _data; }
    GfHalf const *GetArray() const { return data(); }

    /// Indexing.
    GfHalf const &operator[](size_t i) const { return _data[i]; }
    GfHalf &operator[](size_t i) { return _data[i]; }

    /// Hash.
    friend inline size_t hash_value(GfVec3h const &vec) {
        return TfHash::Combine(vec[0], vec[1], vec[2]);
    }

    /// Equality comparison.
    bool operator==(GfVec3h const &other) const {
        return _data[0] == other[0] &&
               _data[1] == other[1] &&
               _data[2] == other[2];
    }
    bool operator!=(GfVec3h const &other) const {
        return !(*this == other);
    }

    // TODO Add inequality for other vec types...
    /// Equality comparison.
    GF_API
    bool operator==(class GfVec3d const &other) const;
    /// Equality comparison.
    GF_API
    bool operator==(class GfVec3f const &other) const;
    /// Equality comparison.
    GF_API
    bool operator==(class GfVec3i const &other) const;
    
    /// Create a vec with negated elements.
    GfVec3h operator-() const {
        return GfVec3h(-_data[0], -_data[1], -_data[2]);
    }

    /// Addition.
    GfVec3h &operator+=(GfVec3h const &other) {
        _data[0] += other[0];
        _data[1] += other[1];
        _data[2] += other[2];
        return *this;
    }
    friend GfVec3h operator+(GfVec3h const &l, GfVec3h const &r) {
        return GfVec3h(l) += r;
    }

    /// Subtraction.
    GfVec3h &operator-=(GfVec3h const &other) {
        _data[0] -= other[0];
        _data[1] -= other[1];
        _data[2] -= other[2];
        return *this;
    }
    friend GfVec3h operator-(GfVec3h const &l, GfVec3h const &r) {
        return GfVec3h(l) -= r;
    }

    /// Multiplication by scalar.
    GfVec3h &operator*=(double s) {
        _data[0] *= s;
        _data[1] *= s;
        _data[2] *= s;
        return *this;
    }
    GfVec3h operator*(double s) const {
        return GfVec3h(*this) *= s;
    }
    friend GfVec3h operator*(double s, GfVec3h const &v) {
        return v * s;
    }

        /// Division by scalar.
    // TODO should divide by the scalar type.
    GfVec3h &operator/=(double s) {
        // TODO This should not multiply by 1/s, it should do the division.
        // Doing the division is more numerically stable when s is close to
        // zero.
        return *this *= (1.0 / s);
    }
    GfVec3h operator/(double s) const {
        return *this * (1.0 / s);
    }
    
    /// See GfDot().
    GfHalf operator*(GfVec3h const &v) const {
        return _data[0] * v[0] + _data[1] * v[1] + _data[2] * v[2];
    }

    /// Returns the projection of \p this onto \p v. That is:
    /// \code
    /// v * (*this * v)
    /// \endcode
    GfVec3h GetProjection(GfVec3h const &v) const {
        return v * (*this * v);
    }

    /// Returns the orthogonal complement of \p this->GetProjection(b).
    /// That is:
    /// \code
    ///  *this - this->GetProjection(b)
    /// \endcode
    GfVec3h GetComplement(GfVec3h const &b) const {
        return *this - this->GetProjection(b);
    }

    /// Squared length.
    GfHalf GetLengthSq() const {
        return *this * *this;
    }

    /// Length
    GfHalf GetLength() const {
        return GfSqrt(GetLengthSq());
    }

    /// Normalizes the vector in place to unit length, returning the
    /// length before normalization. If the length of the vector is
    /// smaller than \p eps, then the vector is set to vector/\c eps.
    /// The original length of the vector is returned. See also GfNormalize().
    ///
    /// \todo This was fixed for bug 67777. This is a gcc64 optimizer bug.
    /// By tickling the code, it no longer tries to write into
    /// an illegal memory address (in the code section of memory).
    GfHalf Normalize(GfHalf eps = 0.001) {
        // TODO this seems suspect...  suggest dividing by length so long as
        // length is not zero.
        GfHalf length = GetLength();
        *this /= (length > eps) ? length : eps;
        return length;
    }

    GfVec3h GetNormalized(GfHalf eps = 0.001) const {
        GfVec3h normalized(*this);
        normalized.Normalize(eps);
        return normalized;
    }

    /// Orthogonalize and optionally normalize a set of basis vectors. This
    /// uses an iterative method that is very stable even when the vectors are
    /// far from orthogonal (close to colinear).  The number of iterations and
    /// thus the computation time does increase as the vectors become close to
    /// colinear, however. Returns a bool specifying whether the solution
    /// converged after a number of iterations.  If it did not converge, the
    /// returned vectors will be as close as possible to orthogonal within the
    /// iteration limit. Colinear vectors will be unaltered, and the method
    /// will return false.
    GF_API
    static bool OrthogonalizeBasis(
        GfVec3h *tx, GfVec3h *ty, GfVec3h *tz,
        const bool normalize,
        double eps = GF_MIN_ORTHO_TOLERANCE);

    /// Sets \c v1 and \c v2 to unit vectors such that v1, v2 and *this are
    /// mutually orthogonal.  If the length L of *this is smaller than \c eps,
    /// then v1 and v2 will have magnitude L/eps.  As a result, the function
    /// delivers a continuous result as *this shrinks in length.
    GF_API
    void BuildOrthonormalFrame(GfVec3h *v1, GfVec3h *v2,
                    GfHalf eps = 0.001) const;

  
private:
    GfHalf _data[3];
};

/// Output a GfVec3h.
/// \ingroup group_gf_DebuggingOutput
GF_API std::ostream& operator<<(std::ostream &, GfVec3h const &);


}  // namespace pxr

#include "./vec3d.h"
#include "./vec3f.h"
#include "./vec3i.h"

namespace pxr {

inline
GfVec3h::GfVec3h(class GfVec3d const &other)
{
    _data[0] = other[0];
    _data[1] = other[1];
    _data[2] = other[2];
}
inline
GfVec3h::GfVec3h(class GfVec3f const &other)
{
    _data[0] = other[0];
    _data[1] = other[1];
    _data[2] = other[2];
}
inline
GfVec3h::GfVec3h(class GfVec3i const &other)
{
    _data[0] = other[0];
    _data[1] = other[1];
    _data[2] = other[2];
}

/// Returns component-wise multiplication of vectors \p v1 and \p v2.
inline GfVec3h
GfCompMult(GfVec3h const &v1, GfVec3h const &v2) {
    return GfVec3h(
        v1[0] * v2[0],
        v1[1] * v2[1],
        v1[2] * v2[2]
        );
}

/// Returns component-wise quotient of vectors \p v1 and \p v2.
inline GfVec3h
GfCompDiv(GfVec3h const &v1, GfVec3h const &v2) {
    return GfVec3h(
        v1[0] / v2[0],
        v1[1] / v2[1],
        v1[2] / v2[2]
        );
}

/// Returns the dot (inner) product of two vectors.
inline GfHalf
GfDot(GfVec3h const &v1, GfVec3h const &v2) {
    return v1 * v2;
}


/// Returns the geometric length of \c v.
inline GfHalf
GfGetLength(GfVec3h const &v)
{
    return v.GetLength();
}

/// Normalizes \c *v in place to unit length, returning the length before
/// normalization. If the length of \c *v is smaller than \p eps then \c *v is
/// set to \c *v/eps.  The original length of \c *v is returned.
inline GfHalf
GfNormalize(GfVec3h *v, GfHalf eps = 0.001)
{
    return v->Normalize(eps);
}

/// Returns a normalized (unit-length) vector with the same direction as \p v.
/// If the length of this vector is smaller than \p eps, the vector divided by
/// \p eps is returned.
inline GfVec3h
GfGetNormalized(GfVec3h const &v, GfHalf eps = 0.001)
{
    return v.GetNormalized(eps);
}

/// Returns the projection of \p a onto \p b. That is:
/// \code
/// b * (a * b)
/// \endcode
inline GfVec3h
GfGetProjection(GfVec3h const &a, GfVec3h const &b)
{
    return a.GetProjection(b);
}

/// Returns the orthogonal complement of \p a.GetProjection(b). That is:
/// \code
///  a - a.GetProjection(b)
/// \endcode
inline GfVec3h
GfGetComplement(GfVec3h const &a, GfVec3h const &b)
{
    return a.GetComplement(b);
}

/// Tests for equality within a given tolerance, returning \c true if the
/// length of the difference vector is less than or equal to \p tolerance.
inline bool
GfIsClose(GfVec3h const &v1, GfVec3h const &v2, double tolerance)
{
    GfVec3h delta = v1 - v2;
    return delta.GetLengthSq() <= tolerance * tolerance;
}


GF_API bool
GfOrthogonalizeBasis(GfVec3h *tx, GfVec3h *ty, GfVec3h *tz,
                     bool normalize, double eps = GF_MIN_ORTHO_TOLERANCE);

GF_API void
GfBuildOrthonormalFrame(GfVec3h const &v0,
                        GfVec3h* v1,
                        GfVec3h* v2,
                        GfHalf eps = 0.001);

/// Returns the cross product of \p v1 and \p v2.
inline GfVec3h
GfCross(GfVec3h const &v1, GfVec3h const &v2)
{
    return GfVec3h(
        v1[1] * v2[2] - v1[2] * v2[1],
        v1[2] * v2[0] - v1[0] * v2[2],
        v1[0] * v2[1] - v1[1] * v2[0]);
}

/// Returns the cross product of \p v1 and \p v2. 
/// \see GfCross()
inline GfVec3h
operator^(GfVec3h const &v1, GfVec3h const &v2)
{
    return GfCross(v1, v2);
}

/// Spherical linear interpolation in three dimensions.
GF_API GfVec3h
GfSlerp(double alpha, GfVec3h const &v0, GfVec3h const &v1);

 
 
}  // namespace pxr

#endif // PXR_GF_VEC3H_H

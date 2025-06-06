// Copyright 2016 Pixar
//
// Licensed under the terms set forth in the LICENSE.txt file available at
// https://openusd.org/license.
//
// Modified by Jeremy Retailleau.

#ifndef PXR_GF_SIZE2_H
#define PXR_GF_SIZE2_H

/// \file gf/size2.h
/// \ingroup group_gf_LinearAlgebra

#include <pxr/arch/inttypes.h>
#include "./vec2i.h"
#include "./api.h"

#include <iosfwd>

namespace pxr {

/// \class GfSize2
/// \ingroup group_gf_LinearAlgebra
///
/// Two-dimensional array of sizes
///
/// GfSize2 is used to represent pairs of counts.  It is based on the datatype
/// size_t, and thus can only represent non-negative values in each dimension.
/// If you need to represent negative numbers as well, use GfVec2i.
///
/// Usage of GfSize2 is similar to that of GfVec2i, except that all
/// mathematical operations are componentwise (including multiplication).
///
class GfSize2 {
public:
    /// Default constructor initializes components to zero.
    GfSize2() {
        Set(0, 0);
    }

    /// Copy constructor.
    GfSize2(const GfSize2& o) {
        *this = o;
    }

    /// Conversion from GfVec2i.
    explicit GfSize2(const GfVec2i&o) {
        Set(o[0], o[1]);
    }

    /// Construct from an array.
    GfSize2(const size_t v[2]) {
        Set(v);
    }

    /// Construct from two values.
    GfSize2(size_t v0, size_t v1) {
        Set(v0, v1);
    }

    /// Set to the values in a given array.
    GfSize2 & Set(const size_t v[2]) {
        _vec[0] = v[0]; 
        _vec[1] = v[1]; 
        return *this;
    }

    /// Set to values passed directly.
    GfSize2 & Set(size_t v0, size_t v1) {
        _vec[0] = v0; 
        _vec[1] = v1; 
        return *this;
    }

    /// Array operator.
    size_t & operator [](size_t i) {
        return _vec[i];
    }

    /// Const array operator.
    const size_t & operator [](size_t i) const {
        return _vec[i];
    }

    /// Component-wise equality.
    bool operator ==(const GfSize2 &v) const {
        return _vec[0] == v._vec[0] && _vec[1] == v._vec[1];
    }

    /// Component-wise inequality.
    bool operator !=(const GfSize2 &v) const {
        return ! (*this == v);
    }

    /// Component-wise in-place addition.
    GfSize2 & operator +=(const GfSize2 &v) {
        _vec[0] += v._vec[0]; 
        _vec[1] += v._vec[1]; 
        return *this;
    }

    /// Component-wise in-place subtraction.
    GfSize2 & operator -=(const GfSize2 &v) {
        _vec[0] -= v._vec[0]; 
        _vec[1] -= v._vec[1]; 
        return *this;
    }

    /// Component-wise in-place multiplication.
    GfSize2 & operator *=(GfSize2 const &v) {
        _vec[0] *= v._vec[0];
        _vec[1] *= v._vec[1];
        return *this;
    }

    /// Component-wise in-place multiplication by a scalar.
    GfSize2 & operator *=(int d) {
        _vec[0] = _vec[0] * d;
        _vec[1] = _vec[1] * d;
        return *this;
    }

    /// Component-wise in-place division by a scalar.
    GfSize2 & operator /=(int d) {
        _vec[0] = _vec[0] / d;
        _vec[1] = _vec[1] / d;
        return *this;
    }

    /// Component-wise addition.
    friend GfSize2 operator +(const GfSize2 &v1, const GfSize2 &v2) {
        return GfSize2(v1._vec[0]+v2._vec[0],
                       v1._vec[1]+v2._vec[1]);
    }

    /// Component-wise subtraction.
    friend GfSize2 operator -(const GfSize2 &v1, const GfSize2 &v2) {
        return GfSize2(v1._vec[0]-v2._vec[0],
                       v1._vec[1]-v2._vec[1]);
    }

    /// Component-wise multiplication.
    friend GfSize2 operator *(const GfSize2 &v1, const GfSize2 &v2) {
        return GfSize2(v1._vec[0]*v2._vec[0],
                       v1._vec[1]*v2._vec[1]);
    }

    /// Component-wise multiplication by a scalar.
    friend GfSize2 operator *(const GfSize2 &v1, int s) {
        return GfSize2(v1._vec[0]*s,
                       v1._vec[1]*s);
    }

    /// Component-wise multiplication by a scalar.
    friend GfSize2 operator *(int s, const GfSize2 &v1) {
        return GfSize2(v1._vec[0]*s,
                       v1._vec[1]*s);
    }

    /// Component-wise division by a scalar.
    friend GfSize2 operator /(const GfSize2 &v1, int s) {
        return GfSize2(v1._vec[0]/s,
                       v1._vec[1]/s);
    }

    /// Output operator.
    GF_API
    friend std::ostream &operator<<(std::ostream &o, GfSize2 const &v);

    /// Conversion to GfVec2i.
    operator GfVec2i() const {
        return GfVec2i(_vec[0], _vec[1]);
    }
 private:
    size_t _vec[2];
};

// Friend functions must be declared
GF_API std::ostream &operator<<(std::ostream &o, GfSize2 const &v);

}  // namespace pxr

#endif // PXR_GF_SIZE2_H 

//
// Copyright 2016 Pixar
//
// Licensed under the Apache License, Version 2.0 (the "Apache License")
// with the following modification; you may not use this file except in
// compliance with the Apache License and the following modification to it:
// Section 6. Trademarks. is deleted and replaced with:
//
// 6. Trademarks. This License does not grant permission to use the trade
//    names, trademarks, service marks, or product names of the Licensor
//    and its affiliates, except as required to comply with Section 4(c) of
//    the License and to reproduce the content of the NOTICE file.
//
// You may obtain a copy of the Apache License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the Apache License with the above modification is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied. See the Apache License for the specific
// language governing permissions and limitations under the Apache License.
//
////////////////////////////////////////////////////////////////////////
// This file is generated by a script.  Do not edit directly.  Edit the
// vec.template.h file to make changes.

#ifndef GF_VEC4D_H
#define GF_VEC4D_H

/// \file gf/vec4d.h
/// \ingroup group_gf_LinearAlgebra

#include "pxr/pxr.h"
#include "pxr/base/tf/diagnostic.h"
#include "pxr/base/gf/api.h"
#include "pxr/base/gf/limits.h"
#include "pxr/base/gf/traits.h"
#include "pxr/base/gf/math.h"

#include <hboost/functional/hash.hpp>

#include <cstddef>
#include <cmath>

#include <iosfwd>

PXR_NAMESPACE_OPEN_SCOPE

class GfVec4d;

template <>
struct GfIsGfVec<class GfVec4d> { static const bool value = true; };

/// \class GfVec4d
/// \ingroup group_gf_LinearAlgebra
///
/// Basic type for a vector of 4 double components.
///
/// Represents a vector of 4 components of type \c double.
/// It is intended to be fast and simple.
///
class GfVec4d
{
public:
    /// Scalar element type and dimension.
    typedef double ScalarType;
    static const size_t dimension = 4;

    /// Default constructor does no initialization.
    GfVec4d() {}

    // Copy constructor.
    // TODO Remove this, use compiler-generated.
    GfVec4d(const GfVec4d &other) {
        *this = other;
    }

    /// Initialize all elements to a single value.
    explicit GfVec4d(double value) {
        _data[0] = value;
        _data[1] = value;
        _data[2] = value;
        _data[3] = value;
    }

    /// Initialize all elements with explicit arguments.
    GfVec4d(double s0, double s1, double s2, double s3) {
        Set(s0, s1, s2, s3);
    }

    /// Construct with pointer to values.
    template <class Scl>
    explicit GfVec4d(Scl const *p) { Set(p); }

    /// Implicitly convert from GfVec4f.
    GfVec4d(class GfVec4f const &other);

    /// Implicitly convert from GfVec4h.
    GfVec4d(class GfVec4h const &other);

    /// Implicitly convert from GfVec4i.
    GfVec4d(class GfVec4i const &other);
 
    /// Create a unit vector along the X-axis.
    static GfVec4d XAxis() {
        GfVec4d result(0);
        result[0] = 1;
        return result;
    }
    /// Create a unit vector along the Y-axis.
    static GfVec4d YAxis() {
        GfVec4d result(0);
        result[1] = 1;
        return result;
    }
    /// Create a unit vector along the Z-axis.
    static GfVec4d ZAxis() {
        GfVec4d result(0);
        result[2] = 1;
        return result;
    }
    /// Create a unit vector along the W-axis.
    static GfVec4d WAxis() {
        GfVec4d result(0);
        result[3] = 1;
        return result;
    }

    /// Create a unit vector along the i-th axis, zero-based.  Return the zero
    /// vector if \p i is greater than or equal to 4.
    static GfVec4d Axis(size_t i) {
        GfVec4d result(0);
        if (i < 4)
            result[i] = 1;
        return result;
    }

    /// Set all elements with passed arguments.
    GfVec4d &Set(double s0, double s1, double s2, double s3) {
        _data[0] = s0;
        _data[1] = s1;
        _data[2] = s2;
        _data[3] = s3;
        return *this;
    }

    /// Set all elements with a pointer to data.
    GfVec4d &Set(double const *a) {
        return Set(a[0], a[1], a[2], a[3]);
    }

    /// Direct data access.
    double const *data() const { return _data; }
    double *data() { return _data; }
    double const *GetArray() const { return data(); }

    /// Indexing.
    double const &operator[](size_t i) const { return _data[i]; }
    double &operator[](size_t i) { return _data[i]; }

    /// Hash.
    friend inline size_t hash_value(GfVec4d const &vec) {
        size_t h = 0;
        hboost::hash_combine(h, vec[0]);
        hboost::hash_combine(h, vec[1]);
        hboost::hash_combine(h, vec[2]);
        hboost::hash_combine(h, vec[3]);
        return h;
    }

    /// Equality comparison.
    bool operator==(GfVec4d const &other) const {
        return _data[0] == other[0] &&
               _data[1] == other[1] &&
               _data[2] == other[2] &&
               _data[3] == other[3];
    }
    bool operator!=(GfVec4d const &other) const {
        return !(*this == other);
    }

    // TODO Add inequality for other vec types...
    /// Equality comparison.
    GF_API
    bool operator==(class GfVec4f const &other) const;
    /// Equality comparison.
    GF_API
    bool operator==(class GfVec4h const &other) const;
    /// Equality comparison.
    GF_API
    bool operator==(class GfVec4i const &other) const;
    
    /// Create a vec with negated elements.
    GfVec4d operator-() const {
        return GfVec4d(-_data[0], -_data[1], -_data[2], -_data[3]);
    }

    /// Addition.
    GfVec4d &operator+=(GfVec4d const &other) {
        _data[0] += other[0];
        _data[1] += other[1];
        _data[2] += other[2];
        _data[3] += other[3];
        return *this;
    }
    friend GfVec4d operator+(GfVec4d const &l, GfVec4d const &r) {
        return GfVec4d(l) += r;
    }

    /// Subtraction.
    GfVec4d &operator-=(GfVec4d const &other) {
        _data[0] -= other[0];
        _data[1] -= other[1];
        _data[2] -= other[2];
        _data[3] -= other[3];
        return *this;
    }
    friend GfVec4d operator-(GfVec4d const &l, GfVec4d const &r) {
        return GfVec4d(l) -= r;
    }

    /// Multiplication by scalar.
    GfVec4d &operator*=(double s) {
        _data[0] *= s;
        _data[1] *= s;
        _data[2] *= s;
        _data[3] *= s;
        return *this;
    }
    GfVec4d operator*(double s) const {
        return GfVec4d(*this) *= s;
    }
    friend GfVec4d operator*(double s, GfVec4d const &v) {
        return v * s;
    }

        /// Division by scalar.
    // TODO should divide by the scalar type.
    GfVec4d &operator/=(double s) {
        // TODO This should not multiply by 1/s, it should do the division.
        // Doing the division is more numerically stable when s is close to
        // zero.
        return *this *= (1.0 / s);
    }
    GfVec4d operator/(double s) const {
        return *this * (1.0 / s);
    }
    
    /// See GfDot().
    double operator*(GfVec4d const &v) const {
        return _data[0] * v[0] + _data[1] * v[1] + _data[2] * v[2] + _data[3] * v[3];
    }

    /// Returns the projection of \p this onto \p v. That is:
    /// \code
    /// v * (*this * v)
    /// \endcode
    GfVec4d GetProjection(GfVec4d const &v) const {
        return v * (*this * v);
    }

    /// Returns the orthogonal complement of \p this->GetProjection(b).
    /// That is:
    /// \code
    ///  *this - this->GetProjection(b)
    /// \endcode
    GfVec4d GetComplement(GfVec4d const &b) const {
        return *this - this->GetProjection(b);
    }

    /// Squared length.
    double GetLengthSq() const {
        return *this * *this;
    }

    /// Length
    double GetLength() const {
        // TODO should use GfSqrt.
        return sqrt(GetLengthSq());
    }

    /// Normalizes the vector in place to unit length, returning the
    /// length before normalization. If the length of the vector is
    /// smaller than \p eps, then the vector is set to vector/\c eps.
    /// The original length of the vector is returned. See also GfNormalize().
    ///
    /// \todo This was fixed for bug 67777. This is a gcc64 optimizer bug.
    /// By tickling the code, it no longer tries to write into
    /// an illegal memory address (in the code section of memory).
    double Normalize(double eps = GF_MIN_VECTOR_LENGTH) {
        // TODO this seems suspect...  suggest dividing by length so long as
        // length is not zero.
        double length = GetLength();
        *this /= (length > eps) ? length : eps;
        return length;
    }

    GfVec4d GetNormalized(double eps = GF_MIN_VECTOR_LENGTH) const {
        GfVec4d normalized(*this);
        normalized.Normalize(eps);
        return normalized;
    }

  
private:
    double _data[4];
};

/// Output a GfVec4d.
/// \ingroup group_gf_DebuggingOutput
GF_API std::ostream& operator<<(std::ostream &, GfVec4d const &);


PXR_NAMESPACE_CLOSE_SCOPE

#include "pxr/base/gf/vec4f.h"
#include "pxr/base/gf/vec4h.h"
#include "pxr/base/gf/vec4i.h"

PXR_NAMESPACE_OPEN_SCOPE

inline
GfVec4d::GfVec4d(class GfVec4f const &other)
{
    _data[0] = other[0];
    _data[1] = other[1];
    _data[2] = other[2];
    _data[3] = other[3];
}
inline
GfVec4d::GfVec4d(class GfVec4h const &other)
{
    _data[0] = other[0];
    _data[1] = other[1];
    _data[2] = other[2];
    _data[3] = other[3];
}
inline
GfVec4d::GfVec4d(class GfVec4i const &other)
{
    _data[0] = other[0];
    _data[1] = other[1];
    _data[2] = other[2];
    _data[3] = other[3];
}

/// Returns component-wise multiplication of vectors \p v1 and \p v2.
inline GfVec4d
GfCompMult(GfVec4d const &v1, GfVec4d const &v2) {
    return GfVec4d(
        v1[0] * v2[0],
        v1[1] * v2[1],
        v1[2] * v2[2],
        v1[3] * v2[3]
        );
}

/// Returns component-wise quotient of vectors \p v1 and \p v2.
inline GfVec4d
GfCompDiv(GfVec4d const &v1, GfVec4d const &v2) {
    return GfVec4d(
        v1[0] / v2[0],
        v1[1] / v2[1],
        v1[2] / v2[2],
        v1[3] / v2[3]
        );
}

/// Returns the dot (inner) product of two vectors.
inline double
GfDot(GfVec4d const &v1, GfVec4d const &v2) {
    return v1 * v2;
}


/// Returns the geometric length of \c v.
inline double
GfGetLength(GfVec4d const &v)
{
    return v.GetLength();
}

/// Normalizes \c *v in place to unit length, returning the length before
/// normalization. If the length of \c *v is smaller than \p eps then \c *v is
/// set to \c *v/eps.  The original length of \c *v is returned.
inline double
GfNormalize(GfVec4d *v, double eps = GF_MIN_VECTOR_LENGTH)
{
    return v->Normalize();
}

/// Returns a normalized (unit-length) vector with the same direction as \p v.
/// If the length of this vector is smaller than \p eps, the vector divided by
/// \p eps is returned.
inline GfVec4d
GfGetNormalized(GfVec4d const &v, double eps = GF_MIN_VECTOR_LENGTH)
{
    return v.GetNormalized(eps);
}

/// Returns the projection of \p a onto \p b. That is:
/// \code
/// b * (a * b)
/// \endcode
inline GfVec4d
GfGetProjection(GfVec4d const &a, GfVec4d const &b)
{
    return a.GetProjection(b);
}

/// Returns the orthogonal complement of \p a.GetProjection(b). That is:
/// \code
///  a - a.GetProjection(b)
/// \endcode
inline GfVec4d
GfGetComplement(GfVec4d const &a, GfVec4d const &b)
{
    return a.GetComplement(b);
}

/// Tests for equality within a given tolerance, returning \c true if the
/// length of the difference vector is less than or equal to \p tolerance.
inline bool
GfIsClose(GfVec4d const &v1, GfVec4d const &v2, double tolerance)
{
    GfVec4d delta = v1 - v2;
    return delta.GetLengthSq() <= tolerance * tolerance;
}

 
 
PXR_NAMESPACE_CLOSE_SCOPE

#endif // GF_VEC4D_H

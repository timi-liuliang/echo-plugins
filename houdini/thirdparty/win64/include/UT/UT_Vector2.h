/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Vector2.h (C++)
 *
 *
 * COMMENTS:
 *	This class handles fpreal vectors of dimension 2.
 *
 * WARNING:
 *	This class should NOT contain any virtual methods, nor should it
 *	define more member data. The size of UT_VectorF2 must always be 
 *	8 bytes (2 floats).
 */

#pragma once

#ifndef __UT_Vector2_h__
#define __UT_Vector2_h__

#include "UT_API.h"
#include "UT_Assert.h"
#include "UT_FixedVector.h"
#include "UT_VectorTypes.h"
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Inline.h>
#include <VM/VM_SIMD.h>
#include <iosfwd>
#include <limits>

class UT_IStream;
class UT_JSONWriter;
class UT_JSONValue;
class UT_JSONParser;

// Free floating functions:

// Operators that involve a UT_Vector2 object:
template <typename T>
inline UT_Vector2T<T>	operator+(const UT_Vector2T<T> &v1, const UT_Vector2T<T> &v2);
template <typename T>
inline UT_Vector2T<T>	operator-(const UT_Vector2T<T> &v1, const UT_Vector2T<T> &v2);
template <typename T>
inline bool		operator<(const UT_Vector2T<T> &v1, const UT_Vector2T<T> &v2);
template <typename T>
inline bool		operator<=(const UT_Vector2T<T> &v1, const UT_Vector2T<T> &v2);
template <typename T>
inline bool		operator>(const UT_Vector2T<T> &v1, const UT_Vector2T<T> &v2);
template <typename T>
inline bool		operator>=(const UT_Vector2T<T> &v1, const UT_Vector2T<T> &v2);
template <typename T, typename S>
inline UT_Vector2T<T>	operator+(const UT_Vector2T<T> &v, S scalar);
template <typename T, typename S>
inline UT_Vector2T<T>	operator-(const UT_Vector2T<T> &v, S scalar);
template <typename T, typename S>
inline UT_Vector2T<T>	operator*(const UT_Vector2T<T> &v, S scalar);
template <typename T, typename S>
inline UT_Vector2T<T>	operator/(const UT_Vector2T<T> &v, S scalar);
template <typename T, typename S>
inline UT_Vector2T<T>	operator+(S scalar, const UT_Vector2T<T> &v);
template <typename T, typename S>
inline UT_Vector2T<T>	operator-(S scalar, const UT_Vector2T<T> &v);
template <typename T, typename S>
inline UT_Vector2T<T>	operator*(S scalar, const UT_Vector2T<T> &v);
template <typename T, typename S>
inline UT_Vector2T<T>	operator/(S scalar, const UT_Vector2T<T> &v);
template <typename T, typename S>
inline UT_Vector2T<T>	operator*(const UT_Vector2T<T> &v, 
				  const UT_Matrix2T<S> &mat);

/// The dot product
template <typename T>
inline T		dot      (const UT_Vector2T<T> &v1, const UT_Vector2T<T> &v2);
/// Cross product, which for 2d vectors results in a fpreal.
template <typename T>
inline T		cross	 (const UT_Vector2T<T> &v1, const UT_Vector2T<T> &v2);

/// Componentwise min and maximum
template <typename T>
inline UT_Vector2T<T>	SYSmin	(const UT_Vector2T<T> &v1, const UT_Vector2T<T> &v2);
template <typename T>
inline UT_Vector2T<T>	SYSmax	(const UT_Vector2T<T> &v1, const UT_Vector2T<T> &v2);
/// Componentwise linear interpolation
template <typename T,typename S>
inline UT_Vector2T<T> SYSlerp(const UT_Vector2T<T> &v1, const UT_Vector2T<T> &v2, S t);
template <typename T,typename S>
inline UT_Vector2T<T> SYSbilerp(const UT_Vector2T<T> &u0v0, const UT_Vector2T<T> &u1v0,
                                  const UT_Vector2T<T> &u0v1, const UT_Vector2T<T> &u1v1,
                                  S u, S v)
{ return SYSlerp(SYSlerp(u0v0, u0v1, v), SYSlerp(u1v0, u1v1, v), u); }

/// The orthogonal projection of a vector u onto a vector v
template <typename T>
inline UT_Vector2T<T>	project  (const UT_Vector2T<T> &u, const UT_Vector2T<T> &v);

/// Multiplication of a row or column vector by a matrix (ie. right vs. left
/// multiplication respectively). The operator*() declared above is an alias 
/// for rowVecMult().
// @{
//
// Notes on optimisation of matrix/vector multiplies:
// - multiply(dest, mat) functions have been deprecated in favour of
// rowVecMult/colVecMult routines, which produce temporaries. For these to
// offer comparable performance, the compiler has to optimize away the
// temporary, but most modern compilers can do this. Performance tests with
// gcc3.3 indicate that this is a realistic expectation for modern
// compilers.
// - since matrix/vector multiplies cannot be done without temporary data,
// the "primary" functions are the global matrix/vector
// rowVecMult/colVecMult routines, rather than the member functions.

template <typename T, typename S>
inline UT_Vector2T<T>	rowVecMult(const UT_Vector2T<T> &v, 
				   const UT_Matrix2T<S> &m);
template <typename T, typename S>
inline UT_Vector2T<T>	colVecMult(const UT_Matrix2T<S> &m, 
				   const UT_Vector2T<T> &v);
// @}

template <typename T>
inline T		distance2d(const UT_Vector2T<T> &v1, const UT_Vector2T<T> &v2);

/// Given a 2D position, input, and a 2D quad, (p0, p0+du, p0+du+dv+duv, p0+dv),
/// finds the 0, 1, or 2 locations in the parameter space of that quad that correspond
/// with the input position.  Only parameter locations approximately between 0 and 1
/// are accepted.  The return value is the number of accepted parameter locations.
template <typename T>
int UTinverseBilerp(const UT_Vector2T<T> &input,
    const UT_Vector2T<T> &p0,
    const UT_Vector2T<T> &du, const UT_Vector2T<T> &dv,
    const UT_Vector2T<T> &duv,
    UT_Vector2T<T> outputs[2]);

/// 2D Vector class.
template <typename T>
class UT_API UT_Vector2T : public UT_FixedVector<T,2,true>
{
public:
    typedef UT_FixedVector<T,2,true> Base;

    // These "using" statements are needed for operator= and operator*=
    // so that the ones in UT_FixedVector aren't hidden by the additional
    // ones here.
    using Base::operator=;
    using Base::operator*=;

    // These "using" statements are needed for GCC and Clang, because
    // otherwise, they ignore all members of UT_FixedVector when
    // checking validity of code in functions in this class.
    using Base::vec;
    using Base::data;
    using Base::length2;
    typedef T value_type;
    static const int tuple_size = 2;

    /// Default constructor.
    /// No data is initialized! Use it for extra speed.
    SYS_FORCE_INLINE UT_Vector2T() = default;

    SYS_FORCE_INLINE UT_Vector2T(const UT_Vector2T<T> &that) = default;
    SYS_FORCE_INLINE UT_Vector2T(UT_Vector2T<T> &&that) = default;

    explicit SYS_FORCE_INLINE UT_Vector2T(T v)
    {
        vec[0] = v;
        vec[1] = v;
    }

    SYS_FORCE_INLINE UT_Vector2T(T vx, T vy)
    {
        vec[0] = vx;
        vec[1] = vy;
    }
    explicit SYS_FORCE_INLINE UT_Vector2T(const fpreal16 v[tuple_size])
        : Base(v)
    {}
    explicit SYS_FORCE_INLINE UT_Vector2T(const fpreal32 v[tuple_size])
        : Base(v)
    {}
    explicit SYS_FORCE_INLINE UT_Vector2T(const fpreal64 v[tuple_size])
        : Base(v)
    {}
    explicit SYS_FORCE_INLINE UT_Vector2T(const int32 v[tuple_size])
        : Base(v)
    {}
    explicit SYS_FORCE_INLINE UT_Vector2T(const int64 v[tuple_size])
        : Base(v)
    {}
    explicit UT_Vector2T(const UT_Vector3T<T> &v);
    explicit UT_Vector2T(const UT_Vector4T<T> &v);

    /// Our own type of any given value_type.
    template <typename S>
    SYS_FORCE_INLINE UT_Vector2T(const UT_Vector2T<S> &v)
        : Base(v)
    {}

    /// Arbitrary UT_FixedVector of the same size
    template <typename S,bool S_INSTANTIATED>
    SYS_FORCE_INLINE UT_Vector2T(const UT_FixedVector<S,tuple_size,S_INSTANTIATED> &v)
        : Base(v)
    {}

    SYS_FORCE_INLINE UT_Vector2T<T> &operator=(const UT_Vector2T<T> &that) = default;
    SYS_FORCE_INLINE UT_Vector2T<T> &operator=(UT_Vector2T<T> &&that) = default;

    template <typename S>
    SYS_FORCE_INLINE UT_Vector2T<T> &operator=(const UT_Vector2T<S> &v)
    { vec[0] = v.x(); vec[1] = v.y();  return *this; }

    // TODO: remove these. They should require an explicit UT_Vector2()
    // construction, since they're unsafe.

    /// Assignment operator that truncates a V3 to a V2.
    UT_Vector2T<T>	&operator=(const UT_Vector3T<T> &v);
    /// Assignment operator that truncates a V4 to a V2.
    UT_Vector2T<T>	&operator=(const UT_Vector4T<T> &v);

    /// It's unclear why this is needed, given UT_FixedVector::operator-(),
    /// but the compiler seems not to accept not having it.
    SYS_FORCE_INLINE  UT_Vector2T<T> operator-() const
    {
        return UT_Vector2T<T>(-vec[0], -vec[1]);
    }
    void	multiplyComponents(const UT_Vector2T<T> &v)
		{
		    vec[0] *= v.vec[0];
		    vec[1] *= v.vec[1];
		}

    /// Given an oriented line from e1 passing through e2, determine on which
    /// side of the line the point p lies.  Alternatively, determine in which
    /// half plane, positive or negative, the point lies.  If the segment
    /// degenerates to a point, then the point is always on it.
    // (Moret and Shapiro 1991)
    T      whichSide(const UT_Vector2T<T> &e1, const UT_Vector2T<T> &e2) const
                { 
		    return (vec[0] - e1.vec[0]) * (e2.vec[1] - e1.vec[1]) -
			   (vec[1] - e1.vec[1]) * (e2.vec[0] - e1.vec[0]);
		}

    template <typename S>
    inline UT_Vector2T<T> &operator*=(const UT_Matrix2T<S> &mat)
		 { return operator=(*this * mat); }

    /// These allow you to find out what indices to use for different axes
    // @{
    int			findMinAbsAxis() const
			{
			    if (SYSabs(x()) < SYSabs(y()))
				return 0;
			    else
				return 1;
			}
    int			findMaxAbsAxis() const
			{
			    if (SYSabs(x()) >= SYSabs(y()))
				return 0;
			    else
				return 1;
			}
    // @}

    /// Calculates the orthogonal projection of a vector u on the *this vector
    UT_Vector2T<T>		project(const UT_Vector2T<T> &u) const;


    /// Vector p (representing a point in 2-space) and vector v define
    /// a line. This member returns the projection of "this" onto the
    /// line (the point on the line that is closest to this point).
    UT_Vector2T<T>		projection(const UT_Vector2T<T> &p, const UT_Vector2T<T> &v) const;

    /// Compute (homogeneous) barycentric co-ordinates of this point
    /// relative to the triangle defined by t0, t1 and t2. (The point is
    /// projected into the triangle's plane.)
    UT_Vector3T<T> getBary(const UT_Vector2T<T> &t0, const UT_Vector2T<T> &t1,
			const UT_Vector2T<T> &t2, bool *degen = NULL) const;


    /// Return the components of the vector. The () operator does NOT check
    /// for the boundary condition.
    // @{
    T		&x(void) 		{ return vec[0]; } 
    T		 x(void) const		{ return vec[0]; } 
    T		&y(void) 		{ return vec[1]; } 
    T		 y(void) const		{ return vec[1]; } 
    T		&operator()(unsigned i)
		 {
		     UT_ASSERT_P(i < tuple_size);
		     return vec[i];
		 }
    T		 operator()(unsigned i) const
		 {
		     UT_ASSERT_P(i < tuple_size);
		     return vec[i];
		 }
    // @}

    /// Compute a hash
    unsigned	hash() const	{ return SYSvector_hash(data(), tuple_size); }

    // TODO: eliminate these methods. They're redundant, given good inline
    // constructors.
    /// Set the values of the vector components
    void         assign(T xx = 0.0f, T yy = 0.0f)
		 {
		    vec[0] = xx; vec[1] = yy;
		 }
    /// Set the values of the vector components
    void	 assign(const T *v) {vec[0]=v[0]; vec[1]=v[1];}

    /// Express the point in homogeneous coordinates or vice-versa
    // @{
    void	 homogenize  (void)	{ vec[0] *= vec[1]; }
    void	 dehomogenize(void)	{ if (vec[1] != 0) vec[0] /= vec[1]; }
    // @}

    /// Protected I/O methods
    // @{
    void	save(std::ostream &os, int binary = 0) const;
    bool	load(UT_IStream &is);
    // @}

    /// @{
    /// Methods to serialize to a JSON stream.  The vector is stored as an
    /// array of 2 reals.
    bool		save(UT_JSONWriter &w) const;
    bool		save(UT_JSONValue &v) const;
    bool		load(UT_JSONParser &p);
    /// @}

    /// Returns the vector size
    static int		entries()	{ return tuple_size; }

private:
    /// I/O friends
    // @{
    friend std::ostream	&operator<<(std::ostream &os, const UT_Vector2T<T> &v)
			{
			    v.save(os);
			    return os;
			}
    // @}
};

#include "UT_Matrix2.h"

// Free floating functions:
template <typename T>
inline UT_Vector2T<T>	operator+(const UT_Vector2T<T> &v1, const UT_Vector2T<T> &v2)
{
    return UT_Vector2T<T>(v1.x()+v2.x(), v1.y()+v2.y());
}
template <typename T>
inline UT_Vector2T<T>	operator-(const UT_Vector2T<T> &v1, const UT_Vector2T<T> &v2)
{
    return UT_Vector2T<T>(v1.x()-v2.x(), v1.y()-v2.y());
}
template <typename T>
inline UT_Vector2T<T>	operator*(const UT_Vector2T<T> &v1, const UT_Vector2T<T> &v2)
{
    return UT_Vector2T<T>(v1.x()*v2.x(), v1.y()*v2.y());
}
template <typename T>
inline UT_Vector2T<T>	operator/(const UT_Vector2T<T> &v1, const UT_Vector2T<T> &v2)
{
    return UT_Vector2T<T>(v1.x()/v2.x(), v1.y()/v2.y());
}
template <typename T>
inline bool		operator<(const UT_Vector2T<T> &v1, const UT_Vector2T<T> &v2)
{
    return ((v1.x() < v2.x()) || (v1.x() == v2.x() && v1.y() < v2.y()));
}
template <typename T>
inline bool		operator<=(const UT_Vector2T<T> &v1, const UT_Vector2T<T> &v2)
{
    return (v1 < v2) || (v1 == v2);
}
template <typename T>
inline bool		operator>(const UT_Vector2T<T> &v1, const UT_Vector2T<T> &v2)
{
    return v2 < v1;
}
template <typename T>
inline bool		operator>=(const UT_Vector2T<T> &v1, const UT_Vector2T<T> &v2)
{
    return v2 <= v1;
}
template <typename T, typename S>
inline UT_Vector2T<T>	operator+(const UT_Vector2T<T> &v, S scalar)
{
    return UT_Vector2T<T>(v.x()+scalar, v.y()+scalar);
}
template <typename T, typename S>
inline UT_Vector2T<T>	operator+(S scalar, const UT_Vector2T<T> &v)
{
    return UT_Vector2T<T>(v.x()+scalar, v.y()+scalar);
}
template <typename T, typename S>
inline UT_Vector2T<T>	operator-(const UT_Vector2T<T> &v, S scalar)
{
    return UT_Vector2T<T>(v.x()-scalar, v.y()-scalar);
}
template <typename T, typename S>
inline UT_Vector2T<T>	operator-(S scalar, const UT_Vector2T<T> &v)
{
    return UT_Vector2T<T>(scalar-v.x(), scalar-v.y());
}
template <typename T, typename S>
inline UT_Vector2T<T>	operator*(const UT_Vector2T<T> &v, S scalar)
{
    return UT_Vector2T<T>(v.x()*scalar, v.y()*scalar);
}
template <typename T, typename S>
inline UT_Vector2T<T>	operator*(S scalar, const UT_Vector2T<T> &v)
{
    return UT_Vector2T<T>(v.x()*scalar, v.y()*scalar);
}
template <typename T, typename S>
inline UT_Vector2T<T>	operator/(const UT_Vector2T<T> &v, S scalar)
{
    return UT_Vector2T<T>(v.x()/scalar, v.y()/scalar);
}
template <typename T, typename S>
inline UT_Vector2T<T>	operator/(S scalar, const UT_Vector2T<T> &v)
{
    return UT_Vector2T<T>(scalar/v.x(), scalar/v.y());
}
template <typename T>
inline T		dot(const UT_Vector2T<T> &v1, const UT_Vector2T<T> &v2)
{
    return v1.x()*v2.x() + v1.y()*v2.y();
}
template <typename T>
inline T		cross(const UT_Vector2T<T> &v1, const UT_Vector2T<T> &v2)
{
    return v1.x() * v2.y() - v1.y() * v2.x();
}
template <typename T>
inline
UT_Vector2T<T>	SYSmin(const UT_Vector2T<T> &v1, const UT_Vector2T<T> &v2)
{
    return UT_Vector2T<T>(
	    SYSmin(v1.x(), v2.x()),
	    SYSmin(v1.y(), v2.y())
    );
}

template <typename T>
inline
UT_Vector2T<T>	SYSmax(const UT_Vector2T<T> &v1, const UT_Vector2T<T> &v2)
{
    return UT_Vector2T<T>(
	    SYSmax(v1.x(), v2.x()),
	    SYSmax(v1.y(), v2.y())
    );
}

template <typename T,typename S>
inline
UT_Vector2T<T> SYSlerp(const UT_Vector2T<T> &v1, const UT_Vector2T<T> &v2, S t)
{
    return UT_Vector2T<T>(
	    SYSlerp(v1.x(), v2.x(), t),
	    SYSlerp(v1.y(), v2.y(), t));
}

template <typename T>
inline UT_Vector2T<T>	project(const UT_Vector2T<T> &u, const UT_Vector2T<T> &v)
{
    return dot(u, v) / v.length2() * v;
}
template <typename T, typename S>
inline UT_Vector2T<T>	rowVecMult(const UT_Vector2T<T> &v, const UT_Matrix2T<S> &m)
{
    return UT_Vector2T<T>(v.x()*m(0,0) + v.y()*m(1,0),
                          v.x()*m(0,1) + v.y()*m(1,1));
}
template <>
inline UT_Vector2T<float> rowVecMult(const UT_Vector2T<float> &v, const UT_Matrix2T<float> &m)
{
    const v4uf l(v.x(), v.x(), v.y(), v.y());
    const v4uf r(m.data());
    const v4uf p = l * r;

    return UT_Vector2T<float>(p[0] + p[2], p[1] + p[3]);
}
template <typename T, typename S>
inline UT_Vector2T<T>	colVecMult(const UT_Matrix2T<S> &m, const UT_Vector2T<T> &v)
{
    return UT_Vector2T<T>(m(0,0)*v.x() + m(0,1)*v.y(),
                          m(1,0)*v.x() + m(1,1)*v.y());
}
template <>
inline UT_Vector2T<float> colVecMult(const UT_Matrix2T<float> &m, const UT_Vector2T<float> &v)
{
    const v4uf l(m.data());
    const v4uf r(v.x(), v.y(), v.x(), v.y());
    const v4uf p = l * r;
    return UT_Vector2T<float>(p[0] + p[1], p[2] + p[3]);
}
template <typename T, typename S>
inline UT_Vector2T<T>	operator*(const UT_Vector2T<T> &v, const UT_Matrix2T<S> &m)
{
    return rowVecMult(v, m);
}
template <typename T>
inline T		distance2d(const UT_Vector2T<T> &v1, const UT_Vector2T<T> &v2)
{
    return (v1 - v2).length();
}

template <typename T>
inline size_t hash_value(const UT_Vector2T<T> &val)
{
    return val.hash();
}

// Overload for custom formatting of UT_Vector2T<T> with UTformat.
template<typename T>
UT_API size_t format(char *buffer, size_t buffer_size, const UT_Vector2T<T> &v);


template<typename T>
struct UT_FixedVectorTraits<UT_Vector2T<T> >
{
    typedef UT_FixedVector<T,2> FixedVectorType;
    typedef T DataType;
    static const exint TupleSize = 2;
    static const bool isVectorType = true;
};

#endif

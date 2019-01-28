/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Utility Library (C++)
 *
 * COMMENTS:
 *	This class handles fpreal vectors of dimension 4.
 *
 * WARNING:
 *	This class should NOT contain any virtual methods, nor should it
 *	define more member data. The size of UT_Vector4 must always be 
 *	16 bytes (4 floats).
 *
 */

#pragma once

#ifndef __UT_Vector4_h__
#define __UT_Vector4_h__

#include "UT_API.h"
#include "UT_Assert.h"
#include "UT_FixedVector.h"
#include "UT_VectorTypes.h"
#include <SYS/SYS_Deprecated.h>
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Math.h>
#include <iosfwd>
#include <limits>

#ifndef UT_DISABLE_VECTORIZE_MATRIX
#include <VM/VM_SIMD.h>
#endif

class UT_IStream;
class UT_JSONWriter;
class UT_JSONValue;
class UT_JSONParser;

// Free floating functions:

// Right-multiply operators (M*v) have been removed. They had previously
// been defined to return v*M, which was too counterintuitive. Once HDK
// etc. users have a chance to update their code (post 7.0) we could
// reintroduce a right-multiply operator that does a colVecMult.

template <typename T, typename S>
inline UT_Vector4T<T>	operator*(const UT_Vector4T<T> &v, const UT_Matrix4T<S> &m);
template <typename T>
inline UT_Vector4T<T>	operator+(const UT_Vector4T<T> &v1, const UT_Vector4T<T> &v2);
template <typename T>
inline UT_Vector4T<T>	operator-(const UT_Vector4T<T> &v1, const UT_Vector4T<T> &v2);
template <typename T, typename S>
inline UT_Vector4T<T>	operator+(const UT_Vector4T<T> &v, S scalar);
template <typename T, typename S>
inline UT_Vector4T<T>	operator-(const UT_Vector4T<T> &v, S scalar);
template <typename T, typename S>
inline UT_Vector4T<T>	operator*(const UT_Vector4T<T> &v, S scalar);
template <typename T, typename S>
inline UT_Vector4T<T>	operator/(const UT_Vector4T<T> &v, S scalar);
template <typename T, typename S>
inline UT_Vector4T<T>	operator+(S scalar, const UT_Vector4T<T> &v);
template <typename T, typename S>
inline UT_Vector4T<T>	operator-(S scalar, const UT_Vector4T<T> &v);
template <typename T, typename S>
inline UT_Vector4T<T>	operator*(S scalar, const UT_Vector4T<T> &v);
template <typename T, typename S>
inline UT_Vector4T<T>	operator/(S scalar, const UT_Vector4T<T> &v);

/// Although the cross product is undefined for 4D vectors, we believe it's
/// useful in practice to define a function that takes two 4D vectors and
/// computes the cross-product of their first 3 components
template <typename T>
UT_API UT_Vector3T<T>	cross(const UT_Vector4T<T> &v1, const UT_Vector4T<T> &v2);
template <typename T>
UT_API UT_Vector3T<T>	cross(const UT_Vector3T<T> &v1, const UT_Vector4T<T> &v2);
template <typename T>
UT_API UT_Vector3T<T>	cross(const UT_Vector4T<T> &v1, const UT_Vector3T<T> &v2);

/// The dot product between two vectors
// @{
template <typename T>
inline T		dot(const UT_Vector4T<T> &v1, const UT_Vector4T<T> &v2);
template <typename T>
inline T		dot(const UT_Vector4T<T> &v1, const UT_Vector3T<T> &v2);
template <typename T>
inline T		dot(const UT_Vector3T<T> &v1, const UT_Vector4T<T> &v2);
// @}

/// Componentwise min and maximum
template <typename T>
inline UT_Vector4T<T>	SYSmin	(const UT_Vector4T<T> &v1, const UT_Vector4T<T> &v2);
template <typename T>
inline UT_Vector4T<T>	SYSmax	(const UT_Vector4T<T> &v1, const UT_Vector4T<T> &v2);

/// Componentwise linear interpolation
template <typename T,typename S>
inline UT_Vector4T<T> SYSlerp(const UT_Vector4T<T> &v1, const UT_Vector4T<T> &v2, S t);
template <typename T,typename S>
inline UT_Vector4T<T> SYSbilerp(const UT_Vector4T<T> &u0v0, const UT_Vector4T<T> &u1v0,
                                  const UT_Vector4T<T> &u0v1, const UT_Vector4T<T> &u1v1,
                                  S u, S v)
{ return SYSlerp(SYSlerp(u0v0, u0v1, v), SYSlerp(u1v0, u1v1, v), u); }


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
// - inlining is explicitly requested only for functions involving the
// native types (UT_Vector4 and UT_Matrix4)
template <typename T, typename S>
UT_API UT_Vector4T<T>	rowVecMult(const UT_Vector4T<T> &v, const UT_Matrix4T<S> &m);
template <typename T, typename S>
UT_API UT_Vector4T<T>	colVecMult(const UT_Matrix4T<S> &m, const UT_Vector4T<T> &v);

template <typename T, typename S>
UT_API UT_Vector4T<T>	rowVecMult3(const UT_Vector4T<T> &v, const UT_Matrix4T<S> &m);
template <typename T, typename S>
UT_API UT_Vector4T<T>	colVecMult3(const UT_Matrix4T<S> &m, const UT_Vector4T<T> &v);
// @}

/// Compute the distance between two points
// @{
template <typename T>
inline T		distance4(const UT_Vector4T<T> &p1, const UT_Vector4T<T> &p2);
template <typename T>
inline T		distance3(const UT_Vector4T<T> &p1, const UT_Vector4T<T> &p2);
template <typename T>
inline T		distance3d(const UT_Vector4T<T> &p1, const UT_Vector4T<T> &p2)
{ return distance3(p1, p2); }
template <typename T>
inline T		distance3d(const UT_Vector3T<T> &p1, const UT_Vector4T<T> &p2)
{ return distance3d(p1, UT_Vector3T<T>(p2)); }
template <typename T>
inline T		distance3d(const UT_Vector4T<T> &p1, const UT_Vector3T<T> &p2)
{ return distance3d(UT_Vector3T<T>(p1), p2); }
// @}

/// 4D Vector class.
template <typename T>
class UT_API UT_Vector4T : public UT_FixedVector<T,4,true>
{
public:
    typedef UT_FixedVector<T,4,true> Base;

    // These "using" statements are needed for operator= and operator*=
    // so that the ones in UT_FixedVector aren't hidden by the additional
    // ones here.
    using Base::operator=;
    using Base::operator*=;

    // These "using" statements are needed for GCC and Clang, because
    // otherwise, they ignore all members of UT_FixedVector when
    // checking validity of code in functions in this class.
private:
    using Base::vec;
public:
    using Base::data;
    typedef T value_type;
    static const int tuple_size = 4;

    /// Default constructor.
    /// No data is initialized! Use it for extra speed.
    SYS_FORCE_INLINE UT_Vector4T() = default;

    SYS_FORCE_INLINE UT_Vector4T(const UT_Vector4T<T> &that) = default;
    SYS_FORCE_INLINE UT_Vector4T(UT_Vector4T<T> &&that) = default;

    SYS_FORCE_INLINE UT_Vector4T(T vx, T vy, T vz, T vw = 1.0f)
    {
        vec[0] = vx;
        vec[1] = vy;
        vec[2] = vz;
        vec[3] = vw;
    }
    SYS_FORCE_INLINE UT_Vector4T(const fpreal32 v[tuple_size])
        : Base(v)
    {}
    SYS_FORCE_INLINE UT_Vector4T(const fpreal64 v[tuple_size])
        : Base(v)
    {}
    SYS_FORCE_INLINE UT_Vector4T(const int32 v[tuple_size])
        : Base(v)
    {}
    SYS_FORCE_INLINE UT_Vector4T(const int64 v[tuple_size])
        : Base(v)
    {}
    
    // Initialises the vector as [x,y,0,1]
    SYS_FORCE_INLINE
    explicit UT_Vector4T(const UT_Vector2T<T> &v);
    SYS_FORCE_INLINE
    explicit UT_Vector4T(const UT_Vector3T<T> &v, T w = 1.f);

    /// Our own type of any given value_type.
    template <typename S>
    SYS_FORCE_INLINE UT_Vector4T(const UT_Vector4T<S> &v)
        : Base(v)
    {}

    /// Arbitrary UT_FixedVector of the same size
    template <typename S,bool S_INSTANTIATED>
    SYS_FORCE_INLINE UT_Vector4T(const UT_FixedVector<S,tuple_size,S_INSTANTIATED> &v)
        : Base(v)
    {}

    SYS_FORCE_INLINE UT_Vector4T<T> &operator=(const UT_Vector4T<T> &that) = default;
    SYS_FORCE_INLINE UT_Vector4T<T> &operator=(UT_Vector4T<T> &&that) = default;

    template <typename S>
    SYS_FORCE_INLINE UT_Vector4T<T> &operator=(const UT_Vector4T<S> &v)
    { vec[0] = v[0]; vec[1] = v[1]; vec[2] = v[2]; vec[3] = v[3];
      return *this; }


    // TODO: We could remove this. It's not as error-prone as some other
    // conversions, but it might still be better to force the user to do
    // an explicit cast i.e., v4 = UT_Vector4(v3)

    /// Assignment operator that creates a V4 from a V3 by adding a '1' 
    /// element.
    SYS_DEPRECATED_HDK_REPLACE(16.0,explicit UT_Vector4 constructor to avoid implicit conversion from UT_Vector3)
    UT_Vector4T<T>	&operator=(const UT_Vector3T<T> &v);

    int		 equalZero3(T tol = 0.00001f) const
		 {
    		    return (vec[0] >= -tol && vec[0] <= tol) &&
    		    	   (vec[1] >= -tol && vec[1] <= tol) &&
    		    	   (vec[2] >= -tol && vec[2] <= tol);
		 }

    void         clampZero(T tol = 0.00001f) 
		 {
		    if (vec[0] >= -tol && vec[0] <= tol) vec[0] = 0;
		    if (vec[1] >= -tol && vec[1] <= tol) vec[1] = 0;
		    if (vec[2] >= -tol && vec[2] <= tol) vec[2] = 0;
		    if (vec[3] >= -tol && vec[3] <= tol) vec[3] = 0;
		 }

    void         clampZero3(T tol = 0.00001f) 
		 {
		    if (vec[0] >= -tol && vec[0] <= tol) vec[0] = 0;
		    if (vec[1] >= -tol && vec[1] <= tol) vec[1] = 0;
		    if (vec[2] >= -tol && vec[2] <= tol) vec[2] = 0;
		 }

    void	 negate3()
		 { vec[0]= -vec[0]; vec[1]= -vec[1]; vec[2]= -vec[2]; }

    void	 multiplyComponents(const UT_Vector4T<T> &v)
		 {
		     vec[0] *= v.vec[0];
		     vec[1] *= v.vec[1];
		     vec[2] *= v.vec[2];
		     vec[3] *= v.vec[3];
		 }

    using Base::isEqual;

    SYS_DEPRECATED_HDK_REPLACE(16.0,explicit conversion to UT_Vector3 followed by isEqual)
    inline int		 isEqual(const UT_Vector3T<T> &vect, T tol = 0.00001f) const;

    /// If you need a multiplication operator that left multiplies the vector
    /// by a matrix (M * v), use the following colVecMult() functions. If
    /// you'd rather not use operator*=() for right-multiplications (v * M),
    /// use the following rowVecMult() functions.
    // @{
    template <typename S>
    inline void	 rowVecMult(const UT_Matrix4T<S> &m)
		 { operator=(::rowVecMult(*this, m)); }
    template <typename S>
    inline void	 colVecMult(const UT_Matrix4T<S> &m)
		 { operator=(::colVecMult(m, *this)); }
    // @}

    /// This multiply will ignore the 4th component both in the vector an in
    /// the matrix. This helps when you want to avoid affecting the 'w'
    /// component. This in turns annihilates the translation components (row 4)
    /// in mat, so be careful.
    // @{
    template <typename S>
    void	  rowVecMult3(const UT_Matrix4T<S> &m)
		  { operator=(::rowVecMult3(*this, m)); }
    // @}

    // The *= and multiply3 routines are provided for
    // legacy reasons. They all assume that *this is a row vector. Generally,
    // the rowVecMult and colVecMult methods are preferred, since they're
    // more explicit about the row vector assumption.
    // @{
    template <typename S>
    inline
    UT_Vector4T<T>	&operator*=(const UT_Matrix4T<S> &mat)
		 { rowVecMult(mat); return *this; }

    template <typename S>
    inline void	 multiply3(const UT_Matrix4T<S> &mat)
		 { rowVecMult3(mat); }
    template <typename S>
    inline void	 multiply3(UT_Vector4T<T> &dest, const UT_Matrix4T<S> &mat) const
		 { dest = ::rowVecMult3(*this, mat); }
    // @}

    /// These allow you to find out what indices to use for different axes
    // @{
    int		 findMinAbsAxis() const
		 {
		    if (SYSabs(x()) < SYSabs(y()))
			if (SYSabs(z()) < SYSabs(x()))
			    if (SYSabs(w()) < SYSabs(z()))
				return 3;
			    else
				return 2;
			else
			    if (SYSabs(w()) < SYSabs(x()))
				return 3;
			    else
				return 0;
		    else
			if (SYSabs(z()) < SYSabs(y()))
			    if (SYSabs(w()) < SYSabs(z()))
				return 3;
			    else
				return 2;
			else
			    if (SYSabs(w()) < SYSabs(y()))
				return 3;
			    else
				return 1;
		 }
    int		 findMaxAbsAxis() const
		 {
		    if (SYSabs(x()) >= SYSabs(y()))
			if (SYSabs(z()) >= SYSabs(x()))
			    if (SYSabs(w()) >= SYSabs(z()))
				return 3;
			    else
				return 2;
			else
			    if (SYSabs(w()) >= SYSabs(x()))
				return 3;
			    else
				return 0;
		    else
			if (SYSabs(z()) >= SYSabs(y()))
			    if (SYSabs(w()) >= SYSabs(z()))
				return 3;
			    else
				return 2;
			else
			    if (SYSabs(w()) >= SYSabs(y()))
				return 3;
			    else
				return 1;
		 }
    // @}

    /// Return the components of the vector. The () operator does NOT check
    /// for the boundary condition.
    // @{
    inline T	&x(void) 		{ return vec[0]; } 
    inline T	 x(void) const		{ return vec[0]; } 
    inline T	&y(void) 		{ return vec[1]; } 
    inline T	 y(void) const		{ return vec[1]; } 
    inline T	&z(void) 		{ return vec[2]; } 
    inline T	 z(void) const		{ return vec[2]; } 
    inline T	&w(void) 		{ return vec[3]; } 
    inline T	 w(void) const		{ return vec[3]; } 
    inline T	&operator()(unsigned i)
		 {
		     UT_ASSERT_P(i < tuple_size);
		     return vec[i];
		 }
    inline T	 operator()(unsigned i) const
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
    void	 assign(T xx = 0.0f, T yy = 0.0f, T zz = 0.0f, 
			T ww = 1.0f)
		 {
		     vec[0] = xx; vec[1] = yy; vec[2] = zz; vec[3] = ww;
		 }
    /// Set the values of the vector components
    void	 assign(const T *v, int size = tuple_size)
		 {
		     vec[0] = v[0];
		     vec[1] = v[1];
		     vec[2] = v[2];
		     if (size == tuple_size) vec[3] = v[3];
		 }

    /// Express the point in homogeneous coordinates or vice-versa
    // @{
    void	 homogenize(void)
		 {
		     vec[0] *= vec[3];
		     vec[1] *= vec[3];
		     vec[2] *= vec[3];
		 }
    void	 dehomogenize(void)
		 {
		     if (vec[3] != 0)
		     {
			 T denom = 1.0f / vec[3];
			 vec[0] *= denom;
			 vec[1] *= denom;
			 vec[2] *= denom;
		     }
		 }
    // @}

    void	save(std::ostream &os, int binary=0) const;
    bool	load(UT_IStream &is);

    /// @{
    /// Methods to serialize to a JSON stream.  The vector is stored as an
    /// array of 4 reals.
    bool		save(UT_JSONWriter &w) const;
    bool		save(UT_JSONValue &v) const;
    bool		load(UT_JSONParser &p);
    /// @}

    /// Returns the vector size
    static int		entries()	{ return tuple_size; }

private:
    /// I/O friends
    // @{
    friend std::ostream	&operator<<(std::ostream &os, const UT_Vector4T<T> &v)
			{
			    v.save(os);
			    return os;
			}
    // @}

    /// The negate operator is not provided, because of potentially
    /// unintuitive behaviour: you very rarely actually want to negate the
    /// w component.
    UT_Vector4T<T> operator-() const
    {
        UT_ASSERT(0);
        UT_Vector4T a(*this);
        a.negate();
        return a;
    }
};

#include "UT_Vector2.h"
#include "UT_Vector3.h"

template <typename T>
inline UT_Vector4T<T>::UT_Vector4T(const UT_Vector2T<T> &v)
{
    vec[0] = v.x();
    vec[1] = v.y();
    vec[2] = T(0);
    vec[3] = T(1);
}
template <typename T>
inline UT_Vector4T<T>::UT_Vector4T(const UT_Vector3T<T> &v, T vw)
{
    vec[0] = v.x();
    vec[1] = v.y();
    vec[2] = v.z();
    vec[3] = vw;
}

#ifndef UT_DISABLE_VECTORIZE_MATRIX
template <> inline void
UT_Vector4T<float>::multiplyComponents(const UT_Vector4T<float>& v)
{
    v4uf l(this->data());
    const v4uf r(v.data());
    l *= r;

    vm_store(this->data(), l.vector);
}
#endif

template <typename T>
inline int
UT_Vector4T<T>::isEqual(const UT_Vector3T<T> &vect, T tol) const
{
    return ((vec[0]>=vect.x()-tol) && (vec[0]<=vect.x()+tol) &&
	    (vec[1]>=vect.y()-tol) && (vec[1]<=vect.y()+tol) &&
	    (vec[2]>=vect.z()-tol) && (vec[2]<=vect.z()+tol));
}

// Free floating functions:
template <typename T>
inline UT_Vector4T<T>	operator+(const UT_Vector4T<T> &v1, const UT_Vector4T<T> &v2)
{
    return UT_Vector4T<T>(v1.x()+v2.x(), v1.y()+v2.y(),
		      v1.z()+v2.z(), v1.w()+v2.w());
}
template <typename T>
inline UT_Vector3T<T>	operator+(const UT_Vector4T<T> &v1, const UT_Vector3T<T> &v2)
{
    return UT_Vector3T<T>(v1.x()+v2.x(), v1.y()+v2.y(),
		      v1.z()+v2.z());
}
template <typename T>
inline UT_Vector3T<T>	operator+(const UT_Vector3T<T> &v1, const UT_Vector4T<T> &v2)
{
    return UT_Vector3T<T>(v1.x()+v2.x(), v1.y()+v2.y(),
		      v1.z()+v2.z());
}
template <typename T>
inline UT_Vector4T<T>	operator-(const UT_Vector4T<T> &v1, const UT_Vector4T<T> &v2)
{
    return UT_Vector4T<T>(v1.x()-v2.x(), v1.y()-v2.y(),
		      v1.z()-v2.z(), v1.w()-v2.w());
}
template <typename T>
inline UT_Vector3T<T>	operator-(const UT_Vector4T<T> &v1, const UT_Vector3T<T> &v2)
{
    return UT_Vector3T<T>(v1.x()-v2.x(), v1.y()-v2.y(),
		      v1.z()-v2.z());
}
template <typename T>
inline UT_Vector3T<T>	operator-(const UT_Vector3T<T> &v1, const UT_Vector4T<T> &v2)
{
    return UT_Vector3T<T>(v1.x()-v2.x(), v1.y()-v2.y(),
		      v1.z()-v2.z());
}
template <typename T>
inline UT_Vector4T<T>	operator*(const UT_Vector4T<T> &v1, const UT_Vector4T<T> &v2)
{
    return UT_Vector4T<T>(v1.x()*v2.x(), v1.y()*v2.y(), v1.z()*v2.z(), v1.w()*v2.w());
}

template <typename T>
inline UT_Vector4T<T>	operator/(const UT_Vector4T<T> &v1, const UT_Vector4T<T> &v2)
{
    return UT_Vector4T<T>(v1.x()/v2.x(), v1.y()/v2.y(), v1.z()/v2.z(), v1.w()/v2.w());
}
#ifndef UT_DISABLE_VECTORIZE_MATRIX
template <>
inline UT_Vector4T<float> operator*(const UT_Vector4T<float> &v1, const UT_Vector4T<float> &v2)
{
    const v4uf l(v1.data());
    const v4uf r(v2.data());
    const v4uf result = l * r;
    return UT_Vector4T<float>((float*) &result);
}
template <>
inline UT_Vector4T<float> operator/(const UT_Vector4T<float> &v1, const UT_Vector4T<float> &v2)
{
    const v4uf l(v1.data());
    const v4uf r(v2.data());
    const v4uf result = l / r;
    return UT_Vector4T<float>((float*) &result);
}
#endif

template <typename T, typename S>
inline UT_Vector4T<T>	operator+(const UT_Vector4T<T> &v, S scalar)
{
    return UT_Vector4T<T>(v.x()+scalar, v.y()+scalar, v.z()+scalar, v.w()+scalar);
}
template <typename T, typename S>
inline UT_Vector4T<T>	operator+(S scalar, const UT_Vector4T<T> &v)
{
    return UT_Vector4T<T>(v.x()+scalar, v.y()+scalar, v.z()+scalar, v.w()+scalar);
}
template <typename T, typename S>
inline UT_Vector4T<T>	operator-(const UT_Vector4T<T> &v, S scalar)
{
    return UT_Vector4T<T>(v.x()-scalar, v.y()-scalar, v.z()-scalar, v.w()-scalar);
}
template <typename T, typename S>
inline UT_Vector4T<T>	operator-(S scalar, const UT_Vector4T<T> &v)
{
    return UT_Vector4T<T>(scalar-v.x(), scalar-v.y(), scalar-v.z(), v.w()-scalar);
}
template <typename T, typename S>
inline UT_Vector4T<T>	operator*(const UT_Vector4T<T> &v, S scalar)
{
    return UT_Vector4T<T>(v.x()*scalar, v.y()*scalar, v.z()*scalar, v.w()*scalar);
}
template <typename T, typename S>
inline UT_Vector4T<T>	operator*(S scalar, const UT_Vector4T<T> &v)
{
    return UT_Vector4T<T>(v.x()*scalar, v.y()*scalar, v.z()*scalar, v.w()*scalar);
}
template <typename T, typename S>
inline UT_Vector4T<T>	operator/(const UT_Vector4T<T> &v, S scalar)
{
    // This has to be T because S may be int for "v = v/2" code
    // For the same reason we must cast the 1
    T		inv = ((T)1) / scalar;
    return UT_Vector4T<T>(v.x()*inv, v.y()*inv, v.z()*inv, v.w()*inv);
}
template <typename T, typename S>
inline UT_Vector4T<T>	operator/(S scalar, const UT_Vector4T<T> &v)
{
    return UT_Vector4T<T>(scalar/v.x(), scalar/v.y(), scalar/v.z(), scalar/v.w());
}
template <typename T>
inline T		dot(const UT_Vector4T<T> &v1, const UT_Vector4T<T> &v2)
{
    return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z() + v1.w()*v2.w();
}
#ifndef UT_DISABLE_VECTORIZE_MATRIX
template <>
inline float		dot(const UT_Vector4T<float> &v1, const UT_Vector4T<float> &v2)
{
    return dot4(v4uf(v1.data()), v4uf(v2.data()));
}
#endif

template <typename T>
inline T		dot(const UT_Vector4T<T> &v1, const UT_Vector3T<T> &v2)
{
    return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z();
}
template <typename T>
inline T		dot(const UT_Vector3T<T> &v1, const UT_Vector4T<T> &v2)
{
    return v1.x()*v2.x() + v1.y()*v2.y() + v1.z()*v2.z();
}
template <typename T>
inline
UT_Vector4T<T>	SYSmin(const UT_Vector4T<T> &v1, const UT_Vector4T<T> &v2)
{
    return UT_Vector4T<T>(
	    SYSmin(v1.x(), v2.x()),
	    SYSmin(v1.y(), v2.y()),
	    SYSmin(v1.z(), v2.z()),
	    SYSmin(v1.w(), v2.w())
    );
}

template <typename T>
inline
UT_Vector4T<T>	SYSmax(const UT_Vector4T<T> &v1, const UT_Vector4T<T> &v2)
{
    return UT_Vector4T<T>(
	    SYSmax(v1.x(), v2.x()),
	    SYSmax(v1.y(), v2.y()),
	    SYSmax(v1.z(), v2.z()),
	    SYSmax(v1.w(), v2.w())
    );
}

template <typename T,typename S>
inline
UT_Vector4T<T> SYSlerp(const UT_Vector4T<T> &v1, const UT_Vector4T<T> &v2, S t)
{
    return UT_Vector4T<T>(
	    SYSlerp(v1.x(), v2.x(), t),
	    SYSlerp(v1.y(), v2.y(), t),
	    SYSlerp(v1.z(), v2.z(), t),
	    SYSlerp(v1.w(), v2.w(), t));
}
#ifndef UT_DISABLE_VECTORIZE_MATRIX
template <>
inline
UT_Vector4T<float> SYSlerp(const UT_Vector4T<float> &v1, const UT_Vector4T<float> &v2, float t)
{
    const v4uf l(v1.data());
    const v4uf r(v2.data());
    const v4uf result = SYSlerp(l, r, t);
    return UT_Vector4T<float>((float*) &result);
}
#endif


template <typename T, typename S>
inline UT_Vector4T<T>	operator*(const UT_Vector4T<T> &v, const UT_Matrix4T<S> &m)
{
    return rowVecMult(v, m);
}
template <typename T>
inline T		distance(const UT_Vector4T<T> &v1, const UT_Vector4T<T> &v2)
{
    T	x = v1.x()-v2.x();
    T	y = v1.y()-v2.y();
    T	z = v1.z()-v2.z();
    T	w = v1.w()-v2.w();
    return SYSsqrt(x*x + y*y + z*z + w*w);
}
template <typename T>
inline T		distance3(const UT_Vector4T<T> &v1, const UT_Vector4T<T> &v2)
{
    T	x = v1.x()-v2.x();
    T	y = v1.y()-v2.y();
    T	z = v1.z()-v2.z();
    return SYSsqrt(x*x + y*y + z*z);
}

#ifndef UT_DISABLE_VECTORIZE_MATRIX
template <>
inline float distance(const UT_Vector4T<float> &v1, const UT_Vector4T<float> &v2)
{
    const v4uf l(v1.data());
    const v4uf r(v2.data());
    v4uf result = l - r;
    result *= result;

    return SYSsqrt(result[0] + result[1] + result[2] + result[3]);
}
template <>
inline float distance3(const UT_Vector4T<float> &v1, const UT_Vector4T<float> &v2)
{
    const v4uf l(v1.data());
    const v4uf r(v2.data());
    v4uf result = l - r;
    result *= result;

    return SYSsqrt(result[0] + result[1] + result[2]);
}
#endif

template <typename T>
inline size_t hash_value(const UT_Vector4T<T> &val)
{
    return val.hash();
}

// Overload for custom formatting of UT_Vector4T<T> with UTformat.
template<typename T>
UT_API size_t format(char *buffer, size_t buffer_size, const UT_Vector4T<T> &v);


template<typename T>
struct UT_FixedVectorTraits<UT_Vector4T<T> >
{
    typedef UT_FixedVector<T,4> FixedVectorType;
    typedef T DataType;
    static const exint TupleSize = 4;
    static const bool isVectorType = true;
};

#endif

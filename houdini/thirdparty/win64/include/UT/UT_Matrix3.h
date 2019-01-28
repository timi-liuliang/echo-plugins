/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#pragma once

#ifndef __UT_Matrix3_h__
#define __UT_Matrix3_h__

#include "UT_API.h"
#include "UT_Assert.h"
#include "UT_Axis.h"
#include "UT_FixedVector.h"
#include "UT_Swap.h"
#include "UT_SymMatrix3.h"
#include "UT_Vector3.h"
#include "UT_VectorTypes.h"

#include <SYS/SYS_Deprecated.h>
#include <SYS/SYS_Math.h>
#include <iosfwd>

class UT_IStream;
class UT_JSONParser;
class UT_JSONWriter;
class UT_JSONValue;

// Free floating operators that return a UT_Matrix3T object. 
template <typename T>
UT_API UT_Matrix3T<T>	operator+(const UT_Matrix3T<T> &m1,  const UT_Matrix3T<T> &m2);
template <typename T>
UT_API UT_Matrix3T<T>	operator-(const UT_Matrix3T<T> &m1,  const UT_Matrix3T<T> &m2);
template <typename T>
UT_API UT_Matrix3T<T>	operator*(const UT_Matrix3T<T> &m1,  const UT_Matrix3T<T> &m2);
template <typename T, typename S>
UT_API UT_Matrix3T<T>	operator+(const UT_Matrix3T<T> &m, const UT_Vector3T<S> &v);
template <typename T, typename S>
inline UT_Matrix3T<T>	operator+(const UT_Vector3T<S> &v, const UT_Matrix3T<T> &m);
template <typename T, typename S>
UT_API UT_Matrix3T<T>	operator-(const UT_Matrix3T<T> &m, const UT_Vector3T<S> &v);
template <typename T, typename S>
UT_API UT_Matrix3T<T>	operator-(const UT_Vector3T<S> &v, const UT_Matrix3T<T> &m);
template <typename T, typename S>
UT_API UT_Matrix3T<T>	operator+(const UT_Matrix3T<T> &mat, S sc);
template <typename T, typename S>
inline UT_Matrix3T<T>	operator-(const UT_Matrix3T<T> &mat, S sc);
template <typename T, typename S>
UT_API UT_Matrix3T<T>	operator*(const UT_Matrix3T<T> &mat, S sc);
template <typename T, typename S>
inline UT_Matrix3T<T>	operator/(const UT_Matrix3T<T> &mat, S sc);
template <typename T, typename S>
inline UT_Matrix3T<T>	operator+(S sc, const UT_Matrix3T<T> &mat);
template <typename T, typename S>
UT_API UT_Matrix3T<T>	operator-(S sc, const UT_Matrix3T<T> &mat);
template <typename T, typename S>
inline UT_Matrix3T<T>	operator*(S sc, const UT_Matrix3T<T> &mat);
template <typename T, typename S>
UT_API UT_Matrix3T<T>	operator/(S sc, const UT_Matrix3T<T> &mat);

template <typename T>
inline UT_Matrix3T<T>	SYSmin	(const UT_Matrix3T<T> &v1, const UT_Matrix3T<T> &v2);
template <typename T>
inline UT_Matrix3T<T>	SYSmax	(const UT_Matrix3T<T> &v1, const UT_Matrix3T<T> &v2);
template <typename T,typename S>
inline UT_Matrix3T<T> SYSlerp(const UT_Matrix3T<T> &v1, const UT_Matrix3T<T> &v2, S t);
template <typename T,typename S>
inline UT_Matrix3T<T> SYSbilerp(const UT_Matrix3T<T> &u0v0, const UT_Matrix3T<T> &u1v0,
                                  const UT_Matrix3T<T> &u0v1, const UT_Matrix3T<T> &u1v1,
                                  S u, S v)
{ return SYSlerp(SYSlerp(u0v0, u0v1, v), SYSlerp(u1v0, u1v1, v), u); }

/// This class implements a 3x3 float matrix in row-major order.
///
/// Most of Houdini operates with row vectors that are left-multiplied with
/// matrices. e.g.,   z = v * M
///
/// This convention, combined with row-major order, is directly compatible
/// with OpenGL matrix requirements.
template <typename T>
class UT_API UT_Matrix3T
{
public:

    typedef T value_type;
    static const int tuple_size = 9;

    /// Construct uninitialized matrix.
    SYS_FORCE_INLINE UT_Matrix3T() = default;

    /// Default copy constructor
    constexpr UT_Matrix3T(const UT_Matrix3T &) = default;

    /// Default move constructor
    constexpr UT_Matrix3T(UT_Matrix3T &&) = default;

    /// Construct identity matrix, multipled by scalar.
    explicit constexpr UT_Matrix3T(fpreal64 val) noexcept
        : matx{{T(val),0,0},{0,T(val),0},{0,0,T(val)}}
    {
        SYS_STATIC_ASSERT(sizeof(UT_Matrix3T<T>) == tuple_size * sizeof(T));
    }
    /// Construct a deep copy of the input row-major data.
    /// @{
    template <typename S>
    explicit constexpr UT_Matrix3T(const S m[3][3]) noexcept
        : matx{
            {T(m[0][0]),T(m[0][1]),T(m[0][2])},
            {T(m[1][0]),T(m[1][1]),T(m[1][2])},
            {T(m[2][0]),T(m[2][1]),T(m[2][2])}}
    {}
    /// @}

    /// Arbitrary UT_FixedVector of the same size
    template <typename S,bool INST>
    SYS_FORCE_INLINE explicit UT_Matrix3T(const UT_FixedVector<S, tuple_size, INST> &v) noexcept
        : matx{
            {v[0],v[1],v[2]},
            {v[3],v[4],v[5]},
            {v[6],v[7],v[8]}}
    {}

    /// Construct from 3 row vectors
    template <typename S,bool INST>
    SYS_FORCE_INLINE UT_Matrix3T(
        const UT_FixedVector<S,3,INST> &r0,
        const UT_FixedVector<S,3,INST> &r1,
        const UT_FixedVector<S,3,INST> &r2) noexcept
        : matx{
            {r0[0],r0[1],r0[2]},
            {r1[0],r1[1],r1[2]},
            {r2[0],r2[1],r2[2]}}
    {}

    /// This constructor is for convenience.
    constexpr UT_Matrix3T(
        T val00, T val01, T val02,
        T val10, T val11, T val12,
        T val20, T val21, T val22) noexcept
        : matx{
            {val00,val01,val02},
            {val10,val11,val12},
            {val20,val21,val22}}
    {}

    /// Base type conversion constructor
    template <typename S>
    UT_Matrix3T(const UT_Matrix3T<S> &m) noexcept
        : matx{
            {T(m(0,0)),T(m(0,1)),T(m(0,2))},
            {T(m(1,0)),T(m(1,1)),T(m(1,2))},
            {T(m(2,0)),T(m(2,1)),T(m(2,2))}}
    {}
    explicit UT_Matrix3T(const UT_Matrix4T<fpreal32> &m);
    explicit UT_Matrix3T(const UT_Matrix4T<fpreal64> &m);

    /// Construct from a symmetric 3x3 matrix
    template <typename S>
    explicit UT_Matrix3T(const UT_SymMatrix3T<S> m)
    {
	this->operator=(m);
    }

    /// Default copy assignment operator
    UT_Matrix3T<T> &operator=(const UT_Matrix3T<T> &m) = default;

    /// Default move assignment operator
    UT_Matrix3T<T> &operator=(UT_Matrix3T<T> &&m) = default;

    /// Conversion operator that returns a 3x3 from a 4x4 matrix by ignoring the
    /// last row and last column.
    /// @{
    UT_Matrix3T<T>	&operator=(const UT_Matrix4T<fpreal32> &m);
    UT_Matrix3T<T>	&operator=(const UT_Matrix4T<fpreal64> &m);
    /// @}
    template <typename S>
    UT_Matrix3T<T> &operator=(const UT_Matrix3T<S> &m) noexcept
    {
	matx[0][0]=m(0,0); matx[0][1]=m(0,1); matx[0][2]=m(0,2);
	matx[1][0]=m(1,0); matx[1][1]=m(1,1); matx[1][2]=m(1,2);
	matx[2][0]=m(2,0); matx[2][1]=m(2,1); matx[2][2]=m(2,2);
	return *this;
    }

    /// Convert from a symmetric matrix to non-symmetric.
    template <typename S>
    UT_Matrix3T<T>	&operator=(const UT_SymMatrix3T<S> &m)
    {
	typedef typename UT_SymMatrix3T<S>::LowerTri LowerTri;
	const LowerTri &l = m.lowerTri();
	matx[0][0] = l.q00; matx[0][1] = l.q10;	matx[0][2] = l.q20;
	matx[1][0] = l.q10; matx[1][1] = l.q11; matx[1][2] = l.q21;
	matx[2][0] = l.q20; matx[2][1] = l.q21;	matx[2][2] = l.q22;
	return *this;
    }

    /// Convert this to a symmetric matrix, using the lower-triangular portion
    UT_SymMatrix3T<T>	lowerTriangularSymMatrix3() const
    {
	return UT_SymMatrix3T<T>(matx[0][0],
				 matx[1][0], matx[1][1],
				 matx[2][0], matx[2][1], matx[2][2]);
    }

    /// Convert this to a symmetric matrix, using the upper-triangular portion
    UT_SymMatrix3T<T>	upperTriangularSymMatrix3() const
    {
	return UT_SymMatrix3T<T>(matx[0][0],
				 matx[0][1], matx[1][1],
				 matx[0][2], matx[1][2], matx[2][2]);
    }

    /// Convert this to a symmetric matrix, using averaged components
    UT_SymMatrix3T<T>	averagedSymMatrix3() const
    {
	return UT_SymMatrix3T<T>(matx[0][0],
				 (matx[0][1] + matx[1][0]) / 2,
				 matx[1][1],
				 (matx[0][2] + matx[2][0]) / 2,
				 (matx[1][2] + matx[2][1]) / 2,
				 matx[2][2]);
    }

    UT_Matrix3T<T>   operator-() const
                 {
                     return UT_Matrix3T<T>(-matx[0][0], -matx[0][1], -matx[0][2],
                                       -matx[1][0], -matx[1][1], -matx[1][2],
                                       -matx[2][0], -matx[2][1], -matx[2][2]);
                 }
    
    // Does += k * m
    SYS_FORCE_INLINE
    void	 addScaledMat(T k, const UT_Matrix3T<T> &m)
		{
		    matx[0][0]+=k*m.matx[0][0];
		    matx[0][1]+=k*m.matx[0][1];
		    matx[0][2]+=k*m.matx[0][2];

		    matx[1][0]+=k*m.matx[1][0];
		    matx[1][1]+=k*m.matx[1][1];
		    matx[1][2]+=k*m.matx[1][2];

		    matx[2][0]+=k*m.matx[2][0];
		    matx[2][1]+=k*m.matx[2][1];
		    matx[2][2]+=k*m.matx[2][2];
		}
    SYS_FORCE_INLINE
    UT_Matrix3T<T>	&operator+=(const UT_Matrix3T<T> &m)
		{
		    matx[0][0]+=m.matx[0][0];
		    matx[0][1]+=m.matx[0][1];
		    matx[0][2]+=m.matx[0][2];

		    matx[1][0]+=m.matx[1][0];
		    matx[1][1]+=m.matx[1][1];
		    matx[1][2]+=m.matx[1][2];

		    matx[2][0]+=m.matx[2][0];
		    matx[2][1]+=m.matx[2][1];
		    matx[2][2]+=m.matx[2][2];
		    return *this;
		}
    SYS_FORCE_INLINE
    UT_Matrix3T<T>	&operator-=(const UT_Matrix3T<T> &m)
		{
		    matx[0][0]-=m.matx[0][0];
		    matx[0][1]-=m.matx[0][1];
		    matx[0][2]-=m.matx[0][2];

		    matx[1][0]-=m.matx[1][0];
		    matx[1][1]-=m.matx[1][1];
		    matx[1][2]-=m.matx[1][2];

		    matx[2][0]-=m.matx[2][0];
		    matx[2][1]-=m.matx[2][1];
		    matx[2][2]-=m.matx[2][2];
		    return *this;
		}

    UT_Matrix3T<T>	&operator*=(const UT_Matrix3T<T> &m)
    {
	T a, b, c;
	a = matx[0][0]; b = matx[0][1]; c = matx[0][2];
	matx[0][0] = a*m(0,0) + b*m(1,0) + c*m(2,0);
	matx[0][1] = a*m(0,1) + b*m(1,1) + c*m(2,1);
	matx[0][2] = a*m(0,2) + b*m(1,2) + c*m(2,2);

	a = matx[1][0]; b = matx[1][1]; c = matx[1][2];
	matx[1][0] = a*m(0,0) + b*m(1,0) + c*m(2,0);
	matx[1][1] = a*m(0,1) + b*m(1,1) + c*m(2,1);
	matx[1][2] = a*m(0,2) + b*m(1,2) + c*m(2,2);

	a = matx[2][0]; b = matx[2][1]; c = matx[2][2];
	matx[2][0] = a*m(0,0) + b*m(1,0) + c*m(2,0);
	matx[2][1] = a*m(0,1) + b*m(1,1) + c*m(2,1);
	matx[2][2] = a*m(0,2) + b*m(1,2) + c*m(2,2);
	return *this;
    }

    /// Multiply given symmetric matrix on the right
    template <typename S>
    UT_Matrix3T<T>	&operator*=(const UT_SymMatrix3T<S> &m)
    {
	typedef typename UT_SymMatrix3T<S>::LowerTri LowerTri;

	const LowerTri&	l = m.lowerTri();
	T		a, b, c;

	a = matx[0][0]; b = matx[0][1]; c = matx[0][2];
	matx[0][0] = a*l.q00 + b*l.q10 + c*l.q20;
	matx[0][1] = a*l.q10 + b*l.q11 + c*l.q21;
	matx[0][2] = a*l.q20 + b*l.q21 + c*l.q22;

	a = matx[1][0]; b = matx[1][1]; c = matx[1][2];
	matx[1][0] = a*l.q00 + b*l.q10 + c*l.q20;
	matx[1][1] = a*l.q10 + b*l.q11 + c*l.q21;
	matx[1][2] = a*l.q20 + b*l.q21 + c*l.q22;

	a = matx[2][0]; b = matx[2][1]; c = matx[2][2];
	matx[2][0] = a*l.q00 + b*l.q10 + c*l.q20;
	matx[2][1] = a*l.q10 + b*l.q11 + c*l.q21;
	matx[2][2] = a*l.q20 + b*l.q21 + c*l.q22;

	return *this;
    }

    /// Multiply given symmetric matrix on the left
    template <typename S>
    void		leftMult(const UT_SymMatrix3T<S> &m)
    {
	typedef typename UT_SymMatrix3T<S>::LowerTri LowerTri;

	const LowerTri&	l = m.lowerTri();
	T		a, b, c;

	a = matx[0][0]; b = matx[1][0]; c = matx[2][0];
	matx[0][0] = a*l.q00 + b*l.q10 + c*l.q20;
	matx[1][0] = a*l.q10 + b*l.q11 + c*l.q21;
	matx[2][0] = a*l.q20 + b*l.q21 + c*l.q22;

	a = matx[0][1]; b = matx[1][1]; c = matx[2][1];
	matx[0][1] = a*l.q00 + b*l.q10 + c*l.q20;
	matx[1][1] = a*l.q10 + b*l.q11 + c*l.q21;
	matx[2][1] = a*l.q20 + b*l.q21 + c*l.q22;

	a = matx[0][2]; b = matx[1][2]; c = matx[2][2];
	matx[0][2] = a*l.q00 + b*l.q10 + c*l.q20;
	matx[1][2] = a*l.q10 + b*l.q11 + c*l.q21;
	matx[2][2] = a*l.q20 + b*l.q21 + c*l.q22;
    }

    /// Multiply given matrix3 on the left
    template <typename S>
    void		leftMult(const UT_Matrix3T<S> &m)
    {
	T a, b, c;

	a = matx[0][0]; b = matx[1][0]; c = matx[2][0];
	matx[0][0] = a*m(0,0) + b*m(0,1) + c*m(0,2);
	matx[1][0] = a*m(1,0) + b*m(1,1) + c*m(1,2);
	matx[2][0] = a*m(2,0) + b*m(2,1) + c*m(2,2);

	a = matx[0][1]; b = matx[1][1]; c = matx[2][1];
	matx[0][1] = a*m(0,0) + b*m(0,1) + c*m(0,2);
	matx[1][1] = a*m(1,0) + b*m(1,1) + c*m(1,2);
	matx[2][1] = a*m(2,0) + b*m(2,1) + c*m(2,2);

	a = matx[0][2]; b = matx[1][2]; c = matx[2][2];
	matx[0][2] = a*m(0,0) + b*m(0,1) + c*m(0,2);
	matx[1][2] = a*m(1,0) + b*m(1,1) + c*m(1,2);
	matx[2][2] = a*m(2,0) + b*m(2,1) + c*m(2,2);
    }

    void	multiply3(const UT_Matrix4 &m);
    void	multiply3(const UT_DMatrix4 &m);

    constexpr bool operator==(const UT_Matrix3T<T> &m) const noexcept
    {
        return (&m == this) || (
            matx[0][0]==m(0,0) && matx[0][1]==m(0,1) && matx[0][2]==m(0,2) &&
            matx[1][0]==m(1,0) && matx[1][1]==m(1,1) && matx[1][2]==m(1,2) &&
            matx[2][0]==m(2,0) && matx[2][1]==m(2,1) && matx[2][2]==m(2,2) );
    }
    constexpr bool operator!=(const UT_Matrix3T<T> &m) const noexcept
		{
		    return !(*this == m);
		}

    // Scalar operators:
    UT_Matrix3T<T>  &operator= (T val)
		{
		    matx[0][0] = val; matx[0][1] = 0; matx[0][2] = 0;
		    matx[1][0] = 0; matx[1][1] = val; matx[1][2] = 0;
		    matx[2][0] = 0; matx[2][1] = 0; matx[2][2] = val;
		    return *this;
		}
    SYS_FORCE_INLINE
    UT_Matrix3T<T>  &operator*=(T scalar)
		{
		    matx[0][0]*=scalar; matx[0][1]*=scalar; matx[0][2]*=scalar;
		    matx[1][0]*=scalar; matx[1][1]*=scalar; matx[1][2]*=scalar;
		    matx[2][0]*=scalar; matx[2][1]*=scalar; matx[2][2]*=scalar;
		    return *this;
		}
    SYS_FORCE_INLINE
    UT_Matrix3T<T>  &operator/=(T scalar)
		{
		    return operator*=( 1.0f/scalar );
		}

    // Vector3 operators:
    template <typename S>
    inline UT_Matrix3T<T>	&operator= (const UT_Vector3T<S> &vec);
    template <typename S>
    inline UT_Matrix3T<T>	&operator+=(const UT_Vector3T<S> &vec);
    template <typename S>
    inline UT_Matrix3T<T>	&operator-=(const UT_Vector3T<S> &vec);

    /// Scaled outer product update (given scalar b, row vectors v1 and v2)
    /// this += b * transpose(v1) * v2
    /// @{
    template <typename S>
    inline void		 outerproductUpdateT(T b, 
					    const UT_Vector3T<S> &v1,
					    const UT_Vector3T<S> &v2);
    SYS_FORCE_INLINE
    void		 outerproductUpdate(T b, 
					    const UT_Vector3F &v1,
					    const UT_Vector3F &v2)
    { outerproductUpdateT(b, v1, v2); }
    SYS_FORCE_INLINE
    void		 outerproductUpdate(T b, 
					    const UT_Vector3D &v1,
					    const UT_Vector3D &v2)
    { outerproductUpdateT(b, v1, v2); }
    /// @}

    // Other matrix operations:
    // Computes the basis of a 3x3 matrix.  This is used by OBB code
    // to determine what orientation of a bounding box best represents
    // a cloud of points.  To do this, first fill this matrix with
    // the sum of p*pT for all points in the cloud.
    // The result is in this matrix itself.
    void		getBasis();

    // Sets this matrix to the canonical 180 rotation matrix (about the y axis)
    // that is used in the orient() and dihedral() functions when given two
    // opposing vectors.
    void		arbitrary180rot();
    
    /// Compute the dihedral: return the matrix that computes the rotation
    /// around the axes normal to both a and b, (their cross product), by the
    /// angle which is between a and b. The resulting matrix maps a onto  b. If
    /// a and b have the same direction, what comes out is the identity matrix.
    /// If a and b are opposed, the rotation is undefined and the method
    /// returns a vector c of zeroes (check with !c); if all is OK, c != 0.
    /// The transformation is a symmetry around vector a, then a symmetry
    /// around (norm(a) and norm(b)). If a or b needs to be normalized, pass a
    /// non-zero value in norm.
    /// The second function places the result in 'm', and returns 0 if a and b
    /// are not opposed; otherwise, it returns 1.
    // @{
    template <typename S>
    static UT_Matrix3T<T>	dihedral(UT_Vector3T<S> &a, UT_Vector3T<S> &b,
				     UT_Vector3T<S> &c,int norm=1);
    /// @note If dihedral() returns 0, this matrix is NOT modified!
    template <typename S>
    int			dihedral(UT_Vector3T<S> &a, UT_Vector3T<S> &b,
				     int norm=1);
    // @}

    // Compute a lookat matrix:  These functions will compute a rotation matrix
    // (yes, with all the properties of a rotation matrix), which will provide
    // the rotates needed for "from" to look at "to".  One method uses a "roll"
    // the other method uses an "up" vector to determine the orientation.  The
    // "roll" is not as well defined as using an "up" vector.  If the two points
    // are co-incident, the result will be an identity matrix.  If norm is set,
    // then the up vector will be normalized.
    // The lookat matrix will have the -Z axis pointing at the "to" point.
    // The y axis will be pointing "up"
    template <typename S>
    void	lookat(const UT_Vector3T<S> &from, const UT_Vector3T<S> &to,
			   T roll = 0);
    template <typename S>
    void	lookat(const UT_Vector3T<S> &from, const UT_Vector3T<S> &to,
			   const UT_Vector3T<S> &up);


    // Compute the transform matrix to orient given a direction and an up vector
    // If the direction and up are not orthogonal, then the up vector will be
    // shifted along the direction to make it orthogonal...  If the up and
    // direction vectors are co-linear, then an identity matrix will be
    // returned.
    // The second function will return 0 to indicate this.  The inputs may
    // be modified by the orient functions.
    template <typename S>
    int		orient(UT_Vector3T<S> &dir, UT_Vector3T<S> &up, int norm=1);
    template <typename S>
    int		orientInverse(UT_Vector3T<S> &dir, UT_Vector3T<S> &up, int norm=1);


    // Computes a transform to orient to a given direction (v) with a scale
    // (pscale).  The up vector (up) is optional and will orient the matrix to
    // this up vector. If no up vector is given, the z axis will be oriented to
    // point in the v direction. If a quaternion (q) is specified, the
    // orientation will be additionally transformed by the rotation specified
    // by the quaternion.
    // The matrix is scaled non-uniformly about each axis using s3, if s3
    // is non-zero.  A uniform scale of pscale is applied regardless, so if
    // s3 is non-zero, the x axis will be scaled by pscale * s3->x().
    template <typename S>
    void		orientT(const UT_Vector3T<S>& v, 
				   T pscale, const UT_Vector3T<S> *s3,
				   const UT_Vector3T<S>* up,
				   const UT_QuaternionT<S> * q);
    void		orient(const UT_Vector3F &v, 
				   T pscale, const UT_Vector3F *s3,
				   const UT_Vector3F *up,
				   const UT_QuaternionF *q)
    { orientT(v, pscale, s3, up, q); }
    void		orient(const UT_Vector3D &v, 
				   T pscale, const UT_Vector3D *s3,
				   const UT_Vector3D *up,
				   const UT_QuaternionD *q)
    { orientT(v, pscale, s3, up, q); }
    template <typename S>
    void		orientInverseT(const UT_Vector3T<S>& v, 
				   T pscale, const UT_Vector3T<S> *s3,
				   const UT_Vector3T<S>* up,
				   const UT_QuaternionT<S> * q);
    void		orientInverse(const UT_Vector3F &v, 
				   T pscale, const UT_Vector3F *s3,
				   const UT_Vector3F *up,
				   const UT_QuaternionF *q)
    { orientInverseT(v, pscale, s3, up, q); }
    void		orientInverse(const UT_Vector3D &v, 
				   T pscale, const UT_Vector3D *s3,
				   const UT_Vector3D *up,
				   const UT_QuaternionD *q)
    { orientInverseT(v, pscale, s3, up, q); }

    // Return the cofactor of the matrix, ie the determinant of the 2x2 
    // submatrix that results from removing row 'k' and column 'l' from the 
    // 3x3.
    SYS_FORCE_INLINE
    T		coFactor(int k, int l) const
		{
		    int		r[2], c[2];

		    // r, c should evaluate to compile time constants
		    coVals(k, r);
		    coVals(l, c);
		    if ((k ^ l) & 1)
			UTswap(c[0], c[1]);

		    return matx[r[0]][c[0]]*matx[r[1]][c[1]] -
			   matx[r[0]][c[1]]*matx[r[1]][c[0]];
		}
    
    T		determinant() const
		{
		    return(matx[0][0]*coFactor(0,0) +
			   matx[0][1]*coFactor(0,1) +
			   matx[0][2]*coFactor(0,2));
		}
    T		trace() const
                { return matx[0][0] + matx[1][1] + matx[2][2]; }

    /// Computes the eigenvalues.  Returns number of real eigenvalues
    /// found.  Uses UT_RootFinder::cubic
    /// @param r The real eigenvalues
    /// @param i The imaginary eigenvalues
    template <typename S>
    int		eigenvalues(UT_Vector3T<S> &r, UT_Vector3T<S> &i) const;

    /// Invert this matrix and return 0 if OK, 1 if singular.
    int		invert();
    /// Invert the matrix and return 0 if OK, 1 if singular.
    /// Puts the inverted matrix in m, and leaves this matrix unchanged.
    int		invert(UT_Matrix3T<T> &m)const;

    /// Use Kramer's method to compute the matrix inverse
    /// If abs( det(m) ) <= abs_det_threshold, then return 1 without changing m
    /// Otherwise, return 0, storing the inverse matrix in m
    ///
    /// NOTE: The default FLT_EPSILON is kept here to preserve existing behavior
    ///       This is not a good default!
    ///       The right threshold must be decided separately for each use case.
    int		invertKramer(UT_Matrix3T<T> &m, T abs_det_threshold = FLT_EPSILON) const;

    // Shorthand for invertKramer(*this)
    int		invertKramer(T abs_det_threshold = FLT_EPSILON);

    // Invert *this.
    // *this should be symmetric.
    // *this does NOT have to be non-singular.  Singular matrices will be
    // inverted by filtering out the singular component.
    // This means you can reliably solve:
    // M x = b
    // for the x that is closest to satisfying b
    // Returns 0 on success, 1 if something really bad happened.
    int		safeInvertSymmetric(T tol=1e-6f);

    // Diagonalize *this.
    // *this should be symmetric.
    // The matrix is factored into:
    // *this = Rt D R
    // The diagonalization is done with a serios of jacobi rotations.
    // *this is unchanged by the operations.
    // Returns true if successful, false if reached maximum iterations rather
    // than desired tolerance.
    // Tolerance is with respect to the maximal element of the matrix.
    bool	diagonalizeSymmetric(UT_Matrix3T<T> &R, UT_Matrix3T<T> &D, T tol=1e-6f, int maxiter = 100) const;

    // Compute the SVD decomposition of *this
    // The matrix is factored into
    // *this = U * S * Vt
    // Where S is diagonal, and U and Vt are both orthognal matrices
    // Tolerance is with respect to the maximal element of the matrix
    void	svdDecomposition(UT_Matrix3T<T> &U, UT_Matrix3T<T> &S, UT_Matrix3T<T> &V, T tol=1e-6f) const;

    /// Splits a covariance matrix into a scale & rotation component.
    /// This should be built by a series of outer product updates.
    /// R is the rotation component, S the inverse scale.
    /// If this is A, finds the inverse square root of AtA, S. 
    /// We then set R = transpose(AS) to give us a rotation matrix.
    /// NOTE: For general matrices, use makeRotationMatrix()!
    void	splitCovarianceToRotationScale(UT_Matrix3T<T> &R, UT_Matrix3T<T> &S, T tol=1e-6f) const;

    bool	isSymmetric(T tolerance = T(SYS_FTOLERANCE)) const;

    // Transpose this matrix or return its transpose.
    void	transpose(void)
		{
		    T	 tmp;
		    tmp=matx[0][1];  matx[0][1]=matx[1][0];  matx[1][0]=tmp;
		    tmp=matx[0][2];  matx[0][2]=matx[2][0];  matx[2][0]=tmp;
		    tmp=matx[1][2];  matx[1][2]=matx[2][1];  matx[2][1]=tmp;
		}
    UT_Matrix3T<T> transposedCopy() const
		{
		    return UT_Matrix3T<T>(matx[0][0], matx[1][0], matx[2][0],
					  matx[0][1], matx[1][1], matx[2][1],
					  matx[0][2], matx[1][2], matx[2][2]);
		}

    // check for equality within a tolerance level
    bool	isEqual( const UT_Matrix3T<T> &m,
			 T tolerance=T(SYS_FTOLERANCE) ) const
		{
		    return (&m == this) || (
		    SYSisEqual( matx[0][0], m.matx[0][0], tolerance ) &&
		    SYSisEqual( matx[0][1], m.matx[0][1], tolerance ) &&
		    SYSisEqual( matx[0][2], m.matx[0][2], tolerance ) &&

		    SYSisEqual( matx[1][0], m.matx[1][0], tolerance ) &&
		    SYSisEqual( matx[1][1], m.matx[1][1], tolerance ) &&
		    SYSisEqual( matx[1][2], m.matx[1][2], tolerance ) &&

		    SYSisEqual( matx[2][0], m.matx[2][0], tolerance ) &&
		    SYSisEqual( matx[2][1], m.matx[2][1], tolerance ) &&
		    SYSisEqual( matx[2][2], m.matx[2][2], tolerance ) );
		}

    /// Post-multiply this matrix by a 3x3 rotation matrix determined by the
    /// axis and angle of rotation in radians.
    /// If 'norm' is not 0, the axis vector is normalized before computing the
    /// rotation matrix. rotationMat() returns a rotation matrix, and could as
    /// well be defined as a free floating function.
    /// @{
    template <typename S>
    void rotate(UT_Vector3T<S> &axis, T theta, int norm=1);
    void rotate(UT_Axis3::axis a, T theta);
    template<UT_Axis3::axis A>
    void rotate(T theta);
    /// @}

    /// Create a rotation matrix for the given angle in radians around the axis
    /// @{
    template <typename S>
    static UT_Matrix3T<T> rotationMat(UT_Vector3T<S> &axis, T theta, int norm=1);
    static UT_Matrix3T<T> rotationMat(UT_Axis3::axis a, T theta);
    /// @}

    /// Pre-multiply this matrix by the theta radians rotation around the axis
    /// @{
    template <typename S>
    void prerotate(UT_Vector3T<S> &axis, T theta, int norm=1);
    void prerotate(UT_Axis3::axis a, T theta);
    template<UT_Axis3::axis A>
    void prerotate(T theta);
    /// @}

    /// Pre-rotate by rx, ry, rz radians around the three basic axes in the
    /// order given by UT_XformOrder.
    /// @{
    void	prerotate(T rx, T ry, T rz,
			  const UT_XformOrder &order);
    inline void	prerotate(const UT_Vector3T<T> &rad, const UT_XformOrder &order)
		    { prerotate(rad(0), rad(1), rad(2), order); }
    /// @}


    /// Post-rotate by rx, ry, rz radians around the three basic axes in the
    /// order given by UT_XformOrder.
    /// @{
    void	rotate(T rx, T ry, T rz,
		       const UT_XformOrder &ord);
    inline void	rotate(const UT_Vector3T<T> &rad, const UT_XformOrder &ord)
		    { rotate(rad(0), rad(1), rad(2), ord); }
    /// @}

    /// Post-multiply this matrix by a scale matrix with diagonal (sx, sy, sz)
    /// @{
    void	scale(T sx, T sy, T sz)
		{
		    matx[0][0] *= sx; matx[0][1] *= sy; matx[0][2] *= sz;
		    matx[1][0] *= sx; matx[1][1] *= sy; matx[1][2] *= sz;
		    matx[2][0] *= sx; matx[2][1] *= sy; matx[2][2] *= sz;
		}
    SYS_FORCE_INLINE
    void	scale(const UT_Vector3T<T> &s)
		    { scale(s(0), s(1), s(2)); }
    /// @}

    /// Pre-multiply this matrix by a scale matrix with diagonal (sx, sy, sz)
    /// @{
    void	prescale(T sx, T sy, T sz)
		{
		    matx[0][0] *= sx; matx[1][0] *= sy; matx[2][0] *= sz;
		    matx[0][1] *= sx; matx[1][1] *= sy; matx[2][1] *= sz;
		    matx[0][2] *= sx; matx[1][2] *= sy; matx[2][2] *= sz;
		}
    SYS_FORCE_INLINE
    void	prescale(const UT_Vector3T<T> &s)
		    { prescale(s(0), s(1), s(2)); }
    /// @}

    /// Negates this matrix, i.e. multiplies it by -1.
    SYS_FORCE_INLINE void negate()
    {
        matx[0][0] = -matx[0][0]; matx[0][1] = -matx[0][1]; matx[0][2] = -matx[0][2];
        matx[1][0] = -matx[1][0]; matx[1][1] = -matx[1][1]; matx[1][2] = -matx[1][2];
        matx[2][0] = -matx[2][0]; matx[2][1] = -matx[2][1]; matx[2][2] = -matx[2][2];
    }

    /// Post-multiply this matrix by a translation matrix determined by (dx,dy)
    /// @{
    void	translate(T dx, T dy)
		{
		    matx[0][0] += matx[0][2] * dx;
		    matx[0][1] += matx[0][2] * dy;
		    matx[1][0] += matx[1][2] * dx;
		    matx[1][1] += matx[1][2] * dy;
		    matx[2][0] += matx[2][2] * dx;
		    matx[2][1] += matx[2][2] * dy;
		}
    SYS_FORCE_INLINE
    void	translate(const UT_Vector2T<T> &delta)
		    { translate(delta(0), delta(1)); }
    /// @}

    /// Pre-multiply this matrix by the translation matrix determined by (dx,dy)
    /// @{
    void	pretranslate(T dx, T dy)
		{
		    matx[2][0] += matx[0][0] * dx + matx[1][0] * dy;
		    matx[2][1] += matx[0][1] * dx + matx[1][1] * dy;
		    matx[2][2] += matx[0][2] * dx + matx[1][2] * dy;
		}
    SYS_FORCE_INLINE
    void	pretranslate(const UT_Vector2T<T> &delta)
		    { pretranslate(delta(0), delta(1)); }
    /// @}

    /// Generate the x, y, and z values of rotation in radians.
    /// Return 0 if successful, and a non-zero value otherwise: 1 if the matrix
    /// is not a valid rotation matrix, 2 if rotOrder is invalid, and 3 for
    /// other errors. rotOrder must be given as a UT_XformOrder permulation.
    /// WARNING: For animation or transform blending, use polarDecompose()
    ///          or makeRotationMatrix()!
    /// WARNING: The non-const method changes the matrix!
    template <typename S>
    int		crack(UT_Vector3T<S> &rvec, const UT_XformOrder &order,
		      int remove_scales=1);
    template <typename S>
    int		crack(UT_Vector3T<S> &rvec, const UT_XformOrder &order,
		      int remove_scales=1) const;
    int		crack2D(T &r) const;

    /// Perform the polar decomposition M into an orthogonal matrix Q and an
    /// symmetric positive-semidefinite matrix S. This is more useful than
    /// crack() or conditionRotate() when the desire is to blend transforms.
    /// By default, it gives M=SQ, a left polar decomposition. If reverse is
    /// false, then it gives M=QS, a right polar decomposition.
    /// @pre *this is non-singular
    /// @post *this = Q, and if stretch != 0: *stretch = S
    /// @return True if successful
    bool	polarDecompose(
			UT_Matrix3T<T> *stretch = nullptr,
			bool reverse = true,
			const int max_iter = 64,
			const T rel_tol = FLT_EPSILON);

    /// Turn this matrix into the "closest" rotation matrix.
    ///
    /// It uses polarDecompose and then negates the matrix and stretch
    /// if there is a negative determinant (scale).  It returns false iff
    /// polarDecompose failed, possibly due to a singular matrix.
    ///
    /// This is currently the one true way to turn an arbitrary
    /// matrix into a rotation matrix.  If that ever changes,
    /// put a warning here, and you may want to update
    /// UT_Matrix4::makeRigidMatrix too.
    bool        makeRotationMatrix(
                        UT_Matrix3T<T> *stretch = nullptr,
                        bool reverse = true,
                        const int max_iter = 64,
                        const T rel_tol = FLT_EPSILON);

    /// This method will condition the matrix so that it's a valid rotation
    /// matrix (i.e. crackable).  Ideally, the scales should be removed first,
    /// but this method will attempt to do this as well.  It will return the
    /// conditioned scales if a vector is passed in.
    /// WARNING: If you just want a rotation matrix, use makeRotationMatrix()!
    /// @{
    template <typename S>
    void	conditionRotateT(UT_Vector3T<S> *scales);
    void	conditionRotate(UT_Vector3F *scales=0)
    { conditionRotateT(scales); }
    void	conditionRotate(UT_Vector3D *scales)
    { conditionRotateT(scales); }
    /// @}

    /// Extract scales and shears from the matrix leaving the matrix as a nice
    /// orthogonal rotation matrix.  The shears are:
    ///			XY, XZ, and YZ
    /// WARNING: If you just want a rotation matrix, use makeRotationMatrix()!
    /// @{
    template <typename S>
    void	extractScalesT(UT_Vector3T<S> &scales, UT_Vector3T<S> *shears);
    void	extractScales(UT_Vector3D &scales, UT_Vector3D *shears=0)
		{ extractScalesT(scales, shears); }
    void	extractScales(UT_Vector3F &scales, UT_Vector3F *shears=0)
		{ extractScalesT(scales, shears); }
    void	extractScales(UT_Vector3T<fpreal16> &scales,
		    UT_Vector3T<fpreal16> *shears=0)
		{ extractScalesT(scales, shears); }
    /// @}

    // Extract scales and shears from the matrix leaving the matrix as a nice
    // orthogonal rotation matrix (assuming it was only a 2D xform to begin
    // with).  The shears are:
    //			XY
    template <typename S>
    void	extractScales2D(UT_Vector2T<S> &scales, T *shears=0);

    // Shear the matrix according to the values.  This is equivalent to
    // multiplying the matrix by the shear matrix with the given value.
    void	shearXY(T val);
    void	shearXZ(T val);
    void	shearYZ(T val);

    /// Post-multiply this matrix by the shear matrix formed by (sxy, sxz, syz)
    /// @{
    void	shear(T s_xy, T s_xz, T s_yz)
    {
	matx[0][0] += matx[0][1]*s_xy + matx[0][2]*s_xz;
	matx[0][1] += matx[0][2]*s_yz;

	matx[1][0] += matx[1][1]*s_xy + matx[1][2]*s_xz;
	matx[1][1] += matx[1][2]*s_yz;

	matx[2][0] += matx[2][1]*s_xy + matx[2][2]*s_xz;
	matx[2][1] += matx[2][2]*s_yz;
    }
    SYS_FORCE_INLINE
    void	shear(const UT_Vector3T<T> &sh)
		    { shear(sh(0), sh(1), sh(2)); }
    /// @}

    // Solve a 3x3 system of equations whose parameters are held in this
    // matrix, and whose rhs constants are cx, cy, and cz. The method returns
    // 0 if the determinant is not 0, and 1 otherwise.
    template <typename S>
    int		solve(T cx, T cy, T cz,
		      UT_Vector3T<S> &result) const;

    // Space change operation: right multiply this matrix by the 3x3 matrix
    // of the transformation which moves the vector space defined by
    // (iSrc, jSrc, cross(iSrc,jSrc)) into the space defined by
    // (iDest, jDest, cross(iDest,jDest)). iSrc, jSrc, iDest, and jDest will
    // be normalized before the operation if norm is 1. This matrix transforms
    // iSrc into iDest, and jSrc into jDest.
    template <typename S>
    void	changeSpace(UT_Vector3T<S> &iSrc, UT_Vector3T<S> &jSrc,
			    UT_Vector3T<S> &iDest,UT_Vector3T<S> &jDest,
			    int norm=1);

    // Multiply this matrix by the general transform matrix built from 
    // translations (tx,ty), degree rotation (rz), scales (sx,sy),
    // and possibly a pivot point (px,py). The second methos leaves us
    // unchanged, and returns a new (this*xform) instead. The order of the 
    // multiplies (SRT, RST, Rz, etc) is stored in 'order'. Normally you
    // will ignore the 'reverse' parameter, which tells us to build the
    // matrix from last to first xform, and to apply some inverses to 
    // txy, rz, and sxy.
    void	xform(const UT_XformOrder &order, 
		  T tx=0.0f, T ty=0.0f, T rz=0.0f,
		  T sx=1.0f, T sy=1.0f,T px=0.0f,T py=0.0f,
		  int reverse=0);

    // this version handles shears as well
    void	xform(const UT_XformOrder &order, 
		  T tx, T ty, T rz,
		  T sx, T sy, T s_xy,
		  T px, T py,
		  int reverse=0);

    // Right multiply this matrix by a 3x3 matrix which scales by a given 
    // amount along the direction of vector v. When applied to a vector w,
    // the stretched matrix (*this) stretches w in v by the amount given.
    // If norm is non-zero, v will be normalized prior to the operation.
    template <typename S>
    void	stretch(UT_Vector3T<S> &v, T amount, int norm=1);
    template <typename S>
    SYS_DEPRECATED(12.0) UT_Matrix3T<T>	stretch(UT_Vector3T<S> &v, T amount, int norm=1) const;

    //
    // This does a test to see if a vector transformed by the matrix
    // will maintain it's length (i.e. there are no scales in the matrix)
    int		isNormalized() const;

    // Return the dot product between two rows i and j:
    T		dot(unsigned i, unsigned j) const
		{
		    return (i <= 2 && j <= 2) ?
		    matx[i][0]*matx[j][0] + matx[i][1]*matx[j][1] +
					    matx[i][2]*matx[j][2] : (T)0;
		}

    /// Set the matrix to identity
    void	identity()	{ *this = 1; }
    /// Set the matrix to zero
    void	zero()	        { *this = 0; }

    int		isIdentity() const
		{
		    // NB: DO NOT USE TOLERANCES
		    return(
			    matx[0][0]==1.0f && matx[0][1]==0.0f &&
			    matx[0][2]==0.0f && matx[1][0]==0.0f &&
			    matx[1][1]==1.0f && matx[1][2]==0.0f &&
			    matx[2][0]==0.0f && matx[2][1]==0.0f &&
			    matx[2][2]==1.0f
			  );
		}

    int		isZero() const
		{
		    // NB: DO NOT USE TOLERANCES
		    return (
			    matx[0][0]==0.0f && matx[0][1]==0.0f &&
			    matx[0][2]==0.0f && matx[1][0]==0.0f &&
			    matx[1][1]==0.0f && matx[1][2]==0.0f &&
			    matx[2][0]==0.0f && matx[2][1]==0.0f &&
			    matx[2][2]==0.0f
			   );
		}
    
    /// Return the raw matrix data.
    // @{
    const T	  *data(void) const	{ return myFloats; }
    T		   *data(void)		{ return myFloats; }
    // @}

    /// Compute a hash
    unsigned	hash() const	{ return SYSvector_hash(data(), tuple_size); }

    /// Return a matrix entry. No bounds checking on subscripts.
    // @{
    SYS_FORCE_INLINE T &operator()(unsigned row, unsigned col) noexcept
		 {
		     UT_ASSERT_P(row < 3 && col < 3);
		     return matx[row][col];
		 }
    SYS_FORCE_INLINE
    T operator()(unsigned row, unsigned col) const noexcept
		 {
		     UT_ASSERT_P(row < 3 && col < 3);
		     return matx[row][col];
		 }
    // @}
    
    /// Return a matrix row. No bounds checking on subscript.
    // @{
    T		*operator()(unsigned row)
		 {
		     UT_ASSERT_P(row < 3);
		     return matx[row];
		 }
    const T	*operator()(unsigned row) const
		 {
		     UT_ASSERT_P(row < 3);
		     return matx[row];
		 }
    inline const UT_Vector3T<T> &operator[](unsigned row) const;
    inline UT_Vector3T<T> &operator[](unsigned row);
    // @}

    /// Euclidean or Frobenius norm of a matrix.
    /// Does sqrt(sum(a_ij ^2))
    T		 getEuclideanNorm() const
		 { return SYSsqrt(getEuclideanNorm2()); }
    /// Euclidean norm squared.
    T		 getEuclideanNorm2() const;

    /// Get the squared Euclidean distance between '*this' and 'from'
    /// Returns sum_ij(sqr(b_ij-a_ij))
    T		 distanceSquared(const UT_Matrix3T<T> &src);

    /// Get the 1-norm of this matrix, assuming a row vector convention.
    /// Returns the maximum absolute row sum. ie. max_i(sum_j(abs(a_ij)))
    T		 getNorm1() const;

    /// Get the inf-norm of this matrix, assuming a row vector convention.
    /// Returns the maximum absolute column sum. ie. max_j(sum_i(abs(a_ij)))
    T		 getNormInf() const;

    // I/O methods.  Return 0 if read/write successful, -1 if unsuccessful.
    int			 save(std::ostream &os, int binary) const;
    bool		 load(UT_IStream &is);

    void		 outAsciiNoName(std::ostream &os) const;

    static const UT_Matrix3T<T> &getIdentityMatrix();

    // I/O friends:
    friend std::ostream	&operator<<(std::ostream &os, const UT_Matrix3T<T> &v)
			{
			    v.writeClassName(os);
			    v.outAsciiNoName(os);
			    return os;
			}

    /// @{
    /// Methods to serialize to a JSON stream.  The matrix is stored as an
    /// array of 9 reals.
    bool		save(UT_JSONWriter &w) const;
    bool		save(UT_JSONValue &v) const;
    bool		load(UT_JSONParser &p);
    /// @}

    /// Returns the vector size
    static int		entries()	{ return tuple_size; }

private:
    // Check this matrix to see if it is a valid rotation matrix, and permute
    // its elements depending on the rotOrder value in crack(). checkRot() 
    // returns 0 if OK and 1 otherwise.
    int			checkRot() const;
    void		permute(int l0, int l1, int l2);

    // Operation to aid in cofactor computation
    SYS_FORCE_INLINE
    void		coVals(int k, int r[2]) const
			{
			    switch(k)
			    {
				case 0: r[0] = 1; r[1] = 2; break;
				case 1: r[0] = 0; r[1] = 2; break;
				case 2: r[0] = 0; r[1] = 1; break;
			    }
			}


    void		 writeClassName(std::ostream &os) const;
    static const char	*className();

    // The matrix data:
    union {
	T	matx[3][3];
	T	myFloats[tuple_size];
    };
};

#include "UT_Vector3.h"

template <typename T>
template <typename S>
inline
UT_Matrix3T<T>	&UT_Matrix3T<T>::operator=(const UT_Vector3T<S> &vec)
{
    matx[0][0] = matx[0][1] = matx[0][2] = vec.x();
    matx[1][0] = matx[1][1] = matx[1][2] = vec.y();
    matx[2][0] = matx[2][1] = matx[2][2] = vec.z();
    return *this;
}

template <typename T>
template <typename S>
inline
UT_Matrix3T<T>	&UT_Matrix3T<T>::operator+=(const UT_Vector3T<S> &vec)
{
    T	 x = vec.x(); T	 y = vec.y(); T	 z = vec.z();
    matx[0][0]+=x; matx[0][1]+=x; matx[0][2]+=x;
    matx[1][0]+=y; matx[1][1]+=y; matx[1][2]+=y;
    matx[2][0]+=z; matx[2][1]+=z; matx[2][2]+=z;
    return *this;
}

template <typename T>
template <typename S>
inline
UT_Matrix3T<T>	&UT_Matrix3T<T>::operator-=(const UT_Vector3T<S> &vec)
{
    T	 x = vec.x(); T	 y = vec.y(); T	 z = vec.z();
    matx[0][0]-=x; matx[0][1]-=x; matx[0][2]-=x;
    matx[1][0]-=y; matx[1][1]-=y; matx[1][2]-=y;
    matx[2][0]-=z; matx[2][1]-=z; matx[2][2]-=z;
    return *this;
}

#ifndef UT_DISABLE_VECTORIZE_MATRIX
template <> inline UT_Matrix3T<float>&
UT_Matrix3T<float>::operator*=(const UT_Matrix3T<float> &m)
{
    v4uf r0(m.matx[0]);
    v4uf r1(m.matx[1]);
    v4uf r2(m(2,0), m(2,1), m(2,2), 0.f);

    // Do not alter the order in which the rows are computed.
    // This routine relies on overriding
    const float m10 = matx[1][0];
    const float m20 = matx[2][0];
    {
    	const v4uf row =
    	      r0 * v4uf(matx[0][0])
    	    + r1 * v4uf(matx[0][1])
    	    + r2 * v4uf(matx[0][2]);
	vm_store(matx[0], row.vector);
    }
    {
    	const v4uf row =
    	      r0 * v4uf(m10)
    	    + r1 * v4uf(matx[1][1])
    	    + r2 * v4uf(matx[1][2]);
	vm_store(matx[1], row.vector);
    }
    {
    	const v4uf row =
    	      r0 * v4uf(m20)
    	    + r1 * v4uf(matx[2][1])
    	    + r2 * v4uf(matx[2][2]);

    	matx[2][0] = row[0];
    	matx[2][1] = row[1];
    	matx[2][2] = row[2];
    }
    return *this;
}
template <> template <> inline UT_Matrix3T<float>&
UT_Matrix3T<float>::operator*=(const UT_SymMatrix3T<float> &m)
{
    const auto& l = m.lowerTri();

    v4uf r0(l.q00, l.q10, l.q20, 0.f);
    v4uf r1(l.q10, l.q11, l.q21, 0.f);
    v4uf r2(l.q20, l.q21, l.q22, 0.f);

    // Do not alter the order in which the rows are computed.
    // This routine relies on overriding
    const float m10 = matx[1][0];
    const float m20 = matx[2][0];
    {
    	const v4uf row =
    	      r0 * v4uf(matx[0][0])
    	    + r1 * v4uf(matx[0][1])
    	    + r2 * v4uf(matx[0][2]);
	vm_store(matx[0], row.vector);
    }
    {
    	const v4uf row =
    	      r0 * v4uf(m10)
    	    + r1 * v4uf(matx[1][1])
    	    + r2 * v4uf(matx[1][2]);
	vm_store(matx[1], row.vector);
    }
    {
    	const v4uf row =
    	      r0 * v4uf(m20)
    	    + r1 * v4uf(matx[2][1])
    	    + r2 * v4uf(matx[2][2]);

    	matx[2][0] = row[0];
    	matx[2][1] = row[1];
    	matx[2][2] = row[2];
    }
    return *this;
}
#endif

// Scaled outer product update (given scalar b, row vectors v1 and v2)
// this += b * transpose(v1) * v2
template <typename T>
template <typename S>
inline
void		 UT_Matrix3T<T>::outerproductUpdateT(T b, 
					       const UT_Vector3T<S> &v1,
					       const UT_Vector3T<S> &v2)
{
    T bv1;
    bv1 = b * v1.x();
    matx[0][0]+=bv1*v2.x();
    matx[0][1]+=bv1*v2.y();
    matx[0][2]+=bv1*v2.z();
    bv1 = b * v1.y();
    matx[1][0]+=bv1*v2.x();
    matx[1][1]+=bv1*v2.y();
    matx[1][2]+=bv1*v2.z();
    bv1 = b * v1.z();
    matx[2][0]+=bv1*v2.x();
    matx[2][1]+=bv1*v2.y();
    matx[2][2]+=bv1*v2.z();
}

template <typename T>
inline
const UT_Vector3T<T> &UT_Matrix3T<T>::operator[](unsigned row) const
{
    UT_ASSERT_P(row < 3);
    return *(const UT_Vector3T<T>*)(matx[row]);
}

template <typename T>
inline
UT_Vector3T<T> &UT_Matrix3T<T>::operator[](unsigned row)
{
    UT_ASSERT_P(row < 3);
    return *(UT_Vector3T<T>*)(matx[row]);
}


// Free floating functions:
template <typename T, typename S>
inline
UT_Matrix3T<T>      operator+(const UT_Vector3T<S> &vec, const UT_Matrix3T<T> &mat)
{
    return mat+vec;
}

template <typename T, typename S>
inline
UT_Matrix3T<T>      operator*(S sc, const UT_Matrix3T<T> &m1)
{
    return m1*sc;
}

template <typename T, typename S>
inline
UT_Matrix3T<T>      operator/(const UT_Matrix3T<T> &m1, S scalar)
{
    return (m1 * (1.0f/scalar));
}

/// Multiplication of a row or column vector by a matrix (ie. right vs. left
/// multiplication respectively). The operator*() declared above is an alias 
/// for rowVecMult(). The functions that take a 4x4 matrix first extend
/// the vector to 4D (with a trailing 1.0 element).
//
// @{
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
// - inlining is explicitly requested only for non-deprecated functions
// involving the native types (UT_Vector3 and UT_Matrix3)

template <typename T, typename S>
inline UT_Vector3T<T> rowVecMult(const UT_Vector3T<T> &v, const UT_Matrix3T<S> &m);
template <typename T, typename S>
inline UT_Vector3T<T> colVecMult(const UT_Matrix3T<S> &m, const UT_Vector3T<T> &v);
// @}

template <typename T, typename S>
inline
UT_Vector3T<T>	rowVecMult(const UT_Vector3T<T> &v, const UT_Matrix3T<S> &m)
{
    return UT_Vector3T<T>(
	v.x()*m(0,0) + v.y()*m(1,0) + v.z()*m(2,0),
	v.x()*m(0,1) + v.y()*m(1,1) + v.z()*m(2,1),
	v.x()*m(0,2) + v.y()*m(1,2) + v.z()*m(2,2)
    );
}

template <typename T, typename S>
inline
UT_Vector3T<T>	operator*(const UT_Vector3T<T> &v, const UT_Matrix3T<S> &m)
{
    return rowVecMult(v, m);
}

template <typename T, typename S>
inline
UT_Vector3T<T>	colVecMult(const UT_Matrix3T<S> &m, const UT_Vector3T<T> &v)
{
    return UT_Vector3T<T>(
	v.x()*m(0,0) + v.y()*m(0,1) + v.z()*m(0,2),
	v.x()*m(1,0) + v.y()*m(1,1) + v.z()*m(1,2),
	v.x()*m(2,0) + v.y()*m(2,1) + v.z()*m(2,2)
    );
}

template <typename T>
SYS_FORCE_INLINE void
UT_Vector3T<T>::rowVecMult(const UT_Matrix3F &m)
{ 
    operator=(::rowVecMult(*this, m));
}
template <typename T>
SYS_FORCE_INLINE void
UT_Vector3T<T>::rowVecMult(const UT_Matrix3D &m)
{ 
    operator=(::rowVecMult(*this, m));
}
template <typename T>
SYS_FORCE_INLINE void
UT_Vector3T<T>::colVecMult(const UT_Matrix3F &m)
{ 
    operator=(::colVecMult(m, *this));
}
template <typename T>
SYS_FORCE_INLINE void
UT_Vector3T<T>::colVecMult(const UT_Matrix3D &m)
{ 
    operator=(::colVecMult(m, *this));
}
template <typename T>
template <typename S>
SYS_FORCE_INLINE UT_Vector3T<T> &
UT_Vector3T<T>::operator*=(const UT_Matrix3T<S> &m)
{
    rowVecMult(m);
    return *this;
}
template <typename T>
template <typename S>
SYS_FORCE_INLINE void
UT_Vector3T<T>::multiplyT(const UT_Matrix3T<S> &mat)
{
    colVecMult(mat);
}
template <typename T>
template <typename S>
SYS_FORCE_INLINE void
UT_Vector3T<T>::multiplyT(UT_Vector3T<T> &dest, const UT_Matrix3T<S> &mat) const
{
    dest = ::colVecMult(mat, *this);
}
template <typename T>
template <typename S>
SYS_FORCE_INLINE void
UT_Vector3T<T>::multiply(UT_Vector3T<T> &dest, const UT_Matrix3T<S> &mat) const
{
    dest = ::rowVecMult(*this, mat);
}

template <typename T>
inline
UT_Matrix3T<T>	SYSmin(const UT_Matrix3T<T> &v1, const UT_Matrix3T<T> &v2)
{
    return UT_Matrix3T<T>(
	    SYSmin(v1(0,0), v2(0,0)),
	    SYSmin(v1(0,1), v2(0,1)),
	    SYSmin(v1(0,2), v2(0,2)),
	    SYSmin(v1(1,0), v2(1,0)),
	    SYSmin(v1(1,1), v2(1,1)),
	    SYSmin(v1(1,2), v2(1,2)),
	    SYSmin(v1(2,0), v2(2,0)),
	    SYSmin(v1(2,1), v2(2,1)),
	    SYSmin(v1(2,2), v2(2,2))
    );
}

template <typename T>
inline
UT_Matrix3T<T>	SYSmax(const UT_Matrix3T<T> &v1, const UT_Matrix3T<T> &v2)
{
    return UT_Matrix3T<T>(
	    SYSmax(v1(0,0), v2(0,0)),
	    SYSmax(v1(0,1), v2(0,1)),
	    SYSmax(v1(0,2), v2(0,2)),
	    SYSmax(v1(1,0), v2(1,0)),
	    SYSmax(v1(1,1), v2(1,1)),
	    SYSmax(v1(1,2), v2(1,2)),
	    SYSmax(v1(2,0), v2(2,0)),
	    SYSmax(v1(2,1), v2(2,1)),
	    SYSmax(v1(2,2), v2(2,2))
    );
}

template <typename T,typename S>
inline
UT_Matrix3T<T> SYSlerp(const UT_Matrix3T<T> &v1, const UT_Matrix3T<T> &v2, S t)
{
    return UT_Matrix3T<T>(
            SYSlerp(v1(0,0), v2(0,0), t),
            SYSlerp(v1(0,1), v2(0,1), t),
            SYSlerp(v1(0,2), v2(0,2), t),
            SYSlerp(v1(1,0), v2(1,0), t),
            SYSlerp(v1(1,1), v2(1,1), t),
            SYSlerp(v1(1,2), v2(1,2), t),
            SYSlerp(v1(2,0), v2(2,0), t),
            SYSlerp(v1(2,1), v2(2,1), t),
            SYSlerp(v1(2,2), v2(2,2), t)
    );
}

#ifndef UT_DISABLE_VECTORIZE_MATRIX
template <>
inline
UT_Matrix3T<float> SYSlerp(const UT_Matrix3T<float> &v1, const UT_Matrix3T<float> &v2, float t)
{
    UT_Matrix3T<float> result;
    {
    	const v4uf l(v1.data());
    	const v4uf r(v2.data());
    	vm_store(result.data(), SYSlerp(l, r, t).vector);
    }
    {
    	const v4uf l(v1.data() + 4);
    	const v4uf r(v2.data() + 4);
    	vm_store(result.data() + 4, SYSlerp(l, r, t).vector);
    }
    {
    	result(2,2) = SYSlerp(v1(2,2), v2(2,2), t);
    }
    return result;
}
#endif

template<typename T>
struct UT_FixedVectorTraits<UT_Matrix3T<T> >
{
    typedef UT_FixedVector<T,9> FixedVectorType;
    typedef T DataType;
    static const exint TupleSize = 9;
    static const bool isVectorType = true;
};

// Overload for custom formatting of UT_Matrix3T<T> with UTformat.
template<typename T>
UT_API size_t format(char *buffer, size_t buffer_size, const UT_Matrix3T<T> &v);

#endif

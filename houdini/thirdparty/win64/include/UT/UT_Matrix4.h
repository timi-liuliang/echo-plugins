/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#pragma once

#ifndef __UT_Matrix4_h__
#define __UT_Matrix4_h__

#include "UT_API.h"
#include "UT_Assert.h"
#include "UT_Axis.h"
#include "UT_FixedVector.h"
#include "UT_SymMatrix4.h"
#include "UT_Vector3.h"
#include "UT_Vector4.h"
#include "UT_VectorTypes.h"
#include "UT_XformOrder.h"

#include <SYS/SYS_Math.h>
#include <iosfwd>

#ifndef UT_DISABLE_VECTORIZE_MATRIX
#include <VM/VM_SIMD.h>
#endif


class UT_IStream;
class UT_JSONWriter;
class UT_JSONValue;
class UT_JSONParser;


// Free floating operators that return a UT_Matrix4 object. 
template <typename T>
inline UT_Matrix4T<T>	operator+(const UT_Matrix4T<T> &m1,  const UT_Matrix4T<T> &m2);
template <typename T>
inline UT_Matrix4T<T>	operator-(const UT_Matrix4T<T> &m1,  const UT_Matrix4T<T> &m2);
template <typename T>
inline UT_Matrix4T<T>	operator*(const UT_Matrix4T<T> &m1,  const UT_Matrix4T<T> &m2);
template <typename T, typename S>
inline UT_Matrix4T<T>	operator+(const UT_Matrix4T<T> &mat, const UT_Vector4T<S> &vec);
template <typename T, typename S>
inline UT_Matrix4T<T>	operator+(const UT_Vector4T<S> &vec, const UT_Matrix4T<T> &mat);
template <typename T, typename S>
inline UT_Matrix4T<T>	operator-(const UT_Matrix4T<T> &mat, const UT_Vector4T<S> &vec);
template <typename T, typename S>
inline UT_Matrix4T<T>	operator-(const UT_Vector4T<S> &vec, const UT_Matrix4T<T> &mat);
template <typename T, typename S>
inline UT_Matrix4T<T>	operator+(const UT_Matrix4T<T> &mat, S sc);
template <typename T, typename S>
inline UT_Matrix4T<T>	operator-(const UT_Matrix4T<T> &mat, S sc);
template <typename T, typename S>
inline UT_Matrix4T<T>	operator*(const UT_Matrix4T<T> &mat, S sc);
template <typename T, typename S>
inline UT_Matrix4T<T>	operator/(const UT_Matrix4T<T> &mat, S sc);
template <typename T, typename S>
inline UT_Matrix4T<T>	operator+(S sc, const UT_Matrix4T<T> &mat);
template <typename T, typename S>
inline UT_Matrix4T<T>	operator-(S sc, const UT_Matrix4T<T> &mat);
template <typename T, typename S>
inline UT_Matrix4T<T>	operator*(S sc, const UT_Matrix4T<T> &mat);
template <typename T, typename S>
inline UT_Matrix4T<T>	operator/(S sc, const UT_Matrix4T<T> &mat);

template <typename T>
inline UT_Matrix4T<T>	SYSmin	(const UT_Matrix4T<T> &v1, const UT_Matrix4T<T> &v2);
template <typename T>
inline UT_Matrix4T<T>	SYSmax	(const UT_Matrix4T<T> &v1, const UT_Matrix4T<T> &v2);
template <typename T,typename S>
inline UT_Matrix4T<T> SYSlerp(const UT_Matrix4T<T> &v1, const UT_Matrix4T<T> &v2, S t);
template <typename T,typename S>
inline UT_Matrix4T<T> SYSbilerp(const UT_Matrix4T<T> &u0v0, const UT_Matrix4T<T> &u1v0,
                                  const UT_Matrix4T<T> &u0v1, const UT_Matrix4T<T> &u1v1,
                                  S u, S v)
{ return SYSlerp(SYSlerp(u0v0, u0v1, v), SYSlerp(u1v0, u1v1, v), u); }

/// This class implements a 4x4 fpreal matrix in row-major order.
///
/// Most of Houdini operates with row vectors that are left-multiplied with
/// matrices. e.g.,   z = v * M
///   As a result, translation data is in row 3 of the matrix, rather than
/// column 3.
template <typename T>
class UT_API UT_Matrix4T
{
public:

    typedef T value_type;
    static constexpr const int tuple_size = 16;

    /// Construct uninitialized matrix.
    SYS_FORCE_INLINE UT_Matrix4T() = default;

    /// Default copy constructor
    constexpr UT_Matrix4T(const UT_Matrix4T &) = default;

    /// Default move constructor
    constexpr UT_Matrix4T(UT_Matrix4T &&) = default;

    /// Construct identity matrix, multipled by scalar.
    explicit constexpr UT_Matrix4T(fpreal64 val) noexcept
        : matx{
            {T(val),0,0,0},
            {0,T(val),0,0},
            {0,0,T(val),0},
            {0,0,0,T(val)}}
    {
        SYS_STATIC_ASSERT(sizeof(UT_Matrix4T<T>) == tuple_size * sizeof(T));
    }
    /// Construct a deep copy of the input row-major data.
    /// @{
    template <typename S>
    explicit constexpr UT_Matrix4T(const S m[4][4]) noexcept
        : matx{
            {T(m[0][0]),T(m[0][1]),T(m[0][2]),T(m[0][3])},
            {T(m[1][0]),T(m[1][1]),T(m[1][2]),T(m[1][3])},
            {T(m[2][0]),T(m[2][1]),T(m[2][2]),T(m[2][3])},
            {T(m[3][0]),T(m[3][1]),T(m[3][2]),T(m[3][3])}}
    {}
    /// @}

    /// Arbitrary UT_FixedVector of the same size
    template <typename S>
    SYS_FORCE_INLINE explicit UT_Matrix4T(const UT_FixedVector<S, tuple_size> &v)
    {
        matx[0][0]=v[0]; matx[0][1]=v[1]; matx[0][2]=v[2]; matx[0][3]=v[3];
        matx[1][0]=v[4]; matx[1][1]=v[5]; matx[1][2]=v[6]; matx[1][3]=v[7];
        matx[2][0]=v[8]; matx[2][1]=v[9]; matx[2][2]=v[10];matx[2][3]=v[11];
        matx[3][0]=v[12];matx[3][1]=v[13];matx[3][2]=v[14];matx[3][3]=v[15];
    }

    /// This constructor is for convenience.
    UT_Matrix4T(T val00, T val01, T val02, T val03,
	       T val10, T val11, T val12, T val13,
	       T val20, T val21, T val22, T val23,
	       T val30, T val31, T val32, T val33)
    {
	matx[0][0] = val00; matx[0][1] = val01; matx[0][2] = val02;
							    matx[0][3] = val03;
	matx[1][0] = val10; matx[1][1] = val11; matx[1][2] = val12;
							    matx[1][3] = val13;
	matx[2][0] = val20; matx[2][1] = val21; matx[2][2] = val22;
							    matx[2][3] = val23;
	matx[3][0] = val30; matx[3][1] = val31; matx[3][2] = val32;
							    matx[3][3] = val33;
    }

    /// Base type conversion constructor
    template <typename S>
    explicit UT_Matrix4T(const UT_Matrix4T<S> &m)
    {
	matx[0][0]=m(0,0); matx[0][1]=m(0,1); matx[0][2]=m(0,2); matx[0][3]=m(0,3);
	matx[1][0]=m(1,0); matx[1][1]=m(1,1); matx[1][2]=m(1,2); matx[1][3]=m(1,3);
	matx[2][0]=m(2,0); matx[2][1]=m(2,1); matx[2][2]=m(2,2); matx[2][3]=m(2,3);
	matx[3][0]=m(3,0); matx[3][1]=m(3,1); matx[3][2]=m(3,2); matx[3][3]=m(3,3);
    }
    template <typename S>
    explicit UT_Matrix4T(const UT_Matrix3T<S> &m)
    {
        matx[0][0]=m(0,0); matx[0][1]=m(0,1); matx[0][2]=m(0,2); matx[0][3]=(T)0.;
        matx[1][0]=m(1,0); matx[1][1]=m(1,1); matx[1][2]=m(1,2); matx[1][3]=(T)0.;
        matx[2][0]=m(2,0); matx[2][1]=m(2,1); matx[2][2]=m(2,2); matx[2][3]=(T)0.;
        matx[3][0]=(T)0.;  matx[3][1]=(T)0.;  matx[3][2]=(T)0.;  matx[3][3]=(T)1.;
    }

    template<typename S>
    UT_Matrix4T(const UT_SymMatrix4T<S> &m)
    {
	*this = m;
    }

    /// Default copy assignment operator
    UT_Matrix4T<T> &operator=(const UT_Matrix4T<T> &m) = default;

    /// Default move assignment operator
    UT_Matrix4T<T> &operator=(UT_Matrix4T<T> &&m) = default;

    /// Conversion operator that expands a 3x3 into a 4x4 matrix by adding a
    /// row and column of zeroes, except the diagonal element which is 1.
    // @{
    template <typename S>
    UT_Matrix4T<T>	&operator=(const UT_Matrix3T<S> &m)
    {
        matx[0][0]=m(0,0); matx[0][1]=m(0,1); 
        matx[0][2]=m(0,2); matx[0][3]=(T)0.;
        matx[1][0]=m(1,0); matx[1][1]=m(1,1); 
        matx[1][2]=m(1,2); matx[1][3]=(T)0.;
        matx[2][0]=m(2,0); matx[2][1]=m(2,1); 
        matx[2][2]=m(2,2); matx[2][3]=(T)0.;
        matx[3][0]=(T)0.; matx[3][1]=(T)0.;
        matx[3][2]=(T)0.; matx[3][3]=(T)1.;
        return *this;
    }
    // @}
    template <typename S>
    UT_Matrix4T<T>	&operator=(const UT_Matrix4T<S> &m)
    {
	matx[0][0]=m(0,0); matx[0][1]=m(0,1); 
	matx[0][2]=m(0,2); matx[0][3]=m(0,3);
	matx[1][0]=m(1,0); matx[1][1]=m(1,1); 
	matx[1][2]=m(1,2); matx[1][3]=m(1,3);
	matx[2][0]=m(2,0); matx[2][1]=m(2,1); 
	matx[2][2]=m(2,2); matx[2][3]=m(2,3);
	matx[3][0]=m(3,0); matx[3][1]=m(3,1); 
	matx[3][2]=m(3,2); matx[3][3]=m(3,3);
	return *this;
    }

    /// Conversion from a symmetric to a non symmetric matrix
    template <typename S>
    UT_Matrix4T<T>	&operator=(const UT_SymMatrix4T<S> &m)
    {
	matx[0][0] = m.q00;	matx[0][1] = m.q01;	matx[0][2] = m.q02;
	matx[0][3] = m.q03;	matx[1][0] = m.q01;	matx[1][1] = m.q11;
	matx[1][2] = m.q12;	matx[1][3] = m.q13;	matx[2][0] = m.q02;
	matx[2][1] = m.q12;	matx[2][2] = m.q22;	matx[2][3] = m.q23;
	matx[3][0] = m.q03;	matx[3][1] = m.q13;	matx[3][2] = m.q23;
	matx[3][3] = m.q33;
	return *this;
    }

    UT_Matrix4T<T>   operator-() const
                 {
                     return UT_Matrix4T<T>(
                        -matx[0][0], -matx[0][1], -matx[0][2], -matx[0][3],
                        -matx[1][0], -matx[1][1], -matx[1][2], -matx[1][3],
                        -matx[2][0], -matx[2][1], -matx[2][2], -matx[2][3],
                        -matx[3][0], -matx[3][1], -matx[3][2], -matx[3][3]);
                 }
    
    SYS_FORCE_INLINE
    UT_Matrix4T<T>  &operator+=(const UT_Matrix4T<T> &m)
		{
		    matx[0][0]+=m.matx[0][0]; matx[0][1]+=m.matx[0][1]; 
		    matx[0][2]+=m.matx[0][2]; matx[0][3]+=m.matx[0][3];

		    matx[1][0]+=m.matx[1][0]; matx[1][1]+=m.matx[1][1]; 
		    matx[1][2]+=m.matx[1][2]; matx[1][3]+=m.matx[1][3];

		    matx[2][0]+=m.matx[2][0]; matx[2][1]+=m.matx[2][1]; 
		    matx[2][2]+=m.matx[2][2]; matx[2][3]+=m.matx[2][3];

		    matx[3][0]+=m.matx[3][0]; matx[3][1]+=m.matx[3][1];
		    matx[3][2]+=m.matx[3][2]; matx[3][3]+=m.matx[3][3];
		    return *this;
		}
    SYS_FORCE_INLINE
    UT_Matrix4T<T>  &operator-=(const UT_Matrix4T<T> &m)
		{
		    matx[0][0]-=m.matx[0][0]; matx[0][1]-=m.matx[0][1]; 
		    matx[0][2]-=m.matx[0][2]; matx[0][3]-=m.matx[0][3];

		    matx[1][0]-=m.matx[1][0]; matx[1][1]-=m.matx[1][1]; 
		    matx[1][2]-=m.matx[1][2]; matx[1][3]-=m.matx[1][3];

		    matx[2][0]-=m.matx[2][0]; matx[2][1]-=m.matx[2][1]; 
		    matx[2][2]-=m.matx[2][2]; matx[2][3]-=m.matx[2][3];

		    matx[3][0]-=m.matx[3][0]; matx[3][1]-=m.matx[3][1];
		    matx[3][2]-=m.matx[3][2]; matx[3][3]-=m.matx[3][3];
		    return *this;
		}
    template<typename S>
    inline UT_Matrix4T<T> &operator*=(const UT_Matrix4T<S> &m);
    template<typename S>
    inline UT_Matrix4T<T> &operator*=(const UT_Matrix3T<S> &m);

    // test for exact floating point equality.  
    // for equality within a threshold, see isEqual()
    bool	operator==(const UT_Matrix4T<T> &m) const
		{
		    return (&m == this) || (
		    matx[0][0]==m.matx[0][0] && matx[0][1]==m.matx[0][1] &&
		    matx[0][2]==m.matx[0][2] && matx[0][3]==m.matx[0][3] &&

		    matx[1][0]==m.matx[1][0] && matx[1][1]==m.matx[1][1] &&
		    matx[1][2]==m.matx[1][2] && matx[1][3]==m.matx[1][3] &&

		    matx[2][0]==m.matx[2][0] && matx[2][1]==m.matx[2][1] &&
		    matx[2][2]==m.matx[2][2] && matx[2][3]==m.matx[2][3] &&

		    matx[3][0]==m.matx[3][0] && matx[3][1]==m.matx[3][1] &&
		    matx[3][2]==m.matx[3][2] && matx[3][3]==m.matx[3][3] );
		}

    bool	operator!=(const UT_Matrix4T<T> &m) const
		{
		    return !(*this == m);
		}

    // Scalar operators:
    UT_Matrix4T<T>  &operator= (fpreal64 v)
		{
		    matx[0][0]= v; matx[0][1]= 0; matx[0][2]= 0; matx[0][3]= 0;
		    matx[1][0]= 0; matx[1][1]= v; matx[1][2]= 0; matx[1][3]= 0;
		    matx[2][0]= 0; matx[2][1]= 0; matx[2][2]= v; matx[2][3]= 0;
		    matx[3][0]= 0; matx[3][1]= 0; matx[3][2]= 0; matx[3][3]= v;
		    return *this;
		}
    /// NOTE: DO NOT use this for scaling the transform,
    ///       since this scales the w column (3) as well,
    ///       causing problems with translation later.
    ///       Use M.scale(scalar) instead.
    SYS_FORCE_INLINE
    UT_Matrix4T<T>  &operator*=(T scalar)
		{
		    matx[0][0]*=scalar; matx[0][1]*=scalar; 
		    matx[0][2]*=scalar; matx[0][3]*=scalar;

		    matx[1][0]*=scalar; matx[1][1]*=scalar; 
		    matx[1][2]*=scalar; matx[1][3]*=scalar;

		    matx[2][0]*=scalar; matx[2][1]*=scalar; 
		    matx[2][2]*=scalar; matx[2][3]*=scalar;

		    matx[3][0]*=scalar; matx[3][1]*=scalar; 
		    matx[3][2]*=scalar; matx[3][3]*=scalar;
		    return *this;
		}
    SYS_FORCE_INLINE
    UT_Matrix4T<T>  &operator/=(T scalar)
		{
		    return operator*=( 1.0f/scalar );
		}

    // Vector4 operators:
    template <typename S>
    inline UT_Matrix4T<T>  &operator=(const UT_Vector4T<S> &vec);
    template <typename S>
    inline UT_Matrix4T<T>  &operator+=(const UT_Vector4T<S> &vec);
    template <typename S>
    inline UT_Matrix4T<T>  &operator-=(const UT_Vector4T<S> &vec);

    // Other matrix operations:

    /// Multiply the passed-in matrix (on the left) by this (on the right)
    /// and assign the result to this.
    /// (operator*= does right-multiplication)
    /// @{
    inline void	leftMult( const UT_Matrix4T<T> &m );
    void	preMultiply(const UT_Matrix4T<T> &m)	{ leftMult(m); }
    /// @}

    // Return the cofactor of the matrix, ie the determinant of the 3x3
    // submatrix that results from removing row 'k' and column 'l' from the 
    // 4x4.
    SYS_FORCE_INLINE T	coFactor(int k, int l) const
		{
		    int		r[3], c[3];
		    T		det;

		    // r, c should evaluate to compile time constants
		    coVals(k, r);
		    coVals(l, c);

		    det = matx[r[0]][c[0]]*
			     (matx[r[1]][c[1]]*matx[r[2]][c[2]]-
			      matx[r[1]][c[2]]*matx[r[2]][c[1]]) +
			  matx[r[0]][c[1]]*
			     (matx[r[1]][c[2]]*matx[r[2]][c[0]]-
			      matx[r[1]][c[0]]*matx[r[2]][c[2]]) +
			  matx[r[0]][c[2]]*
			     (matx[r[1]][c[0]]*matx[r[2]][c[1]]-
			      matx[r[1]][c[1]]*matx[r[2]][c[0]]);
		    
		    if ((k ^ l) & 1)
			det = -det;

		    return det;
		}

    T		determinant() const
		{
		    return(matx[0][0]*coFactor(0,0) +
			   matx[0][1]*coFactor(0,1) +
			   matx[0][2]*coFactor(0,2) +
			   matx[0][3]*coFactor(0,3) );
		}
    /// Compute determinant of the upper-left 3x3 sub-matrix
    T		determinant3() const
		{
		    return(matx[0][0]*
			       (matx[1][1]*matx[2][2]-matx[1][2]*matx[2][1]) +
			   matx[0][1]*
			       (matx[1][2]*matx[2][0]-matx[1][0]*matx[2][2]) +
			   matx[0][2]*
			       (matx[1][0]*matx[2][1]-matx[1][1]*matx[2][0]) );

		}
    T	      trace() const
                { return matx[0][0]+matx[1][1]+matx[2][2]+matx[3][3]; }

    /// Invert this matrix and return 0 if OK, 1 if singular.
    // @{
    int		invert(T tol = 0.0F);
    int		invertDouble();
    // @}

    /// Invert the matrix and return 0 if OK, 1 if singular.
    /// Puts the inverted matrix in m, and leaves this matrix unchanged.
    // @{
    int		invert(UT_Matrix4T<T> &m) const;
    int		invertDouble(UT_Matrix4T<T> &m) const;
    // @}
    int		invertKramer(void);
    int		invertKramer(UT_Matrix4T<T> &m)const;

    // Computes a transform to orient to a given direction (v) at a given
    // position (p) and with a scale (s). The up vector (up) is optional
    // and will orient the matrix to this up vector. If no up vector is given,
    // the z axis will be oriented to point in the v direction. If a
    // quaternion (q) is specified, the orientation will be additionally
    // transformed by the rotation specified by the quaternion. If a
    // translation (tr) is specified, the entire frame of reference will
    // be moved by this translation (unaffected by the scale or rotation).
    // If an orientation (orient) is specified, the orientation (using the
    // velocity and up vector) will not be performed and this orientation will 
    // instead be used to define an original orientation.
    //
    // The matrix is scaled non-uniformly about each axis using s3, if s3
    // is non-zero.  A uniform scale of pscale is applied regardless, so if
    // s3 is non-zero, the x axis will be scaled by pscale * s3->x().
    template <typename S>
    void instanceT(const UT_Vector3T<S>& p, const UT_Vector3T<S>& v, T s,
		  const UT_Vector3T<S>* s3,
		  const UT_Vector3T<S>* up, const UT_QuaternionT<S>* q,
		  const UT_Vector3T<S>* tr, const UT_QuaternionT<S>* orient,
		  const UT_Vector3T<S>* pivot);
    void instance(const UT_Vector3F& p, const UT_Vector3F& v, T s,
		  const UT_Vector3F* s3,
		  const UT_Vector3F* up, const UT_QuaternionF* q,
		  const UT_Vector3F* tr, const UT_QuaternionF* orient,
		  const UT_Vector3F* pivot = NULL )
    { instanceT(p, v, s, s3, up, q, tr, orient, pivot); }
    void instance(const UT_Vector3D& p, const UT_Vector3D& v, T s,
		  const UT_Vector3D* s3,
		  const UT_Vector3D* up, const UT_QuaternionD* q,
		  const UT_Vector3D* tr, const UT_QuaternionD* orient,
		  const UT_Vector3D* pivot = NULL )
    { instanceT(p, v, s, s3, up, q, tr, orient, pivot); }
    
    template <typename S>
    void instanceInverseT(const UT_Vector3T<S>& p, const UT_Vector3T<S>& v, T s,
		  const UT_Vector3T<S>* s3,
		  const UT_Vector3T<S>* up, const UT_QuaternionT<S>* q,
		  const UT_Vector3T<S>* tr, const UT_QuaternionT<S>* orient,
		  const UT_Vector3T<S>* pivot);
    void instanceInverse(const UT_Vector3F& p, const UT_Vector3F& v, T s,
		  const UT_Vector3F* s3,
		  const UT_Vector3F* up, const UT_QuaternionF* q,
		  const UT_Vector3F* tr, const UT_QuaternionF* orient,
		  const UT_Vector3F* pivot = NULL )
    { instanceInverseT(p, v, s, s3, up, q, tr, orient, pivot); }
    void instanceInverse(const UT_Vector3D& p, const UT_Vector3D& v, T s,
		  const UT_Vector3D* s3,
		  const UT_Vector3D* up, const UT_QuaternionD* q,
		  const UT_Vector3D* tr, const UT_QuaternionD* orient,
		  const UT_Vector3D* pivot = NULL )
    { instanceInverseT(p, v, s, s3, up, q, tr, orient, pivot); }

    // Transpose this matrix or return its transpose.
    void	transpose(void)
		{
		    T tmp;
		    tmp=matx[0][1];  matx[0][1]=matx[1][0];  matx[1][0]=tmp;
		    tmp=matx[0][2];  matx[0][2]=matx[2][0];  matx[2][0]=tmp;
		    tmp=matx[0][3];  matx[0][3]=matx[3][0];  matx[3][0]=tmp;
		    tmp=matx[1][2];  matx[1][2]=matx[2][1];  matx[2][1]=tmp;
		    tmp=matx[1][3];  matx[1][3]=matx[3][1];  matx[3][1]=tmp;
		    tmp=matx[2][3];  matx[2][3]=matx[3][2];  matx[3][2]=tmp;
		}

    // check for equality within a tolerance level
    bool	isEqual( const UT_Matrix4T<T> &m,
			 T tolerance=T(SYS_FTOLERANCE) ) const
		{
		    return (&m == this) || (
		    SYSisEqual( matx[0][0], m.matx[0][0], tolerance ) &&
		    SYSisEqual( matx[0][1], m.matx[0][1], tolerance ) &&
		    SYSisEqual( matx[0][2], m.matx[0][2], tolerance ) &&
		    SYSisEqual( matx[0][3], m.matx[0][3], tolerance ) &&

		    SYSisEqual( matx[1][0], m.matx[1][0], tolerance ) &&
		    SYSisEqual( matx[1][1], m.matx[1][1], tolerance ) &&
		    SYSisEqual( matx[1][2], m.matx[1][2], tolerance ) &&
		    SYSisEqual( matx[1][3], m.matx[1][3], tolerance ) &&

		    SYSisEqual( matx[2][0], m.matx[2][0], tolerance ) &&
		    SYSisEqual( matx[2][1], m.matx[2][1], tolerance ) &&
		    SYSisEqual( matx[2][2], m.matx[2][2], tolerance ) &&
		    SYSisEqual( matx[2][3], m.matx[2][3], tolerance ) &&

		    SYSisEqual( matx[3][0], m.matx[3][0], tolerance ) &&
		    SYSisEqual( matx[3][1], m.matx[3][1], tolerance ) &&
		    SYSisEqual( matx[3][2], m.matx[3][2], tolerance ) &&
		    SYSisEqual( matx[3][3], m.matx[3][3], tolerance ) );
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

    /// Post-multiply this matrix by a 3x3 rotation matrix (on the right)
    /// for a quarter turn (90 degrees) around the specified axis
    template<UT_Axis3::axis A,bool reverse=false>
    SYS_FORCE_INLINE void rotateQuarter()
    {
        constexpr uint col0 = (A == UT_Axis3::XAXIS) ? 1 : ((A == UT_Axis3::YAXIS) ? 2 : 0);
        constexpr uint col1 = (A == UT_Axis3::XAXIS) ? 2 : ((A == UT_Axis3::YAXIS) ? 0 : 1);
        for (uint row = 0; row < 4; ++row)
        {
            T v1 = matx[row][col0];
            if (!reverse)
            {
                matx[row][col0] = -matx[row][col1];
                matx[row][col1] = v1;
            }
            else
            {
                matx[row][col0] = matx[row][col1];
                matx[row][col1] = -v1;
            }
        }
    }

    /// Post-multiply this matrix by a 3x3 rotation matrix (on the right)
    /// for a half turn (180 degrees) around the specified axis
    template<UT_Axis3::axis A>
    SYS_FORCE_INLINE void rotateHalf()
    {
        // In this case, order doesn't matter, so make col0 and col1 in increasing order.
        constexpr uint col0 = (A == UT_Axis3::XAXIS) ? 1 : 0;
        constexpr uint col1 = (A == UT_Axis3::ZAXIS) ? 1 : 2;
        for (uint row = 0; row < 4; ++row)
        {
            matx[row][col0] = -matx[row][col0];
            matx[row][col1] = -matx[row][col1];
        }
    }

    /// Create a rotation matrix for the given angle in radians around the axis
    /// @{
    template <typename S>
    static UT_Matrix4T<T> rotationMat(UT_Vector3T<S> &axis, T theta, int norm=1);
    static UT_Matrix4T<T> rotationMat(UT_Axis3::axis a, T theta);
    /// @}

    /// Pre-multiply this matrix by a 3x3 rotation matrix determined by the
    /// axis and angle of rotation in radians.
    /// If 'norm' is not 0, the axis vector is normalized before computing the
    /// rotation matrix. rotationMat() returns a rotation matrix, and could as
    /// well be defined as a free floating function.
    /// @{
    template <typename S>
    void prerotate(UT_Vector3T<S> &axis, T theta, int norm=1);
    void prerotate(UT_Axis3::axis a, T theta);
    template<UT_Axis3::axis A>
    void prerotate(T theta);
    /// @}

    /// Pre-multiply this matrix by a 3x3 rotation matrix (on the left)
    /// for a quarter turn (90 degrees) around the specified axis
    template<UT_Axis3::axis A,bool reverse=false>
    SYS_FORCE_INLINE void prerotateQuarter()
    {
        constexpr uint row0 = (A == UT_Axis3::XAXIS) ? 1 : ((A == UT_Axis3::YAXIS) ? 2 : 0);
        constexpr uint row1 = (A == UT_Axis3::XAXIS) ? 2 : ((A == UT_Axis3::YAXIS) ? 0 : 1);
        T v1[4];
        for (uint col = 0; col < 4; ++col)
            v1[col] = matx[row0][col];
        if (!reverse)
        {
            for (uint col = 0; col < 4; ++col)
                matx[row0][col] = matx[row1][col];
            for (uint col = 0; col < 4; ++col)
                matx[row1][col] = -v1[col];
        }
        else
        {
            for (uint col = 0; col < 4; ++col)
                matx[row0][col] = -matx[row1][col];
            for (uint col = 0; col < 4; ++col)
                matx[row1][col] = v1[col];
        }
    }

    /// Pre-multiply this matrix by a 3x3 rotation matrix (on the left)
    /// for a half turn (180 degrees) around the specified axis
    template<UT_Axis3::axis A>
    SYS_FORCE_INLINE void prerotateHalf()
    {
        // In this case, order doesn't matter, so make row0 and row1 in increasing order.
        constexpr uint row0 = (A == UT_Axis3::XAXIS) ? 1 : 0;
        constexpr uint row1 = (A == UT_Axis3::ZAXIS) ? 1 : 2;
        for (uint col = 0; col < 4; ++col)
            matx[row0][col] = -matx[row0][col];
        for (uint col = 0; col < 4; ++col)
            matx[row1][col] = -matx[row1][col];
    }

    /// Post-rotate by rx, ry, rz radians around the three basic axes in the
    /// order given by UT_XformOrder.
    /// @{
    void	rotate(T rx, T ry, T rz, const UT_XformOrder &ord);
    SYS_FORCE_INLINE
    void	rotate(const UT_Vector3T<T> &rad, const UT_XformOrder &ord)
			{ rotate(rad(0), rad(1), rad(2), ord); }
    /// @}

    /// Pre-rotate by rx, ry, rz radians around the three basic axes in the
    /// order given by UT_XformOrder.
    /// @{
    void	prerotate(T rx, T ry, T rz, 
			  const UT_XformOrder &ord);
    SYS_FORCE_INLINE
    void	prerotate(const UT_Vector3T<T> &rad, const UT_XformOrder &ord)
		    { prerotate(rad(0), rad(1), rad(2), ord); }
    /// @}

    /// Post-multiply this matrix by a scale matrix with diagonal (sx, sy, sz)
    /// @{
    void	scale(T sx, T sy, T sz, T sw = 1.0f)
		{
		    matx[0][0] *= sx; matx[0][1] *= sy; 
		    matx[0][2] *= sz; matx[0][3] *= sw;

		    matx[1][0] *= sx; matx[1][1] *= sy; 
		    matx[1][2] *= sz; matx[1][3] *= sw;

		    matx[2][0] *= sx; matx[2][1] *= sy; 
		    matx[2][2] *= sz; matx[2][3] *= sw;

		    matx[3][0] *= sx; matx[3][1] *= sy;
		    matx[3][2] *= sz; matx[3][3] *= sw;
		}
    SYS_FORCE_INLINE void scale(const UT_Vector3T<T> &s)
    { scale(s(0), s(1), s(2)); }
    SYS_FORCE_INLINE void scale(T s)
    { scale(s, s, s); }
    /// @}

    /// Pre-multiply this matrix by a scale matrix with diagonal (sx, sy, sz)
    /// @{
    void	prescale(T sx, T sy, T sz, T sw = 1.0f)
		{
		    matx[0][0] *= sx; matx[1][0] *= sy; 
		    matx[2][0] *= sz; matx[3][0] *= sw;

		    matx[0][1] *= sx; matx[1][1] *= sy; 
		    matx[2][1] *= sz; matx[3][1] *= sw;

		    matx[0][2] *= sx; matx[1][2] *= sy; 
		    matx[2][2] *= sz; matx[3][2] *= sw;

		    matx[0][3] *= sx; matx[1][3] *= sy; 
		    matx[2][3] *= sz; matx[3][3] *= sw;
		}
    SYS_FORCE_INLINE void prescale(const UT_Vector3T<T> &s)
    { prescale(s(0), s(1), s(2)); }
    SYS_FORCE_INLINE void prescale(T s)
    { prescale(s, s, s); }
    /// @}

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

	matx[3][0] += matx[3][1]*s_xy + matx[3][2]*s_xz;
	matx[3][1] += matx[3][2]*s_yz;
    }
    SYS_FORCE_INLINE
    void	shear(const UT_Vector3T<T> &sh)
		    { shear(sh(0), sh(1), sh(2)); }
    /// @}

    /// Post-multiply this matrix by the translation determined by dx, dy, dz.
    /// @{
    void	translate(T dx, T dy, T dz = 0.0f)
		{
		    T a;
		    a = matx[0][3];
		    matx[0][0] += a*dx; matx[0][1] += a*dy; matx[0][2] += a*dz;
		    a = matx[1][3];
		    matx[1][0] += a*dx; matx[1][1] += a*dy; matx[1][2] += a*dz;
		    a = matx[2][3];
		    matx[2][0] += a*dx; matx[2][1] += a*dy; matx[2][2] += a*dz;
		    a = matx[3][3];
		    matx[3][0] += a*dx; matx[3][1] += a*dy; matx[3][2] += a*dz;
		}
    SYS_FORCE_INLINE
    void	translate(const UT_Vector3T<T> &delta)
		    { translate(delta(0), delta(1), delta(2)); }
    /// @}

    /// Pre-multiply this matrix by the translation determined by dx, dy, dz.
    /// @{
    void	pretranslate(T dx, T dy, T dz = 0.0f)
		{
		    matx[3][0] += matx[0][0]*dx + matx[1][0]*dy + matx[2][0]*dz;
		    matx[3][1] += matx[0][1]*dx + matx[1][1]*dy + matx[2][1]*dz;
		    matx[3][2] += matx[0][2]*dx + matx[1][2]*dy + matx[2][2]*dz;
		    matx[3][3] += matx[0][3]*dx + matx[1][3]*dy + matx[2][3]*dz;
		}
    SYS_FORCE_INLINE
    void	pretranslate(const UT_Vector3T<T> &delta)
		    { pretranslate(delta(0), delta(1), delta(2)); }
    /// @}

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
    // translations (tx,ty,tz), degree rotations (rx,ry,rz), scales (sx,sy,sz),
    // and possibly a pivot point (px,py,pz). The second methos leaves us
    // unchanged, and returns a new (this*xform) instead. The order of the 
    // multiplies (SRT, RST, RxRyRz, etc) is stored in 'order'. Normally you
    // will ignore the 'reverse' parameter, which tells us to build the
    // matrix from last to first xform, and to apply some inverses to 
    // txyz, rxyz, and sxyz.
    void	xform(const UT_XformOrder &order, 
		      T tx=0.0f, T ty=0.0f, T tz=0.0f,
		      T rx=0.0f, T ry=0.0f, T rz=0.0f,
		      T sx=1.0f, T sy=1.0f, T sz=1.0f,
		      T px=0.0f, T py=0.0f, T pz=0.0f,
		      int reverse=0);

    // This version handles shears as well
    void	xform(const UT_XformOrder &order, 
		      T tx, T ty, T tz,
		      T rx, T ry, T rz,
		      T sx, T sy, T sz,
		      T s_xy, T s_xz, T s_yz,
		      T px, T py, T pz,
		      int reverse=0);

    /// Define parameters for Houdini's pivot space.
    template <typename S>
    struct PivotSpaceT
    {
	/// Constructor with default values for data members
	PivotSpaceT()
	    : myTranslate(0, 0, 0)
	    , myRotate(0, 0, 0)
	{
	}

	/// Convenience constructor with translate and rotate.
	PivotSpaceT(const UT_Vector3T<S> &translate,
		    const UT_Vector3T<S> &rotate)
	    : myTranslate(translate)
	    , myRotate(rotate)
	{
	}

	UT_Vector3T<S>	 myTranslate;	// Translate (corresponds to px, py, pz)
	UT_Vector3T<S>	 myRotate;	// Rotation (degrees, XYZ order)
    };

    typedef PivotSpaceT<T>	 PivotSpace;

    // This version handles a more general PivotSpace.
    void	xform(const UT_XformOrder &order, 
		      T tx, T ty, T tz,
		      T rx, T ry, T rz,
		      T sx, T sy, T sz,
		      const PivotSpace &pivot,
		      int reverse=0);

    // This version handles a more general PivotSpace as well as shears.
    void	xform(const UT_XformOrder &order, 
		      T tx, T ty, T tz,
		      T rx, T ry, T rz,
		      T sx, T sy, T sz,
		      T s_xy, T s_xz, T s_yz,
		      const PivotSpace &pivot,
		      int reverse=0);

    /// Define parameters for Houdini's full transform model
    struct FullTransformModel
    {
	/// Constructor with default values for data members
	FullTransformModel()
	    : myOrder(UT_XformOrder::SRT, UT_XformOrder::XYZ)
	    , myTranslate(0, 0, 0)
	    , myRotateOffset(0, 0, 0)
	    , myParentRotate(0, 0, 0)
	    , myRotate(0, 0, 0)
	    , myChildRotate(0, 0, 0)
	    , myRotatePivot(0, 0, 0)
	    , myScaleOffset(0, 0, 0)
	    , myScale(1, 1, 1)
	    , myShear(0, 0, 0)
	    , myScalePivot(0, 0, 0)
	    , myPivot(0, 0, 0)
	    , myPivotRotate(0, 0, 0)
	{
	}

	UT_XformOrder	myOrder;	/// transform and rotation order
	UT_Vector3T<T>	myTranslate;	/// Translate
	UT_Vector3T<T>	myRotateOffset;	/// Rotation offset
	UT_Vector3T<T>	myParentRotate;	/// Parent rotation (degrees, XYZ order)
	UT_Vector3T<T>	myRotate;	/// Rotation (degrees, myOrder order)
	UT_Vector3T<T>	myChildRotate;	/// Child rotation (degrees, XYZ order)
	UT_Vector3T<T>	myRotatePivot;	/// Rotation pivot
	UT_Vector3T<T>	myScaleOffset;	/// Scale offset
	UT_Vector3T<T>	myScale;	/// Scale
	UT_Vector3T<T>	myShear;	/// Shear (within scale pivot)
	UT_Vector3T<T>	myScalePivot;	/// Scale pivot
	UT_Vector3T<T>	myPivot;	/// Overall pivot
	UT_Vector3T<T>	myPivotRotate;	/// Overall pivot rotation(degrees,XYZ)
    };

    void	xform(const FullTransformModel &parms, T min_abs_scale = T(0));

    // These versions of xform and rotate can be used to apply selected parts
    // of a transform. The applyType controls how far into the xform
    // it should go. For example: applyXform(order, BEFORE_EQUAL, 'T', ...)
    // will apply all the components of the transform up to and equal to the
    // translates (depending on UT_XformOrder)
    //
    // For xform char can be T, R, S, P, or p (P is for pivot)
    // For rotate char can be X, Y, or Z
    //
    // TODO add a reverse option

    enum applyType { BEFORE=1, EQUAL=2, AFTER=4, BEFORE_EQUAL=4, AFTER_EQUAL=6};
    void		 xform(const UT_XformOrder &order,
			       applyType type, char limit,
			       T tx, T ty, T tz,
			       T rx, T ry, T rz,
			       T sx, T sy, T sz,
			       T px, T py, T pz);

    void		 rotate(const UT_XformOrder &order,
				applyType type, char limit,
				T rx, T ry, T rz);

    // extract only the translates from the matrix;
    template <typename S>
    inline void	getTranslates(UT_Vector3T<S> &translates) const;
    template <typename S>
    inline void	setTranslates(const UT_Vector3T<S> &translates);

    // This is a super-crack that returns the translation, scale, and radian
    // rotation vectors given a transformation order and a valid xform matrix.
    // It returns 0 if succesful, and non-zero otherwise: 1 if the embedded
    // rotation matrix is invalid, 2 if the rotation order is invalid, 
    // and 3 for other problems. If any of the scaling values is 0, the method 
    // returns all zeroes, and a 0 return value.
    template <typename S>
    int		explodeT(const UT_XformOrder &order,
			UT_Vector3T<S> &r, UT_Vector3T<S> &s, UT_Vector3T<S> &t,
			UT_Vector3T<S> *shears) const;
    int		explode(const UT_XformOrder &order,
			UT_Vector3F &r, UT_Vector3F &s, UT_Vector3F &t,
			UT_Vector3F *shears = 0) const
    { return explodeT(order, r, s, t, shears); }
    int		explode(const UT_XformOrder &order,
			UT_Vector3D &r, UT_Vector3D &s, UT_Vector3D &t,
			UT_Vector3D *shears = 0) const
    { return explodeT(order, r, s, t, shears); }

    // This version of explode returns the t, r, and s, given a pivot value.
    template <typename S>
    int		explodeT(const UT_XformOrder &order,
			UT_Vector3T<S> &r, UT_Vector3T<S> &s,
			UT_Vector3T<S> &t, const UT_Vector3T<S> &p,
			UT_Vector3T<S> *shears) const;
    int		explode(const UT_XformOrder &order,
			UT_Vector3F &r, UT_Vector3F &s,
			UT_Vector3F &t, const UT_Vector3F &p,
			UT_Vector3F *shears = 0) const
    { return explodeT(order, r, s, t, p, shears); }
    int		explode(const UT_XformOrder &order,
			UT_Vector3D &r, UT_Vector3D &s,
			UT_Vector3D &t, const UT_Vector3D &p,
			UT_Vector3D *shears = 0) const
    { return explodeT(order, r, s, t, p, shears); }

    // This version of explode returns the t, r, and s, given a PivotSpace.
    template <typename S>
    int		explodeT(const UT_XformOrder &order,
			UT_Vector3T<S> &r, UT_Vector3T<S> &s,
			UT_Vector3T<S> &t, const PivotSpaceT<S> &p,
			UT_Vector3T<S> *shears) const;
    int		explode(const UT_XformOrder &order,
			UT_Vector3F &r, UT_Vector3F &s,
			UT_Vector3F &t, const PivotSpaceT<fpreal32> &p,
			UT_Vector3F *shears = 0) const
    { return explodeT(order, r, s, t, p, shears); }
    int		explode(const UT_XformOrder &order,
			UT_Vector3D &r, UT_Vector3D &s,
			UT_Vector3D &t, const PivotSpaceT<fpreal64> &p,
			UT_Vector3D *shears = 0) const
    { return explodeT(order, r, s, t, p, shears); }


    // These versions treat the matrix as only containing a 2D 
    // transformation, that is in x, y, with a rotate around z.
    template <typename S>
    int		explode2D(const UT_XformOrder &order,
			  T &r, UT_Vector2T<S> &s, UT_Vector2T<S> &t,
			  T *shears = 0) const;

    template <typename S>
    int		explode2D(const UT_XformOrder &order,
			T &r, UT_Vector2T<S> &s,
			UT_Vector2T<S> &t, const UT_Vector2T<S> &p,
			T *shears = 0) const;

    /// WARNING: This may not produce good results!  Instead, get the
    ///          UT_Matrix3 part and call UT_Matrix3T::makeRotationMatrix().
    template <typename S>
    void	extractRotate(UT_Matrix3T<S> &dst) const;

    /// Perform the polar decomposition of the 3x3 matrix M into an orthogonal
    /// matrix Q and an symmetric positive-semidefinite matrix S. This is more
    /// useful than explode() or extractRotate() when the desire is to blend
    /// transforms.  By default, it gives M=SQ, a left polar decomposition. If
    /// reverse is false, then it gives M=QS, a right polar decomposition.
    ///
    /// This method is similar to the UT_Matrix3 version except it only
    /// operates on the upper-right 3x3 portion.
    ///
    /// @pre    The upper-right 3x3 portion of *this is non-singular
    /// @post   The upper-right 3x3 porition = Q,
    ///	        and if stretch != 0: *stretch = S.
    /// @return True if successful
    bool	polarDecompose(
			UT_Matrix3T<T> *stretch = nullptr,
			bool reverse = true,
			const int max_iter = 64,
			const T rel_tol = FLT_EPSILON);

    /// Turn this matrix into the "closest" rigid transformation
    /// (only rotations and translations) matrix.
    ///
    /// It uses polarDecompose and then negates the matrix if
    /// there is a negative determinant (scale).  It returns false iff
    /// polarDecompose failed, possibly due to a singular matrix.
    ///
    /// This is currently the one true way to turn an arbitrary
    /// matrix4 into a rotation and translation matrix.  If that
    /// ever changes, put a warning here, and you may want to update
    /// UT_Matrix3::makeRotationMatrix too.
    bool        makeRigidMatrix(
                        UT_Matrix3T<T> *stretch = nullptr,
                        bool reverse = true,
                        const int max_iter = 64,
                        const T rel_tol = FLT_EPSILON);

    // Right multiply this matrix by a 3x3 matrix which scales by a given 
    // amount along the direction of vector v. When applied to a vector w,
    // the stretched matrix (*this) stretches w in v by the amount given.
    // If norm is non-zero, v will be normalized prior to the operation.
    template <typename S>
    void	stretch(UT_Vector3T<S> &v, T amount, int norm=1);

    T		dot(unsigned i, unsigned j) const
		{
		    return (i <= 3 && j <= 3) ?
		    matx[i][0]*matx[j][0] + matx[i][1]*matx[j][1] +
		    matx[i][2]*matx[j][2] + matx[i][3]*matx[j][3] : (T)0;
		}

    // Matrix += b * v1 * v2T
    template <typename S>
    void	outerproductUpdate(T b, 
			     const UT_Vector4T<S> &v1, const UT_Vector4T<S> &v2);

    // Sets the current matrix to a linear interpolation of the two homogenous
    // matrices given.
    void	lerp(const UT_Matrix4T<T> &a, const UT_Matrix4T<T> &b, T t)
    {
	if (t == 0)
	    *this = a;
	else if(t == 1)
	    *this = b;
	else
	{
	    for (size_t i = 0; i < tuple_size; i++)
		myFloats[i] = SYSlerp(a.myFloats[i], b.myFloats[i], t);
	}
    }
    
    /// Set the matrix to identity
    void	identity()	{ *this = 1; }
    /// Set the matrix to zero
    void	zero()	        { *this = 0; }

    int		isIdentity() const
		{
		    // NB: DO NOT USE TOLERANCES!
		    return( 
			matx[0][0]==1.0f && matx[0][1]==0.0f &&
			matx[0][2]==0.0f && matx[0][3]==0.0f &&
			matx[1][0]==0.0f && matx[1][1]==1.0f &&
			matx[1][2]==0.0f && matx[1][3]==0.0f &&
			matx[2][0]==0.0f && matx[2][1]==0.0f &&
			matx[2][2]==1.0f && matx[2][3]==0.0f &&
			matx[3][0]==0.0f && matx[3][1]==0.0f &&
			matx[3][2]==0.0f && matx[3][3]==1.0f);
		}

    int		isZero() const
		{
		    // NB: DO NOT USE TOLERANCES!
		    return(
			matx[0][0]==0.0f && matx[0][1]==0.0f &&
			matx[0][2]==0.0f && matx[0][3]==0.0f &&
			matx[1][0]==0.0f && matx[1][1]==0.0f &&
			matx[1][2]==0.0f && matx[1][3]==0.0f &&
			matx[2][0]==0.0f && matx[2][1]==0.0f &&
			matx[2][2]==0.0f && matx[2][3]==0.0f &&
			matx[3][0]==0.0f && matx[3][1]==0.0f &&
			matx[3][2]==0.0f && matx[3][3]==0.0f);
		}


    /// Return the raw matrix data.
    // @{
    const T	*data(void) const	{ return myFloats; }
    T		*data(void)		{ return myFloats; }
    // @}

    /// Compute a hash
    unsigned	hash() const	{ return SYSvector_hash(data(), tuple_size); }

    /// Return a matrix entry. No bounds checking on subscripts.
    // @{
    SYS_FORCE_INLINE
    T		&operator()(unsigned row, unsigned col)
		 {
		     UT_ASSERT_P(row < 4 && col < 4);
		     return matx[row][col];
		 }
    SYS_FORCE_INLINE
    T		 operator()(unsigned row, unsigned col) const
		 {
		     UT_ASSERT_P(row < 4 && col < 4);
		     return matx[row][col];
		 }
    // @}

    /// Return a matrix row. No bounds checking on subscript.
    // @{
    SYS_FORCE_INLINE
    T		*operator()(unsigned row)
		 {
		     UT_ASSERT_P(row < 4);
		     return matx[row];
		 }
    SYS_FORCE_INLINE
    const T	*operator()(unsigned row) const
		 {
		     UT_ASSERT_P(row < 4);
		     return matx[row];
		 }
    inline
    const UT_Vector4T<T> &operator[](unsigned row) const;
    inline
    UT_Vector4T<T> &operator[](unsigned row);
    // @}

    /// Euclidean or Frobenius norm of a matrix.
    /// Does sqrt(sum(a_ij ^2))
    T		 getEuclideanNorm() const
		 { return SYSsqrt(getEuclideanNorm2()); }
    /// Euclidean norm squared.
    T		 getEuclideanNorm2() const;

    /// L-Infinity Norm
    T		 getInfinityNorm() const;

    /// Compute the exponential of A using Pade approximants with scaling and squaring by q
    UT_Matrix4T<T> &exp(int q);

    /// Compute the log of A using Taylor approximation
    UT_Matrix4T<T> &log(T tolerance = T(SYS_FTOLERANCE), int max_iterations = 10);

    /// Compute the square root of A
    UT_Matrix4T<T> &sqrt(T tolerance = T(SYS_FTOLERANCE), int max_iterations = 10);

    // I/O methods.  Return 0 if read/write successful, -1 if unsuccessful.
    int			 save(std::ostream &os, int binary) const;
    bool		 load(UT_IStream &is);
    void		 dump(const char *msg="") const;

    void		 outAsciiNoName(std::ostream &os) const;

    /// @{
    /// Methods to serialize to a JSON stream.  The matrix is stored as an
    /// array of 16 reals.
    bool		save(UT_JSONWriter &w) const;
    bool		save(UT_JSONValue &v) const;
    bool		load(UT_JSONParser &p);
    /// @}

    static const UT_Matrix4T<T> &getIdentityMatrix();

    // I/O friends:
    friend std::ostream	&operator<<(std::ostream &os, const UT_Matrix4T<T> &v)
			{
			    v.writeClassName(os);
			    v.outAsciiNoName(os);
			    return os;
			}

    /// Returns the vector size
    static int		entries()	{ return tuple_size; }


    // The matrix data:
    union {
	T	 matx[4][4];
	T	 myFloats[tuple_size];
    };

    /// Create a perspective projection matrix with the given parameters.
    /// This can be used to project points onto the so-called NDC coordinates
    /// of a camera.  For example, given a point @c P (in the space of a
    /// camera):
    /// @code
    ///	   UT_Vector3	ndc;
    ///	   UT_Matrix4R  proj;
    ///    proj.perspective(zoom, image_aspect);
    ///    ndc = P * proj;
    /// @endcode
    ///
    /// - @c zoom		@n The zoom for the lens
    /// - @c image_aspect	@n The aspect ratio of the image
    /// - @c pixel_aspect	@n The pixel aspect (the aspect ratio of pixels)
    /// - @c near,far		@n The near/far clipping planes
    /// - @c window		@n The offset for the projection window.
    ///
    /// The projection transform will transform the z coordinate of the camera
    /// space point such that the near coordinate will map to 0 and the far
    /// coordinate will map to 1.
    /// That is <tt>n dz.z = fit(P.z, near, far, 0, 1); </tt>.
    /// Thus, if the near/far are set to 0/1, the NDC z-coordinate will
    /// be the same as the camera space z.  If the near/far are set to 0/-1,
    /// the Z coordinate will be negated.
    ///
    /// @note Sometimes the @c zoom is expressed in terms of @c focal and @c
    ///    aperture.  In this case: <tt> zoom = focal/aperture </tt>
    /// @note Sometimes the @c image_aspect is expressed in terms of @c xres
    ///    and @c yres.  In this case: <tt> image_aspect = xres / yres </tt>
    /// @note To make a single transform from world space to NDC space given a
    ///    camera matrix and a projection matrix, you would use
    ///    <tt> worldToNDC = worldToCamera * projection; </tt>
    ///
    void	perspective(fpreal zoom,
			    fpreal image_aspect,
			    fpreal pixel_aspect=1,
			    fpreal clip_near=0, fpreal clip_far=1,
			    fpreal window_xmin=0, fpreal window_xmax=1,
			    fpreal window_ymin=0, fpreal window_ymax=1);
    /// Create an orthographic projection matrix with the given parameters.
    /// This can be used to project points onto the so-called NDC coordinates
    /// of a camera.  For example, given a point @c P (in the space of a
    /// camera):
    /// @code
    ///	   UT_Vector3	ndc;
    ///	   UT_Matrix4R  proj;
    ///    proj.orthographic(zoom, 1, image_aspect);
    ///    ndc = P * proj;
    /// @endcode
    ///
    /// - @c zoom		@n The zoom for the lens
    /// - @c orthowidth		@n An additional "zoom" factor
    /// - @c image_aspect	@n The resolution of the image
    /// - @c pixel_aspect	@n The pixel aspect (the aspect ratio of pixels)
    /// - @c near,far		@n The near/far clipping planes
    /// - @c window		@n The offset for the projection window.
    ///
    /// The projection transform will transform the z coordinate of the camera
    /// space point such that the near coordinate will map to 0 and the far
    /// coordinate will map to 1.
    /// That is <tt>n dz.z = fit(P.z, near, far, 0, 1); </tt>.
    /// Thus, if the near/far are set to 0/1, the NDC z-coordinate will
    /// be the same as the camera space z.  If the near/far are set to 0/-1,
    /// the Z coordinate will be negated.
    ///
    /// @note Sometimes the @c zoom is expressed in terms of @c focal and @c
    ///    aperture.  In this case: <tt> zoom = focal/aperture </tt>
    /// @note Sometimes the @c image_aspect is expressed in terms of @c xrex
    ///    and @c yres.  In this case: <tt> image_aspect = xres / yres </tt>
    /// @note To make a single transform from world space to NDC space given a
    ///    camera matrix and a projection matrix, you would use
    ///    <tt> worldToNDC = worldToCamera * projection; </tt>
    ///
    void	orthographic(fpreal zoom,
			    fpreal orthowidth,
			    fpreal image_aspect,
			    fpreal pixel_aspect=1,
			    fpreal clip_near=0, fpreal clip_far=1,
			    fpreal window_xmin=0, fpreal window_xmax=1,
			    fpreal window_ymin=0, fpreal window_ymax=1);

private:
    // Operation to aid in cofactor computation
    SYS_FORCE_INLINE
    void		coVals(int k, int r[3]) const
			{
			    switch (k)
			    {
				case 0: r[0] = 1; r[1] = 2; r[2] = 3; break;
				case 1: r[0] = 0; r[1] = 2; r[2] = 3; break;
				case 2: r[0] = 0; r[1] = 1; r[2] = 3; break;
				case 3: r[0] = 0; r[1] = 1; r[2] = 2; break;
			    }
			}

    static UT_Matrix4T<T> theIdentityMatrix;

    void		 writeClassName(std::ostream &os) const;
    static const char	*className();
};

// Vector4 operators:

template <typename T>
template <typename S>
inline
UT_Matrix4T<T>  &UT_Matrix4T<T>::operator=(const UT_Vector4T<S> &vec)
{
    matx[0][0] = matx[0][1] = matx[0][2] = matx[0][3] = vec.x();
    matx[1][0] = matx[1][1] = matx[1][2] = matx[1][3] = vec.y();
    matx[2][0] = matx[2][1] = matx[2][2] = matx[2][3] = vec.z();
    matx[3][0] = matx[3][1] = matx[3][2] = matx[3][3] = vec.w();
    return *this;
}

template <typename T>
template <typename S>
inline
UT_Matrix4T<T>  &UT_Matrix4T<T>::operator+=(const UT_Vector4T<S> &vec)
{
    T x = vec.x(); T y = vec.y();
    T z = vec.z(); T w = vec.w();
    matx[0][0]+=x; matx[0][1]+=x; matx[0][2]+=x; matx[0][3]+=x;
    matx[1][0]+=y; matx[1][1]+=y; matx[1][2]+=y; matx[1][3]+=y;
    matx[2][0]+=z; matx[2][1]+=z; matx[2][2]+=z; matx[2][3]+=z;
    matx[3][0]+=w; matx[3][1]+=w; matx[3][2]+=w; matx[3][3]+=w;
    return *this;
}

template <typename T>
template <typename S>
inline
UT_Matrix4T<T>  &UT_Matrix4T<T>::operator-=(const UT_Vector4T<S> &vec)
{
    T x = vec.x(); T y = vec.y();
    T z = vec.z(); T w = vec.w();
    matx[0][0]-=x; matx[0][1]-=x; matx[0][2]-=x; matx[0][3]-=x;
    matx[1][0]-=y; matx[1][1]-=y; matx[1][2]-=y; matx[1][3]-=y;
    matx[2][0]-=z; matx[2][1]-=z; matx[2][2]-=z; matx[2][3]-=z;
    matx[3][0]-=w; matx[3][1]-=w; matx[3][2]-=w; matx[3][3]-=w;
    return *this;
}

template <typename T>
template <typename S>
inline
void	    UT_Matrix4T<T>::getTranslates(UT_Vector3T<S> &translates) const
{
    translates.x() = matx[3][0];
    translates.y() = matx[3][1];
    translates.z() = matx[3][2];
}

template <typename T>
template <typename S>
inline
void	    UT_Matrix4T<T>::setTranslates(const UT_Vector3T<S> &translates)
{
    matx[3][0] = translates.x();
    matx[3][1] = translates.y();
    matx[3][2] = translates.z();
}

template <typename T>
inline
const UT_Vector4T<T> &UT_Matrix4T<T>::operator[](unsigned row) const
{
    UT_ASSERT_P(row < 4);
    return *(const UT_Vector4T<T>*)(matx[row]);
}

template <typename T>
inline
UT_Vector4T<T> &UT_Matrix4T<T>::operator[](unsigned row)
{
    UT_ASSERT_P(row < 4);
    return *(UT_Vector4T<T>*)(matx[row]);
}

template <typename T>
template <typename S>
inline UT_Matrix4T<T> &
UT_Matrix4T<T>::operator*=(const UT_Matrix4T<S> &m)
{
    T a, b, c, d;
    a = matx[0][0]; b = matx[0][1]; c = matx[0][2]; d = matx[0][3];
    matx[0][0] = a*m(0,0) + b*m(1,0) + c*m(2,0) + d*m(3,0);
    matx[0][1] = a*m(0,1) + b*m(1,1) + c*m(2,1) + d*m(3,1);
    matx[0][2] = a*m(0,2) + b*m(1,2) + c*m(2,2) + d*m(3,2);
    matx[0][3] = a*m(0,3) + b*m(1,3) + c*m(2,3) + d*m(3,3);

    a = matx[1][0]; b = matx[1][1]; c = matx[1][2]; d = matx[1][3];
    matx[1][0] = a*m(0,0) + b*m(1,0) + c*m(2,0) + d*m(3,0);
    matx[1][1] = a*m(0,1) + b*m(1,1) + c*m(2,1) + d*m(3,1);
    matx[1][2] = a*m(0,2) + b*m(1,2) + c*m(2,2) + d*m(3,2);
    matx[1][3] = a*m(0,3) + b*m(1,3) + c*m(2,3) + d*m(3,3);

    a = matx[2][0]; b = matx[2][1]; c = matx[2][2]; d = matx[2][3];
    matx[2][0] = a*m(0,0) + b*m(1,0) + c*m(2,0) + d*m(3,0);
    matx[2][1] = a*m(0,1) + b*m(1,1) + c*m(2,1) + d*m(3,1);
    matx[2][2] = a*m(0,2) + b*m(1,2) + c*m(2,2) + d*m(3,2);
    matx[2][3] = a*m(0,3) + b*m(1,3) + c*m(2,3) + d*m(3,3);

    a = matx[3][0]; b = matx[3][1]; c = matx[3][2]; d = matx[3][3];
    matx[3][0] = a*m(0,0) + b*m(1,0) + c*m(2,0) + d*m(3,0);
    matx[3][1] = a*m(0,1) + b*m(1,1) + c*m(2,1) + d*m(3,1);
    matx[3][2] = a*m(0,2) + b*m(1,2) + c*m(2,2) + d*m(3,2);
    matx[3][3] = a*m(0,3) + b*m(1,3) + c*m(2,3) + d*m(3,3);
    return *this;
}

#ifndef UT_DISABLE_VECTORIZE_MATRIX
template <>
template <>
inline UT_Matrix4T<float> &
UT_Matrix4T<float>::operator*=(const UT_Matrix4T<float> &m)
{
    // rows of right matrix
    const v4uf	m0(m.matx[0]);
    const v4uf	m1(m.matx[1]);
    const v4uf	m2(m.matx[2]);
    const v4uf	m3(m.matx[3]);

    v4uf	row;

    for (int i = 0; i < 4; i++)
    {
	row = m0 * v4uf(matx[i][0]);
	row += m1 * v4uf(matx[i][1]);
	row += m2 * v4uf(matx[i][2]);
	row += m3 * v4uf(matx[i][3]);

	VM_STORE(matx[i], row.vector);
    }
    return *this;
}
#endif

template <typename T>
template <typename S>
inline UT_Matrix4T<T> &
UT_Matrix4T<T>::operator*=(const UT_Matrix3T<S> &m)
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

    a = matx[3][0]; b = matx[3][1]; c = matx[3][2];
    matx[3][0] = a*m(0,0) + b*m(1,0) + c*m(2,0);
    matx[3][1] = a*m(0,1) + b*m(1,1) + c*m(2,1);
    matx[3][2] = a*m(0,2) + b*m(1,2) + c*m(2,2);
    return *this;
}

#ifndef UT_DISABLE_VECTORIZE_MATRIX
#include "UT_Matrix3.h"
template <>
template <>
inline UT_Matrix4T<float> &
UT_Matrix4T<float>::operator*=(const UT_Matrix3T<float> &m)
{
    const v4uf m0(m(0));
    const v4uf m1(m(1));
    const v4uf m2(m(2,0), m(2,1), m(2,2), 0);

    for (int i = 0; i < 4; ++i)
    {
	// Load ith row of this matrix
	v4uf row = m0 * v4uf(matx[i][0]);
	row += m1 * v4uf(matx[i][1]);
	row += m2 * v4uf(matx[i][2]);

	const float last = matx[i][3];
	vm_store(matx[i], row.vector);
	matx[i][3] = last;
    }
    return *this;
}
template <> inline void
UT_Matrix4T<float>::lerp(const UT_Matrix4T<float> &a, const UT_Matrix4T<float> &b, float t)
{
    // Lerp row by row
    for (int i = 0; i < 4; ++i) // Should unroll this
    {
	const v4uf ar(a.matx[i]);
	const v4uf br(b.matx[i]);
	const v4uf cr = SYSlerp(ar, br, t);
	vm_store(matx[i], cr.vector);
    }
}
#endif


template <typename T>
inline void
UT_Matrix4T<T>::leftMult( const UT_Matrix4T<T> &m )
{
    T a, b, c, d;
    a = matx[0][0]; b = matx[1][0]; c = matx[2][0]; d = matx[3][0];
    matx[0][0] = a*m(0,0) + b*m(0,1) + c*m(0,2) + d*m(0,3);
    matx[1][0] = a*m(1,0) + b*m(1,1) + c*m(1,2) + d*m(1,3);
    matx[2][0] = a*m(2,0) + b*m(2,1) + c*m(2,2) + d*m(2,3);
    matx[3][0] = a*m(3,0) + b*m(3,1) + c*m(3,2) + d*m(3,3);

    a = matx[0][1]; b = matx[1][1]; c = matx[2][1]; d = matx[3][1];
    matx[0][1] = a*m(0,0) + b*m(0,1) + c*m(0,2) + d*m(0,3);
    matx[1][1] = a*m(1,0) + b*m(1,1) + c*m(1,2) + d*m(1,3);
    matx[2][1] = a*m(2,0) + b*m(2,1) + c*m(2,2) + d*m(2,3);
    matx[3][1] = a*m(3,0) + b*m(3,1) + c*m(3,2) + d*m(3,3);

    a = matx[0][2]; b = matx[1][2]; c = matx[2][2]; d = matx[3][2];
    matx[0][2] = a*m(0,0) + b*m(0,1) + c*m(0,2) + d*m(0,3);
    matx[1][2] = a*m(1,0) + b*m(1,1) + c*m(1,2) + d*m(1,3);
    matx[2][2] = a*m(2,0) + b*m(2,1) + c*m(2,2) + d*m(2,3);
    matx[3][2] = a*m(3,0) + b*m(3,1) + c*m(3,2) + d*m(3,3);

    a = matx[0][3]; b = matx[1][3]; c = matx[2][3]; d = matx[3][3];
    matx[0][3] = a*m(0,0) + b*m(0,1) + c*m(0,2) + d*m(0,3);
    matx[1][3] = a*m(1,0) + b*m(1,1) + c*m(1,2) + d*m(1,3);
    matx[2][3] = a*m(2,0) + b*m(2,1) + c*m(2,2) + d*m(2,3);
    matx[3][3] = a*m(3,0) + b*m(3,1) + c*m(3,2) + d*m(3,3);
}

#ifndef UT_DISABLE_VECTORIZE_MATRIX
template <>
inline void
UT_Matrix4T<float>::leftMult( const UT_Matrix4T<float> &m )
{
    const v4uf m0(matx[0]);
    const v4uf m1(matx[1]);
    const v4uf m2(matx[2]);
    const v4uf m3(matx[3]);

    for (int i = 0; i < 4; ++i)
    {
	const v4uf row = m0 * v4uf(m.matx[i][0])
	    + m1 * v4uf(m.matx[i][1])
	    + m2 * v4uf(m.matx[i][2])
	    + m3 * v4uf(m.matx[i][3]);

	VM_STORE(matx[i], row.vector);
    }
}
#endif

// Free floating functions:
template <typename T>
inline UT_Matrix4T<T>
operator+(const UT_Matrix4T<T> &m1, const UT_Matrix4T<T> &m2)
{
    T m[4][4];
    m[0][0] = m1(0,0)+m2(0,0); m[0][1] = m1(0,1)+m2(0,1);
    m[0][2] = m1(0,2)+m2(0,2); m[0][3] = m1(0,3)+m2(0,3);

    m[1][0] = m1(1,0)+m2(1,0); m[1][1] = m1(1,1)+m2(1,1);
    m[1][2] = m1(1,2)+m2(1,2); m[1][3] = m1(1,3)+m2(1,3);

    m[2][0] = m1(2,0)+m2(2,0); m[2][1] = m1(2,1)+m2(2,1);
    m[2][2] = m1(2,2)+m2(2,2); m[2][3] = m1(2,3)+m2(2,3);

    m[3][0] = m1(3,0)+m2(3,0); m[3][1] = m1(3,1)+m2(3,1);
    m[3][2] = m1(3,2)+m2(3,2); m[3][3] = m1(3,3)+m2(3,3);
    return UT_Matrix4T<T>(m);
}
template <typename T>
inline UT_Matrix4T<T>
operator-(const UT_Matrix4T<T> &m1, const UT_Matrix4T<T> &m2)
{
    T m[4][4];
    m[0][0] = m1(0,0)-m2(0,0); m[0][1] = m1(0,1)-m2(0,1);
    m[0][2] = m1(0,2)-m2(0,2); m[0][3] = m1(0,3)-m2(0,3);

    m[1][0] = m1(1,0)-m2(1,0); m[1][1] = m1(1,1)-m2(1,1);
    m[1][2] = m1(1,2)-m2(1,2); m[1][3] = m1(1,3)-m2(1,3);

    m[2][0] = m1(2,0)-m2(2,0); m[2][1] = m1(2,1)-m2(2,1);
    m[2][2] = m1(2,2)-m2(2,2); m[2][3] = m1(2,3)-m2(2,3);

    m[3][0] = m1(3,0)-m2(3,0); m[3][1] = m1(3,1)-m2(3,1);
    m[3][2] = m1(3,2)-m2(3,2); m[3][3] = m1(3,3)-m2(3,3);
    return UT_Matrix4T<T>(m);
}
template <typename T>
inline UT_Matrix4T<T>
operator*(const UT_Matrix4T<T> &m1, const UT_Matrix4T<T> &m2)
{
    T m[4][4];
    m[0][0] = m1(0,0)*m2(0,0) + m1(0,1)*m2(1,0) + 
	      m1(0,2)*m2(2,0) + m1(0,3)*m2(3,0) ;
    m[0][1] = m1(0,0)*m2(0,1) + m1(0,1)*m2(1,1) +
	      m1(0,2)*m2(2,1) + m1(0,3)*m2(3,1) ;
    m[0][2] = m1(0,0)*m2(0,2) + m1(0,1)*m2(1,2) +
	      m1(0,2)*m2(2,2) + m1(0,3)*m2(3,2) ;
    m[0][3] = m1(0,0)*m2(0,3) + m1(0,1)*m2(1,3) +
	      m1(0,2)*m2(2,3) + m1(0,3)*m2(3,3) ;

    m[1][0] = m1(1,0)*m2(0,0) + m1(1,1)*m2(1,0) +
	      m1(1,2)*m2(2,0) + m1(1,3)*m2(3,0) ;
    m[1][1] = m1(1,0)*m2(0,1) + m1(1,1)*m2(1,1) + 
	      m1(1,2)*m2(2,1) + m1(1,3)*m2(3,1) ;
    m[1][2] = m1(1,0)*m2(0,2) + m1(1,1)*m2(1,2) + 
	      m1(1,2)*m2(2,2) + m1(1,3)*m2(3,2) ;
    m[1][3] = m1(1,0)*m2(0,3) + m1(1,1)*m2(1,3) + 
	      m1(1,2)*m2(2,3) + m1(1,3)*m2(3,3) ;

    m[2][0] = m1(2,0)*m2(0,0) + m1(2,1)*m2(1,0) +
	      m1(2,2)*m2(2,0) + m1(2,3)*m2(3,0) ;
    m[2][1] = m1(2,0)*m2(0,1) + m1(2,1)*m2(1,1) + 
	      m1(2,2)*m2(2,1) + m1(2,3)*m2(3,1) ;
    m[2][2] = m1(2,0)*m2(0,2) + m1(2,1)*m2(1,2) + 
	      m1(2,2)*m2(2,2) + m1(2,3)*m2(3,2) ;
    m[2][3] = m1(2,0)*m2(0,3) + m1(2,1)*m2(1,3) + 
	      m1(2,2)*m2(2,3) + m1(2,3)*m2(3,3) ;

    m[3][0] = m1(3,0)*m2(0,0) + m1(3,1)*m2(1,0) +
	      m1(3,2)*m2(2,0) + m1(3,3)*m2(3,0) ;
    m[3][1] = m1(3,0)*m2(0,1) + m1(3,1)*m2(1,1) + 
	      m1(3,2)*m2(2,1) + m1(3,3)*m2(3,1) ;
    m[3][2] = m1(3,0)*m2(0,2) + m1(3,1)*m2(1,2) + 
	      m1(3,2)*m2(2,2) + m1(3,3)*m2(3,2) ;
    m[3][3] = m1(3,0)*m2(0,3) + m1(3,1)*m2(1,3) + 
	      m1(3,2)*m2(2,3) + m1(3,3)*m2(3,3) ;
    return UT_Matrix4T<T>(m);
}
#ifndef UT_DISABLE_VECTORIZE_MATRIX
template <>
inline UT_Matrix4T<float>
operator*(const UT_Matrix4T<float> &m1, const UT_Matrix4T<float> &m2)
{
    UT_Matrix4T<float> result;

    const v4uf r0(m2.matx[0]);
    const v4uf r1(m2.matx[1]);
    const v4uf r2(m2.matx[2]);
    const v4uf r3(m2.matx[3]);

    for (int i = 0; i < 4; ++i)
    {
	const v4uf row = r0 * v4uf(m1.matx[i][0])
	    + r1 * v4uf(m1.matx[i][1])
	    + r2 * v4uf(m1.matx[i][2])
	    + r3 * v4uf(m1.matx[i][3]);

	VM_STORE(result.matx[i], row.vector);
    }
    return result;
}
#endif

template <typename T, typename S>
inline UT_Matrix4T<T>	
operator+(const UT_Matrix4T<T> &m1, const UT_Vector4T<S> &vec)
{
    T   m[4][4];
    T   x=vec.x(); T y=vec.y(); T z=vec.z(); T w=vec.w();
    m[0][0] = m1(0,0) + x; m[0][1] = m1(0,1) + x;
    m[0][2] = m1(0,2) + x; m[0][3] = m1(0,3) + x;

    m[1][0] = m1(1,0) + y; m[1][1] = m1(1,1) + y;
    m[1][2] = m1(1,2) + y; m[1][3] = m1(1,3) + y;

    m[2][0] = m1(2,0) + z; m[2][1] = m1(2,1) + z;
    m[2][2] = m1(2,2) + z; m[2][3] = m1(2,3) + z;

    m[3][0] = m1(3,0) + w; m[3][1] = m1(3,1) + w;
    m[3][2] = m1(3,2) + w; m[3][3] = m1(3,3) + w;
    return UT_Matrix4T<T>(m);
}

template <typename T, typename S>
inline UT_Matrix4T<T>	
operator+(const UT_Vector4T<S> &vec, const UT_Matrix4T<T> &mat)
{
    return mat+vec;
}

template <typename T, typename S>
inline UT_Matrix4T<T>	
operator-(const UT_Matrix4T<T> &m1, const UT_Vector4T<S> &vec)
{
    T   m[4][4];
    T   x=vec.x(); T y=vec.y(); T z=vec.z(); T w=vec.w();
    m[0][0] = m1(0,0) - x; m[0][1] = m1(0,1) - x;
    m[0][2] = m1(0,2) - x; m[0][3] = m1(0,3) - x;

    m[1][0] = m1(1,0) - y; m[1][1] = m1(1,1) - y;
    m[1][2] = m1(1,2) - y; m[1][3] = m1(1,3) - y;

    m[2][0] = m1(2,0) - z; m[2][1] = m1(2,1) - z;
    m[2][2] = m1(2,2) - z; m[2][3] = m1(2,3) - z;

    m[3][0] = m1(3,0) - w; m[3][1] = m1(3,1) - w;
    m[3][2] = m1(3,2) - w; m[3][3] = m1(3,3) - w;
    return UT_Matrix4T<T>(m);
}

template <typename T, typename S>
inline UT_Matrix4T<T>
operator-(const UT_Vector4T<S> &vec, const UT_Matrix4T<T> &m1)
{
    T   m[4][4];
    T   x=vec.x(); T y=vec.y(); T z=vec.z(); T w=vec.w();
    m[0][0] = x - m1(0,0); m[0][1] = x - m1(0,1);
    m[0][2] = x - m1(0,2); m[0][3] = x - m1(0,3);

    m[1][0] = y - m1(1,0); m[1][1] = y - m1(1,1);
    m[1][2] = y - m1(1,2); m[1][3] = y - m1(1,3);

    m[2][0] = z - m1(2,0); m[2][1] = z - m1(2,1);
    m[2][2] = z - m1(2,2); m[2][3] = z - m1(2,3);

    m[3][0] = w - m1(3,0); m[3][1] = w - m1(3,1);
    m[3][2] = w - m1(3,2); m[3][3] = w - m1(3,3);
    return UT_Matrix4T<T>(m);
}

template <typename T, typename S>
inline UT_Matrix4T<T>	
operator+(const UT_Matrix4T<T> &n, S sc)
{
    T   m[4][4];
    m[0][0]=n(0,0)+sc; m[0][1]=n(0,1)+sc; m[0][2]=n(0,2)+sc; m[0][3]=n(0,3)+sc;
    m[1][0]=n(1,0)+sc; m[1][1]=n(1,1)+sc; m[1][2]=n(1,2)+sc; m[1][3]=n(1,3)+sc;
    m[2][0]=n(2,0)+sc; m[2][1]=n(2,1)+sc; m[2][2]=n(2,2)+sc; m[2][3]=n(2,3)+sc;
    m[3][0]=n(3,0)+sc; m[3][1]=n(3,1)+sc; m[3][2]=n(3,2)+sc; m[3][3]=n(3,3)+sc;
    return UT_Matrix4T<T>(m);
}

template <typename T, typename S>
inline UT_Matrix4T<T>	
operator-(S sc, const UT_Matrix4T<T> &n)
{
    T   m[4][4];
    m[0][0]=sc-n(0,0); m[0][1]=sc-n(0,1); m[0][2]=sc-n(0,2); m[0][3]=sc-n(0,3);
    m[1][0]=sc-n(1,0); m[1][1]=sc-n(1,1); m[1][2]=sc-n(1,2); m[1][3]=sc-n(1,3);
    m[2][0]=sc-n(2,0); m[2][1]=sc-n(2,1); m[2][2]=sc-n(2,2); m[2][3]=sc-n(2,3);
    m[3][0]=sc-n(3,0); m[3][1]=sc-n(3,1); m[3][2]=sc-n(3,2); m[3][3]=sc-n(3,3);
    return UT_Matrix4T<T>(m);
}

template <typename T, typename S>
inline UT_Matrix4T<T>
operator*(S sc, const UT_Matrix4T<T> &m1)
{
    return m1*sc;
}

template <typename T, typename S>
inline UT_Matrix4T<T>	
operator*(const UT_Matrix4T<T> &n, S sc)
{
    T   m[4][4];
    m[0][0]=n(0,0)*sc; m[0][1]=n(0,1)*sc; m[0][2]=n(0,2)*sc; m[0][3]=n(0,3)*sc;
    m[1][0]=n(1,0)*sc; m[1][1]=n(1,1)*sc; m[1][2]=n(1,2)*sc; m[1][3]=n(1,3)*sc;
    m[2][0]=n(2,0)*sc; m[2][1]=n(2,1)*sc; m[2][2]=n(2,2)*sc; m[2][3]=n(2,3)*sc;
    m[3][0]=n(3,0)*sc; m[3][1]=n(3,1)*sc; m[3][2]=n(3,2)*sc; m[3][3]=n(3,3)*sc;
    return UT_Matrix4T<T>(m);
}

template <typename T, typename S>
inline  
UT_Matrix4T<T>
operator/(const UT_Matrix4T<T> &m1, S scalar)
{
    return (m1 * (1.0f/scalar));
}

template <typename T, typename S>
inline UT_Matrix4T<T>	
operator/(S sc, const UT_Matrix4T<T> &n)
{
    T   m[4][4];
    m[0][0]=sc/n(0,0); m[0][1]=sc/n(0,1); m[0][2]=sc/n(0,2); m[0][3]=sc/n(0,3);
    m[1][0]=sc/n(1,0); m[1][1]=sc/n(1,1); m[1][2]=sc/n(1,2); m[1][3]=sc/n(1,3);
    m[2][0]=sc/n(2,0); m[2][1]=sc/n(2,1); m[2][2]=sc/n(2,2); m[2][3]=sc/n(2,3);
    m[3][0]=sc/n(3,0); m[3][1]=sc/n(3,1); m[3][2]=sc/n(3,2); m[3][3]=sc/n(3,3);
    return UT_Matrix4T<T>(m);
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
inline UT_Vector3T<T> rowVecMult(const UT_Vector3T<T> &v, const UT_Matrix4T<S> &m);
template <typename T, typename S>
inline UT_Vector3T<T> colVecMult(const UT_Matrix4T<S> &m, const UT_Vector3T<T> &v);

template <typename T, typename S>
inline UT_Vector3T<T> rowVecMult3(const UT_Vector3T<T> &v, const UT_Matrix4T<S> &m);
template <typename T, typename S>
inline UT_Vector3T<T> colVecMult3(const UT_Matrix4T<S> &m, const UT_Vector3T<T> &v);
// @}

template <typename T, typename S>
inline UT_Vector3T<T>
rowVecMult(const UT_Vector3T<T> &v, const UT_Matrix4T<S> &m)
{
    return UT_Vector3T<T>(
	v.x()*m(0,0) + v.y()*m(1,0) + v.z()*m(2,0) + m(3,0),
	v.x()*m(0,1) + v.y()*m(1,1) + v.z()*m(2,1) + m(3,1),
	v.x()*m(0,2) + v.y()*m(1,2) + v.z()*m(2,2) + m(3,2)
    );
}
template <typename T, typename S>
inline UT_Vector3T<T>
operator*(const UT_Vector3T<T> &v, const UT_Matrix4T<S> &m)
{
    return rowVecMult(v, m);
}

template <typename T, typename S>
inline UT_Vector3T<T>
rowVecMult3(const UT_Vector3T<T> &v, const UT_Matrix4T<S> &m)
{
    return UT_Vector3T<T>(
	v.x()*m(0,0) + v.y()*m(1,0) + v.z()*m(2,0),
	v.x()*m(0,1) + v.y()*m(1,1) + v.z()*m(2,1),
	v.x()*m(0,2) + v.y()*m(1,2) + v.z()*m(2,2)
    );
}

template <typename T, typename S>
inline UT_Vector3T<T>
colVecMult(const UT_Matrix4T<S> &m, const UT_Vector3T<T> &v)
{
    return UT_Vector3T<T>(
	v.x()*m(0,0) + v.y()*m(0,1) + v.z()*m(0,2) + m(0,3),
	v.x()*m(1,0) + v.y()*m(1,1) + v.z()*m(1,2) + m(1,3),
	v.x()*m(2,0) + v.y()*m(2,1) + v.z()*m(2,2) + m(2,3)
    );
}

template <typename T, typename S>
inline UT_Vector3T<T>
colVecMult3(const UT_Matrix4T<S> &m, const UT_Vector3T<T> &v)
{
    return UT_Vector3T<T>(
	v.x()*m(0,0) + v.y()*m(0,1) + v.z()*m(0,2),
	v.x()*m(1,0) + v.y()*m(1,1) + v.z()*m(1,2),
	v.x()*m(2,0) + v.y()*m(2,1) + v.z()*m(2,2)
    );
}
#ifndef UT_DISABLE_VECTORIZE_MATRIX
template <>
inline UT_Vector3T<float>
rowVecMult(const UT_Vector3T<float> &v, const UT_Matrix4T<float> &m)
{
    const v4uf result =
	  v4uf(m.matx[0]) * v4uf(v[0])
	+ v4uf(m.matx[1]) * v4uf(v[1])
	+ v4uf(m.matx[2]) * v4uf(v[2])
	+ v4uf(m.matx[3]); // * v[3] == 1.0

    // Requires v4uf to be contiguous in memory
    return UT_Vector3T<float>((float*) &result);
}
template <>
inline UT_Vector3T<float>
rowVecMult3(const UT_Vector3T<float> &v, const UT_Matrix4T<float> &m)
{
    const v4uf result =
	  v4uf(m.matx[0]) * v4uf(v[0])
	+ v4uf(m.matx[1]) * v4uf(v[1])
	+ v4uf(m.matx[2]) * v4uf(v[2]);

    // Requires v4uf to be contiguous in memory
    return UT_Vector3T<float>((float*) &result);
}
template <>
inline UT_Vector3T<float>
colVecMult(const UT_Matrix4T<float> &m, const UT_Vector3T<float> &v)
{
    const v4uf result =
	  v4uf(m(0,0), m(1,0), m(2,0), m(3,0)) * v4uf(v[0])
	+ v4uf(m(0,1), m(1,1), m(2,1), m(3,1)) * v4uf(v[1])
	+ v4uf(m(0,2), m(1,2), m(2,2), m(3,2)) * v4uf(v[2])
	+ v4uf(m(0,3), m(1,3), m(2,3), m(3,3)); // * v[3] == 1.0

    // Requires v4uf to be contiguous in memory
    return UT_Vector3T<float>((float*) &result);
}
template <>
inline UT_Vector3T<float>
colVecMult3(const UT_Matrix4T<float> &m, const UT_Vector3T<float> &v)
{
    const v4uf result =
	  v4uf(m(0,0), m(1,0), m(2,0), m(3,0)) * v4uf(v[0])
	+ v4uf(m(0,1), m(1,1), m(2,1), m(3,1)) * v4uf(v[1])
	+ v4uf(m(0,2), m(1,2), m(2,2), m(3,2)) * v4uf(v[2]);

    // Requires v4uf to be contiguous in memory
    return UT_Vector3T<float>((float*) &result);
}
#endif


template <typename T>
SYS_FORCE_INLINE void
UT_Vector3T<T>::rowVecMult(const UT_Matrix4F &m)
{ 
    operator=(::rowVecMult(*this, m));
}
template <typename T>
SYS_FORCE_INLINE void
UT_Vector3T<T>::rowVecMult(const UT_Matrix4D &m)
{ 
    operator=(::rowVecMult(*this, m));
}
template <typename T>
SYS_FORCE_INLINE void
UT_Vector3T<T>::colVecMult(const UT_Matrix4F &m)
{ 
    operator=(::colVecMult(m, *this));
}
template <typename T>
SYS_FORCE_INLINE void
UT_Vector3T<T>::colVecMult(const UT_Matrix4D &m)
{ 
    operator=(::colVecMult(m, *this));
}
template <typename T>
SYS_FORCE_INLINE void
UT_Vector3T<T>::rowVecMult3(const UT_Matrix4F &m)
{ 
    operator=(::rowVecMult3(*this, m));
}
template <typename T>
SYS_FORCE_INLINE void
UT_Vector3T<T>::rowVecMult3(const UT_Matrix4D &m)
{ 
    operator=(::rowVecMult3(*this, m));
}
template <typename T>
SYS_FORCE_INLINE void
UT_Vector3T<T>::colVecMult3(const UT_Matrix4F &m)
{ 
    operator=(::colVecMult3(m, *this));
}
template <typename T>
SYS_FORCE_INLINE void
UT_Vector3T<T>::colVecMult3(const UT_Matrix4D &m)
{ 
    operator=(::colVecMult3(m, *this));
}
template <typename T>
template <typename S>
SYS_FORCE_INLINE UT_Vector3T<T> &
UT_Vector3T<T>::operator*=(const UT_Matrix4T<S> &m)
{
    rowVecMult(m);
    return *this;
}
template <typename T>
template <typename S>
SYS_FORCE_INLINE void
UT_Vector3T<T>::multiply3(const UT_Matrix4T<S> &mat)
{
    rowVecMult3(mat);
}
template <typename T>
template <typename S>
SYS_FORCE_INLINE void
UT_Vector3T<T>::multiply3T(const UT_Matrix4T<S> &mat)
{
    colVecMult3(mat);
}
template <typename T>
template <typename S>
SYS_FORCE_INLINE void
UT_Vector3T<T>::multiply3(UT_Vector3T<T> &dest, const UT_Matrix4T<S> &mat) const
{
    dest = ::rowVecMult3(*this, mat);
}
template <typename T>
template <typename S>
SYS_FORCE_INLINE void
UT_Vector3T<T>::multiply3T(UT_Vector3T<T> &dest, const UT_Matrix4T<S> &mat) const
{
    dest = ::colVecMult3(mat, *this);
}
template <typename T>
template <typename S>
SYS_FORCE_INLINE void
UT_Vector3T<T>::multiply(UT_Vector3T<T> &dest, const UT_Matrix4T<S> &mat) const
{
    dest = ::rowVecMult(*this, mat);
}

template <typename T>
inline
UT_Matrix4T<T>	SYSmin(const UT_Matrix4T<T> &v1, const UT_Matrix4T<T> &v2)
{
    return UT_Matrix4T<T>(
	    SYSmin(v1(0,0), v2(0,0)),
	    SYSmin(v1(0,1), v2(0,1)),
	    SYSmin(v1(0,2), v2(0,2)),
	    SYSmin(v1(0,3), v2(0,3)),
	    SYSmin(v1(1,0), v2(1,0)),
	    SYSmin(v1(1,1), v2(1,1)),
	    SYSmin(v1(1,2), v2(1,2)),
	    SYSmin(v1(1,3), v2(1,3)),
	    SYSmin(v1(2,0), v2(2,0)),
	    SYSmin(v1(2,1), v2(2,1)),
	    SYSmin(v1(2,2), v2(2,2)),
	    SYSmin(v1(2,3), v2(2,3)),
	    SYSmin(v1(3,0), v2(3,0)),
	    SYSmin(v1(3,1), v2(3,1)),
	    SYSmin(v1(3,2), v2(3,2)),
	    SYSmin(v1(3,3), v2(3,3))
    );
}

template <typename T>
inline
UT_Matrix4T<T>	SYSmax(const UT_Matrix4T<T> &v1, const UT_Matrix4T<T> &v2)
{
    return UT_Matrix4T<T>(
	    SYSmax(v1(0,0), v2(0,0)),
	    SYSmax(v1(0,1), v2(0,1)),
	    SYSmax(v1(0,2), v2(0,2)),
	    SYSmax(v1(0,3), v2(0,3)),
	    SYSmax(v1(1,0), v2(1,0)),
	    SYSmax(v1(1,1), v2(1,1)),
	    SYSmax(v1(1,2), v2(1,2)),
	    SYSmax(v1(1,3), v2(1,3)),
	    SYSmax(v1(2,0), v2(2,0)),
	    SYSmax(v1(2,1), v2(2,1)),
	    SYSmax(v1(2,2), v2(2,2)),
	    SYSmax(v1(2,3), v2(2,3)),
	    SYSmax(v1(3,0), v2(3,0)),
	    SYSmax(v1(3,1), v2(3,1)),
	    SYSmax(v1(3,2), v2(3,2)),
	    SYSmax(v1(3,3), v2(3,3))
    );
}

template <typename T,typename S>
inline
UT_Matrix4T<T> SYSlerp(const UT_Matrix4T<T> &v1, const UT_Matrix4T<T> &v2, S t)
{
    return UT_Matrix4T<T>(
            SYSlerp(v1(0,0), v2(0,0), t),
            SYSlerp(v1(0,1), v2(0,1), t),
            SYSlerp(v1(0,2), v2(0,2), t),
            SYSlerp(v1(0,3), v2(0,3), t),
            SYSlerp(v1(1,0), v2(1,0), t),
            SYSlerp(v1(1,1), v2(1,1), t),
            SYSlerp(v1(1,2), v2(1,2), t),
            SYSlerp(v1(1,3), v2(1,3), t),
            SYSlerp(v1(2,0), v2(2,0), t),
            SYSlerp(v1(2,1), v2(2,1), t),
            SYSlerp(v1(2,2), v2(2,2), t),
            SYSlerp(v1(2,3), v2(2,3), t),
            SYSlerp(v1(3,0), v2(3,0), t),
            SYSlerp(v1(3,1), v2(3,1), t),
            SYSlerp(v1(3,2), v2(3,2), t),
            SYSlerp(v1(3,3), v2(3,3), t)
    );
}
#ifndef UT_DISABLE_VECTORIZE_MATRIX
template <>
inline
UT_Matrix4T<float> SYSlerp(const UT_Matrix4T<float> &v1, const UT_Matrix4T<float> &v2, float t)
{
    UT_Matrix4T<float> result;
    for (int i = 0; i < 4; ++i)
    {
	const v4uf r1(v1.matx[i]);
	const v4uf r2(v2.matx[i]);
	const v4uf rr = SYSlerp(r1, r2, t);
	vm_store(result.matx[i], rr.vector);
    }
    return result;
}
#endif

template<typename T>
struct UT_FixedVectorTraits<UT_Matrix4T<T> >
{
    typedef UT_FixedVector<T,16> FixedVectorType;
    typedef T DataType;
    static const exint TupleSize = 16;
    static const bool isVectorType = true;
};

// Overload for custom formatting of UT_Matrix4T<T> with UTformat.
template<typename T>
UT_API size_t format(char *buffer, size_t buffer_size, const UT_Matrix4T<T> &v);

#endif

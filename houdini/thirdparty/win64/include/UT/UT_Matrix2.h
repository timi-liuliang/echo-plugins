/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#pragma once

#ifndef __UT_Matrix2_h__
#define __UT_Matrix2_h__

#include "UT_API.h"
#include "UT_Assert.h"
#include "UT_FixedVector.h"
#include "UT_VectorTypes.h"

#include <SYS/SYS_Types.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_StaticAssert.h>
#include <iosfwd>

class UT_IStream;
class UT_JSONWriter;
class UT_JSONValue;
class UT_JSONParser;

// Free floating operators that return a UT_Matrix2T<T> object. 
template <typename T> UT_API
UT_Matrix2T<T>	operator+(const UT_Matrix2T<T> &m1, const UT_Matrix2T<T> &m2);
template <typename T, typename S> UT_API
UT_Matrix2T<T>	operator+(const UT_Matrix2T<T> &m, const UT_Vector2T<S> &v);
template <typename T, typename S> UT_API
UT_Matrix2T<T>	operator+(const UT_Vector2T<S> &v, const UT_Matrix2T<T> &m);
template <typename T> UT_API
UT_Matrix2T<T>	operator+(const UT_Matrix2T<T> &mat, T sc);
template <typename T> UT_API
UT_Matrix2T<T>	operator+(T sc, const UT_Matrix2T<T> &mat);

template <typename T> UT_API
UT_Matrix2T<T>	operator-(const UT_Matrix2T<T> &m1, const UT_Matrix2T<T> &m2);
template <typename T, typename S> UT_API
UT_Matrix2T<T>	operator-(const UT_Matrix2T<T> &m, const UT_Vector2T<S> &v);
template <typename T, typename S> UT_API
UT_Matrix2T<T>	operator-(const UT_Vector2T<S> &v, const UT_Matrix2T<T> &m);
template <typename T> UT_API
UT_Matrix2T<T>	operator-(const UT_Matrix2T<T> &mat, T sc);
template <typename T> UT_API
UT_Matrix2T<T>	operator-(T sc, const UT_Matrix2T<T> &mat);

template <typename T> UT_API
UT_Matrix2T<T> operator*(const UT_Matrix2T<T> &m1, const UT_Matrix2T<T> &m2);
template <typename T,typename S> UT_API
UT_Matrix2T<T> operator*(const UT_Matrix2T<T> &mat, S sc);
template <typename T,typename S> UT_API
UT_Matrix2T<T> operator*(S sc, const UT_Matrix2T<T> &mat);

template <typename T> UT_API
UT_Matrix2T<T>	operator/(const UT_Matrix2T<T> &mat, T sc);
template <typename T> UT_API
UT_Matrix2T<T>	operator/(T sc, const UT_Matrix2T<T> &mat);

template <typename T>
inline UT_Matrix2T<T>	SYSmin	(const UT_Matrix2T<T> &v1, const UT_Matrix2T<T> &v2);
template <typename T>
inline UT_Matrix2T<T>	SYSmax	(const UT_Matrix2T<T> &v1, const UT_Matrix2T<T> &v2);
template <typename T,typename S>
inline UT_Matrix2T<T> SYSlerp(const UT_Matrix2T<T> &v1, const UT_Matrix2T<T> &v2, S t);
template <typename T,typename S>
inline UT_Matrix2T<T> SYSbilerp(const UT_Matrix2T<T> &u0v0, const UT_Matrix2T<T> &u1v0,
                                  const UT_Matrix2T<T> &u0v1, const UT_Matrix2T<T> &u1v1,
                                  S u, S v)
{ return SYSlerp(SYSlerp(u0v0, u0v1, v), SYSlerp(u1v0, u1v1, v), u); }

/// This class implements a 2x2 matrix in row-major order.
///
/// Most of Houdini operates with row vectors that are left-multiplied with
/// matrices. e.g.,   z = v * M
template <typename T>
class UT_API UT_Matrix2T
{
public:

    typedef T value_type;
    static const int tuple_size = 4;

    /// Construct uninitialized matrix.
    SYS_FORCE_INLINE UT_Matrix2T() = default;

    /// Default copy constructor
    constexpr UT_Matrix2T(const UT_Matrix2T &) = default;

    /// Default move constructor
    constexpr UT_Matrix2T(UT_Matrix2T &&) = default;

    /// Construct identity matrix, multipled by scalar.
    explicit constexpr UT_Matrix2T(T val) noexcept
        : matx{{val,T(0)},{T(0),val}}
    {
        SYS_STATIC_ASSERT(sizeof(UT_Matrix2T<T>) == tuple_size * sizeof(T));
    }
    /// Construct a deep copy of the input row-major data.
    /// @{
    explicit constexpr UT_Matrix2T(const fpreal32 m[2][2]) noexcept
        : matx{{T(m[0][0]),T(m[0][1])},{T(m[1][0]),T(m[1][1])}}
    {}
    explicit constexpr UT_Matrix2T(const fpreal64 m[2][2]) noexcept
        : matx{{T(m[0][0]),T(m[0][1])},{T(m[1][0]),T(m[1][1])}}
    {}
    /// @}

    /// Arbitrary UT_FixedVector of the same size
    template <typename S>
    SYS_FORCE_INLINE explicit UT_Matrix2T(const UT_FixedVector<S, tuple_size> &v) noexcept
        : matx{{T(v[0]),T(v[1])},{T(v[2]),T(v[3])}}
    {}

    /// This constructor is for convenience.
    constexpr UT_Matrix2T(T val00, T val01, T val10, T val11) noexcept
        : matx{{val00,val01},{val10,val11}}
    {}

    /// Base type conversion constructor
    template <typename S>
    UT_Matrix2T(const UT_Matrix2T<S> &m) noexcept
        : matx{{T(m(0,0)),T(m(0,1))},{T(m(1,0)),T(m(1,1))}}
    {}

    /// Conversion to a 2x2 from a 3x3 matrix by ignoring the
    /// last row and last column.
    template <typename S>
    explicit UT_Matrix2T(const UT_Matrix3T<S> &m) noexcept
        : matx{{T(m(0,0)),T(m(0,1))},{T(m(1,0)),T(m(1,1))}}
    {}

    /// Default copy assignment operator
    UT_Matrix2T<T> &operator=(const UT_Matrix2T<T> &m) = default;

    /// Default move assignment operator
    UT_Matrix2T<T> &operator=(UT_Matrix2T<T> &&m) = default;

    /// Conversion operator that returns a 2x2 from a 3x3 matrix by ignoring the
    /// last row and last column.
    template <typename S>
    UT_Matrix2T<T>	&operator=(const UT_Matrix3T<S> &m);

    UT_Matrix2T<T> operator-() const
    {
        return UT_Matrix2T<T>(
            -matx[0][0], -matx[0][1],
            -matx[1][0], -matx[1][1]);
    }

    UT_Matrix2T<T>	&operator+=(const UT_Matrix2T<T> &m)
			{
			    matx[0][0]+=m.matx[0][0]; matx[0][1]+=m.matx[0][1];
			    matx[1][0]+=m.matx[1][0]; matx[1][1]+=m.matx[1][1];
			    return *this;
			}
    UT_Matrix2T<T>	&operator-=(const UT_Matrix2T<T> &m)
			{
			    matx[0][0]-=m.matx[0][0]; matx[0][1]-=m.matx[0][1];
			    matx[1][0]-=m.matx[1][0]; matx[1][1]-=m.matx[1][1];
			    return *this;
			}
    UT_Matrix2T<T>	&operator*=(const UT_Matrix2T<T> &m);

    constexpr bool operator==(const UT_Matrix2T<T> &m) const noexcept
    {
        return (&m == this) || (
            matx[0][0]==m(0,0) && matx[0][1]==m(0,1) &&
            matx[1][0]==m(1,0) && matx[1][1]==m(1,1) );
    }
    constexpr bool operator!=(const UT_Matrix2T<T> &m) const noexcept
		{
		    return !(*this == m);
		}

    UT_Matrix2T<T>  &operator=(T val)
		    {
			matx[0][0] = val; matx[0][1] = 0;
			matx[1][0] = 0;   matx[1][1] = val;
			return *this;
		    }
    UT_Matrix2T<T>  &operator+=(T scalar)
		    {
			matx[0][0]+=scalar; matx[0][1]+=scalar;
			matx[1][0]+=scalar; matx[1][1]+=scalar;
			return *this;
		    }
    UT_Matrix2T<T>  &operator-=(T scalar)
		    {
			return operator+=(-scalar);
		    }
    UT_Matrix2T<T>  &operator*=(T scalar)
		    {
			matx[0][0]*=scalar; matx[0][1]*=scalar;
			matx[1][0]*=scalar; matx[1][1]*=scalar;
			return *this;
		    }
    UT_Matrix2T<T>  &operator/=(T scalar)
		    {
			return operator*=( 1.0f/scalar );
		    }

    // Vector2 operators:
    template <typename S>
    inline
    UT_Matrix2T<T>  &operator=(const UT_Vector2T<S> &vec);
    template <typename S>
    inline
    UT_Matrix2T<T>  &operator+=(const UT_Vector2T<S> &vec);
    template <typename S>
    inline
    UT_Matrix2T<T>  &operator-=(const UT_Vector2T<S> &vec);

    constexpr T determinant() const noexcept
		{
		    return matx[0][0]*matx[1][1] - matx[0][1]*matx[1][0];
		}
    constexpr T trace() const noexcept
                { return matx[0][0] + matx[1][1]; }

    /// Returns eigenvalues of this matrix
    template <typename S>
    int		eigenvalues(UT_Vector2T<S> &r, UT_Vector2T<S> &i) const;

    /// Invert this matrix and return 0 if OK, 1 if singular.
    int		invert()
    {
        // NOTE: The other invert function should work on *this,
        //       since it does all reading before all writing.
        return invert(*this);
    }

    /// Invert the matrix and return 0 if OK, 1 if singular.
    /// Puts the inverted matrix in m, and leaves this matrix unchanged.
    int		invert(UT_Matrix2T<T> &m) const
    {
        // Effectively rescale the matrix for the determinant check,
        // because if the whole matrix is close to zero, having a
        // small determinant may be fine.
        T scale2 = SYSmax(matx[0][0]*matx[0][0],
                          matx[0][1]*matx[0][1],
                          matx[1][0]*matx[1][0],
                          matx[1][1]*matx[1][1]);
        T det = determinant();
        if (!SYSequalZero(det, tolerance()*scale2))
        {
            // Finish all reading before writing to m,
            // so that invert() can call invert(*this)
            m = UT_Matrix2T<T>( matx[1][1],-matx[0][1],
                               -matx[1][0], matx[0][0]);
            m *= (1.0f/det);

            return 0;
        }
        return 1;
    }

    /// Returns the tolerance of our class.
    T		tolerance() const;

    // Solve a 2x2 system of equations Ax=b, where A is this matrix, b is
    // given and x is unknown. The method returns 0 if the determinant is not
    // 0, and 1 otherwise.
    template <typename S>
    int		solve(const UT_Vector2T<S> &b, UT_Vector2T<S> &x) const
    {
        // Effectively rescale the matrix for the determinant check,
        // because if the whole matrix is close to zero, having a
        // small determinant may be fine.
        T scale2 = SYSmax(matx[0][0]*matx[0][0],
                          matx[0][1]*matx[0][1],
                          matx[1][0]*matx[1][0],
                          matx[1][1]*matx[1][1]);
        T det = determinant();
        if (!SYSequalZero(det, tolerance()*scale2))
        {
            T recipdet = 1.0f/det;
            x = UT_Vector2T<S>(
                    recipdet * (matx[1][1]*b.x() - matx[0][1]*b.y()),
                    recipdet * (matx[0][0]*b.y() - matx[1][0]*b.x()));
            return 0;
        }
        return 1;
    }

    // Transpose this matrix or return its transpose.
    void	transpose(void)
		{
		    T tmp;
		    tmp=matx[0][1];  matx[0][1]=matx[1][0];  matx[1][0]=tmp;
		}
    UT_Matrix2T<T>	transpose(void) const;

    // check for equality within a tolerance level
    bool	isEqual( const UT_Matrix2T<T> &m,
			 T tolerance=T(SYS_FTOLERANCE) ) const
		{
		    return (&m == this) || (
		    SYSisEqual( matx[0][0], m.matx[0][0], tolerance ) &&
		    SYSisEqual( matx[0][1], m.matx[0][1], tolerance ) &&

		    SYSisEqual( matx[1][0], m.matx[1][0], tolerance ) &&
		    SYSisEqual( matx[1][1], m.matx[1][1], tolerance ) );
		}

    // Matrix += b * v1 * v2T
    template <typename S>
    void	outerproductUpdate(T b, 
			     const UT_Vector2T<S> &v1, const UT_Vector2T<S> &v2)
    {
	T bv1;
	bv1 = b * v1.x();
	matx[0][0]+=bv1*v2.x();
	matx[0][1]+=bv1*v2.y();
	bv1 = b * v1.y();
	matx[1][0]+=bv1*v2.x();
	matx[1][1]+=bv1*v2.y();
    }

    /// Set the matrix to identity
    void	identity()
		{
		    matx[0][0]=(T)1;matx[0][1]=(T)0;
		    matx[1][0]=(T)0;matx[1][1]=(T)1;
		}
    /// Set the matrix to zero
    void	zero()	        { *this = 0; }

    bool isIdentity() const
    {
        // NB: DO NOT USE TOLERANCES
        return(
            matx[0][0]==1.0f && matx[0][1]==0.0f &&
            matx[1][0]==0.0f && matx[1][1]==1.0f);
    }
    bool isZero() const
    {
        // NB: DO NOT USE TOLERANCES
        return (
            matx[0][0]==0.0f && matx[0][1]==0.0f &&
            matx[1][0]==0.0f && matx[1][1]==0.0f);
    }

    /// Create a rotation matrix for the given angle in radians
    static UT_Matrix2T<T>	rotationMat(T theta);

    /// Rotate by theta radians
    void	rotate(T theta)
		    { (*this) *= rotationMat(theta); }

    /// Multiply this matrix by a scale matrix with diagonal (sx, sy):
    /// @{
    void	scale(T sx, T sy)
		{
		    matx[0][0] *= sx; matx[0][1] *= sy; 
		    matx[1][0] *= sx; matx[1][1] *= sy;
		}
    inline void scale(const UT_Vector2T<T> &s)
		{ scale(s(0), s(1)); }
    /// @}

    /// Initialize this matrix to zero.
    void	initialize()
		{
		    matx[0][0]=matx[0][1]= (T)0;
		    matx[1][0]=matx[1][1]= (T)0;
		}

    /// Return a matrix entry. No bounds checking on subscripts.
    // @{
    SYS_FORCE_INLINE T &operator()(unsigned row, unsigned col) noexcept
		 {
		     UT_ASSERT_P(row < 2 && col < 2);
		     return matx[row][col];
		 }
    SYS_FORCE_INLINE T operator()(unsigned row, unsigned col) const noexcept
		 {
		     UT_ASSERT_P(row < 2 && col < 2);
		     return matx[row][col];
		 }
    // @}

    /// Return the raw matrix data.
    // @{
    const T	  *data(void) const	{ return myFloats; }
    T		   *data(void)		{ return myFloats; }
    // @}

    /// Compute a hash
    unsigned	hash() const	{ return SYSvector_hash(data(), tuple_size); }

    /// Return a matrix row. No bounds checking on subscript.
    // @{
    T		*operator()(unsigned row)
		 {
		     UT_ASSERT_P(row < 2);
		     return matx[row];
		 }
    const T	*operator()(unsigned row) const
		 {
		     UT_ASSERT_P(row < 2);
		     return matx[row];
		 }
    inline
    const UT_Vector2T<T> &operator[](unsigned row) const;
    inline
    UT_Vector2T<T> &operator[](unsigned row);
    // @}
	
    /// Euclidean or Frobenius norm of a matrix.
    /// Does sqrt(sum(a_ij ^2))
    T		 getEuclideanNorm() const
		 { return SYSsqrt(getEuclideanNorm2()); }
    /// Euclidean norm squared.
    T		 getEuclideanNorm2() const;

    // I/O methods.  Return 0 if read/write successful, -1 if unsuccessful.
    int			 save(std::ostream &os, int binary) const;
    bool		 load(UT_IStream &is);

    void		 outAsciiNoName(std::ostream &os) const;

    /// @{
    /// Methods to serialize to a JSON stream.  The matrix is stored as an
    /// array of 4 reals.
    bool		save(UT_JSONWriter &w) const;
    bool		save(UT_JSONValue &v) const;
    bool		load(UT_JSONParser &p);
    /// @}

    // I/O friends:
    friend std::ostream	&operator<<(std::ostream &os, const UT_Matrix2T<T> &v)
			{
			    // v.className(os);
			    v.outAsciiNoName(os);
			    return os;
			}

    /// Returns the vector size
    static int		entries()	{ return tuple_size; }

private:
    void		 writeClassName(std::ostream &os) const;
    static const char	*className();
    
    union {
	T	matx[2][2];
	T	myFloats[tuple_size];
    };
};

#include "UT_Vector2.h"

template <>
inline
float UT_Matrix2T<float>::tolerance() const
{
    return 1e-5f;
}

template <>
inline
double UT_Matrix2T<double>::tolerance() const
{
    return 1e-11;
}

// Special instantiations to handle the fact that we have UT_Vector2T<int64>::getBary()
template <>
inline
int32 UT_Matrix2T<int32>::tolerance() const
{
    return 0;
}

template <>
inline
int64 UT_Matrix2T<int64>::tolerance() const
{
    return 0;
}

template <typename T>
template <typename S>
inline 
UT_Matrix2T<T>  &UT_Matrix2T<T>::operator=(const UT_Vector2T<S> &vec)
{
    matx[0][0] = matx[0][1] = vec.x();
    matx[1][0] = matx[1][1] = vec.y();
    return *this;
}

template <typename T>
template <typename S>
inline
UT_Matrix2T<T>	&UT_Matrix2T<T>::operator+=(const UT_Vector2T<S> &vec)
{
    matx[0][0]+=vec.x(); matx[0][1]+=vec.x();
    matx[1][0]+=vec.y(); matx[1][1]+=vec.y();
    return *this;
}

template <typename T>
template <typename S>
inline
UT_Matrix2T<T>  &UT_Matrix2T<T>::operator-=(const UT_Vector2T<S> &vec)
{
    matx[0][0]-=vec.x(); matx[0][1]-=vec.x();
    matx[1][0]-=vec.y(); matx[1][1]-=vec.y();
    return *this;
}

template <typename T>
inline
const UT_Vector2T<T> &UT_Matrix2T<T>::operator[](unsigned row) const
{
    UT_ASSERT_P(row < 2);
    return *(const UT_Vector2T<T>*)(matx[row]);
}

template <typename T>
inline
UT_Vector2T<T> &UT_Matrix2T<T>::operator[](unsigned row)
{
    UT_ASSERT_P(row < 2);
    return *(UT_Vector2T<T>*)(matx[row]);
}


// Free floating functions:
template <typename T, typename S>
inline
UT_Matrix2T<T>      operator+(const UT_Vector2T<S> &vec, const UT_Matrix2T<T> &mat)
{
    return mat+vec;
}

template <typename T>
inline
UT_Matrix2T<T>      operator+(T sc, const UT_Matrix2T<T> &m1)
{
    return m1+sc;
}

template <typename T>
inline
UT_Matrix2T<T>      operator-(const UT_Matrix2T<T> &m1, T sc)
{
    return m1+(-sc);
}

template <typename T,typename S>
inline
UT_Matrix2T<T>      operator*(S sc, const UT_Matrix2T<T> &m1)
{
    return m1*sc;
}

template <typename T>
inline
UT_Matrix2T<T>      operator/(const UT_Matrix2T<T> &m1, T scalar)
{
    return m1 * (1.0f/scalar);
}

template <typename T>
inline
UT_Matrix2T<T>	SYSmin(const UT_Matrix2T<T> &v1, const UT_Matrix2T<T> &v2)
{
    return UT_Matrix2T<T>(
	    SYSmin(v1(0,0), v2(0,0)),
	    SYSmin(v1(0,1), v2(0,1)),
	    SYSmin(v1(1,0), v2(1,0)),
	    SYSmin(v1(1,1), v2(1,1))
    );
}

template <typename T>
inline
UT_Matrix2T<T>	SYSmax(const UT_Matrix2T<T> &v1, const UT_Matrix2T<T> &v2)
{
    return UT_Matrix2T<T>(
	    SYSmax(v1(0,0), v2(0,0)),
	    SYSmax(v1(0,1), v2(0,1)),
	    SYSmax(v1(1,0), v2(1,0)),
	    SYSmax(v1(1,1), v2(1,1))
    );
}

template <typename T,typename S>
inline
UT_Matrix2T<T> SYSlerp(const UT_Matrix2T<T> &v1, const UT_Matrix2T<T> &v2, S t)
{
    return UT_Matrix2T<T>(
            SYSlerp(v1(0,0), v2(0,0), t),
            SYSlerp(v1(0,1), v2(0,1), t),
            SYSlerp(v1(1,0), v2(1,0), t),
            SYSlerp(v1(1,1), v2(1,1), t)
    );
}

#ifndef UT_DISABLE_VECTORIZE_MATRIX
template <>
inline
UT_Matrix2T<float> SYSlerp(const UT_Matrix2T<float> &v1, const UT_Matrix2T<float> &v2, float t)
{
    const v4uf l(v1.data());
    const v4uf r(v2.data());

    UT_Matrix2T<float> result_mat;
    vm_store(result_mat.data(), SYSlerp(l, r, t).vector);
    return result_mat;
}
#endif

template<typename T>
struct UT_FixedVectorTraits<UT_Matrix2T<T> >
{
    typedef UT_FixedVector<T,4> FixedVectorType;
    typedef T DataType;
    static const exint TupleSize = 4;
    static const bool isVectorType = true;
};

// Overload for custom formatting of UT_Matrix2T<T> with UTformat.
template<typename T>
UT_API size_t format(char *buffer, size_t buffer_size, const UT_Matrix2T<T> &v);

#endif

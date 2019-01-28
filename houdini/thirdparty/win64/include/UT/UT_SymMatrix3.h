/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_SymMatrix3.h (UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_SYMMATRIX3_H_INCLUDED__
#define __UT_SYMMATRIX3_H_INCLUDED__

#include "UT_API.h"

#include "UT_Assert.h"
#include "UT_VectorTypes.h"
#include <SYS/SYS_Inline.h>
#include <SYS/SYS_Math.h>   // for SYSlerp, ...
#include <SYS/SYS_Types.h>

#include <string.h>	    // for memset, ...


// Forward declaration
template <typename T> class UT_SymMatrix3T;

// Free floating methods that operate on UT_SymMatrix3T
template <typename T>
UT_SymMatrix3T<T> operator+(const UT_SymMatrix3T<T> &m1, const UT_SymMatrix3T<T> &m2);
template <typename T>
UT_SymMatrix3T<T> operator-(const UT_SymMatrix3T<T> &m1, const UT_SymMatrix3T<T> &m2);
template <typename T>
UT_SymMatrix3T<T> operator*(const UT_SymMatrix3T<T> &m, const T scale);
template <typename T>
UT_SymMatrix3T<T> operator*(const T scale, const UT_SymMatrix3T<T> &m);


/// Generic symmetric 3x3 matrix
template <typename T>
class UT_SymMatrix3T
{
public:
    typedef T			value_type;
    typedef UT_SymMatrix3T<T>	type;
    static const int		tuple_size = 6;

    /// Construct uninitialized matrix
    SYS_FORCE_INLINE UT_SymMatrix3T<T>() = default;

    /// Construct matrix with uniform scale
    explicit UT_SymMatrix3T<T>(T s)
    {
	setScale(s, s, s);
    }

    /// Construct matrix with arbitrary scale
    explicit UT_SymMatrix3T<T>(const UT_Vector3T<T>& s)
    {
	setScale(s.x(), s.y(), s.z());
    }

    /// Construct matrix with full components
    UT_SymMatrix3T<T>(
	T q00, T q10, T q11, T q20, T q21, T q22)
    {
	myLower.q00 = q00;
	myLower.q10 = q10; myLower.q11 = q11;
	myLower.q20 = q20; myLower.q21 = q21; myLower.q22 = q22;
    }

    /// Set this to the zero matrix
    inline void zero()
    {
	::memset(&myV[0], 0, sizeof(myV));
    }

    /// Set this to the identity matrix
    inline void identity()
    {
	setScale(1, 1, 1);
    }

    /// Return whether this is the identity matrix.
    inline bool isIdentity() const
    {
        return myLower.q00 == 1.0f && myLower.q11 == 1.0f &&
               myLower.q22 == 1.0f && myLower.q10 == 0.0f &&
               myLower.q20 == 0.0f && myLower.q21 == 0.0f;
    }

    /// Set this to a scale matrix
    inline void setScale(T sx, T sy, T sz)
    {
	myLower.q00 = sx;
	myLower.q11 = sy;
	myLower.q22 = sz;
	myLower.q10 = myLower.q20 = myLower.q21 = 0;
    }
    /// Set this to a scale matrix
    inline void setScale(const UT_Vector3T<T>& s)
    {
	setScale(s.x(), s.y(), s.z());
    }

    /// Return element (i,j)
    inline T operator()(const int i, const int j) const
    {
	UT_ASSERT_P(i >= 0 && i < 3 && j >= 0 && j < 3);
	if (i <= j)
	    return myV[(j*(j + 1))/2 + i];
	else
	    return myV[(i*(i + 1))/2 + j];
    }
    /// Return reference to element (i,j)
    inline T& operator()(const int i, const int j)
    {
	UT_ASSERT_P(i >= 0 && i < 3 && j >= 0 && j < 3);
	if (i <= j)
	    return myV[(j*(j + 1))/2 + i];
	else
	    return myV[(i*(i + 1))/2 + j];
    }

    inline type& operator+=(const type& m)
    {
	for (int i = 0; i < tuple_size; ++i)
	    myV[i] += m.myV[i];
	return *this;
    }
    inline type& operator-=(const type& m)
    {
	for (int i = 0; i < tuple_size; ++i)
	    myV[i] -= m.myV[i];
	return *this;
    }
    inline type& operator*=(T scalar)
    {
	for (int i = 0; i < tuple_size; ++i)
	    myV[i] *= scalar;
	return *this;
    }
    inline type& operator/=(T scalar)
    {
	return operator*=(1.0/scalar);
    }

    /// Set this to a linear interpolation of the two given transforms:
    /// *this = a + t*(b - a)
    void lerp(const type& a, const type& b, T t)
    {
	for (int i = 0; i < 6; ++i)
	    myV[i] = SYSlerp(a.myV[i], b.myV[i], t);
    }

    /// Inner class to access the elements symbolically
    struct LowerTri
    {
	T   q00, q10, q11, q20, q21, q22;
    };
    struct UpperTri
    {
	T   q00, q01, q11, q02, q12, q22;
    };

    /// Return the raw pointer to an array of tuple_size (6) elements
    /// @{
    const T*	data() const		{ return &myV[0]; }
    T*		data()			{ return &myV[0]; }
    /// @}

    /// Return reference to the lower triangular elements for symbolic access
    const LowerTri &   lowerTri() const	{ return myLower; }

    /// Return reference to the upper triangular elements for symbolic access
    const UpperTri &   upperTri() const	{ return myUpper; }


    void		transpose() {}

    // Sets the matrix to v * vT
    void		outerproduct(const UT_Vector3T<T> &v);
    void		outerproduct(T a, T b, T c);

    // Updates: Matrix += v * vT
    void		outerproductUpdate(const UT_Vector3T<T> &v);
    void		outerproductUpdate(T a, T b, T c);

    // Updates: Matrix += coef * v * vT
    void		outerproductUpdate(const UT_Vector3T<T> &v, T coef);
    void		outerproductUpdate(T a, T b, T c, T coef);

    // Calculates vT * Matrix * v
    T			vQv(const UT_Vector3T<T> &v) const;

private:
    // myV stores the elements of upper triangular portion of the symmetric
    // matrix in a column-major form (or equivalently, the lower triangular
    // portion in row-major form). The element (I, J) with I <= J, is stored
    // at index: c = (J * (J + 1))/2 + I
    // So the matrix is:
    //     myV[0] myV[1] myV[3]
    //     myV[1] myV[2] myV[4]
    //     myV[3] myV[4] myV[5]
    //
    union {
	T	    myV[6];
	LowerTri    myLower;
	UpperTri    myUpper;
    };

    template <typename S> friend class	UT_Matrix3T;
    template <typename S> friend class	UT_Matrix4T;
};
typedef UT_SymMatrix3T<float>	UT_SymMatrix3F;
typedef UT_SymMatrix3T<double>	UT_SymMatrix3D;
typedef UT_SymMatrix3T<fpreal>	UT_SymMatrix3R;

///////////////////////////////////////////////////////////////////////////////
//
// Implementations
//

/// Return (m1 + m2)
template <typename T>
UT_SymMatrix3T<T>
operator+(const UT_SymMatrix3T<T> &m1, const UT_SymMatrix3T<T> &m2)
{
    typedef typename UT_SymMatrix3T<T>::LowerTri LowerTri;
    const LowerTri& x = m1.lowerTri();
    const LowerTri& y = m2.lowerTri();
    return UT_SymMatrix3T<T>(x.q00 + y.q00,
			     x.q10 + y.q10, x.q11 + y.q11,
			     x.q20 + y.q20, x.q21 + y.q21, x.q22 + y.q22);
}

/// Return (m1 - m2)
template <typename T>
UT_SymMatrix3T<T>
operator-(const UT_SymMatrix3T<T> &m1, const UT_SymMatrix3T<T> &m2)
{
    typedef typename UT_SymMatrix3T<T>::LowerTri LowerTri;
    const LowerTri& x = m1.lowerTri();
    const LowerTri& y = m2.lowerTri();
    return UT_SymMatrix3T<T>(x.q00 - y.q00,
			     x.q10 - y.q10, x.q11 - y.q11,
			     x.q20 - y.q20, x.q21 - y.q21, x.q22 - y.q22);
}

/// Return (m * s) for scalar s
template <typename T>
UT_SymMatrix3T<T>
operator*(const UT_SymMatrix3T<T> &m, const T scale)
{
    typedef typename UT_SymMatrix3T<T>::LowerTri LowerTri;
    const LowerTri& l = m.lowerTri();
    return UT_SymMatrix3T<T>(l.q00 * scale,
			     l.q10 * scale, l.q11 * scale,
			     l.q20 * scale, l.q21 * scale, l.q22 * scale);
}

/// Return (s * m) for scalar s
template <typename T>
UT_SymMatrix3T<T>
operator*(const T scale, const UT_SymMatrix3T<T> &m)
{
    typedef typename UT_SymMatrix3T<T>::LowerTri LowerTri;
    const LowerTri& l = m.lowerTri();
    return UT_SymMatrix3T<T>(l.q00 * scale,
			     l.q10 * scale, l.q11 * scale,
			     l.q20 * scale, l.q21 * scale, l.q22 * scale);
}

template <typename T>
void
UT_SymMatrix3T<T>::outerproduct(const UT_Vector3T<T> &v)
{
    outerproduct(v.x(), v.y(), v.z());
}

template <typename T>
void
UT_SymMatrix3T<T>::outerproduct(T a, T b, T c)
{
    myUpper.q00 = a * a; myUpper.q01 = a * b; myUpper.q02 = a * c;
    myUpper.q11 = b * b; myUpper.q12 = b * c;
    myUpper.q22 = c * c;
}

template <typename T>
void
UT_SymMatrix3T<T>::outerproductUpdate(const UT_Vector3T<T> &v)
{
    outerproductUpdate(v.x(), v.y(), v.z());
}

template <typename T>
void
UT_SymMatrix3T<T>::outerproductUpdate(T a, T b, T c)
{
    myUpper.q00 += a * a; myUpper.q01 += a * b; myUpper.q02 += a * c;
    myUpper.q11 += b * b; myUpper.q12 += b * c;
    myUpper.q22 += c * c;

}

template <typename T>
void
UT_SymMatrix3T<T>::outerproductUpdate(const UT_Vector3T<T> &v, T coef)
{
    outerproductUpdate(v.x(), v.y(), v.z(), coef);
}

template <typename T>
void
UT_SymMatrix3T<T>::outerproductUpdate(T a, T b, T c, T coef)
{
    myUpper.q00 += coef * (a * a); myUpper.q01 += coef * (a * b); 
    myUpper.q02 += coef * (a * c);
    myUpper.q11 += coef * (b * b); myUpper.q12 += coef * (b * c);
    myUpper.q22 += coef * (c * c);

}

// Calculates vT this v
template <typename T>
T
UT_SymMatrix3T<T>::vQv(const UT_Vector3T<T> &v) const
{
    T a = v.x(), b = v.y(), c = v.z();

    return (a * (a * myUpper.q00 + 2 * (b * myUpper.q01 + c * myUpper.q02)) +
	    b * (b * myUpper.q11 + 2 * (c * myUpper.q12)) +
	    c * (c * myUpper.q22 ));
}


#endif // __UT_SYMMATRIX3_H_INCLUDED__

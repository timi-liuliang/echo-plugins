/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Complex.h ( UT Library, C++)
 *
 * COMMENTS:
 *
 *   Defines a complex number and some simple complex operations.
 *
 */

#ifndef __UT_COMPLEX__
#define __UT_COMPLEX__

#include "UT_API.h"
#include "UT_Vector2.h"

#include <SYS/SYS_Math.h>
#include <SYS/SYS_Types.h>
#include <SYS/SYS_TypeDecorate.h>


template <typename T> class UT_ComplexT;
typedef UT_ComplexT<fpreal32> UT_ComplexF;
typedef UT_ComplexT<fpreal64> UT_ComplexD;
// UT_Complex typedef for backward compatibility with old non-template version.
typedef UT_ComplexT<fpreal64> UT_Complex;

// Declare the following types as POD so that UT_ValArray can be optimized
SYS_DECLARE_IS_POD(UT_ComplexF)
SYS_DECLARE_IS_POD(UT_ComplexD)


template <typename T>
class UT_ComplexT
{
public:
    UT_ComplexT();
    UT_ComplexT(T r,T i);
    UT_ComplexT(const UT_ComplexT &);
    UT_ComplexT(const UT_Vector2T<T> &vec)
	: myReal(vec.x())
	, myImag(vec.y()) {}

    void         set(T r,T i);

    UT_ComplexT & operator=(const UT_ComplexT &);
    int          operator==(const UT_ComplexT &) const;

    UT_ComplexT operator+(T) const;
    UT_ComplexT operator+(const UT_ComplexT &) const;
    UT_ComplexT operator-(T) const;
    UT_ComplexT operator-(const UT_ComplexT &) const;

    UT_ComplexT operator*(T) const;
    UT_ComplexT operator*(const UT_ComplexT &) const;
    UT_ComplexT operator/(T) const;
    UT_ComplexT operator/(const UT_ComplexT &) const;

    operator	UT_Vector2T<T>() const
			{ return UT_Vector2T<T>(myReal, myImag); }

    UT_ComplexT pow(T exp) const;

    T	  real() const		{ return myReal; }
    T&	  real() 		{ return myReal; }
    T	  imaginary() const	{ return myImag; }
    T&	  imaginary() 		{ return myImag; }

    T      magnitude() const;
    T      magnitude2() const;

    // Phase is returned in radians
    T      phase() const;


private:
    T      myReal;
    T      myImag;
};

// Implementation
template <typename T>
inline
UT_ComplexT<T>::UT_ComplexT()
{
    myReal = 0.0F;
    myImag = 0.0F;
}

template <typename T>
inline
UT_ComplexT<T>::UT_ComplexT(T r,T i)
{
    myReal = r;
    myImag = i;
}

template <typename T>
inline
UT_ComplexT<T>::UT_ComplexT(const UT_ComplexT &source)
{
    myReal = source.real();
    myImag = source.imaginary();
}

template <typename T>
inline void
UT_ComplexT<T>::set(T r,T i)
{
    myReal = r;
    myImag = i;
}

template <typename T>
inline UT_ComplexT<T> &
UT_ComplexT<T>::operator=(const UT_ComplexT &source)
{
    myReal = source.real();
    myImag = source.imaginary();

    return *this;
}

template <typename T>
inline int
UT_ComplexT<T>::operator==(const UT_ComplexT &source) const
{
    if(SYSisEqual(source.real(), myReal) &&
       SYSisEqual(source.imaginary(), myImag))
	return 1;
    return 0;
}

template <typename T>
inline UT_ComplexT<T>
UT_ComplexT<T>::operator+(T r) const
{
    return UT_ComplexT(myReal + r, myImag);
}

template <typename T>
inline UT_ComplexT<T>
UT_ComplexT<T>::operator-(T r) const
{
    return UT_ComplexT(myReal - r, myImag);
}

template <typename T>
inline UT_ComplexT<T>
UT_ComplexT<T>::operator+(const UT_ComplexT &add) const
{
    return UT_ComplexT(myReal + add.real(), myImag + add.imaginary());
}

template <typename T>
inline UT_ComplexT<T>
UT_ComplexT<T>::operator-(const UT_ComplexT &sub) const
{
    return UT_ComplexT(myReal - sub.real(), myImag - sub.imaginary());
}

template <typename T>
inline UT_ComplexT<T>
UT_ComplexT<T>::operator*(T a) const
{
    return UT_ComplexT(myReal * a, myImag * a);
}

template <typename T>
inline UT_ComplexT<T>
UT_ComplexT<T>::operator*(const UT_ComplexT &mult) const
{
    T        r,i;

    r = mult.real();
    i = mult.imaginary();

    return UT_ComplexT(myReal * r - myImag * i,
		      myReal * i + myImag * r);
}

template <typename T>
inline UT_ComplexT<T>
UT_ComplexT<T>::pow(T exp) const
{
    UT_ComplexT raised;
    T mag,phs;

    mag = magnitude();
    phs=phase();

    mag = SYSpow(mag, exp);

    phs *= exp;

    return UT_ComplexT( mag * SYScos(phs), mag * SYSsin(phs));
}

template <typename T>
inline UT_ComplexT<T>
UT_ComplexT<T>::operator/(T d) const
{
    return UT_ComplexT(myReal/d,myImag/d);
}

template <typename T>
inline UT_ComplexT<T>
UT_ComplexT<T>::operator/(const UT_ComplexT & div) const
{
    T        r,i,d;

    r = div.real();
    i = div.imaginary();

    d = r*r + i*i;

    return UT_ComplexT((myReal * r + myImag * i)/d,
		      (myImag * r - myReal * i)/d);
}

template <typename T>
inline T
UT_ComplexT<T>::magnitude() const
{
    return SYSsqrt(myReal*myReal + myImag *myImag);
}

template <typename T>
inline T
UT_ComplexT<T>::magnitude2() const
{
    return myReal*myReal + myImag *myImag;
}

template <typename T>
inline T
UT_ComplexT<T>::phase() const
{
    return SYSatan(-myImag, myReal);
}

#endif


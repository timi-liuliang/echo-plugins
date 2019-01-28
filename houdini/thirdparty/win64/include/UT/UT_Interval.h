/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __UT_Interval_h__
#define __UT_Interval_h__

#include "UT_API.h"
#include <SYS/SYS_Types.h>
#include <SYS/SYS_Math.h>
#include <SYS/SYS_Hash.h>

#ifdef WIN32
    #undef min
    #undef max
#endif

template <typename T>
class UT_IntervalT
{
public:
    typedef T			value_type;
    typedef UT_IntervalT<T>	this_type;

       explicit UT_IntervalT(T a = 0);
		UT_IntervalT(T a, T b, bool order=false);

    UT_IntervalT &  operator+=(const UT_IntervalT &rhs);
    UT_IntervalT &  operator-=(const UT_IntervalT &rhs);
    UT_IntervalT &  operator*=(const UT_IntervalT &rhs);
    UT_IntervalT &  operator/=(const UT_IntervalT &rhs); 
    UT_IntervalT &  operator|=(const UT_IntervalT &rhs);
    UT_IntervalT &  operator&=(const UT_IntervalT &rhs);
    UT_IntervalT &  operator+=(T rhs);
    UT_IntervalT &  operator-=(T rhs);
    UT_IntervalT &  operator*=(T rhs);
    bool	    operator==(const UT_IntervalT &rhs) const;
    bool	    operator!=(const UT_IntervalT &rhs) const;

    void	assign(T Min, T Max, bool order_them=false)
		{
		    min = Min;
		    max = Max;
		    if (order_them && min > max)
		    {
			T tmp = min;
			min = max;
			max = tmp;
		    }
		}
    void	extendToContain( T a );
    void	minWith(const UT_IntervalT &rhs);
    void	maxWith(const UT_IntervalT &rhs);
    void	order();
    T		closest(T val) const;
    T		closest(T val, T wrap) const;
    T		avg() const	{ return (max+min)*0.5; }
    T		delta() const	{ return max - min; }
    void	square();

    UT_IntervalT    sqr() const;
    UT_IntervalT    abs() const;
    UT_IntervalT    pow(T arg) const;

    int		contains(T arg) const;
    int		isValid(T tol = 0.f) const;
    void	display() const;

    int equalZero(T tol = 0.00001f) const
    {
	return ((min>=-tol) && (min <= tol) && (max >=-tol) && (max <= tol));
    }

    int isEqual(const UT_IntervalT &v, T tol = 0.00001f) const
    {
	return ((min>=v.min-tol) && (min<=v.min+tol) && 
		(max>=v.max-tol) && (max<=v.max+tol));
    }

    void clampZero(T tol = 0.00001f)
    {
	if (min>=-tol && min<= tol) min = 0;
	if (max>=-tol && max<= tol) max = 0;
    }

    void negate() 
    { 
	T tmp = min; 
	min = -max; 
	max = -tmp; 
    }
    void invert(UT_IntervalT &v) const
    {
	v.min = min;
	v.max = max;
	if (!v.min) 
	    v.min+=0.00001f; 
	if(!v.max) 
	    v.max+=0.00001f; 
	T tmp = v.min; 
	v.min = 1/v.max; 
	v.max = 1/tmp; 

    }
    void invert() 
    { 
	if (!min) 
	    min+=0.00001f; 
	if(!max) 
	    max+=0.00001f; 
	T tmp = min; 
	min = 1/max; 
	max = 1/tmp; 
    }
    /// @{
    /// Compute a hash
    SYS_HashType	hash() const
    {
	SYS_HashType	h = SYShash(min);
	SYShashCombine(h, max);
	return h;
    }
    friend std::size_t	hash_value(const this_type &t) { return t.hash(); }
    /// @}
  public:
    T	min, max;
};

typedef UT_IntervalT<fpreal>	UT_IntervalR;
typedef UT_IntervalT<fpreal32>	UT_IntervalF;
typedef UT_IntervalT<fpreal64>	UT_IntervalD;
typedef UT_IntervalT<float>	UT_Interval;	// deprecated

template <typename T> UT_IntervalT<T> operator+(const UT_IntervalT<T> &lhs, const UT_IntervalT<T> &rhs);
template <typename T> UT_IntervalT<T> operator+(T lhs, const UT_IntervalT<T> &rhs);
template <typename T> UT_IntervalT<T> operator+(const UT_IntervalT<T> &lhs, T rhs);
template <typename T> UT_IntervalT<T> operator-(const UT_IntervalT<T> &lhs, const UT_IntervalT<T> &rhs);
template <typename T> UT_IntervalT<T> operator-(T lhs, const UT_IntervalT<T> &rhs);
template <typename T> UT_IntervalT<T> operator-(const UT_IntervalT<T> &lhs, T rhs);
template <typename T> UT_IntervalT<T> operator-(const UT_IntervalT<T> &rhs);
template <typename T> UT_IntervalT<T> operator*(const UT_IntervalT<T> &lhs, const UT_IntervalT<T> &rhs);
template <typename T> UT_IntervalT<T> operator*(T lhs, const UT_IntervalT<T> &rhs);
template <typename T> UT_IntervalT<T> operator*(const UT_IntervalT<T> &lhs, T rhs);
template <typename T> UT_IntervalT<T> operator/(const UT_IntervalT<T> &lhs, const UT_IntervalT<T> &rhs);
template <typename T> UT_IntervalT<T> operator/(T lhs, const UT_IntervalT<T> &rhs);
template <typename T> UT_IntervalT<T> operator/(const UT_IntervalT<T> &lhs, T rhs);
template <typename T> UT_IntervalT<T> operator|(const UT_IntervalT<T> &lhs, const UT_IntervalT<T> &rhs);
template <typename T> UT_IntervalT<T> operator&(const UT_IntervalT<T> &lhs, const UT_IntervalT<T> &rhs);
template <typename T> UT_IntervalT<T> maxOf(const UT_IntervalT<T> &arg0, const UT_IntervalT<T> &arg1);
template <typename T> UT_IntervalT<T> minOf(const UT_IntervalT<T> &arg0, const UT_IntervalT<T> &arg1);

//////////////////////////////////////////////////////////////////////////
//////////////// Inline Implementations //////////////////////////////////
//////////////////////////////////////////////////////////////////////////

template <typename T>
UT_IntervalT<T>::UT_IntervalT(T a): min(a), max(a) {}
template <typename T>
UT_IntervalT<T>::UT_IntervalT(T a, T b, bool order_them)
    : min(a), 
      max(b) 
{
    if (order_them && min > max)
    {
	T tmp = min;
	min = max;
	max = tmp;
    }
}

template <typename T>
UT_IntervalT<T> &
UT_IntervalT<T>::operator+=(const UT_IntervalT &rhs) 
{
  min += rhs.min;
  max += rhs.max;
  return *this;
}

template <typename T>
UT_IntervalT<T> &
UT_IntervalT<T>::operator+=(T rhs) 
{
  min += rhs;
  max += rhs;
  return *this;
}

template <typename T>
UT_IntervalT<T> &
UT_IntervalT<T>::operator-=(const UT_IntervalT<T> &rhs) 
{
  T t = rhs.min;
  min -= rhs.max;
  max -= t;
  return *this;
}

template <typename T>
UT_IntervalT<T> &
UT_IntervalT<T>::operator-=(T rhs) 
{
  min -= rhs;
  max -= rhs;
  return *this;
}

template <typename T>
UT_IntervalT<T> &
UT_IntervalT<T>::operator*=(const UT_IntervalT<T> &rhs) 
{
  T		t1 = min*rhs.min;
  T		t2 = max*rhs.min;
  T		t3 = min*rhs.max;
  T		t4 = max*rhs.max;
  T		tmin, tmax;

  if (t1 < t2) { tmin = t1; tmax = t2; }
  else { tmin = t2; tmax = t1; }
  if (tmax < t3) { tmax = t3; }
  if (tmin > t3) { tmin = t3; }
  if (tmax < t4) { tmax = t4; }
  if (tmin > t4) { tmin = t4; }
  min = tmin;
  max = tmax;
  return *this;
}

template <typename T>
UT_IntervalT<T> &
UT_IntervalT<T>::operator/=(const UT_IntervalT<T> &rhs) 
{
    UT_IntervalT<T> inverted;
    rhs.invert(inverted);
    (*this) *= inverted;
    return *this;
}



template <typename T>
UT_IntervalT<T> &
UT_IntervalT<T>::operator*=(T rhs) 
{
  if (rhs >= 0.0F)  { min *= rhs;  max *= rhs; }
  else { T t = min * rhs;  min = max * rhs; max = t; }
  return *this;
}
template <typename T>
bool
UT_IntervalT<T>::operator==(const UT_IntervalT<T> &rhs) const
{
    return min == rhs.min && max == rhs.max;
}
template <typename T>
bool
UT_IntervalT<T>::operator!=(const UT_IntervalT<T> &rhs) const
{
    return min != rhs.min || max != rhs.max;
}
 

template <typename T>
void
UT_IntervalT<T>::maxWith(const UT_IntervalT<T> &rhs) 
{
  if (min < rhs.min) min = rhs.min;
  if (max < rhs.max) max = rhs.max;
}

template <typename T>
void
UT_IntervalT<T>::extendToContain( T a )
{
    if( min>a )
	min = a;
    else if( max<a )
	max = a;
}

template <typename T>
void
UT_IntervalT<T>::minWith(const UT_IntervalT<T> &rhs) 
{
  if (min > rhs.min) min = rhs.min;
  if (max > rhs.max) max = rhs.max;
}

template <typename T>
UT_IntervalT<T> &
UT_IntervalT<T>::operator|=(const UT_IntervalT<T> &rhs) 
{
  if (min > rhs.min) min = rhs.min;
  if (max < rhs.max) max = rhs.max;
  return *this;
}

template <typename T>
UT_IntervalT<T> &
UT_IntervalT<T>::operator&=(const UT_IntervalT<T> &rhs) 
{
  if (min < rhs.min) min = rhs.min;
  if (max > rhs.max) max = rhs.max;
  return *this;
}


template <typename T>
UT_IntervalT<T>
UT_IntervalT<T>::sqr() const
{
  T tmin, tmax;
  if (min < 0.0F) {
    if (max < 0.0F) {
	tmin = max*max;
	tmax = min*min;
    }
    else {
	tmin = min*min;
	tmax = max*max;
	tmax = (tmin < tmax) ? tmax :  tmin;
	tmin = 0.0F;
    }
  }
  else {
    tmin = min*min;
    tmax = max*max;
  }
  return UT_IntervalT<T>(tmin, tmax);
}

template
<typename T>
void
UT_IntervalT<T>::square()
{
  T tmin, tmax;
  if (min < 0.0F) {
    if (max < 0.0F) {
	tmin = max*max;
	tmax = min*min;
    }
    else {
	tmin = min*min;
	tmax = max*max;
	tmax = (tmin < tmax) ? tmax :  tmin;
	tmin = 0.0F;
    }
  }
  else {
    tmin = min*min;
    tmax = max*max;
  }
  min = tmin;
  max = tmax;
}


template <typename T>
UT_IntervalT<T>
UT_IntervalT<T>::abs() const
{
  if (max < 0.0F) {
    return UT_IntervalT<T>(-max, -min);
  }
  else if (min < 0.0F) {
    T tmax;
    if (-min > max) tmax = -min;
    else tmax = max;
    return UT_IntervalT<T>(0.0F, tmax);
  }
  return *this;
}

template <typename T>
UT_IntervalT<T>
UT_IntervalT<T>::pow(T arg) const
{
  if (arg > 0) return UT_IntervalT<T>(SYSpow(min, arg), SYSpow(max, arg));
  else return UT_IntervalT<T>(SYSpow(max, arg), SYSpow(min, arg));
}

template <typename T>
T
UT_IntervalT<T>::closest(T val) const
{
    T	dmin, dmax;

    dmin = val - min;
    dmax = max - val;
    if (dmin < 0.0F)
	return min;
    else if (dmax < 0.0F)
	return max;
    return val;
}

template <typename T>
T
UT_IntervalT<T>::closest(T val, T wrap) const
{
    T	dmin, dmax;

    dmin = val - min;
    dmax = max - val;
    if (dmin < 0.0F)
    {
	if (dmin < dmax - wrap)
	    return max;
	else
	    return min;
    }
    else if (dmax < 0.0F)
    {
	if (dmin - wrap < dmax)
	    return max;
	else
	    return min;
    }
    return val;
}

#if 0
// WARNING! assumes that the interval is positive.
//
template <typename T>
UT_IntervalT<T>
UT_IntervalT<T>::pow(T arg) const
{
  if (arg > 0) return UT_IntervalT<T>(powf(min, arg), powf(max, arg));
  else return UT_IntervalT<T>(powf(max, arg), powf(min, arg));
}
#endif

template <typename T>
int
UT_IntervalT<T>::contains(T arg) const
{
  return ((arg >= min) && (arg <= max));
}

template <typename T>
int
UT_IntervalT<T>::isValid(T tol) const
{
  return SYSisLessOrEqual(min, max, tol);
}

template <typename T>
void
UT_IntervalT<T>::order()
{
    if (min > max)
    {
	T tmp = min;
	min = max;
	max = tmp;
    }
}

// Free functions

template <typename T>
UT_IntervalT<T>
operator+(const UT_IntervalT<T> &lhs, const UT_IntervalT<T> &rhs)
{
  return UT_IntervalT<T>(lhs.min + rhs.min, lhs.max + rhs.max);
}

template <typename T>
UT_IntervalT<T>
operator+(T lhs, const UT_IntervalT<T> &rhs)
{
  return UT_IntervalT<T>(rhs.min + lhs, rhs.max + lhs);
}


template <typename T>
UT_IntervalT<T>
operator+(const UT_IntervalT<T> &lhs, T rhs)
{
  return UT_IntervalT<T>(lhs.min + rhs, lhs.max + rhs);
}

template <typename T>
UT_IntervalT<T>
operator-(const UT_IntervalT<T> &lhs, const UT_IntervalT<T> &rhs)
{
  return UT_IntervalT<T>(lhs.min - rhs.max, lhs.max - rhs.min);
}

template <typename T>
UT_IntervalT<T>
operator-(T lhs, const UT_IntervalT<T> &rhs)
{
  return UT_IntervalT<T>(lhs - rhs.max, lhs - rhs.min);
}

template <typename T>
UT_IntervalT<T>
operator-(const UT_IntervalT<T> &lhs, T rhs)
{
  return UT_IntervalT<T>(lhs.min - rhs, lhs.max - rhs);
}

template <typename T>
UT_IntervalT<T>
operator-(const UT_IntervalT<T> &rhs)
{
  return UT_IntervalT<T>(-rhs.max, -rhs.min);
}

template <typename T>
UT_IntervalT<T>
operator*(const UT_IntervalT<T> &lhs, const UT_IntervalT<T> &rhs)
{
  T		t1 = lhs.min*rhs.min;
  T		t2 = lhs.max*rhs.min;
  T		t3 = lhs.min*rhs.max;
  T		t4 = lhs.max*rhs.max;
  T		tmin, tmax;

  if (t1 < t2) { tmin = t1; tmax = t2; }
  else { tmin = t2; tmax = t1; }
  if (tmax < t3) { tmax = t3; }
  if (tmin > t3) { tmin = t3; }
  if (tmax < t4) { tmax = t4; }
  if (tmin > t4) { tmin = t4; }
  return UT_IntervalT<T>(tmin, tmax);
}

template <typename T>
UT_IntervalT<T>
operator*(T lhs, const UT_IntervalT<T> &rhs)
{
  T tmin, tmax;
  if (lhs > 0.0F) { tmin = lhs*rhs.min;  tmax = lhs*rhs.max; }
  else { tmin = lhs*rhs.max;  tmax = lhs*rhs.min; }
  return UT_IntervalT<T>(tmin, tmax);
}

template <typename T>
UT_IntervalT<T>
operator*(const UT_IntervalT<T> &lhs, T rhs)
{
  T tmin, tmax;
  if (rhs > 0.0F) { tmin = rhs*lhs.min;  tmax = rhs*lhs.max; }
  else { tmin = rhs*lhs.max;  tmax = rhs*lhs.min; }
  return UT_IntervalT<T>(tmin, tmax);
}

template <typename T>
UT_IntervalT<T>
operator|(const UT_IntervalT<T> &lhs, const UT_IntervalT<T> &rhs)
{
  // extra parentheses to workaround bad g++ template parsing
  return UT_IntervalT<T>(((lhs.min) < (rhs.min))? lhs.min : rhs.min,
		  ((lhs.max) < (rhs.max))? rhs.max : lhs.max);
}

template <typename T>
UT_IntervalT<T>
operator&(const UT_IntervalT<T> &lhs, const UT_IntervalT<T> &rhs)
{
  // extra parentheses to workaround bad g++ template parsing
  return UT_IntervalT<T>(((lhs.min) < (rhs.min))? rhs.min : lhs.min,
		  ((lhs.max) < (rhs.max))? lhs.max : rhs.max);
}
template <typename T>
UT_IntervalT<T>
operator/(const UT_IntervalT<T> &lhs, const UT_IntervalT<T> &rhs)
{
    UT_IntervalT<T> v;
    rhs.invert(v);
    return lhs * v;
}

template <typename T>
UT_IntervalT<T>
operator/(T lhs, const UT_IntervalT<T> &rhs)
{
    UT_IntervalT<T> v;
    rhs.invert(v);
    v *= lhs;
    return v;
}

template <typename T>
UT_IntervalT<T>
operator/(const UT_IntervalT<T> &lhs, T rhs)
{
    if (!rhs) rhs += 0.00001f;
    return UT_IntervalT<T>(lhs.min / rhs, lhs.max / rhs);
}


template <typename T>
UT_IntervalT<T>
maxOf(const UT_IntervalT<T> &arg0, const UT_IntervalT<T> &arg1)
{
  // extra parentheses to workaround bad g++ template parsing
  return UT_IntervalT<T>(((arg0.min) < (arg1.min))? arg1.min : arg0.min,
		  ((arg0.max) < (arg1.max))? arg1.max : arg0.max);
}

template <typename T>
UT_IntervalT<T>
minOf(const UT_IntervalT<T> &arg0, const UT_IntervalT<T> &arg1)
{
  // extra parentheses to workaround bad g++ template parsing
  return UT_IntervalT<T>(((arg0.min) < (arg1.min))? arg0.min : arg1.min,
		  ((arg0.max) < (arg1.max))? arg0.max : arg1.max);
}

#include <stdio.h>

template <typename T>
void
UT_IntervalT<T>::display() const
{
    printf( "[%g, %g]", min, max );
}

#endif // __UT_Interval_h__

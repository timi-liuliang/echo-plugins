/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_SuperInterval.h ( UT Library, C++)
 *
 * COMMENTS: This is an extension to UT_Interval which allows exclusive
 *           and inclusive ranges.
 */

#ifndef __UT_SuperInterval__
#define __UT_SuperInterval__

#include "UT_API.h"
#include "UT_Interval.h"
#include <SYS/SYS_Math.h>

template <typename T>
class UT_SuperIntervalT : public UT_IntervalT<T>
{
public:
    UT_SuperIntervalT( UT_IntervalT<T> const& interval )
    {
	*this = interval;
    }

    UT_SuperIntervalT(T a, T b, bool ia = true, bool ib = true):
	UT_IntervalT<T>( a, b )
    {
	myMinInclusive = ia;
	myMaxInclusive = ib;
    }

    UT_SuperIntervalT()
    {
	myMinInclusive = true;
	myMaxInclusive = true;
    }

    ~UT_SuperIntervalT() {}

    UT_SuperIntervalT &operator=( UT_IntervalT<T> const& interval )
    {
	if( this != &interval )
	{
	    *(UT_IntervalT<T>*)this = interval;
	    myMinInclusive = true;
	    myMaxInclusive = true;
	}
	return *this;
    }

    // Returns true if arg is inside our interval range.
    // WARNING: This is masking the int contains(T) function
    // defined by UT_Interval!
    bool	contains(T arg, T tol=T(SYS_FTOLERANCE) ) const
    {
	return
	    (myMinInclusive ? 
		SYSisLessOrEqual(UT_IntervalT<T>::min, arg, tol) :
		SYSisLess(UT_IntervalT<T>::min, arg, tol) )
		&&
	    (myMaxInclusive ?
		SYSisLessOrEqual(arg, UT_IntervalT<T>::max, tol) :
		SYSisLess(arg, UT_IntervalT<T>::max, tol) );
    }

    // This adjusts the range of our own interval so it doesn't
    // exceed that of [c_min..c_max].  These boundaries are treated
    // as inclusive boundaries, and the interval is made inclusive
    // if clamped on an edge.
    void	clamp(T c_min, T c_max);

    void	display() const;

public:
    bool		myMinInclusive, myMaxInclusive;
};

typedef UT_SuperIntervalT<fpreal>	UT_SuperIntervalR;


//////////////////////////////////////////////////////////////////////////////
//
// Implementation
//

template <typename T>
void	
UT_SuperIntervalT<T>::clamp(T c_min, T c_max)
{
    if (UT_IntervalT<T>::min < c_min)
    {
	UT_IntervalT<T>::min = c_min;
	myMinInclusive = true;
    }
    if (UT_IntervalT<T>::max > c_max)
    {
	UT_IntervalT<T>::max = c_max;
	myMaxInclusive = true;
    }
}

#include <stdio.h>

template <typename T>
void
UT_SuperIntervalT<T>::display() const
{
    printf( "%c%g, %g%c",
	myMinInclusive ? '[' : '(',
	UT_IntervalT<T>::min,
	UT_IntervalT<T>::max,
	myMaxInclusive ? ']' : ')'
	);
}

#endif // __UT_SuperInterval__

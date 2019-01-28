/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT library (C++)
 *
 * COMMENTS:	This is a generalized look up table for effecient
 *		computation of single valued arrays.
 *		These use inline methods since the whole purpose
 *		of a LUT is speed.
 *
 * Revision 1.1.1.1  1995/09/17  18:10:22  mark
 * initial source release
 *
 * Revision 1.1  94/06/16  18:33:33  prisms
 * Initial revision
 * 
 */

#ifndef __UT_LUT_H__
#define __UT_LUT_H__

#include "UT_API.h"
#include <SYS/SYS_Types.h>
#include "UT_Assert.h"

class UT_API UT_LUT {
public:
     UT_LUT(fpreal val, int size, float (*eval)(float val, float t) = 0);
     explicit UT_LUT(int size, float (*eval)(float t) = 0);
    ~UT_LUT();

    /// Given a LUT defining a function which is already parametrized from 0 to
    /// 1, we then compute the "inverse" warp LUT which performs an equal-area
    /// warp from the unit parametrization to the map.
    void	buildConformalWarp(const UT_LUT &src);

    void	buildLUT(float (*eval)(float val, float t));
    void	buildLUT(float (*eval)(float t));

    void	setValue(fpreal v)	{ myVal = v; }
    fpreal	getValue() const	{ return myVal; }
    int		getSize() const		{ return mySize; }
    fpreal	getFSize() const	{ return myFSize; }
    fpreal	operator()(int i) const
		{
		    UT_ASSERT_P(i >= 0 && i <= mySize);
		    return myLut[i];
		}

//
//  The fast routines assume that t is guaranteed to be [0, 1]
//  The safe method does bounds checking.
//  Since floating point values are truncated (not rounded), the fast lookup is
//  safe for values up to 1+0.5F/mySize.
//
    fpreal	fastValue(fpreal t) const
    {
        UT_ASSERT_MSG_P((t==t), "t can't be NaN here!");
        return myLut[(int)(t*myFSize+0.5F)];
    }
    fpreal	safeValue(fpreal t) const
		{
		    fpreal rval;
			 if (t < 0) rval = myLut[0];
		    else if (t < 1) rval = myLut[(int)(t*myFSize+0.5F)];
		    else            rval = myLut[mySize]; // t >= 1 *OR* NaN
		    return rval;
		}

//
//  lerp methods do linear interpolation on the LUT
//
    fpreal	lerpFast(fpreal t) const
		{
                    UT_ASSERT_MSG_P((t==t), "t can't be NaN here!");
		    t *= myFSize;
		    int i1 = (int)t;
		    t -= (fpreal)i1;
		    return (1.0F - t)*myLut[i1] + t*myLut[i1+1];
		}
    fpreal	lerpSafe(fpreal t) const
		{
		    fpreal rval;
			 if (t < 0.0F)	rval = myLut[0];
		    else if (t < 1.0F)
		    {
			t *= myFSize;
			int i1 = (int)t;
			t -= (fpreal)i1;
			rval = (1.0F - t)*myLut[i1] + t*myLut[i1+1];
		    }
		    else // t >= 1.0F *OR* t is NaN
                        rval = myLut[mySize];
		    return rval;
		}

    int		 getLUTSize() const { return mySize; }
    fpreal	*getRawLUT() { return myLut; }

    void	bumpRefCount(void) { myRefCount++; }
    int		downRefCount(void) { return --myRefCount; }
protected:
private:
    fpreal	*myLut;		// The lookup table data
    fpreal	 myVal;		// The value associated with this LUT
    int		 mySize;	// Size of the LUT
    fpreal	 myFSize;	// Size as a fpreal (to avoid the cast)
    int		 myRefCount;	// Reference count for this LUT
};
#endif

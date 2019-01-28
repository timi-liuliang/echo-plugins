/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_Parametric.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_Parametric__
#define __GT_Parametric__

#include "GT_API.h"
#include <SYS/SYS_Types.h>

class UT_JSONWriter;

/// Class to keep track for a 2D parametric interval
class GT_API GT_Parametric {
public:
    GT_Parametric(fpreal u0=0, fpreal u1=1, fpreal v0=0, fpreal v1=1)
	    : myU0(u0), myU1(u1), myV0(v0), myV1(v1)
    {
    }
    GT_Parametric(const GT_Parametric &src)
	: myU0(src.myU0),
	  myU1(src.myU1),
	  myV0(src.myV0),
	  myV1(src.myV1)
    {
    }
    ~GT_Parametric() {}

    GT_Parametric	&operator=(const GT_Parametric &src)
			 {
			     init(src.myU0, src.myU1, src.myV0, src.myV1);
			     return *this;
			 }

    /// Split this parametric rectangle in U.  This will become the left, the
    /// returned value will be the right.
    GT_Parametric	splitU()
			{
			    fpreal	u1 = myU1;
			    myU1 = (myU0 + myU1) * .5;
			    return GT_Parametric(myU1, u1, myV0, myV1);
			}
    /// Split this parametric rectangle in V.  This will become the bottom, the
    /// returned value will be the top.
    GT_Parametric	splitV()
			{
			    fpreal	v1 = myV1;
			    myV1 = (myV0 + myV1) * .5;
			    return GT_Parametric(myU0, myU1, myV1, v1);
			}

    bool	containsU(fpreal u) const
		    { return u >= myU0 && u < myU1; }
    bool	containsV(fpreal v) const
		    { return v >= myV0 && v < myV1; }

    fpreal	closestU(fpreal u) const
		    { return getClosest(u, myU0, myU1); }
    fpreal	closestV(fpreal v) const
		    { return getClosest(v, myV0, myV1); }

    fpreal	getU0() const	{ return myU0; }
    fpreal	getU1() const	{ return myU1; }
    fpreal	getV0() const	{ return myV0; }
    fpreal	getV1() const	{ return myV1; }

    fpreal	getDU() const	{ return myU1 - myU0; }
    fpreal	getDV() const	{ return myV1 - myV0; }

    void	init(fpreal u0, fpreal u1, fpreal v0, fpreal v1)
		{
		    myU0 = u0;
		    myU1 = u1;
		    myV0 = v0;
		    myV1 = v1;
		}

    void	splitU(GT_Parametric &left, GT_Parametric &right) const;
    void	splitV(GT_Parametric &bottom, GT_Parametric &top) const;

    /// Save to JSON
    bool	save(UT_JSONWriter &w) const;
private:
    static inline fpreal	getClosest(fpreal v, fpreal min, fpreal max)
				{
				    if (v < min)
					return min;
				    if (v > max)
					return max;
				    return v;
				}

    fpreal	myU0, myU1, myV0, myV1;
};

#endif


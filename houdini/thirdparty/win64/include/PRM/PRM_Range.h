/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	PRM_Range.h (Parameter Library)
 *
 * COMMENTS:
 *		This parameter is used to set upper and lower limits on
 *		parameters. For simplicity only floating point numbers are
 *		used. These limits are not actually enforced by the parmeter
 *		library. But maybe they should. Discuss.
 */


#ifndef __PRM_RANGE__
#define __PRM_RANGE__

#include "PRM_API.h"
#include <SYS/SYS_Types.h>

enum PRM_RangeFlag
{
    PRM_RANGE_FREE	= 0x0,		// ie range is ignored.
    PRM_RANGE_UI	= 0x1,		// For some UI (sliders, etc) only
    PRM_RANGE_PRM	= 0x2,		// Strictly enforced by PRM, too
    PRM_RANGE_RESTRICTED= (PRM_RANGE_UI | PRM_RANGE_PRM) // both UI and PRM
};

class PRM_API PRM_Range
{
public:
    PRM_Range(PRM_RangeFlag theminflag = PRM_RANGE_UI, fpreal themin = 0,
	      PRM_RangeFlag themaxflag = PRM_RANGE_UI, fpreal themax = 1)
    {
	myMinFlag = theminflag;
	myMin = themin;
	myMaxFlag = themaxflag;
	myMax = themax;
    }

    bool	hasParmMin() const
		{
		    return ((myMinFlag & PRM_RANGE_PRM) != 0);
		}
    bool	hasParmMax() const
		{
		    return ((myMaxFlag & PRM_RANGE_PRM) != 0);
		}

    fpreal	getParmMin() const
		{
		    if (myMinFlag != PRM_RANGE_FREE)
			return myMin;
		    return -SYS_FPREAL_MAX;
		}
    fpreal	getParmMax() const
		{
		    if (myMaxFlag != PRM_RANGE_FREE)
			return myMax;
		    return  SYS_FPREAL_MAX;
		}

    bool	hasUIMin() const
		{
		    return ((myMinFlag & PRM_RANGE_UI) != 0);
		}
    bool	hasUIMax() const
		{
		    return ((myMaxFlag & PRM_RANGE_UI) != 0);
		}

    fpreal	getUIMin() const { return getParmMin(); }
    fpreal	getUIMax() const { return getParmMax(); }

private:
    PRM_RangeFlag	myMinFlag;
    PRM_RangeFlag       myMaxFlag;
    fpreal		myMin;
    fpreal		myMax;
};

#endif

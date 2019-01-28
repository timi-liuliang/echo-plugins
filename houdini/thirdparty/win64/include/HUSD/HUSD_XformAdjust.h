/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_XformAdjust_h__
#define __HUSD_XformAdjust_h__

#include "HUSD_API.h"
#include "HUSD_DataHandle.h"
#include <UT/UT_UniquePtr.h>

class HUSD_TimeCode;

class HUSD_API HUSD_XformAdjust
{
public:
			 HUSD_XformAdjust(HUSD_AutoAnyLock &lock,
				const HUSD_TimeCode &timecode);
			~HUSD_XformAdjust();

    bool		 adjustXformsForAuthoredPrims(
				const HUSD_AutoWriteLock &lock) const;

    bool		 getIsTimeVarying() const;

private:
    class husd_XformAdjustPrivate;

    UT_UniquePtr<husd_XformAdjustPrivate>	 myPrivate;
};

#endif


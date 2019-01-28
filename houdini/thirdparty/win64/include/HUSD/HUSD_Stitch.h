/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_Stitch_h__
#define __HUSD_Stitch_h__

#include "HUSD_API.h"
#include "HUSD_DataHandle.h"
#include <UT/UT_StringHolder.h>
#include <UT/UT_UniquePtr.h>

class HUSD_API HUSD_Stitch
{
public:
			 HUSD_Stitch();
			~HUSD_Stitch();

    bool		 addHandle(const HUSD_DataHandle &src);
    bool		 execute(HUSD_AutoWriteLock &lock) const;

private:
    class husd_StitchPrivate;

    UT_UniquePtr<husd_StitchPrivate>	 myPrivate;
};

#endif


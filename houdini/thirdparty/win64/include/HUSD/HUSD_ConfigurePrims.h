/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_ConfigurePrims_h__
#define __HUSD_ConfigurePrims_h__

#include "HUSD_API.h"
#include "HUSD_DataHandle.h"
#include "HUSD_TimeCode.h"
#include <UT/UT_StringHolder.h>

class HUSD_FindPrims;

class HUSD_API HUSD_ConfigurePrims
{
public:
			 HUSD_ConfigurePrims(HUSD_AutoWriteLock &lock);
			~HUSD_ConfigurePrims();

    bool		 setKind(const HUSD_FindPrims &findprims,
				const UT_StringRef &kind) const;
    bool		 setDrawMode(const HUSD_FindPrims &findprims,
				const UT_StringRef &drawmode) const;
    bool		 setPurpose(const HUSD_FindPrims &findprims,
				const UT_StringRef &purpose) const;
    bool		 setProxy(const HUSD_FindPrims &findprims,
				const UT_StringRef &proxy) const;
    bool		 setInstanceable(const HUSD_FindPrims &findprims,
				bool instanceable) const;
    bool		 setInvisible(const UT_StringRef &sdfpath,
				bool invisible,
				bool for_all_time,
				const HUSD_TimeCode &timecode) const;
    bool		 setInvisible(const HUSD_FindPrims &findprims,
				bool invisible,
				bool for_all_time,
				const HUSD_TimeCode &timecode) const;

private:
    HUSD_AutoWriteLock	&myWriteLock;
};

#endif


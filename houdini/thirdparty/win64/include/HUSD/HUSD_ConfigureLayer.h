/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_ConfigureLayer_h__
#define __HUSD_ConfigureLayer_h__

#include "HUSD_API.h"
#include "HUSD_DataHandle.h"
#include <UT/UT_StringHolder.h>

class HUSD_API HUSD_ConfigureLayer
{
public:
			 HUSD_ConfigureLayer(HUSD_AutoWriteLock &lock);
			~HUSD_ConfigureLayer();

    bool		 setSavePath(const UT_StringRef &save_path) const;
    bool		 setSaveControl(const UT_StringRef &save_control) const;
    bool		 setStartTime(fpreal64 start_time) const;
    bool		 setEndTime(fpreal64 end_time) const;
    bool		 setTimePerSecond(fpreal64 time_per_second) const;
    bool		 setFramesPerSecond(fpreal64 frames_per_second) const;
    bool		 setDefaultPrim(const UT_StringRef &primpath) const;

private:
    HUSD_AutoWriteLock	&myWriteLock;
};

#endif


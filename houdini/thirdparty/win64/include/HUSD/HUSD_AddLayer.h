/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_AddLayer_h__
#define __HUSD_AddLayer_h__

#include "HUSD_API.h"
#include "HUSD_DataHandle.h"
#include "HUSD_LayerOffset.h"
#include <GU/GU_DetailHandle.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_StringMap.h>

class HUSD_API HUSD_AddLayer
{
public:
			 HUSD_AddLayer(HUSD_AutoWriteLock &lock);
			~HUSD_AddLayer();

    bool		 addLayer(const UT_StringRef &filepath,
				const HUSD_LayerOffset &offset =
				    HUSD_LayerOffset(),
				const UT_StringMap<UT_StringHolder> &refargs =
				    UT_StringMap<UT_StringHolder>(),
				const GU_DetailHandle &gdh =
				    GU_DetailHandle());
    bool		 addLayer();

private:
    HUSD_AutoWriteLock	&myWriteLock;
};

#endif


/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_LockedStageRegistry_h__
#define __HUSD_LockedStageRegistry_h__

#include "HUSD_API.h"
#include "HUSD_LockedStage.h"
#include <UT/UT_Map.h>

class HUSD_API HUSD_LockedStageRegistry
{
public:
    static HUSD_LockedStageRegistry	&getInstance();

    HUSD_LockedStagePtr			 getLockedStage(int nodeid);
    void				 clearLockedStage(int nodeid);

private:
					 HUSD_LockedStageRegistry();
					~HUSD_LockedStageRegistry();

    UT_Map<int, HUSD_LockedStageWeakPtr> myLockedStageMap;
};

#endif


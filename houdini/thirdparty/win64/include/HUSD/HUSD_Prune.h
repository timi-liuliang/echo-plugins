/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_Prune_h__
#define __HUSD_Prune_h__

#include "HUSD_API.h"
#include "HUSD_DataHandle.h"

class HUSD_FindPrims;
class HUSD_TimeCode;

class HUSD_API HUSD_Prune
{
public:
			 HUSD_Prune(HUSD_AutoWriteLock &dest);
			~HUSD_Prune();

    enum PruneMethod
    {
	Deactivate,
	MakeInvisible
    };

    void		 prune(const HUSD_FindPrims &findprims,
				const HUSD_TimeCode &timecode,
				PruneMethod prune_method) const;

private:
    HUSD_AutoWriteLock	&myWriteLock;
};

#endif


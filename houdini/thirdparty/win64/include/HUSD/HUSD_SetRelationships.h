/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_SetRelationships_h__
#define __HUSD_SetRelationships_h__

#include "HUSD_API.h"
#include "HUSD_DataHandle.h"
#include <UT/UT_StringHolder.h>

class HUSD_API HUSD_SetRelationships
{
public:
			 HUSD_SetRelationships(HUSD_AutoWriteLock &lock);
			~HUSD_SetRelationships();

    bool		 setRelationship(const UT_StringRef &primpath,
				const UT_StringRef &relname,
				const UT_StringArray &destpaths) const;

private:
    HUSD_AutoWriteLock	&myWriteLock;
};

#endif


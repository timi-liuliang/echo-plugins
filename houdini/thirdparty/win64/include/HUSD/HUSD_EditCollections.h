/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __HUSD_EditCollections_h__
#define __HUSD_EditCollections_h__

#include "HUSD_API.h"
#include "HUSD_DataHandle.h"
#include <UT/UT_StringHolder.h>

class HUSD_FindPrims;

class HUSD_API HUSD_EditCollections
{
public:
			 HUSD_EditCollections(HUSD_AutoWriteLock &lock);
			~HUSD_EditCollections();

    bool		 createCollection(const UT_StringRef &primpath,
				const UT_StringRef &collectionname,
				const UT_StringRef &expansionrule,
				const HUSD_FindPrims &includeprims,
				const HUSD_FindPrims &excludeprims,
				bool createprim);

    bool		 createCollection(const UT_StringRef &primpath,
				const UT_StringRef &collectionname,
				const UT_StringRef &expansionrule,
				const HUSD_FindPrims &includeprims,
				bool createprim);

private:
    HUSD_AutoWriteLock	&myWriteLock;
};

#endif


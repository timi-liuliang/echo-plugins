/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __XUSD_PathPattern_h__
#define __XUSD_PathPattern_h__

#include "HUSD_API.h"
#include "HUSD_PathPattern.h"
#include <pxr/usd/sdf/path.h>

PXR_NAMESPACE_OPEN_SCOPE

struct XUSD_SpecialTokenData
{
    SdfPathSet	 myExpandedPathSet;
    SdfPathSet	 myCollectionPathSet;
};

class HUSD_API XUSD_PathPattern : public HUSD_PathPattern
{
public:
			 XUSD_PathPattern(const UT_StringArray &pattern_tokens,
				HUSD_AutoAnyLock &lock,
				HUSD_PrimTraversalDemands demands);
			 XUSD_PathPattern(const UT_StringRef &pattern,
				HUSD_AutoAnyLock &lock,
				HUSD_PrimTraversalDemands demands);
			~XUSD_PathPattern();

    void		 getCollections(SdfPathSet &collection_paths,
				SdfPathSet &expanded_collection_paths) const;
};

PXR_NAMESPACE_CLOSE_SCOPE

#endif


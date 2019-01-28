/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_GEOPrimCollectPacked.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_GEOPrimCollectPacked__
#define __GT_GEOPrimCollectPacked__

#include "GT_API.h"
#include "GT_GEOPrimCollect.h"
#include <GA/GA_Types.h>

/// Collector for packed primitives.
class GT_API GT_GEOPrimCollectPacked : public GT_GEOPrimCollect
{
public:
	     GT_GEOPrimCollectPacked() {}
    virtual ~GT_GEOPrimCollectPacked();

    /// @{
    /// Interface defined for GT_GEOPrimCollect
    virtual GT_GEOPrimCollectData *
		beginCollecting(const GT_GEODetailListHandle &geometry,
				const GT_RefineParms *parms) const;
    virtual GT_PrimitiveHandle
		collect(const GT_GEODetailListHandle &geo,
				const GEO_Primitive *const* prim_list,
				int nsegments,
				GT_GEOPrimCollectData *data) const;
    virtual GT_PrimitiveHandle
		endCollecting(const GT_GEODetailListHandle &geometry,
				GT_GEOPrimCollectData *data) const;
    /// @}
private:
};

#endif

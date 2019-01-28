/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *	This defines the detail class (the container for all points/primitives)
 *
 */

#ifndef __GEO_Macros_h__
#define __GEO_Macros_h__

#include <GA/GA_GBMacros.h>	// FOR loops for groups (point and prim)
#include "GEO_PrimList.h"

SYS_DEPRECATED_PUSH_DISABLE()

/// An adapter class to allow the iteration macros in this file to maintain a
/// local GEO_PrimList::GroupIterationCache object.
class GEO_API GEO_GBMixGroupFwdIterator
{
public:
    template<typename DETAIL_TYPE, typename PRIM_TYPE, typename SEC_PRIM_TYPE>
    SYS_DEPRECATED_HDK(13.0)
	inline GEO_GBMixGroupFwdIterator(
			    DETAIL_TYPE *gdp, const GA_PrimitiveGroup &group,
			    PRIM_TYPE *&prim, SEC_PRIM_TYPE *&sec)
			{
			    gdp->primitives().headMix(group, prim, sec,
						      myCache);
			}

    template<typename DETAIL_TYPE, typename PRIM_TYPE, typename SEC_PRIM_TYPE>
    SYS_DEPRECATED_HDK(13.0)
	inline int	nextMix(DETAIL_TYPE *gdp,
				const GA_PrimitiveGroup &group,
				PRIM_TYPE *&prim, SEC_PRIM_TYPE *&sec,
				PRIM_TYPE *&next_prim, SEC_PRIM_TYPE *&next_sec)
	{
	    return gdp->primitives().nextMix(group, prim, sec,
					     next_prim, next_sec, myCache);
	}
private:
    GEO_PrimList::GroupIterationCache	 myCache;
};

#define FOR_ALL_MIX_GROUP_PRIMITIVES(gdp, grp, prim, sec) \
	    for (GEO_GBMixGroupFwdIterator __iter(gdp, *grp, prim, sec); prim; \
		 __iter.nextMix(gdp, *grp, prim, sec, prim, sec))

#define FOR_SAFE_MIX_GROUP_PRIMITIVES(gdp, grp, prim, sec, nextprim, nextsec) \
	    for (GEO_GBMixGroupFwdIterator __iter(gdp, *grp, prim, sec); \
		 prim && (__iter.nextMix(gdp,*grp,prim,sec,nextprim,nextsec) || true) && prim; \
		 prim = nextprim, sec = nextsec)

SYS_DEPRECATED_POP_DISABLE()

#endif

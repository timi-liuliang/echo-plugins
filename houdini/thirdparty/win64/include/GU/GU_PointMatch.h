/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_PointMatch.h ( GU Library, C++)
 *
 * COMMENTS:
 *	Provides a structure to match offsets between two details.
 */

#ifndef __GU_PointMatch__
#define __GU_PointMatch__

#include "GU_API.h"
#include <GA/GA_Types.h>
#include <GA/GA_Handle.h>
#include "GU_Detail.h"

class GU_API GU_PointMatch
{
public:
    /// Initialize a lookup from src to dst on the given dictionary.
    /// The idname attribute will be matched if it is a string or integer,
    /// if not found in both it will match by raw index.
    /// dst must remain unchanged as we stash LUT pointers built by it.
    GU_PointMatch(GA_AttributeOwner dict, const GU_Detail *src, const GU_Detail *dst, const char *idname);

    /// Given offset in src, return correspnding offset in dst, or
    /// GA_INVALID_OFFSET if none exists
    GA_Offset	matchOffset(GA_Offset srcoff) const;
    /// Given index in src, return correspnding index in dst, or
    /// GA_INVALID_INDEX if none exists
    GA_Index	matchIndex(GA_Index srcindex) const;

protected:
    GA_ROHandleI	 mySrcId, myDstId;
    GA_ROHandleS	 mySrcName, myDstName;

    GA_AttributeOwner	 myDict;
    GA_Size		 myNumDstIndices;
    const GU_Detail	*mySrcGdp, *myDstGdp;

    // These are cached references into dst.
    const GU_Detail::AttribSingleValueLookupTable	*myLutId;
    const GU_Detail::AttribSingleValueLookupTable	*myLutName;
};

#endif


/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_AttributeSwap.h ( GU Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GU_AttributeSwap__
#define __GU_AttributeSwap__

#include "GU_API.h"
#include <GA/GA_Types.h>

class GU_Detail;
class UT_StringRef;


class GU_API GU_AttributeSwap
{
public:
    /// Operation to perform.
    /// Move will remove the source.  Note the point attribute P cannot
    /// be moved so it will be instead silently copied.
    /// Swap exchanges the pair.  If destination is missing, it is copied.
    enum SwapMethod
    {
	METHOD_SWAP,
	METHOD_COPY,
	METHOD_MOVE,
	NUM_METHODS
    };

    /// What to do about typeinfos.  When stashing attributes one often
    /// wants to clear the transform type so succeeding deformers don't
    /// also change your stashed values, this becomes the preserve-dest.
    /// OTOH, when duplicating, you want the copy to have the source typeinfo.
    /// This is ignored for swapping.
    enum TypeInfo
    {
	TYPEINFO_USE_SOURCE,
	TYPEINFO_PRESERVE_DEST,
	NUM_TYPEINFOS
    };

    /// Does the method to the two named attributes.  If swapping a missing
    /// attribute, will first create it.
    /// source attribute is assumed to exist.
    /// Returns false if failed for some reason; note there are special
    /// rules about how P can change.
    /// You must invoke gdp->refreshCachedAttributes() if there is a risk
    /// you changed the P attribute!
    static bool	swapAttribute(GU_Detail *gdp, SwapMethod method, TypeInfo typeinfo, GA_AttributeOwner owner, const UT_StringRef &srcname, const UT_StringRef &dstname);
};

#endif


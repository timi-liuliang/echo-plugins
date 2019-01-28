/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_AttributeRefMapOffsetMap.h ( GA Library, C++)
 *
 * COMMENTS:	A mapping from a particular element to the corresponding
 *              elements in other index maps, updated on the fly.
 */

#ifndef __GA_AttributeRefMapOffsetMap__
#define __GA_AttributeRefMapOffsetMap__

#include "GA_API.h"
#include "GA_Attribute.h"
#include "GA_Detail.h"
#include "GA_Types.h"

#include <SYS/SYS_Types.h>


class GA_API GA_AttributeRefMapOffsetMap
{
private:
    GA_Offset		myOffsets[GA_ATTRIB_OWNER_N];
    GA_AttributeOwner	myOwner;

#if !defined(GA_STRICT_TYPES)
    static const GA_Offset	theNeedLookup = (GA_INVALID_OFFSET-1);
#else
    static const GA_Offset	theNeedLookup;
#endif

public:
    GA_AttributeRefMapOffsetMap(GA_AttributeOwner owner, GA_Offset off)
	: myOwner(owner)
    {
	for (int i = 0; i < GA_ATTRIB_OWNER_N; ++i)
	    myOffsets[i] = theNeedLookup;
	myOffsets[owner] = off;
	myOffsets[GA_ATTRIB_DETAIL] = GA_Offset(0);
    }

    SYS_FORCE_INLINE void
	reset(GA_AttributeOwner owner, GA_Offset off)
	{
	    for (int i = 0; i < GA_ATTRIB_OWNER_N; ++i)
		myOffsets[i] = theNeedLookup;
	    myOffsets[owner] = off;
	    myOffsets[GA_ATTRIB_DETAIL] = GA_Offset(0);
	    myOwner = owner;
	}

    SYS_FORCE_INLINE GA_Offset
	getOwnerOffset() const { return myOffsets[myOwner]; }

    SYS_FORCE_INLINE GA_Offset 
	getOffset(const GA_Attribute *attrib)
    {
	GA_AttributeOwner aowner = attrib->getOwner();
	if (myOffsets[aowner] == theNeedLookup)
	{
	    if (aowner == GA_ATTRIB_GLOBAL)
		return myOffsets[aowner] = GA_Offset(0);

	    if (myOwner == GA_ATTRIB_VERTEX)
	    {
		const GA_Detail	&gdp = attrib->getDetail();
		if (aowner == GA_ATTRIB_POINT)
		    return myOffsets[aowner] = gdp.vertexPoint(
							    myOffsets[myOwner]);
		if (aowner == GA_ATTRIB_PRIMITIVE)
		    return myOffsets[aowner] = gdp.vertexPrimitive(
							    myOffsets[myOwner]);
	    }

	    return myOffsets[aowner] = GA_INVALID_OFFSET;
	}
    
	return myOffsets[aowner];
    }
};

#endif // __GA_AttributeRefMapOffsetMap__

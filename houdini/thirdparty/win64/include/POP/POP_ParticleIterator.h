/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __POP_ParticleIterator_h__
#define __POP_ParticleIterator_h__

#include "POP_API.h"
#include <GEO/GEO_PrimPart.h>

class POP_API POP_ParticleIterator
{
public:
    POP_ParticleIterator(GEO_PrimParticle *part)
    {
	myPart = part;
	myGdp = myPart->getParent();
	myNumPt = myPart->getNumParticles();
	myIdx = GA_Index(0);
	if (!atEnd())
	    myPtOff = myPart->vertexPoint(myIdx);
    }

    bool		atEnd() const
    {
	return myIdx >= myNumPt;
    }
    void		advance()
    {
	myIdx++;
	if (!atEnd())
	{
	    myPtOff = myPart->vertexPoint(myIdx);
	}
    }
    POP_ParticleIterator	&operator++()	 { advance(); return *this; }
    // No post increment as it is dangerous.

    bool		blockAdvance(GA_Offset &start, GA_Offset &end)
    {
	if (atEnd())
	    return false;
	start = myPtOff;
	end = start+1;
	while (1)
	{
	    advance();
	    if (atEnd())
		return true;
	    if (myPtOff == end)
	    {
		// still contiguous, make sure pages match.
		if ((end >> GA_PAGE_BITS) == (start >> GA_PAGE_BITS))
		{
		    end++;
		    continue;
		}
	    }
	    // Not the same, so return where we are.
	    // The current myPtOff will be picked up as the start
	    // of the next block
	    return true;
	}
    }

    GA_Offset		getOffset() const { return myPtOff; }
    GA_Offset		operator*() const { return myPtOff; }

protected:
    GEO_Detail				*myGdp;
    GEO_PrimParticle			*myPart;
    GA_Index				 myIdx;
    GA_Size				 myNumPt;
    GA_Offset				 myPtOff;
};


#endif

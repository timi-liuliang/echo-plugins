/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __POP_ParticleList_h__
#define __POP_ParticleList_h__

#include "POP_API.h"
class GEO_PrimParticle;
class POP_Node;
class POP_ParticleList;
class GEO_Detail;

// The POP_ParticleList is just a linked list of particle primitives used
// to keep track of which primitives each POP is supposed to process.

class POP_API POP_ParticleNode
{
public:
    POP_ParticleNode(GEO_PrimParticle *part, POP_Node *pop)
	: myPart(part)
	, myPop(pop)
	, myNext(NULL)
    {}
    ~POP_ParticleNode() {}

    bool	operator==(const POP_ParticleNode &src) const
		{
		    return myPart == src.myPart
			&& myPop == src.myPop;
		}
    bool	operator!=(const POP_ParticleNode &src) const
		    { return !(*this == src); }

    GEO_PrimParticle	*myPart;
    POP_Node		*myPop;
    POP_ParticleNode	*myNext;
};

class POP_API POP_ParticleList 
{
public:
    POP_ParticleList (void)
	: myHead(NULL)
	, myCurr(NULL)
    {
    }
    POP_ParticleList (const POP_ParticleList &src)
	: myHead(NULL)
	, myCurr(NULL)
    {
	POP_ParticleNode	*curr;
	for (curr = src.myHead; curr; curr = curr->myNext)
	    add(curr->myPart, curr->myPop);
    }
    ~POP_ParticleList (void)
    {
	clear();
    }

    POP_ParticleList	&operator=(const POP_ParticleList &src)
			{
			    if (&src != this)
			    {
				POP_ParticleNode	*curr;
				clear();
				for (curr = src.myHead; curr; curr=curr->myNext)
				    add(curr->myPart, curr->myPop);
			    }
			    return *this;
			}
    bool		operator==(const POP_ParticleList &src) const
			{
			    if (this == &src)
				return true;
			    const POP_ParticleNode	*p0, *p1;
			    for (p0 = myHead, p1 = src.myHead;
				    p0 && p1;
				    p0 = p0->myNext, p1 = p1->myNext)
			    {
				if (*p0 != *p1)
				    return false;
			    }
			    return true;
			}

    void		 clear(void);
    POP_ParticleNode	*add(GEO_PrimParticle* part,
				 POP_Node* pop = NULL);
    void		merge (const POP_ParticleList* list);
    void		copyNewGdp (const POP_ParticleList& list,
						 const GEO_Detail *gdp);
    GEO_PrimParticle	*iterateInit(void);
    GEO_PrimParticle	*iterateNext(void);
    POP_ParticleNode	*find(GEO_PrimParticle* part) const;
    bool		 isEmpty() const	{ return !myHead; }

protected:
    POP_ParticleNode	*myHead;
    POP_ParticleNode	*myCurr;
};
#endif

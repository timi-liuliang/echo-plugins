/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_ContextSpecificObject.h ( RE Library, C++)
 *
 * COMMENTS:
 *	This base class is for OpenGL objects that are context-specific,
 *	rather than shared. Example objects are:
 *
 *	- Vertex Array Objects
 *	- Occlusion Query Objects
 *	- Timer Objects
 *
 *	These objects are defined to be non-sharable by the OpenGL spec, even
 *	if two contexts belong to the same share-set.
 */
#ifndef RE_ContextSpecificObject_h
#define RE_ContextSpecificObject_h

#include <UT/UT_Assert.h>
#include "RE_Render.h"

class RE_API RE_ContextSpecificObject
{
public:
	     RE_ContextSpecificObject() : myContext(0) {}
    virtual ~RE_ContextSpecificObject() {}

    virtual bool		   init(RE_Render *r) = 0;

    void	initContext(RE_Render *r);
    bool	matchesContext(RE_Render *r) const;
    void	clearContext() { myContext = 0; }

protected:
    RE_Render	*getMyRender();

    // Call in subclasses to ensure that the incoming render context matches
    // the context this object was created with.
    bool	 initOrCheckRender(RE_Render *r)
    {
	UT_ASSERT(r);
	if(myContext == 0)
	{
	    myContext = r->getContext();
	    return true;
	}
	else if(myContext == r->getContext())
	    return true;

	UT_ASSERT(!"Mismatched GL context with context-specific object");
	return false;
    }
    
private:
    RE_OGLContext	myContext;
};

// To be used within derived classes.
#define RE_CONTEXT_CHECK(r) UT_ASSERT(matchesContext(r))

#endif

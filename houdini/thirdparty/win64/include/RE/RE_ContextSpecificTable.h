/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_ContextSpecificTable.h ( RE Library, C++)
 *
 * COMMENTS:
 *	Contains context-specific objects, creating them as needed for new
 *	GL contexts.
 */
#ifndef RE_ContextSpecificTable_h
#define RE_ContextSpecificTable_h

#include "RE_API.h"
#include <UT/UT_ValArray.h>

#include "RE_ContextSpecificObject.h"
#include "RE_Render.h"
#include "RE_Server.h"

template <class CObj>
class RE_ContextSpecificTable
{
public:
    RE_ContextSpecificTable() {} 
    ~RE_ContextSpecificTable()
	{
	    for(int i=0; i<myObjects.entries(); i++)
		if(myObjects(i))
		{
		    // avoid an attempt to clear the GL object, since deleting
		    // the table implies we're exiting and contexts are not
		    // likely to be valid. Use clear() if you need to free
		    // the GL objects themselves.
		    if(RE_Server::isAppExiting())
			myObjects(i)->clearContext();
		    delete myObjects(i);
		}
	}

    CObj *get(RE_Render *r)
	{
	    int id = r->getContextID();
	    RE_ContextSpecificObject *obj = myObjects.forcedRef(id);
	    if(!obj)
	    {
		obj = myObjects(id) = new CObj;
		obj->init(r);
	    }

	    return static_cast<CObj *>(obj);
	}
    CObj *peek(RE_Render *r)
	{
	    int id = r->getContextID();
	    if(id < myObjects.entries())
	       return static_cast<CObj *>(myObjects(id));
	    return NULL;
	}
    
    void  free(RE_Render *r)
	{
	    int	id = r->getContextID();

	    if(id >= myObjects.entries())
		return;
    
	    delete myObjects(id);
	    myObjects(id) = NULL;
	}

    void  clear()
	{
	    for(int i=0; i<myObjects.entries(); i++)
	    {
		delete myObjects(i);
		myObjects(i) = NULL;
	    }
	    myObjects.entries(0);
	}

    /// Returns the amount of main memory (NOT graphics memory!)
    /// owned by this RE_ContextSpecificTable.
    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        mem += myObjects.getMemoryUsage(false);
        return mem;
    }

private:
    UT_ValArray<RE_ContextSpecificObject *> myObjects;

    friend class RE_OGLRender;
};

#endif

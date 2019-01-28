/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_OGLVertexArrayList.h ( RE Library, C++)
 *
 * COMMENTS:
 *	Implements an OpenGL Vertex Array Object, which is a collection of
 *	vertex buffer bindings to various attribute types (vertex, color,
 *	normal...)
 *
 *	Note that this object is not shared with other shared contexts. It only
 *	exists in the OpenGL context that created it.
 */
#ifndef RE_OGLVertexArrayList_h
#define RE_OGLVertexArrayList_h

#include "RE_ContextSpecificObject.h"
#include "RE_ContextSpecificTable.h"

class RE_API RE_OGLVertexArrayList : public RE_ContextSpecificObject
{
public:
	     RE_OGLVertexArrayList() : myID(0), myCurrentElementBufferID(-1) {}
    virtual ~RE_OGLVertexArrayList();

    static void		createContextDefault(RE_Render *r);
    static void		bindContextDefault(RE_Render *r);
    static void		deleteContextDefault(RE_Render *r);

    virtual bool	init(RE_Render *r);
    bool		bind(RE_Render *r);
    static void		unbind(RE_Render *r);

    bool		setElementBuffer(int id)
			    {
				if(myCurrentElementBufferID != id)
				{
				    myCurrentElementBufferID = id;
				    return true;
				}
				return false;
			    }

    unsigned int	getID() const { return myID; }
private:
    unsigned int myID;
    unsigned int myCurrentElementBufferID;
};

typedef RE_ContextSpecificTable<RE_OGLVertexArrayList>
	RE_SharedVertexArray;

#endif

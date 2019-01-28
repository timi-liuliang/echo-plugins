/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	RE_PerContextData.h ( RE Library, C++)
 *
 * COMMENTS:
 *	Data that is kept with a RE_OGLContext, rather than RE_Render.
 *	RE_Renders are continually created and destroyed, while RE_OGLContexts
 *	are not.
 */
#ifndef RE_PerContextData_h
#define RE_PerContextData_h

#include <UT/UT_Array.h>
#include <UT/UT_Pair.h>

#include "RE_API.h"
#include "RE_Font.h"

class RE_BufferGroup;
class RE_Render;
class RE_OGLVertexArrayList;

class RE_API RE_PerContextData
{
public:
    static RE_PerContextData *getDataForContext(RE_OGLContext context);
    static void		      clearContextData(RE_OGLContext context);

    // bucketed buffer groups for font rendering.
    RE_FontBuffers *fetchFontBufferFromPool(RE_Render *r, int size);
    void	    returnFontBufferToPool(RE_FontBuffers *buf);

    // buffer groups for immediate mode emuluation.
    RE_BufferGroup *getBufferGroup(RE_Render *r, int size);

    void	    initBuffers(RE_Render *r);

private:
    void	    initBufferGroupRing();
    RE_BufferGroup *getNextBufferGroup(RE_Render *r);
    RE_BufferGroup *getLargeBufferGroup(RE_Render *r, int size);

    RE_BufferGroup *myBufferGroups;
    RE_BufferGroup *myCurrentBufferGroup;
    RE_BufferGroup *myLargeBufferGroups;
    int		    myLargeBufferSerial;

    bool	    myInitialized;
    
    UT_Array< UT_Pair<RE_FontBuffers*,RE_FontBuffers*> > myFontBufferBuckets;
    
     RE_PerContextData();
    ~RE_PerContextData();
};


class RE_BufferGroup
{
public:
    RE_BufferGroup()
	: myElemSize(0),
	  mySerial(0),
	  myP(NULL),
	  myC(NULL),
	  myImgUV(NULL),
	  myPatUV(NULL),
	  myPSize(NULL),
	  myElemList(NULL),
	  myVAO(NULL),
	  myNext(NULL)		{}

    ~RE_BufferGroup();

    void init(RE_Render *r, const int size);

    RE_VertexArray	*myP;
    RE_VertexArray	*myC;
    RE_VertexArray	*myPatUV;
    RE_VertexArray	*myImgUV;
    RE_VertexArray	*myPSize;
    
    RE_VertexArray	*myElemList;
    RE_OGLVertexArrayList *myVAO;

    int			 myElemSize;
    int			 mySerial;
   
    RE_BufferGroup	*myNext;
};

#endif

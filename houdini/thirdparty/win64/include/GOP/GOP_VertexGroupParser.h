/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:  This class contains a parser for the vertex groups.
 *	      The doIt() method returns the number of vertices parsed,
 *	      or -1 if an error has occured.
 *
 */

#ifndef __GOP_VertexGroupParser_h__
#define __GOP_VertexGroupParser_h__

#include "GOP_API.h"
#include "GOP_Parser.h"
#include <GA/GA_Types.h>

class GEO_Detail;
class GOP_Manager;
class GA_VertexGroup;

class GOP_API GOP_VertexGroupParser : public GOP_Parser
{
public:
    GOP_VertexGroupParser(GOP_Manager *manager, const GEO_Detail &gdp,
	                  const UT_String &str, bool strict = true,
			  GA_Index prim_offset = GA_Index(0));

    // Build a string that contains the id's of the breakpoints in a breakpoint
    // group.  The offset is the amount to add to the ids of the primitives for
    // each breakpoint in the group.
    static void		 buildGroupToken(const GA_VertexGroup *vtxgrp,
					 UT_String &token, GA_Index offset = GA_Index(0));
    // FIXME: Delete this once we split for 15.0.
    static void		 buildGroupToken(const GA_VertexGroup *vtxgrp,
					 UT_String &token, int offset);

private:

    virtual void	init();
    virtual void	finish();
    virtual bool	dispatch(char c);
    
    // State Handlers
    virtual void	handleStart();
    virtual void	handleError();

    void		handleCollectPrim();
    void		handleCollectPrim1();
    void		handleCollectName();
    void		handleCollectVertex0();
    void		handleCollectVertex1();

    // State Variables	
    GA_Index		myPrimIndex;
    GA_Index		myPrim1;
    int			myVertex0;
    int			myVertex1;

    // Vertex Specific Methods
    bool		addVertex();
    bool		addAllPrimVertices(GA_Offset primoff = GA_INVALID_OFFSET);
    bool		addVertexRange(int u0, int u1);
    bool		addPrimGroup(const char *name);
    bool		addVertexGroup(const char *name);

    // Primitive Specific Methods
    bool		addPrimRange(int u0, int u1);

    // Utility methods
    GA_Offset           getPrim();

    
    const GEO_Detail	&myGdp;
    GA_VertexGroup	*myGroup;
    GOP_Manager		*myManager;
    bool		 myStrictFlag;
    GA_Index		 myPrimOffset;
};


#endif

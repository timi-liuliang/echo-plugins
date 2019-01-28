/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:  This class contains a parser for the breakpoint groups.
 *	      The doIt() method returns the number of breakpoints parsed,
 *	      or -1 if an error has occured.
 *
 */

#ifndef __GOP_BreakpointGroupParser_h__
#define __GOP_BreakpointGroupParser_h__

#include "GOP_API.h"
#include "GOP_Parser.h"
#include <GA/GA_Types.h>

class GEO_Detail;
class GA_BreakpointGroup;
class GOP_Manager;
class GA_Primitive;
class GEO_Primitive;

class GOP_API GOP_BreakpointGroupParser : public GOP_Parser
{
public:
    GOP_BreakpointGroupParser(GOP_Manager *manager, const GEO_Detail &gdp,
			      const UT_String &str,
			      bool strict = true, GA_Index prim_offset = GA_Index(0));

    // Build a string that contains the id's of the breakpoints in a breakpoint
    // group.  The offset is the amount to add to the ids of the primitives for
    // each breakpoint in the group.
    static void		 buildGroupToken(const GEO_Detail *gdp,
					 const GA_BreakpointGroup *bkptgrp,
					 UT_String &token, GA_Index offset = GA_Index(0));
    // FIXME: Delete this once we split for 15.0.
    static void		 buildGroupToken(const GEO_Detail *gdp,
					 const GA_BreakpointGroup *bkptgrp,
					 UT_String &token, int offset);

private:

    virtual void	init();
    virtual void	finish();
    virtual bool	dispatch(char c);
    
    // State Handlers
    virtual void	handleStart();
    virtual void	handleError();

    void		handleCollectPrim();
    void		handleCollectSec();
    void		handleCollectName();
    void		handleCollectBreakpointU0();
    void		handleCollectBreakpointU1();
    void		handleCollectBreakpointV0();
    void		handleCollectBreakpointV1();

    // State Variables	
    bool		myOpenBracket;
    GA_Index		myPrimIndex;
    int			mySecIndex;
    int			myU0;
    int			myU1;
    int			myV0;
    int			myV1;

    // Breakpoint Specific Methods
    bool		addCurveBreakpoint();
    bool		addSurfBreakpoint();
    bool		addSecBreakpoint();
    bool		addAllPrimBreakpoints(const GEO_Primitive *prim = 0);
    bool		addAllSecBreakpoints();
    bool		addAllSecBreakpoints(const GA_Primitive *prim);
    bool		addCurveBreakpointRange(int u0, int u1);
    bool		addSecBreakpointRange(int u0, int u1);
    bool		addSurfBreakpointRange(int u0, int u1, int v0, int v1);
    bool		addPrimGroup(const char *name);

    // Utility methods
    const GEO_Primitive      *getPrim();

    const GEO_Detail   	     &myGdp;	
    GA_BreakpointGroup       *myGroup;
    GOP_Manager	       	     *myManager;	
    bool		      myStrictFlag;
    GA_Index		      myPrimOffset;
};


#endif

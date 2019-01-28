/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:  This class contains a parser for the edge groups.
 *	      The doIt() method returns the number of edges parsed,
 *	      or -1 if an error has occured.
 *
 */

#ifndef __GOP_EdgeGroupParser_h__
#define __GOP_EdgeGroupParser_h__

#include "GOP_API.h"
#include "GOP_Parser.h"

#include <GA/GA_PrimitiveTypeId.h>
#include <GA/GA_Types.h>

///
/// Specifies edges 3-6 and 6-9
/// p3-6-9
///
/// Specifies edges whose both points lie in foo, foo a point
/// group exression.
/// p{foo}
/// p{3-5:4}
/// p{bar* @id>5}

class UT_String;
class GA_Primitive;
class GEO_Detail;
class GOP_Manager;
class GEO_Primitive;
class GA_EdgeGroup;

class GOP_API GOP_EdgeGroupParser : public GOP_Parser
{
public:
    GOP_EdgeGroupParser(GOP_Manager *manager, const GEO_Detail &gdp,
	                const UT_String &str,
			const GA_PrimitiveTypeId &id,
			bool strict = true,
			GA_Index prim_offset = GA_Index(0),
                        GA_Index point_offset = GA_Index(0));

    // Build a string that contains the id's of the edges in an edge group.
    // The prim and point offsets are the amount to add to the ids of the
    // primitives/points for each edge in the group.
    static void		buildGroupToken(const GEO_Detail *gdp,
					const GA_EdgeGroup *edgegrp,
					UT_String &token,
					GA_Index prim_offset = GA_Index(0),
                                        GA_Index pt_offset = GA_Index(0));
    // FIXME: Delete this once we split for 15.0.
    static void		buildGroupToken(const GEO_Detail *gdp,
					const GA_EdgeGroup *edgegrp,
					UT_String &token,
					int prim_offset, int pt_offset);

    // This is only made public so that code from within the parser
    // can access it.
    bool 		addEdge(GA_Offset pt0, GA_Offset pt1,
				const GEO_Primitive *pprim);
private:

    virtual void	init();
    virtual void	finish();
    virtual bool	dispatch(char c);
    
    // State Handlers
    virtual void	handleStart();
    virtual void	handleError();

    void		handleCollectPoint();
    void		handleCollectPrim();
    void		handleCollectSec();
    void		handleCollectName();
    void		handleCollectEdgeU();
    void		handleCollectEdgeV();
    void		handleCollectPointGroup();
    void		handleCollectAll();

    void		handleInverseGroup();

    bool		matchTypeId(const GA_Primitive *prim) const;

    // State Variables	
    bool	 	myOpenBracket;
    bool	 	myAllSpaces;
    bool	 	myRangeStarted;
    bool		myIsNegating;
    bool		myInverse;
    int			myLastPoint;
    int			myIndexStart;
    int			myPrimIndex;
    int			mySecIndex;
    int			myU;
    GA_PrimitiveTypeId	myId;

    // Edge Specific Methods
    const GEO_Primitive *getPrimitive(GA_Index prim) const;    

    bool		addEdge(GA_Index p0, GA_Index p1, GA_Index prim);
    bool		addPoint(GA_Index p0);
    bool		addAllPrimEdges(GA_Index prim);
    bool		addAllPrimEdges(const GEO_Primitive &prim);
    bool		addPrimEdge(GA_Size vertex_first, GA_Index prim);
    bool		addPrimEdgeRange(GA_Size vertex_first, 
					 GA_Size vertex_last, 
					 GA_Index prim);
    bool		addPrimRange(GA_Index prim_first, GA_Index prim_last);
    bool		addHullEdge(unsigned int row, unsigned int col, 
				    GA_Index prim);
    bool		addPrimGroup(const char *name);
    
    const GEO_Detail    &myGdp;
    GA_EdgeGroup       *myEdgeGroup;	    // Group to hold all lone edges
    GA_EdgeGroup       *myInverseEdgeGroup; // Temporary group for inverse edges
    GOP_Manager	       *myManager;	
    bool		myStrictFlag;
    GA_Index		myPrimOffset;
    GA_Index		myPointOffset;
};


#endif

/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Topology.h ( GU Library, C++)
 *
 * COMMENTS:	Topology checker - builds a structure with which you can
 *		determine different degrees of topology changes (this is
 *		done when you set up the checker)
 */

#ifndef __GU_Topology__
#define __GU_Topology__

#include "GU_API.h"

#include <UT/UT_ValArray.h>
#include <GA/GA_Types.h>

class GU_Detail;
class gu_TopologyAttrib;
class gu_TopologyGroup;

class GU_API GU_TopologyFlags
{
public:
    GU_TopologyFlags() {
	myPrimCount = 1;		// We're interested in # primitives
	myPointCount = 1;		// Interested in # points
	myVertexCount = 0;		// Not in # vertices per prim
	myGroups = 1;			// Interested if groups change
	myAttributes = 1;		// Interested in attribute changes
    };

    unsigned	myPrimCount:1,
		myPointCount:1,
		myVertexCount:1,
		myGroups:1,
		myAttributes:1;
};

/// WARNING: The behaviour of this class is highly suspect!
///          Please, do not use it in new code!
class GU_API GU_Topology
{
public:
     GU_Topology();
    ~GU_Topology();

    void		setFlags(const GU_TopologyFlags &flags);
    void		initialize(const GU_Detail *gdp);
    int			isDifferent(const GU_Detail *gdp) const;

private:
    void		 clearAndDestroy();
    void		 clearAttributes();
    void		 clearGroups();

    GU_TopologyFlags			myFlags;
    GA_Size				myPrims;
    GA_Size				myPoints;
    GA_Size				myVertices;
    UT_ValArray<gu_TopologyAttrib *>	myPtAttribs;
    UT_ValArray<gu_TopologyAttrib *>	myPrimAttribs;
    UT_ValArray<gu_TopologyAttrib *>	myVtxAttribs;
    UT_ValArray<gu_TopologyAttrib *>	myDetailAttribs;
    UT_ValArray<gu_TopologyGroup *>	myPtGroups;
    UT_ValArray<gu_TopologyGroup *>	myPrimGroups;
    UT_ValArray<gu_TopologyGroup *>	myVtxGroups;
};

#endif


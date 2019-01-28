/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Loft.C (C++)
 *
 * COMMENTS:
 *	Stitch polygons together without changing the number of points in the
 *	gdp.
 *
 */

#ifndef __GU_Loft_h__
#define __GU_Loft_h__

#include "GU_API.h"
class GA_PrimitiveGroup;

enum GU_LoftClosureType
{
    GU_LOFT_CLOSE_NONE	= 0,
    GU_LOFT_CLOSE	= 1,
    GU_LOFT_CLOSE_AUTO	= 2
};

enum GU_LoftMinimize
{
    GU_LOFT_2POINT_DIST	= 0,
    GU_LOFT_3POINT_DIST	= 1
};

class GU_API GU_LoftParms
{
public:
     GU_LoftParms();
    ~GU_LoftParms() {}

    GA_PrimitiveGroup	*polys;		// group of created polygons

    int			 keepPrims;	// keep primitives
    int			 proximity;	// find start point based on proximity
    int			 consolidate;	// consolidate neighbouring points
    float		 distance;	// consolidation distance
    GU_LoftMinimize	 goal;		// minimization goal
    GU_LoftClosureType	 wrapu;		// wrap each cross-section
    GU_LoftClosureType	 wrapv;		// wrap first to last skin
    float		 collinearity_tol;  // tolerance for collinearity check
					    // before building a triangle
};

#endif

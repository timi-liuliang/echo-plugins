/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Join.C (C++)
 *
 * COMMENTS:
 *	This is the filter used to join faces or surfaces.
 */

#ifndef __GU_Join_h__
#define __GU_Join_h__

#include "GU_API.h"
#include <GEO/GEO_Primitive.h>

class GA_PrimitiveGroup;

class GU_API GU_JoinParms
{
public:
    GU_JoinParms(); 
   ~GU_JoinParms() {}


    int			 blend;
    float		 bias;

    float		 tolerance;
    int			 unrefine;
    int			 uJoin;

    int			 loop;
    int		 	 keepPrimitives;
    int			 checkProximity;

    int			 onlyAttached;

    const GA_PrimitiveGroup *prims;
};

#endif

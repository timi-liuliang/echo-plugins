/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	stitched polygons (C++)
 *
 * COMMENTS:
 */

#ifndef __GQ_Stitch_h__
#define __GQ_Stitch_h__

#include "GQ_API.h"
class GA_PointGroup;

class GQ_API GQ_StitchParms
{
public:
    GQ_StitchParms();
    ~GQ_StitchParms();

    const GA_PointGroup	*corners;
    float		 tol3d;
    int			 clamp;
    int			 consolidate;
    int			 findcorners;
    float		 angle;
};
    

#endif



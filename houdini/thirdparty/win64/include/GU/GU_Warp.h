/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Warp.C (C++)
 *
 * COMMENTS:
 *	This is the filter used to warp faces or surfaces.
 */

#ifndef __GU_Warp_h__
#define __GU_Warp_h__

#include "GU_API.h"
#include <UT/UT_Matrix4.h>
#include <GEO/GEO_Primitive.h>

class GA_PrimitiveGroup;

enum GU_WARP_OP
{
    GU_MATRIX_WARP	= 0,
    GU_NORMAL_WARP	= 1,
    GU_POSITION_WARP	= 2,
    GU_SELFUV_WARP	= 3
};

class GU_API GU_WarpParms
{
public:
    GU_WarpParms(); 
   ~GU_WarpParms() {}


    GU_WARP_OP		 op;

    int			 warpU;
    float		 u;	 // [0,1]
    int			 useBiasU;
    float		 biasU;
    float		 sharpnessU;

    int			 warpV;
    float		 v;	 // [0,1]
    int			 useBiasV;
    float		 biasV;
    float		 sharpnessV;

    // xform, normalDist, pos, and selfu/v are exclusive
    UT_Matrix4		 xform;
    float		 normalDist;
    float		 selfu, selfv; // must be unit coords ([0,1])
    UT_Vector4		 pos;

    const GA_PrimitiveGroup	*prims;
};

#endif

/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Utility Library (C++)
 *
 * COMMENTS:
 *		Creates torus' from various primitives
 */

#ifndef __GU_TORUS_H__
#define __GU_TORUS_H__

#include "GU_API.h"
#include "GU_Detail.h"

class GU_API GU_TorusParms
{
public:
    GU_TorusParms();

    int   		rows, cols; 
    float 		outsideRadius; 
    float 		insideRadius; 
    float 		xc, yc, zc;
    GU_AxisType 	axis;
    GEO_SurfaceType	type; 
    float 		begin_uangle, end_uangle;
    float 		begin_vangle, end_vangle;
    int 		wrapu, wrapv;
    int			orderu, orderv;
    int			imperfect;
    int			capu, capv;
};

#endif

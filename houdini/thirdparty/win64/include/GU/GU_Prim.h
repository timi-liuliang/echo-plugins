/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Prim.h (GU Library, C++)
 *
 * COMMENTS:	This contains functions that are common to all primitive types.
 */

#ifndef __GU_Prim_h__
#define __GU_Prim_h__

#include "GU_API.h"
#include <UT/UT_Vector3.h>

class GEO_Primitive;

/// Find the minimum distance between a primitive and a point
GU_API bool GUminimumDistPrimPoint(
    const GEO_Primitive &prim,
    const UT_Vector3 &point,
    float distmax = 1E17F, float *distance = 0,
    float *u = 0, float *v = 0,
    bool accurate = true);

/// Find the minimum distance between two primitives.
/// (u1,v1) is place on prim1, (u2,v2) is place on prim2.
/// NOTE: This treats polygon primitives as just their edges,
///       not filled polygons.
GU_API bool GUminimumDistPrimPrim(
    const GEO_Primitive &prim1,
    const GEO_Primitive &prim2,
    float distmax = 1E17F, float *distance = 0,
    float *u1 = 0, float *v1 = 0,
    float *u2 = 0, float *v2 = 0,
    bool accurate = true);

#endif

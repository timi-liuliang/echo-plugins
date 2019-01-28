/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * COMMENTS:
 */

#ifndef __SOP_Expression_h__
#define __SOP_Expression_h__

#include "SOP_API.h"

class GU_Detail;

// Given a GU_Detail, a prim_idx of -1, and a position, find the closest
// primitive to that position and return it, the corresponding (u, v)
// coordinate, and the distance to that primitive.  If prim_idx is not
// negative, returns the closest position to a particular primitive.
SOP_API void SOPfindNearestToPrim(const GU_Detail &gdp, int prim_idx,
    const UT_Vector3 &position, const GEO_Primitive *&prim,
    float &u, float &v, float &distance);

// Thread specific value that tracks if anything invoked the spare
// inputs on forbidden node caches.
SOP_API bool SOPswapUsesExprInputs(bool newvalue);

#endif

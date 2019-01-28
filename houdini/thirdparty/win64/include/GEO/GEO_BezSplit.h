/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 */


#ifndef __GEO_BezSplit_h__
#define __GEO_BezSplit_h__

#include "GEO_API.h"
#include <UT/UT_VectorTypes.h>

// Dir specifies which direction to split (u = 0, v = 1)
template <class T, int Dir>
class GEO_API GEO_BezSplit {
public:
    // Given a bezier hull in 'left', partition it into two new hulls that
    // represent the same curve.  Partition at parametric value 't'.
    static void	split(T *left, T *right, int uorder, int vorder, float t = 0.5);
};

#endif

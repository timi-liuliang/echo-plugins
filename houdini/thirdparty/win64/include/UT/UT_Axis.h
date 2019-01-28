/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Axis.h (C++)
 *
 * COMMENTS:    Axes for performing rotations
 *
 */

#ifndef __UT_Axis_h__
#define __UT_Axis_h__

#include "UT_API.h"
// Dummy class to avoid global namespace pollution
class UT_API UT_Axis3 {
public:
    // Enumerator for the 3 possible axes. Always use it with the scope
    // operator: UT_Axis3::axis, UT_Axis3::XAXIS, etc.
    // The axis values must be powers of 2.
    enum	axis { XAXIS=1, YAXIS=2, ZAXIS=4 };
};

#endif

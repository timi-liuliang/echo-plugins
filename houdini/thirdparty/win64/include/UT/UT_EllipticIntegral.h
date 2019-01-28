/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_EllipticIntegral.h ( UT Library, C++)
 *
 * COMMENTS:
 *		Performs elliptical integrals of various types.
 */

#ifndef __UT_EllipticIntegral__
#define __UT_EllipticIntegral__

#include "UT_API.h"
#include <SYS/SYS_Types.h>

// Find the perimeter of the ellipse of the specified axes.
UT_API fpreal
UTellipsePerimeter(fpreal a, fpreal b);

// Find the surface area of a sphereoid with axes a, b, and c.
// iter is the number of samples to take for the integration.
// As a closed form solution is currently used, it is unnecessary.
UT_API fpreal
UTspheroidArea(fpreal a, fpreal b, fpreal c, int iter=32);

#endif


/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	rounded fillets (C++)
 *
 * COMMENTS:
 */

#ifndef __GU_RoundFillet_h__
#define __GU_RoundFillet_h__

#include "GU_API.h"
class GEO_Face;

class GU_API GU_RoundFilletParms
{
public:
    GU_RoundFilletParms();
    ~GU_RoundFilletParms();

    // Intersection curve, and curves on surface 1 & 2 (in space coords)
    GEO_Face	*isectcurve, *curve1, *curve2;

    float	 density;	// Density of spans
    float	 tol3d;		// Tolerance to do curvesects with
    float	 tol2d;		// Tolerance to triming with.
    float	 angle;		// Angle tolerance
    float	 radius;
    int		 trimmed;	// Is it to be trimmed?
    int		 reva, revb;	// Reverse profile directions?
};
    

#endif


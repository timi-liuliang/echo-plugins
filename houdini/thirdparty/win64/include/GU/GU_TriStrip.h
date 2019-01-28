/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_TriStrip.h ( GU Library, C++)
 *
 * COMMENTS: Tri-Stripper
 */

#ifndef __GU_TriStrip__
#define __GU_TriStrip__

#include "GU_API.h"
#include <limits.h>

class GU_API GU_TriStripParms
{
    // used: constrainStrips, maxTriStripLength
public:
    int				makeStrips, constrainStrips;
    int				makeFans, constrainFans;
    int				maxTriStripLength, maxTriFanLength;
    const GA_PrimitiveGroup	*primGroup;

    GU_TriStripParms( ):
	makeStrips( 1 ),
    constrainStrips( 0 ),
    makeFans( 0 ),
    constrainFans( 0 ),
    maxTriStripLength( INT_MAX ),
    maxTriFanLength( INT_MAX ),
    primGroup( NULL )
    { }

    GU_TriStripParms(	int makeS, int consS, int mtsl, int makeF, int consF,
	    		int mtfl, const GA_PrimitiveGroup *pg ):
	makeStrips( makeS ),
    constrainStrips( consS ),
    makeFans( makeF ),
    constrainFans( consF ),
    maxTriStripLength( mtsl ),
    maxTriFanLength( mtfl ),
    primGroup( pg )
    { }

    ~GU_TriStripParms() { }
};


int GU_API GUtriStrip(GU_Detail *gdp, GU_TriStripParms &parms);

#endif


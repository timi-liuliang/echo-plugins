/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU_Twist.h	
 *
 * COMMENTS:
 *
 */

#ifndef __GU_TWIST__
#define __GU_TWIST__

#include "GU_API.h"
#include "GU_Types.h"
#include <UT/UT_Vector3.h>

class GU_Detail;
class GA_PointGroup;

class GU_API GU_TwistParms
{
public:						// Used by
    GU_TwistParms();
    const GU_Detail		*source;	// All
    GU_AxisType			 primaryAxis;	// All
    GU_AxisType			 secondaryAxis;	// Bend, Shear, Taper, LTaper
    UT_Vector3			 origin;	// All - Shear
    float			 value; 	// All
    float			 power;		// All - LinearTaper
    const GA_PointGroup	*grp;		// All
    const GA_PointGroup	*sgrp;		// All
    int				 clamping;	// Bend & Linear Taper
};

#endif

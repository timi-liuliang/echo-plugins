/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	align parameters (C++)
 *
 * COMMENTS:
 */

#ifndef __GU_AlignParms_h__
#define __GU_AlignParms_h__

#include "GU_API.h"
#include <UT/UT_Matrix4.h>

class GA_PrimitiveGroup;
class GA_PointGroup;
class GEO_Primitive;

class GU_API GU_AlignParms
{
public:

    GU_AlignParms();
     ~GU_AlignParms() {}

    float   bias;       // which prim to alter

    float   lu, lv;     // which point/segment to align
    float   ru, rv;
    float   ruend, rvend;

    int     individual; // align each separately else
                        // follow the first prim

    int     dotrans;    // align position
    int     dorotate;   // align orientation

    UT_Matrix4 postxform; // Transform prims after alignment

    const GEO_Primitive *auxprim;   // what to align to (optional)
				    // note this may belong to another
				    // detail object altogether but
				    // only positions and derivatives
				    // are calculated from it. ie.
				    // no attributes

    const GA_PrimitiveGroup *primGroup;

    // Cache of the points in the right and left prims.
    GA_PointGroup *myRPoints, *myLPoints;
};

#endif

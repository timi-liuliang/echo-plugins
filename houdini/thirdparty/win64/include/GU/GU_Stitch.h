/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	stitch parameters (C++)
 *
 * COMMENTS:
 */


#ifndef __GU_StitchParms_h__
#define __GU_StitchParms_h__

#include "GU_API.h"
#include <UT/UT_ValArray.h>
#include <UT/UT_VectorTypes.h>

class GA_PrimitiveGroup;
class GEO_Primitive;


class GU_API GU_StitchParms
{
public:

    GU_StitchParms();
    ~GU_StitchParms() {}

    int     stitchU;        // stitch in U or V

    float   tolerance;      // affect of stitching
    float   bias;           // which prim to alter

    float   ldepth;         // which point/segment to stitch
    float   rdepth;
    float   lwidth1;
    float   lwidth2;
    float   rwidth1;
    float   rwidth2;

    int     dotangent;      // stitch tangentially
    int     fixed;          // fixed or free intersection
    float   scale1;         // tangent scales
    float   scale2;

    int     dostitch;       // coincide endpoints

    int     sharp;          // refine or subdivide partial
                            // stitching/tangents

    int     loop;           // fillet last to first

    GEO_Primitive *auxprim; // stitch everyone to this prim

    const GA_PrimitiveGroup *primGroup;

    // stash each set of stitch prims
    // for guide display or other purposes.
    UT_ValArray<GA_PrimitiveGroup *>  *outputGroups;
};

#endif

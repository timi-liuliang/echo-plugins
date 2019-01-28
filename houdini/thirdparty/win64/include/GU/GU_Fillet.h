/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	fillet parameters (C++)
 *
 * COMMENTS:
 */


#ifndef __GU_FilletParms_h__
#define __GU_FilletParms_h__

#include "GU_API.h"
#include <GA/GA_PrimCompat.h>
class GA_PrimitiveGroup;
class GEO_Primitive;

enum GU_FILLET_OP
{
    GU_FREEFORM_FILLET	= 0,
    GU_CONVEX_FILLET	= 1,
    GU_CIRCULAR_FILLET	= 2
};

class GU_API GU_FilletParms
{
public:

    GU_FilletParms();
    ~GU_FilletParms() {}

    int     filletU; // fillet in U or V

    GU_FILLET_OP fillettype;

    GA_PrimCompat::TypeMask primtype; // primitive type of fillet
    int     order;

    float   ldepth;     // which point/segment to fillet
    float   rdepth;

    float   lwidth1;    // for partial fillets
    float   lwidth2;

    float   rwidth1;
    float   rwidth2;

    float   lscale;     // tangent scales
    float   rscale;
    float   loffset;
    float   roffset;

    int     seamless;   // modify hulls by merging bases,etc.
    int     cut;        // cut input source
    int     loop;       // fillet last to first

    GEO_Primitive *auxprim; // fillet all to this primitive

    const GA_PrimitiveGroup *primGroup;
};

#endif

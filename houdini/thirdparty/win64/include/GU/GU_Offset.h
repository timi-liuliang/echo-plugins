/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	rounded fillets (C++)
 *
 * COMMENTS:
 */

#ifndef __GU_Offset_h__
#define __GU_Offset_h__

#include "GU_API.h"
class GEO_TPSurf;

class GU_API GU_OffsetParms
{
public:
    GU_OffsetParms();
    ~GU_OffsetParms();

    GEO_TPSurf	*surf;
    float	 radius;
    // The domain area, relative to [0,1]x[0,1] to offset.
    float	 umin, umax, vmin, vmax;
    // The refinement to perform before spawning.
    int		 refinement;
};
    

#endif



/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        Curve Clay Parameter(C++)
 *
 * COMMENTS:
 */

#ifndef __GU_CurveClay_h__
#define __GU_CurveClay_h__

#include "GU_API.h"
#include <UT/UT_VectorTypes.h>
class GEO_Detail;
class GA_PrimitiveGroup;

enum GU_CurveClayRestType
{
    GU_MINIMUM = 0,
    GU_PROJECT = 1,
    GU_PROFILE = 2
};

enum GU_CurveClayDeformType
{
    GU_CURVE = 0,
    GU_DIRECTION = 1,
    GU_NORMAL = 2
};

class GU_API GU_CurveClayParms
{
public:
    GU_CurveClayParms(void);

    GU_CurveClayRestType	 restType;
    GU_CurveClayDeformType	 deformType;

    UT_Vector3		 	 restDir;
    int				 restDirFNorm;	// whether to use face normal
    UT_Vector3			 deformDir;

    int				 numdivs;
    float			 refinement;
    float			 sharpness;

    GEO_Detail			*restdp;
    GEO_Detail			*deformdp;

    const GA_PrimitiveGroup	*restfaces;
    const GA_PrimitiveGroup	*deformfaces;
    const GA_PrimitiveGroup	*surfaces;

    float			 deformLength;

    int				 deforminside;
    int				 individual;
};

#endif

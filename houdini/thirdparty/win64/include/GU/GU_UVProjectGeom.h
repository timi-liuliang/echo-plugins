/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU Library (C++)
 *
 * COMMENTS: Geometry (as displayed on-screen) for UV projections
 *
 */
#ifndef __GU_UVProjectGeom_h__
#define __GU_UVProjectGeom_h__

#include "GU_API.h"
class GU_Detail;
#include <UT/UT_VectorTypes.h>

class GU_API GU_UVProjectGeom
{
public:
    GU_UVProjectGeom();
    ~GU_UVProjectGeom();

    static void	 buildOrtho(GU_Detail &gdp);
    static void	 buildPolar(GU_Detail &gdp);
    static void	 buildCylindrical(GU_Detail &gdp);
    static void	 buildToroidal(GU_Detail &gdp, float innerrad);
    static void	 buildPlasticWrap(GU_Detail &gdp);
};

#endif

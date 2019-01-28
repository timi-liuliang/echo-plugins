/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU library (C++)
 *
 * COMMENTS:	Superquadric creation
 *
 */

#ifndef __GU_SuperQuad_h__
#define __GU_SuperQuad_h__

#include "GU_API.h"
#include <UT/UT_Matrix4.h>
#include <GEO/GEO_SurfaceType.h>

class GU_API GU_SuperQuadParms
{
public:
    GU_SuperQuadParms();

    UT_Matrix4		xform;		// Quadric transform
    int			rows, cols;	// For mesh
    float		xyexp, zexp;
    GEO_SurfaceType	type;
    unsigned		doPolys;	// Make polys instead of mesh
    int			uniquePoles;
};

#endif

/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU Library (C++)
 *
 * COMMENTS: Texture coordinate unwrapping code.
 *
 */
#ifndef __GU_TextureUnwrap_h__
#define __GU_TextureUnwrap_h__

#include "GU_API.h"
#include <GA/GA_Handle.h>
#include <UT/UT_Matrix3.h>
#include <UT/UT_StringHolder.h>
#include <UT/UT_Vector3Array.h>

class GA_PrimitiveGroup;
class GU_Detail;

class GU_API GU_TextureUnwrapParms
{
public:
    enum LayoutType
    {
	LAYOUT_STRIP = 0,
	LAYOUT_SQUARE = 1
    };

    enum ScaleType
    {
	SCALE_NONE = 0,
	SCALE_UNIFORM = 1,
	SCALE_STRETCH = 2
    };

    GU_TextureUnwrapParms(); 

    void initializeTetrahedron();
    void initialize5();
    void initializeCube();
    void initialize8();

    void initializeGeometry(const GU_Detail *gdp,
			    const GA_PrimitiveGroup *group);

    // Build polygon geometry representation of the planes being
    // projected onto.
    static void buildGeometry4(GU_Detail *gdp);
    static void buildGeometry5(GU_Detail *gdp);
    static void buildGeometry6(GU_Detail *gdp);
    static void buildGeometry8(GU_Detail *gdp);

    GA_RWHandleV3               myAttrib;

    const GA_PrimitiveGroup	*myGroup;

    UT_Vector3Array		myPlaneNormals;
    UT_Vector3Array		myPlaneXAxes;

    ScaleType			myScaleType;
    LayoutType			myLayoutType;

    float			mySpacing;
};


class GU_API GU_TextureUnwrap
{
public:

    GU_TextureUnwrap(GU_Detail *gdp);
    ~GU_TextureUnwrap();


    // Performs the extrusion
    void	unwrap(const GU_TextureUnwrapParms &parms);

private:
    GU_Detail		*myGdp;
};

#endif

/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	
 *
 * COMMENTS:
 *
 */

#ifndef __GU_Extrude_h__
#define __GU_Extrude_h__

#include "GU_API.h"
#include <GEO/GEO_SurfaceType.h>
class GU_Detail;


enum GU_ExtrudeFaceType
{
    GU_EXTRUDE_FACE_NONE	= 0,
    GU_EXTRUDE_FACE_OUTPUT	= 1,
    GU_EXTRUDE_FACE_CONVEX	= 2
};

class GU_API GU_ExtrudeParms 
{
public:

    enum GU_ExtrudeFusion	{
				  GU_NO_FUSION,
				  GU_CLAMP_ALL_POINTS,
				  GU_CLAMP_SINGLE_POINT,
				  GU_CLAMP_FACE
				};

    void	 crossSection(int vertex);
    GU_ExtrudeParms()			// some valid no-op defaults
    {
	sourceGroup = 0;
	xsection_single = 0;
	xsection = 0;
	xsectionGroup = 0;
	doInit = 0;
	doFuse = GU_NO_FUSION;
	shareFaces = 0;
	detectSharedEdges = 0;		// Don't detect shared edges by default
	keepOriginal = 0;
	frontOutput = GU_EXTRUDE_FACE_NONE;
	backOutput = GU_EXTRUDE_FACE_NONE;
	sideType = GEO_PATCH_QUADS;
	sideOutput = 0;
	vertex = 0;
	tx = 0;
	ty = 0;
	sx = 1;
	sy = 1;
	frontGroup = 0;
	backGroup = 0;
	sideGroup = 0;
	fullTransform = 0;
	translateDelta.assign(0,0,0);
	translateDist = 0;
	scalex = 1;
	scaley = 1;
	outputpoly = 0;
	holefaces = true;
    }
	
    const GA_PrimitiveGroup	*sourceGroup;		// prims to extrude

    const GEO_Face		*xsection_single;	// if non-zero just
							// use this primitive
    const GU_Detail		*xsection;		// else use the
    const GA_PrimitiveGroup	*xsectionGroup;		// gdp-group data

    int				 doInit; 		// initialize nicely
    GU_ExtrudeFusion		 doFuse;		// fuse points
    int				 keepOriginal;
    int				 shareFaces;		// share points
							// between faces and
							// hull.

    GU_ExtrudeFaceType		 frontOutput;
    GU_ExtrudeFaceType		 backOutput;
    GEO_SurfaceType		 sideType;
    int				 sideOutput;
    int				 vertex;
    float			 tx, ty;
    float			 sx, sy;


    GA_PrimitiveGroup		*frontGroup;		// optional
    GA_PrimitiveGroup		*backGroup;
    GA_PrimitiveGroup		*sideGroup;

    // post translation allows the inner row to be relocated arbitrarily.
    // this is used by the selection extrusion to handle shared points 
    // correctly.

    int				 fullTransform;		// 0 = no change
							// 1 = along vector
							// 2 = along normal
    UT_Vector3			 translateDelta;
    float			 translateDist;
    float			 scalex, scaley;

    bool			 holefaces;
    

    // used to specify whether shared edges should be detected
    // and the resulting extra walls removed.
    int				 detectSharedEdges;	// 0 = no checking
							// 1 = checking done


    // Used to see if we output to polygons for sidemeshes.
    int				 outputpoly;
};

#endif



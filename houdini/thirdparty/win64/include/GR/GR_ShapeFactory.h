/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 * 
 * NAME:        GR_ShapeFactory ( GR Library, C++)
 *
 * COMMENTS:
 *	Generates basic shapes in the form of RE_Geometry objects.
 *	Wireframe versions are connectivity group RE_GEO_WIRE_IDX.
 *	Shaded versions (if applicable) are in RE_GEO_SHADED_IDX.
 */
#ifndef GR_ShapeFactory_h
#define GR_ShapeFactory_h

#include "GR_API.h"
#include <UT/UT_VectorTypes.h>

class RE_Render;
class RE_Geometry;

namespace GR_ShapeFactory
{

enum Orientation
{
    ORIENT_XY,
    ORIENT_XZ,
    ORIENT_YZ
};

GR_API extern RE_Geometry *buildPoint(RE_Render *r,
				      UT_Vector3F pos,
				      const char *cache_name = NULL);
GR_API extern RE_Geometry *buildPointCloud(RE_Render *r,
					   const UT_Vector3FArray &pos,
					   const char *cache_name = NULL);
    
GR_API extern RE_Geometry *buildGrid(RE_Render *r,
				     UT_Vector3F center,
				     UT_Vector2F size,
				     Orientation orient,
				     const char *cache_name = NULL);

GR_API extern RE_Geometry *buildBox(RE_Render *r,
				    UT_Vector3F center,
				    UT_Vector3F size,
				    const char *cache_name = NULL);
    
GR_API extern RE_Geometry *buildLine(RE_Render *r,
				    UT_Vector3F p0,
				    UT_Vector3F p1,
				    const char *cache_name = NULL);

GR_API extern RE_Geometry *buildCircle(RE_Render *r,
				       UT_Vector3F center,
				       UT_Vector2F radius,
				       Orientation orient,
				       const char *cache_name = NULL);

GR_API extern RE_Geometry *buildSphere(RE_Render *r,
				       UT_Vector3F center,
				       UT_Vector3F radius,
				       const char *cache_name = NULL);

// 3D L, each axis colored. Wireframe only.
GR_API extern RE_Geometry *buildAxes(RE_Render *r,
				     UT_Vector3F center,
				     fpreal scale,
				     const char *cache_name = NULL);
// 3D +. Wireframe Only.
GR_API extern RE_Geometry *buildNull(RE_Render *r,
				     UT_Vector3F center,
				     fpreal scale,
				     const char *cache_name = NULL);
// simple bone
GR_API extern RE_Geometry *buildBone(RE_Render *r,
				     const char *cache_name = NULL);

// solved bone, a few more kinks in the middle
GR_API extern RE_Geometry *buildSolverBone(RE_Render *r,
					   const char *cache_name = NULL);

}

#endif

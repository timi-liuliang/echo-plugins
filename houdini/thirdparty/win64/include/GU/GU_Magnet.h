/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Utility Library (C++)
 *
 * COMMENTS:	GU_Magnet.h
 *
 */

#include "GU_API.h"
#include <UT/UT_XformOrder.h>
class GU_Detail;

class GA_Group;

class GU_API GU_MagnetParms 
{
public:
    const GU_Detail	*source;		// the source geometry 
    GU_Detail		*magnet;		// the magnet geometry
    UT_XformOrder 	order;			// deformation transform order
    float 		tx, ty, tz;		// translate value 
    float 		rx, ry, rz;		// rotate value
    float 		sx, sy, sz; 		// scale value
    float 		px, py, pz;		// pivot point
    int 		position; 		// affect position
    int 		color; 			// affect color
    bool		clampColor;		// clamp affected color 0..1
    int 		normals; 		// affect normals
    int 		velocity;		// affect velocity
    const GA_Group		*sourceGroup;	// source geometry to deform
    const GA_PrimitiveGroup	*magnetGroup;	// source magnets 
};

/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GU Library.
 *
 * COMMENTS:	This file gives a utility to generate a polygon surface given
 * 		a tetrahedron mesh
 *
 */
#ifndef __GU_TetraSurface_h__
#define __GU_TetraSurface_h__

#include "GU_API.h"

class GU_Detail;

bool GU_API GUgetTetraSurface(GU_Detail *gdp, const GU_Detail *src_gdp);

#endif

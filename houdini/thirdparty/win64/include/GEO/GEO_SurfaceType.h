/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *	This class is used as a base class for all patch types supported.
 *	The vertex list management is handled by this class, however, all
 *	other function should be handled by the subclass.
 *	The insertRow and insertCol are meant to be virtual since it
 *	may be possible for the patch type to insert a row or column
 *	without distorting the shape of the patch (as in NURBs or MESH)
 *
 */

#ifndef __GEO_SurfaceType_H__
#define __GEO_SurfaceType_H__

#include "GEO_API.h"

enum GEO_SurfaceType
{
	GEO_PATCH_ROWS		= 0,	// Just the rows
	GEO_PATCH_COLS		= 1,	// Just the columns
	GEO_PATCH_ROWCOL	= 2,	// Both rows and columns
	GEO_PATCH_TRIANGLE	= 3,	// Solid surface of triagles
	GEO_PATCH_QUADS		= 4,	// Solid surface of quads
	GEO_PATCH_ALTTRIANGLE	= 5,	// Triangles but common corners
	GEO_PATCH_REVTRIANGLE	= 6,	// Triangles but opposite direction
	GEO_PATCH_NUMTYPES		// Number of patch types
};

GEO_API extern GEO_SurfaceType	 GEOgetSurfaceType(const char *label);
GEO_API extern const char	*GEOgetSurfaceType(GEO_SurfaceType t);

#endif

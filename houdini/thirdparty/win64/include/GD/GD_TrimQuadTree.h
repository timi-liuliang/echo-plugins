/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry Library (C++)
 *
 * COMMENTS:
 *	This class handles trimming loops.  Each loop is a series of
 * bezier segments which in totality is closed.  Loops themselves
 * are embedded in a linked list of loops.
 * 
 */

#ifndef __GD_TrimQuadTree_h__
#define __GD_TrimQuadTree_h__

#include "GD_API.h"
#include <UT/UT_Defines.h>
#include <UT/UT_VectorTypes.h>
#include <UT/UT_BoundingRect.h>

class GD_TrimLoop;
class GD_TrimQuadBaseNode;

class GD_API GD_TrimQuadTree
{
public:
	GD_TrimQuadTree(const GD_TrimLoop &loops, const UT_BoundingRect &rect);
	~GD_TrimQuadTree();

	// When querying whether a region is entirely inside or outside, the
	// following enum is used.  Out means that the region is entirely
	// trimmed out.  In means that there are no trim curves on the interior
	// of the region.  Mix means that there are trim curves which pass
	// through the region.
	enum	GD_TrimState	{ GD_TRIM_OUT, GD_TRIM_IN, GD_TRIM_MIX };

	// Not const as updates cache:
	int	isTrimmedIn(const UT_Vector2 &pt);
	int	isTrimmedIn(float u, float v);

	// This also updates the cache.  It's a quick check to see if a whole
	// region is trimmed out.  It's only reasonably accurate though.
	GD_TrimState	isTrimmedOut(const UT_BoundingRect &rect);
	GD_TrimState	isTrimmedOut(float u0, float u1, float v0, float v1);

private:
	float	myUStart, myULength, myUConversion;
	float	myVStart, myVLength, myVConversion;
	GD_TrimQuadBaseNode	*myNodes;
	GD_TrimQuadBaseNode	*myCachedNode;
	GD_TrimLoop		*myLoops;
};

#endif

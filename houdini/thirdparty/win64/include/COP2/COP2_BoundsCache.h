/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	COP2_BoundsCache.h
 *
 * COMMENTS:
 *	Small cache class for image bounds, to avoid unnessary calls to
 *      getContextData.
 */

#ifndef __COP2_BoundsCache_h__
#define __COP2_BoundsCache_h__

#include "COP2_API.h"

#include <UT/UT_LinkList.h>
#include <OP/OP_Version.h>

class TIL_Plane;
class cop2_BoundsCacheEntry;

class COP2_API COP2_BoundsCache : protected UT_LinkList
{
public:
    COP2_BoundsCache(int limit = 16);
    ~COP2_BoundsCache();

    // returns false if the bound isn't found. Also clears any entries that
    // have a OP_VERSION less than v.
    bool	getBound(const TIL_Plane *plane, int array_index,
			 float t, int xres, int yres, OP_VERSION v,
			 int &x1, int &y1, int &x2, int &y2);

    // Cache a bound.
    void	addBound(const TIL_Plane *plane, int array_index,
			 float t, int xres, int yres, OP_VERSION v,
			 int x1, int y1, int x2, int y2);
private:
    int		myLimit;
};

#endif

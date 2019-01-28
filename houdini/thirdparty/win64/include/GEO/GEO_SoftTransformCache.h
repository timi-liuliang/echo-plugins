/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Geometry library (C++)
 *
 * COMMENTS:	Simple cache for distances between geometry elements
 * 		during soft transforms.  These structures are in essense
 * 		groups that are not attached to the gdp.  Along with
 * 		each soft element of the group, we have a float value which
 * 		stores the squared distance to the nearest hard element.
 *
 * 		Thus, with this cache we are able to store soft squared
 * 		distances, and implicitly store connectivity information.
 * 		Simply, if an element is not contained in the cache,
 * 		it will not be affected by the soft operation.
 */

#ifndef __GU_SoftTransformCache_H__
#define __GU_SoftTransformCache_H__

#include "GEO_API.h"
#include <GA/GA_Types.h>
#include <UT/UT_Array.h>
#include <UT/UT_FloatArray.h>

class GEO_SoftTransformCache
{
public:
    GEO_SoftTransformCache() {}
    ~GEO_SoftTransformCache() {}

    void appendHardOffset(GA_Offset offset)
    { myHardArray.append(offset); }

    void appendSoftOffset(GA_Offset offset, float dist2)
    {
        mySoftArray.append(offset);
        mySoftDistArray.append(dist2);
    }

    GA_Offset getHardOffset(exint i) const
    { return myHardArray(i); }

    GA_Offset getSoftOffset(exint i) const
    { return mySoftArray(i); }

    float getSoftDist2(exint i) const
    { return mySoftDistArray(i); }

    /// Number of hard points
    exint hardEntries() const
    { return myHardArray.entries(); }

    /// Number of soft points
    exint softEntries() const
    {
        UT_ASSERT_P(mySoftArray.entries() == mySoftDistArray.entries());
        return mySoftArray.entries();
    }

    void clearCache()
    {
        myHardArray.setCapacity(0);
        mySoftArray.setCapacity(0);
        mySoftDistArray.setCapacity(0);
    }

    // Access the arrays
    UT_Array<GA_Offset>	&hardArray() { return myHardArray; }
    UT_Array<GA_Offset>	&softArray() { return mySoftArray; }
    UT_FloatArray&       softDistArray() { return mySoftDistArray; }

private:
    UT_Array<GA_Offset>  myHardArray;
    UT_Array<GA_Offset>  mySoftArray;
    UT_FloatArray        mySoftDistArray;
};

#endif

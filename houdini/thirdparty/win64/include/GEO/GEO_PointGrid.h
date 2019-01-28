/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:    GEO_PointGrid (GEO Library, C++)
 *
 * COMMENTS:
 *
 */

#ifndef __GEO_PointGrid_h__
#define __GEO_PointGrid_h__

#include <UT/UT_PointGrid.h>
#include <UT/UT_Assert.h>

#include <GA/GA_Types.h>
#include <GA/GA_OffsetList.h>

#include <GEO/GEO_Detail.h>

template <typename INDEX, typename KEY>
class GEO_PointGridGDPAccessor
{
public:
    GEO_PointGridGDPAccessor(const GEO_Detail *gdp,
                             const GA_PointGroup *ptgrp = NULL):
                             myGdp(gdp), myPtgrp(ptgrp),
                             myOffsets(NULL), myBuilt(false)
    {
        UT_ASSERT(myGdp);
    }

    ~GEO_PointGridGDPAccessor()
    {
        delete myOffsets;
    }

    // indextype must be signed.
    typedef INDEX indextype;
    typedef KEY keytype;

    /// Invalid key value.
    static const keytype INVALIDKEY;

    // Return GA_Size instead of indextype so UT_PointGrid::canBuild can
    // test if points will fit in 32-bit index.
    GA_Size     entries() const
    {
        UT_ASSERT(myBuilt);
        return myOffsets ? myOffsets->entries() : myGdp->getNumPoints();
    }

    UT_Vector3  getPos(indextype idx) const
    {
        UT_ASSERT(myBuilt);
        return myOffsets ? myGdp->getPos3(myOffsets->get(idx)) :
                           myGdp->getPos3(myGdp->pointOffset(idx));
    }

    keytype     getKey(indextype idx) const
    {
        UT_ASSERT(myBuilt);
        // If no offset list exists, then the index represents the GA_Index
        // within the entire GEO_Detail.  If the point is not in a specified
        // point group, just return invalid, which tells the calling PointGrid
        // to skip adding this point to the grid.
        if (myOffsets)
            return keytype(myOffsets->get(idx));
        if (!myPtgrp)
            return keytype(myGdp->pointOffset(idx));
        GA_Offset ptoff = myGdp->pointOffset(idx);
        return myPtgrp->containsOffset(ptoff) ? keytype(ptoff) : INVALIDKEY;
    }

    // Return GA_Size instead of keytype so UT_PointGrid::canBuild can
    // test if key will fit in 32-bit.
    GA_Size     maxKeyValue() const
    {
        return myGdp->getPointMap().offsetSize();
    }

    int64 getMemoryUsage() const
    {
        UT_ASSERT(myBuilt);
        return sizeof(GEO_PointGridGDPAccessor<INDEX, KEY>) +
               (myOffsets ? myOffsets->getMemoryUsage(true) : 0);
    }

    // If a small enough point group is specified, build a contiguous list
    // of GA_Offsets.  The above functions then use the index into
    // this list to reference each point in sequence.
    void        build()
    {
        if (myPtgrp)
        {
            GA_Size numGroupPts = myPtgrp->entries();
            GA_Size numPts = myGdp->getNumPoints();
            // Only create offsets list if group comprises < 90% of all points.
            if (fpreal(numGroupPts) / numPts < 0.9)
            {
            myOffsets = new GA_OffsetList();
            myOffsets->setEntries(numGroupPts);
            GA_Size i = 0;
            for (GA_Iterator it = myGdp->getPointRange(myPtgrp).begin();
                !it.atEnd();
                ++it)
                    myOffsets->set(i++, *it);
            }
        }
        myBuilt = true;
    }

protected:
    const GEO_Detail    *myGdp;
    const GA_PointGroup *myPtgrp;
    GA_OffsetList       *myOffsets;
    bool                myBuilt;
};


// Default invalid key value.
template <typename INDEX, typename KEY>
const KEY
GEO_PointGridGDPAccessor<INDEX, KEY>::INVALIDKEY = std::numeric_limits<KEY>::max();


typedef UT_PointGrid<GEO_PointGridGDPAccessor<GA_Size, GA_Offset> > GEO_PointGrid;
typedef UT_PointGrid<GEO_PointGridGDPAccessor<int32, uint32> > GEO_PointGrid32;
typedef GEO_PointGrid::queuetype GEO_PointGridQueue;
typedef GEO_PointGrid32::queuetype GEO_PointGrid32Queue;

#endif

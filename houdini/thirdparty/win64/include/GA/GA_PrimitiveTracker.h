/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_PrimitiveTracker.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_PrimitiveTracker__
#define __GA_PrimitiveTracker__

#include "GA_API.h"
#include "GA_PrimitiveTypeId.h"
#include "GA_Types.h"
#include <UT/UT_Assert.h>

class GA_PrimitiveFactory;

/// Class which keeps counts of each type of primitive
class GA_API GA_PrimitiveTracker
{
public:
    SYS_FORCE_INLINE
    GA_PrimitiveTracker()
        : myCounts(nullptr)
        , myEntries(0)
    {}
    SYS_FORCE_INLINE
    ~GA_PrimitiveTracker()
    {
        clear();
    }

    SYS_FORCE_INLINE
    void add(int id)
    {
        if (id >= myEntries)
            grow(id+1);
        myCounts[id]++;
    }
    void add(const GA_PrimitiveTracker &that)
    {
        if (that.myEntries > myEntries)
            grow(that.myEntries);
        for (int id = 0; id < that.myEntries; ++id)
            myCounts[id] += that.myCounts[id];
    }
    SYS_FORCE_INLINE
    void addMultiple(int id, GA_Size n)
    {
        if (id >= myEntries)
            grow(id+1);
        myCounts[id] += n;
    }
    /// this += (changed - orig)
    /// This is so that items can be added or removed in separate trackers in
    /// parallel, and then the changes can be merged back into the original.
    void	applyDiff(const GA_PrimitiveTracker &orig, const GA_PrimitiveTracker &changed)
    {
        int maxentries = SYSmax(orig.myEntries, changed.myEntries);
        if (maxentries > myEntries)
            grow(maxentries);
        for (int id = 0; id < maxentries; ++id)
            myCounts[id] += ((id < changed.myEntries) ? changed.myCounts[id] : 0)
                          - ((id < orig.myEntries) ? orig.myCounts[id] : 0);
    }
    SYS_FORCE_INLINE
    void del(int id)
    {
        UT_ASSERT_P(id < myEntries);
        myCounts[id]--;
        UT_ASSERT_P(myCounts[id] >= 0);
    }
    void	clear()
		{
		    if (myEntries)
			grow(0);
		}
    bool	contains(const GA_PrimitiveTypeId &type) const
		{
		    int id = type.get();
		    return id >= myEntries ? false : myCounts[id] > 0;
		}
    bool	containsOnly(const UT_Array<GA_PrimitiveTypeId> &allowed) const
		{
		    for (int id = 0; id < myEntries; id++)
		    {
			if (!myCounts[id])
			    continue;

			// See if we are in the allowed list.
			bool matched = false;
			for (auto && allow : allowed)
			{
			    if (allow.get() == id)
			    {
				matched = true;
				break;
			    }
			}
			if (!matched)
			    return false;
		    }
		    return true;
		}
    GA_Size	count(const GA_PrimitiveTypeId &type) const
		{
		    int id = type.get();
		    return id >= myEntries ? 0 : myCounts[id];
		}

    GA_PrimitiveTracker &operator=(const GA_PrimitiveTracker &that)
    {
        if (myEntries != that.myEntries)
        {
            grow(that.myEntries);
        }
        for (int i = 0, n = myEntries; i < n; ++i)
        {
            myCounts[i] = that.myCounts[i];
        }
        return *this;
    }

    /// Check to see if there are any primitives which have hasLocalTransform()
    bool	hasLocalTransform(const GA_PrimitiveFactory &f) const;

    /// Check to see if there are any primitives that may need to flush
    /// CE buffesr.
    bool	hasCECaches(const GA_PrimitiveFactory &f) const;

    /// Extract the counts for each primitive type into an array.  The array
    /// should be large enough to hold gdp.getPrimitiveFactory().getTypeCount().
    /// The method returns the sum of all the primitive in the list
    GA_Size	fillCounts(GA_Size *counts, GA_Size array_size) const;

    int64 getMemoryUsage(bool inclusive) const
    {
        int64 mem = inclusive ? sizeof(*this) : 0;
        if (myEntries)
            mem += myEntries * sizeof(myCounts[0]);
        return mem;
    }

private:
    void	 grow(int size);

    GA_Size	*myCounts;
    int		 myEntries;
};

#endif

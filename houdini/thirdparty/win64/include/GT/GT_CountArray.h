/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GT_CountArray.h ( GT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GT_CountArray__
#define __GT_CountArray__

#include "GT_API.h"
#include "GT_DataArray.h"
#include "GT_Handles.h"

class UT_JSONWriter;

/// A count/offset array
///
/// In a polygon mesh, there is a list of faces.  Each face has a number of
/// vertices and a list of vertices.  Thus, for each polygon, there's an offset
/// into the vertex array and a count of the number of vertices to extract.
/// This array manages those offsets/counts efficiently
/// For example, if there are counts [3, 4, 2], the corresponding offset array
/// would be [0, 3, 7].  Thus: @code
///	counts.getOffset(0) == 0;
///	counts.getOffset(1) == 3;
///	counts.getOffset(2) == 7;
///	counts.getCount(0) == 3;
///	counts.getCount(1) == 4;
///	counts.getCount(2) == 2;
/// @endcode
class GT_API GT_CountArray
{
public:
    GT_CountArray()
	: myOffsets()
	, myMaxCount(0)
	, myMinCount(SYS_INT32_MAX)
    {
    }
    GT_CountArray(const GT_CountArray &src)
	: myOffsets(src.myOffsets)
	, myMinCount(src.myMinCount)
	, myMaxCount(src.myMaxCount)
    {
    }
    GT_CountArray(const GT_DataArrayHandle &counts,
	    GT_Size mincount=0, GT_Size maxcount=0)
	: myOffsets()
    {
	init(counts, mincount, maxcount);
    }

    /// Return memory used
    exint	getMemoryUsage() const;
    /// harden the data array
    void	harden();

    /// Clear the count array
    void	clear()
		{
		    myOffsets = GT_DataArrayHandle();
		    myMaxCount = 0;
		}

    /// Return the storage type
    GT_Storage	getStorage() const
		{
		    return myOffsets ? myOffsets->getStorage() : GT_STORE_INT32;
		}

    /// Initialize given an array of counts
    void	init(const GT_DataArrayHandle &counts,
			GT_Size mincount=0, GT_Size maxcount=0);
    /// Initialize when you know that there are @c entries that are all the
    /// same @c repeated_value.
    void	init(exint entries, exint repeated_value);

    GT_Size	entries() const	{ return myOffsets ? myOffsets->entries() : 0; }
    GT_Offset	getOffset(exint idx) const
		    { return idx ? myOffsets->getI64(idx-1) : 0; }
    GT_Size	getCount(exint idx) const
		    {
			GT_Size	size = myOffsets->getI64(idx);
			if (idx)
			    size -= myOffsets->getI64(idx-1);
			return size;
		    }

    /// Test equality
    bool	isEqual(const GT_CountArray &other) const
		{
		    if (myMinCount != other.myMinCount
			    || myMaxCount != other.myMaxCount)
		    {
			return false;
		    }
		    if (!myOffsets || !other.myOffsets)
			return !myOffsets && !other.myOffsets;
		    return myOffsets->isEqual(*other.myOffsets);
		}
    bool	operator==(const GT_CountArray &other) const
		    { return isEqual(other); }
    bool	operator!=(const GT_CountArray &other) const
		    { return !isEqual(other); }

    /// Return the minimum value that @c getCount() can return
    GT_Size	getMinCount() const	{ return myMinCount; }
    /// Return the maximum value that @c getCount() can return
    GT_Size	getMaxCount() const	{ return myMaxCount; }

    /// Return the sum of all the counts
    GT_Size	sumCounts() const	{ return getOffset(entries()); }

    GT_DataArrayHandle	extractCounts() const;
    GT_DataArrayHandle	extractOffsets() const;

    /// Creates a list of {0,0,0, 1,1,1,1,1, 2,2} from {3,5,2} 
    GT_DataArrayHandle  buildRepeatList() const;

    /// Saves as an array of the counts (not the offsets).  i.e.
    ///  [ getCount(0), getCount(1), ... getCount(n) ]
    bool	save(UT_JSONWriter &w) const;
private:
    GT_DataArrayHandle	myOffsets;
    GT_Size		myMinCount;
    GT_Size		myMaxCount;
};

#endif

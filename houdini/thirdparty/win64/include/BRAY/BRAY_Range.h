/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	BRAY_Range.h (BRAY Library, C++)
 *
 * COMMENTS:
 */

#ifndef __BRAY_Range__
#define __BRAY_Range__

#include "BRAY_API.h"
#include <iterator>
#include <UT/UT_Array.h>
#include <UT/UT_ArrayMap.h>
#include <UT/UT_Map.h>
#include <GA/GA_OffsetList.h>

class UT_JSONWriter;
class BRAY_HitList;

/// @brief Define a partition of hits/rays.
///
/// This class provides an indirection array to allow a caller to process a
/// sub-set of rays/hits without having to copy the ray/hit information to a
/// separate array.
class BRAY_API BRAY_Range
{
public:
    using ListType = GA_ListType<exint, exint>;

    BRAY_Range()
    {
    }
    /// Initialize with a contiguous range [0, size)
    explicit BRAY_Range(exint size);
    ~BRAY_Range()
    {
    }

    /// @{
    /// Add elements to the range
    void	append(exint idx) { myIndices.append(idx); }
    void	append(const exint *idx, exint n)
    {
	myIndices.reserve(myIndices.size() + n);
	for (exint i = 0; i < n; ++i)
	    myIndices.append(idx[i]);
    }
    /// @}

    void	set(const exint *idx, exint n)
    {
	myIndices.clear();
	append(idx, n);
    }

    /// Prune list based on hits
    void	prune(const BRAY_HitList &hits);

    /// Prune list based on a sub-range.  The range contains the indices to
    /// remove.
    void	prune(const BRAY_Range &range);

    /// Prune list based on a set of indices...  The list of indices to remove
    /// must be sorted.
    void	prune(const UT_Array<exint> &rmlist);

    /// Return the number of elements in this range
    exint		size() const { return myIndices.size(); }

    /// Return the index of the maximum element in this range
    exint		maxIndex() const
			{
			    return myIndices.size() ?  myIndices.last() : -1;
			}
    /// Return the index of the maximum element in this range
    exint		maxSize() const { return myIndices.last()+1; }

    /// Ensure indices are sorted for more coherent processing
    void		optimize()
    {
	// The list checks for trivial entries for us.
	myIndices.sortAndRemoveDuplicates();
    }

    /// Test to see if it's an ordered array of [0..size)
    bool		isTrivial() const { return myIndices.isTrivial(); }

    /// Partition this range into multiple ranges based on a separate array.
    /// Any items in the predicate that match the @c ignore are skipped in the
    /// results.  The predicate should be indexed using the indirection array
    /// in the range (i.e. 0->range.size() with no holes).
    ///
    /// Specialized for T := { int32, int64 }
    template <typename T>
    void		partition(UT_Map<T, BRAY_Range> &results,
				const T *predicate,
				T ignore=-1) const;

    /// @{
    /// Look up the index associated with the given index
    exint		index(exint i) const { return myIndices.get(i); }
    exint		operator[](exint i) const { return myIndices.get(i); }
    /// @}

    /// Most efficient iterator.  The functor takes the iteration index as an
    /// argument.
    template <typename FUNC>
    void	forEach(FUNC &&func) const
    {
	myIndices.forEach(func);
    }

    /// @{
    /// Standard iterators
    using iterator = ListType::iterator;
    using const_iterator = ListType::const_iterator;
    iterator		begin() { return myIndices.begin(); }
    iterator		end() { return myIndices.end(); }
    const_iterator	begin() const { return myIndices.begin(); }
    const_iterator	end() const { return myIndices.end(); }
    /// @}

    void	dump() const;
    void	dump(UT_JSONWriter &w) const;

private:
    ListType	myIndices;
};


#endif

/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_MergeOffsetMap.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_MergeOffsetMap__
#define __GA_MergeOffsetMap__

#include <UT/UT_Map.h>
#include "GA_API.h"
#include "GA_Types.h"
#include "GA_Range.h"
#include "GA_MergeOptions.h"


class GA_MergeMap;
class GA_MergeOptions;
template <typename T> class GA_TempBuffer;


/// @brief Keeps track of offset mapping when merging index lists
/// 
/// When an index map is merged with an existing index map, there are several
/// different ways the index lists can be merged.
/// # A simple merge.  This happens when the destination index map is empty.
/// # A full append merge.  The source list is appended to the destination
/// # A full interleaved merge.  Source elements use up holes in the dest.
/// # A selected merge.  Only selected elements from the source are merged
///
/// @see GA_MergeMap, GA_MergeOptions
///
class GA_API GA_MergeOffsetMap 
{
public:
     GA_MergeOffsetMap(GA_MergeMap &map,
			const GA_Range *sit,
			GA_AttributeOwner owner);
    ~GA_MergeOffsetMap();

    GA_MergeOptions::MergeStrategy getMergeStrategy() const
				{ return myStrategy; }

    /// Create a merge-iterator.  The merge iterator will iterate over the
    /// source elements which are to be merged into the destination
    const GA_Range	&getSourceRange() const { return mySourceRange; }

    /// Create a merge-iterator.  The merge iterator will iterate over the
    /// elements which were created in the destination detail.
    ///
    /// @note It is more efficient to use mapDestFromSource() with the source
    /// range, especially when iterating over both.
    const GA_Range	&getDestRange() const { return myDestRange; }

    /// The array capacity is the original capacity of attribute arrays.
    /// This is set for MERGE_COPY, MERGE_APPEND and MERGE_INTERLEAVE.
    GA_Offset		getDestInitCapacity() const
						{ return myDestInitCapacity; }

    /// The array capacity is the required capacity of attribute arrays to
    /// perform the merge.  This is set for MERGE_COPY, MERGE_APPEND and
    /// MERGE_INTERLEAVE, though in the MERGE_INTERLEAVE scenario, the
    /// additional capacity will already have been allocated.
    GA_Offset		getDestCapacity() const { return myDestCapacity; }

    /// @{
    /// Set for MERGE_COPY and MERGE_APPEND.  Specify where in the attribute
    /// arrays to place the merged source data.
    GA_Offset		getDestStart() const { return myDestStart; }
    GA_Offset		getDestEnd() const { return myDestEnd; }
    /// @}

    /// Sets map to be a detail map (GA_Offset(0)-->GA_Offset(0))
    void		 setDetail();

    /// Set destination offsets for source range
    void                 setDestForSourceRange(GA_Offset d);

    /// Given the source offset, find the corresponding destination offset.
    GA_Offset		mapDestFromSource(GA_Offset source_offset) const;

    /// An identity map means mapDestFromSource(i) == i for all elements.
    /// @note A false return value doesn't necessarily mean this map is not
    /// an identity map, but only that our simple detection failed.
    bool		isIdentityMap() const;

    /// @private Used by the index map to set the destination initial capacity
    /// for merging.  Valid for MERGE_COPY, MERGE_APPEND, MERGE_INTERLEAVE.
    void		 setDestInitCapacity(GA_Offset s)
						{ myDestInitCapacity = s; }

    /// @private Used by the index map to set the destination capacity for
    /// merging.  Valid for MERGE_COPY, MERGE_APPEND, MERGE_INTERLEAVE.
    void		 setDestCapacity(GA_Offset s)	{ myDestCapacity = s; }

    /// @{
    /// @private Used by the index map to set the destination range where
    /// the merged data is to be placed (MERGE_COPY, MERGE_APPEND).  This
    /// range is inclusive [start..end].
    void		setDestStart(GA_Offset s)	{ myDestStart = s; }
    void		setDestEnd(GA_Offset e)		{ myDestEnd = e; }
    /// @}

    /// @private Used by the GA_MergeMap to finish constructing iterators
    void		 makeRanges(GA_MergeMap &map);

    /// @private Used by the GA_MergeMap to finish constructing trivial map
    void                 makeTrivialRanges(GA_MergeMap &map);

    /// @private Used to fill out GA_MergeOptions point maps
    void		 buildOptionPointMaps(GA_MergeOptions &options);

    /// Returns true if the map is trivial
    bool                 isTrivial() const { return myIsTrivial; }

    /// Returns the value that should be added to source indices to
    /// map onto destination indices
    GA_Size              getTrivialDiff() const { return myTrivialSrcToDest; }

protected:
    /// Builds the myOffsetArray or myOffsetTable
    /// Assumes already guaranteed not trivial.
    void		 buildSourceToDestMap(GA_Offset d);

private:
    GA_MergeMap			&myMap;
    GA_Range			 mySourceRange, myDestRange;
    GA_TempBuffer<GA_Offset>	*myOffsetArray;
    UT_Map<GA_Offset, GA_Offset>		*myOffsetTable;
    GA_Offset			 myDestStart, myDestEnd;
    GA_Offset			 myDestInitCapacity, myDestCapacity;
    GA_AttributeOwner		 myOwner;
    GA_MergeOptions::MergeStrategy	 myStrategy;
    bool                         myIsTrivial;
    GA_Size                      myTrivialSrcToDest;
    /// NOTE: myTrivialDestEnd is the end of the trivial range (NOT minus 1);
    ///       myDestEnd is sometimes instead the offset *capacity* (minus 1).
    GA_Offset                    myTrivialDestEnd;
    /// Mapping not yet built, but delayed.
    bool			 myDelayedMap;
    mutable int			 myDelayedMapBuilt;
};

#endif

/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_Iterator.h (GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_Iterator__
#define __GA_Iterator__

#include "GA_API.h"
#include "GA_IndexMap.h"
#include "GA_IteratorState.h"
#include "GA_Range.h"
#include "GA_Types.h"
#include <UT/UT_Assert.h>


/// @brief Iteration over a range of elements
///
/// An iterator is used to iterate over the elements of a given range.  All
/// state information for the iteration is stored by the iterator, external
/// to the range in question.  All iterators are constructed rewound, so in
/// most cases there is no need for an explicit rewind() call.
class GA_API GA_Iterator
{
public:
     GA_Iterator();
     explicit GA_Iterator(const GA_Range &range);
     /// Ensure safe iteration if elements from the range are deleted during
     /// traversal.
     GA_Iterator(const GA_Range &range, GA_Range::safedeletions);
     GA_Iterator(const GA_Iterator &iterator);
     /// The end-condition iterator
     explicit GA_Iterator(GA_Range::enditerator)
	: myRange()
	, myState()
	, myCurrent(GA_INVALID_OFFSET)
	, myEnd(GA_INVALID_OFFSET)
	, myEndIterator(true)
    {}
    ~GA_Iterator();

    /// Test to see whether the iterator is valid
    bool	isValid() const	{ return myRange.isValid(); }

    /// @{
    /// Standard operators
    const GA_Iterator	&operator=(const GA_Iterator &src);
    bool		 operator==(const GA_Iterator &src) const;
    bool                 operator!=(const GA_Iterator &src) const
			    { return !(*this == src); }
    /// @}

    /// Query the element type that is being iterated over
    GA_AttributeOwner	getOwner() const
    {
	return myRange.getOwner();
    }

    /// @{
    /// Query state of the iterator
    GA_Offset		getOffset() const
    {
        UT_ASSERT_P(!atEnd());
        return myCurrent;
    }
    GA_Offset		operator*() const
    {
        UT_ASSERT_P(!atEnd());
        return myCurrent;
    }
    /// @}

    /// @{
    /// Query the ordered index for the current state.  Note that this will
    /// perform a lookup into the index map, which may not be extremely
    /// efficient.
    GA_Index		getIndex() const
    {
        UT_ASSERT_P(!atEnd());
	if (!myEndIterator && myRange.getRTI())
	{
	    return myRange.getRTI()->getIndexMap().indexFromOffset(myCurrent);
	}
	return GA_INVALID_INDEX;
    }
    /// @}

    /// @{
    /// Standard iterator methods.
    /// @warning We do not have a post-increment as it would have to
    /// be written in a side-effect free way that would cause problmse.
    /// @note All non-default constructors call rewind() already, so an
    /// explicit call is usually not needed.
    void	rewind();
    bool	atEnd() const { return myEndIterator || myCurrent >= myEnd; }
    void	advance()
    {
        UT_ASSERT_P(!atEnd());
	myCurrent++;
	if (myCurrent == myEnd)
	{
	    // We hit the end of our cached block, so iterate to the next block
	    myRange.iterateNext(myState, myCurrent, myEnd);
	}
    }
    GA_Iterator	&operator++()		{ advance(); return *this; }
    /// @}
    
    /// Perform a block iteration.  The block iteration will provide a
    /// contiguous block of offsets which can be operated upon in batch.
    /// Blocks are constrained to occupy a single page, so no block can
    /// cross a page boundary.  Consequently contiguous sequences that
    /// cross page boundaries are automatically split into multiple blocks.
    /// The (start, end] range is like Python in that @c start is included
    /// in the range, but @c end is not.  For example:@code
    ///	for (it.rewind(); iterator.blockAdvance(start, end); )
    ///	    for (; start < end; ++start)
    ///		operation(start);
    /// @endcode
    bool	blockAdvance(GA_Offset &start, GA_Offset &end);

    /// Like blockAdvance, except that the block isn't guaranteed to be
    /// all within one page.  The block is the maximum contiguous block
    /// returned by the GA_Range.
    bool        fullBlockAdvance(GA_Offset &start, GA_Offset &end);

private:
    GA_Range		myRange;
    GA_IteratorState	myState;
    GA_Offset		myCurrent, myEnd; // Block information
    bool		myEndIterator;
};

#endif

/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_RTISingle.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_RTISingle__
#define __GA_RTISingle__

#include "GA_API.h"
#include "GA_RangeTypeInterface.h"
#include "GA_Types.h"


class GA_IndexMap;
class GA_IteratorState;


/// @brief Base class for ranges providing consolidation to contiguous blocks.
///
/// GA_Iterator uses contiguous blocks for iteration, but it is often easier
/// to implement iteration over single elements.  Such range implementations
/// can derive from this class to automatically perform the required
/// consolidation.
///
/// @see GA_Range
class GA_API GA_RTISingle : public GA_RangeTypeInterface 
{
public:
	     GA_RTISingle(const GA_IndexMap &list);
	     GA_RTISingle(const GA_RTISingle &other);
    virtual ~GA_RTISingle();

    /// Rewind the iterator (i.e. reset the iterator state).  The start/offset
    /// should be initialized so that they define the first contiguous "block"
    /// for iteration.  If the range is empty, @c start should be greater than
    /// @c end
    virtual void	iterateRewind(GA_IteratorState &state,
				GA_Offset &start, GA_Offset &end) const;

    /// Choose the next contiguous range.  If the iteration is complete,
    /// @c start should be set to a value greater than @c end.
    virtual void	iterateNext(GA_IteratorState &state,
				GA_Offset &start, GA_Offset &end) const;

protected:
    /// @{
    /// Some iterators are not set up to perform block iteration.  These
    /// methods can be used to define single iteration.  The default
    /// iterateRewind() and iterateNext() methods will call these methods to
    /// collate single iterations into a block.
    ///
    /// - @c singleRewind() @n
    ///	  Start iteration process
    /// - @c singleGet() @n
    ///	  Get the current value of the iteration.  Return false if the
    ///	  iteration is complete.
    /// - @c singleNext() @n
    ///   Iterate to the next value
    ///
    /// Iteration using the single interface goes something like: @code
    ///	for (singleRewind(state); singleGet(state, offset); singleNext(state))
    /// @endcode
    /// @warning Even if you've optimized the iterateRewind()/iterateNext()
    /// code, you still need to implement these methods.
    virtual void	singleRewind(GA_IteratorState &state) const=0;
    virtual bool	singleGet(const GA_IteratorState &state,
				GA_Offset &value) const = 0;
    virtual void	singleNext(GA_IteratorState &state) const = 0;
    /// @}

private:
    void	singleIterate(GA_IteratorState &state,
			GA_Offset &start, GA_Offset &end) const;
};
#endif

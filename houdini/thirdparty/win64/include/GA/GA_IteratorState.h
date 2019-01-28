/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_IteratorState.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_IteratorState__
#define __GA_IteratorState__

#include "GA_API.h"
#include "GA_Types.h"

#include <UT/UT_NonCopyable.h>

#include <stddef.h>


/// State information for a GA_Iterator.  This stores unique information for
/// each iterator which the GA_Range bound to the iterator can reference.
class GA_API GA_IteratorState : public UT_NonCopyable
{
public:
    GA_IteratorState()
	: myPointer(NULL)
	, myOffset(GA_INVALID_OFFSET)
	, myIndex(GA_INVALID_INDEX)
	, mySize(0)
	, myNestedState(NULL)
    {}

    ~GA_IteratorState();

    /// Copy the POD state (not including nested state)
    void		copyState(const GA_IteratorState &src)
			{
			    myPointer = src.myPointer;
			    myOffset = src.myOffset;
			    myIndex = src.myIndex;
			    mySize = src.mySize;
			}

    /// These variables provide easy storage to GA_RangeTypeInterface
    /// without the style necessarily having to allocate state
    /// information (i.e. the named storage)
    void		*myPointer;
    GA_Offset		 myOffset;
    GA_Index		 myIndex;
    GA_Size		 mySize;

    /// Some ranges contain nested ranges.  These RTIs can create nested
    /// states.  These nested states should be used to store the iteration
    /// state for the nested range.
    /// Call @c createNestedState() in GA_RangeTypeInterface::iterateCreate().
    /// And it should only be called once.
    /// For example, a nested range might have code like: @code
    ///  GA_RTINested::iterateCreate(GA_IteratorState &state)
    ///  {
    ///      myBase->iterateCreate(state.createNestedState());
    ///      // Use any data in state to store the parent's state information
    ///  }
    ///  
    ///  GA_RTINested::iterateDestroy(GA_IteratorState &state)
    ///  {
    ///      myBase->iterateDestroy(&state.getNestedState());
    ///      state.clearNestedState();
    ///      // Do any cleanup on state that you need
    ///  }
    ///  
    ///  GA_RTINested::iterateRewind(GA_IteratorState &state,
    ///  	    GA_Offset &start, GA_Offset &end) const
    ///  {
    ///      GA_Offset	n_start, n_end;
    ///      myBase->iterateRewind(state.getNestedState(), n_start, n_end);
    ///      // Use base_start base_end and the state to rewind the parent
    ///  }
    ///  GA_RTINested::iterateCopy(GA_IteratorState &d,
    ///                            const GA_IteratorState &s)
    ///  {
    ///      GA_IteratorState &nest = dest.createNestedState();
    ///      myBase->iterateCopy(nest, src.getNestedState());
    ///      state.copyState(src);
    ///  }
    /// @endcode
    GA_IteratorState	&createNestedState();

    /// Clear the nested pointer.
    void		 clearNestedState();

    /// Access to the nested state.
    GA_IteratorState	&getNestedState() const;

private:
    GA_IteratorState	*myNestedState;
};

#endif

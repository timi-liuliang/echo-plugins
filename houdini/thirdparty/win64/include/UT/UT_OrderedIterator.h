/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_OrderedIterator.h ( UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_OrderedIterator__
#define __UT_OrderedIterator__

#include "UT_ValArray.h"
#include "UT_SharedPtr.h"

/// Given an iterator and a comparison function, iterate in sorted order
///
/// The iterator type must support:
///	- atEnd()
///	- T operator*()
/// Currently, the ordered iterator extracts the items from the source
/// iterator, storing them in an array which is then sorted.
template <typename T, typename ITERATOR_T>
class UT_OrderedIterator
{
public:
    /// Storage for the list of items
    typedef UT_ValArray<T>	ListType;
    typedef int (*Comparator)(const T *, const T *);

    /// Default c-tor
    UT_OrderedIterator()
	: myList()
	, myCurr(0)
    {
    }
    /// Copy c-tor
    UT_OrderedIterator(const UT_OrderedIterator &src)
	: myList(src.myList)
	, myCurr(src.myCurr)
    {
    }
    /// Construct an ordered list by creating a copy of the items and sorting
    /// them.
    UT_OrderedIterator(ITERATOR_T it, Comparator cmp)
	: myList(new ListType())
	, myCurr(0)
    {
	for (; !it.atEnd(); ++it)
	    myList->append(*it);
	myList->sort(cmp);
    }

    /// Test equality
    bool	operator==(const UT_OrderedIterator<T, ITERATOR_T> &src) const
		{
		    if (atEnd() && src.atEnd())
			return true;
		    return myList == src.myList && myCurr == src.myCurr;
		}
    /// Test inequality
    bool	operator!=(const UT_OrderedIterator<T, ITERATOR_T> &src) const
		{
		    return !(*this == src);
		}
    /// Assignment operator
    UT_OrderedIterator<T,ITERATOR_T>	&operator=(const UT_OrderedIterator<T,ITERATOR_T> &src)
		{
		    myList = src.myList;
		    myCurr = src.myCurr;
		    return *this;
		}

    /// Pre-increment operator
    UT_OrderedIterator<T,ITERATOR_T>	&operator++()
		{
		    advance();
		    return *this;
		}
    /// Post-increment operator
    UT_OrderedIterator<T,ITERATOR_T>	&operator++(int)
		{
		    UT_OrderedIterator<T,ITERATOR_T>	tmp(*this);
		    advance();
		    return tmp;
		}

    /// @{
    /// Get the current item
    T		&item() const		{ return (*myList)(myCurr); }
    T		&operator*() const	{ return item(); }
    /// @}

    /// Return current index
    exint	index() const		{ return myCurr; }

    /// Return the number of elemenets in the iteration
    exint	entries() const		{ return myList ? myList->entries():0; }

    /// Rewind and start iteration again
    void	rewind()	{ myCurr = 0; }
    /// Test if at end of the list
    bool	atEnd() const	{ return !myList || myCurr>=myList->entries(); }
    /// Advance to the next iteration
    void	advance()	{ ++myCurr; }

private:
    UT_SharedPtr<ListType>	myList;
    exint			myCurr;
};

#endif

/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	GA_TempBuffer.h ( GA Library, C++)
 *
 * COMMENTS:
 */

#ifndef __GA_TempBuffer__
#define __GA_TempBuffer__

#include "GA_API.h"
#include "GA_Types.h"

#include <UT/UT_Algorithm.h>
#include <UT/UT_ArrayHelp.h>
#include <UT/UT_Assert.h>

#include <SYS/SYS_Types.h>

#include <string.h>


#define	GA_TEMP_BUFFER_SIZE	32

/// Template class to hold an array of temporary objects.
///
/// These arrays have a temporary buffer.  When the buffer fills, the array
/// will grow by allocating off the heap.  The buffers are implemented for
/// simple POD objects.
template <typename T>
class GA_TempBuffer
{
public:
    GA_TempBuffer()
	: myData(myBuffer)
	, mySize(0)
	, myCapacity(GA_TEMP_BUFFER_SIZE)
    {}
    ~GA_TempBuffer()
    {
	clear();
    }

    /// Clear all entries
    void	clear()
		{
		    if (myData != myBuffer)
		    {
			free(myData);
		    }
		    myData = myBuffer;
		    mySize = 0;
		    myCapacity = GA_TEMP_BUFFER_SIZE;
		}

    /// How many elements in the buffer
    GA_Size	entries() const		{ return mySize; }

    /// Query the reserved capacity
    GA_Size	getCapacity() const	{ return myCapacity; }

    /// Reserve storage
    void	reserve(GA_Size size, bool setsize=false)
		{
		    if (size > myCapacity)
			growBuffer(size);
                    if (setsize)
                        mySize = size;
		    UT_ASSERT(myCapacity >= size);
		}

    /// Append an element to the array
    void	append(const T &value)
		{
		    if (mySize == myCapacity)
			growBuffer();
		    myData[mySize] = value;
		    mySize++;
		}
    /// Set an item in the buffer to the given value
    void	setValue(GA_Size i, const T &value)
		{
		    UT_ASSERT_P(i >= 0 && i < mySize);
		    myData[i] = value;
		}
    /// @{
    /// Return the value from the array
    const T	&getValue(GA_Size i) const
		{
		    UT_ASSERT_P(i >= 0 && i < mySize);
		    return myData[i];
		}
    const T	&operator()(GA_Size i) const	{ return getValue(i); }
    const T	&last() const
		{
		    UT_ASSERT_P(mySize > 0);
		    return myData[mySize-1];
		}
    /// @}

    /// Return the last entry and remove it from the list
    const T	&removeLast()
		{
		    UT_ASSERT_P(mySize > 0);
		    mySize--;
		    return myData[mySize];
		}
    void	truncate(GA_Size sz)
		{
		    mySize = SYSmin(mySize, sz);
		}
    exint	removeMatchingItems(const T &value)
		{
		    exint	s, d;
		    exint	nrm = 0;
		    for (s = d = 0; s < mySize; ++s)
		    {
			if (myData[s] != value)
			{
			    if (s != d)
				myData[d] = myData[s];
			    d++;
			}
			else
			    nrm++;
		    }
		    mySize = d;
		    return nrm;
		}
    void	removeIndex(exint index)
		{
		    UT_ASSERT_P(index >= 0 && index < mySize);
		    for (exint i = index+1; i < mySize; ++i)
			myData[i-1] = myData;
		    mySize--;
		}

    const T	*data() const	{ return myData; }
    T		*data(GA_Size size)
		 {
		     reserve(size);
		     return myData;
		 }

    void	swap(GA_Size i0, GA_Size i1)
		{
		    UT_ASSERT(i0 >= 0 && i0 < mySize);
		    UT_ASSERT(i1 >= 0 && i1 < mySize);
		    UTswap(myData[i0], myData[i1]);
		}

private:
    void	growBuffer(GA_Size nsize=-1)
		{
                    // Always grow at least to the next UTbumpAlloc
		    nsize = SYSmax(nsize, UTbumpAlloc(myCapacity+1));
		    if (myData == myBuffer)
		    {
			UT_ASSERT_P(myCapacity == GA_TEMP_BUFFER_SIZE);
			myData = (T *)malloc(sizeof(T)*nsize);
			memcpy(myData, myBuffer, sizeof(T)*myCapacity);
		    }
		    else
		    {
			myData = (T *)realloc(myData, sizeof(T)*nsize);
		    }
		    myCapacity = nsize;
		}
    T		*myData;
    T		 myBuffer[GA_TEMP_BUFFER_SIZE];
    GA_Size	 mySize, myCapacity;
};

#endif

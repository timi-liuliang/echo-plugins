/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_RingBuffer.C ( UT Library, C++)
 *
 * COMMENTS:
 */

#include "UT_RingBuffer.h"
#include "UT_Assert.h"
#include "UT_ArrayHelp.h"
#include "UT_Memory.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

template <typename T>
UT_RingBuffer<T>::UT_RingBuffer()
{
    clear();
}

template <typename T>
UT_RingBuffer<T>::UT_RingBuffer(int capacity)
    : myData(capacity, capacity)
{
    clear();
}

template <typename T>
UT_RingBuffer<T>::~UT_RingBuffer()
{
}

template <typename T>
void
UT_RingBuffer<T>::insert(int id, const T &data)
{
    if (!entries())
    {
	growIfNeeded(1);
	myFirst = myLast = 0;
	myFirstId = myLastId = id;
    }
    else
    {
	UT_ASSERT(id > myLastId);
	growIfNeeded(id - myLastId);
	myLast = idIndex(id);
	myLastId = id;
    }
    myData(myLast) = data;
}

template <typename T>
void
UT_RingBuffer<T>::remove(int id)
{
    int		idx;

    if (id == myFirstId)
	pop();
    else
    {
	idx = idIndex(id);
	myData(idx) = 0;
    }
}

template <typename T>
void
UT_RingBuffer<T>::clear()
{
    myFirst = 0;
    myLast = -1;
    myFirstId = 0;
    myLastId = -1;
    myPeak = 0;
}

template <typename T>
void
UT_RingBuffer<T>::pop(int number)
{
    int		i;

    UT_ASSERT_P(number <= myLastId - myFirstId + 1);
    for (i = 0; i < number; i++)
	pop();
}

template <typename T>
T
UT_RingBuffer<T>::operator[](int index) const
{
    if (index < myFirstId || index > myLastId)
	return 0;
    return myData(idIndex(index));
}

template <typename T>
void
UT_RingBuffer<T>::display() const
{
    fprintf(stderr, "First = %d\n", myFirst);
    fprintf(stderr, "Last = %d\n", myLast);
    fprintf(stderr, "FirstId = %d\n", myFirstId);
    fprintf(stderr, "LastId = %d\n", myLastId);
    fprintf(stderr, "Capacity = %d\n", myData.entries());
    fprintf(stderr, "Peak = %d\n", myPeak);
}

template <typename T>
void
UT_RingBuffer<T>::growData(int number)
{
    if (entries() > myPeak)
	myPeak = entries();
    if (entries() + number > myData.entries())
    {
	int	origcapacity;
	int	capacity;

	origcapacity = myData.entries();
	capacity = UTbumpAlloc(origcapacity + number);

	// This is to ensure the array is made exactly the given
	// capacity
	myData.setCapacity(capacity);
	myData.entries(capacity);
	
	// If Necessary, we need to move one portion of the data to the end
	// of the array.
	if (myLast >= 0 && myLast < myFirst)
	{
	    myData.move(myFirst,
			myFirst + capacity - origcapacity,
			origcapacity - myFirst);
	    myFirst += capacity - origcapacity;
	}
    }
}

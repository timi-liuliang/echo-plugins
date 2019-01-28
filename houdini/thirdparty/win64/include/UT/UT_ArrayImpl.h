/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Utility Library (C++)
 *
 * COMMENTS:
 *	This is meant to be included by UT_Array.h and includes
 *	the template implementations needed by external code.
 */

#pragma once

#ifndef __UT_ARRAYIMPL_H_INCLUDED__
#define __UT_ARRAYIMPL_H_INCLUDED__

#include "UT_ArrayHelp.h"
#include "UT_Assert.h"
#include "UT_Compare.h"
#include "UT_Swap.h"
#include "UT_Types.h"
#include <VM/VM_Math.h>
#include <SYS/SYS_Math.h>

#include <algorithm>
#include <utility>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Implemented in UT_Array.C
UT_API void ut_ArrayImplFree(void *p);


template <typename T>
UT_Array<T>::UT_Array(const UT_Array<T> &a)
    : myCapacity(a.size()), mySize(a.size())
{
    if (myCapacity)
    {
	myData = allocateCapacity(myCapacity);
	copyConstructRange(myData, a.array(), mySize);
    }
    else
    {
	myData = nullptr;
    }
}

template <typename T>
UT_Array<T>::UT_Array(std::initializer_list<T> init)
    : myCapacity(init.size()), mySize(init.size())
{
    if (myCapacity)
    {
	myData = allocateCapacity(myCapacity);
	copyConstructRange(myData, init.begin(), mySize);
    }
    else
    {
	myData = nullptr;
    }
}

template <typename T>
UT_Array<T>::UT_Array(UT_Array<T> &&a) SYS_NOEXCEPT 
{
    if (!a.isHeapBuffer())
    {
	myData = nullptr;
	myCapacity = 0;
	mySize = 0;
	operator=(std::move(a));
	return;
    }

    myCapacity = a.myCapacity;
    mySize = a.mySize;
    myData = a.myData;
    a.myCapacity = a.mySize = 0;
    a.myData = nullptr;
}


template <typename T>
UT_Array<T>::~UT_Array()
{
    // NOTE: We call setCapacity to ensure that we call trivialDestructRange,
    //       then call free on myData.
    setCapacity(0);
}

template <typename T>
inline T *
UT_Array<T>::allocateCapacity(exint capacity)
{
    T *data = (T *)malloc(capacity * sizeof(T));
    // Avoid degenerate case if we happen to be aliased the wrong way
    if (!isHeapBuffer(data))
    {
	T *prev = data;
	data = (T *)malloc(capacity * sizeof(T));
	ut_ArrayImplFree(prev);
    }
    return data;
}

template <typename T>
void
UT_Array<T>::swap( UT_Array<T> &other )
{
    UTswap( myData, other.myData );
    UTswap( myCapacity, other.myCapacity );
    UTswap( mySize, other.mySize );
}


template <typename T>
exint	
UT_Array<T>::insert(exint index)
{
    if (index >= mySize)
    {
	bumpCapacity(index + 1);

	trivialConstructRange(myData + mySize, index - mySize + 1);

	mySize = index+1;
	return index;
    }
    bumpCapacity(mySize + 1);

    UT_ASSERT_P(index >= 0);
    ::memmove((void *)&myData[index+1], (void *)&myData[index],
              ((mySize-index)*sizeof(T)));

    trivialConstruct(myData[index]);

    mySize++;
    return index;
}

template <typename T>
template <typename S>
exint
UT_Array<T>::appendImpl(S &&s)
{
    if (mySize == myCapacity)
    {
	exint idx = safeIndex(s);

        // NOTE: UTbumpAlloc always returns a strictly larger value.
	setCapacity(UTbumpAlloc(myCapacity));
        if (idx >= 0)
            construct(myData[mySize], std::forward<S>(myData[idx]));
        else
            construct(myData[mySize], std::forward<S>(s));
    }
    else
    {
	construct(myData[mySize], std::forward<S>(s));
    }
    return mySize++;
}

template <typename T>
template <typename... S>
exint
UT_Array<T>::emplace_back(S&&... s)
{
#if UT_ASSERT_LEVEL >= UT_ASSERT_LEVEL_PARANOID
    validateEmplaceArgs(std::forward<S>(s)...);
#endif

    if (mySize == myCapacity)
	setCapacity(UTbumpAlloc(myCapacity));

    construct(myData[mySize], std::forward<S>(s)...);
    return mySize++;
}

template <typename T>
void
UT_Array<T>::append(const T *pt, exint count)
{
    bumpCapacity(mySize + count);
    copyConstructRange(myData + mySize, pt, count);
    mySize += count;
}

template <typename T>
void
UT_Array<T>::appendMultiple(const T &t, exint count)
{
    UT_ASSERT_P(count >= 0);
    if (count <= 0)
	return;
    if (mySize + count >= myCapacity)
    {
	exint tidx = safeIndex(t);

        bumpCapacity(mySize + count);

	for (exint i = 0; i < count; i++)
	    copyConstruct(myData[mySize+i], tidx >= 0 ? myData[tidx] : t);
    }
    else
    {
	for (exint i = 0; i < count; i++)
	    copyConstruct(myData[mySize+i], t);
    }
    mySize += count;
}

template <typename T>
exint
UT_Array<T>::sortedInsert(const T &t, Comparator compare)
{
    exint     low, mid, high;

    low = 0;
    high = size() - 1;
    while (low <= high)
    {
        mid = (low + high) / 2;
        if (compare(&t, &myData[mid]) < 0)
            high = mid - 1;
        else if (compare(&t, &myData[mid]) > 0)
            low = mid + 1;
        else
        {
            insertAt(t, mid);
            return mid;
        }
    }
    insertAt(t, low);
    return low;
}

template <typename T>
template <typename ComparatorBool>
exint
UT_Array<T>::sortedInsert(const T &t, ComparatorBool is_less)
{
    exint     low, mid, high;

    low = 0;
    high = size() - 1;
    while (low <= high)
    {
        mid = (low + high) / 2;
        if (t == myData[mid])
        {
            insertAt(t, mid);
            return mid;
        }
	else if (is_less(t, myData[mid]))
            high = mid - 1;
        else
            low = mid + 1;
    }
    insertAt(t, low);
    return low;
}

template <typename T>
exint
UT_Array<T>::uniqueSortedInsert(const T &t, Comparator compare)
{
    exint     low, mid, high;

    low = 0;
    high = size() - 1;
    while (low <= high)
    {
        mid = (low + high) / 2;
        if (compare(&t, &myData[mid]) < 0)
            high = mid - 1;
        else if (compare(&t, &myData[mid]) > 0)
            low = mid + 1;
        else
            return (exint)mid;
    }
    insertAt(t, low);
    return low;
}

template <typename T>
template <typename ComparatorBool>
exint
UT_Array<T>::uniqueSortedInsert(const T &t, ComparatorBool is_less)
{
    exint     low, mid, high;

    low = 0;
    high = size() - 1;
    while (low <= high)
    {
        mid = (low + high) / 2;
        if (t == myData[mid])
            return (exint)mid;
	else if (is_less(t, myData[mid]))
            high = mid - 1;
        else
            low = mid + 1;
    }
    insertAt(t, low);
    return low;
}

template <typename T>
template <typename ComparatorBool>
exint
UT_Array<T>::uniqueSortedFind(const T &item, ComparatorBool is_less) const
{
    exint	len = size();
    exint	idx = 0;

    while(len > 0)
    {
	exint	h = len / 2;
	if (is_less(myData[idx + h], item))
	{
	    idx += h + 1;
	    len -= h + 1;
	}
	else
	    len = h;
    }

    return (idx != size() && !is_less(item, myData[idx])) ? idx : -1;
}

template <typename T>
exint
UT_Array<T>::uniqueSortedFind(const T &item, Comparator compare) const
{
    exint	len = size();
    exint	idx = 0;

    while(len > 0)
    {
	exint	h = len / 2;
	if (compare(&myData[idx + h], &item) < 0)
	{
	    idx += h + 1;
	    len -= h + 1;
	}
	else
	    len = h;
    }

    return (idx != size() && compare(&myData[idx], &item) == 0) ? idx : -1;
}

template <typename T>
exint
UT_Array<T>::heapPush(const T &t, Comparator compare)
{
    UT_ASSERT(safeIndex(t) < 0);
    exint i = append(t);

    // walk up towards the root restoring the heap condition
    while( i > 0 && compare(&myData[(i - 1) / 2], &t) < 0 )
    {
	myData[i] = myData[(i - 1) / 2];
	i = (i - 1) / 2;
    }
    myData[i] = t;
    return i;
}

template <typename T>
T
UT_Array<T>::heapPop(Comparator compare)
{
    UT_ASSERT_P(mySize > 0);

    T result = myData[0];
    // move last item into the newly created hole
    myData[0] = myData[mySize - 1];
    removeAt(mySize - 1);
    // percolate down until the heap condition is restored
    exint idx = 0;
    for(;;)
    {
        exint largest = idx;

        // test heap condition with left child
        exint cidx = 2 * idx + 1;
        if( cidx < mySize && compare(&myData[largest], &myData[cidx]) < 0 )
            largest = cidx;
        // test heap condition with right child
        ++cidx;
        if( cidx < mySize && compare(&myData[largest], &myData[cidx]) < 0 )
            largest = cidx;
        if( largest == idx )
        {
            // heap condition has been restored
            break;
        }

        UTswap(myData[idx], myData[largest]);
        idx = largest;
    }

    return result;
}

template <typename T>
exint
UT_Array<T>::concat(const UT_Array<T> &a)
{
    bumpCapacity(mySize + a.mySize);
    copyConstructRange(myData + mySize, a.myData, a.mySize);
    mySize += a.mySize;

    return mySize;
}

template <typename T>
exint
UT_Array<T>::multipleInsert(exint beg_index, exint count)
{
    exint end_index = beg_index + count;

    if (beg_index >= mySize)
    {
	bumpCapacity(end_index);

	trivialConstructRange(myData + mySize, end_index - mySize);

	mySize = end_index;
	return beg_index;
    }
    bumpCapacity(mySize+count);

    ::memmove((void *)&myData[end_index], (void *)&myData[beg_index],
              ((mySize-beg_index)*sizeof(T)));
    mySize += count;

    trivialConstructRange(myData + beg_index, count);

    return beg_index;
}

template <typename T>
template <typename S>
exint
UT_Array<T>::insertImpl(S &&s, exint index)
{
    if (index == mySize)
    {
        // This case avoids an extraneous call to trivialConstructRange()
        // which the compiler may not optimize out.
        (void) appendImpl(std::forward<S>(s));
    }
    else if (index > mySize)
    {
	exint src_i = safeIndex(s);

	bumpCapacity(index + 1);

	trivialConstructRange(myData + mySize, index - mySize);

        if (src_i >= 0)
	    construct(myData[index], std::forward<S>(myData[src_i]));
        else
	    construct(myData[index], std::forward<S>(s));

	mySize = index + 1;
    }
    else // (index < mySize)
    {
	exint src_i = safeIndex(s);

        bumpCapacity(mySize + 1);

        ::memmove((void *)&myData[index+1], (void *)&myData[index],
                  ((mySize-index)*sizeof(T)));

        if (src_i >= index)
            ++src_i;

        if (src_i >= 0)
	    construct(myData[index], std::forward<S>(myData[src_i]));
        else
	    construct(myData[index], std::forward<S>(s));

        ++mySize;
    }

    return index;
}

template <typename T>
exint
UT_Array<T>::findAndRemove(const T &t)
{
    exint	idx = find(t);
    return (idx < 0) ? -1 : removeAt((exint)idx);
}

template <typename T>
exint
UT_Array<T>::removeAt(exint idx)
{
    trivialDestruct(myData[idx]);
    if (idx != --mySize)
    {
	::memmove((void *)&myData[idx], (void *)&myData[idx+1],
	          ((mySize-idx)*sizeof(T)));
    }

    return idx;
}

template <typename T>
void
UT_Array<T>::removeRange(exint begin_i, exint end_i)
{
    UT_ASSERT(begin_i <= end_i);
    UT_ASSERT(end_i <= size());
    if (end_i < size())
    {
	trivialDestructRange(myData + begin_i, end_i - begin_i);
	::memmove((void *)&myData[begin_i], (void *)&myData[end_i],
	          (mySize - end_i)*sizeof(T));
    }
    setSize(mySize - (end_i - begin_i));
}

template <typename T>
void
UT_Array<T>::extractRange(exint begin_i, exint end_i, UT_Array<T>& dest)
{
    UT_ASSERT_P(begin_i >= 0);
    UT_ASSERT_P(begin_i <= end_i);
    UT_ASSERT_P(end_i <= size());
    UT_ASSERT(this != &dest);

    exint nelements = end_i - begin_i;

    // grow the raw array if necessary.
    dest.setCapacityIfNeeded(nelements);

    ::memmove((void*)dest.myData, (void*)&myData[begin_i],
              nelements * sizeof(T));
    dest.mySize = nelements;

    // we just asserted this was true, but just in case
    if (this != &dest)
    {
        if (end_i < size())
        {
            ::memmove((void*)&myData[begin_i], (void*)&myData[end_i],
                      (mySize - end_i) * sizeof(T));
        }
        setSize(mySize - nelements);
    }
}

template <typename T>
void
UT_Array<T>::move(exint srcIdx, exint destIdx, exint howMany)
{
    // Make sure all the parameters are valid.
    if( srcIdx < 0 )
	srcIdx = 0;
    if( destIdx < 0 )
	destIdx = 0;
    // If we are told to move a set of elements that would extend beyond the
    // end of the current array, trim the group.
    if( srcIdx + howMany > size() )
	howMany = size() - srcIdx;
    // If the destIdx would have us move the source beyond the end of the
    // current array, move the destIdx back.
    if( destIdx + howMany > size() )
	destIdx = size() - howMany;
    if( srcIdx != destIdx && howMany > 0 )
    {
	void		**tmp = 0;
	exint	  	savelen;

	savelen = SYSabs(srcIdx - destIdx);
	tmp = (void **)::malloc(savelen*sizeof(T));
	if( srcIdx > destIdx && howMany > 0 )
	{
	    // We're moving the group backwards. Save all the stuff that
	    // we would overwrite, plus everything beyond that to the
	    // start of the source group. Then move the source group, then
	    // tack the saved data onto the end of the moved group.
	    ::memcpy(tmp, (void *)&myData[destIdx],  (savelen*sizeof(T)));
	    ::memmove((void *)&myData[destIdx], (void *)&myData[srcIdx],
	              (howMany*sizeof(T)));
	    ::memcpy((void *)&myData[destIdx+howMany], tmp, (savelen*sizeof(T)));
	}
	if( srcIdx < destIdx && howMany > 0 )
	{
	    // We're moving the group forwards. Save from the end of the
	    // group being moved to the end of the where the destination
	    // group will end up. Then copy the source to the destination.
	    // Then move back up to the original source location and drop
	    // in our saved data.
	    ::memcpy(tmp, (void *)&myData[srcIdx+howMany],  (savelen*sizeof(T)));
	    ::memmove((void *)&myData[destIdx], (void *)&myData[srcIdx],
	              (howMany*sizeof(T)));
	    ::memcpy((void *)&myData[srcIdx], tmp, (savelen*sizeof(T)));
	}
	::free(tmp);
    }
}

template <typename T>
template <typename IsEqual>
exint
UT_Array<T>::removeIf(IsEqual is_equal)
{
    // Move dst to the first element to remove.
    exint dst;
    for (dst = 0; dst < mySize; dst++)
    {
	if (is_equal(myData[dst]))
	    break;
    }
    // Now start looking at all the elements past the first one to remove.
    for (exint idx = dst+1; idx < mySize; idx++)
    {
	if (!is_equal(myData[idx]))
	{
	    UT_ASSERT(idx != dst);
	    myData[dst] = myData[idx];
	    dst++;
	}
	// On match, ignore.
    }
    // New size
    mySize = dst;
    return mySize;
}

template <typename T>
void
UT_Array<T>::cycle(exint howMany)
{
    char	*tempPtr;
    exint	 numShift;	//  The number of items we shift
    exint   	 remaining;	//  mySize - numShift

    if (howMany == 0 || mySize < 1) return;

    numShift = howMany % (exint)mySize;
    if (numShift < 0) numShift += mySize;
    remaining = mySize - numShift;
    tempPtr = new char[numShift*sizeof(T)];

    ::memmove(tempPtr, (void *)&myData[remaining], (numShift * sizeof(T)));
    ::memmove((void *)&myData[numShift], (void *)&myData[0], (remaining * sizeof(T)));
    ::memmove((void *)&myData[0], tempPtr, (numShift * sizeof(T)));

    delete [] tempPtr;
}

template <typename T>
void
UT_Array<T>::constant(const T &value)
{
    for (exint i = 0; i < mySize; i++)
    {
	myData[i] = value;
    }
}

// This constant() specialization needs to be declared here and implemented
// in UT_Array.C.
template <>
UT_API void UT_Array<fpreal32>::constant(const fpreal32 &val);

template <typename T>
void
UT_Array<T>::zero()
{
    if (isPOD())
	::memset((void *)myData, 0, mySize*sizeof(T));
    else
	trivialConstructRange(myData, mySize);
}

template <typename T>
exint
UT_Array<T>::find(const T &t, exint s) const
{
    const T *end = myData + mySize;
    for (const T *p = myData + s; p < end; ++p)
        if (*p == t)
            return (p - myData);
    return -1;
}

template <typename T>
exint
UT_Array<T>::find(const T &t, Comparator compare) const
{
    T	*found;

    if( mySize == 0 ) return -1;

    found = (T *)::bsearch(&t, myData, mySize, sizeof(T),
			(ut_ptr_compare_func_t)compare);
    return found ? (found - myData) : -1;
}

template <typename T>
void
UT_Array<T>::reverse()
{
    exint n = mySize / 2;
    for (exint i = 0; i < n; i++ )
	UTswap(myData[i], myData[mySize-1-i]);
}

template <typename T>
void		
UT_Array<T>::sort(Comparator compare)
{
    stdsort(UTcompareLess(compare));
}

template <typename T>
template <typename ComparatorBool>
T
UT_Array<T>::selectNthLargest(exint idx, ComparatorBool is_less)
{
    // The idea of returning doesn't make sense if we have
    // an empty array.
    UT_ASSERT(size() > 0);
    if (size() == 0)
	return T();

    idx = SYSclamp(idx, (exint)0, (exint)(size())-1);

    UTnth_element(myData, &myData[idx], &myData[size()], is_less);

    return myData[idx];
}

template <typename T>
void		
UT_Array<T>::setCapacity(exint capacity)
{
    // Do nothing when new capacity is the same as the current
    if (capacity == myCapacity)
	return;

    // Special case for non-heap buffers
    if (!isHeapBuffer())
    {
	if (capacity < mySize)
	{
	    // Destroy the extra elements without changing myCapacity
	    trivialDestructRange(myData + capacity, mySize - capacity);
	    mySize = capacity;
	}
	else if (capacity > myCapacity)
	{
	    T *prev = myData;
	    myData = (T *)malloc(sizeof(T) * capacity);
	    // myData is safe because we're already a stack buffer
	    UT_ASSERT_P(isHeapBuffer());
	    if (mySize > 0)
		memcpy((void *)myData, (void *)prev, sizeof(T) * mySize);
	    myCapacity = capacity;
	}
	else 
	{
	    // Keep myCapacity unchanged in this case
	    UT_ASSERT_P(capacity >= mySize && capacity <= myCapacity);
	}
	return;
    }

    if (capacity == 0)
    {
	if (myData)
	{
	    trivialDestructRange(myData, mySize);
	    free(myData);
	}
	myData     = 0;
	myCapacity = 0;
        mySize = 0;
	return;
    }

    if (capacity < mySize)
    {
	trivialDestructRange(myData + capacity, mySize - capacity);
	mySize = capacity;
    }

    if (myData)
	myData = (T *)realloc(myData, capacity*sizeof(T));
    else
	myData = (T *)malloc(sizeof(T) * capacity);

    // Avoid degenerate case if we happen to be aliased the wrong way
    if (!isHeapBuffer())
    {
	T *prev = myData;
	myData = (T *)malloc(sizeof(T) * capacity);
	if (mySize > 0)
	    memcpy((void *)myData, (void *)prev, sizeof(T) * mySize);
	ut_ArrayImplFree(prev);
    }

    myCapacity = capacity;
    UT_ASSERT(myData);
}

template <typename T>
UT_Array<T> &
UT_Array<T>::operator=(const UT_Array<T> &a)
{
    if (this == &a)
	return *this;

    // Grow the raw array if necessary.
    setCapacityIfNeeded(a.size());

    // Make sure destructors and constructors are called on all elements
    // being removed/added.
    trivialDestructRange(myData, mySize);
    copyConstructRange(myData, a.myData, a.size());

    mySize = a.size();

    return *this;
}

template <typename T>
UT_Array<T> &
UT_Array<T>::operator=(std::initializer_list<T> a)
{
    const exint new_size = a.size();

    // Grow the raw array if necessary.
    setCapacityIfNeeded(new_size);

    // Make sure destructors and constructors are called on all elements
    // being removed/added.
    trivialDestructRange(myData, mySize);

    copyConstructRange(myData, a.begin(), new_size);

    mySize = new_size;

    return *this;
}

template <typename T>
UT_Array<T> &
UT_Array<T>::operator=(UT_Array<T> &&a)
{
    if (!a.isHeapBuffer())
    {
	// Cannot steal from non-heap buffers
	clear();
	const exint n = a.size();
	setCapacityIfNeeded(n);
	if (isPOD())
	{
	    if (n > 0)
		memcpy(myData, a.myData, n * sizeof(T));
	}
	else
	{
	    for (exint i = 0; i < n; ++i)
		new (&myData[i]) T(std::move(a.myData[i]));
	}
	mySize = a.mySize;
	a.mySize = 0;
	return *this;
    }
    // else, just steal even if we're a small buffer

    // Destroy all the elements we're currently holding.
    if (myData)
    {
	trivialDestructRange(myData, mySize);
	if (isHeapBuffer())
	    ::free(myData);
    }
    
    // Move the contents of the other array to us and empty the other container
    // so that it destructs cleanly.
    myCapacity = a.myCapacity;
    mySize = a.mySize;
    myData = a.myData;
    a.myCapacity = a.mySize = 0;
    a.myData = nullptr;

    return *this;
}


template <typename T>
bool
UT_Array<T>::operator==(const UT_Array<T> &a) const
{
    if (this == &a) return true;
    if (mySize != a.size()) return false;
    for (exint i = 0; i < mySize; i++)
	if (!(myData[i] == a(i))) return false;
    return true;
}

template <typename T>
bool
UT_Array<T>::operator!=(const UT_Array<T> &a) const
{
    return (!operator==(a));
}

template <typename T>
int
UT_Array<T>::isEqual(const UT_Array<T> &a, Comparator compare) const
{
    if (this == &a) return 1;
    if (mySize != a.size()) return 0;
    for (exint i = 0; i < mySize; i++)
    {
	T	v2 = a(i);
	if (compare(&myData[i], &v2)) return 0;
    }
    return 1;
}

template <typename T>
exint	
UT_Array<T>::apply(int (*applyFct)(T &t, void *d), void *d)
{
    exint i;
    for (i = 0; i < mySize; i++)
	if (applyFct(myData[i], d)) break;
    return i;
}

// Merge the given array into us.
// If direction is -1, then it assumes us and 'other' are both already
// sorted in descending order. Similarly, +1 means ascending.
// If allow_dups is false, then it further assumes that both arrays have no
// duplicates and will produce a result that also has no duplicates.
// More work will be needed if you want allow_dups to mean remove duplicates
template <typename T>
template <typename ComparatorBool>
void
UT_Array<T>::merge(
	const UT_Array<T> &other, int direction, bool allow_dups,
	ComparatorBool is_less)
{
    UT_Array<T>	result;
    exint		our_idx;
    exint		other_idx;

    // handle trivial cases to avoid extra work
    if (other.size() == 0)
	return;
    if (size() == 0)
    {
	concat(other);
	return;
    }

    UT_ASSERT( direction == -1 || direction == +1 );
    direction = (direction > 0) ? +1 : -1;

    our_idx = 0;
    other_idx = 0;
    while( our_idx < size() && other_idx < other.size() )
    {
	const T	&our_item = (*this)(our_idx);
	const T	&other_item = other(other_idx);
	exint	 item_dir;

	if (our_item == other_item)
	    item_dir = 0;
	else if (is_less(our_item, other_item))
	    item_dir = -1;
	else
	    item_dir = +1;

	if( item_dir != 0 )
	{
	    // we need to do an comparison in the next line to take care of the
	    // fact that -INT_MIN is still less than 0.
	    item_dir = ( (item_dir > 0) ? +1 : -1 ) * direction;
	}

	if( item_dir < 0 )
	{
	    result.append( our_item );
	    our_idx++;
	}
	else if( item_dir > 0 )
	{
	    result.append( other_item );
	    other_idx++;
	}
	else
	{
	    result.append( our_item );
	    our_idx++;
	    if( allow_dups )
		result.append( other_item );
	    other_idx++;
	}
    }

    UT_ASSERT( our_idx == size() || other_idx == other.size() );
    for( ; our_idx < size(); our_idx++ )
	result.append( (*this)(our_idx) );
    for( ; other_idx < other.size(); other_idx++ )
	result.append( other(other_idx) );

    // finally swap the result into us
    swap( result );
}

template <typename T>
bool
UT_Array<T>::hasSortedSubset(
	const UT_Array<T> &other,
	Comparator compare) const
{
    return std::includes(
		myData, myData + mySize,
		other.myData, other.myData + other.mySize,
		UTcompareLess(compare));
}

template <typename T>
void
UT_Array<T>::sortedUnion(const UT_Array<T> &other, Comparator compare)
{
    UT_Array<T>	    temp;
    sortedUnion( other, temp, compare );
    swap( temp );
}

template <typename T>
void
UT_Array<T>::sortedUnion(const UT_Array<T> &other,
			    UT_Array<T> &result, Comparator compare) const
{
    T	    *first1 = myData;
    T	    *last1 = myData + mySize;
    T	    *first2 = other.myData;
    T	    *last2 = other.myData + other.mySize;

    // Can't store to either input.
    UT_ASSERT(&result != this && &result != &other);
    UT_ASSERT(result.size() == 0);

    while( first1 != last1 && first2 != last2 )
    {
	if( compare(first1, first2) < 0 )
	{
	    result.append( *first1 );
	    ++first1;
	}
	else if( compare(first2, first1) < 0 )
	{
	    result.append( *first2 );
	    ++first2;
	}
	else
	{
	    result.append( *first1 );
	    ++first1;
	    ++first2;
	}
    }
    while( first1 != last1 )
    {
	result.append( *first1 );
	++first1;
    }
    while( first2 != last2 )
    {
	result.append( *first2 );
	++first2;
    }
}

template <typename T>
void
UT_Array<T>::sortedIntersection(const UT_Array<T> &other, Comparator compare)
{
    UT_Array<T>	    temp;
    sortedIntersection( other, temp, compare );
    swap( temp );
}

template <typename T>
void
UT_Array<T>::sortedIntersection( const UT_Array<T> &other,
	UT_Array<T> &result, Comparator compare ) const
{
    T		*first1 = myData;
    T		*last1 = myData + mySize;
    T		*first2 = other.myData;
    T		*last2 = other.myData + other.mySize;

    while (first1 != last1 && first2 != last2) 
	if( compare(first1, first2) < 0 )
	    ++first1;
	else if( compare(first2, first1) < 0 )
	    ++first2;
	else {
	    result.append( *first1 );
	    ++first1;
	    ++first2;
	}
}

template <typename T>
void
UT_Array<T>::sortedSetDifference(const UT_Array<T> &other, Comparator compare)
{
    UT_Array<T>	    temp;
    sortedSetDifference(other, temp, compare);
    swap( temp );
}

template <typename T>
void
UT_Array<T>::sortedSetDifference( const UT_Array<T> &other,
			    UT_Array<T> &result,
			    Comparator compare) const
{
    T		*first1 = myData;
    T		*last1 = myData + mySize;
    T		*first2 = other.myData;
    T		*last2 = other.myData + other.mySize;

    while (first1 != last1 && first2 != last2)
    {
	if( compare(first1, first2) < 0 )
	{
	    result.append( *first1 );
	    ++first1;
	}
	else if( compare(first2, first1) < 0 )
	    ++first2;
	else
	{
	    ++first1;
	    ++first2;
	}
    }
    while( first1 != last1 )
    {
	result.append( *first1 );
	++first1;
    }
}

template <typename T>
template <typename CompareEqual>
exint
UT_Array<T>::sortedRemoveDuplicatesIf(CompareEqual compare_equal)
{
    exint n = size();

    // Trivial, no duplicates!
    if (n < 2)
	return 0;

    exint dst = 1;
    for (exint i = 1; i < n; i++)
    {
	if (!compare_equal((*this)(i), (*this)(i-1)))
	{
	    if (i != dst)
		(*this)(dst) = (*this)(i);
	    dst++;
	}
    }

    // Store the number of remaining elements.
    setSize(dst);
    return n - dst;	// Return the number of elements removed
}

namespace {
    template<typename T>
    struct srdCompareEqual
    {
	bool operator()(const T& x, const T& y) const { return (x == y); }
    };
}

template <typename T>
exint
UT_Array<T>::sortedRemoveDuplicates()
{
    srdCompareEqual<T> cmp;
    return sortedRemoveDuplicatesIf(cmp);
}

template <typename T>
template <typename BinaryOp>
T
UT_Array<T>::accumulate(const T &init_value, BinaryOp add) const
{
    T sum(init_value);
    for (exint i = 0; i < mySize; i++)
	sum = add(sum, myData[i]);
    return sum;
}

#endif // __UT_ARRAYIMPL_H_INCLUDED__

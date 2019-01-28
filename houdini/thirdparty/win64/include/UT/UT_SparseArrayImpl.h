/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT library (C++)
 *
 * COMMENTS:	Mantains a sparse array of values.
 *
 */

#ifndef __UT_SparseArrayImpl__
#define __UT_SparseArrayImpl__

#ifndef __UT_SparseArray_h__
#error Do not include this file directly. Include UT_SparesArray.h instead.
#endif

template <typename T>
UT_SparseArray<T>::UT_SparseArray()
{
}

template <typename T>
UT_SparseArray<T>::~UT_SparseArray()
{
    clear();
}

template <typename T>
void
UT_SparseArray<T>::append(int index, T data)
{
    int		i;

    // TODO:  Optimize by using bisection to find where it needs to be located
    for (i = myArray.entries()-1; i >= 0; i--)
    {
	if (myArray(i)->myIndex == index)
	{
	    myArray(i)->myData = data;
	    return;
	}
	if (myArray(i)->myIndex < index) break;
    }
    if (i == (int)(myArray.entries()-1))
	myArray.append(new SparseEntry(index, data));
    else
    {
	i++;
	myArray.insert(new SparseEntry(index, data), i);
    }
}

template <typename T>
void
UT_SparseArray<T>::removeIndex(int index)
{
    int		iidx;		// Internal index
    if ((iidx = find(index)) >= 0)
    {
	delete myArray(iidx);
	myArray.removeIndex(iidx);
    }
}

template <typename T>
void
UT_SparseArray<T>::clear()
{
    for (exint i = 0; i < myArray.entries(); ++i)
	delete myArray(i);
    myArray.setCapacity(0);
}

template <typename T>
T
UT_SparseArray<T>::getRawEntry(int raw_index, int &index)
{
    index = -1;
    if (raw_index >= 0 && raw_index < myArray.entries())
    {
	index = myArray(raw_index)->myIndex;
	return myArray(raw_index)->myData;
    }
    return T(0);
}

template <typename T>
T
UT_SparseArray<T>::operator()(unsigned int index)
{
    int		iidx;
    return ((iidx = find(index)) >= 0) ? myArray(iidx)->myData : T(0);
}

template <typename T>
const T
UT_SparseArray<T>::operator()(unsigned int index) const
{
    int		iidx;
    return ((iidx = find(index)) >= 0) ? myArray(iidx)->myData : T(0);
}

//
//  Use bisection to find the value if it exists
template <typename T>
int
UT_SparseArray<T>::find(int index) const
{
    int		low, high, mid, val;

    high = myArray.entries()-1;
    if (high < 1)
    {
	return (high == 0 && myArray(0)->myIndex == index) ? 0 : -1;
    }
    low = 0;

    do {
	mid = (high+low) >> 1;
	val = myArray(mid)->myIndex;
	if (val == index) return mid;
	if (val < index) low = mid;
	else		 high = mid;
    } while (high - low > 1);
    if (myArray(high)->myIndex == index) return high;
    if (myArray(low)->myIndex == index) return low;
    return -1;		// Not found
}

#endif // __UT_SparseArrayImpl__

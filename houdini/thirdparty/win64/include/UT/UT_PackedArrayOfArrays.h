/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:        UT_PackedArrayOfArrays.h (UT Library, C++)
 *
 * COMMENTS:    An array of arrays that is more efficiently
 *		laid out than the UT_Array<UT_Array<T> >
 *		Only allows appending, however.
 */

#pragma once

#ifndef __UT_PACKED_ARRAY_OF_ARRAYS_H__
#define __UT_PACKED_ARRAY_OF_ARRAYS_H__

#include "UT_API.h"

#include "UT_Array.h"
#include "UT_IteratorRange.h"

template <typename T>
class UT_API_TMPL UT_PackedArrayOfArrays
{
public:
    typedef UT_Array<T> value_type;

    UT_PackedArrayOfArrays()
    {
    }

    void	clear()
		{ myOffsets.clear(); myData.clear(); }
    bool	isEmpty() const
		{ return size() == 0; }

    /// Returns the number of arrays in this structure.
    exint	size() const { return SYSmax(myOffsets.size() - 1, 0); }
    exint	entries() const { return size(); }

    int64       getMemoryUsage(bool inclusive = false) const
                {
                    return (inclusive ? sizeof(*this) : 0) +
                           myOffsets.getMemoryUsage(false) +
                           myData.getMemoryUsage(false);
                }

    /// Adds a new array to the end of our list of arrays.
    void	append(const UT_Array<T> &arr)
		{ append(arr.array(), arr.entries()); }
    void	append(const T *data, exint len)
		{
		    if (myOffsets.isEmpty())
			myOffsets.append(0);
		    for (exint i = 0; i < len; i++)
			myData.append(data[i]);
		    myOffsets.append(myData.entries());
		}
    void	append(const UT_PackedArrayOfArrays<T> &arr, exint idx)
		{ append(arr.arrayData(idx), arr.arrayLen(idx)); }

    /// Appends an array of arrays to our list.
    void	concat(const UT_PackedArrayOfArrays<T> &arr)
		{ 
		    for (exint i = 0; i < arr.entries(); i++)
			append(arr, i);
		}

    /// Appends a new array and returns a pointer to the resulting
    /// data so it can be filled in.
    T		*appendArray(exint len)
		{
		    if (myOffsets.isEmpty())
			myOffsets.append(0);
		    exint base = myOffsets.last();
		    myOffsets.append(base+len);
		    myData.bumpSize(base+len);
		    return myData.array() + base;
		}

    /// From the given idxth array, return the idx_in_array element
    const T	&operator()(exint idx, exint idx_in_array) const
		{
		    UT_ASSERT_P(idx >= 0 && idx < size());
		    exint	base = myOffsets(idx);
		    UT_ASSERT_P(base+idx_in_array < myOffsets(idx+1));
		    return myData(base+idx_in_array);
		}

    /// Extracts an array into a UT_Array.
    void	extract(UT_Array<T> &result, exint idx) const
		{
		    UT_ASSERT_P(idx >= 0 && idx < size());
		    exint	base = myOffsets(idx);
		    exint	len = myOffsets(idx+1)-base;
		    result.entries(len);
		    for (exint i = 0; i < len; i++)
			result(i) = myData(base+i);
		}

    T		*arrayData(exint idx)
		{
		    // This is less than equal to as it is valid to
		    // point to the final element if the corresponding
		    // array is empty!
		    UT_ASSERT_P(myOffsets(idx) <= myData.size());
		    return myData.array() + myOffsets(idx);
		}
    const T	*arrayData(exint idx) const
		{
		    UT_ASSERT_P(myOffsets(idx) <= myData.size());
		    return myData.array() + myOffsets(idx);
		}
    exint	arrayLen(exint idx) const
		{
		    return myOffsets(idx+1)-myOffsets(idx);
		}

    /// @{
    /// Returns an iterator range for the specified array.
    UT_IteratorRange<T *> arrayRange(exint idx)
                { return UTmakeRange(arrayData(idx), arrayData(idx + 1)); }
    UT_IteratorRange<const T *> arrayRange(exint idx) const
                { return UTmakeRange(arrayData(idx), arrayData(idx + 1)); }
    /// @}

    /// Decreases, but never expands, to the given maxsize.
    void            truncate(exint maxsize)
    {
        if (maxsize >= 0 && size() > maxsize)
	{
	    if (maxsize == 0)
		clear();
	    else
	    {
		myOffsets.setSize(maxsize+1);
		myData.setSize(myOffsets(maxsize));
	    }
	}
    }

    /// These adjust the capacity without adjusting the entries
    /// and are useful for pre-allocing the amount when known
    /// in advance.
    void	setDataCapacity(exint capacity)
		{ myData.setCapacity(capacity); }
    void	setDataCapacityIfNeeded(exint capacity)
		{ myData.setCapacityIfNeeded(capacity); }
    void	setArrayCapacity(exint capacity)
		{ myOffsets.setCapacity(capacity); }
    void	setArrayCapacityIfNeeded(exint capacity)
		{ myOffsets.setCapacityIfNeeded(capacity); }
    
    /// The raw offset table stores the end of each array
    /// in the data array.  
    /// Each array has [rawOffsets(idx) ... rawOffsets(idx+1))
    UT_Array<exint>		&rawOffsets() { return myOffsets; }
    const UT_Array<exint>	&rawOffsets() const { return myOffsets; }

    /// All of the array data is in one contiguous block.
    UT_Array<T>			&rawData() { return myData; }
    const UT_Array<T>		&rawData() const { return myData; }
    
protected:
    UT_Array<exint>	myOffsets;
    UT_Array<T>		myData;
};

#endif


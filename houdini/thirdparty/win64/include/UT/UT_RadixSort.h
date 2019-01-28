/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Utility Library (C++)
 *
 * COMMENTS:
 *	This is a class which implements a radix sort, but for 4 byte
 *	integer and (IEEE) floating point numbers.  The sort algorithms
 *	take endianness into consideration so the user does not need to
 *	worry about it.
 *
 *	This class is based off of the comments on Pierre Terdiman's
 *	web page "Radix Sort Revisted" as well as the provided source
 *	code.
 *
 *	http://www.codercorner.com/RadixSortRevisited.htm
 *
 *	Original code:
 *		Source code for "Radix Sort Revisited"
 *		(C) 2000, Pierre Terdiman (p.terdiman@wanadoo.fr)
 */

#ifndef __UT_RadixSort_h__
#define __UT_RadixSort_h__

#include "UT_API.h"
#include <SYS/SYS_Types.h>

class UT_API UT_RadixSort
{
public:
    UT_RadixSort();
    ~UT_RadixSort();

    // Sorting methods
    //     After sorting, use the getIndices() method to get the
    //     list of indices in sorted order.
    // NB: The input array is NOT sorted by this routine,
    // you must use the index array to figure out where
    // each element belongs.
    UT_RadixSort&	sort(uint *input, uint nb, 
			     bool signedvalues=true);
    UT_RadixSort&	sort(float *input, uint nb);

    // Return a pointer to the list of indices in sorted order.
    // This list is internally stored in myIndices.
    unsigned int	*getIndices()	{ return myIndices; }

    // Reset the internal indices.  Note that the list of indices
    // in sorted order is kept internally and is checked at the
    // beginning of the sort routine to check if the list is
    // already sorted.
    UT_RadixSort	&resetIndices();

    // Return the amount of ram used by this particular object.
    uint		getUsedRam();
private:
    void		resizeIndices(unsigned int new_size);

    uint		*myHistogram;	// Counters for each byte
    uint		*myOffset;	// Offsets (nearly a cumulative 
					// distribution function)

    uint		myCurrentSize;	// Current size of the indices list

    uint		*myIndices;	// Two lists, swapped each pass
    uint		*myIndices2;
};

#endif // __UT_RadixSort_h__

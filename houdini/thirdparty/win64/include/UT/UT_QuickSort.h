/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 */

#ifndef __UT_QuickSort__
#define __UT_QuickSort__

#include "UT_API.h"
#include <stddef.h>		// For definition of size_t
#include "UT_IntArray.h"

#if 0
typedef int UTcompareFunc(void * /*userdata*/, const void *, const void *);
#endif

/// This is a bare bones implementation of the Quick Sort,
/// so those who don't believe in Radix sort have something to use.
class UT_API UT_QuickSort
{
public:
    UT_QuickSort();
    virtual ~UT_QuickSort();

    /// Sorting methods
    ///     After sorting, use the getIndices() method to get the
    ///     list of indices in sorted order.
    /// NB: Unlike the UT_RadixSort, the input array IS sorted
    /// with this quick sort.
    // @{
    UT_QuickSort&	sort(unsigned int *input, unsigned int nb,
			     int useindex = 1,
			     bool signedvalues=true);
    UT_QuickSort&	sort(fpreal32 *input, unsigned int nb,
			     int useindex = 1);
    UT_QuickSort&	sort(fpreal64 *input, unsigned int nb,
			     int useindex = 1);
    // @}

    enum compareType {
	COMPARE_INT,
	COMPARE_UINT,
	COMPARE_FLOAT
    };

    /// The following sort method is used to sort arbitrary structures.
    /// The first element of the structure must be an int, unsigned, or
    /// a float depending on the compareType chosen.
    UT_QuickSort&	sort(void *input, size_t size, unsigned int nb,
			     compareType type);

#if 0
    /// The following sort method is used to sort arbitrary structures.
    static void		sort(void *input, size_t size, unsigned int nb,
			     void *userdata, UTcompareFunc *cmp);
#endif

    /// Return a pointer to the list of indices in sorted order.
    /// This list is internally stored in myIndices.
    int*		getIndices()	{ return &myIndices(0); }

    int			operator()(int idx) { return myIndices((unsigned)idx); }

    /// Reset the internal indices.  Note that the list of indices
    /// in sorted order is kept internally and is checked at the
    /// beginning of the sort routine to check if the list is
    /// already sorted.
    UT_QuickSort&	resetIndices() { myIndices.setCapacity(0); return *this; }

    /// Return the amount of ram used by this particular object.
    int64		getUsedRam() const 
			{ return myIndices.getMemoryUsage(); }
protected:
    UT_IntArray		myIndices;

    /// These are the unsigned, signed, and float sort routines.
    /// The ones without an index variable don't sort an index array
    /// along with the main array.
    // @{
    static void		uisort(unsigned int *input, int *index, int len);
    static void		sisort(int *input, int *index, int len);
    static void		fisort(fpreal32 *input, int *index, int len);
    static void		fisort(fpreal32 *input, int len);
    static void		disort(fpreal64 *input, int *index, int len);
    static void		disort(fpreal64 *input, int len);
    static void		sisort(int *input, int len);
    static void		uisort(unsigned int *input, int len);
    static void		disort(void *input, size_t size, int len, char *tinput);
    static void		fisort(void *input, size_t size, int len, char *tinput);
    static void		sisort(void *input, size_t size, int len, char *tinput);
    static void		uisort(void *input, size_t size, int len, char *tinput);
    // @}

    static void		urecurse(unsigned int *input, int *index, int len);
    static void		srecurse(int *input, int *index, int len);
    static void		frecurse(fpreal32 *input, int *index, int len);
    static void		drecurse(fpreal64 *input, int *index, int len);
    
    static void		frecurse(fpreal32 *input, int len);
    static void		drecurse(fpreal64 *input, int len);
    static void		srecurse(int *input, int len);
    static void		urecurse(unsigned int *input, int len);

    static void		drecurse(void *input, size_t size, int len, 
	                         char *tinput);
    static void		frecurse(void *input, size_t size, int len, 
	                         char *tinput);
    static void		srecurse(void *input, size_t size, int len, 
				 char *tinput);
    static void		urecurse(void *input, size_t size, int len,
				 char *tinput);
};

#endif

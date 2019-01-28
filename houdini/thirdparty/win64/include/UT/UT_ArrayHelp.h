/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_ArrayHelp.h ( UT Library, C++)
 *
 * COMMENTS:
 *	Provides simple helper functions for the UT_*Array and UT_Symbol/
 *	HashTable classes.
 *
 *	Should be private to the UT library, but needs to be exported
 *	so NT style template instantiation will work.
 */

#ifndef __UT_ArrayHelp__
#define __UT_ArrayHelp__

#include "UT_API.h"

#include <SYS/SYS_Types.h>
#include <SYS/SYS_BitUtil.h>
typedef int (*ut_ptr_compare_func_t)(const void *, const void *);

// NOTE: These must be powers of two.  See UTbumpAlloc below.
#define SMALL_ALLOC	16
#define BIG_ALLOC	128

// Computes the size of a constant-sized array (e.g. int foo[10]). 
// Only use when a constant expression is needed. Otherwise, use the templated
// version below for type safety.
#define UT_ARRAYSIZE(A) (sizeof(A)/sizeof(A[0]))

// Better than the UT_ARRAYSIZE() macro, since it validates that
// the argument is actually an array (and so you can't pass a pointer).
// Can't be used as a constant expression in MSVC2008, however.
template<typename A, exint N>
exint UTarraySize(A(&)[N]) { return N; }


/// This routine describes how to change the size of an array.
/// It must increase the current_size by at least one!
///
/// Current expected sequence of small sizes:
///    4,    8,   16,   32,   48,   64,   80,   96,  112,
///  128,  256,  384,  512,  640,  768,  896, 1024,
/// (increases by approx factor of 1.125 each time after this)
template <typename T>
static inline T
UTbumpAlloc(T current_size)
{
    // For small values, we increment by fixed amounts.  For
    // large values, we increment by one eighth of the current size.
    // This prevents n^2 behaviour with allocation one element at a time.
    // A factor of 1/8 will waste 1/16 the memory on average, and will
    // double the size of the array in approximately 6 reallocations.
    if (current_size < T(8))
    {
        return (current_size < T(4)) ? T(4) : T(8);
    }
    if (current_size < T(BIG_ALLOC))
    {
        // Snap up to next multiple of SMALL_ALLOC (must be power of 2)
        return (current_size + T(SMALL_ALLOC)) & ~T(SMALL_ALLOC-1);
    }
    if (current_size < T(BIG_ALLOC * 8))
    {
        // Snap up to next multiple of BIG_ALLOC (must be power of 2)
        return (current_size + T(BIG_ALLOC)) & ~T(BIG_ALLOC-1);
    }

    T bump = current_size >> 3; // Divided by 8.
    current_size += bump;
    return current_size;
}

// This function is used by the symbol tables to bump to the next/previous
// prime size.
UT_API exint UTbumpAllocToPrime(exint current_size);
UT_API exint UTdecreaseAllocToPrime(exint current_size);

#endif


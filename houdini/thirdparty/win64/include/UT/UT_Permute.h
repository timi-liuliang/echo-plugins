/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Permute.h ( UT Library, C++)
 *
 * COMMENTS:	
 */

#ifndef __UT_Permute__
#define __UT_Permute__

#include "UT_API.h"
#include "UT_Swap.h"
#include "UT_MTwister.h"
#include "UT_StackBuffer.h"

#include <tbb/parallel_invoke.h>

#include <algorithm>
#include <iterator>

#include <string.h>


/// This is a custom implementation of std::partition,
/// just so that we don't keep having issues of different
/// platforms giving different results.
///
/// This implementation requires bidirectional iterators.
template<typename IT, typename PRED>
IT UTpartition(IT start, IT end, PRED isbeforesplit)
{
    while (true)
    {
        if (start == end)
            return start;

        // Skip elements at the start that belong before the split.
        while (isbeforesplit(*start))
        {
            ++start;
            if (start == end)
                return start;
        }

        // Skip elements at the end that belong after the split.
        do
        {
            --end;
            if (start == end)
                return start;
        } while(!isbeforesplit(*end));

        // start points to something that goes after the split,
        // and end points to something that goes before the split.
        UTswap(*start, *end);
        ++start;
    }
}

/// This is a custom implementation of std::nth_element,
/// just so that we don't keep having issues of different
/// platforms giving different results.
///
/// (Having a comparator that forced strict ordering wasn't
/// sufficient for std::nth_element, because that only guarantees
/// that nth is greater than everything before it and less than
/// everything after it.  We'd have to then sort the two sections
/// with the comparator have consistency.)
///
/// This implementation requires a comparator.
template<typename IT, typename COMPARE>
void UTnth_element(IT start, IT nth, IT end, COMPARE isAbeforeB)
{
    if (start == end)
        return;

    while (start+2 < end)
    {
        // We have at least 3 elements

        // We could do a more formal introselect that falls back
        // to the proper median of medians algorithm that guarantees
        // O(n) time, but for now, let's just pick a few arbitrary
        // points and pick the median as the pivot.

        auto diff = (end-start);
        auto hdiff = (diff>>1);
        const IT mid = (start+hdiff);
        const IT last = end-1;

        // This is set up so that if they're equal,
        // pivot will be mid.
        typename std::iterator_traits<IT>::value_type pivot;
        if (isAbeforeB(*last, *start))
        {
            if (isAbeforeB(*mid, *start))
            {
                if (isAbeforeB(*last, *mid))
                    pivot = *mid;    // LMS
                else
                    pivot = *last;   // MLS
            }
            else
                pivot = *start;      // LSM
        }
        else
        {
            if (isAbeforeB(*mid, *start))
                pivot = *start;      // MSL
            else
            {
                if (isAbeforeB(*last, *mid))
                    pivot = *last;   // SLM
                else
                    pivot = *mid;    // SML
            }
        }

        // This partition moves everything belonging before the pivot
        // to the beginning, i.e.:
        // [(elements < pivot) (elements >= pivot)]
        IT split = UTpartition(start, end,
            [&pivot,&isAbeforeB](const typename std::iterator_traits<IT>::value_type &v)
            { return isAbeforeB(v, pivot); }
        );
        // Since there may be many duplicates and pivot could be
        // the smallest element, we must move all elements that equal
        // pivot (i.e. (v <= pivot) && (v >= pivot), the latter of which
        // is dealt with above) to the beginning of the split.
        // If we don't do this, we may make no progress on some iteration
        // and loop forever.
        // [(elements < pivot) (elements == pivot) (elements > pivot)]
        IT split2 = UTpartition(split, end,
            [&pivot,&isAbeforeB](const typename std::iterator_traits<IT>::value_type &v)
        { return !isAbeforeB(pivot, v); } // !(pivot < v) <--> (pivot >= v) <--> (v <= pivot)
        );

        // Just "recurse" on the section containing nth.
        if (nth < split)
            end = split;
        else if (nth < split2)
            return; // Between split and split2, everything is equal to pivot.
        else
            start = split2;
    }

    if (start+1 == end)
        return;

    // 2 elements left
    if (isAbeforeB(*(end-1), *start))
        UTswap(*start, *(end-1));
}

/// This is a custom implementation of std::nth_element,
/// just so that we don't keep having issues of different
/// platforms giving different results.
/// This implementation uses std::less.
template<typename IT>
void UTnth_element(IT start, IT nth, IT end)
{
    UTnth_element(start, nth, end, std::less<typename std::iterator_traits<IT>::value_type>());
}

namespace UT_Permute {

template <typename T, typename INT>
class Partition {
public:
    Partition(T *low, const INT *permute, INT mid)
	: myLow(low)
	, myPermute(permute)
	, myMid(mid) {}
    bool operator()(const T &rhs) const
    {
	return myPermute[&rhs-myLow] < myMid;
    }
private:
    T		*myLow;
    const INT	*myPermute;
    INT		 myMid;
};

template<typename INT>
class PartitionPermute {
public:
    PartitionPermute(INT mid) : myMid(mid) {}
    bool operator()(INT rhs) const
    {
	return rhs < myMid;
    }
private:
    INT myMid;
};

static const int theCrossover = 1024;

template <typename T, typename INT>
static void
permuteInPlaceRHelper(T *vals, INT *permute, INT low, INT high)
{
    INT size = high-low;
    T tmp[theCrossover];
    for (INT i = low; i < high; i++)
	tmp[permute[i]-low] = vals[i];
    memcpy(vals + low, tmp, size*sizeof(T));
}

// Permute by parallel recursive partitioning
template <typename T, typename INT>
static void
permuteInPlaceR(T *vals, INT *permute, INT low, INT high)
{
    INT size = high-low;
    if (size < theCrossover)
    {
	permuteInPlaceRHelper(vals, permute, low, high);
	return;
    }

    INT mid = (low + high) / 2;

    UTpartition(vals + low,
		vals + high,
		Partition<T,INT>(vals + low, permute + low, mid));
    UTpartition(permute + low,
		permute + high,
		PartitionPermute<INT>(mid));

    tbb::parallel_invoke(
        [=]() { permuteInPlaceR(vals, permute, low, mid); },
        [=]() { permuteInPlaceR(vals, permute, mid, high); }
    );
}

} // UT_Permute

// This method will reorder vals using the given permutation int array
// which must contain at least size elements.  T[i] is moved to
// T[permute[i]] for all valid indices i. The permute array is modified by
// this method.
template <typename T, typename INT>
static inline void
UTpermuteInPlace(T *vals, INT *permute, INT size)
{
    UT_Permute::permuteInPlaceR(vals, permute, INT(0), size);
}

// Makes a temporary copy of permute before calling UTpermuteInPlace.
template <typename T, typename INT>
static inline void
UTpermute(T *vals, const INT *permute, INT size)
{
    UT_StackBuffer<INT> tmp_permute(size);
    memcpy(tmp_permute, permute, size*sizeof(INT));

    UT_Permute::permuteInPlaceR(vals, tmp_permute.array(), INT(0), size);
}

// This method will reorder vals using the given permutation int array
// which must contain at least size elements.  T[permute[i]] is moved to
// T[i] for all valid indices i.
template <typename T, typename INT>
static inline void
UTinversePermute(T *vals, const INT *permute, INT size)
{
    UT_StackBuffer<INT> tmp_permute(size);
    for (INT i = 0; i < size; i++)
	tmp_permute[permute[i]] = i;

    UT_Permute::permuteInPlaceR(vals, tmp_permute.array(), INT(0), size);
}

template <typename INT>
static inline void
UTfillRandomPermutation(UT_MersenneTwister &twist, INT *permute, INT size)
{
    for (int i = 0; i < size; i++)
	permute[i] = i;

    for (int i = size; i-- > 1; )
    {
	int k = twist.urandom() % (i+1);
	if (k != i)
	    std::swap(permute[k], permute[i]);
    }
}

#endif

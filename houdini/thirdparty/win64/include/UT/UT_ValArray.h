/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	Utility Library (C++)
 *
 * COMMENTS:
 *	This is a class that is used by UT_ValArray to do all the work...
 *
 */

#ifndef __UT_VALARRAY_H_INCLUDED__
#define __UT_VALARRAY_H_INCLUDED__

#include "UT_API.h"
#include "UT_Array.h"
#include "UT_Assert.h"
#include "UT_Swap.h"
#include "UT_VectorTypes.h"

#include <SYS/SYS_Compiler.h>
#include <SYS/SYS_Deprecated.h>
#include <SYS/SYS_Types.h>

#include <initializer_list>
#include <stdio.h>
#include <string.h>


UT_API extern void UTsetCompareFloatsTolerance(float tol);
UT_API extern float UTgetCompareFloatsTolerance();
UT_API extern int UTcompareFloats(const float *a, const float *b);
UT_API extern int UTcompareInts(const int *a, const int *b);
UT_API extern int UTcomparePointers(void *const* a, void *const* b);


SYS_DEPRECATED_PUSH_DISABLE()
template <typename T>
class UT_API_TMPL UT_ValArray : public UT_Array<T>
{
public:
    typedef int (*Comparator)(const T *, const T *);

    /// Copy constructor
    /// Copy constructor. It duplicates the data.
    /// It's marked explicit so that it's not accidentally passed by value.
    /// You can always pass by reference and then copy it, if needed.
    /// If you have a line like:
    /// UT_ValArray<int> a = otherarray;
    /// and it really does need to copy instead of referencing,
    /// you can rewrite it as:
    /// UT_ValArray<int> a(otherarray);
    explicit UT_ValArray(const UT_ValArray<T> &src)
        : UT_Array<T>::UT_Array(src)
    {}
    explicit UT_ValArray(const UT_Array<T> &src)
        : UT_Array<T>::UT_Array(src)
    {}
    UT_ValArray(UT_ValArray<T> &&src) SYS_NOEXCEPT
        : UT_Array<T>::UT_Array(std::move(src))
    {}
    UT_ValArray(UT_Array<T> &&src) SYS_NOEXCEPT
        : UT_Array<T>::UT_Array(std::move(src))
    {}
    explicit UT_ValArray(exint capacity = 0)
        : UT_Array<T>::UT_Array(capacity)
    {}
    UT_ValArray(exint capacity, exint entries)
        : UT_Array<T>::UT_Array(capacity, entries)
    {}
    explicit UT_ValArray(std::initializer_list<T> init)
        : UT_Array<T>::UT_Array(init)
    {}

    UT_ValArray &operator=(const UT_ValArray<T> &src)
    {
        UT_Array<T>::operator=(src);
        return *this;
    }
    UT_ValArray &operator=(const UT_Array<T> &src)
    {
        UT_Array<T>::operator=(src);
        return *this;
    }
    UT_ValArray &operator=(std::initializer_list<T> src)
    {
        UT_Array<T>::operator=(src);
        return *this;
    }
    UT_ValArray &operator=(UT_ValArray<T> &&src)
    {
        UT_Array<T>::operator=(std::move(src));
        return *this;
    }
    UT_ValArray &operator=(UT_Array<T> &&src)
    {
        UT_Array<T>::operator=(std::move(src));
        return *this;
    }

    static bool	    compareElementsBool(const T &a, const T &b)
		    {
			return a < b;
		    }

    /// Sort and then remove all duplicate entries.  This returns the number of
    /// elements removed.
    exint	    sortAndRemoveDuplicates()
		    {
			stableSort(compareElementsBool);
			return UT_Array<T>::sortedRemoveDuplicates();
		    }

    static int	    compareElements(const T *a, const T *b)
		    {
			if (*a > *b)
			    return 1;
			if (*a < *b)
			    return -1;
			return 0;
		    }

    void	    sort(Comparator compare)
		    {
			UT_Array<T>::sort(compare);
		    }
    template <typename ComparatorBool>
    void	    stdsort(ComparatorBool is_less)
		    {
			UT_Array<T>::stdsort(is_less);
		    }
    void	    sort()
		    {
			stdsort(Less());
		    }
    void	    sortAscending()
		    {
			sort();
		    }

    template<typename ComparatorBool>
    void	    stableSort(ComparatorBool is_less)
		    {
			UT_Array<T>::stableSort(is_less);
		    }
    void	    stableSort()
		    {
			stableSort(Less());
		    }

    template <typename ComparatorBool>
    T		    selectNthLargest(int idx, ComparatorBool is_less)
		    {
			return UT_Array<T>::selectNthLargest(idx, is_less);
		    }
    T		    selectNthLargest(int idx)
		    {
			return selectNthLargest(idx, Less());
		    }

    exint	    uniqueSortedFind(const T &item, Comparator compare) const
		    {
			return UT_Array<T>::uniqueSortedFind(item, compare);
		    }
    template <typename ComparatorBool>
    exint	    uniqueSortedFind(const T &item, ComparatorBool is_less) const
		    {
			return UT_Array<T>::uniqueSortedFind(item, is_less);
		    }
    exint	    uniqueSortedFind(const T &item) const
		    {
			return uniqueSortedFind(item, Less());
		    }
    exint	    uniqueSortedFindAscending(const T &item) const
		    {
			return uniqueSortedFind(item);
		    }

    SYS_DEPRECATED_HDK(13.0)
    exint	    sortedInsert(const T &t, Comparator compare)
		    {
			return UT_Array<T>::sortedInsert(t, compare);
		    }
    template <typename ComparatorBool>
    SYS_DEPRECATED_HDK(13.0)
    exint	    sortedInsert(const T &t, ComparatorBool is_less)
		    {
			return UT_Array<T>::sortedInsert(t, is_less);
		    }
    SYS_DEPRECATED_HDK(13.0)
    exint	    sortedInsert(const T &t)
		    {
			return sortedInsert(t, Less());
		    }

    SYS_DEPRECATED_HDK(13.0)
    exint	    uniqueSortedInsert(const T &t, Comparator compare)
		    {
			return UT_Array<T>::uniqueSortedInsert(t, compare);
		    }
    template <typename ComparatorBool>
    SYS_DEPRECATED_HDK(13.0)
    exint	    uniqueSortedInsert(const T &t, ComparatorBool is_less)
		    {
			return UT_Array<T>::uniqueSortedInsert(t, is_less);
		    }
    SYS_DEPRECATED_HDK(13.0)
    exint	    uniqueSortedInsert(const T &t)
		    {
			return uniqueSortedInsert(t, Less());
		    }
    exint SYS_DEPRECATED(13.0) uniqueSortedInsertAscending(const T &t)
		    {
			return uniqueSortedInsert(t);
		    }

    template <typename ComparatorBool>
    void	    merge(
			    const UT_Array<T> &other,
			    int direction,
			    bool allow_dups,
			    ComparatorBool is_less)
		    {
			UT_Array<T>::merge(other, direction, allow_dups,
					   is_less);
		    }
    void	    merge(
			    const UT_ValArray<T> &other,
			    int direction,
			    bool allow_dups)
		    {
			merge(other, direction, allow_dups, Less());
		    }

    bool	    hasSortedSubset(
			    const UT_ValArray<T> &other,
			    Comparator compare = compareElements) const
		    {
			return UT_Array<T>::hasSortedSubset(other, compare);
		    }
    void	    sortedUnion(
			    const UT_ValArray<T> &other,
			    Comparator compare = compareElements)
		    {
			UT_Array<T>::sortedUnion(other, compare);
		    }
    void	    sortedUnion(
			    const UT_ValArray<T> &other,
			    UT_ValArray<T> &result,
			    Comparator compare = compareElements) const
		    {
			UT_Array<T>::sortedUnion(other, result, compare);
		    }
    void	    sortedIntersection(
			    const UT_ValArray<T> &other,
			    Comparator compare = compareElements)
		    {
			UT_Array<T>::sortedIntersection(other, compare);
		    }
    void	    sortedIntersection(
			    const UT_ValArray<T> &other,
			    UT_ValArray<T> &result,
			    Comparator compare = compareElements) const
		    {
			UT_Array<T>::sortedIntersection(other, result, compare);
		    }
    void	    sortedSetDifference(
			    const UT_ValArray<T> &other,
			    Comparator compare = compareElements)
		    {
			UT_Array<T>::sortedSetDifference(other, compare);
		    }
    void	    sortedSetDifference(
			    const UT_ValArray<T> &other,
			    UT_ValArray<T> &result,
			    Comparator compare = compareElements) const
		    {
			UT_Array<T>::sortedSetDifference(other, result, compare);
		    }

    exint	    heapPush(const T &t, Comparator compare)
		    {
			return UT_Array<T>::heapPush(t, compare);
		    }
    T		    heapPop(Comparator compare)
		    {
			return UT_Array<T>::heapPop(compare);
		    }

    static bool	    isElementZero(const T &a)
		    {
			return !a;
		    }

    int		    removeZeros()
		    {
			return this->removeIf(isElementZero);
		    }

    // Remove zeros and also sets the capacity of the array.
    void	    collapse()
		    {
			this->collapseIf(isElementZero);
		    }

    /// Functions which are only specialized for int{32,64}, fpreal{32,64}
    // @{
    /// Returns the sum of the entries in the array.
    T		    sum() const
		    {
			UT_ASSERT(!"Invalid function");
			return T();
		    }
    /// Prints the constents of the array
    void	    display() const
		    {
			printf("%" SYS_PRId64 " entries, contents not displayable.\n",
				UT_Array<T>::size());
		    }
    // @}

private:
    // manual wrapper for std functors to avoid including <functional>
    struct Less
    {
	bool operator()(const T& lhs, const T& rhs) const
	    { return lhs < rhs; }
    };
};

SYS_DEPRECATED_POP_DISABLE()

template <>
inline int
UT_ValArray<fpreal32>::compareElements(const fpreal32 *a, const fpreal32 *b)
{
    const float	tol = UTgetCompareFloatsTolerance();
    if( *a>*b+tol )
	return 1;
    if( *a<*b-tol )
	return -1;
    return 0;
}

template <>
inline int
UT_ValArray<fpreal64>::compareElements(const fpreal64 *a, const fpreal64 *b)
{
    const float	tol = UTgetCompareFloatsTolerance();
    if( *a>*b+tol )
	return 1;
    if( *a<*b-tol )
	return -1;
    return 0;
}

#define UT_DECL_ARITHMETIC_SPECIALIZATION(T)	\
	    template <> UT_API T    UT_ValArray<T>::sum() const; \
	    template <> UT_API void UT_ValArray<T>::display() const; \
	    /**/
    UT_DECL_ARITHMETIC_SPECIALIZATION(int32)
    UT_DECL_ARITHMETIC_SPECIALIZATION(int64)
    UT_DECL_ARITHMETIC_SPECIALIZATION(fpreal32)
    UT_DECL_ARITHMETIC_SPECIALIZATION(fpreal64)
#undef UT_DECL_ARITHMETIC_SPECIALIZATION

template <>
inline int
UT_ValArray<const char *>::compareElements(const char * const *a,
                                           const char * const *b)
{
    if (*a && *b) return strcmp(*a, *b);
    // Use the same comparison order for NULL vs non-NULL as UT_String.
    // Don't think it's correct, however.
    if (*a) return -1;
    if (*b) return 1;
    return 0;
}

/// I don't know why this is needed, given that there's one for UT_Array,
/// but UTswap called on two UT_ValArray's was using the default "copy
/// to temp" function.
UT_SWAPPER_TEMPLATE( UT_ValArray );

typedef UT_ValArray<const char *>	UT_StringList;

// For UT::ArraySet.
namespace UT
{
template <typename T>
struct DefaultClearer;

template <typename T>
struct DefaultClearer<UT_ValArray<T>>
{
    static void clear(UT_ValArray<T> &v) { v.setCapacity(0); }
    static bool isClear(const UT_ValArray<T> &v) { return v.capacity() == 0; }
    static void clearConstruct(UT_ValArray<T> *p)
    {
        new ((void *)p) UT_ValArray<T>();
    }
    static const bool clearNeedsDestruction = false;
};
} // namespace UT

#endif // __UT_VALARRAY_H_INCLUDED__

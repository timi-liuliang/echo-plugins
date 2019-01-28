/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_Algorithm.h ( UT Library, C++)
 *
 * COMMENTS:
 * 	this is supposed to be like STL's algorithm header -- put stuff in here
 * 	that doesn't really belong in any other header.
 */

#ifndef __UT_Algorithm__
#define __UT_Algorithm__

#include "UT_Assert.h"

#include "UT_IteratorRange.h"

#include <SYS/SYS_Types.h>

#include <algorithm>
#include <iterator>
#include <limits>
#include <type_traits>

#include <stddef.h>


/// A generic cycle detector that takes constant space and will detect cycles
/// using at most O(N) extra calls to detect() for a cycle of length N (if it
/// exists) using O(1) space.
///
/// NOTE: In general, this won't detect cycles immediately and so your loop
///       must be able to support extra iterations.
///
///       You might have as many as 3 times the number of iterations as your
///       total path length.
///
/// Example:
///
/// T			walk;
/// UT_CycleDetect<T>	cycle;
/// for (walk = first; walk && !cycle.detect(walk); walk = walk->next)
/// {
///     .... do stuff with walk ....
/// }
///
/// @internal
/// This is an implementation of (Richard P.) Brent's cycle detection
/// algorithm. For an alternate cycle detector, there is also Floyd's cycle
/// detection algorithm but that one needs to be given 2 iterators.
/// @endinternal
template <typename T>
class UT_CycleDetect
{
public:

    ///
    /// Helper class for automatically saving and restoring the state of the
    /// cycle detection within a scope block.
    ///
    /// Example:
    ///
    ///	    T walk;
    ///	    UT_CycleDetect<T> cycle;
    ///	    .....
    ///	    {
    ///		UT_CycleDetect<T>::AutoScope cycle_detect_scope(cycle);
    ///		if (!cycle.detect(<some_object>))
    ///		{
    ///		    ... do some work that may recursively update the cycle ...
    ///		}
    ///		else
    ///		{
    ///		    ... cycle detected.  Do work to report/handle cycle ...
    ///
    ///		    cycle_detect_scope.reset();
    ///		}
    ///	    }
    ///
    class AutoScope
    {
    public:
	AutoScope(UT_CycleDetect<T> *cycle)
	    : myCycle(cycle)
	    , myShouldReset(false)
	{
	    mySavedHead = myCycle->myHead;
	    mySavedCount = myCycle->myCount;
	    mySavedStartCount = myCycle->myStartCount;
	}

	~AutoScope()
	{
	    if (myShouldReset)
	    {
		myCycle->reset();
	    }
	    else
	    {
		myCycle->myHead = mySavedHead;
		myCycle->myCount = mySavedCount;
		myCycle->myStartCount = mySavedStartCount;
	    }
	}

	void reset()
	{
	    myShouldReset = true;
	}
    private:
	UT_CycleDetect<T> *myCycle;
	T mySavedHead;
	int64 mySavedCount;
	int64 mySavedStartCount;
	bool myShouldReset;
    };

    UT_CycleDetect()
    {
	reset();
    }

    void reset()
    {
	myCount = 2;	    // must be a power of 2
	myCycleDetected = false;
    }

    bool detect(const T &tail)
    {
	if (myCycleDetected)
	    return true;

	// Reset before detection so that for loop idiom works.
	// The if test is one iteration of the standard Brian Kernighan
	// bit count algorithm. We use this to check if myCount is a power
	// of 2 (ie. at most 1 bit set).
	if ((myCount & (myCount - 1)) == 0)
	{
	    myHead = tail;
	    myStartCount = myCount;
	}
	else if (myHead == tail)
	{
	    myCycleDetected = true;
	    return true;
	}

	myCount++;
	return false;
    }

    exint length() const
    {
	return myCount - myStartCount;
    }

private:
    T	    myHead;
    int64   myCount;
    int64   myStartCount;
    bool    myCycleDetected;
};
/// A version of UT_CycleDetect that additionally supports querying of the
/// cycle length and detection of cycles with a minimum length.
/// @note Does not support cycle lengths larger than an exint
template <typename T>
class UT_CycleDetectEx
{
public:
    inline UT_CycleDetectEx()
    {
	reset();
    }

    inline void reset()
    {
	// Initialize myCount to the smallest value such that myHead is
	// assigned on first but not the second call to detect().
	myCount = 1;
	myStartCount = myCount;
	myCycleDetected = false;
    }

    inline exint length() const
    {
	return myCount - myStartCount;
    }

    inline bool detect(const T &tail, exint min_length = 2)
    {
	UT_ASSERT_P(min_length >= 2);

	if (myCycleDetected)
	    return true;

	++myCount;

	// Reset before detection so that for loop idiom works.
	// The if test is one iteration of the standard Brian Kernighan
	// bit count algorithm. We use this to check if myCount is a power
	// of 2 (ie. at most 1 bit set).
	if ((myCount & (myCount - 1)) == 0)
	{
	    myHead = tail;
	    myStartCount = myCount - 1;
	}
	else if (myHead == tail && length() >= min_length)
	{
	    myCycleDetected = true;
	    return true;
	}
	return false;
    }

    /// The algorithm guarantees that will detect a cycle with no more than
    /// MAX_CYCLE_FACTOR*min_length calls to detect()
    enum { MAX_CYCLE_FACTOR = 3 };

private:
    T	    myHead;
    exint   myCount;
    exint   myStartCount;
    bool    myCycleDetected;
};

/// Delete all items in the range using the @c delete operator. The item type
/// must be a pointer type.
template<typename ForwardIt>
void UTdeleteAll(ForwardIt begin, ForwardIt end)
{
    static_assert(std::is_pointer<typename ForwardIt::value_type>::value,
                  "The element type of the iterator must be a pointer." );
    while (begin != end)
    {
	delete *begin;
	*begin = nullptr;
	++begin;
    }
}

template<typename ForwardIt>
void UTdeleteAll(UT_IteratorRange<ForwardIt> range)
{
    UTdeleteAll(range.begin(), range.end());
}


/// Deletes all items from a container. The container item type must be a
/// pointer type.
template<typename T>
void UTdeleteAll(T &container)
{
    static_assert(std::is_pointer<typename T::value_type>::value, 
                  "The element type of the container must be a pointer." );
    UTdeleteAll(container.begin(), container.end());
}

/// Get the min/max values of an array.
template<typename InputIt>
inline void
UTgetArrayMinMax(InputIt begin,
                 InputIt end,
                 typename std::iterator_traits<InputIt>::value_type &min,
                 typename std::iterator_traits<InputIt>::value_type &max)
{
    typedef typename std::iterator_traits<InputIt>::value_type input_t;

    std::for_each(begin, end,
        [&min, &max](const input_t &v)
        {
            min = SYSmin(min, v);
            max = SYSmax(max, v);
        });
}

/// Normalize an array.
template<typename InputIt, typename OutputIt>
inline void
UTnormalizeArray(InputIt begin,
                 InputIt end,
                 OutputIt d_begin)
{
    typedef typename std::iterator_traits<InputIt>::value_type input_t;
    input_t min = std::numeric_limits<input_t>::max();
    input_t max = std::numeric_limits<input_t>::min();
    UTgetArrayMinMax(begin, end, min, max);
    std::transform(begin, end, d_begin,
        [&min, &max](const input_t &v)
        {
            return (v-min) / (max-min);
        });
}

/// Selects between arguments 'src1/src2' based on the lower of 'a/b'.
template<typename T>
inline size_t UTfastSelectLow(T a, T b, size_t src1, size_t src2)
{
#if defined(__clang__) && defined(__x86_64)
    size_t res = src1;
    asm("cmpq %1, %2; cmovaeq %4, %0"
        :
    "=q" (res)
        :
        "q" (a),
        "q" (b),
        "q" (src1),
        "q" (src2),
        "0" (res)
        :
        "cc");
    return res;
#else
    return b >= a ? src2 : src1;
#endif
}

/// Fast upper bound search.
/// Adapted from "How We Beat C++ STL Binary Search"
/// Reference: https://realm.io/news/how-we-beat-cpp-stl-binary-search/
template<typename ForwardIt, typename T>
inline ForwardIt UTfastUpperBound(ForwardIt first, ForwardIt last, const T &value)
{
    size_t size = last - first;
    size_t low = 0;

    while (size >= 8)
    {
        size_t half = size / 2;
        size_t other_half = size - half;
        size_t probe = low + half;
        size_t other_low = low + other_half;
        size = half;
        low = UTfastSelectLow(*(first+probe), value, low, other_low);

        half = size / 2;
        other_half = size - half;
        probe = low + half;
        other_low = low + other_half;
        size = half;
        low = UTfastSelectLow(*(first+probe), value, low, other_low);

        half = size / 2;
        other_half = size - half;
        probe = low + half;
        other_low = low + other_half;
        size = half;
        low = UTfastSelectLow(*(first+probe), value, low, other_low);
    }

    while (size > 0)
    {
        size_t half = size / 2;
        size_t other_half = size - half;
        size_t probe = low + half;
        size_t other_low = low + other_half;
        size = half;
        low = UTfastSelectLow(*(first+probe), value, low, other_low);
    }

    return first + low;
}

/// Find the value associated with the key. If the key is not found in the map,
/// a value is created with the given callback function and then inserted into
/// the map before returning.
template <typename Map, typename Key, typename CreateValueCB>
const typename Map::mapped_type &
UTfindOrInsert(Map &map, const Key &key, CreateValueCB create_value)
{
    auto it = map.find(key);
    if (it == map.end())
        it = map.emplace(key, create_value()).first;

    return it->second;
}

#endif


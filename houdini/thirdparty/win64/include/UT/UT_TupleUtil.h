/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_TupleUtil.h (UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_TUPLEUTIL_H_INCLUDED__
#define __UT_TUPLEUTIL_H_INCLUDED__

#include "UT_API.h"

#include <tuple>
#include <type_traits>
#include <utility>
#include <stddef.h>

namespace UT_TupleUtilImpl
{
    template <typename T, typename F, size_t I, size_t N>
    struct AnyOf;

    // Base case for last element
    template <typename T, typename F, size_t N>
    struct AnyOf<T, F, N, N>
    {
	bool operator()(T&& tuple, F&& f)
	{
	    return false;
	}
    };

    // Recursive case
    template <typename T, typename F, size_t I, size_t N>
    struct AnyOf
    {
	bool operator()(T&& tuple, F&& f)
	{
	    if (f(std::get<I>(std::forward<T>(tuple))))
		return true;
	    return AnyOf<T, F, I+1, N>()(std::forward<T>(tuple),
					 std::forward<F>(f));
	}
    };

    template <typename T, typename F, size_t I, size_t N>
    struct ForEach;

    // Base case for last element
    template <typename T, typename F, size_t N>
    struct ForEach<T, F, N, N>
    {
	void operator()(T&& tuple, F&& f)
	{
	}
    };

    // Recursive case
    template <typename T, typename F, size_t I, size_t N>
    struct ForEach
    {
	void operator()(T&& tuple, F&& f)
	{
	    f(std::get<I>(std::forward<T>(tuple)));
	    ForEach<T, F, I+1, N>()(std::forward<T>(tuple), std::forward<F>(f));
	}
    };
}

/// Invoke functor f for each element e in a tuple, returning true as soon as
/// f(e) returns true. Returns false when f(e) is false for all elements.
///
/// @note This is like std::any_of() except for tuples.
/// @note The tuple need not be std::tuple, and instead may be anything that
///       supports std::get and std::tuple_size; in particular, std::array and
///       std::pair may be used. 
template <typename T, typename F>
bool
UTtupleAnyOf(T&& tuple, F&& f)
{
    return UT_TupleUtilImpl::AnyOf<
	    T, F, 0, std::tuple_size<typename std::decay<T>::type>::value>()(
		std::forward<T>(tuple), std::forward<F>(f));
}

/// Invoke functor f for each element e in a tuple.
///
/// @note This is like std::for_each() except for tuples.
/// @note The tuple need not be std::tuple, and instead may be anything that
///       supports std::get and std::tuple_size; in particular, std::array and
///       std::pair may be used. 
template <typename T, typename F>
void
UTtupleForEach(T&& tuple, F&& f)
{
    UT_TupleUtilImpl::ForEach<
	    T, F, 0, std::tuple_size<typename std::decay<T>::type>::value>()(
		std::forward<T>(tuple), std::forward<F>(f));
}

#endif // __UT_TUPLEUTIL_H_INCLUDED__

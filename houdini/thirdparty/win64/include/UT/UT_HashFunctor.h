/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	UT_HashFunctor.h (UT Library, C++)
 *
 * COMMENTS:
 */

#ifndef __UT_HashFunctor__
#define __UT_HashFunctor__

#include "UT_API.h"
#include <cstddef>

/// A generic hash object which can be used by UT_Map and UT_ConcurrentHashMap.
/// The template argument must have a @c hash() method and a correct @c ==
/// operator.
template <typename T>
class UT_HashFunctor
{
public:
    /// hash() used by UT_ConcurrentHashMap
    static size_t	hash(const T &o)	{ return o.hash(); }
    /// equal() function used by UT_ConcurrentHashMap
    static bool		equal(const T &a, const T &b)	{ return a == b; }

    /// () operator for UT_Map
    inline size_t	operator()(const T &o) const	{ return o.hash(); }
};

#endif

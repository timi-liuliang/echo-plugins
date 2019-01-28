/*
 * PROPRIETARY INFORMATION.  This software is proprietary to
 * Side Effects Software Inc., and is not to be reproduced,
 * transmitted, or disclosed in any way without written permission.
 *
 * NAME:	SYS_StaticAssert.h ( SYS Library, C++)
 *
 * COMMENTS:
 */

#ifndef __SYS_StaticAssert__
#define __SYS_StaticAssert__

/// A macro to trigger assertions at compile-time. Comes in two flavor, one
/// which simply shows the expression that failed, another one that allows
/// a custom message to be supplied.

#if (__cplusplus >= 201103L) || (_MSC_VER >= 1700)
// C++11 has native static_assert support. For C++03 and prior, use Boost's
// version.
#  define SYS_STATIC_ASSERT(expr) static_assert(expr, #expr)
#  define SYS_STATIC_ASSERT_MSG(expr, msg) static_assert(expr, msg)
#else
#  include <hboost/static_assert.hpp>
#  define SYS_STATIC_ASSERT(expr) HBOOST_STATIC_ASSERT(expr)
#  define SYS_STATIC_ASSERT_MSG(expr, msg) HBOOST_STATIC_ASSERT_MSG(expr, msg)
#endif

/// Use this macro to trigger a compilation error on deliberately unimplemented
/// template functions. Pass in one of the template arguments, to make this
/// type-dependent.
#define SYS_UNIMPLEMENTED_TEMPLATE(T) \
    SYS_STATIC_ASSERT_MSG(sizeof(T) == 0, "Unimplemented template function.")

#endif // __SYS_StaticAssert__

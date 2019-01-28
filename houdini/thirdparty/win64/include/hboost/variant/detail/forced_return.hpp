//-----------------------------------------------------------------------------
// boost variant/detail/forced_return.hpp header file
// See http://www.boost.org for updates, documentation, and revision history.
//-----------------------------------------------------------------------------
//
// Copyright (c) 2003
// Eric Friedman
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#ifndef HBOOST_VARIANT_DETAIL_FORCED_RETURN_HPP
#define HBOOST_VARIANT_DETAIL_FORCED_RETURN_HPP

#include "hboost/config.hpp"
#include "hboost/variant/detail/generic_result_type.hpp"
#include "hboost/assert.hpp"

namespace hboost {
namespace detail { namespace variant {

///////////////////////////////////////////////////////////////////////////////
// (detail) function template forced_return
//
// Logical error to permit invocation at runtime, but (artificially) satisfies
// compile-time requirement of returning a result value.
//

#if !defined(HBOOST_MSVC)                                \
 && !defined(HBOOST_NO_VOID_RETURNS)

// "standard" implementation:

template <typename T>
inline T forced_return()
{
    // logical error: should never be here! (see above)
    HBOOST_ASSERT(false);

    T (*dummy_function_ptr)() = 0;
    return dummy_function_ptr();
}

template <>
inline void forced_return<void>()
{
    // logical error: should never be here! (see above)
    HBOOST_ASSERT(false);
}

#elif !defined(HBOOST_MSVC)

// workaround implementation
//
// TODO: Determine the most efficient way to handle this -- as below? by
// throwing? by recursive call to forced_return itself? etc.
//

template <typename T>
inline
    HBOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(T)
forced_return()
{
    // logical error: should never be here! (see above)
    HBOOST_ASSERT(false);

    HBOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(T) (*dummy)() = 0;
    return dummy();
}

#else // defined(HBOOST_MSVC)

# pragma warning( push )
# pragma warning( disable : 4702 ) // unreachable code
// msvc-specific implementation
//
// Leverages __declspec(noreturn) for optimized implementation.
//

__declspec(noreturn)
inline void forced_return_no_return() {};

template <typename T>
inline
    HBOOST_VARIANT_AUX_GENERIC_RESULT_TYPE(T)
forced_return()
{
    // logical error: should never be here! (see above)
    HBOOST_ASSERT(false);

    forced_return_no_return();
}

# pragma warning( pop )

#endif // HBOOST_MSVC optimization

}} // namespace detail::variant
} // namespace hboost

#endif // HBOOST_VARIANT_DETAIL_FORCED_RETURN_HPP

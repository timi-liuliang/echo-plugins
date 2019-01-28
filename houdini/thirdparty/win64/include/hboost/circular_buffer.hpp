// Circular buffer library header file.

// Copyright (c) 2003-2008 Jan Gaspar

// Use, modification, and distribution is subject to the Boost Software
// License, Version 1.0. (See accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

//  See www.boost.org/libs/circular_buffer for documentation.

#if !defined(HBOOST_CIRCULAR_BUFFER_HPP)
#define HBOOST_CIRCULAR_BUFFER_HPP

#if defined(_MSC_VER)
    #pragma once
#endif

#include <hboost/circular_buffer_fwd.hpp>
#include <hboost/detail/workaround.hpp>
#include <hboost/static_assert.hpp>

// HBOOST_CB_ENABLE_DEBUG: Debug support control.
#if defined(NDEBUG) || defined(HBOOST_CB_DISABLE_DEBUG)
    #define HBOOST_CB_ENABLE_DEBUG 0
#else
    #define HBOOST_CB_ENABLE_DEBUG 1
#endif

// HBOOST_CB_ASSERT: Runtime assertion.
#if HBOOST_CB_ENABLE_DEBUG
    #include <hboost/assert.hpp>
    #define HBOOST_CB_ASSERT(Expr) HBOOST_ASSERT(Expr)
#else
    #define HBOOST_CB_ASSERT(Expr) ((void)0)
#endif

// HBOOST_CB_IS_CONVERTIBLE: Check if Iterator::value_type is convertible to Type.
#if HBOOST_WORKAROUND(__BORLANDC__, <= 0x0550) || HBOOST_WORKAROUND(__MWERKS__, <= 0x2407)
    #define HBOOST_CB_IS_CONVERTIBLE(Iterator, Type) ((void)0)
#else
    #include <hboost/detail/iterator.hpp>
    #include <hboost/type_traits/is_convertible.hpp>
    #define HBOOST_CB_IS_CONVERTIBLE(Iterator, Type) \
        HBOOST_STATIC_ASSERT((is_convertible<typename detail::iterator_traits<Iterator>::value_type, Type>::value))
#endif

// HBOOST_CB_ASSERT_TEMPLATED_ITERATOR_CONSTRUCTORS:
// Check if the STL provides templated iterator constructors for its containers.
#if defined(HBOOST_NO_TEMPLATED_ITERATOR_CONSTRUCTORS)
    #define HBOOST_CB_ASSERT_TEMPLATED_ITERATOR_CONSTRUCTORS HBOOST_STATIC_ASSERT(false);
#else
    #define HBOOST_CB_ASSERT_TEMPLATED_ITERATOR_CONSTRUCTORS ((void)0);
#endif

#include <hboost/circular_buffer/debug.hpp>
#include <hboost/circular_buffer/details.hpp>
#include <hboost/circular_buffer/base.hpp>
#include <hboost/circular_buffer/space_optimized.hpp>

#undef HBOOST_CB_ASSERT_TEMPLATED_ITERATOR_CONSTRUCTORS
#undef HBOOST_CB_IS_CONVERTIBLE
#undef HBOOST_CB_ASSERT
#undef HBOOST_CB_ENABLE_DEBUG

#endif // #if !defined(HBOOST_CIRCULAR_BUFFER_HPP)

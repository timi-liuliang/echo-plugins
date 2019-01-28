/*
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * Copyright (c) 2014 Andrey Semashev
 */
/*!
 * \file   atomic/detail/atomic_flag.hpp
 *
 * This header contains interface definition of \c atomic_flag.
 */

#ifndef HBOOST_ATOMIC_DETAIL_ATOMIC_FLAG_HPP_INCLUDED_
#define HBOOST_ATOMIC_DETAIL_ATOMIC_FLAG_HPP_INCLUDED_

#include <hboost/assert.hpp>
#include <hboost/memory_order.hpp>
#include <hboost/atomic/detail/config.hpp>
#include <hboost/atomic/detail/operations_lockfree.hpp>

#ifdef HBOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

/*
 * IMPLEMENTATION NOTE: All interface functions MUST be declared with HBOOST_FORCEINLINE,
 *                      see comment for convert_memory_order_to_gcc in ops_gcc_atomic.hpp.
 */

namespace hboost {
namespace atomics {

#if defined(HBOOST_NO_CXX11_CONSTEXPR) || defined(HBOOST_NO_CXX11_UNIFIED_INITIALIZATION_SYNTAX)
#define HBOOST_ATOMIC_NO_ATOMIC_FLAG_INIT
#else
#define HBOOST_ATOMIC_FLAG_INIT {}
#endif

struct atomic_flag
{
    typedef atomics::detail::operations< 1u, false > operations;
    typedef operations::storage_type storage_type;

    operations::aligned_storage_type m_storage;

    HBOOST_FORCEINLINE HBOOST_CONSTEXPR atomic_flag() HBOOST_NOEXCEPT : m_storage(0)
    {
    }

    HBOOST_FORCEINLINE bool test_and_set(memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        return operations::test_and_set(m_storage.value, order);
    }

    HBOOST_FORCEINLINE void clear(memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        HBOOST_ASSERT(order != memory_order_acquire);
        HBOOST_ASSERT(order != memory_order_acq_rel);
        operations::clear(m_storage.value, order);
    }

    HBOOST_DELETED_FUNCTION(atomic_flag(atomic_flag const&))
    HBOOST_DELETED_FUNCTION(atomic_flag& operator= (atomic_flag const&))
};

} // namespace atomics
} // namespace hboost

#endif // HBOOST_ATOMIC_DETAIL_ATOMIC_FLAG_HPP_INCLUDED_

/*
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * Copyright (c) 2014 Andrey Semashev
 */
/*!
 * \file   atomic/detail/ops_cas_based.hpp
 *
 * This header contains CAS-based implementation of the \c operations template.
 */

#ifndef HBOOST_ATOMIC_DETAIL_OPS_CAS_BASED_HPP_INCLUDED_
#define HBOOST_ATOMIC_DETAIL_OPS_CAS_BASED_HPP_INCLUDED_

#include <hboost/memory_order.hpp>
#include <hboost/atomic/detail/config.hpp>
#include <hboost/atomic/detail/storage_type.hpp>

#ifdef HBOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

namespace hboost {
namespace atomics {
namespace detail {

template< typename Base >
struct cas_based_exchange :
    public Base
{
    typedef typename Base::storage_type storage_type;

    static HBOOST_FORCEINLINE storage_type exchange(storage_type volatile& storage, storage_type v, memory_order order) HBOOST_NOEXCEPT
    {
        storage_type old_val;
        atomics::detail::non_atomic_load(storage, old_val);
        while (!Base::compare_exchange_weak(storage, old_val, v, order, memory_order_relaxed)) {}
        return old_val;
    }
};

template< typename Base >
struct cas_based_operations :
    public Base
{
    typedef typename Base::storage_type storage_type;

    static HBOOST_FORCEINLINE storage_type fetch_add(storage_type volatile& storage, storage_type v, memory_order order) HBOOST_NOEXCEPT
    {
        storage_type old_val;
        atomics::detail::non_atomic_load(storage, old_val);
        while (!Base::compare_exchange_weak(storage, old_val, old_val + v, order, memory_order_relaxed)) {}
        return old_val;
    }

    static HBOOST_FORCEINLINE storage_type fetch_sub(storage_type volatile& storage, storage_type v, memory_order order) HBOOST_NOEXCEPT
    {
        storage_type old_val;
        atomics::detail::non_atomic_load(storage, old_val);
        while (!Base::compare_exchange_weak(storage, old_val, old_val - v, order, memory_order_relaxed)) {}
        return old_val;
    }

    static HBOOST_FORCEINLINE storage_type fetch_and(storage_type volatile& storage, storage_type v, memory_order order) HBOOST_NOEXCEPT
    {
        storage_type old_val;
        atomics::detail::non_atomic_load(storage, old_val);
        while (!Base::compare_exchange_weak(storage, old_val, old_val & v, order, memory_order_relaxed)) {}
        return old_val;
    }

    static HBOOST_FORCEINLINE storage_type fetch_or(storage_type volatile& storage, storage_type v, memory_order order) HBOOST_NOEXCEPT
    {
        storage_type old_val;
        atomics::detail::non_atomic_load(storage, old_val);
        while (!Base::compare_exchange_weak(storage, old_val, old_val | v, order, memory_order_relaxed)) {}
        return old_val;
    }

    static HBOOST_FORCEINLINE storage_type fetch_xor(storage_type volatile& storage, storage_type v, memory_order order) HBOOST_NOEXCEPT
    {
        storage_type old_val;
        atomics::detail::non_atomic_load(storage, old_val);
        while (!Base::compare_exchange_weak(storage, old_val, old_val ^ v, order, memory_order_relaxed)) {}
        return old_val;
    }

    static HBOOST_FORCEINLINE bool test_and_set(storage_type volatile& storage, memory_order order) HBOOST_NOEXCEPT
    {
        return !!Base::exchange(storage, (storage_type)1, order);
    }

    static HBOOST_FORCEINLINE void clear(storage_type volatile& storage, memory_order order) HBOOST_NOEXCEPT
    {
        Base::store(storage, (storage_type)0, order);
    }
};

} // namespace detail
} // namespace atomics
} // namespace hboost

#endif // HBOOST_ATOMIC_DETAIL_OPS_CAS_BASED_HPP_INCLUDED_

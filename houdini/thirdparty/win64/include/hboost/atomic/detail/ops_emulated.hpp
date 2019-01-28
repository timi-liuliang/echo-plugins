/*
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * Copyright (c) 2014 Andrey Semashev
 */
/*!
 * \file   atomic/detail/ops_emulated.hpp
 *
 * This header contains lockpool-based implementation of the \c operations template.
 */

#ifndef HBOOST_ATOMIC_DETAIL_OPS_EMULATED_HPP_INCLUDED_
#define HBOOST_ATOMIC_DETAIL_OPS_EMULATED_HPP_INCLUDED_

#include <cstddef>
#include <hboost/memory_order.hpp>
#include <hboost/atomic/detail/config.hpp>
#include <hboost/atomic/detail/storage_type.hpp>
#include <hboost/atomic/detail/operations_fwd.hpp>
#include <hboost/atomic/detail/lockpool.hpp>
#include <hboost/atomic/capabilities.hpp>

#ifdef HBOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

namespace hboost {
namespace atomics {
namespace detail {

template< typename T >
struct emulated_operations
{
    typedef T storage_type;

    static HBOOST_FORCEINLINE void store(storage_type volatile& storage, storage_type v, memory_order) HBOOST_NOEXCEPT
    {
        lockpool::scoped_lock lock(&storage);
        const_cast< storage_type& >(storage) = v;
    }

    static HBOOST_FORCEINLINE storage_type load(storage_type const volatile& storage, memory_order) HBOOST_NOEXCEPT
    {
        lockpool::scoped_lock lock(&storage);
        return const_cast< storage_type const& >(storage);
    }

    static HBOOST_FORCEINLINE storage_type fetch_add(storage_type volatile& storage, storage_type v, memory_order) HBOOST_NOEXCEPT
    {
        storage_type& s = const_cast< storage_type& >(storage);
        lockpool::scoped_lock lock(&storage);
        storage_type old_val = s;
        s += v;
        return old_val;
    }

    static HBOOST_FORCEINLINE storage_type fetch_sub(storage_type volatile& storage, storage_type v, memory_order) HBOOST_NOEXCEPT
    {
        storage_type& s = const_cast< storage_type& >(storage);
        lockpool::scoped_lock lock(&storage);
        storage_type old_val = s;
        s -= v;
        return old_val;
    }

    static HBOOST_FORCEINLINE storage_type exchange(storage_type volatile& storage, storage_type v, memory_order) HBOOST_NOEXCEPT
    {
        storage_type& s = const_cast< storage_type& >(storage);
        lockpool::scoped_lock lock(&storage);
        storage_type old_val = s;
        s = v;
        return old_val;
    }

    static HBOOST_FORCEINLINE bool compare_exchange_strong(
        storage_type volatile& storage, storage_type& expected, storage_type desired, memory_order, memory_order) HBOOST_NOEXCEPT
    {
        storage_type& s = const_cast< storage_type& >(storage);
        lockpool::scoped_lock lock(&storage);
        storage_type old_val = s;
        const bool res = old_val == expected;
        if (res)
            s = desired;
        expected = old_val;

        return res;
    }

    static HBOOST_FORCEINLINE bool compare_exchange_weak(
        storage_type volatile& storage, storage_type& expected, storage_type desired, memory_order, memory_order) HBOOST_NOEXCEPT
    {
        // Note: This function is the exact copy of compare_exchange_strong. The reason we're not just forwarding the call
        // is that MSVC-12 ICEs in this case.
        storage_type& s = const_cast< storage_type& >(storage);
        lockpool::scoped_lock lock(&storage);
        storage_type old_val = s;
        const bool res = old_val == expected;
        if (res)
            s = desired;
        expected = old_val;

        return res;
    }

    static HBOOST_FORCEINLINE storage_type fetch_and(storage_type volatile& storage, storage_type v, memory_order) HBOOST_NOEXCEPT
    {
        storage_type& s = const_cast< storage_type& >(storage);
        lockpool::scoped_lock lock(&storage);
        storage_type old_val = s;
        s &= v;
        return old_val;
    }

    static HBOOST_FORCEINLINE storage_type fetch_or(storage_type volatile& storage, storage_type v, memory_order) HBOOST_NOEXCEPT
    {
        storage_type& s = const_cast< storage_type& >(storage);
        lockpool::scoped_lock lock(&storage);
        storage_type old_val = s;
        s |= v;
        return old_val;
    }

    static HBOOST_FORCEINLINE storage_type fetch_xor(storage_type volatile& storage, storage_type v, memory_order) HBOOST_NOEXCEPT
    {
        storage_type& s = const_cast< storage_type& >(storage);
        lockpool::scoped_lock lock(&storage);
        storage_type old_val = s;
        s ^= v;
        return old_val;
    }

    static HBOOST_FORCEINLINE bool test_and_set(storage_type volatile& storage, memory_order order) HBOOST_NOEXCEPT
    {
        return !!exchange(storage, (storage_type)1, order);
    }

    static HBOOST_FORCEINLINE void clear(storage_type volatile& storage, memory_order order) HBOOST_NOEXCEPT
    {
        store(storage, (storage_type)0, order);
    }

    static HBOOST_FORCEINLINE bool is_lock_free(storage_type const volatile&) HBOOST_NOEXCEPT
    {
        return false;
    }
};

template< std::size_t Size, bool Signed >
struct operations :
    public emulated_operations< typename make_storage_type< Size, Signed >::type >
{
    typedef typename make_storage_type< Size, Signed >::aligned aligned_storage_type;
};

} // namespace detail
} // namespace atomics
} // namespace hboost

#endif // HBOOST_ATOMIC_DETAIL_OPS_EMULATED_HPP_INCLUDED_

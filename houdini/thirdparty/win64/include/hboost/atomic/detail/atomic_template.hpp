/*
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * Copyright (c) 2011 Helge Bahmann
 * Copyright (c) 2013 Tim Blechmann
 * Copyright (c) 2014 Andrey Semashev
 */
/*!
 * \file   atomic/detail/atomic_template.hpp
 *
 * This header contains interface definition of \c atomic template.
 */

#ifndef HBOOST_ATOMIC_DETAIL_ATOMIC_TEMPLATE_HPP_INCLUDED_
#define HBOOST_ATOMIC_DETAIL_ATOMIC_TEMPLATE_HPP_INCLUDED_

#include <cstddef>
#include <hboost/cstdint.hpp>
#include <hboost/assert.hpp>
#include <hboost/type_traits/is_signed.hpp>
#include <hboost/type_traits/is_integral.hpp>
#include <hboost/atomic/detail/config.hpp>
#include <hboost/atomic/detail/bitwise_cast.hpp>
#include <hboost/atomic/detail/operations_fwd.hpp>

#ifdef HBOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

#if defined(HBOOST_MSVC)
#pragma warning(push)
// 'hboost::atomics::atomic<T>' : multiple assignment operators specified
#pragma warning(disable: 4522)
#endif

/*
 * IMPLEMENTATION NOTE: All interface functions MUST be declared with HBOOST_FORCEINLINE,
 *                      see comment for convert_memory_order_to_gcc in ops_gcc_atomic.hpp.
 */

namespace hboost {
namespace atomics {
namespace detail {

HBOOST_FORCEINLINE HBOOST_CONSTEXPR memory_order deduce_failure_order(memory_order order) HBOOST_NOEXCEPT
{
    return order == memory_order_acq_rel ? memory_order_acquire : (order == memory_order_release ? memory_order_relaxed : order);
}

HBOOST_FORCEINLINE HBOOST_CONSTEXPR bool cas_failure_order_must_not_be_stronger_than_success_order(memory_order success_order, memory_order failure_order) HBOOST_NOEXCEPT
{
    // 15 == (memory_order_seq_cst | memory_order_consume), see memory_order.hpp
    // Given the enum values we can test the strength of memory order requirements with this single condition.
    return (failure_order & 15u) <= (success_order & 15u);
}

template< typename T, bool IsInt = hboost::is_integral< T >::value >
struct classify
{
    typedef void type;
};

template< typename T >
struct classify< T, true > { typedef int type; };

template< typename T >
struct classify< T*, false > { typedef void* type; };

template< typename T, typename Kind >
class base_atomic;

//! Implementation for integers
template< typename T >
class base_atomic< T, int >
{
private:
    typedef T value_type;
    typedef T difference_type;
    typedef atomics::detail::operations< storage_size_of< value_type >::value, hboost::is_signed< T >::value > operations;

protected:
    typedef value_type value_arg_type;

public:
    typedef typename operations::storage_type storage_type;

protected:
    typename operations::aligned_storage_type m_storage;

public:
    HBOOST_DEFAULTED_FUNCTION(base_atomic(), {})
    HBOOST_CONSTEXPR explicit base_atomic(value_type v) HBOOST_NOEXCEPT : m_storage(v) {}

    HBOOST_FORCEINLINE void store(value_type v, memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        HBOOST_ASSERT(order != memory_order_consume);
        HBOOST_ASSERT(order != memory_order_acquire);
        HBOOST_ASSERT(order != memory_order_acq_rel);

        operations::store(m_storage.value, static_cast< storage_type >(v), order);
    }

    HBOOST_FORCEINLINE value_type load(memory_order order = memory_order_seq_cst) const volatile HBOOST_NOEXCEPT
    {
        HBOOST_ASSERT(order != memory_order_release);
        HBOOST_ASSERT(order != memory_order_acq_rel);

        return static_cast< value_type >(operations::load(m_storage.value, order));
    }

    HBOOST_FORCEINLINE value_type fetch_add(difference_type v, memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        return static_cast< value_type >(operations::fetch_add(m_storage.value, static_cast< storage_type >(v), order));
    }

    HBOOST_FORCEINLINE value_type fetch_sub(difference_type v, memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        return static_cast< value_type >(operations::fetch_sub(m_storage.value, static_cast< storage_type >(v), order));
    }

    HBOOST_FORCEINLINE value_type exchange(value_type v, memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        return static_cast< value_type >(operations::exchange(m_storage.value, static_cast< storage_type >(v), order));
    }

    HBOOST_FORCEINLINE bool compare_exchange_strong(value_type& expected, value_type desired, memory_order success_order, memory_order failure_order) volatile HBOOST_NOEXCEPT
    {
        HBOOST_ASSERT(failure_order != memory_order_release);
        HBOOST_ASSERT(failure_order != memory_order_acq_rel);
        HBOOST_ASSERT(cas_failure_order_must_not_be_stronger_than_success_order(success_order, failure_order));

        storage_type old_value = static_cast< storage_type >(expected);
        const bool res = operations::compare_exchange_strong(m_storage.value, old_value, static_cast< storage_type >(desired), success_order, failure_order);
        expected = static_cast< value_type >(old_value);
        return res;
    }

    HBOOST_FORCEINLINE bool compare_exchange_strong(value_type& expected, value_type desired, memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        return compare_exchange_strong(expected, desired, order, atomics::detail::deduce_failure_order(order));
    }

    HBOOST_FORCEINLINE bool compare_exchange_weak(value_type& expected, value_type desired, memory_order success_order, memory_order failure_order) volatile HBOOST_NOEXCEPT
    {
        HBOOST_ASSERT(failure_order != memory_order_release);
        HBOOST_ASSERT(failure_order != memory_order_acq_rel);
        HBOOST_ASSERT(cas_failure_order_must_not_be_stronger_than_success_order(success_order, failure_order));

        storage_type old_value = static_cast< storage_type >(expected);
        const bool res = operations::compare_exchange_weak(m_storage.value, old_value, static_cast< storage_type >(desired), success_order, failure_order);
        expected = static_cast< value_type >(old_value);
        return res;
    }

    HBOOST_FORCEINLINE bool compare_exchange_weak(value_type& expected, value_type desired, memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        return compare_exchange_weak(expected, desired, order, atomics::detail::deduce_failure_order(order));
    }

    HBOOST_FORCEINLINE value_type fetch_and(value_type v, memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        return static_cast< value_type >(operations::fetch_and(m_storage.value, static_cast< storage_type >(v), order));
    }

    HBOOST_FORCEINLINE value_type fetch_or(value_type v, memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        return static_cast< value_type >(operations::fetch_or(m_storage.value, static_cast< storage_type >(v), order));
    }

    HBOOST_FORCEINLINE value_type fetch_xor(value_type v, memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        return static_cast< value_type >(operations::fetch_xor(m_storage.value, static_cast< storage_type >(v), order));
    }

    HBOOST_FORCEINLINE bool is_lock_free() const volatile HBOOST_NOEXCEPT
    {
        return operations::is_lock_free(m_storage.value);
    }

    HBOOST_FORCEINLINE value_type operator++(int) volatile HBOOST_NOEXCEPT
    {
        return fetch_add(1);
    }

    HBOOST_FORCEINLINE value_type operator++() volatile HBOOST_NOEXCEPT
    {
        return fetch_add(1) + 1;
    }

    HBOOST_FORCEINLINE value_type operator--(int) volatile HBOOST_NOEXCEPT
    {
        return fetch_sub(1);
    }

    HBOOST_FORCEINLINE value_type operator--() volatile HBOOST_NOEXCEPT
    {
        return fetch_sub(1) - 1;
    }

    HBOOST_FORCEINLINE value_type operator+=(difference_type v) volatile HBOOST_NOEXCEPT
    {
        return fetch_add(v) + v;
    }

    HBOOST_FORCEINLINE value_type operator-=(difference_type v) volatile HBOOST_NOEXCEPT
    {
        return fetch_sub(v) - v;
    }

    HBOOST_FORCEINLINE value_type operator&=(value_type v) volatile HBOOST_NOEXCEPT
    {
        return fetch_and(v) & v;
    }

    HBOOST_FORCEINLINE value_type operator|=(value_type v) volatile HBOOST_NOEXCEPT
    {
        return fetch_or(v) | v;
    }

    HBOOST_FORCEINLINE value_type operator^=(value_type v) volatile HBOOST_NOEXCEPT
    {
        return fetch_xor(v) ^ v;
    }

    HBOOST_DELETED_FUNCTION(base_atomic(base_atomic const&))
    HBOOST_DELETED_FUNCTION(base_atomic& operator=(base_atomic const&))
};

//! Implementation for bool
template< >
class base_atomic< bool, int >
{
private:
    typedef bool value_type;
    typedef atomics::detail::operations< 1u, false > operations;

protected:
    typedef value_type value_arg_type;

public:
    typedef operations::storage_type storage_type;

protected:
    operations::aligned_storage_type m_storage;

public:
    HBOOST_DEFAULTED_FUNCTION(base_atomic(), {})
    HBOOST_CONSTEXPR explicit base_atomic(value_type v) HBOOST_NOEXCEPT : m_storage(v) {}

    HBOOST_FORCEINLINE void store(value_type v, memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        HBOOST_ASSERT(order != memory_order_consume);
        HBOOST_ASSERT(order != memory_order_acquire);
        HBOOST_ASSERT(order != memory_order_acq_rel);

        operations::store(m_storage.value, static_cast< storage_type >(v), order);
    }

    HBOOST_FORCEINLINE value_type load(memory_order order = memory_order_seq_cst) const volatile HBOOST_NOEXCEPT
    {
        HBOOST_ASSERT(order != memory_order_release);
        HBOOST_ASSERT(order != memory_order_acq_rel);

        return !!operations::load(m_storage.value, order);
    }

    HBOOST_FORCEINLINE value_type exchange(value_type v, memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        return !!operations::exchange(m_storage.value, static_cast< storage_type >(v), order);
    }

    HBOOST_FORCEINLINE bool compare_exchange_strong(value_type& expected, value_type desired, memory_order success_order, memory_order failure_order) volatile HBOOST_NOEXCEPT
    {
        HBOOST_ASSERT(failure_order != memory_order_release);
        HBOOST_ASSERT(failure_order != memory_order_acq_rel);
        HBOOST_ASSERT(cas_failure_order_must_not_be_stronger_than_success_order(success_order, failure_order));

        storage_type old_value = static_cast< storage_type >(expected);
        const bool res = operations::compare_exchange_strong(m_storage.value, old_value, static_cast< storage_type >(desired), success_order, failure_order);
        expected = !!old_value;
        return res;
    }

    HBOOST_FORCEINLINE bool compare_exchange_strong(value_type& expected, value_type desired, memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        return compare_exchange_strong(expected, desired, order, atomics::detail::deduce_failure_order(order));
    }

    HBOOST_FORCEINLINE bool compare_exchange_weak(value_type& expected, value_type desired, memory_order success_order, memory_order failure_order) volatile HBOOST_NOEXCEPT
    {
        HBOOST_ASSERT(failure_order != memory_order_release);
        HBOOST_ASSERT(failure_order != memory_order_acq_rel);
        HBOOST_ASSERT(cas_failure_order_must_not_be_stronger_than_success_order(success_order, failure_order));

        storage_type old_value = static_cast< storage_type >(expected);
        const bool res = operations::compare_exchange_weak(m_storage.value, old_value, static_cast< storage_type >(desired), success_order, failure_order);
        expected = !!old_value;
        return res;
    }

    HBOOST_FORCEINLINE bool compare_exchange_weak(value_type& expected, value_type desired, memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        return compare_exchange_weak(expected, desired, order, atomics::detail::deduce_failure_order(order));
    }

    HBOOST_FORCEINLINE bool is_lock_free() const volatile HBOOST_NOEXCEPT
    {
        return operations::is_lock_free(m_storage.value);
    }

    HBOOST_DELETED_FUNCTION(base_atomic(base_atomic const&))
    HBOOST_DELETED_FUNCTION(base_atomic& operator=(base_atomic const&))
};


//! Implementation for user-defined types, such as structs and enums
template< typename T >
class base_atomic< T, void >
{
private:
    typedef T value_type;
    typedef atomics::detail::operations< storage_size_of< value_type >::value, false > operations;

protected:
    typedef value_type const& value_arg_type;

public:
    typedef typename operations::storage_type storage_type;

protected:
    typename operations::aligned_storage_type m_storage;

public:
    HBOOST_FORCEINLINE explicit base_atomic(value_type const& v = value_type()) HBOOST_NOEXCEPT : m_storage(atomics::detail::bitwise_cast< storage_type >(v))
    {
    }

    HBOOST_FORCEINLINE void store(value_type v, memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        HBOOST_ASSERT(order != memory_order_consume);
        HBOOST_ASSERT(order != memory_order_acquire);
        HBOOST_ASSERT(order != memory_order_acq_rel);

        operations::store(m_storage.value, atomics::detail::bitwise_cast< storage_type >(v), order);
    }

    HBOOST_FORCEINLINE value_type load(memory_order order = memory_order_seq_cst) const volatile HBOOST_NOEXCEPT
    {
        HBOOST_ASSERT(order != memory_order_release);
        HBOOST_ASSERT(order != memory_order_acq_rel);

        return atomics::detail::bitwise_cast< value_type >(operations::load(m_storage.value, order));
    }

    HBOOST_FORCEINLINE value_type exchange(value_type v, memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        return atomics::detail::bitwise_cast< value_type >(operations::exchange(m_storage.value, atomics::detail::bitwise_cast< storage_type >(v), order));
    }

    HBOOST_FORCEINLINE bool compare_exchange_strong(value_type& expected, value_type desired, memory_order success_order, memory_order failure_order) volatile HBOOST_NOEXCEPT
    {
        HBOOST_ASSERT(failure_order != memory_order_release);
        HBOOST_ASSERT(failure_order != memory_order_acq_rel);
        HBOOST_ASSERT(cas_failure_order_must_not_be_stronger_than_success_order(success_order, failure_order));

        storage_type old_value = atomics::detail::bitwise_cast< storage_type >(expected);
        const bool res = operations::compare_exchange_strong(m_storage.value, old_value, atomics::detail::bitwise_cast< storage_type >(desired), success_order, failure_order);
        expected = atomics::detail::bitwise_cast< value_type >(old_value);
        return res;
    }

    HBOOST_FORCEINLINE bool compare_exchange_strong(value_type& expected, value_type desired, memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        return compare_exchange_strong(expected, desired, order, atomics::detail::deduce_failure_order(order));
    }

    HBOOST_FORCEINLINE bool compare_exchange_weak(value_type& expected, value_type desired, memory_order success_order, memory_order failure_order) volatile HBOOST_NOEXCEPT
    {
        HBOOST_ASSERT(failure_order != memory_order_release);
        HBOOST_ASSERT(failure_order != memory_order_acq_rel);
        HBOOST_ASSERT(cas_failure_order_must_not_be_stronger_than_success_order(success_order, failure_order));

        storage_type old_value = atomics::detail::bitwise_cast< storage_type >(expected);
        const bool res = operations::compare_exchange_weak(m_storage.value, old_value, atomics::detail::bitwise_cast< storage_type >(desired), success_order, failure_order);
        expected = atomics::detail::bitwise_cast< value_type >(old_value);
        return res;
    }

    HBOOST_FORCEINLINE bool compare_exchange_weak(value_type& expected, value_type desired, memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        return compare_exchange_weak(expected, desired, order, atomics::detail::deduce_failure_order(order));
    }

    HBOOST_FORCEINLINE bool is_lock_free() const volatile HBOOST_NOEXCEPT
    {
        return operations::is_lock_free(m_storage.value);
    }

    HBOOST_DELETED_FUNCTION(base_atomic(base_atomic const&))
    HBOOST_DELETED_FUNCTION(base_atomic& operator=(base_atomic const&))
};


//! Implementation for pointers
template< typename T >
class base_atomic< T*, void* >
{
private:
    typedef T* value_type;
    typedef std::ptrdiff_t difference_type;
    typedef atomics::detail::operations< storage_size_of< value_type >::value, false > operations;

protected:
    typedef value_type value_arg_type;

public:
    typedef typename operations::storage_type storage_type;

protected:
    typename operations::aligned_storage_type m_storage;

public:
    HBOOST_DEFAULTED_FUNCTION(base_atomic(), {})
    HBOOST_FORCEINLINE explicit base_atomic(value_type const& v) HBOOST_NOEXCEPT : m_storage(atomics::detail::bitwise_cast< storage_type >(v))
    {
    }

    HBOOST_FORCEINLINE void store(value_type v, memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        HBOOST_ASSERT(order != memory_order_consume);
        HBOOST_ASSERT(order != memory_order_acquire);
        HBOOST_ASSERT(order != memory_order_acq_rel);

        operations::store(m_storage.value, atomics::detail::bitwise_cast< storage_type >(v), order);
    }

    HBOOST_FORCEINLINE value_type load(memory_order order = memory_order_seq_cst) const volatile HBOOST_NOEXCEPT
    {
        HBOOST_ASSERT(order != memory_order_release);
        HBOOST_ASSERT(order != memory_order_acq_rel);

        return atomics::detail::bitwise_cast< value_type >(operations::load(m_storage.value, order));
    }

    HBOOST_FORCEINLINE value_type fetch_add(difference_type v, memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        return atomics::detail::bitwise_cast< value_type >(operations::fetch_add(m_storage.value, static_cast< storage_type >(v * sizeof(T)), order));
    }

    HBOOST_FORCEINLINE value_type fetch_sub(difference_type v, memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        return atomics::detail::bitwise_cast< value_type >(operations::fetch_sub(m_storage.value, static_cast< storage_type >(v * sizeof(T)), order));
    }

    HBOOST_FORCEINLINE value_type exchange(value_type v, memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        return atomics::detail::bitwise_cast< value_type >(operations::exchange(m_storage.value, atomics::detail::bitwise_cast< storage_type >(v), order));
    }

    HBOOST_FORCEINLINE bool compare_exchange_strong(value_type& expected, value_type desired, memory_order success_order, memory_order failure_order) volatile HBOOST_NOEXCEPT
    {
        HBOOST_ASSERT(failure_order != memory_order_release);
        HBOOST_ASSERT(failure_order != memory_order_acq_rel);
        HBOOST_ASSERT(cas_failure_order_must_not_be_stronger_than_success_order(success_order, failure_order));

        storage_type old_value = atomics::detail::bitwise_cast< storage_type >(expected);
        const bool res = operations::compare_exchange_strong(m_storage.value, old_value, atomics::detail::bitwise_cast< storage_type >(desired), success_order, failure_order);
        expected = atomics::detail::bitwise_cast< value_type >(old_value);
        return res;
    }

    HBOOST_FORCEINLINE bool compare_exchange_strong(value_type& expected, value_type desired, memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        return compare_exchange_strong(expected, desired, order, atomics::detail::deduce_failure_order(order));
    }

    HBOOST_FORCEINLINE bool compare_exchange_weak(value_type& expected, value_type desired, memory_order success_order, memory_order failure_order) volatile HBOOST_NOEXCEPT
    {
        HBOOST_ASSERT(failure_order != memory_order_release);
        HBOOST_ASSERT(failure_order != memory_order_acq_rel);
        HBOOST_ASSERT(cas_failure_order_must_not_be_stronger_than_success_order(success_order, failure_order));

        storage_type old_value = atomics::detail::bitwise_cast< storage_type >(expected);
        const bool res = operations::compare_exchange_weak(m_storage.value, old_value, atomics::detail::bitwise_cast< storage_type >(desired), success_order, failure_order);
        expected = atomics::detail::bitwise_cast< value_type >(old_value);
        return res;
    }

    HBOOST_FORCEINLINE bool compare_exchange_weak(value_type& expected, value_type desired, memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        return compare_exchange_weak(expected, desired, order, atomics::detail::deduce_failure_order(order));
    }

    HBOOST_FORCEINLINE bool is_lock_free() const volatile HBOOST_NOEXCEPT
    {
        return operations::is_lock_free(m_storage.value);
    }

    HBOOST_FORCEINLINE value_type operator++(int) volatile HBOOST_NOEXCEPT
    {
        return fetch_add(1);
    }

    HBOOST_FORCEINLINE value_type operator++() volatile HBOOST_NOEXCEPT
    {
        return fetch_add(1) + 1;
    }

    HBOOST_FORCEINLINE value_type operator--(int) volatile HBOOST_NOEXCEPT
    {
        return fetch_sub(1);
    }

    HBOOST_FORCEINLINE value_type operator--() volatile HBOOST_NOEXCEPT
    {
        return fetch_sub(1) - 1;
    }

    HBOOST_FORCEINLINE value_type operator+=(difference_type v) volatile HBOOST_NOEXCEPT
    {
        return fetch_add(v) + v;
    }

    HBOOST_FORCEINLINE value_type operator-=(difference_type v) volatile HBOOST_NOEXCEPT
    {
        return fetch_sub(v) - v;
    }

    HBOOST_DELETED_FUNCTION(base_atomic(base_atomic const&))
    HBOOST_DELETED_FUNCTION(base_atomic& operator=(base_atomic const&))
};


//! Implementation for void pointers
template< >
class base_atomic< void*, void* >
{
private:
    typedef void* value_type;
    typedef std::ptrdiff_t difference_type;
    typedef atomics::detail::operations< storage_size_of< value_type >::value, false > operations;

protected:
    typedef value_type value_arg_type;

public:
    typedef operations::storage_type storage_type;

protected:
    operations::aligned_storage_type m_storage;

public:
    HBOOST_DEFAULTED_FUNCTION(base_atomic(), {})
    HBOOST_FORCEINLINE explicit base_atomic(value_type const& v) HBOOST_NOEXCEPT : m_storage(atomics::detail::bitwise_cast< storage_type >(v))
    {
    }

    HBOOST_FORCEINLINE void store(value_type v, memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        HBOOST_ASSERT(order != memory_order_consume);
        HBOOST_ASSERT(order != memory_order_acquire);
        HBOOST_ASSERT(order != memory_order_acq_rel);

        operations::store(m_storage.value, atomics::detail::bitwise_cast< storage_type >(v), order);
    }

    HBOOST_FORCEINLINE value_type load(memory_order order = memory_order_seq_cst) const volatile HBOOST_NOEXCEPT
    {
        HBOOST_ASSERT(order != memory_order_release);
        HBOOST_ASSERT(order != memory_order_acq_rel);

        return atomics::detail::bitwise_cast< value_type >(operations::load(m_storage.value, order));
    }

    HBOOST_FORCEINLINE value_type fetch_add(difference_type v, memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        return atomics::detail::bitwise_cast< value_type >(operations::fetch_add(m_storage.value, static_cast< storage_type >(v), order));
    }

    HBOOST_FORCEINLINE value_type fetch_sub(difference_type v, memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        return atomics::detail::bitwise_cast< value_type >(operations::fetch_sub(m_storage.value, static_cast< storage_type >(v), order));
    }

    HBOOST_FORCEINLINE value_type exchange(value_type v, memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        return atomics::detail::bitwise_cast< value_type >(operations::exchange(m_storage.value, atomics::detail::bitwise_cast< storage_type >(v), order));
    }

    HBOOST_FORCEINLINE bool compare_exchange_strong(value_type& expected, value_type desired, memory_order success_order, memory_order failure_order) volatile HBOOST_NOEXCEPT
    {
        HBOOST_ASSERT(failure_order != memory_order_release);
        HBOOST_ASSERT(failure_order != memory_order_acq_rel);
        HBOOST_ASSERT(cas_failure_order_must_not_be_stronger_than_success_order(success_order, failure_order));

        storage_type old_value = atomics::detail::bitwise_cast< storage_type >(expected);
        const bool res = operations::compare_exchange_strong(m_storage.value, old_value, atomics::detail::bitwise_cast< storage_type >(desired), success_order, failure_order);
        expected = atomics::detail::bitwise_cast< value_type >(old_value);
        return res;
    }

    HBOOST_FORCEINLINE bool compare_exchange_strong(value_type& expected, value_type desired, memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        return compare_exchange_strong(expected, desired, order, atomics::detail::deduce_failure_order(order));
    }

    HBOOST_FORCEINLINE bool compare_exchange_weak(value_type& expected, value_type desired, memory_order success_order, memory_order failure_order) volatile HBOOST_NOEXCEPT
    {
        HBOOST_ASSERT(failure_order != memory_order_release);
        HBOOST_ASSERT(failure_order != memory_order_acq_rel);
        HBOOST_ASSERT(cas_failure_order_must_not_be_stronger_than_success_order(success_order, failure_order));

        storage_type old_value = atomics::detail::bitwise_cast< storage_type >(expected);
        const bool res = operations::compare_exchange_weak(m_storage.value, old_value, atomics::detail::bitwise_cast< storage_type >(desired), success_order, failure_order);
        expected = atomics::detail::bitwise_cast< value_type >(old_value);
        return res;
    }

    HBOOST_FORCEINLINE bool compare_exchange_weak(value_type& expected, value_type desired, memory_order order = memory_order_seq_cst) volatile HBOOST_NOEXCEPT
    {
        return compare_exchange_weak(expected, desired, order, atomics::detail::deduce_failure_order(order));
    }

    HBOOST_FORCEINLINE bool is_lock_free() const volatile HBOOST_NOEXCEPT
    {
        return operations::is_lock_free(m_storage.value);
    }

    HBOOST_FORCEINLINE value_type operator++(int) volatile HBOOST_NOEXCEPT
    {
        return fetch_add(1);
    }

    HBOOST_FORCEINLINE value_type operator++() volatile HBOOST_NOEXCEPT
    {
        return (char*)fetch_add(1) + 1;
    }

    HBOOST_FORCEINLINE value_type operator--(int) volatile HBOOST_NOEXCEPT
    {
        return fetch_sub(1);
    }

    HBOOST_FORCEINLINE value_type operator--() volatile HBOOST_NOEXCEPT
    {
        return (char*)fetch_sub(1) - 1;
    }

    HBOOST_FORCEINLINE value_type operator+=(difference_type v) volatile HBOOST_NOEXCEPT
    {
        return (char*)fetch_add(v) + v;
    }

    HBOOST_FORCEINLINE value_type operator-=(difference_type v) volatile HBOOST_NOEXCEPT
    {
        return (char*)fetch_sub(v) - v;
    }

    HBOOST_DELETED_FUNCTION(base_atomic(base_atomic const&))
    HBOOST_DELETED_FUNCTION(base_atomic& operator=(base_atomic const&))
};

} // namespace detail

template< typename T >
class atomic :
    public atomics::detail::base_atomic< T, typename atomics::detail::classify< T >::type >
{
private:
    typedef T value_type;
    typedef atomics::detail::base_atomic< T, typename atomics::detail::classify< T >::type > base_type;
    typedef typename base_type::value_arg_type value_arg_type;

public:
    typedef typename base_type::storage_type storage_type;

public:
    HBOOST_DEFAULTED_FUNCTION(atomic(), HBOOST_NOEXCEPT {})

    // NOTE: The constructor is made explicit because gcc 4.7 complains that
    //       operator=(value_arg_type) is considered ambiguous with operator=(atomic const&)
    //       in assignment expressions, even though conversion to atomic<> is less preferred
    //       than conversion to value_arg_type.
    HBOOST_FORCEINLINE explicit HBOOST_CONSTEXPR atomic(value_arg_type v) HBOOST_NOEXCEPT : base_type(v) {}

    HBOOST_FORCEINLINE value_type operator= (value_arg_type v) volatile HBOOST_NOEXCEPT
    {
        this->store(v);
        return v;
    }

    HBOOST_FORCEINLINE operator value_type() volatile const HBOOST_NOEXCEPT
    {
        return this->load();
    }

    HBOOST_FORCEINLINE storage_type& storage() HBOOST_NOEXCEPT { return this->m_storage.value; }
    HBOOST_FORCEINLINE storage_type volatile& storage() volatile HBOOST_NOEXCEPT { return this->m_storage.value; }
    HBOOST_FORCEINLINE storage_type const& storage() const HBOOST_NOEXCEPT { return this->m_storage.value; }
    HBOOST_FORCEINLINE storage_type const volatile& storage() const volatile HBOOST_NOEXCEPT { return this->m_storage.value; }

    HBOOST_DELETED_FUNCTION(atomic(atomic const&))
    HBOOST_DELETED_FUNCTION(atomic& operator= (atomic const&))
    HBOOST_DELETED_FUNCTION(atomic& operator= (atomic const&) volatile)
};

typedef atomic< char > atomic_char;
typedef atomic< unsigned char > atomic_uchar;
typedef atomic< signed char > atomic_schar;
typedef atomic< uint8_t > atomic_uint8_t;
typedef atomic< int8_t > atomic_int8_t;
typedef atomic< unsigned short > atomic_ushort;
typedef atomic< short > atomic_short;
typedef atomic< uint16_t > atomic_uint16_t;
typedef atomic< int16_t > atomic_int16_t;
typedef atomic< unsigned int > atomic_uint;
typedef atomic< int > atomic_int;
typedef atomic< uint32_t > atomic_uint32_t;
typedef atomic< int32_t > atomic_int32_t;
typedef atomic< unsigned long > atomic_ulong;
typedef atomic< long > atomic_long;
typedef atomic< uint64_t > atomic_uint64_t;
typedef atomic< int64_t > atomic_int64_t;
#ifdef HBOOST_HAS_LONG_LONG
typedef atomic< hboost::ulong_long_type > atomic_ullong;
typedef atomic< hboost::long_long_type > atomic_llong;
#endif
typedef atomic< void* > atomic_address;
typedef atomic< bool > atomic_bool;
typedef atomic< wchar_t > atomic_wchar_t;
#if !defined(HBOOST_NO_CXX11_CHAR16_T)
typedef atomic< char16_t > atomic_char16_t;
#endif
#if !defined(HBOOST_NO_CXX11_CHAR32_T)
typedef atomic< char32_t > atomic_char32_t;
#endif

typedef atomic< int_least8_t > atomic_int_least8_t;
typedef atomic< uint_least8_t > atomic_uint_least8_t;
typedef atomic< int_least16_t > atomic_int_least16_t;
typedef atomic< uint_least16_t > atomic_uint_least16_t;
typedef atomic< int_least32_t > atomic_int_least32_t;
typedef atomic< uint_least32_t > atomic_uint_least32_t;
typedef atomic< int_least64_t > atomic_int_least64_t;
typedef atomic< uint_least64_t > atomic_uint_least64_t;
typedef atomic< int_fast8_t > atomic_int_fast8_t;
typedef atomic< uint_fast8_t > atomic_uint_fast8_t;
typedef atomic< int_fast16_t > atomic_int_fast16_t;
typedef atomic< uint_fast16_t > atomic_uint_fast16_t;
typedef atomic< int_fast32_t > atomic_int_fast32_t;
typedef atomic< uint_fast32_t > atomic_uint_fast32_t;
typedef atomic< int_fast64_t > atomic_int_fast64_t;
typedef atomic< uint_fast64_t > atomic_uint_fast64_t;
typedef atomic< intmax_t > atomic_intmax_t;
typedef atomic< uintmax_t > atomic_uintmax_t;

typedef atomic< std::size_t > atomic_size_t;
typedef atomic< std::ptrdiff_t > atomic_ptrdiff_t;

#if defined(HBOOST_HAS_INTPTR_T)
typedef atomic< intptr_t > atomic_intptr_t;
typedef atomic< uintptr_t > atomic_uintptr_t;
#endif

} // namespace atomics
} // namespace hboost

#if defined(HBOOST_MSVC)
#pragma warning(pop)
#endif

#endif // HBOOST_ATOMIC_DETAIL_ATOMIC_TEMPLATE_HPP_INCLUDED_

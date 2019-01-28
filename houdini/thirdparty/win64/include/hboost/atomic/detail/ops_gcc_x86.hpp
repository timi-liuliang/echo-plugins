/*
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * Copyright (c) 2009 Helge Bahmann
 * Copyright (c) 2012 Tim Blechmann
 * Copyright (c) 2014 Andrey Semashev
 */
/*!
 * \file   atomic/detail/ops_gcc_x86.hpp
 *
 * This header contains implementation of the \c operations template.
 */

#ifndef HBOOST_ATOMIC_DETAIL_OPS_GCC_X86_HPP_INCLUDED_
#define HBOOST_ATOMIC_DETAIL_OPS_GCC_X86_HPP_INCLUDED_

#include <hboost/memory_order.hpp>
#include <hboost/atomic/detail/config.hpp>
#include <hboost/atomic/detail/storage_type.hpp>
#include <hboost/atomic/detail/operations_fwd.hpp>
#include <hboost/atomic/capabilities.hpp>
#if defined(HBOOST_ATOMIC_DETAIL_X86_HAS_CMPXCHG8B) || defined(HBOOST_ATOMIC_DETAIL_X86_HAS_CMPXCHG16B)
#include <hboost/atomic/detail/ops_gcc_x86_dcas.hpp>
#include <hboost/atomic/detail/ops_cas_based.hpp>
#endif

#ifdef HBOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

#if defined(__x86_64__)
#define HBOOST_ATOMIC_DETAIL_TEMP_CAS_REGISTER "rdx"
#else
#define HBOOST_ATOMIC_DETAIL_TEMP_CAS_REGISTER "edx"
#endif

namespace hboost {
namespace atomics {
namespace detail {

struct gcc_x86_operations_base
{
    static HBOOST_FORCEINLINE void fence_before(memory_order order) HBOOST_NOEXCEPT
    {
        if ((order & memory_order_release) != 0)
            __asm__ __volatile__ ("" ::: "memory");
    }

    static HBOOST_FORCEINLINE void fence_after(memory_order order) HBOOST_NOEXCEPT
    {
        if ((order & memory_order_acquire) != 0)
            __asm__ __volatile__ ("" ::: "memory");
    }
};

template< typename T, typename Derived >
struct gcc_x86_operations :
    public gcc_x86_operations_base
{
    typedef T storage_type;

    static HBOOST_FORCEINLINE void store(storage_type volatile& storage, storage_type v, memory_order order) HBOOST_NOEXCEPT
    {
        if (order != memory_order_seq_cst)
        {
            fence_before(order);
            storage = v;
            fence_after(order);
        }
        else
        {
            Derived::exchange(storage, v, order);
        }
    }

    static HBOOST_FORCEINLINE storage_type load(storage_type const volatile& storage, memory_order order) HBOOST_NOEXCEPT
    {
        storage_type v = storage;
        fence_after(order);
        return v;
    }

    static HBOOST_FORCEINLINE storage_type fetch_sub(storage_type volatile& storage, storage_type v, memory_order order) HBOOST_NOEXCEPT
    {
        return Derived::fetch_add(storage, -v, order);
    }

    static HBOOST_FORCEINLINE bool compare_exchange_weak(
        storage_type volatile& storage, storage_type& expected, storage_type desired, memory_order success_order, memory_order failure_order) HBOOST_NOEXCEPT
    {
        return Derived::compare_exchange_strong(storage, expected, desired, success_order, failure_order);
    }

    static HBOOST_FORCEINLINE bool test_and_set(storage_type volatile& storage, memory_order order) HBOOST_NOEXCEPT
    {
        return !!Derived::exchange(storage, (storage_type)1, order);
    }

    static HBOOST_FORCEINLINE void clear(storage_type volatile& storage, memory_order order) HBOOST_NOEXCEPT
    {
        store(storage, (storage_type)0, order);
    }

    static HBOOST_FORCEINLINE bool is_lock_free(storage_type const volatile&) HBOOST_NOEXCEPT
    {
        return true;
    }
};

template< bool Signed >
struct operations< 1u, Signed > :
    public gcc_x86_operations< typename make_storage_type< 1u, Signed >::type, operations< 1u, Signed > >
{
    typedef gcc_x86_operations< typename make_storage_type< 1u, Signed >::type, operations< 1u, Signed > > base_type;
    typedef typename base_type::storage_type storage_type;
    typedef typename make_storage_type< 1u, Signed >::aligned aligned_storage_type;

    static HBOOST_FORCEINLINE storage_type fetch_add(storage_type volatile& storage, storage_type v, memory_order) HBOOST_NOEXCEPT
    {
        __asm__ __volatile__
        (
            "lock; xaddb %0, %1"
            : "+q" (v), "+m" (storage)
            :
            : HBOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC_COMMA "memory"
        );
        return v;
    }

    static HBOOST_FORCEINLINE storage_type exchange(storage_type volatile& storage, storage_type v, memory_order) HBOOST_NOEXCEPT
    {
        __asm__ __volatile__
        (
            "xchgb %0, %1"
            : "+q" (v), "+m" (storage)
            :
            : "memory"
        );
        return v;
    }

    static HBOOST_FORCEINLINE bool compare_exchange_strong(
        storage_type volatile& storage, storage_type& expected, storage_type desired, memory_order, memory_order) HBOOST_NOEXCEPT
    {
        storage_type previous = expected;
        bool success;
        __asm__ __volatile__
        (
            "lock; cmpxchgb %3, %1\n\t"
            "sete %2"
            : "+a" (previous), "+m" (storage), "=q" (success)
            : "q" (desired)
            : HBOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC_COMMA "memory"
        );
        expected = previous;
        return success;
    }

#define HBOOST_ATOMIC_DETAIL_CAS_LOOP(op, argument, result)\
    __asm__ __volatile__\
    (\
        "xor %%" HBOOST_ATOMIC_DETAIL_TEMP_CAS_REGISTER ", %%" HBOOST_ATOMIC_DETAIL_TEMP_CAS_REGISTER "\n\t"\
        ".align 16\n\t"\
        "1: movb %[arg], %%dl\n\t"\
        op " %%al, %%dl\n\t"\
        "lock; cmpxchgb %%dl, %[storage]\n\t"\
        "jne 1b"\
        : [res] "+a" (result), [storage] "+m" (storage)\
        : [arg] "q" (argument)\
        : HBOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC_COMMA HBOOST_ATOMIC_DETAIL_TEMP_CAS_REGISTER, "memory"\
    )

    static HBOOST_FORCEINLINE storage_type fetch_and(storage_type volatile& storage, storage_type v, memory_order) HBOOST_NOEXCEPT
    {
        storage_type res = storage;
        HBOOST_ATOMIC_DETAIL_CAS_LOOP("andb", v, res);
        return res;
    }

    static HBOOST_FORCEINLINE storage_type fetch_or(storage_type volatile& storage, storage_type v, memory_order) HBOOST_NOEXCEPT
    {
        storage_type res = storage;
        HBOOST_ATOMIC_DETAIL_CAS_LOOP("orb", v, res);
        return res;
    }

    static HBOOST_FORCEINLINE storage_type fetch_xor(storage_type volatile& storage, storage_type v, memory_order) HBOOST_NOEXCEPT
    {
        storage_type res = storage;
        HBOOST_ATOMIC_DETAIL_CAS_LOOP("xorb", v, res);
        return res;
    }

#undef HBOOST_ATOMIC_DETAIL_CAS_LOOP
};

template< bool Signed >
struct operations< 2u, Signed > :
    public gcc_x86_operations< typename make_storage_type< 2u, Signed >::type, operations< 2u, Signed > >
{
    typedef gcc_x86_operations< typename make_storage_type< 2u, Signed >::type, operations< 2u, Signed > > base_type;
    typedef typename base_type::storage_type storage_type;
    typedef typename make_storage_type< 2u, Signed >::aligned aligned_storage_type;

    static HBOOST_FORCEINLINE storage_type fetch_add(storage_type volatile& storage, storage_type v, memory_order) HBOOST_NOEXCEPT
    {
        __asm__ __volatile__
        (
            "lock; xaddw %0, %1"
            : "+q" (v), "+m" (storage)
            :
            : HBOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC_COMMA "memory"
        );
        return v;
    }

    static HBOOST_FORCEINLINE storage_type exchange(storage_type volatile& storage, storage_type v, memory_order) HBOOST_NOEXCEPT
    {
        __asm__ __volatile__
        (
            "xchgw %0, %1"
            : "+q" (v), "+m" (storage)
            :
            : "memory"
        );
        return v;
    }

    static HBOOST_FORCEINLINE bool compare_exchange_strong(
        storage_type volatile& storage, storage_type& expected, storage_type desired, memory_order, memory_order) HBOOST_NOEXCEPT
    {
        storage_type previous = expected;
        bool success;
        __asm__ __volatile__
        (
            "lock; cmpxchgw %3, %1\n\t"
            "sete %2"
            : "+a" (previous), "+m" (storage), "=q" (success)
            : "q" (desired)
            : HBOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC_COMMA "memory"
        );
        expected = previous;
        return success;
    }

#define HBOOST_ATOMIC_DETAIL_CAS_LOOP(op, argument, result)\
    __asm__ __volatile__\
    (\
        "xor %%" HBOOST_ATOMIC_DETAIL_TEMP_CAS_REGISTER ", %%" HBOOST_ATOMIC_DETAIL_TEMP_CAS_REGISTER "\n\t"\
        ".align 16\n\t"\
        "1: movw %[arg], %%dx\n\t"\
        op " %%ax, %%dx\n\t"\
        "lock; cmpxchgw %%dx, %[storage]\n\t"\
        "jne 1b"\
        : [res] "+a" (result), [storage] "+m" (storage)\
        : [arg] "q" (argument)\
        : HBOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC_COMMA HBOOST_ATOMIC_DETAIL_TEMP_CAS_REGISTER, "memory"\
    )

    static HBOOST_FORCEINLINE storage_type fetch_and(storage_type volatile& storage, storage_type v, memory_order) HBOOST_NOEXCEPT
    {
        storage_type res = storage;
        HBOOST_ATOMIC_DETAIL_CAS_LOOP("andw", v, res);
        return res;
    }

    static HBOOST_FORCEINLINE storage_type fetch_or(storage_type volatile& storage, storage_type v, memory_order) HBOOST_NOEXCEPT
    {
        storage_type res = storage;
        HBOOST_ATOMIC_DETAIL_CAS_LOOP("orw", v, res);
        return res;
    }

    static HBOOST_FORCEINLINE storage_type fetch_xor(storage_type volatile& storage, storage_type v, memory_order) HBOOST_NOEXCEPT
    {
        storage_type res = storage;
        HBOOST_ATOMIC_DETAIL_CAS_LOOP("xorw", v, res);
        return res;
    }

#undef HBOOST_ATOMIC_DETAIL_CAS_LOOP
};

template< bool Signed >
struct operations< 4u, Signed > :
    public gcc_x86_operations< typename make_storage_type< 4u, Signed >::type, operations< 4u, Signed > >
{
    typedef gcc_x86_operations< typename make_storage_type< 4u, Signed >::type, operations< 4u, Signed > > base_type;
    typedef typename base_type::storage_type storage_type;
    typedef typename make_storage_type< 4u, Signed >::aligned aligned_storage_type;

    static HBOOST_FORCEINLINE storage_type fetch_add(storage_type volatile& storage, storage_type v, memory_order) HBOOST_NOEXCEPT
    {
        __asm__ __volatile__
        (
            "lock; xaddl %0, %1"
            : "+r" (v), "+m" (storage)
            :
            : HBOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC_COMMA "memory"
        );
        return v;
    }

    static HBOOST_FORCEINLINE storage_type exchange(storage_type volatile& storage, storage_type v, memory_order) HBOOST_NOEXCEPT
    {
        __asm__ __volatile__
        (
            "xchgl %0, %1"
            : "+r" (v), "+m" (storage)
            :
            : "memory"
        );
        return v;
    }

    static HBOOST_FORCEINLINE bool compare_exchange_strong(
        storage_type volatile& storage, storage_type& expected, storage_type desired, memory_order, memory_order) HBOOST_NOEXCEPT
    {
        storage_type previous = expected;
        bool success;
        __asm__ __volatile__
        (
            "lock; cmpxchgl %3, %1\n\t"
            "sete %2"
            : "+a" (previous), "+m" (storage), "=q" (success)
            : "r" (desired)
            : HBOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC_COMMA "memory"
        );
        expected = previous;
        return success;
    }

#define HBOOST_ATOMIC_DETAIL_CAS_LOOP(op, argument, result)\
    __asm__ __volatile__\
    (\
        "xor %%" HBOOST_ATOMIC_DETAIL_TEMP_CAS_REGISTER ", %%" HBOOST_ATOMIC_DETAIL_TEMP_CAS_REGISTER "\n\t"\
        ".align 16\n\t"\
        "1: movl %[arg], %%edx\n\t"\
        op " %%eax, %%edx\n\t"\
        "lock; cmpxchgl %%edx, %[storage]\n\t"\
        "jne 1b"\
        : [res] "+a" (result), [storage] "+m" (storage)\
        : [arg] "r" (argument)\
        : HBOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC_COMMA HBOOST_ATOMIC_DETAIL_TEMP_CAS_REGISTER, "memory"\
    )

    static HBOOST_FORCEINLINE storage_type fetch_and(storage_type volatile& storage, storage_type v, memory_order) HBOOST_NOEXCEPT
    {
        storage_type res = storage;
        HBOOST_ATOMIC_DETAIL_CAS_LOOP("andl", v, res);
        return res;
    }

    static HBOOST_FORCEINLINE storage_type fetch_or(storage_type volatile& storage, storage_type v, memory_order) HBOOST_NOEXCEPT
    {
        storage_type res = storage;
        HBOOST_ATOMIC_DETAIL_CAS_LOOP("orl", v, res);
        return res;
    }

    static HBOOST_FORCEINLINE storage_type fetch_xor(storage_type volatile& storage, storage_type v, memory_order) HBOOST_NOEXCEPT
    {
        storage_type res = storage;
        HBOOST_ATOMIC_DETAIL_CAS_LOOP("xorl", v, res);
        return res;
    }

#undef HBOOST_ATOMIC_DETAIL_CAS_LOOP
};

#if defined(HBOOST_ATOMIC_DETAIL_X86_HAS_CMPXCHG8B)

template< bool Signed >
struct operations< 8u, Signed > :
    public cas_based_operations< gcc_dcas_x86< Signed > >
{
};

#elif defined(__x86_64__)

template< bool Signed >
struct operations< 8u, Signed > :
    public gcc_x86_operations< typename make_storage_type< 8u, Signed >::type, operations< 8u, Signed > >
{
    typedef gcc_x86_operations< typename make_storage_type< 8u, Signed >::type, operations< 8u, Signed > > base_type;
    typedef typename base_type::storage_type storage_type;
    typedef typename make_storage_type< 8u, Signed >::aligned aligned_storage_type;

    static HBOOST_FORCEINLINE storage_type fetch_add(storage_type volatile& storage, storage_type v, memory_order) HBOOST_NOEXCEPT
    {
        __asm__ __volatile__
        (
            "lock; xaddq %0, %1"
            : "+r" (v), "+m" (storage)
            :
            : HBOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC_COMMA "memory"
        );
        return v;
    }

    static HBOOST_FORCEINLINE storage_type exchange(storage_type volatile& storage, storage_type v, memory_order) HBOOST_NOEXCEPT
    {
        __asm__ __volatile__
        (
            "xchgq %0, %1"
            : "+r" (v), "+m" (storage)
            :
            : "memory"
        );
        return v;
    }

    static HBOOST_FORCEINLINE bool compare_exchange_strong(
        storage_type volatile& storage, storage_type& expected, storage_type desired, memory_order, memory_order) HBOOST_NOEXCEPT
    {
        storage_type previous = expected;
        bool success;
        __asm__ __volatile__
        (
            "lock; cmpxchgq %3, %1\n\t"
            "sete %2"
            : "+a" (previous), "+m" (storage), "=q" (success)
            : "r" (desired)
            : HBOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC_COMMA "memory"
        );
        expected = previous;
        return success;
    }

#define HBOOST_ATOMIC_DETAIL_CAS_LOOP(op, argument, result)\
    __asm__ __volatile__\
    (\
        "xor %%" HBOOST_ATOMIC_DETAIL_TEMP_CAS_REGISTER ", %%" HBOOST_ATOMIC_DETAIL_TEMP_CAS_REGISTER "\n\t"\
        ".align 16\n\t"\
        "1: movq %[arg], %%rdx\n\t"\
        op " %%rax, %%rdx\n\t"\
        "lock; cmpxchgq %%rdx, %[storage]\n\t"\
        "jne 1b"\
        : [res] "+a" (result), [storage] "+m" (storage)\
        : [arg] "r" (argument)\
        : HBOOST_ATOMIC_DETAIL_ASM_CLOBBER_CC_COMMA HBOOST_ATOMIC_DETAIL_TEMP_CAS_REGISTER, "memory"\
    )

    static HBOOST_FORCEINLINE storage_type fetch_and(storage_type volatile& storage, storage_type v, memory_order) HBOOST_NOEXCEPT
    {
        storage_type res = storage;
        HBOOST_ATOMIC_DETAIL_CAS_LOOP("andq", v, res);
        return res;
    }

    static HBOOST_FORCEINLINE storage_type fetch_or(storage_type volatile& storage, storage_type v, memory_order) HBOOST_NOEXCEPT
    {
        storage_type res = storage;
        HBOOST_ATOMIC_DETAIL_CAS_LOOP("orq", v, res);
        return res;
    }

    static HBOOST_FORCEINLINE storage_type fetch_xor(storage_type volatile& storage, storage_type v, memory_order) HBOOST_NOEXCEPT
    {
        storage_type res = storage;
        HBOOST_ATOMIC_DETAIL_CAS_LOOP("xorq", v, res);
        return res;
    }

#undef HBOOST_ATOMIC_DETAIL_CAS_LOOP
};

#endif

#if defined(HBOOST_ATOMIC_DETAIL_X86_HAS_CMPXCHG16B)

template< bool Signed >
struct operations< 16u, Signed > :
    public cas_based_operations< gcc_dcas_x86_64< Signed > >
{
};

#endif // defined(HBOOST_ATOMIC_DETAIL_X86_HAS_CMPXCHG16B)

HBOOST_FORCEINLINE void thread_fence(memory_order order) HBOOST_NOEXCEPT
{
    if (order == memory_order_seq_cst)
    {
        __asm__ __volatile__
        (
#if defined(__x86_64__) || defined(__SSE2__)
            "mfence\n"
#else
            "lock; addl $0, (%%esp)\n"
#endif
            ::: "memory"
        );
    }
    else if ((order & (memory_order_acquire | memory_order_release)) != 0)
    {
        __asm__ __volatile__ ("" ::: "memory");
    }
}

HBOOST_FORCEINLINE void signal_fence(memory_order order) HBOOST_NOEXCEPT
{
    if (order != memory_order_relaxed)
        __asm__ __volatile__ ("" ::: "memory");
}

} // namespace detail
} // namespace atomics
} // namespace hboost

#undef HBOOST_ATOMIC_DETAIL_TEMP_CAS_REGISTER

#endif // HBOOST_ATOMIC_DETAIL_OPS_GCC_X86_HPP_INCLUDED_
/*
 * Distributed under the Boost Software License, Version 1.0.
 * (See accompanying file LICENSE_1_0.txt or copy at
 * http://www.boost.org/LICENSE_1_0.txt)
 *
 * Copyright (c) 2011 Helge Bahmann
 * Copyright (c) 2013-2014 Andrey Semashev
 */
/*!
 * \file   atomic/detail/lockpool.hpp
 *
 * This header contains declaration of the lockpool used to emulate atomic ops.
 */

#ifndef HBOOST_ATOMIC_DETAIL_LOCKPOOL_HPP_INCLUDED_
#define HBOOST_ATOMIC_DETAIL_LOCKPOOL_HPP_INCLUDED_

#include <hboost/atomic/detail/config.hpp>
#include <hboost/atomic/detail/link.hpp>

#ifdef HBOOST_HAS_PRAGMA_ONCE
#pragma once
#endif

namespace hboost {
namespace atomics {
namespace detail {

struct lockpool
{
    class scoped_lock
    {
        void* m_lock;

    public:
        explicit HBOOST_ATOMIC_DECL scoped_lock(const volatile void* addr) HBOOST_NOEXCEPT;
        HBOOST_ATOMIC_DECL ~scoped_lock() HBOOST_NOEXCEPT;

        HBOOST_DELETED_FUNCTION(scoped_lock(scoped_lock const&))
        HBOOST_DELETED_FUNCTION(scoped_lock& operator=(scoped_lock const&))
    };

    static HBOOST_ATOMIC_DECL void thread_fence() HBOOST_NOEXCEPT;
    static HBOOST_ATOMIC_DECL void signal_fence() HBOOST_NOEXCEPT;
};

} // namespace detail
} // namespace atomics
} // namespace hboost

#endif // HBOOST_ATOMIC_DETAIL_LOCKPOOL_HPP_INCLUDED_

#ifndef HBOOST_THREAD_WIN32_MUTEX_HPP
#define HBOOST_THREAD_WIN32_MUTEX_HPP
// (C) Copyright 2005-7 Anthony Williams
// (C) Copyright 2011-2012 Vicente J. Botet Escriba
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)

#include <hboost/thread/win32/basic_timed_mutex.hpp>
#include <hboost/thread/exceptions.hpp>
#if defined HBOOST_THREAD_PROVIDES_NESTED_LOCKS
#include <hboost/thread/lock_types.hpp>
#endif
#include <hboost/thread/detail/delete.hpp>

#include <hboost/config/abi_prefix.hpp>

namespace hboost
{
    namespace detail
    {
        typedef ::hboost::detail::basic_timed_mutex underlying_mutex;
    }

    class mutex:
        public ::hboost::detail::underlying_mutex
    {
    public:
        HBOOST_THREAD_NO_COPYABLE(mutex)
        mutex()
        {
            initialize();
        }
        ~mutex()
        {
            destroy();
        }

#if defined HBOOST_THREAD_PROVIDES_NESTED_LOCKS
        typedef unique_lock<mutex> scoped_lock;
        typedef detail::try_lock_wrapper<mutex> scoped_try_lock;
#endif
    };

    typedef mutex try_mutex;

    class timed_mutex:
        public ::hboost::detail::basic_timed_mutex
    {
    public:
        HBOOST_THREAD_NO_COPYABLE(timed_mutex)
        timed_mutex()
        {
            initialize();
        }

        ~timed_mutex()
        {
            destroy();
        }

#if defined HBOOST_THREAD_PROVIDES_NESTED_LOCKS
        typedef unique_lock<timed_mutex> scoped_timed_lock;
        typedef detail::try_lock_wrapper<timed_mutex> scoped_try_lock;
        typedef scoped_timed_lock scoped_lock;
#endif
    };
}

#include <hboost/config/abi_suffix.hpp>

#endif

#ifndef HBOOST_RECURSIVE_MUTEX_WIN32_HPP
#define HBOOST_RECURSIVE_MUTEX_WIN32_HPP

//  recursive_mutex.hpp
//
//  (C) Copyright 2006-7 Anthony Williams
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)


#include <hboost/thread/win32/basic_recursive_mutex.hpp>
#include <hboost/thread/exceptions.hpp>
#include <hboost/thread/detail/delete.hpp>
#if defined HBOOST_THREAD_PROVIDES_NESTED_LOCKS
#include <hboost/thread/lock_types.hpp>
#endif

#include <hboost/config/abi_prefix.hpp>

namespace hboost
{
    class recursive_mutex:
        public ::hboost::detail::basic_recursive_mutex
    {
    public:
        HBOOST_THREAD_NO_COPYABLE(recursive_mutex)
        recursive_mutex()
        {
            ::hboost::detail::basic_recursive_mutex::initialize();
        }
        ~recursive_mutex()
        {
            ::hboost::detail::basic_recursive_mutex::destroy();
        }

#if defined HBOOST_THREAD_PROVIDES_NESTED_LOCKS
        typedef unique_lock<recursive_mutex> scoped_lock;
        typedef detail::try_lock_wrapper<recursive_mutex> scoped_try_lock;
#endif
    };

    typedef recursive_mutex recursive_try_mutex;

    class recursive_timed_mutex:
        public ::hboost::detail::basic_recursive_timed_mutex
    {
    public:
        HBOOST_THREAD_NO_COPYABLE(recursive_timed_mutex)
        recursive_timed_mutex()
        {
            ::hboost::detail::basic_recursive_timed_mutex::initialize();
        }
        ~recursive_timed_mutex()
        {
            ::hboost::detail::basic_recursive_timed_mutex::destroy();
        }

#if defined HBOOST_THREAD_PROVIDES_NESTED_LOCKS
        typedef unique_lock<recursive_timed_mutex> scoped_timed_lock;
        typedef detail::try_lock_wrapper<recursive_timed_mutex> scoped_try_lock;
        typedef scoped_timed_lock scoped_lock;
#endif
    };
}

#include <hboost/config/abi_suffix.hpp>

#endif

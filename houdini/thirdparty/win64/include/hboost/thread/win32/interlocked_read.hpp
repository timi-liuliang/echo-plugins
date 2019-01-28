#ifndef HBOOST_THREAD_DETAIL_INTERLOCKED_READ_WIN32_HPP
#define HBOOST_THREAD_DETAIL_INTERLOCKED_READ_WIN32_HPP

//  interlocked_read_win32.hpp
//
//  (C) Copyright 2005-8 Anthony Williams
//  (C) Copyright 2012 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <hboost/detail/interlocked.hpp>
#include <hboost/thread/detail/config.hpp>

#include <hboost/config/abi_prefix.hpp>

#ifdef HBOOST_MSVC

namespace hboost
{
    namespace detail
    {
        // Since VS2005 volatile reads always acquire
        inline long interlocked_read_acquire(long volatile* x) HBOOST_NOEXCEPT
        {
            long const res=*x;
            return res;
        }
        inline void* interlocked_read_acquire(void* volatile* x) HBOOST_NOEXCEPT
        {
            void* const res=*x;
            return res;
        }

        // Since VS2005 volatile writes always release
        inline void interlocked_write_release(long volatile* x,long value) HBOOST_NOEXCEPT
        {
            *x=value;
        }
        inline void interlocked_write_release(void* volatile* x,void* value) HBOOST_NOEXCEPT
        {
            *x=value;
        }
    }
}

#else

namespace hboost
{
    namespace detail
    {
        inline long interlocked_read_acquire(long volatile* x) HBOOST_NOEXCEPT
        {
            return HBOOST_INTERLOCKED_COMPARE_EXCHANGE(x,0,0);
        }
        inline void* interlocked_read_acquire(void* volatile* x) HBOOST_NOEXCEPT
        {
            return HBOOST_INTERLOCKED_COMPARE_EXCHANGE_POINTER(x,0,0);
        }
        inline void interlocked_write_release(long volatile* x,long value) HBOOST_NOEXCEPT
        {
            HBOOST_INTERLOCKED_EXCHANGE(x,value);
        }
        inline void interlocked_write_release(void* volatile* x,void* value) HBOOST_NOEXCEPT
        {
            HBOOST_INTERLOCKED_EXCHANGE_POINTER(x,value);
        }
    }
}

#endif

#include <hboost/config/abi_suffix.hpp>

#endif

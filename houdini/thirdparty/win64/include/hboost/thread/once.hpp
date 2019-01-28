#ifndef HBOOST_THREAD_ONCE_HPP
#define HBOOST_THREAD_ONCE_HPP

//  once.hpp
//
//  (C) Copyright 2006-7 Anthony Williams
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <hboost/thread/detail/config.hpp>
#include <hboost/thread/detail/platform.hpp>
#if defined(HBOOST_THREAD_PLATFORM_WIN32)
#include <hboost/thread/win32/once.hpp>
#elif defined(HBOOST_THREAD_PLATFORM_PTHREAD)
#if defined HBOOST_THREAD_ONCE_FAST_EPOCH
#include <hboost/thread/pthread/once.hpp>
#elif defined HBOOST_THREAD_ONCE_ATOMIC
#include <hboost/thread/pthread/once_atomic.hpp>
#else
#error "Once Not Implemented"
#endif
#else
#error "Boost threads unavailable on this platform"
#endif

#include <hboost/config/abi_prefix.hpp>

namespace hboost
{
  // template<class Callable, class ...Args> void
  // call_once(once_flag& flag, Callable&& func, Args&&... args);
template<typename Function>
inline void call_once(Function func,once_flag& flag)
//inline void call_once(void (*func)(),once_flag& flag)
    {
        call_once(flag,func);
    }
}

#include <hboost/config/abi_suffix.hpp>

#endif

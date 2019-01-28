#ifndef HBOOST_THREAD_SHARED_MUTEX_HPP
#define HBOOST_THREAD_SHARED_MUTEX_HPP

//  shared_mutex.hpp
//
//  (C) Copyright 2007 Anthony Williams
//  (C) Copyright 2011-2012 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)

#include <hboost/thread/detail/config.hpp>
#if defined(HBOOST_THREAD_PLATFORM_WIN32)
#if defined(HBOOST_THREAD_PROVIDES_GENERIC_SHARED_MUTEX_ON_WIN)
#include <hboost/thread/pthread/shared_mutex.hpp>
#else
#include <hboost/thread/win32/shared_mutex.hpp>
#endif
#elif defined(HBOOST_THREAD_PLATFORM_PTHREAD)
//#include <hboost/thread/v2/shared_mutex.hpp>
#include <hboost/thread/pthread/shared_mutex.hpp>
#else
#error "Boost threads unavailable on this platform"
#endif

#include <hboost/thread/lockable_traits.hpp>

namespace hboost
{
  typedef shared_mutex shared_timed_mutex;
  namespace sync
  {
#ifdef HBOOST_THREAD_NO_AUTO_DETECT_MUTEX_TYPES
    template<>
    struct is_basic_lockable<shared_mutex>
    {
      HBOOST_STATIC_CONSTANT(bool, value = true);
    };
    template<>
    struct is_lockable<shared_mutex>
    {
      HBOOST_STATIC_CONSTANT(bool, value = true);
    };
#endif

  }
}

#endif

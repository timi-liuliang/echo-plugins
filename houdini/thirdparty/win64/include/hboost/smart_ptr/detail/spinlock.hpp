#ifndef HBOOST_SMART_PTR_DETAIL_SPINLOCK_HPP_INCLUDED
#define HBOOST_SMART_PTR_DETAIL_SPINLOCK_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  hboost/detail/spinlock.hpp
//
//  Copyright (c) 2008 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
//  struct spinlock
//  {
//      void lock();
//      bool try_lock();
//      void unlock();
//
//      class scoped_lock;
//  };
//
//  #define HBOOST_DETAIL_SPINLOCK_INIT <unspecified>
//

#include <hboost/config.hpp>
#include <hboost/smart_ptr/detail/sp_has_sync.hpp>

#if defined( HBOOST_SP_USE_STD_ATOMIC )
# if !defined( __clang__ )
#   include <hboost/smart_ptr/detail/spinlock_std_atomic.hpp>
# else
//  Clang (at least up to 3.4) can't compile spinlock_pool when
//  using std::atomic, so substitute the __sync implementation instead.
#   include <hboost/smart_ptr/detail/spinlock_sync.hpp>
# endif

#elif defined( HBOOST_SP_USE_PTHREADS )
#  include <hboost/smart_ptr/detail/spinlock_pt.hpp>

#elif defined(__GNUC__) && defined( __arm__ ) && !defined( __thumb__ )
#  include <hboost/smart_ptr/detail/spinlock_gcc_arm.hpp>

#elif defined( HBOOST_SP_HAS_SYNC )
#  include <hboost/smart_ptr/detail/spinlock_sync.hpp>

#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#  include <hboost/smart_ptr/detail/spinlock_w32.hpp>

#elif defined(HBOOST_HAS_PTHREADS)
#  include <hboost/smart_ptr/detail/spinlock_pt.hpp>

#elif !defined(HBOOST_HAS_THREADS)
#  include <hboost/smart_ptr/detail/spinlock_nt.hpp>

#else
#  error Unrecognized threading platform
#endif

#endif // #ifndef HBOOST_SMART_PTR_DETAIL_SPINLOCK_HPP_INCLUDED

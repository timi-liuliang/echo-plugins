#ifndef HBOOST_SMART_PTR_DETAIL_ATOMIC_COUNT_HPP_INCLUDED
#define HBOOST_SMART_PTR_DETAIL_ATOMIC_COUNT_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  hboost/detail/atomic_count.hpp - thread/SMP safe reference counter
//
//  Copyright (c) 2001, 2002 Peter Dimov and Multi Media Ltd.
//  Copyright (c) 2013 Peter Dimov
//
//  Distributed under the Boost Software License, Version 1.0.
//  See accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt
//
//  typedef <implementation-defined> hboost::detail::atomic_count;
//
//  atomic_count a(n);
//
//    (n is convertible to long)
//
//    Effects: Constructs an atomic_count with an initial value of n
//
//  a;
//
//    Returns: (long) the current value of a
//    Memory Ordering: acquire
//
//  ++a;
//
//    Effects: Atomically increments the value of a
//    Returns: (long) the new value of a
//    Memory Ordering: acquire/release
//
//  --a;
//
//    Effects: Atomically decrements the value of a
//    Returns: (long) the new value of a
//    Memory Ordering: acquire/release
//

#include <hboost/config.hpp>
#include <hboost/smart_ptr/detail/sp_has_sync.hpp>

#if defined( HBOOST_AC_DISABLE_THREADS )
# include <hboost/smart_ptr/detail/atomic_count_nt.hpp>

#elif defined( HBOOST_AC_USE_STD_ATOMIC )
# include <hboost/smart_ptr/detail/atomic_count_std_atomic.hpp>

#elif defined( HBOOST_AC_USE_SPINLOCK )
# include <hboost/smart_ptr/detail/atomic_count_spin.hpp>

#elif defined( HBOOST_AC_USE_PTHREADS )
# include <hboost/smart_ptr/detail/atomic_count_pt.hpp>

#elif defined( HBOOST_SP_DISABLE_THREADS )
# include <hboost/smart_ptr/detail/atomic_count_nt.hpp>

#elif defined( HBOOST_SP_USE_STD_ATOMIC )
# include <hboost/smart_ptr/detail/atomic_count_std_atomic.hpp>

#elif defined( HBOOST_SP_USE_SPINLOCK )
# include <hboost/smart_ptr/detail/atomic_count_spin.hpp>

#elif defined( HBOOST_SP_USE_PTHREADS )
# include <hboost/smart_ptr/detail/atomic_count_pt.hpp>

#elif defined( HBOOST_DISABLE_THREADS ) && !defined( HBOOST_SP_ENABLE_THREADS ) && !defined( HBOOST_DISABLE_WIN32 )
# include <hboost/smart_ptr/detail/atomic_count_nt.hpp>

#elif defined( __GNUC__ ) && ( defined( __i386__ ) || defined( __x86_64__ ) ) && !defined( __PATHSCALE__ )
# include <hboost/smart_ptr/detail/atomic_count_gcc_x86.hpp>

#elif defined( HBOOST_SP_HAS_SYNC )
# include <hboost/smart_ptr/detail/atomic_count_sync.hpp>

#elif defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
# include <hboost/smart_ptr/detail/atomic_count_win32.hpp>

#elif defined(__GLIBCPP__) || defined(__GLIBCXX__)
# include <hboost/smart_ptr/detail/atomic_count_gcc.hpp>

#elif !defined( HBOOST_HAS_THREADS )
# include <hboost/smart_ptr/detail/atomic_count_nt.hpp>

#else
# include <hboost/smart_ptr/detail/atomic_count_spin.hpp>

#endif

#endif // #ifndef HBOOST_SMART_PTR_DETAIL_ATOMIC_COUNT_HPP_INCLUDED

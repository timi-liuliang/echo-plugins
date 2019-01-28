#ifndef HBOOST_SMART_PTR_DETAIL_LIGHTWEIGHT_MUTEX_HPP_INCLUDED
#define HBOOST_SMART_PTR_DETAIL_LIGHTWEIGHT_MUTEX_HPP_INCLUDED

// MS compatible compilers support #pragma once

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
# pragma once
#endif

//
//  hboost/detail/lightweight_mutex.hpp - lightweight mutex
//
//  Copyright (c) 2002, 2003 Peter Dimov and Multi Media Ltd.
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  typedef <unspecified> hboost::detail::lightweight_mutex;
//
//  hboost::detail::lightweight_mutex is a header-only implementation of
//  a subset of the Mutex concept requirements:
//
//  http://www.boost.org/doc/html/threads/concepts.html#threads.concepts.Mutex
//
//  It maps to a CRITICAL_SECTION on Windows or a pthread_mutex on POSIX.
//

#include <hboost/config.hpp>

#if !defined(HBOOST_HAS_THREADS)
#  include <hboost/smart_ptr/detail/lwm_nop.hpp>
#elif defined(HBOOST_HAS_PTHREADS)
#  include <hboost/smart_ptr/detail/lwm_pthreads.hpp>
#elif defined(HBOOST_HAS_WINTHREADS) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#  include <hboost/smart_ptr/detail/lwm_win32_cs.hpp>
#else
// Use #define HBOOST_DISABLE_THREADS to avoid the error
#  error Unrecognized threading platform
#endif

#endif // #ifndef HBOOST_SMART_PTR_DETAIL_LIGHTWEIGHT_MUTEX_HPP_INCLUDED

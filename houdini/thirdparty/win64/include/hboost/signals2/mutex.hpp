//
//  hboost/signals2/mutex.hpp - header-only mutex
//
//  Copyright (c) 2002, 2003 Peter Dimov and Multi Media Ltd.
//  Copyright (c) 2008 Frank Mori Hess
//
// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//  hboost::signals2::mutex is a modification of
//  hboost::detail::lightweight_mutex to follow the newer Lockable
//  concept of Boost.Thread.
//

#ifndef HBOOST_SIGNALS2_MUTEX_HPP
#define HBOOST_SIGNALS2_MUTEX_HPP

// MS compatible compilers support #pragma once

#if defined(_MSC_VER)
# pragma once
#endif

#include <hboost/config.hpp>

#if !defined(HBOOST_HAS_THREADS)
# include <hboost/signals2/detail/lwm_nop.hpp>
#elif defined(HBOOST_HAS_PTHREADS)
#  include <hboost/signals2/detail/lwm_pthreads.hpp>
#elif defined(HBOOST_HAS_WINTHREADS)
#  include <hboost/signals2/detail/lwm_win32_cs.hpp>
#else
// Use #define HBOOST_DISABLE_THREADS to avoid the error
#  error Unrecognized threading platform
#endif

#endif // #ifndef HBOOST_SIGNALS2_MUTEX_HPP

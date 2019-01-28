//
// detail/posix_mutex.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_POSIX_MUTEX_HPP
#define HBOOST_ASIO_DETAIL_POSIX_MUTEX_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if defined(HBOOST_ASIO_HAS_PTHREADS)

#include <pthread.h>
#include <hboost/asio/detail/noncopyable.hpp>
#include <hboost/asio/detail/scoped_lock.hpp>

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace detail {

class posix_event;

class posix_mutex
  : private noncopyable
{
public:
  typedef hboost::asio::detail::scoped_lock<posix_mutex> scoped_lock;

  // Constructor.
  HBOOST_ASIO_DECL posix_mutex();

  // Destructor.
  ~posix_mutex()
  {
    ::pthread_mutex_destroy(&mutex_); // Ignore EBUSY.
  }

  // Lock the mutex.
  void lock()
  {
    (void)::pthread_mutex_lock(&mutex_); // Ignore EINVAL.
  }

  // Unlock the mutex.
  void unlock()
  {
    (void)::pthread_mutex_unlock(&mutex_); // Ignore EINVAL.
  }

private:
  friend class posix_event;
  ::pthread_mutex_t mutex_;
};

} // namespace detail
} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#if defined(HBOOST_ASIO_HEADER_ONLY)
# include <hboost/asio/detail/impl/posix_mutex.ipp>
#endif // defined(HBOOST_ASIO_HEADER_ONLY)

#endif // defined(HBOOST_ASIO_HAS_PTHREADS)

#endif // HBOOST_ASIO_DETAIL_POSIX_MUTEX_HPP

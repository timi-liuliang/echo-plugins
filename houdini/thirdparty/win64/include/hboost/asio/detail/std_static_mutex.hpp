//
// detail/std_static_mutex.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_STD_STATIC_MUTEX_HPP
#define HBOOST_ASIO_DETAIL_STD_STATIC_MUTEX_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if defined(HBOOST_ASIO_HAS_STD_MUTEX_AND_CONDVAR)

#include <mutex>
#include <hboost/asio/detail/noncopyable.hpp>
#include <hboost/asio/detail/scoped_lock.hpp>

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace detail {

class std_event;

class std_static_mutex
  : private noncopyable
{
public:
  typedef hboost::asio::detail::scoped_lock<std_static_mutex> scoped_lock;

  // Constructor.
  std_static_mutex(int)
  {
  }

  // Destructor.
  ~std_static_mutex()
  {
  }

  // Initialise the mutex.
  void init()
  {
    // Nothing to do.
  }

  // Lock the mutex.
  void lock()
  {
    mutex_.lock();
  }

  // Unlock the mutex.
  void unlock()
  {
    mutex_.unlock();
  }

private:
  friend class std_event;
  std::mutex mutex_;
};

#define HBOOST_ASIO_STD_STATIC_MUTEX_INIT 0

} // namespace detail
} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#endif // defined(HBOOST_ASIO_HAS_STD_MUTEX_AND_CONDVAR)

#endif // HBOOST_ASIO_DETAIL_STD_STATIC_MUTEX_HPP
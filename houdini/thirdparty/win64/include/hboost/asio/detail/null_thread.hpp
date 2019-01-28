//
// detail/null_thread.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_NULL_THREAD_HPP
#define HBOOST_ASIO_DETAIL_NULL_THREAD_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if !defined(HBOOST_ASIO_HAS_THREADS)

#include <hboost/asio/detail/noncopyable.hpp>
#include <hboost/asio/detail/throw_error.hpp>
#include <hboost/asio/error.hpp>

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace detail {

class null_thread
  : private noncopyable
{
public:
  // Constructor.
  template <typename Function>
  null_thread(Function, unsigned int = 0)
  {
    hboost::asio::detail::throw_error(
        hboost::asio::error::operation_not_supported, "thread");
  }

  // Destructor.
  ~null_thread()
  {
  }

  // Wait for the thread to exit.
  void join()
  {
  }
};

} // namespace detail
} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#endif // !defined(HBOOST_ASIO_HAS_THREADS)

#endif // HBOOST_ASIO_DETAIL_NULL_THREAD_HPP

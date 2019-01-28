//
// detail/std_thread.hpp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_STD_THREAD_HPP
#define HBOOST_ASIO_DETAIL_STD_THREAD_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if defined(HBOOST_ASIO_HAS_STD_THREAD)

#include <thread>
#include <hboost/asio/detail/noncopyable.hpp>

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace detail {

class std_thread
  : private noncopyable
{
public:
  // Constructor.
  template <typename Function>
  std_thread(Function f, unsigned int = 0)
    : thread_(f)
  {
  }

  // Destructor.
  ~std_thread()
  {
    join();
  }

  // Wait for the thread to exit.
  void join()
  {
    if (thread_.joinable())
      thread_.join();
  }

private:
  std::thread thread_;
};

} // namespace detail
} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#endif // defined(HBOOST_ASIO_HAS_STD_THREAD)

#endif // HBOOST_ASIO_DETAIL_STD_THREAD_HPP

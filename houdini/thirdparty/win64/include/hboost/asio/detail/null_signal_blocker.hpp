//
// detail/null_signal_blocker.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_NULL_SIGNAL_BLOCKER_HPP
#define HBOOST_ASIO_DETAIL_NULL_SIGNAL_BLOCKER_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if !defined(HBOOST_ASIO_HAS_THREADS) \
  || defined(HBOOST_ASIO_WINDOWS) \
  || defined(HBOOST_ASIO_WINDOWS_RUNTIME) \
  || defined(__CYGWIN__) \
  || defined(__SYMBIAN32__)

#include <hboost/asio/detail/noncopyable.hpp>

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace detail {

class null_signal_blocker
  : private noncopyable
{
public:
  // Constructor blocks all signals for the calling thread.
  null_signal_blocker()
  {
  }

  // Destructor restores the previous signal mask.
  ~null_signal_blocker()
  {
  }

  // Block all signals for the calling thread.
  void block()
  {
  }

  // Restore the previous signal mask.
  void unblock()
  {
  }
};

} // namespace detail
} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#endif // !defined(HBOOST_ASIO_HAS_THREADS)
       // || defined(HBOOST_ASIO_WINDOWS)
       // || defined(HBOOST_ASIO_WINDOWS_RUNTIME)
       // || defined(__CYGWIN__)
       // || defined(__SYMBIAN32__)

#endif // HBOOST_ASIO_DETAIL_NULL_SIGNAL_BLOCKER_HPP

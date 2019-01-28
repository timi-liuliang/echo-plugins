//
// detail/signal_init.hpp
// ~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_SIGNAL_INIT_HPP
#define HBOOST_ASIO_DETAIL_SIGNAL_INIT_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if !defined(HBOOST_ASIO_WINDOWS) && !defined(__CYGWIN__)

#include <csignal>

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace detail {

template <int Signal = SIGPIPE>
class signal_init
{
public:
  // Constructor.
  signal_init()
  {
    std::signal(Signal, SIG_IGN);
  }
};

} // namespace detail
} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#endif // !defined(HBOOST_ASIO_WINDOWS) && !defined(__CYGWIN__)

#endif // HBOOST_ASIO_DETAIL_SIGNAL_INIT_HPP

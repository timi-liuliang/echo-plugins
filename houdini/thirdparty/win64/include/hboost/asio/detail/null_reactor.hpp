//
// detail/null_reactor.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_NULL_REACTOR_HPP
#define HBOOST_ASIO_DETAIL_NULL_REACTOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if defined(HBOOST_ASIO_WINDOWS_RUNTIME)

#include <hboost/asio/io_service.hpp>

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace detail {

class null_reactor
  : public hboost::asio::detail::service_base<null_reactor>
{
public:
  // Constructor.
  null_reactor(hboost::asio::io_service& io_service)
    : hboost::asio::detail::service_base<null_reactor>(io_service)
  {
  }

  // Destructor.
  ~null_reactor()
  {
  }

  // Destroy all user-defined handler objects owned by the service.
  void shutdown_service()
  {
  }

  // No-op because should never be called.
  void run(bool /*block*/, op_queue<operation>& /*ops*/)
  {
  }

  // No-op.
  void interrupt()
  {
  }
};

} // namespace detail
} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#endif // defined(HBOOST_ASIO_WINDOWS_RUNTIME)

#endif // HBOOST_ASIO_DETAIL_NULL_REACTOR_HPP

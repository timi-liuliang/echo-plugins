//
// ip/impl/host_name.ipp
// ~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_IP_IMPL_HOST_NAME_IPP
#define HBOOST_ASIO_IP_IMPL_HOST_NAME_IPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>
#include <hboost/asio/detail/socket_ops.hpp>
#include <hboost/asio/detail/throw_error.hpp>
#include <hboost/asio/detail/winsock_init.hpp>
#include <hboost/asio/ip/host_name.hpp>

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace ip {

std::string host_name()
{
  char name[1024];
  hboost::system::error_code ec;
  if (hboost::asio::detail::socket_ops::gethostname(name, sizeof(name), ec) != 0)
  {
    hboost::asio::detail::throw_error(ec);
    return std::string();
  }
  return std::string(name);
}

std::string host_name(hboost::system::error_code& ec)
{
  char name[1024];
  if (hboost::asio::detail::socket_ops::gethostname(name, sizeof(name), ec) != 0)
    return std::string();
  return std::string(name);
}

} // namespace ip
} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#endif // HBOOST_ASIO_IP_IMPL_HOST_NAME_IPP

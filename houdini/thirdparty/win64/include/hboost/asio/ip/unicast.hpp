//
// ip/unicast.hpp
// ~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_IP_UNICAST_HPP
#define HBOOST_ASIO_IP_UNICAST_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>
#include <cstddef>
#include <hboost/asio/ip/detail/socket_option.hpp>

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace ip {
namespace unicast {

/// Socket option for time-to-live associated with outgoing unicast packets.
/**
 * Implements the IPPROTO_IP/IP_UNICAST_TTL socket option.
 *
 * @par Examples
 * Setting the option:
 * @code
 * hboost::asio::ip::udp::socket socket(io_service); 
 * ...
 * hboost::asio::ip::unicast::hops option(4);
 * socket.set_option(option);
 * @endcode
 *
 * @par
 * Getting the current option value:
 * @code
 * hboost::asio::ip::udp::socket socket(io_service); 
 * ...
 * hboost::asio::ip::unicast::hops option;
 * socket.get_option(option);
 * int ttl = option.value();
 * @endcode
 *
 * @par Concepts:
 * GettableSocketOption, SettableSocketOption.
 */
#if defined(GENERATING_DOCUMENTATION)
typedef implementation_defined hops;
#else
typedef hboost::asio::ip::detail::socket_option::unicast_hops<
  HBOOST_ASIO_OS_DEF(IPPROTO_IP),
  HBOOST_ASIO_OS_DEF(IP_TTL),
  HBOOST_ASIO_OS_DEF(IPPROTO_IPV6),
  HBOOST_ASIO_OS_DEF(IPV6_UNICAST_HOPS)> hops;
#endif

} // namespace unicast
} // namespace ip
} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#endif // HBOOST_ASIO_IP_UNICAST_HPP

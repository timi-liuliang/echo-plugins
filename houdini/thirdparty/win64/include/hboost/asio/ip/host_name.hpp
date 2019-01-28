//
// ip/host_name.hpp
// ~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_IP_HOST_NAME_HPP
#define HBOOST_ASIO_IP_HOST_NAME_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>
#include <string>
#include <hboost/system/error_code.hpp>

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace ip {

/// Get the current host name.
HBOOST_ASIO_DECL std::string host_name();

/// Get the current host name.
HBOOST_ASIO_DECL std::string host_name(hboost::system::error_code& ec);

} // namespace ip
} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#if defined(HBOOST_ASIO_HEADER_ONLY)
# include <hboost/asio/ip/impl/host_name.ipp>
#endif // defined(HBOOST_ASIO_HEADER_ONLY)

#endif // HBOOST_ASIO_IP_HOST_NAME_HPP

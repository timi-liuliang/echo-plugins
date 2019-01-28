//
// detail/descriptor_ops.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_DETAIL_DESCRIPTOR_OPS_HPP
#define HBOOST_ASIO_DETAIL_DESCRIPTOR_OPS_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if !defined(HBOOST_ASIO_WINDOWS) \
  && !defined(HBOOST_ASIO_WINDOWS_RUNTIME) \
  && !defined(__CYGWIN__)

#include <cstddef>
#include <hboost/system/error_code.hpp>
#include <hboost/asio/detail/socket_types.hpp>

#include <hboost/asio/detail/push_options.hpp>

namespace hboost {
namespace asio {
namespace detail {
namespace descriptor_ops {

// Descriptor state bits.
enum
{
  // The user wants a non-blocking descriptor.
  user_set_non_blocking = 1,

  // The descriptor has been set non-blocking.
  internal_non_blocking = 2,

  // Helper "state" used to determine whether the descriptor is non-blocking.
  non_blocking = user_set_non_blocking | internal_non_blocking,

  // The descriptor may have been dup()-ed.
  possible_dup = 4
};

typedef unsigned char state_type;

template <typename ReturnType>
inline ReturnType error_wrapper(ReturnType return_value,
    hboost::system::error_code& ec)
{
  ec = hboost::system::error_code(errno,
      hboost::asio::error::get_system_category());
  return return_value;
}

HBOOST_ASIO_DECL int open(const char* path, int flags,
    hboost::system::error_code& ec);

HBOOST_ASIO_DECL int close(int d, state_type& state,
    hboost::system::error_code& ec);

HBOOST_ASIO_DECL bool set_user_non_blocking(int d,
    state_type& state, bool value, hboost::system::error_code& ec);

HBOOST_ASIO_DECL bool set_internal_non_blocking(int d,
    state_type& state, bool value, hboost::system::error_code& ec);

typedef iovec buf;

HBOOST_ASIO_DECL std::size_t sync_read(int d, state_type state, buf* bufs,
    std::size_t count, bool all_empty, hboost::system::error_code& ec);

HBOOST_ASIO_DECL bool non_blocking_read(int d, buf* bufs, std::size_t count,
    hboost::system::error_code& ec, std::size_t& bytes_transferred);

HBOOST_ASIO_DECL std::size_t sync_write(int d, state_type state,
    const buf* bufs, std::size_t count, bool all_empty,
    hboost::system::error_code& ec);

HBOOST_ASIO_DECL bool non_blocking_write(int d,
    const buf* bufs, std::size_t count,
    hboost::system::error_code& ec, std::size_t& bytes_transferred);

HBOOST_ASIO_DECL int ioctl(int d, state_type& state, long cmd,
    ioctl_arg_type* arg, hboost::system::error_code& ec);

HBOOST_ASIO_DECL int fcntl(int d, int cmd, hboost::system::error_code& ec);

HBOOST_ASIO_DECL int fcntl(int d, int cmd,
    long arg, hboost::system::error_code& ec);

HBOOST_ASIO_DECL int poll_read(int d,
    state_type state, hboost::system::error_code& ec);

HBOOST_ASIO_DECL int poll_write(int d,
    state_type state, hboost::system::error_code& ec);

} // namespace descriptor_ops
} // namespace detail
} // namespace asio
} // namespace hboost

#include <hboost/asio/detail/pop_options.hpp>

#if defined(HBOOST_ASIO_HEADER_ONLY)
# include <hboost/asio/detail/impl/descriptor_ops.ipp>
#endif // defined(HBOOST_ASIO_HEADER_ONLY)

#endif // !defined(HBOOST_ASIO_WINDOWS)
       //   && !defined(HBOOST_ASIO_WINDOWS_RUNTIME)
       //   && !defined(__CYGWIN__)

#endif // HBOOST_ASIO_DETAIL_DESCRIPTOR_OPS_HPP

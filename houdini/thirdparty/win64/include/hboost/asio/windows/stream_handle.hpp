//
// windows/stream_handle.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_WINDOWS_STREAM_HANDLE_HPP
#define HBOOST_ASIO_WINDOWS_STREAM_HANDLE_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if defined(HBOOST_ASIO_HAS_WINDOWS_STREAM_HANDLE) \
  || defined(GENERATING_DOCUMENTATION)

#include <hboost/asio/windows/basic_stream_handle.hpp>

namespace hboost {
namespace asio {
namespace windows {

/// Typedef for the typical usage of a stream-oriented handle.
typedef basic_stream_handle<> stream_handle;

} // namespace windows
} // namespace asio
} // namespace hboost

#endif // defined(HBOOST_ASIO_HAS_WINDOWS_STREAM_HANDLE)
       //   || defined(GENERATING_DOCUMENTATION)

#endif // HBOOST_ASIO_WINDOWS_STREAM_HANDLE_HPP

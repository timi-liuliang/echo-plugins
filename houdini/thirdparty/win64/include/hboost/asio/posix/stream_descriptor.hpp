//
// posix/stream_descriptor.hpp
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_POSIX_STREAM_DESCRIPTOR_HPP
#define HBOOST_ASIO_POSIX_STREAM_DESCRIPTOR_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if defined(HBOOST_ASIO_HAS_POSIX_STREAM_DESCRIPTOR) \
  || defined(GENERATING_DOCUMENTATION)

#include <hboost/asio/posix/basic_stream_descriptor.hpp>

namespace hboost {
namespace asio {
namespace posix {

/// Typedef for the typical usage of a stream-oriented descriptor.
typedef basic_stream_descriptor<> stream_descriptor;

} // namespace posix
} // namespace asio
} // namespace hboost

#endif // defined(HBOOST_ASIO_HAS_POSIX_STREAM_DESCRIPTOR)
       //   || defined(GENERATING_DOCUMENTATION)

#endif // HBOOST_ASIO_POSIX_STREAM_DESCRIPTOR_HPP

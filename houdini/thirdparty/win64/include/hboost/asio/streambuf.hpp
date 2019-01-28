//
// streambuf.hpp
// ~~~~~~~~~~~~~
//
// Copyright (c) 2003-2015 Christopher M. Kohlhoff (chris at kohlhoff dot com)
//
// Distributed under the Boost Software License, Version 1.0. (See accompanying
// file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef HBOOST_ASIO_STREAMBUF_HPP
#define HBOOST_ASIO_STREAMBUF_HPP

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
# pragma once
#endif // defined(_MSC_VER) && (_MSC_VER >= 1200)

#include <hboost/asio/detail/config.hpp>

#if !defined(HBOOST_ASIO_NO_IOSTREAM)

#include <hboost/asio/basic_streambuf.hpp>

namespace hboost {
namespace asio {

/// Typedef for the typical usage of basic_streambuf.
typedef basic_streambuf<> streambuf;

} // namespace asio
} // namespace hboost

#endif // !defined(HBOOST_ASIO_NO_IOSTREAM)

#endif // HBOOST_ASIO_STREAMBUF_HPP
